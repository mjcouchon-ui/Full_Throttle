"""Tileable cloud-deck texture for the dome's curved cloud shells (owner's references, 14 Nov: the Bayside ferris-wheel
sky, large wind-combed coral cloud masses with blue gaps, and sparse broken altocumulus puffs). Pure NumPy, periodic in U
and V. Texture +U runs along the wind.

The field is built as a mass hierarchy (plan 04, step #4): a few large macro masses, flow-warped streaks combed about
6.7:1 along the wind, and fine fibres only near the cloud edge. The shader thresholds R itself (crisp at any distance).
  R = continuous cloud field, cloud edge at R = 0.5 (coverage = fraction of R > 0.5)
  G = smooth thickness, reaching a little past the edge (the sunward second tap gives the lighting gradient)
  B = anisotropic fibres, 0..1
  A = sparse, clustered low cloudlets (periodic Worley puffs), continuous, edge at A = 0.5 (step #10)
Noise: periodic gradient noise (quintic fade, 1024 random unit vectors indexed by a wrapped permutation hash).
Run with Blender's Python (NumPy + OpenImageIO):
  "<Blender>/5.2/python/bin/python.exe" -I bake_deck.py [--size 2048] [--seed 7] [--coverage 0.42] -> bakes/Day/T_BSky_Day_Deck.png
"""
import os
import time
import argparse
import numpy as np
import OpenImageIO as oiio

ap = argparse.ArgumentParser()
ap.add_argument("--size", type=int, default=2048)
ap.add_argument("--seed", type=int, default=7)
ap.add_argument("--coverage", type=float, default=0.42)
ap.add_argument("--contrast", type=float, default=1.8, help="stretch of the field around the edge (1 = plan's raw field)")
ap.add_argument("--puff-lift", type=float, default=0.3, help="raises the cloudlets inside a cluster (A coverage)")
ap.add_argument("--no-cloudlets", action="store_true", help="A = 0 everywhere")
ap.add_argument("--out", default=os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "bakes", "Day", "T_BSky_Day_Deck.png"))
ap.add_argument("--channels-png", default="", help="optional 2x2 top-down visualisation of R, G, B, A (each 1024^2)")
a = ap.parse_args()
t_start = time.time()
N = a.size
rng = np.random.default_rng(a.seed)
PERM = rng.permutation(1024).astype(np.int32)
ang = rng.uniform(0.0, 2.0 * np.pi, 1024)
GRAD = np.stack([np.cos(ang), np.sin(ang)], -1).astype(np.float32)
JIT = rng.uniform(0.0, 1.0, (1024, 2)).astype(np.float32)


def phash(i, j, px, py):
    """Wrapped lattice hash: (i mod px, j mod py) -> 0..1023."""
    return PERM[(PERM[(i % px) & 1023] + (j % py)) & 1023]


def pnoise(x, y, px, py):
    """Gradient noise on an integer lattice that wraps every px cells in x and py in y (x, y in lattice units), about -1..1."""
    xf, yf = np.floor(x), np.floor(y)
    ix, iy = xf.astype(np.int32), yf.astype(np.int32)
    fx, fy = (x - xf).astype(np.float32), (y - yf).astype(np.float32)
    ux = fx * fx * fx * (fx * (fx * 6 - 15) + 10)
    uy = fy * fy * fy * (fy * (fy * 6 - 15) + 10)

    def g(di, dj):
        gv = GRAD[phash(ix + di, iy + dj, px, py)]
        return gv[..., 0] * (fx - di) + gv[..., 1] * (fy - dj)
    n00, n10, n01, n11 = g(0, 0), g(1, 0), g(0, 1), g(1, 1)
    a0 = n00 + (n10 - n00) * ux
    a1 = n01 + (n11 - n01) * ux
    return (a0 + (a1 - a0) * uy) * 1.4


