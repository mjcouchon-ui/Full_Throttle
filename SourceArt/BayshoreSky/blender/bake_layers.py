"""BayshoreSky periodic cloud LAYERS (Blender 5.2, Cycles GPU, headless): the altocumulus deck (2.5 km, 24 km tile) and
the cirrus (7 km, 48 km tile), rendered as a volumetric slab from BELOW with an orthographic camera looking straight up.

Run (the runner sets BelowNormal priority and the log folder; about 5 minutes on an RTX 4080 SUPER):
  powershell -NoProfile -File run_blender.ps1 -Script bake_layers.py -Log layers -PyArgs "--stages maps,render,compose"
The maps, quick and compose stages are pure NumPy and also run in Blender's Python:
  "<Blender>/5.2/python/bin/python.exe" -I bake_layers.py --stages maps [--layers cirrus] [--tag _x]
Stages:
  marker   one puff through the full map -> image texture -> render path; logs where it lands (expected U 0.30, V 0.70)
           and the azimuth of its sunlit side (expected theta: right and slightly down), plus the tau pass vs the
           analytic optical depth. Checked 14 Nov: (0.300, 0.700); lit side at 11 deg for theta 13.3 and 64 deg for 60.
  skycal   the sky model's sun-disc irradiance and sky radiance at each layer's altitude (gives CFG "sun_phys")
  maps     2D periodic NumPy maps -> RAW/<layer>_map<tag>.exr (R cloud base z, G cloud top z, B density x, A unused),
           plus a quick look in bakes/Day/raw (layers_map_<layer><tag>.png) and coverage / optical depth statistics
  quick    look-dev only: Light / Alpha textures from the maps with a cheap shading proxy into --out, so
           preview_layers.py can judge coverage and shapes in perspective before a Cycles render (not the bake's light)
  render   Cycles render of the slab: light groups "sun" (white Sun lamp) and "sky" (multiple-scattering sky, no disc,
           strength 1), then the optical-depth pass (emission = extinction, so each camera ray integrates tau); a JSON
           sidecar records the lamp
  compose  textures in bakes/Day: T_BSky_Day_<Alto|Cirrus>Light.exr (R sunY, G skyY per unit opacity, B tauN, half) and
           T_BSky_Day_<Alto|Cirrus>Alpha.png (8-bit grey opacity = 1 - exp(-tau), straight), layers_meta.json. The sun
           light group is rescaled from the rendered lamp to the shipped sun factor (SUN_SHIP); hot thin texels are
           clamped; the deck's B is its dilated phase-blend depth (B_DILATE); recommended values from --rec_exposure /
           --rec_tauscale (defaults: the round-2 choices)
Round 2 (14 Nov, after review): sparse cirrus brush-stroke bundles, scattered small-celled altocumulus, one sun factor,
one tauScaleRef (10) for both layers, so one runtime Exposure and one TauScale serve both.

Tile frame: Blender x = U * T, y = V * T (metres), z = altitude. Texture column = U, row = V (row 0 at the top), so the
camera (rotation 180 deg about X) shows +x to the right and +y downward. The sun in tile space is at azimuth theta from
+U toward +V, elevation 6 deg, so the lit side of every cloud faces right and slightly down in the image.
Clouds are 2.5D height fields: per texel a base and a top height and a density, smoothstep-ramped over 'soft' metres
inside the base and top. The density is periodic in U and V with period one tile (the maps tile through REPEAT image
lookups), and the slab box reaches 'margin' tiles past the tile on every side, so the 6 deg sun, which crosses the slab
almost horizontally, sees an endless layer. Optional sub-texel 3D detail (--detail 1) is 4D noise on two circles,
(Ru cos 2piU, Ru sin 2piU, Rv cos 2piV + kz z, Rv sin 2piV), periodic for any z; it is off by default (4x the render time).
Phase: 0.7 HG(0.8) + 0.3 HG(-0.2) (two Volume Scatter closures), the runtime's own HG2, so thin parts baked at the
vertical view scale exactly by HG2(c) / HG2(cBake) in the game.
"""
import sys
import os
import math
import json
import time

import numpy as np
import OpenImageIO as oiio
try:                       # the map and compose stages also run in Blender's bundled Python (python.exe -I)
    import bpy
    from mathutils import Vector
except ImportError:
    bpy = None

    class Vector(tuple):
        def __new__(cls, v):
            return tuple.__new__(cls, v)
        x = property(lambda s: s[0])
        y = property(lambda s: s[1])
        z = property(lambda s: s[2])

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))   # SourceArt/BayshoreSky (this script is in blender/)
OUT = os.path.join(ROOT, "bakes", "Day")
# working files (raw renders and maps in raw/layers, quick-look images in raw) live with the bakes, so a compose-only
# rerun works on any machine (review: they were in a session's temporary folder). The shipped round is tag _r2f.
SCR = os.path.join(OUT, "raw")

ARGV = sys.argv[sys.argv.index("--") + 1:] if "--" in sys.argv else (sys.argv[1:] if bpy is None else [])


def arg(name, default):
    key = "--" + name
    if key in ARGV:
        return type(default)(ARGV[ARGV.index(key) + 1])
    return default


STAGES = arg("stages", "maps,render,compose").split(",")
LAYERS = arg("layers", "alto,cirrus").split(",")
RES = arg("res", 2048)            # render resolution (the shipped textures are 2048)
MAPRES = arg("mapres", 2048)      # 2D map resolution
SPP = arg("spp", 0)               # 0 = per layer (CFG "spp")
TAU_SPP = arg("tauspp", 48)
BIASED = arg("biased", 0)
BOUNCES = arg("bounces", 16)
WALLS = arg("walls", 1)
MARGIN = arg("margin", 0.0)       # >0 overrides CFG "margin" (tiles past the tile on every side)
TAG = arg("tag", "")              # suffix for raw files (test rounds)
OUT = arg("out", OUT)             # where compose writes the textures and layers_meta.json
MAPTAG = arg("maptag", TAG)       # suffix of the map the render stage reads ("none" = no suffix)
MAPTAG = "" if MAPTAG == "none" else MAPTAG
RAW = arg("raw", os.path.join(SCR, "layers"))
SEED = arg("seed", 7)
SUN_EL = 6.0
SUN_STRENGTH = arg("sun", 0.0)    # white Sun lamp irradiance; 0 = sun_phys x TINT_BALANCE per layer
# The runtime tints the sun term with SunTint (luminance 0.9189) and the sky term with AmbTint (0.2267). The lamp is the
# sky model's own sun irradiance at the layer (skycal stage) times lum(AmbTint) / lum(SunTint), so after the tints the
# sun / sky luminance balance in the game equals the physical balance of the sky model.
TINT_BALANCE = 0.2267 / 0.9189
# Used: SUN_FACTOR = 0.5 (between the tint balance 0.247 and the plain physical sun 1.0): with it the shaded cores
# read mauve-pink (sun and sky terms comparable, as in the reference) and the lit / core contrast stays near 8:1.
SUN_FACTOR = arg("sunfactor", 0.5)
CLAMP_IND = arg("clampind", 0.0)  # Cycles sample_clamp_indirect for the layer renders (0 = off; see setup_render)
os.makedirs(RAW, exist_ok=True)
os.makedirs(OUT, exist_ok=True)

T0 = time.time()


def log(*a):
    print("BSKYB [%6.1fs]" % (time.time() - T0), *a, flush=True)


# ------------------------------------------------------------------------------------------------ layer configs
# theta: sun azimuth in tile space, from +U toward +V (deg). z0..z1: slab (m). sigma: extinction (1/m) at map density 1.
CFG = {
    "alto": dict(name="Alto", T=24000.0, theta=13.30, alt=2500.0, sun_phys=69.9, z0=2280.0, z1=2820.0, sigma=0.017, margin=1.0, spp=256,
                 # 3D billow noise: features per tile along U and V at scale 1, vertical features per metre
                 nU=150.0, nV=190.0, kz=1.0 / 70.0, amp1=55.0, amp2=22.0, soft=30.0),
    "cirrus": dict(name="Cirrus", T=48000.0, theta=28.30, alt=7000.0, sun_phys=91.6, z0=6650.0, z1=7350.0, sigma=0.004, margin=1.0, spp=512,
                   nU=26.0, nV=420.0, kz=1.0 / 90.0, amp1=60.0, amp2=25.0, soft=40.0),
}
HG2 = ((0.7, 0.8), (0.3, -0.2))


def sun_dir(theta_deg, el_deg=SUN_EL):
    t, e = math.radians(theta_deg), math.radians(el_deg)
    return Vector((math.cos(e) * math.cos(t), math.cos(e) * math.sin(t), math.sin(e)))


# ------------------------------------------------------------------------------------------------ image io (bake_sky.py)
def oiio_write(path, data, fmt, attrs=(), names=None):
    data = np.ascontiguousarray(data)
    if data.ndim == 2:
        data = data[:, :, None]
    h, w, c = data.shape
    spec = oiio.ImageSpec(w, h, c, fmt)
    if names:
        spec.channelnames = tuple(names)
    for k, v in attrs:
        spec.attribute(k, v)
    out = oiio.ImageOutput.create(path)
    if out is None or not out.open(path, spec):
        raise RuntimeError("cannot write %s: %s" % (path, oiio.geterror()))
    if not out.write_image(data):
        raise RuntimeError("write failed %s: %s" % (path, out.geterror()))
    out.close()


def write_exr(path, data, half=True, names=None):
    oiio_write(path, data.astype(np.float32), "half" if half else "float", attrs=[("compression", "zip")], names=names)


def read_exr_all(path):
    inp = oiio.ImageInput.open(path)
    if inp is None:
        raise RuntimeError("cannot open %s: %s" % (path, oiio.geterror()))
    out = {}
    sub = 0
    while inp.seek_subimage(sub, 0):
        spec = inp.spec()
        px = np.asarray(inp.read_image(sub, 0, 0, spec.nchannels, "float")).reshape(spec.height, spec.width, spec.nchannels)
        for i, n in enumerate(spec.channelnames):
            out[n] = px[:, :, i].astype(np.float32)
        sub += 1
    inp.close()
    return out


def ch(d, pass_name, c):
    suffix = "." + pass_name + "." + c
    for k, v in d.items():
        if k.endswith(suffix) or k == pass_name + "." + c:
            return v
    raise KeyError("no channel %s.%s in %s" % (pass_name, c, sorted(d.keys())[:40]))


def lum(rgb):
    return rgb[..., 0] * 0.2126 + rgb[..., 1] * 0.7152 + rgb[..., 2] * 0.0722


