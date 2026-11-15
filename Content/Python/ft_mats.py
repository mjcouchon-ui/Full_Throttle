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
    """Make the material, or clear an existing one's graph and reuse it: deleting it would orphan the
    palette instances parented to it and drop the usage flags the cooked game needs (2.2)."""
    _dir(path)
    full = path + '/' + name
    if AL.does_asset_exist(full):
        m = AL.load_asset(full)
        MEL.delete_all_material_expressions(m)
    else:
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
    # 2.2 cel look: the post-process cel pass reads light as scene colour / base colour, so keep paint mostly
    # diffuse (the 1.0 toon art had no metal at all); MetalScale and RoughMin are the knobs.
    mscale = sparam(m, 'MetalScale', 0.25, y=600)
    rmin = sparam(m, 'RoughMin', 0.4, y=700)
    em = custom(m, "return Col * Glow * (Gain + Alert * Gain * 2.5);", [('Col', col, ''), ('Glow', glow, ''), ('Alert', alert, ''), ('Gain', gain, '')], y=0, desc='Glow')
    rr = custom(m, "return saturate(max(Rough, RoughMin) + Soot * 0.4);", [('Rough', rough, ''), ('Soot', soot, ''), ('RoughMin', rmin, '')], out_type=unreal.CustomMaterialOutputType.CMOT_FLOAT1, y=200, desc='Rough')
    mm = custom(m, "return Metal * MetalScale;", [('Metal', metal, ''), ('MetalScale', mscale, '')], out_type=unreal.CustomMaterialOutputType.CMOT_FLOAT1, y=100, desc='Metal')
    MEL.connect_material_property(base, '', unreal.MaterialProperty.MP_BASE_COLOR)
    MEL.connect_material_property(em, '', unreal.MaterialProperty.MP_EMISSIVE_COLOR)
    MEL.connect_material_property(mm, '', unreal.MaterialProperty.MP_METALLIC)
    MEL.connect_material_property(rr, '', unreal.MaterialProperty.MP_ROUGHNESS)
    return finish(m)


# --------------------------------------------------------------------------------------------- road
ROAD_HLSL = """
float u = UV.x; float v = UV.y;
float x = (u - 0.5) * 2742.0;
float ax = abs(x);
float2 g = floor(float2(u * 2600.0, v * 2600.0));
float n = frac(sin(dot(g, float2(12.9898, 78.233))) * 43758.5453);
float3 c = lerp(RoadA, Pastel, Lift) * (1.0 - Grain + 2.0 * Grain * n);
// Worn wheel lines, seam repairs and shoulder grit sit underneath all track markings.
float wheelWear = 1.0 - smoothstep(48.0, 132.0, abs(abs(ax - 290.0) - 49.0));
float patch = smoothstep(0.64, 0.86, frac(sin(floor(v*2.7)*40.31 + floor(u*9.0)*8.91) * 17543.11));
float joint = 1.0 - smoothstep(0.006, 0.025, abs(frac(v * 2.7) - 0.23));
float edgeDirt = smoothstep(945.0, 1140.0, ax) * (0.65 + n * 0.35);
c *= 1.0 - wheelWear * 0.074 - patch * 0.088 - joint * 0.044 - 0.11 * edgeDirt;
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
    # 2.2: the C++ track sets RoadA per level; a level's MI_Road_<Level> can lift it toward a pastel tone
    # (Bayshore's 1.0 grey-violet) and calm the asphalt noise, which turns to speckle under cel bands.
    pastel = vparam(m, 'RoadPastel', (0.11, 0.1, 0.17, 1), y=500)
    lift = sparam(m, 'RoadLift', 0.0, y=600)
    grain = sparam(m, 'Grain', 0.18, y=700)
    col = custom(m, ROAD_HLSL, [('UV', uv, ''), ('VC', vc, ''), ('RoadA', ra, ''), ('Line', ln, ''), ('KerbA', ka, ''), ('KerbB', kb, ''), ('Kicker', ki, ''),
                                ('Pastel', pastel, ''), ('Lift', lift, ''), ('Grain', grain, '')], desc='Road')
    rough = custom(m, """
