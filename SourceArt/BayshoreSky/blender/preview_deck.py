"""Preview of the BayshoreSky dome before Unreal, ported line for line from the runtime shader (plan 04: GRAD, step #3's
curved shells, #4's in-shader threshold and wisps, #5's sun disc, #6's lighting, #8 at Time = 0, #10's cloudlets).
Pinhole camera about 6 m above sea level, 90 deg horizontal FOV, rendered 2x2 supersampled; texture taps are trilinear
from a wrapped mip pyramid with an anisotropic-style LOD (minor axis, at most 8:1), standing in for SampleGrad.
Run with Blender's Python (NumPy + OpenImageIO):
  preview_deck.py <deck.png> <out.png> [yaw_deg] [pitch_deg] [cam_x_m] [cam_y_m]
"""
import sys
import os
import numpy as np
import OpenImageIO as oiio

sys.path.insert(0, os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "unreal"))
from bsky_params import COLORS, SCALARS, VECTORS, srgb

# step #1 palette (used whether or not bsky_params.py already carries it)
STEP1_COLORS = {"CloudLitColor": "#F6A196", "CloudRimColor": "#FFD9CC", "CloudShadowColor": "#9A8DB8",
                "ZenithColor": "#5A80CC", "MidColor": "#94B2E6", "HorizonColor": "#F7DDD3", "HazeColor": "#F6D0C4",
                "SunHaloColor": "#FFE2CC", "SunColor": "#FFF6EA", "SeaDeepColor": "#188A95", "SeaHorizonColor": "#6BBFBE"}
STEP1_SCALARS = {"GradientMidPoint": 0.38, "CloudRimStrength": 0.6}

deck_path, out_path = sys.argv[1], sys.argv[2]
yaw = np.radians(float(sys.argv[3]) if len(sys.argv) > 3 else 30.0)
pitch = np.radians(float(sys.argv[4]) if len(sys.argv) > 4 else 18.0)
cam_x = float(sys.argv[5]) * 100.0 if len(sys.argv) > 5 else 0.0
cam_y = float(sys.argv[6]) * 100.0 if len(sys.argv) > 6 else 0.0
cols = dict(COLORS)
# (step #1 values now live in bsky_params.py)
C = {k: np.array(srgb(v), np.float32) for k, v in cols.items()}
S = dict(SCALARS)

TIME = 0.0
SPEED = S["CloudDriftSpeed"]
SUND = np.array(dict(VECTORS)["SunDirection"][:3])
WIND = np.array([1.0, 0.3])
Rc = 3.0e7
SS = 2
W, H, fov = 1920, 1080, np.radians(90.0)


def sat(x):
    return np.clip(x, 0.0, 1.0)


def smoothstep(e0, e1, x):
    t = sat((x - e0) / (e1 - e0))
    return t * t * (3 - 2 * t)


def lerp(a, b, t):
    return a + (b - a) * t


# --- texture: wrapped mip pyramid, trilinear, LOD from the derivative footprint
_cfg = oiio.ImageSpec()
_cfg.attribute("oiio:UnassociatedAlpha", 1)              # A is data (cloudlets): read RGB raw, not premultiplied
src = oiio.ImageBuf(deck_path, 0, 0, _cfg).get_pixels(oiio.FLOAT)
N = src.shape[0]
MIPS = [src.astype(np.float32)]
while MIPS[-1].shape[0] > 1:
    m = MIPS[-1]
    MIPS.append(0.25 * (m[0::2, 0::2] + m[1::2, 0::2] + m[0::2, 1::2] + m[1::2, 1::2]))


def bilinear(img, u, v):
    n = img.shape[0]
    x, y = u * n - 0.5, v * n - 0.5
    x0, y0 = np.floor(x), np.floor(y)
    fx, fy = (x - x0)[..., None], (y - y0)[..., None]
    x0, y0 = x0.astype(np.int64) % n, y0.astype(np.int64) % n
    x1, y1 = (x0 + 1) % n, (y0 + 1) % n
    a = img[y0, x0] + (img[y0, x1] - img[y0, x0]) * fx
    b = img[y1, x0] + (img[y1, x1] - img[y1, x0]) * fx
    return a + (b - a) * fy


