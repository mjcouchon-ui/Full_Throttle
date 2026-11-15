"""FULL THROTTLE 2.2: the 1.0 cel-shaded look, back in the Unreal game.

1.0 was drawn with M_Toon (three hard light bands, a cool shadow tint) and ink outlines baked by
tools/bake.py (ink #1b1033, on silhouettes, depth steps and creases). 2.x lights the world with Lumen.
This keeps Lumen, the shadows, the headlights and the explosion light, and turns them into cel bands in
one post-process material, M_PP_Cel, that also draws the ink:

  cel  light = scene colour / base colour (the GBuffer), cut into hard bands (lit, mid, shadow) with the
       1.0 cool shadow tint; the light's hue is kept, so neon spill and dusk light still colour the bands.
       Unlit pixels (sky, neon, fire) and emissive glow pass through untouched; the effect fades out with
       distance so the fog still hazes the skyline.
  ink  where 1/depth bends (silhouettes and steps; flat planes are 0 even at grazing angles) or the normal
       turns sharply (creases), at a width that follows the screen height.

It runs at "scene colour before DOF", before translucency, so smoke and glass are not banded or inked;
the masked cel fire (M_Fire) is opaque, so it gets ink like the 1.0 flipbooks did.

Each level gets an instance (MI_PP_Cel_<Level>) in its unbound post-process volume.
Run over the Unreal MCP connector:
    import ft_cel; ft_cel.build_materials(); ft_cel.apply_level(0); ft_cel.apply_level(1); ft_cel.apply_level(2)
"""
import math
import unreal
import ft_mats

AL = unreal.EditorAssetLibrary
MEL = unreal.MaterialEditingLibrary
AT = unreal.AssetToolsHelpers.get_asset_tools()
LES = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
EAS = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
CORE = '/Game/Core/Materials'
FX = '/Game/FX'


def hex_lin(h):
    h = h.lstrip('#')
    def f(v):
        v = v / 255.0
        return v / 12.92 if v <= 0.04045 else ((v + 0.055) / 1.055) ** 2.4
    return tuple(f(int(h[i:i + 2], 16)) for i in (0, 2, 4))


# --------------------------------------------------------------------------------------------- cel + ink
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

CEL_PARAMS = [  # (name, default) scalars
    ('Key', 1.0), ('Bright', 1.0), ('Th1', 0.62), ('Th2', 0.36), ('Hot', 1.9), ('HotLevel', 1.3), ('EmisCut', 4.0),
    ('MidLevel', 0.78), ('ShadowLevel', 0.58), ('HueKeep', 0.6), ('FadeNear', 60000.0), ('FadeFar', 160000.0),
    ('InkPx', 1.0), ('InkDepth', 0.035), ('InkNormal', 0.45), ('InkFar', 0.35), ('InkFadeNear', 15000.0),
    ('InkFadeFar', 90000.0), ('InkAmount', 1.0), ('SkyDepth', 150000.0), ('Debug', 0.0),
]
CEL_VECTORS = [('LitTint', (1, 1, 1, 1)), ('ShadowTint', (0.86, 0.84, 1.0, 1)), ('InkCol', hex_lin('1b1033') + (1,))]


def _scene_tex(m, ppi, y):
    e = MEL.create_material_expression(m, unreal.MaterialExpressionSceneTexture, -1100, y)
    e.set_editor_property('scene_texture_id', ppi)
    return e


def pp_cel():
    m = ft_mats.new_material(CORE, 'M_PP_Cel')
    m.set_editor_property('material_domain', unreal.MaterialDomain.MD_POST_PROCESS)
    m.set_editor_property('blendable_location', unreal.BlendableLocation.BL_SCENE_COLOR_BEFORE_DOF)
    col = _scene_tex(m, unreal.SceneTextureId.PPI_POST_PROCESS_INPUT0, -400)
    base = _scene_tex(m, unreal.SceneTextureId.PPI_BASE_COLOR, -250)
    dep = _scene_tex(m, unreal.SceneTextureId.PPI_SCENE_DEPTH, -100)
    nrm = _scene_tex(m, unreal.SceneTextureId.PPI_WORLD_NORMAL, 50)
    ins = [('Col', col, 'Color'), ('Base', base, 'Color'), ('Depth', dep, 'Color'), ('Nrm', nrm, 'Color')]
    y = 200
    for nm, v in CEL_PARAMS:
        ins.append((nm, ft_mats.sparam(m, nm, v, x=-1100, y=y), ''))
        y += 70
    for nm, v in CEL_VECTORS:
        ins.append((nm, ft_mats.vparam(m, nm, v, x=-1100, y=y), ''))
        y += 70
    cu = ft_mats.custom(m, CEL_HLSL, ins, desc='CelInk')
    MEL.connect_material_property(cu, '', unreal.MaterialProperty.MP_EMISSIVE_COLOR)
    return ft_mats.finish(m)


