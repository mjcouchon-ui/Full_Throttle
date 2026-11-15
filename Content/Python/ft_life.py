"""FULL THROTTLE (Unreal) scenery that moves: the living world (3.1, the game-jam pass).

Looks only. Nothing made here has collision, deals damage or marks a hazard, and the shared cel pass is untouched:
M_Life is an ordinary lit material, so the cel pass bands and inks it like every other prop.

  M_Life       the master. Paint like the props' own (Color, Glow, Rough, Metal, the visual-finish wear), plus ways of
               moving, each behind a static switch, so a way that is off costs nothing:
                 Sway      bends with height above the base, down a world wind (trees, cacti, kelp, masts)
                 Flutter   a ripple travelling along cloth pinned at one edge (flags, pennants, awnings)
                 Spin      turns about a local axis through a local pivot; SpinSwing > 0 rocks instead (wheels, fans,
                           carousels, rocking pump beams, hanging chairs); the normal turns with it
                 Bob       up and down (or any local direction): a sine, or a drop-ride cycle (BobShape 1)
                 Breathe   swells along the normal (smoke balls, bubbles, canvas)
                 Light     the glow blinks, pulses, runs along an axis (chase lights) or switches cell by cell
                           (windows); with FlowMix the base colour runs too (liquid in a pipe)
                 Plume / Conveyor / Burst   for the new meshes built here with Kit: parts made about the origin and
                           told apart by UV1/UV2, sent up a smoke column, along a line (traffic, gondolas, a coaster
                           train) or out from a shell burst (fireworks)
               MaskMin/MaskMax (a box in the mesh's own space) keep a movement to part of a mesh.
  M_LifeFar    the same, always moving. The track switches vertex movement off past 180 m for roadside props (a frond
               cannot be seen moving from there); a landmark that turns must keep turning, so it takes this one.
  animate()    puts M_Life on an existing prop's material slots, keeping each slot's colour and glow. The mesh is
               saved in place: nothing is rebuilt, deleted or re-referenced. Run again after ft_export re-exports a
               prop (the export puts the palette instances back).
  Kit          builds a new animated prop and saves it with M_Life instances already on it.
  air          the air round the camera (FTAmbient.cpp): one mesh of 600 tiny cards and M_Air, which keeps every
               card where it is in the world and wraps it into a box round the view, so the car drives through the
               dust, snow, embers or bubbles instead of carrying them along. One instance per level, MI_Air_L0..7.

Run in the editor:  import ft_life; ft_life.build_all()     (masters, air, new props, then every animate() call)
The per-level lists are at the bottom (LEVELS); the placing rules for the new props are in FTTrackLife.cpp.
"""
import hashlib
import math
import random
import unreal
import ft_art
import ft_mats
from ft_desert import rod, tilt_box

AL = unreal.EditorAssetLibrary
MEL = unreal.MaterialEditingLibrary
AT = unreal.AssetToolsHelpers.get_asset_tools()
GS = unreal.GeometryScript_Primitives
UVS = unreal.GeometryScript_UVs
MQ = unreal.GeometryScript_MeshQueries

LIFE = '/Game/Core/Life'
INST = LIFE + '/Instances'
AIR = '/Game/FX/Ambient'
MODES = ('Sway', 'Flutter', 'Spin', 'Bob', 'Breathe', 'Light', 'Plume', 'Conveyor', 'Burst')
F1 = unreal.CustomMaterialOutputType.CMOT_FLOAT1
F3 = unreal.CustomMaterialOutputType.CMOT_FLOAT3


def log(*a):
    unreal.log('FTLIFE ' + ' '.join(str(x) for x in a))


# ------------------------------------------------------------------------------------------ M_Life
SCALARS = [
    ('Glow', 0.0), ('Glow2', 0.0), ('GlowGain', 10.5), ('Rough', 0.55), ('Metal', 0.1), ('MetalScale', 0.32), ('RoughMin', 0.32), ('Wear', 1.0),
    ('SwayAmp', 30.0), ('SwayBase', 0.0), ('SwayHeight', 600.0), ('SwayPow', 1.6), ('SwayRate', 1.4), ('SwayGust', 1.0), ('SwayLean', 0.15),
    ('FlutterAmp', 14.0), ('FlutterLen', 120.0), ('FlutterRate', 9.0), ('FlutterSpan', 120.0),
    ('SpinRate', 1.0), ('SpinSwing', 0.0), ('SpinRand', 1.0),
    ('BobAmp', 20.0), ('BobRate', 0.3), ('BobRand', 1.0), ('BobShape', 0.0), ('BobDrift', 0.0),
    ('BreatheAmp', 6.0), ('BreatheRate', 1.2), ('BreatheVar', 1.0),
    ('LightRate', 1.0), ('LightDuty', 0.5), ('LightSoft', 0.06), ('LightMin', 0.08), ('LightLen', 1000000.0), ('LightRand', 1.0),
    ('LightHash', 0.0), ('LightCell', 100.0), ('FlowMix', 0.0),
    ('PlumeRate', 0.25), ('PlumeRise', 900.0), ('PlumeEase', 0.8), ('PlumeDrift', 500.0), ('PlumeSpread', 60.0), ('PlumeSize0', 0.5),
    ('PlumeSize1', 2.2), ('PlumeCycle', 0.0), ('PlumeDuty', 1.0),
    ('ConvRate', 0.05), ('ConvLen', 3000.0), ('ConvArc', 0.0), ('ConvRand', 0.0),
    ('BurstRate', 0.2), ('BurstLife', 0.35), ('BurstRadius', 900.0), ('BurstFall', 250.0), ('BurstPhase', 0.0),
]
VECTORS = [
    ('Color', (0.3, 0.3, 0.3, 1.0)), ('Color2', (0.02, 0.02, 0.03, 1.0)),
    ('MaskMin', (-1.0e7, -1.0e7, -1.0e7, 0.0)), ('MaskMax', (1.0e7, 1.0e7, 1.0e7, 0.0)), ('Wind', (1.0, 0.35, 0.0, 0.0)),
    ('FlutterPin', (0.0, 0.0, 0.0, 0.0)), ('FlutterAxis', (0.0, 1.0, 0.0, 0.0)), ('FlutterDir', (1.0, 0.0, 0.0, 0.0)),
    ('FlutterPhase', (0.0, 0.0, 0.0, 0.0)),
    ('SpinPivot', (0.0, 0.0, 0.0, 0.0)), ('SpinAxis', (0.0, 0.0, 1.0, 0.0)),
    ('BobDir', (0.0, 0.0, 1.0, 0.0)), ('BobPhase', (0.0, 0.0, 0.0, 0.0)),
    ('LightAxis', (0.0, 0.0, 1.0, 0.0)), ('LightOffset', (0.0, 0.0, 0.0, 0.0)),
    ('PlumeBase', (0.0, 0.0, 0.0, 0.0)), ('ConvStart', (0.0, 0.0, 0.0, 0.0)), ('ConvDir', (1.0, 0.0, 0.0, 0.0)),
    ('BurstBase', (0.0, 0.0, 0.0, 0.0)),
]
MASK = "float mk = (all(L >= MaskMin.xyz) && all(L <= MaskMax.xyz)) ? 1.0 : 0.0;\n"
HLSL = {
    # world-space offsets
    'Sway': (MASK + r"""
// SwayHeight < 0: the thing hangs from SwayBase (a chair on a cable) and swings more the lower it is
float hang = step(SwayHeight, 0.0);
float hgt = max(lerp(L.z - SwayBase, SwayBase - L.z, hang), 0.0);
float w = pow(saturate(hgt / max(abs(SwayHeight), 1.0)), SwayPow) * mk;
float ph = Rnd * 6.2832 + dot(WP.xy, float2(0.0011, 0.0017)) * SwayGust;
float t = T * SwayRate * (0.85 + 0.3 * Rnd);
float a = sin(t + ph) + 0.4 * sin(t * 2.3 + ph * 1.7) + 0.15 * sin(t * 5.1 + ph * 0.6 + L.z * 0.01);
float2 wd = normalize(Wind.xy + float2(1e-4, 0.0));
float2 cw = float2(-wd.y, wd.x);
float2 o = (wd * (a * 0.6 + SwayLean) + cw * (0.35 * sin(t * 1.3 + ph * 2.1))) * (SwayAmp * w);
// a stem keeps its length: what it gives sideways it loses in height
return float3(o, lerp(-1.0, 1.0, hang) * dot(o, o) / max(2.0 * max(hgt, 30.0), 1.0));
""", ['L', 'WP', 'T', 'Rnd', 'Wind', 'MaskMin', 'MaskMax', 'SwayAmp', 'SwayBase', 'SwayHeight', 'SwayPow', 'SwayRate', 'SwayGust', 'SwayLean']),
    'Breathe': (MASK + r"""
float u = T * BreatheRate + Rnd * 6.2832 + dot(L, float3(0.004, 0.005, 0.006)) * BreatheVar;
return normalize(N) * (BreatheAmp * sin(u) * mk);
""", ['L', 'N', 'T', 'Rnd', 'MaskMin', 'MaskMax', 'BreatheAmp', 'BreatheRate', 'BreatheVar']),
    # offsets in the mesh's own space (turned into the world's afterwards)
    'Flutter': (MASK + r"""
float s = dot(L - FlutterPin.xyz, normalize(FlutterAxis.xyz));
float w = saturate(s / max(FlutterSpan, 1.0)) * mk;
float ph = Rnd * 6.2832;
float k = 6.2832 / max(FlutterLen, 1.0);
float ph2 = ph + dot(L, FlutterPhase.xyz) * 6.2832;      // FlutterPhase: each flag of a row out of step with the next
// Nothing here but s and time: with a very long FlutterLen the cloth is a stiff flap (the offset is a straight line
// in s), which is what a flag built from one box and a stripe laid on it needs, or the stripe leaves the flag
float a = sin(s * k - T * FlutterRate + ph2) + 0.5 * sin(s * k * 1.9 - T * FlutterRate * 1.6 + ph2 * 1.3);
return normalize(FlutterDir.xyz) * (a * FlutterAmp * w);
""", ['L', 'T', 'Rnd', 'MaskMin', 'MaskMax', 'FlutterAmp', 'FlutterLen', 'FlutterRate', 'FlutterSpan', 'FlutterPin', 'FlutterAxis', 'FlutterDir', 'FlutterPhase']),
    'Spin': (MASK + r"""
float3 ax = normalize(SpinAxis.xyz);
float ph = Rnd * 6.2832 * SpinRand;
float ang = (SpinSwing > 0.0) ? radians(SpinSwing) * sin(T * SpinRate + ph) : T * SpinRate + ph;
float3 p = L - SpinPivot.xyz;
float c = cos(ang), s = sin(ang);
float3 r = p * c + cross(ax, p) * s + ax * (dot(ax, p) * (1.0 - c));
return (r - p) * mk;
""", ['L', 'T', 'Rnd', 'MaskMin', 'MaskMax', 'SpinRate', 'SpinSwing', 'SpinRand', 'SpinPivot', 'SpinAxis']),
    'SpinNormal': (MASK + r"""
float3 ax = normalize(SpinAxis.xyz);
float ph = Rnd * 6.2832 * SpinRand;
float ang = ((SpinSwing > 0.0) ? radians(SpinSwing) * sin(T * SpinRate + ph) : T * SpinRate + ph) * mk;
float c = cos(ang), s = sin(ang);
return Nl * c + cross(ax, Nl) * s + ax * (dot(ax, Nl) * (1.0 - c));
""", ['L', 'Nl', 'T', 'Rnd', 'MaskMin', 'MaskMax', 'SpinRate', 'SpinSwing', 'SpinRand', 'SpinAxis']),
    'Bob': (MASK + r"""
float u = T * BobRate + Rnd * BobRand + dot(L, BobPhase.xyz);
float f = frac(u);
// BobShape 1: a drop ride. Up slowly, hold at the top, drop, wait at the bottom
float ride = smoothstep(0.0, 0.5, f) * (1.0 - smoothstep(0.62, 0.7, f));
float a = (BobShape < 0.5) ? sin(u * 6.2832) : ride;
return normalize(BobDir.xyz) * (BobAmp * a * mk) + float3(sin(u * 4.1 + 1.3), cos(u * 3.3), 0.0) * (BobDrift * mk);
""", ['L', 'T', 'Rnd', 'MaskMin', 'MaskMax', 'BobAmp', 'BobRate', 'BobRand', 'BobShape', 'BobDrift', 'BobDir', 'BobPhase']),
    'Plume': (r"""
// one puff of a smoke column. UV1.x = where in the cycle it starts, UV1.y = its size, UV2.x = its own drift
float u = frac(T * PlumeRate + UV1.x + Rnd);
float size = lerp(PlumeSize0, PlumeSize1, pow(u, 0.7)) * UV1.y * sin(min(u * 7.0, 1.5708)) * (1.0 - smoothstep(0.72, 1.0, u));
// timed venting: only the puffs born while the vent was open exist (and they live their lives out)
float born = T - u / max(PlumeRate, 1e-3);
float open = (PlumeCycle > 0.0) ? step(frac(born * PlumeCycle + Rnd * 3.7), PlumeDuty) : 1.0;
float3 c = float3(0.0, 0.0, PlumeRise * pow(u, PlumeEase)) + WindL * (PlumeDrift * u * u)
         + float3(sin(UV2.x * 40.0 + u * 5.0), cos(UV2.x * 31.0 + u * 4.0), 0.0) * (PlumeSpread * u);
return (L * (size * open) + c + PlumeBase.xyz) - L;
""", ['L', 'T', 'Rnd', 'UV1', 'UV2', 'WindL', 'PlumeRate', 'PlumeRise', 'PlumeEase', 'PlumeDrift', 'PlumeSpread', 'PlumeSize0', 'PlumeSize1',
      'PlumeCycle', 'PlumeDuty', 'PlumeBase']),
    'Conveyor': (r"""
// one part travelling a line and coming round again. UV1.x = where along the line it starts
float f = frac(UV1.x + T * ConvRate + Rnd * ConvRand);
float3 c = ConvStart.xyz + ConvDir.xyz * (f * ConvLen);
c.z += ConvArc * sin(f * 3.14159);
float ends = smoothstep(0.0, 0.03, f) * (1.0 - smoothstep(0.97, 1.0, f));   // nothing pops at the ends
return (L * ends + c) - L;
""", ['L', 'T', 'Rnd', 'UV1', 'ConvRate', 'ConvLen', 'ConvArc', 'ConvRand', 'ConvStart', 'ConvDir']),
    'Burst': (r"""
// one spark of a shell burst. UV1 = its direction (azimuth in turns, elevation -1..1), UV2.x = its size
float u = frac(T * BurstRate + Rnd * 7.31 + BurstPhase);
float live = step(u, BurstLife);
float v = u / max(BurstLife, 1e-3);
float az = UV1.x * 6.2832, el = UV1.y * 1.5708;
float3 d = float3(cos(az) * cos(el), sin(az) * cos(el), sin(el));
float3 c = BurstBase.xyz + d * (BurstRadius * (1.0 - exp(-4.0 * v))) - float3(0.0, 0.0, BurstFall * v * v);
float size = (1.0 - v) * live * (0.4 + 0.6 * UV2.x);
return (L * size + c) - L;
""", ['L', 'T', 'Rnd', 'UV1', 'UV2', 'BurstRate', 'BurstLife', 'BurstRadius', 'BurstFall', 'BurstPhase', 'BurstBase']),
    # pixel side
    'LightLevel': (r"""
float ph = dot(Lp, LightAxis.xyz) / max(LightLen, 1.0) + Rnd * LightRand - T * LightRate;
float f = frac(ph);
float run = smoothstep(0.0, LightSoft, f) * (1.0 - smoothstep(LightDuty, LightDuty + LightSoft, f));
// LightHash > 0: cells of the mesh switch on and off on their own clocks (windows, a string of bulbs)
float3 cell = floor((Lp + LightOffset.xyz) / max(LightCell, 1.0));   // LightOffset: slide the cells so none cuts a window in two
float hsh = frac(sin(dot(cell, float3(12.9898, 78.233, 37.719)) + Rnd * 17.0) * 43758.5453);
float tick = floor(T * LightRate + hsh * 7.0);
float on = step(LightHash, frac(sin((tick + hsh * 91.0) * 12.9898) * 43758.5453));
return (LightHash > 0.0) ? on : run;
""", ['Lp', 'T', 'Rnd', 'LightRate', 'LightDuty', 'LightSoft', 'LightLen', 'LightAxis', 'LightRand', 'LightHash', 'LightCell', 'LightOffset']),
    'BurstLevel': (r"""
float u = frac(T * BurstRate + Rnd * 7.31 + BurstPhase);
return step(u, BurstLife) * sqrt(saturate(1.0 - u / max(BurstLife, 1e-3)));
""", ['T', 'Rnd', 'BurstRate', 'BurstLife', 'BurstPhase']),
    'Base': (r"""
float3 col = lerp(Col.rgb, Col2.rgb, (1.0 - Lvl) * saturate(FlowMix));
// the visual-finish wear (flecks and broad drift in the mesh's own space), so a slot keeps its look
float3 cell = floor(Lp * 0.095);
float fleck = frac(dot(frac(cell * float3(0.0639, 0.1301, 0.0098)), float3(4.527, 8.831, 4.191)));
float3 broad = sin(Lp * float3(0.012, 0.016, 0.021));
float grain = lerp(fleck - 0.5, 0.0, saturate(max(length(ddx(Lp)), length(ddy(Lp))) * 0.11));
float wear = ((broad.x + broad.y + broad.z) * 0.023 + grain * 0.095) * Wear;
float contrast = lerp(1.0, 0.35, saturate(Metal * 1.6 + Glow));
return col * (1.0 + wear * contrast);
""", ['Col', 'Col2', 'Lvl', 'FlowMix', 'Lp', 'Wear', 'Metal', 'Glow']),
    'Emissive': (r"""
float3 off = Col2.rgb * Glow2 + Col.rgb * (Glow * LightMin);
return lerp(off, Col.rgb * Glow, Lvl) * (Gain * BurstLvl);
""", ['Col', 'Col2', 'Glow', 'Glow2', 'Gain', 'LightMin', 'Lvl', 'BurstLvl']),
}


