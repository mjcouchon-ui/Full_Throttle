"""Build MPC_BayshoreSky, MF_SemiCelRamp, the sky master materials and their instances.

Run in the offscreen editor (shaders compile, errors are raised):  ue_run.ps1 -Editor -Script 30_materials.py
Re-runnable: graphs are rebuilt; MPC parameters keep their ids (read, append, write back).
"""
import sys
import os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)) if "__file__" in globals() else r"K:\Full Throttle\SkyScratch\SourceArt\BayshoreSky\unreal")
import unreal
from bsky_ue import log, enum_entry, ensure_dir, save, load_or_create, MEL

MAT = "/BayshoreSky/Materials"
INST = "/BayshoreSky/Materials/Instances"
FN = "/BayshoreSky/Materials/Functions"


from bsky_params import srgb, SCALARS, COLORS, VECTORS


def build_mpc():
    mpc = load_or_create(MAT, "MPC_BayshoreSky", unreal.MaterialParameterCollection, unreal.MaterialParameterCollectionFactoryNew())
    sp = list(mpc.get_editor_property("ScalarParameters"))
    have = {str(p.get_editor_property("ParameterName")) for p in sp}
    for name, val in SCALARS:
        if name not in have:
            p = unreal.CollectionScalarParameter()
            p.set_editor_property("ParameterName", name)
            p.set_editor_property("DefaultValue", val)
            sp.append(p)
    mpc.set_editor_property("ScalarParameters", sp)
    vp = list(mpc.get_editor_property("VectorParameters"))
    have = {str(p.get_editor_property("ParameterName")) for p in vp}
    for name, val in VECTORS:
        if name not in have:
            p = unreal.CollectionVectorParameter()
            p.set_editor_property("ParameterName", name)
            p.set_editor_property("DefaultValue", unreal.LinearColor(*val))
            vp.append(p)
    mpc.set_editor_property("VectorParameters", vp)
    save(mpc)
    log("MPC scalars", len(sp), "vectors", len(vp))
    return mpc


