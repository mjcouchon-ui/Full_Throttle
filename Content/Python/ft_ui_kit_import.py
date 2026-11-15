# -*- coding: utf-8 -*-
"""
FULL THROTTLE - import the UI kit into the project (Unreal editor Python; in the repo: Content/Python/ft_ui_kit_import.py).

    import ft_ui_kit_import; ft_ui_kit_import.run()            # textures and the font
    ft_ui_kit_import.import_textures(); ft_ui_kit_import.import_bungee()

What it does:
  1. SourceArt/UI/Kit/T_UI_*.png (written by Tools/ui/ft_ui_kit.py)  ->  /Game/UI/Kit/<same name>
     with UI settings: texture group UI, compression UserInterface2D (RGBA8), sRGB on, no mip maps, never streamed,
     no virtual texturing, filter and address mode per texture as the kit's manifest says (kit_manifest.json next to
     the PNGs; the table below is used when it is missing).
  2. SourceArt/BayshoreSky/fonts/Bungee/Bungee-Regular.ttf  ->  /Game/UI/Fonts/Bungee-Regular (a FontFace) and
     /Game/UI/Fonts/F_Bungee (a runtime Font whose default typeface is that face), made the way the project's
     F_Rajdhani / Rajdhani-Bold pair is: the settings are READ from that pair and copied, so the three fonts match.
     Bungee's licence goes to Content/Data/Licenses/OFL-Bungee.txt beside the other three.

It logs one line per asset (prefix JAM FTUIKIT) and returns a report. Importing again replaces the assets in place.
As a queue job: SourceArt/Jam/unreal/jam_run.ps1 -Script <a three line job: import ft_ui_kit_import, run(), quit>.

THIS SCRIPT HAS NOT BEEN RUN: it was written in a session that was not allowed to start Unreal. The texture half
follows the project's proven Interchange route (ft_billboards.import_textures, bsky_ue.import_texture). The font
half has no precedent in the repo (F_Orbitron and F_Rajdhani were made by hand in the editor); it tries the
struct-text route first, a field-by-field route second and the engine's own font file factory third, and says in
the log which one worked.
"""
import os
import json
import shutil

import unreal

AL = unreal.EditorAssetLibrary
AT = unreal.AssetToolsHelpers.get_asset_tools()

PROJECT = unreal.Paths.convert_relative_path_to_full(unreal.Paths.project_dir())
SRC = os.path.join(PROJECT, 'SourceArt', 'UI', 'Kit')
DEST = '/Game/UI/Kit'
FONT_DIR = '/Game/UI/Fonts'
BUNGEE_TTF = os.path.join(PROJECT, 'SourceArt', 'BayshoreSky', 'fonts', 'Bungee', 'Bungee-Regular.ttf')
BUNGEE_OFL = os.path.join(PROJECT, 'SourceArt', 'BayshoreSky', 'fonts', 'Bungee', 'OFL.txt')
LICENCES = os.path.join(PROJECT, 'Content', 'Data', 'Licenses')
FACE_NAME = 'Bungee-Regular'            # as Rajdhani-Bold and Orbitron-900: the face asset is named after its file
FONT_NAME = 'F_Bungee'
REF_FONT = FONT_DIR + '/F_Rajdhani'     # the pair whose settings are copied
REF_FACE = FONT_DIR + '/Rajdhani-Bold'
TAG = 'JAM FTUIKIT '                    # "JAM " first: jam_run.ps1 prints the lines that carry it

# Address mode per texture when kit_manifest.json is not there: these tile, everything else clamps. All are linear.
WRAP_FALLBACK = {
    'T_UI_Dots': 'xy', 'T_UI_Paper': 'xy', 'T_UI_SlateDust': 'xy', 'T_UI_ClapStripes': 'xy', 'T_UI_HazardStripes': 'xy',
    'T_UI_Hatch': 'xy', 'T_UI_FilmPerf': 'xy', 'T_UI_Chase_Skyline': 'x',
}


