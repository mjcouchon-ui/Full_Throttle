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
    if AL.does_asset_exist(path):
        AL.delete_asset(path)
    s = AL.duplicate_asset(base, path)
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
HOT = ((9, 7, 4, 1), (6, 2.6, 0.6, 1), (3, 0.6, 0.12, 0.8), (0.6, 0.08, 0.04, 0.4), (0, 0, 0, 0))


def build_all():
    made = []
    # ---- explosion: a billowing fireball that lights the road
    made.append(make('NS_Explosion', BURST, {
        'emitter': {'SpawnInfos': '((Amount=(Min=46,Max=46)))', 'FixedBounds': BIG, 'EmitterState': '(LoopBehavior=Once,LoopDuration=(Min=2.0,Max=2.0,Mode=UniformConstant))'},
        'modules': {
            'InitializeParticle': {'LifetimeDistribution': rng(0.55, 1.3), 'SpriteSizeDistribution': v2rng((150, 150), (330, 330)), 'SpriteRotationDistribution': rng(0, 360)},
            'ShapeLocation': {'ShapePrimitive': 'Sphere'},
            'AddVelocity': {'VelocityType': 'InCone', 'ConeAngle': '180.0', 'ConeVelocityDistribution': rng(250, 950)},
            'Drag': {'DragDistribution': const(3.2)},
            'GravityForce': {'GravityDistribution': vconst(0, 0, 260)},
            'SolveVelocitiesAndForces': {},
            'ScaleColor': {'ScaleDistribution': colors(*HOT)},
            'ScaleSpriteSize': {'ScaleDistribution': sizes(0.6, 1.6, 2.3, 2.6)},
            'SpriteRotationRate': {'RotationRateDistribution': rng(-90, 90)},
            'ApplyOwnerScaleToAttributes': {},
        },
        'renderer': {'Material': mat('/Game/FX/M_FX_Fire.M_FX_Fire'), 'SortMode': 'ViewDepth'},
    }))
    made.append(make('NS_ExplosionSparks', BURST, {
        'emitter': {'SpawnInfos': '((Amount=(Min=70,Max=70)))', 'FixedBounds': BIG, 'EmitterState': '(LoopBehavior=Once,LoopDuration=(Min=1.6,Max=1.6,Mode=UniformConstant))'},
        'modules': {
            'InitializeParticle': {'LifetimeDistribution': rng(0.4, 1.2), 'SpriteSizeDistribution': v2rng((5, 5), (10, 10))},
            'ShapeLocation': {'ShapePrimitive': 'Sphere'},
            'AddVelocity': {'VelocityType': 'InCone', 'ConeAngle': '180.0', 'ConeVelocityDistribution': rng(900, 2600)},
            'Drag': {'DragDistribution': const(0.9)},
            'GravityForce': {},
            'SolveVelocitiesAndForces': {},
            'ScaleColor': {'ScaleDistribution': colors((10, 6, 2, 1), (6, 2, 0.4, 1), (2, 0.3, 0.05, 0))},
            'ScaleSpriteSizeBySpeed': {'VelocityThreshold': const(1800), 'MinScaleFactor': v2const(1, 1), 'MaxScaleFactor': v2const(1, 14)},
            'ApplyOwnerScaleToAttributes': {},
        },
        'renderer': {'Material': mat('/Game/FX/M_FX_Spark.M_FX_Spark'), 'Alignment': 'VelocityAligned'},
    }))
    made.append(make('NS_ExplosionSmoke', BURST, {
        'emitter': {'SpawnInfos': '((Amount=(Min=16,Max=16)))', 'FixedBounds': BIG, 'EmitterState': '(LoopBehavior=Once,LoopDuration=(Min=4.0,Max=4.0,Mode=UniformConstant))'},
        'modules': {
            'InitializeParticle': {'LifetimeDistribution': rng(1.6, 3.2), 'SpriteSizeDistribution': v2rng((220, 220), (420, 420)), 'SpriteRotationDistribution': rng(0, 360)},
            'ShapeLocation': {'ShapePrimitive': 'Sphere'},
            'AddVelocity': {'VelocityType': 'InCone', 'ConeAngle': '70.0', 'ConeVelocityDistribution': rng(120, 420)},
            'Drag': {'DragDistribution': const(1.4)},
            'GravityForce': {'GravityDistribution': vconst(0, 0, 120)},
            'SolveVelocitiesAndForces': {},
            'ScaleColor': {'ScaleDistribution': colors((0.05, 0.035, 0.04, 0.0), (0.04, 0.03, 0.035, 0.75), (0.03, 0.025, 0.03, 0.45), (0.02, 0.02, 0.025, 0.0))},
            'ScaleSpriteSize': {'ScaleDistribution': sizes(0.5, 1.6, 2.6)},
            'SpriteRotationRate': {'RotationRateDistribution': rng(-25, 25)},
            'ApplyOwnerScaleToAttributes': {},
        },
        'renderer': {'Material': mat('/Game/FX/M_FX_Smoke.M_FX_Smoke'), 'SortMode': 'ViewDepth'},
    }))
    # ---- burning wreck / burning KESTREL
    made.append(make('NS_Fire', LOOP, {
        'emitter': {'SpawnInfos': '((Type=Rate,Rate=(Min=46,Max=46)))', 'FixedBounds': BIG},
        'modules': {
            'InitializeParticle': {'LifetimeDistribution': rng(0.45, 0.9), 'SpriteSizeDistribution': v2rng((70, 70), (150, 150)), 'SpriteRotationDistribution': rng(0, 360)},
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
    made.append(make('NS_Smoke', LOOP, {
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
    made.append(make('NS_Sparks', LOOP, {
        'emitter': {'SpawnInfos': '((Type=Rate,Rate=(Min=240,Max=240)))', 'FixedBounds': BIG},
        'modules': {
            'InitializeParticle': {'LifetimeDistribution': rng(0.18, 0.5), 'SpriteSizeDistribution': v2rng((4, 4), (8, 8))},
            'AddVelocity': {'VelocityType': 'InCone', 'ConeAngle': '55.0', 'ConeVelocityDistribution': rng(500, 1500), 'ConeDirection': vconst(-1, 0, 0.6)},
            'GravityForce': {},
            'SolveVelocitiesAndForces': {},
            'ScaleColor': {'ScaleDistribution': colors((10, 7, 3, 1), (6, 2, 0.4, 1), (2, 0.2, 0.02, 0))},
            'ScaleSpriteSizeBySpeed': {'VelocityThreshold': const(1200), 'MinScaleFactor': v2const(1, 1), 'MaxScaleFactor': v2const(1, 10)},
        },
        'renderer': {'Material': mat('/Game/FX/M_FX_Spark.M_FX_Spark'), 'Alignment': 'VelocityAligned'},
    }))
    made.append(make('NS_Impact', BURST, {
        'emitter': {'SpawnInfos': '((Amount=(Min=12,Max=12)))', 'FixedBounds': BIG, 'EmitterState': '(LoopBehavior=Once,LoopDuration=(Min=0.6,Max=0.6,Mode=UniformConstant))'},
        'modules': {
            'InitializeParticle': {'LifetimeDistribution': rng(0.1, 0.35), 'SpriteSizeDistribution': v2rng((4, 4), (7, 7))},
            'AddVelocity': {'VelocityType': 'InCone', 'ConeAngle': '180.0', 'ConeVelocityDistribution': rng(400, 1300)},
            'GravityForce': {},
            'SolveVelocitiesAndForces': {},
            'ScaleColor': {'ScaleDistribution': colors((12, 9, 4, 1), (4, 1.2, 0.2, 0))},
            'ScaleSpriteSizeBySpeed': {'VelocityThreshold': const(1000), 'MinScaleFactor': v2const(1, 1), 'MaxScaleFactor': v2const(1, 8)},
        },
        'renderer': {'Material': mat('/Game/FX/M_FX_Spark.M_FX_Spark'), 'Alignment': 'VelocityAligned'},
    }))
    made.append(make('NS_Muzzle', BURST, {
        'emitter': {'SpawnInfos': '((Amount=(Min=3,Max=3)))', 'EmitterState': '(LoopBehavior=Once,LoopDuration=(Min=0.2,Max=0.2,Mode=UniformConstant))'},
        'modules': {
            'InitializeParticle': {'LifetimeDistribution': rng(0.04, 0.07), 'SpriteSizeDistribution': v2rng((35, 35), (70, 70)), 'SpriteRotationDistribution': rng(0, 360)},
            'ScaleColor': {'ScaleDistribution': colors((10, 8, 4, 1), (6, 3, 0.6, 0))},
        },
        'renderer': {'Material': mat('/Game/FX/M_FX_Glow.M_FX_Glow')},
    }))
    # ---- driving: turbo flame from the quad pipes, tyre smoke while drifting, missile trail
    made.append(make('NS_TurboFlame', LOOP, {
        'emitter': {'SpawnInfos': '((Type=Rate,Rate=(Min=140,Max=140)))', 'FixedBounds': BIG},
        'modules': {
            'InitializeParticle': {'LifetimeDistribution': rng(0.06, 0.13), 'SpriteSizeDistribution': v2rng((22, 22), (44, 44)), 'SpriteRotationDistribution': rng(0, 360)},
            'ShapeLocation': {'ShapePrimitive': 'Box', 'BoxSize': vconst(4, 110, 8)},
            'AddVelocity': {'VelocityType': 'InCone', 'ConeAngle': '8.0', 'ConeVelocityDistribution': rng(500, 900), 'ConeDirection': vconst(-1, 0, 0)},
            'SolveVelocitiesAndForces': {},
            'ScaleColor': {'ScaleDistribution': colors((3, 5, 12, 1), (8, 5, 2, 1), (4, 1, 0.2, 0))},
            'ScaleSpriteSize': {'ScaleDistribution': sizes(1.0, 0.4)},
        },
        'renderer': {'Material': mat('/Game/FX/M_FX_Fire.M_FX_Fire')},
    }))
    made.append(make('NS_TyreSmoke', LOOP, {
        'emitter': {'SpawnInfos': '((Type=Rate,Rate=(Min=55,Max=55)))', 'FixedBounds': BIG},
        'modules': {
            'InitializeParticle': {'LifetimeDistribution': rng(0.7, 1.4), 'SpriteSizeDistribution': v2rng((70, 70), (120, 120)), 'SpriteRotationDistribution': rng(0, 360)},
            'ShapeLocation': {'ShapePrimitive': 'Box', 'BoxSize': vconst(40, 230, 10)},
            'AddVelocity': {'VelocityType': 'InCone', 'ConeAngle': '40.0', 'ConeVelocityDistribution': rng(60, 200)},
            'Drag': {'DragDistribution': const(1.5)},
            'SolveVelocitiesAndForces': {},
            'ScaleColor': {'ScaleDistribution': colors((0.5, 0.45, 0.55, 0), (0.45, 0.42, 0.5, 0.32), (0.4, 0.4, 0.45, 0))},
            'ScaleSpriteSize': {'ScaleDistribution': sizes(0.6, 2.2, 3.6)},
            'SpriteRotationRate': {'RotationRateDistribution': rng(-40, 40)},
        },
        'renderer': {'Material': mat('/Game/FX/M_FX_Smoke.M_FX_Smoke')},
    }))
    made.append(make('NS_MissileTrail', LOOP, {
        'emitter': {'SpawnInfos': '((Type=Rate,Rate=(Min=70,Max=70)))', 'FixedBounds': BIG},
        'modules': {
            'InitializeParticle': {'LifetimeDistribution': rng(0.4, 0.8), 'SpriteSizeDistribution': v2rng((30, 30), (55, 55)), 'SpriteRotationDistribution': rng(0, 360)},
            'AddVelocity': {'VelocityType': 'InCone', 'ConeAngle': '30.0', 'ConeVelocityDistribution': rng(20, 80)},
            'SolveVelocitiesAndForces': {},
            'ScaleColor': {'ScaleDistribution': colors((6, 3, 1, 1), (0.4, 0.36, 0.4, 0.5), (0.3, 0.3, 0.32, 0))},
            'ScaleSpriteSize': {'ScaleDistribution': sizes(0.5, 2.5)},
        },
        'renderer': {'Material': mat('/Game/FX/M_FX_Smoke.M_FX_Smoke')},
    }))
    for p in ('_probe_FountainLightweight', '_probe_DirectionalBurstLightweight', '_probe_MinimalLightweight'):
        if AL.does_asset_exist(FX + p):
            AL.delete_asset(FX + p)
    return 'built: ' + ', '.join(s.get_name() for s in made)
