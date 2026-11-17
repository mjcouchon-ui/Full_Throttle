"""Enemy looks for the brawl: colour tables applied in the sprite material (M_BrawlEnemyV).
One table per character: up to 4 ZONES (a range of hue / saturation / value in the painted frame) and 6 LOOKS
(look 0 = as drawn; each other look gives every zone a change). The same arithmetic is in the material's custom
node (Content/Python/brawl_variety.py) and in preview() here.
  python looks.py preview [Who ...]   -> preview_<Who>.png (6 looks x 3 drawings)
  python looks.py inl <out.inl>       -> the C++ table
"""
import sys
import numpy as np
from PIL import Image, ImageDraw
SRC = 'C:/ftwork/final/brawl/src'


# zone = (hue centre, hue half width, sat min, sat max, val min, val max)
def Z(hc, hw, smin=0.0, smax=1.0, vmin=0.0, vmax=1.0):
    return (hc, hw, smin, smax, vmin, vmax)


SKIN = Z(0.050, 0.045, 0.34, 0.80, 0.70, 1.0)
DARK = Z(0.77, 0.10, 0.14, 0.49, 0.125, 0.62)     # near-black cloth (less saturated than the ink line)
LIGHT = Z(0.0, 0.5, 0.0, 0.24, 0.72, 1.0)         # white / cream cloth
MID = Z(0.0, 0.5, 0.0, 0.22, 0.36, 0.72)          # grey cloth


# change = (absolute?, hue (target or shift), sat mul, sat add, val mul)
def shift(dh, sm=1.0, vm=1.0):
    return (0, dh, sm, 0.0, vm)


def to(h, sm=1.0, sa=0.0, vm=1.0):
    return (1, h, sm, sa, vm)


SAME = shift(0.0)
SKINS = {  # natural tones only: a step of value and saturation, hue kept within the skin range
    'asdrawn': SAME, 'tan': shift(-0.004, 1.12, 0.86), 'brown': shift(-0.010, 1.20, 0.66),
    'deep': shift(-0.014, 1.16, 0.47), 'light': shift(0.006, 0.80, 1.0)}
DARKS = [to(0.62, 1.5, 0.05, 1.45), to(0.98, 1.6, 0.08, 1.45), to(0.42, 1.2, 0.02, 1.35), to(0.75, 0.25, 0.0, 1.7), to(0.06, 1.5, 0.06, 1.4)]
LIGHTS = [to(0.58, 1.0, 0.24, 0.97), to(0.07, 1.0, 0.22, 1.0), to(0.42, 1.0, 0.22, 0.97), to(0.80, 1.0, 0.20, 0.98), to(0.14, 1.0, 0.30, 1.0)]
MIDS = [to(0.60, 1.0, 0.30, 0.95), to(0.08, 1.0, 0.34, 1.0), to(0.27, 1.0, 0.30, 0.95), to(0.76, 1.0, 0.26, 0.92), to(0.98, 1.0, 0.34, 0.9)]
G1 = [shift(0.33, 0.78, 0.92), shift(0.58), shift(-0.20), shift(0.12), shift(0.45)]
G2 = [shift(0.50), shift(0.17), shift(-0.33), shift(0.70), shift(0.30)]
HAIR = [shift(0.22), shift(-0.30), shift(0.60), shift(0.40, 0.25, 0.95), shift(-0.12)]
SK = ['brown', 'tan', 'deep', 'asdrawn', 'brown']