float x = abs((UV.x - 0.5) * 2742.0);
float2 g = floor(float2(UV.x * 120.0, UV.y * 60.0));
float puddle = frac(sin(dot(g, float2(41.31, 17.77))) * 9631.13);
return lerp(Wet + Puddles * puddle, 0.55, step(1172.0, x));
""", [('UV', uv, ''), ('Wet', wet, ''), ('Puddles', sparam(m, 'Puddles', 0.25, y=800), '')], out_type=unreal.CustomMaterialOutputType.CMOT_FLOAT1, y=200, desc='Wet')
    MEL.connect_material_property(col, '', unreal.MaterialProperty.MP_BASE_COLOR)
    MEL.connect_material_property(rough, '', unreal.MaterialProperty.MP_ROUGHNESS)
    return finish(m)


def simple_lit(name, color, rough=0.5, metal=0.0, path=CORE, vertex_color=False):
    m = new_material(path, name)
    if vertex_color:
        vc = expr(m, unreal.MaterialExpressionVertexColor)
        wp = expr(m, unreal.MaterialExpressionWorldPosition, y=200)
        c = custom(m, """
float2 p = WP.xy * 0.0013;
float ridges = sin(p.x + 1.2 * sin(p.y*1.4)) * cos(p.y*0.95 + p.x*0.33);
float washes = sin(p.x*0.42 - p.y*0.65) * sin(p.x*0.8 + p.y*0.38);
float layers = floor((ridges*0.58 + washes*0.42 + 1.0)*2.0) * 0.25;
float2 gritCell = floor(WP.xy * 0.036);
float grit = frac(dot(frac(gritCell*float2(0.1273,0.3129)),float2(7.13,8.63)));
float blur = saturate(length(fwidth(WP.xy))*0.023);
return VC.rgb * (0.86 + 0.26*layers + lerp(grit - 0.5,0.0,blur)*0.09);
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
col += step(0.9978, s) * saturate(h * 3.0) * 2.5 * tw * Stars;
float3 md3 = normalize(MoonDir.xyz);
float md = dot(d, md3);
// 2.4: the disc is drawn like the 1.x canvas sun and moon (render.js drawSky / drawNight), in its own flat
// frame (q in disc radii, q.y up): a soft glow, slow rays, a pale halo ring, a pale upper half and a deeper
// lower half below a chord, and four bands cut out of the lower half, each thicker than the last.
float front = step(0.0, md);
float3 upv = normalize(float3(0, 0, 1) - md3 * md3.z);
float3 rtv = cross(upv, md3);
float rad = sqrt(max(1.0 - DiscSize * DiscSize, 1e-6));
float2 q = float2(dot(d, rtv), dot(d, upv)) / rad;
float rr = length(q);
float aa = max(fwidth(rr), 1e-4);
float cut = 0.0;
for (int b = 0; b < 4; b++)
{
    float top = -(0.22 + b * 0.2);
    float bot = top - (0.05 + b * 0.025);
    cut = max(cut, smoothstep(bot - aa, bot + aa, q.y) * (1.0 - smoothstep(top - aa, top + aa, q.y)));
}
cut *= Stripes;
float disc = (1.0 - smoothstep(1.0 - aa, 1.0 + aa, rr)) * front * (1.0 - cut);
float halo = (1.0 - smoothstep(1.45 - aa, 1.45 + aa, rr)) * front * (1.0 - cut * step(rr, 1.0)) * Halo;
float ang = atan2(q.y, q.x) - T * 0.05;
float ray = step(frac(ang / 0.5236), 0.172) * (1.0 - smoothstep(4.0, 9.0, rr)) * front * Rays;
col += HaloCol.rgb * Glow * pow(saturate(1.0 - rr / 4.2), 2.0) * front;
col = lerp(col, HaloCol.rgb, ray);
col = lerp(col, HaloCol.rgb, halo);
float3 dcol = lerp(DiscCol2.rgb, DiscCol.rgb, smoothstep(-0.149 - aa, -0.149 + aa, q.y));
col = lerp(col, dcol, disc);
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
    # 2.2: Stars 0 for a day sky; the disc is the moon at night or the 1.0 striped 80s sun by day
    st = sparam(m, 'Stars', 1.0, y=800)
    ds = sparam(m, 'DiscSize', 0.99955, y=900)
    dc = vparam(m, 'DiscCol', (6.0, 5.64, 4.92, 1), y=1000)
    sp = sparam(m, 'Stripes', 0.0, y=1100)
    # 2.4: the 1.x two-tone banded disc with halo, glow and rays (DiscCol upper half, DiscCol2 lower half)
    dc2 = vparam(m, 'DiscCol2', (6.0, 5.64, 4.92, 1), y=1200)
    hc = vparam(m, 'HaloCol', (1.0, 0.9, 0.6, 1), y=1300)
    ha = sparam(m, 'Halo', 0.0, y=1400)
    gl = sparam(m, 'Glow', 0.2, y=1500)
    ry = sparam(m, 'Rays', 0.0, y=1600)
    c = custom(m, SKY_HLSL, [('WP', wp, ''), ('CP', cp, ''), ('T', t, ''), ('Sky0', s0, ''), ('Sky1', s1, ''), ('Sky2', s2, ''), ('MoonDir', md, ''), ('Gain', g, ''),
                             ('Stars', st, ''), ('DiscSize', ds, ''), ('DiscCol', dc, ''), ('Stripes', sp, ''), ('DiscCol2', dc2, ''),
                             ('HaloCol', hc, ''), ('Halo', ha, ''), ('Glow', gl, ''), ('Rays', ry, '')], desc='Sky')
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