def _log(s):
    unreal.log(TAG + s)


def _warn(s):
    unreal.log_warning(TAG + s)


def _enum(cls, label):
    """an enum entry by its C++ name, whatever the Python spelling is (TC_EditorIcon / TC_EDITOR_ICON)"""
    key = label.replace('_', '').lower()
    for name in dir(cls):
        if name.replace('_', '').lower() == key:
            return getattr(cls, name)
    raise RuntimeError('missing Unreal enum %s.%s' % (cls.__name__, label))


def _dir(p):
    if not AL.does_directory_exist(p):
        AL.make_directory(p)


def _manifest():
    p = os.path.join(SRC, 'kit_manifest.json')
    if os.path.isfile(p):
        with open(p, 'r') as f:
            return json.load(f).get('textures', {})
    _warn('kit_manifest.json is missing in %s: using the built-in table (everything linear, %d textures wrap)' % (SRC, len(WRAP_FALLBACK)))
    return None


# ------------------------------------------------------------------------------------------ textures
def import_textures(only=None):
    """Import every T_UI_*.png of SourceArt/UI/Kit to /Game/UI/Kit with UI settings. only: a list of name fragments.
    -> [(name, width, height, wrap, filter)]"""
    if not os.path.isdir(SRC):
        raise RuntimeError('missing folder %s (run: python Tools/ui/ft_ui_kit.py textures SourceArt/UI/Kit)' % SRC)
    man = _manifest()
    _dir(DEST)
    files = sorted(f for f in os.listdir(SRC) if f.lower().endswith('.png') and f.startswith('T_UI_'))
    if only:
        files = [f for f in files if any(o in f for o in only)]
    if not files:
        raise RuntimeError('no T_UI_*.png in ' + SRC)
    out, total = [], 0
    for fn in files:
        name = os.path.splitext(fn)[0]
        src = os.path.join(SRC, fn)
        path = '%s/%s' % (DEST, name)
        entry = (man or {}).get(name)
        if man is not None and entry is None:
            _warn('%s is not in kit_manifest.json: imported as a clamped, linear texture' % name)
        wrap = (entry or {}).get('wrap', WRAP_FALLBACK.get(name, '')) if entry is not None else WRAP_FALLBACK.get(name, '')
        filt = (entry or {}).get('filter', 'linear')
        # the project's proven Interchange route (ft_billboards.import_textures)
        pipe = unreal.InterchangeGenericTexturePipeline()
        pipe.set_editor_property('asset_name', name)
        pipe.set_editor_property('detect_normal_map_texture', False)
        pipe.set_editor_property('detect_long_lat_cubemap', False)
        pipe.set_editor_property('file_extensions_to_import_as_long_lat_cubemap', set())
        stack = unreal.InterchangePipelineStackOverride()
        stack.add_pipeline(pipe)
        task = unreal.AssetImportTask()
        task.set_editor_property('filename', src)
        task.set_editor_property('destination_path', DEST)
        task.set_editor_property('destination_name', name)
        task.set_editor_property('automated', True)
        task.set_editor_property('replace_existing', True)
        task.set_editor_property('save', False)
        task.set_editor_property('options', stack)
        AT.import_asset_tasks([task])
        tex = unreal.load_asset(path)
        if not isinstance(tex, unreal.Texture2D):
            raise RuntimeError('%s did not import as a Texture2D (%s)' % (path, type(tex).__name__))
        tex.set_editor_property('lod_group', _enum(unreal.TextureGroup, 'TEXTUREGROUP_UI'))
        tex.set_editor_property('compression_settings', _enum(unreal.TextureCompressionSettings, 'TC_EditorIcon'))   # "UserInterface2D (RGBA)"
        tex.set_editor_property('srgb', True)
        tex.set_editor_property('mip_gen_settings', _enum(unreal.TextureMipGenSettings, 'TMGS_NoMipmaps'))
        tex.set_editor_property('never_stream', True)
        tex.set_editor_property('virtual_texture_streaming', False)
        tex.set_editor_property('filter', _enum(unreal.TextureFilter, 'TF_Nearest' if filt == 'nearest' else 'TF_Bilinear'))
        tex.set_editor_property('address_x', _enum(unreal.TextureAddress, 'TA_Wrap' if 'x' in wrap else 'TA_Clamp'))
        tex.set_editor_property('address_y', _enum(unreal.TextureAddress, 'TA_Wrap' if 'y' in wrap else 'TA_Clamp'))
        tex.set_editor_property('lod_bias', 0)
        tex.set_editor_property('max_texture_size', 0)
        AL.save_loaded_asset(tex, only_if_is_dirty=False)
        w, h = tex.blueprint_get_size_x(), tex.blueprint_get_size_y()
        total += w * h * 4
        note = ''
        if entry is not None and [w, h] != list(entry.get('size', [w, h])):
            note = '  SIZE DIFFERS FROM THE MANIFEST %s' % (entry.get('size'),)
        _log('%s %dx%d %s %s -> %s%s' % (name, w, h, ('wrap ' + wrap) if wrap else 'clamp', filt, path, note))
        if note:
            _warn(name + note)
        out.append((name, w, h, wrap, filt))
    # anything in the folder that the kit no longer makes is reported, never deleted
    have = set(n for n, _, _, _, _ in out)
    if not only:
        for a in AL.list_assets(DEST, recursive=False, include_folder=False):
            stem = a.split('/')[-1].split('.')[0]
            if stem.startswith('T_UI_') and stem not in have:
                _warn('%s is in %s but not in SourceArt/UI/Kit (left alone)' % (stem, DEST))
    _log('textures: %d imported, %.2f MB as RGBA8' % (len(out), total / 1048576.0))
    return out


