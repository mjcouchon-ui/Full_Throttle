"""FULL THROTTLE (Unreal) the population of level 6, Under the Sea (the road to Nereida): /Game/People/UnderTheSea/.

Who lives here: the aquanauts of Nereida (pressure suits in strong colours, white helmets with a dark visor, a pack on
the back) who work the reef, the station and the salvage field and come out to watch the chase; sport divers in the
water over the reef; Nereida's mounted patrol on giant seahorses; and the sea's own: turtles, rose dolphins, an
octopus, crabs, giant jellyfish. The level's schools, rays, sharks and whales are the dressing's (ft_dress_abyss.py)
and are not copied here; nothing here is grey with a white belly and a tall back fin (the boss is GREAT WHITE), and
nothing is the blue of his torpedo fish.

What reads from this road (learned in frames): the road is a raised ribbon with a metre of barrier and the sand lies
0.9 m under it, so whatever stands on the sand by the barrier is hidden to the waist or altogether. But this is water:
people SWIM. Every piece here carries its people 2.5 to 12 m over the sand (swimming, on a stand, on a scaffold, on
a lift bag), in suits of yellow, orange, pink, white and lime against the blue, and moves them: slow floats (bob < 0),
kicking fins, travel along the road. Such pieces are TALL: 24 m or more from the centre line.

A piece is drawn like every prop: cm; z = 0 is the sand; the middle of the footprint is the origin; the FRONT faces -Y
(the road); long pieces run along X (the road).

Run in the editor:  import ft_people_sea; ft_people_sea.run()        (or run(["SM_DiveTeam_A"]))
The placing rules: Source/FullThrottle/FTPeopleL6.inl.
"""
import math
import random
import unreal
import ft_people as P
import ft_people_crew as C
from ft_desert import rod, tilt_box

DIR = '/Game/People/UnderTheSea'

# the suits of Nereida and of the divers: strong against blue water, never blue
SUITS = ('#ffc93c', '#ff7a3c', '#ff5fa2', '#fff6ea', '#b8ff5a', '#ff2d3c', '#f4a6ff', '#7dffb0')
FINS = ('#ff2d3c', '#ffc93c', '#22202a', '#ff5fa2', '#fff6ea', '#b8ff5a')
HELM, VISOR, STEEL, DARK, WHITE, YEL = '#f4f9ff', '#22202a', '#8f9bb3', '#22202a', '#fff6ea', '#ffc93c'
ROCK, ROCK_D, ROCK_L = '#9a6a7e', '#6a4562', '#c79aa2'          # the level's own rock (ft_dress_abyss.py)
SAND_D = '#b9a77e'
BUBBLE = '#e6f6ff'
DECK, DECK_D = '#cfe3ea', '#7fa2b4'                            # the station's metal
CORAL, CORAL2, MAG = '#ff7a6a', '#ffb04f', '#ff4fb8'


# ------------------------------------------------------------------------------------------ a swimmer
def _frame(x, y, z, heading, pitch):
    """A swimmer's own frame: forward (along the body, towards the head), right, up. Returns f(a, b, c) -> a point."""
    h, p = math.radians(heading), math.radians(pitch)
    f = (math.cos(h) * math.cos(p), math.sin(h) * math.cos(p), math.sin(p))
    rt = (-math.sin(h), math.cos(h), 0.0)
    u = (-math.cos(h) * math.sin(p), -math.sin(h) * math.sin(p), math.cos(p))

    def W(a, b=0.0, c=0.0):
        return (x + f[0] * a + rt[0] * b + u[0] * c, y + f[1] * a + rt[1] * b + u[1] * c, z + f[2] * a + rt[2] * b + u[2] * c)
    return W


def swimmer(sc, x, y, z, heading=0.0, pitch=0.0, s=1.0, suit=SUITS[0], fins=FINS[0], tank=STEEL, helmet=True, skin='#eebf98',
            arms='fwd', kick=18.0, rate=1.1, phase=0.0, bob=-8.0, travel=None, orbit=None, react=False, wave=0.0, band=None,
            bubbles=True, hold=None, sway=None):
    """One person swimming, belly down, the middle of the hips at (x, y, z), the head towards `heading` (degrees in the
    ground plane, 0 = +X) and raised by `pitch`. helmet: an aquanaut's white helmet with a dark visor (False: a diver's
    hood and mask). arms: 'fwd' (stretched ahead), 'side' (along the body), 'tow' (both ahead and down, holding),
    'wave' (the right arm up: give wave degrees). kick: degrees the legs swing, one against the other. bob < 0: a float.
    travel: (length cm, speed cm/s) along the mesh's X (heading 0 with a positive speed, 180 with a negative).
    hold: f(model, hand point W) draws what the hands hold (it moves with the body). About 170 triangles."""
    W0 = _frame(x, y, z, heading, pitch)

    def W(a, b=0.0, c=0.0):
        return W0(a * s, b * s, c * s)

    tv, base = (0.0, 0.0), (x, y)
    if travel:
        tv = (float(travel[0]), float(travel[1]) / float(travel[0]))
    elif orbit:
        base, tv = (orbit[0], orbit[1]), (-1.0, float(orbit[2]))
    common = dict(phase=phase, rate=rate, bob=bob, base=base, travel=tv, react=react)
    piv = (x, y, z)
    if sway:            # the whole swimmer swings about one pivot (sliding with a sled); the limbs keep still
        common.update(sway)
        kick, wave, bubbles = 0.0, 0.0, False
        b = sc.part(P.motion(**common))
    else:
        b = sc.part(P.motion(pivot=piv, **common))
    tilt_box(b, suit, W(-6), W(60), 46 * s, 26 * s)
    if band:
        tilt_box(b, band, W(34), W(44), 48 * s, 28 * s)
    tilt_box(b, tank, W(2, 0, 22), W(54, 0, 22), 20 * s, 18 * s)
    hd = W(82, 0, 6)
    if helmet:
        b.sphere(HELM, hd, 27 * s, steps=(4, 8))
        tilt_box(b, VISOR, W(92, 0, -8), W(108, 0, 0), 34 * s, 16 * s)
    else:
        b.sphere(suit, hd, 24 * s, steps=(4, 8))
        tilt_box(b, skin, W(90, 0, -6), W(104, 0, 0), 28 * s, 22 * s)
        tilt_box(b, VISOR, W(96, 0, 0), W(108, 0, 4), 30 * s, 10 * s)
    sh = [W(52, sd * 28, 0) for sd in (-1, 1)]
    hands = []
    for k, sd in enumerate((-1, 1)):
        if arms == 'fwd':
            hand = W(118, sd * 22, 4)
        elif arms == 'side':
            hand = W(-6, sd * 34, -4)
        elif arms == 'tow':
            hand = W(104, sd * 14, -26)
        else:   # 'wave': the left ahead, the right straight up
            hand = W(118, sd * 22, 4) if sd < 0 else W(60, 34, 62)
        m = b
        if arms == 'wave' and sd > 0 and wave:
            m = sc.part(P.motion(swing=wave, pivot=sh[k], axis=heading, **common))
        tilt_box(m, suit, sh[k], hand, 13 * s, 13 * s)
        m.box(WHITE if helmet else skin, hand, (15 * s, 15 * s, 15 * s))
        hands.append(hand)
    if hold:
        hold(b, W)
    for sd in (-1, 1):
        hip = W(-4, sd * 11, 0)
        m = sc.part(P.motion(swing=sd * kick, pivot=hip, axis=heading + 90.0, **common)) if kick else b
        tilt_box(m, suit, hip, W(-80, sd * 12, 0), 17 * s, 17 * s)
        tilt_box(m, fins, W(-78, sd * 13, 0), W(-132, sd * 15, -4), 30 * s, 6 * s)
    if bubbles:
        for k in range(3):
            bb = sc.part(P.motion(pivot=piv, phase=phase + 0.31 * k, rate=0.6, bob=-22.0, base=base, travel=tv))
            bb.sphere(BUBBLE, W(70 - k * 34, (k % 2) * 16 - 8, 80 + k * 70), (12 + 5 * k) * s, steps=(4, 6))
    return W


