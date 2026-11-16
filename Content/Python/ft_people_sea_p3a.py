"""FULL THROTTLE (Unreal) the population of level 6, Under the Sea, pass 3, builder p3a: THE GAPS.

/Game/People/UnderTheSea/SM_P3A_*. The general cast of the road to Nereida, in pieces that fit any district: people
at work, at rest and passing through, made to stand where the first two passes left nobody (the verge of Nereida's
17 km, the water behind the travel tube, the far side of the pipeline, the free slots of the first 8 km).

Two families, after what the first pass learned in frames (the sand lies 0.9 m under the road behind a metre of
barrier; the camera rides 3.5 m up):

  LOW (the whole piece under 2 m as built; placed at scale 1, 21.6 to 23.6 m out, where the camera sees over the
  barrier down to about half a metre over the sand). Everybody stands on a ledge or sits on a wall, small (s 0.80
  to 0.88), in the strong suits of Nereida, and nobody carries anything over the head:
    SM_P3A_VergeGang_A / _B   aquanauts behind a striped rail on a ledge: waving, pointing, filming, a child, a walker
    SM_P3A_WallSitters        a shift at rest on a sea wall, legs over the edge, one leaning at its end
    SM_P3A_Gardeners          the coral gardeners: a bed of young coral, a raker, a carrier, a planter
    SM_P3A_Surveyors          a survey party: an instrument on a tripod, a striped staff, a slate, pennants on stakes
    SM_P3A_EelGarden          a garden of striped eels swaying out of the rock, a parent and a child looking on
    SM_P3A_LobsterMarch       spiny lobsters (purple, pink, lime: no claws, not the crabs) marched along a ledge by a drover

  TALL (24 m and more; placed at 30 to 60 m and scaled 1.3 to 1.7: a diver at 1.0 is a speck). Their people are 4 to
  12 m over the sand, in the water or on something that carries them there:
    SM_P3A_Commuters_A / _B   Nereida going home: swimmers with cases and bundles, a parent and a child, one who waves
    SM_P3A_SchoolClass        a teacher, five children on a rope, a rear guard with a pennant: slowly along the road
    SM_P3A_KelpFarmers        kelp grown on lines under floats; farmers hanging on the lines, a basket going up
    SM_P3A_MastCrew           a service mast: two at work on its deck, one sitting on the edge, a swimmer circling it
    SM_P3A_RestBuoy           a tethered rest float: a shift sitting round its rim, a flag, one hanging under it
    SM_P3A_Catwalk            a catwalk on stilts: porters walking both ways, a trolley, two at the rail

Nothing here is lit (no glow at all), nothing blinks, nothing is grey with a white belly (the boss is GREAT WHITE) or
the blue of his torpedo fish, no box is the pickups' orange carrier or green toolbox. Travel is along the mesh's X
(the road's own line) only: nothing crosses towards the road.

Run in the editor:  import ft_people_sea_p3a as K; K.run()        (or K.run(['SM_P3A_EelGarden']))
The placing rules: C:/ftwork/p3/L6a.inl (pasted into Source/FullThrottle/FTPeopleL6.inl by the integrator).
"""
import math
import random
import unreal
import ft_people as P
import ft_people_crew as C
import ft_people_sea as S
from ft_desert import rod, tilt_box

DIR = '/Game/People/UnderTheSea'

YEL, WHITE, DARK, STEEL, MAG = S.YEL, S.WHITE, S.DARK, S.STEEL, S.MAG
DECK, DECK_D = S.DECK, S.DECK_D
LILAC, PINK, LIME, ORANGE, RED = '#8f6bff', '#ff5fa2', '#b8ff5a', '#ff7a3c', '#ff2d3c'
KELP, KELP_L = '#3fae6a', '#7dffb0'


# ------------------------------------------------------------------------------------------ helpers
def _naut(r, lo=0.80, hi=0.88, **fixed):
    """An aquanaut for a LOW piece: small enough that, on a 22 cm ledge, helmet and raised hands stay under 1.9 m."""
    d = S.aquanaut(r)
    d.update(s=r.uniform(lo, hi), hair='short')
    d.update(fixed)
    return d


def _rail(g, length, y, z0, h=62.0):
    """A low rail of yellow and white panels: people by the road stand BEHIND something."""
    n = 6
    for k in range(n + 1):
        g.box(DECK_D, (-length / 2.0 + k * length / n, y, z0 + h / 2.0), (7, 7, h))
    for k in range(n):
        g.box(YEL if k % 2 else WHITE, (-length / 2.0 + (k + 0.5) * length / n, y, z0 + h - 9), (length / n, 5, 13))


def _hold_case(col=YEL):
    """A swimmer's luggage: a case held ahead and under (for ft_people_sea.swimmer's hold=)."""
    def f(m, W):
        tilt_box(m, col, W(88, 0, -64), W(152, 0, -64), 52, 38)
        tilt_box(m, DARK, W(112, 0, -42), W(128, 0, -42), 30, 6)
    return f


