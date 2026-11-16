"""Ice Mountain, Stage 1, builder s1d4: district 4 "The Abominable's glacier" (8.94 to 11.6 km), the expedition camp.
/Game/World/IceMountain/SM_S1D4_*. Run: import ft_world_ice_s1d4 as K; K.run()

A polar expedition on the ice: geodesic domes, barrel-vault huts on sled runners, ridge tents with bell ends, a balloon
shed with nothing in it, crates, drums, sledges, flags on bamboo; and the hero, the ICEBOUND SHIP. One storey, round or
vaulted, nothing gabled. Yellow leads; navy, steel, dark timber and asphalt blue carry it; red is for flags (and the
ship's waterline). No figure, no animal, nothing pale that stands upright, no smoke puffs, nothing that glows by the
road (lit windows only on the buildings, which stand 61 m out and more, and on the ship, 150 m out).

How the round things are made without a single rotated profile: a barrel vault is a row of flat slabs laid from point
to point of its arc (tilt_box), its end wall a disc (rod) whose lower half is underground or behind a skirt; a dome is
three cone frusta with a frame of struts standing just off the skin; a tent's bell end is a cone whose far half is
inside the tent. The ship is drawn upright in its own frame and the whole frame is heeled and laid along the grade.
"""
import math
import random
import ft_art
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/IceMountain'
YEL, NAVY, STEEL, TIMBER, ASPH, RED = '#ffd23f', '#1d2740', '#9aa4b8', '#6a4c36', '#3d4763', '#ff2d3c'
WOOD, STEEL_D, LIT, SNOW_SH, ICE_D, ICE_DEEP = '#9a6a44', '#5a6478', '#ffe28a', '#cdd6f1', '#7fb0e0', '#4f8fd2'
STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))


def _c(a):
    return math.cos(math.radians(a))


def _s(a):
    return math.sin(math.radians(a))


# ------------------------------------------------------------------------------------------ helpers
def _pad(g, w, d, x=0.0, y=0.0, top=10.0):
    """Packed snow under a building: the hill falls 7 in 100 along the road and a piece stands level."""
    g.box(SNOW_SH, (x, y, (top - 260.0) / 2.0), (w, d, top + 260.0))


def _arc(r, rz, n, a0=0.0, a1=180.0):
    return [(r * _c(a0 + (a1 - a0) * k / float(n)), rz * _s(a0 + (a1 - a0) * k / float(n))) for k in range(n + 1)]


def _vault_x(g, col, x0, x1, yc, r, z0, n=8, thick=10.0, rz=None):
    """A barrel vault whose axis runs along X (the road): n slabs over the arc."""
    pts = _arc(r, rz or r, n)
    xm = (x0 + x1) / 2.0
    for (c0, s0), (c1, s1) in zip(pts, pts[1:]):
        tilt_box(g, col, (xm, yc + c0, z0 + s0), (xm, yc + c1, z0 + s1), x1 - x0, thick)


def _vault_y(g, col, y0, y1, xc, r, z0, n=8, thick=10.0, rz=None):
    """A barrel vault whose axis runs along Y (its open end faces the road)."""
    pts = _arc(r, rz or r, n)
    ym = (y0 + y1) / 2.0
    for (c0, s0), (c1, s1) in zip(pts, pts[1:]):
        tilt_box(g, col, (xc + c0, ym, z0 + s0), (xc + c1, ym, z0 + s1), y1 - y0, thick)


def _dome(g, col, x, y, z0, R, H, steps=10, frame=None, zig=2, angs=(0.0, 32.0, 60.0, 80.0)):
    """A geodesic dome: three frusta, a ring at every joint and a zigzag of struts standing just off the skin."""
    for a, b in zip(angs, angs[1:]):
        g.cyl(col, (x, y, z0 + H * _s(a)), R * _c(a), H * (_s(b) - _s(a)), steps=steps, top_r=R * _c(b))
    if not frame:
        return
    for a in angs:
        g.cyl(frame, (x, y, z0 + H * _s(a) - 5.0), R * _c(a) + 6.0, 10.0, steps=steps)
    for t in range(zig):
        a, b = angs[t], angs[t + 1]
        ra, rb, za, zb = R * _c(a) + 5.0, R * _c(b) + 5.0, z0 + H * _s(a), z0 + H * _s(b)
        for j in range(steps):
            f0 = 360.0 * j / steps + (180.0 / steps) * t
            f1 = f0 + 180.0 / steps
            f2 = f0 + 360.0 / steps
            lo0 = (x + ra * _c(f0), y + ra * _s(f0), za)
            up = (x + rb * _c(f1), y + rb * _s(f1), zb)
            lo1 = (x + ra * _c(f2), y + ra * _s(f2), za)
            tilt_box(g, frame, lo0, up, 9.0, 9.0)
            tilt_box(g, frame, up, lo1, 9.0, 9.0)


def _porthole(g, x, y, z, yaw, w=100.0, h=96.0, glow=1.0):
    """A window in a box frame, standing out of a curved wall. yaw: the frame's turn (its depth lies along its local Y)."""
    g.box(NAVY, (x, y, z), (w, 36.0, h), rot=(0, yaw, 0))
    g.box(LIT, (x, y, z), (w * 0.66, 42.0, h * 0.64), rot=(0, yaw, 0), glow=glow)


def _drum(g, col, x, y, z=0.0, r=29.0, h=88.0, band=STEEL):
    g.cyl(col, (x, y, z), r, h, steps=8)
    g.cyl(band, (x, y, z + h * 0.6), r + 1.5, 6.0, steps=8)


def _crate(g, col, x, y, z, w=100.0, d=80.0, h=64.0, yaw=0.0, mark=None, strap=ASPH):
    g.box(col, (x, y, z + h / 2.0), (w, d, h), rot=(0, yaw, 0))
    if strap:
        g.box(strap, (x, y, z + h / 2.0), (8.0, d + 3.0, h + 3.0), rot=(0, yaw, 0))
    if mark:    # a stencilled diamond on the front: a shape, never a word
        g.box(mark, (x + w * 0.27, y - d / 2.0 - 1.0, z + h / 2.0), (h * 0.3, 4.0, h * 0.3), rot=(45, yaw, 0))


def _skis(g, x, y, z=0.0, h=185.0, cols=(STEEL, NAVY)):
    """A pair of skis planted crosswise in the snow."""
    tilt_box(g, cols[0], (x - 20.0, y, z - 20.0), (x + 14.0, y, z + h), 4.0, 9.0)
    tilt_box(g, cols[1], (x + 20.0, y + 5.0, z - 20.0), (x - 14.0, y + 5.0, z + h), 4.0, 9.0)


def _flag(sc, x, y, z0, h, w=90.0, fh=54.0, col=RED, pole=STEEL, r=4.0, rate=0.7, swing=16.0, phase=0.0):
    """A flag on a pole: the cloth turns about the pole (red is this district's flag colour, and only that)."""
    sc.still().cyl(pole, (x, y, z0), r, h, steps=6)
    f = sc.part(P.motion(swing=swing, pivot=(x, y, z0 + h - fh / 2.0), axis=P.UP, rate=rate, phase=phase))
    f.box(col, (x + r + w / 2.0, y, z0 + h - fh / 2.0 - 4.0), (w, 4.0, fh))
    f.box(col, (x + r + w * 1.16, y, z0 + h - fh / 2.0 - 4.0), (w * 0.34, 4.0, fh * 0.56))


