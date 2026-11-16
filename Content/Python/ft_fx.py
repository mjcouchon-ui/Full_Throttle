"""FULL THROTTLE (Unreal) Niagara effects, built from the engine's lightweight (stateless) templates.

The emitter stacks can't be edited from Python, so this uses the project's editor tool
(UFTEditorTools.SetPath) to set the stateless modules' properties by reflection, then recompiles.
Run over the Unreal MCP connector:  import ft_fx; ft_fx.build_all()

The combat-effects overhaul (15 Nov 2026): M_FX_Toon and its instances are built here too (toon_materials): the fireball, the
flame, the smoke that erodes as the camera nears it, debris and the scorch a blast leaves on the road. The systems the
overhaul added or redrew are in overhaul_definitions() and marked "FX overhaul" where an older one was changed in place.
DevLog/jam/overhaul/fx/notes.md has the effect-by-effect table and the counts.
"""
import unreal

AL = unreal.EditorAssetLibrary
T = unreal.FTEditorTools
FX = '/Game/FX/'
LOOP = '/Niagara/DefaultAssets/Templates/Systems/FountainLightweight'
BURST = '/Niagara/DefaultAssets/Templates/Systems/DirectionalBurstLightweight'
E = 'EmitterHandles[0].StatelessEmitter.'


def _set(s, path, val):
    r = T.set_path(s, E + path, val)
    ok = r[0] if isinstance(r, tuple) else (r == '' or r is True)
    if not ok:
        raise RuntimeError('%s <- %s : %s' % (path, val, r))


def _mods(s):
    out = {}
    i = 0
    while True:
        v = T.get_path(s, E + 'Modules[%d]' % i)
        if v.startswith('ERROR'):
            return out
        out[v.split("'")[0].split('.')[-1].replace('NiagaraStatelessModule_', '')] = i
        i += 1


def make(name, base, cfg):
    path = FX + name
    s = AL.load_asset(path) if AL.does_asset_exist(path) else AL.duplicate_asset(base, path)
    if s is None:
        raise RuntimeError('could not load or create ' + path)
    mods = _mods(s)
    want = set(cfg.get('modules', {}).keys())
    for mname, idx in mods.items():
        _set(s, 'Modules[%d].bModuleEnabled' % idx, 'True' if mname in want else 'False')
    for mname, props in cfg.get('modules', {}).items():
        idx = mods[mname]
        for k, v in props.items():
            _set(s, 'Modules[%d].%s' % (idx, k), v)
    for mname, props in cfg.get('soft', {}).items():
        # FX overhaul: a module this build may not take as written. The system is still built, without it, and says so
        try:
            _set(s, 'Modules[%d].bModuleEnabled' % mods[mname], 'True')
            for k, v in props.items():
                _set(s, 'Modules[%d].%s' % (mods[mname], k), v)
        except Exception as e:  # noqa
            print('JAM SOFT %s: %s not set (%s)' % (name, mname, repr(e)[:200]))
            if mname in mods:
                T.set_path(s, E + 'Modules[%d].bModuleEnabled' % mods[mname], 'False')
    for k, v in cfg.get('emitter', {}).items():
        _set(s, k, v)
    for k, v in cfg.get('renderer', {}).items():
        _set(s, 'RendererProperties[0].' + k, v)
    if 'pool' in cfg:
        # components made ahead of the first shot, and a cap, so a burst of fire does not grow the pool mid-race
        s.set_editor_property('pool_prime_size', cfg['pool'][0])
        s.set_editor_property('max_pool_size', cfg['pool'][1])
    T.compile_niagara(s)
    AL.save_loaded_asset(s)
    return s


def rng(a, b):
    return '(Min=%f,Max=%f,Mode=UniformRange,ChannelConstantsAndRanges=(%f,%f))' % (a, b, a, b)


def const(a):
    return '(Min=%f,Max=%f,Mode=UniformConstant,ChannelConstantsAndRanges=(%f))' % (a, a, a)


def vconst(x, y, z):
    return '(Min=(X=%f,Y=%f,Z=%f),Max=(X=%f,Y=%f,Z=%f),Mode=NonUniformConstant,ChannelConstantsAndRanges=(%f,%f,%f))' % (x, y, z, x, y, z, x, y, z)


def v2const(x, y):
    return '(Min=(X=%f,Y=%f),Max=(X=%f,Y=%f),Mode=NonUniformConstant,ChannelConstantsAndRanges=(%f,%f))' % (x, y, x, y, x, y)


def v2rng(a, b):
    return '(Min=(X=%f,Y=%f),Max=(X=%f,Y=%f),Mode=UniformRange,ChannelConstantsAndRanges=(%f,%f))' % (a[0], a[1], b[0], b[1], a[0], b[0])


def _curve(vals):
    n = len(vals)
    keys = ','.join('(InterpMode=RCIM_Linear,Time=%f,Value=%f)' % (i / float(max(n - 1, 1)), v) for i, v in enumerate(vals))
    return '(Keys=(%s))' % keys


def colors(*cols):
    ch = ','.join(_curve([c[k] for c in cols]) for k in range(4))
    return '(Mode=ColorGradient,ValuesTimeRange=(X=0.000000,Y=1.000000),ChannelCurves=(%s))' % ch


def sizes(*xs):
    ch = ','.join(_curve(list(xs)) for _ in range(2))
    return '(Mode=NonUniformCurve,ValuesTimeRange=(X=0.000000,Y=1.000000),ChannelCurves=(%s))' % ch


def mat(m):
    return "/Script/Engine.Material'%s'" % m


BIG = '(Min=(X=-3000,Y=-3000,Z=-3000),Max=(X=3000,Y=3000,Z=3000),IsValid=True)'
HOT = ((7, 5.7, 3.5, 1), (6.5, 2.6, 0.45, 1), (3.4, 0.58, 0.11, 0.78), (0.7, 0.09, 0.06, 0.3), (0, 0, 0, 0))

# ---------------------------------------------------------------------------------------------- 3.1 attack effects
# The cel sprite material (ft_attackfx.py, M_FX_Cel): flat banded shapes as on the effects sheet. Its instances:
# Burst (banded octagon with rays), Flash (a spiky star), Blob (a banded blob), Ring, Puff (two-tone smoke), Shard.
# For these the particle's colour is the shape's middle band and its alpha is its heat (1 = new, 0 = gone).
SPARK = mat('/Game/FX/M_FX_Spark.M_FX_Spark')
GLOW = mat('/Game/FX/M_FX_Glow.M_FX_Glow')


def cel(kind):
    return "/Script/Engine.MaterialInstanceConstant'/Game/FX/MI_FX_Cel%s.MI_FX_Cel%s'" % (kind, kind)


def heat(*keys):
    """A cel colour ramp: (rgb, heat) keys, evenly spaced over the particle's life."""
    return [(c[0], c[1], c[2], h) for c, h in keys]


# ---------------------------------------------------------------------------------------------- FX overhaul: the toon sprites
# M_FX_Toon: the overhaul's own sprite material, beside M_FX_Cel (ft_attackfx.py) and in its language: flat bands, hard
# edges, no soft fades. As there, the particle's alpha is heat (1 = new, 0 = gone). What is new:
#   Shape 0 fire     a billowing blob (scalloped like a cartoon blast cloud, Spike < 0) or a flame with pointed tongues
#                    on its upper side (Spike > 0, Up). Its bands are temperature: they cool from the middle outwards as
#                    the heat falls, cream to yellow, orange, red, maroon and soot, so a fireball turns to dark smoke
#                    before it goes. Pal 1 uses the effects sheet's own fire colours; Pal 0 works the bands out of the
#                    particle's colour (the SFX DEPT's blue and toxic wrecks).
#   Shape 1 smoke    a faceted two-tone puff with a dark belly, as on the sheet
#   Shape 2 chunk    a flat piece of debris, three to five sides, with an ember edge while it is hot
#   Shape 3 scorch   the soot a blast leaves on the road (drawn on a ground-facing sprite)
# Every shape goes the same way: it shrinks, notches cut in from the rim until it is separate lumps, and a hole opens
# in the middle (Break). And every shape does the same as the camera nears it (NearA..NearB, cm from the lens): its
# heat is scaled down, so it breaks up and is gone before the lens reaches it. Smoke never fills the screen, never
# stands between the chase camera and the car, and the road just ahead of the car is always clear of it.
TOON_HLSL = r"""
float2 q = (UV - 0.5) * 2.0;
float r = length(q);
float a = atan2(q.y, q.x);
float rnd = Rand;
float nearK = saturate((Depth - NearA) / max(NearB - NearA, 1.0));
float heat = saturate(PC.a) * nearK;
float3 mid = PC.rgb;
float aa = max(fwidth(r) * 1.2, 0.006);
float3 warm = pow(max(mid, 0.0001), 0.5);
float3 hot = lerp(warm, float3(1.0, 0.95, 0.80), 0.75) * CoreGain;
float3 rimc = pow(max(mid, 0.0001), 1.6) * 0.75;
float3 edgec = rimc * 0.55;
float3 soot = lerp(edgec * 0.4, float3(0.09, 0.07, 0.11), 0.7);
if (Pal > 0.5)
{
    // the effects sheet's own fire bands (linear): cream, yellow, orange, red, maroon, soot
    hot = float3(1.0, 0.87, 0.53) * CoreGain;
    warm = float3(1.0, 0.58, 0.05);
    mid = float3(1.0, 0.27, 0.02);
    rimc = float3(0.70, 0.045, 0.03);
    edgec = float3(0.22, 0.03, 0.05);
    soot = float3(0.07, 0.045, 0.085);
}
float3 col = mid;
float op = 0.0;
// the outline every shape is cut from: a polygon (Sides) or a round blob, with slow lobes
float seg = 6.2831853 / max(Sides, 3.0);
float ap = fmod(a + rnd * 6.2831853 + 62.831853, seg) - seg * 0.5;
float poly = Sides > 2.5 ? cos(seg * 0.5) / max(cos(ap), 0.2) : 1.0;
float lob = 1.0 + Lobe * (0.105 * sin(a * 5.0 + rnd * 40.0) + 0.075 * sin(a * 3.0 - rnd * 17.0) + 0.05 * sin(a * 9.0 + rnd * 9.0));
// how it goes: it shrinks, notches cut in from the rim until it is separate lumps, a hole opens in the middle
float nN = 3.0 + step(0.5, frac(rnd * 7.31));
float notch = pow(saturate(0.5 + 0.5 * sin(a * nN + rnd * 31.0)), 3.0);
float late = 1.0 - smoothstep(0.0, 0.55, heat);
float shrink = 0.45 + 0.55 * smoothstep(0.0, 0.5, heat);
float R = Body * poly * lob * shrink * (1.0 - Break * late * notch);
float holeR = Break * (1.0 - smoothstep(0.0, 0.35, heat)) * 0.5 * Body;
if (Shape < 0.5)
{
    float nT = 6.0 + floor(frac(rnd * 3.7) * 3.99);
    float sw = abs(sin(a * nT * 0.5 + rnd * 50.0));
    float edge = Spike < 0.0 ? pow(sw, 0.6) : 1.0 - sw;
    float sUp = -q.y / max(r, 0.001);
    float w = lerp(1.0, smoothstep(-0.3, 0.5, sUp), saturate(Up * 4.0));
    float Rf = R * (1.0 - abs(Spike) * w * (1.0 - edge)) * (1.0 + Up * (sUp - 0.25));
    float d = r / max(Rf, 0.001);
    float da = aa / max(Rf, 0.05);
    float temp = heat * (1.30 - 1.20 * d);
    col = temp > 0.78 ? hot : (temp > 0.58 ? warm : (temp > 0.40 ? mid : (temp > 0.24 ? rimc : (temp > 0.10 ? edgec : soot))));
    if (d > 1.0 - Ink) col = edgec * 0.5;
    op = (1.0 - smoothstep(1.0 - da, 1.0 + da, d)) * smoothstep(holeR - aa, holeR + aa, r);
}
else if (Shape < 1.5)
{
    float d = r / max(R, 0.001);
    float da = aa / max(R, 0.05);
    op = (1.0 - smoothstep(1.0 - da, 1.0 + da, d)) * smoothstep(holeR - aa, holeR + aa, r);
    float lit = step(0.0, -q.y * 0.85 - q.x * 0.35 + 0.16 + 0.10 * sin(a * 4.0 + rnd * 23.0));
    float belly = step(0.52, q.y * 0.9 + q.x * 0.2 + 0.12 * sin(a * 3.0 + rnd * 11.0));
    col = lerp(mid * 0.72, mid * 1.24, lit);
    col = lerp(col, mid * 0.46, belly);
    if (d > 1.0 - Ink) col = mid * 0.40;
}
else if (Shape < 2.5)
{
    float cn = 3.0 + floor(frac(rnd * 5.3) * 2.99);
    float cseg = 6.2831853 / cn;
    float cap = fmod(a + rnd * 6.2831853 + 62.831853, cseg) - cseg * 0.5;
    float d = r * cos(cap) / cos(cseg * 0.5) / max(Body, 0.05);
    float da = aa / max(Body, 0.05);
    op = 1.0 - smoothstep(1.0 - da, 1.0 + da, d);
    float lit = step(0.0, q.x * cos(rnd * 12.0) + q.y * sin(rnd * 12.0));
    col = lerp(mid * 0.7, mid * 1.5, lit);
    float glow = smoothstep(0.45, 0.85, heat);
    if (d > 0.84) col = lerp(col, float3(1.0, 0.33, 0.04), glow);
}
else
{
    float sN = 9.0 + floor(frac(rnd * 4.1) * 4.99);
    float ray = pow(saturate(0.5 + 0.5 * sin(a * sN + rnd * 21.0)), 2.5) * (0.6 + 0.4 * sin(a * 2.0 + rnd * 5.0));
    float Rs = Body * shrink * (0.52 + 0.40 * ray + 0.08 * sin(a * 3.0 + rnd * 13.0));
    float d = r / max(Rs, 0.001);
    float da = aa / max(Rs, 0.05);
    op = 1.0 - smoothstep(1.0 - da, 1.0 + da, d);
    col = r < 0.30 * Body * shrink ? mid * 0.55 : mid;
}
op *= smoothstep(0.0, 0.06, heat);
return float4(col * Gain, op * Opacity);
"""

TOON_PARAMS = [('Shape', 0.0), ('Sides', 0.0), ('Body', 0.84), ('Lobe', 0.8), ('Spike', -0.26), ('Up', 0.0), ('Break', 0.75), ('Ink', 0.07),
               ('Pal', 0.0), ('Gain', 1.05), ('CoreGain', 1.25), ('Opacity', 1.0), ('NearA', 250.0), ('NearB', 700.0)]
