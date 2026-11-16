"""FULL THROTTLE (Unreal) the population of level 6, Under the Sea, pass 3, builder p3c: THE MOVERS.
/Game/People/UnderTheSea/SM_P3C_*.

Everything here TRAVELS: along the road and round again (travel), or round a ring (orbit). Nothing crosses the road:
a travel path is the mesh's own X, which the placement lays along the road, and a ring's radius is part of the piece
(the table keeps a ring 60 m and more out).

  SM_P3C_Scooters_A / _B   aquanauts towed by scooters in a fast file, streamers behind them (A goes +X, B goes -X)
  SM_P3C_FinRace           a fin race: six swimmers in lanes one over another between two chequered gates, each at
                           her own speed, so the order changes all the time
  SM_P3C_AquaBikes         the cyclists of Nereida: four riders pedalling propeller bikes, pennants on whips
  SM_P3C_SledTrain         a tug sled and three cargo sleds in tow: crates, tanks, two passengers waving
  SM_P3C_TurtleWagon       a canopied wagon drawn by a great turtle, a driver, a child on the load, a young turtle after it
  SM_P3C_Walkway           a catwalk on stilts between two airlock booths: walkers both ways, a runner, a trolley
  SM_P3C_ManateeHerd       a herd of sea cows with calves, a herder riding the biggest, another swimming behind with a staff
  SM_P3C_CableCar          two pylons 52 m apart, two cables, four open cars with passengers going both ways
  SM_P3C_RidingRing        a riding school: four riders on giant seahorses circling the riding master's mast
  SM_P3C_BuoyLaps          swimmers lapping a marker buoy in two rings, one inside the other, opposite ways

The level's lesson holds (ft_people_sea.py): the sand lies 0.9 m under the road behind a metre of barrier, so everybody
here is carried 3 to 15 m over the sand, in suits of strong colour against the blue. Every piece is therefore TALL:
24 m and more from the centre line. Nothing glows, nothing blinks. Nothing here is grey with a white belly and a back
fin (the boss), nothing is the blue of his torpedo fish; the sea cows are peach and rose and have no fin at all.

A piece is drawn like every prop: cm; z = 0 is the sand; the middle of the footprint is the origin; the FRONT faces -Y
(the road); long pieces run along X (the road). The swimmer, the turtle and the seahorse are ft_people_sea.py's own
builders, called from here (that file is not changed).

Run in the editor:  import ft_people_sea_p3c; ft_people_sea_p3c.run()
The placing rules: C:/ftwork/p3/L6c.inl (the integrator pastes it at the end of FTPeopleL6.inl).
"""
import math
import random
import unreal
import ft_people as P
import ft_people_sea as S
from ft_desert import rod, tilt_box

DIR = '/Game/People/UnderTheSea'

SUITS, FINS = S.SUITS, S.FINS
HELM, STEEL, DARK, WHITE, YEL = S.HELM, S.STEEL, S.DARK, S.WHITE, S.YEL
DECK, DECK_D, MAG = S.DECK, S.DECK_D, S.MAG
ORANGE, PINK, LIME, RED, GLASS, CORAL = '#ff7a3c', '#ff5fa2', '#b8ff5a', '#ff2d3c', '#bfe9f2', '#ff7a6a'


def _tv(travel):
    """(length cm, speed cm/s) -> what a motion wants: (length, rounds a second)."""
    return (float(travel[0]), float(travel[1]) / float(travel[0]))


def _rigid(sc, x, y, z, travel=None, orbit=None, **kw):
    """The model of a rigid thing whose middle is (x, y, z): it travels or circles as one (its own place is its base,
    its middle the point it folds into at a travel's ends)."""
    d = dict(pivot=(float(x), float(y), float(z)))
    if travel:
        d.update(base=(float(x), float(y)), travel=_tv(travel))
    elif orbit:
        d.update(base=(float(orbit[0]), float(orbit[1])), travel=(-1.0, float(orbit[2])))
    d.update(kw)
    return sc.part(P.motion(**d))


def _aquanaut(r, s=1.0, **fixed):
    d = S.aquanaut(r, **fixed)
    d['s'] = s
    return d


