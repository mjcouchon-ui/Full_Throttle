"""Volcano, Stage 1, builder s1people: the people pass 3 left out. /Game/World/Volcano/SM_S1PEOPLE_*.
Run: import ft_world_volcano_s1people as K; K.run()

Twenty-five small scenes (about 4.5 x 3.4 m), three for each of the eight Stage 1 districts and one basalt pier for
the left-hand banked bends. Every piece brings what its people stand on (a deck on legs, an open stair, a terrace, a
veranda, a scaffold, a cradle, a pipe landing, a wall walk: floors 2.6 to 3.6 m up), so that nobody is a head behind
the 1.6 m barrier, and every piece is open underneath or narrow, so that the lava channel behind it stays in sight.
People are drawn a quarter over life size. They bathe, serve tea, watch the cone, read the storm, blow glass, rig,
come off shift, cook, play cards, sweep, carry water, ring a bell, haul sacks, mend a sail, wave ribbons, read gauges,
turn a valve, take tea on a pipe, blow horns, sell cloth, lay stone, paint a sky, carry a flat, queue in costume.
Limbs, flags, bells and cloth swing in the vertex shader; the miners walk the length of their own boardwalk and no
further; nothing leaves its piece, nothing flies, nothing circles, nothing blinks, nothing is thrown.
Steady light only: the storm gallery's bead and the kiln's mouth (every rule that places them is 35 m out or more).
No fire crew, no hose, nobody in red, nothing red or orange, no vehicle with an engine, no camera, no drone, no
weapon, no wrench, no box or lantern that could be a pickup; long things (poles, staffs, horns, brushes) are held up
or along the road, never at it. No words, no brand, no likeness, no outside asset.
The placing rules are the block C:/ftwork/s1/L3_s1people.inl."""
import math
import random
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/Volcano'
FOOT = -200.0
SHOES = ('#22202a', '#fff6ea', '#5a4a3c')

# the districts' palettes (C:/ftwork/final/s1/L3_plan.md) and the kit's pale stones
W, COB, LEM, MINT, PANE = '#f4efe6', '#2456c8', '#ffe45c', '#8fe0c0', '#14101a'       # d1 Cinder Springs
STEAM, WATER, STONE = '#fbf7ee', '#c9f4e6', '#cdc5c8'
VIO, MAG, LIL, WH2, CYAN = '#7a4cff', '#ff4fb8', '#b9b3d6', '#f2f0ff', '#3fe0ff'      # d2 Stormwatch
MUS, CRM, TEAL, COCOA, WH3 = '#c7a13b', '#f1e3c0', '#1f8a8a', '#5c3324', '#fbf7ee'    # d3 Slagtown
SNOW, JADE, GOLD, PLUM, PST = '#f6f3ea', '#2fae7a', '#ffd23f', '#5a2a6a', '#b0a088'   # d4 the Cliff Temple
PINK, SKY, WH5, LCR, ASHL = '#ff8fb1', '#6fc8ff', '#fbfbf5', '#fff1a8', '#6f6272'     # d5 Windmill Flats
LIME, WH6, GRAPH, SILV, CONC = '#b6f03a', '#f2f4ee', '#3a4a66', '#c9c4d6', '#9a98a8'  # d6 Geotherm
SAND, TURQ, NAVY, CHALK, OGOLD = '#e8d9b0', '#19c5c0', '#1c2a4a', '#fbf7ee', '#c8b14b'  # d7 the Wall Keep
MNT, BUT, WHITE = '#7fe0b8', '#fff1c9', '#ffffff'                                     # d8 the Backlot (with NAVY, GOLD)
ROPE = '#d8c9a0'


# ------------------------------------------------------------------------------------------ helpers
def who(sc, r, x, y, z, yaw=0.0, s=1.25, tops=None, bottoms=None, detail=1, **kw):
    d = P.look(r, tops=tops, bottoms=bottoms, shoes=r.choice(SHOES))
    d['s'] = d['s'] * s
    d['phase'] = r.random()
    d.update(kw)
    return P.figure(sc, x, y, z, yaw=yaw, detail=detail, **d)


def posts(g, col, L, D, top, w=16.0, inset=14.0, foot=FOOT):
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(col, (sx * (L / 2.0 - inset), sy * (D / 2.0 - inset), (top + foot) / 2.0), (w, w, top - foot))


def xbrace(g, col, L, y, z0, z1, w=8.0):
    tilt_box(g, col, (-L / 2.0 + 14, y, z0), (L / 2.0 - 14, y, z1), w, w)
    tilt_box(g, col, (-L / 2.0 + 14, y, z1), (L / 2.0 - 14, y, z0), w, w)


def deck(g, col, L, D, z, t=18.0, edge=None, y=0.0, x=0.0):
    g.box(col, (x, y, z - t / 2.0), (L, D, t))
    if edge:
        g.box(edge, (x, y - D / 2.0 - 3, z - t / 2.0), (L + 6, 8, t + 8))


def rail(g, col, L, y, z, h=100.0, n=4, w=7.0, x=0.0):
    g.box(col, (x, y, z + h), (L, w, w))
    for k in range(n):
        g.box(col, (x - L / 2.0 + w / 2.0 + k * (L - w) / (n - 1.0), y, z + h / 2.0), (w, w, h))


def slab(g, col, L, y0, z0, y1, z1, t=10.0, x=0.0):
    """A sloping slab L long (along X) from the edge (y0, z0) to the edge (y1, z1)."""
    tilt_box(g, col, (x, y0, z0), (x, y1, z1), L, t)


def open_stair(g, col, col2, x0, x1, y, z0, z1, n=6, w=90.0):
    """Treads on two stringers, climbing along X from (x0, z0) to (x1, z1): the lava shows between the treads."""
    for sy in (-1, 1):
        tilt_box(g, col2, (x0, y + sy * (w / 2.0 - 4), z0 - 14), (x1, y + sy * (w / 2.0 - 4), z1 - 14), 8, 16)
    for k in range(n):
        f = (k + 0.5) / n
        g.box(col, (x0 + (x1 - x0) * f, y, z0 + (z1 - z0) * (k + 1.0) / n - 4), (abs(x1 - x0) / n * 0.8, w, 8))


def steps(g, col, x, y0, y1, top, n=5, w=160.0, foot=FOOT, edge=None):
    """Solid steps w wide at x, climbing from the front (y0) to the back (y1), the last one `top` high."""
    for k in range(n):
        h = top * (k + 1.0) / n
        yc = y0 + (y1 - y0) * (k + 0.5) / n
        g.box(col, (x, yc, (h + foot) / 2.0), (w, abs(y1 - y0) / n + 2.0, h - foot))
        if edge:
            g.box(edge, (x, yc - abs(y1 - y0) / n / 2.0, h - 4), (w + 4, 6, 8))


def pennant(sc, r, col, x, y, z, w=80.0, h=38.0, swing=14.0, rate=0.45):
    f = sc.part(P.motion(swing=swing, pivot=(x, y, z), axis=P.UP, rate=rate, phase=r.random()))
    f.box(col, (x + w / 2.0 + 4, y, z), (w, 5, h))


def hang(sc, r, col, x, y, z, w=40.0, h=60.0, swing=7.0, rate=0.4):
    """Cloth hanging from a line or rail at (x, y, z): it swings a little about the line."""
    f = sc.part(P.motion(swing=swing, pivot=(x, y, z), axis=0.0, rate=rate, phase=r.random()))
    f.box(col, (x, y, z - h / 2.0), (w, 5, h))


def bunting(g, cols, x0, x1, y, z, n=7, sag=18.0):
    for k in range(n):
        f = k / (n - 1.0)
        g.box(cols[k % len(cols)], (x0 + (x1 - x0) * f, y, z - sag * math.sin(math.pi * f)), (20, 5, 20), rot=(0, 0, 45))


def parasol(g, col, rim, x, y, z, r=95.0, h=250.0):
    g.box(STONE, (x, y, z + h / 2.0), (6, 6, h))
    g.cyl(col, (x, y, z + h - 8), r, 30, steps=8, top_r=8)
    g.cyl(rim, (x, y, z + h - 13), r + 3, 5, steps=8)


def upturn_roof(g, col, trim, L, D, z, rise=60.0, x=0.0, y=0.0):
    """A deep roof with a ridge along X and corners that turn up (the temple's)."""
    slab(g, col, L, y - D / 2.0, z, y, z + rise, 12, x)
    slab(g, col, L, y + D / 2.0, z, y, z + rise, 12, x)
    g.box(trim, (x, y, z + rise + 6), (L * 0.9, 12, 12))
    for sx in (-1, 1):
        for sy in (-1, 1):
            tilt_box(g, col, (x + sx * (L / 2.0 - 30), y + sy * D / 2.0, z), (x + sx * (L / 2.0 + 16), y + sy * (D / 2.0 + 10), z + 26), 16, 10)


def bench(g, col, x, y, z, L=120.0, top=56.0, d=32.0):
    g.box(col, (x, y, z + top - 5), (L, d, 10))
    for sx in (-1, 1):
        g.box(col, (x + sx * (L / 2.0 - 8), y, z + (top - 10) / 2.0), (8, d - 4, top - 10))


def table(g, col, x, y, z, L=90.0, top=92.0, d=70.0):
    g.box(col, (x, y, z + top - 4), (L, d, 8))
    g.box(col, (x, y, z + (top - 8) / 2.0), (10, 10, top - 8))


# what a hand holds: f(model, hand, side)
def cup(col):
    def f(m, h, side):
        m.cyl(col, (h[0], h[1] - 2, h[2] + 4), 6, 15, steps=6, top_r=8)
    return f


def tray(col, col2):
    def f(m, h, side):
        m.box(col, (h[0], h[1] - 10, h[2] + 10), (44, 30, 5))
        m.cyl(col2, (h[0] - 8, h[1] - 10, h[2] + 12), 7, 16, steps=6, top_r=5)
        m.cyl(col2, (h[0] + 12, h[1] - 12, h[2] + 12), 5, 10, steps=6)
    return f