# the instances the systems use: name -> scalar overrides. NearA..NearB (cm from the lens): fire only gives way when it
# is about to hit the lens; world smoke opens up from 20 m and is gone at 8 m (the car sits 8 to 11 m from the chase
# camera); SmokeCar is for what rides on the car itself (a backblast, landing dust) and only clears the lens.
TOON_INSTANCES = {
    'Fire': {'Pal': 1.0},
    'FireTint': {'Pal': 0.0},
    'Flame': {'Pal': 1.0, 'Body': 0.56, 'Lobe': 0.5, 'Spike': 0.55, 'Up': 0.55, 'Break': 0.6},
    'Smoke': {'Shape': 1.0, 'Sides': 8.0, 'Body': 0.80, 'Lobe': 0.45, 'Break': 0.8, 'Ink': 0.0, 'Gain': 1.0, 'CoreGain': 1.0, 'Opacity': 0.92,
              'NearA': 800.0, 'NearB': 2000.0},
    'SmokeCar': {'Shape': 1.0, 'Sides': 8.0, 'Body': 0.80, 'Lobe': 0.45, 'Break': 0.8, 'Ink': 0.0, 'Gain': 1.0, 'CoreGain': 1.0, 'Opacity': 0.80,
                 'NearA': 200.0, 'NearB': 520.0},
    'Chunk': {'Shape': 2.0, 'Body': 0.9, 'Gain': 1.0, 'NearA': 150.0, 'NearB': 400.0},
    'Scorch': {'Shape': 3.0, 'Body': 0.95, 'Gain': 1.0, 'Opacity': 0.82, 'NearA': 0.0, 'NearB': 1.0},
    # FX round 3: the smoke off a damaged foe (thin, and it gives way nearer the lens than a blast's does)
    'SmokeFoe': {'Shape': 1.0, 'Sides': 8.0, 'Body': 0.80, 'Lobe': 0.45, 'Break': 0.8, 'Ink': 0.0, 'Gain': 1.0, 'CoreGain': 1.0, 'Opacity': 0.86,
                 'NearA': 450.0, 'NearB': 1000.0},
}


def toon(kind):
    return "/Script/Engine.MaterialInstanceConstant'/Game/FX/MI_FX_Toon%s.MI_FX_Toon%s'" % (kind, kind)


def toon_materials():
    """M_FX_Toon and its instances. Returns the pixel-shader instruction count (0 = it did not compile)."""
    import ft_mats
    MEL = unreal.MaterialEditingLibrary
    m = ft_mats.new_material('/Game/FX', 'M_FX_Toon', blend=unreal.BlendMode.BLEND_TRANSLUCENT, shading=unreal.MaterialShadingModel.MSM_UNLIT, two_sided=True)
    for flag in ('used_with_niagara_sprites', 'used_with_particle_sprites'):
        # the cooked game falls back to the default material without the usage flag (README: material usage flags)
        m.set_editor_property(flag, True)
    X = -1100
    uv = ft_mats.expr(m, unreal.MaterialExpressionTextureCoordinate, x=X, y=-580)
    pc = ft_mats.expr(m, unreal.MaterialExpressionParticleColor, x=X, y=-500)
    rnd = ft_mats.expr(m, unreal.MaterialExpressionParticleRandom, x=X, y=-420)
    ins = [('UV', uv, ''), ('PC', pc, 'RGBA'), ('Rand', rnd, '')]
    code = TOON_HLSL
    depth = getattr(unreal, 'MaterialExpressionPixelDepth', None)
    if depth is not None:
        ins.append(('Depth', ft_mats.expr(m, depth, x=X, y=-340), ''))
    else:
        # no node for it in this build's Python: the same value, read in the custom node itself
        code = '\nfloat Depth = GetScreenPosition(Parameters).w;' + TOON_HLSL
    y = -240
    for nm, v in TOON_PARAMS:
        ins.append((nm, ft_mats.sparam(m, nm, v, x=X, y=y), ''))
        y += 70
    cu = ft_mats.custom(m, code, ins, out_type=unreal.CustomMaterialOutputType.CMOT_FLOAT4, x=-500, y=0, desc='ToonFX')
    rgb = ft_mats.expr(m, unreal.MaterialExpressionComponentMask, x=-200, y=-60)
    for ch, on in (('r', True), ('g', True), ('b', True), ('a', False)):
        rgb.set_editor_property(ch, on)
    alpha = ft_mats.expr(m, unreal.MaterialExpressionComponentMask, x=-200, y=80)
    for ch, on in (('r', False), ('g', False), ('b', False), ('a', True)):
        alpha.set_editor_property(ch, on)
    MEL.connect_material_expressions(cu, '', rgb, '')
    MEL.connect_material_expressions(cu, '', alpha, '')
    MEL.connect_material_property(rgb, '', unreal.MaterialProperty.MP_EMISSIVE_COLOR)
    MEL.connect_material_property(alpha, '', unreal.MaterialProperty.MP_OPACITY)
    MEL.layout_material_expressions(m)
    MEL.recompile_material(m)
    AL.save_loaded_asset(m)
    for name, scalars in TOON_INSTANCES.items():
        ft_mats.mi(m, '/Game/FX', 'MI_FX_Toon' + name, scalars=scalars)
    try:
        return MEL.get_statistics(m).get_editor_property('num_pixel_shader_instructions')
    except Exception:  # noqa
        return -1


def fx(n=None, rate=None, life=(0.2, 0.3), size=((50, 50), (80, 80)), mat=None, ramp=None, grow=None, speed=None, cone=180.0, axis=(0, 0, 1),
       inner=0.0, world=False, drag=None, gravity=None, sphere=None, box=None, rot=False, spin=None, stretch=None, align=False, sort=False,
       owner=True, pool=None, bounds=BIG, curl=None, ring=None, shape_scale=None, facing=None, toward=None):
    """One stateless emitter as a make() config. n = a burst of n sprites (the system ends by itself), rate = sprites a
    second for as long as the component is active. speed/cone/axis: a cone of velocity (cm/s, half-angle in degrees,
    direction in the emitter's own space, or in the world's with world=True: smoke off a tumbling wreck still rises).
    Every emitter here is local space (the engine's stateless emitters always are): sprites of a moving emitter travel
    with it, so a 'trail' is a fast cone straight back, and anything short-lived on a car is spawned attached to it.
    stretch = (speed at which, times) lengthens velocity-aligned sprites; sort for the translucent cel shapes.
    FX overhaul: curl = (strength cm/s, frequency) is the engine's curl noise (smoke that wanders and curls); ring = (radius,
    coverage 0..1) spawns on a ring or disc in the emitter's XY plane; shape_scale squashes the spawn shape; facing =
    (x, y, z) lays every sprite flat against that direction of the emitter's own space instead of facing the camera
    (a ring or a scorch on the road: spawn the system turned to the road's frame and give (0, 0, 1)); toward = cm draws
    every sprite that much nearer the camera than it is (fire that starts inside a wreck's body is seen in front of it)."""
    em = {'FixedBounds': bounds}
    if rate is not None:
        em['SpawnInfos'] = '((Type=Rate,Rate=(Min=%f,Max=%f)))' % (rate, rate)
    else:
        em['SpawnInfos'] = '((Amount=(Min=%d,Max=%d)))' % (n, n)
        d = life[1] + 0.15
        em['EmitterState'] = '(LoopBehavior=Once,LoopDuration=(Min=%f,Max=%f,Mode=UniformConstant))' % (d, d)
    mods = {'InitializeParticle': {'LifetimeDistribution': rng(*life), 'SpriteSizeDistribution': v2rng(*size),
                                   'SpriteRotationDistribution': rng(0, 360) if rot else const(0)}}
    if sphere is not None:
        mods['ShapeLocation'] = {'ShapePrimitive': 'Sphere', 'SphereRadius': rng(0, sphere)}
    elif box is not None:
        mods['ShapeLocation'] = {'ShapePrimitive': 'Box', 'BoxSize': vconst(*box)}
    elif ring is not None:
        mods['ShapeLocation'] = {'ShapePrimitive': 'Ring', 'RingRadius': const(ring[0]), 'DiscCoverage': const(ring[1])}
    if shape_scale is not None and 'ShapeLocation' in mods:
        mods['ShapeLocation']['ShapeScale'] = vconst(*shape_scale)
    if speed is not None:
        mods['AddVelocity'] = {'VelocityType': 'InCone', 'ConeAngle': '%f' % cone, 'InnerCone': '%f' % inner, 'ConeVelocityDistribution': rng(*speed),
                               'ConeRotationType': 'Direction', 'ConeDirection': vconst(*axis), 'CoordinateSpace': 'World' if world else 'Local'}
    if drag is not None:
        mods['Drag'] = {'DragDistribution': const(drag)}
    if gravity is not None:
        mods['GravityForce'] = {'GravityDistribution': vconst(0, 0, gravity)}
    if curl is not None:
        mods['CurlNoiseForce'] = {'NoiseStrength': '%f' % curl[0], 'NoiseFrequency': '%f' % curl[1]}
    if speed is not None or drag is not None or gravity is not None or curl is not None:
        mods['SolveVelocitiesAndForces'] = {}
    if ramp:
        mods['ScaleColor'] = {'ScaleDistribution': colors(*ramp)}
    if grow:
        mods['ScaleSpriteSize'] = {'ScaleDistribution': sizes(*grow)}
    if spin:
        mods['SpriteRotationRate'] = {'RotationRateDistribution': rng(-spin, spin)}
    if stretch:
        mods['ScaleSpriteSizeBySpeed'] = {'VelocityThreshold': const(stretch[0]), 'MinScaleFactor': v2const(1, 1), 'MaxScaleFactor': v2const(1, stretch[1])}
    if owner:
        mods['ApplyOwnerScaleToAttributes'] = {}
    cfg = {'emitter': em, 'modules': mods,
           'renderer': {'Material': mat, 'Alignment': 'VelocityAligned' if align else 'Unaligned', 'SortMode': 'ViewDepth' if sort else 'None'}}
    if facing is not None:
        mods['SpriteFacingAndAlignment'] = {'bSpriteFacingEnabled': 'True', 'bSpriteAlignmentEnabled': 'False', 'SpriteFacing': vconst(*facing)}
        cfg['renderer']['FacingMode'] = 'CustomFacingVector'
    if toward is not None:
        cfg['soft'] = {'CameraOffset': {'CameraOffsetDistribution': '(Mode=UniformConstant,ChannelConstantsAndRanges=(%f),Values=(%f))' % (toward, toward)}}
    if pool:
        cfg['pool'] = pool
    return cfg


# colours (linear): whose it is and what it is made of has to read at 120 mph
C_STD = (1.0, 0.74, 0.16)      # the KESTREL's standard rounds (the tracer's own yellow)
C_AP = (0.42, 0.80, 1.0)       # armour-piercing: blue-white
C_HE = (1.0, 0.44, 0.08)       # high explosive: orange
C_FOE = (1.0, 0.07, 0.26)      # theirs: hot red-magenta, on every hostile round, muzzle and wind-up
C_FIRE = (1.0, 0.40, 0.06)
C_EMBER = (0.85, 0.13, 0.05)
C_ICE = (0.62, 0.86, 1.0)
C_SNOW = (0.86, 0.93, 1.0)
C_ZAP = (0.50, 0.78, 1.0)
C_SEA = (0.16, 0.74, 0.95)
C_GOLD = (1.0, 0.76, 0.22)
C_WHITE = (1.0, 0.98, 0.92)
C_SMOKE = (0.26, 0.23, 0.33)   # the sheet's lavender-grey
C_SMOKE_L = (0.36, 0.32, 0.44)
C_SMOKE_D = (0.15, 0.13, 0.20)
C_DUST = (0.42, 0.36, 0.34)
C_CHUNK = (0.10, 0.09, 0.13)   # FX overhaul: debris, flat and dark with an ember edge
C_SOOT = (0.05, 0.04, 0.07)    # FX overhaul: the scorch a blast leaves on the road
C_EXHAUST = (0.95, 0.70, 0.45)  # FX overhaul: smoke still lit by the flame it came out of
C_DRIFT = ((0.25, 0.75, 1.0), (1.0, 0.55, 0.10), (1.0, 0.35, 0.75))   # FX overhaul: the drift's three charges: blue, orange, pink


