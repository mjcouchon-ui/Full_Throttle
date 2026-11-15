"""FULL THROTTLE (Unreal) 3.1: how the attacks look. Materials, the pie, and the one call that builds everything.

The effects sheet (Concept Art Sheets / 00D Shared Effects) draws every effect as flat, banded, faceted shapes: an
octagon in cream, yellow, orange, red and maroon rings with thin rays behind it; smoke as two-tone lavender blobs;
sparks as thin spears. M_FX_Cel draws exactly those on a sprite, with nothing but the particle's colour and age:

    particle colour  the middle band's colour; the core, the warm band and the dark rim are worked out from it
    particle alpha   heat, 1 to 0 over the particle's life: a burst hollows out from the middle as it cools, a ring
                     thins, a puff erodes, and all of them are gone at 0 (no soft fades: cel shapes are there or not)

    Shape 0 burst    a banded polygon (Sides) with rays behind it (Rays, RayLen, RayWidth)
    Shape 1 ring     a band at the sprite's edge, Body wide
    Shape 2 puff     a lobed blob in two flat tones, lit from above
    Shape 3 shard    a diamond along the sprite's long axis (use with velocity-aligned sprites)

It is translucent and unlit, so it draws after the shared cel pass (which is not touched): the colours written here
are the colours on screen (Gain; the core is brighter by CoreGain). Gain stays near 1: the first captures at 1.5 to
1.9 came out as pale, blown-out shapes with no bands left.
The instances MI_FX_CelBurst, CelFlash, CelBlob, CelRing, CelPuff and CelShard are what the systems in ft_fx.py use.

Run in the editor (never during PIE):  import ft_attackfx; ft_attackfx.build_all()
That builds these materials, the hazard marker (ft_mats.hazard), the pie, every system in ft_fx.py and the SFX DEPT's
recoloured copies (ft_v26.make_tints), in that order. Tuning and the attack-by-attack table: DevLog/jam/attacks.md.
"""
import math
import unreal
import ft_mats

AL = unreal.EditorAssetLibrary
MEL = unreal.MaterialEditingLibrary
FX = '/Game/FX'


def log(*a):
    unreal.log('FTFX ' + ' '.join(str(x) for x in a))