# per character: zones in order (kind, zone); kinds: skin | g1 | g2 | dark | light | mid | hair
C = {
    'Punk': [('skin', SKIN), ('hair', Z(0.915, 0.045, 0.5, 1, 0.5, 1)), ('g1', Z(0.61, 0.05, 0.2, 0.62, 0.45, 1))],
    'Biker': [('skin', SKIN), ('g1', Z(0.61, 0.05, 0.2, 0.62, 0.35, 1)), ('g2', Z(0.985, 0.03, 0.6, 1, 0.45, 1)), ('dark', Z(0.84, 0.05, 0.12, 0.42, 0.22, 0.55))],
    'Sleaze': [('skin', SKIN), ('g1', Z(0.505, 0.055, 0.35, 1, 0.3, 1)), ('g2', Z(0.93, 0.045, 0.5, 1, 0.5, 1))],
    'Bouncer': [('skin', SKIN), ('dark', DARK)],
    'Guard': [('skin', SKIN), ('g1', Z(0.74, 0.055, 0.42, 1, 0.30, 1))],
    'Dealer': [('skin', SKIN), ('dark', Z(0.80, 0.06, 0.18, 0.50, 0.22, 0.62)), ('g2', Z(0.985, 0.03, 0.6, 1, 0.5, 1)), ('g1', Z(0.56, 0.06, 0.4, 1, 0.5, 1))],
    'Dock': [('skin', Z(0.050, 0.040, 0.34, 0.74, 0.70, 1.0)), ('g1', Z(0.60, 0.05, 0.2, 0.62, 0.4, 1)), ('g2', Z(0.093, 0.02, 0.76, 1, 0.7, 1))],
    'Disco': [('skin', SKIN), ('light', LIGHT), ('g1', Z(0.52, 0.06, 0.4, 1, 0.5, 1))],
    'Hustler': [('skin', SKIN), ('g1', Z(0.945, 0.04, 0.5, 1, 0.3, 1)), ('g2', Z(0.806, 0.045, 0.2, 0.58, 0.7, 1))],
    'Barman': [('light', Z(0.0, 0.5, 0.0, 0.15, 0.72, 1.0)), ('g2', Z(0.93, 0.045, 0.5, 1, 0.5, 1))],
    'Roller': [('skin', Z(0.035, 0.040, 0.34, 0.80, 0.70, 1.0)), ('g1', Z(0.123, 0.028, 0.62, 1, 0.6, 1)), ('g2', Z(0.653, 0.045, 0.3, 0.75, 0.3, 0.8)), ('hair', Z(0.52, 0.05, 0.4, 1, 0.5, 1))],
    'Muscle': [('dark', Z(0.785, 0.09, 0.14, 0.50, 0.13, 0.62))],
    'Firedancer': [('skin', SKIN), ('g1', Z(0.968, 0.03, 0.62, 1, 0.5, 1))],
    'Surfer': [('g1', Z(0.517, 0.05, 0.5, 1, 0.4, 1)), ('g2', Z(0.139, 0.03, 0.6, 1, 0.7, 1))],
    'Tourist': [('g1', Z(0.135, 0.03, 0.7, 1, 0.7, 1)), ('g2', Z(0.904, 0.045, 0.5, 1, 0.5, 1))],
    'Pitcook': [('g1', Z(0.945, 0.05, 0.5, 1, 0.5, 1)), ('mid', Z(0.0, 0.5, 0.0, 0.24, 0.36, 0.72))],
    'Pinsetter': [('skin', Z(0.078, 0.022, 0.40, 0.80, 0.70, 1.0)), ('mid', MID), ('g1', Z(0.005, 0.025, 0.66, 1, 0.5, 1))],
    'Leaguer': [('skin', SKIN), ('g1', Z(0.514, 0.05, 0.5, 1, 0.5, 1)), ('dark', Z(0.765, 0.08, 0.2, 0.50, 0.14, 0.62))],
    'Shoeclerk': [('skin', SKIN), ('g1', Z(0.112, 0.028, 0.7, 1, 0.6, 1))],
    'Lanehog': [('skin', SKIN), ('g1', Z(0.93, 0.045, 0.5, 1, 0.5, 1)), ('mid', MID)],
    'Skibum': [('skin', SKIN), ('g1', Z(0.915, 0.04, 0.5, 1, 0.5, 1)), ('g2', Z(0.615, 0.05, 0.5, 1, 0.5, 1))],
    'Linemate': [('skin', SKIN), ('g1', Z(0.978, 0.03, 0.55, 1, 0.5, 1)), ('dark', Z(0.765, 0.08, 0.2, 0.50, 0.15, 0.62))],
    'Boarder': [('skin', SKIN), ('g1', Z(0.53, 0.045, 0.5, 1, 0.4, 1)), ('g2', Z(0.784, 0.045, 0.45, 1, 0.45, 1)), ('mid', Z(0.0, 0.5, 0.0, 0.16, 0.45, 0.80))],
    'Goalie': [('g1', Z(0.973, 0.03, 0.6, 1, 0.5, 1)), ('g2', Z(0.70, 0.045, 0.4, 1, 0.28, 0.75))],
    'Deckhand': [('skin', Z(0.097, 0.022, 0.52, 0.80, 0.70, 1.0)), ('g1', Z(0.645, 0.045, 0.4, 1, 0.3, 0.8)), ('g2', Z(0.985, 0.018, 0.72, 1, 0.6, 1))],
    'Firstmate': [('skin', SKIN), ('g1', Z(0.765, 0.055, 0.36, 0.80, 0.25, 0.62))],
    'Waiter': [('g1', Z(0.765, 0.055, 0.36, 0.80, 0.25, 0.62)), ('g2', Z(0.925, 0.045, 0.6, 1, 0.5, 1))],
    'Tuxbrute': [('skin', SKIN), ('dark', DARK), ('g2', Z(0.985, 0.03, 0.6, 1, 0.5, 1))],
}
OVERRIDE = {}   # (who, look 1..5, kind) -> change
SKIN_ORDER = {}  # who -> list of 5 skin names (default SK)