def _sledge(g, x, y, z=0.0, yaw=0.0, load=True):
    """A man-hauled sledge, 3.4 m, its nose towards +X at yaw 0: runners with upturned tips, a slatted deck, a handle."""
    cy, sy = _c(yaw), _s(yaw)

    def W(px, py, pz):
        return (x + px * cy - py * sy, y + px * sy + py * cy, z + pz)

    rot = (0, yaw, 0)
    for s in (-1, 1):
        g.box(TIMBER, W(0, s * 36, 5), (300, 8, 10), rot=rot)
        tilt_box(g, TIMBER, W(148, s * 36, 5), W(186, s * 36, 42), 8, 10)
        tilt_box(g, TIMBER, W(-150, s * 36, 8), W(-186, s * 36, 104), 7, 7)
        for px in (-105, 105):
            g.box(TIMBER, W(px, s * 36, 20), (7, 7, 24), rot=rot)
    g.box(WOOD, W(0, 0, 34), (292, 80, 6), rot=rot)
    g.box(TIMBER, W(-186, 0, 104), (7, 80, 7), rot=rot)
    if load:
        g.box(YEL, W(-12, 0, 64), (176, 70, 54), rot=rot)            # the tent and the food under a yellow cover
        g.box(NAVY, W(104, 0, 54), (52, 64, 34), rot=rot)            # a kit box
        g.box(TIMBER, W(-122, 0, 58), (40, 66, 42), rot=rot)         # the paraffin tin's crate
        for px in (-60, 30):
            g.box(ASPH, W(px, 0, 64), (8, 74, 58), rot=rot)          # lashings
        g.box(STEEL, W(0, -15, 94), (250, 9, 4), rot=rot)            # skis and a pole on top
        g.box(NAVY, W(0, 12, 94), (250, 9, 4), rot=rot)


# ------------------------------------------------------------------------------------------ the buildings
def big_dome(sc):
    """BUILDING. The big dome: 13 m across, 6.9 m to the vent, yellow panels in a navy frame, a navy door tunnel."""
    g = sc.still()
    _pad(g, 1440, 1440)
    g.cyl(TIMBER, (0, 0, -20), 700, 50, steps=10)                              # the deck it stands on
    _dome(g, YEL, 0, 0, 30, 650, 640, steps=10, frame=NAVY, zig=2)
    zt = 30 + 640 * _s(80)
    g.cyl(STEEL, (0, 0, zt), 70, 46, steps=8)                                  # the vent
    g.cyl(NAVY, (0, 0, zt + 46), 100, 16, steps=8, top_r=36)
    _vault_y(g, NAVY, -900, -560, 0, 150, 30, n=6, thick=10, rz=200)           # the door tunnel
    g.side(ASPH, [(c, 30 + s) for c, s in _arc(146, 196, 6)], -896, -888)
    g.box(YEL, (0, -899, 150), (56, 6, 56))
    g.box(LIT, (0, -901, 150), (38, 6, 38), glow=1.2)
    g.box(STEEL, (56, -899, 104), (8, 6, 24))
    g.box(TIMBER, (0, -965, 6), (250, 120, 20))
    g.box(TIMBER, (0, -935, 22), (220, 60, 14))
    for f in (-140, -40, 0, 180, 40, 140):
        _porthole(g, 628 * _c(f), 628 * _s(f), 206, f + 90)
    _crate(g, TIMBER, 330, -770, 0)
    _crate(g, NAVY, 448, -700, 0, yaw=22, mark=YEL)
    _crate(g, WOOD, 345, -765, 64, w=78, d=66, h=50, yaw=-10, mark=YEL)
    _drum(g, NAVY, -330, -750)
    _drum(g, YEL, -404, -700)
    _skis(g, -540, -560)
    _flag(sc, 0, 0, zt + 56, 300, w=110, fh=64, rate=0.6, swing=18)


def store_dome(sc):
    """BUILDING. The store dome: 7 m across, navy panels in a yellow frame, a ladder, a row of drums, a yellow porch."""
    g = sc.still()
    _pad(g, 900, 860)
    g.cyl(TIMBER, (0, 0, -20), 392, 40, steps=8)
    _dome(g, NAVY, 0, 0, 20, 360, 400, steps=8, frame=YEL, zig=1)
    zt = 20 + 400 * _s(80)
    g.cyl(STEEL, (0, 0, zt), 44, 34, steps=8)
    g.cyl(YEL, (0, 0, zt + 34), 66, 14, steps=8, top_r=24)
    _vault_y(g, YEL, -520, -300, 0, 95, 20, n=6, thick=8, rz=165)
    g.side(NAVY, [(c, 20 + s) for c, s in _arc(91, 161, 6)], -517, -510)
    g.box(LIT, (0, -519, 120), (30, 6, 30), glow=1.2)
    rod(g, STEEL, (150, 120, 330), (196, 156, 540), 11, steps=6)               # the stove pipe, canted
    g.cyl(NAVY, (196, 156, 536), 22, 14, steps=6, top_r=8)
    for sy in (-1, 1):                                                          # a ladder up the east side
        tilt_box(g, TIMBER, (372, sy * 26, -10), (196, sy * 26, 330), 7, 7)
    for k in range(5):
        t = 0.14 + k * 0.18
        g.box(TIMBER, (372 - 176 * t, 0, -10 + 340 * t), (7, 56, 7))
    for k, col in enumerate((YEL, NAVY, STEEL, YEL)):                           # the drums, never orange
        _drum(g, col, -440, -130 + k * 66)
    _crate(g, TIMBER, 300, -420, 0, mark=YEL)
    _crate(g, WOOD, 290, -410, 64, w=80, d=64, h=52, yaw=14)
    for f in (-150, -30, 180):
        _porthole(g, 350 * _c(f), 350 * _s(f), 124, f + 90, w=74, h=70)
    _flag(sc, 0, 0, zt + 46, 190, w=76, fh=46, rate=0.8, swing=18, phase=0.4)


def vault_hut(sc):
    """BUILDING. The vault hut on runners: 8.4 m, a yellow barrel vault with steel ribs, navy ends, a tow bar."""
    g = sc.still()
    _pad(g, 980, 520)
    for sy in (-1, 1):                                                          # the runners, tips turned up
        g.box(TIMBER, (0, sy * 150, 4), (900, 24, 48))
        tilt_box(g, TIMBER, (446, sy * 150, 6), (520, sy * 150, 66), 24, 22)
    for x in (-330, 0, 330):
        g.box(TIMBER, (x, 0, 34), (22, 330, 16))
    g.box(WOOD, (0, 0, 49), (840, 424, 14))                                    # the floor
    _vault_x(g, YEL, -420, 420, 0, 205, 55, n=8, thick=10)
    for x in (-280, 0, 280):
        _vault_x(g, STEEL, x - 8, x + 8, 0, 212, 55, n=8, thick=8)
    for sx in (-1, 1):                                                          # end walls: a disc, a skirt under it
        rod(g, NAVY, (sx * 410, 0, 55), (sx * 420, 0, 55), 197, steps=16)
        g.box(TIMBER, (sx * 424, 0, 28), (12, 424, 62))
    g.box(YEL, (-428, 0, 148), (8, 92, 184))                                   # the door, uphill end
    g.box(LIT, (-433, 0, 196), (6, 40, 40), glow=1.2)
    g.box(TIMBER, (-470, 0, 26), (70, 110, 14))
    g.box(TIMBER, (-520, 0, 8), (60, 110, 14))
    for x in (-140, 140):                                                       # dormer windows to the road
        g.box(NAVY, (x, -172, 172), (84, 44, 72))
        g.box(LIT, (x, -176, 172), (58, 44, 46), glow=1.0)
    g.cyl(STEEL, (200, 40, 250), 12, 120, steps=6)                             # the stove pipe
    g.cyl(NAVY, (200, 40, 370), 24, 14, steps=6, top_r=8)
    tilt_box(g, TIMBER, (430, 0, 30), (556, 0, 8), 12, 10)                     # the tow bar and its shackle
    g.box(STEEL, (556, 0, 10), (16, 44, 12))
    _drum(g, NAVY, -470, -150)
    _drum(g, YEL, -470, 150)
    _crate(g, TIMBER, 300, -280, 0, w=90, d=70, h=56, mark=YEL)
    _skis(g, 380, -250)
    _flag(sc, 380, 0, 255, 150, w=64, fh=40, rate=0.9, swing=18, phase=0.2)