# ------------------------------------------------------------------------------------------ scooters
def scooters(seed=1, sign=1, n=5, length=4400.0, speed=560.0):
    """Aquanauts towed by scooters, in file along the road 5 to 9 m over the sand, fast, a streamer behind each."""
    def build(sc):
        r = random.Random(seed)
        head = 0.0 if sign > 0 else 180.0
        tv = (length, sign * speed)
        ribbons = (PINK, YEL, WHITE, LIME, ORANGE)
        for k in range(n):
            x = (-(n - 1) / 2.0 + k) * 520.0 * sign + r.uniform(-50, 50)
            y = r.uniform(-80, 80)
            z = 520.0 + ((k * 2) % n) * 100.0
            ph, rt, bb = r.random(), r.uniform(0.5, 0.7), -r.uniform(10, 18)
            s = r.uniform(1.3, 1.42)
            W = S.swimmer(sc, x, y, z, heading=head, pitch=4.0, s=s, suit=SUITS[(k * 3 + seed) % len(SUITS)], fins=FINS[(k + seed) % len(FINS)],
                          helmet=(k + seed) % 4 != 0, skin=r.choice(P.SKINS), arms='tow', kick=5.0, rate=rt, phase=ph, bob=bb, travel=tv,
                          hold=S.hold_scooter((YEL, ORANGE, WHITE)[(k + seed) % 3]), band=(WHITE, None, DARK)[k % 3], bubbles=(k % 2 == 0))
            rb = sc.part(P.motion(phase=ph, rate=rt, bob=bb, swing=9.0, pivot=W(-130), axis=P.UP, base=(x, y), travel=_tv(tv)))
            col = ribbons[(k + seed) % len(ribbons)]
            tilt_box(rb, col, W(-134, 0, 6), W(-300, 0, 22), 6, 30 * s)
            tilt_box(rb, WHITE, W(-300, 0, 22), W(-380, 0, 14), 6, 30 * s)
            tilt_box(rb, col, W(-380, 0, 14), W(-460, 0, 24), 6, 30 * s)
    return build


# ------------------------------------------------------------------------------------------ the fin race
def fin_race(seed=1, length=3800.0):
    """A race along the road: six swimmers in lanes one over another (so that the race reads from the side), each at
    her own speed, between two chequered gates; a rope of floats over the lanes and one under them."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        for sx in (-1, 1):
            X = sx * (length / 2.0 + 40.0)
            for sy in (-1, 1):
                g.box(DECK_D, (X, sy * 170, 520), (16, 16, 1040))
            g.box(DECK_D, (X, 0, 1040), (16, 356, 16))
            for k in range(6):
                g.box(YEL if (k + (1 if sx > 0 else 0)) % 2 else DARK, (X, -170 + (k + 0.5) * 340 / 6.0, 990), (10, 340 / 6.0, 70))
        for z in (300.0, 930.0):
            g.box(WHITE, (0, 0, z), (length, 5, 5))
            for k in range(10):
                g.box((RED, WHITE)[k % 2], (-length / 2.0 + (k + 0.5) * length / 10.0, 0, z), (60, 16, 16))
        for j in range(6):
            z = 370.0 + j * 96.0
            suit = SUITS[(j * 3 + seed) % len(SUITS)]
            S.swimmer(sc, r.uniform(-1500, 1500), (-70.0, 70.0)[j % 2], z, heading=0.0, pitch=2.0, s=1.3, suit=suit, fins=FINS[(j + seed) % len(FINS)],
                      tank=suit, helmet=False, skin=r.choice(P.SKINS), arms=('fwd', 'wave')[j % 2], wave=40.0 if j % 2 else 0.0, kick=26.0,
                      rate=r.uniform(1.5, 1.9), phase=r.random(), bob=-6.0, travel=(length, r.uniform(250.0, 340.0)), bubbles=(j % 3 == 0),
                      band=(WHITE, DARK, YEL)[j % 3])
    return build


# ------------------------------------------------------------------------------------------ the cyclists
def _bike(sc, r, x, y, z, sign, tv, col, trim, ph, rt, flag):
    """A propeller bike and its rider: a hull tube, a nose, hydroplanes, a guard ring and two blades that turn to and
    fro; the rider stands in the pedals, legs going. The saddle is 78 cm over z."""
    bb = -8.0
    common = dict(phase=ph, rate=rt, bob=bb, base=(x, y), travel=_tv(tv))
    m = sc.part(P.motion(pivot=(x, y, z + 60.0), **common))
    rod(m, col, (x - sign * 130, y, z + 52), (x + sign * 110, y, z + 52), 13, steps=6)
    m.sphere(col, (x + sign * 135, y, z + 60), 40, scale=(1.5, 0.8, 0.9), steps=(4, 6))
    m.box(GLASS, (x + sign * 120, y, z + 104), (8, 56, 40))
    tilt_box(m, DARK, (x + sign * 100, y, z + 60), (x + sign * 62, y, z + 122), 7, 7)
    m.box(DARK, (x + sign * 62, y, z + 124), (8, 74, 8))
    m.box(DARK, (x - sign * 10, y, z + 72), (44, 26, 12))
    m.box(trim, (x - sign * 50, y, z + 40), (64, 156, 6))
    rod(m, DARK, (x - sign * 160, y, z + 52), (x - sign * 142, y, z + 52), 40, steps=8)
    m.box(DARK, (x - sign * 100, y, z + 130), (5, 5, 150))
    m.box(flag, (x - sign * 124, y, z + 188), (48, 4, 30))
    pr = sc.part(P.motion(swing=85.0, pivot=(x - sign * 172, y, z + 52), axis=0.0, **common))
    pr.box(trim, (x - sign * 172, y, z + 52), (6, 100, 14))
    pr.box(trim, (x - sign * 172, y, z + 52), (6, 14, 100))
    P.figure(sc, x - sign * 10, y, z=z + 12.0, yaw=90.0 * sign, arm_l='fwd', arm_r='fwd', stride=34.0, phase=ph, rate=rt, bob=bb, travel=tv, detail=1,
             **_aquanaut(r, 1.0))


def aqua_bikes(seed=1, n=4, length=4000.0, speed=360.0, sign=1):
    """Nereida's cyclists: riders pedalling propeller bikes along the road in a staggered file, 4 to 8 m up."""
    def build(sc):
        r = random.Random(seed)
        tv = (length, sign * speed)
        cols = ((YEL, RED), (PINK, WHITE), (LIME, DARK), (ORANGE, YEL), (WHITE, MAG))
        for k in range(n):
            x = (-(n - 1) / 2.0 + k) * 560.0 * sign + r.uniform(-60, 60)
            col, trim = cols[(k + seed) % len(cols)]
            _bike(sc, r, x, r.uniform(-60, 60), 400.0 + ((k * 3) % n) * 95.0, sign, tv, col, trim, r.random(), r.uniform(1.2, 1.5),
                  (MAG, YEL, WHITE, RED)[k % 4])
    return build