def _hold_bundle(cols=(PINK, WHITE, YEL)):
    """A net of floats and sacks towed under the hands."""
    def f(m, W):
        for k, col in enumerate(cols):
            m.sphere(col, W(96 + k * 30, (k % 2) * 30 - 15, -78 - (k % 2) * 16), 30, steps=(3, 6))
        tilt_box(m, DARK, W(104, 0, -30), W(118, 0, -62), 5, 5)
    return f


def _hold_basket(col='#c8683a', fill=KELP_L):
    def f(m, W):
        tilt_box(m, col, W(96, 0, -58), W(150, 0, -58), 56, 34)
        tilt_box(m, fill, W(100, 0, -38), W(146, 0, -38), 48, 10)
    return f


def _hold_pennant(col=RED):
    """A pennant on a short staff over a swimmer's back."""
    def f(m, W):
        tilt_box(m, WHITE, W(40, 0, 20), W(40, 0, 190), 6, 6)
        tilt_box(m, col, W(40, 0, 130), W(-50, 0, 160), 6, 56)
    return f


def _hold_staff():
    """A survey staff in red and white bands, 1.5 m, upright in the hand."""
    def f(m, h, side):
        for k in range(5):
            m.box(RED if k % 2 else WHITE, (h[0], h[1] - 5, h[2] - 65 + (k + 0.5) * 30), (7, 7, 30))
    return f


# ------------------------------------------------------------------------------------------ LOW pieces
def verge_gang(seed=1, sign=1):
    """Aquanauts on a ledge of rock behind a striped rail: one waves, one has both arms up, one points down the road,
    one films, a child hops; a sixth walks the back of the ledge and comes round again."""
    def build(sc):
        r = random.Random(seed)
        L, H = 720.0, 22.0
        g = sc.still()
        S._rock(g, r, 0, 0, L, 190, H)
        _rail(g, L - 60, -84, H)
        acts = [
            dict(arm_r='wave', swing_r=28, arm_l='hip', rate=1.0, bob=4, react=True),
            dict(arm_l='up', arm_r='up', swing_l=14, swing_r=-14, rate=0.8, bob=6, react=True),
            dict(arm_r='point', swing_r=10, axis_r='x', arm_l='down', rate=0.6, bob=-3),
            dict(arm_l='hold', arm_r='hold', hold_r=P.hold_camera(col=YEL, lens=DARK), turn=14, rate=0.35),
            dict(arm_l='wave', swing_l=24, arm_r='vee', swing_r=-10, rate=0.9, bob=5, react=True),
        ]
        n = 5
        for k in range(n):
            d = _naut(r)
            d.update(acts[(k + seed) % len(acts)])
            if k == (seed + 1) % n:
                d.update(build='kid', s=0.6, bob=7, rate=1.3)
            x = -L / 2.0 + 90 + k * (L - 180) / (n - 1) + r.uniform(-14, 14)
            P.figure(sc, x, -22 + r.uniform(-10, 10), z=H, yaw=r.uniform(-16, 16), phase=r.random(), detail=2 if k % 2 == 0 else 1, **d)
        d = _naut(r, hi=0.86)
        P.figure(sc, 0, 58, z=H, yaw=90 * sign, stride=22, swing_l=16, swing_r=-16, axis_l='x', axis_r='x', rate=0.9, bob=3, phase=r.random(),
                 travel=(L - 150, sign * 46.0), detail=1, **d)
    return build


def wall_sitters(seed=3):
    """A shift at rest: five on a sea wall with their legs over the edge (one films, one shades the visor, two wave,
    a child points), a sixth leaning at the wall's end."""
    def build(sc):
        r = random.Random(seed)
        L, WH = 680.0, 85.0
        g = sc.still()
        g.box(DECK, (0, 2, WH / 2.0 - 20), (L, 58, WH + 40))
        g.box(DECK_D, (0, -28, WH - 9), (L + 6, 5, 16))
        for k in range(7):
            g.box(MAG if k % 2 else YEL, (-L / 2.0 + (k + 0.5) * L / 7.0, -28.5, 30), (L / 7.0 - 30, 5, 22))
        acts = [
            dict(arm_r='wave', swing_r=26, arm_l='down', rate=0.9, react=True),
            dict(arm_l='hold', arm_r='hold', hold_r=P.hold_camera(col=WHITE, lens=DARK), turn=10, rate=0.3),
            dict(arm_l='vee', arm_r='vee', swing_l=12, swing_r=-12, rate=0.7, react=True),
            dict(arm_r='face', arm_l='hip', turn=12, rate=0.25),
            dict(arm_l='wave', swing_l=22, arm_r='point', rate=0.8, react=True),
        ]
        n = 5
        for k in range(n):
            d = _naut(r, 0.80, 0.86)
            d.update(acts[(k * 2 + seed) % len(acts)])
            if k == 3:
                d.update(build='kid', s=0.62)
            x = -L / 2.0 + 80 + k * (L - 160) / (n - 1) + r.uniform(-10, 10)
            P.figure(sc, x, 0, z=WH - 46.0 * d['s'], legs='sit', yaw=r.uniform(-8, 8), bob=-2, phase=r.random(), detail=2 if k % 2 else 1, **d)
        d = _naut(r, 0.84, 0.88)
        P.figure(sc, L / 2.0 + 62, -6, z=0, yaw=-24, arm_l='hip', arm_r='wave', swing_r=18, rate=0.7, bob=-3, phase=r.random(), detail=2, **d)
        g = sc.still()
        g.box(S.ROCK_D, (L / 2.0 + 60, 20, -10), (150, 130, 26))
    return build