def radio_hut(sc):
    """BUILDING. The radio hut: a small vault hut and a 17 m lattice mast on four stays, aerials and a pennant on top."""
    g = sc.still()
    _pad(g, 600, 440, x=-180)
    _pad(g, 240, 240, x=150)
    for sy in (-1, 1):
        g.box(TIMBER, (-180, sy * 130, 6), (520, 26, 40))
    _vault_x(g, YEL, -430, 70, 0, 170, 22, n=8, thick=10)
    for x in (-300, -60):
        _vault_x(g, STEEL, x - 8, x + 8, 0, 177, 22, n=8, thick=8)
    for x in (-425, 65):
        rod(g, NAVY, (x - 5, 0, 22), (x + 5, 0, 22), 163, steps=16)
    g.box(YEL, (-437, 0, 104), (8, 84, 164))
    g.box(LIT, (-442, 0, 150), (6, 36, 36), glow=1.2)
    g.box(TIMBER, (-470, 0, 10), (60, 100, 16))
    g.box(NAVY, (-180, -142, 140), (84, 44, 68))
    g.box(LIT, (-180, -146, 140), (58, 44, 44), glow=1.0)
    g.cyl(STEEL, (-330, 30, 190), 11, 110, steps=6)
    g.cyl(NAVY, (-330, 30, 300), 22, 12, steps=6, top_r=8)
    # the mast: four legs drawing together, a ring every 2.4 m, a zigzag on the two faces the road sees
    mx, top = 150.0, 1700.0

    def hw(z):
        return 55.0 - 45.0 * z / top

    for sx in (-1, 1):
        for sy in (-1, 1):
            tilt_box(g, STEEL, (mx + sx * 55, sy * 55, -30), (mx + sx * 10, sy * 10, top), 7, 7)
            tilt_box(g, ASPH, (mx + sx * 14, sy * 14, 1150), (mx + sx * 330, sy * 330, -10), 6, 6)   # stays
    z = 240.0
    k = 0
    while z < top - 10:
        h0, h1 = hw(z), hw(min(z + 240.0, top))
        for s in (-1, 1):
            g.box(STEEL, (mx, s * h0, z), (2 * h0, 6, 6))
            g.box(STEEL, (mx + s * h0, 0, z), (6, 2 * h0, 6))
            d = 1 if k % 2 else -1
            tilt_box(g, STEEL_D, (mx - d * h0, s * h0, z), (mx + d * h1, s * h1, min(z + 240.0, top)), 6, 6)
        z += 240.0
        k += 1
    for zz, ln in ((1500, 170), (1590, 136), (1680, 100)):                      # the aerial's elements
        g.box(NAVY, (mx, 0, zz), (ln, 6, 6))
    tilt_box(g, ASPH, (mx - 30, 0, 620), (40, 0, 330), 6, 6)                   # the feeder, slack
    tilt_box(g, ASPH, (40, 0, 330), (30, 0, 190), 6, 6)
    _crate(g, NAVY, 290, -130, 0, w=84, d=64, h=54, mark=YEL)                  # the batteries
    _crate(g, TIMBER, 300, 110, 0, w=90, d=70, h=58)
    g.box(STEEL, (40, -150, 30), (90, 56, 64))                                 # the hand generator
    g.box(NAVY, (40, -150, 66), (96, 62, 10))
    tilt_box(g, STEEL_D, (86, -150, 40), (120, -150, 78), 6, 6)
    _flag(sc, mx, 0, top, 230, w=120, fh=42, r=4.0, rate=0.8, swing=20, phase=0.6)


def mess_tent(sc):
    """BUILDING. The mess tent: 13 m, a yellow ridge tent with bell ends (no gable), a navy porch, a stove pipe, guys."""
    g = sc.still()
    _pad(g, 1380, 620)
    for sy in (-1, 1):
        g.box(YEL, (0, sy * 250, 35), (800, 8, 110))
        tilt_box(g, YEL, (0, sy * 258, 86), (0, 0, 352), 800, 8)
        g.box(NAVY, (0, sy * 254, 4), (800, 12, 26))                            # the sod cloth
    for sx in (-1, 1):                                                          # the bell ends
        g.cyl(YEL, (sx * 400, 0, -20), 252, 108, steps=10)
        g.cyl(YEL, (sx * 400, 0, 88), 258, 264, steps=10, top_r=6)
        g.cyl(NAVY, (sx * 400, 0, -8), 256, 26, steps=10)
        g.cyl(NAVY, (sx * 400, 0, 346), 9, 46, steps=6)
    g.box(NAVY, (0, 0, 354), (810, 16, 10))                                    # the ridge
    _vault_y(g, NAVY, -420, -236, 120, 85, 0, n=6, thick=8, rz=185)            # the porch
    g.side(YEL, [(120 + c, s) for c, s in _arc(80, 178, 6)], -424, -418)
    g.box(NAVY, (120, -426, 80), (10, 4, 150))
    for x in (-310, -160, -20, 300):
        g.box(LIT, (x, -255, 52), (104, 6, 38), glow=1.0)
    g.cyl(STEEL, (-250, 60, 286), 13, 190, steps=6)                            # the galley's pipe
    g.cyl(NAVY, (-250, 60, 476), 26, 14, steps=6, top_r=8)
    for x in (-330, -110):
        tilt_box(g, ASPH, (x, -258, 88), (x, -392, -12), 6, 6)
    for x in (-330, -110, 110, 330):
        tilt_box(g, ASPH, (x, 258, 88), (x, 392, -12), 6, 6)
    g.box(WOOD, (-220, -470, 78), (200, 70, 7))                                # the serving table and two pots
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(TIMBER, (-220 + sx * 88, -470 + sy * 26, 36), (7, 7, 84))
    g.cyl(STEEL, (-260, -470, 82), 20, 28, steps=8)
    g.cyl(STEEL_D, (-190, -470, 82), 15, 20, steps=8)
    for sx in (-1, 1):                                                          # a striped fly over it on four poles
        g.box(TIMBER, (-220 + sx * 120, -410, 100), (6, 6, 230))
        g.box(TIMBER, (-220 + sx * 120, -530, 88), (6, 6, 206))
    for k in range(5):
        tilt_box(g, YEL if k % 2 == 0 else NAVY, (-320 + k * 50, -400, 216), (-320 + k * 50, -540, 192), 50, 5)
    for k in range(4):                                                          # bunting down from the mast, red
        t = 0.2 + k * 0.2
        g.box(RED, (400 * t, 0, 590 - 210 * t - 14), (26, 4, 30))
        g.box(RED, (-400 * t, 0, 590 - 210 * t - 14), (26, 4, 30))
    tilt_box(g, ASPH, (0, 0, 592), (400, 0, 382), 5, 5)
    tilt_box(g, ASPH, (0, 0, 592), (-400, 0, 382), 5, 5)
    _crate(g, TIMBER, 330, -390, 0, mark=YEL)
    _crate(g, NAVY, 440, -380, 0, w=84, d=70, h=56, yaw=18)
    _crate(g, WOOD, 340, -388, 64, w=76, d=62, h=46, yaw=-12)
    _flag(sc, 0, 0, 352, 250, w=100, fh=58, rate=0.6, swing=18, phase=0.1)