# ------------------------------------------------------------------------------------------ the sled train
def sled_train(seed=1, sign=1, length=4400.0, speed=230.0):
    """A tug sled with its driver and three cargo sleds in tow, floating 3 m over the sand: crates, tanks, and a bench
    with two passengers who wave at the chase."""
    def build(sc):
        r = random.Random(seed)
        tv = (length, sign * speed)
        Z, rt, bb = 330.0, 0.5, -12.0

        def sled(m, x, L, col):
            m.box(col, (x, 0, Z), (L, 150, 14))
            for sy in (-1, 1):
                rod(m, DECK_D, (x - L / 2.0 + 10, sy * 66, Z - 30), (x + L / 2.0 - 10, sy * 66, Z - 30), 15, steps=6)
                for sx in (-1, 1):
                    m.box(DARK, (x + sx * L * 0.3, sy * 66, Z - 14), (8, 8, 28))

        x0 = sign * 700.0
        ph = r.random()
        m = _rigid(sc, x0, 0.0, Z, travel=tv, phase=ph, rate=rt, bob=bb)
        sled(m, x0, 300.0, YEL)
        m.box(MAG, (x0 + sign * 95, 0, Z + 45), (110, 130, 76))        # fix round: was ORANGE, the missile pickup's colour and size
        m.box(GLASS, (x0 + sign * 40, 0, Z + 100), (8, 120, 60))
        m.box(DARK, (x0 - sign * 60, 0, Z + 30), (60, 110, 46))
        for sy in (-1, 1):
            rod(m, DARK, (x0 - sign * 150, sy * 105, Z + 20), (x0 - sign * 40, sy * 105, Z + 20), 28, steps=8)
            rod(m, YEL, (x0 - sign * 40, sy * 105, Z + 20), (x0 - sign * 20, sy * 105, Z + 20), 20, steps=8)
            m.box(DARK, (x0 - sign * 95, sy * 84, Z + 12), (30, 30, 10))
        P.figure(sc, x0 - sign * 60, 0, z=Z + 7.0, yaw=90.0 * sign, legs='sit', seat=46, arm_l='fwd', arm_r='fwd', phase=ph, rate=rt, bob=bb, travel=tv,
                 detail=1, **_aquanaut(r, 1.0, top=RED, bottom=RED))
        for k in (1, 2, 3):
            x = x0 - sign * k * 430.0
            ph = r.random()
            m = _rigid(sc, x, 0.0, Z, travel=tv, phase=ph, rate=rt, bob=bb)
            sled(m, x, 330.0, (DECK, WHITE, DECK)[k - 1])
            m.box(DARK, (x + sign * 215, 0, Z), (100, 8, 8))
            if k == 1:
                m.box('#c8683a', (x - 70, 0, Z + 62), (150, 130, 110))
                m.box('#7a3f2a', (x - 70, 0, Z + 62), (156, 30, 116))
                m.box(YEL, (x + 80, 10, Z + 42), (110, 100, 70))
                m.box(PINK, (x + 80, 10, Z + 108), (90, 80, 60))
            elif k == 2:
                for sy, col in ((-1, LIME), (1, WHITE)):
                    rod(m, col, (x - 130, sy * 38, Z + 44), (x + 130, sy * 38, Z + 44), 36, steps=8)
                    rod(m, DARK, (x - 12, sy * 38, Z + 44), (x + 12, sy * 38, Z + 44), 39, steps=8)
                rod(m, ORANGE, (x - 110, 0, Z + 104), (x + 110, 0, Z + 104), 32, steps=8)
            else:
                m.box(DECK_D, (x, 34, Z + 30), (260, 50, 46))
                m.box(DARK, (x - sign * 150, 60, Z + 110), (5, 5, 210))
                m.box(MAG, (x - sign * 182, 60, Z + 190), (60, 4, 40))
                P.figure(sc, x - 60, 14, z=Z + 7.0, yaw=0.0, legs='sit', seat=46, arm_r='wave', swing_r=28, arm_l='hip', react=True, phase=ph, rate=rt,
                         bob=bb, travel=tv, detail=1, **_aquanaut(r, 1.0))
                P.figure(sc, x + 60, 14, z=Z + 7.0, yaw=0.0, legs='sit', seat=46, arm_l='up', arm_r='up', swing_l=14, swing_r=-14, react=True, phase=ph,
                         rate=rt, bob=bb, travel=tv, detail=1, **_aquanaut(r, 0.8, build='kid'))
    return build


