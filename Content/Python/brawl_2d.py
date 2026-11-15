"""FULL THROTTLE brawl 0.3: the 2.5D art. Painted sprites and painted backdrops in a lit 3D room.

Sources are in the repo: beat-em-up-unreal/ArtSource/
  frames/<who>_<frame>.png    1024x1024 sprite frames, feet anchored at (512, 1000), facing RIGHT
                              (cut from sheets/ by tools/frames.py; who = nova, dock, punk, biker, sleaze, bouncer, earl, prop)
  backgrounds/bg_*.png        straight-on back wall panels (3:2), floor.png the top-down plank floor
Run in the editor (Unreal MCP connector, ue_exec_python): import brawl_2d; brawl_2d.build()
or headless: UnrealEditor-Cmd FullThrottleBrawl.uproject -run=pythonscript -script="<abs>/Content/Python/brawl_2d.py"

Makes (AFTBrawlFighter / AFTBrawlGameMode find them by path):
  /Game/Brawl/Sprites/<Who>/T_<who>_<frame>      Who = Nova, Dock, Punk, Biker, Sleaze, Bouncer, Earl, Props, Items
  /Game/Brawl/Backdrop/T_bg_bar, T_bg_games, T_bg_booths, T_floor
  /Game/Brawl/Materials/M_BrawlSprite    masked, lit, two-sided: Tex, Tint, Flash, SelfLit (casts real shadows)
  /Game/Brawl/Materials/M_BrawlEnemy     combat sprite with ink separation and directional faction light
  /Game/Brawl/Materials/M_BrawlPaint     opaque, lit, painted backdrop: Tex, Tint, Glow, TileU, TileV
"""
import os
import unreal

AL = unreal.EditorAssetLibrary
MEL = unreal.MaterialEditingLibrary
AT = unreal.AssetToolsHelpers.get_asset_tools()
HERE = os.path.dirname(os.path.abspath(__file__))
SRC = os.path.normpath(os.path.join(HERE, '..', '..', 'ArtSource'))
if not os.path.isdir(os.path.join(SRC, 'frames')):
    SRC = os.path.normpath(os.path.join(HERE, '..', '..', '..', 'beat-em-up-unreal', 'ArtSource'))
MATS = '/Game/Brawl/Materials'
WHO = {'nova': 'Nova', 'dock': 'Dock', 'punk': 'Punk', 'biker': 'Biker', 'sleaze': 'Sleaze',
       'bouncer': 'Bouncer', 'earl': 'Earl', 'prop': 'Props', 'item': 'Items',
       'vinnie': 'Vinnie', 'guard': 'Guard',   # night 2, the Velvet Room (run 16961)
       'voss': 'Voss', 'car': 'Props',         # night 1's cutscenes (run 49531)
       'kip': 'Kip',                           # day 1's cutscene (run 73444)
       'pitboss': 'Pitboss', 'dealer': 'Dealer',   # night 3, the Lucky 7 (run 52616)
       'frankie': 'Frankie', 'kingpin': 'Kingpin', 'sven': 'Sven', 'commodore': 'Commodore'}   # nights 4-7 (run 32365)


def _dir(p):
    if not AL.does_directory_exist(p):
        AL.make_directory(p)


def _import(files, dest):
    _dir(dest)
    tasks = []
    for f in files:
        t = unreal.AssetImportTask()
        t.set_editor_property('filename', f)
        t.set_editor_property('destination_path', dest)
        t.set_editor_property('destination_name', 'T_' + os.path.splitext(os.path.basename(f))[0])
        t.set_editor_property('automated', True)
        t.set_editor_property('replace_existing', True)
        t.set_editor_property('save', False)
        tasks.append(t)
    AT.import_asset_tasks(tasks)
    return [p for t in tasks for p in t.get_editor_property('imported_object_paths')]


def textures():
    got = []
    fr = os.path.join(SRC, 'frames')
    for pre, who in WHO.items():
        files = sorted(os.path.join(fr, f) for f in os.listdir(fr) if f.startswith(pre + '_') and f.endswith('.png'))
        for p in _import(files, '/Game/Brawl/Sprites/' + who):
            t = AL.load_asset(p)
            t.set_editor_property('lod_group', unreal.TextureGroup.TEXTUREGROUP_CHARACTER)
            t.set_editor_property('never_stream', True)
            AL.save_loaded_asset(t)
            got.append(p)
    bg = os.path.join(SRC, 'backgrounds')
    for p in _import(sorted(os.path.join(bg, f) for f in os.listdir(bg) if f.endswith('.png')), '/Game/Brawl/Backdrop'):
        t = AL.load_asset(p)
        t.set_editor_property('lod_group', unreal.TextureGroup.TEXTUREGROUP_WORLD)
        t.set_editor_property('never_stream', True)
        if p.endswith('T_floor'):
            t.set_editor_property('address_x', unreal.TextureAddress.TA_MIRROR)
            t.set_editor_property('address_y', unreal.TextureAddress.TA_MIRROR)
        else:
            t.set_editor_property('address_x', unreal.TextureAddress.TA_CLAMP)
            t.set_editor_property('address_y', unreal.TextureAddress.TA_CLAMP)
        AL.save_loaded_asset(t)
        got.append(p)
    return got