CEL_HLSL = r"""
float2 q = (UV - 0.5) * 2.0;
float r = length(q);
float heat = saturate(PC.a);
float3 mid = PC.rgb;
float a = atan2(q.y, q.x);
float rnd = Rand;
float aa = max(fwidth(r) * 1.2, 0.006);
// the bands, from the one colour the particle carries: lighter towards the middle by lifting each channel (so orange
// goes to yellow and blue to pale blue, each in its own hue), darker and more saturated towards the rim
float3 warm = pow(max(mid, 0.0001), 0.5);
float3 hot = lerp(warm, float3(1.0, 0.95, 0.80), 0.75) * CoreGain;
float3 rimc = pow(max(mid, 0.0001), 1.6) * 0.75;
float3 edgec = rimc * 0.55;
float3 col = mid;
float op = 0.0;
// the shard's own distance and its screen-space width, taken out here: no derivatives inside the branches
float2 sq = abs(q);
float sd = sq.x / max(1.0 - sq.y, 0.001);
float sda = max(fwidth(sd), 0.02);
if (Shape < 0.5)
{
    // burst: a banded polygon with rays behind it; it hollows out from the middle as it cools
    float seg = 6.2831853 / max(Sides, 3.0);
    float ap = fmod(a + rnd * 6.2831853 + 62.831853, seg) - seg * 0.5;
    float d = r * cos(ap) / cos(seg * 0.5) / max(Body, 0.05);
    float da = aa / max(Body, 0.05);
    float hole = saturate(1.0 - heat * 1.7) * 0.92;
    float body = (1.0 - smoothstep(1.0 - da, 1.0 + da, d)) * smoothstep(hole - da, hole + da, d);
    col = d < 0.34 ? hot : (d < 0.56 ? warm : (d < 0.76 ? mid : (d < 0.92 ? rimc : edgec)));
    op = body;
    if (Rays > 0.5)
    {
        float rs = 6.2831853 / Rays;
        float t2 = a + rnd * 3.7 + 62.831853;
        float idx = floor(t2 / rs);
        float ang = fmod(t2, rs) - rs * 0.5;
        float h = frac(sin(idx * 12.9898 + rnd * 78.233) * 43758.5453);
        float len = (Body + RayLen * (0.45 + 0.55 * h)) * (0.35 + 0.65 * heat);
        float hw = RayWidth * saturate(1.0 - r / max(len, 0.001));
        // behind the body: only what sticks out past its edge shows (never inside the hollow)
        float ray = (1.0 - smoothstep(hw - aa, hw + aa, abs(sin(ang)) * r)) * step(r, len) * smoothstep(1.0 - da, 1.0 + da, d);
        float3 rc = h > 0.5 ? warm : mid;
        col = lerp(rc, col, body);
        op = max(op, ray);
    }
}
else if (Shape < 1.5)
{
    // ring: a band at the sprite's edge that thins as it cools
    float w = Body * (0.35 + 0.65 * heat);
    float outer = 0.96;
    float inner = outer - w;
    op = (1.0 - smoothstep(outer - aa, outer + aa, r)) * smoothstep(inner - aa, inner + aa, r);
    col = r > outer - w * 0.45 ? rimc : warm;
}
else if (Shape < 2.5)
{
    // puff: a lobed blob in two flat tones, lit from above; it erodes, it does not fade
    float lobes = 0.80 + 0.085 * sin(a * 5.0 + rnd * 40.0) + 0.06 * sin(a * 3.0 - rnd * 17.0 + T * 0.35) + 0.04 * sin(a * 9.0 + rnd * 9.0);
    float R = lobes * (0.42 + 0.58 * smoothstep(0.0, 0.5, heat));
    op = 1.0 - smoothstep(R - aa, R + aa, r);
    float lit = step(0.0, -q.y * 0.85 - q.x * 0.35 + 0.16 + 0.10 * sin(a * 4.0 + rnd * 23.0));
    col = lerp(mid * 0.70, mid * 1.22, lit);
}
else
{
    // shard: a diamond along the sprite's long axis, three bands
    op = (1.0 - smoothstep(1.0 - sda, 1.0 + sda, sd)) * step(sq.y, 0.999);
    col = sd < 0.30 ? hot : (sd < 0.62 ? warm : (sd < 0.86 ? mid : rimc));
}
op *= smoothstep(0.0, 0.06, heat);
return float4(col * Gain, op * Opacity);
"""

CEL_PARAMS = [('Shape', 0.0), ('Sides', 8.0), ('Body', 0.62), ('Rays', 9.0), ('RayLen', 0.36), ('RayWidth', 0.07),
              ('Gain', 1.05), ('CoreGain', 1.3), ('Opacity', 1.0)]
# the instances the Niagara systems use: name -> scalar overrides
CEL_INSTANCES = {
    'Burst': {'Shape': 0.0, 'Sides': 8.0, 'Body': 0.62, 'Rays': 9.0, 'RayLen': 0.36, 'RayWidth': 0.07},
    'Flash': {'Shape': 0.0, 'Sides': 6.0, 'Body': 0.36, 'Rays': 6.0, 'RayLen': 0.62, 'RayWidth': 0.10, 'Gain': 1.25, 'CoreGain': 1.35},
    'Blob': {'Shape': 0.0, 'Sides': 7.0, 'Body': 0.92, 'Rays': 0.0},
    'Ring': {'Shape': 1.0, 'Body': 0.22, 'Gain': 1.2},
    'Puff': {'Shape': 2.0, 'Gain': 1.0, 'CoreGain': 1.0, 'Opacity': 0.94},
    'Shard': {'Shape': 3.0, 'Gain': 1.3, 'CoreGain': 1.3},
}


def _sprite_flags(m):
    # the cooked game falls back to the default material without the usage flag (README: material usage flags)
    for flag in ('used_with_niagara_sprites', 'used_with_particle_sprites'):
        try:
            m.set_editor_property(flag, True)
        except Exception as e:
            unreal.log_warning('FTFX %s: could not set %s (%s)' % (m.get_name(), flag, e))