def hold_scooter(col=YEL):
    """A diver's scooter held ahead: a fat tube, a shroud, the handle."""
    def f(m, W):
        rod(m, col, W(100, 0, -34), W(176, 0, -34), 17, steps=8)
        rod(m, DARK, W(176, 0, -34), W(190, 0, -34), 9, steps=6)
        rod(m, DARK, W(86, 0, -34), W(100, 0, -34), 24, steps=8)
    return f


def hold_camera_housing():
    """An underwater camera in its housing with two lamp arms (unlit: it may be anywhere)."""
    def f(m, W):
        tilt_box(m, DARK, W(110, 0, -30), W(150, 0, -30), 34, 30)
        rod(m, STEEL, W(150, 0, -30), W(164, 0, -30), 13, steps=6)
        for sd in (-1, 1):
            tilt_box(m, STEEL, W(128, sd * 14, -24), W(140, sd * 46, -4), 5, 5)
            m.sphere(WHITE, W(142, sd * 48, -2), 9, steps=(3, 5))
    return f


def aquanaut(r, **fixed):
    """An aquanaut of Nereida standing: figure() values. A suit of one strong colour, the white helmet, a pack."""
    suit = r.choice(SUITS)
    d = P.look(r, sleeves='long', wear='trousers', hat='helmet', hat_col=HELM, pack=r.choice((STEEL, YEL, DARK)))
    d.update(top=suit, bottom=suit, shoes=r.choice((DARK, WHITE)), band=r.choice((None, WHITE, DARK, '#2ee6ff')), s=r.uniform(0.92, 1.0))
    d.update(fixed)
    return d


def crowd_aquanaut(sc, r, x, y, z, yaw=0.0, s=1.0, **kw):
    """An aquanaut for a crowd, about 120 triangles: figure() at detail 0 with the head itself as the white helmet,
    and the dark visor drawn on (it moves with the body: the same motion values as figure()'s own body part)."""
    suit = r.choice(SUITS)
    d = dict(skin=HELM, hair='bald', hat=None, top=suit, bottom=suit, shoes=DARK, sleeves='long', wear='trousers', build=r.choice(('avg', 'stocky', 'avg')),
             pack=r.choice((STEEL, YEL, DARK)), detail=0, s=s)
    d.update(kw)
    P.figure(sc, x, y, z=z, yaw=yaw, **d)
    if d.get('rock') or d.get('turn') or d.get('sway') or d.get('travel'):
        return
    mo = P.motion(phase=d.get('phase', 0.0), rate=d.get('rate', 0.0), bob=d.get('bob', 0.0), react=d.get('react', False), base=(x, y),
                  pivot=(x, y, z + d['s'] * 100.0), axis=yaw)
    v = sc.part(mo, (x, y, z), yaw, d['s'])
    v.box(VISOR, (0, -21, 149), (34, 8, 16))


def _banner(sc, x, y, z0, h, col, stripe, phase=0.0, w=110.0):
    """A tall pennant on a pole: big flat colour that reads from far off; it swings a little about the pole."""
    g = sc.still()
    g.box(DECK_D, (x, y, z0 + h / 2.0), (8, 8, h))
    f = sc.part(P.motion(phase=phase, rate=0.45, swing=9.0, pivot=(x, y, z0 + h), axis=P.UP))
    f.box(col, (x + w / 2.0 + (4 if w > 0 else -4), y, z0 + h - 120), (abs(w), 5, 240))
    f.box(stripe, (x + w / 2.0 + (4 if w > 0 else -4), y - 3.5, z0 + h - 120), (abs(w), 2, 60))


def _rock(m, r, cx, cy, w, d, h, n=5):
    """A ledge of the level's rose rock: overlapping flat lumps, the top at h."""
    for k in range(n):
        x = cx + (k + 0.5 - n / 2.0) * w / n + r.uniform(-20, 20)
        ww, dd = w / n * r.uniform(1.3, 1.7), d * r.uniform(0.8, 1.0)
        hh = h * (1.0 if k % 2 == 0 else r.uniform(0.82, 0.95))
        m.box(ROCK if k % 2 else ROCK_D, (x, cy + r.uniform(-10, 10), hh / 2.0 - 20), (ww, dd, hh + 40), rot=(0, r.uniform(-8, 8), 0))
    m.box(ROCK_L, (cx, cy, h - 3), (w * 0.86, d * 0.7, 8))


