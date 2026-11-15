"""Everyone else on the road: Syndicate foes and beach-town traffic. Same conventions as ft_car:
origin = middle of the rear bumper on the ground, X forward, Y right, Z up, centimetres.
Widths are set so 1 game unit = 9.14 cm (the KESTREL-88 is 28 units = 256 cm across its tyres).
"""
import math
import ft_art
from ft_car import wheel, BLACK, GLASS, CHROME, STEEL, TYRE, GOLD, PINK, CYAN, YEL, WHITE

U = 9.14


def tail(m, hw, z, h, col='#ff3355', w=None, inset=0.18):
    w = w or hw * 0.42
    for s in (-1, 1):
        m.box(col, (-2, s * (hw - hw * inset - w / 2 + w * 0.5 - w * 0.5), z), (3, w, h), glow=1.0)


def car(m, W, L, col, cab=0.42, cab_x=(0.28, 0.72), body_h=78, cab_h=130, wedge=True, wheel_r=34, accent=None, glass=GLASS):
    hw = W / 2.0 - 12
    nose = 40 if wedge else 62
    m.side(col, [(0, 26), (L - 10, 26), (L, 36), (L, nose), (L * 0.78, body_h - 6), (L * 0.2, body_h), (0, body_h - 4)], -hw, hw)
    x0, x1 = L * cab_x[0], L * cab_x[1]
    cw = hw * cab * 2
    m.side(glass, [(x1, body_h - 6), (x1 - (x1 - x0) * 0.32, cab_h), (x0 + (x1 - x0) * 0.1, cab_h), (x0 - 10, body_h)], -cw, cw)
    m.side(col, [(x1 - (x1 - x0) * 0.32, cab_h), (x1 - (x1 - x0) * 0.32 - 4, cab_h + 4), (x0 + (x1 - x0) * 0.1, cab_h + 4), (x0 + (x1 - x0) * 0.1 - 2, cab_h)], -cw + 4, cw - 4)
    m.box(BLACK, (-1, 0, 40), (3, hw * 2, 18))
    for s in (-1, 1):
        m.box('#ff3355', (-3, s * (hw - 26), body_h - 22), (2, 36, 14), glow=1.0)
        if accent:
            m.side(accent, [(10, body_h - 26), (L - 30, body_h - 30), (L - 30, body_h - 24), (10, body_h - 20)], *((hw, hw + 1.5) if s > 0 else (-hw - 1.5, -hw)))
        wheel(m, L * 0.18, wheel_r, wheel_r, hw - 26, W / 2.0, s)
        wheel(m, L * 0.8, wheel_r, wheel_r, hw - 26, W / 2.0, s)
    m.box(WHITE, (-2.5, 0, 46), (2, 40, 12))


def rammer(m, ramming=False):
    """An 80s muscle wedge in Syndicate purple, with a ram bar and a blower of its own."""
    W, L = 30 * U, 470
    col = '#ffb0c8' if ramming else '#7b3bd6'
    car(m, W, L, col, cab=0.38, cab_x=(0.34, 0.66), body_h=74, cab_h=118, accent=YEL)
    m.box(STEEL, (L + 6, 0, 46), (18, W - 10, 22))
    for s in (-1, 1):
        m.box(STEEL, (L + 2, s * (W / 2 - 20), 46), (14, 10, 46))
    m.box(CHROME, (L * 0.62, 0, 86), (60, 50, 22))
    m.box(BLACK, (L * 0.62, 0, 100), (40, 40, 8))
    m.side(BLACK, [(0, 76), (40, 76), (34, 104), (4, 104)], -6, 6)
    m.box(BLACK, (18, 0, 106), (34, W - 30, 6))
    for y in (-30, 30):
        m.cyl(CHROME, (6, y, 30), 6, 18, rot=(90, 0, 0), steps=12)


def suv(m):
    """The gunship: a black 4x4 with a roof turret, roll bar lights and a spare wheel."""
    W, L = 36 * U, 520
    car(m, W, L, '#23202c', cab=0.44, cab_x=(0.12, 0.78), body_h=112, cab_h=196, wedge=False, wheel_r=44, accent=PINK)
    m.cyl(TYRE, (-24, 0, 92), 40, 22, rot=(90, 0, 0), steps=18)
    m.cyl(GOLD, (-26, 0, 92), 24, 3, rot=(90, 0, 0), steps=16)
    m.box(STEEL, (L * 0.45, 0, 206), (90, 80, 22))
    m.cyl(BLACK, (L * 0.45, 0, 216), 26, 26, steps=16)
    for y in (-10, 10):
        m.cyl(BLACK, (L * 0.45 + 10, y, 230), 4.5, 120, rot=(-90, 0, 0), steps=8)
    m.box(BLACK, (L * 0.75, 0, 200), (10, W * 0.8, 8))
    for y in (-60, -20, 20, 60):
        m.box(YEL, (L * 0.75 + 6, y, 208), (4, 22, 14), glow=0.5)