HAZARD_HLSL = """
// The marker is a plane the game scales to the attack's real area (a lane, a square mark, a wave across the road).
// Size is that area in cm (AFTHazard::SetStyle), so the rails and patterns keep their scale on any of them.
float2 c = (UV - 0.5) * 2.0;
float2 d = abs(c);
float2 hs = max(Size.xy * 0.5, 1.0);
float2 p = c * hs;
float2 inset = hs - abs(p);
float em = min(inset.x, inset.y);
float rw = max(18.0, 0.07 * min(hs.x, hs.y));
// the rail sits on the real edge: what is inside it is what gets hit
float rail = smoothstep(rw * 0.25, rw * 0.5, em) * (1.0 - smoothstep(rw * 1.25, rw * 1.5, em));
float u = p.x * 0.01;
float v = p.y * 0.01;
// the warning closes in: across a lane or a wave from both rails to the middle, on a mark from the rim to the centre.
// Progress is the real time left (0 at the warning's start, 1 when it goes live), so the gap IS the countdown
float prog = saturate(Progress);
float dd = Mark > 0.5 ? max(d.x, d.y) : d.y;
float aw = max(fwidth(dd) * 1.5, 0.004);
float fill = lerp(smoothstep(1.0 - prog - aw, 1.0 - prog + aw, dd), 1.0, saturate(Live));
float front = (1.0 - smoothstep(0.0, 0.05, abs(dd - (1.0 - prog)))) * (1.0 - saturate(Live)) * step(0.02, prog);
float pat = 0.0;
if (Style < 0.5)
{
    // electric: jagged arcs that jump
    float tq = floor(T * 14.0);
    float z1 = v * 1.4 + 0.5 * sin(u * 2.1 + tq * 1.7) + 0.28 * sin(u * 5.3 - tq * 2.9);
    float z2 = v * 0.9 - 0.4 * sin(u * 3.3 - tq * 2.3) + 0.3;
    pat = max(1.0 - smoothstep(0.03, 0.10, abs(frac(z1) - 0.5)), 0.6 * (1.0 - smoothstep(0.02, 0.07, abs(frac(z2) - 0.5))));
}
else if (Style < 1.5)
{
    // ice: a lattice of facets with a glint running through it
    float g1 = abs(frac(u * 0.9 + v * 0.7) - 0.5);
    float g2 = abs(frac(u * 0.9 - v * 0.7) - 0.5);
    float glint = smoothstep(0.75, 1.0, sin(u * 1.7 + v * 2.1 - T * 5.0));
    pat = (1.0 - smoothstep(0.02, 0.07, min(g1, g2))) * 0.9 + 0.3 * glint;
}
else if (Style < 2.5)
{
    // fire: tongues licking along
    float f = sin(u * 3.1 - T * 9.0 + 2.2 * sin(v * 2.3 + T * 4.0)) + 0.6 * sin(u * 7.3 - T * 13.0 + v * 3.0);
    pat = smoothstep(0.25, 0.45, f) * 0.9;
}
else
{
    // water: ripples
    float rr = Mark > 0.5 ? length(p) * 0.01 : abs(v) + 0.25 * sin(u * 1.3 + T * 2.0);
    pat = smoothstep(0.55, 0.85, sin(rr * 5.0 - T * 7.0)) * 0.8;
}
float chev = smoothstep(0.52, 0.67, frac(u * 0.7 + d.y * 0.9 - T * 0.85));
// levels: the inside is as bright as the old marker's was (0.065 base, 0.285 on its stripes), with the pattern drawn
// in that range. Brighter (0.5 to 1.3 live in the first build, then 0.14 to 0.59) a whole lane or mark went flat
// white on screen under the game's live intensity: a flash the size of a lane, and no pattern left to read
float warn = 0.05 + 0.14 * chev * (1.0 - fill) + fill * (0.065 + 0.20 * pat) + 0.5 * front;
float live = 0.065 + 0.22 * pat;
return Col * Gain * (lerp(warn, live, saturate(Live)) + 0.9 * rail);
"""