def pfbm(u, v, fx, fy, octaves, gain=0.5, off=0.0):
    """Periodic fbm over the unit tile, remapped *0.5+0.5 (about 0..1). fx, fy: lattice cells across the tile at the first
    octave (integers); off and the per-octave shifts are whole lattice cells, so the tile stays periodic."""
    s = np.zeros(u.shape, np.float32)
    amp, norm = 1.0, 0.0
    for o in range(octaves):
        s += amp * pnoise(u * fx + off + o * 7.0, v * fy - off + o * 3.0, fx, fy)
        norm += amp
        amp *= gain
        fx, fy = fx * 2, fy * 2
    return s / norm * 0.5 + 0.5


SIZE = rng.uniform(0.55, 1.35, 1024).astype(np.float32)


def worley_f1(u, v, cx, cy):
    """Periodic Worley F1 (distance to the nearest feature point, in cell units) on a cx x cy cell grid, 3x3 search
    with wrapped neighbours. Each point's distance is divided by its own size, so puffs vary and big neighbours merge."""
    x, y = u * cx, v * cy
    ix, iy = np.floor(x).astype(np.int32), np.floor(y).astype(np.int32)
    best = np.full(u.shape, 9.0, np.float32)
    for dj in (-1, 0, 1):
        for di in (-1, 0, 1):
            hh = phash(ix + di, iy + dj, cx, cy) ^ 517
            jx = JIT[hh]
            px = ix + di + 0.15 + 0.7 * jx[..., 0]
            py = iy + dj + 0.15 + 0.7 * jx[..., 1]
            best = np.minimum(best, np.sqrt((x - px) ** 2 + (y - py) ** 2) / SIZE[hh])
    return best


def sstep(e0, e1, x):
    t = np.clip((x - e0) / (e1 - e0), 0.0, 1.0)
    return t * t * (3 - 2 * t)


c = ((np.arange(N) + 0.5) / N).astype(np.float32)
U, V = np.meshgrid(c, c)                      # U = columns = along the wind, V = rows = across

# --- R: the mass hierarchy
macro = pfbm(U, V, 2, 3, 3, 0.5)                                             # a few large masses
Vw = V + 0.08 * (pfbm(U, V, 2, 5, 3, off=31.0) * 2 - 1)                      # flow warp, mostly across the wind
Uw = U + 0.02 * (pfbm(U, V, 2, 5, 3, off=37.0) * 2 - 1)
streak = pfbm(Uw, Vw, 3, 20, 5, 0.48)                                         # wind-combed streaks, ~6.7:1
base = macro * 0.62 + streak * 0.30
thr = float(np.quantile(base, 1.0 - a.coverage))
edge = np.exp(-((base - thr) ** 2) / 0.01)
fine = pfbm(Uw, Vw, 12, 64, 3)
field = base + 0.12 * edge * (fine - 0.5)
thr = float(np.quantile(field, 1.0 - a.coverage))                             # re-taken on the final field: coverage stays exact
# edge contrast: the layered field only spans about +-0.2 around thr, so R would use 0.04..0.84 and the edge gradient
# would be shallow (soft edges at the shader's fixed width). Stretch the deviation from thr so R uses its full range.
field = thr + (field - thr) * a.contrast
R = np.clip((field - (thr - 0.25)) / 0.5, 0.0, 1.0)

# --- G: smooth thickness from the smooth base (no edge fibres), reaching a little past the edge
thrb = float(np.quantile(base, 1.0 - a.coverage))
core = float(np.quantile(base, 1.0 - a.coverage * 0.06))                     # only the thickest cores read as full
G = np.clip((base - thrb + 0.03) / (core - thrb + 0.03), 0.0, 1.0)
G = np.clip(np.sqrt(G) * (0.85 + 0.3 * (fine - 0.5)), 0.0, 1.0)              # quick rise past the edge, textured inside

# --- B: anisotropic fibres
B = pfbm(Uw, Vw, 16, 96, 3)