def tex(uv, dx, dy):
    """Texture2DSampleGrad stand-in. uv (...,2): texture x = uv.x (columns, along the wind), y = uv.y (rows)."""
    lx = np.linalg.norm(dx, axis=-1) * N
    ly = np.linalg.norm(dy, axis=-1) * N
    major, minor = np.maximum(lx, ly), np.minimum(lx, ly)
    lod = np.clip(np.log2(np.maximum(np.maximum(minor, major / 8.0), 1e-8)), 0.0, len(MIPS) - 1.001)
    l0 = np.floor(lod).astype(int)
    out = np.zeros(uv.shape[:-1] + (4,), np.float32)
    for k in np.unique(l0):
        msk = l0 == k
        u, v = uv[msk, 0], uv[msk, 1]
        f = (lod[msk] - k)[..., None]
        s0 = bilinear(MIPS[k], u, v)
        s1 = bilinear(MIPS[k + 1], u, v)
        out[msk] = s0 + (s1 - s0) * f
    return out


def ddxy(a):
    """ddx/ddy over the supersampled grid (forward difference, last row/column repeated)."""
    dx = np.empty_like(a)
    dy = np.empty_like(a)
    dx[:, :-1] = a[:, 1:] - a[:, :-1]
    dx[:, -1] = dx[:, -2]
    dy[:-1] = a[1:] - a[:-1]
    dy[-1] = dy[-2]
    return dx, dy


def shell_t(h, dz):
    return (2.0 * Rc * h + h * h) / (Rc * dz + np.sqrt(Rc * Rc * dz * dz + 2.0 * Rc * h + h * h))


# --- camera
w2, h2 = W * SS, H * SS
xs = (np.arange(w2) + 0.5) / w2 * 2 - 1
ys = ((np.arange(h2) + 0.5) / h2 * 2 - 1) * H / W
X, Y = np.meshgrid(xs * np.tan(fov / 2), -ys * np.tan(fov / 2))
f = np.array([np.cos(pitch) * np.cos(yaw), np.cos(pitch) * np.sin(yaw), np.sin(pitch)])
r = np.array([np.sin(yaw), -np.cos(yaw), 0.0])
up = np.cross(r, f)
d = f[None, None] + X[..., None] * r[None, None] + Y[..., None] * up[None, None]
d /= np.linalg.norm(d, axis=-1, keepdims=True)

# --- GRAD
el = np.arcsin(np.clip(d[..., 2], -1, 1))
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
Sd = SUND / np.linalg.norm(SUND)
sang = np.arccos(np.clip(d @ Sd, -1, 1))
c = c + C["SunHaloColor"] * (0.35 * np.exp(-sang / max(np.radians(S["SunHaloSize"]) * 1.6, 1e-3)))[..., None]
c = lerp(c, C["SunHaloColor"] * 1.25, sat(toward ** 5.0 * np.exp(-np.maximum(el, 0.0) / 0.06))[..., None])
c = lerp(c, C["SunColor"] * 1.4, sat(toward ** 16.0 * np.exp(-np.maximum(el, 0.0) / 0.025))[..., None])
c = np.where((el < 0)[..., None], lerp(C["HazeColor"], C["HorizonColor"] * 0.55, sat(-el / 0.35)[..., None]), c)

# --- SKY_PREP (the baked Atmo strip is left out: base = c)
base = c.copy()
# step #5: the sun disc in the dome
sdx, sdy = ddxy(sang)
sw = np.maximum(np.abs(sdx) + np.abs(sdy), 1e-5) * SS          # fwidth at the output resolution
dR = np.radians(S["SunDiscSize"])
disc = sat((dR - sang) / sw + 0.5)
limb = 1.0 - 0.3 * sat(sang / max(dR, 1e-4)) ** 2
base = lerp(base, C["SunColor"][None, None] * 1.4 * S["SunIntensity"] * limb[..., None], disc[..., None])

rel = np.array([cam_x, cam_y, 600.0])
dz = d[..., 2]
fadeEl = smoothstep(-0.004, 0.04, dz)
wd = WIND / np.linalg.norm(WIND + np.array([1e-5, 0.0]))
wp = np.array([-wd[1], wd[0]])
sunT = np.array([Sd[:2] @ wd, Sd[:2] @ wp])
fwd = np.exp(-sang / 0.55)
sunSide = sat(dh @ sh) ** 2
# deck (step #3 shell)
h1 = max(250000.0 - rel[2], 20000.0)
den1 = Rc * dz + np.sqrt(Rc * Rc * dz * dz + 2.0 * Rc * h1 + h1 * h1)
t1 = (2.0 * Rc * h1 + h1 * h1) / den1
p1 = rel[:2] + d[..., :2] * t1[..., None]
uv1 = np.stack([p1 @ wd - TIME * SPEED, p1 @ wp], -1) / 2400000.0
# wisps (step #4 rotation and scale, step #8c at Time 0)
h2 = max(700000.0 - rel[2], 200000.0)
t2 = shell_t(h2, dz)
p2 = rel[:2] + d[..., :2] * t2[..., None]
q2 = np.stack([p2 @ wd - TIME * SPEED * 2.2, p2 @ wp], -1)
q2[..., 0] += np.sin(TIME * 0.02) * 40000.0
uv2 = np.stack([q2[..., 0] * 0.966 - q2[..., 1] * 0.259, q2[..., 0] * 0.259 + q2[..., 1] * 0.966], -1) \
    * np.array([0.6, 1.0]) / 9000000.0 + 0.37