# --------------------------------------------------------------------------------------------- cel fire
# The 1.4 temperature bands of M_Fire (full-throttle-art ft_fire.py), hottest first.
BANDS = [(0.95, '#fffbe8'), (0.78, '#ffe75a'), (0.60, '#ffae24'), (0.44, '#ff6a1f'), (0.30, '#e2321f'), (0.17, '#8e1630'), (-9, '#3a2034')]

FIRE_NOISE = r"""
float2 q = (UV - 0.5) * 2.0;
float r = length(q);
float s = Rand * 37.0;
float2 p = q * Freq + float2(s, s * 1.7) - float2(0.0, T * Flow);
float tot = 0.0;
float amp = 0.5;
float2 K = float2(127.1, 311.7);
for (int o = 0; o < 4; o++)
{
    float2 i = floor(p);
    float2 f = frac(p);
    f = f * f * (3.0 - 2.0 * f);
    float a = frac(sin(dot(i, K)) * 43758.5453);
    float b = frac(sin(dot(i + float2(1, 0), K)) * 43758.5453);
    float c = frac(sin(dot(i + float2(0, 1), K)) * 43758.5453);
    float d = frac(sin(dot(i + float2(1, 1), K)) * 43758.5453);
    tot += amp * lerp(lerp(a, b, f.x), lerp(c, d, f.x), f.y);
    p = p * 2.03 + float2(1.7, 9.2);
    amp *= 0.5;
}
tot = tot / 0.9375;
float lum = dot(PC.rgb, float3(0.3, 0.59, 0.11));
float heat = saturate(log2(1.0 + lum) / 3.1) * HeatK + (1.0 - r) * CoreK - RadCool + (tot - 0.5) * NoiseK;
float erode = saturate(1.0 - PC.a);
float m = tot * 0.9 + 0.74 - r * 0.9 - erode * ErodeK;
"""


def _fire_bands():
    out = ['float3 c = float3(0, 0, 0);']
    for th, hx in reversed(BANDS):
        r, g, b = hex_lin(hx)
        out.append('if (heat >= %.3f) c = float3(%.5f, %.5f, %.5f);' % (th, r, g, b))
    return '\n'.join(out)


FIRE_PARAMS = [('Freq', 2.2), ('Flow', 0.9), ('HeatK', 1.15), ('CoreK', 0.4), ('RadCool', 0.2), ('NoiseK', 0.55), ('ErodeK', 0.7), ('Gain', 3.0),
               ('Rim', 0.0)]  # 2.4: Rim > 0 draws an ink line (the 1.0 ink #1b1033) round the flame's edge, like a cartoon cel

# 2.4: the burning KESTREL. 2.2 drew it with the explosion's fire (Gain 3, white-hot core, 1.2 m puffs) and the
# chase camera saw a sun sitting on the car. This instance is flat cel fire: no bloom (every band below the
# bloom threshold), mostly orange and red with a small yellow core, inked round the edge.
CAR_FIRE = dict(Gain=0.85, Rim=0.07, HeatK=1.25, CoreK=0.55, NoiseK=0.45, Freq=1.8)


