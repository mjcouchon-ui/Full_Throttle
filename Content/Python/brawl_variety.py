"""FULL THROTTLE brawl, stage 2 "variety": the enemy sprite material with colour tables and the boss's second-phase glow.

Makes /Game/Brawl/Materials/M_BrawlEnemyV. It is M_BrawlEnemy (brawl_2d.py: ink separation, coloured edge light, Tint,
Flash, SelfLit) plus:
  VarA0..3, VarB0..3, VarC0..3   four colour rules (AFTBrawlFighter::SetEnemyLook sets them from FTBrawlLooks.inl):
        A = hue centre, hue half width, saturation min, saturation max       (which painted colours the rule takes)
        B = value min, value max, absolute hue? (1/0), hue (target or shift)
        C = saturation multiply, saturation add, value multiply, on (1/0)
      With every rule off the picture is the old material's.
  Rage, RageColor                the boss's second phase: the ink line round the figure burns in RageColor (emissive).
M_BrawlEnemy itself is not touched; the fighter falls back to it if this material is missing.

Headless:
  powershell -NoProfile -ExecutionPolicy Bypass -File C:\\ftwork\\tools\\run_job.ps1 -Job <abs>/Content/Python/brawl_variety.py -Project <project> -Tag s2_variety_mat
Log line: FTBRAWL VARIETY material=<path> saved=<True|False>
"""
import unreal

AL = unreal.EditorAssetLibrary
MEL = unreal.MaterialEditingLibrary
AT = unreal.AssetToolsHelpers.get_asset_tools()
MATS = '/Game/Brawl/Materials'
NAME = 'M_BrawlEnemyV'

PAINT = """
float4 source = Texture2DSample(Frame, FrameSampler, UV);
// the colour tables work on the painted (display) values
float3 g = pow(max(source.rgb, 0.0), 1.0 / 2.2);
float mx = max(g.r, max(g.g, g.b));
float mn = min(g.r, min(g.g, g.b));
float d = mx - mn;
float h = 0.0;
if (d > 0.00001)
{
    if (mx == g.r) h = (g.g - g.b) / d;
    else if (mx == g.g) h = 2.0 + (g.b - g.r) / d;
    else h = 4.0 + (g.r - g.g) / d;
    h = frac(h / 6.0 + 1.0);
}
float s = mx > 0.00001 ? d / mx : 0.0;
float v = mx;
float4 As[4] = { A0, A1, A2, A3 };
float4 Bs[4] = { B0, B1, B2, B3 };
float4 Cs[4] = { C0, C1, C2, C3 };
float3 o = g;
for (int i = 0; i < 4; i++)
{
    float4 A = As[i]; float4 B = Bs[i]; float4 C = Cs[i];
    if (C.w > 0.5)
    {
        float hd = abs(frac(h - A.x + 1.5) - 0.5);
        float w = (1.0 - smoothstep(A.y, A.y * 1.5 + 0.01, hd))
            * smoothstep(A.z - 0.06, A.z, s) * (1.0 - smoothstep(A.w, A.w + 0.06, s))
            * smoothstep(B.x - 0.06, B.x, v) * (1.0 - smoothstep(B.y, B.y + 0.06, v));
        float nh = frac((B.z > 0.5 ? B.w : h + B.w) + 1.0);
        float ns = saturate(s * C.x + C.y);
        float nv = saturate(v * C.z);
        float3 k = fmod(float3(5.0, 3.0, 1.0) + nh * 6.0, 6.0);
        float3 c = nv - nv * ns * saturate(min(k, 4.0 - k));
        o = lerp(o, c, w);
    }
}
float3 paint = pow(max(o, 0.0), 2.2);
float lum = dot(paint, float3(0.2126, 0.7152, 0.0722));
float3 graded = max(lerp(lum.xxx, paint, 1.1), 0.0);
graded *= lerp(0.82, 1.04, smoothstep(0.045, 0.46, lum));
float above = Texture2DSample(Frame, FrameSampler, UV + float2(-0.004, -0.007)).a;
float side = Texture2DSample(Frame, FrameSampler, UV + float2(-0.006, 0.002)).a;
float edge = saturate(source.a - min(above, side));
float upperLight = lerp(1.0, 0.44, smoothstep(0.56, 0.98, UV.y));
float3 illumination = Key.rgb * (0.16 * edge * upperLight * Strength);
return graded + illumination;
"""

GLOW = """
// the second phase: the outline round the whole figure burns (a ring just inside the painted alpha), strongest low
// on the figure, and the ink inside warms a little
float a = Texture2DSample(Frame, FrameSampler, UV).a;
float r = 0.0075;
float m = min(min(Texture2DSample(Frame, FrameSampler, UV + float2(r, 0.0)).a, Texture2DSample(Frame, FrameSampler, UV + float2(-r, 0.0)).a),
              min(Texture2DSample(Frame, FrameSampler, UV + float2(0.0, r)).a, Texture2DSample(Frame, FrameSampler, UV + float2(0.0, -r)).a));
float r2 = 0.016;
float m2 = min(min(Texture2DSample(Frame, FrameSampler, UV + float2(r2, 0.0)).a, Texture2DSample(Frame, FrameSampler, UV + float2(-r2, 0.0)).a),
               min(Texture2DSample(Frame, FrameSampler, UV + float2(0.0, r2)).a, Texture2DSample(Frame, FrameSampler, UV + float2(0.0, -r2)).a));
float ring = saturate(a - m) * 1.0 + saturate(a - m2) * 0.45;
float low = lerp(0.75, 1.25, smoothstep(0.25, 0.95, UV.y));
return Color.rgb * (ring * low * Amount * 2.6 + 0.05 * Amount * a);
"""


