"""FULL THROTTLE (Unreal) Niagara effects, built from the engine's lightweight (stateless) templates.

The emitter stacks can't be edited from Python, so this uses the project's editor tool
(UFTEditorTools.SetPath) to set the stateless modules' properties by reflection, then recompiles.
Run over the Unreal MCP connector:  import ft_fx; ft_fx.build_all()
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


def fx(n=None, rate=None, life=(0.2, 0.3), size=((50, 50), (80, 80)), mat=None, ramp=None, grow=None, speed=None, cone=180.0, axis=(0, 0, 1),
       inner=0.0, world=False, drag=None, gravity=None, sphere=None, box=None, rot=False, spin=None, stretch=None, align=False, sort=False,
       owner=True, pool=None, bounds=BIG):
    """One stateless emitter as a make() config. n = a burst of n sprites (the system ends by itself), rate = sprites a
    second for as long as the component is active. speed/cone/axis: a cone of velocity (cm/s, half-angle in degrees,
    direction in the emitter's own space, or in the world's with world=True: smoke off a tumbling wreck still rises).
    Every emitter here is local space (the engine's stateless emitters always are): sprites of a moving emitter travel
    with it, so a 'trail' is a fast cone straight back, and anything short-lived on a car is spawned attached to it.
    stretch = (speed at which, times) lengthens velocity-aligned sprites; sort for the translucent cel shapes."""
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
    if speed is not None:
        mods['AddVelocity'] = {'VelocityType': 'InCone', 'ConeAngle': '%f' % cone, 'InnerCone': '%f' % inner, 'ConeVelocityDistribution': rng(*speed),
                               'ConeRotationType': 'Direction', 'ConeDirection': vconst(*axis), 'CoordinateSpace': 'World' if world else 'Local'}
    if drag is not None:
        mods['Drag'] = {'DragDistribution': const(drag)}
    if gravity is not None:
        mods['GravityForce'] = {'GravityDistribution': vconst(0, 0, gravity)}
    if speed is not None or drag is not None or gravity is not None:
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
    made.append(('NS_Smoke', LOOP, {
        'emitter': {'SpawnInfos': '((Type=Rate,Rate=(Min=16,Max=16)))', 'FixedBounds': BIG},
        'modules': {
            'InitializeParticle': {'LifetimeDistribution': rng(1.2, 2.2), 'SpriteSizeDistribution': v2rng((60, 60), (110, 110)), 'SpriteRotationDistribution': rng(0, 360)},
            'ShapeLocation': {'ShapePrimitive': 'Sphere'},
            'AddVelocity': {'VelocityType': 'InCone', 'ConeAngle': '25.0', 'ConeVelocityDistribution': rng(150, 320)},
            'Drag': {'DragDistribution': const(0.8)},
            'SolveVelocitiesAndForces': {},
            'ScaleColor': {'ScaleDistribution': colors((0.08, 0.07, 0.08, 0), (0.05, 0.045, 0.05, 0.55), (0.03, 0.03, 0.035, 0))},
            'ScaleSpriteSize': {'ScaleDistribution': sizes(0.6, 2.0, 3.4)},
            'SpriteRotationRate': {'RotationRateDistribution': rng(-30, 30)},
        },
        'renderer': {'Material': mat('/Game/FX/M_FX_Smoke.M_FX_Smoke')},
    }))
    # ---- scraping the barrier, impacts
    made.append(('NS_Sparks', LOOP, {
        'emitter': {'SpawnInfos': '((Type=Rate,Rate=(Min=240,Max=240)))', 'FixedBounds': BIG},
        'modules': {
            'InitializeParticle': {'LifetimeDistribution': rng(0.14, 0.40), 'SpriteSizeDistribution': v2rng((4, 7), (8, 13))},
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
    made.append(('NS_TurboFlame', LOOP, {
        'emitter': {'SpawnInfos': '((Type=Rate,Rate=(Min=108,Max=108)))', 'FixedBounds': BIG},
        'modules': {
            'InitializeParticle': {'LifetimeDistribution': rng(0.075, 0.14), 'SpriteSizeDistribution': v2rng((25, 23), (55, 50)), 'SpriteRotationDistribution': rng(0, 360)},
            'ShapeLocation': {'ShapePrimitive': 'Box', 'BoxSize': vconst(4, 110, 8)},
            'AddVelocity': {'VelocityType': 'InCone', 'ConeAngle': '8.0', 'ConeVelocityDistribution': rng(500, 900),
                            'ConeRotationType': 'Direction', 'ConeDirection': vconst(-1, 0, 0)},
            'SolveVelocitiesAndForces': {},
            'ScaleColor': {'ScaleDistribution': colors((4.5, 7.2, 10.5, 1), (2.4, 5.2, 10, 0.82), (5.1, 2.0, 0.36, 0.49), (2.5, 0.42, 0.06, 0))},
            'ScaleSpriteSize': {'ScaleDistribution': sizes(1.0, 0.4)},
        },
        'renderer': {'Material': mat('/Game/FX/M_FX_Fire.M_FX_Fire')},
    }))
    made.append(('NS_TyreSmoke', LOOP, {
        'emitter': {'SpawnInfos': '((Type=Rate,Rate=(Min=55,Max=55)))', 'FixedBounds': BIG},
        'modules': {
            'InitializeParticle': {'LifetimeDistribution': rng(0.56, 1.12), 'SpriteSizeDistribution': v2rng((78, 52), (135, 99)), 'SpriteRotationDistribution': rng(0, 360)},
            'ShapeLocation': {'ShapePrimitive': 'Box', 'BoxSize': vconst(40, 230, 10)},
            'AddVelocity': {'VelocityType': 'InCone', 'ConeAngle': '40.0', 'ConeVelocityDistribution': rng(60, 200)},
            'Drag': {'DragDistribution': const(1.5)},
            'SolveVelocitiesAndForces': {},
            'ScaleColor': {'ScaleDistribution': colors((0.5, 0.45, 0.55, 0), (0.45, 0.42, 0.5, 0.32), (0.4, 0.4, 0.45, 0))},
            'ScaleSpriteSize': {'ScaleDistribution': sizes(0.73, 1.64, 2.54)},
            'SpriteRotationRate': {'RotationRateDistribution': rng(-40, 40)},
        },
        'renderer': {'Material': mat('/Game/FX/M_FX_Smoke.M_FX_Smoke')},
    }))
    # 3.1: a trail that trails. The emitter is local space, so its old sprites (20-80 cm/s, any direction) rode along
    # as a ball round the missile. Now a fast narrow cone straight back: a line of puffs 6-12 m long, as on the sheet
    made.append(('NS_MissileTrail', LOOP, fx(rate=46, life=(0.22, 0.36), size=((26, 26), (40, 40)), mat=cel('Puff'),
                                             ramp=heat((C_SMOKE_L, 1.0), (C_SMOKE, 0.7), (C_SMOKE, 0.0)), grow=(0.5, 1.4, 2.2),
                                             speed=(2400, 3400), cone=5.0, axis=(-1, 0, 0), sort=True, owner=False)))
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
    add('NS_MuzzleTongue', fx(n=3, life=(0.045, 0.08), size=((16, 46), (24, 70)), mat=cel('Shard'),
                              ramp=heat(((1.0, 0.9, 0.6), 1.0), ((1.0, 0.6, 0.15), 0.5), ((1.0, 0.3, 0.05), 0.0)),
                              speed=(1500, 3000), cone=9.0, axis=(1, 0, 0), stretch=(1500, 2.2), align=True, sort=True, pool=(4, 12)))
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
    add('NS_ImpactArmor', fx(n=14, life=(0.10, 0.26), size=((5, 10), (9, 16)), mat=SPARK, ramp=[(9, 10, 12, 1), (6, 5, 3, 1), (2, 0.6, 0.1, 0)],
                             speed=(900, 2600), cone=75.0, axis=(-1, 0, 0.35), gravity=-900.0, stretch=(1200, 9.0), align=True, pool=(2, 8)))
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
    add('NS_MissileFlame', fx(rate=70, life=(0.05, 0.09), size=((22, 50), (34, 84)), mat=cel('Shard'),
                              ramp=heat(((1.0, 0.95, 0.7), 1.0), ((1.0, 0.6, 0.12), 0.6), ((1.0, 0.25, 0.05), 0.0)),
                              speed=(900, 1600), cone=6.0, axis=(-1, 0, 0), stretch=(900, 1.6), align=True, sort=True, owner=False))
    add('NS_MissileLaunch', fx(n=1, life=(0.09, 0.11), size=((230, 230), (270, 270)), mat=cel('Flash'), ramp=heat((C_WHITE, 1.0), (C_HE, 0.55), (C_HE, 0.0)),
                               grow=(0.5, 1.0, 1.1), rot=True, sort=True, pool=(2, 6)))
    add('NS_MissileBackblast', fx(n=7, life=(0.35, 0.6), size=((60, 60), (110, 110)), mat=cel('Puff'), ramp=heat((C_SMOKE_L, 1.0), (C_SMOKE, 0.7), (C_SMOKE, 0.0)),
                                  grow=(0.6, 1.5, 2.0), speed=(500, 1500), cone=28.0, axis=(-1, 0, 0.15), drag=3.0, sort=True))
    add('NS_MissileHit', fx(n=22, life=(0.16, 0.36), size=((12, 40), (20, 80)), mat=cel('Shard'), ramp=heat((C_WHITE, 1.0), (C_HE, 0.6), (C_EMBER, 0.0)),
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
    add('NS_WreckSmoke', fx(rate=9, life=(0.8, 1.3), size=((80, 80), (130, 130)), mat=cel('Puff'), ramp=heat((C_SMOKE_D, 1.0), (C_SMOKE, 0.6), (C_SMOKE, 0.0)),
                            grow=(0.6, 1.5, 2.1), speed=(220, 430), cone=18.0, world=True, drag=0.8, sort=True, owner=False))
    return d


def build_all():
    if not AL.does_asset_exist('/Game/FX/MI_FX_CelBurst'):
        # the cel sprite material's instances come first (several systems here point at them)
        import ft_attackfx
        ft_attackfx.cel_materials()
    made = [make(name, base, cfg) for name, base, cfg in definitions() + attack_definitions()]
    for p in ('_probe_FountainLightweight', '_probe_DirectionalBurstLightweight', '_probe_MinimalLightweight'):
        if AL.does_asset_exist(FX + p):
            AL.delete_asset(FX + p)
    return 'built: ' + ', '.join(s.get_name() for s in made)