def stack(cols, w=40.0):
    def f(m, h, side):
        if side > 0:
            for k, c in enumerate(cols):
                m.box(c, (h[0] - 24, h[1] - 6, h[2] + 10 + k * 11), (w, 30, 10))
    return f


def pole(col, length=230.0, top=None, lean=0.0):
    """A long thing held UPRIGHT (lean: cm the top goes along X): a staff, a storm pole, a banner pole."""
    def f(m, h, side):
        tilt_box(m, col, (h[0], h[1], h[2] - 60), (h[0] + lean, h[1], h[2] + length - 60), 5, 5)
        if top:
            m.sphere(top, (h[0] + lean, h[1], h[2] + length - 54), 9, steps=(3, 6))
    return f


def banner(col, fringe, length=240.0, w=58.0, h=84.0):
    def f(m, hd, side):
        m.box(SILV, (hd[0], hd[1], hd[2] + length / 2.0 - 50), (5, 5, length))
        m.box(col, (hd[0] + side * (w / 2.0 + 3), hd[1], hd[2] + length - 50 - h / 2.0), (w, 5, h))
        m.box(fringe, (hd[0] + side * (w / 2.0 + 3), hd[1], hd[2] + length - 54 - h), (w, 5, 8))
    return f


def clipboard(col=WH2, clip=VIO):
    def f(m, h, side):
        m.box(col, (h[0], h[1] - 6, h[2] + 10), (22, 5, 30))
        m.box(clip, (h[0], h[1] - 7, h[2] + 24), (12, 6, 5))
    return f


def broom(col, head):
    def f(m, h, side):
        tilt_box(m, col, (h[0], h[1] + 10, h[2] + 60), (h[0], h[1] - 50, h[2] - 110), 5, 5)
        m.box(head, (h[0], h[1] - 54, h[2] - 118), (40, 14, 18))
    return f


def mallet(col, head):
    def f(m, h, side):
        m.box(col, (h[0], h[1], h[2] + 18), (5, 5, 44))
        m.box(head, (h[0], h[1], h[2] + 42), (26, 14, 14))
    return f


def horn(col, bell_col, length=170.0):
    """A long straight horn held UP and along the road (side: which way it leans), a short banner hanging from it."""
    def f(m, h, side):
        tip = (h[0] + side * length * 0.35, h[1], h[2] + length * 0.9)
        tilt_box(m, col, (h[0] - side * 12, h[1], h[2] - 16), tip, 7, 7)
        m.cyl(col, (tip[0], tip[1], tip[2] - 4), 7, 22, steps=6, top_r=17, rot=(-side * 21, 0, 0))
        m.box(bell_col, (h[0] + side * length * 0.2, h[1], h[2] + length * 0.5 - 30), (34, 5, 46))
    return f


def ribbon(col, stick=WH5):
    def f(m, h, side):
        m.box(stick, (h[0], h[1], h[2] + 26), (5, 5, 60))
        tilt_box(m, col, (h[0], h[1], h[2] + 54), (h[0] + side * 70, h[1], h[2] + 84), 5, 12)
        tilt_box(m, col, (h[0] + side * 70, h[1], h[2] + 84), (h[0] + side * 120, h[1], h[2] + 58), 5, 12)
    return f


def roller(col, head, length=190.0):
    def f(m, h, side):
        m.box(col, (h[0], h[1], h[2] + length / 2.0 - 30), (5, 5, length))
        m.cyl(head, (h[0] - 16, h[1], h[2] + length - 26), 8, 32, steps=6, rot=(90, 0, 0))
    return f


def sack(col, tie):
    def f(m, h, side):
        if side > 0:
            m.box(col, (h[0] - 24, h[1] - 8, h[2] + 14), (52, 34, 36))
            m.box(tie, (h[0] - 24, h[1] - 8, h[2] + 35), (20, 12, 8))
    return f


WAVE = dict(arm_r='wave', swing_r=22, rate=1.0)
WAVE_L = dict(arm_l='wave', swing_l=22, rate=0.9)
CHEER = dict(arm_l='vee', arm_r='vee', bob=5, rate=1.3)


# ------------------------------------------------------------------------------------------ d1 Cinder Springs
ROBES = (W, LEM, MINT, '#bfd4ff')


def bath_deck(sc):
    """A whitewashed plunge deck on round legs: a bather on the rim with her feet out, an attendant with towels, a
    guest under a lemon parasol waving at the chase, steam off the pale water, a steam pipe with a mushroom cap."""
    r = random.Random(301)
    g = sc.still()
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.cyl(W, (sx * 196, sy * 138, FOOT), 17, 282 - FOOT, steps=8)
    deck(g, W, 440, 330, 300, t=20, edge=COB)
    g.box(MINT, (95, 45, 318), (230, 180, 36))
    g.box(WATER, (95, 45, 338), (200, 150, 6))
    for k, (x, y) in enumerate(((45, 70), (120, 30), (160, 95))):
        st = sc.part(P.motion(bob=-12, rate=0.22, phase=k * 0.31))
        st.sphere(STEAM, (x, y, 400 + k * 34), 20 + 5 * k, steps=(3, 6))
    rail(g, COB, 440, 158, 300, h=105, n=5)
    for k, c in enumerate((LEM, W, MINT, LEM)):
        hang(sc, r, c, -180 + k * 62, 158, 404, w=44, h=64)
    parasol(g, LEM, W, -125, 30, 300)
    g.cyl(MINT, (188, 132, 300), 11, 300, steps=6)
    g.cyl(MINT, (188, 132, 600), 34, 14, steps=8, top_r=10)
    who(sc, r, 60, -32, 300, 6, tops=ROBES, wear='coat', sleeves='long', hair='bun', legs='sit', seat=29, hold_r=cup(W), arm_r='face', swing_r=6, rate=0.3)
    who(sc, r, -40, -110, 300, -12, top=COB, bottom=W, sleeves='short', hat='cap', hat_col=W, arm_l='carry', arm_r='carry', hold_r=stack((W, LEM, MINT, W)))
    who(sc, r, -150, -70, 300, 8, tops=ROBES, wear='coat', sleeves='long', **WAVE)
    who(sc, r, 170, -100, 300, -20, tops=ROBES, wear='coat', sleeves='long', build='kid', s=0.86, **CHEER)


def tea_terrace(sc):
    """The tea pavilion's steps: a narrow whitewashed stair to a vaulted canopy on thin posts, a server with a tray
    coming down, two guests on a bench with cups, a lemon tree in a cobalt tub."""
    r = random.Random(302)
    g = sc.still()
    steps(g, W, -130, -165, -15, 300, n=5, w=170, edge=COB)
    posts(g, W, 440, 190, 300, inset=12)
    deck(g, W, 440, 190, 300, t=20, edge=COB, y=70)
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(W, (sx * 205, 70 + sy * 82, 430), (9, 9, 260))
    for k in range(5):                                             # a barrel vault in five facets
        a0, a1 = math.radians(180.0 * k / 5.0), math.radians(180.0 * (k + 1) / 5.0)
        slab(g, COB if k % 2 == 0 else W, 440, 70 - math.cos(a0) * 96, 560 + math.sin(a0) * 70, 70 - math.cos(a1) * 96, 560 + math.sin(a1) * 70, 9)
    bench(g, COB, 120, 120, 300, L=170)
    g.cyl(COB, (-185, 110, 300), 28, 44, steps=8, top_r=34)
    g.box(STONE, (-185, 110, 384), (7, 7, 80))
    g.sphere('#5fbf6a', (-185, 110, 446), 40, steps=(4, 7))
    for a in range(3):
        g.sphere(LEM, (-185 + 34 * math.cos(a * 2.1), 110 + 34 * math.sin(a * 2.1) - 8, 440 + 10 * a), 9, steps=(3, 5))
    who(sc, r, -130, -90, 180, 0, top=COB, bottom=W, sleeves='long', hat='cap', hat_col=LEM, arm_l='hold', arm_r='hold', hold_r=tray(BRASS_T, W), legs='stride')
    who(sc, r, 80, 100, 300, 5, tops=ROBES, wear='coat', sleeves='long', legs='sit', seat=45, hold_r=cup(LEM), arm_r='face', swing_r=5, rate=0.25)
    who(sc, r, 160, 100, 300, -8, tops=ROBES, wear='coat', sleeves='long', legs='sit', seat=45, hair='long', **WAVE_L)
    who(sc, r, -20, 20, 300, -4, tops=ROBES, wear='coat', sleeves='long', arm_r='point', swing_r=8, rate=0.5)


BRASS_T = '#c8b14b'