def gardeners(seed=5):
    """The coral gardeners of Nereida: a long bed of young coral that sways, and behind it, on a low deck, one raking,
    one carrying a tray of cuttings along the bed, one planting, one who has stopped to wave, a child pointing."""
    def build(sc):
        r = random.Random(seed)
        L = 700.0
        g = sc.still()
        g.box(DECK_D, (0, -62, 4), (L, 112, 48))
        g.box(S.SAND_D, (0, -62, 27), (L - 16, 96, 6))
        g.box(DECK, (0, 44, 0), (L, 124, 40))
        cols = (S.CORAL, S.CORAL2, MAG, '#f4a6ff', YEL, WHITE)
        for k in range(9):
            x = -L / 2.0 + 44 + k * (L - 88) / 8.0 + r.uniform(-10, 10)
            y = -62 + r.uniform(-22, 22)
            h = r.uniform(42, 78)
            c = sc.part(P.motion(phase=r.random(), rate=r.uniform(0.3, 0.5), swing=r.uniform(7, 11), pivot=(x, y, 28.0), axis=r.uniform(0, 180)))
            c.cyl(cols[k % len(cols)], (x, y, 28), 9, h, steps=6, top_r=24)
            c.sphere(cols[(k + 2) % len(cols)], (x, y, 28 + h + 6), 20, scale=(1.2, 1.2, 0.6), steps=(3, 6))
        Z = 20.0
        d = _naut(r)
        P.figure(sc, -250, 30, z=Z, yaw=8, arm_r='fwd', arm_l='hip', swing_r=14, axis_r='x', rate=0.9, phase=r.random(), detail=2,
                 hold_r=P.hold_tool(col=STEEL, grip=DARK, length=40.0), **d)
        d = _naut(r, hi=0.86)
        P.figure(sc, 0, 70, z=Z, yaw=90, arm_l='carry', arm_r='carry', hold_r=P.hold_box(col='#f4a6ff', w=44.0), stride=20, rate=0.8, bob=3,
                 phase=r.random(), travel=(L - 140, 40.0), detail=1, **d)
        d = _naut(r)
        P.figure(sc, -40, 26, z=Z, yaw=-6, arm_l=((40, 70), (58, 84)), arm_r=((40, 70), (58, 84)), swing_l=10, swing_r=10, axis_l='x', axis_r='x',
                 rate=0.6, bob=-3, phase=r.random(), detail=1, **d)
        d = _naut(r)
        P.figure(sc, 150, 28, z=Z, yaw=10, arm_r='wave', swing_r=28, arm_l='down', rate=1.0, bob=5, react=True, phase=r.random(), detail=2, **d)
        d = _naut(r, build='kid', s=0.6)
        P.figure(sc, 262, 24, z=Z, yaw=-12, arm_r='point', swing_r=12, axis_r='x', arm_l='hip', rate=1.2, bob=6, react=True, phase=r.random(),
                 detail=2, **d)
        g = sc.still()
        C.case(g, -320, 62, z=Z, w=60.0, d=40.0, h=34.0, col=YEL)
    return build


def surveyors(seed=7):
    """A survey party on a ledge: one at an instrument on its tripod, turning it; one holding a banded staff; one
    writing on a slate; one waving at the chase. Pennants on stakes flutter."""
    def build(sc):
        r = random.Random(seed)
        L, H = 700.0, 18.0
        g = sc.still()
        S._rock(g, r, 0, 0, L, 180, H)
        C.tripod(g, -170, -36, top=132.0)
        g.box(YEL, (-170, -40, 145), (34, 46, 26))
        g.cyl(DARK, (-170, -62, 145), 9, 16, rot=(0, 0, 90), steps=6)
        g.box(DARK, (-170, -36, 160), (16, 16, 6))
        d = _naut(r)
        P.figure(sc, -170, 30, z=H, yaw=0, arm_l='hold', arm_r='hold', turn=8, rate=0.3, phase=r.random(), detail=2, **d)
        d = _naut(r, hi=0.85)
        P.figure(sc, 210, -24, z=H, yaw=18, arm_r='hold', arm_l='hip', hold_r=_hold_staff(), rock=3, rate=0.4, phase=r.random(), detail=2, **d)
        d = _naut(r)
        P.figure(sc, 20, 10, z=H, yaw=-30, arm_l='hold', arm_r='hold', hold_l=C.hold_clipboard(), swing_r=8, axis_r='x', rate=1.4, bob=-2,
                 phase=r.random(), detail=1, **d)
        d = _naut(r)
        P.figure(sc, -40, -44, z=H, yaw=6, arm_r='wave', swing_r=28, arm_l='point', rate=1.0, bob=5, react=True, phase=r.random(), detail=2, **d)
        for k, (x, y, col) in enumerate(((-310, -50, MAG), (95, -62, YEL), (318, 30, PINK), (-60, 62, WHITE))):
            g = sc.still()
            g.box(DARK, (x, y, H + 40), (5, 5, 84))
            f = sc.part(P.motion(phase=0.23 * k, rate=0.8, swing=16.0, pivot=(x, y, H + 70.0), axis=P.UP))
            f.box(col, (x + 22, y, H + 70), (40, 4, 22))
        g = sc.still()
        C.case(g, 110, 48, z=H, w=64.0, d=40.0, h=32.0, col=WHITE)
    return build