def change(who, kind, look):
    """look 1..5"""
    if (who, look, kind) in OVERRIDE:
        return OVERRIDE[(who, look, kind)]
    i = look - 1
    if kind == 'skin':
        return SKINS[SKIN_ORDER.get(who, SK)[i]]
    return {'dark': DARKS, 'light': LIGHTS, 'mid': MIDS, 'g1': G1, 'g2': G2, 'hair': HAIR}[kind][i]


def rules(who, look):
    """-> 4 x (A, B, C); A=(hc,hw,smin,smax) B=(vmin,vmax,abs,hue) C=(satMul,satAdd,valMul,on)"""
    out = []
    for kind, z in C[who]:
        ch = SAME if look == 0 else change(who, kind, look)
        on = 0.0 if ch == SAME else 1.0
        out.append(((z[0], z[1], z[2], z[3]), (z[4], z[5], float(ch[0]), ch[1]), (ch[2], ch[3], ch[4], on)))
    while len(out) < 4:
        out.append(((0, 0, 0, 0), (0, 0, 0, 0), (1, 0, 1, 0)))
    return out


# ---- the arithmetic (mirrors the material) ----
def rgb2hsv(a):
    r, g, b = a[..., 0], a[..., 1], a[..., 2]
    mx = a.max(-1)
    mn = a.min(-1)
    d = mx - mn
    dd = np.where(d > 1e-6, d, 1)
    h = np.where(mx == r, (g - b) / dd, np.where(mx == g, 2 + (b - r) / dd, 4 + (r - g) / dd))
    h = np.where(d > 1e-6, (h / 6) % 1, 0)
    s = np.where(mx > 1e-6, d / np.where(mx > 1e-6, mx, 1), 0)
    return h, s, mx


def hsv2rgb(h, s, v):
    def f(n):
        k = (n + h * 6) % 6
        return v - v * s * np.clip(np.minimum(k, 4 - k), 0, 1)
    return np.stack([f(5), f(3), f(1)], -1)


def ss(a, b, x):
    t = np.clip((x - a) / max(b - a, 1e-5), 0, 1)
    return t * t * (3 - 2 * t)


