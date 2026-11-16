"""The animated title logo's textures, cut and derived from the layers logo.html draws.

    python Tools/title/make_layers.py        (render_layers.ps1 renders the five L_<name>.png first, then runs this)

Input, beside this file: L_throttle.png L_star1.png L_star2.png L_full.png L_lines.png (1600x608, straight alpha, one
part of the logo each: logo.html?layer=<name>) and the flat T_TitleLogo.png (read only, for the check at the end).
Output, beside this file: eleven T_TitleLogo_<Name>.png and layers.json. Content/Python/ft_title_logo_import.py
imports them to /Game/UI/Title; FTTitleLogo.cpp draws them.

All positions are in logo px, the pixels of the 1600x608 flat canvas. A texture's "cover" is the logo-px rectangle it
is drawn over. The crops keep the flat texel grid (whole-number origins), so they register 1:1 with the flat logo.
Nothing here is random and nothing is redrawn: every pixel comes from the generator's own layers. Needs Pillow, numpy.

The "lit" textures (Star1Lit, Star2Lit, FullGlow, FullGlowIn, ThrottleLit) are for additive drawing: colour already
multiplied by alpha, on black, alpha 255. The others are straight alpha, like T_TitleLogo.
"""
import json
import os
import sys

import numpy as np
from PIL import Image

HERE = os.path.dirname(os.path.abspath(__file__))
W, H = 1600, 608
PINK = (255, 47, 168)        # the neon's own colour (logo.html), for the colour of fully transparent texels
SOFT_SIGMA = 12.0            # FullSoft: the slam's defocused copy
GLOW_SIGMA = 35.0            # FullGlow / FullGlowIn
FEATHER = 40                 # FullGlowIn: px over which the glow rises from 0 at the top and left edges of the logo
CANVAS = (960, 640)          # the glow canvas, logo px; the logo's origin is at CANVAS_ORG in it
CANVAS_ORG = (48, 112)
STRAIGHT = ('Throttle', 'Lines', 'Full', 'Star1', 'Star2', 'FullSoft')

# name, source layer or None (derived), PNG size, cover (x, y, w, h)
TABLE = [
    ('Throttle',    'throttle', (1600, 320), (0, 256, 1600, 320)),
    ('Lines',       'lines',    (1600, 128), (0, 480, 1600, 128)),
    ('Full',        'full',     (768, 416),  (48, 0, 768, 416)),
    ('Star1',       'star1',    (128, 128),  (370, 273, 128, 128)),
    ('Star2',       'star2',    (128, 128),  (1232, 342, 128, 128)),
    ('Star1Lit',    None,       (128, 128),  (370, 273, 128, 128)),
    ('Star2Lit',    None,       (128, 128),  (1232, 342, 128, 128)),
    ('FullSoft',    None,       (240, 160),  (-48, -112, 960, 640)),
    ('FullGlow',    None,       (240, 160),  (-48, -112, 960, 640)),
    ('FullGlowIn',  None,       (228, 132),  (0, 0, 912, 528)),
    ('ThrottleLit', None,       (800, 176),  (0, 256, 1600, 352)),
]
ORDER = ['Throttle', 'Star1', 'Star2', 'Full', 'Lines']     # the generator's draw order


def load(name):
    im = Image.open(os.path.join(HERE, name))
    if im.mode != 'RGBA' or im.size != (W, H):
        raise SystemExit('%s: expected RGBA %dx%d, got %s %s' % (name, W, H, im.mode, im.size))
    return np.asarray(im, dtype=np.uint8)


def save(name, a):
    a = np.ascontiguousarray(a, dtype=np.uint8)
    Image.fromarray(a, 'RGBA').save(os.path.join(HERE, 'T_TitleLogo_%s.png' % name), compress_level=9)


def crop(layer, name, cover):
    """The cover rectangle of a full-canvas layer; fails if the layer has any alpha outside it."""
    x, y, w, h = cover
    outside = layer[:, :, 3].copy()
    outside[y:y + h, x:x + w] = 0
    if outside.max() != 0:
        ys, xs = np.nonzero(outside)
        raise SystemExit('%s: alpha outside its crop %s (stray alpha in %d,%d..%d,%d, max %d)' % (
            name, cover, xs.min(), ys.min(), xs.max(), ys.max(), outside.max()))
    return layer[y:y + h, x:x + w].copy()


def lit(rgba):
    """RGB = round(RGB * A / 255), A = 255."""
    o = np.empty_like(rgba)
    o[:, :, :3] = np.round(rgba[:, :, :3].astype(np.float64) * rgba[:, :, 3:4] / 255.0)
    o[:, :, 3] = 255
    return o


