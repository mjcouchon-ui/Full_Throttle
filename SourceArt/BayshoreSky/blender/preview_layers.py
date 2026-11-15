"""Ground preview of the BayshoreSky cloud LAYERS, applying the runtime model of the game line for line, so what is judged
here is what the game shows (Time = 0):

  shell:  h = altitude - camera height (cm); t = (2 Rc h + h^2) / (Rc d.z + sqrt(Rc^2 d.z^2 + 2 Rc h + h^2)), Rc = 3.0e7 cm
          p = camXY + d.xy t;  uv = (dot(p, aU) - Time speed, dot(p, aV)) / TileSize + offset;  aV = (-aU.y, aU.x)
          texture column = u, row = v (row 0 at the top); wrapped trilinear taps from a mip pyramid, anisotropic as on
          a GPU (8x): N = min(ceil(major / minor), 8) taps spread along the footprint's major axis at LOD log2(major / N)
  light:  (sunY, skyY, tauN) = Light(uv); A = Alpha(uv).r
          c = dot(d, S); HG2(c) = 0.7 HG(c, 0.8) + 0.3 HG(c, -0.2); ratio = HG2(c) / HG2(cBake), cBake = sin 6 deg
          ws = exp(-tauN TauScale); col = Exposure (sunY SunTint lerp(1, min(ratio, 24), ws) + skyY AmbTint)
          col = lerp(col, skyBehind, Haze (1 - exp(-t / 2.5e6)));  base = lerp(base, col, A smoothstep(-0.004, 0.04, d.z))
          cirrus first, then the altocumulus deck in front of it
  sky:    the dome gradient, haze, glow, halo and sun disc of preview_deck.py (GRAD / SKY_PREP), colours from
          unreal/bsky_params.py; the plain sea below the horizon; x SkyBrightness; viewed with col / (1 + 0.15 col), sRGB
  camera: 6 m above the sea, 90 deg horizontal FOV, 1280 x 720, 2 x 2 supersampled. Unreal axes (left-handed: X
          forward, Y right, Z up), so +Y is on the right of the image as in the game.

Run with Blender's Python (NumPy + OpenImageIO):
  "<Blender>/5.2/python/bin/python.exe" -I preview_layers.py [--exposure E] [--exposure-cirrus E] [--tauscale K]
      [--tauscale-alto K] [--tauscale-cirrus K] [--bakes <dir>] [--out <dir>] [--views 30:12,120:14,210:12,up]
      [--no-seams] [--tag _x] [--ss 2] [--aniso 8]
Without arguments it uses the recommended values in bakes/Day/layers_meta.json (one Exposure and one TauScale for both
layers; per-layer values if the meta gives them).
Writes <out>/layers_<view>.png and the seam checks <out>/layers_tiled_<texture>.png (each texture 2 x 2), and prints
per view: the share of the upper sky that is pinker or brighter than its surroundings (the review's measures, R7: 7-8 %
pink, 0.5-1.5 % structured), cloud brightness against the sky behind it (p50 / p90 over cloud pixels), and clipping.
"""
import sys
import os
import json
import math
import argparse

import numpy as np
import OpenImageIO as oiio

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.join(os.path.dirname(HERE), "unreal"))
from bsky_params import COLORS, SCALARS, VECTORS, srgb  # noqa: E402