# ------------------------------------------------------------------------------------------ the pieces
def dive_team(seed=1, n=5, lead='scooter', length=2600.0, speed=150.0, sign=1):
    """Divers swimming along the road in a loose line, 3 to 7 m over the sand: kicking fins, bubbles, a leader with a
    scooter (or a camera housing), one who turns to wave at the chase."""
    def build(sc):
        r = random.Random(seed)
        head = 0.0 if sign > 0 else 180.0
        tv = (length, sign * speed)
        for k in range(n):
            x = (-(n - 1) / 2.0 + k) * 330.0 * sign + r.uniform(-40, 40)
            y = r.uniform(-120, 120)
            z = 330.0 + ((k * 3) % n) * 85.0 + r.uniform(-20, 20)
            suit = SUITS[(k * 3 + seed) % len(SUITS)]
            kw = dict(heading=head, pitch=r.uniform(-4, 10), s=r.uniform(1.25, 1.45), suit=suit, fins=FINS[(k + seed) % len(FINS)],
                      helmet=(k + seed) % 3 != 0, skin=r.choice(P.SKINS), rate=r.uniform(0.9, 1.3), phase=r.random(), bob=-r.uniform(8, 16),
                      travel=tv, tank=r.choice((STEEL, YEL, WHITE)), band=r.choice((None, WHITE, DARK)))
            if k == n - 1 and lead == 'scooter':
                swimmer(sc, x, y, z, arms='tow', kick=6.0, hold=hold_scooter(), **kw)
            elif k == n - 1 and lead == 'camera':
                swimmer(sc, x, y, z, arms='tow', hold=hold_camera_housing(), **kw)
            elif k == 1:
                swimmer(sc, x, y, z, arms='wave', wave=26.0, **kw)
            else:
                swimmer(sc, x, y, z, arms=r.choice(('fwd', 'side', 'side')), **kw)
    return build