# ------------------------------------------------------------------------------------------ the font
def _prop(obj, name, default=None):
    try:
        return obj.get_editor_property(name)
    except Exception as e:                                   # a property this engine version does not show to Python
        _warn('cannot read %s.%s (%s)' % (type(obj).__name__, name, e))
        return default


def _import_face():
    """the .ttf as a FontFace asset, with the reference face's hinting and loading policy"""
    if not os.path.isfile(BUNGEE_TTF):
        raise RuntimeError('missing font file ' + BUNGEE_TTF)
    path = '%s/%s' % (FONT_DIR, FACE_NAME)
    task = unreal.AssetImportTask()
    task.set_editor_property('filename', BUNGEE_TTF)
    task.set_editor_property('destination_path', FONT_DIR)
    task.set_editor_property('destination_name', FACE_NAME)
    task.set_editor_property('automated', True)              # automated: the factory makes the face only, and asks nothing
    task.set_editor_property('replace_existing', True)
    task.set_editor_property('save', False)
    task.set_editor_property('factory', unreal.FontFileImportFactory())
    AT.import_asset_tasks([task])
    face = unreal.load_asset(path)
    if not isinstance(face, unreal.FontFace):
        raise RuntimeError('%s did not import as a FontFace (%s)' % (path, type(face).__name__))
    ref = unreal.load_asset(REF_FACE)
    if isinstance(ref, unreal.FontFace):
        for prop in ('hinting', 'loading_policy', 'layout_method'):
            v = _prop(ref, prop)
            if v is not None:
                try:
                    face.set_editor_property(prop, v)
                except Exception as e:
                    _warn('cannot set %s.%s (%s)' % (FACE_NAME, prop, e))
    else:
        _warn('%s is missing: %s keeps the importer\'s defaults (the project\'s faces load Inline)' % (REF_FACE, FACE_NAME))
        try:
            face.set_editor_property('loading_policy', _enum(unreal.FontLoadingPolicy, 'Inline'))
        except Exception as e:
            _warn('cannot set %s.loading_policy (%s)' % (FACE_NAME, e))
    AL.save_loaded_asset(face, only_if_is_dirty=False)
    _log('%s  FontFace  hinting=%s loading=%s  <- %s' % (path, _prop(face, 'hinting'), _prop(face, 'loading_policy'), BUNGEE_TTF))
    return face