def fire():
    """M_Fire: the 1.4 cel temperature fire as a Niagara sprite material (unlit, masked). Heat comes from the
    particle's colour (the emitters' hot-to-cold colour curves), the middle of the puff and value noise; it is
    cut into the 1.4 bands, and the same noise eats the puff away as its alpha dies."""
    m = ft_mats.new_material(FX, 'M_Fire', blend=unreal.BlendMode.BLEND_MASKED, shading=unreal.MaterialShadingModel.MSM_UNLIT, two_sided=True)
    m.set_editor_property('used_with_niagara_sprites', True)
    m.set_editor_property('used_with_particle_sprites', True)
    m.set_editor_property('opacity_mask_clip_value', 0.5)
    uv = ft_mats.expr(m, unreal.MaterialExpressionTextureCoordinate)
    pc = ft_mats.expr(m, unreal.MaterialExpressionParticleColor, y=100)
    t = ft_mats.expr(m, unreal.MaterialExpressionTime, y=200)
    rnd = ft_mats.expr(m, unreal.MaterialExpressionParticleRandom, y=300)
    ins = [('UV', uv, ''), ('PC', pc, 'RGBA'), ('T', t, ''), ('Rand', rnd, '')]
    y = 400
    for nm, v in FIRE_PARAMS:
        ins.append((nm, ft_mats.sparam(m, nm, v, y=y), ''))
        y += 70
    ink = '\nif (Rim > 0.0 && m < 0.5 + Rim) return float3(%.5f, %.5f, %.5f);' % hex_lin('1b1033')
    col = ft_mats.custom(m, FIRE_NOISE + _fire_bands() + ink + '\nreturn c * Gain;', ins, y=-100, desc='FireCel')
    msk = ft_mats.custom(m, FIRE_NOISE + '\nreturn m;', ins, out_type=unreal.CustomMaterialOutputType.CMOT_FLOAT1, y=200, desc='FireMask')
    MEL.connect_material_property(col, '', unreal.MaterialProperty.MP_EMISSIVE_COLOR)
    MEL.connect_material_property(msk, '', unreal.MaterialProperty.MP_OPACITY_MASK)
    return ft_mats.finish(m)


def fire_to_niagara(systems=('NS_Explosion', 'NS_Fire')):
    """Point the explosion and burning-wreck emitters at M_Fire (through the project's reflection tool)."""
    T = unreal.FTEditorTools
    done = []
    for s in systems:
        sys = AL.load_asset(FX + '/' + s)
        r = T.set_path(sys, 'EmitterHandles[0].StatelessEmitter.RendererProperties[0].Material', "/Script/Engine.Material'/Game/FX/M_Fire.M_Fire'")
        T.compile_niagara(sys)
        AL.save_loaded_asset(sys)
        done.append('%s:%s' % (s, r))
    return done


def car_fire():
    """NS_CarFire + MI_Fire_Car: the burning KESTREL (2.4). Low flames licking off the engine deck, sized to the car,
    not a 2 m column of white-hot puffs between the chase camera and the car."""
    import ft_fx
    mi = _mi(FX + '/M_Fire', FX, 'MI_Fire_Car', None, CAR_FIRE)
    s = ft_fx.make('NS_CarFire', ft_fx.LOOP, {
        'emitter': {'SpawnInfos': '((Type=Rate,Rate=(Min=30,Max=30)))', 'FixedBounds': ft_fx.BIG},
        'modules': {
            'InitializeParticle': {'LifetimeDistribution': ft_fx.rng(0.35, 0.7), 'SpriteSizeDistribution': ft_fx.v2rng((29, 39), (64, 77)), 'SpriteRotationDistribution': ft_fx.rng(0, 360)},
            'ShapeLocation': {'ShapePrimitive': 'Box', 'BoxSize': ft_fx.vconst(70, 90, 10)},
            'AddVelocity': {'VelocityType': 'InCone', 'ConeAngle': '18.0', 'ConeVelocityDistribution': ft_fx.rng(160, 340)},
            'Drag': {'DragDistribution': ft_fx.const(1.0)},
            'GravityForce': {'GravityDistribution': ft_fx.vconst(0, 0, 260)},
            'SolveVelocitiesAndForces': {},
            # hot enough at birth for the yellow and orange bands (the first pass read as dark red blobs)
            'ScaleColor': {'ScaleDistribution': ft_fx.colors((4.0, 2.0, 0.5, 1), (2.6, 0.9, 0.2, 1), (1.0, 0.2, 0.05, 0.6), (0, 0, 0, 0))},
            'ScaleSpriteSize': {'ScaleDistribution': ft_fx.sizes(0.64, 1.04, 0.36)},
            'SpriteRotationRate': {'RotationRateDistribution': ft_fx.rng(-90, 90)},
            'ApplyOwnerScaleToAttributes': {},
        },
        'renderer': {'Material': "/Script/Engine.MaterialInstanceConstant'/Game/FX/MI_Fire_Car.MI_Fire_Car'"},
    })
    return '%s, %s' % (mi.get_name(), s.get_name())