def watchers(seed=1, n=13):
    """Nereida has come out to watch: aquanauts on a stand of three steps on stilts (the decks 2.4, 3.2 and 4.0 m over
    the sand, clear of the barrier), arms up, floating up off the deck and settling, two more hanging in the water
    over it with pennants. Striped pennants on the rail. They liven up as the chase comes by."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        L = 860.0
        decks = ((-110.0, 240.0), (0.0, 320.0), (110.0, 400.0))
        for y, z in decks:
            g.box(DECK, (0, y, z - 8), (L, 116, 16))
            g.box(DECK_D, (0, y - 56, z - 30), (L, 6, 44))
        for sx in (-1, 0, 1):
            for y, z in decks:
                g.box(DECK_D, (sx * (L / 2.0 - 30), y, (z - 16) / 2.0 - 10), (16, 16, z - 16 + 20))
        g.box(DECK_D, (0, -166, 240 + 96), (L, 6, 6))                      # the front rail
        for k in range(8):
            xx = -L / 2.0 + (k + 0.5) * L / 8.0
            g.box(DECK_D, (xx - L / 16.0, -166, 240 + 48), (6, 6, 96))
            g.box((MAG, WHITE, YEL, WHITE)[k % 4], (xx, -168, 240 + 62), (L / 8.0 - 8, 4, 44))
        g.box(DECK_D, (L / 2.0, -166, 240 + 48), (6, 6, 96))
        acts = (
            dict(arm_l='up', arm_r='up', swing_l=16, swing_r=-16, bob=-14, rate=0.7),
            dict(arm_l='vee', arm_r='vee', swing_l=12, swing_r=-12, bob=-18, rate=0.6),
            dict(arm_r='wave', swing_r=26, arm_l='hip', bob=-8, rate=0.9),
            dict(arm_r='point', swing_r=10, axis_r='x', arm_l='down', bob=-6, rate=0.6),
            dict(arm_l='wave', swing_l=24, arm_r='up', swing_r=-12, bob=-12, rate=0.8),
        )
        per = (4, 4, 3) if n >= 11 else (4, 3, 3)
        i = 0
        for row, (y, z) in enumerate(decks):
            for k in range(per[row]):
                x = -L / 2.0 + 100 + (k + 0.5 * (row % 2)) * (L - 200) / (per[row] - 0.5) + r.uniform(-14, 14)
                d = dict(acts[(i * 3 + seed) % len(acts)])
                if i % 6 == 4:
                    d.update(build='kid', s=0.8)
                crowd_aquanaut(sc, r, x, y + r.uniform(-10, 10), z + 16, yaw=r.uniform(-14, 14), s=d.pop('s', r.uniform(1.12, 1.22)), phase=r.random(),
                               react=True, **d)
                i += 1
        # two hang in the water over the stand, each with a pennant
        for sx, col in ((-1, '#ff2d3c'), (1, YEL)):
            crowd_aquanaut(sc, r, sx * 300, 40, 640 + (sx > 0) * 70, yaw=sx * -12, s=1.25, arm_r='up', arm_l='out', hold_r=P.hold_flag(col=col, w=110.0),
                           swing_r=14, bob=-30, rate=0.35, phase=r.random())
        _banner(sc, -L / 2.0 - 40, 120, 0, 780, YEL, '#ff2d3c', 0.1)
        _banner(sc, L / 2.0 + 40, 120, 0, 700, WHITE, '#ff7a3c', 0.6, w=-110.0)
    return build


def _turtle(sc, x, y, z, heading, s, shell, skin, trim, phase, rate, travel, flap=30.0, bob=-14.0):
    W0 = _frame(x, y, z, heading, 6.0)

    def W(a, b=0.0, c=0.0):
        return W0(a * s, b * s, c * s)
    tv = (float(travel[0]), float(travel[1]) / float(travel[0])) if travel else (0.0, 0.0)
    common = dict(phase=phase, rate=rate, bob=bob, base=(x, y), travel=tv)
    b = sc.part(P.motion(pivot=(x, y, z), **common))
    b.sphere(shell, W(0, 0, 6), 100 * s, scale=(1.25, 1.0, 0.42), steps=(4, 8), rot=(6.0, heading, 0))
    b.sphere(trim, W(0, 0, -6), 96 * s, scale=(1.2, 0.96, 0.22), steps=(3, 8), rot=(6.0, heading, 0))
    tilt_box(b, trim, W(-70, 0, 34), W(70, 0, 40), 46 * s, 10 * s)                       # the ridge along the shell
    tilt_box(b, skin, W(100, 0, 0), W(150, 0, 12), 30 * s, 26 * s)                       # the neck
    b.sphere(skin, W(170, 0, 18), 30 * s, scale=(1.2, 1, 0.9), steps=(3, 6))
    for sd in (-1, 1):
        b.box(DARK, W(182, sd * 22, 24), (7 * s, 7 * s, 7 * s))
    tilt_box(b, skin, W(-118, 0, -2), W(-160, 0, -8), 18 * s, 10 * s)                    # the tail
    for sd in (-1, 1):
        fl = sc.part(P.motion(swing=sd * flap, pivot=W(66, sd * 70, 0), axis=heading, **common))
        tilt_box(fl, skin, W(70, sd * 70, 0), W(40, sd * 150, -4), 52 * s, 9 * s)
        tilt_box(fl, skin, W(40, sd * 150, -4), W(-30, sd * 215, -10), 40 * s, 8 * s)
        rf = sc.part(P.motion(swing=-sd * flap * 0.4, pivot=W(-80, sd * 60, 0), axis=heading, **common))
        tilt_box(rf, skin, W(-80, sd * 60, -2), W(-140, sd * 104, -8), 40 * s, 8 * s)


def turtles(seed=1, sign=1, length=3000.0):
    """Sea turtles, big ones (the shell 2.5 to 3.5 m: at 120 mph a true one is a speck), swimming along the road 4 to
    9 m over the sand, their long flippers beating slowly; a small one follows."""
    def build(sc):
        r = random.Random(seed)
        head = 0.0 if sign > 0 else 180.0
        cols = (('#c9853a', '#d7e38a', '#f1e4c8'), ('#b8a23c', '#e8d79a', '#fff1c0'), ('#d96a3c', '#e8c98a', '#f1e4c8'))
        for k, (x, y, z, s) in enumerate(((300, -40, 520, 1.5), (-500, 120, 760, 1.2), (-60, 60, 380, 0.7), (-1100, -80, 600, 1.35))):
            sh, sk, tr = cols[(k + seed) % 3]
            _turtle(sc, x * sign, y, z, head, s, sh, sk, tr, r.random(), r.uniform(0.38, 0.5), (length, sign * 110.0))
    return build


def _crab(sc, x, y, z, s, shell, claw, phase, rate, travel, yaw=0.0):
    """A crab facing the road (-Y), walking sideways along X. s = 1: 125 cm across the legs, 75 cm to the claws' tips."""
    tv = (float(travel[0]), float(travel[1]) / float(travel[0])) if travel else (0.0, 0.0)
    common = dict(phase=phase, rate=rate, base=(x, y), travel=tv)
    cy, sy = math.cos(math.radians(yaw)), math.sin(math.radians(yaw))

    def M(px, py, pz):      # the crab's frame -> the mesh's (pivots are always in the mesh's space)
        return (x + s * (px * cy - py * sy), y + s * (px * sy + py * cy), z + s * pz)
    b = sc.part(P.motion(pivot=(x, y, z), bob=4.0, **common), (x, y, z), yaw, s)
    b.box(shell, (0, 0, 30), (70, 46, 26))
    b.box('#fff1c0', (0, -6, 19), (56, 38, 8))
    for sd in (-1, 1):
        b.box(WHITE, (sd * 15, -20, 52), (13, 11, 22))
        b.box(DARK, (sd * 15, -26, 56), (7, 3, 8))
        lg = sc.part(P.motion(pivot=M(sd * 30, 0, 30), swing=sd * 12.0, axis=yaw + 90.0, bob=4.0, **common), (x, y, z), yaw, s)
        for j in range(3):
            tilt_box(lg, shell, (sd * 30, -12 + j * 15, 34), (sd * 62, -14 + j * 19, 0), 8, 8)
        cl = sc.part(P.motion(pivot=M(sd * 30, -18, 30), swing=sd * 24.0, axis=yaw + 90.0, phase=phase + 0.25 * sd, rate=rate * 0.5, base=(x, y), travel=tv),
                     (x, y, z), yaw, s)
        tilt_box(cl, shell, (sd * 30, -18, 30), (sd * 48, -34, 50), 11, 11)
        cl.box(claw, (sd * 52, -38, 62), (28, 16, 30))
        cl.box(claw, (sd * 58, -38, 84), (12, 14, 16))


