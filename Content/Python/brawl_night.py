"""FULL THROTTLE brawl: the dive-bar night pass (run 99056). A dark room lit by its neon and its bar lamps,
real windows on the booths panels with the harbour outside moving in parallax behind them.

Sources: beat-em-up-unreal/ArtSource/night/*.png (made by ArtSource/tools/night.py from Codex art in night/raw/).
Run in the editor: import brawl_night; brawl_night.build()
or headless: UnrealEditor-Cmd FullThrottleBrawl.uproject -run=pythonscript -script="<abs>/Content/Python/brawl_night.py"

Makes (AFTBrawlGameMode::BuildVenue finds them by path; without them it keeps the old bright look):
  /Game/Brawl/Night/T_bg_booths_win      booths panel, window panes cut out (alpha 0)
  /Game/Brawl/Night/T_glow_bar|booths|games   emissive masks for the painted neon / lamps
  /Game/Brawl/Night/T_out_sky|far|near|moon   the outside, T_lamp the hanging bar lamp
  /Game/Brawl/Materials/M_BrawlWall2D    masked, lit wall panel: Tex, GlowTex, Glow, Base, Tint
  /Game/Brawl/Materials/M_BrawlOutside   masked, unlit layer: Tex, TileU, TileV, PanU, Bright
"""
import os
import unreal

AL = unreal.EditorAssetLibrary
MEL = unreal.MaterialEditingLibrary
AT = unreal.AssetToolsHelpers.get_asset_tools()
HERE = os.path.dirname(os.path.abspath(__file__))
SRC = os.path.normpath(os.path.join(HERE, '..', '..', 'ArtSource', 'night'))
MATS = '/Game/Brawl/Materials'
DEST = '/Game/Brawl/Night'


def _dir(p):
    if not AL.does_directory_exist(p):
        AL.make_directory(p)


def textures():
    _dir(DEST)
    tasks = []
    for f in sorted(os.listdir(SRC)):
        if not f.endswith('.png'):
            continue
        t = unreal.AssetImportTask()
        t.set_editor_property('filename', os.path.join(SRC, f))
        t.set_editor_property('destination_path', DEST)
        t.set_editor_property('destination_name', 'T_' + os.path.splitext(f)[0])
        t.set_editor_property('automated', True)
        t.set_editor_property('replace_existing', True)
        t.set_editor_property('save', False)
        tasks.append(t)
    AT.import_asset_tasks(tasks)
    got = [p for t in tasks for p in t.get_editor_property('imported_object_paths')]
    for p in got:
        t = AL.load_asset(p)
        n = p.split('.')[-1]
        t.set_editor_property('lod_group', unreal.TextureGroup.TEXTUREGROUP_WORLD)
        t.set_editor_property('never_stream', True)
        tile = n in ('T_out_sky', 'T_out_far', 'T_out_near')
        t.set_editor_property('address_x', unreal.TextureAddress.TA_WRAP if tile else unreal.TextureAddress.TA_CLAMP)
        t.set_editor_property('address_y', unreal.TextureAddress.TA_CLAMP)
        if n.startswith('T_glow_'):
            t.set_editor_property('srgb', False)
            t.set_editor_property('compression_settings', unreal.TextureCompressionSettings.TC_GRAYSCALE)
        AL.save_loaded_asset(t)
    return got


def _mat(name):
    _dir(MATS)
    full = MATS + '/' + name
    if AL.does_asset_exist(full):
        m = AL.load_asset(full)
        MEL.delete_all_material_expressions(m)
    else:
        m = AT.create_asset(name, MATS, unreal.Material, unreal.MaterialFactoryNew())
    return m


def _e(m, cls, x, y, **props):
    e = MEL.create_material_expression(m, cls, x, y)
    for k, v in props.items():
        e.set_editor_property(k, v)
    return e


def _done(m):
    MEL.layout_material_expressions(m)
    MEL.recompile_material(m)
    AL.save_loaded_asset(m)