def car_smoke():
    """NS_Smoke (the damaged KESTREL's smoke, 2.4): 2.0's puffs grew to 3.7 m and stood as a dark wall between the chase
    camera and the road ahead whenever the car burned. Now smaller, thinner and shorter-lived."""
    import ft_fx
    # edited in place: NS_Smoke is referenced, so ft_fx.make's delete-and-duplicate can't replace it
    s = AL.load_asset(FX + '/NS_Smoke')
    cfg = {
        'emitter': {'SpawnInfos': '((Type=Rate,Rate=(Min=12,Max=12)))'},
        'modules': {
            'InitializeParticle': {'LifetimeDistribution': ft_fx.rng(0.8, 1.4), 'SpriteSizeDistribution': ft_fx.v2rng((45, 40), (86, 70)), 'SpriteRotationDistribution': ft_fx.rng(0, 360)},
            'ShapeLocation': {'ShapePrimitive': 'Sphere'},
            'AddVelocity': {'VelocityType': 'InCone', 'ConeAngle': '25.0', 'ConeVelocityDistribution': ft_fx.rng(150, 300)},
            'Drag': {'DragDistribution': ft_fx.const(0.8)},
            'SolveVelocitiesAndForces': {},
            'ScaleColor': {'ScaleDistribution': ft_fx.colors((0.08, 0.07, 0.08, 0), (0.06, 0.055, 0.06, 0.32), (0.04, 0.04, 0.045, 0))},
            'ScaleSpriteSize': {'ScaleDistribution': ft_fx.sizes(0.6, 1.3, 1.9)},
            'SpriteRotationRate': {'RotationRateDistribution': ft_fx.rng(-30, 30)},
        },
    }
    mods = ft_fx._mods(s)
    for mname, props in cfg['modules'].items():
        for k, v in props.items():
            ft_fx._set(s, 'Modules[%d].%s' % (mods[mname], k), v)
    for k, v in cfg['emitter'].items():
        ft_fx._set(s, k, v)
    ft_fx.T.compile_niagara(s)
    AL.save_loaded_asset(s)
    return s.get_name()


def build_materials():
    ft_mats.paint()
    ft_mats.road()
    ft_mats.sky()
    pp_cel()
    fire()
    return 'cel materials built; ' + ', '.join(fire_to_niagara())


# --------------------------------------------------------------------------------------------- levels
def _mi(parent, path, name, vectors=None, scalars=None):
    return ft_mats.mi(AL.load_asset(parent), path, name, vectors, scalars)


def disc_dir(azimuth_deg, elevation_deg):
    """Direction to a painted sun or moon: azimuth from +X toward +Y (degrees), elevation above the horizon."""
    a, e = math.radians(azimuth_deg), math.radians(elevation_deg)
    return (math.cos(e) * math.cos(a), math.cos(e) * math.sin(a), math.sin(e))


def disc_size(radius_deg):
    return math.cos(math.radians(radius_deg))


# 2.4: the 1.x banded disc (render.js): 7 degrees across like the canvas sun (viewH * 0.11 at the chase FOV), with
# its halo ring, glow and slow rays. The light itself still comes from behind and high, so the road and the car
# face it for the chase camera (the 1.0 M_Toon cheat); the painted disc sits ahead, where the player looks.
def _x(c, k):
    return tuple(v * k for v in c)


# the disc a little over 1.0 so the tonemapper doesn't grey it (still under the day bloom threshold of 1.5)
SUN_1X = dict(DiscCol=_x(hex_lin('fff3a8'), 1.3), DiscCol2=_x(hex_lin('ffc94d'), 1.3), HaloCol=hex_lin('fff4be'), Halo=0.32, Glow=0.45, Rays=0.07,
              Stripes=1.0, Stars=0.0, DiscSize=disc_size(7.0), Gain=1.0)
MOON_1X = dict(DiscCol=hex_lin('e8fbff'), DiscCol2=hex_lin('9fe8ff'), HaloCol=hex_lin('a0f0ff'), Halo=0.25, Glow=0.3, Rays=0.0,
               Stripes=1.0, DiscSize=disc_size(6.0), Gain=1.0)

