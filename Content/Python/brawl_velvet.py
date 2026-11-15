"""FULL THROTTLE brawl, night 2: THE VELVET ROOM (run 16961). Imports the club and its new cast, builds its materials
and its map. Headless (the module must be built first; run it through wispos --backstage):
  UnrealEditor-Cmd FullThrottleBrawl.uproject -run=pythonscript -script="<abs>/Content/Python/brawl_velvet.py" -AllowCommandletRendering
or in the editor: import brawl_velvet; brawl_velvet.build()

Sources: ArtSource/velvet/*.png (tools/velvet.py, from the Codex originals in velvet/raw/) and ArtSource/frames/
(vinnie_*, guard_*, the disco pickups item_cocktail|mic|bucket|champ, props prop_vstool|vtable|palm|rope).
Makes:
  /Game/Brawl/Velvet/T_vbg_bar|dj|vip, T_vglow_*, T_vfloor, T_vfloor_glow, T_vball, T_ring
  /Game/Brawl/Sprites/Vinnie|Guard/T_*, the new Items/Props textures
  /Game/Brawl/Materials/M_BrawlDisco   lit light-up dance floor: Tex, MaskTex, TileU, TileV, Glow, Base, Tint
  /Game/Brawl/Materials/M_BrawlGlow    unlit additive decal: Tex (grayscale), Color, Glow
  /Game/Brawl/Maps/L_VelvetRoom        empty, like L_RustyAnchor (the game mode builds the club at BeginPlay)
"""
import os
import sys
import unreal
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import brawl_2d

AL = unreal.EditorAssetLibrary
MEL = unreal.MaterialEditingLibrary
HERE = os.path.dirname(os.path.abspath(__file__))
SRC = os.path.normpath(os.path.join(HERE, '..', '..', 'ArtSource'))
DEST = '/Game/Brawl/Velvet'
FRAMES = ['vinnie_' + n for n in ('idle', 'walk1', 'walk2', 'wind', 'punch', 'kick', 'hurt', 'down', 'taunt', 'spinb', 'spin', 'dizzy')] + \
         ['guard_' + n for n in ('idle', 'walk1', 'walk2', 'wind', 'punch', 'bash', 'hurt', 'down')] + \
         ['item_cocktail', 'item_mic', 'item_bucket', 'item_champ', 'prop_vstool', 'prop_vtable', 'prop_palm', 'prop_rope']
MASKS = ('T_vglow_bar', 'T_vglow_dj', 'T_vglow_vip', 'T_vfloor_glow', 'T_ring')


def textures():
    d = os.path.join(SRC, 'velvet')
    got = brawl_2d._import(sorted(os.path.join(d, f) for f in os.listdir(d) if f.endswith('.png')), DEST)
    for p in got:
        t = AL.load_asset(p)
        n = p.split('.')[-1]
        t.set_editor_property('lod_group', unreal.TextureGroup.TEXTUREGROUP_WORLD)
        t.set_editor_property('never_stream', True)
        wrap = n in ('T_vfloor', 'T_vfloor_glow')
        t.set_editor_property('address_x', unreal.TextureAddress.TA_WRAP if wrap else unreal.TextureAddress.TA_CLAMP)
        t.set_editor_property('address_y', unreal.TextureAddress.TA_WRAP if wrap else unreal.TextureAddress.TA_CLAMP)
        if n in MASKS:
            t.set_editor_property('srgb', False)
            t.set_editor_property('compression_settings', unreal.TextureCompressionSettings.TC_GRAYSCALE)
        AL.save_loaded_asset(t)
    return got + brawl_2d.only(FRAMES)


_e = brawl_2d._e


def _input(name):
    ci = unreal.CustomInput()
    ci.set_editor_property('input_name', name)
    return ci
DISCO_HLSL = """
float2 c = floor(UV);
float b = floor(T * 2.0);
float h = frac(sin(dot(c, float2(12.9898, 78.233))) * 43758.5453);
float h2 = frac(sin(dot(c + float2(b * 7.13, b * 3.71), float2(39.3468, 11.135))) * 24634.6345);
float on = step(0.7, h2);
float k = frac(h + b * 0.37);
float3 col = k < 0.25 ? float3(1.0, 0.31, 0.72) : (k < 0.5 ? float3(0.21, 0.95, 1.0) : (k < 0.75 ? float3(1.0, 0.82, 0.25) : float3(0.6, 0.4, 1.0)));
float fade = 1.0 - 0.55 * frac(T * 2.0);
return col * on * fade;
"""


