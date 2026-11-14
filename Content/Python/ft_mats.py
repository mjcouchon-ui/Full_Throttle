"""FULL THROTTLE (Unreal) master materials, built as real material graphs in the editor.

Run over the Unreal MCP connector:  import ft_mats; ft_mats.build_all()
Lit PBR for the world (Lumen lights it), emissive neon, a procedural road, a night sky, particle materials.
"""
import unreal

AL = unreal.EditorAssetLibrary
MEL = unreal.MaterialEditingLibrary
AT = unreal.AssetToolsHelpers.get_asset_tools()
CORE = '/Game/Core/Materials'
FX = '/Game/FX'


def _dir(p):
    if not AL.does_directory_exist(p):
        AL.make_directory(p)


def new_material(path, name, blend=None, shading=None, two_sided=False):
    _dir(path)
    full = path + '/' + name
    if AL.does_asset_exist(full):
        AL.delete_asset(full)
    m = AT.create_asset(name, path, unreal.Material, unreal.MaterialFactoryNew())
    if blend is not None:
        m.set_editor_property('blend_mode', blend)
    if shading is not None:
        m.set_editor_property('shading_model', shading)
    m.set_editor_property('two_sided', two_sided)
    return m


def vparam(m, name, val, x=-900, y=0):
    e = MEL.create_material_expression(m, unreal.MaterialExpressionVectorParameter, x, y)
    e.set_editor_property('parameter_name', name)
    e.set_editor_property('default_value', unreal.LinearColor(*val))
    return e


def sparam(m, name, val, x=-900, y=0):
    e = MEL.create_material_expression(m, unreal.MaterialExpressionScalarParameter, x, y)
    e.set_editor_property('parameter_name', name)
    e.set_editor_property('default_value', val)
    return e


def custom(m, code, inputs, out_type=unreal.CustomMaterialOutputType.CMOT_FLOAT3, x=-400, y=0, desc='FT'):
    """inputs: list of (name, expression, output_name)."""
    cu = MEL.create_material_expression(m, unreal.MaterialExpressionCustom, x, y)
    cu.set_editor_property('code', code)
    cu.set_editor_property('output_type', out_type)
    cu.set_editor_property('description', desc)
    ins = []
    for nm, _, _ in inputs:
        ci = unreal.CustomInput()
        ci.set_editor_property('input_name', nm)
        ins.append(ci)
    cu.set_editor_property('inputs', ins)
    for nm, ex, outn in inputs:
        MEL.connect_material_expressions(ex, outn, cu, nm)
    return cu


def expr(m, cls, x=-900, y=0):
    return MEL.create_material_expression(m, cls, x, y)


def finish(m):
    MEL.layout_material_expressions(m)
    MEL.recompile_material(m)
    AL.save_loaded_asset(m)
    return m


def mi(parent, path, name, vectors=None, scalars=None):
    _dir(path)
    full = path + '/' + name
    if AL.does_asset_exist(full):
        inst = AL.load_asset(full)
    else:
        inst = AT.create_asset(name, path, unreal.MaterialInstanceConstant, unreal.MaterialInstanceConstantFactoryNew())
    inst.set_editor_property('parent', parent)
    for k, v in (vectors or {}).items():
        MEL.set_material_instance_vector_parameter_value(inst, k, unreal.LinearColor(*v))
    for k, v in (scalars or {}).items():
        MEL.set_material_instance_scalar_parameter_value(inst, k, v)
    MEL.update_material_instance(inst)
    AL.save_loaded_asset(inst)
    return inst