def truck(m, boss=False):
    """The armored truck (olive plates, rivets, a rear door). The boss is a bigger black version
    with a twin-gun turret, hazard stripes and red eyes."""
    W, L = (64 if boss else 46) * U, (1250 if boss else 900)
    body, cab = ('#2e2b38', '#3f3c4c') if boss else ('#5f7a45', '#6f8a52')
    hw = W / 2 - 16
    bh = 300 if boss else 240
    m.box(body, (L * 0.39, 0, bh / 2 + 18), (L * 0.78, hw * 2, bh - 36))
    m.side(cab, [(L * 0.8, 40), (L, 40), (L, 150), (L * 0.95, 200 if not boss else 230), (L * 0.8, 210 if not boss else 240)], -hw + 10, hw - 10)
    for k in range(4):
        m.box(BLACK, (L * 0.39, 0, 60 + k * (bh - 70) / 3.0), (L * 0.79, hw * 2 + 3, 4))
    m.box(BLACK, (-1, 0, bh / 2 + 18), (3, hw * 1.1, bh * 0.75))
    m.box(STEEL, (-3, 0, bh / 2 + 18), (2, 10, bh * 0.75))
    for s in (-1, 1):
        m.box('#ff3355', (-3, s * (hw - 20), 50), (2, 26, 20), glow=1.0)
        for x in (0.1, 0.24, 0.62, 0.9):
            wheel(m, L * x, 46, 46, hw - 40, W / 2.0, s)
    m.box(BLACK, (0, 0, 22), (8, W - 20, 18))
    if boss:
        for k in range(10):
            m.box(YEL if k % 2 else BLACK, (-3, -hw + (k + 0.5) * hw * 0.2, 22), (2, hw * 0.2, 18))
        m.box('#4a4e58', (L * 0.3, 0, bh + 30), (260, 200, 60))
        m.cyl('#4a4e58', (L * 0.3, 0, bh + 40), 90, 60, steps=18)
        for y in (-40, 40):
            m.cyl(BLACK, (L * 0.3 - 40, y, bh + 70), 12, 300, rot=(90, 0, 0), steps=10)
        for y in (-70, 70):
            m.box('#ff2244', (L * 0.3 - 92, y, bh + 76), (4, 30, 10), glow=1.5)
        for s in (-1, 1):
            m.cyl(CHROME, (L * 0.84, s * (hw - 10), 200), 12, 160, steps=12)
    else:
        m.box(YEL, (L * 0.9, 0, 222), (30, 120, 14), glow=0.6)


def sedan(m, col, kind='sedan'):
    W = {'sedan': 28, 'hatch': 26, 'van': 32}[kind] * U
    if kind == 'sedan':
        car(m, W, 460, col, cab=0.42, cab_x=(0.25, 0.68), body_h=84, cab_h=138, wedge=False, accent=WHITE)
    elif kind == 'hatch':
        car(m, W, 390, col, cab=0.44, cab_x=(0.06, 0.62), body_h=82, cab_h=140, wedge=False, accent=WHITE)
        m.box(GLASS, (2, 0, 112), (4, W * 0.62, 40))
    else:
        car(m, W, 520, col, cab=0.46, cab_x=(0.02, 0.78), body_h=96, cab_h=200, wedge=False, wheel_r=36)
        m.box(col, (0, 0, 150), (4, W * 0.8, 90))
        m.box(GLASS, (-2, 0, 166), (2, W * 0.66, 44))
        m.box('#ffd23f', (240, 0, 186), (400, W * 0.92 + 4, 12))
        m.box('#ff8a3d', (240, 0, 160), (400, W * 0.92 + 4, 12))


def bus(m):
    W, L = 36 * U, 1100
    hw = W / 2 - 10
    m.box('#ffd23f', (L / 2, 0, 170), (L, hw * 2, 280))
    for k in range(9):
        m.box(GLASS, (60 + k * 110, 0, 230), (86, hw * 2 + 3, 70))
    m.box(GLASS, (-2, 0, 230), (3, hw * 1.6, 90))
    m.box(BLACK, (L / 2, 0, 160), (L + 2, hw * 2 + 2, 6))
    m.box(BLACK, (-2, 0, 110), (3, hw * 2, 30))
    for s in (-1, 1):
        m.box('#ff3355', (-4, s * (hw - 18), 80), (2, 18, 30), glow=1.0)
        for x in (0.16, 0.8):
            wheel(m, L * x, 48, 48, hw - 40, W / 2.0, s)


def ramp(m):
    """The car carrier with its tail ramp down: a yellow cab, a steel deck and a striped ramp."""
    W, L = 42 * U, 1130
    hw = W / 2 - 8
    m.side('#ffb020', [(L * 0.9, 40), (L, 40), (L, 260), (L * 0.93, 300), (L * 0.9, 300)], -hw, hw)
    m.box(STEEL, (L * 0.57, 0, 115), (L * 0.66, hw * 2, 40))
    m.side('#ffd23f', [(0, 0), (L * 0.24, 135), (L * 0.24, 95), (L * 0.02, 0)], -hw + 4, hw - 4)
    for k in range(6):
        t = (k + 0.5) / 6.0
        m.side(BLACK, [(L * 0.24 * (t - 0.03), 135 * (t - 0.03) + 0.5), (L * 0.24 * (t + 0.03), 135 * (t + 0.03) + 0.5), (L * 0.24 * (t + 0.03), 135 * (t + 0.03) + 4), (L * 0.24 * (t - 0.03), 135 * (t - 0.03) + 4)], -hw + 4, hw - 4)
    for s in (-1, 1):
        m.box(PINK, (L * 0.57, s * hw, 140), (L * 0.66, 6, 10), glow=0.4)
        for x in (0.3, 0.42, 0.85):
            wheel(m, L * x, 46, 46, hw - 40, W / 2.0, s)


def moto(m):
    from ft_bikes import moto as build_moto
    build_moto(m)