def balcony_watch(sc):
    """A guest-house balcony on two thin legs: guests watching the cone through glasses, a child waving through the
    cobalt rail, a lemon awning that lifts in the hot wind, a round-topped door, towels on a line."""
    r = random.Random(303)
    g = sc.still()
    for sx in (-1, 1):
        g.box(W, (sx * 200, 120, (340 + FOOT) / 2.0), (26, 60, 340 - FOOT))
        tilt_box(g, W, (sx * 200, 100, 150), (sx * 200, -150, 322), 14, 14)
    deck(g, W, 440, 320, 340, t=20, edge=MINT)
    g.box(W, (0, 152, 500), (440, 16, 320))                      # the wall behind, thin: a front
    g.box(COB, (-110, 142, 450), (84, 6, 200))
    g.cyl(COB, (-110, 145, 550), 42, 6, rot=(0, 0, 90), steps=10)
    g.box(PANE, (90, 142, 480), (100, 6, 110))
    for sx in (-1, 1):
        g.box(LEM, (90 + sx * 62, 141, 480), (20, 6, 116))
    g.sphere(COB, (0, 150, 668), 120, scale=(1.5, 0.12, 0.5), steps=(4, 8))   # a shallow dome over the front
    rail(g, COB, 440, -156, 340, h=100, n=6)
    aw = sc.part(P.motion(swing=5, pivot=(0, 144, 640), axis=0.0, rate=0.3, phase=0.2))
    slab(aw, LEM, 420, 144, 640, -60, 590, 6)
    for k in range(4):
        aw.box(W, (-150 + k * 100, -62, 580), (50, 5, 22))
    who(sc, r, -150, -110, 340, 10, tops=ROBES, wear='coat', sleeves='long', arm_l='face', arm_r='face', hold_r=glasses(COB))
    who(sc, r, -50, -105, 340, -6, tops=ROBES, wear='coat', sleeves='long', build='kid', s=0.84, **WAVE)
    who(sc, r, 60, -100, 340, 4, tops=ROBES, wear='coat', sleeves='long', hair='bun', arm_r='point', swing_r=7, rate=0.4)
    who(sc, r, 165, -90, 340, -14, top=COB, bottom=W, hat='cap', hat_col=W, arm_l='hold', arm_r='hold', hold_r=tray(BRASS_T, MINT))


def glasses(col):
    def f(m, h, side):
        m.box(col, (h[0] - side * 12, h[1] - 8, h[2] + 4), (26, 16, 10))
    return f


# ------------------------------------------------------------------------------------------ d2 Stormwatch
STORM = (VIO, LIL, WH2, '#9d7bff')


def storm_gallery(sc):
    """A ring gallery on a lilac lattice: storm watchers with tall poles and clipboards reading the ash cloud, a stack
    of ring insulators with one steady bead, a cup vane turning a little each way."""
    r = random.Random(311)
    g = sc.still()
    for sx in (-1, 1):
        for sy in (-1, 1):
            tilt_box(g, LIL, (sx * 205, sy * 150, FOOT), (sx * 150, sy * 110, 320), 12, 12)
    xbrace(g, LIL, 400, -128, 40, 290)
    g.box(LIL, (0, -128, 165), (330, 8, 8))
    deck(g, VIO, 440, 320, 330, t=18, edge=MAG)
    rail(g, LIL, 440, -156, 330, h=100, n=5)
    for k in range(5):
        g.cyl(WH2 if k % 2 else VIO, (150, 90, 330 + k * 46), 46 - k * 4, 30, steps=8, top_r=30 - k * 3)
        g.cyl(LIL, (150, 90, 360 + k * 46), 12, 16, steps=6)
    g.sphere(CYAN, (150, 90, 584), 24, steps=(4, 7), glow=0.4)
    g.box(LIL, (-170, 100, 520), (7, 7, 380))
    v = sc.part(P.motion(swing=50, pivot=(-170, 100, 716), axis=P.UP, rate=0.25, phase=0.4))
    for a in range(3):
        an = math.radians(a * 120.0)
        tilt_box(v, LIL, (-170, 100, 716), (-170 + 62 * math.cos(an), 100 + 62 * math.sin(an), 716), 5, 5)
        v.sphere(MAG, (-170 + 66 * math.cos(an), 100 + 66 * math.sin(an), 716), 14, steps=(3, 6))
    kw = dict(tops=STORM, bottoms=(VIO, '#2b2350', LIL), sleeves='long', band=MAG)
    who(sc, r, -160, -90, 330, 8, hat='hard', hat_col=WH2, arm_r='hold', hold_r=pole(LIL, 300, MAG), arm_l='face', **kw)
    who(sc, r, -40, -100, 330, -6, hat='hard', hat_col=WH2, arm_l='hold', hold_l=clipboard(), arm_r='point', swing_r=8, rate=0.5, **kw)
    who(sc, r, 60, -80, 330, 5, hair='pony', arm_l='up', swing_l=10, rate=0.6, arm_r='hold', hold_r=clipboard(), **kw)
    who(sc, r, 20, 70, 330, -10, hat='hard', hat_col=MAG, arm_l='hold', hold_l=pole(LIL, 320, CYAN, lean=20), **kw)


def kiln_mouth(sc):
    """The glass-blower's kiln on a riveted drum: he turns a blowpipe held up along the road, a gather of glass on its
    end; a helper works the bellows, a third holds up a finished violet flask to the light."""
    r = random.Random(312)
    g = sc.still()
    g.cyl(LIL, (-90, 30, FOOT), 128, 300 - FOOT, steps=10, top_r=120)
    for z in (60, 180):
        g.cyl(MAG, (-90, 30, z), 131, 12, steps=10)
    posts(g, LIL, 440, 320, 300, w=12, inset=10)
    deck(g, VIO, 440, 320, 300, t=16, edge=MAG)
    g.cyl(VIO, (-120, 90, 300), 78, 150, steps=8, top_r=70)             # the kiln: a drum with a ribbed white cap
    g.sphere(WH2, (-120, 90, 450), 70, scale=(1, 1, 0.6), steps=(4, 8))
    g.box(PANE, (-120, 12, 380), (64, 8, 64))
    g.box('#fff3c4', (-120, 9, 376), (44, 5, 40), glow=0.4)             # the mouth: pale, steady
    g.cyl(LIL, (-120, 90, 490), 10, 130, steps=6)
    table(g, LIL, 150, 90, 300, L=110)
    for k, c in enumerate((MAG, VIO, CYAN, WH2)):
        g.cyl(c, (112 + k * 26, 90, 392), 9, 22 + 6 * (k % 2), steps=6, top_r=4)
    bl = sc.part(P.motion(swing=9, pivot=(10, 100, 330), axis=0.0, rate=0.6, phase=0.1))
    bl.box(COCOA, (10, 70, 345), (60, 80, 18))
    g.box(COCOA, (10, 70, 322), (60, 80, 14))
    kw = dict(tops=STORM, bottoms=(VIO, '#2b2350'), sleeves='long')
    who(sc, r, -40, -80, 300, -10, build='stocky', band=MAG, hair='bald', arm_r=((120, 0), (150, 0)), swing_r=8, axis_r='y', rate=0.5,
        hold_r=blowpipe(), **kw)
    who(sc, r, 15, -15, 300, 165, arm_l='push', arm_r='push', swing_l=9, swing_r=9, axis_l='x', axis_r='x', rate=0.6, hat='cap', hat_col=MAG, **kw)
    who(sc, r, 150, -90, 300, -8, hair='bun', band=MAG, arm_r='up', swing_r=6, rate=0.3, hold_r=flask(VIO), **kw)
    who(sc, r, -170, -110, 300, 10, tops=STORM, build='kid', s=0.84, hair='pony', **CHEER)


def blowpipe():
    def f(m, h, side):
        tip = (h[0] + 100, h[1], h[2] + 90)
        tilt_box(m, SILV, (h[0] - 30, h[1], h[2] - 27), tip, 5, 5)
        m.sphere(MAG, tip, 15, steps=(3, 6))
    return f


def flask(col):
    def f(m, h, side):
        m.sphere(col, (h[0], h[1], h[2] + 16), 13, steps=(3, 6))
        m.cyl(col, (h[0], h[1], h[2] + 24), 5, 20, steps=6)
    return f


def rigger_frame(sc):
    """Riggers on a ladder frame: one climbs, one hauls a coil drum up on a rope, one on the top stage steadies a
    lightning rod with three ring fins. The whole thing is thin steel: the lava shows through it."""
    r = random.Random(313)
    g = sc.still()
    for sx in (-1, 1):
        for sy in (-1, 1):
            tilt_box(g, LIL, (sx * 200, sy * 140, FOOT), (sx * 120, sy * 80, 620), 11, 11)
    xbrace(g, LIL, 390, -122, 30, 270)
    deck(g, VIO, 400, 280, 290, t=14, edge=MAG)
    deck(g, VIO, 250, 170, 620, t=14, edge=MAG)
    rail(g, LIL, 250, -86, 620, h=90, n=3)
    for sy in (-1, 1):
        g.box(LIL, (-150 + sy * 0, -110 + sy * 24, 455), (7, 7, 330))    # the ladder's rails
    for k in range(7):
        g.box(LIL, (-150, -110, 310 + k * 44), (7, 54, 6))
    g.box(LIL, (60, 20, 860), (8, 8, 480))                              # the rod
    for k in range(3):
        g.cyl(MAG if k % 2 == 0 else WH2, (60, 20, 900 + k * 60), 36 - k * 8, 8, steps=8)
    g.sphere(WH2, (60, 20, 1104), 14, steps=(3, 6))
    g.box(LIL, (135, -90, 760), (120, 7, 7))                            # the hoist arm
    g.box(ROPE, (185, -90, 560), (5, 5, 400))
    dr = sc.part(P.motion(bob=-26, rate=0.2, phase=0.6))
    dr.cyl(MAG, (185, -110, 420), 34, 40, rot=(0, 0, 90), steps=8)
    dr.cyl(WH2, (185, -114, 420), 16, 48, rot=(0, 0, 90), steps=6)
    kw = dict(tops=STORM, bottoms=(VIO, '#2b2350'), sleeves='long', band=MAG, hat='hard', hat_col=WH2)
    who(sc, r, -150, -134, 400, 180, arm_l='up', arm_r='vee', swing_l=14, swing_r=-14, axis_l='x', axis_r='x', stride=10, rate=0.5, **kw)
    who(sc, r, 150, 0, 290, -10, arm_l='up', arm_r='up', swing_l=10, swing_r=10, axis_l='x', axis_r='x', rate=0.4, **kw)
    who(sc, r, 0, 20, 620, 6, arm_r='out', swing_r=5, rate=0.4, **kw)
    who(sc, r, -40, -60, 290, 10, arm_l='point', swing_l=7, rate=0.5, arm_r='hold', hold_r=clipboard(), **kw)