def crab_row(seed=1, n=4):
    """Crabs on a ledge of rock by the barrier: big red and orange ones (over a metre across) walking sideways along it,
    claws up and snapping at the traffic. LOW: the whole piece stays under 2 m."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        L = 900.0
        _rock(g, r, 0, 0, L, 200, 84)
        cols = (('#ff2d3c', '#ff7a3c'), ('#ff7a3c', '#ffc93c'), ('#ff3c2d', '#fff1c0'), ('#ffc93c', '#ff2d3c'))
        for k in range(n):
            x = -L / 2.0 + 110 + k * (L - 220) / (n - 1)
            sh, cl = cols[(k + seed) % len(cols)]
            s = r.uniform(1.0, 1.18)
            _crab(sc, x, r.uniform(-30, 20), 84, s, sh, cl, r.random(), r.uniform(1.4, 2.0), (L - 200, (1 if k % 2 else -1) * r.uniform(30, 50)),
                  yaw=r.uniform(-12, 12))
    return build


def _lift_bag(m, x, y, z, col, s=1.0):
    """A salvage lift bag: a fat teardrop, open at the bottom, straps down to the load at z."""
    m.sphere(col, (x, y, z + 300 * s), 120 * s, scale=(1, 1, 1.15), steps=(4, 8))
    m.cyl(col, (x, y, z + 150 * s), 60 * s, 110 * s, steps=8, top_r=112 * s)
    m.cyl(WHITE, (x, y, z + 262 * s), 122 * s, 16 * s, steps=8)
    for a in (0.6, 2.7, 4.8):
        tilt_box(m, DARK, (x + math.cos(a) * 54 * s, y + math.sin(a) * 54 * s, z + 156 * s), (x + math.cos(a) * 40 * s, y + math.sin(a) * 40 * s, z), 5, 5)


def aqua_weld(seed=1):
    """Aquanauts at work on a pipe junction, on a scaffold 2.6 m over the sand: one welds (a small steady light at the
    torch: the piece stands 24 m and more from the road), one carries a case along the deck, one has stopped to wave
    at the chase, one hangs in the water above on a line with a tool."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        L, Z = 760.0, 260.0
        g.box(DECK, (0, 0, Z - 8), (L, 220, 16))
        for sx in (-1, 1):
            for sy in (-1, 1):
                g.box(DECK_D, (sx * (L / 2.0 - 20), sy * 96, Z / 2.0 - 20), (14, 14, Z + 20))
            tilt_box(g, DECK_D, (sx * (L / 2.0 - 20), -96, 20), (sx * (L / 2.0 - 20), 96, Z - 30), 8, 8)
        tilt_box(g, DECK_D, (-L / 2.0 + 20, -98, 20), (L / 2.0 - 20, -98, Z - 30), 8, 8)
        for k in range(6):
            g.box(YEL if k % 2 else DARK, (-L / 2.0 + (k + 0.5) * L / 6.0, -110, Z - 8), (L / 6.0, 5, 18))
        # the junction: a fat pipe along the back of the deck, a riser, a valve wheel
        rod(g, DECK_D, (-L / 2.0 - 60, 60, Z + 70), (L / 2.0 + 60, 60, Z + 70), 46, steps=8)
        for x in (-250, 20, 280):
            rod(g, YEL, (x - 12, 60, Z + 70), (x + 12, 60, Z + 70), 56, steps=8)
        g.cyl(DECK_D, (20, 60, Z + 70), 34, 190, steps=8)
        g.cyl(MAG, (20, 60, Z + 262), 62, 12, steps=8)
        g.box(MAG, (20, 60, Z + 268), (130, 12, 12))
        # the welder stands at the pipe; the torch's light is steady
        P.figure(sc, -200, -10, z=Z, yaw=170, arm_r='fwd', arm_l='hold', swing_r=6, axis_r='x', rate=0.8, bob=-2, phase=r.random(),
                 hold_r=P.hold_tool(col=STEEL, grip=DARK, length=30.0), **aquanaut(r, top='#ff7a3c', bottom='#ff7a3c'))
        g.sphere('#fff1c0', (-204, 34, Z + 96), 9, steps=(3, 5), glow=1.2)
        P.figure(sc, 0, -50, z=Z, yaw=90, arm_l='carry', arm_r='carry', hold_r=P.hold_box(col=YEL), stride=20, rate=0.7, bob=-4, phase=r.random(),
                 travel=(L - 160, 55), **aquanaut(r, top='#ff5fa2', bottom='#ff5fa2'))
        P.figure(sc, 260, -70, z=Z, yaw=-10, arm_r='wave', swing_r=28, arm_l='hip', rate=1.0, bob=-8, phase=r.random(), react=True,
                 **aquanaut(r, top='#b8ff5a', bottom='#b8ff5a'))
        # the one on the line
        g.box(DARK, (140, 60, Z + 250), (5, 5, 320))
        swimmer(sc, 150, 10, Z + 400, heading=200, pitch=50, s=1.2, suit='#ffc93c', fins='#ff2d3c', arms='wave', wave=20.0, kick=10.0, rate=0.5,
                phase=r.random(), bob=-22.0, band=WHITE)
        C.case(g, -330, -40, z=Z, col=YEL)
        g.box(ROCK_D, (300, 150, 30), (160, 120, 60))
        g.box(DECK_D, (-120, 170, 45), (120, 90, 90))
    return build