def _eel(sc, r, x, y, z, h, rad, cols, phase):
    """A garden eel: a banded column out of its hole, swaying about its foot, a head with two eyes on the road side."""
    e = sc.part(P.motion(phase=phase, rate=r.uniform(0.35, 0.6), swing=r.uniform(11, 17), pivot=(x, y, z), axis=r.uniform(0, 180)))
    seg = h / 3.0
    for k in range(3):
        e.cyl(cols[k % 2], (x, y, z + k * seg), rad, seg, steps=6, top_r=rad * (0.96 if k < 2 else 0.9))
    e.sphere(cols[0], (x, y - 2, z + h + rad * 0.5), rad * 1.25, scale=(1.0, 1.25, 0.9), steps=(3, 6))
    for sd in (-1, 1):
        e.box(DARK, (x + sd * rad * 0.6, y - rad * 1.45, z + h + rad * 0.7), (5, 4, 6))
    sc.still().cyl(S.ROCK_D, (x, y, z - 4), rad + 9, 8, steps=6)


def eel_garden(seed=9):
    """A garden of big striped eels standing out of a ledge and swaying, each to its own time; a parent pointing them
    out and a child hopping beside."""
    def build(sc):
        r = random.Random(seed)
        L, H = 700.0, 14.0
        S._rock(sc.still(), r, 0, 0, L, 180, H)
        pairs = ((WHITE, ORANGE), (YEL, DARK), (WHITE, MAG), (LIME, DARK), (PINK, WHITE), (YEL, LILAC))
        n = 7
        for k in range(n):
            x = -L / 2.0 + 60 + k * (L - 240) / (n - 1) + r.uniform(-16, 16)
            _eel(sc, r, x, r.uniform(-50, 30), H, r.uniform(92, 150), r.uniform(10, 13), pairs[(k + seed) % len(pairs)], r.random())
        d = _naut(r)
        P.figure(sc, L / 2.0 - 120, 10, z=H, yaw=38, arm_r='point', swing_r=10, axis_r='x', arm_l='hip', rate=0.6, bob=-3, phase=r.random(),
                 detail=1, **d)
        d = _naut(r, build='kid', s=0.6)
        P.figure(sc, L / 2.0 - 50, -30, z=H, yaw=30, arm_l='up', arm_r='up', swing_l=16, swing_r=-16, rate=1.3, bob=7, react=True, phase=r.random(),
                 detail=2, **d)
    return build


def _lobster(sc, x, y, z, s, sign, col, trim, phase, rate, tv):
    """A spiny lobster walking along X (sign: which way), 1.2 m of body and as much again of feelers. No claws."""
    common = dict(phase=phase, rate=rate, base=(x, y), travel=tv)

    def Q(a, b, c):
        return (x + sign * a * s, y + b * s, z + c * s)
    b = sc.part(P.motion(pivot=(x, y, z), bob=3.0, **common))
    tilt_box(b, col, Q(-10, 0, 33), Q(52, 0, 37), 36 * s, 27 * s)
    for k in range(3):
        tilt_box(b, trim if k % 2 == 0 else col, Q(-10 - k * 20, 0, 32 - k * 3), Q(-30 - k * 20, 0, 29 - k * 3), (31 - k * 5) * s, (21 - k * 3) * s)
    tilt_box(b, trim, Q(-70, 0, 23), Q(-88, 0, 20), 42 * s, 6 * s)
    for sd in (-1, 1):
        b.box(WHITE, Q(54, sd * 11, 44), (8 * s, 8 * s, 10 * s))
        b.box(DARK, Q(58, sd * 11, 45), (4 * s, 5 * s, 6 * s))
        an = sc.part(P.motion(pivot=Q(50, sd * 8, 40), swing=sd * 18.0, axis=P.UP, **common))
        tilt_box(an, trim, Q(50, sd * 8, 40), Q(122, sd * 30, 80), 5.5 * s, 5.5 * s)
        lg = sc.part(P.motion(pivot=Q(14, sd * 14, 26), swing=sd * 11.0, axis=P.UP, **common))
        for j in range(4):
            tilt_box(lg, trim, Q(32 - j * 14, sd * 13, 28), Q(36 - j * 16, sd * 42, 0), 5.5 * s, 5.5 * s)