def definitions():
    made = []
    made.append(('NS_ExplosionFlash', FX + 'NS_Explosion', {
        'emitter': {'SpawnInfos': '((Amount=(Min=1,Max=1)))', 'FixedBounds': BIG,
                    'EmitterState': '(LoopBehavior=Once,LoopDuration=(Min=0.16,Max=0.16,Mode=UniformConstant))'},
        'modules': {
            'InitializeParticle': {'LifetimeDistribution': const(0.12), 'SpriteSizeDistribution': v2const(268, 268)},
            'ScaleColor': {'ScaleDistribution': colors((4.5, 3.3, 2.1, 1), (3.2, 1.5, 0.42, 0.42), (1.5, 0.24, 0.05, 0))},
            'ScaleSpriteSize': {'ScaleDistribution': sizes(0.42, 1.05, 1.48)},
            'ApplyOwnerScaleToAttributes': {},
        },
        'renderer': {'Material': mat('/Game/FX/M_FX_Glow.M_FX_Glow')},
    }))
    # The short flash gives way to a low, lobed fireball, streaks and a cooler smoke tail.
    made.append(('NS_Explosion', BURST, {
        'emitter': {'SpawnInfos': '((Amount=(Min=31,Max=31)))', 'FixedBounds': BIG, 'EmitterState': '(LoopBehavior=Once,LoopDuration=(Min=2.0,Max=2.0,Mode=UniformConstant))'},
        'modules': {
            'InitializeParticle': {'LifetimeDistribution': rng(0.44, 1.15), 'SpriteSizeDistribution': v2rng((178, 155), (395, 295)), 'SpriteRotationDistribution': rng(0, 360)},
            'ShapeLocation': {'ShapePrimitive': 'Sphere'},
            'AddVelocity': {'VelocityType': 'InCone', 'ConeAngle': '180.0', 'ConeVelocityDistribution': rng(310, 820)},
            'Drag': {'DragDistribution': const(3.2)},
            'GravityForce': {'GravityDistribution': vconst(0, 0, 260)},
            'SolveVelocitiesAndForces': {},
            'ScaleColor': {'ScaleDistribution': colors(*HOT)},
            'ScaleSpriteSize': {'ScaleDistribution': sizes(0.53, 1.23, 1.61, 1.94)},
            'SpriteRotationRate': {'RotationRateDistribution': rng(-90, 90)},
            'ApplyOwnerScaleToAttributes': {},
        },
        'renderer': {'Material': mat('/Game/FX/M_FX_Fire.M_FX_Fire'), 'SortMode': 'ViewDepth'},
    }))
    made.append(('NS_ExplosionSparks', BURST, {
        'emitter': {'SpawnInfos': '((Amount=(Min=70,Max=70)))', 'FixedBounds': BIG, 'EmitterState': '(LoopBehavior=Once,LoopDuration=(Min=1.6,Max=1.6,Mode=UniformConstant))'},
        'modules': {
            'InitializeParticle': {'LifetimeDistribution': rng(0.28, 0.94), 'SpriteSizeDistribution': v2rng((7, 6), (13, 16))},
            'ShapeLocation': {'ShapePrimitive': 'Sphere'},
            'AddVelocity': {'VelocityType': 'InCone', 'ConeAngle': '180.0', 'ConeVelocityDistribution': rng(820, 2480)},
            'Drag': {'DragDistribution': const(0.9)},
            'GravityForce': {},
            'SolveVelocitiesAndForces': {},
            'ScaleColor': {'ScaleDistribution': colors((10, 6, 2, 1), (6, 2, 0.4, 1), (2, 0.3, 0.05, 0))},
            'ScaleSpriteSizeBySpeed': {'VelocityThreshold': const(1800), 'MinScaleFactor': v2const(1, 1), 'MaxScaleFactor': v2const(1, 16)},
            'ApplyOwnerScaleToAttributes': {},
        },
        'renderer': {'Material': mat('/Game/FX/M_FX_Spark.M_FX_Spark'), 'Alignment': 'VelocityAligned'},
    }))
    # 3.1: the sheet's explosion smoke, flat two-tone lavender puffs that linger and rise after the blast (it was a
    # soft brown haze). Fewer, bigger, longer: the car has left it behind long before it is gone
    made.append(('NS_ExplosionSmoke', BURST, fx(n=11, life=(1.3, 2.6), size=((150, 150), (250, 250)), mat=cel('Puff'),
                                                 ramp=heat((C_SMOKE_L, 1.0), (C_SMOKE, 0.8), (C_SMOKE, 0.45), (C_SMOKE_D, 0.0)), grow=(0.5, 1.4, 2.0),
                                                 speed=(120, 420), cone=70.0, world=True, drag=1.4, gravity=110.0, sphere=90.0, sort=True)))
    # ---- burning wreck / burning KESTREL
    made.append(('NS_Fire', LOOP, {
        'emitter': {'SpawnInfos': '((Type=Rate,Rate=(Min=46,Max=46)))', 'FixedBounds': BIG},
        'modules': {
            'InitializeParticle': {'LifetimeDistribution': rng(0.38, 0.81), 'SpriteSizeDistribution': v2rng((55, 92), (123, 165)), 'SpriteRotationDistribution': rng(0, 360)},
            'ShapeLocation': {'ShapePrimitive': 'Sphere'},
            'AddVelocity': {'VelocityType': 'InCone', 'ConeAngle': '22.0', 'ConeVelocityDistribution': rng(180, 420)},
            'Drag': {'DragDistribution': const(1.0)},
            'GravityForce': {'GravityDistribution': vconst(0, 0, 300)},
            'SolveVelocitiesAndForces': {},
            'ScaleColor': {'ScaleDistribution': colors((7, 5, 2.4, 1), (5, 1.8, 0.4, 1), (1.6, 0.25, 0.06, 0.6), (0, 0, 0, 0))},
            'ScaleSpriteSize': {'ScaleDistribution': sizes(0.8, 1.2, 0.5)},
            'SpriteRotationRate': {'RotationRateDistribution': rng(-120, 120)},
            'ApplyOwnerScaleToAttributes': {},
        },
        'renderer': {'Material': mat('/Game/FX/M_FX_Fire.M_FX_Fire')},
    }))
    # FX overhaul: the damaged KESTREL's smoke as 2.4 set it (ft_cel.car_smoke: smaller, thinner, shorter-lived). The spec here
    # was still 2.0's, whose puffs grew to 3.7 m and stood as a dark wall between the chase camera and the road, and
    # every rebuild of this file put that back over 2.4's fix. Both generators now say the same thing
    made.append(('NS_Smoke', LOOP, {
        'emitter': {'SpawnInfos': '((Type=Rate,Rate=(Min=12,Max=12)))', 'FixedBounds': BIG},
        'modules': {
            'InitializeParticle': {'LifetimeDistribution': rng(0.8, 1.4), 'SpriteSizeDistribution': v2rng((45, 40), (86, 70)), 'SpriteRotationDistribution': rng(0, 360)},
            'ShapeLocation': {'ShapePrimitive': 'Sphere'},
            'AddVelocity': {'VelocityType': 'InCone', 'ConeAngle': '25.0', 'ConeVelocityDistribution': rng(150, 300)},
            'Drag': {'DragDistribution': const(0.8)},
            'SolveVelocitiesAndForces': {},
            'ScaleColor': {'ScaleDistribution': colors((0.08, 0.07, 0.08, 0), (0.06, 0.055, 0.06, 0.32), (0.04, 0.04, 0.045, 0))},
            'ScaleSpriteSize': {'ScaleDistribution': sizes(0.6, 1.3, 1.9)},
            'SpriteRotationRate': {'RotationRateDistribution': rng(-30, 30)},
        },
        'renderer': {'Material': mat('/Game/FX/M_FX_Smoke.M_FX_Smoke')},
    }))
    # ---- scraping the barrier, impacts
    made.append(('NS_Sparks', LOOP, {
        # FX overhaul: fewer and fatter (240 a second of 4 cm sparks was a haze at 120 mph): each one now reads as a streak
        'emitter': {'SpawnInfos': '((Type=Rate,Rate=(Min=150,Max=150)))', 'FixedBounds': BIG},
        'modules': {
            'InitializeParticle': {'LifetimeDistribution': rng(0.14, 0.40), 'SpriteSizeDistribution': v2rng((6, 10), (11, 18))},
            # 3.1: ConeRotationType. The template turns the cone with a rotator and ignores ConeDirection unless told
            # otherwise, so until now these sparks (and the turbo flame below) left along the emitter's +Z, straight up
            'AddVelocity': {'VelocityType': 'InCone', 'ConeAngle': '55.0', 'ConeVelocityDistribution': rng(500, 1500),
                            'ConeRotationType': 'Direction', 'ConeDirection': vconst(-1, 0, 0.6)},
            'GravityForce': {},
            'SolveVelocitiesAndForces': {},
            'ScaleColor': {'ScaleDistribution': colors((10, 7, 3, 1), (6, 2, 0.4, 1), (2, 0.2, 0.02, 0))},
            'ScaleSpriteSizeBySpeed': {'VelocityThreshold': const(1200), 'MinScaleFactor': v2const(1, 1), 'MaxScaleFactor': v2const(1, 10)},
        },
        'renderer': {'Material': mat('/Game/FX/M_FX_Spark.M_FX_Spark'), 'Alignment': 'VelocityAligned'},
    }))
    made.append(('NS_Impact', BURST, {
        'emitter': {'SpawnInfos': '((Amount=(Min=12,Max=12)))', 'FixedBounds': BIG, 'EmitterState': '(LoopBehavior=Once,LoopDuration=(Min=0.6,Max=0.6,Mode=UniformConstant))'},
        'modules': {
            'InitializeParticle': {'LifetimeDistribution': rng(0.08, 0.27), 'SpriteSizeDistribution': v2rng((5, 8), (9, 14))},
            'AddVelocity': {'VelocityType': 'InCone', 'ConeAngle': '180.0', 'ConeVelocityDistribution': rng(400, 1300)},
            'GravityForce': {},
            'SolveVelocitiesAndForces': {},
            'ScaleColor': {'ScaleDistribution': colors((12, 9, 4, 1), (4, 1.2, 0.2, 0))},
            'ScaleSpriteSizeBySpeed': {'VelocityThreshold': const(1000), 'MinScaleFactor': v2const(1, 1), 'MaxScaleFactor': v2const(1, 8)},
            'ApplyOwnerScaleToAttributes': {},
        },
        'renderer': {'Material': mat('/Game/FX/M_FX_Spark.M_FX_Spark'), 'Alignment': 'VelocityAligned'},
    }))
    # 3.1: bounds and the owner's scale (the Backfire Pops spawn it at 1.3x and 2.2x, which did nothing without it)
    made.append(('NS_Muzzle', BURST, {
        'emitter': {'SpawnInfos': '((Amount=(Min=3,Max=3)))', 'FixedBounds': BIG, 'EmitterState': '(LoopBehavior=Once,LoopDuration=(Min=0.2,Max=0.2,Mode=UniformConstant))'},
        'modules': {
            'InitializeParticle': {'LifetimeDistribution': rng(0.04, 0.08), 'SpriteSizeDistribution': v2rng((40, 31), (77, 62)), 'SpriteRotationDistribution': rng(0, 360)},
            'ScaleColor': {'ScaleDistribution': colors((10, 8, 4, 1), (6, 3, 0.6, 0))},
            'ApplyOwnerScaleToAttributes': {},
        },
        'renderer': {'Material': mat('/Game/FX/M_FX_Glow.M_FX_Glow')},
    }))
    # ---- driving: turbo flame from the quad pipes, tyre smoke while drifting, missile trail
    # FX overhaul: a longer flame that tapers (it was a 60 cm tuft behind a 4.8 m car): same count, each sprite lives a little
    # longer, leaves faster and thins to a point. The six nitro colours copy this spec (ft_v26.make_tints)
    made.append(('NS_TurboFlame', LOOP, {
        'emitter': {'SpawnInfos': '((Type=Rate,Rate=(Min=108,Max=108)))', 'FixedBounds': BIG},
        'modules': {
            'InitializeParticle': {'LifetimeDistribution': rng(0.085, 0.16), 'SpriteSizeDistribution': v2rng((30, 27), (62, 56)), 'SpriteRotationDistribution': rng(0, 360)},
            'ShapeLocation': {'ShapePrimitive': 'Box', 'BoxSize': vconst(4, 110, 8)},
            'AddVelocity': {'VelocityType': 'InCone', 'ConeAngle': '6.0', 'ConeVelocityDistribution': rng(700, 1250),
                            'ConeRotationType': 'Direction', 'ConeDirection': vconst(-1, 0, 0)},
            'SolveVelocitiesAndForces': {},
            'ScaleColor': {'ScaleDistribution': colors((4.5, 7.2, 10.5, 1), (2.4, 5.2, 10, 0.82), (5.1, 2.0, 0.36, 0.49), (2.5, 0.42, 0.06, 0))},
            'ScaleSpriteSize': {'ScaleDistribution': sizes(1.0, 0.62, 0.28)},
        },
        'renderer': {'Material': mat('/Game/FX/M_FX_Fire.M_FX_Fire')},
    }))
    # FX overhaul: tyre smoke that streams back low from the rear tyres (the sheet: "keep drift smoke low and let the car
    # silhouette stay readable"). It was a slow ball of puffs growing to 3.4 m round the back of the car, straight
    # between the chase camera and the car. The emitter is local space, so "left behind" is a fast narrow cone back
    # along the emitter's -X, and the car turns the emitter to face the way it is really travelling (a drifting car
    # points 24 degrees off its line). The four smoke colours copy this spec (ft_v26.make_tints)
    made.append(('NS_TyreSmoke', LOOP, {
        'emitter': {'SpawnInfos': '((Type=Rate,Rate=(Min=46,Max=46)))', 'FixedBounds': BIG},
        'modules': {
            'InitializeParticle': {'LifetimeDistribution': rng(0.30, 0.55), 'SpriteSizeDistribution': v2rng((50, 40), (84, 66)), 'SpriteRotationDistribution': rng(0, 360)},
            'ShapeLocation': {'ShapePrimitive': 'Box', 'BoxSize': vconst(30, 230, 8)},
            'AddVelocity': {'VelocityType': 'InCone', 'ConeAngle': '14.0', 'ConeVelocityDistribution': rng(500, 1100),
                            'ConeRotationType': 'Direction', 'ConeDirection': vconst(-1, 0, 0.12)},
            'Drag': {'DragDistribution': const(2.2)},
            'SolveVelocitiesAndForces': {},
            'ScaleColor': {'ScaleDistribution': colors((0.5, 0.45, 0.55, 0), (0.45, 0.42, 0.5, 0.36), (0.4, 0.4, 0.45, 0))},
            'ScaleSpriteSize': {'ScaleDistribution': sizes(0.6, 1.3, 1.9)},
            'SpriteRotationRate': {'RotationRateDistribution': rng(-40, 40)},
        },
        'renderer': {'Material': mat('/Game/FX/M_FX_Smoke.M_FX_Smoke')},
    }))
    # FX overhaul: the missile's hot exhaust core, on the missile (this slot is its Main layer: it shows at every quality).
    # The smoke is no longer a cone of puffs that rode along behind the missile: it is laid in the WORLD, where the
    # missile has been (NS_MissileSmoke below, dropped by FTFx::Trail), so it hangs, spreads and breaks up after it.
    # A rocket seen from the chase camera is seen from behind: the core is short, wide and nearly white, so it reads
    # as a hot point inside the orange taper (NS_MissileFlame)
    made.append(('NS_MissileTrail', LOOP, fx(rate=64, life=(0.05, 0.085), size=((26, 70), (36, 104)), mat=cel('Shard'),
                                             ramp=heat(((1.0, 0.97, 0.82), 1.0), ((1.0, 0.80, 0.30), 0.6), ((1.0, 0.5, 0.1), 0.0)),
                                             speed=(1300, 2100), cone=4.0, axis=(-1, 0, 0), stretch=(1200, 1.5), align=True, sort=True, owner=False)))
    return made