def _compile(m):
    """Recompile and save, and say what the shader came to (a material that fails still saves: check the log)."""
    errs = list(MEL.recompile_material(m) or [])
    if errs:
        raise RuntimeError('%s did not compile: %s' % (m.get_name(), errs))
    AL.save_loaded_asset(m)
    try:
        st = MEL.get_statistics(m)
        ps = st.get_editor_property('num_pixel_shader_instructions')
        log('compiled', m.get_name(), 'PS', ps, 'VS', st.get_editor_property('num_vertex_shader_instructions'))
        if not ps:
            unreal.log_warning('FTLIFE %s reports no pixel-shader instructions: check that it compiled' % m.get_name())
    except Exception as e:
        log('compiled', m.get_name(), '(no statistics: %s)' % e)
    return m


def _enum(cls, *names):
    for n in names:
        if hasattr(cls, n):
            return getattr(cls, n)
    raise RuntimeError('none of %s on %s' % (names, cls))


def _connect(a, out, b, pin_names):
    for pin in pin_names:
        if MEL.connect_material_expressions(a, out, b, pin):
            return True
    raise RuntimeError('could not connect to any of %s' % (pin_names,))


def _master(name, always=False):
    """Build (or rebuild in place) one M_Life master. always: its vertex movement ignores the WPO-disable distance."""
    m = ft_mats.new_material(LIFE, name, blend=unreal.BlendMode.BLEND_OPAQUE, shading=unreal.MaterialShadingModel.MSM_DEFAULT_LIT)
    m.set_editor_property('used_with_instanced_static_meshes', True)
    m.set_editor_property('tangent_space_normal', False)   # the normal is given in the world's space (Spin turns it)
    for prop, val in (('always_evaluate_world_position_offset', always),):
        try:
            m.set_editor_property(prop, val)
        except Exception as e:
            unreal.log_warning('FTLIFE %s: could not set %s (%s)' % (name, prop, e))
    P = {}
    y = -2600
    for nm, v in VECTORS:
        P[nm] = ft_mats.vparam(m, nm, v, x=-1700, y=y)
        y += 70
    for nm, v in SCALARS:
        P[nm] = ft_mats.sparam(m, nm, v, x=-1700, y=y)
        y += 60
    X = -1250
    lp = ft_mats.expr(m, unreal.MaterialExpressionLocalPosition, x=X, y=-2600)
    t = ft_mats.expr(m, unreal.MaterialExpressionTime, x=X, y=-2500)
    rnd = ft_mats.expr(m, unreal.MaterialExpressionPerInstanceRandom, x=X, y=-2400)
    wp = ft_mats.expr(m, unreal.MaterialExpressionWorldPosition, x=X, y=-2300)
    try:
        wp.set_editor_property('world_position_shader_offset', _enum(unreal.WorldPositionIncludedOffsets, 'WPT_EXCLUDE_ALL_SHADER_OFFSETS'))
    except Exception as e:
        unreal.log_warning('FTLIFE world position keeps its offsets (%s)' % e)
    nws = ft_mats.expr(m, unreal.MaterialExpressionVertexNormalWS, x=X, y=-2200)
    uv1 = ft_mats.expr(m, unreal.MaterialExpressionTextureCoordinate, x=X, y=-2100)
    uv1.set_editor_property('coordinate_index', 1)
    uv2 = ft_mats.expr(m, unreal.MaterialExpressionTextureCoordinate, x=X, y=-2000)
    uv2.set_editor_property('coordinate_index', 2)
    SRC, DST = unreal.MaterialVectorCoordTransformSource, unreal.MaterialVectorCoordTransform

    def xform(src, a, b, y):
        e = ft_mats.expr(m, unreal.MaterialExpressionTransform, x=X + 300, y=y)
        e.set_editor_property('transform_source_type', _enum(SRC, 'TRANSFORMSOURCE_' + a, 'TRANSFORMSOURCE_LOCAL'))
        e.set_editor_property('transform_type', _enum(DST, 'TRANSFORM_' + b, 'TRANSFORM_LOCAL'))
        _connect(src, '', e, ('Input', ''))
        return e

    # "instance" space is the mesh's own for a single mesh and each instance's own for an instanced prop
    wind_l = xform(P['Wind'], 'WORLD', 'INSTANCE', -1900)
    n_l = xform(nws, 'WORLD', 'INSTANCE', -1800)
    # the mesh's own position for the pixel side: taken at the vertex (before any movement) and carried across
    lpi = ft_mats.expr(m, unreal.MaterialExpressionVertexInterpolator, x=X + 300, y=-2600)
    _connect(lp, '', lpi, ('Input', ''))
    src = {'L': lp, 'Lp': lpi, 'T': t, 'Rnd': rnd, 'WP': wp, 'N': nws, 'Nl': n_l, 'UV1': uv1, 'UV2': uv2, 'WindL': wind_l,
           'Col': P['Color'], 'Col2': P['Color2'], 'Gain': P['GlowGain']}
    src.update(P)
    zero = ft_mats.expr(m, unreal.MaterialExpressionConstant3Vector, x=-500, y=-2700)
    zero.set_editor_property('constant', unreal.LinearColor(0.0, 0.0, 0.0, 0.0))
    one = ft_mats.expr(m, unreal.MaterialExpressionConstant, x=-500, y=-2620)
    one.set_editor_property('r', 1.0)

    def cu(key, y, out=F3, extra=None):
        code, names = HLSL[key]
        s = dict(src)
        s.update(extra or {})
        return ft_mats.custom(m, code, [(n, s[n], '') for n in names], out_type=out, x=-800, y=y, desc='Life' + key)

    def switch(name, a, b, y):
        sw = ft_mats.expr(m, unreal.MaterialExpressionStaticSwitchParameter, x=-450, y=y)
        sw.set_editor_property('parameter_name', name)
        sw.set_editor_property('default_value', False)
        _connect(a, '', sw, ('A', 'True'))
        _connect(b, '', sw, ('B', 'False'))
        return sw

    def add(a, b, y):
        e = ft_mats.expr(m, unreal.MaterialExpressionAdd, x=-200, y=y)
        _connect(a, '', e, ('A',))
        _connect(b, '', e, ('B',))
        return e

    y = -2500
    world = None
    for key in ('Sway', 'Breathe'):
        s = switch(key, cu(key, y), zero, y)
        world = s if world is None else add(world, s, y)
        y += 220
    local = None
    for key in ('Flutter', 'Spin', 'Bob', 'Plume', 'Conveyor', 'Burst'):
        s = switch(key, cu(key, y), zero, y)
        local = s if local is None else add(local, s, y)
        y += 220
    local_w = ft_mats.expr(m, unreal.MaterialExpressionTransform, x=-100, y=y)
    local_w.set_editor_property('transform_source_type', _enum(SRC, 'TRANSFORMSOURCE_INSTANCE', 'TRANSFORMSOURCE_LOCAL'))
    local_w.set_editor_property('transform_type', _enum(DST, 'TRANSFORM_WORLD'))
    _connect(local, '', local_w, ('Input', ''))
    MEL.connect_material_property(add(world, local_w, y), '', unreal.MaterialProperty.MP_WORLD_POSITION_OFFSET)
    y += 220
    # Spin turns the normal with the part (at the vertex, then carried across: flat shading keeps its facets)
    n_spin_l = cu('SpinNormal', y)
    n_spin = ft_mats.expr(m, unreal.MaterialExpressionTransform, x=-620, y=y + 120)
    n_spin.set_editor_property('transform_source_type', _enum(SRC, 'TRANSFORMSOURCE_INSTANCE', 'TRANSFORMSOURCE_LOCAL'))
    n_spin.set_editor_property('transform_type', _enum(DST, 'TRANSFORM_WORLD'))
    _connect(n_spin_l, '', n_spin, ('Input', ''))
    n_norm = ft_mats.custom(m, 'return normalize(V);', [('V', n_spin, '')], x=-560, y=y + 200, desc='LifeNormalise')
    n_int = ft_mats.expr(m, unreal.MaterialExpressionVertexInterpolator, x=-500, y=y + 280)
    _connect(n_norm, '', n_int, ('Input', ''))
    MEL.connect_material_property(switch('Spin', n_int, nws, y + 360), '', unreal.MaterialProperty.MP_NORMAL)
    y += 520
    lvl = switch('Light', cu('LightLevel', y, out=F1), one, y)
    y += 220
    blvl = switch('Burst', cu('BurstLevel', y, out=F1), one, y)
    y += 220
    base = cu('Base', y, extra={'Lvl': lvl})
    MEL.connect_material_property(base, '', unreal.MaterialProperty.MP_BASE_COLOR)
    y += 220
    em = cu('Emissive', y, extra={'Lvl': lvl, 'BurstLvl': blvl})
    MEL.connect_material_property(em, '', unreal.MaterialProperty.MP_EMISSIVE_COLOR)
    y += 220
    r = ft_mats.custom(m, 'return saturate(max(RoughMin, Rough));', [('Rough', P['Rough'], ''), ('RoughMin', P['RoughMin'], '')], out_type=F1, x=-800, y=y, desc='LifeRough')
    MEL.connect_material_property(r, '', unreal.MaterialProperty.MP_ROUGHNESS)
    mm = ft_mats.custom(m, 'return Metal * MetalScale;', [('Metal', P['Metal'], ''), ('MetalScale', P['MetalScale'], '')], out_type=F1, x=-800, y=y + 120, desc='LifeMetal')
    MEL.connect_material_property(mm, '', unreal.MaterialProperty.MP_METALLIC)
    _compile(m)
    log('master', name, 'built')
    return m