def _compile(m):
    MEL.layout_material_expressions(m)
    errs = list(MEL.recompile_material(m) or [])
    if errs:
        raise RuntimeError('%s did not compile: %s' % (m.get_name(), errs))
    AL.save_loaded_asset(m)
    return m


def cel_materials():
    """M_FX_Cel and its instances."""
    m = ft_mats.new_material(FX, 'M_FX_Cel', blend=unreal.BlendMode.BLEND_TRANSLUCENT, shading=unreal.MaterialShadingModel.MSM_UNLIT, two_sided=True)
    _sprite_flags(m)
    X = -1100
    uv = ft_mats.expr(m, unreal.MaterialExpressionTextureCoordinate, x=X, y=-500)
    pc = ft_mats.expr(m, unreal.MaterialExpressionParticleColor, x=X, y=-420)
    t = ft_mats.expr(m, unreal.MaterialExpressionTime, x=X, y=-340)
    rnd = ft_mats.expr(m, unreal.MaterialExpressionParticleRandom, x=X, y=-260)
    ins = [('UV', uv, ''), ('PC', pc, 'RGBA'), ('T', t, ''), ('Rand', rnd, '')]
    y = -160
    for nm, v in CEL_PARAMS:
        ins.append((nm, ft_mats.sparam(m, nm, v, x=X, y=y), ''))
        y += 70
    cu = ft_mats.custom(m, CEL_HLSL, ins, out_type=unreal.CustomMaterialOutputType.CMOT_FLOAT4, x=-500, y=0, desc='CelFX')
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
    _compile(m)
    for name, scalars in CEL_INSTANCES.items():
        ft_mats.mi(m, FX, 'MI_FX_Cel' + name, scalars=scalars)
    log('cel materials built:', ', '.join('MI_FX_Cel' + n for n in CEL_INSTANCES))
    return m


def stock_flags():
    """The stock sprite materials keep their Niagara flag only because their assets are reused; say it out loud."""
    for name in ('M_FX_Fire', 'M_FX_Smoke', 'M_FX_Spark', 'M_FX_Glow'):
        m = AL.load_asset(FX + '/' + name)
        if m is not None:
            _sprite_flags(m)
            AL.save_loaded_asset(m)


# ------------------------------------------------------------------------------------------ the pie
def pie(m):
    """MR. CHUCKLES' cream pie, top towards +X (it flies face first): a tin, a cream dome, a cherry."""
    # rot is (pitch, yaw, roll): pitch -90 lays a cylinder's axis along +X
    m.cyl('#8f9bb3', (-24, 0, 0), 50, 10, rot=(-90, 0, 0), steps=12)           # the tin's base
    m.cyl('#c9c4d6', (-14, 0, 0), 62, 8, rot=(-90, 0, 0), steps=12)            # its rim
    m.sphere('#fff6e8', (0, 0, 0), 54, scale=(0.55, 1.0, 1.0), steps=(6, 12))  # cream
    for k in range(6):                                                         # piped rosettes round the edge
        a = k * math.pi / 3.0
        m.sphere('#ffe9f2', (10, math.cos(a) * 36, math.sin(a) * 36), 15, steps=(4, 6))
    m.sphere('#ff3355', (32, 0, 6), 13, steps=(5, 8), glow=1.2)                # the cherry


def pie_mesh():
    import ft_export
    sm = ft_export.save_mesh(ft_export.model(pie).meshes, '/Game/FX/SM_Pie', 'prop', nanite=False)
    log('pie built', sm.get_name() if sm else None)
    return sm


def build_all(meshes=True):
    import importlib
    import ft_fx
    import ft_v26
    importlib.reload(ft_fx)
    importlib.reload(ft_v26)
    cel_materials()
    stock_flags()
    ft_mats.hazard()
    ft_mats.mi(AL.load_asset(FX + '/M_Hazard'), FX, 'MI_Hazard')
    if meshes:
        pie_mesh()
    log(ft_fx.build_all())
    log(ft_v26.make_tints())
    return 'attack effects built'