def sledge_garage(sc):
    """BUILDING. The sledge garage: an open-fronted yellow vault 9 m wide, two sledges nosing out, a bench, a ski rack."""
    g = sc.still()
    _pad(g, 1120, 640)
    g.box(WOOD, (0, 0, 2), (880, 524, 12))
    for sx in (-1, 1):
        g.box(TIMBER, (sx * 450, 0, -110), (44, 548, 270))                      # the sills, footed
    _vault_y(g, YEL, -260, 260, 0, 450, 20, n=10, thick=12, rz=380)
    _vault_y(g, NAVY, -276, -256, 0, 458, 20, n=10, thick=18, rz=388)          # the front arch
    g.side(NAVY, [(c, 20 + s) for c, s in _arc(444, 374, 10)], 250, 262)       # the back wall
    _sledge(g, -200, -150, 8, yaw=-90, load=False)
    _sledge(g, 150, -60, 8, yaw=-78, load=True)
    g.box(TIMBER, (0, 214, 80), (360, 60, 10))                                 # the bench at the back
    for x in (-160, 160):
        g.box(TIMBER, (x, 214, 42), (12, 54, 76))
    g.box(STEEL, (-110, 204, 98), (40, 30, 26))                                # a vice, a tin
    g.cyl(YEL, (90, 210, 85), 16, 30, steps=6)
    rod(g, WOOD, (300, 246, 200), (300, 232, 200), 44, steps=8)                # a coil of rope on the wall
    for k in range(2):                                                          # spare runners leaning
        tilt_box(g, TIMBER, (-330 - k * 26, 236, 8), (-300 - k * 26, 244, 250), 8, 10)
    g.box(LIT, (0, -262, 372), (36, 22, 30), glow=1.5)                         # the lamp under the arch, steady
    g.box(NAVY, (0, -262, 392), (46, 30, 10))
    g.box(TIMBER, (565, -290, 110), (200, 10, 10))                             # the ski rack by the east sill
    for x in (475, 655):
        g.box(TIMBER, (x, -290, 55), (10, 10, 130))
    for k, col in enumerate((STEEL, NAVY, YEL, STEEL, NAVY)):
        tilt_box(g, col, (493 + k * 36, -312, -10), (493 + k * 36, -297, 190), 9, 4)
    _drum(g, NAVY, -520, -210)
    _crate(g, TIMBER, -530, -90, 0, w=80, d=80, h=56, mark=YEL)
    g.cyl(STEEL, (-240, 120, 330), 12, 150, steps=6)
    g.cyl(NAVY, (-240, 120, 480), 24, 14, steps=6, top_r=8)
    _flag(sc, 0, -266, 400, 200, w=84, fh=50, rate=0.7, swing=18, phase=0.3)
    g.cyl(STEEL, (240, 0, 330), 5, 150, steps=6)                               # a wind vane on the roof: an arrow that hunts
    v = sc.part(P.motion(swing=24, pivot=(240, 0, 470), axis=P.UP, rate=0.25, phase=0.7))
    v.box(NAVY, (240, 0, 470), (150, 6, 8))
    v.box(YEL, (176, 0, 470), (44, 5, 40))
    v.box(NAVY, (322, 0, 470), (20, 6, 22), rot=(45, 0, 0))


def balloon_shed(sc):
    """BUILDING. The balloon shed: a tall narrow vault, 6.4 m wide and 10.5 m high, its doors ajar on a dark, empty
    floor; moored with four ropes; a basket and sandbags by the door. No balloon."""
    g = sc.still()
    _pad(g, 840, 1040)
    g.box(TIMBER, (0, 0, -100), (700, 930, 250))                               # the plinth
    _vault_y(g, YEL, -450, 450, 0, 320, 0, n=10, thick=12, rz=1050)
    for y in (-150, 150):
        _vault_y(g, STEEL, y - 9, y + 9, 0, 328, 0, n=10, thick=8, rz=1058)
    _vault_y(g, NAVY, -466, -446, 0, 330, 0, n=10, thick=18, rz=1060)
    g.side(YEL, [(c, s) for c, s in _arc(314, 1044, 10)], -448, -440)          # the front wall
    g.side(NAVY, [(c, s) for c, s in _arc(314, 1044, 10)], 440, 452)           # the back wall
    g.box(NAVY, (0, -450, 352), (462, 8, 704))                                 # the doorway: dark, nothing inside
    for sx in (-1, 1):                                                          # the leaves, 32 degrees open
        cx, cy = sx * (236 - 116 * _c(32)), -452 - 116 * _s(32)
        g.box(YEL, (cx, cy, 352), (232, 10, 700), rot=(0, sx * 32, 0))
        for zz in (120, 350, 580):
            g.box(NAVY, (cx, cy, zz), (236, 16, 22), rot=(0, sx * 32, 0))
    g.box(LIT, (0, -452, 800), (70, 8, 70), glow=1.0)                          # a round-ish light over the door
    g.box(NAVY, (0, -451, 800), (96, 6, 96))
    for sx in (-1, 1):                                                          # mooring ropes to pegs
        for sy in (-1, 1):
            tilt_box(g, ASPH, (sx * 276, sy * 300, 540), (sx * 400, sy * 500, -10), 7, 7)
            g.box(TIMBER, (sx * 400, sy * 500, 6), (12, 12, 40))
    g.box(WOOD, (-330, -600, 46), (110, 90, 92))                               # the basket, empty, and its rope
    g.box(TIMBER, (-330, -600, 96), (120, 100, 10))
    rod(g, WOOD, (-200, -620, 0), (-200, -620, 22), 40, steps=8)
    for k, (x, y, zz) in enumerate(((300, -600, 0), (360, -590, 0), (330, -640, 0), (332, -604, 24), (300, -560, 0))):   # sandbags
        g.box(TIMBER if k % 2 else WOOD, (x, y, zz + 12), (56, 36, 24), rot=(0, 20 * k, 0))
    g.box(STEEL, (120, -640, 40), (12, 70, 80))                                # the hand winch
    g.box(STEEL, (200, -640, 40), (12, 70, 80))
    rod(g, TIMBER, (126, -640, 62), (194, -640, 62), 20, steps=8)
    tilt_box(g, STEEL_D, (206, -640, 62), (240, -640, 96), 6, 6)
    # a long streamer on the roof mast: the wind's gauge, in two lengths
    g.cyl(STEEL, (0, 0, 1040), 5, 250, steps=6)
    for k in range(2):
        f = sc.part(P.motion(swing=14 + 10 * k, pivot=(0, 0, 1260), axis=P.UP, rate=0.7, phase=0.15 * k))
        f.box(RED, (95 + 170 * k, 0, 1262 - 6 * k), (176, 4, 44 - 12 * k))