def masters():
    return _master('M_Life'), _master('M_LifeFar', always=True)


# ------------------------------------------------------------------------------------------ instances
def _lin_hex(c):
    def enc(v):
        v = max(0.0, min(1.0, v))
        v = v * 12.92 if v <= 0.0031308 else 1.055 * (v ** (1 / 2.4)) - 0.055
        return int(round(v * 255))
    return '%02x%02x%02x' % (enc(c[0]), enc(c[1]), enc(c[2]))


def _vec(v):
    v = tuple(float(x) for x in v)
    return v + (0.0,) * (4 - len(v))


def life_mi(color, glow=0.0, rough=0.55, metal=0.1, modes=(), far=False, look=None, **params):
    """An M_Life instance: a colour (linear rgb), its finish, the ways it moves (names from MODES) and their
    parameters by M_Life's own parameter names (see SCALARS and VECTORS; a 3-tuple for a vector)."""
    modes = tuple(sorted(set((modes,) if isinstance(modes, str) else modes)))
    for md in modes:
        if md not in MODES:
            raise ValueError('unknown way of moving %r (have %s)' % (md, MODES))
    if 'Spin' in modes:
        # a turning part's normal turns with it in the pixel shader, which the track's "hold still past 180 m" does not
        # stop: the vertices must never stop either, or the shading would go on turning over a part that stands still
        far = True
    look = dict(look or {})
    known_s, known_v = dict(SCALARS), dict(VECTORS)
    for k in list(params) + list(look):
        if k not in known_s and k not in known_v:
            raise ValueError('M_Life has no parameter %r' % k)
    key = repr((tuple(round(x, 4) for x in color[:3]), round(glow, 3), round(rough, 3), round(metal, 3), modes, bool(far),
                sorted((k, _vec(v) if k in known_v else round(float(v), 5)) for k, v in list(params.items()) + list(look.items()))))
    name = 'MI_Life_%s_g%d_%s' % (_lin_hex(color), int(round(glow * 100)), hashlib.md5(key.encode('utf-8')).hexdigest()[:8])
    path = INST + '/' + name
    if AL.does_asset_exist(path):
        return AL.load_asset(path)
    parent = AL.load_asset(LIFE + ('/M_LifeFar' if far else '/M_Life'))
    if parent is None:
        raise RuntimeError('build the masters first: ft_life.masters()')
    if not AL.does_directory_exist(INST):
        AL.make_directory(INST)
    mi = AT.create_asset(name, INST, unreal.MaterialInstanceConstant, unreal.MaterialInstanceConstantFactoryNew())
    mi.set_editor_property('parent', parent)
    MEL.set_material_instance_vector_parameter_value(mi, 'Color', unreal.LinearColor(color[0], color[1], color[2], 1.0))
    scal = {'Glow': glow, 'Rough': rough, 'Metal': metal}
    scal.update({k: v for k, v in look.items() if k in known_s})
    scal.update({k: v for k, v in params.items() if k in known_s})
    for k, v in scal.items():
        MEL.set_material_instance_scalar_parameter_value(mi, k, float(v))
    for k, v in list(look.items()) + list(params.items()):
        if k in known_v:
            MEL.set_material_instance_vector_parameter_value(mi, k, unreal.LinearColor(*_vec(v)))
    for md in modes:
        MEL.set_material_instance_static_switch_parameter_value(mi, md, True)
    MEL.update_material_instance(mi)
    AL.save_loaded_asset(mi)
    return mi


def _read(mi):
    """(colour, glow, rough, metal, look) of a prop's present instance: a palette, visual-finish or M_Life one."""
    if not isinstance(mi, unreal.MaterialInstanceConstant):
        return None
    base = mi.get_base_material()
    bname = base.get_name() if base else ''
    c = MEL.get_material_instance_vector_parameter_value(mi, 'Color')
    look = {}
    if bname == 'M_Paint':
        look = {'GlowGain': 14.0, 'MetalScale': 0.25, 'RoughMin': 0.4, 'Wear': 0.0}
    elif bname.startswith('M_Life'):
        look = {k: MEL.get_material_instance_scalar_parameter_value(mi, k) for k in ('GlowGain', 'MetalScale', 'RoughMin', 'Wear')}
    elif bname == 'M_VisualFinish':
        # M_Life's own defaults, written out: a second animate() reads them back from the M_Life instance, and the
        # instance's name is made from them (left out here, every re-apply made a second, identical set of instances)
        look = {'GlowGain': 10.5, 'MetalScale': 0.32, 'RoughMin': 0.32, 'Wear': 1.0}
    else:
        return None
    g = MEL.get_material_instance_scalar_parameter_value
    return (c.r, c.g, c.b), g(mi, 'Glow'), g(mi, 'Rough'), g(mi, 'Metal'), look


def _wanted(slots, slot, mi, col, glow):
    if slots is None:
        return True
    if callable(slots):
        return bool(slots(slot, _lin_hex(col), glow))
    for s in slots:
        s = s.lower()
        if s == slot.lower():
            return True
        if s.startswith('#') and s[1:7] == _lin_hex(col):
            if (s.endswith('*') and glow <= 0.0) or (s.endswith('-') and glow > 0.0):
                continue
            return True
        if s == 'glow' and glow > 0.0:
            return True
    return False


def animate(mesh_path, mode, slots=None, far=False, **params):
    """Put M_Life on an existing prop: its material slots keep their colour, glow and finish and gain a way of moving.

    mode    one of MODES or several ('Sway', 'Light')
    slots   None = every slot; or a list of slot names ('M3'), colours as the builders write them ('#2f6b52'; with a
            trailing * only where that colour glows, with a trailing - only where it does not) and/or 'glow' (every
            slot that glows); or a function (slot name, colour hex without #, glow) -> bool
    far     the mesh is a landmark seen moving from far away (M_LifeFar)
    params  M_Life parameters by name: animate(p, 'Sway', SwayAmp=40, SwayHeight=900)
    Slots that move together must get the same call (the same parameters), or they come apart.
    Returns the number of slots changed. The mesh is saved in place."""
    try:
        return _animate(mesh_path, mode, slots, far, params)
    except Exception as e:
        # one prop that fails must not stop the level's other props being done
        import traceback
        unreal.log_error('FTLIFE animate %s %s failed: %r %s' % (mesh_path, mode, e, traceback.format_exc()))
        return 0


def _animate(mesh_path, mode, slots, far, params):
    sm = AL.load_asset(mesh_path)
    if sm is None:
        unreal.log_warning('FTLIFE animate: no mesh %s' % mesh_path)
        return 0
    n = 0
    mats = list(sm.get_editor_property('static_materials'))
    for i, st in enumerate(mats):
        got = _read(st.material_interface)
        if got is None:
            continue
        col, glow, rough, metal, look = got
        if not _wanted(slots, str(st.material_slot_name), st.material_interface, col, glow):
            continue
        new = life_mi(col, glow, rough, metal, mode, far, look, **params)
        if new != st.material_interface:
            st.set_editor_property('material_interface', new)
            n += 1
    if n:
        # every slot in one assignment: each change of a slot rebuilds the mesh (and its distance field)
        sm.set_editor_property('static_materials', mats)
        AL.save_loaded_asset(sm)
    log('animate', mesh_path.rsplit('/', 1)[-1], mode, 'slots', n)
    return n