def lobster_march(seed=11, sign=1):
    """Five spiny lobsters walking nose to tail along a ledge, feelers sweeping, and the drover who walks them, a
    crook in the hand."""
    def build(sc):
        r = random.Random(seed)
        L, H = 780.0, 30.0
        S._rock(sc.still(), r, 0, 0, L, 190, H)
        TL, SP = L - 120.0, 30.0
        tv = (TL, sign * SP / TL)
        cols = ((LILAC, YEL), (PINK, WHITE), (LIME, DARK), (ORANGE, WHITE), ('#f4a6ff', LILAC))
        for k in range(5):
            col, trim = cols[(k + seed) % len(cols)]
            _lobster(sc, -TL / 2.0 + 70 + k * 128.0, -30 + (k % 2) * 16, H, r.uniform(1.15, 1.35), sign, col, trim, r.random(), r.uniform(1.6, 2.2), tv)
        d = _naut(r, 0.8, 0.84)
        P.figure(sc, 0, 56, z=H, yaw=90 * sign, arm_r='fwd', arm_l='down', hold_r=P.hold_tool(col=YEL, grip=DARK, length=36.0), stride=18,
                 swing_l=12, axis_l='x', rate=0.8, bob=2, phase=r.random(), travel=(TL, sign * SP), detail=1, **d)
    return build


# ------------------------------------------------------------------------------------------ TALL pieces
def commuters(seed=1, sign=1, length=2600.0, speed=110.0):
    """Nereida going home along the road, 4 to 9 m over the sand: a child kicking hard beside a parent, one with a
    case, one who turns to wave, and a leader towing a net of floats."""
    def build(sc):
        r = random.Random(seed)
        head = 0.0 if sign > 0 else 180.0
        tv = (length, sign * speed)
        n = 5
        for k in range(n):
            x = (-(n - 1) / 2.0 + k) * 350.0 * sign + r.uniform(-40, 40)
            y = r.uniform(-110, 110)
            z = 430.0 + ((k * 2) % n) * 105.0 + r.uniform(-20, 20)
            kw = dict(heading=head, pitch=r.uniform(-3, 8), s=r.uniform(1.3, 1.45), suit=S.SUITS[(k * 3 + seed) % len(S.SUITS)],
                      fins=S.FINS[(k + seed) % len(S.FINS)], helmet=(k + seed) % 3 != 1, skin=r.choice(P.SKINS), rate=r.uniform(0.9, 1.2),
                      phase=r.random(), bob=-r.uniform(8, 14), travel=tv, tank=r.choice((STEEL, YEL, WHITE)), band=r.choice((None, WHITE, DARK)))
            if k == 0:
                kw.update(s=0.9, rate=1.9, kick=24.0)
                S.swimmer(sc, x, y, z + 60, arms='fwd', bubbles=False, **kw)
            elif k == 1:
                S.swimmer(sc, x, y, z, arms='side', **kw)
            elif k == 2:
                S.swimmer(sc, x, y, z, arms='tow', hold=_hold_case(YEL if seed % 2 else WHITE), bubbles=False, **kw)
            elif k == 3:
                S.swimmer(sc, x, y, z, arms='wave', wave=26.0, react=True, **kw)
            else:
                S.swimmer(sc, x, y, z, arms='tow', kick=12.0, hold=_hold_bundle(), **kw)
    return build


def school_class(seed=21, length=2400.0, speed=70.0):
    """A class out from Nereida: the teacher ahead in white with a red band, waving them on; five children in a line
    on a rope, kicking hard; a rear guard with a pennant. Slowly along the road, 4 to 6 m up."""
    def build(sc):
        r = random.Random(seed)
        tv = (length, speed)
        S.swimmer(sc, 760, 0, 520, heading=0.0, pitch=8, s=1.45, suit=WHITE, fins=RED, band=RED, arms='wave', wave=24.0, rate=0.9,
                  phase=r.random(), bob=-10, travel=tv, react=True)
        hands = []
        for k in range(5):
            x, y, z, s = 420.0 - k * 235.0, (k % 2) * 70.0 - 35.0, 440.0 + (k % 2) * 50.0, 0.92
            S.swimmer(sc, x, y, z, heading=0.0, pitch=4, s=s, suit=S.SUITS[(k * 3 + 1) % len(S.SUITS)], fins=S.FINS[(k * 2) % len(S.FINS)],
                      tank=YEL, arms='tow', kick=24.0, rate=r.uniform(1.7, 2.1), phase=r.random(), bob=-6, travel=tv, bubbles=False)
            hands.append((x + 104 * s, y, z - 26 * s))
        hands.insert(0, (760 - 60, 0, 500))
        for a, b in zip(hands[:-1], hands[1:]):
            mid = ((a[0] + b[0]) / 2.0, (a[1] + b[1]) / 2.0, (a[2] + b[2]) / 2.0)
            rp = sc.part(P.motion(pivot=mid, base=(mid[0], mid[1]), travel=(length, speed / length)))
            tilt_box(rp, YEL, a, b, 6, 6)
        S.swimmer(sc, -960, 20, 560, heading=0.0, pitch=6, s=1.4, suit=ORANGE, fins=DARK, band=WHITE, arms='fwd', rate=1.0, phase=r.random(),
                  bob=-10, travel=tv, hold=_hold_pennant(RED), helmet=False, skin=r.choice(P.SKINS))
    return build