SCR = os.path.join(os.path.dirname(HERE), "bakes", "Day", "raw", "previews")   # not shipped (raw is never copied)
os.makedirs(SCR, exist_ok=True)
ap = argparse.ArgumentParser()
ap.add_argument("--bakes", default=os.path.join(os.path.dirname(HERE), "bakes", "Day"))
ap.add_argument("--out", default=SCR)
ap.add_argument("--tag", default="")
ap.add_argument("--exposure", type=float, default=None, help="default: layers_meta.json recommended value")
ap.add_argument("--exposure-cirrus", type=float, default=None, help="cirrus layer only (default: same as --exposure)")
ap.add_argument("--tauscale", type=float, default=None, help="both layers (default: layers_meta.json)")
ap.add_argument("--tauscale-alto", type=float, default=None)
ap.add_argument("--tauscale-cirrus", type=float, default=None)
ap.add_argument("--views", default="30:12,120:14,210:12,up")
ap.add_argument("--width", type=int, default=1280)
ap.add_argument("--height", type=int, default=720)
ap.add_argument("--ss", type=int, default=2)
ap.add_argument("--aniso", type=int, default=8, help="max anisotropic taps (1 = plain trilinear at the minor-axis LOD)")
ap.add_argument("--no-seams", action="store_true")
ap.add_argument("--only", default="", help="alto or cirrus: show one layer")
a = ap.parse_args()

C = {k: np.array(srgb(v), np.float32) for k, v in COLORS.items()}
S = dict(SCALARS)
SUND = np.array(dict(VECTORS)["SunDirection"][:3], np.float64)
Sd = SUND / np.linalg.norm(SUND)
Rc = 3.0e7
CAMZ = 600.0
TIME = 0.0
SUN_TINT = np.array(srgb("#FFB88C"), np.float32) * 1.6
AMB_TINT = np.array(srgb("#8784BA"), np.float32) * 0.9
C_BAKE = math.sin(math.radians(6.0))
META = {}
mp = os.path.join(a.bakes, "layers_meta.json")
if os.path.exists(mp):
    META = json.load(open(mp))
REC = META.get("recommended", {})
EXPOSURE = a.exposure if a.exposure is not None else REC.get("Exposure", 1.0)
EXPOSURE_L = {"alto": EXPOSURE,
              "cirrus": a.exposure_cirrus if a.exposure_cirrus is not None else
              (REC.get("Exposure_cirrus", EXPOSURE) if a.exposure is None else EXPOSURE)}
_ts = REC.get("TauScale", 10.0)


def _ts_rec(key):
    return float(_ts.get(key, 10.0)) if isinstance(_ts, dict) else float(_ts)


TAUSCALE = {k: (getattr(a, "tauscale_" + k) if getattr(a, "tauscale_" + k) is not None else
                (a.tauscale if a.tauscale is not None else _ts_rec(k))) for k in ("alto", "cirrus")}
# composited in this order: cirrus first, then the deck in front of it
LAYERS = [
    dict(key="cirrus", name="Cirrus", alt=700000.0, tile=4800000.0, az=1.70, speed=1320.0, offset=(0.37, 0.37)),
    dict(key="alto", name="Alto", alt=250000.0, tile=2400000.0, az=16.70, speed=600.0, offset=(0.0, 0.0)),
]
if a.only:
    LAYERS = [L for L in LAYERS if L["key"] == a.only]


def sat(x):
    return np.clip(x, 0.0, 1.0)


def smoothstep(e0, e1, x):
    t = sat((x - e0) / (e1 - e0))
    return t * t * (3 - 2 * t)


def lerp(p, q, t):
    return p + (q - p) * t


def hg(c, g):
    return (1.0 - g * g) / (4.0 * np.pi * (1.0 + g * g - 2.0 * g * c) ** 1.5)


def hg2(c):
    return 0.7 * hg(c, 0.8) + 0.3 * hg(c, -0.2)


# ------------------------------------------------------------------------------------------------ textures
def read_img(path, channels):
    cfg = oiio.ImageSpec()
    cfg.attribute("oiio:UnassociatedAlpha", 1)           # straight values: never (un)premultiply
    px = oiio.ImageBuf(path, 0, 0, cfg).get_pixels(oiio.FLOAT)
    if px.ndim == 2:
        px = px[..., None]
    return np.ascontiguousarray(px[..., :channels]).astype(np.float32)


def mips_of(img):
    m = [img]
    while m[-1].shape[0] > 1:
        x = m[-1]
        m.append(0.25 * (x[0::2, 0::2] + x[1::2, 0::2] + x[0::2, 1::2] + x[1::2, 1::2]))
    return m