def attack_definitions():
    """3.1: what every attack shows, stage by stage (DevLog/jam/attacks.md has the table). Layered effects are several
    systems spawned together by FTFx (FTFx.cpp): each system here is one emitter. Directions: +X is the way the thing
    points (a barrel, a round's travel), -X is behind it; world-space bursts use +Z for up."""
    d = []

    def add(name, cfg):
        d.append((name, LOOP if 'Type=Rate' in cfg['emitter']['SpawnInfos'] else BURST, cfg))

    # ---- the KESTREL's guns: a star at the barrel per load, tongues of flame out of it
    add('NS_MuzzleStar', fx(n=1, life=(0.05, 0.065), size=((78, 78), (96, 96)), mat=cel('Flash'), ramp=heat((C_STD, 1.0), (C_STD, 0.55), (C_STD, 0.0)),
                            grow=(0.55, 1.0, 1.15), rot=True, sort=True, pool=(4, 12)))
    add('NS_MuzzleStarAP', fx(n=1, life=(0.05, 0.065), size=((86, 86), (104, 104)), mat=cel('Flash'), ramp=heat((C_AP, 1.0), (C_AP, 0.55), (C_AP, 0.0)),
                              grow=(0.55, 1.0, 1.15), rot=True, sort=True, pool=(4, 12)))
    add('NS_MuzzleStarHE', fx(n=1, life=(0.055, 0.07), size=((98, 98), (120, 120)), mat=cel('Burst'), ramp=heat((C_HE, 1.0), (C_HE, 0.6), (C_EMBER, 0.0)),
                              grow=(0.55, 1.0, 1.2), rot=True, sort=True, pool=(4, 12)))
    # FX overhaul: four tongues, longer: at the chase camera's distance three 46 cm ones were lost under the star
    add('NS_MuzzleTongue', fx(n=4, life=(0.045, 0.08), size=((18, 56), (26, 84)), mat=cel('Shard'),
                              ramp=heat(((1.0, 0.9, 0.6), 1.0), ((1.0, 0.6, 0.15), 0.5), ((1.0, 0.3, 0.05), 0.0)),
                              speed=(1500, 3000), cone=9.0, axis=(1, 0, 0), stretch=(1500, 2.6), align=True, sort=True, pool=(4, 12)))
    # ---- what a round does where it lands (attached to what it hit; +X = the round's way)
    add('NS_ImpactStar', fx(n=1, life=(0.09, 0.12), size=((95, 95), (125, 125)), mat=cel('Burst'),
                            ramp=heat((C_STD, 1.0), (C_STD, 0.6), (C_FIRE, 0.25), (C_FIRE, 0.0)), grow=(0.5, 1.0, 1.25), rot=True, sort=True, pool=(4, 12)))
    add('NS_ImpactStarAP', fx(n=1, life=(0.09, 0.12), size=((120, 120), (150, 150)), mat=cel('Flash'),
                              ramp=heat((C_AP, 1.0), (C_AP, 0.6), ((0.25, 0.5, 1.0), 0.0)), grow=(0.5, 1.0, 1.2), rot=True, sort=True, pool=(2, 8)))
    add('NS_ImpactStarHE', fx(n=1, life=(0.1, 0.13), size=((130, 130), (160, 160)), mat=cel('Burst'),
                              ramp=heat((C_HE, 1.0), (C_HE, 0.6), (C_EMBER, 0.25), (C_EMBER, 0.0)), grow=(0.5, 1.0, 1.25), rot=True, sort=True, pool=(2, 8)))
    # an AP round goes on through: blue-white spears out the far side
    add('NS_ImpactPierce', fx(n=5, life=(0.07, 0.12), size=((10, 60), (16, 110)), mat=cel('Shard'), ramp=heat((C_WHITE, 1.0), (C_AP, 0.5), (C_AP, 0.0)),
                              speed=(2600, 4600), cone=7.0, axis=(1, 0, 0), stretch=(2500, 2.5), align=True, sort=True))
    # armour: nothing goes in, sparks come back off the plate
    # FX overhaul: a ricochet you can see: nine long steel-white shards thrown back off the plate (it was fourteen 5 cm sparks)
    add('NS_ImpactArmor', fx(n=9, life=(0.10, 0.22), size=((8, 46), (13, 96)), mat=cel('Shard'), ramp=heat((C_WHITE, 1.0), ((0.75, 0.86, 1.0), 0.6), ((0.9, 0.7, 0.3), 0.0)),
                             speed=(1500, 3600), cone=66.0, axis=(-1, 0, 0.45), gravity=-700.0, stretch=(1500, 2.2), align=True, sort=True, pool=(2, 8)))
    add('NS_ImpactRing', fx(n=1, life=(0.11, 0.13), size=((150, 150), (170, 170)), mat=cel('Ring'), ramp=heat((C_WHITE, 1.0), ((0.8, 0.9, 1.0), 0.0)),
                            grow=(0.35, 1.0), sort=True))
    add('NS_Fizzle', fx(n=3, life=(0.08, 0.14), size=((12, 12), (20, 20)), mat=GLOW, ramp=[(6, 5, 3, 1), (2, 1, 0.4, 0)], speed=(100, 400)))
    # ---- theirs: every hostile muzzle, round and hit is the same hot red-magenta
    add('NS_EnemyMuzzle', fx(n=1, life=(0.08, 0.10), size=((190, 190), (230, 230)), mat=cel('Flash'), ramp=heat((C_FOE, 1.0), (C_FOE, 0.55), (C_FOE, 0.0)),
                             grow=(0.5, 1.0, 1.1), rot=True, sort=True, pool=(4, 16)))
    add('NS_EnemyTrail', fx(rate=64, life=(0.10, 0.17), size=((24, 44), (34, 62)), mat=SPARK, ramp=[(14, 1.2, 3.6, 1), (8, 0.4, 1.9, 0.8), (2, 0.05, 0.5, 0)],
                            speed=(2200, 3000), cone=3.0, axis=(-1, 0, 0), stretch=(2000, 2.4), align=True, owner=False))
    add('NS_BigTracerTrail', fx(rate=56, life=(0.10, 0.16), size=((20, 60), (28, 84)), mat=SPARK, ramp=[(11, 7, 2, 1), (9, 0.8, 2.2, 0.8), (2, 0.05, 0.5, 0)],
                                speed=(2200, 3000), cone=3.0, axis=(-1, 0, 0), stretch=(2000, 1.6), align=True, owner=False))
    add('NS_HitPlayer', fx(n=1, life=(0.10, 0.13), size=((150, 150), (180, 180)), mat=cel('Burst'), ramp=heat((C_FOE, 1.0), (C_FOE, 0.6), ((0.6, 0.04, 0.2), 0.0)),
                           grow=(0.5, 1.0, 1.2), rot=True, sort=True, pool=(2, 8)))
    add('NS_HitPlayerSparks', fx(n=10, life=(0.12, 0.3), size=((5, 9), (9, 14)), mat=SPARK, ramp=[(12, 6, 6, 1), (8, 1, 2, 1), (2, 0.1, 0.4, 0)],
                                 speed=(700, 2000), cone=80.0, axis=(-1, 0, 0.6), gravity=-900.0, stretch=(1000, 8.0), align=True))
    # ---- the bosses' own rounds: fish leave spray, fireballs leave fire, pies leave cream
    add('NS_FishTrail', fx(rate=30, life=(0.2, 0.34), size=((14, 14), (26, 26)), mat=GLOW, ramp=[(1.5, 7, 9, 1), (0.6, 3, 5, 0.6), (0.1, 0.8, 1.6, 0)],
                           speed=(500, 1100), cone=24.0, axis=(-1, 0, 0), gravity=-300.0, owner=False))
    add('NS_FireballTrail', fx(rate=40, life=(0.18, 0.3), size=((52, 52), (84, 84)), mat=cel('Blob'),
                               ramp=heat(((1.0, 0.8, 0.3), 1.0), (C_FIRE, 0.6), (C_EMBER, 0.0)), grow=(1.0, 0.7, 0.3),
                               speed=(450, 900), cone=13.0, axis=(-1, 0, 0), rot=True, sort=True, owner=False))
    add('NS_PieTrail', fx(rate=18, life=(0.25, 0.4), size=((16, 16), (30, 30)), mat=cel('Puff'), ramp=heat(((1.0, 0.95, 0.88), 1.0), ((1.0, 0.86, 0.9), 0.6), ((1.0, 0.8, 0.86), 0.0)),
                          speed=(250, 650), cone=30.0, axis=(-1, 0, 0), gravity=-500.0, sort=True, owner=False))
    # ---- missiles: a flash and a kick of smoke at launch, a flame at the tail, shrapnel on the way through
    # FX overhaul: the tapered flame round the core (NS_MissileTrail): orange to red, each shard thinning to a point as it
    # falls back, 2.5 m long where it was 60 cm
    add('NS_MissileFlame', fx(rate=52, life=(0.10, 0.17), size=((44, 90), (64, 140)), mat=cel('Shard'),
                              ramp=heat(((1.0, 0.62, 0.14), 1.0), ((1.0, 0.36, 0.06), 0.6), ((0.8, 0.12, 0.05), 0.0)), grow=(1.0, 0.6, 0.22),
                              speed=(1500, 2500), cone=7.0, axis=(-1, 0, 0), stretch=(1400, 1.5), align=True, sort=True, owner=False))
    add('NS_MissileLaunch', fx(n=1, life=(0.09, 0.11), size=((300, 300), (340, 340)), mat=cel('Flash'), ramp=heat((C_WHITE, 1.0), (C_HE, 0.55), (C_HE, 0.0)),
                               grow=(0.5, 1.0, 1.1), rot=True, sort=True, pool=(2, 6)))
    # FX overhaul: smaller, shorter and thrown up as well as back, clear of the roof: seven puffs growing to 2.2 m sat
    # between the chase camera and the car for half a second. The smoke that opens up at the lens (SmokeCar)
    add('NS_MissileBackblast', fx(n=6, life=(0.28, 0.45), size=((40, 40), (70, 70)), mat=toon('SmokeCar'), ramp=heat((C_EXHAUST, 1.0), (C_SMOKE_L, 0.7), (C_SMOKE, 0.0)),
                                  grow=(0.6, 1.4, 1.8), speed=(500, 1400), cone=30.0, axis=(-1, 0, 0.5), drag=3.0, rot=True, sort=True))
    add('NS_MissileHit', fx(n=16, life=(0.16, 0.36), size=((12, 40), (20, 80)), mat=cel('Shard'), ramp=heat((C_WHITE, 1.0), (C_HE, 0.6), (C_EMBER, 0.0)),
                            speed=(1200, 3000), cone=55.0, axis=(1, 0, 0.2), gravity=-700.0, stretch=(1500, 2.0), align=True, sort=True, pool=(2, 6)))
    # ---- lobbed things: each leaves its own trail (the emitter is turned to face back along its flight)
    add('NS_FuseTrail', fx(rate=44, life=(0.12, 0.26), size=((4, 8), (7, 13)), mat=SPARK, ramp=[(12, 10, 5, 1), (8, 4, 0.6, 1), (2, 0.4, 0.05, 0)],
                           speed=(250, 700), cone=42.0, axis=(-1, 0, 0.3), gravity=-400.0, stretch=(600, 6.0), align=True, owner=False))
    add('NS_SnowTrail', fx(rate=28, life=(0.3, 0.5), size=((36, 36), (62, 62)), mat=cel('Puff'), ramp=heat((C_SNOW, 1.0), (C_ICE, 0.6), (C_ICE, 0.0)),
                           grow=(0.6, 1.4, 1.9), speed=(250, 650), cone=24.0, axis=(-1, 0, 0), drag=2.0, sort=True, owner=False))
    # ---- and its own landing
    add('NS_LandBomb', fx(n=34, life=(0.4, 0.85), size=((10, 26), (16, 44)), mat=cel('Shard'),
                          ramp=heat(((1.0, 0.25, 0.75), 1.0), ((1.0, 0.85, 0.2), 0.75), ((0.25, 0.9, 1.0), 0.4), ((0.3, 0.4, 1.0), 0.0)),
                          speed=(900, 2600), cone=70.0, gravity=-1300.0, stretch=(1500, 1.6), align=True, sort=True))
    add('NS_FireSplash', fx(n=12, life=(0.24, 0.42), size=((34, 70), (52, 110)), mat=cel('Shard'), ramp=heat(((1.0, 0.85, 0.4), 1.0), (C_FIRE, 0.6), (C_EMBER, 0.0)),
                            speed=(900, 1900), cone=88.0, inner=62.0, gravity=-500.0, stretch=(1200, 1.5), align=True, sort=True))
    # ---- blasts that are not fire: ice, water, electricity (Explode's Look)
    add('NS_ExplosionIce', fx(n=2, life=(0.2, 0.28), size=((300, 300), (380, 380)), mat=cel('Burst'),
                              ramp=heat((C_SNOW, 1.0), (C_ICE, 0.6), ((0.35, 0.6, 1.0), 0.25), ((0.3, 0.5, 0.9), 0.0)), grow=(0.45, 1.0, 1.3), rot=True, sort=True, pool=(2, 8)))
    add('NS_SnowBurst', fx(n=12, life=(0.45, 0.85), size=((90, 90), (150, 150)), mat=cel('Puff'), ramp=heat((C_SNOW, 1.0), (C_SNOW, 0.7), (C_ICE, 0.0)),
                           grow=(0.5, 1.4, 1.9), speed=(250, 900), cone=80.0, drag=2.4, sphere=60.0, sort=True))
    add('NS_IceShards', fx(n=18, life=(0.3, 0.6), size=((10, 34), (16, 56)), mat=cel('Shard'), ramp=heat((C_WHITE, 1.0), (C_ICE, 0.6), (C_ICE, 0.0)),
                           speed=(1100, 2600), cone=42.0, gravity=-1500.0, stretch=(1500, 1.8), align=True, sort=True))
    add('NS_ExplosionWater', fx(n=2, life=(0.2, 0.28), size=((300, 300), (380, 380)), mat=cel('Burst'),
                                ramp=heat((C_WHITE, 1.0), (C_SEA, 0.6), ((0.08, 0.45, 0.8), 0.25), ((0.06, 0.3, 0.6), 0.0)), grow=(0.45, 1.0, 1.3), rot=True, sort=True, pool=(2, 8)))
    add('NS_Splash', fx(n=26, life=(0.4, 0.75), size=((10, 30), (16, 50)), mat=cel('Shard'), ramp=heat((C_WHITE, 1.0), (C_SEA, 0.6), (C_SEA, 0.0)),
                        speed=(900, 2300), cone=48.0, gravity=-1600.0, stretch=(1500, 1.8), align=True, sort=True))
    add('NS_ExplosionZap', fx(n=1, life=(0.07, 0.09), size=((330, 330), (400, 400)), mat=cel('Flash'), ramp=heat((C_WHITE, 1.0), (C_ZAP, 0.5), (C_ZAP, 0.0)),
                              grow=(0.6, 1.0, 1.1), rot=True, sort=True, pool=(2, 8)))
    add('NS_ZapArcs', fx(n=14, life=(0.07, 0.15), size=((8, 70), (12, 130)), mat=cel('Shard'), ramp=heat((C_WHITE, 1.0), (C_ZAP, 0.5), ((0.3, 0.5, 1.0), 0.0)),
                         speed=(1400, 3600), stretch=(1800, 1.8), align=True, sort=True, pool=(4, 12)))
    # ---- hazards while they are live
    add('NS_FlameBurst', fx(n=5, life=(0.28, 0.46), size=((90, 120), (150, 200)), mat=cel('Blob'),
                            ramp=heat(((1.0, 0.86, 0.4), 1.0), (C_FIRE, 0.65), (C_EMBER, 0.3), ((0.5, 0.06, 0.04), 0.0)), grow=(0.7, 1.1, 0.8),
                            speed=(250, 700), cone=22.0, sphere=70.0, rot=True, spin=60.0, sort=True, pool=(8, 40)))
    add('NS_SnowWall', fx(n=5, life=(0.5, 0.8), size=((200, 200), (300, 300)), mat=cel('Puff'), ramp=heat((C_SNOW, 1.0), (C_SNOW, 0.75), (C_ICE, 0.0)),
                          grow=(0.6, 1.3, 1.6), speed=(200, 600), cone=60.0, sphere=110.0, drag=1.5, sort=True, pool=(8, 32)))
    # ---- every explosion: the sheet's banded burst in front of the fireball, a ring, embers that hang in the air
    core = dict(n=2, life=(0.2, 0.28), size=((250, 250), (330, 330)), mat=cel('Burst'), grow=(0.42, 1.0, 1.32), rot=True, sort=True, pool=(3, 10))
    add('NS_BlastCore', fx(ramp=heat(((1.0, 0.72, 0.2), 1.0), (C_FIRE, 0.7), ((0.9, 0.2, 0.05), 0.32), ((0.6, 0.08, 0.04), 0.0)), **core))
    add('NS_BlastCore_s_blue_fire', fx(ramp=heat(((0.45, 0.8, 1.0), 1.0), ((0.12, 0.45, 1.0), 0.7), ((0.05, 0.2, 0.85), 0.32), ((0.03, 0.1, 0.5), 0.0)), **core))
    add('NS_BlastCore_s_toxic_blast', fx(ramp=heat(((0.7, 1.0, 0.35), 1.0), ((0.25, 0.9, 0.12), 0.7), ((0.1, 0.6, 0.06), 0.32), ((0.05, 0.3, 0.04), 0.0)), **core))
    add('NS_ShockRing', fx(n=1, life=(0.26, 0.3), size=((520, 520), (560, 560)), mat=cel('Ring'), ramp=heat(((1.0, 0.9, 0.6), 1.0), (C_FIRE, 0.5), (C_EMBER, 0.0)),
                           grow=(0.25, 0.8, 1.0), sort=True, pool=(2, 8)))
    add('NS_ExplosionEmbers', fx(n=16, life=(1.1, 2.3), size=((7, 7), (13, 13)), mat=GLOW, ramp=[(9, 5, 1.2, 1), (6, 2, 0.3, 0.9), (2, 0.3, 0.05, 0)],
                                 speed=(200, 900), cone=75.0, world=True, drag=1.6, gravity=90.0, sphere=80.0))
    # ---- blasts with a real radius: the ring ends 100 cm from the middle at scale 1, so spawn it at radius / 100
    add('NS_BlastRing', fx(n=1, life=(0.2, 0.22), size=((200, 200), (200, 200)), mat=cel('Ring'), ramp=heat(((1.0, 0.85, 0.5), 1.0), (C_HE, 0.55), (C_HE, 0.0)),
                           grow=(0.5, 0.9, 1.0), sort=True, pool=(2, 8)))
    add('NS_BlastRingGold', fx(n=1, life=(0.2, 0.22), size=((200, 200), (200, 200)), mat=cel('Ring'), ramp=heat((C_WHITE, 1.0), (C_GOLD, 0.55), (C_GOLD, 0.0)),
                               grow=(0.5, 0.9, 1.0), sort=True))
    add('NS_HeBurst', fx(n=1, life=(0.15, 0.19), size=((170, 170), (210, 210)), mat=cel('Burst'), ramp=heat((C_HE, 1.0), (C_HE, 0.6), (C_EMBER, 0.0)),
                         grow=(0.5, 1.0, 1.25), rot=True, sort=True))
    add('NS_ChainBurst', fx(n=1, life=(0.16, 0.2), size=((260, 260), (300, 300)), mat=cel('Flash'), ramp=heat((C_WHITE, 1.0), (C_GOLD, 0.55), (C_GOLD, 0.0)),
                            grow=(0.5, 1.0, 1.15), rot=True, sort=True))
    add('NS_BarrelBurst', fx(n=14, life=(0.45, 0.85), size=((12, 36), (20, 60)), mat=cel('Shard'), ramp=heat(((1.0, 0.85, 0.4), 1.0), (C_FIRE, 0.6), (C_EMBER, 0.0)),
                             speed=(1200, 2800), cone=62.0, gravity=-1300.0, stretch=(1500, 1.8), align=True, sort=True))
    add('NS_BarrelFire', fx(rate=13, life=(0.34, 0.52), size=((60, 76), (90, 112)), mat=cel('Blob'), ramp=heat(((1.0, 0.85, 0.35), 1.0), (C_FIRE, 0.6), (C_EMBER, 0.0)),
                            grow=(1.0, 0.75, 0.3), speed=(160, 330), cone=14.0, world=True, sphere=22.0, rot=True, sort=True, owner=False))
    # ---- close quarters and wind-ups
    add('NS_ChainSwing', fx(n=14, life=(0.1, 0.2), size=((5, 12), (8, 18)), mat=SPARK, ramp=[(12, 11, 8, 1), (8, 5, 1, 1), (2, 0.5, 0.05, 0)],
                            speed=(1500, 3200), cone=26.0, axis=(1, 0, 0.15), gravity=-600.0, stretch=(1500, 8.0), align=True))
    add('NS_RamBurst', fx(n=1, life=(0.11, 0.14), size=((140, 140), (170, 170)), mat=cel('Flash'), ramp=heat(((1.0, 0.8, 0.3), 1.0), ((1.0, 0.7, 0.25), 0.5), (C_FIRE, 0.0)),
                          grow=(0.5, 1.0, 1.15), rot=True, sort=True))
    add('NS_RamSparks', fx(n=22, life=(0.12, 0.32), size=((5, 10), (9, 16)), mat=SPARK, ramp=[(11, 8, 3, 1), (6, 2, 0.4, 1), (2, 0.3, 0.05, 0)],
                           speed=(900, 2800), cone=70.0, axis=(1, 0, 0.3), gravity=-900.0, stretch=(1300, 9.0), align=True))
    add('NS_AlertFlare', fx(n=1, life=(0.22, 0.26), size=((230, 230), (260, 260)), mat=cel('Ring'), ramp=heat(((1.0, 0.45, 0.3), 1.0), (C_FOE, 0.6), (C_FOE, 0.0)),
                            grow=(0.3, 0.85, 1.0), sort=True, pool=(2, 8)))
    add('NS_ChargeTrail', fx(rate=34, life=(0.25, 0.42), size=((50, 50), (84, 84)), mat=cel('Puff'), ramp=heat((C_DUST, 1.0), (C_DUST, 0.6), (C_DUST, 0.0)),
                             grow=(0.6, 1.4, 1.8), speed=(900, 1700), cone=16.0, axis=(-1, 0, 0.18), drag=2.5, box=(20, 150, 6), sort=True, owner=False))
    add('NS_RevSmoke', fx(n=8, life=(0.35, 0.6), size=((60, 60), (100, 100)), mat=cel('Puff'), ramp=heat((C_DUST, 1.0), (C_DUST, 0.6), (C_DUST, 0.0)),
                          grow=(0.6, 1.5, 2.0), speed=(300, 900), cone=40.0, axis=(-1, 0, 0.3), drag=3.0, box=(20, 150, 6), sort=True))
    # ---- what is left: a wreck smokes as it tumbles
    # FX overhaul: it climbs faster (clear of the chase camera's eye line in under half a second), curls, and is the smoke
    # that opens up as the camera nears it: a wreck you drive past no longer wipes a grey puff across the screen
    add('NS_WreckSmoke', fx(rate=8, life=(0.9, 1.5), size=((80, 80), (130, 130)), mat=toon('Smoke'),
                            ramp=heat((C_SMOKE_D, 1.0), (C_SMOKE, 0.75), (C_SMOKE_L, 0.45), (C_SMOKE_L, 0.0)),
                            grow=(0.6, 1.5, 2.1), speed=(380, 640), cone=14.0, world=True, drag=0.7, curl=(60.0, 50.0), rot=True, sort=True, owner=False))
    return d