# ------------------------------------------------------------------------------------------ the turtle wagon
def turtle_wagon(seed=1, sign=1, length=3200.0, speed=115.0):
    """A canopied wagon drawn by a great turtle, slowly, 4 m over the sand: the driver on the bench, a child waving
    from the bales behind, and a young turtle that follows the wagon."""
    def build(sc):
        r = random.Random(seed)
        head = 0.0 if sign > 0 else 180.0
        tv = (length, sign * speed)
        ph, rt, bb = r.random(), 0.45, -14.0
        Z = 420.0
        tx, wx = sign * 340.0, -sign * 300.0
        S._turtle(sc, tx, 0.0, Z, head, 1.3, '#c9853a', '#d7e38a', '#f1e4c8', ph, rt, tv)
        m = _rigid(sc, wx, 0.0, Z, travel=tv, phase=ph, rate=rt, bob=bb)
        m.box(CORAL, (wx, 0, Z - 10), (300, 150, 80))
        m.box(YEL, (wx, 0, Z + 26), (306, 156, 12))
        for sy in (-1, 1):
            rod(m, DARK, (wx, sy * 78, Z - 40), (wx, sy * 92, Z - 40), 70, steps=10)
            rod(m, YEL, (wx, sy * 92, Z - 40), (wx, sy * 100, Z - 40), 24, steps=6)
            tilt_box(m, DARK, (wx + sign * 150, sy * 60, Z), (tx - sign * 150, sy * 96, Z), 8, 8)
            for sx in (-1, 1):
                m.box(DECK_D, (wx + sx * 142, sy * 68, Z + 128), (6, 6, 196))
        for k in range(4):
            m.box((WHITE, PINK)[k % 2], (wx - 120 + k * 80, 0, Z + 232), (80, 176, 12))
        m.box(DECK_D, (wx + sign * 110, 0, Z + 53), (60, 130, 46))
        m.box('#7dffb0', (wx - sign * 70, 0, Z + 65), (130, 124, 70))
        m.box('#3fae7a', (wx - sign * 70, 0, Z + 65), (134, 30, 74))
        P.figure(sc, wx + sign * 110, 0, z=Z + 30.0, yaw=90.0 * sign, legs='sit', seat=46, arm_l='hold', arm_r='hold', phase=ph, rate=rt, bob=bb,
                 travel=tv, detail=1, **_aquanaut(r, 1.0, top=YEL, bottom=YEL))
        P.figure(sc, wx - sign * 70, -20, z=Z + 54.0, yaw=0.0, legs='sit', seat=46, arm_r='wave', swing_r=30, arm_l='up', swing_l=12, react=True,
                 phase=ph, rate=rt, bob=bb, travel=tv, detail=1, **_aquanaut(r, 0.78, build='kid'))
        S._turtle(sc, -sign * 950.0, 110.0, Z + 170.0, head, 0.6, '#d96a3c', '#e8c98a', '#f1e4c8', r.random(), 0.6, tv)
    return build