# --------------------------------------------------------------------------------------------- paint
def paint():
    """The vehicles' and props' master: colour, metal, roughness, neon glow, soot (damage) and alert flare."""
    m = new_material(CORE, 'M_Paint')
    col = vparam(m, 'Color', (0.8, 0.1, 0.1, 1), y=-200)
    glow = sparam(m, 'Glow', 0.0, y=-100)
    metal = sparam(m, 'Metal', 0.0, y=0)
    rough = sparam(m, 'Rough', 0.45, y=100)
    soot = sparam(m, 'Soot', 0.0, y=200)
    alert = sparam(m, 'Alert', 0.0, y=300)
    gain = sparam(m, 'GlowGain', 14.0, y=400)
    wp = expr(m, unreal.MaterialExpressionWorldPosition, y=500)
    base = custom(m, """
float3 p = WP * 0.02;
float n = frac(sin(dot(floor(p), float3(12.9898, 78.233, 37.719))) * 43758.5453);
float s = saturate(Soot * (0.75 + 0.5 * n));
return lerp(Col, float3(0.025, 0.02, 0.022), s);
""", [('Col', col, ''), ('Soot', soot, ''), ('WP', wp, '')], y=-200, desc='Soot')
    em = custom(m, "return Col * Glow * (Gain + Alert * Gain * 2.5);", [('Col', col, ''), ('Glow', glow, ''), ('Alert', alert, ''), ('Gain', gain, '')], y=0, desc='Glow')
    rr = custom(m, "return saturate(Rough + Soot * 0.4);", [('Rough', rough, ''), ('Soot', soot, '')], out_type=unreal.CustomMaterialOutputType.CMOT_FLOAT1, y=200, desc='Rough')
    MEL.connect_material_property(base, '', unreal.MaterialProperty.MP_BASE_COLOR)
    MEL.connect_material_property(em, '', unreal.MaterialProperty.MP_EMISSIVE_COLOR)
    MEL.connect_material_property(metal, '', unreal.MaterialProperty.MP_METALLIC)
    MEL.connect_material_property(rr, '', unreal.MaterialProperty.MP_ROUGHNESS)
    return finish(m)


# --------------------------------------------------------------------------------------------- road
ROAD_HLSL = """
float u = UV.x; float v = UV.y;
float x = (u - 0.5) * 2742.0;
float ax = abs(x);
float2 g = floor(float2(u * 2600.0, v * 2600.0));
float n = frac(sin(dot(g, float2(12.9898, 78.233))) * 43758.5453);
float3 c = RoadA * (0.82 + 0.36 * n);
float lw = 8.0;
float dash = step(frac(v * 1.1), 0.36);
float lines = max(step(abs(ax - 585.0), lw) * dash, step(ax, lw) * dash);
lines = max(lines, step(abs(ax - 1150.0), lw * 1.5));
c = lerp(c, Line, lines);
float kerb = step(1172.0, ax);
float stripe = step(0.5, frac(v * 5.0));
c = lerp(c, lerp(KerbA, KerbB, stripe), kerb);
float warn = VC.x * step(880.0, ax) * (1.0 - kerb);
float ws = step(0.5, frac(v * 10.0 + x / 160.0));
c = lerp(c, lerp(float3(1.0, 0.75, 0.12), float3(0.05, 0.04, 0.1), ws), warn);
float chev = step(0.5, frac(v * 6.0 + ax / 700.0));
c = lerp(c, lerp(Kicker, Kicker * 0.25, chev), VC.y * (1.0 - kerb));
return c;
"""


def road():
    m = new_material(CORE, 'M_Road')
    uv = expr(m, unreal.MaterialExpressionTextureCoordinate, y=-300)
    vc = expr(m, unreal.MaterialExpressionVertexColor, y=-200)
    ra = vparam(m, 'RoadA', (0.03, 0.028, 0.06, 1), y=-100)
    ln = vparam(m, 'Line', (0.95, 0.93, 0.85, 1), y=0)
    ka = vparam(m, 'KerbA', (1.0, 0.1, 0.5, 1), y=100)
    kb = vparam(m, 'KerbB', (0.95, 0.93, 0.85, 1), y=200)
    ki = vparam(m, 'Kicker', (1.0, 0.1, 0.5, 1), y=300)
    wet = sparam(m, 'Wet', 0.28, y=400)
    col = custom(m, ROAD_HLSL, [('UV', uv, ''), ('VC', vc, ''), ('RoadA', ra, ''), ('Line', ln, ''), ('KerbA', ka, ''), ('KerbB', kb, ''), ('Kicker', ki, '')], desc='Road')
    rough = custom(m, """
float x = abs((UV.x - 0.5) * 2742.0);
float2 g = floor(float2(UV.x * 120.0, UV.y * 60.0));
float puddle = frac(sin(dot(g, float2(41.31, 17.77))) * 9631.13);
return lerp(Wet + 0.25 * puddle, 0.55, step(1172.0, x));
""", [('UV', uv, ''), ('Wet', wet, '')], out_type=unreal.CustomMaterialOutputType.CMOT_FLOAT1, y=200, desc='Wet')
    MEL.connect_material_property(col, '', unreal.MaterialProperty.MP_BASE_COLOR)
    MEL.connect_material_property(rough, '', unreal.MaterialProperty.MP_ROUGHNESS)
    return finish(m)