def bilinear(img, u, v):
    n = img.shape[0]
    x, y = u * n - 0.5, v * n - 0.5
    x0, y0 = np.floor(x), np.floor(y)
    fx, fy = (x - x0)[..., None], (y - y0)[..., None]
    x0, y0 = x0.astype(np.int64) % n, y0.astype(np.int64) % n
    x1, y1 = (x0 + 1) % n, (y0 + 1) % n
    p = img[y0, x0] + (img[y0, x1] - img[y0, x0]) * fx
    q = img[y1, x0] + (img[y1, x1] - img[y1, x0]) * fx
    return p + (q - p) * fy


def trilinear(mips, uv, lod):
    l0 = np.floor(lod).astype(int)
    out = np.zeros(uv.shape[:-1] + (mips[0].shape[-1],), np.float32)
    for k in np.unique(l0):
        msk = l0 == k
        u, v = uv[msk, 0], uv[msk, 1]
        f = (lod[msk] - k)[..., None]
        s0 = bilinear(mips[k], u, v)
        s1 = bilinear(mips[k + 1], u, v)
        out[msk] = s0 + (s1 - s0) * f
    return out


def tex(mips, uv, dx, dy):
    """SampleGrad stand-in with GPU-style anisotropic filtering: the pixel footprint's major axis (the longer of the
    two screen derivatives, in texels) is covered by N = min(ceil(major / minor), ANISO) trilinear taps at LOD
    log2(max(minor, major / N)), evenly spread along it and averaged."""
    N = mips[0].shape[0]
    lx = np.linalg.norm(dx, axis=-1) * N
    ly = np.linalg.norm(dy, axis=-1) * N
    major, minor = np.maximum(lx, ly), np.minimum(lx, ly)
    axis = np.where((lx >= ly)[..., None], dx, dy)                       # major axis in uv units
    nt = np.clip(np.ceil(major / np.maximum(minor, 1e-8)), 1, ANISO).astype(int)
    lod = np.clip(np.log2(np.maximum(np.maximum(minor, major / nt), 1e-8)), 0.0, len(mips) - 1.001)
    out = np.zeros(uv.shape[:-1] + (mips[0].shape[-1],), np.float32)
    for n_taps in np.unique(nt):
        msk = nt == n_taps
        acc = np.zeros((int(msk.sum()), mips[0].shape[-1]), np.float32)
        for i in range(n_taps):
            o = (i + 0.5) / n_taps - 0.5
            acc += trilinear(mips, uv[msk] + axis[msk] * o, lod[msk])
        out[msk] = acc / n_taps
    return out


def ddxy(x):
    dx = np.empty_like(x)
    dy = np.empty_like(x)
    dx[:, :-1] = x[:, 1:] - x[:, :-1]
    dx[:, -1] = dx[:, -2]
    dy[:-1] = x[1:] - x[:-1]
    dy[-1] = dy[-2]
    return dx, dy


ANISO = max(1, a.aniso)
TEX = {}
for L in LAYERS:
    lp = os.path.join(a.bakes, "T_BSky_Day_%sLight.exr" % L["name"])
    apth = os.path.join(a.bakes, "T_BSky_Day_%sAlpha.png" % L["name"])
    light = read_img(lp, 3)
    alpha = read_img(apth, 1)
    # Light and Alpha share the footprint, so they are filtered together (4 channels: sunY, skyY, tauN, A)
    TEX[L["key"]] = (mips_of(np.concatenate([light, alpha], -1)), light, alpha)