# ------------------------------------------------------------------------------------------ the walkway
def walkway(seed=1):
    """A catwalk on stilts 3 m over the sand, an airlock booth at each end: aquanauts walk it both ways, one runs, one
    pushes a trolley of crates, one has stopped at the rail to wave at the chase."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        L, Z = 2600.0, 300.0
        g.box(DECK, (0, 0, Z - 8), (L, 230, 16))
        g.box(YEL, (0, -116, Z - 8), (L, 4, 18))
        for k in range(5):
            x = -L / 2.0 + 150 + k * (L - 300) / 4.0
            for sy in (-1, 1):
                g.box(DECK_D, (x, sy * 90, (Z - 16) / 2.0 - 10), (16, 16, Z - 16 + 20))
        RL, n = L - 440.0, 6
        g.box(DECK_D, (0, -112, Z + 100), (RL, 6, 6))
        for k in range(n + 1):
            g.box(DECK_D, (-RL / 2.0 + k * RL / n, -112, Z + 50), (6, 6, 100))
        for k in range(n):
            g.box((MAG, WHITE, YEL, WHITE, LIME, WHITE)[k % 6], (-RL / 2.0 + (k + 0.5) * RL / n, -113, Z + 34), (RL / n - 12, 4, 36))
        for sx in (-1, 1):
            X = sx * (L / 2.0 - 110)
            g.box(DECK, (X, 0, Z + 125), (220, 236, 250))
            g.box(DARK, (X - sx * 111, 0, Z + 100), (4, 110, 200))
            g.cyl((ORANGE, MAG)[1 if sx > 0 else 0], (X, 0, Z + 250), 150, 44, steps=8, top_r=90)
            g.box(YEL, (X, -119, Z + 200), (200, 4, 30))
        TL = L - 300.0          # the walkers come round inside the booths

        def walker(x, y, sign, speed, det, **kw):
            d = _aquanaut(r, r.uniform(0.95, 1.05))
            d.update(kw)
            P.figure(sc, x, y, z=Z, yaw=90.0 * sign, phase=r.random(), travel=(TL, sign * speed), detail=det, **d)

        walk = dict(stride=24, swing_l=18, swing_r=-18, axis_l='x', axis_r='x', rate=0.9, bob=3)
        walker(-760, -60, 1, 120, 1, **walk)
        walker(-80, -40, 1, 135, 0, **walk)
        walker(520, 50, -1, 110, 1, arm_l='carry', arm_r='carry', hold_r=P.hold_box(col=YEL), stride=22, rate=0.8, bob=3)
        walker(-420, 70, -1, 125, 0, **walk)
        walker(300, -70, 1, 340, 0, stride=42, swing_l=34, swing_r=-34, axis_l='x', axis_r='x', rate=1.8, bob=8)
        # the trolley and the one who pushes it (towards -X)
        px = 900.0
        walker(px, 20, -1, 95, 0, arm_l='push', arm_r='push', stride=20, rate=0.8, bob=2)
        t = _rigid(sc, px - 95, 20.0, Z + 50.0, travel=(TL, -95.0))
        t.box(DECK_D, (px - 95, 20, Z + 26), (110, 70, 10))
        t.box(ORANGE, (px - 95, 20, Z + 62), (90, 60, 60))
        t.box(LIME, (px - 95, 20, Z + 112), (60, 50, 40))
        t.box(DECK_D, (px - 44, 20, Z + 70), (5, 60, 90))
        for sx in (-1, 1):
            rod(t, DARK, (px - 95 + sx * 36, -18, Z + 12), (px - 95 + sx * 36, 58, Z + 12), 12, steps=6)
        # the one at the rail does not travel: she waves as the chase comes by
        P.figure(sc, 40, -84, z=Z, yaw=0.0, arm_r='wave', swing_r=28, arm_l='up', swing_l=-14, rate=1.0, bob=-6, phase=r.random(), react=True, detail=1,
                 **_aquanaut(r, 1.05, top=PINK, bottom=PINK))
    return build


# ------------------------------------------------------------------------------------------ the sea cows
def _manatee(sc, x, y, z, sign, s, body, pale, ph, rt, tv):
    """A sea cow heading along the road: a fat round body, a round head with a pale muzzle, a paddle of a tail that
    beats slowly, two flippers. No fin on the back. 4 m long at s = 1.6. Returns the height of its back."""
    head = 0.0 if sign > 0 else 180.0
    W0 = S._frame(x, y, z, head, 0.0)

    def W(a, b=0.0, c=0.0):
        return W0(a * s, b * s, c * s)
    common = dict(phase=ph, rate=rt, bob=-16.0, base=(x, y), travel=_tv(tv))
    b = sc.part(P.motion(pivot=(x, y, z), **common))
    b.sphere(body, W(0), 60 * s, scale=(2.1, 1.0, 0.95), steps=(4, 8), rot=(0, head, 0))
    b.sphere(pale, W(10, 0, -14), 54 * s, scale=(1.9, 0.9, 0.7), steps=(3, 8), rot=(0, head, 0))
    b.sphere(body, W(128, 0, -4), 38 * s, scale=(1.2, 1.0, 0.95), steps=(4, 6), rot=(0, head, 0))
    b.box(pale, W(166, 0, -14), (22 * s, 44 * s, 26 * s))
    for sd in (-1, 1):
        b.box(DARK, W(150, sd * 31, 8), (6 * s, 4 * s, 6 * s))
        fl = sc.part(P.motion(swing=sd * 22.0, pivot=W(60, sd * 50, -20), axis=head, **common))
        tilt_box(fl, body, W(60, sd * 50, -20), W(30, sd * 105, -50), 30 * s, 9 * s)
    tl = sc.part(P.motion(swing=18.0, pivot=W(-110), axis=head + 90.0, **common))
    tilt_box(tl, body, W(-100), W(-170, 0, -4), 40 * s, 26 * s)
    tl.sphere(body, W(-200, 0, -6), 44 * s, scale=(1.0, 1.3, 0.22), steps=(3, 8))
    return z + 57.0 * s


def manatee_herd(seed=1, sign=1, length=3800.0, speed=80.0):
    """A herd of sea cows going slowly along the road 4 to 8 m up: four grown ones and two calves, a herder riding the
    biggest, another swimming behind with a staff."""
    def build(sc):
        r = random.Random(seed)
        head = 0.0 if sign > 0 else 180.0
        tv = (length, sign * speed)
        cols = (('#f2b28c', '#ffe2c8'), ('#e89a9a', '#ffd9d2'), ('#e8b070', '#ffe9c8'))
        herd = ((500, -40, 520, 1.9), (-150, 110, 640, 1.6), (-640, -90, 470, 1.7), (980, 80, 700, 1.5), (120, -120, 380, 0.75), (-900, 100, 600, 0.65))
        for k, (x, y, z, s) in enumerate(herd):
            body, pale = cols[(k + seed) % len(cols)]
            ph, rt = r.random(), r.uniform(0.32, 0.42)
            top = _manatee(sc, x * sign, y, z, sign, s, body, pale, ph, rt, tv)
            if k == 0:
                P.figure(sc, x * sign - sign * 10, y, z=top - 46.0, yaw=90.0 * sign, legs='sit', seat=46, arm_r='up', arm_l='hip',
                         hold_r=P.hold_flag(col=MAG, w=80.0), phase=ph, rate=rt, bob=-16.0, travel=tv, detail=1, **_aquanaut(r, 1.0, top=YEL, bottom=YEL))

        def staff(m, W):
            tilt_box(m, '#8f6b4a', W(110, 0, -60), W(110, 0, 150), 6, 6)
            m.box(YEL, W(110, 0, 150), (16, 16, 16))
        S.swimmer(sc, -sign * 1350.0, 0.0, 560.0, heading=head, pitch=10.0, s=1.35, suit=ORANGE, fins=YEL, arms='tow', kick=14.0, rate=0.8, phase=r.random(),
                  bob=-12.0, travel=tv, hold=staff, band=WHITE)
    return build


# ------------------------------------------------------------------------------------------ the cable car
def cable_car(seed=1, span=5200.0, speed=300.0):
    """A cable car line along the road: two pylons 52 m apart, a cable out and a cable back 15 m up, four open cars
    with passengers, two going each way. A car goes into the hood at one pylon as another comes out at the other."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        H = 1500.0
        for sx in (-1, 1):
            X = sx * span / 2.0
            for sy in (-1, 1):
                tilt_box(g, DECK_D, (X, sy * 260, 0), (X, sy * 40, H), 26, 26)
                g.box((ORANGE, MAG)[1 if sy > 0 else 0], (X, sy * 190, H - 190), (300, 190, 330))
                g.box(DARK, (X - sx * 151, sy * 190, H - 215), (4, 150, 250))
            g.box(DECK_D, (X, 0, 520), (20, 380, 20))
            g.box(DECK_D, (X, 0, 1040), (20, 220, 20))
            g.box(DECK, (X, 0, H + 20), (120, 560, 50))
            g.box(YEL, (X, 0, H + 50), (124, 564, 12))
        for sy in (-1, 1):
            g.box(DARK, (0, sy * 190, H), (span, 6, 6))

        def car(x, sy, v, col, riders):
            y = sy * 190.0
            tv = (span, v)
            m = _rigid(sc, x, y, H - 220.0, travel=tv)
            m.box(DARK, (x, y, H - 60), (8, 8, 120))
            m.box(col, (x, y, H - 122), (210, 150, 12))
            m.box(DECK, (x, y, H - 330), (200, 140, 12))
            for cx in (-1, 1):
                for cy in (-1, 1):
                    m.box(DECK_D, (x + cx * 96, y + cy * 66, H - 226), (6, 6, 200))
            for cy in (-1, 1):
                m.box(col, (x, y + cy * 70, H - 290), (200, 5, 70))
            m.box(WHITE, (x, y - 74, H - 290), (200, 3, 16))
            for j in range(riders):
                dx = (j - (riders - 1) / 2.0) * 90.0
                act = (dict(arm_r='wave', swing_r=26, arm_l='hip'), dict(arm_l='up', arm_r='up', swing_l=14, swing_r=-14))[(j + riders) % 2]
                P.figure(sc, x + dx, y - 20, z=H - 324.0, yaw=0.0, phase=r.random(), rate=0.9, react=True, travel=tv, detail=0,
                         **dict(_aquanaut(r, 1.0), **act))
        car(-span / 4.0, -1, speed, YEL, 2)
        car(span / 4.0, -1, speed, PINK, 1)
        car(-span / 4.0 + 300.0, 1, -speed, LIME, 1)
        car(span / 4.0 + 300.0, 1, -speed, WHITE, 2)
    return build


