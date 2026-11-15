"""FULL THROTTLE brawl 0.2: the art pass (Meshy cast + Codex set dressing + the 80s CRT/VHS pass).

Sources live outside the repo (big): ~/Wisper/Log/Builds/full-throttle/brawl-art-39439/
  glb/<who>.glb      one rigged Meshy character per file, every fight clip as its own animation
                     (merged and root-pinned by scripts/merge.py in Blender: the game moves the bodies)
  import/T_*.png     Codex art, resized: n_* neon signs (on black, drawn additive), p_* posters,
                     t_brick / t_floor tiling textures, h_* HUD portraits
Run headless (the editor closed):
  UnrealEditor-Cmd FullThrottleBrawl.uproject -run=pythonscript -script="<abs>/Content/Python/brawl_art.py"
or in the editor: import brawl_art; brawl_art.build()

What it makes (the game finds them by path, AFTBrawlFighter / AFTBrawlGameMode):
  /Game/Brawl/Characters/<Who>/SK_<Who>, A_<Who>_<clip>     (Who = Nova Punk Biker Sleaze Bouncer Earl)
  /Game/Brawl/Art/T_*                                         the Codex textures
  /Game/Brawl/Materials/M_BrawlNeon, M_BrawlPoster, M_BrawlFloor, M_BrawlWall, M_BrawlBlood, M_BrawlFlash
  /Game/Brawl/Materials/M_PP_Retro + MI_PP_Retro              the CRT / VHS pass, after the tonemapper
"""
import os
import unreal

AL = unreal.EditorAssetLibrary
MEL = unreal.MaterialEditingLibrary
AT = unreal.AssetToolsHelpers.get_asset_tools()
SRC = os.path.expanduser('~/Wisper/Log/Builds/full-throttle/brawl-art-39439')
CHARS = '/Game/Brawl/Characters'
ART = '/Game/Brawl/Art'
MATS = '/Game/Brawl/Materials'
WHO = ['Nova', 'Punk', 'Biker', 'Sleaze', 'Bouncer', 'Earl']


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
        t.set_editor_property('automated', True)
        t.set_editor_property('replace_existing', True)
        t.set_editor_property('save', True)
        tasks.append(t)
    AT.import_asset_tasks(tasks)
    return [p for t in tasks for p in t.get_editor_property('imported_object_paths')]


# ---------------------------------------------------------------- the cast
def characters():
    out = []
    for who in WHO:
        dest = CHARS + '/' + who
        if AL.does_directory_exist(dest):
            AL.delete_directory(dest)
        _import([os.path.join(SRC, 'glb', who.lower() + '.glb')], dest)
        for p in AL.list_assets(dest, recursive=True, include_folder=False):
            a = AL.load_asset(p)
            if a is None:
                continue
            base = p.rsplit('/', 1)[1].split('.')[0]
            folder = p.rsplit('/', 1)[0]
            new = None
            if isinstance(a, unreal.SkeletalMesh):
                new = 'SK_' + who
            elif isinstance(a, unreal.Skeleton):
                new = 'SKEL_' + who
            elif isinstance(a, unreal.PhysicsAsset):
                new = 'PHYS_' + who
            elif isinstance(a, unreal.AnimSequence):
                clip = base.split('_Anim_')[-1] if '_Anim_' in base else base
                for pre in ('Anim_', 'A_'):
                    if clip.startswith(pre):
                        clip = clip[len(pre):]
                new = 'A_%s_%s' % (who, clip)
            if new and new != base:
                AL.rename_asset(p, dest + '/' + new)
            out.append('%s -> %s' % (base, new or base))
        AL.save_directory(dest)
    return out


# ---------------------------------------------------------------- the Codex art
def textures():
    files = sorted(os.path.join(SRC, 'import', f) for f in os.listdir(os.path.join(SRC, 'import')) if f.endswith('.png'))
    got = _import(files, ART)
    for p in got:
        t = AL.load_asset(p)
        if isinstance(t, unreal.Texture2D):
            n = p.rsplit('/', 1)[1]
            if n.startswith('T_h_'):
                t.set_editor_property('lod_group', unreal.TextureGroup.TEXTUREGROUP_UI)
                t.set_editor_property('compression_settings', unreal.TextureCompressionSettings.TC_EDITOR_ICON)
            AL.save_loaded_asset(t)
    return got