LEVELS = [
    # Bayshore: the 1.0 pastel beach town by day. Sun from behind-left and high, like 1.0's M_Toon LightDir;
    # the painted sun (the 1.x banded 80s disc) ahead-left over the bay. The track runs +X for 1 km, then mostly
    # +Y (12 km): 2.2 put the disc at -Y, behind the player for nearly the whole race.
    dict(name='Bayshore', map='/Game/Maps/Bayshore/L_Bayshore', day=True, light_label='Sun',
         sky=dict(SUN_1X, Sky0=hex_lin('8ccdf2'), Sky1=hex_lin('c9d6f5'), Sky2=hex_lin('ffc2d6'), MoonDir=disc_dir(100.0, 15.0)),
         sun=dict(pitch=-50.0, yaw=95.0, lux=9.0, col=hex_lin('fff4e4')), sky_light=1.2,
         fog=dict(col=hex_lin('f0d8f0'), density=0.018, start=4000.0, volumetric=False, opacity=0.6),
         exposure=0.0,
         road=dict(RoadPastel=hex_lin('5f5b78'), RoadLift=1.0, Grain=0.035, Wet=0.7, Puddles=0.0),
         wall=dict(Color=hex_lin('bfe9f2'), Rough=0.75, Metal=0.0),
         # 3.1: the beach runs 300 m down to a real sea (AFTTrack's floor, MI_BaySea) and the town's ground reaches
         # out under the far skyline (it stopped at 300 m, with the towers beyond it standing over nothing)
         ground=[(-30000, -1350, 'd6bd8c'), (-22000, -820, 'e6cc98'), (-11000, -220, 'f2dcae'), (-1600, -70, 'f6e3b6'),
                 (1600, -70, 'f5b6d2'), (6400, -60, 'f8c8de'), (7000, -60, 'd9c6f2'), (64000, -250, 'bba9e0')],
         water=dict(Color=hex_lin('4fcfd0'), Rough=0.3),
         cel=dict(Key=3.0, Bright=0.8, HueKeep=0.35)),
    # Chemical Works: the 1.1 plant by moonlight (2.4): the 1.1 banded pale-cyan moon, big and low ahead, and a
    # cool moonlight strong enough to cut cel bands on the plant and the car, under the neon. 2.2's moon was a
    # 1.7-degree dot 42 degrees up, out of the chase camera's view, with a 3 lux light.
    dict(name='ChemicalWorks', map='/Game/Maps/ChemicalWorks/L_ChemicalWorks', day=False, light_label='Moonlight',
         sky=dict(MOON_1X, Sky0=(0.004, 0.006, 0.026), Sky1=(0.03, 0.04, 0.13), Sky2=(0.3, 0.14, 0.5), Stars=0.8, MoonDir=disc_dir(130.0, 18.0)),
         sun=dict(pitch=-38.0, yaw=125.0, lux=5.5, col=hex_lin('b8d2ff')), sky_light=2.0,
         fog=dict(col=(0.03, 0.015, 0.07), density=0.012, start=4000.0, volumetric=False, opacity=0.6),
         # 2.6: the night cel of ft_v25.NIGHT_CEL (level one's look by moonlight)
         road=dict(Grain=0.015, Wet=0.55, Puddles=0.0), cel=dict(Key=1.1, Bright=1.3, Hot=2.6, EmisCut=6.0, HueKeep=0.6, ShadowLevel=0.5)),
    # Nevada (2.4): the desert by day, was 1.5's dusk. Deep blue overhead to a warm sand haze at the horizon, the
    # banded sun ahead-left of the highway (the track runs +X to Vegas), hard noon-ish light from behind.
    dict(name='Nevada', map='/Game/Maps/Nevada/L_Nevada', day=True, light_label='Sun',
         sky=dict(SUN_1X, Sky0=hex_lin('3f8fdf'), Sky1=hex_lin('a3d6f0'), Sky2=hex_lin('ffd9a6'), DiscCol2=_x(hex_lin('ffa63d'), 1.3),
                  MoonDir=disc_dir(-25.0, 17.0)),
         sun=dict(pitch=-55.0, yaw=20.0, lux=10.0, col=hex_lin('fff0da')), sky_light=1.2,
         fog=dict(col=hex_lin('f4d8b4'), density=0.012, start=6000.0, volumetric=False, opacity=0.55),
         exposure=0.0,
         road=dict(RoadPastel=hex_lin('6b6170'), RoadLift=0.9, Grain=0.03, Wet=0.12, Puddles=0.0),
         cel=dict(Key=3.0, Th2=0.36, Bright=0.8, Hot=1.9, HueKeep=0.4)),
]


def _actor(label):
    for a in EAS.get_all_level_actors():
        if a.get_actor_label() == label:
            return a
    return None