def slots(mesh_path):
    """[(slot name, colour hex, glow, material name)] of a mesh: what animate() can be pointed at."""
    sm = AL.load_asset(mesh_path)
    out = []
    for st in (sm.get_editor_property('static_materials') if sm else []):
        got = _read(st.material_interface)
        mn = st.material_interface.get_name() if st.material_interface else None
        out.append((str(st.material_slot_name), '#' + _lin_hex(got[0]) if got else None, round(got[1], 2) if got else None, mn))
    return out


# ------------------------------------------------------------------------------------------ new props
def _const_uv(mesh, channel, u, v):
    """Give every vertex of a part the same UV in a channel: M_Life tells parts apart by it. A projection from a plane
    ten thousand kilometres wide gives one value to the whole part; that value is then slid to (u, v)."""
    sel = unreal.GeometryScriptMeshSelection()
    plane = unreal.Transform(unreal.Vector(0, 0, 0), unreal.Rotator(0, 0, 0), unreal.Vector(1.0e9, 1.0e9, 1.0e9))
    UVS.set_mesh_u_vs_from_planar_projection(mesh, channel, plane, sel)
    r = MQ.get_triangle_u_vs(mesh, channel, 0)
    a = r[0] if isinstance(r, tuple) else r
    UVS.translate_mesh_u_vs(mesh, channel, unreal.Vector2D(u - a.x, v - a.y), sel)


def _no_distance_field(sm):
    """These meshes are not where their vertices say (the material sends their parts about), so a distance field made
    from them would put soft shadow and occlusion where nothing is. Their real shadows follow the moved vertices."""
    try:
        sm.set_editor_property('generate_mesh_distance_field', False)
    except Exception as e:
        unreal.log_warning('FTLIFE %s keeps its distance field (%s)' % (sm.get_name(), e))


class Group(ft_art.Model):
    """Parts of a new prop that move one way. Draw with ft_art.Model's box, cyl, sphere, side, plan."""

    def __init__(self, modes, far, rough, metal, params):
        ft_art.Model.__init__(self, 'life')
        self.modes, self.far, self.rough, self.metal, self.params = modes, far, rough, metal, params

    def piece(self, draw, a=0.0, b=0.0, c=0.0, d=0.0):
        """One part for Plume, Conveyor or Burst: draw(model) makes it about the origin; (a, b) go to UV1 and (c, d) to
        UV2, the same for every vertex of the part."""
        tmp = ft_art.Model('part')
        draw(tmp)
        for key, pm in tmp.meshes.items():
            UVS.set_num_uv_sets(pm, 3)
            _const_uv(pm, 1, a, b)
            _const_uv(pm, 2, c, d)
            tgt = self._mesh(*key)
            UVS.set_num_uv_sets(tgt, 3)
            unreal.GeometryScript_MeshEdits.append_mesh(tgt, pm, unreal.Transform())


class Kit(object):
    """A new animated prop: groups of parts, each with its way of moving, saved as one static mesh with M_Life on it."""

    def __init__(self):
        self.groups = []

    def group(self, modes=(), far=False, rough=0.6, metal=0.0, **params):
        names = (modes,) if isinstance(modes, str) else tuple(modes)
        if set(names) & set(('Plume', 'Conveyor', 'Burst')):
            far = True   # these parts are built in a heap at the origin: their material must never be switched off
        g = Group(modes, far, rough, metal, params)
        self.groups.append(g)
        return g

    def save(self, path, grow=(0.0, 0.0, 0.0), grow_down=(0.0, 0.0, 0.0)):
        """grow / grow_down: how far (cm) the parts travel beyond the mesh as built (+x, +y, +z and -x, -y, -z), so the
        prop is not culled while its moving parts are still in view."""
        combined = unreal.DynamicMesh()
        UVS.set_num_uv_sets(combined, 3)
        mats, names = [], []
        for g in self.groups:
            for (col, glow), m in g.meshes.items():
                UVS.set_num_uv_sets(m, 3)
                unreal.GeometryScript_Normals.set_per_face_normals(m)
                unreal.GeometryScript_Materials.enable_material_i_ds(m)
                unreal.GeometryScript_Materials.remap_material_i_ds(m, 0, len(mats))
                unreal.GeometryScript_MeshEdits.append_mesh(combined, m, unreal.Transform())
                mats.append(life_mi(ft_art.hex_lin(col), glow, g.rough, g.metal, g.modes, g.far, None, **g.params))
                names.append('L%d' % len(names))
        folder = path.rsplit('/', 1)[0]
        if not AL.does_directory_exist(folder):
            AL.make_directory(folder)
        if AL.does_asset_exist(path):
            sm = AL.load_asset(path)
            opts = unreal.GeometryScriptCopyMeshToAssetOptions()
            opts.enable_recompute_normals = False
            opts.replace_materials = True
            opts.new_materials = mats
            opts.new_material_slot_names = [unreal.Name(n) for n in names]
            opts.emit_transaction = False
            unreal.GeometryScript_AssetUtils.copy_mesh_to_static_mesh(combined, sm, opts, unreal.GeometryScriptMeshWriteLOD())
        else:
            opts = unreal.GeometryScriptCreateNewStaticMeshAssetOptions()
            opts.set_editor_property('enable_nanite', False)
            opts.set_editor_property('enable_collision', False)
            opts.set_editor_property('enable_recompute_normals', False)
            sm, _ = unreal.GeometryScript_NewAssetUtils.create_new_static_mesh_asset_from_mesh(combined, path, opts)
            if sm is None:
                raise RuntimeError('could not create ' + path)
            sm.set_editor_property('static_materials', [unreal.StaticMaterial(material_interface=mats[i], material_slot_name=names[i]) for i in range(len(mats))])
        sm.set_editor_property('positive_bounds_extension', unreal.Vector(*grow))
        sm.set_editor_property('negative_bounds_extension', unreal.Vector(*grow_down))
        _no_distance_field(sm)
        AL.save_loaded_asset(sm)
        log('prop', path.rsplit('/', 1)[-1], 'sections', len(mats), 'triangles', MQ.get_num_triangle_i_ds(combined))
        return sm


# ------------------------------------------------------------------------------------------ the air round the camera
AIR_VS = r"""
// every card belongs to a cell of a 100 x 100 x 100 lattice: its four corners sit round the cell's centre
float3 cell = floor(L);
float3 h = frac(sin(float3(dot(cell, float3(12.9898, 78.233, 37.719)), dot(cell, float3(39.3468, 11.135, 83.155)), dot(cell, float3(73.156, 52.235, 9.151)))) * 43758.5453);
float h4 = frac(h.x * 7.31 + h.y * 3.17 + h.z * 5.77);
float two = step(h4, Split);                                  // the second kind (ash under embers, snow under bubbles)
float3 vel = lerp(Vel.xyz, Vel2.xyz, two);
// where it is in the world: its own place, its drift, its own wander. Nothing here knows about the camera
float3 p = ((cell + 0.5) * 0.01 + (h - 0.5) * 0.01) * Box.xyz + vel * T;
float tt = T * Turb.w;
p += float3(sin(tt * (0.6 + h.x) + h.y * 6.2832), cos(tt * (0.5 + h.z) + h.x * 6.2832), sin(tt * (0.7 + h.y) + h.z * 6.2832)) * Turb.xyz;
// the view takes the box of it that lies round (mostly ahead of) the camera
float3 fwd = ResolvedView.ViewForward;
float3 centre = CP + float3(normalize(fwd.xy + float2(1e-4, 0.0)) * Ahead, Lift);
float3 rel = (frac((p - centre) / Box.xyz + 0.5) - 0.5) * Box.xyz;
float3 wp = centre + rel;
float3 e = abs(rel) / (0.5 * Box.xyz);
float fade = saturate((1.0 - max(e.x, max(e.y, e.z))) * 5.0);   // nothing appears or vanishes at the box's faces
fade *= saturate((length(wp - CP) - NearFade) / max(NearFade, 1.0));   // nor across the lens
fade *= step(frac(h4 * 13.7), Density);
float2 sz = lerp(Size.xy, Size.zw, two);
float size = lerp(sz.x, sz.y, h.x * h.x) * fade;
float2 corner = sign(frac(L.xy) - 0.5) * 0.5;
float a = h.y * 6.2832 + T * Spin * (h.z - 0.5) * 2.0;
float2 rc = float2(corner.x * cos(a) - corner.y * sin(a), corner.x * sin(a) + corner.y * cos(a));
rc.y *= Aspect;
wp += (ResolvedView.ViewRight * rc.x + ResolvedView.ViewUp * rc.y) * size;
return wp - WP;
"""
AIR_PS = r"""
float3 cell = floor(Lp);
float3 h = frac(sin(float3(dot(cell, float3(12.9898, 78.233, 37.719)), dot(cell, float3(39.3468, 11.135, 83.155)), dot(cell, float3(73.156, 52.235, 9.151)))) * 43758.5453);
float h4 = frac(h.x * 7.31 + h.y * 3.17 + h.z * 5.77);
float two = step(h4, Split);
float2 q = (frac(Lp.xy) - 0.5) * 10.0;                         // -1..1 across the card
float r = length(q);
float disc = 1.0 - smoothstep(Soft, 1.0, r);
float ring = lerp(1.0, 0.25 + 0.75 * smoothstep(Ring - 0.3, Ring, r), step(0.01, Ring) * (1.0 - two));
float tw = lerp(1.0, 0.45 + 0.55 * sin(T * Twinkle * (0.5 + h.x) + h.y * 6.2832), saturate(Twinkle) * (1.0 - two));
float3 hue = saturate(abs(frac(h.z + float3(0.0, 0.667, 0.333)) * 6.0 - 3.0) - 1.0);
float3 col = lerp(lerp(ColorA.rgb, ColorB.rgb, two), hue * dot(ColorA.rgb, float3(0.33, 0.33, 0.33)) * 2.0, saturate(Rainbow));
Alpha = saturate(disc * ring * lerp(ColorA.a, ColorB.a, two) * Opacity) * tw;
return col * tw;
"""
AIR_SCALARS = [('Split', 0.0), ('Ahead', 1500.0), ('Lift', 250.0), ('NearFade', 180.0), ('Density', 1.0), ('Spin', 0.0), ('Aspect', 1.0),
               ('Soft', 0.2), ('Ring', 0.0), ('Twinkle', 0.0), ('Rainbow', 0.0), ('Opacity', 1.0)]
AIR_VECTORS = [('Box', (5000.0, 5000.0, 1400.0, 0.0)), ('Vel', (60.0, 20.0, 5.0, 0.0)), ('Vel2', (0.0, 0.0, -30.0, 0.0)),
               ('Turb', (40.0, 40.0, 20.0, 1.0)), ('Size', (3.0, 8.0, 3.0, 8.0)), ('ColorA', (1.0, 1.0, 1.0, 0.6)), ('ColorB', (0.5, 0.5, 0.5, 0.5))]