def _typeface_by_text(font, ref, face):
    """copy the reference font's composite font as text with the face swapped: every setting comes along"""
    comp = ref.get_editor_property('composite_font')
    txt = comp.export_text()
    ref_face = unreal.load_asset(REF_FACE)
    old = ref_face.get_path_name()                           # /Game/UI/Fonts/Rajdhani-Bold.Rajdhani-Bold
    new = face.get_path_name()
    if old not in txt:
        raise RuntimeError('the reference composite font does not name %s: %s' % (old, txt[:200]))
    mine = unreal.CompositeFont()
    mine.import_text(txt.replace(old, new))
    font.set_editor_property('composite_font', mine)


def _typeface_by_fields(font, face):
    """build the default typeface field by field: one entry, 'Regular', pointing at the face"""
    data = unreal.FontData()
    data.set_editor_property('font_face_asset', face)
    entry = unreal.TypefaceEntry()
    entry.set_editor_property('name', 'Regular')
    entry.set_editor_property('font', data)
    typeface = unreal.Typeface()
    typeface.set_editor_property('fonts', [entry])
    comp = unreal.CompositeFont()
    comp.set_editor_property('default_typeface', typeface)
    font.set_editor_property('composite_font', comp)


def _font_names_face(font, face):
    try:
        return face.get_path_name() in font.get_editor_property('composite_font').export_text()
    except Exception:
        return False


def _font_by_engine_factory(face):
    """the engine's own route (UFontFileImportFactory): a font file imported NOT automated is asked "create a Font
    asset too?", and an unattended editor answers that question with its default, Yes. The factory then makes
    <face>_Font beside the face, a runtime font whose default typeface is the face. It is copied to F_Bungee and
    removed. FFontData's fields are not open to Python, so this is the fallback that needs none of them.
    -> the Font at FONT_DIR/FONT_NAME, or None"""
    made = '%s/%s_Font' % (FONT_DIR, FACE_NAME)
    path = '%s/%s' % (FONT_DIR, FONT_NAME)
    task = unreal.AssetImportTask()
    task.set_editor_property('filename', BUNGEE_TTF)
    task.set_editor_property('destination_path', FONT_DIR)
    task.set_editor_property('destination_name', FACE_NAME)
    task.set_editor_property('automated', False)
    task.set_editor_property('replace_existing', True)
    task.set_editor_property('save', False)
    task.set_editor_property('factory', unreal.FontFileImportFactory())
    AT.import_asset_tasks([task])
    if not AL.does_asset_exist(made):
        _warn('the engine factory made no %s (is the editor unattended?)' % made)
        return None
    if AL.does_asset_exist(path):
        AL.delete_asset(path)
    font = AL.duplicate_asset(made, path)
    AL.delete_asset(made)
    if not isinstance(font, unreal.Font):
        return None
    # the face was imported again in place: give it the reference face's settings again
    ref = unreal.load_asset(REF_FACE)
    new_face = unreal.load_asset('%s/%s' % (FONT_DIR, FACE_NAME))
    if isinstance(ref, unreal.FontFace) and isinstance(new_face, unreal.FontFace):
        for prop in ('hinting', 'loading_policy', 'layout_method'):
            v = _prop(ref, prop)
            if v is not None:
                try:
                    new_face.set_editor_property(prop, v)
                except Exception as e:
                    _warn('cannot set %s.%s (%s)' % (FACE_NAME, prop, e))
        AL.save_loaded_asset(new_face, only_if_is_dirty=False)
    return font