# ---------------------------------------------------------------- materials
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


def _tex_param(m, x, y, tex, name='Tex'):
    return _e(m, unreal.MaterialExpressionTextureSampleParameter2D, x, y, parameter_name=name, texture=AL.load_asset(ART + '/' + tex))


def _done(m):
    MEL.layout_material_expressions(m)
    if os.environ.get('BRAWL_COMPILE', '1') == '1':
        MEL.recompile_material(m)   # headless: BRAWL_COMPILE=0 (each compile is minutes there); the game compiles on load
    AL.save_loaded_asset(m)


def m_neon():
    # the Codex neon signs: black = nothing (additive), the tubes glow past the cel pass's EmisCut
    m = _mat('M_BrawlNeon')
    m.set_editor_property('blend_mode', unreal.BlendMode.BLEND_ADDITIVE)
    m.set_editor_property('shading_model', unreal.MaterialShadingModel.MSM_UNLIT)
    m.set_editor_property('two_sided', True)
    tx = _tex_param(m, -600, 0, 'T_n_anchor')
    g = _e(m, unreal.MaterialExpressionScalarParameter, -600, 250, parameter_name='Glow', default_value=6.0)
    fl = _e(m, unreal.MaterialExpressionScalarParameter, -600, 350, parameter_name='Flicker', default_value=1.0)
    mul = _e(m, unreal.MaterialExpressionMultiply, -300, 0)
    MEL.connect_material_expressions(tx, 'RGB', mul, 'A')
    MEL.connect_material_expressions(g, '', mul, 'B')
    mul2 = _e(m, unreal.MaterialExpressionMultiply, -150, 0)
    MEL.connect_material_expressions(mul, '', mul2, 'A')
    MEL.connect_material_expressions(fl, '', mul2, 'B')
    MEL.connect_material_property(mul2, '', unreal.MaterialProperty.MP_EMISSIVE_COLOR)
    _done(m)


def m_poster():
    m = _mat('M_BrawlPoster')
    tx = _tex_param(m, -600, 0, 'T_p_earl')
    tint = _e(m, unreal.MaterialExpressionVectorParameter, -600, 250, parameter_name='Tint', default_value=unreal.LinearColor(1, 1, 1, 1))
    mul = _e(m, unreal.MaterialExpressionMultiply, -300, 0)
    MEL.connect_material_expressions(tx, 'RGB', mul, 'A')
    MEL.connect_material_expressions(tint, '', mul, 'B')
    MEL.connect_material_property(mul, '', unreal.MaterialProperty.MP_BASE_COLOR)
    r = _e(m, unreal.MaterialExpressionConstant, -300, 200, r=0.9)
    MEL.connect_material_property(r, '', unreal.MaterialProperty.MP_ROUGHNESS)
    _done(m)


def m_world(name, tex, axes, tile):
    # world-aligned tiling (no stretching on the long floor / wall boxes): UV = world position / Tile
    m = _mat(name)
    wp = _e(m, unreal.MaterialExpressionWorldPosition, -1000, 0)
    mask = _e(m, unreal.MaterialExpressionComponentMask, -800, 0, r=('R' in axes), g=('G' in axes), b=('B' in axes), a=False)
    MEL.connect_material_expressions(wp, '', mask, '')
    t = _e(m, unreal.MaterialExpressionScalarParameter, -800, 200, parameter_name='Tile', default_value=tile)
    div = _e(m, unreal.MaterialExpressionDivide, -600, 0)
    MEL.connect_material_expressions(mask, '', div, 'A')
    MEL.connect_material_expressions(t, '', div, 'B')
    flip = _e(m, unreal.MaterialExpressionMultiply, -500, 0)
    MEL.connect_material_expressions(div, '', flip, 'A')
    c = _e(m, unreal.MaterialExpressionConstant2Vector, -600, 150, r=1.0, g=-1.0)
    MEL.connect_material_expressions(c, '', flip, 'B')
    tx = _tex_param(m, -350, 0, tex)
    MEL.connect_material_expressions(flip, '', tx, 'UVs')
    tint = _e(m, unreal.MaterialExpressionVectorParameter, -350, 250, parameter_name='Tint', default_value=unreal.LinearColor(1, 1, 1, 1))
    mul = _e(m, unreal.MaterialExpressionMultiply, -150, 0)
    MEL.connect_material_expressions(tx, 'RGB', mul, 'A')
    MEL.connect_material_expressions(tint, '', mul, 'B')
    MEL.connect_material_property(mul, '', unreal.MaterialProperty.MP_BASE_COLOR)
    r = _e(m, unreal.MaterialExpressionScalarParameter, -150, 250, parameter_name='Rough', default_value=0.85)
    MEL.connect_material_property(r, '', unreal.MaterialProperty.MP_ROUGHNESS)
    _done(m)