def simple_lit(name, color, rough=0.5, metal=0.0, path=CORE, vertex_color=False):
    m = new_material(path, name)
    if vertex_color:
        vc = expr(m, unreal.MaterialExpressionVertexColor)
        wp = expr(m, unreal.MaterialExpressionWorldPosition, y=200)
        c = custom(m, """
float2 g = floor(WP.xy * 0.004);
float n = frac(sin(dot(g, float2(12.9898, 78.233))) * 43758.5453);
float2 g2 = floor(WP.xy * 0.03);
float n2 = frac(sin(dot(g2, float2(39.3468, 11.135))) * 24634.6345);
return VC.rgb * (0.82 + 0.22 * n + 0.12 * n2);
""", [('VC', vc, ''), ('WP', wp, '')], desc='Ground')
        MEL.connect_material_property(c, '', unreal.MaterialProperty.MP_BASE_COLOR)
    else:
        c = vparam(m, 'Color', color)
        MEL.connect_material_property(c, '', unreal.MaterialProperty.MP_BASE_COLOR)
    r = sparam(m, 'Rough', rough, y=200)
    mt = sparam(m, 'Metal', metal, y=300)
    MEL.connect_material_property(r, '', unreal.MaterialProperty.MP_ROUGHNESS)
    MEL.connect_material_property(mt, '', unreal.MaterialProperty.MP_METALLIC)
    return finish(m)


def emissive_vc(name, gain=30.0, path=CORE):
    """Unlit neon that takes its colour from the vertex colour (the track's barrier strips)."""
    m = new_material(path, name, shading=unreal.MaterialShadingModel.MSM_UNLIT)
    vc = expr(m, unreal.MaterialExpressionVertexColor)
    g = sparam(m, 'Intensity', gain, y=200)
    mul = expr(m, unreal.MaterialExpressionMultiply)
    MEL.connect_material_expressions(vc, '', mul, 'A')
    MEL.connect_material_expressions(g, '', mul, 'B')
    MEL.connect_material_property(mul, '', unreal.MaterialProperty.MP_EMISSIVE_COLOR)
    return finish(m)


def emissive_const(name, color, gain, path=CORE):
    m = new_material(path, name, shading=unreal.MaterialShadingModel.MSM_UNLIT)
    c = vparam(m, 'Color', color)
    g = sparam(m, 'Intensity', gain, y=200)
    mul = expr(m, unreal.MaterialExpressionMultiply)
    MEL.connect_material_expressions(c, '', mul, 'A')
    MEL.connect_material_expressions(g, '', mul, 'B')
    MEL.connect_material_property(mul, '', unreal.MaterialProperty.MP_EMISSIVE_COLOR)
    return finish(m)


def glass():
    """The plant's glass pipes: an additive fresnel shell, so you see the road through it."""
    m = new_material(CORE, 'M_PipeGlass', blend=unreal.BlendMode.BLEND_ADDITIVE, shading=unreal.MaterialShadingModel.MSM_UNLIT, two_sided=True)
    c = vparam(m, 'Color', (0.37, 0.94, 1.0, 1))
    fr = expr(m, unreal.MaterialExpressionFresnel, y=200)
    fr.set_editor_property('exponent', 2.5)
    g = sparam(m, 'Intensity', 1.6, y=300)
    e = custom(m, "return Col * (0.08 + F) * Gain;", [('Col', c, ''), ('F', fr, ''), ('Gain', g, '')], desc='Glass')
    MEL.connect_material_property(e, '', unreal.MaterialProperty.MP_EMISSIVE_COLOR)
    return finish(m)