def apply(rgb, rs):
    h, s, v = rgb2hsv(rgb)
    out = rgb.copy()
    for A, B, Cc in rs:
        if Cc[3] < 0.5:
            continue
        hc, hw, smin, smax = A
        vmin, vmax, ab, hue = B
        sm, sa, vm, _ = Cc
        d = np.abs((h - hc + 0.5) % 1 - 0.5)
        w = ((1 - ss(hw, hw * 1.5 + 0.01, d)) * ss(smin - 0.06, smin, s) * (1 - ss(smax, smax + 0.06, s))
             * ss(vmin - 0.06, vmin, v) * (1 - ss(vmax, vmax + 0.06, v)))
        nh = (hue if ab > 0.5 else h + hue) % 1
        c = hsv2rgb(nh * np.ones_like(h), np.clip(s * sm + sa, 0, 1), np.clip(v * vm, 0, 1))
        out = out + (c - out) * w[..., None]
    return out


def preview(who, frames=('idle', 'punch', 'hurt'), h=300):
    cells = []
    for look in range(6):
        row = []
        for f in frames:
            try:
                im = np.asarray(Image.open(f'{SRC}/{who}/{f}.png').convert('RGBA')).astype(np.float32) / 255
            except Exception:
                continue
            rgb = apply(im[..., :3], rules(who, look))
            o = Image.fromarray((np.concatenate([rgb, im[..., 3:]], -1) * 255 + 0.5).astype(np.uint8), 'RGBA')
            o = o.crop(o.getbbox())
            k = h / o.height
            o = o.resize((max(1, int(o.width * k)), h), Image.LANCZOS)
            row.append(o)
        cells.append(row)
    W = max(sum(o.width + 8 for o in row) for row in cells)
    sheet = Image.new('RGBA', (W * 3 + 8, 2 * (h + 22)), (118, 118, 124, 255))
    for i, row in enumerate(cells):
        x = (i % 3) * W + 6
        y = (i // 3) * (h + 22)
        ImageDraw.Draw(sheet).text((x, y + 2), f'{who} look {i}', fill=(255, 255, 255, 255))
        for o in row:
            sheet.alpha_composite(o, (x, y + 18))
            x += o.width + 8
    sheet.convert('RGB').save(f'preview_{who}.png')
    return sheet.size


def fnum(x):
    t = '%.4g' % x
    return t + ('f' if ('.' in t or 'e' in t) else '.f')


def inl(path):
    L = ['// Enemy looks: colour tables for M_BrawlEnemyV. Written by Content/Python/brawl_variety_looks.py',
         '// ("python brawl_variety_looks.py inl <this file>"). Do not edit by hand: change the script and write it again.',
         '// Per character 6 looks x 4 rules x 3 vectors: A = hue centre, hue half width, sat min, sat max;',
         '// B = value min, value max, absolute hue?, hue (target or shift); C = sat mul, sat add, value mul, on.',
         'struct FFTBrawlLookTable { const TCHAR* Who; float V[6][4][3][4]; };',
         'static const FFTBrawlLookTable GFTBrawlLooks[] = {']
    for who in C:
        L.append('\t{ TEXT("%s"), {' % who)
        for look in range(6):
            rs = rules(who, look)
            L.append('\t\t{ ' + ', '.join('{ ' + ', '.join('{ ' + ', '.join(fnum(x) for x in vec) + ' }' for vec in r) + ' }' for r in rs) + ' },')
        L.append('\t} },')
    L.append('};')
    open(path, 'w', encoding='utf-8', newline='\n').write('\n'.join(L) + '\n')


if __name__ == '__main__':
    if sys.argv[1] == 'preview':
        for w in (sys.argv[2:] or list(C)):
            print(w, preview(w))
    elif sys.argv[1] == 'inl':
        inl(sys.argv[2])
        print('wrote', sys.argv[2], len(C), 'characters')