# ---------------------------------------------------------------------------------------------- FX round 3: marks on the road
# M_FX_Mark: what stays on the road for the scene. Not a sprite material: FTFx.cpp lays flat planes with it (instanced
# static meshes: FTFx::TyreMark, FTFx::SootMark), so a mark costs no Niagara system and the newest few hundred are one
# draw.
#   Shape 0   a tyre mark: a strip along the plane's X with hard edges, a little lighter down its middle (two treads)
#   Shape 1   the soot a blast leaves: M_FX_Toon's scorch, held (it does not fade: it is cleared with the scene), a
#             shape of its own for every instance (the per-instance random)
# Tint is its flat colour (FTFx.cpp gives the tyre marks the level's), Opacity how much of the road it covers.
MARK_HLSL = r"""
float2 q = (UV - 0.5) * 2.0;
float3 col = Tint;
float op = 0.0;
if (Shape < 0.5)
{
    float v = abs(q.y);
    float e = max(fwidth(v) * 1.2, 0.02);
    op = (1.0 - smoothstep(0.9 - e, 0.9 + e, v)) * (v > 0.28 ? 1.0 : 0.72);
}
else
{
    float r = length(q);
    float a = atan2(q.y, q.x);
    float sN = 9.0 + floor(frac(Rand * 4.1) * 4.99);
    float ray = pow(saturate(0.5 + 0.5 * sin(a * sN + Rand * 21.0)), 2.5) * (0.6 + 0.4 * sin(a * 2.0 + Rand * 5.0));
    float Rs = 0.95 * (0.52 + 0.40 * ray + 0.08 * sin(a * 3.0 + Rand * 13.0));
    float d = r / max(Rs, 0.001);
    float da = max(fwidth(r) * 1.2, 0.006) / max(Rs, 0.05);
    op = 1.0 - smoothstep(1.0 - da, 1.0 + da, d);
    col = r < 0.285 ? Tint * 0.55 : Tint;
}
return float4(col, op * Opacity);
"""


def mark_materials():
    """M_FX_Mark and its two instances (MI_FX_MarkTyre, MI_FX_MarkSoot). Returns the pixel-shader instruction count
    (0 = it did not compile)."""
    import ft_mats
    MEL = unreal.MaterialEditingLibrary
    m = ft_mats.new_material('/Game/FX', 'M_FX_Mark', blend=unreal.BlendMode.BLEND_TRANSLUCENT, shading=unreal.MaterialShadingModel.MSM_UNLIT, two_sided=False)
    # the cooked game falls back to the default material without the usage flag (README: material usage flags)
    m.set_editor_property('used_with_instanced_static_meshes', True)
    X = -1100
    ins = [('UV', ft_mats.expr(m, unreal.MaterialExpressionTextureCoordinate, x=X, y=-580), '')]
    code = MARK_HLSL
    rnd = getattr(unreal, 'MaterialExpressionPerInstanceRandom', None)
    if rnd is not None:
        ins.append(('Rand', ft_mats.expr(m, rnd, x=X, y=-500), ''))
    else:
        # no node for it in this build's Python: every scorch is the same shape
        code = 'float Rand = 0.37;' + MARK_HLSL
    ins.append(('Tint', ft_mats.vparam(m, 'Tint', (0.035, 0.03, 0.045, 1.0), x=X, y=-400), ''))
    ins.append(('Shape', ft_mats.sparam(m, 'Shape', 0.0, x=X, y=-240), ''))
    ins.append(('Opacity', ft_mats.sparam(m, 'Opacity', 0.5, x=X, y=-170), ''))
    cu = ft_mats.custom(m, code, ins, out_type=unreal.CustomMaterialOutputType.CMOT_FLOAT4, x=-500, y=0, desc='MarkFX')
    rgb = ft_mats.expr(m, unreal.MaterialExpressionComponentMask, x=-200, y=-60)
    for ch, on in (('r', True), ('g', True), ('b', True), ('a', False)):
        rgb.set_editor_property(ch, on)
    alpha = ft_mats.expr(m, unreal.MaterialExpressionComponentMask, x=-200, y=80)
    for ch, on in (('r', False), ('g', False), ('b', False), ('a', True)):
        alpha.set_editor_property(ch, on)
    MEL.connect_material_expressions(cu, '', rgb, '')
    MEL.connect_material_expressions(cu, '', alpha, '')
    MEL.connect_material_property(rgb, '', unreal.MaterialProperty.MP_EMISSIVE_COLOR)
    MEL.connect_material_property(alpha, '', unreal.MaterialProperty.MP_OPACITY)
    MEL.layout_material_expressions(m)
    MEL.recompile_material(m)
    AL.save_loaded_asset(m)
    ft_mats.mi(m, '/Game/FX', 'MI_FX_MarkTyre', scalars={'Shape': 0.0, 'Opacity': 0.5})
    ft_mats.mi(m, '/Game/FX', 'MI_FX_MarkSoot', vectors={'Tint': (0.05, 0.04, 0.07, 1.0)}, scalars={'Shape': 1.0, 'Opacity': 0.8})
    # round 4: the scorched strip a sliding wreck leaves (FTFx::WreckTrail)
    ft_mats.mi(m, '/Game/FX', 'MI_FX_MarkSkid', vectors={'Tint': (0.04, 0.03, 0.05, 1.0)}, scalars={'Shape': 0.0, 'Opacity': 0.62})
    try:
        return MEL.get_statistics(m).get_editor_property('num_pixel_shader_instructions')
    except Exception:  # noqa
        return -1