# level by level: what the air carries. Sizes in cm (min, max, then the second kind's), colours linear with alpha last.
AIR_LEVELS = {
    0: dict(Density=0.1, Box=(6000, 6000, 1500), Vel=(70, 25, 6), Turb=(60, 60, 30, 0.6), Size=(3, 6, 3, 6), ColorA=(2.4, 1.9, 1.3, 0.5), Soft=0.1),
    1: dict(Density=0.5, Box=(5200, 5200, 1500), Vel=(40, 15, 80), Turb=(70, 70, 40, 1.3), Size=(2.5, 6, 2.5, 7), ColorA=(9.0, 4.2, 0.9, 0.9),
            ColorB=(1.2, 6.5, 7.5, 0.8), Split=0.25, Vel2=(30, 10, 55), Twinkle=7.0, Soft=0.15),
    2: dict(Density=0.7, Box=(6000, 6000, 1200), Vel=(260, 95, 4), Turb=(90, 90, 30, 0.7), Size=(4, 13, 2, 5), ColorA=(0.95, 0.72, 0.5, 0.3),
            ColorB=(0.5, 0.36, 0.24, 0.8), Split=0.12, Vel2=(420, 150, -6), Soft=0.05, Lift=120),
    3: dict(Density=0.75, Box=(5200, 5200, 1700), Vel=(35, 15, 150), Turb=(80, 80, 50, 1.6), Size=(2.5, 7, 4, 10), ColorA=(10.0, 3.2, 0.5, 0.95),
            ColorB=(0.07, 0.06, 0.07, 0.55), Split=0.45, Vel2=(60, 25, -70), Twinkle=6.0, Soft=0.2),
    4: dict(Density=0.3, Box=(5200, 5200, 1500), Vel=(30, 15, -45), Turb=(80, 80, 30, 1.1), Size=(4, 8, 4, 8), ColorA=(2.6, 2.6, 2.6, 0.9),
            Rainbow=1.0, Spin=5.0, Aspect=0.55, Soft=0.75),
    5: dict(Density=1.0, Box=(5200, 5200, 1500), Vel=(430, 170, -75), Turb=(60, 60, 35, 1.5), Size=(3, 8, 2, 4), ColorA=(1.6, 1.7, 1.9, 0.85),
            ColorB=(1.5, 1.6, 1.8, 0.7), Split=0.3, Vel2=(620, 240, -40), Soft=0.1),
    6: dict(Density=0.8, Box=(4600, 4600, 1700), Vel=(8, 4, 115), Turb=(26, 26, 8, 1.8), Size=(4, 12, 1.5, 3.5), ColorA=(1.3, 2.6, 2.9, 0.75),
            ColorB=(1.2, 1.8, 2.0, 0.5), Split=0.55, Vel2=(18, 9, -11), Ring=0.72, Soft=0.55),
    7: dict(Density=0.4, Box=(5200, 5200, 1500), Vel=(22, 10, 60), Turb=(60, 60, 20, 0.8), Size=(50, 120, 5, 10), ColorA=(0.55, 0.55, 0.7, 0.07),
            ColorB=(1.5, 1.45, 1.3, 0.85), Split=0.3, Vel2=(190, 70, 12), Spin=4.0, Aspect=0.8, Soft=0.0, Lift=320),
}


def air_master():
    m = ft_mats.new_material(AIR, 'M_Air', blend=unreal.BlendMode.BLEND_TRANSLUCENT, shading=unreal.MaterialShadingModel.MSM_UNLIT, two_sided=True)
    # no translucent velocity: it writes the cards' depth too, and the cel pass then inks a ring round every card
    for prop, val in (('always_evaluate_world_position_offset', True), ('output_translucent_velocity', False)):
        try:
            m.set_editor_property(prop, val)
        except Exception as e:
            unreal.log_warning('FTLIFE M_Air: could not set %s (%s)' % (prop, e))
    P = {}
    y = -900
    for nm, v in AIR_VECTORS:
        P[nm] = ft_mats.vparam(m, nm, v, x=-1500, y=y)
        y += 70
    for nm, v in AIR_SCALARS:
        P[nm] = ft_mats.sparam(m, nm, v, x=-1500, y=y)
        y += 60
    lp = ft_mats.expr(m, unreal.MaterialExpressionLocalPosition, x=-1100, y=-900)
    t = ft_mats.expr(m, unreal.MaterialExpressionTime, x=-1100, y=-800)
    wp = ft_mats.expr(m, unreal.MaterialExpressionWorldPosition, x=-1100, y=-700)
    try:
        wp.set_editor_property('world_position_shader_offset', _enum(unreal.WorldPositionIncludedOffsets, 'WPT_EXCLUDE_ALL_SHADER_OFFSETS'))
    except Exception as e:
        unreal.log_warning('FTLIFE M_Air world position keeps its offsets (%s)' % e)
    cp = ft_mats.expr(m, unreal.MaterialExpressionCameraPositionWS, x=-1100, y=-600)
    lpi = ft_mats.expr(m, unreal.MaterialExpressionVertexInterpolator, x=-850, y=-900)
    _connect(lp, '', lpi, ('Input', ''))
    # the alpha components travel as their own inputs (a vector parameter's default output is rgb)
    vs_in = [('L', lp, ''), ('T', t, ''), ('WP', wp, ''), ('CP', cp, ''), ('Box', P['Box'], ''), ('Vel', P['Vel'], ''), ('Vel2', P['Vel2'], '')]

    def rgba(name, y):
        e = ft_mats.expr(m, unreal.MaterialExpressionAppendVector, x=-1250, y=y)
        _connect(P[name], '', e, ('A',))
        _connect(P[name], 'A', e, ('B',))
        return e

    turb, size, col_a, col_b = rgba('Turb', -500), rgba('Size', -420), rgba('ColorA', -340), rgba('ColorB', -260)
    vs_in += [('Turb', turb, ''), ('Size', size, '')] + [(k, P[k], '') for k in ('Split', 'Ahead', 'Lift', 'NearFade', 'Density', 'Spin', 'Aspect')]
    vs = ft_mats.custom(m, AIR_VS, vs_in, x=-500, y=-600, desc='AirPlace')
    MEL.connect_material_property(vs, '', unreal.MaterialProperty.MP_WORLD_POSITION_OFFSET)
    ps_in = [('Lp', lpi, ''), ('T', t, ''), ('ColorA', col_a, ''), ('ColorB', col_b, '')] + [(k, P[k], '') for k in ('Split', 'Soft', 'Ring', 'Twinkle', 'Rainbow', 'Opacity')]
    # made by hand: the extra output (Alpha) has to exist before anything is connected
    ps = MEL.create_material_expression(m, unreal.MaterialExpressionCustom, -500, 200)
    ps.set_editor_property('code', AIR_PS)
    ps.set_editor_property('output_type', F3)
    ps.set_editor_property('description', 'AirCard')
    out = unreal.CustomOutput()
    out.set_editor_property('output_name', 'Alpha')
    out.set_editor_property('output_type', F1)
    ps.set_editor_property('additional_outputs', [out])
    cins = []
    for nm, _, _ in ps_in:
        ci = unreal.CustomInput()
        ci.set_editor_property('input_name', nm)
        cins.append(ci)
    ps.set_editor_property('inputs', cins)
    for nm, ex, outn in ps_in:
        MEL.connect_material_expressions(ex, outn, ps, nm)
    MEL.connect_material_property(ps, '', unreal.MaterialProperty.MP_EMISSIVE_COLOR)
    MEL.connect_material_property(ps, 'Alpha', unreal.MaterialProperty.MP_OPACITY)
    _compile(m)
    return m


def air_mesh(n=600, seed=7):
    """600 cards, each a 0.2 cm square at the centre of its own cell of a 100^3 lattice. M_Air does the rest."""
    r = random.Random(seed)
    m = unreal.DynamicMesh()
    po = unreal.GeometryScriptPrimitiveOptions()
    seen = set()
    while len(seen) < n:
        seen.add((r.randrange(-50, 50), r.randrange(-50, 50), r.randrange(-50, 50)))
    for ix, iy, iz in sorted(seen):
        GS.append_rectangle_xy(m, po, unreal.Transform(unreal.Vector(ix + 0.5, iy + 0.5, iz + 0.5), unreal.Rotator(0, 0, 0), unreal.Vector(1, 1, 1)), 0.2, 0.2, 0, 0)
    path = AIR + '/SM_AirField'
    if not AL.does_directory_exist(AIR):
        AL.make_directory(AIR)
    mat = AL.load_asset(AIR + '/M_Air')
    if AL.does_asset_exist(path):
        sm = AL.load_asset(path)
        opts = unreal.GeometryScriptCopyMeshToAssetOptions()
        opts.enable_recompute_normals = False
        opts.replace_materials = True
        opts.new_materials = [mat]
        opts.new_material_slot_names = [unreal.Name('Air')]
        opts.emit_transaction = False
        unreal.GeometryScript_AssetUtils.copy_mesh_to_static_mesh(m, sm, opts, unreal.GeometryScriptMeshWriteLOD())
    else:
        opts = unreal.GeometryScriptCreateNewStaticMeshAssetOptions()
        opts.set_editor_property('enable_nanite', False)
        opts.set_editor_property('enable_collision', False)
        opts.set_editor_property('enable_recompute_normals', False)
        sm, _ = unreal.GeometryScript_NewAssetUtils.create_new_static_mesh_asset_from_mesh(m, path, opts)
        sm.set_editor_property('static_materials', [unreal.StaticMaterial(material_interface=mat, material_slot_name='Air')])
    # the cards travel up to half a box from the camera: the mesh must count as that big, or it is culled
    sm.set_editor_property('positive_bounds_extension', unreal.Vector(4000.0, 4000.0, 2000.0))
    sm.set_editor_property('negative_bounds_extension', unreal.Vector(4000.0, 4000.0, 2000.0))
    _no_distance_field(sm)
    AL.save_loaded_asset(sm)
    return sm


def air():
    """M_Air, the card mesh and one instance per level (MI_Air_L0..7), which FTAmbient.cpp loads by path."""
    m = air_master()
    air_mesh()
    known_v = dict(AIR_VECTORS)
    for lvl, cfg in AIR_LEVELS.items():
        vec = {k: _vec(v) for k, v in cfg.items() if k in known_v}
        sc = {k: float(v) for k, v in cfg.items() if k not in known_v}
        ft_mats.mi(m, AIR, 'MI_Air_L%d' % lvl, vectors=vec, scalars=sc)
    log('air built for', len(AIR_LEVELS), 'levels')


def build_core():
    masters()
    air()


# ------------------------------------------------------------------------------------------ the new props
# Built like the levels' own (ft_art.Model: centimetres, origin at the middle of the base, X along the road away from
# the chase camera, Y to the road's right, Z up; a prop's front faces -X, towards the oncoming car). The rules that
# place them are in Source/FullThrottle/FTTrackLife.cpp.
CHEM = '/Game/Maps/ChemicalWorks/Life/'
NEV = '/Game/Maps/Nevada/Life/'
PARK = '/Game/Maps/ThemePark/Life/'
ICE = '/Game/Maps/Glacier/Life/'
CITY = '/Game/Maps/Metro/Life/'


def _about_x(cy, cz, deg):
    """A transform that turns whatever is drawn next by deg about the line through (y, z) = (cy, cz) that runs along X:
    for the blades of a wheel, each drawn straight up and turned into its place."""
    return ft_art._xf((0, -cy, -cz)) * ft_art._xf((0, 0, 0), (0, 0, deg)) * ft_art._xf((0, cy, cz))


def steam_vent(path, pipe, body, trim, dark, smoke, top=620.0, rise=1100.0, drift=520.0, r=60.0, n=8, rate=0.22, cycle=0.0, duty=1.0, size1=2.6, emit=0.0):
    """A vent and its steam: chunky balls that leave the mouth small, climb, lean down the wind, swell and thin out.
    pipe: a standing vent pipe (the chemical works); otherwise a grate in the pavement (the city). cycle > 0: it
    vents for `duty` of every 1/cycle seconds and rests. emit: how much the steam shows its own colour whatever the
    light (the city's red sky light turned plain lit steam into strings of pink balls)."""
    k = Kit()
    s = k.group()
    if pipe:
        s.cyl(dark, (0, 0, 0), 74, 30, steps=10)
        s.cyl(body, (0, 0, 30), 46, top - 60, steps=10)
        s.cyl(trim, (0, 0, top * 0.55), 52, 22, steps=10)
        s.cyl(body, (0, 0, top - 30), 60, 30, steps=10)
    else:
        s.box(dark, (0, 0, 6), (170, 170, 12))
        for i in range(5):
            s.box(body, (-64 + i * 32, 0, 13), (10, 150, 4))
    p = k.group('Plume', rough=0.95, PlumeBase=(0.0, 0.0, top), PlumeRise=rise, PlumeDrift=drift, PlumeSize0=0.45, PlumeSize1=size1,
                PlumeRate=rate, PlumeCycle=cycle, PlumeDuty=duty, PlumeSpread=r * 1.1, PlumeEase=0.8)
    for i in range(n):
        p.piece(lambda m: m.sphere(smoke, (0, 0, 0), r, steps=(5, 8), glow=emit), a=i / float(n), b=0.8 + 0.4 * ((i * 7) % 5) / 4.0, c=i * 0.37)
    reach = drift * 1.15 + r * size1 + 60.0
    k.save(path, grow=(reach, reach, top + rise + r * size1), grow_down=(reach, reach, 0.0))