def water():
    m = new_material(CORE, 'M_Water')
    wp = expr(m, unreal.MaterialExpressionWorldPosition)
    t = expr(m, unreal.MaterialExpressionTime, y=100)
    n = custom(m, """
float2 p = WP.xy * 0.0012;
float a = sin(p.x * 3.1 + T * 0.7) * 0.5 + sin(p.y * 2.3 - T * 0.5) * 0.5 + sin((p.x + p.y) * 5.7 + T * 1.3) * 0.25;
float b = cos(p.x * 2.7 - T * 0.6) * 0.5 + cos(p.y * 3.3 + T * 0.8) * 0.5;
return normalize(float3(a * 0.06, b * 0.06, 1.0));
""", [('WP', wp, ''), ('T', t, '')], desc='Waves')
    c = vparam(m, 'Color', (0.004, 0.01, 0.025, 1), y=300)
    MEL.connect_material_property(c, '', unreal.MaterialProperty.MP_BASE_COLOR)
    MEL.connect_material_property(n, '', unreal.MaterialProperty.MP_NORMAL)
    r = sparam(m, 'Rough', 0.04, y=400)
    MEL.connect_material_property(r, '', unreal.MaterialProperty.MP_ROUGHNESS)
    return finish(m)


SKY_HLSL = """
float3 d = normalize(WP - CP);
float h = d.z;
float3 col = lerp(Sky1, Sky0, saturate(h * 2.2));
col = lerp(Sky2, col, saturate(h * 5.0 + 0.12));
col *= lerp(0.6, 1.0, saturate(h * 10.0 + 1.0));
float3 p = floor(d * 520.0);
float s = frac(sin(dot(p, float3(12.9898, 78.233, 37.719))) * 43758.5453);
float tw = 0.6 + 0.4 * sin(T * 3.0 + s * 40.0);
col += step(0.9978, s) * saturate(h * 3.0) * 2.5 * tw;
float md = dot(d, normalize(MoonDir));
col += saturate((md - 0.99955) * 6000.0) * float3(1.0, 0.94, 0.82) * 6.0;
col += pow(saturate(md), 600.0) * float3(0.6, 0.55, 0.8) * 1.2;
return col * Gain;
"""


def sky():
    m = new_material(CORE, 'M_Sky', shading=unreal.MaterialShadingModel.MSM_UNLIT, two_sided=True)
    try:
        m.set_editor_property('is_sky', True)
    except Exception:
        pass
    wp = expr(m, unreal.MaterialExpressionWorldPosition)
    cp = expr(m, unreal.MaterialExpressionCameraPositionWS, y=100)
    t = expr(m, unreal.MaterialExpressionTime, y=200)
    s0 = vparam(m, 'Sky0', (0.004, 0.003, 0.02, 1), y=300)
    s1 = vparam(m, 'Sky1', (0.04, 0.01, 0.12, 1), y=400)
    s2 = vparam(m, 'Sky2', (0.9, 0.12, 0.4, 1), y=500)
    md = vparam(m, 'MoonDir', (0.4, -0.6, 0.35, 0), y=600)
    g = sparam(m, 'Gain', 1.0, y=700)
    c = custom(m, SKY_HLSL, [('WP', wp, ''), ('CP', cp, ''), ('T', t, ''), ('Sky0', s0, ''), ('Sky1', s1, ''), ('Sky2', s2, ''), ('MoonDir', md, ''), ('Gain', g, '')], desc='Sky')
    MEL.connect_material_property(c, '', unreal.MaterialProperty.MP_EMISSIVE_COLOR)
    return finish(m)


def windows():
    """Lit windows for the skylines: a grid of warm/cool panes, some dark, glowing at night."""
    m = new_material(CORE, 'M_Windows')
    uv = expr(m, unreal.MaterialExpressionWorldPosition)
    tint = vparam(m, 'Color', (0.08, 0.06, 0.12, 1), y=100)
    lit = vparam(m, 'Lit', (1.0, 0.75, 0.45, 1), y=200)
    gain = sparam(m, 'Intensity', 6.0, y=300)
    e = custom(m, """
float2 q = float2(WP.x + WP.y, WP.z);
float2 cell = floor(q / float2(260.0, 330.0));
float2 f = frac(q / float2(260.0, 330.0));
float on = step(0.45, frac(sin(dot(cell, float2(12.9898, 78.233))) * 43758.5453));
float pane = step(0.18, f.x) * step(f.x, 0.82) * step(0.25, f.y) * step(f.y, 0.8);
float warm = frac(sin(dot(cell, float2(3.1, 9.7))) * 1234.5);
float3 c = lerp(Lit, Lit.bgr * 0.9, step(0.7, warm));
return c * on * pane * Gain;
""", [('WP', uv, ''), ('Lit', lit, ''), ('Gain', gain, '')], desc='Windows')
    MEL.connect_material_property(tint, '', unreal.MaterialProperty.MP_BASE_COLOR)
    MEL.connect_material_property(e, '', unreal.MaterialProperty.MP_EMISSIVE_COLOR)
    r = sparam(m, 'Rough', 0.35, y=400)
    MEL.connect_material_property(r, '', unreal.MaterialProperty.MP_ROUGHNESS)
    return finish(m)