def bunk_tents(sc):
    """BUILDING. The bunk tents: three two-man ridge tents with bell ends, skis planted by each, the camp flag between."""
    g = sc.still()
    _pad(g, 1120, 380, top=4.0)
    cols = ((YEL, NAVY), (NAVY, YEL), (YEL, ASPH))
    for k, x0 in enumerate((-400, 0, 400)):
        col, door = cols[k]
        for sy in (-1, 1):
            tilt_box(g, col, (x0, sy * 98, -14), (x0, 0, 150), 200, 6)
            for sx in (-1, 1):
                tilt_box(g, ASPH, (x0 + sx * 78, sy * 52, 70), (x0 + sx * 112, sy * 170, -12), 5, 5)
        g.cyl(col, (x0 - 100, 0, -14), 98, 164, steps=8, top_r=4)
        g.cyl(door, (x0 + 100, 0, -14), 98, 164, steps=8, top_r=4)
        g.box(door, (x0, 0, 152), (214, 10, 8))
        _skis(g, x0 - 160, -150 + 40 * k, h=178)
        g.box(NAVY if k != 1 else TIMBER, (x0 + 150, -150, 22), (44, 34, 44))   # a pack by the door
    g.box(STEEL, (-200, -170, 16), (44, 36, 32))                               # the primus and a pot
    g.cyl(STEEL_D, (-200, -170, 32), 15, 18, steps=6)
    g.box(TIMBER, (200, -170, 10), (120, 40, 20))                              # a ration box
    _flag(sc, 200, 140, -30, 370, w=96, fh=56, pole=TIMBER, r=4.0, rate=0.7, swing=18, phase=0.5)


# ------------------------------------------------------------------------------------------ the props
def route_flags(sc):
    """PROP, LOW (1.1 m). A line of route flags on bamboo: four wands, each a red flag that flutters about its wand."""
    g = sc.still()
    r = random.Random(41)
    for k, x in enumerate((-240, -80, 80, 240)):
        dx, dy = r.uniform(-7, 7), r.uniform(-5, 5)
        rod(g, TIMBER, (x, 0, -30), (x + dx, dy, 110), 3.0, steps=5)
        g.cyl(SNOW_SH, (x, 0, -6), 26, 12, steps=6, top_r=8)
        f = sc.part(P.motion(swing=15, pivot=(x + dx, dy, 96), axis=P.UP, rate=0.9, phase=0.23 * k))
        f.box(RED, (x + dx + 19, dy, 96), (34, 4, 22))


def crate_stack(sc):
    """PROP, LOW (1.9 m). A crate stack, timber and navy, lashed, stencilled with diamonds; a tarpaulin, a shovel."""
    g = sc.still()
    g.box(TIMBER, (0, 0, -10), (330, 110, 44))                                 # the pallet, footed
    _crate(g, TIMBER, -106, 0, 12, w=100, d=84, h=64, mark=YEL)
    _crate(g, NAVY, 0, 4, 12, w=104, d=84, h=64, mark=YEL)
    _crate(g, WOOD, 108, -2, 12, w=100, d=84, h=64)
    _crate(g, NAVY, -54, 2, 76, w=100, d=80, h=62, yaw=6)
    _crate(g, TIMBER, 56, 0, 76, w=100, d=80, h=62, yaw=-5, mark=YEL)
    _crate(g, WOOD, -4, 0, 138, w=84, d=70, h=48, yaw=9, mark=NAVY)
    tilt_box(g, YEL, (122, 0, 80), (166, 0, 6), 92, 6)                         # the tarpaulin, hanging off one end
    g.box(YEL, (112, 0, 79), (96, 90, 6))
    tilt_box(g, TIMBER, (-176, 30, -8), (-160, 30, 118), 5, 5)                 # a shovel leaning
    g.box(STEEL, (-178, 30, 6), (8, 26, 34))


def loaded_sledge(sc):
    """PROP, LOW (1.1 m). A sledge loaded and lashed: yellow cover, kit box, skis on top; its traces lie on the snow."""
    g = sc.still()
    _sledge(g, -40, 0, 0, yaw=0, load=True)
    for sy in (-1, 1):                                                          # the traces and the harness bar
        tilt_box(g, ASPH, (146, sy * 30, 20), (280, sy * 22, 3), 5, 5)
    g.box(TIMBER, (284, 0, 4), (8, 70, 8))
    g.box(SNOW_SH, (-40, 0, -14), (330, 100, 30))                              # packed snow under the runners


def fuel_dump(sc):
    """PROP, LOW (1.4 m). The fuel dump: six drums in navy, yellow and steel on sleepers, a hand pump, a funnel, two cans."""
    g = sc.still()
    for sy in (-1, 1):
        g.box(TIMBER, (0, sy * 20, -8), (400, 16, 40))
    for k, col in enumerate((NAVY, YEL, NAVY, STEEL, YEL, NAVY)):
        _drum(g, col, -165 + k * 66, 0, 12, band=STEEL if col != STEEL else NAVY)
    g.cyl(STEEL, (-99, 0, 100), 5, 38, steps=6)                                # the pump on the second drum
    tilt_box(g, STEEL_D, (-99, 0, 132), (-72, 0, 118), 5, 5)
    g.cyl(STEEL, (99, 0, 100), 5, 14, steps=6, top_r=17)                       # the funnel on the fifth
    g.box(NAVY, (-176, -44, 22), (34, 18, 46))                                 # cans
    g.box(YEL, (176, -44, 22), (34, 18, 46))
    g.box(STEEL, (-176, -44, 48), (14, 8, 8))
    g.box(STEEL, (176, -44, 48), (14, 8, 8))


def depot_cairn(sc):
    """PROP, TALL (4.3 m). A depot: drums and crates heaped as a cairn round a bamboo with the depot's flag."""
    g = sc.still()
    g.box(SNOW_SH, (0, 0, -16), (230, 170, 36))
    for k, (x, y, col) in enumerate(((-62, -22, NAVY), (62, -22, YEL), (0, 44, STEEL))):
        _drum(g, col, x, y, 0, band=NAVY if col == STEEL else STEEL)
    _crate(g, TIMBER, 0, -50, 0, w=64, d=50, h=60, strap=None, mark=YEL)
    g.box(WOOD, (0, 6, 92), (160, 120, 8))                                     # a board across them
    _drum(g, NAVY, -30, 6, 96, r=27, h=80)
    _crate(g, TIMBER, 42, 0, 96, w=56, d=56, h=50, yaw=12)
    _skis(g, 100, 8, h=180, cols=(STEEL, YEL))
    g.box(NAVY, (-96, -30, 150), (60, 5, 60), rot=(45, 0, 0))                  # the depot's mark: a navy diamond, a yellow bar
    g.box(YEL, (-96, -34, 150), (46, 4, 12))
    g.cyl(TIMBER, (-96, -30, -20), 3.5, 170, steps=5)
    _flag(sc, 0, 24, -20, 450, w=92, fh=56, pole=TIMBER, r=4.0, rate=0.7, swing=18)