# ------------------------------------------------------------------------------------------------ one view
def render_view(yaw_deg, pitch_deg, W, H, SS):
    yaw, pitch = math.radians(yaw_deg), math.radians(pitch_deg)
    w2, h2 = W * SS, H * SS
    fov = math.radians(90.0)
    xs = (np.arange(w2) + 0.5) / w2 * 2 - 1
    ys = ((np.arange(h2) + 0.5) / h2 * 2 - 1) * H / W
    X, Y = np.meshgrid(xs * math.tan(fov / 2), -ys * math.tan(fov / 2))
    f = np.array([math.cos(pitch) * math.cos(yaw), math.cos(pitch) * math.sin(yaw), math.sin(pitch)])
    r = np.array([-math.sin(yaw), math.cos(yaw), 0.0])          # Unreal: +Y is to the right when looking along +X
    up = np.cross(f, r)
    d = f[None, None] + X[..., None] * r[None, None] + Y[..., None] * up[None, None]
    d /= np.linalg.norm(d, axis=-1, keepdims=True)
    dz = d[..., 2]

    # --- GRAD (preview_deck.py)
    el = np.arcsin(np.clip(dz, -1, 1))
    e = sat(el / 1.5707963)
    m = np.clip(S["GradientMidPoint"], 0.05, 0.95)
    lo = lerp(C["HorizonColor"], C["MidColor"], smoothstep(0.0, 1.0, e / m)[..., None])
    hi = lerp(C["MidColor"], C["ZenithColor"], smoothstep(0.0, 1.0, (e - m) / (1.0 - m))[..., None])
    c = np.where((e < m)[..., None], lo, hi)
    dh = d[..., :2] + np.array([1e-5, 0.0])
    dh /= np.linalg.norm(dh, axis=-1, keepdims=True)
    sh = SUND[:2] / np.linalg.norm(SUND[:2])
    toward = sat((dh @ sh) * 0.5 + 0.5)
    lowband = 1.0 - smoothstep(0.0, 0.45, e)
    c = lerp(c, C["HazeColor"], (toward ** 3 * lowband * 0.45)[..., None])
    c = lerp(c, C["HazeColor"], (sat(S["Haze"]) * np.exp(-np.maximum(el, 0.0) / 0.07))[..., None])
    sang = np.arccos(np.clip(d @ Sd, -1, 1))
    c = c + C["SunHaloColor"] * (0.35 * np.exp(-sang / max(math.radians(S["SunHaloSize"]) * 1.6, 1e-3)))[..., None]
    c = lerp(c, C["SunHaloColor"] * 1.25, sat(toward ** 5.0 * np.exp(-np.maximum(el, 0.0) / 0.06))[..., None])
    c = lerp(c, C["SunColor"] * 1.4, sat(toward ** 16.0 * np.exp(-np.maximum(el, 0.0) / 0.025))[..., None])
    c = np.where((el < 0)[..., None], lerp(C["HazeColor"], C["HorizonColor"] * 0.55, sat(-el / 0.35)[..., None]), c)
    sky_behind = c
    # --- SKY_PREP: the sun disc
    base = c.copy()
    sdx, sdy = ddxy(sang)
    sw = np.maximum(np.abs(sdx) + np.abs(sdy), 1e-5) * SS
    dR = math.radians(S["SunDiscSize"])
    disc = sat((dR - sang) / sw + 0.5)
    limb = 1.0 - 0.3 * sat(sang / max(dR, 1e-4)) ** 2
    base = lerp(base, C["SunColor"][None, None] * 1.4 * S["SunIntensity"] * limb[..., None], disc[..., None])

    # --- the layers (runtime model)
    cosv = d @ Sd
    ratio = np.minimum(hg2(cosv) / hg2(C_BAKE), 24.0)
    fade = smoothstep(-0.004, 0.04, dz)
    clear = base.copy()                                     # the sky with no cloud layers (for the metrics)
    cover = np.zeros(dz.shape, np.float32)                  # combined cloud opacity of both layers
    for L in LAYERS:
        Mm = TEX[L["key"]][0]
        hh = L["alt"] - CAMZ
        den = Rc * dz + np.sqrt(Rc * Rc * dz * dz + 2.0 * Rc * hh + hh * hh)
        t = (2.0 * Rc * hh + hh * hh) / den
        p = d[..., :2] * t[..., None]                       # camXY = 0
        az = math.radians(L["az"])
        aU = np.array([math.cos(az), math.sin(az)])
        aV = np.array([-aU[1], aU[0]])
        uv = np.stack([p @ aU - TIME * L["speed"], p @ aV], -1) / L["tile"] + np.array(L["offset"])
        dxu, dyu = ddxy(uv)
        br = fade > 0.0
        s4 = tex(Mm, uv[br], dxu[br], dyu[br])
        lt, al = s4[:, :3], s4[:, 3]
        ws = np.exp(-lt[:, 2] * TAUSCALE[L["key"]])
        sunk = lerp(1.0, ratio[br], ws)
        col = EXPOSURE_L[L["key"]] * (lt[:, 0:1] * SUN_TINT * sunk[:, None] + lt[:, 1:2] * AMB_TINT)
        col = lerp(col, sky_behind[br], (S["Haze"] * (1.0 - np.exp(-t[br] / 2.5e6)))[:, None])
        wgt = (al * fade[br])[:, None]
        base[br] = lerp(base[br], col, wgt)
        cover[br] = 1.0 - (1.0 - cover[br]) * (1.0 - wgt[:, 0])

    # --- sea (plain backdrop, as preview_deck.py)
    dep = np.maximum(-dz, 0.0)
    elb = np.arcsin(sat(dep))
    seaV = 1.0 - sat(elb / math.radians(28.0))
    sc = lerp(C["SeaDeepColor"], C["SeaHorizonColor"], seaV[..., None])
    sc = lerp(sc, lerp(C["SeaHorizonColor"], C["HazeColor"], 0.35), (sat(S["Haze"]) * np.exp(-elb / 0.006) * 0.5)[..., None])
    col = np.where((dz < 0)[..., None], sc, base) * S["SkyBrightness"]
    col = col.reshape(H, SS, W, SS, 3).mean(axis=(1, 3))
    clr = (np.where((dz < 0)[..., None], sc, clear) * S["SkyBrightness"]).reshape(H, SS, W, SS, 3).mean(axis=(1, 3))
    cov = cover.reshape(H, SS, W, SS).mean(axis=(1, 3))
    sdeg = np.degrees(sang).reshape(H, SS, W, SS).min(axis=(1, 3))
    up = (dz > 0).reshape(H, SS, W, SS).all(axis=(1, 3))
    return col, clr, cov, sdeg, up