def hazard():
    m = new_material(FX, 'M_Hazard', blend=unreal.BlendMode.BLEND_ADDITIVE, shading=unreal.MaterialShadingModel.MSM_UNLIT, two_sided=True)
    uv = expr(m, unreal.MaterialExpressionTextureCoordinate)
    t = expr(m, unreal.MaterialExpressionTime, y=100)
    c = vparam(m, 'Color', (1, 0.2, 0.2, 1), y=200)
    g = sparam(m, 'Intensity', 2.0, y=300)
    e = custom(m, """
float2 d = abs(UV - 0.5) * 2.0;
float edge = saturate(max(d.x, d.y) * 6.0 - 4.8);
float stripes = step(0.5, frac((UV.x + UV.y) * 6.0 - T * 1.5));
return Col * Gain * (0.25 + 0.35 * stripes + 1.5 * edge);
""", [('UV', uv, ''), ('T', t, ''), ('Col', c, ''), ('Gain', g, '')], desc='Hazard')
    MEL.connect_material_property(e, '', unreal.MaterialProperty.MP_EMISSIVE_COLOR)
    return finish(m)


# --------------------------------------------------------------------------------------------- particles
FIRE_HLSL = """
float2 q = UV - 0.5;
float r = length(q) * 2.0;
float a = atan2(q.y, q.x);
float t = T * 1.7 + Rand * 31.0;
float n = sin(a * 5.0 + t * 2.1) * 0.5 + sin(a * 9.0 - t * 3.3) * 0.3 + sin(r * 11.0 - t * 4.0) * 0.2;
float edge = 1.0 - smoothstep(0.55 + 0.18 * n, 1.0, r);
float core = 1.0 - smoothstep(0.0, 0.55, r);
float3 c = PC.rgb * (edge * 0.9 + core * 0.8);
return c * PC.a * Gain;
"""