def hazard():
    """The road marker of every telegraphed attack. Colour and Intensity as before (AFTHazard::SetLook); 3.1 adds the
    attack's Style (0 electric, 1 ice, 2 fire, 3 water), its real countdown (Progress, Live) and its shape (Mark, Size),
    all set each frame by AFTHazard::SetStyle. Left at their defaults it is a full, lit electric marker."""
    m = new_material(FX, 'M_Hazard', blend=unreal.BlendMode.BLEND_ADDITIVE, shading=unreal.MaterialShadingModel.MSM_UNLIT, two_sided=True)
    uv = expr(m, unreal.MaterialExpressionTextureCoordinate)
    t = expr(m, unreal.MaterialExpressionTime, y=100)
    c = vparam(m, 'Color', (1, 0.2, 0.2, 1), y=200)
    g = sparam(m, 'Intensity', 2.0, y=300)
    ins = [('UV', uv, ''), ('T', t, ''), ('Col', c, ''), ('Gain', g, '')]
    y = 400
    for nm, val in (('Style', 0.0), ('Progress', 1.0), ('Live', 0.0), ('Mark', 0.0)):
        ins.append((nm, sparam(m, nm, val, y=y), ''))
        y += 100
    ins.append(('Size', vparam(m, 'Size', (1000.0, 600.0, 0.0, 0.0), y=y), ''))
    e = custom(m, HAZARD_HLSL, ins, desc='Hazard')
    MEL.connect_material_property(e, '', unreal.MaterialProperty.MP_EMISSIVE_COLOR)
    return finish(m)


# --------------------------------------------------------------------------------------------- particles
FIRE_HLSL = """
float2 q = UV - 0.5;
float r = length(q) * 2.0;
float a = atan2(q.y, q.x);
float t = T * 1.7 + Rand * 31.0;
float lobe = sin(a*5.0 + t*1.9) * 0.46 + sin(a*8.0 - t*2.2) * 0.30 + sin(r*12.0 - t*3.5)*0.24;
float qRadius = r - lobe * (0.10 + 0.06*r);
float aa = max(0.018, fwidth(qRadius));
float rim = 1.0 - smoothstep(0.91-aa,0.98+aa,qRadius);
float body = 1.0 - smoothstep(0.61-aa,0.69+aa,qRadius);
float core = 1.0 - smoothstep(0.29-aa,0.38+aa,qRadius);
float3 c = PC.rgb * (0.26*rim + 0.55*body + 0.61*core);
return c * PC.a * Gain;
"""


def fx_materials():
    # fire: additive, temperature colour from the particle, ragged animated edge
    m = new_material(FX, 'M_FX_Fire', blend=unreal.BlendMode.BLEND_ADDITIVE, shading=unreal.MaterialShadingModel.MSM_UNLIT, two_sided=True)
    uv = expr(m, unreal.MaterialExpressionTextureCoordinate)
    pc = expr(m, unreal.MaterialExpressionParticleColor, y=100)
    t = expr(m, unreal.MaterialExpressionTime, y=200)
    rnd = expr(m, unreal.MaterialExpressionParticleRandom, y=300)
    g = sparam(m, 'Intensity', 6.8, y=400)
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
    g = sparam(m, 'Intensity', 22.0, y=200)
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
    g = sparam(m, 'Intensity', 7.0, y=200)
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