def aqua_lift(seed=1):
    """A lift: a crate and a chest coming up from the bottom under two lift bags, sinking and rising slowly; two divers
    swim beside the load to steady it, a third on a stack of crates below points up at it."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        ph, rt, bb = r.random(), 0.11, -70.0
        ld = sc.part(P.motion(phase=ph, rate=rt, bob=bb, pivot=(0.0, 0.0, 500.0)))
        ld.box('#c8683a', (-20, 0, 430), (230, 150, 130))
        ld.box('#7a3f2a', (-20, 0, 430), (236, 30, 136))
        ld.box(YEL, (150, 20, 400), (110, 80, 70))
        ld.box('#7a3f2a', (150, 20, 438), (116, 86, 10))
        _lift_bag(ld, -90, 0, 500, YEL, 1.25)
        _lift_bag(ld, 110, 20, 470, '#ff5fa2', 1.0)
        swimmer(sc, -330, -60, 520, heading=10, pitch=14, s=1.3, suit='#fff6ea', fins='#ff2d3c', arms='fwd', kick=8.0, rate=rt, phase=ph, bob=bb,
                band='#ff2d3c', bubbles=False)
        swimmer(sc, 330, 70, 640, heading=190, pitch=20, s=1.3, suit='#b8ff5a', fins='#22202a', arms='fwd', kick=8.0, rate=rt, phase=ph, bob=bb,
                helmet=False, skin=P.SKINS[3], bubbles=False)
        for k in range(4):
            bub = sc.part(P.motion(phase=0.25 * k, rate=0.5, bob=-30.0, pivot=(0.0, 0.0, 900.0)))
            bub.sphere(BUBBLE, (-90 + k * 60, (k % 2) * 40 - 20, 930 + k * 70), 12 + 4 * k, steps=(3, 5))
        for k, col in enumerate(('#c8683a', '#8f6450', '#c8683a')):
            g.box(col, (360 + (k % 2) * 30, 150, 45 + k * 92), (150, 130, 90))
        P.figure(sc, 370, 150, z=276, yaw=60, arm_r='up', swing_r=10, arm_l='hip', rate=0.5, bob=-3, **aquanaut(r, top='#ff7a3c', bottom='#ff7a3c'))
        g.box('#8f6450', (-300, 120, 40), (170, 120, 80), rot=(0, 20, 0))
    return build


def _dolphin(sc, x, y, z, heading, s, body, belly, phase, rate, travel, arc=24.0, radius=700.0):
    """A rose dolphin: a long beak, a round forehead, a small swept fin, wide flukes. It rides over a hill as it goes
    (a swing about a point far under it)."""
    W0 = _frame(x, y, z, heading, 0.0)

    def W(a, b=0.0, c=0.0):
        return W0(a * s, b * s, c * s)
    tv = (float(travel[0]), float(travel[1]) / float(travel[0])) if travel else (0.0, 0.0)
    m = sc.part(P.motion(phase=phase, rate=rate, swing=arc, pivot=(x, y, z - radius), axis=heading + 90.0, base=(x, y), travel=tv))
    m.sphere(body, W(0, 0, 0), 50 * s, scale=(2.3, 0.62, 0.7), steps=(4, 8), rot=(0, heading, 0))
    m.sphere(belly, W(8, 0, -9), 46 * s, scale=(2.1, 0.56, 0.52), steps=(3, 8), rot=(0, heading, 0))
    tilt_box(m, body, W(-80, 0, 2), W(-150, 0, -4), 20 * s, 26 * s)
    tilt_box(m, body, W(-146, 0, -4), W(-176, 0, -4), 100 * s, 7 * s)
    m.sphere(body, W(92, 0, 8), 25 * s, steps=(3, 6))
    tilt_box(m, belly, W(104, 0, 0), W(152, 0, -3), 13 * s, 11 * s)
    tilt_box(m, body, W(-4, 0, 30), W(-40, 0, 58), 7 * s, 30 * s)
    for sd in (-1, 1):
        tilt_box(m, body, W(46, sd * 24, -14), W(14, sd * 58, -36), 22 * s, 6 * s)
        m.box(DARK, W(104, sd * 21, 10), (6 * s, 4 * s, 6 * s))


def dolphins(seed=1, n=5, sign=1, length=5000.0):
    """A pod of rose dolphins going along the road 7 to 13 m up, one after another over the same hill."""
    def build(sc):
        r = random.Random(seed)
        head = 0.0 if sign > 0 else 180.0
        cols = (('#ff4f9a', '#fff1e6'), ('#ff6a5a', '#fff6ea'), ('#ff5fa2', '#ffe9c8'))      # hot: the water greys a pale pink to a shark's colour
        for k in range(n):
            x = (-(n - 1) / 2.0 + k) * 420.0 * sign + r.uniform(-60, 60)
            body, belly = cols[k % 3]
            _dolphin(sc, x, r.uniform(-260, 260), 700 + r.uniform(0, 600), head, r.uniform(1.25, 1.6) if k != 2 else 0.85, body, belly,
                     phase=k * 0.17 + r.uniform(0, 0.05), rate=0.3, travel=(length, sign * 520.0))
    return build


def octopus(seed=1, body='#ff5fa2', pale='#ffc0dc', spot='#ffc93c'):
    """The octopus of the reef: on a mound of rock, her mantle 5 m up and breathing, eight arms curling and uncurling,
    one raised to wave at whatever goes by. One great animal."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        _rock(g, r, 0, 60, 760, 520, 150, n=5)
        g.box(ROCK_D, (-60, 40, 170), (360, 300, 120), rot=(0, 20, 0))
        hd = sc.part(P.motion(rate=0.22, bob=-10.0, pivot=(0.0, 0.0, 300.0)))
        hd.sphere(body, (0, 70, 420), 150, scale=(1.0, 1.25, 1.3), steps=(5, 9), rot=(-24, 0, 0))
        hd.sphere(body, (0, -30, 290), 120, scale=(1.15, 1.0, 0.8), steps=(4, 8))
        for k in range(6):
            a = k * 1.05 + 0.4
            hd.sphere(spot, (math.cos(a) * 100, 100 + math.sin(a) * 60, 500 + (k % 3) * 34), 24, scale=(1, 1, 0.5), steps=(3, 5))
        for sd in (-1, 1):
            hd.sphere(WHITE, (sd * 70, -110, 330), 40, steps=(3, 6))
            hd.box(DARK, (sd * 70, -146, 330), (34, 8, 14))
            hd.box(body, (sd * 70, -118, 366), (84, 60, 14))
        for k in range(8):
            a = math.radians(-90 + (k - 3.5) * 45.0)
            dx, dy = math.cos(a), math.sin(a)
            root = (dx * 96, -20 + dy * 96, 250)
            raised = k == 4
            arm = sc.part(P.motion(phase=k * 0.29 + r.uniform(0, 0.1), rate=r.uniform(0.22, 0.34) if not raised else 0.7, swing=(26.0 if raised else r.uniform(10, 16)),
                                   pivot=root, axis=(math.degrees(a) + 90.0) if not raised else math.degrees(a)))
            pts = [root]
            reach = r.uniform(520, 680)
            for j in range(1, 6):
                t = j / 5.0
                if raised:
                    pts.append((root[0] + dx * 150 * t, root[1] + dy * 150 * t, 250 + 420 * t + 40 * math.sin(t * 3.0)))
                else:
                    curl = 110 * math.sin(t * math.pi) * (1 if k % 2 else -1)
                    pts.append((root[0] + dx * reach * t - dy * curl * t, root[1] + dy * reach * t + dx * curl * t,
                                250 - 150 * t + 150 * t * t * (1.2 if k % 3 == 0 else 0.3) + (90 * t * t if k % 2 else 0)))
            for j in range(5):
                w = 62 - j * 10
                tilt_box(arm, body, pts[j], pts[j + 1], w, w * 0.8)
                if j < 4:
                    mid = [(pts[j][q] + pts[j + 1][q]) / 2.0 for q in range(3)]
                    arm.box(pale, (mid[0], mid[1], mid[2] - w * 0.42), (w * 0.6, w * 0.6, 6))
    return build