def m_wall():
    """A painted back-wall panel in a dark bar: lit by the room's lights; only the masked neon / lamps / bottles glow
    (GlowTex * Glow), the rest keeps a faint Base so the wall never goes pure black. Masked by Tex alpha (windows)."""
    m = _mat('M_BrawlWall2D')
    m.set_editor_property('blend_mode', unreal.BlendMode.BLEND_MASKED)
    m.set_editor_property('opacity_mask_clip_value', 0.5)
    tx = _e(m, unreal.MaterialExpressionTextureSampleParameter2D, -900, 0, parameter_name='Tex',
            texture=AL.load_asset(DEST + '/T_bg_booths_win'))
    gt = _e(m, unreal.MaterialExpressionTextureSampleParameter2D, -900, 300, parameter_name='GlowTex',
            texture=AL.load_asset(DEST + '/T_glow_booths'), sampler_type=unreal.MaterialSamplerType.SAMPLERTYPE_LINEAR_GRAYSCALE)
    tint = _e(m, unreal.MaterialExpressionVectorParameter, -900, -200, parameter_name='Tint', default_value=unreal.LinearColor(1, 1, 1, 1))
    col = _e(m, unreal.MaterialExpressionMultiply, -600, 0)
    MEL.connect_material_expressions(tx, 'RGB', col, 'A')
    MEL.connect_material_expressions(tint, '', col, 'B')
    MEL.connect_material_property(col, '', unreal.MaterialProperty.MP_BASE_COLOR)
    MEL.connect_material_property(tx, 'A', unreal.MaterialProperty.MP_OPACITY_MASK)
    g = _e(m, unreal.MaterialExpressionScalarParameter, -700, 450, parameter_name='Glow', default_value=1.6)
    base = _e(m, unreal.MaterialExpressionScalarParameter, -700, 550, parameter_name='Base', default_value=0.03)
    gm = _e(m, unreal.MaterialExpressionMultiply, -500, 350)
    MEL.connect_material_expressions(gt, 'R', gm, 'A')
    MEL.connect_material_expressions(g, '', gm, 'B')
    ga = _e(m, unreal.MaterialExpressionAdd, -350, 400)
    MEL.connect_material_expressions(gm, '', ga, 'A')
    MEL.connect_material_expressions(base, '', ga, 'B')
    em = _e(m, unreal.MaterialExpressionMultiply, -200, 150)
    MEL.connect_material_expressions(col, '', em, 'A')
    MEL.connect_material_expressions(ga, '', em, 'B')
    MEL.connect_material_property(em, '', unreal.MaterialProperty.MP_EMISSIVE_COLOR)
    r = _e(m, unreal.MaterialExpressionConstant, -200, 500, r=0.85)
    MEL.connect_material_property(r, '', unreal.MaterialProperty.MP_ROUGHNESS)
    s = _e(m, unreal.MaterialExpressionConstant, -200, 600, r=0.25)
    MEL.connect_material_property(s, '', unreal.MaterialProperty.MP_SPECULAR)
    _done(m)


def m_outside():
    """The world outside the windows (and the hanging lamps): unlit painted layers. TileU repeats along the strip,
    PanU drifts it (tiles per second) so the sky and sea move even when the fight stands still."""
    m = _mat('M_BrawlOutside')
    m.set_editor_property('blend_mode', unreal.BlendMode.BLEND_MASKED)
    m.set_editor_property('shading_model', unreal.MaterialShadingModel.MSM_UNLIT)
    m.set_editor_property('opacity_mask_clip_value', 0.4)
    uv = _e(m, unreal.MaterialExpressionTextureCoordinate, -1400, 0)
    tu = _e(m, unreal.MaterialExpressionScalarParameter, -1400, 150, parameter_name='TileU', default_value=1.0)
    tv = _e(m, unreal.MaterialExpressionScalarParameter, -1400, 250, parameter_name='TileV', default_value=1.0)
    ap = _e(m, unreal.MaterialExpressionAppendVector, -1200, 200)
    MEL.connect_material_expressions(tu, '', ap, 'A')
    MEL.connect_material_expressions(tv, '', ap, 'B')
    mul = _e(m, unreal.MaterialExpressionMultiply, -1050, 0)
    MEL.connect_material_expressions(uv, '', mul, 'A')
    MEL.connect_material_expressions(ap, '', mul, 'B')
    tm = _e(m, unreal.MaterialExpressionTime, -1400, 400)
    pu = _e(m, unreal.MaterialExpressionScalarParameter, -1400, 500, parameter_name='PanU', default_value=0.0)
    pm = _e(m, unreal.MaterialExpressionMultiply, -1200, 450)
    MEL.connect_material_expressions(tm, '', pm, 'A')
    MEL.connect_material_expressions(pu, '', pm, 'B')
    zero = _e(m, unreal.MaterialExpressionConstant, -1200, 550, r=0.0)
    pv = _e(m, unreal.MaterialExpressionAppendVector, -1050, 450)
    MEL.connect_material_expressions(pm, '', pv, 'A')
    MEL.connect_material_expressions(zero, '', pv, 'B')
    add = _e(m, unreal.MaterialExpressionAdd, -900, 100)
    MEL.connect_material_expressions(mul, '', add, 'A')
    MEL.connect_material_expressions(pv, '', add, 'B')
    tx = _e(m, unreal.MaterialExpressionTextureSampleParameter2D, -700, 0, parameter_name='Tex',
            texture=AL.load_asset(DEST + '/T_out_far'))
    MEL.connect_material_expressions(add, '', tx, 'UVs')
    br = _e(m, unreal.MaterialExpressionScalarParameter, -700, 300, parameter_name='Bright', default_value=1.0)
    em = _e(m, unreal.MaterialExpressionMultiply, -400, 100)
    MEL.connect_material_expressions(tx, 'RGB', em, 'A')
    MEL.connect_material_expressions(br, '', em, 'B')
    MEL.connect_material_property(em, '', unreal.MaterialProperty.MP_EMISSIVE_COLOR)
    MEL.connect_material_property(tx, 'A', unreal.MaterialProperty.MP_OPACITY_MASK)
    _done(m)


def build():
    out = textures()
    m_wall()
    m_outside()
    AL.save_directory(DEST)
    unreal.log('BRAWLNIGHT built %d textures + M_BrawlWall2D, M_BrawlOutside' % len(out))
    return out


if __name__ == '__main__':
    build()