# ---------------------------------------------------------------------------------------------- FX round 4: marks on the KESTREL
# M_FX_CarMark: what the car's body wears as its armour drops. Flat planes the car puts on its own panels
# (FTPlayerCar.cpp, CarMarks: the places are the model's own, ft_car.py), one material instance each:
#   Shape 0   a dent: a dark facet of four to six sides with a lit edge along its top (ft_car.py: "dents read as dark
#             facets in the cel light")
#   Shape 1   cracked glass: thin pale lines out from a point a little off the middle, two broken rings round it
#   Shape 2   soot: M_FX_Mark's scorch star
# Seed gives each mark a shape of its own. A material of its own, so a mistake here cannot spoil the road's marks.
CARMARK_HLSL = r"""
float2 q = (UV - 0.5) * 2.0;
float rnd = frac(Seed * 0.6180339 + 0.137);
float3 col = Tint;
float op = 0.0;
float px = max(fwidth(q.x) + fwidth(q.y), 0.004);
if (Shape < 0.5)
{
    float r = length(q);
    float a = atan2(q.y, q.x);
    float nS = 4.0 + floor(frac(rnd * 3.3) * 2.99);
    float seg = 6.2831853 / nS;
    float ap = fmod(a + rnd * 6.2831853 + 62.831853, seg) - seg * 0.5;
    float d = r * cos(ap) / cos(seg * 0.5) / 0.92;
    op = 1.0 - smoothstep(1.0 - px, 1.0 + px, d);
    float lit = step(0.62, d) * step(0.25, -q.y / max(r, 0.001));
    col = lerp(Tint, Tint * 3.0 + 0.02, lit);
}
else if (Shape < 1.5)
{
    float2 c = q - float2(0.16 * sin(rnd * 40.0), 0.16 * cos(rnd * 23.0));
    float r = length(c);
    float a = atan2(c.y, c.x) + 3.1415927;
    float nS = 6.0 + floor(frac(rnd * 5.7) * 3.99);
    float seg = 6.2831853 / nS;
    float k = floor(a / seg);
    float jit = (frac(sin(k * 12.9898 + rnd * 78.233) * 43758.5453) - 0.5) * 0.5 * seg;
    float da = abs(a - (k + 0.5) * seg - jit);
    float aw = px / max(r, 0.06);
    float w = 0.016 / max(r, 0.06);
    float len = 0.5 + 0.42 * frac(sin(k * 7.13 + rnd * 11.0) * 9173.3);
    float spoke = (1.0 - smoothstep(w, w + aw, da)) * step(r, len);
    float ring1 = (1.0 - smoothstep(0.016, 0.016 + px, abs(r - 0.24))) * step(0.4, frac(sin(k * 3.7 + rnd * 5.0) * 311.7));
    float ring2 = (1.0 - smoothstep(0.016, 0.016 + px, abs(r - 0.50))) * step(0.55, frac(sin(k * 9.1 + rnd * 17.0) * 127.1));
    op = saturate(max(spoke, max(ring1, ring2))) * step(length(q), 0.97);
}
else
{
    float r = length(q);
    float a = atan2(q.y, q.x);
    float sN = 9.0 + floor(frac(rnd * 4.1) * 4.99);
    float ray = pow(saturate(0.5 + 0.5 * sin(a * sN + rnd * 21.0)), 2.5) * (0.6 + 0.4 * sin(a * 2.0 + rnd * 5.0));
    float Rs = 0.95 * (0.52 + 0.40 * ray + 0.08 * sin(a * 3.0 + rnd * 13.0));
    float d = r / max(Rs, 0.001);
    float da = px / max(Rs, 0.05);
    op = 1.0 - smoothstep(1.0 - da, 1.0 + da, d);
    col = r < 0.285 ? Tint * 0.55 : Tint;
}
return float4(col, op * Opacity);
"""
CARMARK_INSTANCES = {
    'Dent': dict(vectors={'Tint': (0.10, 0.008, 0.02, 1.0)}, scalars={'Shape': 0.0, 'Opacity': 0.85}),
    'Crack': dict(vectors={'Tint': (0.78, 0.86, 1.0, 1.0)}, scalars={'Shape': 1.0, 'Opacity': 0.9}),
    'Soot': dict(vectors={'Tint': (0.03, 0.025, 0.04, 1.0)}, scalars={'Shape': 2.0, 'Opacity': 0.8}),
}


def car_mark_materials():
    """M_FX_CarMark and MI_FX_CarDent, MI_FX_CarCrack, MI_FX_CarSoot. Returns the pixel-shader instruction count; if
    the material did not compile (0) the instances are removed, so the car simply wears no marks."""
    import ft_mats
    MEL = unreal.MaterialEditingLibrary
    m = ft_mats.new_material('/Game/FX', 'M_FX_CarMark', blend=unreal.BlendMode.BLEND_TRANSLUCENT, shading=unreal.MaterialShadingModel.MSM_UNLIT, two_sided=False)
    X = -1100
    ins = [('UV', ft_mats.expr(m, unreal.MaterialExpressionTextureCoordinate, x=X, y=-580), ''),
           ('Tint', ft_mats.vparam(m, 'Tint', (0.03, 0.025, 0.04, 1.0), x=X, y=-400), ''),
           ('Shape', ft_mats.sparam(m, 'Shape', 0.0, x=X, y=-240), ''),
           ('Opacity', ft_mats.sparam(m, 'Opacity', 0.8, x=X, y=-170), ''),
           ('Seed', ft_mats.sparam(m, 'Seed', 1.0, x=X, y=-100), '')]
    cu = ft_mats.custom(m, CARMARK_HLSL, ins, out_type=unreal.CustomMaterialOutputType.CMOT_FLOAT4, x=-500, y=0, desc='CarMarkFX')
    rgb = ft_mats.expr(m, unreal.MaterialExpressionComponentMask, x=-200, y=-60)
    for ch, on in (('r', True), ('g', True), ('b', True), ('a', False)):
        rgb.set_editor_property(ch, on)
    alpha = ft_mats.expr(m, unreal.MaterialExpressionComponentMask, x=-200, y=80)
    for ch, on in (('r', False), ('g', False), ('b', False), ('a', True)):
        alpha.set_editor_property(ch, on)
    MEL.connect_material_expressions(cu, '', rgb, '')
    MEL.connect_material_expressions(cu, '', alpha, '')
    MEL.connect_material_property(rgb, '', unreal.MaterialProperty.MP_EMISSIVE_COLOR)
    MEL.connect_material_property(alpha, '', unreal.MaterialProperty.MP_OPACITY)
    MEL.layout_material_expressions(m)
    MEL.recompile_material(m)
    AL.save_loaded_asset(m)
    try:
        n = MEL.get_statistics(m).get_editor_property('num_pixel_shader_instructions')
    except Exception:  # noqa
        n = -1
    for name, kw in CARMARK_INSTANCES.items():
        if n == 0:
            if AL.does_asset_exist('/Game/FX/MI_FX_Car' + name):
                AL.delete_asset('/Game/FX/MI_FX_Car' + name)
        else:
            ft_mats.mi(m, '/Game/FX', 'MI_FX_Car' + name, **kw)
    return n