def pumpjack(path, body, trim, dark, rate=1.5):
    """A nodding-donkey pump: the walking beam rocks on its post while the crank and counterweights turn."""
    k = Kit()
    s = k.group()
    s.box(dark, (0, 0, 20), (640, 210, 40))
    for sx in (-1, 1):
        for sy in (-1, 1):
            tilt_box(s, body, (sx * 120, sy * 70, 40), (0, sy * 30, 540), 30, 30)
    s.box(body, (0, 0, 545), (70, 130, 36))
    s.box(dark, (-240, 0, 130), (170, 150, 180))
    s.cyl(dark, (330, 0, 0), 26, 190, steps=8)
    s.box(trim, (330, 0, 200), (64, 64, 20))
    rod(s, trim, (330, 0, 200), (330, 0, 470), 5, steps=4)
    b = k.group('Spin', SpinPivot=(0.0, 0.0, 572.0), SpinAxis=(0.0, 1.0, 0.0), SpinSwing=13.0, SpinRate=rate, SpinRand=1.0)
    b.box(trim, (20, 0, 590), (680, 52, 48))
    b.side(trim, [(330, 640), (400, 650), (424, 470), (352, 452)], -36, 36)
    b.box(dark, (-310, 0, 590), (60, 96, 76))
    c = k.group('Spin', SpinPivot=(-240.0, 0.0, 230.0), SpinAxis=(0.0, 1.0, 0.0), SpinRate=rate, SpinRand=1.0)
    for sy in (-1, 1):
        c.box(body, (-240 + 80, sy * 98, 230), (230, 22, 54))
        c.box(trim, (-240 + 170, sy * 98, 230), (96, 32, 118))
    c.cyl(dark, (-240, -110, 230), 22, 220, rot=(0, 0, 90), steps=8)
    k.save(path, grow=(60, 0, 120), grow_down=(60, 0, 0))


def fan_unit(path, body, trim, dark, blade):
    """A cooling unit with a big fan in its face, turning towards the oncoming car."""
    k = Kit()
    s = k.group()
    s.box(dark, (60, 0, 20), (300, 640, 40))
    s.box(body, (60, 0, 300), (260, 600, 520))
    s.box(trim, (60, 0, 572), (280, 620, 24))
    for sy in (-1, 1):
        s.box(trim, (-74, sy * 270, 300), (8, 20, 480))
    for z in (80, 520):
        s.box(trim, (-74, 0, z), (8, 560, 20))
    f = k.group('Spin', SpinPivot=(-80.0, 0.0, 300.0), SpinAxis=(1.0, 0.0, 0.0), SpinRate=2.4, SpinRand=1.0)
    f.cyl(trim, (-70, 0, 300), 44, 30, rot=(90, 0, 0), steps=10)
    for i in range(5):
        f.base = _about_x(0.0, 300.0, i * 72.0)      # one blade, drawn straight up, turned into its place
        f.box(blade, (-84, 0, 300 + 132), (8, 70, 186))
    f.base = None
    k.save(path)


def flare(path):
    """A flare stack, far off: a thin tower with a flame that whips and flickers at its tip."""
    k = Kit()
    s = k.group()
    H = 5200.0
    s.cyl('#1c1f4a', (0, 0, 0), 130, 90, steps=10)
    s.cyl('#3a5ab8', (0, 0, 90), 62, H, steps=8, top_r=40)
    for z in (1400, 2800, 4200):
        s.cyl('#ffd23f', (0, 0, z), 72, 34, steps=8)
    s.cyl('#1c1f4a', (0, 0, H + 90), 74, 80, steps=8)
    top = H + 170.0
    fl = k.group(('Flutter', 'Light'), far=True, rough=0.9, FlutterPin=(0.0, 0.0, top), FlutterAxis=(0.0, 0.0, 1.0), FlutterDir=(0.8, 0.6, 0.0),
                 FlutterSpan=520.0, FlutterLen=560.0, FlutterRate=7.5, FlutterAmp=90.0, LightHash=0.45, LightCell=100000.0, LightRate=9.0, LightMin=0.62)
    # the flame in short lengths, so it has somewhere to bend
    for col, r0, h, glow in (('#ff8a2a', 110.0, 700.0, 2.2), ('#ffd23f', 62.0, 440.0, 3.0)):
        n = 4
        for i in range(n):
            fl.cyl(col, (0, 0, top + h * i / n), r0 * (1.0 - i / float(n)) + 6, h / n, steps=6, top_r=r0 * (1.0 - (i + 1) / float(n)) + 6, glow=glow)
    k.save(path, grow=(200, 200, 200), grow_down=(200, 200, 0))


def windpump(path):
    """A desert wind pump: a lattice tower, a tail vane, and a wheel of blades that turns in the wind."""
    k = Kit()
    s = k.group()
    H = 900.0
    wood, wood_d, tin = '#9a6a44', '#6a4c36', '#c9c4d6'
    for sx, sy in ((-1, -1), (1, -1), (1, 1), (-1, 1)):
        tilt_box(s, wood, (sx * 120, sy * 120, 0), (sx * 26, sy * 26, H), 16, 16)
    for z in (200, 470, 720):
        half = 120 - (120 - 26) * z / H
        for sgn in (-1, 1):
            s.box(wood_d, (sgn * half, 0, z), (10, 2 * half, 10))
            s.box(wood_d, (0, sgn * half, z), (2 * half, 10, 10))
    s.box(tin, (0, 0, H + 14), (90, 70, 36))
    tilt_box(s, tin, (20, 0, H + 30), (290, 0, H + 30), 6, 50)
    s.box('#e0784a', (330, 0, H + 46), (120, 6, 130))
    s.cyl('#5f8f4a', (0, 0, 0), 150, 26, steps=10)
    w = k.group('Spin', SpinPivot=(-58.0, 0.0, H + 30.0), SpinAxis=(1.0, 0.0, 0.0), SpinRate=2.2, SpinRand=1.0)
    w.cyl(tin, (-40, 0, H + 30), 16, 34, rot=(90, 0, 0), steps=8)
    for i in range(12):
        w.base = _about_x(0.0, H + 30.0, i * 30.0)
        w.box(tin if i % 2 else '#e9e4f2', (-60, 0, H + 30 + 122), (6, 46, 156))
    w.base = None
    k.save(path)


def tumbleweed(path):
    """A tumbleweed rolling along beside the road, 40 m and round again. Bigger than life (1.4 m across): at racing
    speed a true-size one is a speck."""
    k = Kit()
    run = 4000.0
    g = k.group(('Spin', 'Conveyor'), rough=0.9, SpinPivot=(0.0, 0.0, 0.0), SpinAxis=(0.0, 1.0, 0.0), SpinRate=3.2, SpinRand=1.0,
                ConvStart=(-run / 2, 0.0, 70.0), ConvDir=(1.0, 0.0, 0.0), ConvLen=run, ConvRate=0.05, ConvRand=1.0)
    r = random.Random(9)

    def draw(m):
        m.sphere('#8a6a4a', (0, 0, 0), 44, steps=(4, 6))
        for i in range(9):
            th, ph = r.uniform(0, math.pi), r.uniform(0, 2 * math.pi)
            d = (math.sin(th) * math.cos(ph) * 68, math.sin(th) * math.sin(ph) * 68, math.cos(th) * 68)
            tilt_box(m, '#b08a5a' if i % 2 else '#9a6a44', (-d[0], -d[1], -d[2]), d, 7, 7)

    g.piece(draw)
    k.save(path, grow=(run / 2 + 90, 90, 160), grow_down=(run / 2 + 90, 90, 0))


def dust_devil(path):
    """A dust devil out on the flat: a leaning, turning column of dust that wanders a little."""
    k = Kit()
    g = k.group(('Spin', 'Sway'), far=True, rough=0.95, SpinPivot=(0.0, 0.0, 0.0), SpinAxis=(0.0, 0.0, 1.0), SpinRate=2.4, SpinRand=1.0,
                SwayAmp=300.0, SwayHeight=2700.0, SwayPow=1.3, SwayRate=0.6, SwayLean=0.35)
    cols = ('#e9c79a', '#d9b384', '#f2d6ae')
    for i in range(11):
        z, rad = 70 + i * 240, 70 + i * 36
        for j in range(3):
            a = math.radians(j * 120 + i * 37)
            g.sphere(cols[(i + j) % 3], (math.cos(a) * rad * 0.45, math.sin(a) * rad * 0.45, z), rad * 0.78, scale=(1, 1, 0.42), steps=(4, 7))
    for j in range(4):
        a = math.radians(j * 90 + 20)
        g.sphere(cols[j % 3], (math.cos(a) * 170, math.sin(a) * 170, 34), 150, scale=(1, 1, 0.22), steps=(4, 7))
    k.save(path, grow=(500, 500, 120), grow_down=(500, 500, 0))


def buzzards(path):
    """Three buzzards wheeling high over the desert."""
    k = Kit()
    g = k.group('Spin', far=True, SpinPivot=(0.0, 0.0, 0.0), SpinAxis=(0.0, 0.0, 1.0), SpinRate=0.28, SpinRand=1.0)
    for rad, z, a0 in ((900.0, 3300.0, 0.0), (1200.0, 3700.0, 140.0), (700.0, 4000.0, 250.0)):
        a = math.radians(a0)
        cx, cy = math.cos(a) * rad, math.sin(a) * rad
        tx, ty = -math.sin(a), math.cos(a)          # the way it flies (round the circle)
        rx, ry = math.cos(a), math.sin(a)
        tilt_box(g, '#2a2230', (cx - tx * 50, cy - ty * 50, z), (cx + tx * 50, cy + ty * 50, z), 16, 12)
        for sgn in (-1, 1):
            tilt_box(g, '#2a2230', (cx, cy, z), (cx + sgn * rx * 120, cy + sgn * ry * 120, z + 30), 40, 5)
            tilt_box(g, '#3e3244', (cx + sgn * rx * 120, cy + sgn * ry * 120, z + 30), (cx + sgn * rx * 200, cy + sgn * ry * 200, z + 16), 30, 4)
    k.save(path)


def firework(path):
    """Three shells bursting in turn, far off over the park: every spark is a part sent out from its shell."""
    k = Kit()
    r = random.Random(11)
    for col, base, ph in (('#ff4fb8', (0.0, 0.0, 5200.0), 0.0), ('#2ee6ff', (-900.0, 700.0, 6100.0), 0.37), ('#ffe14d', (700.0, -500.0, 4700.0), 0.68)):
        g = k.group('Burst', far=True, BurstBase=base, BurstRadius=2300.0, BurstFall=620.0, BurstRate=0.15, BurstLife=0.36, BurstPhase=ph)
        for i in range(34):
            g.piece(lambda m, c=col: m.sphere(c, (0, 0, 0), 85, steps=(3, 5), glow=3.0), a=r.random(), b=r.uniform(-0.25, 0.95), c=r.random())
    k.save(path, grow=(3300, 3100, 8500), grow_down=(3300, 3100, 0))


