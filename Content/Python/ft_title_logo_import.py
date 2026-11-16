"""The animated title logo's textures: Tools/title/T_TitleLogo_<Name>.png -> /Game/UI/Title/T_TitleLogo_<Name>.

An Unreal Python job. Run headless:
    UnrealEditor-Cmd.exe FullThrottle.uproject -run=pythonscript -script=<project>/Content/Python/ft_title_logo_import.py -unattended -nosplash -nullrhi
or in the editor:
    import ft_title_logo_import; ft_title_logo_import.build()      # import and save all eleven
    ft_title_logo_import.report()                                  # only read the saved assets back
The PNGs are made by Tools/title/render_layers.ps1 (logo.html?layer=..., then make_layers.py); layers.json beside them
lists each texture's size and the logo-px rectangle it covers. FTTitleLogo.cpp draws them.

Settings, the same as the flat T_TitleLogo except that both addresses clamp: UI group, UserInterface2D (TC_EditorIcon,
uncompressed BGRA8), sRGB, no mip maps, never streamed, no virtual texture, default filter. Follows the project's
Interchange route for UI textures (ft_ui_kit_import.import_textures). The flat T_TitleLogo is never touched.

Log: one line per texture, read back from the saved asset,
    JAM TITLELOGO <name> size=<w>x<h> srgb=.. comp=.. mips=.. addr=..
then JAM TITLELOGO done n=11, or JAM TITLELOGO FAILED <traceback>.
"""
import os
import traceback

import unreal

DEST = '/Game/UI/Title'
# name, width, height: the table of Tools/title/make_layers.py
TEXTURES = [
    ('T_TitleLogo_Throttle', 1600, 320),
    ('T_TitleLogo_Lines', 1600, 128),
    ('T_TitleLogo_Full', 768, 416),
    ('T_TitleLogo_Star1', 128, 128),
    ('T_TitleLogo_Star2', 128, 128),
    ('T_TitleLogo_Star1Lit', 128, 128),
    ('T_TitleLogo_Star2Lit', 128, 128),
    ('T_TitleLogo_FullSoft', 240, 160),
    ('T_TitleLogo_FullGlow', 240, 160),
    ('T_TitleLogo_FullGlowIn', 228, 132),
    ('T_TitleLogo_ThrottleLit', 800, 176),
]


def say(text):
    unreal.log('JAM TITLELOGO ' + text)


def _enum(cls, label):
    key = label.replace('_', '').lower()
    for name in dir(cls):
        if name.replace('_', '').lower() == key:
            return getattr(cls, name)
    raise RuntimeError('missing Unreal enum %s.%s' % (cls.__name__, label))


def _src_dir():
    return os.path.join(unreal.Paths.convert_relative_path_to_full(unreal.Paths.project_dir()), 'Tools', 'title')


def _short(v):
    """TextureAddress.TA_CLAMP -> TA_CLAMP"""
    s = str(v)
    return s.split('.')[-1].split(':')[0].strip('<> ')


def line(name):
    """The settings of a saved texture, as one log line (also returned)."""
    tex = unreal.load_asset('%s/%s' % (DEST, name))
    if not isinstance(tex, unreal.Texture2D):
        return '%s MISSING (%s)' % (name, type(tex).__name__)
    g = tex.get_editor_property
    src = '-'
    try:
        aid = g('asset_import_data')
        src = ';'.join(os.path.basename(f) for f in aid.extract_filenames()) if aid else '-'
    except Exception:
        pass
    return '%s size=%dx%d srgb=%s comp=%s mips=%s addr=%s/%s group=%s filter=%s never_stream=%s vt=%s lod_bias=%d max=%d src=%s' % (
        name, tex.blueprint_get_size_x(), tex.blueprint_get_size_y(), g('srgb'), _short(g('compression_settings')),
        _short(g('mip_gen_settings')), _short(g('address_x')), _short(g('address_y')), _short(g('lod_group')),
        _short(g('filter')), g('never_stream'), g('virtual_texture_streaming'), g('lod_bias'), g('max_texture_size'), src)