# ------------------------------------------------------------------------------------------------ periodic noise (bake_deck.py)
class PNoise:
    """Periodic gradient noise and Worley on wrapped integer lattices (bake_deck.py's pnoise / pfbm / worley_f1)."""

    def __init__(self, seed):
        rng = np.random.default_rng(seed)
        self.perm = rng.permutation(1024).astype(np.int32)
        ang = rng.uniform(0.0, 2.0 * np.pi, 1024)
        self.grad = np.stack([np.cos(ang), np.sin(ang)], -1).astype(np.float32)
        self.jit = rng.uniform(0.0, 1.0, (1024, 2)).astype(np.float32)
        self.size = rng.uniform(0.55, 1.35, 1024).astype(np.float32)
        self.rnd = rng.uniform(0.0, 1.0, 1024).astype(np.float32)

    def hash(self, i, j, px, py):
        return self.perm[(self.perm[(i % px) & 1023] + (j % py)) & 1023]

    def noise(self, x, y, px, py):
        """Gradient noise, lattice wraps every px cells in x and py in y, about -1..1."""
        xf, yf = np.floor(x), np.floor(y)
        ix, iy = xf.astype(np.int32), yf.astype(np.int32)
        fx, fy = (x - xf).astype(np.float32), (y - yf).astype(np.float32)
        ux = fx * fx * fx * (fx * (fx * 6 - 15) + 10)
        uy = fy * fy * fy * (fy * (fy * 6 - 15) + 10)

        def g(di, dj):
            gv = self.grad[self.hash(ix + di, iy + dj, px, py)]
            return gv[..., 0] * (fx - di) + gv[..., 1] * (fy - dj)
        n00, n10, n01, n11 = g(0, 0), g(1, 0), g(0, 1), g(1, 1)
        a0 = n00 + (n10 - n00) * ux
        a1 = n01 + (n11 - n01) * ux
        return ((a0 + (a1 - a0) * uy) * 1.4).astype(np.float32)

    def fbm(self, u, v, fx, fy, octaves, gain=0.5, off=0.0, ridged=False):
        """Periodic fbm over the unit tile, about 0..1. fx, fy integer lattice cells across the tile (first octave)."""
        s = np.zeros(u.shape, np.float32)
        amp, norm = 1.0, 0.0
        for o in range(octaves):
            n = self.noise(u * fx + off + o * 7.0, v * fy - off + o * 3.0, fx, fy)
            if ridged:
                n = (1.0 - np.abs(n)) * 2.0 - 1.0
            s += amp * n
            norm += amp
            amp *= gain
            fx, fy = fx * 2, fy * 2
        return s / norm * 0.5 + 0.5

    def worley(self, u, v, cx, cy, jitter=0.85, smooth=0.0, sized=True):
        """Periodic Worley F1 (cell units, divided by each point's size) and the winning point's random id.
        smooth > 0 blends neighbours (smooth minimum) so adjacent puffs merge."""
        x, y = u * cx, v * cy
        ix, iy = np.floor(x).astype(np.int32), np.floor(y).astype(np.int32)
        best = np.full(u.shape, 9.0, np.float32)
        bid = np.zeros(u.shape, np.int32)
        acc = np.zeros(u.shape, np.float32) if smooth > 0 else None
        for dj in (-1, 0, 1):
            for di in (-1, 0, 1):
                hh = self.hash(ix + di, iy + dj, cx, cy) ^ 517
                jx = self.jit[hh]
                px = ix + di + 0.5 + jitter * (jx[..., 0] - 0.5)
                py = iy + dj + 0.5 + jitter * (jx[..., 1] - 0.5)
                d = np.sqrt((x - px) ** 2 + (y - py) ** 2)
                if sized:
                    d = d / self.size[hh]
                closer = d < best
                bid = np.where(closer, hh, bid)
                best = np.minimum(best, d)
                if acc is not None:
                    acc += np.exp(-d / smooth)
        if acc is not None:
            best = -smooth * np.log(np.maximum(acc, 1e-12))
        return best.astype(np.float32), bid


def sstep(e0, e1, x):
    t = np.clip((x - e0) / (e1 - e0), 0.0, 1.0)
    return t * t * (3 - 2 * t)


def tile_grid(n):
    c = ((np.arange(n) + 0.5) / n).astype(np.float32)
    U, V = np.meshgrid(c, c)   # U = columns, V = rows (row 0 at the top)
    return U, V


# ------------------------------------------------------------------------------------------------ maps
# Map channels (float EXR, periodic): R = cloud base z (m), G = cloud top z (m), B = density multiplier (0..1.5),
# A = billow amplitude scale (0..1). A texel is empty where top <= base.
def maps_marker(n):
    cfg = CFG["alto"]
    U, V = tile_grid(n)
    du = (U - 0.30 + 0.5) % 1.0 - 0.5
    dv = (V - 0.70 + 0.5) % 1.0 - 0.5
    r = np.sqrt(du * du + dv * dv) / 0.05
    thick = np.where(r < 1, 320.0 * np.sqrt(np.clip(1 - r * r, 0, 1)), 0.0).astype(np.float32)
    zb = np.full_like(thick, 2450.0) - 0.2 * thick
    zt = zb + thick
    k = np.ones_like(thick)
    amp = np.zeros_like(thick)
    return np.stack([zb, zt, k, amp], -1).astype(np.float32)


def zs(a):
    return ((a - a.mean()) / (a.std() + 1e-9)).astype(np.float32)


def blur_wrap(a, sigma):
    """Gaussian blur on the periodic tile (three wrapped box passes per axis)."""
    if sigma <= 0:
        return a
    r = max(1, int(round(math.sqrt(12.0 * sigma * sigma / 3.0 + 1.0) / 2.0)))
    out = a.astype(np.float32)
    for _ in range(3):
        for ax in (0, 1):
            c = np.cumsum(np.concatenate([np.take(out, np.arange(-r - 1, 0), axis=ax), out,
                                          np.take(out, np.arange(0, r), axis=ax)], axis=ax), axis=ax, dtype=np.float64)
            n = out.shape[ax]
            out = ((np.take(c, np.arange(2 * r + 1, 2 * r + 1 + n), axis=ax) - np.take(c, np.arange(0, n), axis=ax))
                   / (2 * r + 1)).astype(np.float32)
    return out


def smear_u(a, decay_px, direction=1):
    """One-sided exponential smear along U (columns), periodic, via FFT: a cloud edge trails downwind (+1) or upwind."""
    n = a.shape[1]
    j = np.arange(n)
    k = np.exp(-j / float(decay_px))
    k /= k.sum()
    if direction < 0:
        k = np.roll(k[::-1], 1)
    return np.fft.irfft(np.fft.rfft(a, axis=1) * np.fft.rfft(k)[None, :], n=n, axis=1).astype(np.float32)


def alto_presence(field, thr, env_sigma, env_min, open_sigma):
    """Cloud where field > thr, without specks and small isolated cloudlets: an opening (blur the presence and
    re-threshold) removes features narrower than about 2 open_sigma texels, and a cloud texel must sit in a group
    envelope (the presence blurred over env_sigma texels) above env_min."""
    pres = (field > thr).astype(np.float32)
    pres = (blur_wrap(pres, open_sigma) > 0.5).astype(np.float32)
    env = blur_wrap(pres, env_sigma)
    keep = (pres > 0.5) & (env > env_min)
    return keep, env


def maps_alto(n, seed, coverage=0.18):
    """Scattered altocumulus as a 2.5D height field (review round 2): compact groups (~8 x 2.7 km, longer along the wind
    U, 8-12 per tile) of small cells (~240 x 160 m, 4-5 deg overhead) that merge in a group's core and break up into
    separate cloudlets at its edges, with a weak tendency to rows along U, wide clear gaps between groups, frayed edges
    streaked along U, thin flat cloudlets (optical depth median ~1, cores up to ~3) whose thickness grows linearly from
    the outline (no uniform rim band), undulating bases, lumpy tops."""
    pn = PNoise(seed * 101 + 1)
    t0 = time.time()
    U, V = tile_grid(n)
    mpp = CFG["alto"]["T"] / n
    # meanders (km scale) shared by every scale of detail
    wu = (pn.fbm(U, V, 3, 4, 3, off=11.0) - 0.5) * 0.05
    wv = (pn.fbm(U, V, 4, 3, 3, off=23.0) - 0.5) * 0.05
    Uw, Vw = U + wu, V + wv
    # groups: ~8 km along U x 2.7 km across
    P = zs(pn.fbm(Uw, Vw, 3, 9, 5, 0.6, off=3.0))
    # cells ~240 x 160 m (smoothly merging Worley), lumps ~100 x 75 m, rows along U ~800 m apart
    jw = (pn.fbm(U, V, 24, 24, 2, off=41.0) - 0.5) * 0.004
    C1 = zs(1.0 - pn.worley(Uw, Vw, 100, 150, jitter=0.9, smooth=0.08)[0])
    L2 = zs(1.0 - pn.worley(Uw + jw, Vw + jw, 250, 320, jitter=0.9, smooth=0.06)[0])
    roll = np.cos(2 * np.pi * (Vw * 30 + 1.4 * pn.fbm(U, V, 3, 5, 2, off=51.0))).astype(np.float32)
    # frayed edges: streaks along U (~2.4 km x 340 m), only felt near the outline where the field is close to the threshold
    fray = zs(pn.fbm(Uw, Vw + 2 * jw, 10, 70, 3, 0.55, off=61.0))
    field = 2.2 * P + 0.7 * C1 + 0.15 * roll + 0.2 * L2 + arg("al_fray", 0.25) * fray
    field = blur_wrap(field, 0.6)
    env_sigma = 300.0 / mpp
    open_sigma = arg("al_open", 3.0)
    # threshold so the kept cloud covers the target (bisection: the opening and the envelope remove some cloud)
    target = coverage
    lo, hi = float(np.quantile(field, 1.0 - target * 2.0)), float(np.quantile(field, 1.0 - target * 0.8))
    for _ in range(10):
        mid = 0.5 * (lo + hi)
        keep, env = alto_presence(field, mid, env_sigma, 0.22, open_sigma)
        if keep.mean() > target:
            lo = mid
        else:
            hi = mid
    thr = 0.5 * (lo + hi)
    keep, env = alto_presence(field, thr, env_sigma, 0.22, open_sigma)
    e = np.where(keep, np.maximum(field - thr, 1e-3), 0.0)
    # thickness grows linearly from the outline (a sqrt dome gave every cloudlet the same thin, bright rim band), more
    # in a group's core, less in the lanes between cells, lumpy on top
    eref = float(np.quantile(e[e > 0], 0.95))
    s = np.clip(e / eref, 0, 1.4)
    pat = sstep(-0.6, 2.0, P)
    lanes = 0.35 + 0.65 * sstep(-1.4, 0.4, C1)
    dome = arg("al_dome", 200.0) * s * (0.5 + 0.5 * pat) * lanes
    lumps = 1.0 + 0.15 * np.clip(L2, -2, 2)
    fine = 1.0 + 0.10 * (pn.fbm(U, V, 200, 200, 3, 0.55, off=83.0) - 0.5) * 2.0
    cap = arg("al_cap", 240.0)                                       # soft cap: flat cores, optical depth p99 ~3.5
    thick = np.where(keep, cap * np.tanh(dome * lumps * fine / cap), 0.0).astype(np.float32)
    # base: undulating (+-70 m over km), slightly convex under thick cores; top = base + thickness
    zbase = 2450.0 + 140.0 * (pn.fbm(U, V, 3, 3, 3, off=71.0) - 0.5) + 30.0 * (pn.fbm(U, V, 20, 20, 2, off=73.0) - 0.5)
    zb = zbase - 0.22 * thick + 5.0 * np.clip(L2, -2, 2) * sstep(40.0, 150.0, thick)
    zt = zb + thick
    # density multiplier: cores a little denser, varied
    k = (0.6 + 0.5 * sstep(0.0, 0.5, s)) * (0.92 + 0.2 * pn.fbm(U, V, 30, 30, 2, off=81.0))
    empty = thick <= 0
    zb = np.where(empty, 2450.0, zb)
    zt = np.where(empty, 2450.0 - 60.0, zt)                         # empty: top below base
    m = np.stack([zb, zt, k, np.zeros_like(thick)], -1).astype(np.float32)
    log("maps alto %d^2 (%.1f m/px) in %.1fs: kept %.3f, thickness max %.0f m, mean over cloud %.0f m" % (
        n, mpp, time.time() - t0, float(keep.mean()), float(thick.max()), float(thick[keep].mean())))
    return m