def m_disco():
    """The light-up dance floor: the painted glass tiles, lit; on every beat (120 bpm) a random ~30% of the tiles glow
    pink / cyan / gold / violet and fade until the next beat. 4 tiles per texture tile; MaskTex keeps the chrome dark."""
    m = brawl_2d._mat('M_BrawlDisco')
    uv = _e(m, unreal.MaterialExpressionTextureCoordinate, -1600, 0)
    tu = _e(m, unreal.MaterialExpressionScalarParameter, -1600, 150, parameter_name='TileU', default_value=1.0)
    tv = _e(m, unreal.MaterialExpressionScalarParameter, -1600, 250, parameter_name='TileV', default_value=1.0)
    ap = _e(m, unreal.MaterialExpressionAppendVector, -1400, 200)
    MEL.connect_material_expressions(tu, '', ap, 'A')
    MEL.connect_material_expressions(tv, '', ap, 'B')
    uvt = _e(m, unreal.MaterialExpressionMultiply, -1250, 0)
    MEL.connect_material_expressions(uv, '', uvt, 'A')
    MEL.connect_material_expressions(ap, '', uvt, 'B')
    tx = _e(m, unreal.MaterialExpressionTextureSampleParameter2D, -1000, -200, parameter_name='Tex',
            texture=AL.load_asset(DEST + '/T_vfloor'))
    MEL.connect_material_expressions(uvt, '', tx, 'UVs')
    mk = _e(m, unreal.MaterialExpressionTextureSampleParameter2D, -1000, 150, parameter_name='MaskTex',
            texture=AL.load_asset(DEST + '/T_vfloor_glow'), sampler_type=unreal.MaterialSamplerType.SAMPLERTYPE_LINEAR_GRAYSCALE)
    MEL.connect_material_expressions(uvt, '', mk, 'UVs')
    tint = _e(m, unreal.MaterialExpressionVectorParameter, -1000, -400, parameter_name='Tint', default_value=unreal.LinearColor(1, 1, 1, 1))
    bc = _e(m, unreal.MaterialExpressionMultiply, -700, -300)
    MEL.connect_material_expressions(tx, 'RGB', bc, 'A')
    MEL.connect_material_expressions(tint, '', bc, 'B')
    MEL.connect_material_property(bc, '', unreal.MaterialProperty.MP_BASE_COLOR)
    four = _e(m, unreal.MaterialExpressionConstant, -1250, 300, r=4.0)
    cells = _e(m, unreal.MaterialExpressionMultiply, -1100, 350)
    MEL.connect_material_expressions(uvt, '', cells, 'A')
    MEL.connect_material_expressions(four, '', cells, 'B')
    tm = _e(m, unreal.MaterialExpressionTime, -1100, 500)
    cu = _e(m, unreal.MaterialExpressionCustom, -850, 400, code=DISCO_HLSL,
            output_type=unreal.CustomMaterialOutputType.CMOT_FLOAT3,
            inputs=[_input('UV'), _input('T')])
    MEL.connect_material_expressions(cells, '', cu, 'UV')
    MEL.connect_material_expressions(tm, '', cu, 'T')
    g = _e(m, unreal.MaterialExpressionScalarParameter, -850, 600, parameter_name='Glow', default_value=0.6)
    a1 = _e(m, unreal.MaterialExpressionMultiply, -600, 400)
    MEL.connect_material_expressions(cu, '', a1, 'A')
    MEL.connect_material_expressions(mk, 'R', a1, 'B')
    a2 = _e(m, unreal.MaterialExpressionMultiply, -450, 450)
    MEL.connect_material_expressions(a1, '', a2, 'A')
    MEL.connect_material_expressions(g, '', a2, 'B')
    base = _e(m, unreal.MaterialExpressionScalarParameter, -700, 0, parameter_name='Base', default_value=0.04)
    b1 = _e(m, unreal.MaterialExpressionMultiply, -500, 0)
    MEL.connect_material_expressions(bc, '', b1, 'A')
    MEL.connect_material_expressions(base, '', b1, 'B')
    em = _e(m, unreal.MaterialExpressionAdd, -300, 200)
    MEL.connect_material_expressions(b1, '', em, 'A')
    MEL.connect_material_expressions(a2, '', em, 'B')
    MEL.connect_material_property(em, '', unreal.MaterialProperty.MP_EMISSIVE_COLOR)
    r = _e(m, unreal.MaterialExpressionConstant, -300, 500, r=0.3)
    MEL.connect_material_property(r, '', unreal.MaterialProperty.MP_ROUGHNESS)
    sp = _e(m, unreal.MaterialExpressionConstant, -300, 600, r=0.6)
    MEL.connect_material_property(sp, '', unreal.MaterialProperty.MP_SPECULAR)
    brawl_2d._done(m)


def m_glow():
    """A soft additive glow on the floor (the pink ring round Vinnie in his spotlight)."""
    m = brawl_2d._mat('M_BrawlGlow')
    m.set_editor_property('blend_mode', unreal.BlendMode.BLEND_ADDITIVE)
    m.set_editor_property('shading_model', unreal.MaterialShadingModel.MSM_UNLIT)
    tx = _e(m, unreal.MaterialExpressionTextureSampleParameter2D, -800, 0, parameter_name='Tex',
            texture=AL.load_asset(DEST + '/T_ring'), sampler_type=unreal.MaterialSamplerType.SAMPLERTYPE_LINEAR_GRAYSCALE)
    c = _e(m, unreal.MaterialExpressionVectorParameter, -800, 250, parameter_name='Color', default_value=unreal.LinearColor(1, 0.31, 0.72, 1))
    g = _e(m, unreal.MaterialExpressionScalarParameter, -800, 400, parameter_name='Glow', default_value=2.0)
    a = _e(m, unreal.MaterialExpressionMultiply, -500, 100)
    MEL.connect_material_expressions(tx, 'R', a, 'A')
    MEL.connect_material_expressions(c, '', a, 'B')
    b = _e(m, unreal.MaterialExpressionMultiply, -300, 200)
    MEL.connect_material_expressions(a, '', b, 'A')
    MEL.connect_material_expressions(g, '', b, 'B')
    MEL.connect_material_property(b, '', unreal.MaterialProperty.MP_EMISSIVE_COLOR)
    brawl_2d._done(m)


def the_map():
    dst = '/Game/Brawl/Maps/L_VelvetRoom'
    if not AL.does_asset_exist(dst):
        AL.duplicate_asset('/Game/Brawl/Maps/L_RustyAnchor', dst)
    return dst


def build():
    got = textures()
    m_disco()
    m_glow()
    mp = the_map()
    AL.save_directory(DEST)
    AL.save_directory('/Game/Brawl/Maps')
    unreal.log('BRAWLVELVET built %d textures, M_BrawlDisco, M_BrawlGlow, %s' % (len(got), mp))
    print('BRAWLVELVET ok %d %s' % (len(got), mp))
    return got


if __name__ == '__main__':
    build()