def overhaul_definitions():
    """The combat-effects overhaul: what a missile, a blast, a ram and the KESTREL's own driving show now
    (DevLog/jam/overhaul/fx/notes.md has the table, the counts and what each replaced). FTFx.cpp spawns them: a blast
    is a recipe of these layers by what blew up (FTFx::Explosion), a missile's smoke is laid in the world behind it
    (FTFx::Trail). One emitter each, as everywhere here. Directions: +X is the way of the hit or of travel, +Z is up
    off the road (the blast layers are spawned turned to the road's frame)."""
    d = []

    def add(name, cfg):
        d.append((name, LOOP if 'Type=Rate' in cfg['emitter']['SpawnInfos'] else BURST, cfg))

    fire = heat((C_FIRE, 1.0), (C_FIRE, 0.86), (C_FIRE, 0.62), (C_FIRE, 0.36), (C_FIRE, 0.0))
    # ---- a blast's fire. The fireball: seven billowing banded lobes that roll out, cool from the middle and break up
    # (it replaces the stock additive fireball in Explode, which washed the screen white and had no shape)
    ball = dict(n=8, life=(0.38, 0.62), size=((170, 170), (280, 280)), grow=(0.42, 0.95, 1.12, 1.2), speed=(240, 680), cone=115.0, drag=3.0, gravity=220.0,
                sphere=55.0, rot=True, spin=40.0, sort=True, pool=(8, 24), toward=220.0)
    add('NS_FxFireball', fx(mat=toon('Fire'), ramp=fire, **ball))
    for tint, c in (('s_blue_fire', (0.12, 0.45, 1.0)), ('s_toxic_blast', (0.25, 0.9, 0.12))):
        # the SFX DEPT's wreck colours: the same fireball, its bands worked out of the one colour
        add('NS_FxFireball_' + tint, fx(mat=toon('FireTint'), ramp=heat((c, 1.0), (c, 0.86), (c, 0.62), (c, 0.36), (c, 0.0)), **ball))
    # the roll: fire thrown on along the hit (a car hit from behind burns forward, a missile's blast goes on its way)
    add('NS_FxFireRoll', fx(n=5, life=(0.30, 0.50), size=((150, 150), (240, 240)), mat=toon('Fire'), ramp=fire, grow=(0.5, 1.0, 1.15),
                            speed=(700, 1500), cone=24.0, axis=(1, 0, 0.22), drag=2.6, gravity=150.0, sphere=30.0, rot=True, spin=40.0, sort=True, pool=(4, 12),
                            toward=180.0))
    # the jet: a column of flame straight up (a fuel drum, the last blast of a boss)
    add('NS_FxFireJet', fx(n=6, life=(0.32, 0.52), size=((120, 160), (180, 240)), mat=toon('Flame'), ramp=fire, grow=(0.5, 1.0, 1.2),
                           speed=(500, 1500), cone=13.0, drag=2.2, sphere=30.0, sort=True, pool=(2, 8), toward=150.0))
    # the flash: one spiky star for a frame or two (the Flash layer: not with Reduced Motion). It replaces the additive
    # glow ball, which is what turned a blast you drove through into a white frame
    add('NS_FxFlash', fx(n=1, life=(0.07, 0.09), size=((380, 380), (440, 440)), mat=cel('Flash'), ramp=heat((C_WHITE, 1.0), ((1.0, 0.85, 0.45), 0.55), (C_HE, 0.0)),
                         grow=(0.45, 1.0, 1.1), rot=True, sort=True, pool=(6, 16), toward=260.0))
    # streaks: the sheet's "short bright streaks from the impact centre" (twenty, where seventy 7 cm sparks were)
    add('NS_FxStreaks', fx(n=20, life=(0.16, 0.40), size=((10, 60), (16, 120)), mat=cel('Shard'), ramp=heat((C_WHITE, 1.0), (C_HE, 0.6), (C_EMBER, 0.0)),
                           speed=(1300, 3400), cone=120.0, gravity=-500.0, stretch=(1500, 2.2), align=True, sort=True, pool=(8, 20)))
    # debris: dark chunks with ember edges, thrown on along the hit and falling (+X is the hit's way)
    chunk = heat((C_CHUNK, 1.0), (C_CHUNK, 0.7), (C_CHUNK, 0.45), (C_CHUNK, 0.3), (C_CHUNK, 0.0))
    add('NS_FxDebris', fx(n=9, life=(0.7, 1.2), size=((20, 20), (44, 44)), mat=toon('Chunk'), ramp=chunk,
                          speed=(900, 2300), cone=34.0, axis=(1, 0, 0.35), gravity=-1500.0, drag=0.2, rot=True, spin=520.0, sort=True, pool=(8, 20)))
    # ---- a blast's smoke. It goes UP: born dark in the fire, over the chase camera's eye line (3 m) in half a
    # second, lavender as it thins. Nothing is left standing on the road where the next thing is coming from
    smoke = heat((C_SMOKE_D, 1.0), (C_SMOKE, 0.9), (C_SMOKE_L, 0.7), (C_SMOKE_L, 0.45), (C_SMOKE_L, 0.0))
    add('NS_FxSmokeRise', fx(n=7, life=(1.1, 1.9), size=((120, 120), (200, 200)), mat=toon('Smoke'), ramp=smoke, grow=(0.5, 1.2, 1.7, 2.0),
                             speed=(380, 760), cone=24.0, drag=1.3, gravity=120.0, sphere=70.0, curl=(90.0, 50.0), rot=True, spin=25.0, sort=True, pool=(8, 20)))
    # the ball: an airburst's smoke (a chopper), which has nowhere to climb from
    add('NS_FxSmokeBall', fx(n=7, life=(1.0, 1.7), size=((140, 140), (220, 220)), mat=toon('Smoke'), ramp=smoke, grow=(0.5, 1.3, 1.8, 2.1),
                             speed=(150, 520), drag=2.2, gravity=60.0, sphere=80.0, curl=(80.0, 50.0), rot=True, spin=25.0, sort=True, pool=(2, 6)))
    # the column: a boss's last blast, tall and slow
    add('NS_FxSmokeColumn', fx(n=10, life=(1.6, 2.8), size=((180, 180), (300, 300)), mat=toon('Smoke'), ramp=smoke, grow=(0.5, 1.2, 1.7, 2.1),
                               speed=(500, 1500), cone=12.0, drag=1.1, gravity=140.0, sphere=90.0, curl=(110.0, 40.0), rot=True, spin=20.0, sort=True, pool=(1, 3)))
    # ---- what a blast does to the road: a ring flat on it out to the blast's real reach (it ends 100 cm from the
    # middle at scale 1: spawn it at radius / 100, as NS_BlastRing), and the scorch it leaves
    add('NS_FxGroundRing', fx(n=1, life=(0.24, 0.28), size=((200, 200), (200, 200)), mat=cel('Ring'), ramp=heat(((1.0, 0.9, 0.6), 1.0), (C_FIRE, 0.55), (C_EMBER, 0.0)),
                              grow=(0.3, 0.85, 1.0), facing=(0, 0, 1), sort=True, pool=(6, 16)))
    add('NS_FxScorch', fx(n=1, life=(3.6, 4.4), size=((260, 260), (300, 300)), mat=toon('Scorch'),
                          ramp=heat((C_SOOT, 1.0), (C_SOOT, 1.0), (C_SOOT, 0.8), (C_SOOT, 0.5), (C_SOOT, 0.0)), grow=(0.75, 1.0, 1.0, 1.0),
                          facing=(0, 0, 1), rot=True, sort=True, pool=(8, 32)))
    # embers that climb with the smoke (eight, where sixteen hung in the air)
    add('NS_FxEmbers', fx(n=8, life=(0.9, 1.8), size=((7, 7), (13, 13)), mat=GLOW, ramp=[(9, 5, 1.2, 1), (6, 2, 0.3, 0.9), (2, 0.3, 0.05, 0)],
                          speed=(250, 700), cone=30.0, drag=1.2, gravity=60.0, sphere=70.0, pool=(6, 16)))
    # ---- the missile. Its smoke, laid in the world in 8 m lengths as it flies (FTFx::Trail turns each length along
    # the flight): born small and warm from the flame, it spreads to three times the size, wanders, breaks into lumps
    # and is gone. Near is the same smoke with a short life, for the lengths the car is about to drive through
    exhaust = heat((C_EXHAUST, 1.0), (C_SMOKE_L, 0.92), (C_SMOKE_L, 0.82), (C_SMOKE, 0.68), (C_SMOKE, 0.5), (C_SMOKE, 0.28), (C_SMOKE, 0.0))
    trail = dict(n=8, size=((54, 54), (82, 82)), mat=toon('Smoke'), ramp=exhaust, speed=(30, 130), cone=60.0, drag=1.0, box=(800, 26, 26),
                 curl=(70.0, 50.0), rot=True, spin=35.0, sort=True, owner=False)
    add('NS_MissileSmoke', fx(life=(1.0, 1.5), grow=(0.6, 1.5, 2.2, 2.6), pool=(32, 96), **trail))
    # a lock-on rocket coming down on its mark (a hazard: FTEntity.cpp, AFTHazard::SetThing) keeps the line of smoke it
    # has had since 3.1. It rides with the rocket (the mark rides with the car, so world smoke would be wrong there)
    add('NS_RocketTrail', fx(rate=46, life=(0.22, 0.36), size=((26, 26), (40, 40)), mat=cel('Puff'), ramp=heat((C_SMOKE_L, 1.0), (C_SMOKE, 0.7), (C_SMOKE, 0.0)),
                             grow=(0.5, 1.4, 2.2), speed=(2400, 3400), cone=5.0, axis=(-1, 0, 0), sort=True, owner=False))
    add('NS_MissileSmokeNear', fx(life=(0.42, 0.62), grow=(0.6, 1.4, 1.9), pool=(8, 32), **trail))
    # ignition: the motor lights on the rail, a short hard jet of flame back off it (on the car)
    add('NS_MissileIgnite', fx(n=6, life=(0.08, 0.14), size=((20, 60), (32, 100)), mat=cel('Shard'),
                               ramp=heat(((1.0, 0.97, 0.82), 1.0), ((1.0, 0.7, 0.2), 0.55), ((1.0, 0.3, 0.05), 0.0)),
                               speed=(900, 1900), cone=20.0, axis=(-1, 0, 0.1), stretch=(900, 1.8), align=True, sort=True, pool=(2, 6)))
    # ---- the KESTREL's rounds: a streak of the load's own colour behind each one (the foes' rounds have had theirs
    # since 3.1; the player's were a bare tracer)
    for name, r in (('NS_TracerTrail', [(12, 8, 2, 1), (8, 4, 0.6, 0.8), (2, 0.5, 0.05, 0)]), ('NS_TracerTrailAP', [(5, 9, 14, 1), (2, 5, 10, 0.8), (0.3, 1, 3, 0)]),
                    ('NS_TracerTrailHE', [(14, 5, 1, 1), (9, 2.4, 0.3, 0.8), (2, 0.3, 0.05, 0)])):
        add(name, fx(rate=60, life=(0.05, 0.09), size=((10, 50), (14, 74)), mat=SPARK, ramp=r, speed=(2000, 2800), cone=3.0, axis=(-1, 0, 0),
                     stretch=(2000, 1.6), align=True, owner=False))
    # ---- a ram: paint and trim knocked off where the two cars meet
    add('NS_RamChips', fx(n=5, life=(0.35, 0.6), size=((12, 12), (26, 26)), mat=toon('Chunk'), ramp=heat((C_CHUNK, 0.5), (C_CHUNK, 0.4), (C_CHUNK, 0.0)),
                          speed=(500, 1400), cone=60.0, axis=(0.3, 0, 1), gravity=-1400.0, rot=True, spin=600.0, sort=True))
    # ---- the drift. The charge had no look in the world (only the HUD said BLUE, ORANGE, PINK): sparks off the rear
    # tyres in the charge's colour, and a ring that pops as each charge is reached
    for i, c in enumerate(C_DRIFT):
        add('NS_DriftSparks%d' % (i + 1), fx(rate=44, life=(0.12, 0.22), size=((6, 30), (10, 56)), mat=cel('Shard'), ramp=heat((C_WHITE, 1.0), (c, 0.6), (c, 0.0)),
                                             speed=(500, 1300), cone=32.0, axis=(-1, 0, 0.4), gravity=-900.0, box=(16, 236, 4), stretch=(700, 1.8),
                                             align=True, sort=True, owner=False))
        add('NS_DriftPop%d' % (i + 1), fx(n=1, life=(0.16, 0.20), size=((170, 170), (190, 190)), mat=cel('Ring'), ramp=heat((C_WHITE, 1.0), (c, 0.55), (c, 0.0)),
                                          grow=(0.3, 0.85, 1.0), sort=True))
    # ---- the boost and the turbo's punch: a kick of flame out of the pipes and a ring, as it lets go. (Round 3: the kick
    # was eight long shards laid along their flight: from the chase camera, straight behind the car, they were seen
    # end-on, as cream bits. It is six banded lobes of fire now, facing the camera, that swell, cool to dark and break
    # up as they are left behind)
    add('NS_BoostKick', fx(n=6, life=(0.16, 0.26), size=((60, 60), (96, 96)), mat=toon('Fire'), ramp=heat((C_FIRE, 1.0), (C_FIRE, 0.8), (C_FIRE, 0.5), (C_FIRE, 0.0)),
                           grow=(0.55, 1.0, 1.15), speed=(500, 1400), cone=20.0, axis=(-1, 0, 0.06), drag=3.0, box=(4, 110, 8), rot=True, spin=40.0, sort=True,
                           toward=60.0))
    add('NS_BoostRing', fx(n=1, life=(0.18, 0.22), size=((190, 190), (210, 210)), mat=cel('Ring'), ramp=heat((C_WHITE, 1.0), ((1.0, 0.75, 0.3), 0.55), (C_FIRE, 0.0)),
                           grow=(0.3, 0.9, 1.0), sort=True, toward=90.0))
    # the flame's hot core while the turbo or a boost burns: white going blue, so it sits under any nitro colour
    add('NS_TurboCore', fx(rate=40, life=(0.05, 0.08), size=((14, 46), (20, 70)), mat=cel('Shard'),
                           ramp=heat(((1.0, 0.98, 0.9), 1.0), ((0.9, 0.95, 1.0), 0.6), ((0.6, 0.8, 1.0), 0.0)),
                           speed=(900, 1500), cone=5.0, axis=(-1, 0, 0), box=(4, 110, 8), stretch=(900, 1.5), align=True, sort=True, owner=False))
    # ---- a landing: dust thrown out flat from under the car, sparks off its belly
    add('NS_LandDust', fx(n=9, life=(0.26, 0.42), size=((56, 56), (92, 92)), mat=toon('SmokeCar'), ramp=heat((C_DUST, 1.0), (C_DUST, 0.6), (C_DUST, 0.0)),
                          grow=(0.6, 1.4, 1.8), speed=(600, 1300), cone=82.0, inner=35.0, axis=(-1, 0, 0.1), drag=3.5, box=(260, 200, 6), rot=True, sort=True))
    add('NS_LandSparks', fx(n=14, life=(0.12, 0.30), size=((5, 10), (9, 16)), mat=SPARK, ramp=[(11, 8, 3, 1), (6, 2, 0.4, 1), (2, 0.3, 0.05, 0)],
                            speed=(700, 2200), cone=50.0, axis=(-1, 0, 0.25), gravity=-900.0, box=(300, 180, 4), stretch=(1300, 9.0), align=True))
    # ---- a wreck burns as the sheet draws it: banded flames with pointed tongues (it was NS_Fire's white-hot puffs).
    # It takes its owner's scale: AFTWreck sizes the fire to what is burning (a truck's is twice a car's)
    add('NS_WreckFire', fx(rate=13, life=(0.30, 0.48), size=((90, 120), (140, 190)), mat=toon('Flame'), ramp=heat((C_FIRE, 1.0), (C_FIRE, 0.8), (C_FIRE, 0.5), (C_FIRE, 0.0)),
                           grow=(0.7, 1.1, 0.8), speed=(180, 380), cone=14.0, world=True, sphere=45.0, sort=True, toward=140.0))
    # ================================================================================================ FX round 3: weight
    # ---- a foe wears what it has left (FTFx::FoeLook moves these with the foe: +X is its nose, +Z its roof). Smoke
    # once it has lost a third of its armour: thin dark puffs streaming back over its tail and up, off the middle of
    # its silhouette. Flames under a third. Sparks at each step. Sized by the body (the owner's scale)
    add('NS_FoeSmoke', fx(rate=10, life=(0.60, 0.95), size=((62, 62), (104, 104)), mat=toon('SmokeFoe'),
                          ramp=heat((C_SMOKE_D, 1.0), (C_SMOKE, 0.7), (C_SMOKE_L, 0.4), (C_SMOKE_L, 0.0)), grow=(0.6, 1.3, 1.8),
                          speed=(320, 620), cone=20.0, axis=(-1, 0, 0.9), drag=1.0, curl=(60.0, 50.0), rot=True, spin=30.0, sort=True, pool=(4, 12)))
    add('NS_FoeFire', fx(rate=12, life=(0.24, 0.40), size=((78, 104), (120, 160)), mat=toon('Flame'), ramp=heat((C_FIRE, 1.0), (C_FIRE, 0.8), (C_FIRE, 0.5), (C_FIRE, 0.0)),
                         grow=(0.7, 1.1, 0.8), speed=(200, 420), cone=16.0, axis=(-0.5, 0, 1), sphere=30.0, sort=True, toward=90.0, pool=(4, 12)))
    add('NS_FoeHurt', fx(n=9, life=(0.16, 0.34), size=((12, 56), (20, 104)), mat=cel('Shard'), ramp=heat((C_WHITE, 1.0), (C_HE, 0.6), (C_EMBER, 0.0)),
                         speed=(700, 1900), cone=70.0, axis=(-0.4, 0, 1), gravity=-700.0, stretch=(1200, 2.0), align=True, sort=True, pool=(2, 8)))
    # ---- each foe's own gun, all in their colour. A biker's pistol: one small sharp star, a thin quick dart. A
    # gunship's machine gun: a long tongue of flame stabbed out past the star, a fat slug with a long streak, and a
    # ring where it lands on the KESTREL. (A boss keeps the big star and its own rounds.)
    add('NS_EnemyMuzzleBike', fx(n=1, life=(0.05, 0.065), size=((96, 96), (116, 116)), mat=cel('Flash'), ramp=heat((C_FOE, 1.0), (C_FOE, 0.55), (C_FOE, 0.0)),
                                 grow=(0.55, 1.0, 1.1), rot=True, sort=True, pool=(2, 8)))
    add('NS_EnemyMuzzleGun', fx(n=3, life=(0.06, 0.10), size=((26, 110), (38, 170)), mat=cel('Shard'),
                                ramp=heat(((1.0, 0.75, 0.7), 1.0), (C_FOE, 0.6), (C_FOE, 0.0)),
                                speed=(1400, 2600), cone=7.0, axis=(1, 0, 0), stretch=(1400, 2.2), align=True, sort=True, pool=(2, 8)))
    add('NS_EnemyTrailBike', fx(rate=44, life=(0.05, 0.08), size=((10, 26), (14, 38)), mat=SPARK, ramp=[(14, 1.2, 3.6, 1), (8, 0.4, 1.9, 0.8), (2, 0.05, 0.5, 0)],
                                speed=(2200, 3000), cone=3.0, axis=(-1, 0, 0), stretch=(2000, 2.0), align=True, owner=False))
    add('NS_EnemyTrailGun', fx(rate=70, life=(0.10, 0.16), size=((28, 46), (38, 64)), mat=SPARK, ramp=[(14, 1.2, 3.6, 1), (8, 0.4, 1.9, 0.8), (2, 0.05, 0.5, 0)],
                               speed=(2200, 3000), cone=3.0, axis=(-1, 0, 0), stretch=(2000, 2.4), align=True, owner=False))
    # ---- a wreck drags on the road: sparks thrown back along its slide (FTEntity.cpp moves it under the body; +X is
    # the way the wreck is going, +Z off the road)
    add('NS_WreckScrape', fx(rate=34, life=(0.12, 0.26), size=((6, 28), (10, 52)), mat=cel('Shard'), ramp=heat((C_WHITE, 1.0), (C_HE, 0.6), (C_EMBER, 0.0)),
                             speed=(500, 1500), cone=28.0, axis=(-1, 0, 0.45), gravity=-900.0, box=(60, 110, 4), stretch=(700, 1.8), align=True, sort=True,
                             owner=False))
    add('NS_HitPlayerRing', fx(n=1, life=(0.14, 0.17), size=((120, 120), (140, 140)), mat=cel('Ring'), ramp=heat(((1.0, 0.6, 0.6), 1.0), (C_FOE, 0.6), (C_FOE, 0.0)),
                               grow=(0.35, 0.9, 1.0), sort=True, pool=(2, 6)))
    # ================================================================================================ FX round 3: the car in the world
    # ---- what the KESTREL's rear wheels throw off the road, level by level (FTData's order of levels; the car picks
    # NS_WheelA<n> and NS_WheelB<n> by the level: FTPlayerCar.cpp, SetLightsForLevel). A runs whenever the car is at
    # speed on its wheels (the Main layer: a few small sprites); B is the rooster tail of a drift, a boost or the
    # turbo (Extra). The car turns both to the way it is really going: -X is back along its line, +Z off the road,
    # and the box is the two rear tyres. NS_LandKick<n> is what a landing throws out flat from under the car
    # (NS_LandDust, in the level's own stuff)
    def puff(c, rate, life, size, speed, up=0.35, grow=(0.6, 1.3, 1.8), drag=2.4, gravity=None):
        return fx(rate=rate, life=life, size=size, mat=toon('SmokeCar'), ramp=heat((c, 1.0), (c, 0.6), (c, 0.0)), grow=grow, speed=speed, cone=16.0,
                  axis=(-1, 0, up), drag=drag, gravity=gravity, box=(16, 236, 4), rot=True, spin=40.0, sort=True, owner=False)

    def drops(c, rate, life=(0.18, 0.34), size=((8, 8), (16, 16)), speed=(500, 1300), up=0.7):
        return fx(rate=rate, life=life, size=size, mat=cel('Blob'), ramp=heat((C_WHITE, 1.0), (c, 0.6), (c, 0.0)), speed=speed, cone=26.0,
                  axis=(-1, 0, up), gravity=-1300.0, box=(16, 236, 4), sort=True, owner=False)

    def scraps(ramp, rate, life=(0.5, 0.9), size=((9, 9), (17, 17)), speed=(400, 1100), up=0.8):
        # flat chips that tumble (the toon chunk: its heat is kept under 0.45, so it has no ember edge)
        return fx(rate=rate, life=life, size=size, mat=toon('Chunk'), ramp=ramp, speed=speed, cone=34.0, axis=(-1, 0, up), gravity=-500.0, drag=1.6,
                  box=(16, 236, 4), rot=True, spin=500.0, sort=True, owner=False)

    SAND, SPRAY = (0.78, 0.64, 0.42), (0.78, 0.94, 1.0)
    SLICK, STEAM = (0.40, 0.95, 0.22), (0.80, 0.90, 0.84)
    DUST, ASH, SILT = (0.66, 0.44, 0.27), (0.19, 0.17, 0.20), (0.10, 0.24, 0.26)
    RAIN, PAPER = (0.70, 0.82, 1.0), (0.86, 0.84, 0.78)
    confetti = heat(((1.0, 0.25, 0.60), 0.44), ((1.0, 0.82, 0.20), 0.40), ((0.20, 0.90, 1.0), 0.34), ((0.20, 0.90, 1.0), 0.0))
    paper = heat((PAPER, 0.44), (PAPER, 0.38), (PAPER, 0.0))
    small, big = ((34, 34), (56, 56)), ((56, 56), (96, 96))
    wheels = (
        # 0 Bayshore: sand blown over the tarmac; sea spray in the slide
        (puff(SAND, 12, (0.22, 0.36), small, (500, 1000)),
         drops(SPRAY, 34),
         SAND),
        # 1 Chemical Works: the slick comes off the tyres in green drops; steam where they slide through it
        (drops(SLICK, 16, size=((7, 7), (13, 13)), speed=(400, 1000), up=0.5),
         puff(STEAM, 22, (0.40, 0.70), big, (300, 700), up=0.9, drag=1.6, gravity=260.0),
         STEAM),
        # 2 Nevada: dust, and a lot of it in the slide
        (puff(DUST, 16, (0.30, 0.50), small, (500, 1000)),
         puff(DUST, 26, (0.45, 0.80), big, (500, 1100), up=0.6, grow=(0.6, 1.5, 2.2), drag=1.8),
         DUST),
        # 3 Volcano: ash off the road; embers in the slide
        (puff(ASH, 12, (0.26, 0.42), small, (500, 1000)),
         fx(rate=34, life=(0.30, 0.60), size=((6, 6), (11, 11)), mat=GLOW, ramp=[(9, 5, 1.2, 1), (6, 2, 0.3, 0.9), (2, 0.3, 0.05, 0)], speed=(400, 1200), cone=30.0,
            axis=(-1, 0, 0.8), drag=1.2, gravity=120.0, box=(16, 236, 4), owner=False),
         ASH),
        # 4 Theme Park: confetti scraps, pink to yellow to cyan as they fall
        (scraps(confetti, 10),
         scraps(confetti, 30, speed=(600, 1500), up=1.1),
         (1.0, 0.55, 0.75)),
        # 5 Ice Mountain: powder snow; a rooster tail of it in the slide
        (puff(C_SNOW, 16, (0.26, 0.44), small, (500, 1000)),
         puff(C_SNOW, 34, (0.45, 0.75), big, (800, 1600), up=0.9, grow=(0.6, 1.4, 2.0), drag=2.0),
         C_SNOW),
        # 6 Under the Sea: silt off the bed; bubbles in the slide (rings that rise)
        (puff(SILT, 12, (0.40, 0.70), small, (300, 700), up=0.25, drag=3.0),
         fx(rate=26, life=(0.55, 0.95), size=((10, 10), (24, 24)), mat=cel('Ring'), ramp=heat((C_WHITE, 0.9), (C_SEA, 0.6), (C_SEA, 0.3)), speed=(150, 420), cone=40.0,
            axis=(-0.6, 0, 1), drag=1.5, gravity=420.0, box=(16, 236, 4), sort=True, owner=False),
         SILT),
        # 7 City Center: the rain film off the tyres in fine streaks; paper in the slide
        (fx(rate=28, life=(0.10, 0.20), size=((4, 26), (7, 48)), mat=cel('Shard'), ramp=heat((C_WHITE, 0.9), (RAIN, 0.6), (RAIN, 0.0)), speed=(700, 1500), cone=22.0,
            axis=(-1, 0, 0.5), gravity=-900.0, box=(16, 236, 4), stretch=(700, 1.8), align=True, sort=True, owner=False),
         scraps(paper, 9, size=((14, 14), (26, 26))),
         RAIN),
    )
    for i, (run, tail, land) in enumerate(wheels):
        add('NS_WheelA%d' % i, run)
        add('NS_WheelB%d' % i, tail)
        add('NS_LandKick%d' % i, fx(n=9, life=(0.26, 0.42), size=((56, 56), (92, 92)), mat=toon('SmokeCar'), ramp=heat((land, 1.0), (land, 0.6), (land, 0.0)),
                                    grow=(0.6, 1.4, 1.8), speed=(600, 1300), cone=82.0, inner=35.0, axis=(-1, 0, 0.1), drag=3.5, box=(260, 200, 6), rot=True, sort=True))

    # ---- the turbo's flame, banded. On the car it burns in the place of the additive tuft NS_TurboFlame (which stays as it
    # is, and as ft_v26 tints it: FTPlayerCar.cpp, TickFx). Camera-facing lobes streaming back out of the pipes and
    # shrinking to a point as they cool: from the chase camera, straight behind, a banded heart of flame across the tail
    # where the tuft was a white additive glow. One for the stock flame and one for each nitro colour (07 SFX DEPT),
    # its bands worked out of the one colour
    def lin(hexs):
        h = hexs.lstrip('#')
        return tuple((v / 12.92 if v <= 0.04045 else ((v + 0.055) / 1.055) ** 2.4) for v in (int(h[i:i + 2], 16) / 255.0 for i in (0, 2, 4)))

    tflame = dict(rate=54, life=(0.10, 0.17), size=((36, 36), (58, 58)), grow=(1.0, 0.72, 0.34), speed=(700, 1250), cone=6.0, axis=(-1, 0, 0),
                  box=(4, 110, 8), rot=True, sort=True, owner=False)
    add('NS_TurboFlameCel', fx(mat=toon('Fire'), ramp=heat((C_FIRE, 1.0), (C_FIRE, 0.82), (C_FIRE, 0.58), (C_FIRE, 0.30)), **tflame))
    for cid, c in (('s_blue_flame', lin('#3d7bff')), ('s_methanol_green', lin('#9dff8a')), ('s_purple_haze', lin('#9b4dff')), ('s_liquid_gold', lin('#ffc93c')),
                   ('s_white_hot', (0.72, 0.84, 1.0))):
        add('NS_TurboFlameCel_' + cid, fx(mat=toon('FireTint'), ramp=heat((c, 1.0), (c, 0.82), (c, 0.58), (c, 0.30)), **tflame))
    add('NS_TurboFlameCel_s_rainbow_nitro', fx(mat=toon('FireTint'), ramp=heat(((1.0, 0.10, 0.10), 1.0), ((1.0, 0.80, 0.05), 0.82), ((0.10, 0.90, 0.20), 0.62),
                                                                              ((0.10, 0.45, 1.0), 0.45), ((0.60, 0.15, 1.0), 0.30)), **tflame))
    # ---- the exhaust at top speed, when neither the turbo nor a boost burns: short banded licks out of the pipes
    add('NS_ExhaustFlame', fx(rate=18, life=(0.06, 0.10), size=((20, 20), (34, 34)), mat=toon('Fire'), ramp=heat((C_FIRE, 1.0), (C_FIRE, 0.7), (C_FIRE, 0.30)),
                              grow=(1.0, 0.7, 0.4), speed=(350, 800), cone=8.0, axis=(-1, 0, 0), box=(4, 110, 8), rot=True, sort=True, owner=False))
    # ---- speed: thin streaks past the edges of the picture while the turbo or a boost burns and at top speed. The car puts
    # two of these by the camera, one each side of where it looks (FTPlayerCar.cpp, TickFx: +X is the way the camera
    # looks; the box is 14 m on and 9.5 m out to one side, well off the road), and the streaks fly back past the lens.
    # The Flash layer (off with Reduced Motion), not at Low
    add('NS_SpeedLines', fx(rate=36, life=(0.10, 0.16), size=((4, 150), (7, 300)), mat=cel('Shard'), ramp=heat((C_WHITE, 0.9), ((0.75, 0.9, 1.0), 0.6), ((0.6, 0.8, 1.0), 0.0)),
                            speed=(7000, 10000), cone=1.5, axis=(-1, 0, 0), box=(600, 350, 600), stretch=(4000, 1.6), align=True, sort=True, owner=False))
    # ---- the impact card: a missile kill or a turbo ram lands on a flat white star over an ink-dark one, there for two
    # frames, with the blast starting behind it (FTFx::Explosion). The Flash layer, not at Low
    C_INK = (0.02, 0.015, 0.035)
    add('NS_ImpactCard', fx(n=1, life=(0.05, 0.06), size=((430, 430), (470, 470)), mat=cel('Flash'), ramp=heat((C_WHITE, 1.0), (C_WHITE, 0.9), (C_WHITE, 0.7)),
                            rot=True, sort=True, pool=(2, 6), toward=300.0))
    add('NS_ImpactInk', fx(n=1, life=(0.05, 0.06), size=((580, 580), (620, 620)), mat=cel('Flash'), ramp=heat((C_INK, 1.0), (C_INK, 0.9), (C_INK, 0.7)),
                           rot=True, sort=True, pool=(2, 6), toward=280.0))
    # ================================================================================================ FX round 4: the "next" list
    # ---- the KESTREL wears its damage (FTPlayerCar.cpp, SetDamageLook): a cracked lamp or screen comes on with a few
    # chips of glass thrown off it (pale, flat, tumbling: the toon chunk with its heat kept under the ember edge's)
    C_GLASS = (0.62, 0.76, 0.95)
    add('NS_HitGlass', fx(n=6, life=(0.30, 0.55), size=((7, 7), (15, 15)), mat=toon('Chunk'), ramp=heat((C_GLASS, 0.44), (C_GLASS, 0.36), (C_GLASS, 0.0)),
                          speed=(300, 900), cone=55.0, axis=(1, 0, 0.3), gravity=-1200.0, rot=True, spin=600.0, sort=True))
    # ---- a sliding wreck leaves a burning trail (FTFx::WreckTrail): a lick of flame on the scorched road behind it, three
    # small tongues spread along a 3 m length (+X along the trail), burnt out in under a second
    add('NS_TrailFire', fx(n=3, life=(0.5, 0.9), size=((40, 56), (62, 86)), mat=toon('Flame'), ramp=heat((C_FIRE, 1.0), (C_FIRE, 0.8), (C_FIRE, 0.5), (C_FIRE, 0.0)),
                           grow=(0.6, 1.0, 0.7), speed=(60, 160), cone=20.0, box=(300, 60, 4), sort=True, pool=(4, 12), owner=False))
    # ---- the other foes' weapons, each in its own shape (FTGameMode.cpp: effect calls only, in TickEnts and FireBullet)
    # a truck's drum comes off its tail with a kick of dust low along the road
    add('NS_BarrelDrop', fx(n=6, life=(0.28, 0.46), size=((46, 46), (78, 78)), mat=toon('SmokeCar'), ramp=heat((C_DUST, 1.0), (C_DUST, 0.6), (C_DUST, 0.0)),
                            grow=(0.6, 1.4, 1.8), speed=(300, 800), cone=80.0, inner=40.0, drag=3.0, sphere=30.0, rot=True, sort=True))
    # a chopper's rockets leave its pods: a jet of their fire back off the launch (+X is the way the rocket goes)
    foe_fire = heat((C_FOE, 1.0), (C_FOE, 0.8), (C_FOE, 0.5), (C_FOE, 0.0))
    add('NS_FoeRocketFlash', fx(n=5, life=(0.10, 0.18), size=((22, 90), (34, 150)), mat=cel('Shard'), ramp=heat(((1.0, 0.75, 0.7), 1.0), (C_FOE, 0.6), (C_FOE, 0.0)),
                                speed=(1200, 2400), cone=16.0, axis=(-1, 0, 0), stretch=(1200, 2.0), align=True, sort=True, pool=(2, 6)))
    # a Snake Eyes' chain: links lashing out across to its real reach (5 m), in their colour (+X is toward the car)
    add('NS_ChainArc', fx(n=6, life=(0.10, 0.14), size=((8, 60), (12, 110)), mat=cel('Shard'), ramp=heat((C_WHITE, 1.0), (C_FOE, 0.6), (C_FOE, 0.0)),
                          speed=(3000, 4400), cone=14.0, axis=(1, 0, 0.05), stretch=(2500, 2.0), align=True, sort=True, pool=(2, 6)))
    # a rammer's charge starts with a kick of their fire out of its tail (banded lobes worked out of their colour)
    add('NS_RammerLaunch', fx(n=5, life=(0.18, 0.30), size=((60, 60), (100, 100)), mat=toon('FireTint'), ramp=foe_fire, grow=(0.55, 1.0, 1.15),
                              speed=(500, 1300), cone=22.0, axis=(-1, 0, 0.1), drag=3.0, box=(4, 120, 8), rot=True, spin=40.0, sort=True, toward=60.0))
    # MR. CHUCKLES' pies: cream thrown off where one leaves the barrel or lands
    C_CREAM = (1.0, 0.94, 0.80)
    add('NS_PieSplat', fx(n=8, life=(0.25, 0.45), size=((16, 16), (34, 34)), mat=cel('Blob'), ramp=heat((C_WHITE, 1.0), (C_CREAM, 0.7), (C_CREAM, 0.0)),
                          speed=(300, 1100), cone=75.0, gravity=-1100.0, sort=True, pool=(2, 8)))
    return d


def build_all():
    if not AL.does_asset_exist('/Game/FX/MI_FX_CelBurst'):
        # the cel sprite material's instances come first (several systems here point at them)
        import ft_attackfx
        ft_attackfx.cel_materials()
    toon_materials()   # FX overhaul: M_FX_Toon and its instances (several systems below point at them)
    mark_materials()   # FX round 3: M_FX_Mark and its instances (what FTFx.cpp lays on the road)
    car_mark_materials()   # FX round 4: M_FX_CarMark and its instances (what the KESTREL wears as its armour drops)
    made = [make(name, base, cfg) for name, base, cfg in definitions() + attack_definitions() + overhaul_definitions()]
    for p in ('_probe_FountainLightweight', '_probe_DirectionalBurstLightweight', '_probe_MinimalLightweight'):
        if AL.does_asset_exist(FX + p):
            AL.delete_asset(FX + p)
    return 'built: ' + ', '.join(s.get_name() for s in made)