def kelp_farmers(seed=23):
    """Kelp grown on lines under yellow floats, 9 to 10 m tall; three farmers hang on the lines cutting into baskets,
    one on a small deck hauls, and a full basket goes up under a lift bag."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        g.box(DECK_D, (0, 0, 14), (980, 34, 30))
        for sx in (-1, 1):
            g.box(S.ROCK_D, (sx * 470, 0, 30), (130, 150, 70))
        tops = (900.0, 1000.0, 940.0, 860.0)
        for k, x in enumerate((-360.0, -120.0, 120.0, 360.0)):
            g = sc.still()
            g.box(DARK, (x, 0, tops[k] / 2.0), (7, 7, tops[k]))
            fl = sc.part(P.motion(phase=0.2 * k, rate=0.25, bob=-10.0, pivot=(x, 0.0, tops[k])))
            fl.sphere(YEL, (x, 0, tops[k] + 30), 50, steps=(4, 8))
            fl.cyl(WHITE, (x, 0, tops[k] + 22), 52, 14, steps=8)
            for j in range(3):
                zf = 330.0 + j * (tops[k] - 420.0) / 2.0
                sd = 1 if (j + k) % 2 else -1
                fr = sc.part(P.motion(phase=r.random(), rate=r.uniform(0.25, 0.4), swing=r.uniform(9, 14), pivot=(x, 0.0, zf), axis=90.0))
                fr.box(KELP if j % 2 else KELP_L, (x + sd * 52, 0, zf - 70), (96, 5, 150))
                fr.box(KELP_L if j % 2 else KELP, (x + sd * 30, 0, zf - 150), (50, 5, 110))
        for k, (x, z, hd) in enumerate(((-300.0, 540.0, 180.0), (-60.0, 720.0, 0.0), (300.0, 860.0, 180.0))):
            S.swimmer(sc, x + (80 if hd == 0.0 else -80) * -1, -46, z, heading=hd, pitch=62, s=1.35, suit=S.SUITS[(k * 2 + seed) % len(S.SUITS)],
                      fins=S.FINS[(k + 1) % len(S.FINS)], arms='tow', kick=9.0, rate=0.55, phase=r.random(), bob=-14, band=WHITE if k % 2 else None,
                      hold=_hold_basket(), bubbles=(k != 1), helmet=(k != 2), skin=r.choice(P.SKINS))
        g = sc.still()
        g.box(DECK, (150, 170, 252), (300, 220, 16))
        for sx in (-1, 1):
            for sy in (-1, 1):
                g.box(DECK_D, (150 + sx * 130, 170 + sy * 90, 116), (14, 14, 272))
        g.box('#c8683a', (240, 200, 300), (90, 80, 80))
        g.box(KELP_L, (240, 200, 346), (78, 68, 12))
        d = S.aquanaut(r, s=1.2)
        P.figure(sc, 110, 130, z=260, yaw=-10, arm_l='up', arm_r='up', swing_l=14, swing_r=-14, axis_l='x', axis_r='x', rate=0.6, bob=-4,
                 phase=r.random(), detail=1, **d)
        ld = sc.part(P.motion(phase=r.random(), rate=0.1, bob=-80.0, pivot=(-470.0, -60.0, 600.0)))
        ld.box('#c8683a', (-470, -60, 560), (110, 90, 70))
        ld.box(KELP, (-470, -60, 600), (96, 76, 12))
        S._lift_bag(ld, -470, -60, 600, PINK, 0.9)
    return build


def mast_crew(seed=25):
    """A service mast with a round deck 5 m up: one at work on the column with a tool, one waving, one sitting on the
    edge with arms up; a swimmer circling the mast with a case; one more hanging on the cross-arm above; the vane on
    top swings."""
    def build(sc):
        r = random.Random(seed)
        ZD, ZA = 520.0, 880.0
        g = sc.still()
        g.box(DECK_D, (0, 0, 20), (240, 240, 80))
        g.cyl(DECK_D, (0, 0, 0), 24, ZA + 40, steps=8)
        g.cyl(DECK, (0, 0, ZD - 16), 200, 16, steps=10)
        g.cyl(YEL, (0, 0, ZD - 28), 204, 12, steps=10)
        for a in (30, 150, 270):
            ca, sa = math.cos(math.radians(a)), math.sin(math.radians(a))
            tilt_box(g, DECK_D, (ca * 24, sa * 24, ZD - 190), (ca * 170, sa * 170, ZD - 22), 10, 10)
        g.box(DECK_D, (0, 0, ZA), (560, 18, 18))
        g.cyl(MAG, (-260, 0, ZA - 60), 42, 76, steps=8)
        g.cyl(WHITE, (260, 0, ZA + 6), 36, 60, steps=8)
        v = sc.part(P.motion(phase=r.random(), rate=0.2, swing=40.0, pivot=(0.0, 0.0, ZA + 60.0), axis=P.UP))
        v.box(YEL, (60, 0, ZA + 70), (230, 6, 56))
        v.box(DARK, (-80, 0, ZA + 70), (60, 6, 22))
        d = S.aquanaut(r, s=1.18)
        P.figure(sc, -70, -78, z=ZD, yaw=140, arm_r='fwd', arm_l='hold', swing_r=10, axis_r='x', rate=1.2, bob=-2, phase=r.random(), detail=2,
                 hold_r=P.hold_tool(col=STEEL, grip=DARK, length=34.0), **d)
        d = S.aquanaut(r, s=1.2)
        P.figure(sc, 96, -110, z=ZD, yaw=-10, arm_r='wave', swing_r=28, arm_l='hip', rate=1.0, bob=-6, react=True, phase=r.random(), detail=2, **d)
        d = S.aquanaut(r, s=1.15)
        P.figure(sc, -30, -178, z=ZD - 46.0 * 1.15, yaw=0, legs='sit', arm_l='vee', arm_r='vee', swing_l=12, swing_r=-12, rate=0.7, bob=-2,
                 react=True, phase=r.random(), detail=1, **d)
        S.swimmer(sc, 430, 0, ZD + 190, heading=90.0, pitch=0, s=1.35, suit=S.SUITS[(seed + 1) % len(S.SUITS)], fins=RED, arms='tow', kick=16.0,
                  rate=1.0, phase=r.random(), bob=-10, orbit=(0.0, 0.0, 0.05), hold=_hold_case(WHITE), band=WHITE)
        S.swimmer(sc, 130, -40, ZA + 90, heading=200.0, pitch=55, s=1.3, suit=S.SUITS[(seed + 4) % len(S.SUITS)], fins=YEL, arms='wave', wave=22.0,
                  kick=9.0, rate=0.6, phase=r.random(), bob=-14, react=True, bubbles=False)
    return build


def rest_buoy(seed=27):
    """A rest float on its tether, the deck 5.4 m up: four of a shift sitting round the rim with their legs over the
    edge, two standing behind (one with a flag, one pointing), one hanging in the water under the deck, a pennant."""
    def build(sc):
        r = random.Random(seed)
        ZT = 540.0
        g = sc.still()
        g.box(S.ROCK_D, (0, 0, 20), (220, 220, 80))
        g.box(DARK, (0, 0, 250), (9, 9, 400))
        g.cyl(YEL, (0, 0, 430), 150, 92, steps=10)
        g.cyl(WHITE, (0, 0, 466), 154, 20, steps=10)
        g.cyl(DECK, (0, 0, ZT - 18), 255, 18, steps=12)
        g.cyl(MAG, (0, 0, ZT - 30), 258, 12, steps=12)
        acts = [
            dict(arm_r='wave', swing_r=26, arm_l='down', rate=0.9, react=True),
            dict(arm_l='vee', arm_r='vee', swing_l=12, swing_r=-12, rate=0.7, react=True),
            dict(arm_r='point', swing_r=10, axis_r='x', arm_l='hip', rate=0.6),
            dict(arm_l='wave', swing_l=24, arm_r='hip', rate=0.8, react=True),
        ]
        for k, a in enumerate((-146.0, -108.0, -70.0, -32.0)):
            d = S.aquanaut(r, s=1.2 if k != 2 else 0.8)
            if k == 2:
                d.update(build='kid')
            d.update(acts[(k + seed) % len(acts)])
            ca, sa = math.cos(math.radians(a)), math.sin(math.radians(a))
            P.figure(sc, ca * 228, sa * 228, z=ZT - 46.0 * d['s'], legs='sit', yaw=a + 90.0, bob=-2, phase=r.random(), detail=1, **d)
        d = S.aquanaut(r, s=1.2)
        P.figure(sc, -70, 70, z=ZT, yaw=-8, arm_r='up', arm_l='hip', hold_r=P.hold_flag(col=YEL, pole=WHITE, w=90.0), swing_r=14, rate=0.6, bob=-5,
                 phase=r.random(), detail=1, **d)
        d = S.aquanaut(r, s=1.2)
        P.figure(sc, 90, 40, z=ZT, yaw=14, arm_r='point', arm_l='down', swing_r=10, axis_r='x', rate=0.7, bob=-4, phase=r.random(), detail=1, **d)
        S.swimmer(sc, 150, -30, 330, heading=205.0, pitch=42, s=1.3, suit=S.SUITS[(seed + 2) % len(S.SUITS)], fins=RED, arms='wave', wave=20.0,
                  kick=10.0, rate=0.6, phase=r.random(), bob=-16, band=WHITE, bubbles=False)
        S._banner(sc, 0, 170, ZT, 420, PINK, WHITE, 0.3)
    return build


def catwalk(seed=29):
    """A catwalk on stilts, 11 m long, the deck 3 m up behind a rail of yellow and white panels: porters walk it both
    ways with boxes, one pushes a trolley, one leans on the rail and waves, a child beside."""
    def build(sc):
        r = random.Random(seed)
        L, Z = 1100.0, 300.0
        g = sc.still()
        g.box(DECK, (0, 0, Z - 8), (L, 180, 16))
        for k in range(4):
            x = -L / 2.0 + 60 + k * (L - 120) / 3.0
            for sy in (-1, 1):
                g.box(DECK_D, (x, sy * 76, Z / 2.0 - 20), (14, 14, Z + 20))
            tilt_box(g, DECK_D, (x, -76, 30), (x, 76, Z - 30), 8, 8)
        g.box(DECK_D, (0, -88, Z + 92), (L, 6, 6))
        for k in range(10):
            xx = -L / 2.0 + (k + 0.5) * L / 10.0
            g.box(DECK_D, (xx - L / 20.0, -88, Z + 46), (6, 6, 92))
            g.box((YEL, WHITE)[k % 2], (xx, -90, Z + 56), (L / 10.0 - 10, 4, 40))
        g.box(DECK_D, (L / 2.0, -88, Z + 46), (6, 6, 92))
        TL = L - 160.0
        d = S.aquanaut(r, s=1.15)
        P.figure(sc, -260, -30, z=Z, yaw=90, arm_l='carry', arm_r='carry', hold_r=P.hold_box(col=YEL), stride=22, rate=0.8, bob=3, phase=r.random(),
                 travel=(TL, 60.0), detail=1, **d)
        d = S.aquanaut(r, s=1.12)
        P.figure(sc, 280, -30, z=Z, yaw=90, arm_l='down', arm_r='down', hold_r=P.hold_bag(col=PINK), stride=24, swing_l=18, swing_r=-18,
                 axis_l='x', axis_r='x', rate=0.9, bob=3, phase=r.random(), travel=(TL, 60.0), detail=1, **d)
        fx, fy = 120.0, 44.0
        d = S.aquanaut(r, s=1.15)
        P.figure(sc, fx, fy, z=Z, yaw=-90, arm_l='push', arm_r='push', stride=20, rate=0.8, bob=2, phase=r.random(), travel=(TL, -50.0),
                 detail=1, **d)
        tr = sc.part(P.motion(pivot=(fx - 90.0, fy, Z + 40.0), base=(fx, fy), travel=(TL, -50.0 / TL)))
        tr.box(STEEL, (fx - 100, fy, Z + 30), (110, 70, 12))
        tr.box(LILAC, (fx - 100, fy, Z + 66), (90, 60, 60))
        tr.box(WHITE, (fx - 100, fy, Z + 100), (94, 64, 8))
        tr.box(DARK, (fx - 50, fy, Z + 70), (6, 60, 6))
        d = S.aquanaut(r, s=1.18)
        P.figure(sc, -420, -58, z=Z, yaw=8, arm_r='wave', swing_r=28, arm_l='hip', rate=1.0, bob=-4, react=True, phase=r.random(), detail=2, **d)
        d = S.aquanaut(r, s=0.78, build='kid')
        P.figure(sc, -340, -56, z=Z, yaw=-10, arm_l='up', arm_r='up', swing_l=16, swing_r=-16, rate=1.3, bob=-6, react=True, phase=r.random(),
                 detail=2, **d)
    return build


JOBS = [
    ('SM_P3A_VergeGang_A', verge_gang(1, 1)),
    ('SM_P3A_VergeGang_B', verge_gang(2, -1)),
    ('SM_P3A_WallSitters', wall_sitters(3)),
    ('SM_P3A_Gardeners', gardeners(5)),
    ('SM_P3A_Surveyors', surveyors(7)),
    ('SM_P3A_EelGarden', eel_garden(9)),
    ('SM_P3A_LobsterMarch', lobster_march(11, 1)),
    ('SM_P3A_Commuters_A', commuters(1, 1)),
    ('SM_P3A_Commuters_B', commuters(2, -1)),
    ('SM_P3A_SchoolClass', school_class(21)),
    ('SM_P3A_KelpFarmers', kelp_farmers(23)),
    ('SM_P3A_MastCrew', mast_crew(25)),
    ('SM_P3A_RestBuoy', rest_buoy(27)),
    ('SM_P3A_Catwalk', catwalk(29)),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