# ------------------------------------------------------------------------------------------------ graph builder
class Graph:
    def __init__(self, owner, mpc, is_function=False):
        self.o = owner
        self.fn = is_function
        self.mpc_ = mpc
        self.n = 0
        self.cache = {}

    def node(self, cls, **props):
        self.n += 1
        x, y = -2400 + (self.n % 6) * 260, -1200 + (self.n // 6) * 180
        if self.fn:
            e = MEL.create_material_expression_in_function(self.o, cls, x, y)
        else:
            e = MEL.create_material_expression(self.o, cls, x, y)
        for k, v in props.items():
            e.set_editor_property(k, v)
        return e

    def mpc(self, name):
        key = ("mpc", name)
        if key not in self.cache:
            e = self.node(unreal.MaterialExpressionCollectionParameter)
            e.set_editor_property("Collection", self.mpc_)
            e.set_editor_property("ParameterName", name)
            vec = name in dict(VECTORS)
            if vec:
                m = self.node(unreal.MaterialExpressionComponentMask)
                for c, on in (("R", True), ("G", True), ("B", True), ("A", False)):
                    m.set_editor_property(c, on)
                self.link((e, ""), m, "")
                self.cache[key] = (m, "")
            else:
                self.cache[key] = (e, "")
        return self.cache[key]

    def one(self, cls, key=None, **props):
        key = key or (cls.__name__, tuple(sorted(props.items())))
        if key not in self.cache:
            self.cache[key] = (self.node(cls, **props), "")
        return self.cache[key]

    def time(self):
        return self.one(unreal.MaterialExpressionTime)

    def campos(self):
        return self.one(unreal.MaterialExpressionCameraPositionWS)

    def camvec(self):
        return self.one(unreal.MaterialExpressionCameraVectorWS)

    def objpos(self):
        return self.one(unreal.MaterialExpressionObjectPositionWS)

    def worldpos(self, mode="WPT_Default"):
        return self.one(unreal.MaterialExpressionWorldPosition, key=("wp", mode),
                        WorldPositionShaderOffset=enum_entry(unreal.WorldPositionIncludedOffsets, mode))

    def texcoord(self, i=0):
        return self.one(unreal.MaterialExpressionTextureCoordinate, key=("uv", i), CoordinateIndex=i)

    def vcolor(self, c="R"):
        n, _ = self.one(unreal.MaterialExpressionVertexColor)
        return (n, c)

    def normal(self):
        return self.one(unreal.MaterialExpressionVertexNormalWS)

    def custom_data(self, i):
        return self.one(unreal.MaterialExpressionPerInstanceCustomData, key=("pic", i), DataIndex=i)

    def tex_object(self, param, texture):
        e = self.node(unreal.MaterialExpressionTextureObjectParameter)
        e.set_editor_property("ParameterName", param)
        e.set_editor_property("Texture", texture)
        return (e, "")

    def tex_sample(self, param, texture, uv_src):
        e = self.node(unreal.MaterialExpressionTextureSampleParameter2D)
        e.set_editor_property("ParameterName", param)
        e.set_editor_property("Texture", texture)
        self.link(uv_src, e, "UVs")
        return e

    def link(self, src, dst, dst_in):
        node, out = src
        if not MEL.connect_material_expressions(node, out, dst, dst_in):
            raise RuntimeError("link failed %s.%s -> %s.%s" % (node.get_name(), out, dst.get_name(), dst_in))

    def custom(self, desc, code, inputs, out_type="CMOT_Float3", extra=()):
        e = self.node(unreal.MaterialExpressionCustom)
        ins = []
        for name, _ in inputs:
            ci = unreal.CustomInput()
            ci.set_editor_property("InputName", name)
            ins.append(ci)
        e.set_editor_property("Inputs", ins)
        outs = []
        for name, t in extra:
            co = unreal.CustomOutput()
            co.set_editor_property("OutputName", name)
            co.set_editor_property("OutputType", enum_entry(unreal.CustomMaterialOutputType, t))
            outs.append(co)
        e.set_editor_property("AdditionalOutputs", outs)
        e.set_editor_property("OutputType", enum_entry(unreal.CustomMaterialOutputType, out_type))
        e.set_editor_property("Description", desc)
        e.set_editor_property("Code", code)
        for name, src in inputs:
            self.link(src, e, name)
        return e

    def ramp(self, src, ramp_fn):
        call = MEL.create_material_function_call(self.o, ramp_fn, -600, self.n * 40)
        self.link(src, call, "Value")
        return (call, "Result")

    def out(self, src, prop):
        node, o = src
        if not MEL.connect_material_property(node, o, enum_entry(unreal.MaterialProperty, prop)):
            raise RuntimeError("connect to %s failed" % prop)


def reset_material(name, blend, two_sided=False, extra=None):
    m = load_or_create(MAT, name, unreal.Material, unreal.MaterialFactoryNew())
    MEL.delete_all_material_expressions(m)
    m.set_editor_property("MaterialDomain", enum_entry(unreal.MaterialDomain, "MD_Surface"))
    m.set_editor_property("BlendMode", enum_entry(unreal.BlendMode, blend))
    m.set_editor_property("ShadingModel", enum_entry(unreal.MaterialShadingModel, "MSM_Unlit"))
    m.set_editor_property("TwoSided", two_sided)
    # reused assets keep old flags: reset the ones this script sets anywhere, then apply this material's
    defaults = {"bIsSky": False, "DitherOpacityMask": False, "NumCustomizedUVs": 0, "bUsedWithInstancedStaticMeshes": False,
                "MaxWorldPositionOffsetDisplacement": 0.0, "bAlwaysEvaluateWorldPositionOffset": False}
    defaults.update(extra or {})
    extra = defaults
    for k, v in (extra or {}).items():
        m.set_editor_property(k, v)
    return m


def compile_check(m):
    errs = list(MEL.recompile_material(m))
    if errs:
        nodes = MEL.get_material_error_expressions(m)
        for i, msg in enumerate(errs):
            owner = nodes[i].get_name() if i < len(nodes) and nodes[i] else "<material>"
            log("COMPILE ERROR", m.get_name(), msg, owner)
        raise RuntimeError("%s failed to compile" % m.get_name())
    st = MEL.get_statistics(m)
    log("compiled", m.get_name(), "VS", st.get_editor_property("NumVertexShaderInstructions"),
        "PS", st.get_editor_property("NumPixelShaderInstructions"), "samplers", st.get_editor_property("NumSamplers"))
    MEL.layout_material_expressions(m)
    save(m)


# ------------------------------------------------------------------------------------------------ HLSL snippets
GRAD = r"""
float3 d = normalize(D);
float el = asin(clamp(d.z, -1.0, 1.0));
float e = saturate(el / 1.5707963);
// horizon colour at 0, the mid colour at MidPt, the zenith blue by 3x MidPt and above. The reference (R7, measured)
// is already deep blue 20 deg up, and the chase camera sees about 0-25 deg, so the blue has to arrive low
float m = clamp(MidPt, 0.02, 0.6);
float3 c = (e < m) ? lerp(Hor, Mid, smoothstep(0.0, 1.0, e / m)) : lerp(Mid, Zen, smoothstep(0.0, 1.0, saturate((e - m) / (2.0 * m))));
float2 dh = normalize(d.xy + float2(1e-5, 0.0));
float2 sh = normalize(SunD.xy + float2(1e-5, 0.0));
float toward = saturate(dot(dh, sh) * 0.5 + 0.5);
// the warm haze toward the sun stays low (R7: lavender-blue again 12 deg up on the sun side)
float lowband = 1.0 - smoothstep(0.0, 0.2, e);
c = lerp(c, Haz, pow(toward, 3.0) * lowband * 0.45);
c = lerp(c, Haz, saturate(Haze) * exp(-max(el, 0.0) / 0.07));
float sang = acos(clamp(dot(d, normalize(SunD)), -1.0, 1.0));
c += HaloC * 0.25 * exp(-sang / max(radians(HaloSize), 1e-3));
// golden-hour glow on the sun-side horizon (Brickell reference): a saturated orange band, its core a little above SDR
// white. Kept moderate so the tonemapper keeps its colour (R7's horizon under the sun is #FDA170, not white; at 1.6x
// and 2.2x the band bleached to cream in game); the HDR peak is the sun, its halo and the clouds' silver lining
c = lerp(c, HaloC * 1.15, saturate(pow(toward, 5.0) * exp(-max(el, 0.0) / 0.06)));
c = lerp(c, SunC_G * 1.3, saturate(pow(toward, 16.0) * exp(-max(el, 0.0) / 0.025)));
if (el < 0.0) { c = lerp(Haz, Hor * 0.55, saturate(-el / 0.35)); }
"""

EDGE_INK = r"""
float edgeW = max(fwidth(A), 1e-5);
float px = (A - 0.5) / edgeW;
float ink = smoothstep(-0.5, 0.5, px) * (1.0 - smoothstep(Outline - 0.5, Outline + 0.5, px)) * step(0.01, Outline);
float coverage = saturate(px / max(EdgeSoft, 0.35) + 0.5);
"""

BILLBOARD_SUN = r"""
float3 S = normalize(SunD);
float r = radians(max(AngleDeg, 0.01));
float size = 2.0 * Dist * tan(r);
float3 R = normalize(cross(float3(0.0, 0.0, 1.0), S));
float3 U = cross(S, R);
float3 np = CP + S * Dist + ((UV.x - 0.5) * R + (0.5 - UV.y) * U) * size;
return np - WP;
"""

# Large-scale cloud fields, shared by every node that reads the cloud layers (the dome, the sea reflection, the hill
# shadows, the sun occlusion), so they all agree on where the clear sky is. Field() is 1 where a baked layer shows as
# it is and 0 for clear sky; Cover is about the share of the sky the field covers (the bakes alone cover it edge to
# edge, which in-game read as a sky full of streaks; the reference is mostly clear blue between cloud groups).
# Two octaves of value noise (integer hash, quintic fade), mapped through a logistic fit of their distribution
# (std ~0.16) so Cover reads as a share; Cover >= 1 turns the fields off. ~60 ALU, no texture.
COVER = r"""
struct FBskyCover
{
    float H(float2 i)
    {
        uint2 q = asuint(int2(i));
        uint h = (q.x * 1664525u) ^ (q.y * 1013904223u + 0x9e3779b9u);
        h ^= h >> 16; h *= 0x7feb352du; h ^= h >> 15; h *= 0x846ca68bu; h ^= h >> 16;
        return (h & 0xFFFFFFu) * (1.0 / 16777216.0);
    }
    float N(float2 p)
    {
        float2 i = floor(p);
        float2 f = p - i;
        float2 w = f * f * f * (f * (f * 6.0 - 15.0) + 10.0);
        return lerp(lerp(H(i), H(i + float2(1.0, 0.0)), w.x), lerp(H(i + float2(0.0, 1.0)), H(i + float2(1.0, 1.0)), w.x), w.y);
    }
    float Field(float2 p, float Cover)
    {
        if (Cover >= 0.999) return 1.0;
        float n = N(p) * 0.62 + N(p * 2.37 + 17.3) * 0.38;
        float cv = clamp(Cover, 0.02, 0.98);
        float t = 0.5 + 0.094 * log((1.0 - cv) / cv);
        return smoothstep(t - 0.06, t + 0.06, n);
    }
};
FBskyCover cover;
"""
# where each layer's fields are, in its own tile space (they drift with the layer): deck patches ~9 km across; cirrus
# bundles ~20 km along the fibres (U) and ~10 km across (longer bundles read as speed lines when the road runs along
# the fibres)
DECK_FIELD = "cover.Field({uv} * 2.7 + float2(3.1, 7.7), AltoCov)"
CIRRUS_FIELD = "cover.Field(float2({uv}.x * 2.4, {uv}.y * 5.0) + float2(11.3, 5.9), CirCov)"


def bake_meta(name):
    """A bake's metadata (bakes/Day/<name>), next to the scripts in SkyScratch and in the port alike."""
    import json
    here = os.path.dirname(os.path.abspath(__file__)) if "__file__" in globals() else r"K:\Full Throttle\SkyScratch\SourceArt\BayshoreSky\unreal"
    return json.load(open(os.path.join(here, "..", "bakes", "Day", name), encoding="utf-8"))


def layer_tau_multipliers():
    """The multiplier on each layer's B channel (tau / tauScaleRef) in the phase blend, from the bake's own
    layers_meta.json (recommended.TauScale: one number for both layers, or one per layer), so a re-bake that changes
    how B is scaled cannot silently change the look."""
    meta = bake_meta("layers_meta.json")
    ts = meta["recommended"]["TauScale"]
    k = (float(ts["alto"]), float(ts["cirrus"])) if isinstance(ts, dict) else (float(ts), float(ts))
    log("layer tau multipliers from layers_meta.json: alto", k[0], "cirrus", k[1],
        "(tauScaleRef", meta["layers"]["alto"]["tauScaleRef"], meta["layers"]["cirrus"]["tauScaleRef"], ")")
    return k


def cumulus_base_lift():
    """How far above the instance height the card's centre sits, as a share of its height: the bake's cloud base sits
    at atlas v cloud_base_v (cumulus3_meta.json), and the card's centre at v 0.5."""
    v = float(bake_meta("cumulus3_meta.json").get("cloud_base_v", 0.85))
    log("cumulus cloud base at atlas v", v, "-> card lift", round(v - 0.5, 3), "x height")
    return v - 0.5


def with_fields(code):
    """Expand @DECK(uv) and @CIRRUS(uv) in a snippet into the field calls above (the snippets keep their HLSL braces,
    so str.format cannot be used on them), and @KALTO@ / @KCIR@ into the bake's tau multipliers. The node must also
    include COVER and the AltoCov / CirCov inputs."""
    import re
    code = re.sub(r"@DECK\((\w+)\)", lambda mt: DECK_FIELD.replace("{uv}", mt.group(1)), code)
    code = re.sub(r"@CIRRUS\((\w+)\)", lambda mt: CIRRUS_FIELD.replace("{uv}", mt.group(1)), code)
    if "@KALTO@" in code or "@KCIR@" in code:
        ka, kc = layer_tau_multipliers()
        code = code.replace("@KALTO@", "%.4f" % ka).replace("@KCIR@", "%.4f" % kc)
    return code

SUN_VIS = r"""
float4 sc = mul(float4(normalize(SunD) * 1.0e7, 1.0), ResolvedView.TranslatedWorldToClip);
float vis = 0.0;
float2 ndc = float2(2.0, 2.0);
// view-space depth of the 60 km sky dome along the sun direction: anything nearer (buildings, hills, the cumulus cards up
// to ~45 km out) hides the sun; the dome itself does not (review: a fixed 20 km test let distant cards through)
float3 Sv = normalize(SunD);
float3 oc = CP - OP;
float bq = dot(oc, Sv);
float tDome = -bq + sqrt(max(bq * bq - (dot(oc, oc) - 6.0e6 * 6.0e6), 0.0));
float zDome = 0.985 * tDome * max(dot(Sv, ResolvedView.ViewForward), 0.02);
if (sc.w > 0.0)
{
    ndc = sc.xy / sc.w;
    float2 suv = ndc * float2(0.5, -0.5) + 0.5;
    float acc = 0.0;
    [unroll] for (int k = 0; k < 5; k++)
    {
        float2 o = float2(k == 1 ? 1.0 : (k == 2 ? -1.0 : 0.0), k == 3 ? 1.0 : (k == 4 ? -1.0 : 0.0)) * 0.004;
        float2 b = ViewportUVToBufferUV(saturate(suv + o));
        acc += (CalcSceneDepth(b) > zDome) ? 1.0 : 0.0;
    }
    vis = acc / 5.0 * (1.0 - smoothstep(0.95, 1.1, max(abs(ndc.x), abs(ndc.y))));
}
vis *= (1.0 + 0.0 * Dummy);
"""

# the 2.5 km deck along the sun direction (same shell, tile and drift as SKY_PREP): cloud crossing the sun dims the
# halo and the lens ghosts. Vertex shader, mip 2 for a soft crossing.
DECK_SUN_OCC = r"""
{
    // cloud layers in front of the sun (same shells, tiles and drift as SKY_PREP) dim the halo and the lens ghosts;
    // the deck uses its slant-path opacity like the dome does (review: the threshold and shape now match). VS, mip 2.
    float3 Sd = normalize(SunD);
    float3 rl = CP - OP;
    float2 wd = normalize(Wind.xy + float2(1e-5, 0.0));
    float2 wq = float2(-wd.y, wd.x);
    float2 cwd = float2(wd.x * 0.9659 + wd.y * 0.2588, -wd.x * 0.2588 + wd.y * 0.9659);
    float2 cwq = float2(-cwd.y, cwd.x);
    const float RcO = 3.0e7;
    float hO = max(250000.0 - rl.z, 20000.0);
    float sO = sqrt(RcO * RcO * Sd.z * Sd.z + 2.0 * RcO * hO + hO * hO);
    float tO = (2.0 * RcO * hO + hO * hO) / max(RcO * Sd.z + sO, 1.0);
    float2 pO = rl.xy + Sd.xy * tO;
    float2 uvO = float2(dot(pO, wd) - Time * Speed, dot(pO, wq)) / 2400000.0;
    float aO = saturate(Texture2DSampleLevel(AltoA, AltoASampler, uvO, 2.0).r);
    aO = (1.0 - pow(1.0 - aO, clamp((RcO + hO) / max(sO, 1.0), 1.0, 6.0))) * @DECK(uvO);
    float hC = max(700000.0 - rl.z, 200000.0);
    float sC = sqrt(RcO * RcO * Sd.z * Sd.z + 2.0 * RcO * hC + hC * hC);
    float tC = (2.0 * RcO * hC + hC * hC) / max(RcO * Sd.z + sC, 1.0);
    float2 pC = rl.xy + Sd.xy * tC;
    float2 uvC = float2(dot(pC, cwd) - Time * Speed * 2.2, dot(pC, cwq)) / 4800000.0 + 0.37;
    // the cirrus as the dome draws it: slant-path opacity, and none at Low quality, where the dome skips the cirrus
    float aC = saturate(Texture2DSampleLevel(CirA, CirASampler, uvC, 2.0).r);
    aC = (1.0 - pow(1.0 - aC, clamp((RcO + hC) / max(sC, 1.0), 1.0, 6.0))) * @CIRRUS(uvC) * (1.0 - step(0.5, LowQ));
    float up = smoothstep(-0.004, 0.04, Sd.z);
    vis *= (1.0 - 0.9 * aO * up) * (1.0 - 0.45 * aC * up);
}
"""


def occ_inputs(g, alto_a, cir_a):
    return [("AltoA", g.tex_object("AltoAlpha", alto_a)), ("CirA", g.tex_object("CirrusAlpha", cir_a)), ("CP", g.campos()),
            ("OP", g.objpos()), ("Time", g.time()), ("Wind", g.mpc("WindDirection")), ("Speed", g.mpc("CloudDriftSpeed")),
            ("AltoCov", g.mpc("AltoCoverage")), ("CirCov", g.mpc("CirrusCoverage")), ("LowQ", g.mpc("LowQuality"))]


# the sun visibility (vertex shader of the glow and the ghosts): scene depth, then the cloud layers crossing the sun
SUN_OCC = COVER + SUN_VIS + with_fields(DECK_SUN_OCC)


SKY_PREP = r"""
float eld = degrees(el);
float u = atan2(d.y, d.x) * 0.15915494 + 0.5;
float2 uv = float2(u, saturate((32.0 - eld) / 40.0));
float3 atmo = Texture2DSampleLevel(Atmo, AtmoSampler, uv, 0.0).rgb * Expo;
float3 base = lerp(atmo, c, saturate(Stylize));
base = lerp(base, c, smoothstep(24.0, 31.5, eld));
base = (eld >= 32.0) ? c : base;
float3 skyBehind = base;   // aerial perspective: distant cloud fades toward the sky colour behind it
// sun disc, HDR, drawn under the clouds so drifting cloud crosses it
{
    float sw = max(fwidth(sang), 1e-5);
    float dR = radians(DiscSize);
    float disc = saturate((dR - sang) / sw + 0.5);
    float limb = 1.0 - 0.35 * pow(saturate(sang / max(dR, 1e-4)), 2.0);
    base = lerp(base, SunC * SunI * limb, disc);
}
// --- photoreal cloud layers: Cycles volume renders seen from below (blender/bake_layers.py), tiling seamlessly: an
// altocumulus deck at 2.5 km (24 km tile) and cirrus at 7 km (48 km tile, 15 deg off the deck wind, 2.2x faster), on
// curved shells (virtual 300 km radius) so they parallax against each other and the horizon as the car drives. Each
// layer: Light (HDR: R sun, G sky, B optical depth) and Alpha (opacity). Volumetric lighting, live: a two-lobe
// Henyey-Greenstein phase function makes thin cloud toward the sun glow (HDR, the silver lining) while thick cloud
// scatters evenly; the slant path through each layer thickens it toward the horizon.
const float Rc = 3.0e7;
float3 rel = CP - OP;
float fadeEl = smoothstep(-0.004, 0.04, d.z);
float2 wd = normalize(Wind.xy + float2(1e-5, 0.0));
float2 wp = float2(-wd.y, wd.x);
float2 cwd = float2(wd.x * 0.9659 + wd.y * 0.2588, -wd.x * 0.2588 + wd.y * 0.9659);
float2 cwp = float2(-cwd.y, cwd.x);
float3 S = normalize(SunD);
float cth = dot(d, S);
float hgF = 0.36 / pow(max(1.64 - 1.6 * cth, 1e-4), 1.5);          // g = 0.8 (the 4pi cancels in the ratio)
float hgB = 0.96 / pow(1.04 + 0.4 * cth, 1.5);                     // g = -0.2
float hg0 = 0.7 * 0.36 / pow(1.64 - 1.6 * S.z, 1.5) + 0.3 * 0.96 / pow(1.04 + 0.4 * S.z, 1.5);   // the bake looked straight up
// capped at 10: near the sun thin cloud still glows past SDR white (the silver lining), but no longer bleaches whole
// cloud fields to white as the 24x cap did in game (R7's clouds near the sun stay coral, with bright edges)
float ratio = min((0.7 * hgF + 0.3 * hgB) / hg0, 10.0);
float3 SunTint = RimC * 1.6;
float3 AmbTint = ShadowC * 0.9;
float drift = Time * Speed;
float h1 = max(250000.0 - rel.z, 20000.0);
float s1 = sqrt(Rc * Rc * d.z * d.z + 2.0 * Rc * h1 + h1 * h1);
float t1 = (2.0 * Rc * h1 + h1 * h1) / max(Rc * d.z + s1, 1.0);
float2 p1 = rel.xy + d.xy * t1;
float2 uv1 = float2(dot(p1, wd) - drift, dot(p1, wp)) / 2400000.0;
float h2 = max(700000.0 - rel.z, 200000.0);
float s2 = sqrt(Rc * Rc * d.z * d.z + 2.0 * Rc * h2 + h2 * h2);
float t2 = (2.0 * Rc * h2 + h2 * h2) / max(Rc * d.z + s2, 1.0);
float2 p2 = rel.xy + d.xy * t2;
float2 uv2 = float2(dot(p2, cwd) - drift * 2.2, dot(p2, cwp)) / 4800000.0 + 0.37;
float2 dx1 = ddx(uv1), dy1 = ddy(uv1), dx2 = ddx(uv2), dy2 = ddy(uv2);
// isotropic mip from the larger footprint axis: where a layer is seen edge-on its fibres and cells blur into haze, as
// distant cloud does, instead of staying sharp along the view (anisotropic filtering kept the cirrus fibres crisp,
// which read as speed lines converging on the road's vanishing point, and the far deck as busy popcorn)
float lod1 = log2(max(max(length(dx1), length(dy1)) * 2048.0, 1.0)) + 0.25;
float lod2 = log2(max(max(length(dx2), length(dy2)) * 2048.0, 1.0)) + 0.5;
// cloud fields: clear blue sky between deck patches and between cirrus bundles, as in the reference (COVER); the
// layers are only sampled inside a field, which also skips their four texture reads over most of the sky
float m1 = @DECK(uv1);
float m2 = @CIRRUS(uv2);
float a1 = 0.0, a2 = 0.0;
float3 c1 = 0.0, c2 = 0.0;
// aerial perspective: humid bay air, so distant cloud melts into the sky colour behind it (twice the gradient's haze)
float aerial = saturate(Haze * 2.0);
[branch] if (fadeEl > 0.0)
{
    [branch] if (m1 > 0.0)
    {
        float3 L1 = Texture2DSampleLevel(AltoL, AltoLSampler, uv1, lod1).rgb;
        float A1 = saturate(Texture2DSampleLevel(AltoA, AltoASampler, uv1, lod1).r);
        // B holds tau / tauScaleRef; the bake's recommended multiplier on B (layers_meta.json, written in at build time)
        // sets where cloud stops following the phase function; TauScale is a live tuning multiplier on top
        c1 = CloudExpo * (L1.r * SunTint * lerp(1.0, ratio, exp(-L1.b * @KALTO@ * TauScale)) + L1.g * AmbTint);
        c1 = lerp(c1, skyBehind, aerial * (1.0 - exp(-t1 / max(HazeDist, 1.0))));
        a1 = (1.0 - pow(1.0 - A1, clamp((Rc + h1) / max(s1, 1.0), 1.0, 6.0))) * fadeEl * m1;
    }
    [branch] if (LowQ < 0.5 && m2 > 0.0)
    {
        float3 L2 = Texture2DSampleLevel(CirL, CirLSampler, uv2, lod2).rgb;
        float A2 = saturate(Texture2DSampleLevel(CirA, CirASampler, uv2, lod2).r);
        // cirrus: its own exposure and its own recommended multiplier, so the forward glow stays on thin edges near the sun
        c2 = CirExpo * (L2.r * SunTint * lerp(1.0, ratio, exp(-L2.b * @KCIR@ * TauScale)) + L2.g * AmbTint);
        c2 = lerp(c2, skyBehind, aerial * (1.0 - exp(-t2 / max(HazeDist, 1.0))));
        a2 = (1.0 - pow(1.0 - A2, clamp((Rc + h2) / max(s2, 1.0), 1.0, 6.0))) * fadeEl * m2;
    }
}
base = lerp(base, c2, a2);
base = lerp(base, c1, a1);
ElDeg = eld;
SeaV = 1.0 - saturate(asin(saturate(max(-d.z, 0.0))) / radians(28.0));
return base;
"""

SKY_SHADE = r"""
// sea backdrop below the horizon: depth gradient, the sky and a blurred cloud deck reflected with Fresnel, the glitter
// path under the sun and glints anchored to the world (they slide past as the car drives).
// Derivatives first (outside the branch), then the sea is only evaluated where it shows.
float3 sky = Base;
float3 d = normalize(D);
float dep = max(-d.z, 0.0);
float2 sp = d.xy / max(dep, 0.002) + (CP.xy - OP.xy) / 2000.0;
float2 q = sp * float2(2.5, 8.0);
q.y += T * 0.35;
q.x += sin(q.y * 0.7 + T * 0.5) * 0.3;
float aa = saturate(1.5 - max(fwidth(q.x), fwidth(q.y)));
float seaMask = saturate(-ElDeg / max(fwidth(ElDeg), 1e-4) + 0.5) * saturate(SeaOn);
float3 sc = sky;
[branch] if (seaMask > 0.0)
{
    float3 S = normalize(SunD);
    float elb = asin(saturate(dep));
    float t = saturate(elb / radians(28.0));
    float farband = 1.0 - smoothstep(0.05, 0.16, dep);
    sc = lerp(Deep, HorC, SeaV);
    // reflection: the sky gradient and the deck along the mirrored ray, rippled, one mip-3 tap
    float3 rl = CP - OP;
    float2 rxy = d.xy + float2(sin(q.y * 1.3 + T), cos(q.x * 0.9 - T * 0.7)) * 0.012 * (1.0 - 0.7 * farband);
    const float RcR = 3.0e7;
    float hR = max(250000.0 - rl.z, 20000.0);
    float sR = sqrt(RcR * RcR * dep * dep + 2.0 * RcR * hR + hR * hR);
    float tR = (2.0 * RcR * hR + hR * hR) / max(RcR * dep + sR, 1.0);
    float2 pR = rl.xy + rxy * tR;
    float2 wd = normalize(Wind.xy + float2(1e-5, 0.0));
    float2 wq = float2(-wd.y, wd.x);
    float2 uvR = float2(dot(pR, wd) - T * Speed, dot(pR, wq)) / 2400000.0;
    float3 LR = Texture2DSampleLevel(AltoL, AltoLSampler, uvR, 3.0).rgb;
    float ar = saturate(Texture2DSampleLevel(AltoA, AltoASampler, uvR, 3.0).r);
    ar = (1.0 - pow(1.0 - ar, clamp((RcR + hR) / max(sR, 1.0), 1.0, 6.0))) * smoothstep(-0.004, 0.04, dep) * @DECK(uvR);
    float eR = saturate(elb / 1.5707963);
    float mR = clamp(MidPt, 0.02, 0.6);
    float3 cR = (eR < mR) ? lerp(SkyHor, SkyMid, smoothstep(0.0, 1.0, eR / mR)) : lerp(SkyMid, SkyZen, smoothstep(0.0, 1.0, saturate((eR - mR) / (2.0 * mR))));
    cR = lerp(cR, HazC, saturate(Haze) * exp(-elb / 0.07));
    // the reflected sky keeps the sun-side golden glow (review: the sea under the orange sky reflected blue-lavender)
    float2 dhR = normalize(d.xy + float2(1e-5, 0.0));
    float2 shR = normalize(S.xy + float2(1e-5, 0.0));
    float towardR = saturate(dot(dhR, shR) * 0.5 + 0.5);
    cR = lerp(cR, HaloC * 1.15, saturate(pow(towardR, 5.0) * exp(-elb / 0.06)));
    cR = lerp(cR, SunC_G * 1.3, saturate(pow(towardR, 16.0) * exp(-elb / 0.025)));
    float3 rc = CloudExpo * (LR.r * RimC * 1.6 + LR.g * ShadowC * 0.9);
    float F = min(0.02 + 0.98 * pow(1.0 - saturate(dep), 5.0), 0.28);
    sc = lerp(sc, lerp(cR, rc, ar), F);
    // glitter path and glints
    float2 dh = normalize(d.xy + float2(1e-5, 0.0));
    float2 sh = normalize(S.xy + float2(1e-5, 0.0));
    float dAz = acos(clamp(dot(dh, sh), -1.0, 1.0));
    float w = 0.03 + 0.10 * t + 0.12 * (1.0 - S.z);
    float path = exp(-pow(dAz / w, 2.0)) * smoothstep(0.0, 0.004, dep);
    float2 ci = floor(q);
    float2 cf = frac(q);
    uint2 ui = asuint(int2(ci));
    uint hh = (ui.x * 1664525u) ^ (ui.y * 1013904223u + 0x9e3779b9u);
    hh ^= hh >> 16; hh *= 0x7feb352du; hh ^= hh >> 15;
    float h = (hh & 0xFFFFFFu) * (1.0 / 16777216.0);
    float2 rd = (cf - 0.5) * float2(1.0, 2.2);
    float dash = exp(-dot(rd, rd) * 18.0);
    float sparkle = step(1.0 - 0.35 * path, h) * dash * aa * farband;
    sc += Glit * (sparkle * 1.6 + path * 0.35 * (0.35 + 0.65 * farband)) * Strength;
    sc = lerp(sc, lerp(HorC, HazC, 0.35), saturate(Haze) * exp(-elb / 0.006) * 0.5);
}
return lerp(sky, sc, seaMask) * Bright;
"""


# ------------------------------------------------------------------------------------------------ MF_SemiCelRamp
def build_ramp(mpc):
    fn = load_or_create(FN, "MF_SemiCelRamp", unreal.MaterialFunction, unreal.MaterialFunctionFactoryNew())
    MEL.delete_all_material_expressions_in_function(fn)
    fn.set_editor_property("Description",
                           "Semi-cel banding for every BayshoreSky material. Value (0-1, continuous) -> a few bands with soft, narrow edges "
                           "and a gentle gradient inside each band. Band count, softness and in-band gradient come from MPC_BayshoreSky.")
    fn.set_editor_property("bExposeToLibrary", True)
    g = Graph(fn, mpc, is_function=True)
    vin = g.node(unreal.MaterialExpressionFunctionInput)
    vin.set_editor_property("InputName", "Value")
    vin.set_editor_property("InputType", enum_entry(unreal.FunctionInputType, "FunctionInput_Scalar"))
    vin.set_editor_property("SortPriority", 0)
    code = r"""
float n = max(Bands, 2.0);
float v = saturate(Value);
float x = v * (n - 1.0);
float i = floor(x);
float f = x - i;
float aa = 0.0;
#if PIXELSHADER
aa = fwidth(x);
#endif
float s = Softness * (1.0 - 0.75 * saturate(LowQ));
float w = max(s * 0.5, aa);
float t = smoothstep(0.5 - w, 0.5 + w, f);
float q = (i + t) / (n - 1.0);
return lerp(q, v, saturate(Grad));
"""
    c = g.custom("SemiCelRamp", code, [("Value", (vin, "")), ("Bands", g.mpc("BandCount")), ("Softness", g.mpc("BandSoftness")),
                                         ("Grad", g.mpc("BandGradient")), ("LowQ", g.mpc("LowQuality"))], "CMOT_Float1")
    out = g.node(unreal.MaterialExpressionFunctionOutput)
    out.set_editor_property("OutputName", "Result")
    out.set_editor_property("SortPriority", 0)
    g.link((c, ""), out, "")
    MEL.update_material_function(fn)
    save(fn)
    log("MF_SemiCelRamp built")
    return fn


# ------------------------------------------------------------------------------------------------ materials
def grad_inputs(g, d_src):
    return [("D", d_src), ("Zen", g.mpc("ZenithColor")), ("Mid", g.mpc("MidColor")), ("Hor", g.mpc("HorizonColor")),
            ("Haz", g.mpc("HazeColor")), ("SunD", g.mpc("SunDirection")), ("HaloC", g.mpc("SunHaloColor")),
            ("MidPt", g.mpc("GradientMidPoint")), ("Haze", g.mpc("Haze")), ("HaloSize", g.mpc("SunHaloSize")),
            ("SunC_G", g.mpc("SunColor"))]


def neg_camvec(g):
    n = g.node(unreal.MaterialExpressionMultiply)
    n.set_editor_property("ConstB", -1.0)
    g.link(g.camvec(), n, "A")
    return (n, "")


def build_dome(mpc):
    m = reset_material("M_SkyDome", "BLEND_Opaque", extra={"bIsSky": True})
    g = Graph(m, mpc)
    c = g.custom("SkyDome", GRAD + "return c * Bright;", grad_inputs(g, neg_camvec(g)) + [("Bright", g.mpc("SkyBrightness"))])
    g.out((c, ""), "MP_EmissiveColor")
    compile_check(m)
    return m


def build_panorama(mpc, ramp, atmo_tex, clouds_tex):
    m = reset_material("M_SkyPanorama", "BLEND_Opaque")
    g = Graph(m, mpc)
    prep = r"""
""" + GRAD + r"""
float u = atan2(d.y, d.x) * 0.15915494 + 0.5;
float eld = degrees(el);
float2 uv = float2(u, saturate((32.0 - eld) / 40.0));
float3 atmo = Texture2DSampleLevel(Atmo, AtmoSampler, uv, 0.0).rgb * Expo;
float3 base = lerp(atmo, c, saturate(Stylize));
base = lerp(base, c, smoothstep(24.0, 31.5, eld));
float4 cl = Texture2DSampleLevel(Clouds, CloudsSampler, uv, 0.0);
G = cl.g; B = cl.b; A = cl.a; ElDeg = eld;
SunSide = pow(saturate(dot(normalize(d.xy + float2(1e-5, 0.0)), normalize(SunD.xy + float2(1e-5, 0.0)))), 4.0);
return base;
"""
    p = g.custom("PanoramaPrep", prep, grad_inputs(g, neg_camvec(g)) + [
        ("Atmo", g.tex_object("Atmosphere", atmo_tex)), ("Clouds", g.tex_object("CloudBank", clouds_tex)),
        ("Expo", g.mpc("PanoramaExposure")), ("Stylize", g.mpc("PanoramaStylize"))],
        extra=[("G", "CMOT_Float1"), ("B", "CMOT_Float1"), ("A", "CMOT_Float1"), ("ElDeg", "CMOT_Float1"), ("SunSide", "CMOT_Float1")])
    band = g.ramp((p, "G"), ramp)
    shade = r"""
float3 col = lerp(ShadowC, LitC, Banded);
col *= lerp(1.0, 0.82 + 0.18 * B, saturate(AmbGrad));
""" + EDGE_INK + r"""
float rim = (1.0 - smoothstep(0.0, 6.0, px)) * SunSide * Rim;
col = lerp(col, RimC, saturate(rim));
col = lerp(col, InkC, ink);
col = lerp(col, HazC, saturate(Haze) * (1.0 - smoothstep(0.0, 10.0, ElDeg)) * 0.55);
return lerp(Base, col, coverage) * Bright;
"""
    s = g.custom("PanoramaShade", shade, [
        ("Base", (p, "")), ("Banded", band), ("B", (p, "B")), ("A", (p, "A")), ("ElDeg", (p, "ElDeg")), ("SunSide", (p, "SunSide")),
        ("LitC", g.mpc("CloudLitColor")), ("ShadowC", g.mpc("CloudShadowColor")), ("RimC", g.mpc("CloudRimColor")),
        ("InkC", g.mpc("OutlineColor")), ("Outline", g.mpc("OutlineWidth")), ("EdgeSoft", g.mpc("CloudEdgeSoftness")),
        ("Rim", g.mpc("CloudRimStrength")), ("AmbGrad", g.mpc("CloudAmbientGradient")), ("Haze", g.mpc("Haze")),
        ("HazC", g.mpc("HazeColor")), ("Bright", g.mpc("SkyBrightness"))])
    g.out((s, ""), "MP_EmissiveColor")
    compile_check(m)
    return m


def build_sky_dome(mpc, ramp, atmo_tex, alto_l, alto_a, cir_l, cir_a):
    """One sky-pass material for the whole backdrop sphere: the art-directed gradient with its HDR golden-hour glow, the
    baked atmosphere strip (-8 to +32 deg), the HDR sun disc, two photoreal parallax cloud layers (Cycles bakes) and the
    sea below the horizon. One draw, no overlap; as an Is Sky material it skips SSAO and reflections. Scene-referred
    HDR throughout: nothing is clamped, so HDR displays get the glow and SDR rolls it off in the tonemapper. No cel
    bands or ink (owner, 14 Nov). The hills ring (finite distance) is drawn in the base pass in front."""
    m = reset_material("M_SkyDome", "BLEND_Opaque", extra={"bIsSky": True})
    g = Graph(m, mpc)
    al = g.tex_object("AltoLight", alto_l)
    aa = g.tex_object("AltoAlpha", alto_a)
    prep = GRAD + COVER + with_fields(SKY_PREP)
    p = g.custom("SkyPrep", prep, grad_inputs(g, neg_camvec(g)) + [
        ("AltoCov", g.mpc("AltoCoverage")), ("CirCov", g.mpc("CirrusCoverage")),
        ("Atmo", g.tex_object("Atmosphere", atmo_tex)),
        ("Expo", g.mpc("PanoramaExposure")), ("Stylize", g.mpc("PanoramaStylize")),
        ("AltoL", al), ("AltoA", aa), ("CirL", g.tex_object("CirrusLight", cir_l)), ("CirA", g.tex_object("CirrusAlpha", cir_a)),
        ("CP", g.campos()), ("OP", g.objpos()), ("Time", g.time()),
        ("Wind", g.mpc("WindDirection")), ("Speed", g.mpc("CloudDriftSpeed")),
        ("ShadowC", g.mpc("CloudShadowColor")), ("RimC", g.mpc("CloudRimColor")), ("HazC", g.mpc("HazeColor")),
        ("HazeDist", g.mpc("HazeDistance")), ("CloudExpo", g.mpc("CloudExposure")), ("CirExpo", g.mpc("CirrusExposure")),
        ("TauScale", g.mpc("CloudTauScale")), ("LowQ", g.mpc("LowQuality")),
        ("DiscSize", g.mpc("SunDiscSize")), ("SunC", g.mpc("SunColor")), ("SunI", g.mpc("SunIntensity"))],
        extra=[("ElDeg", "CMOT_Float1"), ("SeaV", "CMOT_Float1")])
    s = g.custom("SkyShade", COVER + with_fields(SKY_SHADE), [
        ("AltoCov", g.mpc("AltoCoverage")), ("CirCov", g.mpc("CirrusCoverage")),
        ("Base", (p, "")), ("ElDeg", (p, "ElDeg")), ("SeaV", (p, "SeaV")), ("D", neg_camvec(g)), ("SunD", g.mpc("SunDirection")),
        ("CP", g.campos()), ("OP", g.objpos()), ("AltoL", al), ("AltoA", aa),
        ("Wind", g.mpc("WindDirection")), ("Speed", g.mpc("CloudDriftSpeed")),
        ("ShadowC", g.mpc("CloudShadowColor")), ("RimC", g.mpc("CloudRimColor")), ("CloudExpo", g.mpc("CloudExposure")),
        ("HaloC", g.mpc("SunHaloColor")), ("SunC_G", g.mpc("SunColor")),
        ("SkyHor", g.mpc("HorizonColor")), ("SkyMid", g.mpc("MidColor")), ("SkyZen", g.mpc("ZenithColor")),
        ("MidPt", g.mpc("GradientMidPoint")), ("Haze", g.mpc("Haze")),
        ("HazC", g.mpc("HazeColor")), ("Deep", g.mpc("SeaDeepColor")), ("HorC", g.mpc("SeaHorizonColor")),
        ("Glit", g.mpc("GlitterColor")), ("Strength", g.mpc("SeaGlitterStrength")), ("SeaOn", g.mpc("SeaEnabled")),
        ("T", g.time()), ("Bright", g.mpc("SkyBrightness"))])
    g.out((s, ""), "MP_EmissiveColor")
    compile_check(m)
    return m


def build_cloud(mpc, ramp, cum_sun, cum_aux):
    """Photoreal cumulus cards (Cycles, bake_sky.py stage cumulus3): each card was rendered with the sun behind it
    (back-lit, phi 20), at its side (phi 90) and behind the camera (front-lit, phi 160). The vertex shader works out phi
    between the view and the sun for this card and blends the three, so cards toward the sun read dark with glowing rims
    and cards away from it read evenly lit, as in the reference. The mirror is fixed per card (custom data 5, set at
    placement) so a card never flips while the car drives (review finding)."""
    m = reset_material("M_SemiCelCloud", "BLEND_Masked", two_sided=True, extra={
        "OpacityMaskClipValue": 0.5, "DitherOpacityMask": True, "NumCustomizedUVs": 3,
        "bUsedWithInstancedStaticMeshes": True, "MaxWorldPositionOffsetDisplacement": 1200000.0,
        "bAlwaysEvaluateWorldPositionOffset": True})
    g = Graph(m, mpc)
    vs = r"""
float3 off = WP - OP;
float scale = length(off) / 55.9017;
float Wd = 100.0 * scale * 2.4;   // cumulus cards: big masses, as in the reference
float Ht = 50.0 * scale * 2.4;
float span = max(C4, 1.0);
float fr = frac(T * Speed / span + C1);
float2 wdir = normalize(Wind.xy + float2(1e-5, 0.0));
float3 pivot = OP + float3(wdir * (fr - 0.5) * span, Ht * @CULIFT@);   // the cloud base (cumulus3_meta.json) sits at the instance height
float fade = smoothstep(0.0, 0.08, fr) * (1.0 - smoothstep(0.92, 1.0, fr));
float shown = step(C2 + 0.5, Shown);
float3 F = pivot - CP;
float3 Fh = normalize(float3(F.xy, 0.0) + float3(1e-4, 0.0, 0.0));
float3 R = normalize(cross(float3(0.0, 0.0, 1.0), Fh));
float3 U = float3(0.0, 0.0, 1.0);
float mirror = (C5 > 0.5) ? 1.0 : 0.0;
float3 np = pivot + ((UV.x - 0.5) * Wd * R + (0.5 - UV.y) * Ht * U) * shown;
float cell = floor(C0 + 0.5);
float cx = fmod(cell, 2.0);
float cy = floor(cell / 2.0);
float uu = (mirror > 0.5) ? (1.0 - UV.x) : UV.x;
AtlasUV = float2((cx + uu) * 0.5, (cy + UV.y) * 0.25);
// sun direction against this view: phi 0 = looking toward the sun (back-lit card)
float3 Sd = normalize(SunD);
float2 Sh = normalize(Sd.xy + float2(1e-5, 0.0));
float phi = degrees(acos(clamp(dot(Fh.xy, Sh), -1.0, 1.0)));
float wB = 1.0 - saturate((phi - 20.0) / 70.0);
wB *= wB;   // forward scattering falls off fast away from the sun: by phi 40 most of the back-lit rim is gone (review)
float wF = saturate((phi - 90.0) / 70.0);
float wS = saturate(1.0 - wB - wF);
// the baked side light falls on +u, or on -u when mirrored; if the sun is now on the other side, move that weight
float bakedSide = (mirror > 0.5) ? -1.0 : 1.0;
float agree = saturate(0.5 + 3.0 * dot(R.xy, Sd.xy) * bakedSide);
float give = wS * (1.0 - agree);
float toFront = saturate((phi - 20.0) / 140.0);
wB += give * (1.0 - toFront);
wF += give * toFront;
Extra = float2(fade * shown, 0.0);
Light = float2(wB, wF);
return np - WP;
"""
    vs = vs.replace("@CULIFT@", "%.4f" % cumulus_base_lift())
    v = g.custom("CloudBillboard", vs, [
        ("UV", g.texcoord(0)), ("WP", g.worldpos("WPT_ExcludeAllShaderOffsets")), ("OP", g.objpos()), ("CP", g.campos()),
        ("C0", g.custom_data(0)), ("C1", g.custom_data(1)), ("C2", g.custom_data(2)), ("C3", g.custom_data(3)),
        ("C4", g.custom_data(4)), ("C5", g.custom_data(5)),
        ("T", g.time()), ("Wind", g.mpc("WindDirection")), ("Speed", g.mpc("CloudDriftSpeed")), ("Shown", g.mpc("CloudCardsShown")),
        ("SunD", g.mpc("SunDirection"))], extra=[("AtlasUV", "CMOT_Float2"), ("Extra", "CMOT_Float2"), ("Light", "CMOT_Float2")])
    g.out((v, ""), "MP_WorldPositionOffset")
    g.out((v, "AtlasUV"), "MP_CustomizedUVs0")
    g.out((v, "Extra"), "MP_CustomizedUVs1")
    g.out((v, "Light"), "MP_CustomizedUVs2")
    # aerial perspective toward the sky actually behind the card (the gradient with its sun-side glow, GRAD), as the
    # cloud layers do, so sun-side clusters sit in the glowing haze as in the reference (review: they faded to a flat
    # haze colour before)
    shade = GRAD + r"""
float3 sp = Texture2DSample(CumSun, CumSunSampler, UV0).rgb;     // HDR sun luminance: back, side, front
float4 ax = Texture2DSample(CumAux, CumAuxSampler, UV0);         // sky luminance (normalised), opacity, thickness
float wB = W.x, wF = W.y;
float wS = saturate(1.0 - wB - wF);
float sunL = sp.r * wB + sp.g * wS + sp.b * wF;
float skyL = ax.r * SkyNorm;
float3 col = CardExpo * (sunL * RimC * 1.6 + skyL * ShadowC * 0.9);
float dist = length(WP - CP);
col = lerp(col, c, saturate(Haze * 2.0) * (1.0 - exp(-dist / max(HazeDist, 1.0))));
Opacity = ax.g * X.x;
return col * Bright;
"""
    s = g.custom("CloudShade", shade, grad_inputs(g, neg_camvec(g)) + [
        ("CumSun", g.tex_object("CumulusSun", cum_sun)), ("CumAux", g.tex_object("CumulusAux", cum_aux)),
        ("UV0", g.texcoord(0)), ("X", g.texcoord(1)), ("W", g.texcoord(2)),
        ("WP", g.worldpos()), ("CP", g.campos()),
        ("ShadowC", g.mpc("CloudShadowColor")), ("RimC", g.mpc("CloudRimColor")),
        ("HazeDist", g.mpc("HazeDistance")), ("Bright", g.mpc("SkyBrightness")),
        ("CardExpo", g.mpc("CardExposure")), ("SkyNorm", g.mpc("CumulusSkyNorm"))],
        extra=[("Opacity", "CMOT_Float1")])
    g.out((s, ""), "MP_EmissiveColor")
    g.out((s, "Opacity"), "MP_OpacityMask")
    compile_check(m)
    return m


def sun_vs(g, angle_param, angle_scale):
    k = g.node(unreal.MaterialExpressionMultiply)
    k.set_editor_property("ConstB", angle_scale)
    g.link(g.mpc(angle_param), k, "A")
    return g.custom("SunBillboard", BILLBOARD_SUN, [
        ("UV", g.texcoord(0)), ("WP", g.worldpos("WPT_ExcludeAllShaderOffsets")), ("CP", g.campos()),
        ("SunD", g.mpc("SunDirection")), ("Dist", g.mpc("SunDistance")), ("AngleDeg", (k, ""))])


def build_sun_disc(mpc, ramp):
    m = reset_material("M_SunDisc", "BLEND_Masked", two_sided=True, extra={
        "OpacityMaskClipValue": 0.5, "DitherOpacityMask": True, "bAlwaysEvaluateWorldPositionOffset": True})
    g = Graph(m, mpc)
    g.out((sun_vs(g, "SunDiscSize", 1.35), ""), "MP_WorldPositionOffset")
    prep = r"""
float3 dd = normalize(WP - CP);
float ang = degrees(acos(clamp(dot(dd, normalize(SunD)), -1.0, 1.0)));
float t = 1.0 - ang / max(Size, 0.01);
float aa = max(fwidth(ang), 1e-4);
Coverage = saturate((Size - ang) / aa + 0.5);
return saturate(t * 1.6);
"""
    p = g.custom("SunDiscPrep", prep, [("WP", g.worldpos()), ("CP", g.campos()), ("SunD", g.mpc("SunDirection")),
                                        ("Size", g.mpc("SunDiscSize"))], "CMOT_Float1", extra=[("Coverage", "CMOT_Float1")])
    band = g.ramp((p, ""), ramp)
    shade = g.custom("SunDiscShade", "return lerp(HaloC, SunC * 1.4, Banded) * Bright * Intensity;", [
        ("Banded", band), ("HaloC", g.mpc("SunHaloColor")), ("SunC", g.mpc("SunColor")),
        ("Bright", g.mpc("SkyBrightness")), ("Intensity", g.mpc("SunIntensity"))])
    g.out((shade, ""), "MP_EmissiveColor")
    g.out((p, "Coverage"), "MP_OpacityMask")
    compile_check(m)
    return m


def build_sun_glow(mpc, alto_a, cir_a):
    m = reset_material("M_SunGlow", "BLEND_Additive", two_sided=True, extra={"bAlwaysEvaluateWorldPositionOffset": True, "NumCustomizedUVs": 2,
                                                             "bUseTranslucencyVertexFog": False})
    g = Graph(m, mpc)
    g.out((sun_vs(g, "SunHaloSize", 1.7), ""), "MP_WorldPositionOffset")
    depth = g.node(unreal.MaterialExpressionSceneDepth)
    visv = g.custom("SunVisibilityVS", SUN_OCC + "return float2(vis, 0.0);",
                    [("SunD", g.mpc("SunDirection")), ("Dummy", (depth, ""))] + occ_inputs(g, alto_a, cir_a), "CMOT_Float2")
    g.out((visv, ""), "MP_CustomizedUVs1")
    code = "float vis = VisUV.x;" + r"""
float3 S = normalize(SunD);
float3 dd = normalize(WP - CP);
float ang = degrees(acos(clamp(dot(dd, S), -1.0, 1.0)));
float3 R = normalize(cross(float3(0.0, 0.0, 1.0), S));
float3 U = cross(S, R);
float phi = atan2(dot(dd, U), dot(dd, R));
float halo = exp(-ang / max(HaloSize, 0.1));
float rays = 0.0;
[branch] if (Rays * (1.0 - saturate(LowQ)) > 0.0)   // uniform branch: the Day look has no rays
{
    rays = pow(saturate(cos(phi * 9.0 + T * 0.05)), 18.0) * 0.7 + pow(saturate(cos(phi * 14.0 - T * 0.03 + 1.3)), 24.0) * 0.4;
    rays *= exp(-ang / max(HaloSize * 1.8, 0.1)) * smoothstep(DiscSize * 0.7, DiscSize * 1.8, ang) * Rays * (1.0 - saturate(LowQ));
}
float edge = 1.0 - smoothstep(HaloSize * 1.3, HaloSize * 1.7, ang);
HaloOnly = HaloC * halo * 0.6 * vis * edge * Bright;
return (HaloC * halo * 0.6 + SunC * rays) * vis * edge * Bright;
"""
    c = g.custom("SunGlow", code, [
        ("WP", g.worldpos()), ("CP", g.campos()), ("SunD", g.mpc("SunDirection")), ("HaloSize", g.mpc("SunHaloSize")),
        ("DiscSize", g.mpc("SunDiscSize")), ("Rays", g.mpc("SunRayStrength")), ("LowQ", g.mpc("LowQuality")),
        ("HaloC", g.mpc("SunHaloColor")), ("SunC", g.mpc("SunColor")), ("Bright", g.mpc("SkyBrightness")),
        ("T", g.time()), ("VisUV", g.texcoord(1))], extra=[("HaloOnly", "CMOT_Float3")])
    q = g.node(unreal.MaterialExpressionQualitySwitch)
    g.link((c, ""), q, "Default")
    g.link((c, "HaloOnly"), q, "Low")
    g.out((q, ""), "MP_EmissiveColor")
    compile_check(m)
    return m


def build_flares(mpc, alto_a, cir_a):
    m = reset_material("M_FlareGhosts", "BLEND_Additive", two_sided=True, extra={
        "bDisableDepthTest": True, "TranslucencyPass": enum_entry(unreal.MaterialTranslucencyPass, "MTP_AfterDOF"), "NumCustomizedUVs": 2,
        "bAlwaysEvaluateWorldPositionOffset": True, "bUseTranslucencyVertexFog": False})
    g = Graph(m, mpc)
    vs = r"""
float4 sc = mul(float4(normalize(SunD) * 1.0e7, 1.0), ResolvedView.TranslatedWorldToClip);
float2 ndc = sc.xy / max(sc.w, 1e-4);
float idx = round(VC * 5.0);
float k = (idx < 0.5) ? 0.55 : ((idx < 1.5) ? 0.2 : ((idx < 2.5) ? -0.25 : ((idx < 3.5) ? -0.5 : ((idx < 4.5) ? -0.85 : -1.2))));
float sz = (idx < 0.5) ? 0.05 : ((idx < 1.5) ? 0.025 : ((idx < 2.5) ? 0.08 : ((idx < 3.5) ? 0.035 : ((idx < 4.5) ? 0.12 : 0.06))));
float aspect = ResolvedView.ViewSizeAndInvSize.x * ResolvedView.ViewSizeAndInvSize.w;
float2 gpos = ndc * k + (UV - 0.5) * float2(2.0, -2.0) * sz * float2(1.0 / aspect, 1.0);
float zc = ResolvedView.NearPlane / 2000.0;
float4 p = mul(float4(gpos, zc, 1.0), ResolvedView.ClipToTranslatedWorld);
p.xyz /= p.w;
float3 target = (sc.w > 0.0) ? p.xyz : TWP;
return target - TWP;
"""
    v = g.custom("FlareGhostPlace", vs, [("SunD", g.mpc("SunDirection")), ("VC", g.vcolor("R")), ("UV", g.texcoord(0)),
                                         ("TWP", g.worldpos("WPT_CameraRelativeNoOffsets"))])
    g.out((v, ""), "MP_WorldPositionOffset")
    depth = g.node(unreal.MaterialExpressionSceneDepth)
    visv = g.custom("SunVisibilityVS", SUN_OCC + "VisNdcLen = float2(vis, length(ndc)); return float3(vis, ndc);",
                    [("SunD", g.mpc("SunDirection")), ("Dummy", (depth, ""))] + occ_inputs(g, alto_a, cir_a), "CMOT_Float3",
                    extra=[("VisNdcLen", "CMOT_Float2")])
    g.out((visv, "VisNdcLen"), "MP_CustomizedUVs1")
    ps = "float vis = VisUV.x; float2 ndc = float2(VisUV.y, 0.0);" + r"""
float dd = length(UV - 0.5) * 2.0;
float disc = 1.0 - smoothstep(0.82, 1.0, dd);
float ring = disc * (0.25 + 0.75 * smoothstep(0.35, 0.95, dd));
float idx = round(VC * 5.0);
float3 tint = (idx < 0.5) ? float3(0.4, 1.0, 0.9) : ((idx < 1.5) ? float3(1.0, 0.45, 0.8) : ((idx < 2.5) ? float3(1.0, 0.75, 0.35)
            : ((idx < 3.5) ? float3(0.5, 0.6, 1.0) : ((idx < 4.5) ? float3(0.35, 1.0, 0.6) : float3(1.0, 0.5, 0.4)))));
float centre = 1.0 - smoothstep(0.2, 1.0, length(ndc));
return tint * ring * vis * centre * Strength * 0.25 * (1.0 - saturate(LowQ)) * Bright;
"""
    c = g.custom("FlareGhostShade", ps, [("VC", g.vcolor("R")), ("UV", g.texcoord(0)),
                                         ("Strength", g.mpc("FlareStrength")), ("LowQ", g.mpc("LowQuality")),
                                         ("Bright", g.mpc("SkyBrightness")), ("VisUV", g.texcoord(1))])
    q = g.node(unreal.MaterialExpressionQualitySwitch)
    zero = g.node(unreal.MaterialExpressionConstant3Vector)
    g.link((c, ""), q, "Default")
    g.link((zero, ""), q, "Low")
    g.out((q, ""), "MP_EmissiveColor")
    compile_check(m)
    return m


def build_sea(mpc, ramp):
    m = reset_material("M_SeaBackdrop", "BLEND_Opaque")
    g = Graph(m, mpc)
    prep = r"""
float3 d = normalize(D);
float dep = max(-d.z, 0.0);
float elb = asin(saturate(dep));
return 1.0 - saturate(elb / radians(28.0));
"""
    p = g.custom("SeaDepthValue", prep, [("D", neg_camvec(g))], "CMOT_Float1")
    band = g.ramp((p, ""), ramp)
    shade = r"""
float3 d = normalize(D);
float3 S = normalize(SunD);
float dep = max(-d.z, 0.0);
float elb = asin(saturate(dep));
float t = saturate(elb / radians(28.0));
float3 col = lerp(Deep, HorC, Banded);
float2 dh = normalize(d.xy + float2(1e-5, 0.0));
float2 sh = normalize(S.xy + float2(1e-5, 0.0));
float dAz = acos(clamp(dot(dh, sh), -1.0, 1.0));
float w = 0.03 + 0.10 * t + 0.12 * (1.0 - S.z);
float path = exp(-pow(dAz / w, 2.0)) * smoothstep(0.0, 0.004, dep);
float farband = 1.0 - smoothstep(0.05, 0.16, dep);
float2 sp = d.xy / max(dep, 0.002);
float2 q = sp * float2(2.5, 8.0);
q.y += T * 0.35;
q.x += sin(q.y * 0.7 + T * 0.5) * 0.3;
float2 ci = floor(q);
float2 cf = frac(q);
float h = frac(sin(dot(ci, float2(127.1, 311.7))) * 43758.5453);
float dash = smoothstep(0.5, 0.15, abs(cf.x - 0.5)) * smoothstep(0.5, 0.3, abs(cf.y - 0.5));
float aa = saturate(1.5 - max(fwidth(q.x), fwidth(q.y)));
float sparkle = step(1.0 - 0.35 * path, h) * dash * aa * farband;
float2 q2 = sp * float2(0.9, 3.0) + float2(T * 0.03, T * 0.11);
float h2 = frac(sin(dot(floor(q2), float2(269.5, 183.3))) * 43758.5453);
float waves = step(0.93, h2) * smoothstep(0.5, 0.2, abs(frac(q2.x) - 0.5)) * saturate(1.5 - max(fwidth(q2.x), fwidth(q2.y))) * farband;
col += float3(1.0, 1.0, 1.0) * waves * 0.06 * (1.0 - path);
col += Glit * (sparkle * 1.6 + path * 0.35 * (0.35 + 0.65 * farband)) * Strength;
col = lerp(col, Haz, saturate(Haze) * exp(-elb / 0.02) * 0.8);
return col * Bright;
"""
    s = g.custom("SeaShade", shade, [("D", neg_camvec(g)), ("SunD", g.mpc("SunDirection")), ("Banded", band),
                                     ("Deep", g.mpc("SeaDeepColor")), ("HorC", g.mpc("SeaHorizonColor")),
                                     ("Glit", g.mpc("GlitterColor")), ("Strength", g.mpc("SeaGlitterStrength")),
                                     ("Haz", g.mpc("HazeColor")), ("Haze", g.mpc("Haze")), ("T", g.time()),
                                     ("Bright", g.mpc("SkyBrightness"))])
    g.out((s, ""), "MP_EmissiveColor")
    compile_check(m)
    return m


def build_horizon(mpc, ramp, hz_tex, alto_a):
    m = reset_material("M_HorizonBackdrop", "BLEND_Masked", extra={"OpacityMaskClipValue": 0.5, "DitherOpacityMask": True})
    g = Graph(m, mpc)
    prep = r"""
float3 d0 = normalize(WP - OP);
float u = atan2(d0.y, d0.x) * 0.15915494 + 0.5;
float eld = degrees(asin(clamp(d0.z, -1.0, 1.0)));
// a flat bay shore, as in the reference: above the horizon the baked hills are drawn at HillsH of their height
float eldT = (eld > 0.0) ? eld / max(HillsH, 0.05) : eld;
float2 uv = float2(u, saturate((6.5 - eldT) / 16.5));
float uB = frac(u + 0.5);
float2 dxA = ddx(uv), dyA = ddy(uv);
float2 dxB = float2(ddx(uB), dxA.y), dyB = float2(ddy(uB), dyA.y);
bool useB = (abs(dxA.x) + abs(dyA.x)) > (abs(dxB.x) + abs(dyB.x));
float4 s = Texture2DSampleGrad(Tex, TexSampler, uv, useB ? dxB : dxA, useB ? dyB : dyA);
G = s.g; B = s.b; A = s.a;
return s.r;
"""
    p = g.custom("HorizonPrep", prep, [("WP", g.worldpos()), ("OP", g.objpos()), ("Tex", g.tex_object("Horizon", hz_tex)),
                                       ("HillsH", g.mpc("HillsHeight"))],
                 "CMOT_Float1", extra=[("G", "CMOT_Float1"), ("B", "CMOT_Float1"), ("A", "CMOT_Float1")])
    # no cel bands or ink on the sky (owner, 14 Nov): the shade value goes straight in. The hills get aerial perspective
    # and the shadows of the 2.5 km cloud deck drifting across them (one mip-2 tap on a thin band of pixels).
    shade = r"""
float3 base = lerp(NearC, FarC, G);
base = lerp(base, SkyC, step(0.5, B));
float3 col = base * lerp(0.78, 1.0, Banded);
col = lerp(col, col * float3(0.86, 0.9, 1.08), (1.0 - Banded) * 0.6);
float px = (A - 0.5) / max(fwidth(A), 1e-5);
float coverage = saturate(px / max(EdgeSoft, 0.35) + 0.5);
float3 S = normalize(SunD);
float3 rl = WP - OP;
float2 wd = normalize(Wind.xy + float2(1e-5, 0.0));
float2 wq = float2(-wd.y, wd.x);
const float RcH = 3.0e7;   // the dome's curved shell, so each shadow lands under its own deck cloud (review finding)
float hH = max(250000.0 - rl.z, 20000.0);
float sH = sqrt(RcH * RcH * S.z * S.z + 2.0 * RcH * hH + hH * hH);
float2 ph = rl.xy + S.xy * ((2.0 * RcH * hH + hH * hH) / max(RcH * S.z + sH, 1.0));
float2 uvh = float2(dot(ph, wd) - T * Speed, dot(ph, wq)) / 2400000.0;
// the deck's opacity along the low sun's slanted path, as the dome and the sun occlusion use it (review)
float shd = saturate(Texture2DSampleLevel(AltoA, AltoASampler, uvh, 2.0).r);
shd = (1.0 - pow(1.0 - shd, clamp((RcH + hH) / max(sH, 1.0), 1.0, 6.0))) * @DECK(uvh);
col *= lerp(1.0, 0.72, shd);
col = lerp(col, col * float3(0.9, 0.92, 1.08), shd * 0.5);
col = lerp(col, HazC, saturate(Haze) * (0.5 + 0.4 * G));
Opacity = coverage;
return col * Bright;
"""
    s = g.custom("HorizonShade", COVER + with_fields(shade), [
                                         ("AltoCov", g.mpc("AltoCoverage")), ("CirCov", g.mpc("CirrusCoverage")),
                                         ("Banded", (p, "")), ("G", (p, "G")), ("B", (p, "B")), ("A", (p, "A")),
                                         ("NearC", g.mpc("HillNearColor")), ("FarC", g.mpc("HillFarColor")), ("SkyC", g.mpc("SkylineColor")),
                                         ("EdgeSoft", g.mpc("CloudEdgeSoftness")), ("Haze", g.mpc("Haze")), ("HazC", g.mpc("HazeColor")),
                                         ("WP", g.worldpos()), ("OP", g.objpos()), ("SunD", g.mpc("SunDirection")),
                                         ("Wind", g.mpc("WindDirection")), ("Speed", g.mpc("CloudDriftSpeed")), ("T", g.time()),
                                         ("AltoA", g.tex_object("AltoAlpha", alto_a)),
                                         ("Bright", g.mpc("SkyBrightness"))], extra=[("Opacity", "CMOT_Float1")])
    g.out((s, ""), "MP_EmissiveColor")
    g.out((s, "Opacity"), "MP_OpacityMask")
    compile_check(m)
    return m


BLIMP_VS = r"""
float3 local = WP - OP;
float th = T * Speed;
float psi = th + 1.5707963;
float cs = cos(psi), sn = sin(psi);
float3 rl = float3(local.x * cs - local.y * sn, local.x * sn + local.y * cs, local.z);
float3 rn = float3(N.x * cs - N.y * sn, N.x * sn + N.y * cs, N.z);
float3 centre = OP + float3(cos(th), sin(th), 0.0) * Radius + float3(0.0, 0.0, sin(T * 0.21) * 250.0);
float3 np = centre + rl;
float dist = length(np - CP);
float pxw = Push * dist * 2.0 * ResolvedView.ClipToView[1][1] * ResolvedView.ViewSizeAndInvSize.w;
np += normalize(rn) * pxw;
return np - WP;
"""


def blimp_vs(g, push_src):
    return g.custom("BlimpPath", BLIMP_VS, [("WP", g.worldpos("WPT_ExcludeAllShaderOffsets")), ("OP", g.objpos()),
                                            ("CP", g.campos()), ("N", g.normal()), ("T", g.time()),
                                            ("Speed", g.mpc("BlimpSpeed")), ("Radius", g.mpc("BlimpRadius")), ("Push", push_src)])


def build_blimp(mpc, ramp, mask_tex):
    m = reset_material("M_SemiCelBlimp", "BLEND_Opaque", extra={"MaxWorldPositionOffsetDisplacement": 300000.0,
                                                               "bAlwaysEvaluateWorldPositionOffset": True})
    g = Graph(m, mpc)
    zero = g.node(unreal.MaterialExpressionConstant)
    g.out((blimp_vs(g, (zero, "")), ""), "MP_WorldPositionOffset")
    lam = r"""
float psi = T * Speed + 1.5707963;
float cs = cos(psi), sn = sin(psi);
float3 n = normalize(float3(N.x * cs - N.y * sn, N.x * sn + N.y * cs, N.z));
return saturate(dot(n, normalize(SunD)) * 0.5 + 0.5);
"""
    p = g.custom("BlimpLambert", lam, [("N", g.normal()), ("T", g.time()), ("Speed", g.mpc("BlimpSpeed")),
                                       ("SunD", g.mpc("SunDirection"))], "CMOT_Float1")
    band = g.ramp((p, ""), ramp)
    t = g.tex_sample("BlimpMasks", mask_tex, g.texcoord(0))
    shade = r"""
float la = saturate((Ra - 0.5) / max(fwidth(Ra), 1e-4) + 0.5);   // crisp letters from the soft edge field
float ga = saturate((Ga - 0.5) / max(fwidth(Ga), 1e-4) + 0.5);
float3 base = lerp(Body, Accent, ga);
base = lerp(base, Letter, la);
float3 col = base * lerp(0.58, 1.0, Banded) * (0.85 + 0.15 * Ba);
col = lerp(col, col * float3(0.85, 0.9, 1.1), (1.0 - Banded) * 0.6);
float dist = length(WP - CP);
col = lerp(col, HazC, saturate(Haze) * (1.0 - exp(-dist / max(HazeDist, 1.0))) * 0.85);
return col * Bright;
"""
    s = g.custom("BlimpShade", shade, [("Banded", band), ("Ra", (t, "R")), ("Ga", (t, "G")), ("Ba", (t, "B")),
                                       ("Body", g.mpc("BlimpBodyColor")), ("Accent", g.mpc("BlimpAccentColor")),
                                       ("Letter", g.mpc("BlimpLetterColor")), ("WP", g.worldpos()), ("CP", g.campos()),
                                       ("HazC", g.mpc("HazeColor")), ("Haze", g.mpc("Haze")), ("HazeDist", g.mpc("HazeDistance")),
                                       ("Bright", g.mpc("SkyBrightness"))])
    g.out((s, ""), "MP_EmissiveColor")
    compile_check(m)
    return m


def build_blimp_outline(mpc):
    m = reset_material("M_InkOutlineBlimp", "BLEND_Opaque", extra={"MaxWorldPositionOffsetDisplacement": 300000.0,
                                                                  "bAlwaysEvaluateWorldPositionOffset": True})
    g = Graph(m, mpc)
    neg = g.node(unreal.MaterialExpressionMultiply)
    neg.set_editor_property("ConstB", -1.0)   # the ink hull's faces (and normals) are reversed, so push against them
    g.link(g.mpc("BlimpOutlineWidth"), neg, "A")
    g.out((blimp_vs(g, (neg, "")), ""), "MP_WorldPositionOffset")
    s = g.custom("InkShade", r"""
float dist = length(WP - CP);
float3 col = lerp(InkC, HazC, saturate(Haze) * (1.0 - exp(-dist / max(HazeDist, 1.0))) * 0.85);
return col * Bright;
""", [("InkC", g.mpc("OutlineColor")), ("WP", g.worldpos()), ("CP", g.campos()), ("HazC", g.mpc("HazeColor")),
      ("Haze", g.mpc("Haze")), ("HazeDist", g.mpc("HazeDistance")), ("Bright", g.mpc("SkyBrightness"))])
    g.out((s, ""), "MP_EmissiveColor")
    compile_check(m)
    return m


def build_gull(mpc, ramp):
    m = reset_material("M_SemiCelGull", "BLEND_Opaque", two_sided=True, extra={
        "bUsedWithInstancedStaticMeshes": True, "MaxWorldPositionOffsetDisplacement": 1500000.0,
        "bAlwaysEvaluateWorldPositionOffset": True})
    g = Graph(m, mpc)
    # the flock follows the car: it wheels over the 2.4 km cell nearest the camera (faded out at the cell edge so the
    # jump to the next cell is never seen), and gulls near the car flap harder and lift away as it passes
    vs = r"""
float3 local = WP - OP;
float dir = (C1 >= 0.0) ? 1.0 : -1.0;
float th = T * C1 + C2 * 6.2831853;
float L = 240000.0;
float2 cam = (CP.xy - OP.xy) / L;
float2 cell = round(cam);
float3 centre = OP + float3(cell * L, 0.0) + float3(cos(th), sin(th), 0.0) * C0 + float3(0.0, 0.0, sin(T * 0.5 + C2 * 9.0) * C3);
float2 fe = abs(cam - cell) * 2.0;
float keep = 1.0 - smoothstep(0.75, 1.0, max(fe.x, fe.y));
float k = smoothstep(6000.0, 1500.0, length(centre - CP));
// near the car the beat blends to a 2.5x faster stroke: two oscillators with fixed rates, so the phase never jumps
// (review: a distance-dependent rate times absolute time made the wings thrash whenever k changed)
float flap = lerp(sin(T * Flap + C2 * 40.0), sin(T * Flap * 2.5 + C2 * 40.0), k) * 0.55 * VC;
local.z += sin(flap) * abs(local.y) * 0.9;
local.y *= cos(flap);
float bank = dir * 0.35;
float cb = cos(bank), sb = sin(bank);
local = float3(local.x, local.y * cb - local.z * sb, local.y * sb + local.z * cb);
float psi = th + dir * 1.5707963;
float cs = cos(psi), sn = sin(psi);
float3 rl = float3(local.x * cs - local.y * sn, local.x * sn + local.y * cs, local.z);
rl *= keep;
float3 pos = centre + rl;
float3 aw = pos - CP;
float dd = length(aw);
pos += normalize(aw + float3(0.0, 0.0, dd * 0.6)) * k * 3000.0;
return pos - WP;
"""
    v = g.custom("GullFlight", vs, [("WP", g.worldpos("WPT_ExcludeAllShaderOffsets")), ("OP", g.objpos()), ("CP", g.campos()),
                                    ("C0", g.custom_data(0)), ("C1", g.custom_data(1)), ("C2", g.custom_data(2)),
                                    ("C3", g.custom_data(3)), ("T", g.time()), ("Flap", g.mpc("GullFlapSpeed")),
                                    ("VC", g.vcolor("R"))])
    g.out((v, ""), "MP_WorldPositionOffset")
    lam = g.custom("GullLight", "return saturate(abs(N.z) * 0.6 + 0.4 * saturate(dot(N, normalize(SunD))));",
                   [("N", g.normal()), ("SunD", g.mpc("SunDirection"))], "CMOT_Float1")
    band = g.ramp((lam, ""), ramp)
    s = g.custom("GullShade", r"""
float3 col = lerp(GullC, InkC * 1.6, saturate(Tip) * 0.85);
col *= lerp(0.7, 1.0, Banded);
float dist = length(WP - CP);
col = lerp(col, HazC, saturate(Haze) * (1.0 - exp(-dist / max(HazeDist, 1.0))) * 0.85);
return col * Bright;
""", [("GullC", g.mpc("GullColor")), ("InkC", g.mpc("OutlineColor")), ("Tip", g.vcolor("G")), ("Banded", band),
      ("WP", g.worldpos()), ("CP", g.campos()), ("HazC", g.mpc("HazeColor")), ("Haze", g.mpc("Haze")),
      ("HazeDist", g.mpc("HazeDistance")), ("Bright", g.mpc("SkyBrightness"))])
    g.out((s, ""), "MP_EmissiveColor")
    compile_check(m)
    return m


# ------------------------------------------------------------------------------------------------ instances
def instance(name, parent, textures=None):
    ensure_dir(INST)
    mi = load_or_create(INST, name, unreal.MaterialInstanceConstant, unreal.MaterialInstanceConstantFactoryNew())
    MEL.set_material_instance_parent(mi, parent)
    MEL.clear_all_material_instance_parameters(mi)   # drop overrides of parameters the master no longer has
    for pname, tex in (textures or {}).items():
        MEL.set_material_instance_texture_parameter_value(mi, pname, tex)
    MEL.update_material_instance(mi)
    save(mi)
    log("instance", name, "->", parent.get_name())
    return mi


def main():
    mpc = build_mpc()
    ramp = build_ramp(mpc)
    P = "Day"
    T = "/BayshoreSky/Textures/" + P
    atmo = unreal.load_asset("%s/T_BSky_%s_HorizonAtmo" % (T, P))
    hz = unreal.load_asset("%s/T_BSky_%s_Horizon" % (T, P))
    mask = unreal.load_asset("/BayshoreSky/Textures/T_BSky_Blimp_Masks")
    alto_l = unreal.load_asset("%s/T_BSky_%s_AltoLight" % (T, P))
    alto_a = unreal.load_asset("%s/T_BSky_%s_AltoAlpha" % (T, P))
    cir_l = unreal.load_asset("%s/T_BSky_%s_CirrusLight" % (T, P))
    cir_a = unreal.load_asset("%s/T_BSky_%s_CirrusAlpha" % (T, P))
    cum_sun = unreal.load_asset("%s/T_BSky_%s_CumulusSun" % (T, P))
    cum_aux = unreal.load_asset("%s/T_BSky_%s_CumulusAux" % (T, P))
    for n, x in (("AltoLight", alto_l), ("AltoAlpha", alto_a), ("CirrusLight", cir_l), ("CirrusAlpha", cir_a),
                 ("CumulusSun", cum_sun), ("CumulusAux", cum_aux), ("HorizonAtmo", atmo), ("Horizon", hz)):
        if x is None:
            raise RuntimeError("texture missing, run 21_import_textures.py first: " + n)
    built = {}
    built["dome"] = build_sky_dome(mpc, ramp, atmo, alto_l, alto_a, cir_l, cir_a)
    built["cloud"] = build_cloud(mpc, ramp, cum_sun, cum_aux)
    built["sun"] = build_sun_disc(mpc, ramp)
    built["glow"] = build_sun_glow(mpc, alto_a, cir_a)
    built["flare"] = build_flares(mpc, alto_a, cir_a)
    built["horizon"] = build_horizon(mpc, ramp, hz, alto_a)
    built["blimp"] = build_blimp(mpc, ramp, mask)
    built["outline"] = build_blimp_outline(mpc)
    built["gull"] = build_gull(mpc, ramp)
    instance("MI_SkyDome_%s" % P, built["dome"], {"Atmosphere": atmo, "AltoLight": alto_l, "AltoAlpha": alto_a,
                                                    "CirrusLight": cir_l, "CirrusAlpha": cir_a})
    instance("MI_SemiCelCloud_%s" % P, built["cloud"], {"CumulusSun": cum_sun, "CumulusAux": cum_aux})
    instance("MI_HorizonBackdrop_%s" % P, built["horizon"], {"Horizon": hz, "AltoAlpha": alto_a})
    instance("MI_SunDisc", built["sun"])
    instance("MI_SunGlow", built["glow"])
    instance("MI_FlareGhosts", built["flare"])
    instance("MI_SemiCelBlimp", built["blimp"])
    instance("MI_InkOutlineBlimp", built["outline"])
    instance("MI_SemiCelGull", built["gull"])
    log("materials done")


main()