def report(also_flat=True):
    """Read every saved texture back and log it; nothing is changed. -> the lines"""
    names = [n for n, _, _ in TEXTURES]
    if also_flat:
        names.append('T_TitleLogo')       # the flat logo, for comparison only
    lines = [line(n) for n in names]
    for ln in lines:
        say(ln)
    return lines


def build():
    """Import the eleven PNGs (replacing what is there), apply the UI settings, save. -> the report lines"""
    al = unreal.EditorAssetLibrary
    tools = unreal.AssetToolsHelpers.get_asset_tools()
    src_dir = _src_dir()
    missing = [n for n, _, _ in TEXTURES if not os.path.isfile(os.path.join(src_dir, n + '.png'))]
    if missing:
        raise RuntimeError('missing in %s: %s (run Tools/title/render_layers.ps1)' % (src_dir, ', '.join(missing)))
    if not al.does_directory_exist(DEST):
        al.make_directory(DEST)
    failed = []
    for name, w, h in TEXTURES:
        path = '%s/%s' % (DEST, name)
        pipe = unreal.InterchangeGenericTexturePipeline()
        pipe.set_editor_property('asset_name', name)
        pipe.set_editor_property('detect_normal_map_texture', False)
        pipe.set_editor_property('detect_long_lat_cubemap', False)
        pipe.set_editor_property('file_extensions_to_import_as_long_lat_cubemap', set())
        pipe.set_editor_property('allow_non_power_of_two', True)      # 228 x 132, 240 x 160 ...: UI textures, no mips
        stack = unreal.InterchangePipelineStackOverride()
        stack.add_pipeline(pipe)
        task = unreal.AssetImportTask()
        task.set_editor_property('filename', os.path.join(src_dir, name + '.png'))
        task.set_editor_property('destination_path', DEST)
        task.set_editor_property('destination_name', name)
        task.set_editor_property('automated', True)
        task.set_editor_property('replace_existing', True)
        task.set_editor_property('save', False)
        task.set_editor_property('options', stack)
        tools.import_asset_tasks([task])
        tex = unreal.load_asset(path) if al.does_asset_exist(path) else None
        if not isinstance(tex, unreal.Texture2D):
            failed.append('%s did not import as a Texture2D (%s)' % (name, type(tex).__name__))
            continue
        tex.set_editor_property('lod_group', _enum(unreal.TextureGroup, 'TEXTUREGROUP_UI'))
        tex.set_editor_property('compression_settings', _enum(unreal.TextureCompressionSettings, 'TC_EditorIcon'))
        tex.set_editor_property('srgb', True)
        tex.set_editor_property('mip_gen_settings', _enum(unreal.TextureMipGenSettings, 'TMGS_NoMipmaps'))
        tex.set_editor_property('never_stream', True)
        tex.set_editor_property('virtual_texture_streaming', False)
        tex.set_editor_property('filter', _enum(unreal.TextureFilter, 'TF_Default'))
        tex.set_editor_property('address_x', _enum(unreal.TextureAddress, 'TA_Clamp'))
        tex.set_editor_property('address_y', _enum(unreal.TextureAddress, 'TA_Clamp'))
        tex.set_editor_property('lod_bias', 0)
        tex.set_editor_property('max_texture_size', 0)
        if not al.save_loaded_asset(tex, only_if_is_dirty=False):
            failed.append('%s was not saved' % name)
            continue
        if (tex.blueprint_get_size_x(), tex.blueprint_get_size_y()) != (w, h):
            failed.append('%s is %dx%d, expected %dx%d' % (name, tex.blueprint_get_size_x(), tex.blueprint_get_size_y(), w, h))
    lines = report(also_flat=False)
    if failed:
        raise RuntimeError('; '.join(failed))
    return lines


def main():
    try:
        lines = build()
        say('done n=%d' % len(lines))
    except Exception:
        say('FAILED ' + traceback.format_exc())


if __name__ == '__main__':
    main()