def fx_materials():
    # fire: additive, temperature colour from the particle, ragged animated edge
    m = new_material(FX, 'M_FX_Fire', blend=unreal.BlendMode.BLEND_ADDITIVE, shading=unreal.MaterialShadingModel.MSM_UNLIT, two_sided=True)
    uv = expr(m, unreal.MaterialExpressionTextureCoordinate)
    pc = expr(m, unreal.MaterialExpressionParticleColor, y=100)
    t = expr(m, unreal.MaterialExpressionTime, y=200)
    rnd = expr(m, unreal.MaterialExpressionParticleRandom, y=300)
    g = sparam(m, 'Intensity', 18.0, y=400)
    e = custom(m, FIRE_HLSL, [('UV', uv, ''), ('PC', pc, 'RGBA'), ('T', t, ''), ('Rand', rnd, ''), ('Gain', g, '')], desc='Fire')
    MEL.connect_material_property(e, '', unreal.MaterialProperty.MP_EMISSIVE_COLOR)
    finish(m)
    # smoke: translucent billows, colour and opacity from the particle
    m = new_material(FX, 'M_FX_Smoke', blend=unreal.BlendMode.BLEND_TRANSLUCENT, shading=unreal.MaterialShadingModel.MSM_UNLIT, two_sided=True)
    uv = expr(m, unreal.MaterialExpressionTextureCoordinate)
    pc = expr(m, unreal.MaterialExpressionParticleColor, y=100)
    t = expr(m, unreal.MaterialExpressionTime, y=200)
    rnd = expr(m, unreal.MaterialExpressionParticleRandom, y=300)
    op = custom(m, """
float2 q = UV - 0.5;
float r = length(q) * 2.0;
float a = atan2(q.y, q.x);
float t = T * 0.6 + Rand * 17.0;
float n = sin(a * 4.0 + t) * 0.5 + sin(a * 7.0 - t * 1.4) * 0.3 + sin(r * 6.0 + t) * 0.2;
return saturate((1.0 - smoothstep(0.35 + 0.2 * n, 1.0, r)) * PC.a);
""", [('UV', uv, ''), ('PC', pc, 'RGBA'), ('T', t, ''), ('Rand', rnd, '')], out_type=unreal.CustomMaterialOutputType.CMOT_FLOAT1, desc='SmokeMask')
    col = custom(m, "return PC.rgb;", [('PC', pc, 'RGBA')], y=150, desc='SmokeCol')
    MEL.connect_material_property(col, '', unreal.MaterialProperty.MP_EMISSIVE_COLOR)
    MEL.connect_material_property(op, '', unreal.MaterialProperty.MP_OPACITY)
    finish(m)
    # sparks and tracers: hot additive streaks
    m = new_material(FX, 'M_FX_Spark', blend=unreal.BlendMode.BLEND_ADDITIVE, shading=unreal.MaterialShadingModel.MSM_UNLIT, two_sided=True)
    uv = expr(m, unreal.MaterialExpressionTextureCoordinate)
    pc = expr(m, unreal.MaterialExpressionParticleColor, y=100)
    g = sparam(m, 'Intensity', 40.0, y=200)
    e = custom(m, """
float2 q = abs(UV - 0.5) * 2.0;
float s = saturate(1.0 - q.x) * saturate(1.0 - q.y * q.y);
return PC.rgb * PC.a * s * s * Gain;
""", [('UV', uv, ''), ('PC', pc, 'RGBA'), ('Gain', g, '')], desc='Spark')
    MEL.connect_material_property(e, '', unreal.MaterialProperty.MP_EMISSIVE_COLOR)
    finish(m)
    # flash/glow sprite
    m = new_material(FX, 'M_FX_Glow', blend=unreal.BlendMode.BLEND_ADDITIVE, shading=unreal.MaterialShadingModel.MSM_UNLIT, two_sided=True)
    uv = expr(m, unreal.MaterialExpressionTextureCoordinate)
    pc = expr(m, unreal.MaterialExpressionParticleColor, y=100)
    g = sparam(m, 'Intensity', 12.0, y=200)
    e = custom(m, """
float r = length(UV - 0.5) * 2.0;
float s = saturate(1.0 - r);
return PC.rgb * PC.a * s * s * s * Gain;
""", [('UV', uv, ''), ('PC', pc, 'RGBA'), ('Gain', g, '')], desc='Glow')
    MEL.connect_material_property(e, '', unreal.MaterialProperty.MP_EMISSIVE_COLOR)
    finish(m)
    # tracer mesh material (static mesh, not particles)
    emissive_const('M_Tracer', (1.0, 0.75, 0.3, 1), 40.0, path=FX)


def build_all():
    paint()
    road()
    mi(AL.load_asset(CORE + '/M_Road'), CORE, 'MI_Road')
    simple_lit('M_Wall', (0.035, 0.03, 0.05, 1), rough=0.35, metal=0.3)
    mi(AL.load_asset(CORE + '/M_Wall'), CORE, 'MI_Wall')
    simple_lit('M_Pillar', (0.09, 0.08, 0.12, 1), rough=0.8)
    mi(AL.load_asset(CORE + '/M_Pillar'), CORE, 'MI_Pillar')
    simple_lit('M_Ground', (1, 1, 1, 1), rough=0.9, vertex_color=True)
    mi(AL.load_asset(CORE + '/M_Ground'), CORE, 'MI_Ground')
    emissive_vc('M_RailNeon', 40.0)
    mi(AL.load_asset(CORE + '/M_RailNeon'), CORE, 'MI_RailNeon')
    glass()
    mi(AL.load_asset(CORE + '/M_PipeGlass'), CORE, 'MI_PipeGlass')
    emissive_const('M_PipeRib', (1.0, 0.82, 0.25, 1), 14.0)
    mi(AL.load_asset(CORE + '/M_PipeRib'), CORE, 'MI_PipeRib')
    water()
    mi(AL.load_asset(CORE + '/M_Water'), CORE, 'MI_Water')
    sky()
    windows()
    hazard()
    mi(AL.load_asset(FX + '/M_Hazard'), FX, 'MI_Hazard')
    fx_materials()
    return 'materials built'