def theodolite(sc):
    """PROP, TALL (2.7 m). The surveyor's station: a theodolite on a timber tripod under a yellow umbrella, its case,
    a folding stool, a ranging rod banded yellow and navy."""
    g = sc.still()
    for a in (90, 210, 330):
        tilt_box(g, TIMBER, (58 * _c(a), 58 * _s(a), -20), (0, 0, 132), 7, 7)
    g.cyl(STEEL, (0, 0, 130), 14, 10, steps=8)
    g.box(STEEL, (0, 0, 152), (22, 16, 34))
    rod(g, NAVY, (-22, 0, 160), (24, 0, 164), 7, steps=6)                      # the telescope, along the road
    g.cyl(YEL, (0, 0, 168), 9, 6, steps=6)
    g.box(NAVY, (-70, 44, 14), (60, 34, 30))                                   # the case, a yellow clasp
    g.box(YEL, (-70, 26, 16), (10, 4, 8))
    for sx in (-1, 1):                                                          # the stool
        tilt_box(g, TIMBER, (150 - sx * 20, 40, -8), (150 + sx * 20, 40, 44), 30, 5)
    g.box(ASPH, (150, 40, 46), (50, 34, 5))
    for k in range(6):                                                          # the ranging rod
        g.cyl(YEL if k % 2 else NAVY, (-170, 20, -20 + k * 36), 3.5, 36, steps=5)
    u = sc.part(P.motion(swing=2.5, pivot=(74, 0, 0), axis=0.0, rate=0.3, phase=0.2))   # the umbrella nods
    u.cyl(STEEL, (74, 0, -20), 3.5, 262, steps=5)
    u.cyl(YEL, (74, 0, 222), 96, 34, steps=8, top_r=6)
    u.cyl(NAVY, (74, 0, 256), 6, 12, steps=5)
    u.cyl(NAVY, (74, 0, 218), 98, 5, steps=8)


def signal_mast(sc):
    """PROP, TALL (9.2 m). The signal mast: a steel mast on sleepers, a yard with cones and squares hoisted (no balls),
    two stays along the road, a red pennant at the truck."""
    g = sc.still()
    g.box(TIMBER, (0, 0, -6), (220, 26, 30))
    g.box(TIMBER, (0, 0, 12), (26, 84, 22))
    g.cyl(STEEL, (0, 0, -30), 8, 910, steps=6, top_r=5)
    g.cyl(NAVY, (0, 0, 876), 12, 8, steps=6)
    g.box(TIMBER, (0, 0, 640), (320, 10, 10))                                  # the yard
    for sx in (-1, 1):
        tilt_box(g, ASPH, (sx * 6, 0, 600), (sx * 300, 0, -10), 6, 6)          # stays, fore and aft
        g.box(TIMBER, (sx * 300, 0, 4), (12, 12, 36))
        g.box(WOOD if sx > 0 else TIMBER, (sx * 80, 0, 22), (56, 36, 22))      # sandbags on the sleepers
    g.box(STEEL, (0, -9, 120), (10, 8, 30))                                    # the cleat
    for sx, shapes in ((-1, ('cone_up', 'square')), (1, ('square45', 'cone_down'))):
        x = sx * 140.0
        h = sc.part(P.motion(swing=4.0, pivot=(x, 0, 636), axis=0.0, rate=0.35, phase=0.25 + 0.3 * sx))
        h.box(ASPH, (x, 0, 470), (5, 5, 330))                                  # the halyard
        for k, sh in enumerate(shapes):
            zz = 540 - k * 110
            col = NAVY if (k + (sx > 0)) % 2 == 0 else YEL
            if sh == 'cone_up':
                h.cyl(col, (x, 0, zz - 34), 38, 70, steps=8, top_r=3)
            elif sh == 'cone_down':
                h.cyl(col, (x, 0, zz - 34), 3, 70, steps=8, top_r=38)
            elif sh == 'square':
                h.box(col, (x, 0, zz), (62, 8, 62))
            else:
                h.box(col, (x, 0, zz), (54, 8, 54), rot=(45, 0, 0))
    f = sc.part(P.motion(swing=18, pivot=(0, 0, 850), axis=P.UP, rate=0.8, phase=0.1))
    f.box(RED, (76, 0, 850), (140, 4, 30))
    f.box(RED, (176, 0, 850), (60, 4, 16))


def drying_line(sc):
    """PROP, LOW (1.95 m). The drying line between two pairs of planted skis: an anorak, long johns, socks and mitts
    that swing; a wash tub and a pair of boots under it."""
    g = sc.still()
    for sx in (-1, 1):
        _skis(g, sx * 200, 0, h=190, cols=(STEEL, NAVY) if sx < 0 else (NAVY, STEEL))
    g.box(ASPH, (0, 2, 160), (400, 5, 5))                                      # the line
    items = (                                                                   # x on the line, colour, (dx, drop, w, h) each
        (-140, YEL, ((0, 0, 70, 78), (-44, 6, 20, 50), (44, 6, 20, 50))),      # the anorak and its sleeves
        (-52, NAVY, ((0, 0, 46, 22), (-13, 20, 20, 70), (13, 20, 20, 70))),    # long johns
        (20, STEEL, ((-9, 0, 13, 44), (9, 0, 13, 44))),                        # socks
        (84, TIMBER, ((-10, 8, 16, 26), (10, 8, 16, 26), (0, 0, 5, 10))),      # mitts on their string
        (146, ASPH, ((0, 0, 22, 74),)),                                        # a scarf
    )
    for k, (x, col, boxes) in enumerate(items):
        m = sc.part(P.motion(swing=8.0 + (k % 2) * 3.0, pivot=(x, 2, 160), axis=0.0, rate=0.5 + 0.06 * k, phase=0.19 * k))
        for dx, drop, w, h in boxes:
            m.box(col, (x + dx, 2, 158 - drop - h / 2.0), (w, 5, h))
    g.cyl(STEEL, (-60, -4, -6), 34, 30, steps=8)                               # the tub
    g.box(NAVY, (40, 0, 12), (14, 30, 30))                                     # boots
    g.box(NAVY, (62, 0, 12), (14, 30, 30))
    g.box(SNOW_SH, (0, 0, -16), (470, 60, 30))


# ------------------------------------------------------------------------------------------ the hero
SHIP = (-4.0, 0.0, -12.0)      # (pitch, yaw, roll): bow down the hill's grade, heeled 12 degrees towards the road
LIFT = 150.0                   # she was squeezed up as the ice closed: her waterline stands this far out of it
GROUND = (-4.0, 0.0, -6.8)     # the snow slope she lies on, 150 m out (it rises 12 cm a metre away from the road)


def _turn(rot, p):
    """Where a point of a turned frame lies in the mesh (Unreal's rotator, yaw 0)."""
    cp, sp, cr, sr = _c(rot[0]), _s(rot[0]), _c(rot[2]), _s(rot[2])
    x, y, z = p
    return (x * cp + y * sr * sp - z * cr * sp, y * cr + z * sr, x * sp - y * sr * cp + z * cr * cp)


def _ship(p):
    q = _turn(SHIP, p)
    return (q[0], q[1], q[2] + LIFT)


def _poly(pts, sx=1.0, sy=1.0):
    return [(x * sx, y * sy) for x, y in pts]


