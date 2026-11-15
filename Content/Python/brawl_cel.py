"""FULL THROTTLE brawl: the main game's cel look, copied (not referenced) into /Game/Brawl.

M_PP_Cel is full-throttle-unreal's Content/Python/ft_cel.py pp_cel(), word for word: the same HLSL
(CEL_HLSL), the same parameters and defaults, the same graph (four scene textures into one Custom node,
into emissive, at "scene colour before DOF"). Only the folder differs: /Game/Brawl/Materials, so the
brawl merges into full-throttle-unreal without touching /Game/Core (MERGE.md: on merge, point
MI_PP_Cel_RustyAnchor at /Game/Core/Materials/M_PP_Cel and delete this copy).

Run over the Unreal MCP connector:  import brawl_cel; brawl_cel.build()
"""
import unreal

AL = unreal.EditorAssetLibrary
MEL = unreal.MaterialEditingLibrary
AT = unreal.AssetToolsHelpers.get_asset_tools()
MATS = '/Game/Brawl/Materials'
MAPS = '/Game/Brawl/Maps'


def hex_lin(h):
    h = h.lstrip('#')
    def f(v):
        v = v / 255.0
        return v / 12.92 if v <= 0.04045 else ((v + 0.055) / 1.055) ** 2.4
    return tuple(f(int(h[i:i + 2], 16)) for i in (0, 2, 4))


# ---- ft_cel.py CEL_HLSL (FULL THROTTLE 2.2+), unchanged
CEL_HLSL = r"""
float3 W = float3(0.2126, 0.7152, 0.0722);
float3 C = Col.rgb;
float3 B = Base.rgb;
float d0 = max(Depth.r, 1.0);
float bl = dot(B, W);
float3 o = C;
if (bl > 0.004)
{
    float l = dot(C, W) / bl / Key;
    float3 hue = C / max(B, 0.004);
    hue = hue / max(dot(hue, W), 1e-4);
    hue = lerp(float3(1, 1, 1), clamp(hue, 0.0, 3.0), HueKeep);
    float3 cel;
    if (l >= EmisCut)   cel = C;
    else if (l >= Hot)  cel = B * Key * HotLevel * hue * LitTint.rgb;
    else if (l >= Th1)  cel = B * Key * hue * LitTint.rgb;
    else if (l >= Th2)  cel = B * Key * MidLevel * hue * lerp(ShadowTint.rgb, LitTint.rgb, 0.5);
    else                cel = B * Key * ShadowLevel * hue * ShadowTint.rgb;
    cel *= (l >= EmisCut) ? 1.0 : Bright;
    if (Debug > 0.5)
    {
        // false colour of l (light / Key), to tune Key and the thresholds from a game screenshot
        float e = log2(max(l, 1e-4));
        cel = e < -3 ? float3(0.2, 0, 0.3) : e < -2 ? float3(0.6, 0, 0.6) : e < -1 ? float3(1, 0, 0) : e < -0.5 ? float3(1, 0.5, 0)
            : e < 0 ? float3(1, 1, 0) : e < 0.5 ? float3(0, 1, 0) : e < 1 ? float3(0, 1, 1) : e < 2 ? float3(0, 0.3, 1) : float3(1, 1, 1);
        cel *= Key;
    }
    float fade = 1.0 - smoothstep(FadeNear, FadeFar, d0);
    o = lerp(C, cel, fade);
}
// ---- ink
float2 uv = GetDefaultSceneTextureUV(Parameters, 1);
float2 px = View.BufferSizeAndInvSize.zw * max(InkPx * View.ViewSizeAndInvSize.y / 1080.0, 0.75);
float dl = max(SceneTextureLookup(uv - float2(px.x, 0), 1, false).r, 1.0);
float dr = max(SceneTextureLookup(uv + float2(px.x, 0), 1, false).r, 1.0);
float du = max(SceneTextureLookup(uv - float2(0, px.y), 1, false).r, 1.0);
float dd = max(SceneTextureLookup(uv + float2(0, px.y), 1, false).r, 1.0);
float i0 = 1.0 / d0;
float lap = max(abs(1.0 / dl + 1.0 / dr - 2.0 * i0), abs(1.0 / du + 1.0 / dd - 2.0 * i0)) * d0;
float de = smoothstep(InkDepth, InkDepth * 2.0, lap);
float3 n0 = Nrm.rgb;
float ln0 = length(n0);
float ne = 0.0;
if (ln0 > 0.5)
{
    n0 /= ln0;
    float3 a = SceneTextureLookup(uv - float2(px.x, 0), 8, false).rgb;
    float3 b = SceneTextureLookup(uv + float2(px.x, 0), 8, false).rgb;
    float3 c = SceneTextureLookup(uv - float2(0, px.y), 8, false).rgb;
    float3 e = SceneTextureLookup(uv + float2(0, px.y), 8, false).rgb;
    float m = 0.0;
    if (length(a) > 0.5) m = max(m, 1.0 - dot(n0, normalize(a)));
    if (length(b) > 0.5) m = max(m, 1.0 - dot(n0, normalize(b)));
    if (length(c) > 0.5) m = max(m, 1.0 - dot(n0, normalize(c)));
    if (length(e) > 0.5) m = max(m, 1.0 - dot(n0, normalize(e)));
    ne = smoothstep(InkNormal, InkNormal + 0.12, m);
}
float ink = max(de, ne) * lerp(1.0, InkFar, smoothstep(InkFadeNear, InkFadeFar, d0)) * InkAmount;
// no ink in the empty sky itself
ink *= step(d0, SkyDepth);
return lerp(o, InkCol.rgb * Key * Bright, saturate(ink));
"""