SEAHORSE = [(-6, 0), (22, 4), (40, 26), (40, 58), (22, 78), (30, 104), (52, 132), (60, 176), (50, 210), (66, 232), (120, 222), (124, 238), (72, 252),
            (58, 276), (30, 284), (22, 262), (4, 250), (-6, 214), (10, 178), (0, 140), (-28, 108), (-34, 70), (-18, 44), (2, 36), (8, 22), (-4, 14), (-22, 22),
            (-30, 12)]


def _seahorse(m, x, y, z, s, col, trim, sign=1):
    """A seahorse in profile, heading along +X (sign -1: -X), its curled tail's tip at z. 2.8 m tall at s = 1."""
    pts = [(x + sign * px * s, z + pz * s) for px, pz in SEAHORSE]
    if sign < 0:
        pts.reverse()
    m.side(col, pts, y - 17 * s, y + 17 * s)
    for k in range(4):
        m.box(trim, (x + sign * (38 - k * 4) * s, y, z + (120 + k * 26) * s), (10 * s, 38 * s, 12 * s))
    for sd in (-1, 1):
        m.box(DARK, (x + sign * 56 * s, y + sd * 18 * s, z + 250 * s), (10 * s, 4 * s, 10 * s))
    m.box(trim, (x + sign * 16 * s, y, z + 284 * s), (40 * s, 8 * s, 22 * s))        # the crest
    m.box(trim, (x - sign * 14 * s, y, z + 170 * s), (26 * s, 6 * s, 60 * s))         # the back fin


def seahorse_patrol(seed=1, n=4, sign=1, length=2400.0, speed=85.0):
    """Nereida's mounted patrol: aquanauts riding giant seahorses in line along the road, 3 to 5 m up, rising and
    falling as they go, lances with pennants."""
    def build(sc):
        r = random.Random(seed)
        cols = ((YEL, '#ff7a3c'), ('#ff7a3c', YEL), ('#ff5fa2', WHITE), ('#b8ff5a', YEL))
        for k in range(n):
            x = (-(n - 1) / 2.0 + k) * 360.0 + r.uniform(-30, 30)
            y = r.uniform(-60, 60)
            z = 250.0 + (k % 2) * 90.0
            s = r.uniform(1.0, 1.15)
            ph, rt, bb = r.random(), r.uniform(0.3, 0.42), -r.uniform(22, 34)
            tv = (length, sign * speed)
            col, trim = cols[(k + seed) % len(cols)]
            m = sc.part(P.motion(phase=ph, rate=rt, bob=bb, pivot=(x, y, z + 150), base=(x, y), travel=(tv[0], tv[1] / tv[0])))
            _seahorse(m, x, y, z, s, col, trim, sign)
            d = aquanaut(r, top=WHITE if k % 2 else '#ff2d3c', bottom=WHITE if k % 2 else '#ff2d3c', band=MAG)
            d['s'] = 0.95
            P.figure(sc, x - sign * 4 * s, y, z=z + 150 * s, yaw=90 * sign, legs='sit', seat=46, arm_l='hold', arm_r='up',
                     hold_r=P.hold_flag(col=(MAG, YEL)[k % 2], w=80.0) if k % 2 == 0 else None, phase=ph, rate=rt, bob=bb, travel=tv, detail=1, **d)
    return build


def _jelly(sc, x, y, z, R, col, pale, phase, rate=0.12, bob=-140.0, glow=0.22):
    b = sc.part(P.motion(phase=phase, rate=rate, bob=bob, pivot=(x, y, z)))
    b.sphere(col, (x, y, z), R, scale=(1, 1, 0.72), steps=(4, 8), glow=glow)
    b.cyl(pale, (x, y, z - R * 0.34), R * 0.96, R * 0.16, steps=8, glow=glow * 0.7)
    b.cyl(pale, (x, y, z - R * 1.5), R * 0.2, R * 1.3, steps=6, top_r=R * 0.5, glow=glow * 0.5)
    for j in range(6):
        a = j * 1.047 + phase * 6.0
        t = sc.part(P.motion(phase=phase + j * 0.13, rate=rate * 2.0, bob=bob, swing=7.0, pivot=(x + math.cos(a) * R * 0.7, y + math.sin(a) * R * 0.7, z - R * 0.3),
                             axis=math.degrees(a)))
        t.box(col, (x + math.cos(a) * R * 0.7, y + math.sin(a) * R * 0.7, z - R * (1.6 + (j % 3) * 0.3)), (R * 0.09, R * 0.09, R * (2.6 + (j % 3) * 0.6)), glow=glow * 0.5)


def jelly_giants(seed=1):
    """Giant jellyfish far out over the trench walls: bells 5 to 9 m across, 18 to 45 m up, rising and sinking very
    slowly, arms trailing 15 m. Their light is steady (and they stand 150 m and more from the road)."""
    def build(sc):
        r = random.Random(seed)
        cols = ((MAG, '#ffd0ec'), ('#b9a4ff', '#ece4ff'), ('#ffc93c', '#fff1c0'), ('#ff7a6a', '#ffe0d6'), ('#f4a6ff', '#fff6ea'))
        for k in range(5):
            col, pale = cols[(k + seed) % 5]
            _jelly(sc, (k - 2) * 900 + r.uniform(-200, 200), r.uniform(-500, 500), 2400 + ((k * 2) % 5) * 520 + r.uniform(-100, 100), r.uniform(250, 450),
                   col, pale, r.random())
    return build