def bunting(path, cols, pole, knob):
    """A line of pennants across two poles, each fluttering from its top edge. The line runs across the road's
    direction, so the driver sees the flags face on."""
    k = Kit()
    L, H, n = 900.0, 520.0, 9
    turn = ft_art._xf((0, 0, 0), (0, 90, 0))     # drawn along X, then turned to run along Y
    s = k.group()
    s.base = turn
    for sx in (-1, 1):
        s.cyl(pole, (sx * L / 2, 0, 0), 9, H, steps=6)
        s.sphere(knob, (sx * L / 2, 0, H + 10), 15, steps=(4, 6), glow=0.4)
    rod(s, '#e8e0f0', (-L / 2, 0, H - 20), (L / 2, 0, H - 20), 2.5, steps=4)
    f = k.group('Flutter', rough=0.8, FlutterPin=(0.0, 0.0, H - 20.0), FlutterAxis=(0.0, 0.0, -1.0), FlutterDir=(1.0, 0.0, 0.0), FlutterSpan=70.0,
                FlutterLen=110.0, FlutterRate=9.0, FlutterAmp=13.0, FlutterPhase=(0.0, 0.021, 0.0))
    f.base = turn
    for i in range(n):
        x = -L / 2 + L * (i + 0.5) / n
        f.side(cols[i % len(cols)], [(x - 40, H - 20), (x + 40, H - 20), (x, H - 110)], -1.5, 1.5)
    k.save(path, grow=(30, 0, 0), grow_down=(30, 0, 0))


def searchlight(path):
    """A searchlight sweeping the night sky from the far side of the park. The beam is a solid (the cel pass inks
    whatever it is given, and a see-through one would be drawn after it), so it is painted the way a cel painter would
    paint light: slim, in lengths that step down from warm white at the lamp to the violet of the upper sky, where
    it is lost. One plain cream cone read as a white pole (first look, 15 Nov)."""
    k = Kit()
    s = k.group()
    s.cyl('#3a2f6a', (0, 0, 0), 95, 130, steps=10)
    s.cyl('#ffe14d', (0, 0, 130), 74, 16, steps=10)
    b = k.group('Spin', far=True, rough=0.9, SpinPivot=(0.0, 0.0, 210.0), SpinAxis=(1.0, 0.0, 0.0), SpinSwing=24.0, SpinRate=0.42, SpinRand=1.0)
    b.cyl('#2b2350', (0, 0, 160), 62, 110, steps=10)
    z, rad = 270.0, 16.0
    for col, glow, h in (('#fff1c0', 0.3, 1100.0), ('#f3c4dc', 0.16, 1400.0), ('#c391d8', 0.1, 1700.0), ('#8662b8', 0.08, 1700.0), ('#523f8c', 0.06, 1400.0)):
        top = rad + h * 0.013
        b.cyl(col, (0, 0, z), rad, h, steps=8, top_r=top, glow=glow)
        z, rad = z + h, top
    k.save(path, grow=(200, 3300, 0), grow_down=(200, 3300, 0))


def gondola(path):
    """A stretch of gondola lift: cabins going up one cable and down the other between two pylons."""
    k = Kit()
    s = k.group()
    # tall: the rules stand props at the road's height, and out where this one goes the snowfield has risen 10-16 m,
    # so the bottom third of the pylons is under the snow (as the level's own ridges are composed half buried)
    L, H = 4200.0, 3000.0
    for sx in (-1, 1):
        s.cyl('#9aa4b8', (sx * L / 2, 0, 0), 60, H, steps=8, top_r=28)
        s.box('#5a6478', (sx * L / 2, 0, H), (56, 520, 44))
        s.sphere('#ff2d3c', (sx * L / 2, 0, H + 44), 20, steps=(4, 6), glow=1.6)
    for sy in (-1, 1):
        rod(s, '#3a3846', (-L / 2, sy * 200, H - 40), (L / 2, sy * 200, H - 40), 4, steps=4)
    for sy, col, d in ((-1, '#ff2d3c', 1.0), (1, '#2e6bff', -1.0)):
        g = k.group('Conveyor', far=True, ConvStart=(-d * L / 2, sy * 200.0, H - 40.0), ConvDir=(d, 0.0, 0.0), ConvLen=L, ConvRate=0.035, ConvRand=1.0)
        for i in range(3):
            def cabin(m, c=col):
                rod(m, '#5a6478', (0, 0, -120), (0, 0, 0), 5, steps=4)
                m.box(c, (0, 0, -205), (190, 130, 150))
                m.box('#bfe3ff', (0, 0, -188), (194, 134, 58))
                m.box('#f4f9ff', (0, 0, -124), (200, 140, 14))
            g.piece(cabin, a=i / 3.0)
    k.save(path, grow=(0, 0, 0), grow_down=(0, 0, 0))


def spindrift(path):
    """Snow blowing off a crest: a plume laid over by the wind, up where the ridges are. Out there (265 m and more)
    the snowfield stands 22-26 m above the road and a prop stands at the road's height, so the plume starts at 33 m:
    at 21 m (the first build) it was inside the snow."""
    k = Kit()
    g = k.group('Plume', far=True, rough=0.9, PlumeBase=(0.0, 0.0, 3300.0), PlumeRise=420.0, PlumeDrift=2600.0, PlumeSize0=0.6, PlumeSize1=3.2,
                PlumeRate=0.2, PlumeSpread=170.0, PlumeEase=0.6)
    for i in range(9):
        g.piece(lambda m: m.sphere('#f4f9ff', (0, 0, 0), 110, scale=(1.3, 1, 0.7), steps=(4, 7)), a=i / 9.0, b=0.8 + 0.5 * ((i * 5) % 4) / 3.0, c=i * 0.61)
    k.save(path, grow=(3500, 3500, 4500), grow_down=(3500, 3500, 0))


def far_traffic(path):
    """A length of elevated expressway behind the towers: traffic as streaks of light, white coming and red going.
    It is never nearer than 100 m and the towers hide most of it, so it is drawn for where it does show (down the
    street's far end on a bend, and through the gaps): high up, long, and the cars as light trails 5 m long. The first
    one (a deck 7 m up, car-sized lamps) could not be found in a frame."""
    k = Kit()
    s = k.group()
    L, Z = 6000.0, 2600.0
    s.box('#33304e', (0, 0, Z), (L, 520, 90))
    for sx in (-1.0, 0.0, 1.0):
        s.box('#2a2840', (sx * L / 3, 0, (Z - 45) / 2), (170, 260, Z - 45))
    for sy in (-1, 1):
        s.box('#3a3456', (0, sy * 255, Z + 70), (L, 18, 50))
        s.box('#9ff0ff', (0, sy * 268, Z + 10), (L, 6, 16), glow=0.5)
    for sy, col, d, glow in ((-1, '#fff1c0', 1.0, 2.6), (1, '#ff2d3c', -1.0, 2.2)):
        g = k.group('Conveyor', far=True, ConvStart=(-d * L / 2, sy * 120.0, Z + 50.0), ConvDir=(d, 0.0, 0.0), ConvLen=L, ConvRate=0.33)
        r = random.Random(3 + int(sy))
        for i in range(8):
            g.piece(lambda m, c=col, gl=glow: m.box(c, (0, 0, 34), (500, 96, 60), glow=gl), a=(i + r.uniform(-0.3, 0.3)) / 8.0)
    k.save(path)


PROPS = [
    (steam_vent, (CHEM + 'SM_VentStack', True, '#2a3f8f', '#ffd23f', '#1c1f4a', '#d8d0f0'),
     dict(top=620.0, rise=1150.0, drift=520.0, r=60.0, n=8, rate=0.22, cycle=0.075, duty=0.55, size1=2.6)),
    (pumpjack, (CHEM + 'SM_Pumpjack', '#2a3f8f', '#ffd23f', '#1c1f4a'), {}),
    (fan_unit, (CHEM + 'SM_FanUnit', '#19b5c9', '#ffd23f', '#1c1f4a', '#eef6ff'), {}),
    (flare, (CHEM + 'SM_Flare',), {}),
    (windpump, (NEV + 'SM_Windpump',), {}),
    (pumpjack, (NEV + 'SM_Pumpjack', '#e0784a', '#2b2350', '#22202a'), dict(rate=1.3)),
    (tumbleweed, (NEV + 'SM_Tumbleweed',), {}),
    (dust_devil, (NEV + 'SM_DustDevil',), {}),
    (buzzards, (NEV + 'SM_Buzzards',), {}),
    (firework, (PARK + 'SM_Firework',), {}),
    (bunting, (PARK + 'SM_Bunting', ('#ff2d3c', '#ffe14d', '#2ee6ff', '#ff4fb8', '#3dff6a'), '#fff6ea', '#ffc93c'), {}),
    (searchlight, (PARK + 'SM_Searchlight',), {}),
    (gondola, (ICE + 'SM_Gondola',), {}),
    (spindrift, (ICE + 'SM_Spindrift',), {}),
    (bunting, (ICE + 'SM_Bunting', ('#ff2d3c', '#f4f9ff', '#2e6bff', '#ffe14d'), '#6a4c36', '#ff2d3c'), {}),
    (steam_vent, (CITY + 'SM_SteamGrate', False, '#3a3848', '#3a3848', '#24203a', '#aeb6d2'),
     dict(top=16.0, rise=720.0, drift=230.0, r=44.0, n=9, rate=0.3, size1=2.3, emit=0.09)),
    (far_traffic, (CITY + 'SM_FarTraffic',), {}),
]


def props(only=None):
    """Build the new props (all of them, or those whose asset name contains `only`). One that fails is reported and
    the rest are still built."""
    import traceback
    bad = []
    for fn, args, kw in PROPS:
        if only and only not in args[0]:
            continue
        try:
            fn(*args, **kw)
        except Exception as e:
            bad.append(args[0])
            unreal.log_error('FTLIFE prop %s failed: %r %s' % (args[0], e, traceback.format_exc()))
    log('props built', 'FAILED: %s' % bad if bad else 'all')
    return bad


# ------------------------------------------------------------------------------------------ the levels' own props
# What moves on the props each level already has, level by level. Every call names a mesh, a way of moving, which
# material slots (by the colour the level's builder gave them) and the numbers. Run again after ft_export re-exports
# a level: the export puts the palette instances back on the slots.
BLINK = dict(LightRate=0.7, LightDuty=0.45, LightSoft=0.06, LightMin=0.12, LightRand=1.0)      # a beacon
PULSE = dict(LightRate=0.3, LightDuty=0.5, LightSoft=0.5, LightMin=0.5, LightRand=1.0)         # a slow swell


def _chase(axis, length, rate=0.8, duty=0.45, low=0.25, rand=0.0):
    """Lights that run along an axis of the mesh: `length` cm from one lit stretch to the next."""
    return dict(LightAxis=axis, LightLen=length, LightRate=rate, LightDuty=duty, LightSoft=0.1, LightMin=low, LightRand=rand)


def _windows(cell, rate=0.1, off=0.22, low=0.12):
    """Windows that go on and off one by one: `cell` cm is one window, `off` the share dark at any moment."""
    return dict(LightHash=off, LightCell=cell, LightRate=rate, LightMin=low, LightRand=1.0)


def _glow_is(v):
    return lambda slot, colour, glow: abs(glow - v) < 0.011


