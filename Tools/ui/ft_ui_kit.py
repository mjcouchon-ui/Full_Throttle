# -*- coding: utf-8 -*-
"""
FULL THROTTLE - the UI kit ("THE STUDIO LOT, INKED").

One file, Pillow only, the four fonts of the repo only. It is two things:

1. The game's UI texture generator.
       python ft_ui_kit.py textures [out folder]     every texture of KIT.md as PNG (+ kit_manifest.json);
                                                     without a folder: SourceArt/UI/Kit of this checkout
       python ft_ui_kit.py sheet <tex folder> <jpg>  a contact sheet of a generated folder
       python ft_ui_kit.py manifest                  the manifest as JSON on stdout
   It lives at Tools/ui/ft_ui_kit.py and writes to SourceArt/UI/Kit/ (the folder Content/Python/ft_ui_kit_import.py
   imports to /Game/UI/Kit). The output is the same on every run (seeded).

2. A module for mock-ups (import ft_ui_kit as kit). Screens are plain 1920 x 1080 RGBA PIL images; every helper
   takes the image first and 1080p pixels after it, and draws only what the game's canvas can draw: boxes,
   textures (stretched, tiled, sliced, tinted, rotated about a pivot), live text in Bungee and Rajdhani.
   See the section HELPERS of KIT.md.

Texture pixels are 1080p pixels (the game's 1280 x 720 design canvas draws a texture at size / 1.5).
All art here is drawn by code. No outside image, no brand, no person.
"""
import os
import sys
import math
import json
import random
from PIL import Image, ImageDraw, ImageFont, ImageFilter, ImageChops

HERE = os.path.dirname(os.path.abspath(__file__))
SS = 4                      # supersampling of the generator (textures are box-reduced to 1x)


def _find_repo():
    """The four fonts live in the game's checkout. In the repo this file is Tools/ui/ft_ui_kit.py."""
    cands = [os.environ.get("FT_REPO", ""), os.path.normpath(os.path.join(HERE, "..", "..")),
             "K:/Full Throttle/UE/full-throttle-unreal"]
    for c in cands:
        if c and os.path.isfile(os.path.join(c, "Tools", "title", "Rajdhani-Bold.ttf")):
            return c.replace("\\", "/")
    raise SystemExit("ft_ui_kit: the repo's fonts were not found; set FT_REPO to the checkout")


REPO = _find_repo()
# where the textures go by default: SourceArt/UI/Kit of the project folder this file is in (Tools/ui/ -> ../..)
KIT_OUT = os.path.normpath(os.path.join(HERE, "..", "..", "SourceArt", "UI", "Kit"))
FONTS = {
    "BUN": REPO + "/SourceArt/BayshoreSky/fonts/Bungee/Bungee-Regular.ttf",    # display: titles, buttons, plaques
    "RAJ": REPO + "/Tools/title/Rajdhani-Bold.ttf",                             # body and labels
    "ORB": REPO + "/Tools/title/Orbitron-900.ttf",                              # (legacy; not used by the kit)
    "DAF": REPO + "/Tools/title/MrDafoe-Regular.ttf",                           # script, baked only
}

# ====================================================================== palette
PAL = {
    "Ink": (0x16, 0x12, 0x1E), "Slate": (0x1B, 0x18, 0x24), "Slate2": (0x2C, 0x28, 0x42), "Steel": (0x42, 0x3D, 0x5E),
    "Mute": (0xA0, 0x9C, 0xBA), "Chalk": (0xF8, 0xF3, 0xE7), "Paper": (0xF4, 0xE8, 0xCE), "PaperShade": (0xDF, 0xCE, 0xAA),
    "Manila": (0xE9, 0xC6, 0x82), "Aqua": (0xCB, 0xF0, 0xE9), "Sticky": (0xFF, 0xE0, 0x78), "Ledger": (0xE8, 0xF2, 0xDE),
    "LedgerBar": (0xCD, 0xE4, 0xC4), "Red": (0xEE, 0x3C, 0x3E), "Pink": (0xFF, 0x4F, 0xA3), "Cyan": (0x49, 0xE2, 0xD5),
    "Teal": (0x06, 0x78, 0x76), "Amber": (0xFF, 0xC4, 0x4C), "AmberInk": (0xA0, 0x5C, 0x00), "Green": (0x76, 0xEA, 0xA0),
    "GreenInk": (0x14, 0x84, 0x50), "White": (0xFF, 0xFF, 0xFF),
    "CycTop": (0xB8, 0xA8, 0xDE), "CycMid": (0xE2, 0xB8, 0xD6), "CycLow": (0xFF, 0xD0, 0xB0), "CycFloor": (0xF7, 0xBE, 0xA0),
    "CycShade": (0x8E, 0x7C, 0xC0),
    "Rate0": (0xEE, 0x3C, 0x3E), "Rate1": (0x24, 0x54, 0xC4), "Rate2": (0xD4, 0x5C, 0x00), "Rate3": (0xC0, 0x1C, 0x78),
    "BranchDem": (0xFF, 0x6B, 0x4A), "BranchWhl": (0x49, 0xE2, 0xD5), "BranchStn": (0xFF, 0x4F, 0xA3),
    "BranchCar": (0xFF, 0xC4, 0x4C), "BranchGet": (0x76, 0xEA, 0xA0),
    "Shoot1": (0xFF, 0x80, 0x8E), "Shoot2": (0x96, 0xE2, 0x60), "Shoot3": (0xFF, 0xA8, 0x54), "Shoot4": (0xF2, 0x54, 0x3C),
    "Shoot5": (0xFF, 0x6E, 0xC4), "Shoot6": (0x8C, 0xD0, 0xFF), "Shoot7": (0x40, 0xCC, 0xCC), "Shoot8": (0xA8, 0x8A, 0xFF),
}
INK, SLATE, SLATE2, STEEL, MUTE, CHALK = (PAL[k] for k in ("Ink", "Slate", "Slate2", "Steel", "Mute", "Chalk"))
PAPER, PAPER_D, MANILA, AQUA, STICKY, LEDGER, LEDGER_D = (PAL[k] for k in ("Paper", "PaperShade", "Manila", "Aqua", "Sticky", "Ledger", "LedgerBar"))
RED, PINK, CYAN, TEAL, AMBER, AMBER_INK, GREEN, GREEN_INK, WHITE = (PAL[k] for k in ("Red", "Pink", "Cyan", "Teal", "Amber", "AmberInk", "Green", "GreenInk", "White"))
CYC_TOP, CYC_MID, CYC_LOW, CYC_FLOOR, CYC_SHADE = (PAL[k] for k in ("CycTop", "CycMid", "CycLow", "CycFloor", "CycShade"))

SHOOTS = [("BAYSHORE", "THE HIGHWAY ASSASSINATION"), ("CHEMICAL PLANT", "THE FORMULA"), ("NEVADA DESERT", "ALL IN"),
          ("VOLCANO", "BAPTISM OF FIRE"), ("THEME PARK", "THE LAST RIDE"), ("ICE MOUNTAIN", "WHITE OUT"),
          ("UNDER THE SEA", "SINK OR SWIM"), ("CITY CENTER", "HOSTILE TAKEOVER")]
BOSSES = ["JUGGERNAUT", "ALCHEMIST", "BIG DEALER", "ENGINE 13", "MR. CHUCKLES", "ABOMINABLE", "GREAT WHITE", "CHAIRMAN"]
BOSS_HAND = ["the Juggernaut", "the Alchemist", "the Big Dealer", "Engine 13", "Mr. Chuckles", "the Abominable",
             "the Great White", "the Chairman"]
RATINGS = ["B-MOVIE", "ACTION HERO", "BLOCKBUSTER", "LEGENDARY DIRECTOR"]
BRANCHES = [("DEMOLITION", "burst", "BranchDem"), ("WHEELMAN", "wheel", "BranchWhl"), ("STUNT DOUBLE", "helmet", "BranchStn"),
            ("STAR CAR", "star", "BranchCar"), ("GETAWAY", "flag", "BranchGet")]


def col(c, a=None):
    """a palette name or an (r, g, b[, a]) tuple -> (r, g, b) or, with a (0..1), (r, g, b, A)"""
    if isinstance(c, str):
        c = PAL[c]
    if a is None:
        return tuple(c)
    return (c[0], c[1], c[2], int(round(max(0.0, min(1.0, a)) * 255)))


def rgba(c, a=None):
    """-> (r, g, b, A) with A 0..255; a (0..1) overrides"""
    if isinstance(c, str):
        c = PAL[c]
    if a is not None:
        return (c[0], c[1], c[2], int(round(max(0.0, min(1.0, a)) * 255)))
    return tuple(c) if len(c) == 4 else (c[0], c[1], c[2], 255)


def mix(a, b, t):
    a, b = col(a), col(b)
    return tuple(int(round(a[i] + (b[i] - a[i]) * t)) for i in range(3))


def hexs(c):
    c = col(c)
    return "%02X%02X%02X" % (c[0], c[1], c[2])


# ====================================================================== fonts and live text
_fc = {}
CAPR = {}      # cap height / font size
LINE = {}      # line height (what the game's Print calls "height") / font size
ASC = {}       # ascent / font size


def fontpx(key, px):
    """PIL font of key at a font size (em) in px"""
    px = max(4, int(round(px)))
    if (key, px) not in _fc:
        _fc[(key, px)] = ImageFont.truetype(FONTS[key], px)
    return _fc[(key, px)]


for _k in FONTS:
    _f = fontpx(_k, 1000)
    _a, _d = _f.getmetrics()
    CAPR[_k] = -_f.getbbox("H", anchor="ls")[1] / 1000.0
    LINE[_k] = (_a + _d) / 1000.0
    ASC[_k] = _a / 1000.0


def font(key, height):
    """PIL font for a text HEIGHT as the game's Print means it: the line height, in 1080p px
    (game design px x 1.5). Rajdhani 24 here = LabelSize 16 in the game."""
    return fontpx(key, height / LINE[key])


def cap_h(key, height):
    """cap height of a text of that HEIGHT"""
    return height / LINE[key] * CAPR[key]


def cap_off(key, height):
    """distance from the top of the line box (where Print's y is) down to the top of the capitals"""
    return height / LINE[key] * (ASC[key] - CAPR[key])


def height_for_cap(key, cap):
    """the HEIGHT that gives capitals of 'cap' px"""
    return cap / CAPR[key] * LINE[key]


def text_w(s, key, height, track=0.0):
    """width of live text; track = extra px between letters"""
    if not s:
        return 0.0
    return font(key, height).getlength(s) + track * (len(s) - 1)


def _text_layer(s, f, fill, track=0.0):
    """-> (RGBA layer, ox, oy): the text drawn with its baseline-left origin at (ox, oy) in the layer"""
    fill = rgba(fill)
    if track:
        w = sum(f.getlength(ch) for ch in s) + track * (len(s) - 1)
    else:
        w = f.getlength(s)
    asc, desc = f.getmetrics()
    pad = 4
    lay = Image.new("RGBA", (int(w) + 2 * pad + 2, asc + desc + 2 * pad), fill[:3] + (0,))
    d = ImageDraw.Draw(lay)
    if track:
        x = float(pad)
        for ch in s:
            d.text((x, pad + asc), ch, font=f, fill=fill, anchor="ls")
            x += f.getlength(ch) + track
    else:
        d.text((pad, pad + asc), s, font=f, fill=fill, anchor="ls")
    return lay, pad, pad + asc


def comp(dst, src, x, y):
    """alpha-composite src onto RGBA dst at (x, y), clipped"""
    x, y = int(round(x)), int(round(y))
    sx, sy = max(0, -x), max(0, -y)
    ex, ey = min(src.width, dst.width - x), min(src.height, dst.height - y)
    if ex <= sx or ey <= sy:
        return
    if (sx, sy, ex, ey) != (0, 0, src.width, src.height):
        src = src.crop((sx, sy, ex, ey))
    if src.mode != "RGBA":
        src = src.convert("RGBA")
    dst.alpha_composite(src, (x + sx, y + sy))


def text(im, x, y, s, key="RAJ", height=28.5, fill=INK, align=0, track=0.0, maxw=None):
    """LIVE TEXT, as the game's Print: (x, y) is the top left of the line box, height is the line height in
    1080p px, align 0 left / 1 centre / 2 right, maxw squeezes horizontally (never wraps). Returns the width."""
    if not s:
        return 0.0
    f = font(key, height)
    w = text_w(s, key, height, track)
    lay, ox, oy = _text_layer(s, f, fill, track)
    if maxw and w > maxw:
        k = maxw / w
        lay = lay.resize((max(1, int(round(lay.width * k))), lay.height), Image.LANCZOS)
        ox *= k
        w = maxw
    x0 = x - (0 if align == 0 else w / 2.0 if align == 1 else w)
    asc = f.getmetrics()[0]
    comp(im, lay, x0 - ox, y + asc - oy)
    return w


def text_cap(im, x, y, s, key="RAJ", cap=14.5, fill=INK, align=0, track=0.0, maxw=None):
    """the same live text placed by its capitals: y = top of the capitals, cap = their height"""
    h = height_for_cap(key, cap)
    return text(im, x, y - cap_off(key, h), s, key, h, fill, align, track, maxw)


def text_cap_w(s, key, cap, track=0.0):
    return text_w(s, key, height_for_cap(key, cap), track)


def wrap_text(s, key, height, maxw, track=0.0):
    """word wrap, as the game's WrapText: -> list of lines"""
    words, lines, cur = s.split(" "), [], ""
    for wd in words:
        t = (cur + " " + wd).strip()
        if cur and text_w(t, key, height, track) > maxw:
            lines.append(cur)
            cur = wd
        else:
            cur = t
    if cur:
        lines.append(cur)
    return lines