def icebound_ship(sc):
    """HERO. THE ICEBOUND SHIP: a 60 m expedition ship locked in the glacier and heeled over 12 degrees towards the
    road: dark timber hull, red waterline, yellow funnel, three bare masts with slack rigging, yards askew, one long
    red pennant at the main truck and an ensign aft that still fly; boats under yellow covers, lit ports, a gangway
    down to the ice, floes rafted against her sides."""
    r = random.Random(77)
    g = sc.still()
    # ---- the ice she lies in (the slope's own frame): rafted floes along both sides, low and lying, and drift
    g.base = ft_art._xf((0, 0, 0), GROUND)
    for k in range(16):
        side = -1 if k % 2 else 1
        x = -2700 + (k // 2) * 760 + r.uniform(-120, 120)
        y = side * (620 + r.uniform(0, 150))
        w, d, h = r.uniform(320, 560), r.uniform(200, 340), r.uniform(150, 260)
        g.box(ICE_DEEP if k % 3 == 0 else ICE_D, (x, y, 40 - h / 2.0 + r.uniform(0, 50)), (w, d, h),
              rot=(r.uniform(-12, 12), r.uniform(-30, 30), side * r.uniform(8, 22)))
    for side in (-1, 1):
        g.box(SNOW_SH, (0, side * 800, -45), (5600, 440, 110))
    g.box(SNOW_SH, (0, 0, -110), (6400, 1500, 200))
    # ---- the ship (her own frame: z = 0 is her waterline, +x the bow)
    g.base = ft_art._xf((0, 0, LIFT), SHIP)
    H = [(3000, 0), (2500, 330), (1700, 520), (-1800, 540), (-2600, 430), (-2950, 220),
         (-2950, -220), (-2600, -430), (-1800, -540), (1700, -520), (2500, -330)]
    g.plan(TIMBER, _poly(H, 0.97, 0.86), -620, -150)                           # the bottom
    g.plan(RED, _poly(H, 0.99, 0.97), -150, 110)                                # the red waterline
    g.plan(TIMBER, H, 110, 432)                                                # the topsides
    g.plan(NAVY, _poly(H, 1.004, 1.02), 372, 440)                              # the sheer strake
    g.plan(WOOD, _poly(H, 0.96, 0.92), 440, 448)                               # the deck
    F = [(3000, 0), (2500, 330), (1900, 474), (1900, -474), (2500, -330)]
    g.plan(TIMBER, F, 440, 566)                                                # the forecastle
    g.plan(WOOD, _poly(F, 0.985, 0.9), 566, 574)
    A = [(-1900, 536), (-2600, 430), (-2950, 220), (-2950, -220), (-2600, -430), (-1900, -536)]
    g.plan(TIMBER, A, 440, 580)                                                # the poop
    g.plan(WOOD, _poly(A, 0.99, 0.9), 580, 588)
    g.box(SNOW_SH, (2350, 60, 578), (700, 420, 10))                            # drift on the forecastle and the poop
    g.box(SNOW_SH, (-2500, -60, 592), (620, 500, 10))
    g.box(SNOW_SH, (600, 250, 452), (1500, 300, 10))
    rod(g, TIMBER, (2800, 0, 560), (3700, 0, 830), 17, steps=6)                # the bowsprit
    # ports along the side the road sees: steel rims, three still lit
    for k in range(8):
        x = -1500 + k * 430
        y = -(540 - (x + 1800) / 3500.0 * 20) - 2
        g.box(STEEL, (x, y, 290), (58, 8, 58))
        if k in (1, 4, 5):
            g.box(LIT, (x, y - 3, 290), (40, 8, 40), glow=1.5)
        else:
            g.box(NAVY, (x, y - 3, 290), (40, 8, 40))
    for k in range(4):                                                          # the stern windows
        g.box(LIT if k in (1, 2) else NAVY, (-2954, -150 + k * 100, 500), (8, 60, 50), glow=1.5 if k in (1, 2) else 0.0)
    # the deckhouse, the wheelhouse on it, and the yellow funnel raked aft
    g.box(STEEL, (-700, 0, 558), (760, 430, 222))
    g.box(NAVY, (-700, 0, 676), (800, 470, 16))
    g.box(STEEL, (-480, 0, 780), (300, 300, 190))
    g.box(NAVY, (-480, 0, 882), (340, 340, 16))
    for k in range(5):
        g.box(LIT, (-1000 + k * 150, -217, 580), (70, 8, 60), glow=1.2)
    for k in range(3):
        g.box(LIT, (-560 + k * 80, -152, 800), (56, 8, 64), glow=1.2)
        g.box(LIT, (-328, -80 + k * 80, 800), (8, 56, 64), glow=1.2)
    rod(g, YEL, (-900, 0, 680), (-970, 0, 1240), 96, steps=10)
    rod(g, NAVY, (-962, 0, 1176), (-972, 0, 1256), 101, steps=10)
    rod(g, STEEL, (-1040, 0, 690), (-1096, 0, 1140), 14, steps=6)              # the steam pipe
    # masts: fore, main, mizzen; bare, a top on each, the yards askew
    masts = ((1500, 440, 2500), (200, 440, 2900), (-1750, 580, 2150))
    for k, (x, z0, h) in enumerate(masts):
        g.cyl(TIMBER, (x, 0, z0 - 60), 26, h + 60, steps=8, top_r=11)
        g.cyl(NAVY if k == 1 else TIMBER, (x, 0, z0 + h * 0.5), 62, 64 if k == 1 else 12, steps=8)
        for sy in (-1, 1):                                                      # shrouds to the rail
            for dx in (-230, 230):
                tilt_box(g, ASPH, (x, 0, z0 + h * 0.5), (x + dx, sy * 520, 440), 10, 10)
    for x, z, ln, cock, skew in ((1500, 1500, 1100, 70, 120), (1500, 2200, 800, -50, -90), (1500, 2700, 520, 30, 60),
                                 (200, 1650, 1200, -80, -140), (200, 2450, 900, 60, 100), (200, 3050, 560, -30, -50)):
        rod(g, TIMBER, (x - skew, -ln / 2.0, z - cock), (x + skew, ln / 2.0, z + cock), 12, steps=6)
    rod(g, TIMBER, (-1750, 0, 830), (-2760, 0, 900), 13, steps=6)              # the mizzen's boom and gaff
    rod(g, TIMBER, (-1750, 0, 1980), (-2460, 0, 2420), 11, steps=6)
    stays = (((3700, 0, 830), (1500, 0, 2940)), ((3300, 0, 700), (1500, 0, 2200)),
             ((1500, 0, 2940), (850, 0, 2900)), ((850, 0, 2900), (200, 0, 3340)),            # slack: it sags
             ((200, 0, 3340), (-800, 0, 2750)), ((-800, 0, 2750), (-1750, 0, 2730)),
             ((-1750, 0, 2730), (-2940, 0, 600)))
    for a, b in stays:
        tilt_box(g, ASPH, a, b, 10, 10)
    # boats on davits under yellow covers (two on the road's side, one beyond), an anchor and its cable
    for x, sy in ((520, -1), (-260, -1), (150, 1)):
        g.box(NAVY, (x, sy * 580, 520), (300, 96, 44))
        g.box(YEL, (x, sy * 580, 550), (304, 100, 20))
        for dx in (-120, 120):
            tilt_box(g, STEEL, (x + dx, sy * 500, 440), (x + dx, sy * 520, 640), 12, 12)
            tilt_box(g, STEEL, (x + dx, sy * 520, 640), (x + dx, sy * 584, 610), 12, 12)
    rod(g, NAVY, (2560, -350, 330), (2560, -372, 150), 12, steps=6)
    tilt_box(g, NAVY, (2500, -380, 160), (2620, -380, 160), 14, 14)
    tilt_box(g, STEEL, (2600, -330, 400), (2760, -600, -60), 14, 14)
    # deck cargo: crates, drums, a sledge lashed down
    _crate(g, TIMBER, 900, -250, 448, w=160, d=120, h=100, mark=YEL)
    _crate(g, NAVY, 1080, -240, 448, w=130, d=110, h=90)
    _crate(g, WOOD, 940, -245, 548, w=120, d=100, h=70, yaw=10)
    _crate(g, TIMBER, 850, 260, 448, w=180, d=130, h=110)
    for k, col in enumerate((YEL, NAVY, YEL)):
        _drum(g, col, 1250 + k * 90, 250, 448, r=40, h=110)
    _sledge(g, 500, 0, 450, yaw=4, load=True)
    g.cyl(STEEL, (2300, 0, 574), 60, 70, steps=8)                              # the windlass
    for x, z0, h in masts:                                                      # ratlines on the shrouds
        zt = z0 + h * 0.5
        for sy in (-1, 1):
            for t in (0.2, 0.4, 0.6):
                g.box(ASPH, (x, sy * 520 * (1 - t), 440 + (zt - 440) * t), (460 * (1 - t), 10, 10))
    for x, y in ((1150, 0), (-1350, 150), (-1350, -150)):                       # ventilator cowls, yellow
        g.cyl(YEL, (x, y, 448), 24, 150, steps=6)
        rod(g, YEL, (x, y, 590), (x + 60, y, 640), 30, steps=6)
    g.box(TIMBER, (1150, 300, 462), (260, 200, 28))                            # hatches
    g.box(TIMBER, (-150, 0, 462), (220, 240, 28))
    g.box(NAVY, (-150, 0, 480), (236, 256, 8))
    for sy in (-1, 1):                                                          # the rail amidships and its stanchions
        tilt_box(g, NAVY, (1700, sy * 516, 520), (-1800, sy * 536, 520), 8, 8)
        for k in range(8):
            x = 1700 - k * 500
            g.box(NAVY, (x, sy * (516 + (1700 - x) / 3500.0 * 20), 482), (8, 8, 76))
    g.box(STEEL, (-480, -200, 720), (120, 100, 8))                             # the bridge wings
    g.box(STEEL, (-480, 200, 720), (120, 100, 8))
    g.box(LIT, (-2990, 0, 700), (30, 30, 40), glow=1.5)                        # the stern lantern, steady
    g.box(NAVY, (-2990, 0, 726), (40, 40, 10))
    # ---- the shore party's dump at the gangway's foot, and flags leading off towards the road (the slope's frame)
    g.base = ft_art._xf((0, 0, 0), GROUND)
    _crate(g, TIMBER, -900, -1380, 0, w=150, d=110, h=90, mark=YEL)
    _crate(g, NAVY, -740, -1400, 0, w=130, d=100, h=84)
    _crate(g, WOOD, -860, -1385, 90, w=110, d=90, h=64, yaw=12)
    for k, col in enumerate((YEL, NAVY, STEEL)):
        _drum(g, col, -1560 - k * 86, -1330, 0, r=38, h=104)
    _sledge(g, -1500, -1560, 0, yaw=200, load=True)
    for k in range(4):
        y = -1500 - k * 330
        g.cyl(TIMBER, (-1150 + 40 * k, y, -30), 5, 300, steps=5)
        g.box(RED, (-1150 + 40 * k + 50, y, 238), (92, 5, 56))
    # ---- the gangway down to the ice (drawn in the mesh's own frame between the two)
    g.base = None
    top = _ship((-1200, -548, 450))
    foot = _turn(GROUND, (-1200, -1250, 30))
    tilt_box(g, WOOD, top, foot, 110, 10)
    for sx in (-1, 1):
        a = (top[0] + sx * 52, top[1], top[2] + 90)
        b = (foot[0] + sx * 52, foot[1], foot[2] + 90)
        tilt_box(g, ASPH, a, b, 8, 8)
        tilt_box(g, TIMBER, (foot[0] + sx * 52, foot[1], foot[2] - 40), b, 9, 9)
    # ---- what still flies: the long pennant at the main truck, in three lengths, and the ensign aft
    truck = _ship((200, 0, 3340))
    for k in range(3):
        f = sc.part(P.motion(swing=10 + 7 * k, pivot=truck, axis=P.UP, rate=0.5, phase=0.12 * k))
        f.box(RED, (truck[0] - 150 - 290 * k, truck[1], truck[2] - 10 * k), (300, 6, 84 - 22 * k))
    staff0, staff1 = _ship((-2900, 0, 588)), _ship((-3040, 0, 1000))
    rod(g, TIMBER, staff0, staff1, 9, steps=6)
    e = sc.part(P.motion(swing=14, pivot=staff1, axis=P.UP, rate=0.6, phase=0.4))
    e.box(RED, (staff1[0] - 130, staff1[1], staff1[2] - 70), (240, 6, 130))


JOBS = [
    ('SM_S1D4_BigDome', big_dome, dict(grow=(40, 40, 20), grow_down=(40, 40, 0))),
    ('SM_S1D4_StoreDome', store_dome, dict(grow=(30, 30, 15), grow_down=(30, 30, 0))),
    ('SM_S1D4_VaultHut', vault_hut, dict(grow=(30, 30, 15), grow_down=(30, 30, 0))),
    ('SM_S1D4_RadioHut', radio_hut, dict(grow=(40, 60, 20), grow_down=(40, 60, 0))),
    ('SM_S1D4_MessTent', mess_tent, dict(grow=(40, 40, 20), grow_down=(40, 40, 0))),
    ('SM_S1D4_SledgeGarage', sledge_garage, dict(grow=(30, 40, 15), grow_down=(30, 40, 0))),
    ('SM_S1D4_BalloonShed', balloon_shed, dict(grow=(60, 120, 20), grow_down=(60, 120, 0))),
    ('SM_S1D4_BunkTents', bunk_tents, dict(grow=(40, 50, 15), grow_down=(40, 50, 0))),
    ('SM_S1D4_RouteFlags', route_flags, dict(grow=(20, 22, 4), grow_down=(20, 22, 0))),
    ('SM_S1D4_CrateStack', crate_stack, STILL),
    ('SM_S1D4_LoadedSledge', loaded_sledge, STILL),
    ('SM_S1D4_FuelDump', fuel_dump, STILL),
    ('SM_S1D4_DepotCairn', depot_cairn, dict(grow=(30, 40, 10), grow_down=(30, 40, 0))),
    ('SM_S1D4_Theodolite', theodolite, dict(grow=(12, 12, 6), grow_down=(12, 12, 0))),
    ('SM_S1D4_SignalMast', signal_mast, dict(grow=(20, 70, 8), grow_down=(20, 70, 0))),
    ('SM_S1D4_DryingLine', drying_line, dict(grow=(6, 26, 4), grow_down=(6, 26, 0))),
    ('SM_S1D4_IceboundShip', icebound_ship, dict(far=True, grow=(60, 260, 30), grow_down=(60, 260, 0))),
]


def run(only=None):
    P.build_core()                              # loads the two masters; never rebuilds them
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