def m_blood():
    # wet, dark arterial red; the cel pass bands it like everything else, the spec gives it a hard glint
    m = _mat('M_BrawlBlood')
    c = _e(m, unreal.MaterialExpressionVectorParameter, -500, 0, parameter_name='Color', default_value=unreal.LinearColor(0.32, 0.0, 0.01, 1))
    MEL.connect_material_property(c, '', unreal.MaterialProperty.MP_BASE_COLOR)
    r = _e(m, unreal.MaterialExpressionConstant, -500, 200, r=0.18)
    MEL.connect_material_property(r, '', unreal.MaterialProperty.MP_ROUGHNESS)
    s = _e(m, unreal.MaterialExpressionConstant, -500, 300, r=0.9)
    MEL.connect_material_property(s, '', unreal.MaterialProperty.MP_SPECULAR)
    _done(m)


def m_flash():
    # the overlay on a fighter when he is hit: a red-white flash
    m = _mat('M_BrawlFlash')
    m.set_editor_property('blend_mode', unreal.BlendMode.BLEND_TRANSLUCENT)
    m.set_editor_property('shading_model', unreal.MaterialShadingModel.MSM_UNLIT)
    c = _e(m, unreal.MaterialExpressionVectorParameter, -500, 0, parameter_name='Color', default_value=unreal.LinearColor(1, 0.2, 0.2, 1))
    a = _e(m, unreal.MaterialExpressionScalarParameter, -500, 200, parameter_name='Amount', default_value=0.0)
    MEL.connect_material_property(c, '', unreal.MaterialProperty.MP_EMISSIVE_COLOR)
    MEL.connect_material_property(a, '', unreal.MaterialProperty.MP_OPACITY)
    _done(m)


# ---------------------------------------------------------------- the 1986 screen: CRT curve, RGB split, VHS wobble, scanlines, grille, grain, grade
RETRO_HLSL = r"""
float2 vp = GetViewportUV(Parameters);
float2 c = vp - 0.5;
float r2 = dot(c, c);
float2 cv = 0.5 + c * (1.0 + Curve * r2);
if (cv.x < 0.0 || cv.y < 0.0 || cv.x > 1.0 || cv.y > 1.0) return float3(0, 0, 0);
float2 ip = View.ViewSizeAndInvSize.zw;
float k = View.ViewSizeAndInvSize.y / 1080.0;
float ca = Chroma * k * (0.5 + r2 * 3.0);
// tape wobble: a slow sine, and now and then a torn tracking band rolling down the picture
float band = smoothstep(0.0, 0.02, 0.02 - abs(frac(Time * 0.11) * 1.4 - 0.2 - cv.y));
float wob = (sin(cv.y * 160.0 + Time * 6.0) * 0.00035 + band * 0.004 * sin(cv.y * 900.0 + Time * 40.0)) * Wobble;
float2 base = cv + float2(wob, 0);
float3 o;
o.r = SceneTextureLookup(ViewportUVToBufferUV(base + float2(ca * ip.x, 0)), 14, false).r;
o.g = SceneTextureLookup(ViewportUVToBufferUV(base), 14, false).g;
o.b = SceneTextureLookup(ViewportUVToBufferUV(base - float2(ca * ip.x, 0)), 14, false).b;
o = max(o, 0.0);
// grade: punchy colour, violet in the shadows, a warm pink roll-off up top
float l = dot(o, float3(0.299, 0.587, 0.114));
o = lerp(l.xxx, o, Sat);
o += ShadowLift.rgb * (1.0 - saturate(l)) * (1.0 - saturate(l)) * 0.1;
o *= lerp(float3(1, 1, 1), HighTint.rgb, smoothstep(0.45, 1.0, l));
o += band * 0.06;
// scanlines (Lines over the screen height, like a 240p set), aperture grille, grain
float sl = sin(cv.y * Lines * 3.14159265);
o *= 1.0 - Scan * (1.0 - sl * sl);
float gx = frac(vp.x * View.ViewSizeAndInvSize.x / max(3.0 * k, 1.0));
float3 msk = gx < 0.3333 ? float3(1.0, 0.82, 0.82) : (gx < 0.6667 ? float3(0.82, 1.0, 0.82) : float3(0.82, 0.82, 1.0));
o *= lerp(float3(1, 1, 1), msk, Mask);
float n = frac(sin(dot(floor(vp * View.ViewSizeAndInvSize.xy / max(2.0 * k, 1.0)) + frac(Time * 7.13) * 97.0, float2(12.9898, 78.233))) * 43758.5453);
o += (n - 0.5) * Noise;
// the tube's rounded black edge
float e = smoothstep(0.0, 0.012, min(min(cv.x, 1.0 - cv.x), min(cv.y, 1.0 - cv.y)));
return o * e * Bright;
"""
RETRO_PARAMS = [('Curve', 0.07), ('Chroma', 2.2), ('Wobble', 1.0), ('Sat', 1.18), ('Lines', 288.0), ('Scan', 0.3),
                ('Mask', 0.14), ('Noise', 0.045), ('Bright', 1.16)]