def gauss(a, sigma):
    """Separable Gaussian blur of a float array (h, w, c); outside the canvas is zero (transparent black)."""
    r = int(np.ceil(4.0 * sigma))
    k = np.exp(-0.5 * (np.arange(-r, r + 1) / sigma) ** 2)
    k /= k.sum()
    for axis in (0, 1):
        pad = [(0, 0)] * a.ndim
        pad[axis] = (r, r)
        p = np.pad(a, pad)
        out = np.zeros_like(a)
        n = a.shape[axis]
        for i, kv in enumerate(k):
            sl = [slice(None)] * a.ndim
            sl[axis] = slice(i, i + n)
            out += kv * p[tuple(sl)]
        a = out
    return a


def resize(a, size, how):
    """Resize a float array (h, w, c) channel by channel, in float (no 8-bit rounding before the filter)."""
    return np.stack([np.asarray(Image.fromarray(np.ascontiguousarray(a[:, :, c], dtype=np.float32), 'F').resize(size, how),
                                dtype=np.float64) for c in range(a.shape[2])], axis=2)


def u8(a):
    return np.clip(np.round(a), 0, 255).astype(np.uint8)


def main():
    layers = dict((n, load('L_%s.png' % n)) for n in ('throttle', 'star1', 'star2', 'full', 'lines'))
    covers = dict((n, c) for n, _, _, c in TABLE)
    sizes = dict((n, s) for n, _, s, _ in TABLE)
    out = {}
    stats = {}

    # ---- the five crops, straight alpha, texel for texel
    for name, src, size, cover in TABLE:
        if src:
            out[name] = crop(layers[src], name, cover)

    # ---- lit stars
    out['Star1Lit'] = lit(out['Star1'])
    out['Star2Lit'] = lit(out['Star2'])

    # ---- "Full" on the glow canvas, premultiplied float
    cw, ch = CANVAS
    fx, fy = covers['Full'][0] + CANVAS_ORG[0], covers['Full'][1] + CANVAS_ORG[1]     # (96, 112)
    full = out['Full'].astype(np.float64)
    canvas = np.zeros((ch, cw, 4))
    canvas[fy:fy + full.shape[0], fx:fx + full.shape[1], :3] = full[:, :, :3] * full[:, :, 3:4] / 255.0
    canvas[fy:fy + full.shape[0], fx:fx + full.shape[1], 3] = full[:, :, 3]

    # FullSoft: blurred, quarter size, straight alpha. The resize is done on the premultiplied float image and the
    # colour divided out afterwards, so half-transparent texels do not pick up the colour of empty ones.
    soft = resize(gauss(canvas, SOFT_SIGMA), sizes['FullSoft'], Image.LANCZOS)
    a = np.clip(soft[:, :, 3], 0, 255)
    a8 = u8(a)
    rgb = np.empty(soft.shape[:2] + (3,))
    nz = a8 > 0
    rgb[nz] = np.clip(soft[:, :, :3][nz], 0, None) * 255.0 / a[nz][:, None]
    rgb[~nz] = PINK
    out['FullSoft'] = np.dstack([u8(rgb), a8])
    stats['FullSoft_border_alpha_max'] = int(max(a8[0].max(), a8[-1].max(), a8[:, 0].max(), a8[:, -1].max()))

    # FullGlow: the landing flare, lit, quarter size
    glow_full = gauss(canvas[:, :, :3], GLOW_SIGMA)
    g = u8(resize(glow_full, sizes['FullGlow'], Image.LANCZOS))
    border = int(max(g[0].max(), g[-1].max(), g[:, 0].max(), g[:, -1].max()))
    if border > 2:
        raise SystemExit('FullGlow: border value %d > 2, the canvas is too small for the blur' % border)
    stats['FullGlow_border_max_before_forcing'] = border
    g[0] = 0; g[-1] = 0; g[:, 0] = 0; g[:, -1] = 0
    out['FullGlow'] = np.dstack([g, np.full(g.shape[:2], 255, np.uint8)])

    # FullGlowIn: the breathing glow, kept inside the flat logo's rectangle: the same blur, cut to the canvas from the
    # logo's origin, feathered in from the top and left edges (where the rectangle would otherwise cut the halo hard)
    ox, oy = CANVAS_ORG
    gin = glow_full[oy:, ox:].copy()                                   # 528 rows x 912 columns
    if gin.shape[:2] != (covers['FullGlowIn'][3], covers['FullGlowIn'][2]):
        raise SystemExit('FullGlowIn: the crop is %s' % (gin.shape,))
    fy_ = np.clip(np.arange(gin.shape[0]) / float(FEATHER), 0, 1)[:, None, None]
    fx_ = np.clip(np.arange(gin.shape[1]) / float(FEATHER), 0, 1)[None, :, None]
    gi = u8(resize(gin * fy_ * fx_, sizes['FullGlowIn'], Image.LANCZOS))
    stats['FullGlowIn_border_max_before_forcing'] = {'top': int(gi[0].max()), 'left': int(gi[:, 0].max()),
                                                     'bottom': int(gi[-1].max()), 'right': int(gi[:, -1].max())}
    if max(gi[-1].max(), gi[:, -1].max()) > 2:
        raise SystemExit('FullGlowIn: right/bottom border value %d > 2' % max(gi[-1].max(), gi[:, -1].max()))
    gi[0] = 0; gi[-1] = 0; gi[:, 0] = 0; gi[:, -1] = 0
    out['FullGlowIn'] = np.dstack([gi, np.full(gi.shape[:2], 255, np.uint8)])

    # ThrottleLit: white light masked to the chrome and the underlines, half size
    x, y, w, h = covers['ThrottleLit']
    at = layers['throttle'][y:y + h, x:x + w, 3].astype(np.float64) / 255.0
    al = layers['lines'][y:y + h, x:x + w, 3].astype(np.float64) / 255.0
    grey = (1.0 - (1.0 - at) * (1.0 - al)) * 255.0
    gl = u8(resize(grey[:, :, None], sizes['ThrottleLit'], Image.BOX))[:, :, 0]
    out['ThrottleLit'] = np.dstack([gl, gl, gl, np.full(gl.shape, 255, np.uint8)])

    # ---- write
    for name, _, size, cover in TABLE:
        a = out[name]
        if (a.shape[1], a.shape[0]) != size:
            raise SystemExit('%s: built %dx%d, the table says %dx%d' % (name, a.shape[1], a.shape[0], size[0], size[1]))
        save(name, a)

    # ---- check: the five crops, composited at their covers in the generator's order, against the flat logo
    flat = load('T_TitleLogo.png').astype(np.float64)
    worst = 0.0
    for bg in ((13, 10, 23), (230, 222, 199)):
        dst = np.empty((H, W, 3))
        dst[:] = bg
        for name in ORDER:
            x, y, w, h = covers[name]
            s = out[name].astype(np.float64)
            sa = s[:, :, 3:4] / 255.0
            dst[y:y + h, x:x + w] = s[:, :, :3] * sa + dst[y:y + h, x:x + w] * (1.0 - sa)
        fa = flat[:, :, 3:4] / 255.0
        ref = flat[:, :, :3] * fa + np.array(bg, dtype=np.float64) * (1.0 - fa)
        d = np.abs(dst - ref)
        worst = max(worst, d.max())
        print('layers over %s against the flat logo: max %.3f of 255, mean %.5f, px over 1: %d, over 2: %d' % (
            bg, d.max(), d.mean(), int((d.max(axis=2) > 1).sum()), int((d.max(axis=2) > 2).sum())))
    if worst > 4.0:
        raise SystemExit('the layers do not add up to T_TitleLogo.png (max difference %.2f of 255)' % worst)

    with open(os.path.join(HERE, 'layers.json'), 'w', newline='\n') as f:
        json.dump({
            'about': 'The animated title logo\'s textures (make_layers.py). cover = x, y, w, h in logo px (the 1600x608 '
                     'flat canvas of T_TitleLogo.png). For the record: the game does not read this file, '
                     'FTTitleLogo.cpp carries the same table.',
            'canvas': [W, H],
            'draw_order': ['T_TitleLogo_' + n for n in ORDER],
            'textures': [{'name': 'T_TitleLogo_' + n, 'asset': '/Game/UI/Title/T_TitleLogo_' + n, 'size': list(s),
                          'cover': list(c),
                          'alpha': 'straight' if n in STRAIGHT else 'lit: RGB premultiplied on black, A 255, for additive drawing'}
                         for n, _, s, c in TABLE],
            'constants': {'soft_sigma': SOFT_SIGMA, 'glow_sigma': GLOW_SIGMA, 'feather': FEATHER,
                          'glow_canvas': list(CANVAS), 'glow_canvas_origin': list(CANVAS_ORG)},
            'measured': stats,
        }, f, indent=2)
        f.write('\n')
    print('wrote %d textures and layers.json in %s' % (len(TABLE), HERE))


if __name__ == '__main__':
    sys.exit(main())