def _e(m, cls, x, y, **props):
    e = MEL.create_material_expression(m, cls, x, y)
    for k, v in props.items():
        e.set_editor_property(k, v)
    return e


def _custom(m, x, y, desc, code, inputs):
    node = _e(m, unreal.MaterialExpressionCustom, x, y, description=desc,
              output_type=unreal.CustomMaterialOutputType.CMOT_FLOAT3, code=code)
    items = []
    for name, _ in inputs:
        item = unreal.CustomInput()
        item.set_editor_property('input_name', name)
        items.append(item)
    node.set_editor_property('inputs', items)
    for name, source in inputs:
        MEL.connect_material_expressions(source, '', node, name)
    return node


def build():
    full = MATS + '/' + NAME
    if AL.does_asset_exist(full):
        m = AL.load_asset(full)
        MEL.delete_all_material_expressions(m)
    else:
        m = AT.create_asset(NAME, MATS, unreal.Material, unreal.MaterialFactoryNew())
    m.set_editor_property('blend_mode', unreal.BlendMode.BLEND_MASKED)
    m.set_editor_property('two_sided', True)
    m.set_editor_property('opacity_mask_clip_value', 0.4)
    default_frame = AL.load_asset('/Game/Brawl/Sprites/Nova/T_nova_idle')
    frame = _e(m, unreal.MaterialExpressionTextureSampleParameter2D, -900, 0, parameter_name='Tex', texture=default_frame)
    tex = _e(m, unreal.MaterialExpressionTextureObjectParameter, -2100, 0, parameter_name='Tex', texture=default_frame)
    uv = _e(m, unreal.MaterialExpressionTextureCoordinate, -2100, 160)
    key = _e(m, unreal.MaterialExpressionVectorParameter, -2100, 300, parameter_name='EnemyKey',
             default_value=unreal.LinearColor(0.25, 0.46, 0.75, 1))
    rim = _e(m, unreal.MaterialExpressionScalarParameter, -2100, 460, parameter_name='EnemyRim', default_value=0.75)
    inputs = [('Frame', tex), ('UV', uv), ('Key', key), ('Strength', rim)]
    y = 600
    for i in range(4):
        for letter, default in (('A', unreal.LinearColor(0, 0, 0, 0)), ('B', unreal.LinearColor(0, 0, 0, 0)), ('C', unreal.LinearColor(1, 0, 1, 0))):
            p = _e(m, unreal.MaterialExpressionVectorParameter, -2100, y, parameter_name='Var%s%d' % (letter, i), default_value=default)
            # a vector parameter's plain output is RGB: append its alpha to get all four numbers
            ap = _e(m, unreal.MaterialExpressionAppendVector, -1850, y)
            MEL.connect_material_expressions(p, '', ap, 'A')
            MEL.connect_material_expressions(p, 'A', ap, 'B')
            inputs.append(('%s%d' % (letter, i), ap))
            y += 150
    paint = _custom(m, -1400, 0, 'Colour tables, ink separation and coloured edge light inside the painted alpha', PAINT, inputs)
    tint = _e(m, unreal.MaterialExpressionVectorParameter, -900, 300, parameter_name='Tint', default_value=unreal.LinearColor(1, 1, 1, 1))
    col = _e(m, unreal.MaterialExpressionMultiply, -600, 0)
    MEL.connect_material_expressions(paint, '', col, 'A')
    MEL.connect_material_expressions(tint, '', col, 'B')
    MEL.connect_material_property(col, '', unreal.MaterialProperty.MP_BASE_COLOR)
    MEL.connect_material_property(frame, 'A', unreal.MaterialProperty.MP_OPACITY_MASK)
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
    rage = _e(m, unreal.MaterialExpressionScalarParameter, -900, 700, parameter_name='Rage', default_value=0.0)
    rcol = _e(m, unreal.MaterialExpressionVectorParameter, -900, 800, parameter_name='RageColor', default_value=unreal.LinearColor(1.0, 0.25, 0.08, 1))
    glow = _custom(m, -600, 700, 'Second phase: the outline burns', GLOW, [('Frame', tex), ('UV', uv), ('Color', rcol), ('Amount', rage)])
    add2 = _e(m, unreal.MaterialExpressionAdd, 0, 300)
    MEL.connect_material_expressions(add, '', add2, 'A')
    MEL.connect_material_expressions(glow, '', add2, 'B')
    MEL.connect_material_property(add2, '', unreal.MaterialProperty.MP_EMISSIVE_COLOR)
    r = _e(m, unreal.MaterialExpressionConstant, -200, 450, r=0.8)
    MEL.connect_material_property(r, '', unreal.MaterialProperty.MP_ROUGHNESS)
    s = _e(m, unreal.MaterialExpressionConstant, -200, 550, r=0.15)
    MEL.connect_material_property(s, '', unreal.MaterialProperty.MP_SPECULAR)
    MEL.layout_material_expressions(m)
    MEL.recompile_material(m)
    ok = AL.save_loaded_asset(m)
    unreal.log('FTBRAWL VARIETY material=%s saved=%s' % (full, ok))
    return m


build()