# cloudlets (step #10)
h0 = max(90000.0 - rel[2], 8000.0)
t0 = shell_t(h0, dz)
p0 = rel[:2] + d[..., :2] * t0[..., None]
q0 = np.stack([p0 @ wd - TIME * SPEED * 0.6, p0 @ wp], -1)
uv0 = np.stack([q0[..., 0] * 0.8 - q0[..., 1] * 0.6, q0[..., 0] * 0.6 + q0[..., 1] * 0.8], -1) / 1400000.0 + 0.61
p0f = p0 + dh * 30000.0
q0f = np.stack([p0f @ wd - TIME * SPEED * 0.6, p0f @ wp], -1)
uv0f = np.stack([q0f[..., 0] * 0.8 - q0f[..., 1] * 0.6, q0f[..., 0] * 0.6 + q0f[..., 1] * 0.8], -1) / 1400000.0 + 0.61
# derivatives (outside the branch); fp in texels per OUTPUT pixel, as the runtime sees it
dx1, dy1 = ddxy(uv1)
dx2, dy2 = ddxy(uv2)
dx0, dy0 = ddxy(uv0)
fp = np.maximum(np.linalg.norm(dx1, axis=-1), np.linalg.norm(dy1, axis=-1)) * N * SS
fp0 = np.maximum(np.linalg.norm(dx0, axis=-1), np.linalg.norm(dy0, axis=-1)) * N * SS
kdW = sat(1.5 - fp * 2.0)
Rd = np.array([[0.9205, -0.3907], [0.3907, 0.9205]])
uvd = (uv1 - np.array([0.6, 0.15]) * (TIME * SPEED / 2400000.0)) @ Rd.T * 7.31 + 0.19
dxd, dyd = dx1 @ Rd.T * 7.31, dy1 @ Rd.T * 7.31