# ------------------------------------------------------------------------------------------ the riding ring
def riding_ring(seed=1, n=4, R=850.0, orb=0.035):
    """Nereida's riding school: riders on giant seahorses circle the riding master, who turns on a platform on a mast
    with a flag; a tall pennant beside the mast. The ring is 17 m across: 60 m and more from the road."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        g.cyl(DECK_D, (0, 0, 0), 14, 560, steps=6)
        g.cyl(YEL, (0, 0, 548), 126, 12, steps=8)
        g.cyl(DECK, (0, 0, 560), 120, 14, steps=8)
        P.figure(sc, 0, 0, z=574.0, yaw=0.0, arm_r='up', arm_l='hip', hold_r=P.hold_flag(col=MAG, w=90.0), turn=50.0, rate=0.12, detail=1,
                 **_aquanaut(r, 1.1, top=WHITE, bottom=WHITE, band=MAG))
        S._banner(sc, 60.0, 60.0, 0.0, 1000.0, YEL, RED, 0.2)
        cols = ((YEL, ORANGE), (ORANGE, YEL), (PINK, WHITE), (LIME, YEL), (WHITE, MAG))
        for k in range(n):
            a = 360.0 * k / n + r.uniform(-8, 8)
            ar = math.radians(a)
            px, py = R * math.cos(ar), R * math.sin(ar)
            z = 380.0 + (k % 2) * 130.0
            s = r.uniform(1.0, 1.12)
            ph, rt, bb = r.random(), r.uniform(0.3, 0.42), -r.uniform(22, 34)
            col, trim = cols[(k + seed) % len(cols)]
            m = sc.part(P.motion(phase=ph, rate=rt, bob=bb, pivot=(px, py, z + 150.0), base=(0.0, 0.0), travel=(-1.0, orb)), (px, py, 0.0), a + 90.0, 1.0)
            S._seahorse(m, 0.0, 0.0, z, s, col, trim, 1)
            P.figure(sc, px, py, z=z + 150.0 * s, yaw=a + 180.0, legs='sit', seat=46, arm_l='hold', arm_r=('up', 'wave')[k % 2],
                     swing_r=(0, 24)[k % 2], phase=ph, rate=rt, bob=bb, orbit=(0.0, 0.0, orb), detail=0,
                     **_aquanaut(r, 0.95, top=(RED, WHITE)[k % 2], bottom=(RED, WHITE)[k % 2], band=MAG))
    return build


# ------------------------------------------------------------------------------------------ laps round a buoy
def buoy_laps(seed=1):
    """Swimmers lapping a marker buoy on its chain: five in a wide ring one way, three in a ring inside it the other
    way, each a little higher than the last, 5 to 13 m over the sand. 21 m across: 60 m and more from the road."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        g.box(DARK, (0, 0, 30), (120, 120, 60))
        g.box(DECK_D, (0, 0, 700), (8, 8, 1300))
        g.sphere(ORANGE, (0, 0, 1400), 120, steps=(4, 8))
        g.cyl(WHITE, (0, 0, 1385), 124, 30, steps=8)
        g.box(DECK_D, (0, 0, 1590), (8, 8, 160))
        f = sc.part(P.motion(rate=0.4, swing=12.0, pivot=(0.0, 0.0, 1640.0), axis=P.UP))
        f.box(YEL, (72, 0, 1630), (130, 5, 70))
        f.box(RED, (72, -3.5, 1630), (130, 2, 20))
        i = 0
        for R, orb, n in ((950.0, 0.045, 5), (560.0, -0.07, 3)):
            for k in range(n):
                a = 360.0 * k / n + r.uniform(-10, 10) + (40.0 if orb < 0 else 0.0)
                ar = math.radians(a)
                S.swimmer(sc, R * math.cos(ar), R * math.sin(ar), 480.0 + i * 115.0, heading=a + (90.0 if orb > 0 else -90.0), pitch=r.uniform(-4, 8),
                          s=1.35, suit=SUITS[(i * 3 + seed) % len(SUITS)], fins=FINS[(i + seed) % len(FINS)], helmet=(i % 3 != 0), skin=r.choice(P.SKINS),
                          arms=('fwd', 'side', 'wave')[i % 3], wave=24.0 if i % 3 == 2 else 0.0, kick=20.0, rate=r.uniform(1.0, 1.3), phase=r.random(),
                          bob=-r.uniform(8, 14), orbit=(0.0, 0.0, orb), bubbles=(i % 2 == 0), band=(WHITE, None, DARK)[i % 3])
                i += 1
    return build