def only(names):
    """Import just these frames (e.g. ['item_beer', 'nova_drink']), leaving every other texture alone."""
    got = []
    fr = os.path.join(SRC, 'frames')
    for n in names:
        who = WHO[n.split('_')[0]]
        for p in _import([os.path.join(fr, n + '.png')], '/Game/Brawl/Sprites/' + who):
            t = AL.load_asset(p)
            t.set_editor_property('lod_group', unreal.TextureGroup.TEXTUREGROUP_CHARACTER)
            t.set_editor_property('never_stream', True)
            AL.save_loaded_asset(t)
            got.append(p)
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
    if os.environ.get('BRAWL_COMPILE', '1') == '1':
        MEL.recompile_material(m)
    AL.save_loaded_asset(m)


def m_sprite(enemy=False):
    """A painted sprite on a camera-facing quad: lit by the room's real lights, casts a real shadow.
    SelfLit keeps the painted colours (the art already has its own light and ink); Flash = the white hit flash."""
    m = _mat('M_BrawlEnemy' if enemy else 'M_BrawlSprite')
    m.set_editor_property('blend_mode', unreal.BlendMode.BLEND_MASKED)
    m.set_editor_property('two_sided', True)
    m.set_editor_property('opacity_mask_clip_value', 0.4)
    default_frame = AL.load_asset('/Game/Brawl/Sprites/Nova/T_nova_idle')
    frame = _e(m, unreal.MaterialExpressionTextureSampleParameter2D, -900, 0, parameter_name='Tex',
               texture=default_frame)
    if enemy:
        color = _enemy_paint(m, default_frame)
        color_output = ''
    else:
        color = frame
        color_output = 'RGB'
    tint = _e(m, unreal.MaterialExpressionVectorParameter, -900, 300, parameter_name='Tint', default_value=unreal.LinearColor(1, 1, 1, 1))
    col = _e(m, unreal.MaterialExpressionMultiply, -600, 0)
    MEL.connect_material_expressions(color, color_output, col, 'A')
    MEL.connect_material_expressions(tint, '', col, 'B')
    MEL.connect_material_property(col, '', unreal.MaterialProperty.MP_BASE_COLOR)
    MEL.connect_material_property(frame, 'A', unreal.MaterialProperty.MP_OPACITY_MASK)
    # emissive = colour * SelfLit + Flash
    sl = _e(m, unreal.MaterialExpressionScalarParameter, -600, 250, parameter_name='SelfLit', default_value=0.32)
    em = _e(m, unreal.MaterialExpressionMultiply, -400, 150)
    MEL.connect_material_expressions(col, '', em, 'A')
    MEL.connect_material_expressions(sl, '', em, 'B')
    fl = _e(m, unreal.MaterialExpressionScalarParameter, -600, 400, parameter_name='Flash', default_value=0.0)
    fc = _e(m, unreal.MaterialExpressionVectorParameter, -600, 500, parameter_name='FlashColor', default_value=unreal.LinearColor(1.0, 0.85, 0.85, 1))
    fm = _e(m, unreal.MaterialExpressionMultiply, -400, 400)
    MEL.connect_material_expressions(fl, '', fm, 'A')
    MEL.connect_material_expressions(fc, '', fm, 'B')
    add = _e(m, unreal.MaterialExpressionAdd, -200, 200)
    MEL.connect_material_expressions(em, '', add, 'A')
    MEL.connect_material_expressions(fm, '', add, 'B')
    MEL.connect_material_property(add, '', unreal.MaterialProperty.MP_EMISSIVE_COLOR)
    r = _e(m, unreal.MaterialExpressionConstant, -200, 450, r=0.8)
    MEL.connect_material_property(r, '', unreal.MaterialProperty.MP_ROUGHNESS)
    s = _e(m, unreal.MaterialExpressionConstant, -200, 550, r=0.15)
    MEL.connect_material_property(s, '', unreal.MaterialProperty.MP_SPECULAR)
    _done(m)
    return m