# --- A: sparse, clustered low cloudlets
Uc = U + 0.020 * (pfbm(U, V, 6, 8, 3, off=71.0) * 2 - 1)                      # warp: ragged, lumpy outlines
Vc = V + 0.026 * (pfbm(U, V, 6, 8, 3, off=73.0) * 2 - 1)
F1 = worley_f1(Uc, Vc, 12, 16)                                                # few, large cells (plan: 48 x 64)
puff = 1.0 - F1 / 0.55
ero = pfbm(Uc, Vc, 36, 48, 3, 0.55, off=61.0)                                 # 3 warped octaves of erosion
puff = puff - 0.9 * (ero - 0.5)
clus = pfbm(U, V, 3, 4, 3, off=53.0)
q82 = float(np.quantile(clus, 0.82))
mask = sstep(q82 - 0.03, q82 + 0.09, clus)                                   # puffs shrink toward a cluster's rim
A = np.clip((puff + a.puff_lift) * mask, 0.0, 1.0)
if a.no_cloudlets:
    A = np.zeros_like(A)

img = np.stack([R, G, B, A], -1)
img8 = (np.clip(img, 0, 1) * 255.0 + 0.5).astype(np.uint8)
os.makedirs(os.path.dirname(os.path.abspath(a.out)), exist_ok=True)
spec = oiio.ImageSpec(N, N, 4, oiio.UINT8)
spec.attribute("oiio:UnassociatedAlpha", 1)              # A is data, not opacity: never (un)premultiply RGB by it
out = oiio.ImageOutput.create(a.out)
out.open(a.out, spec)
out.write_image(img8)
out.close()
cfg = oiio.ImageSpec()
cfg.attribute("oiio:UnassociatedAlpha", 1)
back = oiio.ImageBuf(a.out, 0, 0, cfg).get_pixels(oiio.UINT8)
print("round trip max |diff| per channel", [int(np.abs(back[..., k].astype(int) - img8[..., k]).max()) for k in range(4)])
print("deck tile", os.path.abspath(a.out), N, "seed", a.seed, "coverage arg %.2f" % a.coverage)
print("coverage R>0.5 %.3f   A>0.5 %.3f   (field thr %.4f, R range %.2f..%.2f)" % (
    float((R > 0.5).mean()), float((A > 0.5).mean()), thr, float(R.min()), float(R.max())))
# seam check: across the U and V wrap vs the interior neighbour difference
for k, nm in enumerate("RGBA"):
    ch = img8[..., k].astype(np.float32) / 255.0
    su = np.abs(ch[:, 0] - ch[:, -1]).mean()
    sv = np.abs(ch[0] - ch[-1]).mean()
    iu = np.abs(ch[:, 1:] - ch[:, :-1]).mean()
    iv = np.abs(ch[1:] - ch[:-1]).mean()
    print("seam %s  u-wrap %.4f (interior %.4f)   v-wrap %.4f (interior %.4f)" % (nm, su, iu, sv, iv))

if a.channels_png:
    h = N // 2
    small = img[::N // h, ::N // h] if N >= 2 * h else img
    small = small[:h, :h]
    grid = np.zeros((2 * h, 2 * h, 3), np.float32)
    for k, (r0, c0) in enumerate(((0, 0), (0, h), (h, 0), (h, h))):
        grid[r0:r0 + h, c0:c0 + h] = small[..., k:k + 1]
    grid[h - 1:h + 1, :] = (1.0, 0.3, 0.3)
    grid[:, h - 1:h + 1] = (1.0, 0.3, 0.3)
    g8 = (np.clip(grid, 0, 1) * 255 + 0.5).astype(np.uint8)
    o = oiio.ImageOutput.create(a.channels_png)
    o.open(a.channels_png, oiio.ImageSpec(2 * h, 2 * h, 3, oiio.UINT8))
    o.write_image(g8)
    o.close()
    print("channels", a.channels_png)
print("baked in %.1f s" % (time.time() - t_start))