def ridge(n, p):
    return (1.0 - np.abs(n)) ** p


def contour_fibres(pn, U, psi, p_x, p_y, N, n, widths, bright, env, lev, seed, jit=0.35, taper=0.10, jn=6, reach=1):
    """Fibres along the contour lines psi = (k + 0.5 + j_k) / N of the stream function psi = V + p (p periodic): the
    lines never cross or loop, they bunch together where psi changes slowly across them and spread elsewhere. Each
    line k has its own level offset j_k (breaks the regular spacing), width (texels) and brightness, and exists where
    the bundle envelope 'env' (plus a little per-line noise along U) is above its own threshold e_k drawn from 'lev':
    in a bundle's core every fibre is there, toward its ends and sides fewer and fewer, so each bundle is a combed
    brush stroke whose fibres end one by one (frayed, tapered ends). p_x, p_y: d p / d column and d p / d row (per
    texel). Returns the summed density of the 2 reach + 1 nearest lines."""
    rng = np.random.default_rng(seed)
    w_k = rng.uniform(widths[0], widths[1], N).astype(np.float32)
    b_k = (rng.uniform(bright[0], bright[1], N) * (rng.random(N) < 0.8)).astype(np.float32)
    j_k = rng.uniform(-jit, jit, N).astype(np.float32)
    e_k = rng.uniform(lev[0], lev[1], N).astype(np.float32)
    o1 = rng.uniform(0, 1000, N).astype(np.float32)
    o2 = rng.uniform(0, 64, N).astype(np.float32)
    o3 = rng.uniform(0, 1000, N).astype(np.float32)
    grad = np.sqrt(p_x * p_x + (1.0 / n + p_y) ** 2)            # |grad psi| per texel
    x = psi * N
    k0 = np.floor(x).astype(np.int64)
    out = np.zeros_like(psi)
    for dk in range(-reach, reach + 1):
        k = k0 + dk
        km = k % N
        dist = np.abs(x - (k + 0.5 + j_k[km])) / (N * grad)      # texels to the line centre
        prof = 0.72 * np.exp(-(dist / w_k[km]) ** 2) + 0.28 * np.exp(-(dist / (2.6 * w_k[km])) ** 2)
        live = prof > 1e-3
        if not live.any():
            continue
        u, kk = U[live], km[live]
        # where this fibre exists: the envelope with a little per-line wobble along U (ends do not line up)
        wob = 0.65 * pn.noise(u * jn + o1[kk], o2[kk], jn, 64) + 0.35 * pn.noise(u * jn * 2 + o3[kk], o2[kk] + 7, jn * 2, 64)
        seg = sstep(e_k[kk], e_k[kk] + taper, env[live] + 0.035 * wob)
        # brightness varies along the fibre (~3 km and ~1.3 km)
        along = 0.45 + 0.55 * sstep(-0.6, 0.6, 0.6 * pn.noise(u * 16 + o3[kk], o2[kk] + 19, 16, 64)
                                    + 0.4 * pn.noise(u * 37 + o1[kk], o2[kk] + 33, 37, 64))
        out[live] += prof[live] * b_k[kk] * seg * along
    return out


def maps_cirrus(n, seed):
    """Cirrus (review round 2): sparse, separate brush-stroke bundles (a few km to ~12 km along U, 1-3 km across) of
    soft, silky ice fibres combed along U, with wide clear-blue gaps and regions of open sky between them. Fibres are
    contour lines of a stream function (they never loop or cross) at an irregular spacing, 70-120 m wide (fine ones
    33-50 m), bending smoothly over 5-20 km; each fibre exists above its own level of the bundle envelope, so a
    bundle's core holds all of them and its ends and sides fray out fibre by fibre; a soft streaky haze fills each
    bundle's body, and rare faint veils; translucent (opacity p90 ~0.53)."""
    pn = PNoise(seed * 103 + 5)
    t0 = time.time()
    U, V = tile_grid(n)
    # stream function psi = V + p(U, V): only the large, smooth bends (no small meanders: those made the fibres wiggle
    # together like a grating), p gentle enough that psi grows monotonically across the flow
    # (the 2-8 km bends are kept small: a bundle seen receding overhead turned them into a wavy curtain)
    p = (0.060 * (pn.fbm(U, V, 2, 2, 3, 0.5, off=17.0) - 0.5) + 0.016 * (pn.fbm(U, V, 3, 8, 2, 0.5, off=19.0) - 0.5)
         + 0.008 * (pn.fbm(U, V, 6, 5, 2, 0.5, off=23.0) - 0.5)).astype(np.float32)
    p_x = (np.roll(p, -1, 1) - np.roll(p, 1, 1)) * 0.5
    p_y = (np.roll(p, -1, 0) - np.roll(p, 1, 0)) * 0.5
    psi = V + p
    mono = float(((1.0 / n + p_y) <= 0).mean())
    # regions where cirrus is frequent (wide clear sky elsewhere), and the bundle envelope along the stream lines:
    # bundles are where it is high (~12 km cells along U, ~3.4 km across), lowered outside the regions
    reg = sstep(arg("ci_reg", 0.45), arg("ci_reg", 0.45) + 0.20, pn.fbm(U, V, 3, 6, 3, off=7.0))
    env = pn.fbm(U, psi, 4, 14, 3, 0.5, off=5.0) - arg("ci_regdrop", 0.16) * (1.0 - reg)
    l0 = arg("ci_lane", 0.43)                # tuned 14 Nov: opacity > 0.1 on ~15 % of the tile, > 0.02 on ~24 %
    # coarse fibres (~600 m apart on average, 70-120 m wide) and fine fibres (~220 m apart, 33-50 m wide); each exists
    # above its own envelope level, so a bundle's core holds all of them and its ends fray out fibre by fibre; a soft
    # streaky haze fills the bundle's body, so it reads as a feathered brush stroke rather than one bright line
    f1 = contour_fibres(pn, U, psi, p_x, p_y, 80, n, (3.0, 5.0), (0.5, 1.0), env, (l0, l0 + 0.12), seed * 7 + 1, reach=2)
    p2 = p + 0.0012 * (pn.fbm(U, V, 8, 40, 2, 0.5, off=27.0) - 0.5)
    p2x = (np.roll(p2, -1, 1) - np.roll(p2, 1, 1)) * 0.5
    p2y = (np.roll(p2, -1, 0) - np.roll(p2, 1, 0)) * 0.5
    f2 = contour_fibres(pn, U, V + p2, p2x, p2y, 220, n, (1.4, 2.2), (0.12, 0.35), env, (l0, l0 + 0.14), seed * 7 + 2)
    fib = f1 + f2
    streak = pn.fbm(U, psi, 12, 420, 3, 0.55, off=59.0)
    haze = arg("ci_haze", 0.25) * sstep(l0 + 0.01, l0 + 0.15, env) * sstep(0.25, 0.80, streak) * (0.6 + 0.8 * pn.fbm(U, psi, 6, 40, 2, off=61.0))
    D = fib + haze
    # rare faint veils (cirrostratus-like) in the cirrus regions, streaky along the flow
    veil = sstep(0.55, 0.82, pn.fbm(U, psi, 3, 14, 4, 0.55, off=91.0)) * reg * (0.6 + 0.8 * pn.fbm(U, psi, 12, 300, 2, off=93.0))
    D = np.maximum(D, arg("ci_veil", 0.02) * veil)
    D = np.clip(D / float(np.quantile(D[D > 0.02], 0.995)), 0, 1.15).astype(np.float32)
    thick = np.where(D > 0.008, 160.0 + 340.0 * np.sqrt(D), 0.0).astype(np.float32)
    zc = 7000.0 + 180.0 * (pn.fbm(U, V, 2, 4, 3, off=71.0) - 0.5)
    zb = zc - thick / 2
    zt = np.where(thick > 0, zc + thick / 2, zc - 50.0)
    m = np.stack([zb, zt, D * arg("ci_dens", 1.1), np.zeros_like(D)], -1).astype(np.float32)   # p90 opacity ~0.53
    log("maps cirrus %d^2 in %.1fs (stream function non-monotonic texels: %.5f)" % (n, time.time() - t0, mono))
    return m


def save_map(path, m):
    write_exr(path, m, half=False, names=("R", "G", "B", "A"))