def _enemy_paint(m, default_frame):
    tex = _e(m, unreal.MaterialExpressionTextureObjectParameter, -1700, 0, parameter_name='Tex', texture=default_frame)
    uv = _e(m, unreal.MaterialExpressionTextureCoordinate, -1700, 160)
    key = _e(m, unreal.MaterialExpressionVectorParameter, -1700, 300, parameter_name='EnemyKey',
             default_value=unreal.LinearColor(0.25, 0.46, 0.75, 1))
    rim = _e(m, unreal.MaterialExpressionScalarParameter, -1700, 460, parameter_name='EnemyRim', default_value=0.75)
    paint = _e(m, unreal.MaterialExpressionCustom, -1400, 0,
               description='Ink separation and colored edge light inside the painted alpha',
               output_type=unreal.CustomMaterialOutputType.CMOT_FLOAT3,
               code="""
float4 source = Texture2DSample(Frame, FrameSampler, UV);
float lum = dot(source.rgb, float3(0.2126, 0.7152, 0.0722));
float3 graded = max(lerp(lum.xxx, source.rgb, 1.1), 0.0);
graded *= lerp(0.82, 1.04, smoothstep(0.045, 0.46, lum));
float above = Texture2DSample(Frame, FrameSampler, UV + float2(-0.004, -0.007)).a;
float side = Texture2DSample(Frame, FrameSampler, UV + float2(-0.006, 0.002)).a;
float edge = saturate(source.a - min(above, side));
float upperLight = lerp(1.0, 0.44, smoothstep(0.56, 0.98, UV.y));
float3 illumination = Key.rgb * (0.16 * edge * upperLight * Strength);
return graded + illumination;
""")
    inputs = (('Frame', tex), ('UV', uv), ('Key', key), ('Strength', rim))
    custom_inputs = []
    for name, _ in inputs:
        item = unreal.CustomInput()
        item.set_editor_property('input_name', name)
        custom_inputs.append(item)
    paint.set_editor_property('inputs', custom_inputs)
    for name, source in inputs:
        MEL.connect_material_expressions(source, '', paint, name)
    return paint


def m_enemy():
    """Regenerate the combat-only material using the existing painted frame textures."""
    return m_sprite(enemy=True)


def m_paint():
    """A painted backdrop panel or the plank floor: lit (it takes the sprites' shadows and the neon's spill),
    Glow keeps the painted neon and lamps bright. TileU/TileV repeat the texture (floor)."""
    m = _mat('M_BrawlPaint')
    uv = _e(m, unreal.MaterialExpressionTextureCoordinate, -1200, 0)
    tu = _e(m, unreal.MaterialExpressionScalarParameter, -1200, 150, parameter_name='TileU', default_value=1.0)
    tv = _e(m, unreal.MaterialExpressionScalarParameter, -1200, 250, parameter_name='TileV', default_value=1.0)
    ap = _e(m, unreal.MaterialExpressionAppendVector, -1000, 200)
    MEL.connect_material_expressions(tu, '', ap, 'A')
    MEL.connect_material_expressions(tv, '', ap, 'B')
    mul = _e(m, unreal.MaterialExpressionMultiply, -850, 0)
    MEL.connect_material_expressions(uv, '', mul, 'A')
    MEL.connect_material_expressions(ap, '', mul, 'B')
    tx = _e(m, unreal.MaterialExpressionTextureSampleParameter2D, -650, 0, parameter_name='Tex',
            texture=AL.load_asset('/Game/Brawl/Backdrop/T_bg_bar'))
    MEL.connect_material_expressions(mul, '', tx, 'UVs')
    tint = _e(m, unreal.MaterialExpressionVectorParameter, -650, 300, parameter_name='Tint', default_value=unreal.LinearColor(1, 1, 1, 1))
    col = _e(m, unreal.MaterialExpressionMultiply, -400, 0)
    MEL.connect_material_expressions(tx, 'RGB', col, 'A')
    MEL.connect_material_expressions(tint, '', col, 'B')
    MEL.connect_material_property(col, '', unreal.MaterialProperty.MP_BASE_COLOR)
    g = _e(m, unreal.MaterialExpressionScalarParameter, -400, 250, parameter_name='Glow', default_value=0.45)
    em = _e(m, unreal.MaterialExpressionMultiply, -200, 150)
    MEL.connect_material_expressions(col, '', em, 'A')
    MEL.connect_material_expressions(g, '', em, 'B')
    MEL.connect_material_property(em, '', unreal.MaterialProperty.MP_EMISSIVE_COLOR)
    r = _e(m, unreal.MaterialExpressionScalarParameter, -200, 350, parameter_name='Rough', default_value=0.9)
    MEL.connect_material_property(r, '', unreal.MaterialProperty.MP_ROUGHNESS)
    s = _e(m, unreal.MaterialExpressionConstant, -200, 450, r=0.2)
    MEL.connect_material_property(s, '', unreal.MaterialProperty.MP_SPECULAR)
    _done(m)


def build():
    out = textures()
    m_sprite()
    m_enemy()
    m_paint()
    AL.save_directory('/Game/Brawl')
    unreal.log('BRAWL2D built %d textures + sprite, enemy and backdrop materials' % len(out))
    return out


if __name__ == '__main__':
    build()