def import_bungee():
    """Bungee-Regular.ttf -> /Game/UI/Fonts/Bungee-Regular and /Game/UI/Fonts/F_Bungee. -> (font, how it was made)"""
    _dir(FONT_DIR)
    face = _import_face()
    path = '%s/%s' % (FONT_DIR, FONT_NAME)
    ref = unreal.load_asset(REF_FONT)
    if not isinstance(ref, unreal.Font):
        ref = None
        _warn('%s is missing: %s is made with the engine\'s defaults for a runtime font' % (REF_FONT, FONT_NAME))
    font = unreal.load_asset(path) if AL.does_asset_exist(path) else None
    if font is None:
        font = AT.create_asset(FONT_NAME, FONT_DIR, unreal.Font, unreal.FontFactory())
    if not isinstance(font, unreal.Font):
        raise RuntimeError('%s is not a Font (%s)' % (path, type(font).__name__))
    font.modify()
    font.set_editor_property('font_cache_type', _prop(ref, 'font_cache_type') if ref else _enum(unreal.FontCacheType, 'Runtime'))
    if ref:
        for prop in ('legacy_font_size', 'legacy_font_name'):
            v = _prop(ref, prop)
            if v is not None:
                try:
                    font.set_editor_property(prop, v)
                except Exception as e:
                    _warn('cannot set %s.%s (%s)' % (FONT_NAME, prop, e))
    how = None
    if ref:
        try:
            _typeface_by_text(font, ref, face)
            how = 'composite font copied from F_Rajdhani as text, face swapped'
        except Exception as e:
            _warn('the struct-text route failed (%s); building the typeface field by field' % e)
    if how is None or not _font_names_face(font, face):
        try:
            _typeface_by_fields(font, face)
            how = 'default typeface built field by field (one entry, Regular)'
        except Exception as e:
            _warn('the field-by-field route failed (%s); using the engine\'s own font factory' % e)
    if not _font_names_face(font, face):
        try:
            made = _font_by_engine_factory(face)
        except Exception as e:
            made = None
            _warn('the engine factory route failed (%s)' % e)
        if made is not None:
            font = made
            face = unreal.load_asset('%s/%s' % (FONT_DIR, FACE_NAME))
            how = 'made by the engine\'s font file factory (default typeface = the face), copied to %s' % FONT_NAME
            if ref:
                for prop in ('legacy_font_size', 'legacy_font_name'):
                    v = _prop(ref, prop)
                    if v is not None:
                        try:
                            font.set_editor_property(prop, v)
                        except Exception as e:
                            _warn('cannot set %s.%s (%s)' % (FONT_NAME, prop, e))
    ok = _font_names_face(font, face)
    AL.save_loaded_asset(font, only_if_is_dirty=False)
    _log('%s  Font  cache=%s legacy_size=%s  face=%s  %s  %s' % (path, _prop(font, 'font_cache_type'), _prop(font, 'legacy_font_size'),
                                                        face.get_path_name(), how, 'OK' if ok else 'COULD NOT VERIFY THE FACE: open the asset and check'))
    if not ok:
        _warn('%s: the default typeface could not be read back; open it in the Font editor and check that its one face is %s' % (path, FACE_NAME))
    # the licence travels with the game, beside the other three
    dst = os.path.join(LICENCES, 'OFL-Bungee.txt')
    if os.path.isfile(BUNGEE_OFL) and os.path.isdir(LICENCES) and not os.path.isfile(dst):
        shutil.copyfile(BUNGEE_OFL, dst)
        _log('licence copied to %s' % dst)
    elif not os.path.isfile(dst):
        _warn('Bungee\'s licence was not copied (looked for %s)' % BUNGEE_OFL)
    return font, how


def run(only=None):
    """textures, then the font. -> a report dict"""
    tex = import_textures(only)
    font, how = import_bungee()
    rep = {'textures': len(tex), 'bytes': sum(w * h * 4 for _, w, h, _, _ in tex), 'font': font.get_path_name(), 'font_how': how}
    _log('done: %d textures (%.2f MB as RGBA8), %s' % (rep['textures'], rep['bytes'] / 1048576.0, rep['font']))
    return rep


if __name__ == '__main__':
    run()