# ------------------------------------------------------------------------------------------ the kit
def _move(length, y=120.0, up=120.0):
    """Scene.save options of a piece whose people travel `length` along X: the always-moving master, bounds grown."""
    return dict(far=True, grow=(length / 2.0 + 200.0, y, up), grow_down=(length / 2.0 + 200.0, y, 40.0))


RING = dict(far=True, grow=(300.0, 300.0, 120.0), grow_down=(300.0, 300.0, 40.0))
JOBS = [
    ('SM_P3C_Scooters_A', scooters(1, 1), _move(4400.0)),
    ('SM_P3C_Scooters_B', scooters(2, -1), _move(4400.0)),
    ('SM_P3C_FinRace', fin_race(1), _move(3800.0)),
    ('SM_P3C_AquaBikes', aqua_bikes(1), _move(4000.0)),
    ('SM_P3C_SledTrain', sled_train(1), _move(4400.0)),
    ('SM_P3C_TurtleWagon', turtle_wagon(1), _move(3200.0, 200.0)),
    ('SM_P3C_Walkway', walkway(1), dict(grow=(60.0, 60.0, 60.0))),
    ('SM_P3C_ManateeHerd', manatee_herd(1), _move(3800.0, 160.0)),
    ('SM_P3C_CableCar', cable_car(1), dict(far=True, grow=(100.0, 100.0, 60.0), grow_down=(100.0, 100.0, 0.0))),
    ('SM_P3C_RidingRing', riding_ring(1), RING),
    ('SM_P3C_BuoyLaps', buoy_laps(1), RING),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