# ------------------------------------------------------------------------------------------ d3 Slagtown
MINER = (MUS, CRM, TEAL, '#e6c46a')
PIT = (COCOA, '#3b2a22', '#2b2350')


def pail(col):
    def f(m, h, side):
        m.box(col, (h[0], h[1], h[2] - 20), (26, 16, 20))
        m.box(SILV, (h[0], h[1], h[2] - 7), (5, 5, 8))
    return f


def shift_walk(sc):
    """The boardwalk on stilts: the shift coming off, walking its length with their pails (they come round again at the
    far end), a foreman at the rail counting them through, an unlit lamp bracket, a notice board of coloured squares."""
    r = random.Random(321)
    g = sc.still()
    posts(g, COCOA, 440, 250, 290)
    xbrace(g, COCOA, 440, -108, 40, 250)
    deck(g, CRM, 450, 260, 290, t=16, edge=TEAL)
    for k in range(6):
        g.box(MUS, (-190 + k * 76, 0, 291), (6, 258, 5))
    rail(g, COCOA, 450, 126, 290, h=100, n=5)
    g.box(COCOA, (-190, 132, 520), (10, 10, 460))
    g.box(WH3, (-120, 132, 610), (150, 8, 110))
    for k, c in enumerate((TEAL, MUS, CRM, TEAL, MUS, WH3)):
        g.box(c, (-165 + (k % 3) * 44, 126, 636 - (k // 3) * 50), (30, 5, 30))
    slab(g, TEAL, 190, 132, 690, 60, 664, 8, x=-120)
    kw = dict(tops=MINER, bottoms=PIT, sleeves='long', hat='hard')
    for k, (y, ph, hc) in enumerate(((-70, 0.0, CRM), (-20, 0.33, MUS), (-80, 0.66, WH3))):
        who(sc, r, (k - 1) * 147, y, 290, 90, hat_col=hc, legs='stride', stride=20, rate=1.1, hold_l=pail(TEAL if k % 2 else SILV),
            travel=(430, 55), **dict(kw, phase=ph))
    who(sc, r, 150, 80, 290, -10, top=TEAL, bottom=COCOA, sleeves='long', hat='peak', hat_col=COCOA, build='stocky',
        arm_l='hold', hold_l=clipboard(WH3, TEAL), arm_r='point', swing_r=9, rate=0.6)


def cook_veranda(sc):
    """The cook house veranda: the cook rings the dinner triangle, two miners at the table with mugs, a dog asleep
    by the step, a fat tin chimney and a cream awning that lifts."""
    r = random.Random(322)
    g = sc.still()
    posts(g, COCOA, 440, 320, 280)
    xbrace(g, COCOA, 440, -144, 40, 240)
    deck(g, CRM, 440, 330, 280, t=18, edge=COCOA)
    g.box(MUS, (0, 156, 430), (440, 14, 300))
    for k in range(8):
        g.box(CRM, (-196 + k * 56, 147, 430), (7, 6, 300))
    g.box(TEAL, (130, 146, 390), (84, 6, 200))
    g.box(PANE, (-90, 146, 440), (110, 6, 80))
    g.cyl(SILV, (-180, 120, 280), 30, 420, steps=8)
    g.cyl(SILV, (-180, 120, 700), 44, 14, steps=8, top_r=12)
    aw = sc.part(P.motion(swing=4, pivot=(0, 150, 590), axis=0.0, rate=0.3, phase=0.5))
    slab(aw, CRM, 400, 150, 590, -150, 520, 7, x=20)
    for k in range(4):
        aw.box(TEAL, (-130 + k * 100, -152, 508), (50, 5, 22))
    g.box(COCOA, (200, -150, 400), (9, 9, 240))
    g.box(COCOA, (170, -150, 516), (70, 7, 7))
    tri = sc.part(P.motion(swing=10, pivot=(150, -150, 512), axis=90.0, rate=1.6, phase=0.2))
    for a, b in (((150, 480), (130, 440)), ((150, 480), (170, 440)), ((130, 440), (170, 440))):
        tilt_box(tri, SILV, (a[0], -150, a[1]), (b[0], -150, b[1]), 5, 5)
    tri.box(ROPE, (150, -150, 496), (5, 5, 32))
    table(g, COCOA, -40, 60, 280, L=120, top=90)
    bench(g, COCOA, -40, 0, 280, L=130)
    who(sc, r, 105, -110, 280, 0, top=WH3, bottom=CRM, hat='chef', hat_col=WH3, build='stocky', arm_r=((120, 30), (150, 30)), swing_r=12, axis_r='y',
        rate=1.6, hold_r=mallet(SILV, SILV), arm_l='hip')
    who(sc, r, -75, 0, 280, 4, tops=MINER, bottoms=PIT, sleeves='long', hat='hard', hat_col=CRM, legs='sit', seat=45, arm_r='face', hold_r=cup(WH3), swing_r=5, rate=0.3)
    who(sc, r, -5, 0, 280, -6, tops=MINER, bottoms=PIT, sleeves='long', hair='short', legs='sit', seat=45, **WAVE_L)
    P.quadruped(sc, -170, -110, 280, yaw=70, body='#d9a070', belly=CRM, s=1.7, length=40, height=20, girth=16, neck=9, neck_up=-10, head=24, ears=COCOA,
                tail=20, wag=20, rate=0.5, phase=0.3)


def card_stair(sc):
    """The bunkhouse's outside stair: children sat on the open treads watching the chase go by, card players at a
    table on the landing under a water tank on legs."""
    r = random.Random(323)
    g = sc.still()
    open_stair(g, CRM, COCOA, -215, -20, -90, 120, 330, n=6, w=130)
    g.box(COCOA, (-215, -90, (120 + FOOT) / 2.0), (14, 120, 120 - FOOT))
    for sx in (-20, 210):
        for sy in (-1, 1):
            g.box(COCOA, (sx, sy * 140, (330 + FOOT) / 2.0), (15, 15, 330 - FOOT))
    deck(g, CRM, 250, 310, 330, t=16, edge=TEAL, x=95)
    rail(g, COCOA, 250, 150, 330, h=100, n=4, x=95)
    for sx in (20, 180):
        g.box(COCOA, (sx, 85, 520), (10, 10, 380))
    g.cyl(TEAL, (100, 85, 690), 78, 120, steps=10)
    for z in (700, 790):
        g.cyl(WH3, (100, 85, z), 80, 8, steps=10)
    g.cyl(MUS, (100, 85, 810), 80, 34, steps=10, top_r=10)
    table(g, COCOA, 100, -20, 330, L=90, top=88, d=80)
    for k in range(3):
        g.box(WH3, (80 + k * 20, -20 - 8 * k, 420), (14, 20, 4), rot=(0, 20 * k, 0))
    who(sc, r, 40, -30, 330, 80, tops=MINER, bottoms=PIT, hat='cap', hat_col=TEAL, legs='sit', seat=44, arm_l='hold', arm_r='hold', swing_r=6, rate=0.4)
    who(sc, r, 165, -30, 330, -80, tops=MINER, bottoms=PIT, hair='bald', build='stocky', legs='sit', seat=44, arm_r='up', swing_r=8, rate=0.5)
    who(sc, r, -166, -105, 156, 0, tops=MINER, build='kid', s=0.84, legs='sit', seat=40, **WAVE)
    who(sc, r, -101, -105, 228, 0, tops=MINER, build='kid', s=0.8, legs='sit', seat=40, hair='pony', **dict(CHEER, bob=0))


# ------------------------------------------------------------------------------------------ d4 the Cliff Temple
ROBE4 = (PLUM, JADE, '#f0c24a', SNOW)


def sweep_steps(sc):
    """A flight of pale stone steps to a moon gate: two robed figures sweep the ash off the treads, a third waits at
    the gate with a staff; stone lanterns without light, a jade roof with upturned corners."""
    r = random.Random(331)
    g = sc.still()
    steps(g, PST, 0, -168, 40, 300, n=6, w=190)
    g.box(PST, (0, 105, (300 + FOOT) / 2.0), (300, 130, 300 - FOOT))
    for sx in (-1, 1):
        g.box(SNOW, (sx * 112, 100, 410), (50, 60, 220))                # the gate's piers
        g.box(PST, (sx * 185, -120, (120 + FOOT) / 2.0), (44, 44, 120 - FOOT))   # lantern plinths
        g.box(PST, (sx * 185, -120, 150), (30, 30, 60))
        g.cyl(PST, (sx * 185, -120, 180), 34, 16, steps=6, top_r=8)
    g.box(SNOW, (0, 100, 550), (290, 60, 70))
    g.cyl(PLUM, (0, 68, 420), 72, 8, rot=(0, 0, 90), steps=10)            # the moon door's ring
    upturn_roof(g, JADE, GOLD, 360, 130, 585, rise=56, y=100)
    g.sphere(GOLD, (0, 100, 664), 12, steps=(3, 6))
    kw = dict(tops=ROBE4, wear='coat', sleeves='long')
    who(sc, r, -50, -116, 100, 90, hair='bald', arm_l='push', arm_r='push', swing_l=14, swing_r=14, axis_l='y', axis_r='y', rate=0.7, hold_r=broom(PST, ROPE), **kw)
    who(sc, r, 55, -47, 200, -90, hair='bun', arm_l='push', arm_r='push', swing_l=14, swing_r=14, axis_l='y', axis_r='y', rate=0.6, hold_l=broom(PST, ROPE), **kw)
    who(sc, r, 0, 45, 300, 0, hair='bald', arm_r='hold', hold_r=pole(PST, 260, GOLD), arm_l='wave', swing_l=14, rate=0.7, **kw)
    who(sc, r, 70, -75, 111, 0, tops=ROBE4, build='kid', s=0.84, hair='bald', legs='sit', seat=46, **WAVE)


def yoke_stair(sc):
    """The water carriers: three robed figures climbing an open stair with yokes and pails, never arriving; a line of
    prayer pennants in white, jade, gold and plum lifting over them."""
    r = random.Random(332)
    g = sc.still()
    open_stair(g, PST, PLUM, -215, 215, 0, 130, 400, n=9, w=150)
    for x, top in ((-205, 130), (0, 265), (205, 400)):
        for sy in (-1, 1):
            g.box(PLUM, (x, sy * 66, (top - 20 + FOOT) / 2.0), (13, 13, top - 20 - FOOT))
    tilt_box(g, PLUM, (-205, 66, 20), (0, 66, 240), 7, 7)
    tilt_box(g, PLUM, (0, 66, 20), (205, 66, 370), 7, 7)
    for x, z in ((-215, 560), (215, 760)):
        g.box(SNOW, (x, 80, (z + 130) / 2.0), (8, 8, z - 130))
        g.sphere(GOLD, (x, 80, z + 8), 10, steps=(3, 6))
    for k, c in enumerate((SNOW, JADE, GOLD, PLUM, SNOW, JADE, GOLD)):
        f = (k + 0.5) / 7.0
        fl = sc.part(P.motion(swing=10, pivot=(-215 + 430 * f, 80, 552 + 200 * f), axis=0.0, rate=0.5, phase=k * 0.13))
        fl.box(c, (-215 + 430 * f, 80, 530 + 200 * f), (40, 5, 44))
    tilt_box(g, SNOW, (-215, 80, 556), (215, 80, 756), 5, 5)
    kw = dict(tops=ROBE4, wear='coat', sleeves='long', legs='stride', stride=14, rate=0.8, arm_l='out', arm_r='out')
    for k, (x, hair) in enumerate(((-145, 'bald'), (0, 'bun'), (145, 'short'))):
        z = 130 + 270 * (x + 215) / 430.0 + 16
        top = who(sc, r, x, 0, z, 90, hair=hair, **kw)
        b = sc.part(P.motion(swing=3, pivot=(x, 0, z + 160), axis=0.0, rate=0.8, phase=k * 0.3))
        b.box(PST, (x, 0, z + 164), (8, 190, 8))                         # the yoke lies across the stair: pails fore and aft
        for sy in (-1, 1):
            b.box(ROPE, (x, sy * 88, z + 132), (5, 5, 60))
            b.cyl(JADE if sy < 0 else SNOW, (x, sy * 88, z + 76), 17, 30, steps=6, top_r=20)


def bell_terrace(sc):
    """A terrace on thin plum posts: the bell ringer hauls the rope and the gold bell swings in its frame under a jade
    roof; two pilgrims rest on the edge with their staffs upright, legs hanging."""
    r = random.Random(333)
    g = sc.still()
    posts(g, PLUM, 440, 320, 310)
    xbrace(g, PLUM, 440, -144, 40, 270)
    deck(g, SNOW, 440, 330, 310, t=20, edge=PST)
    for sx in (-1, 1):
        g.box(PLUM, (60 + sx * 105, 80, 480), (14, 14, 340))
    g.box(PLUM, (60, 80, 640), (250, 14, 14))
    upturn_roof(g, JADE, GOLD, 300, 160, 660, rise=60, x=50, y=80)
    bl = sc.part(P.motion(swing=14, pivot=(60, 80, 634), axis=90.0, rate=0.45, phase=0.0))
    bl.cyl(GOLD, (60, 80, 500), 58, 110, steps=8, top_r=34)
    bl.box(PLUM, (60, 80, 620), (10, 10, 30))
    bl.box(ROPE, (60, 80, 440), (5, 5, 120))
    who(sc, r, 60, -20, 310, 180, tops=ROBE4, wear='coat', sleeves='long', hair='bald', arm_l='up', arm_r='up', swing_l=12, swing_r=12, axis_l='x', axis_r='x', rate=0.45, phase=0.0)
    kw = dict(tops=ROBE4, bottoms=(PST, '#5a4a3c'), sleeves='long', legs='sit', seat=46)
    who(sc, r, -160, -100, 253, 6, hat='sun', hat_col=ROPE, pack=JADE, arm_r='hold', hold_r=pole(PST, 250, None), **kw)
    who(sc, r, -80, -100, 253, -8, hair='long', pack=PLUM, **dict(kw, **WAVE_L))
    who(sc, r, -150, 70, 310, 12, tops=ROBE4, wear='coat', sleeves='long', hair='bun', arm_l='hold', arm_r='hold', hold_r=tray(PST, GOLD))


# ------------------------------------------------------------------------------------------ d5 Windmill Flats
FARM = (PINK, SKY, WH5, LCR)
FARM_B = (ASHL, '#3a5fa8', '#f1e4c8')


def loading_stage(sc):
    """The mill's loading stage: one miller hauls a sack up on the hoist, one carries a sack to the stack, a third
    counts; ash sacks with blue ties, a small cloth vane rocking on its post."""
    r = random.Random(341)
    g = sc.still()
    posts(g, ASHL, 440, 320, 290)
    xbrace(g, ASHL, 440, -144, 40, 250)
    deck(g, WH5, 440, 330, 290, t=18, edge=PINK)
    for k, (x, y, z) in enumerate(((120, 110, 0), (176, 110, 0), (148, 110, 38), (120, 60, 0), (186, 55, 0))):
        g.box(LCR if k % 2 else WH5, (x, y, 290 + 20 + z), (52, 44, 38))
        g.box(SKY, (x, y, 290 + 42 + z), (20, 14, 8))
    g.box(ASHL, (-190, 130, 520), (12, 12, 460))                         # the hoist post and arm
    tilt_box(g, ASHL, (-190, 130, 740), (-190, -130, 760), 10, 10)
    tilt_box(g, ASHL, (-190, 130, 600), (-190, -40, 750), 7, 7)
    hs = sc.part(P.motion(bob=-40, rate=0.18, phase=0.2))
    hs.box(ROPE, (-190, -130, 600), (5, 5, 300))
    hs.box(LCR, (-190, -130, 420), (54, 40, 60))
    hs.box(SKY, (-190, -130, 455), (20, 14, 10))
    g.box(ASHL, (60, 140, 560), (8, 8, 540))
    vn = sc.part(P.motion(swing=24, pivot=(60, 132, 800), axis=90.0, rate=0.3, phase=0.7))
    for a in range(4):
        an = math.radians(a * 90.0 + 20)
        tip = (60 + 120 * math.cos(an), 132, 800 + 120 * math.sin(an))
        tilt_box(vn, WH5, (60, 132, 800), tip, 5, 5)
        mid = (60 + 76 * math.cos(an) - 16 * math.sin(an), 132, 800 + 76 * math.sin(an) + 16 * math.cos(an))
        vn.box((SKY, WH5, LCR, PINK)[a], mid, (70, 5, 30), rot=(a * 90.0 + 20, 0, 0))
    kw = dict(tops=FARM, bottoms=FARM_B)
    who(sc, r, -110, -90, 290, -70, build='stocky', hat='cap', hat_col=SKY, arm_l='up', arm_r='up', swing_l=12, swing_r=12, axis_l='x', axis_r='x', rate=0.36, **kw)
    who(sc, r, 20, -90, 290, 90, hat='sun', hat_col=LCR, legs='stride', stride=12, rate=0.8, arm_l='carry', arm_r='carry', hold_r=sack(WH5, SKY), **kw)
    who(sc, r, 150, -80, 290, -10, hair='bun', wear='skirt', arm_l='hold', hold_l=clipboard(WH5, PINK), arm_r='point', swing_r=8, rate=0.5, **kw)


def sail_mender(sc):
    """A small post mill on its trestle: the farmer up a ladder stitching a patchwork sail that rocks in the wind, a
    helper footing the ladder, a child on the trestle's deck holding the thread basket."""
    r = random.Random(342)
    g = sc.still()
    for sx in (-1, 1):
        for sy in (-1, 1):
            tilt_box(g, ASHL, (sx * 190, sy * 140, FOOT), (sx * 60, sy * 50, 300), 14, 14)
    deck(g, WH5, 400, 300, 300, t=16, edge=SKY)
    g.cyl(PINK, (70, 60, 300), 86, 250, steps=8, top_r=70)
    g.sphere(WH5, (70, 60, 550), 72, scale=(1, 1, 0.75), steps=(4, 8))
    g.box(SKY, (70, -22, 380), (50, 8, 110))
    g.box(LCR, (70, -14, 480), (40, 8, 40))
    rod(g, ASHL, (70, 0, 540), (70, -60, 540), 8, steps=6)
    sl = sc.part(P.motion(swing=7, pivot=(70, -60, 540), axis=90.0, rate=0.3, phase=0.1))
    for a in range(4):
        an = math.radians(a * 90.0 + 38)
        tip = (70 + 200 * math.cos(an), -60, 540 + 200 * math.sin(an))
        tilt_box(sl, WH5, (70, -60, 540), tip, 6, 6)
        mid = (70 + 125 * math.cos(an) - 22 * math.sin(an), -62, 540 + 125 * math.sin(an) + 22 * math.cos(an))
        sl.box((SKY, WH5, LCR, SKY)[a], mid, (120, 5, 44), rot=(a * 90.0 + 38, 0, 0))
    for sy in (-1, 1):
        tilt_box(g, ASHL, (-150 + sy * 22, -120, 300), (-60 + sy * 22, -70, 560), 6, 6)
    for k in range(5):
        f = (k + 0.5) / 5.0
        g.box(ASHL, (-150 + 90 * f, -120 + 50 * f, 300 + 260 * f), (50, 6, 6))
    kw = dict(tops=FARM, bottoms=FARM_B)
    who(sc, r, -112, -118, 420, 150, hat='cap', hat_col=LCR, arm_l='up', arm_r='up', swing_r=10, axis_r='x', rate=1.2, **kw)
    who(sc, r, -175, -60, 300, 25, build='stocky', hair='bald', arm_l='push', arm_r='push', **kw)
    who(sc, r, 170, -70, 300, -8, build='kid', s=0.84, hair='pony', arm_l='carry', arm_r='carry', hold_r=sack(LCR, PINK), **kw)
    who(sc, r, 20, -110, 300, 6, wear='skirt', hair='bun', hat='sun', hat_col=WH5, **dict(kw, **WAVE))


def ribbon_stair(sc):
    """A barn's gable and its outside stair: three children with ribbons on sticks, waving them at the chase, and
    their mother on the landing with a basket. The ribbons stay on their sticks."""
    r = random.Random(343)
    g = sc.still()
    open_stair(g, WH5, ASHL, 215, 0, -100, 130, 320, n=5, w=120)
    g.box(ASHL, (215, -100, (130 + FOOT) / 2.0), (14, 110, 130 - FOOT))
    for sx in (-200, 0):
        for sy in (-1, 1):
            g.box(ASHL, (sx, sy * 140, (320 + FOOT) / 2.0), (15, 15, 320 - FOOT))
    deck(g, WH5, 230, 310, 320, t=16, edge=SKY, x=-100)
    g.box(PINK, (-100, 150, 470), (230, 14, 300))                        # the gable: a thin front
    for k in range(5):                                                   # a barrel roof's end in facets
        a0, a1 = math.radians(180.0 * k / 5.0), math.radians(180.0 * (k + 1) / 5.0)
        tilt_box(g, WH5, (-100 - math.cos(a0) * 120, 150, 620 + math.sin(a0) * 80), (-100 - math.cos(a1) * 120, 150, 620 + math.sin(a1) * 80), 22, 10)
    g.box(SKY, (-130, 140, 420), (70, 6, 180))
    for k, c in enumerate((LCR, SKY, WH5, LCR)):
        g.box(c, (-60 + (k % 2) * 34, 140, 470 + (k // 2) * 40), (28, 6, 34))
    rail(g, ASHL, 230, -152, 320, h=95, n=4, x=-100)
    kw = dict(tops=FARM, bottoms=FARM_B, build='kid')
    who(sc, r, 150, -100, 190, 0, s=0.84, hair='pony', arm_l='up', swing_l=30, rate=1.0, hold_l=ribbon(SKY), **kw)
    who(sc, r, 60, -100, 268, 0, s=0.8, hair='afro', arm_l='up', swing_l=30, rate=0.9, hold_l=ribbon(PINK), **kw)
    who(sc, r, -40, -100, 320, 0, s=0.86, hair='short', arm_r='up', swing_r=28, rate=1.1, hold_r=ribbon(LCR), bob=4, **kw)
    who(sc, r, -150, -60, 320, 8, tops=FARM, bottoms=FARM_B, wear='skirt', hat='sun', hat_col=LCR, arm_l='carry', arm_r='carry', hold_r=sack(ROPE, SKY))


# ------------------------------------------------------------------------------------------ d6 Geotherm
COAT = dict(top=WH6, wear='coat', sleeves='long', band=LIME)
ENG_B = (GRAPH, '#2b2350', CONC)


def gauge_deck(sc):
    """A gauge deck on a concrete stalk: engineers in white coats reading three great dials whose needles tremble, one
    writing it down, one pointing; a lime chevron (pointing UP) on the board's foot."""
    r = random.Random(351)
    g = sc.still()
    g.cyl(WH6, (0, 60, FOOT), 44, 290 - FOOT, steps=10, top_r=36)
    g.cyl(LIME, (0, 60, 150), 42, 22, steps=10)
    for sx in (-1, 1):
        tilt_box(g, CONC, (sx * 30, 60, 170), (sx * 200, -120, 276), 12, 12)
        tilt_box(g, CONC, (sx * 30, 60, 170), (sx * 200, 140, 276), 12, 12)
    deck(g, GRAPH, 440, 320, 300, t=20, edge=LIME)
    rail(g, SILV, 440, -156, 300, h=100, n=5)
    g.box(GRAPH, (0, 140, 480), (400, 14, 320))
    for k in range(3):
        x = -130 + k * 130
        g.cyl(WH6, (x, 132, 530), 52, 8, rot=(0, 0, 90), steps=10)
        g.cyl(SILV, (x, 134, 530), 56, 5, rot=(0, 0, 90), steps=10)
        nd = sc.part(P.motion(swing=22, pivot=(x, 124, 530), axis=90.0, rate=0.5 + 0.2 * k, phase=k * 0.3))
        nd.box(GRAPH, (x, 124, 552), (6, 5, 48))
    for sx in (-1, 1):
        tilt_box(g, LIME, (sx * 60, 130, 350), (0, 130, 400), 6, 16)
    kw = dict(bottoms=ENG_B, hat='hard', hat_col=LIME)
    who(sc, r, -150, -60, 300, 170, arm_r='point', swing_r=7, rate=0.5, **dict(COAT, **kw))
    who(sc, r, -30, -90, 300, -6, arm_l='hold', hold_l=clipboard(WH6, LIME), arm_r='hold', **dict(COAT, **kw))
    who(sc, r, 90, -70, 300, 8, hair='bun', hat=None, **dict(COAT, bottoms=ENG_B, **WAVE))
    who(sc, r, 170, 40, 300, 160, arm_l='face', arm_r='hip', **dict(COAT, **kw))


def valve_crew(sc):
    """The valve crew: three in white coats heave a great silver wheel round together, a quarter turn and back, on a
    fat pipe stub that points straight up; a pale puff off the gland each time."""
    r = random.Random(352)
    g = sc.still()
    posts(g, CONC, 440, 300, 280)
    xbrace(g, CONC, 440, -134, 40, 240)
    deck(g, GRAPH, 440, 310, 280, t=20, edge=LIME)
    g.cyl(SILV, (0, 50, 280), 60, 150, steps=10)
    g.cyl(LIME, (0, 50, 400), 70, 20, steps=10)
    g.cyl(SILV, (0, 50, 430), 26, 110, steps=8)
    wh = sc.part(P.motion(swing=40, pivot=(0, 50, 540), axis=P.UP, rate=0.3, phase=0.0))
    for a in range(8):
        a0, a1 = math.radians(a * 45.0), math.radians(a * 45.0 + 45.0)
        tilt_box(wh, SILV, (105 * math.cos(a0), 50 + 105 * math.sin(a0), 540), (105 * math.cos(a1), 50 + 105 * math.sin(a1), 540), 12, 12)
    for a in range(4):
        an = math.radians(a * 45.0)
        tilt_box(wh, LIME, (-105 * math.cos(an), 50 - 105 * math.sin(an), 540), (105 * math.cos(an), 50 + 105 * math.sin(an), 540), 9, 9)
    for k in range(2):
        st = sc.part(P.motion(bob=-16, rate=0.3, phase=0.25 + k * 0.5))
        st.sphere(STEAM, (60 - k * 110, 110, 600 + k * 40), 22 + 6 * k, steps=(3, 6))
    kw = dict(bottoms=ENG_B, hat='hard', hat_col=LIME, arm_l='fwd', arm_r='fwd', rate=0.3, phase=0.0)
    for x, y, yaw in ((-150, 30, 75), (150, 30, -75), (30, -100, 150)):
        d = dict(COAT, **kw)
        who(sc, r, x, y, 280, yaw, sway=dict(swing=5, pivot=(x, y, 280), axis=yaw + 90.0), **d)
    who(sc, r, -150, -110, 280, 10, arm_l='hold', hold_l=clipboard(WH6, LIME), hair='pony', **dict(COAT, bottoms=ENG_B, **WAVE))


def tea_landing(sc):
    """Tea break on the pipe bridge: a fat silver pipe on trestle legs, three engineers sat along it with mugs, legs
    hanging, a kettle on the landing, lime bands round the pipe."""
    r = random.Random(353)
    g = sc.still()
    for x in (-170, 170):
        for sy in (-1, 1):
            tilt_box(g, CONC, (x, sy * 120, FOOT), (x, sy * 30, 300), 13, 13)
        g.box(CONC, (x, 0, 150), (10, 150, 10))
    rod(g, SILV, (-225, 0, 360), (225, 0, 360), 62, steps=10)
    for x in (-110, 110):
        rod(g, LIME, (x - 9, 0, 360), (x + 9, 0, 360), 66, steps=10)
    deck(g, GRAPH, 440, 110, 300, t=12, edge=LIME, y=115)
    rail(g, SILV, 440, 166, 300, h=110, n=5)
    g.cyl(SILV, (150, 120, 300), 20, 26, steps=8, top_r=16)                 # the kettle on its box
    g.box(WH6, (150, 120, 292), (50, 40, 16))
    st = sc.part(P.motion(bob=-10, rate=0.3, phase=0.4))
    st.sphere(STEAM, (158, 120, 360), 14, steps=(3, 6))
    kw = dict(bottoms=ENG_B, legs='sit', seat=46)
    who(sc, r, -120, 0, 364, 5, hat='hard', hat_col=LIME, arm_r='face', hold_r=cup(LIME), swing_r=5, rate=0.3, **dict(COAT, **kw))
    who(sc, r, -20, 0, 364, -5, hair='bun', **dict(COAT, **dict(kw, **WAVE_L)))
    who(sc, r, 80, 0, 364, 8, hat='hard', hat_col=WH6, arm_l='hold', hold_l=cup(WH6), arm_r='point', swing_r=6, rate=0.4, **dict(COAT, **kw))
    who(sc, r, -170, 115, 300, 10, arm_l='hold', arm_r='hold', hold_r=tray(SILV, LIME), hat='hard', hat_col=LIME, **dict(COAT, bottoms=ENG_B))


# ------------------------------------------------------------------------------------------ d7 the Wall Keep
KEEP = (TURQ, SAND, CHALK, '#2f6f9a')
KEEP_B = (NAVY, '#5a4a3c', SAND)


def herald_walk(sc):
    """A length of wall walk on one great arch: two heralds with long straight horns held up along the wall, a short
    banner hanging from each; a guard with a tall turquoise banner; chalk battlements."""
    r = random.Random(361)
    g = sc.still()
    for sx in (-1, 1):
        g.box(SAND, (sx * 180, 30, (250 + FOOT) / 2.0), (80, 200, 250 - FOOT))
    for k in range(5):                                                    # the arch in five stones
        a0, a1 = math.radians(180.0 * k / 5.0), math.radians(180.0 * (k + 1) / 5.0)
        tilt_box(g, CHALK if k == 2 else SAND, (-math.cos(a0) * 150, 30, 250 + math.sin(a0) * 60), (-math.cos(a1) * 150, 30, 250 + math.sin(a1) * 60), 200, 26)
    g.box(SAND, (0, 30, 322), (440, 230, 24))
    for k in range(5):
        g.box(CHALK, (-196 + k * 98, -74, 366), (48, 22, 64))
    g.box(SAND, (0, -74, 344), (440, 22, 20))
    kw = dict(tops=KEEP, bottoms=KEEP_B, sleeves='long', band=OGOLD)
    who(sc, r, -55, 0, 334, 6, hat='cap', hat_col=NAVY, arm_l=((70, 20), (140, 10)), hold_l=horn(OGOLD, TURQ, 140), arm_r='hip', sway=dict(swing=3, pivot=(-55, 0, 334), axis=96.0), rate=0.4, **kw)
    who(sc, r, 55, 0, 334, -6, hat='cap', hat_col=NAVY, arm_r=((70, 20), (140, 10)), hold_r=horn(OGOLD, CHALK, 140), arm_l='hip', sway=dict(swing=3, pivot=(55, 0, 334), axis=84.0), rate=0.4, **kw)
    who(sc, r, -175, 60, 334, 0, hat='helmet', hat_col=SILV, top=TURQ, bottom=NAVY, sleeves='long', arm_r='hold', hold_r=banner(TURQ, OGOLD, 300), swing_r=4, axis_r='y', rate=0.3)
    who(sc, r, 170, 60, 334, -12, build='kid', s=0.84, tops=KEEP, **CHEER)


def market_steps(sc):
    """Market day on a stepped plinth: a cloth seller under a turquoise striped awning holds a length up, a buyer
    feels it, a child with a basket of loaves, sand-stone balls on the corners."""
    r = random.Random(362)
    g = sc.still()
    steps(g, SAND, -120, -168, -40, 280, n=5, w=180, edge=CHALK)
    for sx in (-196, 196):
        for y in (-20, 150):
            g.box(SAND, (sx, y, (280 + FOOT) / 2.0), (40, 40, 280 - FOOT))
    g.box(SAND, (0, 65, 268), (440, 210, 24))
    for sx in (-1, 1):
        g.sphere(SAND, (sx * 196, -20, 312), 30, steps=(4, 7))
        g.box(NAVY, (sx * 150, 150, 440), (9, 9, 320))
        g.box(NAVY, (sx * 150, 0, 420), (9, 9, 280))
    aw = sc.part(P.motion(swing=4, pivot=(0, 150, 600), axis=0.0, rate=0.3, phase=0.3))
    for k in range(6):
        slab(aw, TURQ if k % 2 == 0 else CHALK, 55, 150, 600, -6, 560, 6, x=-138 + k * 55)
    g.box(NAVY, (60, 120, 330), (220, 60, 100))                           # the stall's counter, bolts of cloth on it
    for k, c in enumerate((TURQ, CHALK, OGOLD, '#2f6f9a')):
        rod(g, c, (-20 + k * 52, 105, 392), (-20 + k * 52, 140, 392), 13, steps=6)
    cl = sc.part(P.motion(swing=6, pivot=(100, 40, 470), axis=0.0, rate=0.5, phase=0.6))
    cl.box(TURQ, (100, 40, 420), (90, 5, 100))
    who(sc, r, 150, 55, 280, 0, tops=KEEP, bottoms=KEEP_B, hat='sun', hat_col=SAND, arm_l=((100, 60), (120, 60)), arm_r='hip')
    who(sc, r, 20, 10, 280, 150, tops=KEEP, bottoms=KEEP_B, wear='dress', hair='long', arm_r='fwd', swing_r=6, rate=0.5)
    who(sc, r, -120, -70, 224, 0, tops=KEEP, build='kid', s=0.84, arm_l='carry', arm_r='carry', hold_r=sack(ROPE, SAND))
    who(sc, r, -110, 40, 280, -10, tops=KEEP, bottoms=KEEP_B, sleeves='long', build='stocky', **WAVE)


def mason_scaffold(sc):
    """A timber scaffold with a pulley: a block of sand stone going slowly up on its rope, the mason on the top stage
    tapping a course level, the apprentice hauling, a wall guard below the arch with his banner upright."""
    r = random.Random(363)
    g = sc.still()
    posts(g, PST, 400, 280, 600, w=14)
    xbrace(g, PST, 400, -126, 40, 260)
    xbrace(g, PST, 400, 126, 320, 580)
    deck(g, ROPE, 410, 290, 290, t=14, edge=NAVY)
    deck(g, ROPE, 410, 170, 600, t=14, edge=NAVY, y=60)
    g.box(SAND, (-60, 110, 650), (240, 60, 100))                          # the course being laid
    g.box(CHALK, (-60, 110, 706), (200, 60, 12))
    tilt_box(g, PST, (150, 60, 600), (150, -150, 800), 10, 10)             # the jib
    g.cyl(OGOLD, (150, -150, 790), 16, 12, rot=(0, 0, 90), steps=8)
    bk = sc.part(P.motion(bob=-50, rate=0.15, phase=0.1))
    bk.box(ROPE, (150, -150, 620), (5, 5, 320))
    bk.box(SAND, (150, -150, 430), (70, 50, 54))
    pennant(sc, r, TURQ, -196, 136, 760, w=90, h=40)
    g.box(PST, (-196, 136, 690), (7, 7, 190))
    kw = dict(tops=KEEP, bottoms=KEEP_B, sleeves='long')
    who(sc, r, -90, 30, 600, 170, hat='cap', hat_col=SAND, build='stocky', arm_r=((80, 60), (110, 70)), swing_r=14, axis_r='x', rate=1.2, hold_r=mallet(PST, SILV), **kw)
    who(sc, r, 60, 40, 600, -10, hair='pony', arm_l='hold', arm_r='hold', hold_r=tray(PST, CHALK), **kw)
    who(sc, r, 60, -60, 290, 30, hair='short', arm_l='up', arm_r='up', swing_l=10, swing_r=10, axis_l='x', axis_r='x', rate=0.3, **kw)
    who(sc, r, -120, -80, 290, 6, hat='helmet', hat_col=SILV, top=TURQ, bottom=NAVY, sleeves='long', band=OGOLD, arm_l='hold', hold_l=banner(TURQ, OGOLD, 280), arm_r='wave', swing_r=18, rate=0.8)


# ------------------------------------------------------------------------------------------ d8 the Backlot
CREW8 = (MNT, BUT, WHITE, '#9fd0ff')
CREW8_B = (NAVY, '#3a5fa8', BUT)


def painter_cradle(sc):
    """Scene painters in a cradle slung from two braced posts, rolling a blue sky with white clouds onto a flat; the
    cradle sways a little, the paint tins sway with it, and a third painter below mixes."""
    r = random.Random(371)
    g = sc.still()
    for sx in (-1, 1):
        g.box(NAVY, (sx * 210, 120, (760 + FOOT) / 2.0), (14, 14, 760 - FOOT))
        tilt_box(g, NAVY, (sx * 210, 120, 420), (sx * 210, -120, FOOT), 9, 9)
    g.box(NAVY, (0, 120, 754), (440, 14, 14))
    g.box('#9fd0ff', (0, 134, 520), (400, 8, 440))                        # the flat: a sky
    g.box(BUT, (0, 132, 296), (400, 8, 10))
    for x, z, w in ((-110, 610, 150), (70, 520, 190), (-40, 400, 120)):
        g.box(WHITE, (x, 128, z), (w, 5, 44))
        g.box(WHITE, (x + 20, 128, z + 30), (w * 0.5, 5, 30))
    sw = dict(swing=1.6, pivot=(0, 60, 754), axis=90.0)
    cr = sc.part(P.motion(rate=0.25, phase=0.0, **sw))
    cr.box(MNT, (0, 50, 372), (330, 80, 12))
    cr.box(MNT, (0, 12, 420), (330, 6, 8))
    for sx in (-1, 1):
        cr.box(ROPE, (sx * 160, 60, 565), (5, 5, 380))
        cr.box(MNT, (sx * 160, 12, 400), (6, 6, 50))
    for k, c in enumerate((WHITE, '#9fd0ff', BUT)):
        cr.cyl(c, (-30 + k * 30, 70, 378), 11, 22, steps=6)
    kw = dict(tops=CREW8, bottoms=CREW8_B, hat='cap', hat_col=WHITE, rate=0.25, phase=0.0)
    who(sc, r, -100, 50, 378, 180, arm_r='up', hold_r=roller(BUT, '#9fd0ff'), sway=sw, **kw)
    who(sc, r, 90, 50, 378, 180, arm_l='up', hold_l=roller(BUT, WHITE), hair='pony', sway=sw, **dict(kw, hat=None))
    posts(g, NAVY, 260, 150, 280, w=11, inset=8)
    deck(g, BUT, 260, 160, 280, t=12, edge=MNT, y=-80)
    for sx in (-1, 1):
        g.box(NAVY, (sx * 122, -150, 140), (11, 11, 280 - FOOT + 120))
    who(sc, r, -40, -90, 280, 10, tops=CREW8, bottoms=CREW8_B, arm_l='hold', arm_r='hold', swing_r=10, axis_r='z', rate=0.9, hold_l=tin(MNT))
    who(sc, r, 70, -100, 280, -8, tops=CREW8, bottoms=CREW8_B, hair='afro', arm_r='point', swing_r=7, rate=0.5, arm_l='hip')


def tin(col):
    def f(m, h, side):
        m.cyl(SILV, (h[0], h[1] - 6, h[2] - 4), 13, 24, steps=6)
        m.cyl(col, (h[0], h[1] - 6, h[2] + 18), 12, 6, steps=6)
    return f


def carpenters(sc):
    """Carpenters on a rostrum of trestles: two walk a painted house front on the spot between them (a false window,
    a mint door), one saws at the bench, an A-brace leans ready, clapper stripes along the rostrum's edge."""
    r = random.Random(372)
    g = sc.still()
    posts(g, NAVY, 440, 300, 280, w=13)
    xbrace(g, NAVY, 440, -134, 40, 240)
    deck(g, BUT, 440, 310, 280, t=16)
    for k in range(8):
        g.box(WHITE if k % 2 else NAVY, (-192 + k * 55, -158, 272), (55, 8, 24))
    fl = sc.part(P.motion(swing=2, pivot=(-60, -60, 280), axis=0.0, rate=0.8, phase=0.0))
    fl.box(MNT, (-60, -60, 440), (210, 8, 250))
    fl.box(BUT, (-60, -60, 572), (220, 9, 16))
    fl.box(PANE, (-100, -65, 470), (60, 5, 80))
    fl.box(WHITE, (-100, -66, 470), (6, 5, 80))
    fl.box(NAVY, (-10, -65, 400), (50, 5, 150))
    fl.box(GOLD, (-60, -66, 545), (16, 5, 16), rot=(0, 0, 45))
    kw = dict(tops=CREW8, bottoms=CREW8_B)
    who(sc, r, -190, -60, 280, 90, legs='stride', stride=12, rate=0.8, arm_r='out', arm_l='hip', hat='cap', hat_col=MNT, **kw)
    who(sc, r, 70, -60, 280, 90, legs='stride', stride=12, rate=0.8, arm_l='out', arm_r='hip', build='stocky', hair='bald', **kw)
    table(g, NAVY, 150, 90, 280, L=120, top=86, d=60)
    g.box(BUT, (150, 90, 372), (170, 26, 8))
    who(sc, r, 150, 40, 280, 180, hair='bun', arm_r='push', swing_r=16, axis_r='x', rate=1.4, arm_l='fwd', hold_r=saw(), **kw)
    who(sc, r, 165, -110, 280, -10, build='kid', s=0.86, hair='afro', arm_l='hold', hold_l=tin(MNT), **dict(kw, **WAVE))
    for sx in (-1, 1):
        tilt_box(g, BUT, (-150 + sx * 50, 140, 280), (-150, 110, 520), 8, 8)
    g.box(BUT, (-150, 128, 380), (60, 8, 8))


def saw():
    def f(m, h, side):
        m.box(SILV, (h[0], h[1] - 30, h[2] - 4), (5, 60, 14))
        m.box(BUT, (h[0], h[1], h[2]), (7, 16, 20))
    return f


def extras_queue(sc):
    """Extras in costume queuing on a stepped rostrum for the next set-up: a plumed hat and cape, a cook's hat, a
    deep-sea helmet under one arm, a child in a star cloak; a rail in clapper stripes, one gold star on a post."""
    r = random.Random(373)
    g = sc.still()
    for k, (z, y) in enumerate(((260, -100), (320, 10), (380, 120))):
        for sx in (-1, 1):
            g.box(NAVY, (sx * 210, y, (z + FOOT) / 2.0), (11, 11, z - FOOT))
        g.box(MNT if k % 2 == 0 else BUT, (0, y, z - 7), (440, 106, 14))
    xbrace(g, NAVY, 430, 120, 40, 340)
    for k in range(8):
        g.box(WHITE if k % 2 else NAVY, (-192 + k * 55, 170, 480), (55, 7, 14))
    for sx in (-1, 1):
        g.box(NAVY, (sx * 214, 170, 430), (8, 8, 100))
    g.box(NAVY, (180, 170, 580), (8, 8, 200))
    st = sc.part(P.motion(swing=20, pivot=(180, 170, 690), axis=P.UP, rate=0.2, phase=0.3))
    for a in range(5):
        an = math.radians(a * 72.0 + 90)
        tilt_box(st, GOLD, (180, 164, 690), (180 + 40 * math.cos(an), 164, 690 + 40 * math.sin(an)), 5, 18)
    who(sc, r, -150, -100, 260, 8, top='#3a5fa8', bottom=NAVY, wear='coat', sleeves='long', hat='cowboy', hat_col=BUT, arm_r='wave', swing_r=20, rate=0.9,
        arm_l='hold', hold_l=plume())
    who(sc, r, -40, -100, 260, -6, top=WHITE, bottom=BUT, hat='chef', hat_col=WHITE, build='stocky', arm_l='hold', arm_r='hold', hold_r=clipboard(BUT, NAVY))
    who(sc, r, 80, 10, 320, 4, top=BUT, bottom='#3a5fa8', sleeves='long', hair='short', arm_l='carry', hold_l=helmet_prop(), arm_r='hip')
    who(sc, r, -90, 10, 320, -10, top=MNT, bottom=NAVY, wear='dress', hair='long', hat='sun', hat_col=WHITE, **WAVE_L)
    who(sc, r, 160, -100, 260, -12, build='kid', s=0.84, top=NAVY, wear='coat', bottom=NAVY, band=GOLD, hair='afro', **CHEER)


def plume():
    """A fan of three feathers held at the chest."""
    def f(m, h, side):
        for k, c in enumerate((WHITE, MNT, WHITE)):
            tilt_box(m, c, (h[0], h[1] - 6, h[2]), (h[0] + (k - 1) * 22, h[1] - 6, h[2] + 46), 5, 14)
    return f


def helmet_prop():
    def f(m, h, side):
        m.sphere(OGOLD, (h[0], h[1] - 14, h[2] + 12), 22, steps=(4, 7))
        m.box(PANE, (h[0], h[1] - 34, h[2] + 14), (20, 6, 16))
    return f


# ------------------------------------------------------------------------------------------ the left-hand bends
def lava_pier(sc):
    """A stack of pale basalt columns standing out of the lava channel with a railed top: three watchers who would
    not leave, one with a tall pennant pole, one pointing the chase out to a child. Footed 4 m down."""
    r = random.Random(381)
    g = sc.still()
    cols = (ASHL, '#857a8c', '#5a4e5e', '#a59bb0')
    k = 0
    for x in (-90, -30, 30, 90):
        for y in (-70, -12, 46):
            h = 330 + ((k * 37) % 5) * 22 - (40 if abs(x) > 60 else 0)
            g.cyl(cols[k % 4], (x + (12 if (y + 70) % 116 else 0), y, -400), 37, h + 400, steps=6)
            k += 1
    g.cyl(STONE, (0, -12, 430), 118, 16, steps=8)
    for a in range(8):
        an = math.radians(a * 45.0 + 22.5)
        g.box(STONE, (112 * math.cos(an), -12 + 112 * math.sin(an), 496), (7, 7, 100))
    g.cyl(STONE, (0, -12, 540), 116, 7, steps=8)
    g.box(STONE, (70, 30, 640), (7, 7, 400))
    pennant(sc, r, SKY, 70, 30, 810, w=110, h=50)
    pennant(sc, r, WH5, 70, 30, 752, w=80, h=36, rate=0.55)
    tops = (SKY, WH5, LCR, MNT, '#bfd4ff')
    who(sc, r, -50, -60, 446, 8, tops=tops, sleeves='long', hat='sun', hat_col=LCR, arm_r='point', swing_r=8, rate=0.5)
    who(sc, r, 20, -70, 446, -6, tops=tops, build='kid', s=0.84, **CHEER)
    who(sc, r, -10, 40, 446, 0, tops=tops, sleeves='long', hair='long', pack=SKY, **WAVE)


JOBS = [
    ('SM_S1PEOPLE_BathDeck', bath_deck), ('SM_S1PEOPLE_TeaTerrace', tea_terrace), ('SM_S1PEOPLE_BalconyWatch', balcony_watch),
    ('SM_S1PEOPLE_StormGallery', storm_gallery), ('SM_S1PEOPLE_KilnMouth', kiln_mouth), ('SM_S1PEOPLE_RiggerFrame', rigger_frame),
    ('SM_S1PEOPLE_ShiftWalk', shift_walk), ('SM_S1PEOPLE_CookVeranda', cook_veranda), ('SM_S1PEOPLE_CardStair', card_stair),
    ('SM_S1PEOPLE_SweepSteps', sweep_steps), ('SM_S1PEOPLE_YokeStair', yoke_stair), ('SM_S1PEOPLE_BellTerrace', bell_terrace),
    ('SM_S1PEOPLE_LoadingStage', loading_stage), ('SM_S1PEOPLE_SailMender', sail_mender), ('SM_S1PEOPLE_RibbonStair', ribbon_stair),
    ('SM_S1PEOPLE_GaugeDeck', gauge_deck), ('SM_S1PEOPLE_ValveCrew', valve_crew), ('SM_S1PEOPLE_TeaLanding', tea_landing),
    ('SM_S1PEOPLE_HeraldWalk', herald_walk), ('SM_S1PEOPLE_MarketSteps', market_steps), ('SM_S1PEOPLE_MasonScaffold', mason_scaffold),
    ('SM_S1PEOPLE_PainterCradle', painter_cradle), ('SM_S1PEOPLE_Carpenters', carpenters), ('SM_S1PEOPLE_ExtrasQueue', extras_queue),
    ('SM_S1PEOPLE_LavaPier', lava_pier),
]


def run(only=None):
    P.build_core()                              # loads the two masters; never rebuilt here
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