# ====================================================================== the generator's canvas (supersampled)
class Cv:
    """RGBA canvas in 1x units, drawn at SS and box-reduced by out(). Alpha fills blend (they do not punch holes)."""

    def __init__(s, w, h, bg=None, ss=None):
        s.ss = ss or SS
        s.w, s.h = w, h
        s.im = Image.new("RGBA", (int(math.ceil(w * s.ss)), int(math.ceil(h * s.ss))), rgba(bg) if bg is not None else (0, 0, 0, 0))
        s.d = ImageDraw.Draw(s.im)

    def set(s, im):
        s.im = im
        s.d = ImageDraw.Draw(im)

    # ---- internals
    def _opaque(s, *cols):
        for c in cols:
            if c is not None and len(c) == 4 and c[3] < 255:
                return False
        return True

    def _blend(s, pts, fn, pad=0.0):
        """draw through a temporary layer so that alpha blends. pts: 1x points bounding the shape"""
        k = s.ss
        x0 = int(math.floor(min(p[0] for p in pts) * k - pad * k)) - 2
        y0 = int(math.floor(min(p[1] for p in pts) * k - pad * k)) - 2
        x1 = int(math.ceil(max(p[0] for p in pts) * k + pad * k)) + 2
        y1 = int(math.ceil(max(p[1] for p in pts) * k + pad * k)) + 2
        x0, y0 = max(x0, 0), max(y0, 0)
        x1, y1 = min(x1, s.im.width), min(y1, s.im.height)
        if x1 <= x0 or y1 <= y0:
            return
        lay = Image.new("RGBA", (x1 - x0, y1 - y0), (0, 0, 0, 0))
        fn(ImageDraw.Draw(lay), x0, y0)
        s.im.alpha_composite(lay, (x0, y0))

    def _do(s, pts, fn, cols, pad=0.0):
        if s._opaque(*cols):
            fn(s.d, 0, 0)
        else:
            s._blend(pts, fn, pad)

    # ---- primitives
    def rect(s, x, y, w, h, fill=None, outline=None, width=0, r=0):
        if w <= 0 or h <= 0:
            return
        k = s.ss
        fill = rgba(fill) if fill is not None else None
        outline = rgba(outline) if outline is not None else None

        def fn(d, ox, oy):
            box = [x * k - ox, y * k - oy, (x + w) * k - ox - 1, (y + h) * k - oy - 1]
            if box[2] < box[0] or box[3] < box[1]:
                return
            wd = max(1, int(round(width * k))) if outline else 0
            if r:
                d.rounded_rectangle(box, radius=r * k, fill=fill, outline=outline, width=wd)
            else:
                d.rectangle(box, fill=fill, outline=outline, width=wd)
        s._do([(x, y), (x + w, y + h)], fn, (fill, outline))

    def poly(s, pts, fill=None, outline=None, width=0):
        k = s.ss
        fill = rgba(fill) if fill is not None else None
        outline = rgba(outline) if outline is not None else None

        def fn(d, ox, oy):
            pp = [(px * k - ox, py * k - oy) for px, py in pts]
            if fill is not None:
                d.polygon(pp, fill=fill)
            if outline is not None:
                d.line(pp + [pp[0], pp[1]], fill=outline, width=max(1, int(round(width * k))), joint="curve")
        s._do(pts, fn, (fill, outline), pad=width)

    def line(s, pts, fill, width=1.0, caps=False):
        k = s.ss
        fill = rgba(fill)

        def fn(d, ox, oy):
            pp = [(px * k - ox, py * k - oy) for px, py in pts]
            wd = max(1, int(round(width * k)))
            d.line(pp, fill=fill, width=wd, joint="curve")
            if caps:
                for (px, py) in (pp[0], pp[-1]):
                    d.ellipse([px - wd / 2.0, py - wd / 2.0, px + wd / 2.0, py + wd / 2.0], fill=fill)
        s._do(pts, fn, (fill,), pad=width)

    def ellipse(s, cx, cy, rx, ry, fill=None, outline=None, width=0):
        k = s.ss
        fill = rgba(fill) if fill is not None else None
        outline = rgba(outline) if outline is not None else None

        def fn(d, ox, oy):
            d.ellipse([(cx - rx) * k - ox, (cy - ry) * k - oy, (cx + rx) * k - ox, (cy + ry) * k - oy], fill=fill,
                      outline=outline, width=max(1, int(round(width * k))) if outline else 0)
        s._do([(cx - rx, cy - ry), (cx + rx, cy + ry)], fn, (fill, outline))

    def circle(s, cx, cy, r, fill=None, outline=None, width=0):
        s.ellipse(cx, cy, r, r, fill, outline, width)

    def arc(s, cx, cy, r, a0, a1, fill, width=1.0, ry=None):
        k = s.ss
        fill = rgba(fill)
        ry_ = r if ry is None else ry

        def fn(d, ox, oy):
            d.arc([(cx - r) * k - ox, (cy - ry_) * k - oy, (cx + r) * k - ox, (cy + ry_) * k - oy], a0, a1, fill=fill,
                  width=max(1, int(round(width * k))))
        s._do([(cx - r, cy - ry_), (cx + r, cy + ry_)], fn, (fill,), pad=width)

    def pie(s, cx, cy, r, a0, a1, fill):
        k = s.ss
        fill = rgba(fill)

        def fn(d, ox, oy):
            d.pieslice([(cx - r) * k - ox, (cy - r) * k - oy, (cx + r) * k - ox, (cy + r) * k - oy], a0, a1, fill=fill)
        s._do([(cx - r, cy - r), (cx + r, cy + r)], fn, (fill,))

    # ---- baked text: y = top of the capitals, cap = cap height, al 0 left / 1 centre / 2 right
    def tw(s, t, key, cap, track=0.0):
        f = fontpx(key, cap / CAPR[key] * s.ss)
        return (f.getlength(t) + track * s.ss * max(0, len(t) - 1)) / s.ss

    def text(s, x, y, t, key, cap, fill, al=0, track=0.0, maxw=None, xs=1.0):
        """xs = horizontal squeeze (condensed lettering); maxw shrinks the whole text to fit"""
        if not t:
            return 0.0
        w = s.tw(t, key, cap, track) * xs
        if maxw and w > maxw:
            sc = maxw / w
            y += cap * (1 - sc) * 0.5
            cap *= sc
            track *= sc
            w = maxw
        f = fontpx(key, cap / CAPR[key] * s.ss)
        lay, ox, oy = _text_layer(t, f, fill, track * s.ss)
        if xs != 1.0:
            lay = lay.resize((max(1, int(round(lay.width * xs))), lay.height), Image.LANCZOS)
            ox *= xs
        px = x * s.ss - (0 if al == 0 else w * s.ss / 2.0 if al == 1 else w * s.ss)
        py = (y + cap) * s.ss
        comp(s.im, lay, px - ox, py - oy)
        return w

    # ---- layers (already at SS)
    def paste(s, img, x, y, op=1.0):
        if img.mode != "RGBA":
            img = img.convert("RGBA")
        if op < 1:
            img = fade(img, op)
        comp(s.im, img, x * s.ss, y * s.ss)

    def paste_c(s, img, cx, cy, angle=0.0, op=1.0):
        if angle:
            img = img.rotate(angle, resample=Image.BICUBIC, expand=True)
        s.paste(img, cx - img.width / (2.0 * s.ss), cy - img.height / (2.0 * s.ss), op)

    def mask_paste(s, mask, x, y, fill):
        """paste an L mask (at SS) in a colour"""
        fill = rgba(fill)
        lay = Image.new("RGBA", mask.size, fill[:3] + (0,))
        lay.putalpha(mask if fill[3] == 255 else mask.point(lambda v: v * fill[3] // 255))
        s.paste(lay, x, y)

    def out(s, size=None):
        """-> the 1x RGBA image (box filter: no ringing on ink lines)"""
        im = s.im.reduce(s.ss) if s.ss > 1 else s.im.copy()
        size = size or (int(round(s.w)), int(round(s.h)))
        if im.size != size:
            im = im.crop((0, 0, size[0], size[1]))
        return im


def fade(img, op):
    img = img.copy()
    img.putalpha(img.getchannel("A").point(lambda v: int(v * op)))
    return img


def tint_img(img, c):
    """what the game's tile colour does: multiply RGB (alpha kept)"""
    c = col(c)
    if c == (255, 255, 255):
        return img
    r, g, b, a = img.convert("RGBA").split()
    r = r.point(lambda v: v * c[0] // 255)
    g = g.point(lambda v: v * c[1] // 255)
    b = b.point(lambda v: v * c[2] // 255)
    return Image.merge("RGBA", (r, g, b, a))


def solid(mask, c):
    """L mask -> RGBA layer of colour c"""
    c = rgba(c)
    lay = Image.new("RGBA", mask.size, c[:3] + (0,))
    lay.putalpha(mask if c[3] == 255 else mask.point(lambda v: v * c[3] // 255))
    return lay


# ---------------------------------------------------------------------- noise, wear, ink
def _noise(size, sigma, seed):
    """seeded gaussian noise, L, mean 128 (Image.effect_noise is not seedable)"""
    rnd = random.Random(seed)
    w, h = size
    data = bytes(max(0, min(255, int(rnd.gauss(128, sigma)))) for _ in range(w * h))
    return Image.frombytes("L", (w, h), data)


def tile_noise(size, cell, sigma, seed, blur=0.0):
    """low-frequency noise that tiles: an L image, mean 128"""
    w, h = size
    gw, gh = max(2, w // cell), max(2, h // cell)
    n = _noise((gw, gh), sigma, seed)
    big = Image.new("L", (gw * 3, gh * 3))
    for j in range(3):
        for i in range(3):
            big.paste(n, (i * gw, j * gh))
    big = big.resize((w * 3, h * 3), Image.BICUBIC)
    if blur:
        big = big.filter(ImageFilter.GaussianBlur(blur))
    return big.crop((w, h, 2 * w, 2 * h))


def add_noise(im, n):
    """im + (n - 128), per channel; alpha kept"""
    if im.mode == "RGBA":
        a = im.getchannel("A")
        out = ImageChops.add(im.convert("RGB"), Image.merge("RGB", (n, n, n)), 1, -128).convert("RGBA")
        out.putalpha(a)
        return out
    return ImageChops.add(im.convert("RGB"), Image.merge("RGB", (n, n, n)), 1, -128)


def grain(im, sigma=6, seed=1):
    return add_noise(im, _noise(im.size, sigma, seed))


def distress(img, amt=0.4, seed=5, ss=None):
    """rubber-stamp wear: knock blotches and speckle out of the alpha"""
    ss = ss or SS
    a = img.getchannel("A")
    w, h = img.size
    z = 1.9 - 1.25 * amt
    t = int(128 - 64 * z)
    n1 = _noise((w // (3 * ss) + 2, h // (3 * ss) + 2), 64, seed).resize((w, h), Image.BICUBIC)
    n1 = n1.point(lambda v: 255 if v > t else 0).filter(ImageFilter.GaussianBlur(0.5 * ss))
    n2 = _noise((w // ss + 1, h // ss + 1), 64, seed + 1).resize((w, h), Image.BILINEAR)
    n2 = n2.point(lambda v: 255 if v > t - 22 else 90)
    a = ImageChops.multiply(ImageChops.multiply(a, n1), n2)
    out = img.copy()
    out.putalpha(a)
    return out


def dilate(a, r):
    """grow an L mask by r px with round corners"""
    if r <= 0:
        return a
    sig = max(0.5, r / 2.05)
    return a.filter(ImageFilter.GaussianBlur(sig)).point(lambda v: 255 if v > 5 else 0).filter(ImageFilter.GaussianBlur(0.8))


def shift(img, dx, dy):
    """translate with transparent fill (no wrap)"""
    out = Image.new(img.mode, img.size, 0)
    out.paste(img, (int(round(dx)), int(round(dy))))
    return out


def inked(layer, ow, ink=INK, shadow=None, shcol=None, border=0, bcol=CHALK, pad=None):
    """Sticker treatment of an RGBA layer (all sizes in the layer's own px):
    optional die-cut border (border px of bcol), ink outline of ow px, optional hard offset shadow.
    -> (layer, pad): the result is 'pad' px larger on every side."""
    sh = max(abs(shadow[0]), abs(shadow[1])) if shadow else 0
    if pad is None:
        pad = int(border + ow * 1.5 + sh + 6)
    big = Image.new("RGBA", (layer.width + 2 * pad, layer.height + 2 * pad), (0, 0, 0, 0))
    big.alpha_composite(layer, (pad, pad))
    a0 = big.getchannel("A")
    ab = dilate(a0, border) if border else a0
    ai = dilate(ab, ow) if ow else ab
    out = Image.new("RGBA", big.size, (0, 0, 0, 0))
    if shadow:
        out.alpha_composite(shift(solid(ai, shcol or ink), shadow[0], shadow[1]))
    if ow:
        out.alpha_composite(solid(ai, ink))
    if border:
        out.alpha_composite(solid(ab, bcol))
    out.alpha_composite(big)
    return out, pad


def bleed(im):
    """fill the colour of fully transparent texels from their neighbours (alpha untouched), so that linear
    filtering in the engine never pulls black into an edge"""
    a = im.getchannel("A")
    if a.getextrema()[0] == 255:
        return im
    lv = im
    levels = []
    while min(lv.size) > 1:
        lv = lv.resize((max(1, lv.width // 2), max(1, lv.height // 2)), Image.BOX)
        levels.append(lv)
    base = Image.new("RGB", im.size, levels[-1].convert("RGB").resize((1, 1)).getpixel((0, 0)) if levels else (0, 0, 0))
    for lv in reversed(levels):
        up = lv.resize(im.size, Image.BILINEAR)
        base.paste(up.convert("RGB"), (0, 0), up.getchannel("A").point(lambda v: 255 if v > 20 else 0))
    base.paste(im.convert("RGB"), (0, 0), a.point(lambda v: 255 if v > 0 else 0))
    out = base.convert("RGBA")
    out.putalpha(a)
    return out


def white_mask(im):
    """RGB forced to white where the art is white-only: used for pure masks (RGB 255 everywhere, alpha = shape)"""
    out = Image.new("RGBA", im.size, (255, 255, 255, 0))
    out.putalpha(im.getchannel("A"))
    return out


# ---------------------------------------------------------------------- shapes
def burst_pts(cx, cy, ro, ri, n, rot=0.0, jit=0.0, seed=0, squash=1.0):
    """star burst outline: n points, outer radius ro, inner ri, jitter as a fraction, squash = y scale"""
    rnd = random.Random(seed)
    pts = []
    for i in range(n * 2):
        a = math.radians(rot + i * 180.0 / n)
        r = (ro if i % 2 == 0 else ri) * (1 + jit * (rnd.random() - 0.5))
        pts.append((cx + r * math.cos(a), cy + r * math.sin(a) * squash))
    return pts


def star_pts(cx, cy, R, r, n=5, rot=-90.0):
    pts = []
    for i in range(n * 2):
        a = math.radians(rot + i * 180.0 / n)
        q = R if i % 2 == 0 else r
        pts.append((cx + q * math.cos(a), cy + q * math.sin(a)))
    return pts


def para(x, y, w, h, lean):
    """parallelogram inscribed in the box x, y, w, h leaning right by 'lean' px at the top"""
    return [(x + lean, y), (x + w, y), (x + w - lean, y + h), (x, y + h)]


def halftone_mask(size, pitch, radius_fn, origin=(0.0, 0.0), ss=None):
    """45 degree dot screen as an L mask at SS. size and pitch in 1x px; radius_fn(x, y) -> dot radius in px (1x)
    for a dot centred at (x, y); origin = a point where a dot sits"""
    ss = ss or SS
    w, h = size
    m = Image.new("L", (int(math.ceil(w * ss)), int(math.ceil(h * ss))), 0)
    d = ImageDraw.Draw(m)
    ox, oy = origin
    i0 = int(math.floor((0 - ox) / pitch)) - 1
    i1 = int(math.ceil((w - ox) / pitch)) + 1
    j0 = int(math.floor((0 - oy) / pitch)) - 1
    j1 = int(math.ceil((h - oy) / pitch)) + 1
    for j in range(j0, j1 + 1):
        for i in range(i0, i1 + 1):
            for off in (0.0, 0.5):
                x = ox + (i + off) * pitch
                y = oy + (j + off) * pitch
                r = radius_fn(x, y)
                if r > 0.3:
                    d.ellipse([(x - r) * ss, (y - r) * ss, (x + r) * ss, (y + r) * ss], fill=255)
    return m


def vgrad(w, h, stops):
    """vertical gradient, RGB. stops: [(t 0..1, (r, g, b)), ...] sorted"""
    im = Image.new("RGB", (1, h))
    px = im.load()
    for y in range(h):
        t = y / float(max(1, h - 1))
        for k in range(len(stops) - 1):
            t0, c0 = stops[k]
            t1, c1 = stops[k + 1]
            if t0 <= t <= t1 or k == len(stops) - 2:
                u = 0.0 if t1 == t0 else max(0.0, min(1.0, (t - t0) / (t1 - t0)))
                px[0, y] = mix(c0, c1, u)
                break
    return im.resize((w, h), Image.NEAREST)


def shear_layer(lay, k):
    """shear an RGBA layer to the right at the top by k px per px of height. -> (layer, dx): the bottom row keeps its
    x, everything above moves right; the result is ceil(k * H) px wider"""
    W, H = lay.size
    ex = int(math.ceil(abs(k) * H))
    out = lay.transform((W + ex, H), Image.AFFINE, (1, k, -k * H, 0, 1, 0), resample=Image.BICUBIC)
    return out, ex


# ====================================================================== the texture registry
TEX = {}        # name -> function() -> RGBA image at 1x
MAN = {}        # name -> manifest entry (what KIT.md says about the texture)
ORDER = []      # names in manifest order


def tex(name, size, kind="colour", draw="1:1", wrap="", **meta):
    """register a texture. kind: 'colour' (draw white) or 'mask' (the game tints it). wrap: '', 'x', 'y' or 'xy'."""
    def deco(fn):
        TEX[name] = fn
        e = {"size": [size[0], size[1]], "kind": kind, "draw": draw, "wrap": wrap, "filter": "linear"}
        e.update(meta)
        MAN[name] = e
        ORDER.append(name)
        return fn
    return deco


# ====================================================================== the wall (a lit cyclorama)
CYC_FLOOR_LINE = 0.80       # the floor line, as a fraction of the screen height


@tex("T_UI_Cyc_Wall", (16, 1024), draw="stretch to the viewport (V follows the screen: v = y / H)")
def t_cyc_wall():
    H = 1024
    fl = CYC_FLOOR_LINE
    stops = [(0.0, CYC_TOP), (0.50, CYC_MID), (fl - 0.012, CYC_LOW), (fl - 0.004, (255, 226, 202)),
             (fl + 0.002, (255, 232, 212)), (fl + 0.010, mix(CYC_FLOOR, CYC_LOW, 0.35)), (fl + 0.05, CYC_FLOOR),
             (1.0, mix(CYC_FLOOR, (226, 150, 140), 0.38))]
    im = vgrad(16, H, stops).convert("RGBA")
    return im


@tex("T_UI_Cyc_Pool", (256, 256), kind="mask", draw="quarter: four mirrored tiles about the pool's centre",
     tint=["Chalk", "Sticky", "White"], note="the pool's centre is this texture's bottom right corner")
def t_cyc_pool():
    R, pitch = 252.0, 14.0

    def rad(x, y):
        d = math.hypot(x - 256.0, y - 256.0) / R
        return pitch * 0.52 * (1 - d) ** 0.9 if d < 1 else 0.0
    m = halftone_mask((256, 256), pitch, rad, origin=(256.0, 256.0)).reduce(SS)
    return solid(m, WHITE)


CYC_SIDE = (224, 672)


@tex("T_UI_Cyc_Side", CYC_SIDE, draw="stretch to the screen height, outside the 16:9 block; mirrored on the right",
     note="left side version: its right edge lies on the left edge of the centre 16:9 block")
def t_cyc_side():
    W, H = 256, 768                             # drawn on a 256 x 768 board, kept at CYC_SIDE
    cv = Cv(W, H)
    fl = CYC_FLOOR_LINE * H                     # floor line in this texture
    face = mix(CYC_SHADE, CYC_MID, 0.30)
    dark = mix(CYC_SHADE, INK, 0.22)
    edge = mix(CYC_SHADE, INK, 0.45)
    lite = mix(CYC_SHADE, CYC_MID, 0.62)
    # a scenery flat seen from behind, leaning on its jack brace
    fx0, fx1, ft, fb = 6, 118, 96, fl + 34
    cv.poly([(fx0, ft + 10), (fx1, ft), (fx1, fb), (fx0, fb + 14)], fill=face, outline=edge, width=3)
    for t in (0.0, 0.34, 0.67, 1.0):            # rails
        ya = ft + 10 + (fb + 14 - ft - 10) * t
        yb = ft + (fb - ft) * t
        cv.line([(fx0, ya), (fx1, yb)], edge, 5 if t in (0.0, 1.0) else 3.5)
    for x in (fx0 + 2, (fx0 + fx1) / 2.0, fx1 - 2):   # stiles
        cv.line([(x, ft + 6), (x, fb + 8)], edge, 4)
    cv.line([(fx0 + 4, ft + 250), (fx1 - 4, ft + 20)], edge, 3)          # corner brace
    cv.line([(fx1 - 2, ft + 190), (fx1 + 74, fb + 22)], edge, 7)          # jack
    cv.line([(fx1 - 2, ft + 190), (fx1 + 74, fb + 22)], dark, 3)
    cv.line([(fx1 - 2, fb + 10), (fx1 + 80, fb + 24)], edge, 6)
    cv.ellipse(fx1 + 62, fb + 18, 30, 12, fill=dark, outline=edge, width=3)   # sandbag
    cv.line([(fx1 + 40, fb + 14), (fx1 + 84, fb + 14)], edge, 2)
    # a light stand with a fresnel, aimed at the stage
    px, top, foot = 196, 238, fl + 62
    for dx in (-44, 0, 40):
        cv.line([(px, foot - 86), (px + dx, foot + (8 if dx else 14))], edge, 6)
    cv.line([(px, top + 40), (px, foot - 70)], edge, 8)
    cv.line([(px, top + 40), (px, foot - 70)], dark, 3)
    cv.rect(px - 7, foot - 150, 14, 22, fill=dark, outline=edge, width=2.5)
    # the lamp head
    hx, hy = px - 6, top
    body = [(hx - 38, hy - 22), (hx + 30, hy - 34), (hx + 44, hy + 16), (hx - 26, hy + 30)]
    cv.poly(body, fill=dark, outline=edge, width=3.5)
    cv.poly([(hx + 30, hy - 34), (hx + 60, hy - 52), (hx + 76, hy + 22), (hx + 44, hy + 16)], fill=face, outline=edge, width=3)
    cv.poly([(hx + 36, hy - 24), (hx + 44, hy + 8), (hx + 62, hy + 12), (hx + 52, hy - 34)], fill=lite)
    cv.line([(hx - 30, hy - 8), (hx + 34, hy - 20)], edge, 2.5)
    cv.line([(hx - 28, hy + 6), (hx + 38, hy - 6)], edge, 2.5)
    cv.arc(px - 2, top + 30, 26, 20, 160, edge, 5)                        # yoke
    # cable to the floor
    pts = [(px + 4, foot - 128)]
    for i in range(1, 9):
        t = i / 8.0
        pts.append((px + 4 + 34 * math.sin(t * 3.0), foot - 128 + 140 * t))
    cv.line(pts, edge, 3)
    return cv.im.resize(CYC_SIDE, Image.LANCZOS)


@tex("T_UI_Dots", (64, 64), kind="mask", draw="tile xy", wrap="xy", tint=["Chalk", "Ink", "Pink"],
     note="45 degree halftone screen, pitch 16 px")
def t_dots():
    m = halftone_mask((64, 64), 16.0, lambda x, y: 3.5).reduce(SS)
    return solid(m, WHITE)


# ====================================================================== stock and pattern (tiling)
@tex("T_UI_Paper", (256, 256), kind="mask", draw="tile xy", wrap="xy",
     tint=["Paper", "Aqua", "Sticky", "Ledger", "Manila", "Chalk"], note="opaque; white based grain: the tint IS the stock colour")
def t_paper():
    S_ = 256
    n = _noise((S_, S_), 4.0, 11)
    lo = tile_noise((S_, S_), 40, 26, 12)
    v = Image.new("L", (S_, S_), 245)
    v = ImageChops.add(v, n, 1, -128)
    v = ImageChops.add(v, lo.point(lambda q: 128 + (q - 128) // 6), 1, -128)
    # fibres
    cv = Cv(S_, S_, ss=2)
    rnd = random.Random(13)
    for _ in range(46):
        x, y, a, ln = rnd.uniform(0, S_), rnd.uniform(0, S_), rnd.uniform(0, math.pi), rnd.uniform(4, 13)
        dx, dy = math.cos(a) * ln, math.sin(a) * ln
        c = (0, 0, 0, rnd.randint(10, 26)) if rnd.random() < 0.7 else (255, 255, 255, rnd.randint(30, 60))
        for ox in (-S_, 0, S_):
            for oy in (-S_, 0, S_):
                cv.line([(x + ox, y + oy), (x + ox + dx, y + oy + dy)], c, 0.8)
    out = Image.merge("RGB", (v, v, v)).convert("RGBA")
    out.alpha_composite(cv.out())
    return out


@tex("T_UI_SlateDust", (256, 256), kind="mask", draw="tile xy", wrap="xy", tint=["Chalk"],
     note="over a flat Slate box; alpha is already low")
def t_slate_dust():
    S_ = 256
    lo = tile_noise((S_, S_), 36, 40, 21)
    a = lo.point(lambda q: max(0, min(255, (q - 136) * 2 // 5)))
    # wipe marks: long soft diagonal streaks, wrapped
    cv = Cv(S_, S_, ss=2)
    rnd = random.Random(22)
    for _ in range(9):
        x, y, ln, wd = rnd.uniform(0, S_), rnd.uniform(0, S_), rnd.uniform(60, 150), rnd.uniform(5, 16)
        al = rnd.randint(6, 14)
        for ox in (-S_, 0, S_):
            for oy in (-S_, 0, S_):
                cv.line([(x + ox, y + oy), (x + ox + ln, y + oy - ln * 0.32)], (255, 255, 255, al), wd, caps=True)
    streak = cv.out().getchannel("A").filter(ImageFilter.GaussianBlur(1.2))
    sp = _noise((S_, S_), 30, 23).point(lambda q: 34 if q > 208 else 0)
    a = ImageChops.add(ImageChops.add(a, streak), sp)
    out = Image.new("RGBA", (S_, S_), (255, 255, 255, 0))
    out.putalpha(a.point(lambda q: min(q, 44)))
    return out


def _stripes(size, period, band, c1=WHITE, c2=INK, alpha2=255):
    """45 degree '/' stripes that tile in x and y: c1 where (x + y) mod period < band"""
    cv = Cv(size, size)
    cv.rect(0, 0, size, size, fill=rgba(c2)[:3] + (alpha2,))
    c = -size
    while c < 2 * size + period:
        cv.poly([(c, 0), (c + band, 0), (c + band - size, size), (c - size, size)], fill=c1)
        c += period
    return cv


@tex("T_UI_ClapStripes", (64, 64), kind="mask", draw="tile xy, never stretched (scale 1 or 0.5)", wrap="xy",
     tint=["Chalk", "Amber", "Steel", "Pink"], note="opaque: white and ink bands of 32 px; under a tint the ink stays dark")
def t_clap_stripes():
    return _stripes(64, 64, 32).out()


@tex("T_UI_HazardStripes", (32, 32), kind="mask", draw="tile xy, never stretched", wrap="xy",
     tint=["Red", "Amber", "Chalk", "Steel"], note="opaque: white and ink bands of 16 px; also the small clapper stripe of button ends")
def t_hazard_stripes():
    return _stripes(32, 32, 16).out()


@tex("T_UI_Hatch", (32, 32), draw="tile xy", wrap="xy", note="ink hairlines at 34 % over disabled buttons and locked tiles")
def t_hatch():
    cv = Cv(32, 32)
    c = -32
    while c < 96:
        cv.poly([(c, 0), (c + 3.2, 0), (c + 3.2 - 32, 32), (c - 32, 32)], fill=rgba(INK, 0.34))
        c += 16
    return cv.out()


FILM_BASE = (12, 10, 17)
FILM_HOLE = (104, 98, 134)


@tex("T_UI_FilmPerf", (16, 16), draw="tile x (a row of perforations) or tile y (a column)", wrap="xy",
     note="opaque film base with one perforation; frames between the two rows are boxes")
def t_film_perf():
    cv = Cv(16, 16, bg=FILM_BASE)
    cv.rect(4.5, 3.5, 7, 9, fill=FILM_HOLE, r=2)
    return cv.out()


# ====================================================================== paper props
@tex("T_UI_Tape", (128, 32), draw="1:1, rot about its centre", pivot=[64, 16])
def t_tape():
    w, h = 128, 32
    cv = Cv(w, h)
    c0 = (240, 222, 160)
    n = 7
    pts = [(1, 1)]
    for k in range(n):
        pts.append((4.5 if k % 2 == 0 else 1, 1 + (k + 0.5) * (h - 2) / n))
    pts += [(1, h - 1), (w - 1, h - 1)]
    for k in range(n):
        pts.append((w - (4.5 if k % 2 == 0 else 1), h - 1 - (k + 0.5) * (h - 2) / n))
    pts.append((w - 1, 1))
    cv.poly(pts, fill=c0 + (232,), outline=mix(c0, INK, 0.45) + (200,), width=1.2)
    cv.rect(6, 4, w - 12, 3, fill=(255, 255, 255, 70))
    cv.rect(6, h - 8, w - 12, 3, fill=mix(c0, INK, 0.3) + (60,))
    return grain(cv.out(), 4, 31)


@tex("T_UI_Paperclip", (32, 64), draw="1:1")
def t_paperclip():
    h = 54.0
    w = h * 0.42
    cv = Cv(32, 64)
    x0, y0 = 4.5, 5.0
    k = cv.ss
    for (c, wd) in ((INK, 5.4), ((186, 192, 214), 2.6)):
        cv.d.rounded_rectangle([x0 * k, y0 * k, (x0 + w) * k, (y0 + h) * k], radius=w / 2 * k, outline=rgba(c), width=int(wd * k))
        cv.d.rounded_rectangle([(x0 + w * 0.24) * k, (y0 + h * 0.2) * k, (x0 + w) * k, (y0 + h * 0.86) * k], radius=w * 0.38 * k,
                               outline=rgba(c), width=int(wd * k))
    return cv.out()


@tex("T_UI_Pin", (28, 28), draw="1:1")
def t_pin():
    cv = Cv(28, 28)
    cv.circle(15, 16, 9, fill=rgba(INK, 0.55))
    cv.circle(12, 12, 9, fill=RED, outline=INK, width=2.5)
    cv.circle(9.4, 9.2, 2.8, fill=(255, 255, 255, 235))
    return cv.out()


@tex("T_UI_ClipboardClip", (208, 64), draw="1:1, centred on the panel's top edge (its jaw line is row 28)")
def t_clipboard_clip():
    w, h = 200.0, 56.0
    cv = Cv(208, 64)
    m = (150, 156, 180)
    cv.rect(4 + 4, 4 + h * 0.42 + 4, w, h * 0.58, fill=rgba(INK, 0.5), r=5)
    cv.rect(4, 4 + h * 0.42, w, h * 0.58, fill=m, outline=INK, width=3, r=5)
    cv.rect(4 + w * 0.28, 4, w * 0.44, h * 0.6, fill=(190, 196, 216), outline=INK, width=3, r=9)
    cv.circle(4 + w / 2.0, 4 + h * 0.26, 7, fill=(40, 36, 56), outline=INK, width=2.5)
    cv.rect(4 + 10, 4 + h * 0.56, w - 20, 4, fill=(214, 218, 236))
    cv.rect(4 + 10, 4 + h * 0.82, w - 20, 4, fill=(118, 122, 150))
    for x in (4 + 16, 4 + w - 16):
        cv.circle(x, 4 + h * 0.72, 4, fill=(96, 100, 126), outline=INK, width=1.5)
    return cv.out()


@tex("T_UI_ClapStick", (704, 48), draw="rot about its hinge", pivot=[24, 24])
def t_clap_stick():
    w, h = 696.0, 40.0
    cv = Cv(704, 48)
    cv.rect(4, 4, w, h, fill=SLATE)
    c = -h
    while c < w + h:
        cv.poly([(4 + c + 20, 4 + h), (4 + c + 62, 4 + h), (4 + c + 62 + h, 4), (4 + c + 20 + h, 4)], fill=CHALK)
        c += 84
    # trim what ran over the ends
    cv.rect(0, 0, 4, 48, fill=(0, 0, 0, 0))
    lay = Image.new("RGBA", cv.im.size, (0, 0, 0, 0))
    lay.paste(cv.im.crop((4 * cv.ss, 4 * cv.ss, int((4 + w) * cv.ss), int((4 + h) * cv.ss))), (4 * cv.ss, 4 * cv.ss))
    cv.set(lay)
    cv.rect(4, 4, w, h, outline=INK, width=3.5)
    cv.rect(4 + 3.5, 4 + 3.5, w - 7, 3, fill=(255, 255, 255, 46))
    cv.circle(24, 24, 9, fill=(170, 176, 198), outline=INK, width=2.5)
    cv.circle(24, 24, 3, fill=INK)
    return cv.out()


@tex("T_UI_FilmCan", (208, 208), draw="1:1")
def t_film_can():
    R = 96.0
    cv = Cv(208, 208)
    c = 104.0
    cv.circle(c + 5, c + 6, R, fill=rgba(INK, 0.55))
    cv.circle(c, c, R, fill=(126, 132, 158), outline=INK, width=4)
    for (rr, colr) in ((0.93, (166, 172, 196)), (0.86, (112, 118, 146)), (0.8, (150, 156, 182)), (0.56, (118, 124, 152)),
                      (0.5, (160, 166, 190))):
        cv.circle(c, c, R * rr, fill=colr, outline=(70, 72, 98), width=1.5)
    # printed shading: a halftone crescent lower right, highlight arcs upper left
    m = Image.new("L", cv.im.size, 0)
    ImageDraw.Draw(m).ellipse([(c - R * 0.95) * cv.ss, (c - R * 0.95) * cv.ss, (c + R * 0.95) * cv.ss, (c + R * 0.95) * cv.ss], fill=255)

    def rad(x, y):
        t = ((x - c) + (y - c)) / (1.4 * R)
        return min(3.0, 5.4 * (t - 0.25)) if t > 0.25 else 0.0
    dots = halftone_mask((208, 208), 9.0, rad, origin=(c, c))
    cv.im.alpha_composite(solid(ImageChops.multiply(dots, m), (52, 54, 84, 150)))
    cv.arc(c, c, R * 0.72, 190, 262, (236, 240, 252, 230), 5)
    cv.arc(c, c, R * 0.3, 200, 250, (236, 240, 252, 200), 3)
    # the label
    lw, lh = R * 1.3, R * 0.5
    lab = Cv(lw + 8, lh + 8)
    lab.rect(4, 4, lw, lh, fill=PAPER, outline=INK, width=2.5)
    lab.text(4 + lw / 2, 4 + lh * 0.14, "THE DAILIES", "BUN", lh * 0.2, INK, al=1, track=1)
    lab.rect(4 + 10, 4 + lh * 0.44, lw - 20, 1.5, fill=PAPER_D)
    lab.text(4 + lw / 2, 4 + lh * 0.50, "print it!", "DAF", lh * 0.3, (40, 60, 150), al=1)
    cv.paste_c(lab.im, c, c + R * 0.02, 4)
    return cv.out()


CHEQUE_ZONES = {"stub": [16, 16, 100, 200], "payee": [150, 62, 440, 34], "amount": [612, 52, 168, 46],
                "words": [150, 112, 630, 30], "memo": [150, 164, 250, 30], "signature": [520, 150, 256, 60]}


@tex("T_UI_Cheque", (800, 232), draw="1:1, never rotated", zones=CHEQUE_ZONES,
     note="blank: payee, amount, memo are live text in the zones (x, y, w, h); the signature is T_UI_Signature")
def t_cheque():
    w, h = 800, 232
    cv = Cv(w, h, bg=(220, 240, 232))
    for j in range(int(h / 9) + 2):
        pts = []
        for x in range(0, w + 8, 6):
            pts.append((x, j * 9 - 4 + 3.2 * math.sin(x / 17.0 + j * 0.9)))
        cv.line(pts, (190, 224, 212), 1.3)
    ink2 = (60, 110, 110)
    cv.rect(0, 0, w, h, outline=INK, width=3.5)
    cv.rect(8, 8, w - 16, h - 16, outline=ink2, width=1.5)
    for k in range(int((h - 20) / 9)):
        cv.circle(124, 14 + k * 9, 1.8, fill=INK)
    # small print (decoration)
    cv.text(150, 22, "PICTURE PAYROLL", "BUN", 15, ink2, track=1.5)
    cv.text(150, 44, "STAGE 88  /  THE STUDIO LOT", "RAJ", 9, ink2, track=1.5)
    cv.text(150, 100, "PAY TO THE ORDER OF", "RAJ", 8.5, ink2, track=1.2)
    cv.rect(150, 97, 440, 1.5, fill=ink2)
    z = CHEQUE_ZONES["amount"]
    cv.rect(z[0], z[1], z[2], z[3], fill=(244, 252, 246), outline=ink2, width=2)
    cv.rect(150, 143, 630, 1.5, fill=ink2)
    cv.text(150, 198, "MEMO", "RAJ", 8.5, ink2, track=1.2)
    cv.rect(150, 195, 250, 1.5, fill=ink2)
    cv.rect(520, 195, 256, 1.5, fill=ink2)
    cv.text(776, 198, "THE DIRECTOR", "RAJ", 8.5, ink2, al=2, track=1.2)
    cv.text(70, 24, "STUB", "RAJ", 9, ink2, al=1, track=2)
    for yy in (60, 100, 140, 180):
        cv.rect(22, yy, 92, 1.2, fill=ink2)
    cv.text(700, 24, "No. 0088", "RAJ", 12, (170, 60, 70), track=1.5)
    return grain(cv.out().convert("RGB"), 3, 41).convert("RGBA")


@tex("T_UI_Signature", (256, 72), kind="mask", draw="1:1", tint=["Ink", "Teal", "Pink"])
def t_signature():
    cv = Cv(256, 72)
    cv.text(128, 16, "The Director", "DAF", 30, WHITE, al=1)
    pts = [(24 + i * 8, 60 - 4 * math.sin(i / 3.2) - i * 0.12) for i in range(27)]
    cv.line(pts, WHITE, 2.4, caps=True)
    return white_mask(cv.out())


@tex("T_UI_Highlighter", (64, 24), kind="mask", draw="3-slice-h", slice=[16, 0, 16, 0], tint=["Amber", "Cyan", "Pink", "Green"],
     note="alpha 86 % baked in")
def t_highlighter():
    cv = Cv(64, 24)
    cv.poly([(4, 2), (20, 1), (44, 2), (61, 1), (63, 11), (60, 22), (40, 23), (20, 22), (3, 23), (1, 12)], fill=(255, 255, 255, 220))
    cv.rect(8, 9, 48, 3, fill=(255, 255, 255, 30))
    return white_mask(cv.out())


def _wax(img, seed):
    """the broken edge of a grease pencil: knock fine speckle out of the alpha"""
    a = img.getchannel("A")
    n = _noise((img.width // 2 + 1, img.height // 2 + 1), 60, seed).resize(img.size, Image.BILINEAR)
    n = n.point(lambda v: 255 if v > 70 else 150)
    out = img.copy()
    out.putalpha(ImageChops.multiply(a, n))
    return out


def grease_ring_pts(cx, cy, rx, ry, seed=1, turns=1.13, n=96):
    r = random.Random(seed)
    p1, p2, st = r.uniform(0, 6.28), r.uniform(0, 6.28), r.uniform(-2.7, -2.1)
    pts = []
    for i in range(int(n * turns) + 1):
        t = st + i / float(n) * 2 * math.pi
        k = 1 + 0.03 * math.sin(2 * t + p1) + 0.018 * math.sin(3 * t + p2) + 0.07 * (i / (n * turns) - 0.5)
        pts.append((cx + math.cos(t) * rx * k, cy + math.sin(t) * ry * k))
    return pts


@tex("T_UI_Grease", (384, 192), kind="mask", draw="atlas: 1:1 or stretch", cell=[192, 96], cols=2,
     cells=["ring", "underline", "cross", "arrow"], tint=["Pink", "Red", "Ink"])
def t_grease():
    cv = Cv(384, 192)
    cv.line(grease_ring_pts(96, 48, 84, 38, seed=3), WHITE, 5.0, caps=True)                 # 0 ring
    pts = [(192 + 12 + i * 8.4, 54 + 5 * math.sin(i / 2.6) - i * 0.25) for i in range(21)]
    cv.line(pts, WHITE, 5.0, caps=True)                                                     # 1 underline
    pts2 = [(192 + 30 + i * 7.0, 66 + 3 * math.sin(i / 2.0 + 1) - i * 0.2) for i in range(19)]
    cv.line(pts2, WHITE, 3.6, caps=True)
    cv.line([(40, 96 + 14), (150, 96 + 82)], WHITE, 5.5, caps=True)                         # 2 cross
    cv.line([(152, 96 + 12), (44, 96 + 84)], WHITE, 5.5, caps=True)
    ax = [(192 + 160, 96 + 14), (192 + 120, 96 + 22), (192 + 80, 96 + 44), (192 + 52, 96 + 74)]   # 3 arrow
    cv.line(ax, WHITE, 5.0, caps=True)
    cv.line([(192 + 84, 96 + 76), (192 + 50, 96 + 78), (192 + 56, 96 + 44)], WHITE, 5.0, caps=True)
    return white_mask(_wax(cv.out(), 51))


# ====================================================================== bulbs
@tex("T_UI_Bulb", (64, 32), draw="atlas: 1:1, N times round a rectangle", cell=[32, 32], cols=2, cells=["lit", "unlit"])
def t_bulb():
    cv = Cv(64, 32)
    c = 16.0
    cv.circle(c, c, 15, fill=rgba(AMBER, 0.20))
    cv.circle(c, c, 11, fill=rgba(AMBER, 0.42))
    cv.circle(c, c, 6.5, fill=(255, 248, 214), outline=INK, width=1.8)
    cv.circle(c - 1.8, c - 2.0, 1.9, fill=WHITE)
    cv.circle(32 + c, c, 6.5, fill=(98, 84, 78), outline=INK, width=1.8)
    cv.circle(32 + c - 1.8, c - 2.0, 1.6, fill=(146, 130, 116))
    return cv.out()


# ====================================================================== stamps
def stamp_art(text_, c, h=56.0, sub=None, stars=0, of=0, wear=0.45, track=1.0, seed=5, xs=1.0):
    """rubber stamp: double rule, Bungee, optional small top line and a star row; worn. xs condenses the big line.
    -> Cv (not reduced)"""
    cap = h * 0.44
    subcap = h * 0.17
    tmp = Cv(8, 8)
    rows_h = cap + (subcap + h * 0.14 if sub else 0) + (h * 0.3 if of else 0)
    H = rows_h + h * 0.52
    W = max(tmp.tw(text_, "BUN", cap, track) * xs, tmp.tw(sub or "", "RAJ", subcap, 2)) + h * 0.8
    cv = Cv(W + 8, H + 8)
    c4 = rgba(c)
    cv.rect(4, 4, W, H, outline=c4, width=h * 0.085, r=h * 0.14)
    cv.rect(4 + h * 0.14, 4 + h * 0.14, W - h * 0.28, H - h * 0.28, outline=c4, width=max(1.0, h * 0.03), r=h * 0.07)
    y = 4 + h * 0.26
    if sub:
        cv.text(4 + W / 2.0, y, sub, "RAJ", subcap, c4, al=1, track=2)
        y += subcap + h * 0.14
    cv.text(4 + W / 2.0, y, text_, "BUN", cap, c4, al=1, track=track, xs=xs)
    y += cap + h * 0.1
    if of:
        tot = of * h * 0.3
        for k in range(of):
            cx = 4 + W / 2.0 - tot / 2.0 + (k + 0.5) * h * 0.3
            cv.poly(star_pts(cx, y + h * 0.12, h * 0.12, h * 0.05), fill=c4[:3] + (255 if k < stars else 92,))
    cv.set(distress(cv.im, wear, seed))
    return cv


def _centre_on(size, art):
    out = Image.new("RGBA", size, (0, 0, 0, 0))
    comp(out, art, (size[0] - art.width) / 2.0, (size[1] - art.height) / 2.0)
    return out


@tex("T_UI_Stamp_Approved", (192, 72), draw="rot about its centre", pivot=[96, 36])
def t_stamp_approved():
    h = 60.0
    while True:
        art = stamp_art("APPROVED", GREEN_INK, h, wear=0.26, seed=7)
        if art.w <= 190 and art.h <= 70:
            break
        h -= 1
    return _centre_on((192, 72), art.out())


RATING_CELL = (448, 132)


@tex("T_UI_Stamp_Rating", (448, 528), draw="atlas: cell, rot about the cell's centre", cell=list(RATING_CELL), cols=1,
     cells=list(RATINGS), pivot=[224, 66])
def t_stamp_rating():
    out = Image.new("RGBA", (448, 528), (0, 0, 0, 0))
    tmp = Cv(8, 8)
    for k, nm in enumerate(RATINGS):
        h = 76.0
        # every rating has the same height; a long one is set condensed instead of small
        natw = tmp.tw(nm, "BUN", h * 0.44, 1.0)
        xs = min(1.0, (438.0 - h * 0.8) / natw)
        while True:
            art = stamp_art(nm, PAL["Rate%d" % k], h, sub="THE DAILIES  /  DIRECTOR'S RATING", stars=k + 1, of=4, seed=60 + k, xs=xs)
            if art.w <= 446 and art.h <= 130:
                break
            h -= 2
        comp(out, _centre_on(RATING_CELL, art.out()), 0, RATING_CELL[1] * k)
    return out


# ====================================================================== icons (64-grid masks, tinted in the game)
class IC:
    """an icon drawn on a 64-unit grid into an L mask (4x supersampled at the size asked for)"""

    def __init__(s, px=64):
        s.px = px
        s.K = px / 64.0 * 4.0
        n = int(round(64 * s.K))
        s.im = Image.new("L", (n, n), 0)
        s.d = ImageDraw.Draw(s.im)

    def p(s, pts, v=255):
        s.d.polygon([(x * s.K, y * s.K) for x, y in pts], fill=v)

    def r(s, x, y, w, h, v=255, rad=0):
        s.d.rounded_rectangle([x * s.K, y * s.K, (x + w) * s.K, (y + h) * s.K], radius=rad * s.K, fill=v)

    def c(s, cx, cy, r, v=255):
        s.d.ellipse([(cx - r) * s.K, (cy - r) * s.K, (cx + r) * s.K, (cy + r) * s.K], fill=v)

    def e(s, cx, cy, rx, ry, v=255):
        s.d.ellipse([(cx - rx) * s.K, (cy - ry) * s.K, (cx + rx) * s.K, (cy + ry) * s.K], fill=v)

    def l(s, pts, w, v=255):
        s.d.line([(x * s.K, y * s.K) for x, y in pts], fill=v, width=max(1, int(round(w * s.K))), joint="curve")
        for x, y in pts:
            s.c(x, y, w / 2.0, v)

    def ring(s, cx, cy, r, w, v=255):
        s.d.ellipse([(cx - r) * s.K, (cy - r) * s.K, (cx + r) * s.K, (cy + r) * s.K], outline=v, width=max(1, int(round(w * s.K))))

    def arc(s, cx, cy, r, a0, a1, w, v=255):
        s.d.arc([(cx - r) * s.K, (cy - r) * s.K, (cx + r) * s.K, (cy + r) * s.K], a0, a1, fill=v, width=max(1, int(round(w * s.K))))

    def star(s, cx, cy, R, r, n=5, v=255, rot=-90):
        s.p(star_pts(cx, cy, R, r, n, rot), v)

    def glyph(s, cx, cy, ch, key, size, v=0):
        f = ImageFont.truetype(FONTS[key], int(round(size * s.K)))
        s.d.text((cx * s.K, cy * s.K), ch, font=f, fill=v, anchor="mm")


def _i_clapper(i):
    i.r(8, 14, 48, 9, rad=1)
    for k in range(4):
        i.p([(15 + 12 * k, 14), (21 + 12 * k, 14), (16 + 12 * k, 23), (10 + 12 * k, 23)], 0)
    i.r(8, 26, 48, 30, rad=3)
    i.r(14, 35, 36, 3, 0)
    i.r(14, 44, 22, 3, 0)


def _i_megaphone(i):
    i.p([(10, 27), (10, 39), (42, 54), (42, 12)])
    i.r(42, 9, 7, 48, rad=2)
    i.r(17, 38, 8, 18, rad=2)
    i.l([(54, 21), (60, 17)], 3.4)
    i.l([(55, 33), (62, 33)], 3.4)
    i.l([(54, 45), (60, 49)], 3.4)


def _i_reel(i):
    i.c(32, 32, 27)
    i.c(32, 32, 5, 0)
    for k in range(5):
        a = math.radians(-90 + k * 72)
        i.c(32 + 15.5 * math.cos(a), 32 + 15.5 * math.sin(a), 6.2, 0)


def _i_shield(i):
    i.p([(32, 5), (55, 13), (55, 31), (32, 59), (9, 31), (9, 13)])
    i.p([(32, 14), (32, 48), (17, 30), (17, 19)], 0)


def _i_bolt(i):
    i.p([(37, 3), (13, 36), (28, 36), (23, 61), (51, 25), (35, 25), (42, 3)])


def _i_missile(i):
    i.l([(19, 45), (40, 24)], 13)
    i.p([(34, 15), (57, 7), (49, 30)])
    i.p([(11, 36), (5, 59), (28, 53)])
    i.l([(25, 33), (31, 39)], 3, 0)


def _i_bullets(i):
    for x in (9, 26.5, 44):
        i.r(x, 22, 11, 36, rad=1)
        i.c(x + 5.5, 15, 5.5)
        i.r(x, 15, 11, 9)
        i.p([(x + 1, 12), (x + 5.5, 4), (x + 10, 12)])
        i.r(x, 45, 11, 3, 0)


def _i_wrench(i):
    i.l([(19, 45), (40, 24)], 10)
    i.c(45, 19, 14)
    i.p([(42, 22), (51, 3), (64, 14)], 0)
    i.c(15, 49, 9)
    i.c(15, 49, 3.5, 0)


def _i_spray(i):
    i.r(16, 24, 28, 36, rad=3)
    i.r(22, 15, 16, 9, rad=2)
    i.r(27, 8, 8, 7)
    i.r(16, 34, 28, 3, 0)
    for (x, y, r) in ((46, 9, 2.6), (53, 14, 2.6), (51, 5, 2), (58, 8, 2.2), (57, 19, 2)):
        i.c(x, y, r)


def _i_burst(i):
    i.star(32, 32, 30, 15, n=10)
    i.star(32, 32, 11, 5, n=5, v=0)


def _i_ticket(i):
    i.r(5, 17, 54, 30, rad=3)
    i.c(5, 32, 5.5, 0)
    i.c(59, 32, 5.5, 0)
    for k in range(4):
        i.r(42, 20 + 6.5 * k, 2.4, 3.6, 0)
    i.star(24, 32, 9, 4, v=0)


def _i_star(i):
    i.star(32, 34, 29, 12.5)


def _i_wheel(i):
    i.ring(32, 32, 27, 7)
    i.c(32, 32, 7.5)
    i.l([(32, 32), (8, 29)], 6)
    i.l([(32, 32), (56, 29)], 6)
    i.l([(32, 32), (32, 56)], 6)


def _i_helmet(i):
    i.c(32, 30, 25)
    i.r(7, 30, 50, 22, rad=5)
    i.r(30, 25, 30, 12, 0, rad=3)
    i.r(0, 52, 64, 12, 0)
    i.r(9, 46, 46, 6, rad=2)


def _i_flag(i):
    i.r(8, 5, 5.5, 55, rad=2)
    i.p([(13, 8), (58, 11), (58, 38), (13, 36)])
    for (x, y) in ((24, 9.5), (46, 11), (13.5, 18.5), (35, 19.5), (24, 28), (46, 29)):
        i.r(x, y, 11, 8.6, 0)


def _i_padlock(i):
    i.r(11, 28, 42, 31, rad=4)
    i.arc(32, 27, 15, 180, 360, 7.5)
    i.r(17, 20, 7.5, 10)
    i.r(39.5, 20, 7.5, 10)
    i.c(32, 40, 5, 0)
    i.r(30, 40, 4, 11, 0)


def _i_warning(i):
    i.p([(32, 4), (61, 57), (3, 57)])
    i.r(28.7, 22, 6.6, 19, 0, rad=2)
    i.c(32, 48.5, 3.8, 0)


def _i_cash(i):
    i.c(32, 32, 27)
    i.ring(32, 32, 21.5, 2.2, 0)
    i.glyph(32, 31.5, "$", "BUN", 30, 0)


def _i_rec(i):
    i.ring(32, 32, 28, 4.5)
    i.c(32, 32, 17)


def _i_check(i):
    i.l([(10, 34), (25, 50), (54, 14)], 10)


def _i_skull(i):
    i.c(32, 26, 21)
    i.r(20, 38, 24, 18, rad=3)
    i.c(23.5, 27, 6.5, 0)
    i.c(40.5, 27, 6.5, 0)
    i.p([(32, 33), (28.5, 40), (35.5, 40)], 0)
    i.r(26.5, 47, 2.4, 9, 0)
    i.r(34.6, 47, 2.4, 9, 0)


def _i_gear(i):
    for k in range(8):
        a = math.radians(k * 45)
        ca, sa = math.cos(a), math.sin(a)
        pts = []
        for (u, v) in ((16, -5.5), (28, -4.5), (28, 4.5), (16, 5.5)):
            pts.append((32 + u * ca - v * sa, 32 + u * sa + v * ca))
        i.p(pts)
    i.c(32, 32, 20)
    i.c(32, 32, 8, 0)


def _i_pad(i):
    i.r(5, 19, 54, 26, rad=12)
    i.c(14, 43, 9.5)
    i.c(50, 43, 9.5)
    i.r(13, 30, 13, 4.4, 0)
    i.r(17.3, 25.7, 4.4, 13, 0)
    i.c(43, 28, 3.2, 0)
    i.c(50, 34, 3.2, 0)


def _i_exit(i):
    i.r(8, 5, 27, 54, rad=2)
    i.c(28, 33, 3, 0)
    i.l([(42, 32), (55, 32)], 6.5)
    i.p([(51, 19), (64, 32), (51, 45)])


def _i_camera(i):
    i.c(18, 16, 11)
    i.c(40, 16, 11)
    i.c(18, 16, 3.5, 0)
    i.c(40, 16, 3.5, 0)
    i.r(6, 27, 40, 28, rad=3)
    i.p([(46, 36), (60, 28), (60, 54), (46, 46)])


def _i_people(i):
    i.c(44, 22, 8)
    i.r(33, 33, 23, 30, rad=8)
    i.c(22, 20, 10, 0)
    i.c(22, 18, 9.5)
    i.r(7, 30, 30, 34, 0, rad=10)
    i.r(8.5, 31.5, 27, 34, rad=9)
    i.r(0, 54, 64, 10, 0)


def _i_fire(i):
    i.p([(31, 3), (45, 21), (42, 30), (52, 25), (55, 42), (45, 59), (19, 59), (9, 42), (13, 25), (22, 34), (23, 17)])
    i.p([(32, 33), (40, 46), (37, 57), (27, 57), (24, 46)], 0)


def _i_arrow(i):
    i.p([(20, 10), (46, 32), (20, 54)])


# ---- added for the ruling
def _i_clock(i):
    i.ring(32, 32, 27, 6)
    i.l([(32, 32), (32, 16)], 5)
    i.l([(32, 32), (43, 38)], 5)


def _i_undo(i):
    i.arc(35, 36, 21, 196, 110, 8)
    i.p([(2, 22), (29, 20), (13, 43)])
    i.c(27.5, 55.5, 4)


def _i_disk(i):
    i.p([(7, 7), (48, 7), (57, 16), (57, 57), (7, 57)])
    i.r(17, 7, 26, 16, 0)
    i.r(33, 10, 6, 10)
    i.r(15, 33, 34, 24, 0, rad=2)
    i.r(20, 39, 24, 3)
    i.r(20, 46, 24, 3)


def _i_crosshair(i):
    i.ring(32, 32, 22, 5)
    for (a, b) in (((32, 2), (32, 18)), ((32, 46), (32, 62)), ((2, 32), (18, 32)), ((46, 32), (62, 32))):
        i.l([a, b], 5)
    i.c(32, 32, 4.5)


def _i_speaker(i):
    i.p([(6, 23), (20, 23), (36, 9), (36, 55), (20, 41), (6, 41)])
    i.arc(34, 32, 13, -52, 52, 5)
    i.arc(34, 32, 24, -48, 48, 5)


def _i_trophy(i):
    i.p([(17, 6), (47, 6), (45, 28), (38, 38), (26, 38), (19, 28)])
    i.arc(17, 17, 10, 90, 270, 5)
    i.arc(47, 17, 10, 270, 90, 5)
    i.r(28, 37, 8, 12)
    i.r(18, 49, 28, 9, rad=2)
    i.star(32, 19, 8, 3.4, v=0)


def _i_car(i):
    i.p([(3, 44), (3, 33), (14, 29), (26, 17), (44, 17), (52, 29), (61, 34), (61, 44)])
    i.p([(28, 21), (36, 21), (36, 29), (21, 29)], 0)
    i.p([(39, 21), (43, 21), (48, 29), (39, 29)], 0)
    i.c(17, 45, 9.5, 0)
    i.c(47, 45, 9.5, 0)
    i.c(17, 45, 7)
    i.c(47, 45, 7)
    i.c(17, 45, 2.4, 0)
    i.c(47, 45, 2.4, 0)


# ---- the eight shoot pictograms
def _i_palm(i):
    i.p([(28, 61), (37, 61), (35, 40), (33, 24), (29, 24), (30, 40)])
    for tri in ([(32, 25), (3, 37), (13, 19)], [(32, 25), (61, 37), (51, 19)], [(32, 25), (5, 9), (22, 11)],
                [(32, 25), (59, 9), (42, 11)], [(32, 27), (24, 2), (40, 2)]):
        i.p(tri)
    i.e(32, 61, 17, 3)
    i.l([(30.5, 34), (35, 33)], 1.6, 0)
    i.l([(30.3, 44), (36, 43)], 1.6, 0)
    i.l([(29.5, 53), (36.5, 52)], 1.6, 0)


def _i_flask(i):
    i.p([(25, 7), (39, 7), (39, 24), (58, 55), (54, 61), (10, 61), (6, 55), (25, 24)])
    i.r(21, 2, 22, 7, rad=2)
    i.p([(20.5, 36), (43.5, 36), (45, 38.6), (19, 38.6)], 0)
    i.c(27, 49, 3.6, 0)
    i.c(38.5, 53, 2.6, 0)
    i.c(36, 44.5, 1.8, 0)


def _i_cactus(i):
    i.r(25, 4, 14, 58, rad=7)
    i.r(8, 22, 10, 22, rad=5)
    i.r(8, 35, 20, 9, rad=4)
    i.r(46, 13, 10, 22, rad=5)
    i.r(36, 26, 20, 9, rad=4)
    i.r(12, 58, 40, 4, rad=2)
    for y in (12, 22, 32, 42):
        i.r(31, y, 2, 5, 0)


def _i_volcano(i):
    i.p([(2, 61), (22, 22), (42, 22), (62, 61)])
    i.p([(22, 22), (42, 22), (39.5, 33), (36, 28), (32, 40), (28, 28), (24.5, 33)], 0)
    i.p([(26, 19), (32, 2), (38, 19)])
    i.p([(13, 15), (20, 8), (23, 18)])
    i.p([(51, 15), (44, 8), (41, 18)])
    i.c(32, 31, 2.2)


def _i_ferris(i):
    i.l([(32, 27), (17, 60)], 5)
    i.l([(32, 27), (47, 60)], 5)
    i.r(10, 57, 44, 5, rad=2)
    i.ring(32, 27, 22, 4.5)
    for k in range(4):
        a = math.radians(k * 45 + 22.5)
        i.l([(32 + 21 * math.cos(a), 27 + 21 * math.sin(a)), (32 - 21 * math.cos(a), 27 - 21 * math.sin(a))], 2.2)
    for k in range(8):
        a = math.radians(k * 45 + 22.5)
        i.c(32 + 22 * math.cos(a), 27 + 22 * math.sin(a), 4.6)
    i.c(32, 27, 4)


def _i_mountain(i):
    i.p([(0, 60), (24, 7), (37, 33), (45, 21), (64, 60)])
    i.l([(15, 27), (20, 31), (24, 25), (28, 32), (33, 27)], 2.6, 0)
    i.l([(39, 31), (43, 35), (46, 30), (50, 35)], 2.4, 0)


def _i_fish(i):
    i.p([(3, 32), (19, 15), (40, 15), (54, 32), (40, 49), (19, 49)])
    i.p([(48, 32), (63, 15), (63, 49)])
    i.p([(25, 15), (36, 4), (41, 15)])
    i.c(15, 28, 4, 0)
    i.arc(18, 32, 13, -52, 52, 2.4, 0)
    i.p([(30, 30), (41, 33), (30, 40)], 0)


def _i_tower(i):
    i.r(4, 29, 17, 33)
    i.r(43, 20, 17, 42)
    i.r(20, 9, 24, 53)
    i.r(30.5, 1, 3, 9)
    i.r(19.2, 9, 1.8, 53, 0)
    i.r(43, 9, 1.8, 53, 0)
    for y in (15, 25, 35, 45):
        i.r(24.5, y, 5.5, 6, 0)
        i.r(34, y, 5.5, 6, 0)
    for y in (35, 46):
        i.r(8, y, 5, 6, 0)
    for y in (27, 38, 49):
        i.r(49, y, 5, 6, 0)


ICON_FN = {
    "clapper": _i_clapper, "megaphone": _i_megaphone, "camera": _i_camera, "reel": _i_reel, "rec": _i_rec, "ticket": _i_ticket,
    "cash": _i_cash, "star": _i_star, "shield": _i_shield, "bolt": _i_bolt, "missile": _i_missile, "bullets": _i_bullets,
    "wrench": _i_wrench, "spray": _i_spray, "fire": _i_fire, "burst": _i_burst, "people": _i_people, "wheel": _i_wheel,
    "helmet": _i_helmet, "flag": _i_flag, "padlock": _i_padlock, "warning": _i_warning, "skull": _i_skull, "check": _i_check,
    "gear": _i_gear, "pad": _i_pad, "exit": _i_exit, "arrow": _i_arrow, "clock": _i_clock, "undo": _i_undo, "disk": _i_disk,
    "crosshair": _i_crosshair, "shoot1": _i_palm, "shoot2": _i_flask, "shoot3": _i_cactus, "shoot4": _i_volcano,
    "shoot5": _i_ferris, "shoot6": _i_mountain, "shoot7": _i_fish, "shoot8": _i_tower, "speaker": _i_speaker,
    "trophy": _i_trophy, "car": _i_car,
}
# the atlas: index = position in this list (8 columns)
ICON_NAMES = ["clapper", "megaphone", "camera", "reel", "rec", "ticket", "cash", "star",
              "shield", "bolt", "missile", "bullets", "wrench", "spray", "fire", "burst",
              "people", "wheel", "helmet", "flag", "padlock", "warning", "skull", "check",
              "gear", "pad", "exit", "arrow", "clock", "undo", "disk", "crosshair",
              "shoot1", "shoot2", "shoot3", "shoot4", "shoot5", "shoot6", "shoot7", "shoot8",
              "badge_check", "badge_lock", "speaker", "trophy", "car", "", "", ""]
ICON_INDEX = dict((n, k) for k, n in enumerate(ICON_NAMES) if n)
SHOOT_ICON = ["shoot%d" % (k + 1) for k in range(8)]
ICON_COLOUR = ("badge_check", "badge_lock")       # full-colour cells: draw with white
_imask = {}


def icon_mask(name, px):
    """L mask of an icon at px x px"""
    key = (name, px)
    if key not in _imask:
        i = IC(px)
        ICON_FN[name](i)
        _imask[key] = i.im.reduce(4)
    return _imask[key]


def _badge(kind):
    cv = Cv(64, 64)
    if kind == "badge_check":
        cv.circle(34, 35, 25, fill=INK)
        cv.circle(31, 31, 25, fill=PAL["Green"], outline=INK, width=4)
        cv.line([(19, 32), (28, 41), (44, 21)], INK, 8.5, caps=True)
    else:
        cv.circle(34, 35, 25, fill=INK)
        cv.circle(31, 31, 25, fill=STEEL, outline=INK, width=4)
        m = icon_mask("padlock", 30)
        cv.mask_paste(m.resize((30 * cv.ss, 30 * cv.ss), Image.LANCZOS), 16, 15, CHALK)
    return cv.out()


@tex("T_UI_Icons", (512, 384), kind="mask", draw="atlas: cell, any size from 20 to 64 px", cell=[64, 64], cols=8,
     cells=ICON_NAMES, colour_cells=list(ICON_COLOUR),
     note="white masks, drawn 60 px inside their cells; badge_check and badge_lock are full colour (draw white)")
def t_icons():
    out = Image.new("RGBA", (512, 384), (255, 255, 255, 0))
    for k, nm in enumerate(ICON_NAMES):
        if not nm:
            continue
        x, y = (k % 8) * 64, (k // 8) * 64
        if nm in ICON_COLOUR:
            out.alpha_composite(_badge(nm), (x, y))
        else:
            out.alpha_composite(solid(icon_mask(nm, 60), WHITE), (x + 2, y + 2))
    return out


BIG_ICONS = [b[1] for b in BRANCHES]


BIG_ICON = 144


@tex("T_UI_IconsBig", (BIG_ICON * 5, BIG_ICON), kind="mask", draw="atlas: cell, on T_UI_BranchBurst", cell=[BIG_ICON, BIG_ICON], cols=5,
     cells=["%s (%s)" % (b[1], b[0]) for b in BRANCHES], tint=["Ink", "Chalk"])
def t_icons_big():
    out = Image.new("RGBA", (BIG_ICON * 5, BIG_ICON), (255, 255, 255, 0))
    for k, nm in enumerate(BIG_ICONS):
        out.alpha_composite(solid(icon_mask(nm, BIG_ICON - 8), WHITE), (k * BIG_ICON + 4, 4))
    return out


@tex("T_UI_BranchBurst", (320, 320), kind="mask", draw="1:1", tint=["BranchDem", "BranchWhl", "BranchStn", "BranchCar", "BranchGet"],
     note="white burst, light halftone core, ink outline and hard shadow: the ink stays dark under the tint")
def t_branch_burst():
    cv = Cv(320, 320)
    c = 154.0
    outer = burst_pts(c, c, 142, 112, 16, rot=6, jit=0.10, seed=4)
    cv.poly([(x + 8, y + 9) for x, y in burst_pts(c, c, 150, 119, 16, rot=6, jit=0.10, seed=4)], fill=INK)
    cv.poly(burst_pts(c, c, 150, 119, 16, rot=6, jit=0.10, seed=4), fill=INK)
    cv.poly(outer, fill=(226, 226, 226))
    cv.poly(burst_pts(c, c, 112, 92, 16, rot=17, jit=0.10, seed=9), fill=WHITE)
    # halftone falling off from the lower right, in the darker tone
    msk = Image.new("L", cv.im.size, 0)
    ImageDraw.Draw(msk).polygon([(x * cv.ss, y * cv.ss) for x, y in burst_pts(c, c, 112, 92, 16, rot=17, jit=0.10, seed=9)], fill=255)

    def rad(x, y):
        t = ((x - c) + (y - c)) / 150.0
        return min(4.6, 7.5 * (t - 0.05)) if t > 0.05 else 0.0
    dots = ImageChops.multiply(halftone_mask((320, 320), 13.0, rad, origin=(c, c)), msk)
    cv.im.alpha_composite(solid(dots, (226, 226, 226)))
    return cv.out()


# ====================================================================== storyboard frames (one per shoot)
def _board_scene(cv, i, w=384, h=216):
    """flat vector location frame for shoot i (0..7), drawn on cv. -> (horizon v, road vanishing u)"""
    X = lambda u: u * w
    Y = lambda v: v * h
    k = w / 384.0
    OL = max(1.0, 2.2 * k)
    P = lambda v: v * cv.ss

    def bands(cols, y0, y1):
        n = len(cols)
        for j, c in enumerate(cols):
            cv.rect(0, Y(y0 + (y1 - y0) * j / n), w, Y((y1 - y0) / n) + 2, fill=c)

    def sun(u, v, r, c, stripes=None):
        cv.circle(X(u), Y(v), r * k, fill=c)
        if stripes:
            for j in range(4):
                cv.rect(X(u) - r * k, Y(v) + r * k * (0.15 + j * 0.22), 2 * r * k, r * k * (0.05 + 0.025 * j), fill=stripes)

    def road(hz, rc, k1, k2, lc, vx=0.5, bl=0.1, br=0.9):
        def ex(t, side):
            b = bl if side == 0 else br
            return X(vx + (b - vx) * t)
        cv.poly([(X(vx) - 2 * k, Y(hz)), (X(vx) + 2 * k, Y(hz)), (X(br), Y(1.02)), (X(bl), Y(1.02))], fill=rc)
        ts = [1.0, 0.72, 0.52, 0.38, 0.28, 0.2, 0.14, 0.1, 0.07, 0.04]
        for j in range(len(ts) - 1):
            t0, t1 = ts[j], ts[j + 1]
            y0, y1 = Y(hz + (1.02 - hz) * t0), Y(hz + (1.02 - hz) * t1)
            for side in (0, 1):
                sg = -1 if side == 0 else 1
                wd0, wd1 = 26 * k * t0, 26 * k * t1
                cv.poly([(ex(t0, side), y0), (ex(t0, side) + sg * wd0, y0), (ex(t1, side) + sg * wd1, y1), (ex(t1, side), y1)],
                        fill=k1 if j % 2 == 0 else k2)
            if j % 2 == 0:
                cv.poly([(X(vx) - 4 * k * t0, y0), (X(vx) + 4 * k * t0, y0), (X(vx) + 4 * k * t1, y1), (X(vx) - 4 * k * t1, y1)], fill=lc)
        cv.line([(X(vx), Y(hz)), (X(bl), Y(1.02))], INK, OL)
        cv.line([(X(vx), Y(hz)), (X(br), Y(1.02))], INK, OL)

    def tower(u, v, bw, bh, c, win=None, wc=None):
        cv.rect(X(u), Y(v) - bh * k, bw * k, bh * k + 2, fill=c, outline=INK, width=OL)
        if win:
            r = random.Random(int(u * 1000))
            for yy in range(int(bh / 9)):
                for xx in range(int(bw / 8)):
                    if r.random() < win:
                        cv.rect(X(u) + (3 + xx * 8) * k, Y(v) - bh * k + (4 + yy * 9) * k, 4 * k, 5 * k, fill=wc[r.randrange(len(wc))])

    def palm(u, v, hgt, lean=0.0):
        bx, by = X(u), Y(v)
        tx, ty = bx + lean * k, by - hgt * k
        cv.line([(bx, by), ((bx + tx) / 2 + lean * 0.3 * k, (by + ty) / 2), (tx, ty)], INK, 6 * k)
        cv.line([(bx, by), ((bx + tx) / 2 + lean * 0.3 * k, (by + ty) / 2), (tx, ty)], (150, 96, 70), 3.2 * k)
        for a in (-160, -125, -90, -55, -20, 20, 200):
            ar = math.radians(a)
            ex_, ey_ = tx + math.cos(ar) * 30 * k, ty + math.sin(ar) * 16 * k + 9 * k
            cv.poly([(tx, ty - 3 * k), (ex_, ey_), (tx, ty + 4 * k)], fill=(38, 150, 110), outline=INK, width=OL * 0.7)

    def ferris(u, v, r, c):
        cx_, cy_ = X(u), Y(v) - r * k
        cv.line([(cx_, cy_), (cx_ - r * 0.6 * k, Y(v))], INK, OL * 1.4)
        cv.line([(cx_, cy_), (cx_ + r * 0.6 * k, Y(v))], INK, OL * 1.4)
        cv.circle(cx_, cy_, r * k, outline=INK, width=OL * 2.4)
        cv.circle(cx_, cy_, r * k - OL * 0.6, outline=c, width=OL * 1.2)
        for j in range(8):
            a = math.radians(j * 45 + 10)
            cv.line([(cx_, cy_), (cx_ + math.cos(a) * r * k, cy_ + math.sin(a) * r * k)], INK, OL * 0.7)
            cv.circle(cx_ + math.cos(a) * r * k, cy_ + math.sin(a) * r * k, 3.4 * k, fill=(255, 226, 120), outline=INK, width=OL * 0.6)

    def pine(u, v, s_):
        for j in range(3):
            cv.poly([(X(u), Y(v) - (34 - j * 4) * s_ * k), (X(u) - (8 + j * 4) * s_ * k, Y(v) - (20 - j * 9) * s_ * k),
                     (X(u) + (8 + j * 4) * s_ * k, Y(v) - (20 - j * 9) * s_ * k)], fill=(28, 110, 112), outline=INK, width=OL * 0.7)

    if i == 0:      # BAYSHORE
        hz = 0.55
        bands([(255, 190, 150), (255, 166, 140), (255, 140, 144), (252, 120, 150)], 0, hz)
        sun(0.74, 0.4, 34, (255, 240, 190))
        cv.rect(0, Y(hz - 0.03), w, Y(0.05), fill=(73, 226, 213))
        cv.rect(0, Y(hz - 0.005), w, Y(0.012), fill=(200, 250, 240))
        cv.rect(0, Y(hz + 0.02), w, h, fill=(243, 207, 175))
        for (u, bw, bh, c) in ((0.02, 46, 52, (255, 182, 200)), (0.15, 34, 70, (160, 232, 220)), (0.25, 30, 44, (255, 232, 190))):
            tower(u, hz + 0.02, bw, bh, c, 0.5, [(255, 250, 220), (120, 90, 150)])
        ferris(0.86, hz + 0.01, 30, (255, 95, 178))
        road(hz + 0.02, (110, 92, 150), (255, 110, 170), (255, 240, 220), (255, 244, 220))
        palm(0.36, 0.86, 96, -14)
        palm(0.66, 0.74, 64, 10)
        hz += 0.02
    elif i == 1:    # CHEMICAL PLANT
        hz = 0.56
        bands([(180, 240, 200), (160, 232, 196), (140, 222, 190), (124, 210, 184)], 0, hz)
        for (u, v, r) in ((0.2, 0.2, 16), (0.26, 0.13, 20), (0.34, 0.09, 15), (0.7, 0.24, 14), (0.76, 0.16, 19)):
            cv.circle(X(u), Y(v), r * k, fill=(236, 252, 230), outline=INK, width=OL * 0.8)
        cv.rect(0, Y(hz), w, h, fill=(76, 128, 132))
        for (u, bw, bh, c) in ((0.16, 14, 86, (240, 236, 220)), (0.66, 14, 70, (240, 236, 220))):
            tower(u, hz, bw, bh, c)
            cv.rect(X(u), Y(hz) - bh * k + 8 * k, bw * k, 8 * k, fill=RED)
        for (u, rw, rh, c) in ((0.02, 44, 48, (150, 226, 96)), (0.25, 52, 60, (250, 214, 90)), (0.74, 60, 52, (150, 226, 96)), (0.9, 40, 66, (255, 150, 90))):
            cv.rect(X(u), Y(hz) - rh * k, rw * k, rh * k + 2, fill=c, outline=INK, width=OL)
            cv.ellipse(X(u) + rw * k / 2, Y(hz) - rh * k, rw * k / 2, 7 * k, fill=mix(c, WHITE, 0.4), outline=INK, width=OL)
            cv.rect(X(u), Y(hz) - rh * k * 0.5, rw * k, 4 * k, fill=INK)
        cv.line([(0, Y(hz - 0.1)), (w, Y(hz - 0.1))], INK, 5 * k)
        cv.line([(0, Y(hz - 0.1)), (w, Y(hz - 0.1))], (250, 214, 90), 2.6 * k)
        road(hz, (70, 76, 100), (250, 214, 90), (30, 28, 44), (240, 250, 230))
    elif i == 2:    # NEVADA DESERT
        hz = 0.56
        bands([(255, 222, 160), (255, 200, 136), (255, 176, 118), (255, 154, 108)], 0, hz)
        sun(0.3, 0.3, 40, (255, 246, 200), (255, 190, 120))
        for pts, c in (([(0.0, hz), (0.03, 0.34), (0.2, 0.33), (0.25, hz)], (200, 92, 70)),
                       ([(0.56, hz), (0.6, 0.4), (0.74, 0.39), (0.78, 0.46), (0.9, 0.45), (0.95, hz)], (214, 108, 76)),
                       ([(0.3, hz), (0.33, 0.47), (0.42, 0.47), (0.45, hz)], (232, 130, 90))):
            cv.poly([(X(u), Y(v)) for u, v in pts], fill=c, outline=INK, width=OL)
        cv.rect(0, Y(hz), w, h, fill=(244, 190, 130))
        road(hz, (112, 100, 140), (255, 150, 70), (255, 240, 214), (255, 244, 220))
        for (u, v, s_) in ((0.16, 0.9, 1.0), (0.86, 0.8, 0.75)):
            bx, by = X(u), Y(v)
            parts = ((0, -58, 13, 58), (-17, -40, 9, 22), (17, -48, 9, 26), (-17, -24, 20, 9), (4, -30, 22, 9))
            for (dx, dy, ww, hh) in parts:
                cv.rect(bx + dx * s_ * k - ww * s_ * k / 2, by + dy * s_ * k, ww * s_ * k, hh * s_ * k, fill=INK, r=4 * s_ * k)
            for (dx, dy, ww, hh) in parts:
                cv.rect(bx + dx * s_ * k - ww * s_ * k / 2 + OL, by + dy * s_ * k + OL, ww * s_ * k - 2 * OL, hh * s_ * k - 2 * OL,
                        fill=(60, 170, 120), r=3 * s_ * k)
    elif i == 3:    # VOLCANO
        hz = 0.58
        bands([(58, 16, 44), (94, 22, 48), (140, 34, 48), (196, 60, 44)], 0, hz)
        cv.poly([(X(0.22), Y(hz)), (X(0.43), Y(0.16)), (X(0.57), Y(0.16)), (X(0.8), Y(hz))], fill=(44, 26, 48), outline=INK, width=OL)
        cv.poly([(X(0.43), Y(0.16)), (X(0.46), Y(0.2)), (X(0.5), Y(0.17)), (X(0.54), Y(0.21)), (X(0.57), Y(0.16))], fill=(255, 170, 60))
        for pts in ([(0.47, 0.19), (0.44, 0.34), (0.4, 0.5)], [(0.53, 0.2), (0.56, 0.36), (0.63, 0.52)], [(0.5, 0.18), (0.5, 0.4)]):
            cv.line([(X(u), Y(v)) for u, v in pts], (255, 120, 40), 4 * k)
            cv.line([(X(u), Y(v)) for u, v in pts], (255, 220, 110), 1.6 * k)
        for (u, v, r) in ((0.47, 0.08, 9), (0.53, 0.05, 12), (0.58, 0.1, 8), (0.42, 0.04, 7)):
            cv.circle(X(u), Y(v), r * k, fill=(110, 40, 60), outline=INK, width=OL * 0.7)
        rr = random.Random(4)
        for _ in range(26):
            cv.circle(X(rr.random()), Y(rr.random() * hz), rr.uniform(0.8, 2.0) * k, fill=(255, 190, 90))
        cv.rect(0, Y(hz), w, h, fill=(36, 18, 34))
        cv.poly([(X(0.0), Y(0.8)), (X(0.12), Y(0.7)), (X(0.2), Y(0.86)), (X(0.1), Y(1)), (X(0), Y(1))], fill=(255, 120, 40))
        road(hz, (58, 44, 70), (255, 140, 50), (40, 22, 40), (255, 210, 120))
    elif i == 4:    # THEME PARK
        hz = 0.57
        bands([(170, 222, 255), (196, 222, 255), (226, 214, 250), (255, 208, 236)], 0, hz)
        cv.rect(0, Y(hz), w, h, fill=(170, 236, 206))
        ferris(0.2, hz, 42, (255, 110, 196))
        cx_, cy_ = X(0.74), Y(hz) - 40 * k
        cv.circle(cx_, cy_, 40 * k, outline=INK, width=9 * k)
        cv.circle(cx_, cy_, 38 * k, outline=(255, 110, 196), width=5 * k)
        cv.line([(X(0.5), Y(hz) - 6 * k), (X(1.0), Y(hz) - 6 * k)], INK, 9 * k)
        cv.line([(X(0.5), Y(hz) - 6 * k), (X(1.0), Y(hz) - 6 * k)], (255, 110, 196), 5 * k)
        tx = X(0.44)
        cv.poly([(tx - 26 * k, Y(hz)), (tx, Y(hz) - 46 * k), (tx + 26 * k, Y(hz))], fill=(255, 244, 220), outline=INK, width=OL)
        cv.poly([(tx - 9 * k, Y(hz)), (tx, Y(hz) - 46 * k), (tx + 9 * k, Y(hz))], fill=RED)
        cv.poly([(tx, Y(hz) - 46 * k), (tx, Y(hz) - 60 * k), (tx + 12 * k, Y(hz) - 54 * k)], fill=(255, 196, 76), outline=INK, width=OL * 0.7)
        road(hz, (120, 104, 160), (255, 110, 196), (255, 250, 230), (255, 250, 230))
    elif i == 5:    # ICE MOUNTAIN
        hz = 0.56
        bands([(96, 176, 255), (130, 196, 255), (170, 216, 255), (206, 234, 255)], 0, hz)
        sun(0.5, 0.2, 26, (255, 246, 190))
        for (a, b, c_, top) in ((-0.05, 0.2, 0.45, 0.14), (0.3, 0.5, 0.72, 0.24), (0.58, 0.82, 1.06, 0.1)):
            cv.poly([(X(a), Y(hz)), (X(b), Y(top)), (X(c_), Y(hz))], fill=(250, 252, 255), outline=INK, width=OL)
            cv.poly([(X(b), Y(top)), (X(c_), Y(hz)), (X(b + 0.04), Y(hz)), (X(b - 0.02), Y(top + 0.16))], fill=(150, 190, 240))
            cv.line([(X(a), Y(hz)), (X(b), Y(top)), (X(c_), Y(hz))], INK, OL)
        cv.rect(0, Y(hz), w, h, fill=(240, 246, 255))
        road(hz, (120, 132, 172), (238, 60, 62), (255, 255, 255), (255, 255, 255))
        for (u, v, s_) in ((0.12, 0.82, 1.3), (0.22, 0.7, 0.9), (0.84, 0.76, 1.1), (0.93, 0.92, 1.5)):
            pine(u, v, s_)
        for j, c in enumerate(((255, 79, 163), (73, 226, 213), (255, 196, 76))):
            fx = X(0.7 + j * 0.035)
            cv.line([(fx, Y(hz + 0.1 + j * 0.02)), (fx, Y(hz - 0.04 + j * 0.02))], INK, OL)
            cv.rect(fx, Y(hz - 0.04 + j * 0.02), 7 * k, 12 * k, fill=c, outline=INK, width=OL * 0.6)
    elif i == 6:    # UNDER THE SEA
        hz = 0.5
        bands([(20, 120, 150), (16, 100, 140), (14, 82, 128), (12, 64, 112), (10, 50, 96), (10, 40, 84)], 0, 1)
        for (u0, u1) in ((0.1, 0.02), (0.36, 0.2), (0.62, 0.54), (0.86, 0.9)):
            cv.poly([(X(u0), 0), (X(u0 + 0.07), 0), (X(u1 + 0.2), Y(0.9)), (X(u1), Y(0.9))], fill=rgba((140, 240, 230), 0.12))
        cv.rect(0, Y(0.62), w, h, fill=(12, 44, 76))
        road(hz, (22, 62, 96), (73, 226, 213), (12, 40, 70), (150, 250, 240))
        for t in (1.0, 0.6, 0.36, 0.2):
            rx, ry = 0.48 * w * t, 0.6 * h * t
            cy_ = Y(hz + (1.02 - hz) * t)
            cv.d.arc([P(X(0.5) - rx), P(cy_ - ry), P(X(0.5) + rx), P(cy_ + ry)], 180, 360, fill=rgba(INK), width=int(P(6 * k * t + 2)))
            cv.d.arc([P(X(0.5) - rx), P(cy_ - ry), P(X(0.5) + rx), P(cy_ + ry)], 180, 360, fill=(73, 226, 213, 255), width=int(P(3 * k * t + 1)))
        rr = random.Random(7)
        for _ in range(16):
            cv.circle(X(rr.random()), Y(rr.random() * 0.7), rr.uniform(1.5, 4) * k, outline=(170, 250, 244), width=1 * k)
        for (u, v, s_, c) in ((0.16, 0.3, 1.2, (255, 150, 90)), (0.8, 0.22, 0.9, (255, 110, 196)), (0.3, 0.14, 0.7, (255, 226, 120))):
            fx, fy = X(u), Y(v)
            cv.ellipse(fx, fy, 13 * s_ * k, 7 * s_ * k, fill=c, outline=INK, width=OL * 0.7)
            cv.poly([(fx - 12 * s_ * k, fy), (fx - 22 * s_ * k, fy - 7 * s_ * k), (fx - 22 * s_ * k, fy + 7 * s_ * k)], fill=c, outline=INK, width=OL * 0.7)
    else:           # CITY CENTER
        hz = 0.6
        bands([(40, 26, 90), (70, 36, 120), (120, 52, 150), (196, 70, 160), (255, 100, 170)], 0, hz)
        rr = random.Random(8)
        for _ in range(20):
            cv.circle(X(rr.random()), Y(rr.random() * 0.3), 1.0 * k, fill=(255, 240, 220))
        cols = [(60, 44, 120), (44, 34, 96), (84, 56, 140), (36, 28, 80)]
        u = -0.02
        j = 0
        while u < 1.0:
            bw = 26 + (j * 37) % 22
            bh = 50 + (j * 53) % 66
            if 0.4 < u < 0.56:
                bh *= 0.45
            tower(u, hz, bw, bh, cols[j % 4], 0.55, [(255, 226, 120), (73, 226, 213), (255, 110, 196)])
            u += bw / 384.0 + 0.004
            j += 1
        cv.rect(X(0.07), Y(0.2), 30 * k, 12 * k, fill=(255, 79, 163), outline=INK, width=OL)
        cv.rect(X(0.8), Y(0.3), 12 * k, 30 * k, fill=(73, 226, 213), outline=INK, width=OL)
        cv.rect(0, Y(hz), w, h, fill=(30, 24, 64))
        road(hz, (52, 44, 96), (255, 79, 163), (30, 24, 64), (255, 226, 120))
    return hz


# ---- the bosses, small, seen ahead on the road. (x, y) = bottom centre, s = scale (1 = about 60 px wide)
def _wheels(cv, x, y, s, half, w=11, h=11):
    for sx in (-1, 1):
        cv.rect(x + sx * half * s - w * s / 2.0, y - h * s, w * s, h * s, fill=(26, 20, 38), outline=INK, width=1.6, r=2.5 * s)


def _boss_juggernaut(cv, x, y, s):
    _wheels(cv, x, y, s, 22, 13, 13)
    cv.rect(x - 31 * s, y - 56 * s, 62 * s, 50 * s, fill=(74, 60, 118), outline=INK, width=2.4)
    cv.rect(x - 31 * s, y - 56 * s, 62 * s, 9 * s, fill=(110, 94, 160))
    cv.rect(x - 31 * s, y - 34 * s, 62 * s, 8 * s, fill=RED)
    cv.rect(x - 1.2 * s, y - 47 * s, 2.4 * s, 41 * s, fill=INK)
    for sx in (-1, 1):
        cv.rect(x + sx * 22 * s - 5 * s, y - 20 * s, 10 * s, 6 * s, fill=AMBER, outline=INK, width=1.4)
        for yy in (-50, -42):
            cv.circle(x + sx * 26 * s, y + yy * s + 6 * s, 1.5 * s, fill=INK)
    cv.rect(x - 31 * s, y - 56 * s, 62 * s, 50 * s, outline=INK, width=2.4)
    cv.rect(x - 34 * s, y - 9 * s, 68 * s, 5 * s, fill=(44, 36, 70), outline=INK, width=1.6)


def _boss_tanker(cv, x, y, s):
    _wheels(cv, x, y, s, 21, 13, 12)
    cv.rect(x - 28 * s, y - 14 * s, 56 * s, 8 * s, fill=(60, 66, 84), outline=INK, width=1.8)
    cv.circle(x, y - 34 * s, 27 * s, fill=(150, 226, 96), outline=INK, width=2.4)
    cv.circle(x, y - 34 * s, 20 * s, outline=(96, 170, 70), width=2 * s)
    cv.poly([(x, y - 48 * s), (x + 12 * s, y - 27 * s), (x - 12 * s, y - 27 * s)], fill=AMBER, outline=INK, width=1.6)
    cv.rect(x - 1.3 * s, y - 42 * s, 2.6 * s, 8 * s, fill=INK)
    cv.circle(x, y - 31 * s, 1.6 * s, fill=INK)
    cv.arc(x - 6 * s, y - 40 * s, 15 * s, 200, 262, (220, 255, 190), 3)


def _boss_carrier(cv, x, y, s):
    _wheels(cv, x, y, s, 23, 12, 12)
    cv.rect(x - 32 * s, y - 16 * s, 64 * s, 10 * s, fill=(70, 48, 60), outline=INK, width=2)
    for sx in (-1, 1):                                       # the frame
        cv.rect(x + sx * 29 * s - 2.5 * s, y - 60 * s, 5 * s, 46 * s, fill=(255, 226, 150), outline=INK, width=1.5)
    cv.rect(x - 32 * s, y - 62 * s, 64 * s, 6 * s, fill=(255, 226, 150), outline=INK, width=1.6)
    cv.rect(x - 30 * s, y - 39 * s, 60 * s, 5 * s, fill=(255, 226, 150), outline=INK, width=1.6)
    for (dx, dy) in ((-14, -43), (13, -43), (-13, -20), (14, -20)):   # bikes on the decks
        cv.circle(x + dx * s, y + dy * s, 5.5 * s, fill=(30, 24, 44), outline=INK, width=1.3)
        cv.rect(x + dx * s - 3 * s, y + dy * s - 12 * s, 6 * s, 9 * s, fill=RED, outline=INK, width=1.3)
    cv.poly([(x, y - 77 * s), (x + 9 * s, y - 68 * s), (x, y - 59 * s), (x - 9 * s, y - 68 * s)], fill=RED, outline=INK, width=1.6)


def _boss_firetruck(cv, x, y, s):
    _wheels(cv, x, y, s, 22, 13, 13)
    cv.rect(x - 30 * s, y - 52 * s, 60 * s, 46 * s, fill=(226, 44, 52), outline=INK, width=2.4)
    cv.rect(x - 30 * s, y - 52 * s, 60 * s, 8 * s, fill=(255, 120, 110))
    cv.rect(x - 22 * s, y - 43 * s, 44 * s, 21 * s, fill=CHALK, outline=INK, width=1.6)
    cv.text(x, y - 40 * s, "13", "BUN", 15 * s, INK, al=1, track=1.5)
    cv.rect(x - 30 * s, y - 52 * s, 60 * s, 46 * s, outline=INK, width=2.4)
    for sx in (-1, 1):
        cv.rect(x + sx * 22 * s - 5 * s, y - 20 * s, 10 * s, 6 * s, fill=AMBER, outline=INK, width=1.4)
    cv.rect(x - 24 * s, y - 60 * s, 48 * s, 7 * s, fill=(190, 196, 216), outline=INK, width=1.6)    # ladder
    for k_ in range(5):
        cv.rect(x - 20 * s + k_ * 9.5 * s, y - 60 * s, 2 * s, 7 * s, fill=INK)
    for sx in (-1, 1):                                       # flame hoses
        fx = x + sx * 30 * s
        cv.poly([(fx, y - 34 * s), (fx + sx * 26 * s, y - 46 * s), (fx + sx * 16 * s, y - 32 * s), (fx + sx * 30 * s, y - 24 * s),
                 (fx, y - 24 * s)], fill=(255, 140, 50), outline=INK, width=1.5)
        cv.poly([(fx, y - 32 * s), (fx + sx * 14 * s, y - 36 * s), (fx + sx * 10 * s, y - 29 * s), (fx, y - 27 * s)], fill=(255, 226, 120))


def _boss_clown(cv, x, y, s):
    _wheels(cv, x, y, s, 17, 11, 11)
    for (dx, dy, c) in ((-16, -74, RED), (2, -84, CYAN), (18, -72, AMBER)):          # bomb balloons
        cv.line([(x, y - 40 * s), (x + dx * s, y + (dy + 11) * s)], INK, 1.3)
        cv.ellipse(x + dx * s, y + dy * s, 10 * s, 12 * s, fill=c, outline=INK, width=1.8)
        cv.circle(x + dx * s - 3 * s, y + dy * s - 4 * s, 2.2 * s, fill=(255, 255, 255, 200))
    cv.rect(x - 25 * s, y - 30 * s, 50 * s, 24 * s, fill=(255, 110, 196), outline=INK, width=2.2, r=9 * s)
    cv.rect(x - 16 * s, y - 44 * s, 32 * s, 18 * s, fill=(255, 226, 120), outline=INK, width=2.2, r=7 * s)
    cv.rect(x - 11 * s, y - 40 * s, 22 * s, 9 * s, fill=(60, 44, 110), outline=INK, width=1.4, r=3 * s)
    for (dx, dy) in ((-15, -20), (0, -14), (15, -20), (-7, -9), (8, -24)):
        cv.circle(x + dx * s, y + dy * s, 3 * s, fill=CHALK)
    cv.circle(x, y - 6 * s, 4.5 * s, fill=RED, outline=INK, width=1.4)


def _boss_yeti(cv, x, y, s):
    fur, shade = (250, 252, 255), (160, 196, 240)
    cv.ellipse(x, y - 4 * s, 26 * s, 5 * s, fill=(120, 132, 172, 150))
    for sx in (-1, 1):                                       # arms up
        cv.poly([(x + sx * 14 * s, y - 44 * s), (x + sx * 34 * s, y - 72 * s), (x + sx * 42 * s, y - 64 * s), (x + sx * 24 * s, y - 34 * s)],
                fill=fur, outline=INK, width=2.2)
        cv.circle(x + sx * 39 * s, y - 70 * s, 7 * s, fill=fur, outline=INK, width=2)
        cv.rect(x + sx * 10 * s - 7 * s, y - 16 * s, 14 * s, 14 * s, fill=fur, outline=INK, width=2, r=4 * s)
    cv.ellipse(x, y - 34 * s, 20 * s, 24 * s, fill=fur, outline=INK, width=2.4)
    cv.poly([(x + 4 * s, y - 56 * s), (x + 19 * s, y - 40 * s), (x + 17 * s, y - 20 * s), (x + 4 * s, y - 12 * s), (x + 12 * s, y - 34 * s)], fill=shade)
    cv.ellipse(x, y - 62 * s, 13 * s, 12 * s, fill=fur, outline=INK, width=2.2)
    cv.ellipse(x, y - 60 * s, 8 * s, 6.5 * s, fill=(120, 170, 235), outline=INK, width=1.4)
    for sx in (-1, 1):
        cv.circle(x + sx * 3.4 * s, y - 62 * s, 1.5 * s, fill=INK)
    cv.rect(x - 4 * s, y - 58.5 * s, 8 * s, 2.6 * s, fill=CHALK, outline=INK, width=1)


def _boss_shark(cv, x, y, s):
    for dx in (-20, 20):
        cv.circle(x + dx * s, y - 7 * s, 7 * s, fill=(26, 20, 38), outline=INK, width=1.6)
    top, belly = (126, 148, 176), (236, 244, 250)
    body = [(x - 40 * s, y - 24 * s), (x - 22 * s, y - 40 * s), (x + 14 * s, y - 40 * s), (x + 30 * s, y - 28 * s),
            (x + 44 * s, y - 42 * s), (x + 41 * s, y - 22 * s), (x + 44 * s, y - 8 * s), (x + 28 * s, y - 16 * s), (x - 20 * s, y - 10 * s)]
    cv.poly(body, fill=top, outline=INK, width=2.3)
    cv.poly([(x - 40 * s, y - 24 * s), (x + 28 * s, y - 20 * s), (x + 28 * s, y - 16 * s), (x - 20 * s, y - 10 * s)], fill=belly)
    cv.poly([(x - 8 * s, y - 40 * s), (x + 4 * s, y - 62 * s), (x + 12 * s, y - 40 * s)], fill=top, outline=INK, width=2.2)
    cv.poly([(x - 38 * s, y - 22 * s), (x - 20 * s, y - 20 * s), (x - 24 * s, y - 14 * s)], fill=RED)
    for k_ in range(4):
        cv.poly([(x - 36 * s + k_ * 4.5 * s, y - 22 * s), (x - 34 * s + k_ * 4.5 * s, y - 18 * s), (x - 32 * s + k_ * 4.5 * s, y - 22 * s)], fill=CHALK)
    cv.circle(x - 24 * s, y - 30 * s, 2.4 * s, fill=INK)
    for k_ in range(3):
        cv.line([(x - 10 * s + k_ * 5 * s, y - 33 * s), (x - 12 * s + k_ * 5 * s, y - 24 * s)], INK, 1.3)
    cv.poly(body, outline=INK, width=2.3)


def _boss_limo(cv, x, y, s):
    _wheels(cv, x, y, s, 27, 12, 10)
    cv.rect(x - 36 * s, y - 28 * s, 72 * s, 23 * s, fill=(34, 28, 52), outline=INK, width=2.4, r=4 * s)
    cv.rect(x - 26 * s, y - 42 * s, 52 * s, 16 * s, fill=(52, 44, 84), outline=INK, width=2.2, r=5 * s)
    cv.rect(x - 21 * s, y - 38 * s, 42 * s, 9 * s, fill=(120, 150, 220), outline=INK, width=1.4, r=2 * s)
    cv.rect(x - 36 * s, y - 16 * s, 72 * s, 3 * s, fill=AMBER)
    for sx in (-1, 1):
        cv.rect(x + sx * 27 * s - 6 * s, y - 25 * s, 12 * s, 5 * s, fill=RED, outline=INK, width=1.3)
        cv.rect(x + sx * 12 * s - 3 * s, y - 56 * s, 6 * s, 15 * s, fill=(150, 156, 182), outline=INK, width=1.5)   # roof guns
        cv.circle(x + sx * 12 * s, y - 58 * s, 3 * s, fill=(255, 226, 120), outline=INK, width=1.2)
    cv.rect(x - 9 * s, y - 13 * s, 18 * s, 6 * s, fill=CHALK, outline=INK, width=1.2)


BOSS_FN = [_boss_juggernaut, _boss_tanker, _boss_carrier, _boss_firetruck, _boss_clown, _boss_yeti, _boss_shark, _boss_limo]
# per board: boss (u, v of its bottom centre, scale), ring (rx, ry), label (u, v of its centre, angle)
BOARD_BOSS = [
    dict(u=0.50, v=0.86, s=0.86, ring=(46, 40), lab=(0.26, 0.17), ang=-5),
    dict(u=0.50, v=0.87, s=0.84, ring=(42, 40), lab=(0.52, 0.30), ang=-4),
    dict(u=0.50, v=0.90, s=0.80, ring=(44, 46), lab=(0.72, 0.17), ang=4),
    dict(u=0.50, v=0.88, s=0.84, ring=(54, 42), lab=(0.20, 0.30), ang=-5),
    dict(u=0.50, v=0.93, s=0.78, ring=(40, 50), lab=(0.60, 0.13), ang=4),
    dict(u=0.50, v=0.90, s=0.86, ring=(50, 46), lab=(0.22, 0.42), ang=-5),
    dict(u=0.50, v=0.88, s=0.90, ring=(56, 38), lab=(0.52, 0.27), ang=-4),
    dict(u=0.50, v=0.86, s=0.84, ring=(48, 36), lab=(0.26, 0.18), ang=-5),
]


def _tape_label(s, ss):
    """a strip of tape with a handwritten name, at SS. -> RGBA layer"""
    tmp = Cv(8, 8, ss=ss)
    cap = 15.0
    w = tmp.tw(s, "DAF", cap) + 26
    h = 28.0
    cv = Cv(w + 6, h + 6, ss=ss)
    c0 = (250, 240, 204)
    pts = [(3, 3)]
    n = 5
    for k in range(n):
        pts.append((3 + (3.5 if k % 2 == 0 else 0), 3 + (k + 0.5) * h / n))
    pts += [(3, 3 + h), (3 + w, 3 + h)]
    for k in range(n):
        pts.append((3 + w - (3.5 if k % 2 == 0 else 0), 3 + h - (k + 0.5) * h / n))
    pts.append((3 + w, 3))
    cv.poly([(x + 2.5, y + 3) for x, y in pts], fill=rgba(INK, 0.5))
    cv.poly(pts, fill=c0, outline=INK, width=1.6)
    cv.text(3 + w / 2.0, 3 + 5.5, s, "DAF", cap, INK, al=1)
    return cv.im


def _board(i, ring=True, ss=None):
    """-> Cv with board i drawn (384 x 216 units)"""
    cv = Cv(384, 216, bg=(0, 0, 0), ss=ss)
    _board_scene(cv, i)
    b = BOARD_BOSS[i]
    bx, by = b["u"] * 384, b["v"] * 216
    cv.ellipse(bx, by - 2, 34 * b["s"], 5 * b["s"], fill=rgba(INK, 0.32))
    BOSS_FN[i](cv, bx, by, b["s"])
    if ring:
        rx, ry = b["ring"]
        cy = by - ry + 6
        ringl = Cv(384, 216, ss=cv.ss)
        ringl.line(grease_ring_pts(bx, cy, rx, ry, seed=30 + i), PINK, 4.2, caps=True)
        # the pencil's arrow from the name to the ring
        lx, ly = b["lab"][0] * 384, b["lab"][1] * 216
        dx, dy = bx - lx, cy - ly
        d = math.hypot(dx, dy)
        ux, uy = dx / d, dy / d
        ax, ay = bx - ux * (rx + 6) * (abs(ux) + abs(uy) * rx / ry) / (abs(ux) + abs(uy)), cy - uy * (ry + 6)
        sx_, sy_ = lx + ux * 30, ly + uy * 18
        if math.hypot(ax - sx_, ay - sy_) > 16:
            mx, my = (sx_ + ax) / 2 - uy * 7, (sy_ + ay) / 2 + ux * 7
            ringl.line([(sx_, sy_), (mx, my), (ax, ay)], PINK, 3.4, caps=True)
            for sg in (-1, 1):
                a = math.atan2(ay - my, ax - mx) + sg * 2.6
                ringl.line([(ax, ay), (ax + math.cos(a) * 11, ay + math.sin(a) * 11)], PINK, 3.4, caps=True)
        cv.im.alpha_composite(_wax(ringl.im, 80 + i))
        cv.paste_c(_tape_label(BOSS_HAND[i], cv.ss), lx, ly, b["ang"])
    return cv


@tex("T_UI_Boards", (1536, 432), draw="atlas: cell, 1:1 or smaller", cell=[384, 216], cols=4, cells=[s[0] for s in SHOOTS],
     note="cell = shoot - 1; the boss is ringed in pink grease pencil and named on a strip of tape (baked); the ink frame round a board is drawn by the game")
def t_boards():
    out = Image.new("RGBA", (1536, 432), (0, 0, 0, 255))
    for i in range(8):
        out.paste(_board(i).out(), ((i % 4) * 384, (i // 4) * 216))
    return out


@tex("T_UI_BoardThumbs", (1024, 72), draw="atlas: cell", cell=[128, 72], cols=8, cells=[s[0] for s in SHOOTS],
     note="the same eight frames small and clean (no ring, no name) for the shoot list")
def t_board_thumbs():
    out = Image.new("RGBA", (1024, 72), (0, 0, 0, 255))
    for i in range(8):
        im = _board(i, ring=False).im.convert("RGB").resize((128, 72), Image.LANCZOS)
        out.paste(im, (i * 128, 0))
    return out


# ====================================================================== shoot lockups (two inks, skew baked)
LOCKUP_SKEW = 0.176         # tan(10 degrees)


def _lockup(i, W=448, H=128):
    name, sub = SHOOTS[i]
    c2 = PAL["Shoot%d" % (i + 1)]
    ss = SS
    # the name: Bungee, condensed to fit, sheared, chalk with an ink stroke and the shoot's colour misregistered behind it
    cap = 58.0
    availw = W - 46 - LOCKUP_SKEW * 80
    f = fontpx("BUN", cap / CAPR["BUN"] * ss)
    lay, ox, oy = _text_layer(name, f, CHALK, 1.0 * ss)
    natw = lay.width / float(ss)
    xs = min(0.80, availw / natw)
    if xs < 0.60:
        cap *= xs / 0.60
        f = fontpx("BUN", cap / CAPR["BUN"] * ss)
        lay, ox, oy = _text_layer(name, f, CHALK, 1.0 * ss)
        xs = 0.60
    lay = lay.crop(lay.getbbox())
    lay = lay.resize((max(1, int(lay.width * xs)), lay.height), Image.LANCZOS)
    lay, _ = shear_layer(lay, LOCKUP_SKEW)
    front, p1 = inked(lay, 4.5 * ss, INK)
    a = front.getchannel("A")
    both = Image.new("RGBA", (front.width + 8 * ss, front.height + 8 * ss), (0, 0, 0, 0))
    both.alpha_composite(solid(a, c2), (6 * ss, 6 * ss))
    both.alpha_composite(front, (0, 0))
    art, p2 = inked(both, 2.6 * ss, INK)
    cv = Cv(W, H)
    # the subtitle strip: ink, skewed the same way, the shoot's number on a block of its colour
    sh, sy = 30.0, H - 36.0
    scap = 14.5
    sw = min(W - 16, 54 + cv.tw(sub, "RAJ", scap, 2.2) + 30)
    lean = LOCKUP_SKEW * sh
    x0 = 4.0
    cv.poly(para(x0 + 4, sy + 4, sw, sh, lean), fill=c2)
    cv.poly(para(x0, sy, sw, sh, lean), fill=INK)
    cv.poly([(x0 + lean, sy), (x0 + lean + 44, sy), (x0 + 44, sy + sh), (x0, sy + sh)], fill=c2)
    cv.poly(para(x0, sy, sw, sh, lean), outline=INK, width=2.5)
    cv.text(x0 + 22 + lean / 2, sy + (sh - 16) / 2.0, "%02d" % (i + 1), "BUN", 16, INK, al=1)
    cv.text(x0 + 56, sy + (sh - scap) / 2.0, sub, "RAJ", scap, CHALK, track=2.2, maxw=sw - 70)
    # the name sits on top, its foot overlapping the strip
    ah = art.height / float(ss)
    cv.paste(art, 2, max(0.0, sy - ah + 14))
    return cv.out()


@tex("T_UI_Lockups", (896, 512), draw="atlas: cell, 1:1", cell=[448, 128], cols=2, cells=["%s / %s" % s for s in SHOOTS],
     note="cell = shoot - 1; chalk name with ink stroke over the shoot's colour, subtitle on an ink strip; the skew is baked")
def t_lockups():
    out = Image.new("RGBA", (896, 512), (0, 0, 0, 0))
    for i in range(8):
        comp(out, _lockup(i), (i % 2) * 448, (i // 2) * 128)
    return out


# ====================================================================== controls
PLAQUE_STATES = ["idle", "focus", "active"]


@tex("T_UI_Plaque", (288, 72), draw="atlas: 3-slice-h of the cell (the cell is 66 px of plaque + 6 px of shadow)", cell=[96, 72], cols=3,
     cells=list(PLAQUE_STATES), slice=[30, 0, 34, 0],
     note="two screws in the caps. idle: Slate2. focus: Steel with a chalk inner line. active: Paper with an amber stripe head, open at the foot (it joins the board below)")
def t_plaque():
    cv = Cv(288, 72)
    for k, st in enumerate(PLAQUE_STATES):
        x, w, h = k * 96, 90.0, 66.0
        if st == "active":
            cv.rect(x + 6, 6, w, 66, fill=INK)
            cv.rect(x, 0, w, 72 + 6, fill=PAPER, outline=INK, width=3)
            cv.rect(x + 3, 3, w - 6, 8, fill=AMBER)
            c = -8
            while c < w + 8:
                cv.poly([(x + c + 8, 3), (x + c + 14, 3), (x + c + 6, 11), (x + c, 11)], fill=INK)
                c += 14
            cv.rect(x + 3, 11, w - 6, 2, fill=INK)
            scr = PAPER_D
        elif st == "focus":
            cv.rect(x + 6, 6, w, h, fill=INK, r=6)
            cv.rect(x, 0, w, h, fill=mix(STEEL, MUTE, 0.12), outline=INK, width=3, r=6)
            cv.rect(x + 5, 5, w - 10, h - 10, outline=CHALK, width=2, r=4)
            scr = (150, 146, 180)
        else:
            cv.rect(x + 6, 6, w, h, fill=INK, r=6)
            cv.rect(x, 0, w, h, fill=SLATE2, outline=INK, width=3, r=6)
            cv.rect(x + 6, 5, w - 12, 2, fill=rgba(CHALK, 0.14))
            scr = (118, 114, 148)
        for sx in (x + 14, x + w - 14):
            cy = 33 if st != "active" else 40
            cv.circle(sx, cy, 4, fill=scr, outline=INK, width=1.5)
            cv.line([(sx - 2.2, cy + 2.2), (sx + 2.2, cy - 2.2)], INK, 1.2)
    return cv.out()


@tex("T_UI_Keycap", (96, 48), draw="atlas: 9-slice of the cell", cell=[48, 48], cols=2, cells=["dark cap (on paper): Chalk text", "light cap (on dark): Ink text"],
     slice=[12, 10, 12, 14], note="the cap is 44 px high in the cell, then a 4 px ink lip; draw it 28 to 44 px high")
def t_keycap():
    cv = Cv(96, 48)
    for k, (body, top) in enumerate(((STEEL, mix(STEEL, MUTE, 0.35)), ((255, 250, 238), WHITE))):
        x = k * 48
        cv.rect(x + 1, 5, 46, 43, fill=INK, r=8)
        cv.rect(x + 1, 1, 46, 43, fill=body, outline=INK, width=2.5, r=8)
        cv.rect(x + 7, 5, 34, 2.5, fill=rgba(top, 0.9), r=1)
    return cv.out()


@tex("T_UI_Cursor", (96, 96), kind="mask", draw="9-slice, centre not drawn", slice=[26, 26, 32, 32], tint=["Pink", "Chalk", "Amber"],
     note="the frame's outer edge is the rectangle given (90 x 90 here) and its hard shadow runs 6 px out to the right and below; the band is 10 px (6 px of colour between two ink lines)")
def t_cursor():
    cv = Cv(96, 96)
    for (ox, oy, c0, c1) in ((6, 6, INK, None), (0, 0, INK, WHITE)):
        cv.rect(ox, oy, 90, 90, outline=c0, width=10)
        if c1:
            cv.rect(ox + 2, oy + 2, 86, 86, outline=c1, width=6)
    return cv.out()


# ====================================================================== paper pieces
@tex("T_UI_SheetFrame", (192, 192), draw="9-slice, centre not drawn (the live view shows through)", slice=[44, 44, 52, 52],
     note="30 px of border (ink, 24 px of paper with registration marks, ink), a hard shadow 8 px out to the right and below, and the cut edge's shadow 6 px into the window. The sheet's outer edge is the rectangle given, the shadow is outside it")
def t_sheet_frame():
    cv = Cv(192, 192)
    W = 184.0
    cv.rect(8, 8, W, W, outline=INK, width=30)                      # hard shadow of the frame
    cv.rect(0, 0, W, W, outline=INK, width=30)
    cv.rect(3, 3, W - 6, W - 6, outline=PAPER, width=24)
    im = cv.out()
    # paper grain on the border
    g = tint_img(t_paper(), PAPER).resize((192, 192))
    band = Image.new("L", (192, 192), 0)
    d = ImageDraw.Draw(band)
    d.rectangle([3, 3, 180, 180], fill=255)
    d.rectangle([27, 27, 156, 156], fill=0)
    im.paste(g, (0, 0), band)
    # the cut edge's shadow on what is behind the window, and nothing else in the window
    px = im.load()
    for y in range(30, 154):
        for x in range(30, 154):
            px[x, y] = (22, 18, 30, 110) if (x < 36 or y < 36) else (0, 0, 0, 0)
    # registration marks in the corners of the border
    cv2 = Cv(192, 192)
    for (x, y) in ((15, 15), (169, 15), (15, 169), (169, 169)):
        cv2.line([(x - 6, y), (x + 6, y)], rgba(INK, 0.55), 1.5)
        cv2.line([(x, y - 6), (x, y + 6)], rgba(INK, 0.55), 1.5)
        cv2.circle(x, y, 3.5, outline=rgba(INK, 0.55), width=1.2)
    im.alpha_composite(cv2.out())
    return im


@tex("T_UI_Memo", (192, 160), draw="9-slice", slice=[84, 46, 36, 34],
     note="upright, Chalk stock, a Manila head band with MEMO baked in its left corner, a dog ear bottom right, ink outline, hard shadow 6 px. The rectangle given is the memo's body; the shadow is outside it. Text starts 20 px in and 52 px down. Smallest size 124 x 84")
def t_memo():
    cv = Cv(192, 160)
    w, h = 186.0, 154.0
    ear = 22.0
    shape = [(0, 0), (w, 0), (w, h - ear), (w - ear, h), (0, h)]
    cv.poly([(x + 6, y + 6) for x, y in shape], fill=INK)
    cv.poly(shape, fill=CHALK)
    cv.rect(0, 0, w, 36, fill=MANILA)
    cv.rect(0, 36, w, 2.5, fill=INK)
    cv.rect(0, 41, w, 1.5, fill=rgba(RED, 0.8))
    cv.text(13, 11, "MEMO", "BUN", 15, INK, track=1.5)
    cv.poly([(w - ear, h), (w - ear, h - ear), (w, h - ear)], fill=PAPER_D, outline=INK, width=2.5)
    cv.poly(shape, outline=INK, width=3)
    im = cv.out()
    return im


@tex("T_UI_Dailies_Card", (192, 160), draw="9-slice", slice=[40, 44, 28, 28],
     note="Paper stock with a slate head band 30 px high (a clapper stripe in its left corner; a Chalk title fits in it), a red double rule, ink outline, hard shadow 6 px. Text starts 20 px in and 50 px down. Smallest size 72 x 76")
def t_dailies_card():
    cv = Cv(192, 160)
    w, h = 186.0, 154.0
    cv.rect(6, 6, w, h, fill=INK)
    cv.rect(0, 0, w, h, fill=PAPER)
    # the head: a slate band with a clapper stripe corner
    cv.rect(0, 0, w, 30, fill=SLATE)
    c = -30
    while c < 36:
        cv.poly([(c + 30, 0), (c + 42, 0), (c + 12, 30), (c, 30)], fill=CHALK)
        c += 24
    cv.rect(38, 0, w - 38, 30, fill=SLATE)
    cv.rect(36, 0, 3, 30, fill=INK)
    cv.rect(0, 30, w, 3, fill=INK)
    cv.rect(0, 37, w, 1.6, fill=rgba(RED, 0.8))
    cv.rect(0, 40.5, w, 1.6, fill=rgba(RED, 0.8))
    cv.rect(0, 0, w, h, outline=INK, width=3)
    return cv.out()


@tex("T_UI_Dailies_Pip", (96, 48), draw="atlas: 1:1, four in a row", cell=[48, 48], cols=2, cells=["lit", "unlit"])
def t_dailies_pip():
    cv = Cv(96, 48)
    for k in range(2):
        cx, cy = k * 48 + 23.0, 23.0
        cv.poly(star_pts(cx + 3, cy + 4, 21, 9.5), fill=INK)
        cv.poly(star_pts(cx, cy + 1, 21, 9.5), fill=INK)
        if k == 0:
            cv.poly(star_pts(cx, cy + 1, 16.5, 6.8), fill=AMBER)
            cv.poly([(cx, cy + 1 - 16.5)] + star_pts(cx, cy + 1, 16.5, 6.8)[1:2] + [(cx, cy + 1)] + star_pts(cx, cy + 1, 16.5, 6.8)[-1:], fill=(255, 226, 150))
        else:
            cv.poly(star_pts(cx, cy + 1, 16.5, 6.8), fill=mix(PAPER_D, STEEL, 0.25))
    return cv.out()


# ====================================================================== stickers (B)
CASH_YELLOW = (255, 224, 84)


@tex("T_UI_Sticker_Cash", (288, 144), draw="1:1", zones={"text": [44, 30, 196, 76]},
     note="yellow price burst; the label and the amount are drawn on it (zone x, y, w, h), in Ink")
def t_sticker_cash():
    cv = Cv(288, 144)
    cx, cy = 140.0, 68.0
    pts = burst_pts(cx, cy, 136, 120, 20, rot=4, jit=0.05, seed=3, squash=0.47)
    cv.poly([(x + 5, y + 6) for x, y in pts], fill=INK)
    cv.poly(pts, fill=CASH_YELLOW, outline=INK, width=3)
    cv.poly(burst_pts(cx, cy, 111, 101, 20, rot=4, jit=0.05, seed=3, squash=0.45), outline=rgba(INK, 0.85), width=1.4)
    # a cel highlight along the upper left
    hi = Cv(288, 144)
    hi.poly(burst_pts(cx, cy, 128, 113, 20, rot=4, jit=0.05, seed=3, squash=0.47), fill=(255, 244, 170))
    m = hi.im.getchannel("A")
    cut = Image.new("L", m.size, 0)
    ImageDraw.Draw(cut).polygon([(0, 0), (150 * cv.ss, 0), (60 * cv.ss, 60 * cv.ss), (0, 70 * cv.ss)], fill=255)
    ring = ImageChops.subtract(m, shift(m, 5 * cv.ss, 5 * cv.ss))
    cv.im.alpha_composite(solid(ImageChops.multiply(ring, cut), (255, 246, 190)))
    return cv.out()


@tex("T_UI_Sticker_SP", (128, 128), draw="1:1", zones={"text": [26, 28, 68, 64]},
     note="cyan skill point token; the number is drawn in its middle, in Ink")
def t_sticker_sp():
    cv = Cv(128, 128)
    c = 60.0
    cv.circle(c + 5, c + 6, 56, fill=INK)
    cv.circle(c, c, 56, fill=CYAN, outline=INK, width=3)
    for k in range(12):                                             # the chip's rim
        a0 = math.radians(k * 30 - 7)
        a1 = math.radians(k * 30 + 7)
        cv.poly([(c + 44 * math.cos(a0), c + 44 * math.sin(a0)), (c + 53 * math.cos(a0), c + 53 * math.sin(a0)),
                 (c + 53 * math.cos(a1), c + 53 * math.sin(a1)), (c + 44 * math.cos(a1), c + 44 * math.sin(a1))], fill=CHALK)
    cv.circle(c, c, 42, fill=mix(CYAN, WHITE, 0.30), outline=INK, width=2)
    cv.arc(c, c, 36, 200, 262, WHITE, 3.5)
    return cv.out()


@tex("T_UI_Sticker_Level", (256, 96), draw="1:1", zones={"number": [6, 6, 84, 78], "text": [102, 10, 140, 70]},
     note="driver level rating box: the number goes in the chalk cell (Ink), two lines of text in the slate cell (Chalk)")
def t_sticker_level():
    cv = Cv(256, 96)
    w, h = 250.0, 90.0
    cv.rect(6, 6, w, h, fill=INK)
    cv.rect(0, 0, w, h, fill=INK)
    cv.rect(3, 3, w - 6, h - 6, fill=CHALK)
    cv.rect(93, 6, w - 99, h - 12, fill=SLATE)
    cv.rect(90, 3, 3, h - 6, fill=INK)
    return cv.out()


# ====================================================================== hero art (the KESTREL cut-out and its burst)
def _kestrel_34(k):
    """KESTREL-88, oblique three-quarter, cel shaded. -> RGBA at k px per unit (1400 x 560 units)"""
    OX, OY, CW, CH = 240, 170, 1400, 560
    dx, dy = -140.0, -80.0
    im = Image.new("RGBA", (int(CW * k), int(CH * k)), (0, 0, 0, 0))
    d = ImageDraw.Draw(im)
    OL = rgba(INK)
    PINKD = (196, 44, 124)
    GOLD = AMBER

    def P(x, y, z=0.0):
        return ((x + z * dx + OX) * k, (y + z * dy + OY) * k)

    def poly(pts, fill, z=0.0, ol=3.0):
        q = [P(x, y, z) for x, y in pts]
        if fill is not None:
            d.polygon(q, fill=rgba(fill))
        if ol:
            d.line(q + [q[0], q[1]], fill=OL, width=max(1, int(ol * k)), joint="curve")

    def quad(p, q, z0, z1, fill, ol=3.0):
        pts = [P(p[0], p[1], z0), P(q[0], q[1], z0), P(q[0], q[1], z1), P(p[0], p[1], z1)]
        d.polygon(pts, fill=rgba(fill))
        if ol:
            d.line(pts + [pts[0], pts[1]], fill=OL, width=max(1, int(ol * k)), joint="curve")

    def circ(cx, cy, r, fill, z=0.0, ol=0.0):
        x, y = P(cx, cy, z)
        d.ellipse([x - r * k, y - r * k, x + r * k, y + r * k], fill=rgba(fill), outline=OL if ol else None,
                  width=max(1, int(ol * k)) if ol else 0)

    def bar(p, q, z, w, fill):
        d.line([P(p[0], p[1], z), P(q[0], q[1], z)], fill=OL, width=int((w + 6) * k))
        d.line([P(p[0], p[1], z), P(q[0], q[1], z)], fill=rgba(fill), width=int(w * k))

    a_, b_, c_, d_, e_, f_, g_, h_, i_, j_ = ((10, 262), (0, 238), (40, 222), (310, 172), (440, 104), (575, 104),
                                              (650, 150), (860, 150), (892, 166), (885, 262))
    body = [a_, b_, c_, d_, e_, f_, g_, h_, i_, j_]
    R_TOP, R_SIDE, R_SILL, R_FAR = (255, 92, 100), (238, 34, 58), (170, 16, 52), (128, 12, 46)
    NAVY, NAVY2, DECK = (30, 38, 100), (86, 110, 190), (44, 36, 84)
    TYRE = (24, 18, 44)
    # underglow: a flat pink streak on the ground (no blur)
    d.polygon([P(40, 296, 0.1), P(900, 296, 0.1), P(880, 296, 0.95), P(10, 296, 0.95)], fill=rgba(PINK))
    d.polygon([P(262, 268), P(650, 268), P(650, 277), P(262, 277)], fill=(255, 170, 214, 255))
    # far wheels and far side
    circ(190, 254, 60, TYRE, 0.95, 3)
    circ(735, 248, 68, TYRE, 0.95, 3)
    poly(body, R_FAR, 1.0)
    poly([(752, 20), (915, 36), (915, 76), (752, 58)], PINKD, 1.0)
    bar((815, 150), (822, 52), 0.8, 10, TYRE)
    # top faces
    quad(a_, b_, 0, 1, (196, 22, 54))
    quad(b_, c_, 0, 1, (255, 120, 120))
    quad(c_, d_, 0, 1, R_TOP)
    quad(d_, e_, 0, 1, NAVY)
    quad(e_, f_, 0, 1, (250, 60, 78))
    quad(g_, h_, 0, 1, DECK)
    quad(h_, i_, 0, 1, R_TOP)
    for p, q in ((b_, c_), (c_, d_), (e_, f_)):          # the cyan stripe over nose, hood and roof
        quad(p, q, 0.44, 0.58, CYAN, 0)
    quad((330, 161.5), (425, 112), 0.10, 0.34, NAVY2, 0)
    quad((345, 154), (425, 112), 0.42, 0.50, NAVY2, 0)
    for z in (0.26, 0.76):                                # hood guns
        bar((150, 196), (345, 156), z, 9, (150, 156, 180))
        bar((150, 196), (176, 190.5), z, 13, (70, 72, 100))
    quad((690, 92), (690, 150), 0.28, 0.72, (150, 150, 176))    # the blower
    quad((690, 92), (772, 92), 0.28, 0.72, GOLD)
    poly([(690, 92), (772, 92), (772, 150), (690, 150)], (228, 228, 240), 0.28)
    for yy in (106, 120, 134):
        d.line([P(698, yy, 0.28), P(764, yy, 0.28)], fill=(120, 120, 150, 255), width=max(1, int(4 * k)))
    for xx in (706, 730, 754):
        quad((xx - 9, 78), (xx + 9, 78), 0.40, 0.56, (255, 226, 120))
        poly([(xx - 9, 78), (xx + 9, 78), (xx + 9, 92), (xx - 9, 92)], GOLD, 0.40)
    d.line([P(596, 104, 0.5), P(582, 6, 0.5)], fill=OL, width=max(1, int(3.5 * k)))
    # near side
    poly(body, R_SIDE, 0, 0)
    poly([(10, 262), (4, 247), (120, 244), (885, 238), (885, 262)], R_SILL, 0, 0)
    poly([(44, 228), (310, 180), (430, 118), (446, 118), (322, 186), (48, 236)], (255, 96, 104), 0, 0)
    poly([(60, 236), (640, 204), (640, 214), (60, 244)], CYAN, 0, 0)
    poly(body, None, 0, 4)
    poly([(338, 172), (448, 115), (566, 115), (630, 152), (630, 160), (338, 178)], NAVY, 0, 3)
    poly([(362, 167), (448, 122), (486, 122), (404, 166)], NAVY2, 0, 0)
    d.line([P(472, 176), P(468, 236)], fill=OL, width=max(1, int(2.5 * k)))
    poly([(6, 242), (44, 228), (128, 213), (128, 223), (46, 239), (10, 252)], (255, 242, 190), 0, 2.5)
    poly([(862, 172), (889, 172), (887, 198), (862, 198)], (255, 170, 70), 0, 2.5)
    for cx, cy, r in ((190, 254, 60), (735, 248, 68)):   # near wheels
        x, y = P(cx, cy)
        rr = (r + 13) * k
        d.pieslice([x - rr, y - rr, x + rr, y + rr], 180, 360, fill=(60, 8, 34, 255))
        circ(cx, cy, r, TYRE, 0, 3.5)
        d.arc([x - (r - 7) * k, y - (r - 7) * k, x + (r - 7) * k, y + (r - 7) * k], 200, 300, fill=(74, 64, 110, 255), width=max(1, int(4 * k)))
        circ(cx, cy, r * 0.60, GOLD, 0, 3)
        circ(cx, cy, r * 0.40, (255, 226, 120))
        for n in range(6):
            a = n * math.pi / 3 + 0.3
            circ(cx + math.cos(a) * r * 0.38, cy + math.sin(a) * r * 0.38, r * 0.085, NAVY)
        circ(cx, cy, r * 0.13, NAVY)
    poly([(520, 204), (612, 204), (612, 212), (520, 212)], (84, 88, 120), 0, 2.5)        # side gun pod
    poly([(540, 215), (612, 215), (612, 223), (540, 223)], (84, 88, 120), 0, 2.5)
    poly([(608, 196), (852, 196), (846, 228), (608, 228)], (150, 156, 182), 0, 3)
    poly([(620, 201), (840, 201), (838, 208), (620, 208)], (206, 210, 228), 0, 0)
    bar((815, 150), (822, 52), 0.2, 10, TYRE)             # wing, near
    quad((765, 34), (905, 46), 0, 1, (34, 28, 66))
    quad((885, 44.3), (905, 46), 0.03, 0.97, PINK, 0)
    poly([(752, 20), (915, 36), (915, 76), (752, 58)], (255, 96, 178), 0, 3)
    poly([(766, 28), (902, 41), (902, 48), (766, 36)], (255, 170, 214), 0, 0)
    poly([(888, 226), (1010, 210), (962, 230), (1090, 240), (966, 250), (1030, 268), (888, 260)], PINK, 0, 0)   # exhaust flame
    poly([(888, 232), (968, 228), (936, 238), (1010, 243), (934, 249), (888, 254)], CYAN, 0, 0)
    poly([(888, 237), (930, 237), (916, 242), (940, 244), (888, 249)], WHITE, 0, 0)
    return im


KESTREL_HERO = (768, 304)


@tex("T_UI_Kestrel_Hero", KESTREL_HERO, draw="1:1 (may scale 0.75 to 1.1)",
     note="the KESTREL-88 as a die-cut sticker: chalk border, ink line, hard shadow; nose to the left, leaping. Lay it over the sheet's edge")
def t_kestrel_hero():
    ss = SS
    car = _kestrel_34(0.58 * ss)
    car = car.crop(car.getbbox())
    car = car.rotate(-7, resample=Image.BICUBIC, expand=True)
    art, pad = inked(car, 3.0 * ss, INK, shadow=(7 * ss, 8 * ss), border=7 * ss, bcol=CHALK)
    art = art.crop(art.getbbox())
    out = Image.new("RGBA", (KESTREL_HERO[0] * ss, KESTREL_HERO[1] * ss), (0, 0, 0, 0))
    k = min(1.0, ((KESTREL_HERO[0] - 6.0) * ss) / art.width, ((KESTREL_HERO[1] - 6.0) * ss) / art.height)
    if k < 1.0:
        art = art.resize((int(art.width * k), int(art.height * k)), Image.LANCZOS)
    comp(out, art, (out.width - art.width) / 2.0, (out.height - art.height) / 2.0)
    return out.reduce(ss)


@tex("T_UI_Hero_Burst", (352, 352), draw="1:1 (may scale 0.75 to 1.25), behind the KESTREL sticker",
     note="the explosion as a die-cut sticker, in daylight colours")
def t_hero_burst():
    ss = SS
    cv = Cv(360, 360)
    cx, cy, r = 172.0, 196.0, 112.0
    rnd = random.Random(5)
    smoke, smoke2 = (150, 132, 198), (192, 176, 228)
    for (ox, oy, rr) in ((0.55, -0.60, 0.42), (0.86, -0.28, 0.34), (0.30, -0.88, 0.30), (-0.25, -0.86, 0.28), (0.98, 0.10, 0.24), (0.66, -0.94, 0.22)):
        cv.circle(cx + ox * r, cy + oy * r, rr * r, fill=smoke, outline=INK, width=3.5)
    for (ox, oy, rr) in ((0.50, -0.66, 0.30), (0.84, -0.34, 0.22), (0.26, -0.92, 0.2)):
        cv.circle(cx + ox * r - 8, cy + oy * r - 8, rr * r, fill=smoke2)
    cv.poly(burst_pts(cx, cy, r, r * 0.60, 13, rot=17, jit=0.22, seed=5, squash=0.94), fill=(255, 70, 60), outline=INK, width=4.5)
    cv.poly(burst_pts(cx, cy, r * 0.80, r * 0.47, 12, rot=29, jit=0.2, seed=6, squash=0.94), fill=(255, 150, 44))
    cv.poly(burst_pts(cx, cy, r * 0.58, r * 0.33, 11, rot=6, jit=0.2, seed=7, squash=0.94), fill=(255, 222, 84))
    cv.poly(burst_pts(cx, cy, r * 0.34, r * 0.20, 9, rot=40, jit=0.2, seed=8, squash=0.94), fill=(255, 250, 224))
    for n in range(13):
        a = rnd.uniform(0, 2 * math.pi)
        rr = r * rnd.uniform(1.0, 1.36)
        x, y = cx + math.cos(a) * rr, cy + math.sin(a) * rr * 0.9
        sz = rnd.uniform(5, 13)
        b = rnd.uniform(0, 3)
        pts = [(x + math.cos(b + q * math.pi / 2) * sz, y + math.sin(b + q * math.pi / 2) * sz) for q in range(4)]
        cv.poly(pts, fill=rnd.choice([(255, 150, 44), INK, (255, 222, 84), INK]), outline=INK, width=2)
    art = cv.im.crop(cv.im.getbbox())
    art, pad = inked(art, 2.5 * ss, INK, shadow=(6 * ss, 7 * ss), border=6 * ss, bcol=CHALK)
    art = art.crop(art.getbbox())
    out = Image.new("RGBA", (352 * ss, 352 * ss), (0, 0, 0, 0))
    k = min(1.0, (348.0 * ss) / art.width, (348.0 * ss) / art.height)
    if k < 1.0:
        art = art.resize((int(art.width * k), int(art.height * k)), Image.LANCZOS)
    comp(out, art, (out.width - art.width) / 2.0, (out.height - art.height) / 2.0)
    return out.reduce(ss)


# ====================================================================== the chase strip (E): side-view sprites and a skyline tile
V1, V2, V3 = (58, 46, 108), (34, 26, 66), (92, 78, 150)        # the Syndicate's purples
KRED = (240, 40, 60)


class _Spr:
    """a small side-view sprite drawn in 'units' at k px per unit, every polygon inked"""

    def __init__(s, wu, hu, k):
        s.k = k
        s.cv = Cv(wu * k + 4, hu * k + 4)

    def pl(s, pts, fill, ow=3.0, ol=INK):
        k = s.k
        s.cv.poly([(2 + x * k, 2 + y * k) for x, y in pts], fill=fill, outline=ol if ow else None, width=max(1.4, ow * k))

    def ci(s, cx, cy, r, fill, ow=3.0):
        k = s.k
        s.cv.circle(2 + cx * k, 2 + cy * k, r * k, fill=fill, outline=INK if ow else None, width=max(1.4, ow * k))

    def ln(s, pts, w, c=INK):
        k = s.k
        s.cv.line([(2 + x * k, 2 + y * k) for x, y in pts], c, max(1.2, w * k))


def _chase_kestrel(k):
    """side profile, nose to the right. 580 x 220 units; wheels touch y = 220"""
    s = _Spr(580, 222, k)
    D1, D2, GRY, NAVY = (34, 26, 66), (58, 48, 104), (150, 156, 190), (36, 44, 110)
    s.pl([(34, 208), (556, 208), (548, 217), (42, 217)], PINK, 0)
    s.pl([(60, 60), (74, 60), (96, 116), (80, 116)], D1)
    s.pl([(100, 64), (112, 64), (126, 112), (112, 112)], D1)
    s.pl([(8, 30), (128, 44), (128, 62), (8, 52)], D1)
    s.pl([(8, 30), (128, 44), (128, 50), (8, 37)], PINK, 0)
    s.pl([(2, 16), (26, 16), (38, 78), (14, 78)], D2)
    s.pl([(6, 20), (24, 20), (26, 30), (8, 30)], PINK, 0)
    s.ln([(178, 26), (188, 98)], 3.5)
    s.pl([(152, 84), (224, 84), (224, 106), (152, 106)], GRY)
    for sx in (160, 178, 196):
        s.pl([(sx, 62), (sx + 13, 62), (sx + 13, 84), (sx, 84)], CHALK, 2.5)
        s.pl([(sx, 76), (sx + 13, 76), (sx + 13, 84), (sx, 84)], AMBER, 0)
    body = [(30, 180), (24, 122), (62, 106), (226, 100), (276, 68), (350, 66), (412, 102), (530, 144), (560, 160), (560, 180)]
    s.pl(body, KRED)
    s.pl([(30, 180), (27, 152), (560, 166), (560, 180)], mix(KRED, INK, 0.28), 0)
    s.pl([(62, 108), (226, 102), (270, 74), (276, 70), (232, 106), (64, 112)], mix(KRED, WHITE, 0.35), 0)
    s.pl(body, None)
    s.pl([(520, 168), (572, 168), (572, 182), (516, 182)], INK, 0)
    s.pl([(244, 100), (284, 76), (346, 74), (398, 102)], NAVY)
    s.pl([(274, 86), (288, 78), (318, 77), (300, 99), (262, 100)], mix(NAVY, WHITE, 0.30), 0)
    s.pl([(70, 130), (520, 150), (520, 157), (70, 138)], CYAN, 0)
    s.pl([(236, 112), (372, 118), (372, 128), (236, 124)], GRY, 2.5)
    s.pl([(372, 119), (398, 121), (398, 126), (372, 127)], D1, 2)
    s.pl([(538, 150), (558, 160), (558, 166), (534, 160)], AMBER, 2)
    s.pl([(26, 126), (40, 122), (42, 140), (28, 142)], PINK, 2)
    for (cx, cy, r) in ((122, 170, 42), (446, 174, 35)):
        s.ci(cx, cy, r + 8, INK, 0)
        s.ci(cx, cy, r, D1)
        s.ci(cx, cy, r * 0.64, AMBER)
        s.ci(cx, cy, r * 0.20, INK, 0)
        for i in range(5):
            a = math.radians(i * 72 + 18)
            s.ci(cx + r * 0.42 * math.cos(a), cy + r * 0.42 * math.sin(a), r * 0.085, INK, 0)
    return s.cv, 220


def _chase_truck(k):
    """the Juggernaut. 440 x 190 units, facing right; wheels touch y = 186"""
    s = _Spr(440, 190, k)
    s.pl([(262, 22), (274, 22), (274, 70), (262, 70)], V2)
    s.pl([(8, 34), (252, 34), (252, 152), (8, 152)], V1)
    s.pl([(8, 34), (252, 34), (252, 50), (8, 50)], V3, 0)
    s.pl([(8, 120), (252, 120), (252, 152), (8, 152)], V2, 0)
    for x in (48, 92, 136, 180, 224):
        s.ln([(x, 52), (x, 118)], 2.5)
    s.pl([(8, 78), (252, 78), (252, 96), (8, 96)], RED, 0)
    s.pl([(118, 70), (136, 87), (118, 104), (100, 87)], INK, 0)
    s.pl([(118, 76), (130, 87), (118, 98), (106, 87)], RED, 0)
    s.pl([(8, 34), (252, 34), (252, 152), (8, 152)], None)
    cab = [(258, 64), (336, 64), (380, 104), (408, 110), (408, 152), (258, 152)]
    s.pl(cab, V3)
    s.pl([(302, 74), (332, 74), (364, 102), (302, 102)], RED, 2.5)
    s.pl([(258, 128), (408, 128), (408, 152), (258, 152)], V2, 0)
    s.pl(cab, None)
    s.pl([(408, 106), (428, 112), (428, 160), (408, 160)], V2)
    s.pl([(396, 116), (408, 116), (408, 126), (396, 126)], AMBER, 2)
    for cx in (52, 110, 212, 340):
        s.ci(cx, 156, 30, V2)
        s.ci(cx, 156, 12, V3, 2.5)
    return s.cv, 186


def _chase_sedan(k):
    """Syndicate sedan with a roof gun. 310 x 120 units, facing right; wheels touch y = 119"""
    s = _Spr(310, 122, k)
    s.pl([(136, 12), (168, 12), (168, 30), (136, 30)], V2)
    s.pl([(168, 16), (214, 18), (214, 24), (168, 26)], V2, 2.5)
    body = [(8, 96), (8, 62), (72, 56), (108, 28), (196, 28), (236, 58), (296, 68), (302, 96)]
    s.pl(body, V1)
    s.pl([(8, 96), (8, 80), (302, 84), (302, 96)], V2, 0)
    s.pl([(114, 34), (192, 34), (222, 58), (100, 58)], V3, 2.5)
    s.pl([(8, 68), (300, 74), (300, 80), (8, 74)], RED, 0)
    s.pl(body, None)
    s.pl([(284, 70), (300, 72), (300, 80), (284, 80)], AMBER, 2)
    for cx in (64, 242):
        s.ci(cx, 96, 23, V2)
        s.ci(cx, 96, 9, V3, 2)
    return s.cv, 119


def _chase_gunship(k):
    """Syndicate gunship. 330 x 150 units, facing right; it flies: its skids are at y = 128"""
    s = _Spr(330, 150, k)
    s.cv.ellipse(2 + 168 * k, 2 + 15 * k, 152 * k, 7 * k, fill=rgba(CHALK, 0.55), outline=INK, width=max(1.4, 2 * k))
    s.pl([(160, 14), (178, 14), (182, 38), (156, 38)], V2)
    s.pl([(4, 24), (22, 24), (34, 62), (10, 62)], V3)
    s.pl([(104, 58), (10, 50), (10, 62), (108, 86)], V1)
    hull = [(112, 36), (218, 36), (270, 62), (280, 88), (248, 106), (124, 106), (98, 82)]
    s.pl(hull, V1)
    s.pl([(124, 106), (248, 106), (280, 88), (268, 84), (240, 96), (120, 96)], V2, 0)
    s.pl([(212, 44), (264, 66), (270, 86), (220, 86)], RED, 2.5)
    s.pl([(104, 68), (200, 70), (200, 78), (104, 77)], RED, 0)
    s.pl(hull, None)
    s.ln([(126, 126), (260, 126)], 5)
    s.ln([(150, 106), (144, 126)], 4)
    s.ln([(226, 106), (232, 126)], 4)
    s.pl([(226, 104), (286, 108), (286, 116), (226, 114)], V2, 2.5)
    return s.cv, 128


CHASE_CELLS = ["KESTREL", "truck", "sedan", "gunship"]
CHASE_GROUND = 120          # the row of a cell the wheels stand on


@tex("T_UI_Chase", (640, 256), draw="atlas: cell, 1:1", cell=[320, 128], cols=2, cells=list(CHASE_CELLS), ground=CHASE_GROUND,
     note="side-view sprites, nose to the right, centred in their cells; wheels stand on row 120 of the cell (the gunship flies: its skids are on row 96)")
def t_chase():
    out = Image.new("RGBA", (640, 256), (0, 0, 0, 0))
    for i, (fn, k, lift) in enumerate(((_chase_kestrel, 0.50, 0), (_chase_truck, 0.60, 0), (_chase_sedan, 0.72, 0), (_chase_gunship, 0.70, 24))):
        cv, ground = fn(k)
        im = cv.out()
        x = (i % 2) * 320 + (320 - im.width) // 2
        y = (i // 2) * 128 + CHASE_GROUND - lift - int(round(2 + ground * k))
        comp(out, im, x, y)
    return out


SKY_COLS = [(255, 143, 184), (127, 227, 210), (255, 226, 176), (200, 182, 255), (242, 240, 255), (255, 124, 142), (154, 216, 255), (255, 192, 138)]


@tex("T_UI_Chase_Skyline", (512, 128), draw="tile x (never stretched), its foot on the chase strip's rule", wrap="x",
     note="pastel city, palms and clouds with a transparent sky; tiles along x")
def t_chase_skyline():
    W, H = 512, 128
    cv = Cv(W, H)
    rnd = random.Random(88)

    def wrapd(fn):
        for ox in (-W, 0, W):
            fn(ox)

    def cloud(x, y, kk):
        puffs = ((0, -5, 12), (15, -11, 14), (32, -5, 11), (45, -1, 7), (-12, -1, 7))

        def f(ox):
            for (dx, dy, r) in puffs:
                cv.circle(ox + x + dx * kk, y + dy * kk, (r + 2.0) * kk, fill=INK)
            cv.rect(ox + x - 21 * kk, y - 3 * kk, 75 * kk, 11 * kk, fill=INK, r=5 * kk)
            for (dx, dy, r) in puffs:
                cv.circle(ox + x + dx * kk, y + dy * kk, r * kk, fill=CHALK)
            cv.rect(ox + x - 19 * kk, y - 3 * kk, 71 * kk, 9 * kk, fill=CHALK, r=4 * kk)
            cv.rect(ox + x - 10 * kk, y + 2 * kk, 50 * kk, 3 * kk, fill=(232, 214, 236), r=1.5 * kk)
        wrapd(f)
    # buildings, back row then front row
    x = 0.0
    j = 0
    rows = []
    while x < W - 20:
        bw = rnd.choice((34, 40, 46, 52, 30))
        if x + bw > W - 4:
            bw = W - x
        bh = rnd.choice((54, 66, 78, 88, 96, 60, 72))
        rows.append((x, bw, bh, SKY_COLS[j % len(SKY_COLS)]))
        x += bw + rnd.choice((0, 0, 6, 18))
        j += 3

    def building(x0, bw, bh, c):
        def f(ox):
            y0 = H - bh
            cv.rect(ox + x0, y0, bw, bh + 2, fill=c)
            sw = bw * 0.24
            cv.rect(ox + x0 + bw - sw, y0, sw, bh + 2, fill=mix(c, (70, 50, 150), 0.32))
            cv.rect(ox + x0, y0, bw, 4, fill=mix(c, WHITE, 0.5))
            r2 = random.Random(int(x0 * 7 + bh))
            wy = y0 + 10
            cols = int((bw - sw - 5) // 9)
            if cols > 0:
                mrg = (bw - sw - cols * 5 - (cols - 1) * 4) / 2.0
                while wy + 5 < H - 6:
                    for c_ in range(cols):
                        lit = r2.random() < 0.22
                        cv.rect(ox + x0 + mrg + c_ * 9, wy, 5, 4.5, fill=(255, 243, 184) if lit else mix(c, (56, 40, 130), 0.55))
                    wy += 10
            cv.rect(ox + x0, y0, bw, bh + 4, outline=INK, width=2.2)
        wrapd(f)
    for (x0, bw, bh, c) in rows[::2]:
        building(x0, bw, bh, c)
    for (x0, bw, bh, c) in rows[1::2]:
        building(x0, bw, bh * 0.72, c)

    def palm(x0):
        def f(ox):
            px, py = ox + x0, H
            cv.line([(px, py), (px - 2, py - 20), (px + 1, py - 40)], INK, 6.5)
            cv.line([(px, py), (px - 2, py - 20), (px + 1, py - 40)], (46, 150, 148), 3)
            for ddx, ddy in ((-24, -4), (-18, -18), (-1, -24), (17, -18), (24, -4), (13, 6), (-13, 6)):
                tx, ty = px + 1, py - 40
                cv.poly([(tx, ty), (tx + ddx * 0.5 - ddy * 0.2, ty + ddy * 0.5 + ddx * 0.2), (tx + ddx, ty + ddy),
                         (tx + ddx * 0.5 + ddy * 0.2, ty + ddy * 0.5 - ddx * 0.2)], fill=(28, 150, 132), outline=INK, width=1.6)
        wrapd(f)
    for x0 in (58, 190, 300, 436):
        palm(x0 + rnd.uniform(-10, 10))
    cloud(96, 22, 1.0)
    cloud(352, 30, 0.8)
    return cv.out()


# ====================================================================== the HUD: ink stickers (C)
HUD_LEAN = 0.12             # the stickers' lean: x per y
HUD_SHADOW = 0.62           # opacity of the HUD's hard shadow (it lies on the road)
HUD_DOT = mix(SLATE, (213, 188, 255), 0.24)


def para_in(x, y, w, h, lean, t):
    """the parallelogram para(x, y, w, h, lean) inset by t on every side (same slope)"""
    q = lean * t / float(h)
    return para(x + t + q, y + t, w - 2 * t - 2 * q, h - 2 * t, lean * (h - 2 * t) / float(h))


def _hud_sticker(W, H, R, sh=5.0, edge=2.0, key=3.0):
    """ink body, white keyline (the game tints it: Chalk), ink edge, hard shadow, halftone in the right end"""
    cv = Cv(W, H)
    bw, bh = W - sh, H - sh
    lean = HUD_LEAN * bh
    cv.poly(para(sh, sh, bw, bh, lean), fill=rgba(INK, HUD_SHADOW))
    cv.poly(para(0, 0, bw, bh, lean), fill=INK)
    cv.poly(para_in(0, 0, bw, bh, lean, edge), fill=WHITE)
    body = para_in(0, 0, bw, bh, lean, edge + key)
    cv.poly(body, fill=SLATE)
    msk = Image.new("L", cv.im.size, 0)
    ImageDraw.Draw(msk).polygon([(x * cv.ss, y * cv.ss) for x, y in body], fill=255)
    x0 = W - R + 6.0

    def rad(x, y):
        t = (x - x0) / float(W - x0)
        return 4.0 * min(1.0, t * 1.15) if t > 0.04 else 0.0
    dots = ImageChops.multiply(halftone_mask((W, H), 10.0, rad, origin=(W - 12.0, bh / 2.0)), msk)
    cv.im.alpha_composite(solid(dots, HUD_DOT))
    return cv.out()


@tex("T_UI_Hud_Plate", (160, 256), kind="mask", draw="3-slice-h, for plates 200 to 300 px high", slice=[48, 0, 96, 0],
     tint=["Chalk", "Red", "Pink", "Cyan", "Amber"],
     note="the tint is the KEYLINE's colour (Chalk normally, Red for armour low); the Slate body and ink stay dark. The whole texture, shadow included, fits inside the layout rectangle")
def t_hud_plate():
    return _hud_sticker(160, 256, 96)


@tex("T_UI_Hud_Bar", (160, 96), kind="mask", draw="3-slice-h, for plates 64 to 130 px high", slice=[32, 0, 64, 0],
     tint=["Chalk", "Red", "Pink", "Cyan", "Amber"], note="the same sticker, low: scene slate, boss plate, caption plates. Tint = keyline")
def t_hud_bar():
    return _hud_sticker(160, 96, 64)


@tex("T_UI_Hud_Chip", (96, 40), kind="mask", draw="3-slice-h, 26 to 48 px high", slice=[20, 0, 20, 0],
     tint=["Cyan", "Pink", "Amber", "Red", "Chalk", "Green", "Steel"], note="small slanted chip: white with an ink outline, no shadow. Plate tabs, TAKE 1, ammo boxes, the keys strip")
def t_hud_chip():
    cv = Cv(96, 40)
    lean = 0.25 * 40
    cv.poly(para(0, 0, 96, 40, lean), fill=INK)
    cv.poly(para_in(0, 0, 96, 40, lean, 2.5), fill=WHITE)
    return cv.out()


@tex("T_UI_Hud_Tag", (112, 44), kind="mask", draw="3-slice-h, 30 to 48 px high", slice=[30, 0, 18, 0], tint=["Amber", "Cyan", "Pink", "Green"],
     note="pointed end with a punched hole on the left, ink outline, hard shadow 4 px (inside the texture). Text starts 30 px in")
def t_hud_tag():
    cv = Cv(112, 44)
    shape = [(16, 1.5), (106.5, 1.5), (106.5, 38.5), (16, 38.5), (1.5, 20)]
    cv.poly([(x + 4, y + 4) for x, y in shape], fill=rgba(INK, HUD_SHADOW))
    cv.poly(shape, fill=WHITE, outline=INK, width=2.6)
    cv.poly([(15, 31), (104, 31), (104, 36), (17, 36)], fill=(214, 214, 214))
    cv.circle(17, 20, 4.2, fill=INK)
    return cv.out()


HUD_PIP_CELLS = ["arrow pip full", "arrow pip empty", "reel pip full", "reel pip empty", "meter segment full", "meter segment flat",
                 "finale mark", "tick"]


@tex("T_UI_Hud_Pips", (256, 32), kind="mask", draw="atlas: cell; 4 and 5 stretch to a segment", cell=[32, 32], cols=8, cells=list(HUD_PIP_CELLS),
     tint=["Amber", "Steel", "Cyan", "Green", "Red", "Pink", "Chalk"],
     note="0-3 at 1:1 (pitch 30 px for arrows). 4 = slanted segment with a cel highlight (fill), 5 = the same shape flat (track): made for segments about twice as wide as high")
def t_hud_pips():
    cv = Cv(256, 32)
    ar = [(4, 0), (16, 0), (26, 9), (13, 18), (0, 18)]

    def at(pts, ox, oy, s=1.0):
        return [(ox + x * s, oy + y * s) for x, y in pts]
    # 0 arrow full
    cv.poly(at(ar, 3, 7), fill=INK, outline=INK, width=3.4)
    cv.poly(at(ar, 3, 7), fill=(226, 226, 226))
    cv.poly(at([(4, 0), (16, 0), (19.5, 4), (3, 4)], 3, 7), fill=WHITE)
    # 1 arrow empty
    cv.poly(at(ar, 35, 7), fill=INK, outline=INK, width=3.4)
    cv.poly(at(ar, 35, 7), fill=WHITE)
    cv.poly(at([(6.5, 3), (15, 3), (21.5, 9), (12, 15), (4, 15)], 35, 7), fill=INK)
    # 2 reel full, 3 reel empty
    m = icon_mask("reel", 24).resize((24 * cv.ss, 24 * cv.ss), Image.LANCZOS)
    cv.circle(64 + 16, 16, 14, fill=INK)
    cv.mask_paste(m, 64 + 4, 4, WHITE)
    cv.circle(96 + 16, 16, 14, fill=INK)
    cv.circle(96 + 16, 16, 12, outline=WHITE, width=3)
    cv.circle(96 + 16, 16, 3, fill=WHITE)
    # 4 segment full, 5 segment flat
    cv.poly(para(128 + 1, 0, 30, 32, 6), fill=(224, 224, 224))
    cv.poly(para(128 + 1 + 6 * 0.7, 0, 30 - 6 * 0.7, 32 * 0.3, 6 * 0.3), fill=WHITE)
    cv.poly(para(160 + 1, 0, 30, 32, 6), fill=WHITE)
    # 6 finale mark (a flag on a staff), 7 tick
    cv.rect(192 + 13, 2, 6, 28, fill=INK)
    cv.poly([(192 + 16, 2), (192 + 31, 9), (192 + 16, 17)], fill=INK, outline=INK, width=3)
    cv.rect(192 + 14.5, 3.5, 3, 25, fill=WHITE)
    cv.poly([(192 + 17, 4), (192 + 28, 9), (192 + 17, 15)], fill=WHITE)
    cv.rect(224 + 12, 0, 8, 32, fill=INK)
    cv.rect(224 + 14, 0, 4, 32, fill=WHITE)
    return cv.out()


@tex("T_UI_Hud_Bracket", (64, 64), kind="mask", draw="1:1 at the TRUE screen corners, mirrored for the other three", tint=["Chalk", "Pink", "Red"],
     note="the top left one: 5 px arms of 48 px with an ink edge, 6 px in from the texture's corner")
def t_hud_bracket():
    cv = Cv(64, 64)
    cv.rect(4.5, 4.5, 51, 8, fill=INK)
    cv.rect(4.5, 4.5, 8, 51, fill=INK)
    cv.rect(6, 6, 48, 5, fill=WHITE)
    cv.rect(6, 6, 5, 48, fill=WHITE)
    return cv.out()


COMBO_C = 76.0


def _combo_pts(grow=0.0, ro=58.0, ri=41.0):
    return burst_pts(COMBO_C, COMBO_C, ro + grow * 1.8, ri + grow, 11, rot=8, jit=0.18, seed=2)


@tex("T_UI_Hud_Combo_Fill", (160, 160), kind="mask", draw="1:1 (scale 0.6 to 1), scale pop about its centre", pivot=[76, 76], tint=["Pink", "Amber"],
     note="the combo starburst's fill; the multiplier is drawn on it with the digit atlas, centred on the pivot")
def t_hud_combo_fill():
    cv = Cv(160, 160)
    cv.poly(_combo_pts(), fill=WHITE)
    return white_mask(cv.out())


@tex("T_UI_Hud_Combo_Ink", (160, 160), draw="under the fill, same rectangle", pivot=[76, 76],
     note="the starburst's ink outline and hard shadow")
def t_hud_combo_ink():
    cv = Cv(160, 160)
    cv.poly([(x + 5, y + 6) for x, y in _combo_pts(4.0)], fill=rgba(INK, HUD_SHADOW))
    cv.poly(_combo_pts(4.0), fill=INK)
    return cv.out()


@tex("T_UI_Hud_ArmorLow", (288, 160), draw="1:1, pinned to the driver plate's top right corner, popped in by scale about its centre", pivot=[144, 80],
     note="the baked shout ARMOR LOW! on a yellow burst (Bungee, leaning 7 degrees)")
def t_hud_armor_low():
    ss = SS
    cap = 27.0
    f = fontpx("BUN", cap / CAPR["BUN"] * ss)
    s = "ARMOR LOW!"
    tw_ = f.getlength(s)
    R = tw_ / 2.0 + 24 * ss
    Wd = int(R * 2 + 44 * ss)
    lay = Image.new("RGBA", (Wd, Wd), (0, 0, 0, 0))
    d = ImageDraw.Draw(lay)
    pts = burst_pts(Wd / 2.0, Wd / 2.0, R, R * 0.76, 14, rot=0, jit=0.22, seed=5, squash=0.46)
    d.polygon(pts, fill=rgba(CASH_YELLOW))
    inner = burst_pts(Wd / 2.0, Wd / 2.0, R * 0.80, R * 0.66, 14, rot=6, jit=0.18, seed=6, squash=0.46)
    d.polygon(inner, fill=(255, 240, 150, 255))
    tl, ox, oy = _text_layer(s, f, WHITE)
    asc = f.getmetrics()[0]
    capoff = asc - cap * ss
    tx = Wd / 2.0 - tw_ / 2.0
    ty = Wd / 2.0 - cap * ss / 2.0 - capoff
    lay.alpha_composite(tl, (int(tx - ox + 2.5 * ss), int(ty + asc - oy + 2.5 * ss)))
    tl2, ox, oy = _text_layer(s, f, INK)
    lay.alpha_composite(tl2, (int(tx - ox), int(ty + asc - oy)))
    lay = lay.crop(lay.getbbox())
    art, pad = inked(lay, 3.6 * ss, INK, shadow=(5 * ss, 6 * ss))
    art = art.rotate(7, resample=Image.BICUBIC, expand=True)
    art = art.crop(art.getbbox())
    out = Image.new("RGBA", (288 * ss, 160 * ss), (0, 0, 0, 0))
    k = min(1.0, 284.0 * ss / art.width, 156.0 * ss / art.height)
    if k < 1.0:
        art = art.resize((int(art.width * k), int(art.height * k)), Image.LANCZOS)
    comp(out, art, (out.width - art.width) / 2.0, (out.height - art.height) / 2.0)
    return out.reduce(ss)


@tex("T_UI_Hud_Balloon", (96, 64), draw="9-slice", slice=[32, 38, 26, 22],
     note="Chalk, 4 px ink outline, hard shadow 6 px (inside the texture), tail on the left near the top. The balloon's body starts 16 px in (the tail); text starts 34 px in. Smallest height 60 px")
def t_hud_balloon():
    cv = Cv(96, 64)
    bx, w, h = 16.0, 74.0, 58.0
    cv.rect(bx + 5, 6, w, h, fill=rgba(INK, HUD_SHADOW), r=13)
    cv.poly([(bx + 6, 13), (bx - 16, 24), (bx + 6, 35)], fill=INK)
    cv.rect(bx, 0, w, h, fill=CHALK, outline=INK, width=4, r=13)
    cv.poly([(bx + 6, 17.5), (bx - 8, 24), (bx + 6, 30.5)], fill=CHALK)
    return cv.out()


@tex("T_UI_Hud_Megaphone", (72, 72), draw="1:1, left of the balloon's tail", note="amber disc with the megaphone, ink outline, hard shadow")
def t_hud_megaphone():
    cv = Cv(72, 72)
    cv.circle(37, 38, 31, fill=rgba(INK, HUD_SHADOW))
    cv.circle(33, 33, 31, fill=AMBER, outline=INK, width=3.5)
    m = icon_mask("megaphone", 36).resize((36 * cv.ss, 36 * cv.ss), Image.LANCZOS)
    cv.mask_paste(m, 14, 15, INK)
    return cv.out()


# ====================================================================== the digit atlas (big numerals are never live text)
DIGIT_CHARS = "0123456789$,./%x+-: "
DIGIT_SHEAR = 0.16
# gap = the room left between the inks of two neighbours (Bungee's own side bearings are dropped: the outline fills it)
_DIGIT_SPEC = {
    "L": dict(cap=64.0, cell=(88, 96), at=(0, 0), pen=(3.0, 77.0), ow=4.0, sh=(4.0, 5.0), gap=6.0),
    "S": dict(cap=36.0, cell=(52, 56), at=(0, 192), pen=(2.5, 44.5), ow=2.6, sh=(2.5, 3.0), gap=3.5),
}
DIGIT_ATLAS = (880, 304)


def _digit_glyph(ch):
    """-> (the character really drawn, x squeeze, size scale, lift as a fraction of the cap)"""
    if ch == "x":
        return "×", 1.0, 0.80, 0.10
    if ch == "%":
        return "%", 0.72, 1.0, 0.0
    return ch, 1.0, 1.0, 0.0


def _ink_box(g, f):
    """(left bearing, ink width) of a glyph in font px"""
    im = Image.new("L", (int(f.size * 2), int(f.size * 2)), 0)
    ImageDraw.Draw(im).text((f.size * 0.4, f.size * 1.4), g, font=f, fill=255, anchor="ls")
    b = im.getbbox()
    return (b[0] - f.size * 0.4, b[2] - b[0]) if b else (0.0, 0.0)


def _digit_metrics():
    """advances and offsets of every glyph. Digits are tabular: each takes the widest digit's ink plus the gap and
    sits centred in it; the other glyphs take their own ink plus the gap."""
    out = {"chars": DIGIT_CHARS, "cols": 10, "shear": DIGIT_SHEAR, "sizes": ["L", "S"]}
    for key, sp in _DIGIT_SPEC.items():
        em = sp["cap"] / CAPR["BUN"]
        Q = 8.0
        f = fontpx("BUN", em * Q)
        boxes = {}
        for ch in DIGIT_CHARS:
            g, xs, sc, lift = _digit_glyph(ch)
            lsb, iw = _ink_box(g, f) if ch != " " else (0.0, 0.0)
            boxes[ch] = (lsb / Q * xs * sc, iw / Q * xs * sc)
        dig = max(boxes[c][1] for c in "0123456789")
        adv, off = [], []
        for ch in DIGIT_CHARS:
            lsb, iw = boxes[ch]
            if ch == " ":
                adv.append(round((dig + sp["gap"]) * 0.32, 1))
                off.append(0.0)
                continue
            a = (dig if ch.isdigit() else iw) + sp["gap"]
            adv.append(round(a, 1))
            off.append(round((a - iw) / 2.0 - lsb, 2))            # where the glyph's own origin goes, from the pen
        out[key] = {"cap": sp["cap"], "cell": list(sp["cell"]), "at": list(sp["at"]), "pen": list(sp["pen"]), "advance": adv,
                    "outline": sp["ow"], "shadow": list(sp["sh"]), "_off": off}
    return out


DIGITS = _digit_metrics()
_digit_cache = {}


def _digit_layers():
    """-> (fill, ink) RGBA atlases at 1x"""
    if "a" in _digit_cache:
        return _digit_cache["a"]
    ss = SS
    W, H = DIGIT_ATLAS
    fill = Image.new("RGBA", (W * ss, H * ss), (255, 255, 255, 0))
    ink = Image.new("RGBA", (W * ss, H * ss), INK + (0,))
    for key, sp in _DIGIT_SPEC.items():
        cw, ch_ = sp["cell"]
        for n, ch in enumerate(DIGIT_CHARS):
            if ch == " ":
                continue
            g, xs, sc, lift = _digit_glyph(ch)
            f = fontpx("BUN", sp["cap"] / CAPR["BUN"] * ss * sc)
            lay, ox, oy = _text_layer(g, f, WHITE)
            if xs != 1.0:
                lay = lay.resize((max(1, int(round(lay.width * xs))), lay.height), Image.LANCZOS)
                ox *= xs
            hh = lay.height
            lay, ex = shear_layer(lay, DIGIT_SHEAR)
            ox2 = ox + DIGIT_SHEAR * (hh - oy)                  # where the pen origin went
            cx = sp["at"][0] + (n % 10) * cw
            cy = sp["at"][1] + (n // 10) * ch_
            px = (cx + sp["pen"][0] + DIGITS[key]["_off"][n]) * ss
            py = (cy + sp["pen"][1] - lift * sp["cap"]) * ss
            cell_f = Image.new("RGBA", (cw * ss, ch_ * ss), (255, 255, 255, 0))
            comp(cell_f, lay, px - cx * ss - ox2, py - cy * ss - oy)
            a = cell_f.getchannel("A")
            ao = dilate(a, sp["ow"] * ss)
            cell_i = Image.new("RGBA", cell_f.size, INK + (0,))
            cell_i.alpha_composite(shift(solid(ao, INK), sp["sh"][0] * ss, sp["sh"][1] * ss))
            cell_i.alpha_composite(solid(ao, INK))
            # nothing may touch the cell's edge (a 1 px gutter keeps neighbours out of the filter)
            edge = Image.new("L", cell_f.size, 0)
            ImageDraw.Draw(edge).rectangle([ss, ss, cw * ss - ss - 1, ch_ * ss - ss - 1], fill=255)
            cell_i.putalpha(ImageChops.multiply(cell_i.getchannel("A"), edge))
            fill.paste(cell_f, (cx * ss, cy * ss))
            ink.paste(cell_i, (cx * ss, cy * ss))
    _digit_cache["a"] = (white_mask(fill.reduce(ss)), ink.reduce(ss))
    return _digit_cache["a"]


_DIGIT_NOTE = ("glyph n is cell n (10 columns, 2 rows) of its size; L: cap 64 px, cells 88 x 96 from (0, 0); S: cap 36 px, cells 52 x 56 "
               "from (0, 192). Pen origin (baseline left) inside a cell and per-glyph advances are in DIGITS / kit_manifest.json")


@tex("T_UI_Digits_Fill", DIGIT_ATLAS, kind="mask", draw="atlas: cell, over the ink cell", cell=[88, 96], cols=10, cells=list(DIGIT_CHARS),
     tint=["Chalk", "Amber", "Cyan", "Red", "Green", "Pink", "Ink"], note="Bungee, sheared 0.16, FILL only. " + _DIGIT_NOTE)
def t_digits_fill():
    return _digit_layers()[0]


@tex("T_UI_Digits_Ink", DIGIT_ATLAS, draw="atlas: cell, under the fill, same rectangle", cell=[88, 96], cols=10, cells=list(DIGIT_CHARS),
     note="the same glyphs: ink outline and hard offset shadow. Draw the ink cells of a whole string first, then its fill cells. " + _DIGIT_NOTE)
def t_digits_ink():
    return _digit_layers()[1]


# ======================================================================================================================
# THE MOCK-UP MODULE: what the game's canvas can draw, on a 1920 x 1080 RGBA image, in 1080p px
# ======================================================================================================================
_tex_cache = {}
_tint_cache = {}
TEX_DIRS = [os.path.join(HERE, "tex"), KIT_OUT]


def new_screen(w=1920, h=1080, fill=None):
    """a blank screen (RGBA). fill: a colour or None for transparent"""
    return Image.new("RGBA", (w, h), rgba(fill) if fill is not None else (0, 0, 0, 0))


def load_tex(name):
    """a kit texture as an RGBA image at 1x: read from the generated folder next to this file (tex/) if it is there,
    otherwise generated now. Also takes an image (returned as it is)."""
    if not isinstance(name, str):
        return name
    if name not in _tex_cache:
        im = None
        for d in TEX_DIRS:
            p = os.path.join(d, name + ".png")
            if os.path.isfile(p):
                im = Image.open(p).convert("RGBA")
                if im.size != tuple(MAN[name]["size"]):
                    im = None                      # a stale folder: use the generator
                else:
                    break
        _tex_cache[name] = im if im is not None else build(name)
    return _tex_cache[name]


def cell_rect(name, i):
    """(x, y, w, h) of cell i of an atlas texture"""
    e = MAN[name]
    cw, ch = e["cell"]
    return ((i % e["cols"]) * cw, (i // e["cols"]) * ch, cw, ch)


def cell(name, i):
    """cell i of an atlas texture, as an image"""
    key = ("cell", name, i)
    if key not in _tex_cache:
        x, y, w, h = cell_rect(name, i)
        _tex_cache[key] = load_tex(name).crop((x, y, x + w, y + h))
    return _tex_cache[key]


def _prep(t, tint=None, alpha=1.0):
    im = load_tex(t)
    if tint is not None and col(tint) != (255, 255, 255):
        key = (id(im), col(tint))
        if key not in _tint_cache:
            _tint_cache[key] = (im, tint_img(im, tint))      # keep im alive: the cache is keyed by its id
        im = _tint_cache[key][1]
    if alpha < 1.0:
        im = fade(im, alpha)
    return im


def box(im, x, y, w, h, fill, alpha=1.0):
    """the game's Box: a flat rectangle"""
    if w <= 0 or h <= 0:
        return
    x0, y0, x1, y1 = int(round(x)), int(round(y)), int(round(x + w)), int(round(y + h))
    if x1 <= x0 or y1 <= y0:
        return
    c = rgba(fill)
    c = (c[0], c[1], c[2], int(round(c[3] * alpha)))
    comp(im, Image.new("RGBA", (x1 - x0, y1 - y0), c), x0, y0)


def frame(im, x, y, w, h, c, th=3.0, alpha=1.0):
    """the game's Frame: four boxes, an outline INSIDE the rectangle"""
    box(im, x, y, w, th, c, alpha)
    box(im, x, y + h - th, w, th, c, alpha)
    box(im, x, y + th, th, h - 2 * th, c, alpha)
    box(im, x + w - th, y + th, th, h - 2 * th, c, alpha)


def paste(im, t, x, y, tint=None, scale=1.0, angle=0.0, pivot=None, alpha=1.0, flip_x=False, flip_y=False):
    """one tile. Without a pivot (x, y) is the top left. With pivot=(px, py) in texture px, the pivot lands on (x, y)
    and angle (degrees, counter-clockwise) turns the tile about it. -> (w, h) drawn"""
    src = _prep(t, tint, alpha)
    if flip_x:
        src = src.transpose(Image.FLIP_LEFT_RIGHT)
    if flip_y:
        src = src.transpose(Image.FLIP_TOP_BOTTOM)
    if scale != 1.0:
        src = src.resize((max(1, int(round(src.width * scale))), max(1, int(round(src.height * scale)))),
                         Image.LANCZOS if scale < 1 else Image.BICUBIC)
    if pivot is None and not angle:
        comp(im, src, x, y)
        return src.size
    w, h = src.size
    px, py = (w / 2.0, h / 2.0) if pivot is None else (pivot[0] * scale, pivot[1] * scale)
    if pivot is None:
        x, y = x + w / 2.0, y + h / 2.0
    if angle:
        rot = src.rotate(angle, resample=Image.BICUBIC, expand=True)
        a = math.radians(angle)
        dx, dy = px - w / 2.0, py - h / 2.0
        nx = math.cos(a) * dx + math.sin(a) * dy
        ny = -math.sin(a) * dx + math.cos(a) * dy
        comp(im, rot, x - (rot.width / 2.0 + nx), y - (rot.height / 2.0 + ny))
    else:
        comp(im, src, x - px, y - py)
    return src.size


def stretch(im, t, x, y, w, h, tint=None, alpha=1.0, flip_x=False):
    """one tile scaled to the rectangle"""
    if w < 1 or h < 1:
        return
    src = _prep(t, tint, alpha)
    if flip_x:
        src = src.transpose(Image.FLIP_LEFT_RIGHT)
    comp(im, src.resize((int(round(w)), int(round(h))), Image.BILINEAR), x, y)


def tiled(im, t, x, y, w, h, tint=None, scale=1.0, alpha=1.0, ox=0.0, oy=0.0):
    """a texture repeated by UV over the rectangle; the pattern starts at the rectangle's top left (+ ox, oy)"""
    w, h = int(round(w)), int(round(h))
    if w < 1 or h < 1:
        return
    src = _prep(t, tint, alpha)
    if scale != 1.0:
        src = src.resize((max(1, int(round(src.width * scale))), max(1, int(round(src.height * scale)))), Image.LANCZOS)
    lay = Image.new("RGBA", (w, h), (0, 0, 0, 0))
    y0 = -int(round(oy)) % src.height - src.height
    while y0 < h:
        x0 = -int(round(ox)) % src.width - src.width
        while x0 < w:
            lay.paste(src, (x0, y0))
            x0 += src.width
        y0 += src.height
    comp(im, lay, x, y)


def nine_slice(im, t, x, y, w, h, margins=None, tint=None, cell_i=None, centre=True, alpha=1.0):
    """nine tiles: corners fixed, edges stretched along their length, centre stretched. margins = (L, T, R, B) in
    texture px (default: the manifest's); cell_i picks a cell of an atlas (T_UI_Keycap)."""
    name = t if isinstance(t, str) else None
    if margins is None:
        margins = MAN[name]["slice"]
    src = _prep(cell(name, cell_i) if cell_i is not None else t, tint, alpha)
    L, T, R, B = [int(v) for v in margins]
    w, h = int(round(w)), int(round(h))
    sw, sh = src.size
    if w < L + R:
        k = w / float(L + R)
        L2, R2 = int(L * k), w - int(L * k)
    else:
        L2, R2 = L, R
    if h < T + B:
        k = h / float(T + B)
        T2, B2 = int(T * k), h - int(T * k)
    else:
        T2, B2 = T, B
    xs = [(0, L, 0, L2), (L, sw - R, L2, w - R2), (sw - R, sw, w - R2, w)]
    ys = [(0, T, 0, T2), (T, sh - B, T2, h - B2), (sh - B, sh, h - B2, h)]
    lay = Image.new("RGBA", (w, h), (0, 0, 0, 0))
    for j, (sy0, sy1, dy0, dy1) in enumerate(ys):
        for i, (sx0, sx1, dx0, dx1) in enumerate(xs):
            if i == 1 and j == 1 and not centre:
                continue
            if dx1 <= dx0 or dy1 <= dy0 or sx1 <= sx0 or sy1 <= sy0:
                continue
            piece = src.crop((sx0, sy0, sx1, sy1))
            if piece.size != (dx1 - dx0, dy1 - dy0):
                piece = piece.resize((dx1 - dx0, dy1 - dy0), Image.BILINEAR)
            lay.paste(piece, (dx0, dy0))
    comp(im, lay, x, y)


def three_slice_h(im, t, x, y, w, h, L=None, R=None, tint=None, cell_i=None, alpha=1.0):
    """three tiles side by side: the whole texture is scaled UNIFORMLY to the height h, then the middle stretches"""
    name = t if isinstance(t, str) else None
    if L is None:
        L, R = MAN[name]["slice"][0], MAN[name]["slice"][2]
    src = _prep(cell(name, cell_i) if cell_i is not None else t, tint, alpha)
    sw, sh = src.size
    w, h = int(round(w)), int(round(h))
    k = h / float(sh)
    Ld, Rd = int(round(L * k)), int(round(R * k))
    if Ld + Rd > w:
        Ld = int(w * L / float(L + R))
        Rd = w - Ld
    lay = Image.new("RGBA", (w, h), (0, 0, 0, 0))
    lay.paste(src.crop((0, 0, L, sh)).resize((max(1, Ld), h), Image.BILINEAR), (0, 0))
    if w - Ld - Rd > 0:
        lay.paste(src.crop((L, 0, sw - R, sh)).resize((w - Ld - Rd, h), Image.BILINEAR), (Ld, 0))
    lay.paste(src.crop((sw - R, 0, sw, sh)).resize((max(1, Rd), h), Image.BILINEAR), (w - Rd, 0))
    comp(im, lay, x, y)


# ---------------------------------------------------------------------- the wall
def cyc_wall_rect(im, x, y, w, h, screen_h=None):
    """the cyclorama inside a rectangle of a screen, its bands where they are on the full wall (the title's left
    two fifths and action bar)"""
    screen_h = screen_h or im.height
    wall = load_tex("T_UI_Cyc_Wall").resize((16, screen_h), Image.BILINEAR)
    y0 = max(0, int(round(y)))
    y1 = min(screen_h, int(round(y + h)))
    if y1 > y0 and w > 0:
        comp(im, wall.crop((0, y0, 16, y1)).resize((int(round(w)), y1 - y0), Image.BILINEAR), x, y0)


def cyc_pool(im, cx, cy, r=256.0, tint="Chalk", alpha=0.5):
    """a halftone light pool: the quarter texture four times, mirrored about the centre"""
    q = _prep("T_UI_Cyc_Pool", tint, alpha)
    r = int(round(r))
    if r != 256:
        q = q.resize((r, r), Image.BILINEAR)
    comp(im, q, cx - r, cy - r)
    comp(im, q.transpose(Image.FLIP_LEFT_RIGHT), cx, cy - r)
    comp(im, q.transpose(Image.FLIP_TOP_BOTTOM), cx - r, cy)
    comp(im, q.transpose(Image.ROTATE_180), cx, cy)


def cyc_wall(w=1920, h=1080, pools=((520, 400, 420, "Chalk", 0.42), (1420, 520, 400, "Chalk", 0.36)), side=True):
    """a whole screen of wall -> new RGBA image. pools: (cx, cy, radius, tint, alpha) in 1080p px of the centre
    16:9 block. On a screen wider than 16:9 the wall runs on and the side dressing stands outside the block."""
    im = new_screen(w, h)
    cyc_wall_rect(im, 0, 0, w, h, h)
    k = h / 1080.0
    bx = (w - h * 16.0 / 9.0) / 2.0
    for (cx, cy, r, tint, a) in pools:
        cyc_pool(im, bx + cx * k, cy * k, r * k, tint, a)
    if side and bx > 8:
        sw = int(round(h / 3.0))
        src = load_tex("T_UI_Cyc_Side").resize((sw, h), Image.BILINEAR)
        comp(im, src, bx - sw, 0)
        comp(im, src.transpose(Image.FLIP_LEFT_RIGHT), w - bx, 0)
    return im


# ---------------------------------------------------------------------- panels (cel finish: ink outline, hard shadow)
OUTLINE = 3         # every panel, card and button
SHADOW = 7          # hard offset shadow of panels (buttons and tiles: 6)


def paper_panel(im, x, y, w, h, stock="Paper", outline=OUTLINE, shadow=SHADOW, shadow_col="Ink", grain=True):
    """call-sheet stock: hard shadow, ink outline, the stock colour with paper grain (T_UI_Paper tiled, tinted).
    stock: Paper, Aqua, Sticky, Ledger, Manila, Chalk or any colour."""
    if shadow:
        box(im, x + shadow, y + shadow, w, h, shadow_col)
    box(im, x, y, w, h, "Ink")
    if grain:
        tiled(im, "T_UI_Paper", x + outline, y + outline, w - 2 * outline, h - 2 * outline, tint=stock, ox=x, oy=y)
    else:
        box(im, x + outline, y + outline, w - 2 * outline, h - 2 * outline, stock)


def slate_panel(im, x, y, w, h, outline=OUTLINE, shadow=SHADOW, rim=True, fill="Slate", shadow_col="Ink", dust=True):
    """clapper-slate black: hard shadow, ink outline, chalk dust (T_UI_SlateDust tiled) and an inner chalk hairline"""
    if shadow:
        box(im, x + shadow, y + shadow, w, h, shadow_col)
    box(im, x, y, w, h, "Ink")
    box(im, x + outline, y + outline, w - 2 * outline, h - 2 * outline, fill)
    if dust:
        tiled(im, "T_UI_SlateDust", x + outline, y + outline, w - 2 * outline, h - 2 * outline, tint="Chalk", ox=x, oy=y)
    if rim:
        frame(im, x + 7, y + 7, w - 14, h - 14, "Chalk", 1.5, alpha=0.22)


def ledger_panel(im, x, y, w, h, row_y, row_h, rows, margin=60, outline=OUTLINE, shadow=SHADOW):
    """green-bar ledger: Ledger stock, every other row a LedgerBar box, two red margin rules, three punch holes.
    row_y = y of the first row, row_h = row pitch, rows = how many"""
    paper_panel(im, x, y, w, h, "Ledger", outline, shadow)
    for k in range(rows):
        if k % 2 == 0:
            box(im, x + outline, row_y + k * row_h, w - 2 * outline, row_h, "LedgerBar")
    for mx in (margin, margin + 5):
        box(im, x + mx, y + outline, 1.5, h - 2 * outline, "Red", 0.65)
    for t in (0.14, 0.5, 0.86):
        cy = y + h * t
        cv = Cv(26, 26)
        cv.circle(13, 13, 10, fill=SLATE, outline=INK, width=2.5)
        comp(im, cv.out(), x + margin / 2.0 - 13, cy - 13)


def clipboard_panel(im, x, y, w, h, pad=14):
    """the clipboard of the skill detail: a Manila board, a sheet of Paper on it, the metal clip on its top edge"""
    paper_panel(im, x, y, w, h, mix("Manila", "AmberInk", 0.35), shadow=SHADOW)
    paper_panel(im, x + pad, y + pad + 10, w - 2 * pad, h - 2 * pad - 10, "Paper", shadow=4, shadow_col=mix("Manila", "Ink", 0.55))
    paste(im, "T_UI_ClipboardClip", x + w / 2.0 - 104, y - 28)


def sheet_frame(im, x, y, w, h, foot=0.0, tape=True):
    """the storyboard sheet as a hollow frame over whatever is on the screen (the live view). x, y, w, h = the sheet's
    outer edge; the window is 30 px inside it. foot > 0 adds a caption foot of that height under the window."""
    nine_slice(im, "T_UI_SheetFrame", x, y, w + 8, h + 8, centre=False)
    if foot:
        paper_panel(im, x, y + h - OUTLINE, w, foot + OUTLINE, "Paper", shadow=8)
        box(im, x + OUTLINE, y + h - OUTLINE, w - 2 * OUTLINE, OUTLINE, "Ink")
    if tape:
        paste(im, "T_UI_Tape", x - 34, y + 2, angle=38, pivot=(64, 16))
        paste(im, "T_UI_Tape", x + w + 34, y + 2, angle=-38, pivot=(64, 16))
    return (x + 30, y + 30, w - 60, h - 60)


def memo(im, x, y, w, h):
    """the upright paper memo (continue label, save status, notices). Text starts at x + 20, y + 52."""
    nine_slice(im, "T_UI_Memo", x, y, w + 6, h + 6)


def dailies_card(im, x, y, w, h):
    """THE DAILIES card stock. Text starts at x + 20, y + 50; the head band (30 px) can carry a title in Chalk."""
    nine_slice(im, "T_UI_Dailies_Card", x, y, w + 6, h + 6)


def title(im, x, y, s, height=66.0, fill="Ink", offset="Pink", rule=True, sub=None, sub_fill="Ink"):
    """a screen title: Bungee with a printed pink offset (3 px), a clapper-stripe rule under it, an optional
    Rajdhani sub line. (x, y) = top left of the line box. -> width"""
    w = text_w(s, "BUN", height, 1.0)
    text(im, x + 4, y + 4, s, "BUN", height, offset, track=1.0)
    text(im, x, y, s, "BUN", height, fill, track=1.0)
    base = y + cap_off("BUN", height) + cap_h("BUN", height)
    if rule:
        box(im, x - 2, base + 12, w + 4, 14, "Ink")
        tiled(im, "T_UI_HazardStripes", x, base + 14, w, 10, tint="Chalk", scale=0.75)
    if sub:
        text(im, x + 2, base + 34, sub, "RAJ", 24, sub_fill, track=1.5)
    return w


# ---------------------------------------------------------------------- small parts
def icon(im, name, x, y, size=32.0, tint="Ink", alpha=1.0):
    """an icon of T_UI_Icons by name (ICON_NAMES), its cell drawn size x size with its top left at (x, y).
    badge_check and badge_lock are full colour: they ignore the tint."""
    c = cell("T_UI_Icons", ICON_INDEX[name])
    if name in ICON_COLOUR:
        tint = None
    size = int(round(size))
    src = _prep(c, tint, alpha)
    comp(im, src if size == 64 else src.resize((size, size), Image.LANCZOS), x, y)


def stamp(im, cx, cy, which="approved", angle=-8.0, scale=1.0, alpha=1.0):
    """a rubber stamp centred on (cx, cy), turned. which: 'approved' or a rating 0..3"""
    if which == "approved":
        paste(im, "T_UI_Stamp_Approved", cx, cy, scale=scale, angle=angle, pivot=(96, 36), alpha=alpha)
    else:
        paste(im, cell("T_UI_Stamp_Rating", int(which)), cx, cy, scale=scale, angle=angle, pivot=(RATING_CELL[0] / 2.0, RATING_CELL[1] / 2.0), alpha=alpha)


def highlighter(im, x, y, w, h=24.0, tint="Amber"):
    """a highlighter stroke behind a value on paper"""
    three_slice_h(im, "T_UI_Highlighter", x, y, w, h, tint=tint)


def chip(im, x, y, label, fill="Teal", fg="White", height=24.0, pad=8.0, key="BUN", align=0, track=1.0):
    """a flat label chip (a box and live text): AVAILABLE, PENDING, T1. (x, y) = top left. -> width"""
    w = text_w(label, key, height, track) + 2 * pad
    x0 = x - (0 if align == 0 else w / 2.0 if align == 1 else w)
    bh = cap_h(key, height) + 12
    box(im, x0, y, w, bh, fill)
    text(im, x0 + pad, y + 6 - cap_off(key, height), label, key, height, fg, track=track)
    return w


def keycap(im, x, y, label, h=32.0, on="dark", align=0, height=None):
    """a key or pad-button chip: T_UI_Keycap nine-sliced, Rajdhani inside. on='dark': the light cap (Ink text),
    on='paper': the dark cap (Chalk text). (x, y) = top left, h includes the 4 px lip. -> width"""
    height = height or max(24.0, h * 0.78)
    w = max(h, text_w(label, "RAJ", height, 0.6) + h * 0.62)
    x0 = x - (0 if align == 0 else w / 2.0 if align == 1 else w)
    nine_slice(im, "T_UI_Keycap", x0, y, w, h, cell_i=1 if on == "dark" else 0)
    body = h - 4
    text(im, x0 + w / 2.0, y + (body - cap_h("RAJ", height)) / 2.0 - cap_off("RAJ", height), label, "RAJ", height,
         "Ink" if on == "dark" else "Chalk", align=1, track=0.6)
    return w


def hint_row(im, x, y, pairs, h=32.0, on="dark", align=0, gap=26.0, height=24.0, fill=None):
    """the bottom hint row, set like a billing block: (key, ACTION) pairs. (x, y) = top left (or centre / right
    edge with align). -> width"""
    fill = fill or ("Chalk" if on == "dark" else "Ink")

    def width():
        t = 0.0
        for kk, lab in pairs:
            t += max(h, text_w(kk, "RAJ", max(24.0, h * 0.78), 0.6) + h * 0.62) + 8 + text_w(lab, "RAJ", height, 1.5) + gap
        return t - gap
    tot = width()
    x0 = x - (0 if align == 0 else tot / 2.0 if align == 1 else tot)
    for kk, lab in pairs:
        x0 += keycap(im, x0, y, kk, h, on) + 8
        x0 += text(im, x0, y + (h - 4 - cap_h("RAJ", height)) / 2.0 - cap_off("RAJ", height), lab, "RAJ", height, fill, track=1.5) + gap
    return tot


def marquee(im, x, y, w, h, band=14.0, pitch=23.0, lit=lambda i: i % 3 != 2, band_col=(96, 24, 40)):
    """the chasing bulb border of the hero action, outside the rectangle: a painted band with an ink outline and
    T_UI_Bulb round it. lit(i) says which bulbs are on (the chase is which index is lit)."""
    box(im, x - band - 3, y - band - 3, w + 2 * band + 6, h + 2 * band + 6, "Ink")
    box(im, x - band, y - band, w + 2 * band, h + 2 * band, band_col)
    nx = max(2, int(round((w + band) / pitch)))
    ny = max(1, int(round((h + band) / pitch)))
    pts = []
    for k in range(nx):
        pts.append((x - band / 2.0 + (w + band) * k / nx, y - band / 2.0))
    for k in range(ny):
        pts.append((x + w + band / 2.0, y - band / 2.0 + (h + band) * k / ny))
    for k in range(nx):
        pts.append((x + w + band / 2.0 - (w + band) * k / nx, y + h + band / 2.0))
    for k in range(ny):
        pts.append((x - band / 2.0, y + h + band / 2.0 - (h + band) * k / ny))
    on, off = cell("T_UI_Bulb", 0), cell("T_UI_Bulb", 1)
    for i, (bx, by) in enumerate(pts):
        comp(im, on if lit(i) else off, bx - 16, by - 16)


BUTTON_STATES = ("idle", "focus", "pressed", "disabled")


def button(im, x, y, w, h, label, state="idle", key=None, icon_name=None, hero=False, lit=None):
    """THE button. idle: a slate plate with a dim clapper-stripe end. focus: flips to lit paper with amber stripes and
    a pink print shadow; hero=True puts the chasing bulb marquee round it instead. pressed: amber, sunk 4 px, no
    shadow. disabled: hatched, no chalk. key = the input's name, set in a key cap under the label.
    -> the rectangle (x, y, w, h): the hit box never moves"""
    sw = 22
    X, Y = x, y
    if state == "focus":
        if hero:
            marquee(im, x, y, w, h, lit=lit or (lambda i: i % 3 != 2))
        else:
            box(im, x + 7, y + 7, w, h, "Pink")
        box(im, x, y, w, h, "Ink")
        tiled(im, "T_UI_Paper", x + 3, y + 3, w - 6, h - 6, tint=(255, 246, 224))
        tiled(im, "T_UI_HazardStripes", x + 3, y + 3, sw, h - 6, tint="Amber")
        fg, on = "Ink", "paper"
    elif state == "pressed":
        X, Y = x + 4, y + 4
        box(im, X, Y, w, h, "Ink")
        box(im, X + 3, Y + 3, w - 6, h - 6, "Amber")
        tiled(im, "T_UI_HazardStripes", X + 3, Y + 3, sw, h - 6, tint="Chalk")
        fg, on = "Ink", "paper"
    elif state == "disabled":
        box(im, x, y, w, h, "Steel")
        box(im, x + 2, y + 2, w - 4, h - 4, mix("Slate2", "Steel", 0.25))
        tiled(im, "T_UI_Hatch", x + 2, y + 2, w - 4, h - 4)
        fg, on = mix("Steel", "Mute", 0.45), None
    else:
        box(im, x + 6, y + 6, w, h, "Ink")
        box(im, x, y, w, h, "Ink")
        box(im, x + 3, y + 3, w - 6, h - 6, "Slate2")
        tiled(im, "T_UI_HazardStripes", x + 3, y + 3, sw, h - 6, tint=mix("Steel", "Mute", 0.55))
        box(im, x + 3 + sw + 3, y + 3, w - sw - 9, 2, "Chalk", 0.16)
        fg, on = "Chalk", "dark"
    if state != "disabled":
        box(im, X + 3 + sw, Y + 3, 3, h - 6, "Ink")
    height = min(45.0 if hero else 36.0, h * 0.50)
    cap = cap_h("BUN", height)
    cx = X + sw + 6 + (w - sw - 6) / 2.0
    isz = cap * 1.5 if icon_name else 0.0
    maxw = w - sw - 30 - (isz + 10 if icon_name else 0)
    lw = min(maxw, text_w(label, "BUN", height, 0.5))
    tot = lw + (isz + 10 if icon_name else 0)
    kh = 30.0 if key else 0.0
    ty = Y + (h - cap - (kh + 6 if key else 0)) / 2.0
    if icon_name:
        icon(im, icon_name, cx - tot / 2.0, ty + cap / 2.0 - isz / 2.0, isz, fg)
    text(im, cx - tot / 2.0 + (isz + 10 if icon_name else 0), ty - cap_off("BUN", height), label, "BUN", height, fg, track=0.5, maxw=maxw)
    if key:
        if on:
            keycap(im, cx, ty + cap + 7, key, kh, on, align=1)
        else:
            text(im, cx, ty + cap + 8, key, "RAJ", 24, fg, align=1, track=0.8)
    return (x, y, w, h)


def plaque(im, x, y, w, label, state="idle", icon_name=None):
    """a tab as a door plaque (T_UI_Plaque, 66 px high + shadow). state: idle / focus / active. The active plaque is
    open at its foot and should overlap the board below by 3 px. -> the rectangle (x, y, w, 66)"""
    i = PLAQUE_STATES.index(state)
    three_slice_h(im, "T_UI_Plaque", x, y, w + 6, 72, cell_i=i)
    fg = "Ink" if state == "active" else ("Chalk" if state == "focus" else mix("Mute", "Chalk", 0.35))
    ic = "Teal" if state == "active" else ("Cyan" if state == "focus" else "Mute")
    height = 28.5
    cap = cap_h("BUN", height)
    lw = text_w(label, "BUN", height, 0.5)
    isz = 28 if icon_name else 0
    tot = lw + (isz + 10 if icon_name else 0)
    yo = 7 if state == "active" else 0
    if icon_name:
        icon(im, icon_name, x + (w - tot) / 2.0, y + (66 - isz) / 2.0 + yo, isz, ic)
    text(im, x + (w - tot) / 2.0 + (isz + 10 if icon_name else 0), y + (66 - cap) / 2.0 + yo - cap_off("BUN", height), label, "BUN",
         height, fg, track=0.5, maxw=w - 60 - (isz + 10 if icon_name else 0))
    return (x, y, w, 66)


def cursor(im, x, y, w, h, tint="Pink", gap=8.0):
    """the selection cursor: T_UI_Cursor nine-sliced round the rectangle, gap px outside it"""
    nine_slice(im, "T_UI_Cursor", x - gap - 10, y - gap - 10, w + 2 * gap + 20 + 6, h + 2 * gap + 20 + 6, tint=tint, centre=False)


def film_strip(im, x, y, w, h, vertical=None):
    """a strip of film: the base and a row (or column) of perforations down both edges"""
    vertical = h > w if vertical is None else vertical
    box(im, x, y, w, h, FILM_BASE)
    if vertical:
        sc = 0.5 if w < 40 else 0.75
        tiled(im, "T_UI_FilmPerf", x, y, 16 * sc, h, scale=sc)
        tiled(im, "T_UI_FilmPerf", x + w - 16 * sc, y, 16 * sc, h, scale=sc)
    else:
        tiled(im, "T_UI_FilmPerf", x, y, w, 12, scale=0.75)
        tiled(im, "T_UI_FilmPerf", x, y + h - 12, w, 12, scale=0.75)


def film_meter(im, x, y, w, h, frac, n, tint="Cyan", empty="Steel", marks=()):
    """a 35 mm strip meter (XP, progress): perforations top and bottom, one frame per segment.
    marks: ((fraction, colour), ...) for scene ticks and the finale mark"""
    box(im, x, y, w, h, FILM_BASE)
    perf = max(6.0, h * 0.24)
    sc = perf / 16.0
    tiled(im, "T_UI_FilmPerf", x + 2, y, w - 4, perf, scale=sc)
    tiled(im, "T_UI_FilmPerf", x + 2, y + h - perf, w - 4, perf, scale=sc)
    iy, ih = y + perf, h - 2 * perf
    gap = 3.0
    cw = (w - 6 - gap * (n - 1)) / float(n)
    filled = max(0.0, min(1.0, frac)) * n
    for k in range(n):
        cx = x + 3 + k * (cw + gap)
        box(im, cx, iy, cw, ih, empty)
        v = max(0.0, min(1.0, filled - k))
        if v > 0:
            box(im, cx, iy, cw * v, ih, tint)
    for (t, c) in marks:
        box(im, x + 3 + (w - 6) * t - 1.5, y - 3, 3, h + 6, c)


def meter(im, x, y, w, h, frac, n, tint="Green", track="Steel", gap=4.0, lean=0.35, plate="Ink", pad=3.0, ghost=0.0):
    """the HUD's chunky segmented meter: slanted segments on an ink bed. In the game each segment is two triangles
    (lean = 0.35 x height); the cel highlight is the top 30 % in a lighter tone. ghost = the white trail of what
    was just lost (a fraction)."""
    ss = 4
    W, H = int(round(w + 2 * pad + h * lean + 4)), int(round(h + 2 * pad + 2))
    cv = Cv(W, H, ss=ss)
    ln = h * lean
    ox, oy = pad + 1, pad + 1
    if plate:
        cv.poly(para(ox - pad, oy - pad, w + 2 * pad + ln, h + 2 * pad, (h + 2 * pad) * lean), fill=col(plate))
    pitch = (w + gap) / float(n)
    sw = pitch - gap
    filled = max(0.0, min(1.0, frac)) * n
    trail = max(0.0, min(1.0, ghost)) * n
    for i in range(n):
        sx = ox + i * pitch
        cv.poly(para(sx, oy, sw + ln, h, ln), fill=col(track))
        v = max(0.0, min(1.0, filled - i))
        was = max(0.0, min(1.0, trail - i))
        if was > v:
            cv.poly(para(sx, oy, sw * was + ln, h, ln), fill=rgba(WHITE, 0.85))
        if v > 0:
            cv.poly(para(sx, oy, sw * v + ln, h, ln), fill=col(tint))
            cv.poly(para(sx + ln * 0.7, oy, sw * v + ln * 0.3, h * 0.3, ln * 0.3), fill=mix(tint, WHITE, 0.45))
    comp(im, cv.out(), x - ox, y - oy)


def blocks(im, x, y, w, h, frac, n, tint="Cyan", track="Steel", gap=4.0):
    """the plain segmented bar of the menus (today's Meter): boxes"""
    cw = (w - gap * (n - 1)) / float(n)
    filled = max(0.0, min(1.0, frac)) * n
    for k in range(n):
        cx = x + k * (cw + gap)
        box(im, cx, y, cw, h, track)
        v = max(0.0, min(1.0, filled - k))
        if v > 0:
            box(im, cx, y, cw * v, h, tint)


# ---------------------------------------------------------------------- big numerals: the digit atlas
_dcell = {}


def _digit_key(size):
    return "L" if size > 44 else "S"


def digits_w(s, size=64.0):
    """width of a string of atlas numerals whose capitals are 'size' px high"""
    D = DIGITS[_digit_key(size)]
    k = size / D["cap"]
    return sum(D["advance"][DIGIT_CHARS.index(ch)] for ch in s) * k


def draw_digits(im, x, y, s, size=64.0, tint="Chalk", align=0, ink=True, ink_tint=None):
    """BIG NUMERALS from the baked atlas (never live text): characters 0-9 $ , . / % x + - : and space.
    (x, y) = top left of the capitals, size = their height in px (L atlas above 44 px, S below), tint = the fill's
    colour, align 0 left / 1 centre / 2 right. Ink cells of the whole string go down first, then the fills,
    exactly as the game must draw them. ink_tint tints the ink layer (default: as baked). -> width"""
    key = _digit_key(size)
    D = DIGITS[key]
    k = size / D["cap"]
    tot = digits_w(s, size)
    x0 = x - (0 if align == 0 else tot / 2.0 if align == 1 else tot)
    base = y + size
    cw, ch_ = D["cell"]
    for layer in (("T_UI_Digits_Ink", ink_tint), ("T_UI_Digits_Fill", tint)):
        if layer[0] == "T_UI_Digits_Ink" and not ink:
            continue
        px = x0
        for c in s:
            n = DIGIT_CHARS.index(c)
            if c != " ":
                ck = (layer[0], key, n, round(k, 4), col(layer[1]) if layer[1] is not None else None)
                if ck not in _dcell:
                    cx = D["at"][0] + (n % 10) * cw
                    cy = D["at"][1] + (n // 10) * ch_
                    g = load_tex(layer[0]).crop((cx, cy, cx + cw, cy + ch_))
                    if layer[1] is not None:
                        g = tint_img(g, layer[1])
                    if abs(k - 1.0) > 1e-3:
                        g = g.resize((max(1, int(round(cw * k))), max(1, int(round(ch_ * k)))), Image.LANCZOS if k < 1 else Image.BICUBIC)
                    _dcell[ck] = g
                comp(im, _dcell[ck], px - D["pen"][0] * k, base - D["pen"][1] * k)
            px += D["advance"][n] * k
    return tot


# ---------------------------------------------------------------------- the shoot list and the skill tiles
def board(im, i, x, y, w=384.0, outline=3.0, shadow=0.0):
    """storyboard frame of shoot i (0..7) with its boss ringed: a cell of T_UI_Boards, w px wide, in an ink frame"""
    h = w * 216.0 / 384.0
    if shadow:
        box(im, x + shadow, y + shadow, w, h, "Ink")
    c = cell("T_UI_Boards", i)
    comp(im, c if int(round(w)) == 384 else c.resize((int(round(w)), int(round(h))), Image.LANCZOS), x, y)
    if outline:
        frame(im, x, y, w, h, "Ink", outline)
    return (x, y, w, h)


def board_thumb(im, i, x, y, w=128.0, outline=2.5):
    """the small clean frame of shoot i: a cell of T_UI_BoardThumbs"""
    h = w * 72.0 / 128.0
    c = cell("T_UI_BoardThumbs", i)
    comp(im, c if int(round(w)) == 128 else c.resize((int(round(w)), int(round(h))), Image.LANCZOS), x, y)
    if outline:
        frame(im, x, y, w, h, "Ink", outline)
    return (x, y, w, h)


def lockup(im, i, x, y, scale=1.0):
    """the two-ink shoot lockup of shoot i (a cell of T_UI_Lockups, 448 x 128)"""
    return paste(im, cell("T_UI_Lockups", i), x, y, scale=scale)


def list_row(im, x, y, w, h, i, focus=False, best=None, ext=0.0):
    """a row of the shoot list (the production strip board). idle: a slate strip with the shoot's colour tab.
    focus: lit paper, pink print shadow, the number on the shoot's colour, an arrow and an amber stripe end.
    best: the best take's figure, '' for an unplayed shoot (shown as BEST ------), None for no best column.
    -> the rectangle"""
    name, sub = SHOOTS[i]
    c2 = PAL["Shoot%d" % (i + 1)]
    th = h - 14
    tw_ = th * 16 / 9.0
    if focus:
        w2 = w + ext
        box(im, x + 7, y + 7, w2, h, "Pink")
        box(im, x, y, w2, h, "Ink")
        tiled(im, "T_UI_Paper", x + 3, y + 3, w2 - 6, h - 6, tint=(255, 246, 224))
        box(im, x + 3, y + 3, 60, h - 6, c2)
        box(im, x + 63, y + 3, 3, h - 6, "Ink")
        text_cap(im, x + 33, y + h / 2.0 - 11, "%02d" % (i + 1), "BUN", 22, "Ink", align=1)
        board_thumb(im, i, x + 76, y + 7, tw_, 2.5)
        tx = x + 76 + tw_ + 16
        end = 44
        tiled(im, "T_UI_HazardStripes", x + w2 - 3 - end, y + 3, end, h - 6, tint="Amber")
        box(im, x + w2 - 6 - end, y + 3, 3, h - 6, "Ink")
        icon(im, "arrow", x + w2 - end - 44, y + h / 2.0 - 16, 32, "Pink")
        right = x + w2 - end - 54
        fg, sf = "Ink", (104, 60, 84)
    else:
        box(im, x, y, w, h, "Ink")
        box(im, x + 2, y + 2, w - 4, h - 4, "Slate2")
        box(im, x + 2, y + 2, w - 4, 2, "Chalk", 0.10)
        box(im, x + 2, y + 2, 10, h - 4, c2)
        text_cap(im, x + 39, y + h / 2.0 - 9, "%02d" % (i + 1), "BUN", 18, c2, align=1)
        board_thumb(im, i, x + 66, y + 7, tw_, 2)
        tx = x + 66 + tw_ + 16
        right = x + w - 14
        fg, sf = "Chalk", "Mute"
    if best is not None:
        bt = "BEST " + (best if best else "------")
        bw = text_w(bt, "RAJ", 24, 1.0)
        text(im, right - bw, y + h / 2.0 - 12, bt, "RAJ", 24, sf if not focus else "AmberInk", track=1.0)
        right -= bw + 14
    nh = 36.0 if focus else 33.0
    text_cap(im, tx, y + h * 0.17, name, "BUN", cap_h("BUN", nh), fg, track=0.5, maxw=right - tx)
    text_cap(im, tx, y + h * 0.17 + cap_h("BUN", nh) + 9, sub, "RAJ", 12.1, sf, track=1.4, maxw=right - tx)
    return (x, y, w, h)


TILE_STATES = ("owned", "available", "pending", "locked")


def skill_tile(im, x, y, w, h, tier, name, code, state="available", cursor_on=False, dim=False):
    """an index card on the film strip. owned: Paper, a green check badge in the top right corner, APPROVED stamped
    low on the card clear of the word OWNED. available: Aqua stock, a teal chip. pending: Sticky note, an orange
    highlighter under PENDING, a paperclip. locked: dark, hatched, a padlock. cursor_on: the 6 px pink frame with
    its hard shadow. dim: one step darker (every tile but the one under the cursor). -> the rectangle"""
    fx = 34                                             # the film strip tab on the left carries the tier number
    stock = {"owned": "Paper", "available": "Aqua", "pending": "Sticky", "locked": mix("Slate2", "Steel", 0.15)}[state]
    box(im, x + 6, y + 6, w, h, "Ink")
    box(im, x, y, w, h, "Ink")
    film_strip(im, x + 3, y + 3, fx - 3, h - 6, vertical=True)
    tc = {"owned": "Green", "available": "Cyan", "pending": "Amber", "locked": "Mute"}[state]
    text_cap(im, x + 3 + (fx - 3) / 2.0, y + h / 2.0 - 9, str(tier), "BUN", 18, tc, align=1)
    if state == "locked":
        box(im, x + fx + 3, y + 3, w - fx - 6, h - 6, stock)
        tiled(im, "T_UI_Hatch", x + fx + 3, y + 3, w - fx - 6, h - 6)
        fg, sub = mix("Mute", "Chalk", 0.3), "Mute"
    else:
        tiled(im, "T_UI_Paper", x + fx + 3, y + 3, w - fx - 6, h - 6, tint=stock)
        fg, sub = "Ink", (92, 80, 96)
    box(im, x + fx, y + 3, 3, h - 6, "Ink")
    tx = x + fx + 14
    room = w - fx - 26 - (30 if state in ("pending", "owned") else 0)
    text(im, tx, y + 9, name, "RAJ", 28.5, fg, track=0.4, maxw=room)
    text(im, tx, y + 37, code, "RAJ", 24, sub, track=0.6, maxw=w - fx - 26)
    by = y + h - 33
    if state == "owned":
        text(im, tx, by, "OWNED", "RAJ", 24, "GreenInk", track=1.2)
        stamp(im, x + w - 60, y + h - 25, "approved", angle=-6, scale=0.52)
        icon(im, "badge_check", x + w - 30, y - 12, 40)
    elif state == "available":
        chip(im, tx - 4, by - 1, "AVAILABLE", "Teal", "White", 24, 6, key="RAJ", track=1.2)
    elif state == "pending":
        tw_ = text_w("PENDING", "RAJ", 24, 1.2)
        highlighter(im, tx - 6, by + 1, tw_ + 12, 24, (255, 150, 60))
        text(im, tx, by, "PENDING", "RAJ", 24, "Ink", track=1.2)
        paste(im, "T_UI_Paperclip", x + w - 24, y + 14, angle=-10, pivot=(16, 32), scale=0.8)
    else:
        icon(im, "padlock", tx - 2, by + 1, 22, "Mute")
        text(im, tx + 26, by, "LOCKED", "RAJ", 24, "Mute", track=1.2)
    if dim:
        box(im, x, y, w, h, "Ink", 0.14)
    if cursor_on:
        cursor(im, x, y, w, h)
    return (x, y, w, h)


# ---------------------------------------------------------------------- stickers and hero art
def sticker_cash(im, x, y, amount, label="CASH", scale=1.0):
    """the yellow price burst with the label (Rajdhani) and the amount (atlas numerals, Ink). (x, y) = top left of
    the 288 x 144 sticker. -> (w, h)"""
    paste(im, "T_UI_Sticker_Cash", x, y, scale=scale)
    cx = x + 140 * scale
    if label:
        text(im, cx, y + 26 * scale, label, "RAJ", 24 * scale, "Ink", align=1, track=3 * scale)
    size = 30.0 * scale
    while digits_w(amount, size) > 186 * scale and size > 14:
        size -= 1
    draw_digits(im, cx - 2 * scale, y + (60 if label else 52) * scale, amount, size, "Ink", align=1, ink=False)
    return (288 * scale, 144 * scale)


def sticker_sp(im, x, y, n, scale=1.0):
    """the cyan skill point token with its number (atlas numerals, Ink). (x, y) = top left of the 128 x 128 sticker"""
    paste(im, "T_UI_Sticker_SP", x, y, scale=scale)
    s = str(n)
    size = (36.0 if len(s) < 3 else 24.0) * scale
    draw_digits(im, x + 58 * scale, y + 60 * scale - size / 2.0, s, size, "Ink", align=1, ink=False)
    return (128 * scale, 128 * scale)


def sticker_level(im, x, y, n, l1="DRIVER", l2="LEVEL"):
    """the driver level rating box: the number in the chalk cell, two lines in the slate cell. (x, y) = top left of
    the 256 x 96 sticker"""
    paste(im, "T_UI_Sticker_Level", x, y)
    s = str(n)
    size = 48.0 if len(s) < 2 else 36.0
    draw_digits(im, x + 46, y + 45 - size / 2.0, s, size, "Ink", align=1, ink=False)
    text(im, x + 104, y + 14, l1, "RAJ", 24, "Chalk", track=2.5)
    text(im, x + 104, y + 40, l2, "BUN", 33, "Chalk", track=0.5, maxw=138)
    return (256, 96)


def chase_strip(im, x, y, w, h=150.0, car=0.62, villains=((0.30, 1), (0.12, 2)), gunship=0.82, scale=0.62, rule=16.0, skyline=True):
    """the masthead band of PRE-PRODUCTION and PAYCHECK: the skyline tile, the clapper-stripe rule, and the chase
    driving along the rule. car / villains / gunship are positions 0..1 along the band (None = leave out);
    villains: (position, cell) with cell 1 truck, 2 sedan. The band's rectangle is x, y, w, h; nothing is clipped
    by the game, so keep sprites inside. -> y of the rule's top"""
    ry = y + h - rule
    if skyline:
        tiled(im, "T_UI_Chase_Skyline", x, ry - 128 * 0.75, w, 128 * 0.75, scale=0.75)
    box(im, x, ry - 2, w, rule + 2, "Ink")
    tiled(im, "T_UI_ClapStripes", x, ry, w, rule - 2, tint="Chalk", scale=0.5)

    def put(ci, t, lift=0.0):
        c = cell("T_UI_Chase", ci)
        cw, ch_ = 320 * scale, 128 * scale
        paste(im, c, x + w * t - cw / 2.0, ry - CHASE_GROUND * scale - lift, scale=scale)
    if gunship is not None:
        put(3, gunship, 26.0)
    for (t, ci) in villains or ():
        put(ci, t)
    if car is not None:
        put(0, car)
    return ry


def kestrel_hero(im, x, y, scale=1.0, burst=True):
    """the KESTREL cut-out sticker (and the explosion sticker behind its tail). (x, y) = top left of the car's
    texture"""
    if burst:
        paste(im, "T_UI_Hero_Burst", x + MAN["T_UI_Kestrel_Hero"]["size"][0] * scale - 300 * scale, y - 150 * scale, scale=scale)
    return paste(im, "T_UI_Kestrel_Hero", x, y, scale=scale)


def branch_art(im, x, y, branch, scale=1.0):
    """the skill detail's picture: T_UI_BranchBurst tinted with the branch's colour, the branch's big icon on it in
    Ink. branch 0..4 (BRANCHES). (x, y) = top left of the 320 x 320 burst"""
    paste(im, "T_UI_BranchBurst", x, y, tint=BRANCHES[branch][2], scale=scale)
    o = 154 - BIG_ICON / 2.0
    paste(im, cell("T_UI_IconsBig", branch), x + (o + 4) * scale, y + (o + 2) * scale, tint="Chalk", scale=scale)
    paste(im, cell("T_UI_IconsBig", branch), x + o * scale, y + (o - 2) * scale, tint="Ink", scale=scale)


# ---------------------------------------------------------------------- the HUD
def hud_plate(im, x, y, w, h, key="Chalk"):
    """an inked HUD sticker in a layout rectangle (the texture, shadow included, fits inside it): T_UI_Hud_Plate
    for tall plates, T_UI_Hud_Bar for low ones. key = the keyline's colour (the tile's tint)."""
    three_slice_h(im, "T_UI_Hud_Plate" if h >= 160 else "T_UI_Hud_Bar", x, y, w, h, tint=key)


def hud_chip(im, x, y, w, h=40.0, tint="Cyan"):
    three_slice_h(im, "T_UI_Hud_Chip", x, y, w, h, tint=tint)


def hud_tab(im, x, y, label, tint="Cyan", fg="Ink", h=40.0, height=28.5, key="BUN", dot=None):
    """a plate's tab: a slanted chip with live text (KESTREL-88 / AGENT N. KANE, ORDNANCE, NOW FILMING). -> width"""
    tw_ = text_w(label, key, height, 0.5)
    w = tw_ + h * 0.9 + (22 if dot else 0)
    hud_chip(im, x, y, w, h, tint)
    tx = x + h * 0.5
    if dot:
        cv = Cv(20, 20)
        cv.circle(10, 10, 7, fill=col(dot), outline=INK, width=2)
        comp(im, cv.out(), tx - 2, y + h / 2.0 - 10)
        tx += 22
    text(im, tx, y + (h - cap_h(key, height)) / 2.0 - cap_off(key, height), label, key, height, fg, track=0.5)
    return w


def hud_tag(im, x, y, label, tint="Amber", fg="Ink", h=44.0, height=27.0):
    """a reward tag under the box-office plate (+3 MISSILES, NEAR MISS +$250). -> width"""
    k = h / 44.0
    w = text_w(label, "BUN", height, 0.5) + (30 + 16) * k + 4
    three_slice_h(im, "T_UI_Hud_Tag", x, y, w, h, tint=tint)
    text(im, x + 30 * k, y + (40 * k - cap_h("BUN", height)) / 2.0 - cap_off("BUN", height), label, "BUN", height, fg, track=0.5)
    return w


def balloon(im, x, y, line, height=33.0, megaphone=True, label=None):
    """the director's line: the megaphone badge and the speech balloon with live Rajdhani text.
    (x, y) = top left of the whole thing (the badge). -> (w, h)"""
    tw_ = text_w(line, "RAJ", height)
    bw = tw_ + 34 + 26 + 4
    bx = x + (60 if megaphone else 0)
    nine_slice(im, "T_UI_Hud_Balloon", bx, y + 6, bw, 64)
    text(im, bx + 36, y + 6 + (58 - cap_h("RAJ", height)) / 2.0 - cap_off("RAJ", height), line, "RAJ", height, "Ink")
    if megaphone:
        paste(im, "T_UI_Hud_Megaphone", x, y + 2)
    if label:
        hud_tab(im, bx + 30, y - 16, label, "Amber", "Ink", h=30, height=24)
    return (bx + bw - x, 72)


def brackets(im, tint="Chalk", inset=18.0):
    """the viewfinder's corner brackets at the TRUE corners of the screen"""
    w, h = im.size
    paste(im, "T_UI_Hud_Bracket", inset, inset, tint=tint)
    paste(im, "T_UI_Hud_Bracket", w - inset - 64, inset, tint=tint, flip_x=True)
    paste(im, "T_UI_Hud_Bracket", inset, h - inset - 64, tint=tint, flip_y=True)
    paste(im, "T_UI_Hud_Bracket", w - inset - 64, h - inset - 64, tint=tint, flip_x=True, flip_y=True)


def combo(im, cx, cy, mult, tint="Pink", scale=0.8, fg="White"):
    """the combo starburst centred on (cx, cy) with its multiplier (atlas numerals)"""
    paste(im, "T_UI_Hud_Combo_Ink", cx, cy, scale=scale, pivot=(76, 76))
    paste(im, "T_UI_Hud_Combo_Fill", cx, cy, tint=tint, scale=scale, pivot=(76, 76))
    s = "x%d" % mult
    size = 36.0 * scale
    draw_digits(im, cx - 2 * scale, cy - size / 2.0 - 1, s, size, fg, align=1)


def armor_low(im, cx, cy, scale=0.8):
    """the baked shout, centred on (cx, cy)"""
    paste(im, "T_UI_Hud_ArmorLow", cx, cy, scale=scale, pivot=(144, 80))


def arrow_pips(im, x, y, have, total, pitch=30.0, tint="Amber", empty="Steel"):
    """the missile count as arrow pips: full ones tinted, empties as outlines"""
    for i in range(total):
        paste(im, cell("T_UI_Hud_Pips", 0 if i < have else 1), x + i * pitch, y, tint=tint if i < have else empty)


def reel_pips(im, x, y, have, total, pitch=32.0, tint="Amber", empty="Steel"):
    """the film left, as reel pips"""
    for i in range(total):
        paste(im, cell("T_UI_Hud_Pips", 2 if i < have else 3), x + i * pitch, y, tint=tint if i < have else empty)


# ---------------------------------------------------------------------- more small helpers
def option_row(im, x, y, w, h, label, value, focus=False, arrows=True):
    """a row of OPTIONS (the calm sheet on Paper): the label on the left, the value between two arrow boxes on the
    right. focus: an Aqua row with an ink outline and a Teal bar. idle: a PaperShade rule under it. -> rectangle"""
    if focus:
        box(im, x, y, w, h, "Ink")
        box(im, x + 3, y + 3, w - 6, h - 6, "Aqua")
        box(im, x + 3, y + 3, 9, h - 6, "Teal")
    else:
        box(im, x, y + h - 2, w, 2, "PaperShade")
    hh = 28.5
    text(im, x + 26, y + (h - cap_h("RAJ", hh)) / 2.0 - cap_off("RAJ", hh), label, "RAJ", hh, "Ink", track=1.0)
    vx = x + w - 170
    if arrows:
        for (ax, flip) in ((vx - 110, True), (vx + 80, False)):
            bs = min(30.0, h - 12)
            box(im, ax, y + (h - bs) / 2.0, bs, bs, "Ink")
            box(im, ax + 2.5, y + (h - bs) / 2.0 + 2.5, bs - 5, bs - 5, (255, 252, 240))
            a = _prep(cell("T_UI_Icons", ICON_INDEX["arrow"]), "Ink").resize((int(bs - 10), int(bs - 10)), Image.LANCZOS)
            comp(im, a.transpose(Image.FLIP_LEFT_RIGHT) if flip else a, ax + 5, y + (h - bs) / 2.0 + 5)
    vh = 27.0
    text(im, vx, y + (h - cap_h("BUN", vh)) / 2.0 - cap_off("BUN", vh), value, "BUN", vh, "Teal" if focus else "Ink", align=1, track=1.0, maxw=150)
    return (x, y, w, h)


def unlock_chip(im, x, y, w, h, icon_name, label, tint="Amber", sub=None):
    """an unlock on the paycheck (two columns of these): a Paper chip with the department's icon on a block of its
    colour, the name in Rajdhani, an optional second line. -> rectangle"""
    box(im, x + 4, y + 4, w, h, "Ink")
    box(im, x, y, w, h, "Ink")
    tiled(im, "T_UI_Paper", x + 3, y + 3, w - 6, h - 6, tint="Paper", ox=x, oy=y)
    box(im, x + 3, y + 3, h - 6, h - 6, tint)
    box(im, x + h - 3, y + 3, 3, h - 6, "Ink")
    isz = min(36.0, h - 18)
    icon(im, icon_name, x + 3 + (h - 6 - isz) / 2.0, y + (h - isz) / 2.0, isz, "Ink")
    hh = 28.5
    if sub:
        text(im, x + h + 12, y + h / 2.0 - 27, label, "RAJ", hh, "Ink", track=0.4, maxw=w - h - 22)
        text(im, x + h + 12, y + h / 2.0 + 1, sub, "RAJ", 24, (92, 80, 96), track=0.6, maxw=w - h - 22)
    else:
        text(im, x + h + 12, y + (h - cap_h("RAJ", hh)) / 2.0 - cap_off("RAJ", hh), label, "RAJ", hh, "Ink", track=0.4, maxw=w - h - 22)
    return (x, y, w, h)


def grease(im, i, x, y, w=None, h=None, tint="Pink"):
    """a grease pencil mark of T_UI_Grease: 0 ring, 1 underline, 2 cross, 3 arrow; stretched to w x h if given"""
    c = cell("T_UI_Grease", i)
    if w is None:
        paste(im, c, x, y, tint=tint)
    else:
        stretch(im, c, x, y, w, h if h is not None else w * 0.5, tint=tint)


def dailies_pips(im, x, y, lit, total=4, scale=0.8, pitch=None):
    """the rating as star pips: 'lit' of 'total'"""
    pitch = pitch or 46.0 * scale
    for k in range(total):
        paste(im, cell("T_UI_Dailies_Pip", 0 if k < lit else 1), x + k * pitch, y, scale=scale)


def hud_keys(im, cx, y, pairs, h=40.0):
    """the play HUD's keys line: a slanted ink strip (T_UI_Hud_Chip tinted Slate) with the hint row on it, centred
    on cx"""
    tot = 0.0
    for kk, lab in pairs:
        tot += max(30.0, text_w(kk, "RAJ", 24.0, 0.6) + 30.0 * 0.62) + 8 + text_w(lab, "RAJ", 24, 1.5) + 24
    w = tot - 24 + 56
    hud_chip(im, cx - w / 2.0, y, w, h, "Slate")
    hint_row(im, cx, y + (h - 30) / 2.0 + 1, pairs, h=30.0, on="dark", align=1, gap=24.0)
    return w


def save(im, path, also_720=False):
    """write a screen as PNG (RGB); also_720 writes the same picture at 1280 x 720 next to it (name_720.png)"""
    rgb = im.convert("RGB")
    rgb.save(path)
    if also_720:
        root, ext = os.path.splitext(path)
        rgb.resize((int(round(rgb.width * 720.0 / rgb.height)), 720), Image.LANCZOS).save(root + "_720" + ext)
    return path


# ====================================================================== building, the manifest, the contact sheet
_built = {}


def build(name):
    """generate one texture -> RGBA at 1x, exactly the manifest's size, edge colours bled"""
    if name not in _built:
        im = TEX[name]()
        if im.mode != "RGBA":
            im = im.convert("RGBA")
        want = tuple(MAN[name]["size"])
        if im.size != want:
            raise ValueError("%s is %s, the manifest says %s" % (name, im.size, want))
        _built[name] = bleed(im)
    return _built[name]


def manifest():
    """the whole manifest as plain data (what kit_manifest.json holds)"""
    return {"kit": "FULL THROTTLE UI kit, THE STUDIO LOT, INKED", "units": "texture px = 1080p px; design px = px / 1.5",
            "palette": dict((k, hexs(v)) for k, v in PAL.items()), "order": list(ORDER),
            "textures": dict((n, MAN[n]) for n in ORDER), "digits": DIGITS, "icons": ICON_INDEX}


def build_all(out_dir, only=None):
    if not os.path.isdir(out_dir):
        os.makedirs(out_dir)
    total = 0
    for n in ORDER:
        if only and not any(o in n for o in only):
            continue
        im = build(n)
        im.save(os.path.join(out_dir, n + ".png"), optimize=True)
        total += im.width * im.height * 4
        print("%-24s %4d x %-4d  %s" % (n, im.width, im.height, MAN[n]["draw"]))
    if not only:
        with open(os.path.join(out_dir, "kit_manifest.json"), "w") as f:
            json.dump(manifest(), f, indent=1, sort_keys=True)
        print("%d textures, %.2f MB uncompressed (RGBA8), written to %s" % (len(ORDER), total / 1048576.0, out_dir))
        if total > 16 * 1048576:
            raise SystemExit("the kit is over its 16 MB budget")
    return total


def contact_sheet(tex_dir, out_path, width=2400):
    """every texture of the folder on one sheet, on the wall colour and on slate, with its name and size"""
    names = [n for n in ORDER if os.path.isfile(os.path.join(tex_dir, n + ".png"))]
    pad, lab = 14, 30
    items = []
    for n in names:
        im = Image.open(os.path.join(tex_dir, n + ".png")).convert("RGBA")
        k = min(1.0, 760.0 / im.width, 400.0 / im.height)
        if im.width < 32 and im.height > 256:                      # the wall strip: show it wide
            im = im.resize((96, 400), Image.BILINEAR)
        elif k < 1.0:
            im = im.resize((max(1, int(im.width * k)), max(1, int(im.height * k))), Image.LANCZOS)
        elif max(im.size) <= 64:
            im = im.resize((im.width * 2, im.height * 2), Image.NEAREST)
        items.append((n, im))
    rows, cur, x = [], [], pad
    for n, im in items:
        w = max(im.width, 150) + pad
        if x + w > width and cur:
            rows.append(cur)
            cur, x = [], pad
        cur.append((n, im))
        x += w
    if cur:
        rows.append(cur)
    hgt = pad + sum(max(im.height for _, im in r) + lab + pad for r in rows)
    sheet = Image.new("RGBA", (width, hgt), rgba(mix(CYC_TOP, CYC_MID, 0.5)))
    y = pad
    for r in rows:
        rh = max(im.height for _, im in r)
        x = pad
        for n, im in r:
            w = max(im.width, 150)
            half = Image.new("RGBA", (im.width, im.height // 2), rgba(SLATE))     # its lower half on slate: masks show on both
            sheet.alpha_composite(half, (x, y + im.height - im.height // 2))
            src = Image.open(os.path.join(tex_dir, n + ".png"))
            sheet.alpha_composite(im, (x, y))
            text(sheet, x, y + rh + 3, "%s  %dx%d" % (n[5:], src.width, src.height), "RAJ", 22, INK)
            x += w + pad
        y += rh + lab + pad
    sheet.convert("RGB").save(out_path, quality=90)
    print("wrote", out_path, sheet.size)


if __name__ == "__main__":
    args = sys.argv[1:]
    if args and args[0] == "textures":
        build_all(args[1] if len(args) >= 2 else KIT_OUT, only=args[2:] or None)
    elif args and args[0] == "sheet" and len(args) >= 3:
        contact_sheet(args[1], args[2])
    elif args and args[0] == "manifest":
        print(json.dumps(manifest(), indent=1, sort_keys=True))
    else:
        print(__doc__)