def far_convoy(seed=1, sign=1, length=9000.0):
    """Far out: a cargo caravan on its way to Nereida. Three loads under lift bags and the swimmers who tow and
    flank them, all three times life size so that they read at 200 m: a line of colour that travels."""
    def build(sc):
        r = random.Random(seed)
        head = 0.0 if sign > 0 else 180.0
        tv = (length, sign * 260.0)
        tvr = (tv[0], tv[1] / tv[0])
        for k in range(3):
            x = (k - 1) * 1500.0 * sign
            z = 1500.0 + (k % 2) * 300.0
            ph = r.random()
            ld = sc.part(P.motion(phase=ph, rate=0.15, bob=-60.0, pivot=(x, 0.0, z), base=(x, 0.0), travel=tvr))
            ld.box(('#c8683a', YEL, '#ff5fa2')[k], (x, 0, z), (520, 360, 300))
            ld.box(DARK, (x, 0, z), (530, 60, 310))
            _lift_bag(ld, x - 130, 0, z + 150, (YEL, '#ff5fa2', WHITE)[k], 2.6)
            _lift_bag(ld, x + 170, 0, z + 150, (WHITE, YEL, '#ff7a3c')[k], 2.2)
            for j, (dx, dy, dz) in enumerate(((620, -200, -80), (620, 220, 60), (-560, 0, 120))):
                swimmer(sc, x + dx * sign, dy, z + dz, heading=head, pitch=6, s=3.0, suit=SUITS[(k * 3 + j) % len(SUITS)], fins=FINS[(k + j) % len(FINS)],
                        arms='fwd' if j < 2 else 'side', kick=16.0, rate=0.7, phase=r.random(), bob=-30.0, travel=tv, bubbles=False)
    return build


def cam_sled(seed=1):
    """The crew's underwater camera position (this level's own crew piece): a camera sled 3 m over the sand sliding to
    and fro along the road on its line, the operator swimming behind it; the clapper loader and the first assistant
    hang in the water beside it; a line of the crew's pink-and-white tape between two buoys."""
    def build(sc):
        r = random.Random(seed)
        w = C.WARDROBES['Sea']
        g = sc.still()
        Z = 300.0
        # the tape, floated: two weights on the sand, two buoys, the tape between them 2.3 m up
        for sx in (-1, 1):
            g.box(DARK, (sx * 520, -150, 20), (60, 60, 40))
            g.box(C.WHITE, (sx * 520, -150, 150), (6, 6, 230))
            g.sphere(C.PINK, (sx * 520, -150, 290), 36, steps=(4, 6))
        for k in range(12):
            g.box(C.PINK if k % 2 else C.WHITE, (-520 + (k + 0.5) * 1040 / 12.0, -150, 232), (1040 / 12.0, 4, 16))
        # the guide line the sled runs on
        for sx in (-1, 1):
            g.box(STEEL, (sx * 470, 0, Z / 2.0), (10, 10, Z))
        g.box(STEEL, (0, 0, Z - 44), (950, 6, 6))
        slide = dict(swing=2.6, pivot=(0.0, 0.0, -6000.0), axis=90.0)
        sl = sc.part(P.motion(rate=0.08, **slide))
        rod(sl, YEL, (-110, 0, Z), (60, 0, Z), 30, steps=8)
        rod(sl, DARK, (-150, 0, Z), (-110, 0, Z), 40, steps=8)
        sl.box(YEL, (-20, 0, Z - 34), (150, 90, 8))
        for sy in (-1, 1):
            sl.box(DARK, (-60, sy * 50, Z - 8), (120, 8, 44))
        C.film_camera(sl, 30, -10, Z + 30)
        swimmer(sc, -260, 0, Z + 16, heading=0, pitch=0, s=1.25, suit=w['tops'][0], fins=DARK, arms='tow', kick=0.0, rate=0.08, band=w['band'],
                bubbles=False, sway=slide)
        swimmer(sc, 300, -40, Z + 60, heading=200, pitch=62, s=1.2, suit=w['tops'][1], fins=YEL, arms='wave', wave=14.0, kick=8.0, rate=0.5, phase=0.3,
                bob=-10.0, band=w['band'])
        d = C.member(r, w)
        P.figure(sc, -420, 60, z=Z - 40, yaw=-20, arm_l='hold', arm_r='hold', hold_r=C.hold_clapper(), rate=0.3, bob=-14, phase=0.6, **d)
        C.case(g, 420, 90, col=YEL)
    return build


MOVE = dict(grow=(1400.0, 80.0, 120.0), grow_down=(1400.0, 80.0, 0.0))
JOBS = [
    ('SM_DiveTeam_A', dive_team(1, 5, 'scooter', sign=1), dict(far=True, **MOVE)),
    ('SM_Watchers_A', watchers(1), dict(grow=(60.0, 60.0, 120.0))),
    ('SM_Turtles', turtles(1), dict(far=True, grow=(1600.0, 120.0, 120.0), grow_down=(1600.0, 120.0, 0.0))),
    ('SM_CrabRow_A', crab_row(1), {}),
]
REST = [
    ('SM_DiveTeam_B', dive_team(2, 4, 'camera', sign=-1), dict(far=True, **MOVE)),
    ('SM_Watchers_B', watchers(2, 12), dict(grow=(60.0, 60.0, 120.0))),
    ('SM_CrabRow_B', crab_row(2, 3), {}),
    ('SM_AquaWeld', aqua_weld(1), dict(grow=(60.0, 60.0, 120.0))),
    ('SM_AquaLift', aqua_lift(1), dict(grow=(60.0, 60.0, 160.0), grow_down=(60.0, 60.0, 0.0))),
    ('SM_Dolphins', dolphins(1), dict(far=True, grow=(2700.0, 200.0, 200.0), grow_down=(2700.0, 200.0, 0.0))),
    ('SM_Octopus', octopus(1), dict(grow=(120.0, 120.0, 160.0), grow_down=(120.0, 120.0, 0.0))),
    ('SM_SeahorsePatrol', seahorse_patrol(1), dict(far=True, **MOVE)),
    ('SM_JellyGiants', jelly_giants(1), dict(far=True, grow=(100.0, 100.0, 200.0), grow_down=(100.0, 100.0, 200.0))),
    ('SM_FarConvoy', far_convoy(1), dict(far=True, grow=(4600.0, 200.0, 200.0), grow_down=(4600.0, 200.0, 0.0))),
    ('SM_SeaCamSled', cam_sled(1), dict(grow=(340.0, 60.0, 120.0), grow_down=(340.0, 60.0, 0.0))),
]


def run(only=None, everything=True):
    P.build_core()
    return P.export(DIR, JOBS + (REST if everything else []), only)


def paths(everything=True):
    return [DIR + '/' + j[0] for j in JOBS + (REST if everything else [])]