def cel_mi(i, **over):
    L = LEVELS[i]
    path = '/Game/Maps/%s/Materials' % L['name']
    sc = dict(L.get('cel', {}))
    sc.update(over)
    vec = {}
    for k in [k for k in sc if isinstance(sc[k], tuple)]:
        v = sc.pop(k)
        vec[k] = v if len(v) == 4 else v + (1,)
    return _mi(CORE + '/M_PP_Cel', path, 'MI_PP_Cel_' + L['name'], vec, sc)


def apply_level(i, save=True):
    L = LEVELS[i]
    world = unreal.get_editor_subsystem(unreal.UnrealEditorSubsystem).get_editor_world()
    if not world.get_path_name().startswith(L['map']):
        LES.load_level(L['map'])
    path = '/Game/Maps/%s/Materials' % L['name']
    log = []

    # post-process: the cel/ink blendable, and a cleaner grade (1.0 had crisp flat colour, little grain)
    ppv = _actor('Grade80s')
    s = ppv.get_editor_property('settings')
    mi = cel_mi(i)
    wb = unreal.WeightedBlendables()
    wb.set_editor_property('array', [unreal.WeightedBlendable(1.0, mi)])
    s.set_editor_property('weighted_blendables', wb)
    def ov(k, v):
        s.set_editor_property('override_' + k, True)
        s.set_editor_property(k, v)
    ov('film_grain_intensity', 0.04)
    ov('scene_fringe_intensity', 0.15)
    ov('vignette_intensity', 0.3)
    # 2.6: Bayshore's grade on every level. Until 2.5 only the day levels got it; the night levels kept the 2.0
    # grade (bloom 1.15 from 1.0, lens flare 0.6, contrast, a colour tint), which glowed the cel bands and the
    # ink away. The cel pass itself sets each level's light and mood, not the grade.
    if 'exposure' in L:
        ov('auto_exposure_bias', L['exposure'])
    ov('bloom_intensity', 0.5)
    ov('bloom_threshold', 1.5)
    ov('color_saturation', unreal.Vector4(1.12, 1.12, 1.12, 1.0))
    ov('color_contrast', unreal.Vector4(1.0, 1.0, 1.0, 1.0))
    ov('scene_color_tint', unreal.LinearColor(1, 1, 1, 1))
    ov('lens_flare_intensity', 0.15)
    ppv.set_editor_property('settings', s)
    log.append('pp ' + mi.get_name())

    # road and walls: per-level instances on the track (the C++ MID still sets RoadA/Line/Kerbs per level)
    track = [a for a in EAS.get_all_level_actors() if a.get_class().get_name() == 'FTTrack'][0]
    if 'road' in L:
        r = L['road']
        vec = {k: v + (1,) for k, v in r.items() if isinstance(v, tuple)}
        sc = {k: v for k, v in r.items() if not isinstance(v, tuple)}
        track.set_editor_property('road_material', _mi(CORE + '/MI_Road', path, 'MI_Road_' + L['name'], vec, sc))
    if 'wall' in L:
        w = L['wall']
        track.set_editor_property('wall_material', _mi(CORE + '/M_Wall', path, 'MI_Wall_' + L['name'], {'Color': w['Color'] + (1,)}, {'Rough': w['Rough'], 'Metal': w['Metal']}))
    if 'ground' in L:
        # colours first: each set re-runs the track's construction, and a profile longer than its colour
        # list indexes past the end (that crashed the editor once)
        track.set_editor_property('ground_colours', [unreal.LinearColor(*(hex_lin(h) + (1,))) for _, _, h in L['ground']])
        track.set_editor_property('ground_profile', [unreal.Vector2D(x, z) for x, z, _ in L['ground']])
    if 'water' in L:
        wm = AL.load_asset(CORE + '/MI_Water')
        MEL.set_material_instance_vector_parameter_value(wm, 'Color', unreal.LinearColor(*(L['water']['Color'] + (1,))))
        MEL.set_material_instance_scalar_parameter_value(wm, 'Rough', L['water']['Rough'])
        MEL.update_material_instance(wm)
        AL.save_loaded_asset(wm)

    # sky (sun or moon disc), the sun or moon light, sky light (every level since 2.4), and the day fog
    if 'sky' in L:
        skymi = AL.load_asset('%s/MI_Sky_%s' % (path, L['name']))
        for k, v in L['sky'].items():
            if isinstance(v, tuple):
                vv = v + (0,) if k == 'MoonDir' else v + (1,)
                MEL.set_material_instance_vector_parameter_value(skymi, k, unreal.LinearColor(*vv))
            else:
                MEL.set_material_instance_scalar_parameter_value(skymi, k, v)
        MEL.update_material_instance(skymi)
        AL.save_loaded_asset(skymi)
        log.append('sky')
    # 2.4: the sky sphere was 2 km across the origin while the tracks run 12 to 23 km away, so the car drove out
    # of the sky mid-race. Now 60 km, and the height fog stops at 50 km (its FogCutoffDistance, made for sky
    # domes) so the painted sky keeps its colours instead of sinking into the haze; the ground still hazes.
    skya = _actor('NightSky')
    if skya:
        skya.set_actor_scale3d(unreal.Vector(120000, 120000, 120000))
        fog_c = _actor('HeightFog').get_component_by_class(unreal.ExponentialHeightFogComponent)
        fog_c.set_editor_property('fog_cutoff_distance', 5000000.0)
    if 'sun' in L:
        sun = _actor('Moonlight') or _actor('Sun')
        sun.set_actor_label(L.get('light_label', 'Sun'))
        su = L['sun']
        sun.set_actor_rotation(unreal.Rotator(roll=0, pitch=su['pitch'], yaw=su['yaw']), False)
        lc = sun.get_component_by_class(unreal.DirectionalLightComponent)
        lc.set_intensity(su['lux'])
        lc.set_light_color(unreal.LinearColor(*(su['col'] + (1,))))
        sl = _actor('SkyLight').get_component_by_class(unreal.SkyLightComponent)
        sl.set_intensity(L['sky_light'])
        log.append('%s %.0f lux' % (sun.get_actor_label(), su['lux']))
    if 'fog' in L:
        fc = _actor('HeightFog').get_component_by_class(unreal.ExponentialHeightFogComponent)
        fo = L['fog']
        fc.set_fog_inscattering_color(unreal.LinearColor(*(fo['col'] + (1,))))
        fc.set_fog_density(fo['density'])
        fc.set_editor_property('start_distance', fo['start'])
        fc.set_editor_property('fog_max_opacity', fo['opacity'])
        fc.set_volumetric_fog(fo['volumetric'])

    if save:
        LES.save_current_level()
    return '%s: %s' % (L['name'], ', '.join(log))