CEL_PARAMS = [
    ('Key', 1.0), ('Bright', 1.0), ('Th1', 0.62), ('Th2', 0.36), ('Hot', 1.9), ('HotLevel', 1.3), ('EmisCut', 4.0),
    ('MidLevel', 0.78), ('ShadowLevel', 0.58), ('HueKeep', 0.6), ('FadeNear', 60000.0), ('FadeFar', 160000.0),
    ('InkPx', 1.0), ('InkDepth', 0.035), ('InkNormal', 0.45), ('InkFar', 0.35), ('InkFadeNear', 15000.0),
    ('InkFadeFar', 90000.0), ('InkAmount', 1.0), ('SkyDepth', 150000.0), ('Debug', 0.0),
]
CEL_VECTORS = [('LitTint', (1, 1, 1, 1)), ('ShadowTint', (0.86, 0.84, 1.0, 1)), ('InkCol', hex_lin('1b1033') + (1,))]

# The Rusty Anchor: an indoor bar under a warm key; tuned from a PIE screenshot (Key = light / base colour on
# the lit side), like ft_cel.LEVELS[i]['cel']
ANCHOR = dict(Key=1.0, Bright=1.0, HueKeep=0.45)


def _dir(p):
    if not AL.does_directory_exist(p):
        AL.make_directory(p)


def _sparam(m, name, val, y):
    e = MEL.create_material_expression(m, unreal.MaterialExpressionScalarParameter, -1100, y)
    e.set_editor_property('parameter_name', name)
    e.set_editor_property('default_value', val)
    return e


def _vparam(m, name, val, y):
    e = MEL.create_material_expression(m, unreal.MaterialExpressionVectorParameter, -1100, y)
    e.set_editor_property('parameter_name', name)
    e.set_editor_property('default_value', unreal.LinearColor(*val))
    return e


def _scene_tex(m, ppi, y):
    e = MEL.create_material_expression(m, unreal.MaterialExpressionSceneTexture, -1100, y)
    e.set_editor_property('scene_texture_id', ppi)
    return e


def pp_cel():
    _dir(MATS)
    full = MATS + '/M_PP_Cel'
    if AL.does_asset_exist(full):
        m = AL.load_asset(full)
        MEL.delete_all_material_expressions(m)
    else:
        m = AT.create_asset('M_PP_Cel', MATS, unreal.Material, unreal.MaterialFactoryNew())
    m.set_editor_property('material_domain', unreal.MaterialDomain.MD_POST_PROCESS)
    m.set_editor_property('blendable_location', unreal.BlendableLocation.BL_SCENE_COLOR_BEFORE_DOF)
    col = _scene_tex(m, unreal.SceneTextureId.PPI_POST_PROCESS_INPUT0, -400)
    base = _scene_tex(m, unreal.SceneTextureId.PPI_BASE_COLOR, -250)
    dep = _scene_tex(m, unreal.SceneTextureId.PPI_SCENE_DEPTH, -100)
    nrm = _scene_tex(m, unreal.SceneTextureId.PPI_WORLD_NORMAL, 50)
    ins = [('Col', col, 'Color'), ('Base', base, 'Color'), ('Depth', dep, 'Color'), ('Nrm', nrm, 'Color')]
    y = 200
    for nm, v in CEL_PARAMS:
        ins.append((nm, _sparam(m, nm, v, y), ''))
        y += 70
    for nm, v in CEL_VECTORS:
        ins.append((nm, _vparam(m, nm, v, y), ''))
        y += 70
    cu = MEL.create_material_expression(m, unreal.MaterialExpressionCustom, -400, 0)
    cu.set_editor_property('code', CEL_HLSL)
    cu.set_editor_property('output_type', unreal.CustomMaterialOutputType.CMOT_FLOAT3)
    cu.set_editor_property('description', 'CelInk')
    cis = []
    for nm, _, _ in ins:
        ci = unreal.CustomInput()
        ci.set_editor_property('input_name', nm)
        cis.append(ci)
    cu.set_editor_property('inputs', cis)
    for nm, ex, outn in ins:
        MEL.connect_material_expressions(ex, outn, cu, nm)
    MEL.connect_material_property(cu, '', unreal.MaterialProperty.MP_EMISSIVE_COLOR)
    MEL.layout_material_expressions(m)
    MEL.recompile_material(m)
    AL.save_loaded_asset(m)
    return m


def cel_mi(**over):
    full = MATS + '/MI_PP_Cel_RustyAnchor'
    if AL.does_asset_exist(full):
        inst = AL.load_asset(full)
    else:
        inst = AT.create_asset('MI_PP_Cel_RustyAnchor', MATS, unreal.MaterialInstanceConstant, unreal.MaterialInstanceConstantFactoryNew())
    inst.set_editor_property('parent', AL.load_asset(MATS + '/M_PP_Cel'))
    for k, v in dict(ANCHOR, **over).items():
        MEL.set_material_instance_scalar_parameter_value(inst, k, v)
    MEL.update_material_instance(inst)
    AL.save_loaded_asset(inst)
    return inst


def level():
    """The empty map: AFTBrawlGameMode builds the venue at BeginPlay."""
    _dir(MAPS)
    path = MAPS + '/L_RustyAnchor'
    les = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
    if not AL.does_asset_exist(path):
        les.new_level(path)
    else:
        les.load_level(path)
    ws = unreal.get_editor_subsystem(unreal.UnrealEditorSubsystem).get_editor_world().get_world_settings()
    ws.set_editor_property('default_game_mode', unreal.load_class(None, '/Script/FullThrottleBrawl.FTBrawlGameMode'))
    les.save_current_level()
    return path


def build():
    pp_cel()
    cel_mi()
    return 'built ' + level()