def map_preview(path, m, cfg):
    """Quick look at a map (no lighting): left, the tile 2x2 at quarter size (opacity); right, a full-size 1024 crop
    shaded by thickness (thick = darker) over the opacity."""
    tau = ramp_tau(m, cfg)
    a = 1.0 - np.exp(-tau)
    n = a.shape[0]
    q = max(1, n // 512)
    h = a[::q, ::q][:512, :512]
    t = np.concatenate([np.concatenate([h, h], 1), np.concatenate([h, h], 1)], 0)
    sky = np.array([0.19, 0.29, 0.58], np.float32)
    left = sky * (1 - t[..., None]) + np.array([0.95, 0.75, 0.75], np.float32) * t[..., None]
    c = min(1024, n)
    ac = a[:c, :c]
    thick = np.clip(m[:c, :c, 1] - m[:c, :c, 0], 0, None)
    shade = np.exp(-thick / max(1.0, float(np.quantile(thick[thick > 0], 0.9)) if (thick > 0).any() else 1.0))
    cl = np.stack([0.95 * (0.45 + 0.55 * shade), 0.75 * (0.45 + 0.55 * shade), 0.78 * (0.5 + 0.5 * shade)], -1)
    right = sky * (1 - ac[..., None]) + cl * ac[..., None]
    img = np.zeros((max(1024, c), 1024 + c, 3), np.float32)
    img[:1024, :1024] = left
    img[:c, 1024:1024 + c] = right
    oiio_write(path, (np.clip(img, 0, 1) ** (1 / 2.2) * 255 + 0.5).astype(np.uint8), "uint8")
    return a


def map_tau(m, sigma):
    """Optical depth of the map alone (no 3D detail): sigma * density * (top - base)."""
    return sigma * np.clip(m[..., 2], 0, None) * np.clip(m[..., 1] - m[..., 0], 0, None)


# ------------------------------------------------------------------------------------------------ scene helpers (bake_sky.py)
def new_scene(name):
    if name in bpy.data.scenes:
        bpy.data.scenes.remove(bpy.data.scenes[name])
    sc = bpy.data.scenes.new(name)
    sc.world = bpy.data.worlds.new(name + "_W")
    return sc


def enable_gpu(sc):
    prefs = bpy.context.preferences.addons["cycles"].preferences
    prefs.compute_device_type = "OPTIX"
    prefs.refresh_devices()
    for d in prefs.devices:
        d.use = (d.type == "OPTIX")
    sc.render.engine = "CYCLES"
    sc.cycles.device = "GPU"


def setup_render(sc, w, h, samples, path):
    enable_gpu(sc)
    r = sc.render
    r.resolution_x, r.resolution_y, r.resolution_percentage = w, h, 100
    r.film_transparent = True
    r.use_file_extension = False
    r.filepath = path
    r.image_settings.media_type = "MULTI_LAYER_IMAGE"
    r.image_settings.file_format = "OPEN_EXR_MULTILAYER"
    r.image_settings.color_depth = "32"
    r.image_settings.exr_codec = "ZIP"
    sc.view_settings.view_transform = "Standard"
    sc.view_settings.look = "None"
    c = sc.cycles
    c.samples = max(1, int(samples))
    c.use_adaptive_sampling = True
    c.adaptive_threshold = 0.004
    c.use_denoising = False
    c.pixel_filter_type = "BOX"
    c.filter_width = 1.0
    # indirect clamp, off by default: tested 14 Nov on the round-2 deck (512 px, 128 spp) against no clamp, it removed
    # 31 % of the mean sun light at 8, 17 % at 30 and 7.5 % at 100 (mostly legitimate bright samples of the g = 0.8
    # forward lobe on the sunlit sides; a second-bounce sample toward the sun carries ~E x p(0) ~ 90). Hot texels are
    # handled in compose instead (firefly pass and the thin-texel clamp), which leaves the means unbiased.
    c.sample_clamp_indirect = CLAMP_IND
    c.max_bounces = BOUNCES
    c.volume_bounces = BOUNCES
    c.diffuse_bounces = 2
    c.glossy_bounces = 0
    c.transmission_bounces = 0
    c.volume_biased = bool(BIASED)
    c.use_volume_guiding = bool(arg("vguide", 1))
    c.use_light_tree = bool(arg("lighttree", 0))        # two lights only (sun, world): the tree costs ~40 % here
    c.seed = SEED


def world_sky(sc, theta_deg, el_deg, strength=1.0, altitude=30.0):
    w = sc.world
    if w.node_tree is None:
        w.use_nodes = True
    nt = w.node_tree
    nt.nodes.clear()
    sky = nt.nodes.new("ShaderNodeTexSky")
    sky.sky_type = "MULTIPLE_SCATTERING"
    sky.sun_disc = False
    sky.sun_size = math.radians(0.545)
    sky.sun_elevation = math.radians(el_deg)
    d = sun_dir(theta_deg, el_deg)
    sky.sun_rotation = math.atan2(d.x, d.y)          # compass azimuth: 0 = +Y, 90 deg = +X
    sky.altitude = altitude
    sky.air_density = 1.0
    sky.aerosol_density = 1.6
    sky.ozone_density = 1.0
    bg = nt.nodes.new("ShaderNodeBackground")
    bg.inputs["Strength"].default_value = strength
    out = nt.nodes.new("ShaderNodeOutputWorld")
    nt.links.new(sky.outputs["Color"], bg.inputs["Color"])
    nt.links.new(bg.outputs["Background"], out.inputs["Surface"])
    return sky


def world_black(sc):
    w = sc.world
    if w.node_tree is None:
        w.use_nodes = True
    nt = w.node_tree
    nt.nodes.clear()
    bg = nt.nodes.new("ShaderNodeBackground")
    bg.inputs["Color"].default_value = (0, 0, 0, 1)
    bg.inputs["Strength"].default_value = 0.0
    out = nt.nodes.new("ShaderNodeOutputWorld")
    nt.links.new(bg.outputs["Background"], out.inputs["Surface"])


def add_sun(sc, direction, strength, angle_deg=0.53):
    ld = bpy.data.lights.new(sc.name + "_Sun", "SUN")
    ld.energy = strength
    ld.angle = math.radians(angle_deg)
    ob = bpy.data.objects.new(sc.name + "_Sun", ld)
    sc.collection.objects.link(ob)
    ob.rotation_euler = Vector(direction).normalized().to_track_quat("Z", "Y").to_euler()
    return ob


def lightgroups(sc, sun_ob):
    vl = sc.view_layers[0]
    for n in ("sun", "sky"):
        if n not in [g.name for g in vl.lightgroups]:
            vl.lightgroups.add(name=n)
    sun_ob.lightgroup = "sun"
    sc.world.lightgroup = "sky"
    vl.use_pass_environment = True


def render(sc):
    log("render", sc.name, "%dx%d" % (sc.render.resolution_x, sc.render.resolution_y), "samples", sc.cycles.samples,
        "biased", sc.cycles.volume_biased, "->", sc.render.filepath)
    t = time.time()
    bpy.ops.render.render(write_still=True, scene=sc.name)
    dt = time.time() - t
    log("  done in %.1fs" % dt)
    return dt


def add_camera(sc, cfg):
    T = cfg["T"]
    cd = bpy.data.cameras.new(sc.name + "_Cam")
    cd.type = "ORTHO"
    cd.ortho_scale = T
    cd.clip_start = 1.0
    cd.clip_end = (cfg["z1"] - cfg["z0"]) + 400.0
    cam = bpy.data.objects.new(sc.name + "_Cam", cd)
    sc.collection.objects.link(cam)
    # rotation 180 deg about X: camera -Z (view) = world +Z (up), image right = +X, image up = -Y (rows grow with +Y)
    cam.rotation_euler = (math.pi, 0.0, 0.0)
    cam.location = (T / 2.0, T / 2.0, cfg["z0"] - 100.0)
    sc.camera = cam
    return cam


def add_domain(sc, cfg, mat):
    import bmesh
    T, m = cfg["T"], (MARGIN if MARGIN > 0 else cfg["margin"])
    x0, x1 = -m * T, (1 + m) * T
    me = bpy.data.meshes.new(sc.name + "_Slab")
    bm = bmesh.new()
    bmesh.ops.create_cube(bm, size=1.0)
    for v in bm.verts:
        v.co = Vector((x0 + (v.co.x + 0.5) * (x1 - x0), x0 + (v.co.y + 0.5) * (x1 - x0),
                       cfg["z0"] + (v.co.z + 0.5) * (cfg["z1"] - cfg["z0"])))
    bm.to_mesh(me)
    bm.free()
    me.materials.append(mat)
    ob = bpy.data.objects.new(sc.name + "_Slab", me)
    sc.collection.objects.link(ob)
    if WALLS:
        # black walls on the four sides of the slab box (slab height only): near-horizontal light may not enter
        # through the box sides, as in an endless layer, where it would cross tens of km of cloud. Without them the
        # bright horizon toward the sun leaks in through the nearer side and the tile edges light differently (a seam).
        wm = bpy.data.materials.new(sc.name + "_Wall")
        if wm.node_tree is None:
            wm.use_nodes = True
        bsdf = next(nd for nd in wm.node_tree.nodes if nd.type == "BSDF_PRINCIPLED")
        bsdf.inputs["Base Color"].default_value = (0, 0, 0, 1)
        bsdf.inputs["Roughness"].default_value = 1.0
        for k, nm in (("Specular IOR Level", 0.0), ("Specular", 0.0)):
            if k in bsdf.inputs:
                bsdf.inputs[k].default_value = nm
        wmesh = bpy.data.meshes.new(sc.name + "_Walls")
        z0, z1 = cfg["z0"], cfg["z1"]
        verts, faces = [], []
        for (xa, ya, xb, yb) in ((x0, x0, x1, x0), (x1, x0, x1, x1), (x1, x1, x0, x1), (x0, x1, x0, x0)):
            b = len(verts)
            verts += [(xa, ya, z0), (xb, yb, z0), (xb, yb, z1), (xa, ya, z1)]
            faces.append((b, b + 1, b + 2, b + 3))
        wmesh.from_pydata(verts, [], faces)
        wmesh.materials.append(wm)
        wob = bpy.data.objects.new(sc.name + "_Walls", wmesh)
        sc.collection.objects.link(wob)
        wob.visible_camera = False
    return ob


# ------------------------------------------------------------------------------------------------ the density shader
def density_nodes(nt, cfg, img, detail=True):
    """Extinction (1/m) at the shading point. Object = world coordinates (identity transform)."""
    N = nt.nodes
    L = nt.links
    T = cfg["T"]

    def math_node(op, a=None, b=None, c=None):
        n = N.new("ShaderNodeMath")
        n.operation = op
        for i, v in enumerate((a, b, c)):
            if v is None:
                continue
            if isinstance(v, (int, float)):
                n.inputs[i].default_value = float(v)
            else:
                L.new(v, n.inputs[i])
        return n.outputs[0]

    tc = N.new("ShaderNodeTexCoord")
    sep = N.new("ShaderNodeSeparateXYZ")
    L.new(tc.outputs["Object"], sep.inputs[0])
    x, y, z = sep.outputs[0], sep.outputs[1], sep.outputs[2]
    # image lookup: u = x / T, v_blender = 1 - y / T (Blender's v = 0 is the bottom row; our row 0 is at the top)
    u = math_node("DIVIDE", x, T)
    vb = math_node("SUBTRACT", 1.0, math_node("DIVIDE", y, T))
    comb = N.new("ShaderNodeCombineXYZ")
    L.new(u, comb.inputs[0])
    L.new(vb, comb.inputs[1])
    it = N.new("ShaderNodeTexImage")
    it.image = img
    it.interpolation = "Linear"
    it.extension = "REPEAT"
    L.new(comb.outputs[0], it.inputs["Vector"])
    sepc = N.new("ShaderNodeSeparateColor")
    L.new(it.outputs["Color"], sepc.inputs[0])
    zb, zt, kd = sepc.outputs[0], sepc.outputs[1], sepc.outputs[2]
    amp = it.outputs["Alpha"]
    # inside distance: min(z - base, top - z), metres
    d_in = math_node("MINIMUM", math_node("SUBTRACT", z, zb), math_node("SUBTRACT", zt, z))
    if detail:
        # periodic 4D noise: (Ru cos 2piU, Ru sin 2piU, Rv cos 2piV + kz z, Rv sin 2piV)
        a = math_node("MULTIPLY", u, 2 * math.pi)
        b = math_node("MULTIPLY", math_node("DIVIDE", y, T), 2 * math.pi)
        Ru, Rv = cfg["nU"] / (2 * math.pi), cfg["nV"] / (2 * math.pi)
        cvec = N.new("ShaderNodeCombineXYZ")
        L.new(math_node("MULTIPLY", math_node("COSINE", a), Ru), cvec.inputs[0])
        L.new(math_node("MULTIPLY", math_node("SINE", a), Ru), cvec.inputs[1])
        L.new(math_node("ADD", math_node("MULTIPLY", math_node("COSINE", b), Rv), math_node("MULTIPLY", z, cfg["kz"])),
              cvec.inputs[2])
        wv = math_node("MULTIPLY", math_node("SINE", b), Rv)
        bill = None
        for k, (scale, ampl, det) in enumerate(((1.0, cfg["amp1"], 2.0), (3.1, cfg["amp2"], 2.0))):
            nz = N.new("ShaderNodeTexNoise")
            nz.noise_dimensions = "4D"
            nz.noise_type = "FBM"
            nz.normalize = True
            nz.inputs["Scale"].default_value = scale
            nz.inputs["Detail"].default_value = det
            nz.inputs["Roughness"].default_value = 0.5
            nz.inputs["Lacunarity"].default_value = 2.0
            nz.inputs["Distortion"].default_value = 0.0
            L.new(cvec.outputs[0], nz.inputs["Vector"])
            L.new(wv, nz.inputs["W"])
            # billow: |2n - 1| - 0.3 (rounded bumps, creases where n = 0.5)
            bl = math_node("MULTIPLY", math_node("SUBTRACT", math_node("ABSOLUTE", math_node("SUBTRACT", math_node(
                "MULTIPLY", nz.outputs["Factor"], 2.0), 1.0)), 0.3), ampl)
            bill = bl if bill is None else math_node("ADD", bill, bl)
        d_in = math_node("ADD", d_in, math_node("MULTIPLY", bill, amp))
    # density: smooth ramp over 'soft' metres inside the boundary, times the map density, times sigma
    ramp = N.new("ShaderNodeMapRange")
    ramp.interpolation_type = "SMOOTHSTEP"
    ramp.clamp = True
    L.new(d_in, ramp.inputs["Value"])
    ramp.inputs["From Min"].default_value = 0.0
    ramp.inputs["From Max"].default_value = cfg["soft"]
    ramp.inputs["To Min"].default_value = 0.0
    ramp.inputs["To Max"].default_value = 1.0
    dens = math_node("MULTIPLY", math_node("MULTIPLY", ramp.outputs[0], kd), cfg["sigma"])
    return dens


def cloud_material(name, cfg, img, mode="scatter", detail=True):
    """mode 'scatter': HG2 phase (0.7 HG(0.8) + 0.3 HG(-0.2)), white; mode 'tau': emission = extinction (camera ray -> tau)."""
    m = bpy.data.materials.new(name)
    if m.node_tree is None:
        m.use_nodes = True
    nt = m.node_tree
    nt.nodes.clear()
    out = nt.nodes.new("ShaderNodeOutputMaterial")
    dens = density_nodes(nt, cfg, img, detail=detail)
    if mode == "scatter":
        shaders = []
        for w, g in HG2:
            vs = nt.nodes.new("ShaderNodeVolumeScatter")
            vs.phase = "HENYEY_GREENSTEIN"
            vs.inputs["Color"].default_value = (1, 1, 1, 1)
            vs.inputs["Anisotropy"].default_value = g
            mul = nt.nodes.new("ShaderNodeMath")
            mul.operation = "MULTIPLY"
            nt.links.new(dens, mul.inputs[0])
            mul.inputs[1].default_value = w
            nt.links.new(mul.outputs[0], vs.inputs["Density"])
            shaders.append(vs.outputs[0])
        add = nt.nodes.new("ShaderNodeAddShader")
        nt.links.new(shaders[0], add.inputs[0])
        nt.links.new(shaders[1], add.inputs[1])
        nt.links.new(add.outputs[0], out.inputs["Volume"])
    else:
        em = nt.nodes.new("ShaderNodeEmission")
        em.inputs["Color"].default_value = (1, 1, 1, 1)
        nt.links.new(dens, em.inputs["Strength"])
        nt.links.new(em.outputs[0], out.inputs["Volume"])
    return m


def load_map_image(path):
    img = bpy.data.images.load(path, check_existing=False)
    img.colorspace_settings.name = "Non-Color"
    img.alpha_mode = "CHANNEL_PACKED"
    return img


# ------------------------------------------------------------------------------------------------ render a layer
def render_layer(key, map_path, res, spp, tau_spp, tag, detail=True, sun_strength=None, do_tau=True):
    cfg = CFG[key]
    if sun_strength is None:
        sun_strength = SUN_STRENGTH if SUN_STRENGTH > 0 else cfg["sun_phys"] * SUN_FACTOR
    img = load_map_image(map_path)
    times = {}
    # lit render
    sc = new_scene("%s_lit%s" % (cfg["name"], tag))
    world_sky(sc, cfg["theta"], SUN_EL, 1.0, altitude=cfg["alt"])
    sun = add_sun(sc, sun_dir(cfg["theta"]), sun_strength)
    lightgroups(sc, sun)
    mat = cloud_material(cfg["name"] + "_Scatter", cfg, img, "scatter", detail)
    add_domain(sc, cfg, mat)
    add_camera(sc, cfg)
    lit_path = os.path.join(RAW, "%s_lit%s.exr" % (key, tag))
    setup_render(sc, res, res, spp, lit_path)
    times["lit"] = render(sc)
    # sidecar: the lamp the sun light group was rendered with (compose rescales sunY to the shipped sun factor)
    with open(lit_path[:-4] + ".json", "w") as fh:
        json.dump({"sun_strength": sun_strength, "sun_factor": sun_strength / cfg["sun_phys"], "spp": spp, "res": res,
                   "clamp_indirect": CLAMP_IND, "map": map_path}, fh, indent=1)
    tau_path = os.path.join(RAW, "%s_tau%s.exr" % (key, tag))
    if do_tau:
        sc2 = new_scene("%s_tau%s" % (cfg["name"], tag))
        world_black(sc2)
        mat2 = cloud_material(cfg["name"] + "_Tau", cfg, img, "tau", detail)
        add_domain(sc2, cfg, mat2)
        add_camera(sc2, cfg)
        setup_render(sc2, res, res, tau_spp, tau_path)
        sc2.render.film_transparent = False
        sc2.cycles.use_adaptive_sampling = False
        sc2.cycles.volume_bounces = 0
        sc2.cycles.max_bounces = 0
        times["tau"] = render(sc2)
    return lit_path, tau_path, times


# ------------------------------------------------------------------------------------------------ stage: marker
def stage_marker():
    """One puff at (U, V) = (0.30, 0.70) through the full map/image/render path. Expect its alpha centroid at column
    0.30 N, row 0.70 N, and the sunlit side toward +U and slightly +V (right, slightly down), at theta = 13.3 deg."""
    n = 256
    m = maps_marker(512)
    path = os.path.join(RAW, "marker_map.exr")
    save_map(path, m)
    mt = arg("mtheta", CFG["alto"]["theta"])
    CFG["alto"]["theta"] = mt
    lit, taup, times = render_layer("alto", path, n, 128, 32, "_marker", detail=False)
    d = read_exr_all(lit)
    a = np.clip(ch(d, "Combined", "A"), 0, 1)
    sun = lum(np.stack([ch(d, "Combined_sun", c) for c in "RGB"], -1))
    rows, cols = np.mgrid[0:n, 0:n].astype(np.float64)
    ca = (float((a * cols).sum() / a.sum()) / n, float((a * rows).sum() / a.sum()) / n)
    s = sun / np.maximum(a, 1e-3) * (a > 0.5)
    cs = (float((s * cols).sum() / s.sum()) / n, float((s * rows).sum() / s.sum()) / n)
    ang = math.degrees(math.atan2(cs[1] - ca[1], cs[0] - ca[0]))
    log("MARKER alpha centroid (U, V) = (%.3f, %.3f), expected (0.300, 0.700)" % ca)
    log("MARKER sunlit centroid offset (dU, dV) = (%+.4f, %+.4f) -> azimuth %.1f deg from +U toward +V (sun theta %.1f)" % (
        cs[0] - ca[0], cs[1] - ca[1], ang, CFG["alto"]["theta"]))
    # tau pass vs the analytic tau of the map with the shader's smoothstep ramp ('soft' metres) at the base and the top
    t = read_exr_all(taup)
    tau_r = lum(np.stack([ch(t, "Combined", c) for c in "RGB"], -1))
    tau_a = ramp_tau(m, CFG["alto"]).reshape(n, 2, n, 2).mean(axis=(1, 3))
    msk = tau_a > 1.0
    q = tau_r[msk] / tau_a[msk]
    log("MARKER tau render/analytic: median ratio %.4f, p5 %.4f, p95 %.4f, max analytic %.2f" % (
        float(np.median(q)), float(np.percentile(q, 5)), float(np.percentile(q, 95)), float(tau_a.max())))
    a_tau = 1 - np.exp(-tau_r)
    log("MARKER alpha (film) vs 1-exp(-tau): mean abs diff %.4f inside, film alpha max %.3f" % (
        float(np.abs(a - a_tau)[a_tau > 0.05].mean()), float(a.max())))
    sky = np.maximum(lum(np.stack([ch(d, "Combined_sky", c) for c in "RGB"], -1)) - lum(np.stack([ch(d, "Environment", c) for c in "RGB"], -1)), 0)
    sk = sky / np.maximum(a, 1e-3) * (a > 0.5)
    cs2 = (float((sk * cols).sum() / sk.sum()) / n, float((sk * rows).sum() / sk.sum()) / n)
    log("MARKER sky-lit centroid offset (dU, dV) = (%+.4f, %+.4f) -> azimuth %.1f deg" % (
        cs2[0] - ca[0], cs2[1] - ca[1], math.degrees(math.atan2(cs2[1] - ca[1], cs2[0] - ca[0]))))
    # a picture: crop around the puff, x4: alpha | sun (straight) | sky (straight)
    r0, c0 = int(ca[1] * n) - 24, int(ca[0] * n) - 24
    crop = lambda img: np.repeat(np.repeat(np.roll(np.roll(img, -r0, 0), -c0, 1)[:48, :48], 4, 0), 4, 1)
    viz = np.zeros((192, 3 * 192, 3), np.float32)
    viz[:, :192] = crop(a)[..., None]
    viz[:, 192:384] = crop(s / max(1e-6, float(np.percentile(s[s > 0], 99))))[..., None]
    viz[:, 384:] = crop(sk / max(1e-6, float(np.percentile(sk[sk > 0], 99))))[..., None]
    viz[:, 191:193] = (1, 0.3, 0.3)
    viz[:, 383:385] = (1, 0.3, 0.3)
    oiio_write(os.path.join(SCR, "layers_marker_t%d.png" % int(round(mt))),
               (np.clip(viz, 0, 1) ** (1 / 2.2) * 255 + 0.5).astype(np.uint8), "uint8")
    log("MARKER times", times)


def ramp_tau(m, cfg):
    """Analytic optical depth of a map column without 3D detail: sigma * k * integral of smoothstep(d_in / soft)."""
    thick = np.clip(m[..., 1] - m[..., 0], 0, None)
    s = cfg["soft"]
    # integral over z of smoothstep(min(z, t - z) / s): t - s for t >= 2s, else 2 * int_0^{t/2} smoothstep(x/s) dx
    h = np.minimum(thick / 2.0, s) / s
    part = s * (h ** 3 - 0.5 * h ** 4)          # int_0^{h s} (3x^2/s^2 - 2x^3/s^3) dx
    full = np.where(thick >= 2 * s, thick - s, 2 * part)
    return cfg["sigma"] * np.clip(m[..., 2], 0, None) * full


# ------------------------------------------------------------------------------------------------ stage: skycal
def stage_skycal():
    """Measures the sky model at each layer's altitude (sun 6 deg): sun-disc normal irradiance (disc on minus off),
    zenith radiance, mean radiance and horizontal irradiance of the upper hemisphere, all as luminance."""
    W, H = 2048, 1024
    res = {}
    for key in LAYERS:
        cfg = CFG[key]
        imgs = {}
        for disc in (0, 1):
            sc = new_scene("SkyCal_%s_%d" % (key, disc))
            sky = world_sky(sc, cfg["theta"], SUN_EL, 1.0, altitude=cfg["alt"])
            sky.sun_disc = bool(disc)
            cd = bpy.data.cameras.new("SkyCalCam")
            cd.type = "PANO"
            cd.panorama_type = "EQUIRECTANGULAR"
            cd.latitude_min, cd.latitude_max = -math.pi / 2, math.pi / 2
            cd.longitude_min, cd.longitude_max = -math.pi, math.pi
            cam = bpy.data.objects.new("SkyCalCam", cd)
            sc.collection.objects.link(cam)
            cam.rotation_euler = (math.radians(90), 0, 0)
            sc.camera = cam
            path = os.path.join(RAW, "skycal_%s_%d.exr" % (key, disc))
            setup_render(sc, W, H, 64, path)
            sc.render.film_transparent = False
            sc.cycles.use_adaptive_sampling = False
            render(sc)
            dd = read_exr_all(path)
            imgs[disc] = lum(np.stack([ch(dd, "Combined", c) for c in "RGB"], -1))
        lat = (0.5 - (np.arange(H) + 0.5) / H) * math.pi
        dom = (2 * math.pi / W) * (math.pi / H) * np.cos(lat)                 # solid angle per pixel by row
        up = lat > 0
        L0 = imgs[0]
        e_sun = float(((imgs[1] - L0) * dom[:, None]).sum())
        e_h = float((L0[up] * (dom[up] * np.sin(lat[up]))[:, None]).sum())
        mean_up = float((L0[up] * dom[up][:, None]).sum() / dom[up].sum())
        zen = float(L0[:8].mean())
        res[key] = {"sun_normal_irradiance": e_sun, "sky_horizontal_irradiance": e_h, "sky_mean_radiance_up": mean_up,
                    "sky_zenith_radiance": zen, "sun_over_sky_irradiance_on_vertical_facing_sun": None}
        log("SKYCAL %s %s" % (key, json.dumps(res[key])))
    return res


# ------------------------------------------------------------------------------------------------ stage: maps
def stage_maps():
    for key in LAYERS:
        cfg = CFG[key]
        # deck coverage 0.18 kept (tuned 14 Nov: opacity > 0.5 on ~7 %, optical depth median ~0.7, p99 ~3.2)
        m = maps_alto(MAPRES, SEED, arg("al_cov", 0.18)) if key == "alto" else maps_cirrus(MAPRES, SEED)
        path = os.path.join(RAW, "%s_map%s.exr" % (key, TAG))
        save_map(path, m)
        a = map_preview(os.path.join(SCR, "layers_map_%s%s.png" % (key, TAG)), m, cfg)
        tau = ramp_tau(m, cfg)
        cl = a > 0.02
        log("map %s: opacity>0.5 %.3f  >0.1 %.3f  >0.02 %.3f  mean %.3f; over opacity>0.02: tau p50 %.2f p90 %.2f p99 %.2f, "
            "opacity p50 %.2f p90 %.2f  (map only, no 3D detail) -> %s" % (
                key, float((a > 0.5).mean()), float((a > 0.1).mean()), float(cl.mean()), float(a.mean()),
                float(np.percentile(tau[cl], 50)), float(np.percentile(tau[cl], 90)), float(np.percentile(tau[cl], 99)),
                float(np.percentile(a[cl], 50)), float(np.percentile(a[cl], 90)), path))


# ------------------------------------------------------------------------------------------------ stage: quick
def stage_quick():
    """Structure check before a Cycles render: Light / Alpha textures from the maps with a cheap shading proxy (sun
    light falls off with the optical depth met toward the sun, sky light dims in thick cores), written to --out, so
    preview_layers.py can show coverage and shapes in perspective. The light is NOT the bake's."""
    for key in LAYERS:
        cfg = CFG[key]
        mm = read_exr_all(os.path.join(RAW, "%s_map%s.exr" % (key, MAPTAG)))
        m = np.stack([mm["R"], mm["G"], mm["B"], mm["A"]], -1)
        tau = ramp_tau(m, cfg).astype(np.float32)
        a = 1.0 - np.exp(-tau)
        n = tau.shape[0]
        # optical depth met toward the sun: tau of the texels up to ~1 km along the sun azimuth (thin slab, 6 deg sun)
        th = math.radians(cfg["theta"])
        step = 60.0 / (cfg["T"] / n)
        acc = np.zeros_like(tau)
        for k in range(1, 17):
            dx, dy = k * step * math.cos(th), k * step * math.sin(th)
            acc += np.roll(np.roll(tau, -int(round(dy)), 0), -int(round(dx)), 1) * math.exp(-k / 8.0)
        acc /= 3.0
        sun = (1.3 * np.exp(-0.9 * (acc + 0.5 * tau)) + 0.08).astype(np.float32)
        sky = (0.75 + 0.45 * np.exp(-tau)).astype(np.float32)
        tref = TAU_REF[key]
        light = np.stack([sun, sky, tau / tref], -1)
        write_exr(os.path.join(OUT, "T_BSky_Day_%sLight.exr" % cfg["name"]), light, half=True, names=("R", "G", "B"))
        write_png_grey(os.path.join(OUT, "T_BSky_Day_%sAlpha.png" % cfg["name"]), a)
        log("quick %s -> %s" % (key, OUT))


# ------------------------------------------------------------------------------------------------ stage: render
def stage_render():
    for key in LAYERS:
        path = os.path.join(RAW, "%s_map%s.exr" % (key, MAPTAG))
        lit, taup, times = render_layer(key, path, RES, SPP if SPP > 0 else CFG[key]["spp"], TAU_SPP, TAG,
                                        detail=bool(arg("detail", 0)))
        log("RENDER %s times %s" % (key, json.dumps({k: round(v, 1) for k, v in times.items()})))
        # quick look: straight sun and sky luminance, alpha from tau
        d = read_exr_all(lit)
        t = read_exr_all(taup)
        tau = lum(np.stack([ch(t, "Combined", c) for c in "RGB"], -1))
        a = 1.0 - np.exp(-tau)
        fa = np.clip(ch(d, "Combined", "A"), 0, 1)
        sun = lum(np.stack([ch(d, "Combined_sun", c) for c in "RGB"], -1))
        env = lum(np.stack([ch(d, "Environment", c) for c in "RGB"], -1))
        sky = np.maximum(lum(np.stack([ch(d, "Combined_sky", c) for c in "RGB"], -1)) - env, 0)
        log("RENDER %s: opacity(tau)>0.5 %.3f, film alpha mean %.3f vs 1-exp(-tau) mean %.3f; sun premult mean %.4g, sky %.4g" % (
            key, float((a > 0.5).mean()), float(fa.mean()), float(a.mean()), float(sun.mean()), float(sky.mean())))


# ------------------------------------------------------------------------------------------------ stage: compose
# tauN = tau / tauScaleRef. Round 2: the same reference for both layers, so one runtime TauScale serves both
TAU_REF = {"alto": 10.0, "cirrus": 10.0}
# shipped sun factor (x the sky model's sun irradiance at the layer = the lamp sunY corresponds to), the same for both
# layers so both share one physical scale. Round 2 (measured on R7, 14 Nov): its mauve altocumulus patches sit on a 1:1
# mix of the runtime's SunTint and AmbTint by luminance, and its brightest cirrus cores (223,161,170) are sun-dominated
# salmon. One factor of 0.31 gives exactly that, physically: the self-shadowed deck comes out at a median sun:sky of
# ~1:1 after the tints, the unshadowed thin cirrus at ~3:1. (0.5 in round 1: lit edges up to 5:1 and clipped.)
SUN_SHIP = {"alto": 0.31, "cirrus": 0.31}
# per-layer factor on the shipped straight light (R and G), recorded in layers_meta.json ("light_scale")
LIGHT_SCALE = {"alto": arg("lightscale_alto", 1.0), "cirrus": arg("lightscale_cirrus", 1.0)}
# B channel dilation (weight, blur sigma in texels): the deck's rims count as part of their thick cells (see compose)
# (0.85, 6) tested against the review's (0.6, 4): 14 % fewer clipped pixels near the sun, same look elsewhere
B_DILATE = {"alto": (arg("bdil_alto", 0.85), arg("bdil_sigma", 6.0)), "cirrus": (arg("bdil_cirrus", 0.0), arg("bdil_sigma", 6.0))}


def roll2(a, dy, dx):
    return np.roll(np.roll(a, dy, 0), dx, 1)


def defirefly(x, k=3.0, eps=1e-4):
    """Single-texel outliers (path-tracing fireflies): a texel brighter than k x the brightest of its 8 wrapped
    neighbours (+eps) takes that neighbour maximum. Real bright edges have bright neighbours and stay."""
    nb = np.max(np.stack([roll2(x, dy, dx) for dy in (-1, 0, 1) for dx in (-1, 0, 1) if dy or dx]), axis=0)
    bad = x > k * nb + eps
    return np.where(bad, nb, x), int(bad.sum())


def dedark(x, a, floor, k=0.35, r=2):
    """Dark specks in the straight light (tau-pass noise divided into thin texels, or a starved path): a cloud texel
    darker than k x its wrapped (2r+1)^2 median, where that median is above 'floor', takes the median."""
    out = x.copy()
    n = x.shape[0]
    bad_total = 0
    for y0 in range(0, n, 256):
        rows = np.arange(y0 - r, min(n, y0 + 256) + r) % n
        blk = x[rows]
        st = np.stack([np.roll(blk, dx, 1)[r + dy:r + dy + (len(rows) - 2 * r)] for dy in range(-r, r + 1) for dx in range(-r, r + 1)])
        med = np.median(st, axis=0)
        cur = x[y0:y0 + med.shape[0]]
        bad = (a[y0:y0 + med.shape[0]] > 0.2) & (med > floor) & (cur < k * med)
        out[y0:y0 + med.shape[0]] = np.where(bad, med, cur)
        bad_total += int(bad.sum())
    return out, bad_total


def box_wrap(a, r):
    """Box mean over (2r+1)^2 texels on the periodic tile."""
    out = a.astype(np.float64)
    for ax in (0, 1):
        c = np.cumsum(np.concatenate([np.take(out, np.arange(-r - 1, 0), axis=ax), out,
                                      np.take(out, np.arange(0, r), axis=ax)], axis=ax), axis=ax)
        n = out.shape[ax]
        out = (np.take(c, np.arange(2 * r + 1, 2 * r + 1 + n), axis=ax) - np.take(c, np.arange(0, n), axis=ax)) / (2 * r + 1)
    return out


def guided_filter(I, p, r, eps):
    """He et al. guided filter on the periodic tile: smooths p where the guide I is flat, keeps p's edges where I has
    edges (the noise-free optical depth carries every cloud edge)."""
    mI, mp = box_wrap(I, r), box_wrap(p, r)
    cov = box_wrap(I * p, r) - mI * mp
    var = box_wrap(I * I, r) - mI * mI
    a = cov / (var + eps)
    b = mp - a * mI
    return (box_wrap(a, r) * I + box_wrap(b, r)).astype(np.float32)


def pull_push_fill_wrap(values, weight, levels=11):
    """Fill texels with weight ~0 from their neighbourhood (premultiplied pyramid, power-of-two periodic tile)."""
    w = np.clip(weight, 0, 1).astype(np.float32)
    v = values.astype(np.float32) * w[..., None]
    pyr = [(v, w)]
    for _ in range(levels):
        v, w = pyr[-1]
        if w.shape[0] < 2:
            break
        v = (v[0::2, 0::2] + v[1::2, 0::2] + v[0::2, 1::2] + v[1::2, 1::2]) * 0.25
        w = (w[0::2, 0::2] + w[1::2, 0::2] + w[0::2, 1::2] + w[1::2, 1::2]) * 0.25
        pyr.append((v, w))
    fv, fw = pyr[-1]
    fill = fv / np.maximum(fw, 1e-8)[..., None]
    for v, w in reversed(pyr[:-1]):
        up = np.repeat(np.repeat(fill, 2, axis=0), 2, axis=1)
        # bilinear-ish smoothing of the coarse fill (wrapped) so filled areas have no blocks
        up = 0.25 * (2 * up + np.roll(up, 1, 0) + np.roll(up, -1, 0)) if up.shape[0] > 2 else up
        up = 0.25 * (2 * up + np.roll(up, 1, 1) + np.roll(up, -1, 1)) if up.shape[1] > 2 else up
        a = np.clip(w * 4.0, 0, 1)[..., None]
        own = v / np.maximum(w, 1e-8)[..., None]
        fill = own * a + up * (1 - a)
    return fill


def clamp_thin(x, a, k=2.5, r=4, a_thin=0.4, a_cloud=0.05):
    """Hot thin texels: where opacity < a_thin, the straight value may be at most k x the median of the cloud texels
    (opacity > a_cloud) in its wrapped (2r+1)^2 window. Dividing noisy light by a small opacity, and multiple-scattering
    paths that found the sun through a gap, made rim texels 3-10x their neighbours (review round 2). Bright thin edges
    have bright neighbours in the window and keep their glow."""
    n = x.shape[0]
    cand = np.argwhere((a < a_thin) & (a > 0.01))
    if not len(cand):
        return x, 0
    xs = np.where(a > a_cloud, x, np.nan).astype(np.float32)
    out = x.copy()
    hit = 0
    for i0 in range(0, len(cand), 200000):
        cy, cx = cand[i0:i0 + 200000, 0], cand[i0:i0 + 200000, 1]
        win = np.stack([xs[(cy + dy) % n, (cx + dx) % n] for dy in range(-r, r + 1) for dx in range(-r, r + 1)], 1)
        with np.errstate(all="ignore"):
            med = np.nanmedian(win, axis=1)
        cur = x[cy, cx]
        lim = np.where(np.isfinite(med), k * med, cur)
        bad = cur > lim
        out[cy[bad], cx[bad]] = lim[bad]
        hit += int(bad.sum())
    return out, hit


def write_png_grey(path, x01):
    g = np.round(np.clip(x01, 0, 1) * 255.0).astype(np.uint8)
    oiio_write(path, g, "uint8", attrs=[("oiio:UnassociatedAlpha", 1)])
    cfg = oiio.ImageSpec()
    cfg.attribute("oiio:UnassociatedAlpha", 1)
    back = oiio.ImageBuf(path, 0, 0, cfg).get_pixels(oiio.UINT8)
    back = back.reshape(g.shape[0], g.shape[1], -1)[..., 0]
    return int(np.abs(back.astype(int) - g.astype(int)).max())


def seam_stats(x):
    su = float(np.abs(x[:, 0] - x[:, -1]).mean())
    sv = float(np.abs(x[0] - x[-1]).mean())
    iu = float(np.abs(x[:, 1:] - x[:, :-1]).mean())
    iv = float(np.abs(x[1:] - x[:-1]).mean())
    return su, iu, sv, iv


def compose_layer(key, tag):
    cfg = CFG[key]
    d = read_exr_all(os.path.join(RAW, "%s_lit%s.exr" % (key, tag)))
    t = read_exr_all(os.path.join(RAW, "%s_tau%s.exr" % (key, tag)))
    tau_r = np.maximum(lum(np.stack([ch(t, "Combined", c) for c in "RGB"], -1)), 0.0)
    n = tau_r.shape[0]
    # The rendered density has no sub-texel detail, so its vertical optical depth is known exactly from the map: the
    # smoothstep-ramp integral per texel, averaged over the render's 1-pixel box of the bilinear map (weights 1/8,
    # 3/4, 1/8 per axis). This is noise free; the emission tau pass is kept as a check (it has rare spikes).
    tau = tau_r
    mpath = os.path.join(RAW, "%s_map%s.exr" % (key, MAPTAG))
    if os.path.exists(mpath) and not bool(arg("detail", 0)):
        mm = read_exr_all(mpath)
        m = np.stack([mm["R"], mm["G"], mm["B"], mm["A"]], -1)
        if m.shape[0] == n:
            ta = ramp_tau(m, cfg).astype(np.float32)
            for ax in (0, 1):
                ta = 0.75 * ta + 0.125 * (np.roll(ta, 1, ax) + np.roll(ta, -1, ax))
            ok = ta > 0.05
            q = tau_r[ok] / ta[ok]
            spikes = int((np.abs(tau_r - ta) > 0.3 * ta + 0.1).sum())
            log("TAU %s analytic vs rendered: median ratio %.4f, p1 %.3f, p99 %.3f, spikes %d, max rendered %.2f vs analytic %.2f" % (
                key, float(np.median(q)), float(np.percentile(q, 1)), float(np.percentile(q, 99)), spikes,
                float(tau_r.max()), float(ta.max())))
            tau = ta
    a = (1.0 - np.exp(-tau)).astype(np.float32)
    sun = lum(np.stack([ch(d, "Combined_sun", c) for c in "RGB"], -1))
    env = lum(np.stack([ch(d, "Environment", c) for c in "RGB"], -1))
    sky = np.maximum(lum(np.stack([ch(d, "Combined_sky", c) for c in "RGB"], -1)) - env, 0.0)
    fa = np.clip(ch(d, "Combined", "A"), 0, 1)
    # the sun light group is linear in the lamp: rescale it from the rendered sun factor to the shipped one
    side = os.path.join(RAW, "%s_lit%s.json" % (key, tag))
    f_render = json.load(open(side))["sun_factor"] if os.path.exists(side) else 0.5
    f_ship = arg("sunship_" + key, SUN_SHIP[key])
    sun = sun * (f_ship / f_render)
    sun, nf1 = defirefly(np.maximum(sun, 0.0))
    sky, nf2 = defirefly(sky)
    # straight (per unit opacity) values where the cloud is there; the rest is filled from the neighbourhood
    w = np.clip((a - 0.01) / 0.10, 0, 1)
    st = np.stack([sun, sky], -1) / np.maximum(a, 1e-4)[..., None]
    # hot thin texels (rims): at most 2.5x the median of the cloud texels around them
    nc = [0, 0]
    for c in range(2):
        st[..., c], h = clamp_thin(st[..., c], a)
        nc[c] += h
    # path-tracing noise: guided filter in log space, guided by log optical depth (noise-free, carries every edge)
    if GF_R > 0:
        g = np.log(tau + 0.02).astype(np.float32)
        for c in range(2):
            v = np.log(np.maximum(st[..., c], 1e-5))
            st[..., c] = np.exp(guided_filter(g, v, GF_R, GF_EPS))
    # dark specks: sky light everywhere in the cloud; sun light only where it is not a deep, sun-starved core
    st[..., 1], nd2 = dedark(st[..., 1], a, 0.05)
    st[..., 0], nd1 = dedark(st[..., 0], a, 0.10)
    for c in range(2):
        st[..., c], h = clamp_thin(st[..., c], a)
        nc[c] += h
    st = pull_push_fill_wrap(st, w)
    st *= LIGHT_SCALE[key]
    # B: the optical depth the runtime's phase blend sees. Deck: max(tau, 0.6 x tau blurred over ~4 texels), so the thin
    # rims of thick cells count as part of the thick cell (a low view through a rim crosses the core: no phase glow,
    # no blown outline) while isolated thin wisps keep their glow; this also keeps mip-averaged B closer to the
    # footprint's real depth. Cirrus: tau. Alpha stays 1 - exp(-tau) either way.
    bd = B_DILATE[key]
    tau_b = np.maximum(tau, bd[0] * blur_wrap(tau, bd[1])) if bd[0] > 0 else tau
    tauN = tau_b / TAU_REF[key]
    light = np.stack([st[..., 0], st[..., 1], tauN], -1).astype(np.float32)
    info = {"fireflies": (nf1, nf2), "dark_specks": (nd1, nd2), "thin_clamped": tuple(nc), "sun_factor_render": f_render,
            "sun_factor_ship": f_ship}
    return light, a, fa, info, (sun, sky)


def stage_compose():
    meta_path = os.path.join(OUT, "layers_meta.json")
    meta = json.load(open(meta_path)) if os.path.exists(meta_path) else {}
    sun_ship = {}
    for key in LAYERS:
        cfg = CFG[key]
        light, a, fa, info, prem = compose_layer(key, TAG)
        sun_ship[key] = info["sun_factor_ship"]
        name = cfg["name"]
        lp = os.path.join(OUT, "T_BSky_Day_%sLight.exr" % name)
        ap = os.path.join(OUT, "T_BSky_Day_%sAlpha.png" % name)
        write_exr(lp, light, half=True, names=("R", "G", "B"))
        rt = write_png_grey(ap, a)
        back = read_exr_all(lp)
        err = max(float(np.abs(back[c] - light[..., i]).max() / max(1e-6, float(np.abs(light[..., i]).max())))
                  for i, c in enumerate("RGB"))
        tau = -np.log(np.maximum(1.0 - a, 1e-6))
        inside = a > 0.5
        cl = a > 0.05
        stats = {
            "coverage_opacity_gt_0.5": float(inside.mean()), "coverage_gt_0.15": float((a > 0.15).mean()),
            "coverage_gt_0.1": float((a > 0.1).mean()), "coverage_gt_0.02": float((a > 0.02).mean()),
            "opacity_mean": float(a.mean()),
            "opacity_p10_p50_p90_over_gt_0.02": [float(np.percentile(a[a > 0.02], q)) for q in (10, 50, 90)],
            "film_alpha_vs_tau_alpha_mean_abs": float(np.abs(fa - a).mean()),
            "sunY_p50_p95_p99_over_gt_0.05": [float(np.percentile(light[..., 0][cl], q)) for q in (50, 95, 99)],
            "skyY_p50_p95_p99_over_gt_0.05": [float(np.percentile(light[..., 1][cl], q)) for q in (50, 95, 99)],
            "tau_p50_p90_p99_over_gt_0.02": [float(np.percentile(tau[a > 0.02], q)) for q in (50, 90, 99)],
            "fireflies_removed_sun_sky": list(info["fireflies"]), "dark_specks_fixed_sun_sky": list(info["dark_specks"]),
            "thin_texels_clamped_sun_sky": list(info["thin_clamped"]),
            "png_round_trip_max_err": rt, "exr_half_round_trip_rel_err": err,
        }
        for nm, x in (("alpha", a), ("sunY", light[..., 0]), ("skyY", light[..., 1]), ("tauN", light[..., 2])):
            su, iu, sv, iv = seam_stats(x)
            stats["seam_" + nm] = {"u_wrap": su, "u_interior": iu, "v_wrap": sv, "v_interior": iv}
        log("COMPOSE %s %s" % (key, json.dumps(stats)))
        bd = B_DILATE[key]
        meta.setdefault("layers", {})[key] = {
            "light_texture": os.path.basename(lp), "alpha_texture": os.path.basename(ap), "size": [a.shape[1], a.shape[0]],
            "tile_size_cm": cfg["T"] * 100.0, "altitude_cm": cfg["alt"] * 100.0,
            "theta_deg": cfg["theta"], "sun_elevation_deg": SUN_EL, "tauScaleRef": TAU_REF[key],
            "B": ("max(tau, %.2f x tau Gaussian-blurred over %.1f texels) / tauScaleRef (phase-blend depth: thin rims of thick "
                  "cells count as thick; Alpha is still 1 - exp(-tau))" % bd if bd[0] > 0 else "tau / tauScaleRef"),
            "light_scale": LIGHT_SCALE[key],
            "sun_factor": sun_ship[key], "sun_strength_W_m2": CFG[key]["sun_phys"] * sun_ship[key],
            "slab_m": [cfg["z0"], cfg["z1"]], "sigma_per_m": cfg["sigma"], "soft_m": cfg["soft"], "stats": stats}
        # 2x2 tiled seam checks
        def tile2(x):
            return np.concatenate([np.concatenate([x, x], 1), np.concatenate([x, x], 1)], 0)
        q = max(1, a.shape[0] // 1024)
        viz = tile2(a[::q, ::q])
        oiio_write(os.path.join(SCR, "layers_tiled_%s_alpha.png" % key), (np.clip(viz, 0, 1) * 255 + 0.5).astype(np.uint8), "uint8")
        s = light[..., 0] * a
        s = s / max(1e-6, float(np.percentile(s[cl], 99))) if cl.any() else s
        viz = tile2(np.clip(s[::q, ::q], 0, 1) ** (1 / 2.2))
        oiio_write(os.path.join(SCR, "layers_tiled_%s_sun.png" % key), (viz * 255 + 0.5).astype(np.uint8), "uint8")
    meta.update({
        "what": "BayshoreSky periodic cloud layers (altocumulus deck, cirrus): baked from below, vertical view (round 2)",
        "channels": {"Light.R": "sunY: sun light-group luminance per unit opacity (white Sun lamp of layers.<key>.sun_strength_W_m2), x light_scale",
                     "Light.G": "skyY: sky light-group luminance per unit opacity (multiple-scattering sky, no disc, strength 1), x light_scale",
                     "Light.B": "tauN = phase-blend optical depth / tauScaleRef (10 for both layers); see layers.<key>.B",
                     "Alpha": "opacity = 1 - exp(-tau), tau the vertical optical depth; 8-bit grey, straight"},
        "texture_axes": "column = U (left to right), row = V (row 0 at the top); aV = (-aU.y, aU.x)",
        "sun_strength_rule": "sky model sun-disc normal irradiance at the layer (luminance, sun 6 deg: sun_phys) x sun_factor; "
                             "the sun light group is linear in the lamp, so compose rescales the render to the shipped factor",
        "sun_phys": {k: CFG[k]["sun_phys"] for k in CFG},
        "sun_factor_note": "0.247 = lum(AmbTint)/lum(SunTint) would make the tinted sun/sky balance physical; 1.0 = plain "
                           "physical sun. Round 2: 0.31 for both layers (was 0.5). After the tints the self-shadowed "
                           "deck's in-cloud sun:sky luminance is ~1:1 at the median (p95 ~3:1, its lit edges), as R7's "
                           "mauve altocumulus patches; the unshadowed thin cirrus comes out ~2.6:1, salmon like R7's "
                           "brightest cirrus cores (223, 161, 170).",
        "sky_strength": 1.0, "sky_model": "Blender MULTIPLE_SCATTERING, aerosol 1.6, altitude = layer altitude",
        "phase_baked": "0.7 HG(0.8) + 0.3 HG(-0.2)", "cBake": math.sin(math.radians(SUN_EL)),
        "light_scale_note": "R and G share one linear scale in both textures: the same sun factor (x each altitude's own "
                            "sky-model sun irradiance), sky strength 1, light_scale 1; one runtime Exposure fits both",
        "bake": {"renderer": "Blender 5.2 Cycles, OptiX, null-scattering volumes, %d volume bounces, light tree off, "
                             "indirect sample clamp %g" % (BOUNCES, CLAMP_IND),
                 "camera": "orthographic, straight up from below the slab, frame = one tile, box pixel filter 1 px",
                 "volume": "2.5D height-field clouds from periodic 2D maps (base, top, density), REPEAT image lookups, "
                           "smoothstep ramp over 'soft' m inside base and top; slab box one tile past the tile on "
                           "every side with black walls on its sides (an endless layer for the 6 deg sun)",
                 "spp": {k: CFG[k]["spp"] for k in CFG},
                 "tau": "analytic from the maps (the density has no sub-texel detail, so this is exact), averaged over "
                        "the render's 1-pixel box; the emission render (camera ray integrates extinction) is a check"},
        "processing": "sunY rescaled to the shipped sun factor; fireflies (texel > 3x its brightest wrapped neighbour) "
                      "clamped; straight = premultiplied / (1 - exp(-tau)); thin texels (opacity < 0.4) at most 2.5x the "
                      "median of the cloud texels in their 9x9 window (before and after filtering); guided filter (r 2, "
                      "eps 0.02, log space, guide log tau); dark specks (cloud texel < 0.35x its 5x5 median) take the "
                      "median; texels under ~6 % opacity take a periodic pull-push fill; x light_scale",
        "runtime": {"Rc_cm": 3.0e7, "camera_height_cm": 600.0, "composite_order": ["cirrus", "alto"],
                    "SunTint": "linear(#FFB88C) * 1.6", "AmbTint": "linear(#8784BA) * 0.9",
                    "phase": "HG2(c) = 0.7 HG(c, 0.8) + 0.3 HG(c, -0.2); ratio = HG2(c) / HG2(cBake), capped at 24",
                    "ws": "exp(-B * TauScale), the same TauScale for both layers",
                    "alto": {"TileSize_cm": 2400000.0, "altitude_cm": 250000.0, "aU_azimuth_deg": 16.70, "speed_cm_s": 600.0, "offset": [0.0, 0.0], "theta_deg": 13.30},
                    "cirrus": {"TileSize_cm": 4800000.0, "altitude_cm": 700000.0, "aU_azimuth_deg": 1.70, "speed_cm_s": 1320.0, "offset": [0.37, 0.37], "theta_deg": 28.30}},
    })
    for k in ("sun_strength", "sun_factor", "light_scale"):
        meta.pop(k, None)
    # round 2 choices (14 Nov), judged with preview_layers.py against R7: Exposure 0.45 puts the cloud pixels at
    # 1.17 / 1.36 (p50 / p90) x their local sky away from the sun (R7: 1.16 / 1.30); TauScale 50 (ws = exp(-5 tau))
    # leaves no cloud pixel pure white near the sun and halves the cloud-made clipping of TauScale 35
    rec = {"Exposure": arg("rec_exposure", 0.45), "TauScale": arg("rec_tauscale", 50.0)}
    if arg("rec_exposure_cirrus", 0.0) > 0:
        rec["Exposure_cirrus"] = arg("rec_exposure_cirrus", 0.0)
    rec["AltoCoverage"] = 1.0
    rec["CirrusCoverage"] = 1.0
    rec["SunTint"] = "unchanged: linear(#FFB88C) * 1.6 (a SunTint near #FFA38C would reach R7's more saturated salmon cores)"
    rec["AmbTint"] = "unchanged: linear(#8784BA) * 0.9"
    rec["Haze"] = "unchanged: 0.3 with the 2.5e6 cm falloff; ratio cap 24"
    rec["note"] = REC_NOTE
    meta["recommended"] = rec
    with open(meta_path, "w") as fh:
        json.dump(meta, fh, indent=2)
    log("compose done ->", OUT)


REC_NOTE = ("One Exposure and one TauScale for both layers: the same tauScaleRef (10), the same sun factor (0.31) and "
            "light_scale 1, so both layers share one physical scale. The clear sky between deck groups and between "
            "cirrus bundles is now in the bake, so the runtime cloud fields (MPC AltoCoverage / CirrusCoverage) should "
            "be off (1.0); with them on, the layers thin out twice. In unreal/30_materials.py terms: c1 and c2 both use "
            "exp(-L.b * 50.0 * TauScale) with CloudTauScale = 1 (replace the 10.0 and 4.0 constants: B now uses "
            "tauScaleRef 10 for the cirrus too), and CirrusExposure = CloudExposure = 0.45. "
            "Recheck Exposure once in-engine with the real tone mapper (the previews view col / (1 + 0.15 col)).")


# ------------------------------------------------------------------------------------------------ main
GF_R = arg("gfr", 2)
GF_EPS = arg("gfeps", 0.02)
log("stages", STAGES, "layers", LAYERS, "res", RES, "spp", SPP, "biased", BIASED)
for st in ("marker", "skycal", "maps", "quick", "render", "compose"):
    if st in STAGES:
        log("=== stage", st)
        globals()["stage_" + st]()
log("all done")