def shot(name, w=1280, h=720):
    """A real in-game screenshot of the running PIE viewport (chase camera and HUD) into Saved/Shots.
    The game's own ft.Shots hook films with a SceneCapture, which skips post-process materials, so it
    can't show the cel pass; HighResShot renders the actual game view."""
    world = unreal.get_editor_subsystem(unreal.UnrealEditorSubsystem).get_game_world()
    if world is None:
        return 'not in PIE'
    unreal.SystemLibrary.execute_console_command(world, 't.IdleWhenNotForeground 0')
    fn = unreal.Paths.convert_relative_path_to_full(unreal.Paths.project_saved_dir() + 'Shots/' + name + '.png')
    unreal.SystemLibrary.execute_console_command(world, 'HighResShot %dx%d filename=%s' % (w, h, fn))
    return fn


def tune(i, **over):
    """Change the level's cel instance live, also during PIE (e.g. tune(0, Key=2.5, Debug=1)).
    During PIE it only sets the parameters (no create, no save: saving in PIE once left a hidden
    'Overwrite Existing Object' modal that froze the editor); call cel_mi(i) after PIE to save."""
    LEVELS[i].setdefault('cel', {}).update(over)
    if not LES.is_in_play_in_editor():
        return cel_mi(i).get_name()
    # EditorAssetLibrary refuses to run in PIE (does_asset_exist answers False, which is how the Overwrite
    # modal happened), so load with the plain loader
    mi = unreal.load_asset('/Game/Maps/%s/Materials/MI_PP_Cel_%s' % (LEVELS[i]['name'], LEVELS[i]['name']))
    for k, v in over.items():
        if isinstance(v, tuple):
            MEL.set_material_instance_vector_parameter_value(mi, k, unreal.LinearColor(*(v if len(v) == 4 else v + (1,))))
        else:
            MEL.set_material_instance_scalar_parameter_value(mi, k, v)
    MEL.update_material_instance(mi)
    return 'live ' + mi.get_name()