def box_bg(img, k=41):
    """k x k box mean with edge padding (the review's 'local sky' for the streak measures)."""
    pad = np.pad(img, ((k // 2, k // 2), (k // 2, k // 2), (0, 0)), mode="edge")
    cs = np.pad(pad.cumsum(0).cumsum(1), ((1, 0), (1, 0), (0, 0)))
    return (cs[k:, k:] - cs[:-k, k:] - cs[k:, :-k] + cs[:-k, :-k]) / (k * k)


def lumv(x):
    return x[..., 0] * 0.2126 + x[..., 1] * 0.7152 + x[..., 2] * 0.0722


def metrics(img8, col, clr, cov, sdeg, up):
    """The review's measures on the upper 300 rows (8-bit sRGB / 255): 'pink' = R - B more than 0.06 above a 41-px box
    background, 'structured' = |mean RGB - background| > 0.08 (R7: 7-8 % and 0.5-1.5 %); cloud brightness against the
    clear sky behind it (linear luminance, pixels with combined cloud opacity > 0.3); clipped pixels (any sRGB channel
    >= 254) in the sky away from the sun disc."""
    f = img8[:300].astype(np.float32) / 255.0
    bg = box_bg(f)
    pink = float((((f[..., 0] - f[..., 2]) - (bg[..., 0] - bg[..., 2])) > 0.06).mean())
    struct = float((np.abs(f.mean(-1) - bg.mean(-1)) > 0.08).mean())
    cl = (cov > 0.3) & up
    r = lumv(col)[cl] / np.maximum(lumv(clr)[cl], 1e-6)
    away = up & (sdeg > 3.0)
    clr8 = to_srgb8(clr)
    hot = img8.max(-1) >= 254
    hot_clear = clr8.max(-1) >= 254
    clip = float((hot & ~hot_clear)[away].mean()) if away.any() else 0.0
    near = up & (sdeg > 3.0) & (sdeg < 25.0)
    clip_near = int((hot & ~hot_clear & near).sum())
    white = int(((img8.min(-1) >= 254) & ~(clr8.min(-1) >= 254) & near).sum())
    return ("pink %.3f structured %.3f | cloud px %.3f of sky, cloud/sky lum p50 %.2f p90 %.2f p99 %.2f | clipped by "
            "cloud %.4f of the sky (%d px 3-25 deg from the sun, %d of them pure white; the clear sky alone clips %d px "
            "there)" % (pink, struct, float(cl.sum() / max(1, up.sum())),
                        *(np.percentile(r, [50, 90, 99]) if r.size else (0, 0, 0)), clip, clip_near, white,
                        int((hot_clear & near).sum())))


def to_srgb8(col):
    col = col / (1 + col * 0.15)
    col = np.where(col <= 0.0031308, col * 12.92, 1.055 * np.power(np.clip(col, 0, None), 1 / 2.4) - 0.055)
    return (np.clip(col, 0, 1) * 255 + 0.5).astype(np.uint8)


def write_png(path, img8):
    o = oiio.ImageOutput.create(path)
    o.open(path, oiio.ImageSpec(img8.shape[1], img8.shape[0], img8.shape[2], oiio.UINT8))
    o.write_image(np.ascontiguousarray(img8))
    o.close()


for v in a.views.split(","):
    if v == "up":
        yaw, pitch, name = 30.0, 90.0, "up"
    else:
        yaw, pitch = (float(x) for x in v.split(":"))
        name = "y%03d_p%02d" % (yaw, pitch)
    col, clr, cov, sdeg, upm = render_view(yaw, pitch, a.width, a.height, a.ss)
    out = os.path.join(a.out, "layers_%s%s.png" % (name, a.tag))
    img8 = to_srgb8(col)
    write_png(out, img8)
    print("view", name, "yaw %.0f pitch %.0f" % (yaw, pitch), "->", out, flush=True)
    print("   ", metrics(img8, col, clr, cov, sdeg, upm), flush=True)

# ------------------------------------------------------------------------------------------------ seam checks (2 x 2)
if not a.no_seams:
    for L in LAYERS:
        _, light, alpha = TEX[L["key"]]
        al = alpha[..., 0]
        bg = C["MidColor"]
        look = EXPOSURE_L[L["key"]] * (light[..., 0:1] * SUN_TINT + light[..., 1:2] * AMB_TINT)
        img = bg * (1 - al[..., None]) + look * al[..., None]
        for nm, x in (("%sLight" % L["name"], img), ("%sAlpha" % L["name"], np.repeat(alpha, 3, -1) ** 2.2)):
            h = x[::2, ::2] if x.shape[0] > 1024 else x
            t2 = np.concatenate([np.concatenate([h, h], 1), np.concatenate([h, h], 1)], 0)
            out = os.path.join(a.out, "layers_tiled_%s%s.png" % (nm, a.tag))
            write_png(out, to_srgb8(t2 / (1.0 if "Alpha" in nm else 1.0)))
            print("seam check", out)
        for k, (nm, x) in enumerate((("sunY", light[..., 0]), ("skyY", light[..., 1]), ("tauN", light[..., 2]), ("alpha", al))):
            su = float(np.abs(x[:, 0] - x[:, -1]).mean())
            iu = float(np.abs(x[:, 1:] - x[:, :-1]).mean())
            sv = float(np.abs(x[0] - x[-1]).mean())
            iv = float(np.abs(x[1:] - x[:-1]).mean())
            print("seam %-6s %-5s  u-wrap %.5f (interior %.5f)  v-wrap %.5f (interior %.5f)" % (L["key"], nm, su, iu, sv, iv))
print("exposure", EXPOSURE_L, "tauscale", TAUSCALE)