a1 = np.zeros_like(dz)
a2 = np.zeros_like(dz)
a0 = np.zeros_like(dz)
dc = np.zeros_like(c)
wc = np.zeros_like(c)
c0 = np.zeros_like(c)
br = fadeEl > 0.0                                                  # [branch] if (fadeEl > 0.0)
if br.any():
    k0 = tex(uv1[br], dx1[br], dy1[br])
    k1 = tex(uv1[br] + sunT * 0.006, dx1[br], dy1[br])
    kd = tex(uvd[br], dxd[br], dyd[br])
    k2 = tex(uv2[br], dx2[br], dy2[br])
    cb, fpb, kdWb, sangb, fwdb, ssb, fe = c[br], fp[br], kdW[br], sang[br], fwd[br], sunSide[br], fadeEl[br]
    slope = sat(0.5 + (k0[:, 1] - k1[:, 1]) * 6.0)
    # step #4 threshold, step #8b breathing at Time 0
    thr1 = 0.5 - (k2[:, 1] - 0.5) * 0.30
    thr1 = thr1 - 0.07 * np.sin(TIME * 0.035 + k0[:, 1] * 9.0) * k0[:, 1]
    fld = k0[:, 0] + (kd[:, 2] - 0.5) * 0.10 * kdWb * (1.0 - k0[:, 1])
    w = np.maximum(lerp(0.10, 0.025, sat(k0[:, 1] * 1.6)), 0.02 * fpb)
    dens = smoothstep(thr1 - w, thr1 + w, fld)
    a1b = dens * fe
    # step #3 optional grazing thickening
    mu1 = den1[br] / (Rc + h1)
    a1b = 1.0 - (1.0 - a1b) ** np.clip(0.25 / np.maximum(mu1, 0.02), 1.0, 4.0)
    # step #6 lighting
    lit = sat(0.6 * slope + 0.85 * (1.0 - k0[:, 1]) - 0.05 + (kd[:, 2] - 0.5) * 0.25 * kdWb)
    lit = lit * lit * (3.0 - 2.0 * lit)
    shC = lerp(C["CloudShadowColor"], cb, 0.2)
    dcb = shC + (C["CloudLitColor"] - shC) * lit[:, None]
    dcb = dcb + (C["CloudRimColor"] - dcb) * sat(dens * (1.0 - k0[:, 1]) * slope * (0.3 + 0.7 * ssb) * S["CloudRimStrength"])[:, None]
    fs = np.exp(-sangb / 0.25)
    thin = (1.0 - k0[:, 1]) * dens
    dcb = dcb + (dcb * 0.75 - dcb) * (fs * k0[:, 1])[:, None]
    dcb = dcb + C["CloudRimColor"] * (fs * thin * 1.4 + fwdb * 0.2 * (1.0 - k0[:, 1]))[:, None]
    dcb = dcb + (C["CloudShadowColor"] * 0.9 - dcb) * (sat(k0[:, 1] - 0.6) * (1.0 - ssb) * 0.5)[:, None]
    dcb = dcb + (C["HazeColor"] - dcb) * (sat(S["Haze"]) * (1.0 - np.exp(-t1[br] / 2500000.0)))[:, None]
    # wisps
    a2b = sat((k2[:, 2] - 0.6) * 3.5) * smoothstep(0.55, 0.2, k2[:, 1]) * 0.55 * fe
    wcb = lerp(lerp(C["CloudLitColor"][None], C["CloudRimColor"][None], (0.35 * ssb)[:, None]), C["HazeColor"][None], 0.2) + C["CloudRimColor"] * (0.5 * fs)[:, None]
    # cloudlets
    pa = tex(uv0[br], dx0[br], dy0[br])[:, 3]
    w0 = np.maximum(0.04, 0.02 * fp0[br])
    dzb = dz[br]
    a0b = smoothstep(0.5 - w0, 0.5 + w0, pa) * smoothstep(0.02, 0.06, dzb) * (1.0 - smoothstep(0.92, 0.97, dzb))
    pf = tex(uv0f[br], dx0[br], dy0[br])[:, 3]
    top = sat(0.5 + (pa - pf) * 5.0)
    lit0 = sat(0.15 + 0.6 * top + 0.45 * (1.0 - pa) + 0.2 * ssb)
    lo0 = lerp(C["CloudShadowColor"], cb, 0.15)
    hi0 = lerp(C["CloudLitColor"][None], C["CloudRimColor"][None], (0.15 + 0.35 * ssb)[:, None])
    c0b = lo0 + (hi0 - lo0) * lit0[:, None] + C["CloudRimColor"] * (fs * (1.0 - pa) * 0.8)[:, None]
    c0b = c0b + (C["HazeColor"] - c0b) * (sat(S["Haze"]) * (1.0 - np.exp(-t0[br] / 2500000.0)))[:, None]
    a1[br], a2[br], a0[br], dc[br], wc[br], c0[br] = a1b, a2b, a0b, dcb, wcb, c0b
base = base + (wc - base) * a2[..., None]
base = base + (dc - base) * a1[..., None]
base = base + (c0 - base) * (a0 * 0.9)[..., None]

# --- sea (plain backdrop: depth bands and step #7f's haze; no glitter or reflection in this preview)
dep = np.maximum(-dz, 0.0)
elb = np.arcsin(sat(dep))
seaV = 1.0 - sat(elb / np.radians(28.0))
sc = lerp(C["SeaDeepColor"], C["SeaHorizonColor"], seaV[..., None])
sc = lerp(sc, lerp(C["SeaHorizonColor"], C["HazeColor"], 0.35), (sat(S["Haze"]) * np.exp(-elb / 0.006) * 0.5)[..., None])
col = np.where((dz < 0)[..., None], sc, base) * S["SkyBrightness"]

# 2x2 box down to the output size, then the filmic-ish tone map + sRGB for viewing
col = col.reshape(H, SS, W, SS, 3).mean(axis=(1, 3))
col = col / (1 + col * 0.15)
col = np.where(col <= 0.0031308, col * 12.92, 1.055 * np.power(np.clip(col, 0, None), 1 / 2.4) - 0.055)
img = (np.clip(col, 0, 1) * 255 + 0.5).astype(np.uint8)
o = oiio.ImageOutput.create(out_path)
o.open(out_path, oiio.ImageSpec(W, H, 3, oiio.UINT8))
o.write_image(img)
o.close()
print("preview", out_path, "yaw %.0f pitch %.0f cam (%.0f, %.0f) m" % (np.degrees(yaw), np.degrees(pitch), cam_x / 100, cam_y / 100))