RETRO_VECTORS = [('ShadowLift', (0.45, 0.08, 0.75, 1)), ('HighTint', (1.06, 0.95, 1.0, 1))]


def pp_retro():
    m = _mat('M_PP_Retro')
    m.set_editor_property('material_domain', unreal.MaterialDomain.MD_POST_PROCESS)
    m.set_editor_property('blendable_location', unreal.BlendableLocation.BL_SCENE_COLOR_AFTER_TONEMAPPING)
    col = _e(m, unreal.MaterialExpressionSceneTexture, -1100, -300, scene_texture_id=unreal.SceneTextureId.PPI_POST_PROCESS_INPUT0)
    tm = _e(m, unreal.MaterialExpressionTime, -1100, -150)
    ins = [('Col', col, 'Color'), ('Time', tm, '')]
    y = 0
    for nm, v in RETRO_PARAMS:
        ins.append((nm, _e(m, unreal.MaterialExpressionScalarParameter, -1100, y, parameter_name=nm, default_value=v), ''))
        y += 70
    for nm, v in RETRO_VECTORS:
        ins.append((nm, _e(m, unreal.MaterialExpressionVectorParameter, -1100, y, parameter_name=nm, default_value=unreal.LinearColor(*v)), ''))
        y += 70
    cu = _e(m, unreal.MaterialExpressionCustom, -400, 0, code=RETRO_HLSL, output_type=unreal.CustomMaterialOutputType.CMOT_FLOAT3, description='Retro')
    cis = []
    for nm, _, _ in ins:
        ci = unreal.CustomInput()
        ci.set_editor_property('input_name', nm)
        cis.append(ci)
    cu.set_editor_property('inputs', cis)
    for nm, ex, outn in ins:
        MEL.connect_material_expressions(ex, outn, cu, nm)
    MEL.connect_material_property(cu, '', unreal.MaterialProperty.MP_EMISSIVE_COLOR)
    _done(m)
    full = MATS + '/MI_PP_Retro'
    inst = AL.load_asset(full) if AL.does_asset_exist(full) else AT.create_asset('MI_PP_Retro', MATS, unreal.MaterialInstanceConstant, unreal.MaterialInstanceConstantFactoryNew())
    inst.set_editor_property('parent', m)
    MEL.update_material_instance(inst)
    AL.save_loaded_asset(inst)


def materials():
    m_neon()
    m_poster()
    m_world('M_BrawlFloor', 'T_t_floor', 'RG', 240.0)
    m_world('M_BrawlWall', 'T_t_brick', 'RB', 300.0)
    m_blood()
    m_flash()
    pp_retro()


def build(what='all'):
    log = []
    if what in ('all', 'tex'):
        log += textures()
    if what in ('all', 'chars'):
        log += characters()
    if what in ('all', 'mats'):
        materials()
    for l in log:
        unreal.log('BRAWLART ' + str(l))
    for p in AL.list_assets('/Game/Brawl', recursive=True, include_folder=False):
        unreal.log('BRAWLART asset ' + p)
    return log


if __name__ == '__main__':
    build(os.environ.get('BRAWL_ART', 'all'))