def chem():
    D = '/Game/Maps/ChemicalWorks/Props/'
    # large: the stacks' beacons blink and their smoke churns and leans; the far plant's lights come and go
    animate(D + 'SM_Stack', 'Light', ['#ff3355*'], **BLINK)
    animate(D + 'SM_Stack', ('Sway', 'Breathe'), ['#d8d0f0'], far=True, SwayBase=3700.0, SwayHeight=800.0, SwayAmp=130.0, SwayPow=1.2,
            SwayRate=0.5, SwayLean=0.5, BreatheAmp=22.0, BreatheRate=0.9, BreatheVar=1.5)
    animate(D + 'SM_Sphere', 'Light', ['#ff3355*'], **dict(BLINK, LightRate=0.9))
    animate(D + 'SM_Skyline', 'Light', ['#ff3355*'], **BLINK)
    animate(D + 'SM_Skyline', 'Light', ['#5ff0ff*', '#ffd23f*'], **_windows(420.0, rate=0.12, off=0.2, low=0.2))
    # medium: lights climb the columns, liquid runs in the pipes, the vats simmer and their stirrers turn
    animate(D + 'SM_Tower', 'Light', ['#5ff0ff*'], **_chase((0.0, 0.0, 1.0), 1300.0, rate=0.5, low=0.25, rand=1.0))
    animate(D + 'SM_Tower', 'Light', ['#b44dff*'], **PULSE)
    for name in ('SM_Tank', 'SM_Tank2'):
        animate(D + name, 'Light', ['#5ff0ff*'], **dict(PULSE, LightRate=0.22, LightMin=0.45))
    animate(D + 'SM_Pipework', 'Light', ['#e84aff-'], FlowMix=1.0, Color2=(1.0, 0.5, 1.0), **_chase((0.0, 1.0, 0.0), 300.0, rate=0.7, rand=1.0))
    animate(D + 'SM_Pipework', 'Light', ['#5ff0ff*'], **PULSE)
    animate(D + 'SM_Vat', 'Light', ['#b44dff*'], **dict(PULSE, LightMin=0.6))
    animate(D + 'SM_Vat', ('Bob', 'Breathe'), ['#e3b8ff*'], BobAmp=10.0, BobRate=0.5, BobPhase=(0.004, 0.003, 0.0), BreatheAmp=9.0, BreatheRate=2.2,
            BreatheVar=3.0)
    animate(D + 'SM_Vat', 'Spin', ['#8f9bb3'], SpinRate=1.1, MaskMin=(-170.0, -170.0, 385.0), MaskMax=(170.0, 170.0, 900.0))


def nevada():
    D = '/Game/Maps/Nevada/Props/'
    # large: Vegas on the horizon winks sign by sign
    animate(D + 'SM_Vegas', 'Light', ['glow'], **_windows(900.0, rate=0.2, off=0.18, low=0.3))
    # medium: the joshua trees and cacti stir, the motel's sign runs, the casino pylon's rings climb and its dice turn
    animate(D + 'SM_Joshua', 'Sway', SwayAmp=9.0, SwayHeight=620.0, SwayPow=2.0, SwayRate=1.1, SwayLean=0.1)
    animate(D + 'SM_Joshua2', 'Sway', SwayAmp=11.0, SwayHeight=780.0, SwayPow=2.0, SwayRate=1.0, SwayLean=0.1)
    animate(D + 'SM_Cactus', 'Sway', SwayAmp=4.0, SwayHeight=200.0, SwayPow=1.6, SwayRate=0.9, SwayLean=0.0)
    animate(D + 'SM_Motel', 'Light', ['#fff3a8*'], **_chase((0.0, 1.0, 0.0), 208.0, rate=1.5, low=0.2))
    animate(D + 'SM_Motel', 'Light', ['#ff4fb8*'], **dict(BLINK, LightRate=0.9, LightDuty=0.6, LightMin=0.3))
    animate(D + 'SM_Motel', 'Light', ['#ffe14d*'], **dict(BLINK, LightRate=1.6, LightDuty=0.5, LightMin=0.3))
    animate(D + 'SM_Motel', 'Light', ['#2ee6ff*'], **PULSE)
    animate(D + 'SM_Pylon', 'Light', ['#ff4fb8*', '#2ee6ff*', '#ffc93c*'], **_chase((0.0, 0.0, 1.0), 420.0, rate=0.8, low=0.45))
    animate(D + 'SM_Pylon', 'Light', ['#fff3a8*'], **_chase((0.0, 1.0, 0.0), 140.0, rate=1.2, low=0.3))
    animate(D + 'SM_Pylon', 'Spin', ['#ff2d3c-', '#fff6ea*'], SpinPivot=(0.0, 0.0, 1900.0), SpinRate=0.6, MaskMin=(-400.0, -400.0, 1730.0),
            MaskMax=(400.0, 400.0, 2200.0))
    animate(D + 'SM_Gas', 'Light', ['#ff3355*'], **PULSE)


def park():
    D = '/Game/Maps/ThemePark/Props/'
    # large: the drop tower's car climbs and drops and its lights run up the shaft; lights run along the coaster;
    # the park on the horizon twinkles
    animate(D + 'SM_DropTower', 'Bob', ['#3a2f6a', '#ffe14d-', '#ff2d3c-'], far=True, BobShape=1.0, BobAmp=1400.0, BobRate=0.09, BobRand=1.0,
            MaskMin=(-400.0, -400.0, 2594.0), MaskMax=(400.0, 400.0, 2776.0))
    animate(D + 'SM_DropTower', 'Light', ['#ff4fb8*', '#2ee6ff*', '#ffe14d*'], **_chase((0.0, 0.0, 1.0), 1008.0, rate=0.8, low=0.25))
    animate(D + 'SM_DropTower', 'Light', ['#ff2d3c*'], **BLINK)
    animate(D + 'SM_Lattice', 'Light', ['#ff4fb8*'], **_chase((0.0, 1.0, 0.0), 900.0, rate=0.6, duty=0.35, low=0.25, rand=1.0))
    animate(D + 'SM_ParkSkyline', 'Light', ['glow'], **_windows(500.0, rate=0.3, off=0.2, low=0.3))
    # medium: the carousel turns (its bulbs twinkle), balloons tug at their strings, the tent's pennant flies, trees stir
    spin = dict(SpinRate=0.55, SpinRand=1.0)
    animate(D + 'SM_Carousel', ('Spin', 'Light'), ['#ffe28a*'], LightHash=0.3, LightCell=60.0, LightRate=2.0, LightMin=0.35, **spin)
    animate(D + 'SM_Carousel', 'Spin', lambda slot, colour, glow: not (colour == 'ffe28a' and glow > 0), **spin)
    animate(D + 'SM_Balloons', 'Sway', SwayBase=110.0, SwayHeight=320.0, SwayAmp=26.0, SwayPow=1.2, SwayRate=1.3, SwayLean=0.3)
    animate(D + 'SM_Tent', 'Flutter', ['#ff4fb8*'], FlutterPin=(0.0, 0.0, 0.0), FlutterAxis=(0.0, 1.0, 0.0), FlutterDir=(1.0, 0.0, 0.0), FlutterSpan=60.0,
            FlutterLen=1.0e6, FlutterRate=9.0, FlutterAmp=10.0)
    animate(D + 'SM_Tent', 'Light', ['#ffe28a*'], LightHash=0.3, LightCell=60.0, LightRate=2.0, LightMin=0.4)
    animate(D + 'SM_Booth', 'Light', ['#ffe14d*'], **dict(BLINK, LightRate=1.2, LightDuty=0.55, LightMin=0.3))
    animate(D + 'SM_Tree', 'Sway', SwayAmp=7.0, SwayHeight=500.0, SwayPow=1.6, SwayRate=1.2, SwayLean=0.1)


def glacier():
    D = '/Game/Maps/Glacier/Props/'
    # large: snow streams off the summit's cornice
    animate(D + 'SM_Peak', ('Sway', 'Breathe'), ['#fff6ea-'], far=True, SwayBase=4600.0, SwayHeight=500.0, SwayAmp=90.0, SwayPow=1.0, SwayRate=0.5,
            SwayLean=0.6, BreatheAmp=30.0, BreatheRate=0.7)
    # medium: the pines bend, the slalom gates' flags snap, the lift's chairs swing, the chalet's windows glow
    animate(D + 'SM_Pine', 'Sway', SwayAmp=26.0, SwayHeight=900.0, SwayPow=1.8, SwayRate=1.5)
    animate(D + 'SM_Pine2', 'Sway', SwayAmp=32.0, SwayHeight=1200.0, SwayPow=1.8, SwayRate=1.3)
    for name, col, side in (('SM_Gate', '#ff2d3c-', 1.0), ('SM_GateR', '#2e6bff-', -1.0)):
        animate(D + name, 'Flutter', [col, '#fff6ea*'], FlutterPin=(0.0, side * 10.0, 0.0), FlutterAxis=(0.0, side, 0.0), FlutterDir=(1.0, 0.0, 0.0),
                FlutterSpan=160.0, FlutterLen=1.0e6, FlutterRate=7.0, FlutterAmp=9.0, MaskMin=(-50.0, -200.0, 250.0), MaskMax=(50.0, 200.0, 410.0))
    animate(D + 'SM_Lift', 'Sway', ['#5a6478', '#3a3846', '#2e6bff', '#ff2d3c-'], SwayBase=1360.0, SwayHeight=-300.0, SwayAmp=22.0, SwayPow=1.0,
            SwayRate=1.6, SwayLean=0.0)
    animate(D + 'SM_Lift', 'Light', ['#ff2d3c*'], **BLINK)
    animate(D + 'SM_Chalet', 'Light', ['#ffe28a*'], **dict(PULSE, LightRate=0.35, LightMin=0.7))
    # small: ice glints. The snow has no glow of its own, so the "dark" share of the cells is what lights: a few
    # hand-sized patches at a time flash a cold white and move on
    glint = dict(Color2=(0.75, 0.92, 1.0), Glow2=0.22, LightRand=1.0)
    animate(D + 'SM_SnowRock', 'Light', ['#f4f9ff'], LightHash=0.05, LightCell=18.0, LightRate=2.5, **glint)
    animate(D + 'SM_Ridge', 'Light', ['#f4f9ff'], LightHash=0.04, LightCell=70.0, LightRate=2.0, **glint)


def metro():
    D = '/Game/Maps/Metro/Props/'
    # large: the Chairman's spire and the far skyline: beacons, and windows that go on and off
    animate(D + 'SM_Spire', 'Light', ['#ff2d3c*'], **dict(BLINK, LightRate=0.6, LightMin=0.1))
    animate(D + 'SM_Spire', 'Light', ['#ffc93c*'], **PULSE)
    animate(D + 'SM_Spire', 'Light', _glow_is(0.9), LightOffset=(0.0, 0.0, 180.0), **_windows(360.0, rate=0.08, off=0.25))
    animate(D + 'SM_Skyline', 'Light', _glow_is(0.9), **_windows(500.0, rate=0.08, off=0.25))
    # medium: the towers by the road: a floor's windows switch one by one, crowns pulse, the newsstand's sign glows
    for name in ('SM_Tower1', 'SM_Tower2', 'SM_Tower3', 'SM_Tower4'):
        animate(D + name, 'Light', _glow_is(0.9), LightOffset=(0.0, 0.0, 165.0), **_windows(330.0, rate=0.1, off=0.22))
        animate(D + name, 'Light', ['#ff2d3c*'], **BLINK)
        animate(D + name, 'Light', ['#ff4fb8*', '#2ee6ff*'], **PULSE)
    animate(D + 'SM_Newsstand', 'Light', ['#ff2d3c*'], **PULSE)


LEVELS = {1: chem, 2: nevada, 4: park, 5: glacier, 7: metro}


def apply_all(only=None):
    for lvl, fn in LEVELS.items():
        if only is None or lvl in only:
            fn()
    log('existing props animated')


def build_all():
    """Everything, in order: the masters and the air, the new props, then the levels' own props."""
    build_core()
    props()
    apply_all()
