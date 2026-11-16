"""FULL THROTTLE (Unreal) the population of level 6, Under the Sea, pass 3, builder p3b: SCENES
(/Game/People/UnderTheSea/SM_P3B_*).

Ten story groups that could only be on the road to Nereida. One mesh a scene, 8 to 13 figures each doing something of
their own, with the props that tell the story:

    SM_P3B_Wedding        a wedding under a coral arch: the couple hand in hand (a veil, a tall hat), the officiant, guests
                          throwing their arms up and clapping, a conch player, two swimmers circling overhead.
    SM_P3B_PearlMarket    three stalls under striped awnings: giant clams with their pearls, sponges, amphorae; a vendor
                          holding a pearl up, hagglers, a porter walking the deck with a crate.
    SM_P3B_TreasureHaul   a chest of gold swinging under a tripod hoist over the rock, two hauling on the rope, one
                          counting coins, one leaping, a photographer, a swimmer filming from above.
    SM_P3B_SeahorseRace   jockeys on giant seahorses racing round the judges' tower, the starter turning with a flag.
    SM_P3B_KelpHarvest    the kelp farm: stalks swaying on their bed, two swimmers cutting, bundles carried along the deck,
                          a child riding the turtle that carries the net.
    SM_P3B_SchoolTrip     a class of Nereida's children (all in yellow) on three steps with two teachers; one child is
                          floating away under a balloon.
    SM_P3B_StatueDig      a sea king's stone head dug out of the rock: brushing and chiselling from a scaffold, a ranging
                          pole, the finds table, one sitting on the crown, a swimmer with the dredge pipe.
    SM_P3B_BubbleBand     a band in a clam-shell stage: drums, a tuba blowing bubbles, a conch, a singer, dancers.
    SM_P3B_RescueDrill    a rescue drill: a stretcher team swimming a casualty in to the recompression capsule, a marshal
                          with paddles, the instructor, a row of trainees in orange.
    SM_P3B_SubLaunch      the launch of a small yellow submarine: a bottle swinging at her bow, the ribbon and the
                          scissors, a crowd with flags, two of the yard's hands waving from her casing.

The same aquanaut as the level's other kits (ft_people_sea.py, ft_people_sea_watch.py: a suit of one strong colour,
never blue; the head is the white helmet with a dark visor; a pack), so the kits meet without a seam.

What reads from this road: the sand lies 0.9 m under the road behind a metre of barrier, so every scene brings its own
deck or rock 2.4 m or more over the sand (with footings 2 m down: a piece may stand on falling ground), its people are
a quarter larger than life, and all of them move. Every piece is TALL: 24 m or more from the centre line, and it is
drawn no deeper than 3.4 m either side of its middle (things that circle stay inside that too), so that at 28 m it
stands whole between the 24 m line and the travel tube. No glow, nothing that blinks. Nothing here is grey with a fin
(the boss), nothing is the blue of his torpedo fish, nothing is an orange or green crate hovering by the road.

A piece is drawn like every prop: cm; z = 0 is the sand; the middle of the footprint is the origin; the FRONT faces -Y
(the road); long pieces run along X (the road).

Run in the editor:  import ft_people_sea_p3b as K; K.run()        (or K.run(['SM_P3B_Wedding']))
The placing rules: C:/ftwork/p3/L6b.inl (pasted by the integrator at the end of Source/FullThrottle/FTPeopleL6.inl).
"""
import math
import random
import unreal
import ft_people as P
import ft_people_sea as S
from ft_desert import rod, tilt_box

DIR = '/Game/People/UnderTheSea'

SUITS = ('#ffc93c', '#ff7a3c', '#ff5fa2', '#fff6ea', '#b8ff5a', '#ff2d3c', '#f4a6ff', '#7dffb0')
HELM, VISOR, STEEL, DARK, WHITE, YEL = '#f4f9ff', '#22202a', '#8f9bb3', '#22202a', '#fff6ea', '#ffc93c'
DECK, DECK_D, HULL_D = '#cfe3ea', '#7fa2b4', '#3d4763'
MAG, RED, ORANGE, LIME, PINK = '#ff4fb8', '#ff2d3c', '#ff7a3c', '#b8ff5a', '#ff5fa2'
CORAL, WOOD, WOOD_D, GOLD = '#ff7a6a', '#c9a070', '#8e5a3a', '#ffd23c'
ROCK_D, STONE, STONE_D = '#6a4562', '#d8d2c0', '#9c9684'
KELP, KELP_L, KELP_D, SOIL = '#5fbf4a', '#a8e36a', '#2f7f4a', '#5a4a3c'
PALE, BUBBLE = '#ffe9d6', '#e6f6ff'
PAN = (MAG, WHITE, YEL, WHITE, ORANGE, WHITE)

# what a bystander does (figure() values). A hop (bob > 0) is slow here: this is water.
CHEER = (
    dict(arm_l='up', arm_r='up', swing_l=16, swing_r=-16, bob=22, rate=0.55, react=True),
    dict(arm_l='hold', arm_r='hold', swing_l=14, swing_r=-14, axis_l='z', axis_r='z', bob=-6, rate=1.2),
    dict(arm_r='wave', swing_r=28, arm_l='hip', bob=-10, rate=0.7),
    dict(arm_l='vee', arm_r='vee', swing_l=12, swing_r=-12, bob=28, rate=0.5, react=True),
    dict(arm_l='wave', swing_l=26, arm_r='up', swing_r=-12, bob=18, rate=0.65, react=True),
    dict(arm_r='point', swing_r=10, axis_r='x', arm_l='down', bob=-8, rate=0.6),
)


# ------------------------------------------------------------------------------------------ helpers
def naut(sc, r, x, y, z=0.0, yaw=0.0, s=1.25, suit=None, **kw):
    """One aquanaut, about 120 triangles: figure() at detail 0 with the head itself as the white helmet, and the dark
    visor drawn on with the body's own motion values (so it stays on the face whatever the figure does)."""
    suit = suit or r.choice(SUITS)
    d = dict(skin=HELM, hair='bald', hat=None, top=suit, bottom=suit, shoes=DARK, sleeves='long', wear='trousers',
             build=r.choice(('avg', 'stocky', 'avg', 'slim')), pack=r.choice((STEEL, YEL, DARK)), detail=0, s=s)
    d.update(kw)
    top = P.figure(sc, x, y, z=z, yaw=yaw, **d)
    s = d['s']
    hr = P.BUILDS[d['build']][3]
    drop = (66.0 - d.get('seat', 46.0)) if d.get('legs') == 'sit' else 0.0
    tv, base = (0.0, 0.0), (x, y)
    if d.get('travel'):
        tv = (float(d['travel'][0]), float(d['travel'][1]) / float(d['travel'][0]))
    elif d.get('orbit'):
        base, tv = (d['orbit'][0], d['orbit'][1]), (-1.0, float(d['orbit'][2]))
    mo = dict(phase=d.get('phase', 0.0), rate=d.get('rate', 0.0), bob=d.get('bob', 0.0), react=d.get('react', False), base=base, travel=tv)
    if d.get('sway'):
        mo.update(d['sway'])
    elif d.get('rock'):
        mo.update(swing=d['rock'], pivot=(x, y, z), axis=yaw + 90.0)
    elif d.get('turn'):
        mo.update(swing=d['turn'], pivot=(x, y, z), axis=P.UP)
    else:
        mo.update(pivot=(x, y, z + s * 100.0), axis=yaw)
    v = sc.part(P.motion(**mo), (x, y, z), yaw, s)
    v.box(VISOR, (0, -(hr * 0.92 - 1.0), 127.0 + hr - drop), (hr * 1.45, 8, 16))
    return top


def with_body(sc, x, y, z, yaw=0.0, s=1.25, phase=0.0, rate=0.0, bob=0.0, react=False):
    """The model that moves as a plain figure's body does (no rock, turn or travel): for a veil, a hat, a thing worn.
    Drawn in the MESH's space."""
    return sc.part(P.motion(phase=phase, rate=rate, bob=bob, react=react, base=(x, y), pivot=(x, y, z + s * 100.0), axis=yaw))


def platform(sc, L, D, Z, y=0.0, off=0):
    """A deck Z over the sand on four legs with footings 2 m down, blocks of colour along its front edge."""
    g = sc.still()
    g.box(DECK, (0, y, Z - 10), (L, D, 20))
    g.box(DECK_D, (0, y, Z - 36), (L - 60, D - 60, 32))
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(HULL_D, (sx * (L / 2.0 - 70), y + sy * (D / 2.0 - 60), (Z - 52 - 200) / 2.0), (30, 30, Z - 52 + 200))
    n = max(int(L / 220.0), 3)
    w = L / float(n)
    for k in range(n):
        g.box(PAN[(k + off) % len(PAN)], (-L / 2.0 + (k + 0.5) * w, y - D / 2.0 - 3, Z - 30), (w - 10, 6, 56))


def bubbles(sc, x, y, z, n=3, phase=0.0, size=12.0):
    """A string of bubbles standing over a point, each rising and settling out of step."""
    for k in range(n):
        b = sc.part(P.motion(phase=phase + 0.31 * k, rate=0.45, bob=-(26.0 + 8.0 * k), pivot=(x, y, z)))
        b.sphere(BUBBLE, (x + (k % 2) * 22 - 11, y, z + k * 62), size + 5 * k, steps=(3, 6))


def pennant(sc, x, y, z0, h, col, stripe, phase=0.0, w=130.0, hang=220.0):
    """A tall pennant on a pole: flat colour that reads from far off; it swings a little about the pole."""
    g = sc.still()
    g.box(DECK_D, (x, y, z0 + h / 2.0), (9, 9, h))
    f = sc.part(P.motion(phase=phase, rate=0.4, swing=10.0, pivot=(x, y, z0 + h), axis=P.UP))
    cx = x + w / 2.0 + (5 if w > 0 else -5)
    f.box(col, (cx, y, z0 + h - hang / 2.0), (abs(w), 5, hang))
    f.box(stripe, (cx, y, z0 + h - hang / 2.0), (abs(w) + 2, 7, hang * 0.26))


def bunting(g, a, b, n=6, cols=(MAG, YEL, WHITE, ORANGE)):
    """A line of flags from a to b."""
    tilt_box(g, DARK, a, b, 5, 5)
    for k in range(n):
        t = (k + 0.5) / n
        g.box(cols[k % len(cols)], (a[0] + (b[0] - a[0]) * t, a[1] + (b[1] - a[1]) * t, a[2] + (b[2] - a[2]) * t - 26), (40, 5, 44))


# ---- what a hand holds: f(model, hand, side), hand in the figure's own frame
def hold_ball(col=WHITE, rad=16.0, up=14.0):
    def f(m, h, side):
        m.sphere(col, (h[0], h[1], h[2] + up), rad, steps=(3, 6))
    return f


def hold_bouquet():
    def f(m, h, side):
        m.sphere(MAG, (h[0], h[1] - 8, h[2] + 16), 17, steps=(3, 6))
        m.box(KELP, (h[0], h[1] - 4, h[2] - 4), (6, 6, 22))
    return f


def hold_conch():
    def f(m, h, side):
        m.cyl(PALE, (h[0], h[1] - 4, h[2] + 4), 7, 46, rot=(0, 0, 90), steps=6, top_r=22)
    return f


def hold_tuba():
    def f(m, h, side):
        m.cyl(YEL, (h[0] - side * 12, h[1] - 12, h[2] - 30), 16, 46, steps=8)
        m.cyl(YEL, (h[0] - side * 12, h[1] - 12, h[2] + 16), 16, 62, steps=8, top_r=48)
    return f


def hold_board(col=WHITE):
    def f(m, h, side):
        m.box(col, (h[0], h[1] - 8, h[2] + 10), (30, 4, 40))
    return f


def hold_stick(col=WOOD):
    def f(m, h, side):
        m.box(col, (h[0], h[1] - 20, h[2] + 4), (5, 44, 5))
    return f


def hold_pole(col=STEEL, length=250.0, tip=RED, bands=False):
    """A long pole held upright (a knife on a staff, a ranging pole in bands)."""
    def f(m, h, side):
        if bands:
            n = 5
            for k in range(n):
                m.box(RED if k % 2 else WHITE, (h[0], h[1], h[2] - 60 + (k + 0.5) * length / n), (7, 7, length / n))
        else:
            m.box(col, (h[0], h[1], h[2] - 40 + length / 2.0), (5, 5, length))
            m.box(tip, (h[0], h[1], h[2] - 40 + length + 14), (9, 30, 34))
    return f


def hold_paddle(col=ORANGE):
    def f(m, h, side):
        m.box(WOOD, (h[0], h[1], h[2] + 16), (5, 5, 30))
        m.box(col, (h[0], h[1], h[2] + 46), (34, 5, 34))
    return f


def hold_urn(col=ORANGE):
    def f(m, h, side):
        if side > 0:
            m.cyl(col, (h[0] - 26, h[1], h[2] + 8), 15, 30, steps=6, top_r=24)
            m.cyl(col, (h[0] - 26, h[1], h[2] + 38), 24, 22, steps=6, top_r=11)
    return f


def hold_scissors():
    def f(m, h, side):
        tilt_box(m, STEEL, (h[0], h[1] - 6, h[2]), (h[0] - 12, h[1] - 76, h[2] + 26), 6, 12)
        tilt_box(m, STEEL, (h[0], h[1] - 6, h[2] + 14), (h[0] + 12, h[1] - 76, h[2] - 10), 6, 12)
        m.box(ORANGE, (h[0], h[1] + 6, h[2] + 7), (22, 12, 30))
    return f


def hold_shears():
    """What a swimmer of the kelp farm holds: f(model, W) for ft_people_sea.swimmer."""
    def f(m, W):
        tilt_box(m, STEEL, W(104, -8, -26), W(160, -22, -20), 6, 14)
        tilt_box(m, STEEL, W(104, 8, -26), W(160, 22, -20), 6, 14)
        tilt_box(m, ORANGE, W(92, 0, -26), W(108, 0, -26), 30, 10)
    return f


# ------------------------------------------------------------------------------------------ the scenes
def wedding(sc):
    """A wedding under a coral arch. 12 figures."""
    r = random.Random(61)
    Z = 280.0
    platform(sc, 1100, 600, Z, y=-10.0, off=0)
    g = sc.still()
    AY = 170.0
    pts = [(230.0, Z)] + [(math.cos(math.radians(a)) * 230.0, Z + 250.0 + math.sin(math.radians(a)) * 230.0) for a in range(0, 181, 30)] + [(-230.0, Z)]
    for k in range(len(pts) - 1):
        tilt_box(g, CORAL if k % 2 else WHITE, (pts[k][0], AY, pts[k][1]), (pts[k + 1][0], AY, pts[k + 1][1]), 44, 44)
    for k, a in enumerate((20, 55, 90, 125, 160)):
        g.sphere((MAG, YEL, WHITE)[k % 3], (math.cos(math.radians(a)) * 240.0, AY - 22, Z + 250.0 + math.sin(math.radians(a)) * 240.0), 34, steps=(3, 6))
    g.box(MAG, (0, -70, Z + 3), (250, 440, 6))
    g.box(DECK_D, (0, 120, Z + 23), (150, 100, 46))
    # the couple, hand in hand, rising and settling together
    cm = dict(phase=0.15, rate=0.35, bob=-12.0)
    naut(sc, r, -52, -30, Z + 6, yaw=8, s=1.3, suit=WHITE, pack=WHITE, build='slim', arm_r=((75, 0), (85, 0)), arm_l='hold', hold_l=hold_bouquet(), **cm)
    v = with_body(sc, -52, -30, Z + 6, 8, 1.3, **cm)
    v.box(WHITE, (-56, 8, Z + 6 + 1.3 * 118), (62, 8, 150))
    v.cyl(GOLD, (-52, -30, Z + 6 + 1.3 * 171), 20, 12, steps=6)
    naut(sc, r, 52, -30, Z + 6, yaw=-8, s=1.35, suit=MAG, pack=DARK, build='avg', band=YEL, arm_l=((75, 0), (85, 0)), arm_r='wave', swing_r=22, **cm)
    v = with_body(sc, 52, -30, Z + 6, -8, 1.35, **cm)
    v.cyl(DARK, (52, -30, Z + 6 + 1.35 * 170), 34, 5, steps=8)
    v.cyl(DARK, (52, -30, Z + 6 + 1.35 * 170), 22, 40, steps=8)
    naut(sc, r, 0, 120, Z + 46, s=1.3, suit=ORANGE, band=WHITE, arm_l='vee', arm_r='vee', swing_l=10, swing_r=-10, rate=0.4, phase=0.6)
    spots = ((-250, -150, 35), (-360, 20, 40), (-470, -130, 45), (250, -150, -35), (360, 20, -40), (470, -130, -45))
    for i, (x, y, yaw) in enumerate(spots):
        naut(sc, r, x, y, Z, yaw=yaw + r.uniform(-8, 8), s=r.uniform(1.18, 1.3), phase=r.random(), **CHEER[(i * 2 + 1) % len(CHEER)])
    for sx, col in ((-1, YEL), (1, WHITE)):
        naut(sc, r, sx * 160, -240, Z, yaw=-sx * 18, s=0.9, build='kid', suit=(LIME, PINK)[sx > 0], arm_r='up', hold_r=P.hold_flag(col=col, w=70.0), swing_r=18,
             arm_l='hip', bob=22, rate=0.6, phase=r.random(), react=True)
    naut(sc, r, 440, 160, Z, yaw=-30, s=1.25, suit=YEL, arm_r='face', hold_r=hold_conch(), arm_l='hip', rock=5, rate=0.5, phase=0.3)
    bubbles(sc, 440, 110, Z + 300, 3, 0.2)
    for k, (sg, zz) in enumerate(((1, Z + 600.0), (-1, Z + 690.0))):
        S.swimmer(sc, 0.0, -sg * 250.0, zz, heading=0.0 if sg > 0 else 180.0, pitch=4, s=1.2, suit=(YEL, LIME)[k], fins=(RED, WHITE)[k], arms='wave', wave=24.0,
                  kick=16.0, rate=0.9, phase=0.5 * k, bob=-8.0, orbit=(0.0, 0.0, 0.06), bubbles=False)
    for k in range(5):
        b = sc.part(P.motion(phase=r.random(), rate=0.3, bob=-46.0, pivot=(0.0, 0.0, Z + 400.0)))
        b.sphere(BUBBLE, (-330 + k * 165, -60 + (k % 2) * 90, Z + 330 + (k % 3) * 60), 13 + 3 * (k % 2), steps=(3, 6))


def pearl_market(sc):
    """The pearl market: three stalls, their vendors, hagglers, a porter. 9 figures."""
    r = random.Random(62)
    Z = 260.0
    platform(sc, 1240, 500, Z, y=-20.0, off=2)
    g = sc.still()
    AW = ((MAG, WHITE), (YEL, WHITE), (ORANGE, WHITE))
    for k, cx in enumerate((-400.0, 0.0, 400.0)):
        g.box(WOOD, (cx, 90, Z + 45), (310, 90, 90))
        g.box(AW[k][0], (cx, 43, Z + 50), (310, 4, 60))
        for sx in (-1, 1):
            g.box(DECK_D, (cx + sx * 160, 200, Z + 195), (10, 10, 390))
            g.box(DECK_D, (cx + sx * 160, -10, Z + 153), (10, 10, 306))
        for j in range(5):
            xx = cx - 136 + j * 68
            tilt_box(g, AW[k][j % 2], (xx, 215, Z + 392), (xx, -20, Z + 306), 68, 8)
    for dx in (-90.0, 30.0):                              # the clams and their pearls
        cx = -400.0 + dx
        g.box(PALE, (cx, 90, Z + 97), (92, 70, 14))
        tilt_box(g, PINK, (cx, 118, Z + 104), (cx, 150, Z + 176), 92, 12)
        g.sphere(WHITE, (cx, 84, Z + 124), 20, steps=(3, 6))
    g.box(DECK_D, (-290, 90, Z + 140), (6, 6, 100))       # the scales
    sb = sc.part(P.motion(rate=0.5, swing=8.0, pivot=(-290.0, 90.0, Z + 190.0), axis=90.0))
    sb.box(STEEL, (-290, 90, Z + 190), (120, 5, 5))
    for sx in (-1, 1):
        sb.box(STEEL, (-290 + sx * 55, 90, Z + 162), (36, 36, 5))
    g = sc.still()
    for k, (dx, col, rad) in enumerate(((-100, YEL, 30), (-40, MAG, 24), (20, YEL, 34), (80, ORANGE, 26), (120, LIME, 20))):
        g.sphere(col, (dx, 90, Z + 90 + rad * 0.8), rad, steps=(3, 6))
    for k, dx in enumerate((-110, -50, 20, 90)):
        g.cyl((ORANGE, CORAL)[k % 2], (400 + dx, 90, Z + 90), 18, 44, steps=6, top_r=26)
        g.cyl((ORANGE, CORAL)[k % 2], (400 + dx, 90, Z + 134), 26, 24, steps=6, top_r=10)
    naut(sc, r, -400, 180, Z, s=1.3, suit=PINK, arm_r='up', swing_r=10, hold_r=hold_ball(WHITE, 17), arm_l='hip', bob=-6, rate=0.5, phase=0.1)
    naut(sc, r, 0, 180, Z, s=1.25, suit=LIME, arm_l='vee', arm_r='vee', swing_l=12, swing_r=-12, bob=16, rate=0.6, phase=0.5, react=True)
    naut(sc, r, 400, 180, Z, s=1.25, suit=RED, arm_l='carry', arm_r='carry', hold_r=P.hold_box(col=WOOD), rock=4, rate=0.4, phase=0.7)
    naut(sc, r, -560, -100, Z, yaw=35, s=1.25, arm_r='point', swing_r=10, axis_r='x', arm_l='down', hold_l=P.hold_bag(col=MAG), bob=12, rate=0.6, phase=r.random(), react=True)
    naut(sc, r, -230, -70, Z, yaw=170, s=1.3, arm_l='out', arm_r='out', swing_l=12, swing_r=-12, rate=0.9, phase=r.random())
    naut(sc, r, 150, -80, Z, yaw=190, s=1.2, arm_r='face', arm_l='hip', rock=5, rate=0.5, phase=r.random())
    naut(sc, r, 300, -180, Z, yaw=-10, s=0.9, build='kid', suit=YEL, arm_l='up', arm_r='up', swing_l=16, swing_r=-16, bob=24, rate=0.6, phase=r.random(), react=True)
    naut(sc, r, 560, -110, Z, yaw=-35, s=1.25, arm_r='wave', swing_r=28, arm_l='down', hold_l=P.hold_bag(col=YEL), bob=-8, rate=0.7, phase=r.random())
    naut(sc, r, 0, -228, Z, yaw=90, s=1.2, suit=ORANGE, arm_l='carry', arm_r='carry', hold_r=P.hold_box(col=WOOD_D), stride=22, bob=3, rate=1.0, phase=0.2,
         travel=(1100.0, 60.0))
    bubbles(sc, 0, 150, Z + 430, 3, 0.4)


def treasure_haul(sc):
    """A chest of gold coming up under a tripod hoist. 8 figures."""
    r = random.Random(63)
    g = sc.still()
    T = 250.0
    S._rock(g, r, 0, 20, 1000, 500, T, n=5)
    g = sc.still()
    g.box(DARK, (0, 20, T + 2), (300, 190, 6))
    AP = (0.0, 20.0, 820.0)
    for fx, fy in ((-300, -120), (300, -120), (0, 190)):
        rod(g, YEL, (fx, fy, T - 10), AP, 10, steps=6)
    g.cyl(DARK, (AP[0], AP[1], AP[2] - 20), 26, 40, steps=6)
    tilt_box(g, DARK, (0, 14, 812), (-316, -60, T + 1.25 * 112), 6, 6)                    # the hauling rope
    g.box(WOOD_D, (350, 130, T + 40), (130, 90, 80))                                       # a second chest, open
    g.box(GOLD, (350, 130, T + 84), (112, 72, 12))
    tilt_box(g, WOOD_D, (350, 172, T + 80), (350, 210, T + 150), 130, 10)
    for k, (ax, ay) in enumerate(((-400, 150), (-340, 185), (-270, 160))):
        g.cyl((ORANGE, CORAL)[k % 2], (ax, ay, T), 20, 50, steps=6, top_r=30)
        g.cyl((ORANGE, CORAL)[k % 2], (ax, ay, T + 50), 30, 26, steps=6, top_r=12)
    g.box(WOOD, (300, -20, T + 23), (60, 50, 46))                                          # the counter's seat
    c = sc.part(P.motion(rate=0.3, swing=5.0, pivot=AP, axis=90.0))                         # the chest, swinging on its chain
    c.box(DARK, (0, 20, 640), (7, 7, 350))
    c.box(WOOD_D, (0, 20, 400), (250, 140, 120))
    for sx in (-1, 1):
        c.box(GOLD, (sx * 85, 20, 400), (14, 146, 126))
        tilt_box(c, DARK, (sx * 110, 20, 462), (0, 20, 500), 6, 6)
    c.box(GOLD, (0, 12, 466), (226, 110, 24))
    for k in range(3):
        c.sphere(GOLD, (-70 + k * 70, 4, 484), 22, steps=(3, 6))
    tilt_box(c, WOOD_D, (0, 88, 462), (0, 150, 566), 250, 14)
    naut(sc, r, -335, -62, T, yaw=72, s=1.25, suit=ORANGE, arm_l='push', arm_r='push', swing_l=14, swing_r=14, axis_l='x', axis_r='x', rate=0.6, phase=0.0)
    naut(sc, r, -440, -40, T, yaw=78, s=1.3, suit=YEL, arm_l='push', arm_r='push', swing_l=14, swing_r=14, axis_l='x', axis_r='x', rate=0.6, phase=0.5)
    naut(sc, r, 235, -70, T, yaw=-60, s=1.25, suit=PINK, arm_r='up', swing_r=10, arm_l='out', bob=-6, rate=0.5, phase=0.3)
    naut(sc, r, 300, -20, T, yaw=-20, s=1.2, suit=LIME, legs='sit', seat=46, arm_l='hold', arm_r='hold', hold_r=hold_ball(GOLD, 9, 10), rock=3, rate=0.5, phase=0.1)
    naut(sc, r, -150, -150, T, yaw=150, s=1.25, suit=WHITE, arm_l='hold', arm_r='hold', hold_r=P.hold_camera(), rock=4, rate=0.4, phase=0.6)
    naut(sc, r, 120, -150, T, yaw=0, s=1.25, suit=RED, arm_l='vee', arm_r='vee', swing_l=12, swing_r=-12, bob=30, rate=0.5, phase=0.2, react=True)
    naut(sc, r, -20, -155, T, yaw=10, s=1.2, suit=MAG, arm_r='point', swing_r=8, axis_r='x', arm_l='hold', hold_l=hold_board(), bob=-6, rate=0.6, phase=0.8)
    S.swimmer(sc, 0.0, -250.0, 900.0, heading=0.0, pitch=-8, s=1.2, suit=YEL, fins=RED, arms='tow', hold=S.hold_camera_housing(), kick=16.0, rate=0.9,
              phase=0.4, bob=-8.0, orbit=(0.0, 0.0, 0.05), bubbles=True)


def seahorse_race(sc):
    """The seahorse race round the judges' tower. 10 figures."""
    r = random.Random(64)
    g = sc.still()
    Z = 300.0
    g.cyl(HULL_D, (0, 0, -200), 60, Z + 180, steps=8)
    g.cyl(DECK_D, (0, 0, Z - 44), 120, 24, steps=12)
    g.cyl(DECK, (0, 0, Z - 20), 165, 20, steps=12)
    for k in range(7):
        a = math.radians(-90 + (k - 3) * 26)
        g.box(PAN[k % len(PAN)], (math.cos(a) * 167, math.sin(a) * 167, Z - 12), (70, 6, 40), rot=(0, math.degrees(a) + 90.0, 0))
    g.cyl(YEL, (0, 30, Z), 46, 80, steps=8)
    g.box(DECK_D, (0, 80, Z + 450), (10, 10, 900))
    for sx in (-1, 1):
        g.box(DECK_D, (sx * 700, 0, 350), (12, 12, 1100))
        g.box(HULL_D, (sx * 700, 0, -40), (70, 70, 80))
        bunting(g, (0, 80, Z + 890), (sx * 700, 0, 890), 7)
        g.box((WHITE, DARK)[sx > 0], (sx * 700 - sx * 50, 0, 860), (90, 5, 60))
    naut(sc, r, 0, 30, Z + 80, s=1.25, suit=RED, band=WHITE, arm_r='up', hold_r=P.hold_flag(col=WHITE, w=80.0), arm_l='hip', turn=35, rate=0.3, phase=0.0)
    for i, a in enumerate((-72, -36, 0, 36, 72)):
        ra = math.radians(a)
        naut(sc, r, math.sin(ra) * 116, -math.cos(ra) * 116, Z, yaw=a, s=1.2 if i != 2 else 0.9, build='kid' if i == 2 else 'avg', phase=r.random(),
             **CHEER[(i * 2) % len(CHEER)])
    lanes = ((235.0, 1, 290.0, 0.07, YEL, ORANGE, RED), (290.0, -1, 330.0, 0.055, PINK, WHITE, LIME), (235.0, -1, 560.0, 0.06, LIME, YEL, MAG),
             (290.0, 1, 610.0, 0.05, ORANGE, WHITE, WHITE))
    for k, (R, sg, zz, rps, col, trim, silk) in enumerate(lanes):
        x0, y0 = 0.0, -sg * R
        ph, rt, bb = r.random(), r.uniform(0.3, 0.4), -r.uniform(18, 28)
        m = sc.part(P.motion(phase=ph, rate=rt, bob=bb, pivot=(x0, y0, zz + 150.0), base=(0.0, 0.0), travel=(-1.0, rps)))
        S._seahorse(m, x0, y0, zz, 1.0, col, trim, sg)
        naut(sc, r, x0 - sg * 4.0, y0, zz + 150.0, yaw=90 * sg, s=0.95, suit=silk, band=(DARK, MAG, YEL, RED)[k], pack=None, build='slim', legs='sit', seat=46,
             arm_l='hold', arm_r='up' if k % 2 == 0 else 'hold', phase=ph, rate=rt, bob=bb, orbit=(0.0, 0.0, rps))


def kelp_harvest(sc):
    """The kelp farm at harvest. 8 figures and a turtle."""
    r = random.Random(65)
    Z = 260.0
    g = sc.still()
    g.box(ROCK_D, (0, 200, 0), (1160, 220, 240))
    g.box(SOIL, (0, 200, 124), (1120, 190, 10))
    platform(sc, 1100, 300, Z, y=-150.0, off=1)
    for k in range(5):
        x = -440.0 + k * 220.0
        h = 640.0 if k != 2 else 190.0
        st = sc.part(P.motion(phase=r.random(), rate=0.22, swing=5.0, pivot=(x, 200.0, 120.0), axis=90.0))
        st.box(KELP_D, (x, 200, 120 + h / 2.0), (24, 24, h))
        if k == 2:
            continue
        for j in range(4):
            sd = 1 if (j + k) % 2 else -1
            z0 = 230.0 + j * 120.0
            tilt_box(st, KELP if j % 2 else KELP_L, (x, 200, z0), (x + sd * 115, 200, z0 + 150), 12, 62)
        st.sphere(KELP_L, (x, 200, 120 + h), 28, steps=(3, 6))
    g = sc.still()
    for k, (bx, by, col) in enumerate(((-70, -60, YEL), (-20, -60, WOOD), (-45, -60, YEL))):       # baskets, stacked
        g.box(col, (bx if k < 2 else -45, by, Z + 22 + (44 if k == 2 else 0)), (48, 60, 44))
        g.box(KELP, (bx if k < 2 else -45, by, Z + 48 + (44 if k == 2 else 0)), (40, 52, 10))
    S.swimmer(sc, -220.0, 120.0, 560.0, heading=90.0, pitch=58, s=1.2, suit=ORANGE, fins=YEL, arms='tow', hold=hold_shears(), kick=10.0, rate=0.8, phase=0.2,
              bob=-10.0, bubbles=True)
    S.swimmer(sc, 220.0, 120.0, 690.0, heading=90.0, pitch=50, s=1.2, suit=PINK, fins=WHITE, arms='tow', hold=hold_shears(), kick=10.0, rate=0.7, phase=0.6,
              bob=-12.0, bubbles=False)
    naut(sc, r, 0, -245, Z, yaw=90, s=1.2, suit=YEL, arm_l='carry', arm_r='carry', hold_r=P.hold_box(col=KELP, w=60.0), stride=22, bob=3, rate=1.0, phase=0.3,
         travel=(1000.0, 55.0))
    naut(sc, r, -400, -120, Z, yaw=20, s=1.25, suit=LIME, arm_r='up', hold_r=hold_pole(STEEL, 260.0, RED), swing_r=10, axis_r='x', arm_l='hip', rate=0.6, phase=0.1)
    naut(sc, r, -160, -90, Z, yaw=150, s=1.25, suit=ORANGE, arm_l='carry', arm_r='carry', swing_l=16, swing_r=16, axis_l='x', axis_r='x', rate=0.7, phase=0.4)
    naut(sc, r, 170, -130, Z, yaw=-10, s=1.3, suit=WHITE, band=RED, arm_r='point', swing_r=8, axis_r='x', arm_l='hold', hold_l=hold_board(YEL), bob=10, rate=0.6,
         phase=0.7, react=True)
    naut(sc, r, 480, -110, Z, yaw=-25, s=1.25, suit=MAG, phase=0.9, **CHEER[3])
    # the turtle that carries the net, and the child who rides her
    tx, ty, tz, tph, trt, tbb = 320.0, -170.0, 640.0, 0.3, 0.45, -14.0
    S._turtle(sc, tx, ty, tz, 180.0, 0.7, '#c9853a', '#d7e38a', '#f1e4c8', tph, trt, None, flap=28.0, bob=tbb)
    nb = sc.part(P.motion(phase=tph, rate=trt, bob=tbb, pivot=(tx, ty, tz), base=(tx, ty)))
    nb.box(KELP, (tx, ty, tz - 86), (120, 80, 50))
    for sx in (-1, 1):
        nb.box(DARK, (tx + sx * 50, ty, tz - 40), (5, 5, 70))
    naut(sc, r, tx, ty, tz + 34 - 0.85 * 46, yaw=-90, s=0.85, build='kid', suit=YEL, pack=None, legs='sit', seat=46, arm_r='wave', swing_r=26, arm_l='hold',
         phase=tph, rate=trt, bob=tbb)


def school_trip(sc):
    """A class of Nereida's children out to see the chase. 13 figures."""
    r = random.Random(66)
    g = sc.still()
    L = 920.0
    tiers = ((-150.0, 240.0), (-20.0, 320.0), (110.0, 400.0))
    for row, (y, z) in enumerate(tiers):
        g.box(DECK, (0, y, z - 8), (L, 130, 16))
        for k in range(4):
            g.box(PAN[(k + row) % len(PAN)], (-L / 2.0 + (k + 0.5) * L / 4.0, y - 66, z - 36), (L / 4.0 - 10, 6, 52))
        for sx in (-1, 1):
            g.box(HULL_D, (sx * (L / 2.0 - 40), y, (z - 16 - 200) / 2.0), (24, 24, z - 16 + 200))
    g.box(DECK_D, (-400, 190, 410), (10, 10, 820))                                         # the lesson board: a fish, a star
    g.box(WHITE, (-400, 184, 720), (250, 8, 170))
    g.box(ORANGE, (-420, 178, 722), (110, 4, 56))
    tilt_box(g, ORANGE, (-365, 178, 722), (-320, 178, 752), 4, 20)
    tilt_box(g, ORANGE, (-365, 178, 722), (-320, 178, 692), 4, 20)
    g.box(MAG, (-320, 178, 770), (30, 4, 30))
    pennant(sc, 430, 190, 0, 800, YEL, MAG, 0.4, w=-140.0)
    packs = (RED, LIME, ORANGE, WHITE, PINK, MAG)
    per = (3, 4, 3)
    i = 0
    for row, (y, z) in enumerate(tiers):
        for k in range(per[row]):
            x = -290.0 + (k + 0.5) * 590.0 / per[row] + r.uniform(-12, 12)
            naut(sc, r, x, y + r.uniform(-8, 8), z, yaw=r.uniform(-12, 12), s=0.92, build='kid', suit=YEL, band=MAG, pack=packs[i % len(packs)],
                 phase=r.random(), **CHEER[(i * 5 + 2) % len(CHEER)])
            i += 1
    naut(sc, r, -400, -150, 240, yaw=10, s=1.35, suit=WHITE, band=RED, build='slim', arm_r='point', swing_r=8, axis_r='x', arm_l='hip', bob=-6, rate=0.5, phase=0.2)
    naut(sc, r, 395, -20, 320, yaw=-10, s=1.3, suit=ORANGE, band=WHITE, arm_l='up', arm_r='up', swing_l=12, swing_r=-12, rate=0.6, phase=0.5)
    naut(sc, r, 420, -50, 650, yaw=-15, s=0.9, build='kid', suit=YEL, band=MAG, pack=LIME, legs='stride', arm_r='up', hold_r=hold_ball(ORANGE, 52, 120), arm_l='out',
         bob=-45, rate=0.22, phase=0.1)


def statue_dig(sc):
    """The dig: a sea king's stone head, and those who are getting him out. 8 figures."""
    r = random.Random(67)
    g = sc.still()
    S._rock(g, r, 0, 60, 1000, 420, 130, n=5)
    g = sc.still()
    HX, HY, HZ = 80.0, 100.0, 330.0
    g.cyl(STONE_D, (HX, HY, 40), 150, 130, steps=8)
    g.sphere(STONE, (HX, HY, HZ), 240, scale=(0.82, 0.9, 1.1), steps=(5, 10))
    g.box(STONE_D, (HX, HY - 226, HZ), (50, 60, 110))
    g.box(STONE_D, (HX, HY - 206, HZ + 70), (260, 40, 34))
    g.box(STONE_D, (HX, HY - 200, HZ - 95), (120, 24, 16))
    for sd in (-1, 1):
        g.box(DARK, (HX + sd * 75, HY - 207, HZ + 42), (60, 20, 22))
    for k in range(5):
        g.box(STONE_D, (HX + (k - 2) * 70, HY - 30, HZ + 262 + (30 if k % 2 == 0 else 0)), (40, 40, 110))
    g.sphere(MAG, (HX + 175, HY - 90, HZ + 90), 58, steps=(3, 6))
    g.sphere(CORAL, (HX - 150, HY - 70, HZ + 180), 46, steps=(3, 6))
    rod(g, STONE_D, (-400, 220, 60), (-340, 190, 640), 14, steps=6)                        # the king's trident, broken
    tilt_box(g, STONE_D, (-400, 190, 640), (-280, 190, 640), 20, 20)
    for dx in (-60, 0, 60):
        g.box(STONE_D, (-340 + dx, 190, 700), (18, 18, 120))
    Z = 270.0
    SY = -205.0
    g.box(WOOD, (40, SY, Z - 8), (780, 150, 16))                                           # the scaffold
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(DECK_D, (40 + sx * 370, SY + sy * 62, (Z + 110 - 200) / 2.0), (12, 12, Z + 110 + 200))
        g.box(DECK_D, (40 + sx * 370, SY, Z + 104), (12, 136, 10))
    g.box(YEL, (40, SY - 76, Z - 30), (780, 6, 40))
    ZF = 240.0
    g.box(DECK, (480, -150, ZF - 8), (200, 240, 16))                                       # the finds deck
    for sy in (-1, 1):
        g.box(HULL_D, (530, -150 + sy * 90, (ZF - 16 - 200) / 2.0), (24, 24, ZF - 16 + 200))
    g.box(WOOD, (490, -80, ZF + 35), (150, 60, 70))
    for k, dx in enumerate((-45, 0, 45)):
        g.cyl((ORANGE, CORAL, GOLD)[k], (490 + dx, -80, ZF + 70), 14, 34, steps=6, top_r=20)
    g.box(WOOD, (520, -10, ZF + 23), (50, 46, 46))
    naut(sc, r, -90, SY, Z, yaw=165, s=1.25, suit=YEL, arm_r='fwd', hold_r=P.hold_tool(col=WOOD, grip=YEL, length=20.0), swing_r=18, axis_r='z', arm_l='hip',
         rate=1.0, phase=0.1)
    naut(sc, r, 200, SY, Z, yaw=195, s=1.25, suit=ORANGE, arm_r=((110, 70), (120, 80)), hold_r=P.hold_tool(), swing_r=24, axis_r='x', arm_l='fwd', rate=1.3, phase=0.4)
    naut(sc, r, -260, SY - 4, Z, yaw=0, s=1.3, suit=PINK, arm_r='fwd', hold_r=hold_pole(length=270.0, bands=True), arm_l='point', swing_l=8, axis_l='x', bob=8,
         rate=0.6, phase=0.6, react=True)
    naut(sc, r, 360, SY, Z, yaw=150, s=1.2, suit=WHITE, arm_l='hold', arm_r='hold', hold_r=P.hold_camera(), rock=4, rate=0.4, phase=0.2)
    naut(sc, r, 440, -190, ZF, yaw=-10, s=1.25, suit=LIME, arm_l='up', arm_r='up', hold_r=hold_urn(), bob=20, rate=0.5, phase=0.3, react=True)
    naut(sc, r, 520, -10, ZF, yaw=0, s=1.2, suit=MAG, legs='sit', seat=46, arm_l='hold', arm_r='hold', hold_r=hold_board(), rock=3, rate=0.5, phase=0.7)
    naut(sc, r, HX, HY - 85, HZ + 240 - 1.2 * 46, yaw=0, s=1.2, suit=RED, legs='sit', seat=46, arm_r='wave', swing_r=28, arm_l='face', rate=0.7, phase=0.5)
    cm = dict(rate=0.4, phase=0.1, bob=-10.0)
    W = S.swimmer(sc, -270.0, -60.0, 720.0, heading=20.0, pitch=-25, s=1.2, suit=YEL, fins=RED, arms='tow', kick=10.0, bubbles=True, **cm)
    hp = sc.part(P.motion(pivot=(-270.0, -60.0, 720.0), base=(-270.0, -60.0), **cm))
    rod(hp, ORANGE, W(104, 0, -26), (-130.0, -20.0, 150.0), 15, steps=6)
    rod(hp, DARK, (-130.0, -20.0, 150.0), (-122.0, -18.0, 110.0), 22, steps=6)


def bubble_band(sc):
    """The band in its clam-shell stage. 10 figures."""
    r = random.Random(68)
    Z = 270.0
    platform(sc, 1040, 480, Z, y=-40.0, off=3)
    g = sc.still()
    for k in range(7):
        a = math.radians(18 + k * 24)
        tilt_box(g, (PINK, WHITE)[k % 2], (math.cos(a) * 60, 250, Z + 20), (math.cos(a) * 520, 250, Z + 20 + math.sin(a) * 520), 24, 150)
    g.cyl(ORANGE, (0, 84, Z + 62), 58, 44, rot=(0, 0, 90), steps=10)                        # the drums
    g.cyl(WHITE, (0, 38, Z + 62), 50, 3, rot=(0, 0, 90), steps=10)
    for sx in (-1, 1):
        g.cyl(YEL, (sx * 74, 96, Z + 104), 28, 26, steps=8)
        g.box(DARK, (sx * 74, 96, Z + 52), (6, 6, 104))
        g.box(DARK, (sx * 470, 120, Z + 110), (110, 90, 220))                              # the speakers
        g.cyl(ORANGE, (sx * 470, 73, Z + 150), 36, 4, rot=(0, 0, 90), steps=8)
        g.cyl(ORANGE, (sx * 470, 73, Z + 62), 24, 4, rot=(0, 0, 90), steps=8)
    g.box(DARK, (125, 112, Z + 85), (5, 5, 170))
    g.box(DARK, (0, 150, Z + 23), (46, 46, 46))
    cy = sc.part(P.motion(phase=0.2, rate=1.1, swing=8.0, pivot=(125.0, 112.0, Z + 170.0), axis=0.0))
    cy.cyl(YEL, (125, 112, Z + 170), 42, 4, steps=8)
    naut(sc, r, 0, 150, Z, s=1.2, suit=RED, legs='sit', seat=46, arm_l=((50, 70), (70, 80)), arm_r=((50, 70), (70, 80)), hold_l=hold_stick(), hold_r=hold_stick(),
         swing_l=20, swing_r=-20, axis_l='x', axis_r='x', rate=1.6, phase=0.0)
    naut(sc, r, -290, 90, Z, yaw=15, s=1.3, suit=YEL, build='stocky', arm_l='hold', arm_r='hold', hold_r=hold_tuba(), rock=5, rate=0.5, phase=0.0)
    bubbles(sc, -300, 60, Z + 330, 4, 0.0, 14.0)
    naut(sc, r, 290, 90, Z, yaw=-15, s=1.25, suit=LIME, arm_r='face', hold_r=hold_conch(), arm_l='hip', rock=5, rate=0.5, phase=0.5)
    bubbles(sc, 290, 40, Z + 320, 3, 0.5)
    naut(sc, r, 0, -150, Z, s=1.3, suit=MAG, band=YEL, arm_r='face', hold_r=hold_ball(DARK, 9, 6), arm_l='up', swing_l=20, bob=-8, rate=0.6, phase=0.3)
    for sx in (-1, 1):
        naut(sc, r, sx * 140, -20, Z, yaw=-sx * 8, s=1.2, suit=PINK, arm_l='up', arm_r='up', swing_l=18, swing_r=-18, bob=20, rate=0.8, phase=0.25 + 0.25 * sx, react=True)
        naut(sc, r, sx * 390, -160, Z, yaw=-sx * 20, s=1.25, suit=(ORANGE, WHITE)[sx > 0], arm_l='out', arm_r='vee', turn=40, rate=0.45, phase=0.5 + 0.2 * sx)
        naut(sc, r, sx * 250, -225, Z, yaw=-sx * 10, s=0.9, build='kid', arm_l='vee', arm_r='vee', swing_l=14, swing_r=-14, bob=26, rate=0.7, phase=r.random(), react=True)
    for k in range(5):
        b = sc.part(P.motion(phase=r.random(), rate=0.3, bob=-50.0, pivot=(0.0, 0.0, Z + 400.0)))
        b.sphere(BUBBLE, (-360 + k * 180, -40 + (k % 2) * 110, Z + 360 + (k % 3) * 70), 14 + 4 * (k % 2), steps=(3, 6))


def rescue_drill(sc):
    """A rescue drill at the recompression capsule. 10 figures."""
    r = random.Random(69)
    Z = 260.0
    platform(sc, 1040, 440, Z, y=40.0, off=4)
    g = sc.still()
    CY, CZ = 170.0, Z + 112.0
    rod(g, WHITE, (-430, CY, CZ), (90, CY, CZ), 105, steps=10)
    for x in (-380, -170, 40):
        rod(g, ORANGE, (x - 22, CY, CZ), (x + 22, CY, CZ), 109, steps=10)
    g.sphere(WHITE, (-430, CY, CZ), 105, scale=(0.5, 1, 1), steps=(4, 10))
    rod(g, DARK, (90, CY, CZ), (100, CY, CZ), 72, steps=8)
    for x in (-290, -90):
        g.box(DARK, (x, CY - 104, CZ + 14), (60, 10, 50))
        g.box(HULL_D, (x, CY, Z + 6), (60, 190, 12))
    wh = sc.part(P.motion(rate=0.4, swing=40.0, pivot=(106.0, CY, CZ), axis=0.0))
    wh.box(YEL, (106, CY, CZ), (6, 92, 10))
    wh.box(YEL, (106, CY, CZ), (6, 10, 92))
    g = sc.still()
    for k in range(4):
        g.cyl(YEL, (250 + k * 44, 220, Z), 17, 120, steps=6)
        g.cyl(STEEL, (250 + k * 44, 220, Z + 120), 8, 16, steps=6)
    pennant(sc, -500, 240, Z, 500, ORANGE, WHITE, 0.2, w=140.0)
    tv = (1500.0, 70.0)
    cm = dict(rate=0.9, phase=0.0, bob=-8.0)
    yT, zT = -262.0, Z + 190.0
    S.swimmer(sc, 330.0, yT, zT, heading=0.0, s=1.15, suit=ORANGE, fins=DARK, arms='side', band=WHITE, travel=tv, bubbles=False, **cm)
    S.swimmer(sc, -455.0, yT, zT, heading=0.0, s=1.15, suit=ORANGE, fins=DARK, arms='fwd', band=WHITE, travel=tv, bubbles=False, **cm)
    st = sc.part(P.motion(pivot=(0.0, yT, zT), base=(0.0, yT), travel=(tv[0], tv[1] / tv[0]), **cm))
    st.box(RED, (0, yT, zT - 16), (420, 72, 10))
    for sy in (-1, 1):
        st.box(STEEL, (0, yT + sy * 32, zT - 12), (650, 7, 7))
    S.swimmer(sc, -40.0, yT, zT + 12.0, heading=0.0, s=1.0, suit=YEL, fins=WHITE, arms='wave', wave=24.0, kick=0.0, travel=tv, bubbles=False, **cm)
    naut(sc, r, 440, -110, Z, yaw=-75, s=1.3, suit=WHITE, band=ORANGE, arm_l='up', arm_r='up', hold_l=hold_paddle(), hold_r=hold_paddle(), swing_l=20, swing_r=-20,
         rate=0.8, phase=0.1)
    naut(sc, r, 190, 150, Z, yaw=-90, s=1.25, suit=ORANGE, band=WHITE, arm_l='push', arm_r='push', swing_l=10, swing_r=-10, axis_l='x', axis_r='x', rate=0.6, phase=0.4)
    naut(sc, r, 30, -120, Z, yaw=10, s=1.3, suit=WHITE, band=RED, arm_r='face', arm_l='hold', hold_l=hold_board(YEL), rock=3, rate=0.5, phase=0.6)
    for k in range(4):
        naut(sc, r, -440 + k * 112, -95 + (k % 2) * 30, Z, yaw=r.uniform(-8, 8), s=r.uniform(1.18, 1.28), suit=ORANGE, band=WHITE, phase=r.random(),
             **CHEER[(k * 3) % len(CHEER)])


def sub_launch(sc):
    """The launch of a small yellow submarine. 10 figures."""
    r = random.Random(70)
    Z = 260.0
    g = sc.still()
    SY, SZ, SR = 165.0, 330.0, 145.0
    rod(g, YEL, (-320, SY, SZ), (320, SY, SZ), SR, steps=10)
    for sx in (-1, 1):
        g.sphere(YEL, (sx * 320, SY, SZ), SR, scale=(1.3, 1, 1), steps=(4, 10))
    rod(g, ORANGE, (-40, SY, SZ), (40, SY, SZ), SR + 4, steps=10)
    g.box(YEL, (40, SY, SZ + SR + 50), (220, 110, 110))
    g.box(ORANGE, (40, SY, SZ + SR + 110), (232, 122, 14))
    g.box(DECK_D, (40, SY, SZ + SR + 270), (10, 10, 310))
    for x in (-230, -120, 150, 250):
        g.box(DARK, (x, SY - SR + 4, SZ + 24), (56, 14, 56))
    g.box(ORANGE, (-470, SY, SZ), (120, 14, 250))
    g.box(ORANGE, (-470, SY, SZ), (120, 250, 14))
    for x in (-250, 0, 250):
        g.box(HULL_D, (x, SY, -8), (50, 240, 390))
    tilt_box(g, MAG, (470, SY - 60, SZ + 60), (530, SY - 90, SZ + 150), 14, 60)             # the bow she wears
    tilt_box(g, MAG, (470, SY - 60, SZ + 60), (540, SY - 90, SZ - 10), 14, 60)
    bunting(g, (40, SY, SZ + SR + 420), (500, SY, SZ + 60), 6)
    bunting(g, (40, SY, SZ + SR + 420), (-500, SY, SZ + 60), 6)
    pr = sc.part(P.motion(rate=0.5, swing=60.0, pivot=(-528.0, SY, SZ), axis=0.0))
    pr.box(DARK, (-528, SY, SZ), (10, 200, 30))
    pr.box(DARK, (-528, SY, SZ), (10, 30, 200))
    platform(sc, 1100, 260, Z, y=-200.0, off=5)
    g = sc.still()
    for sx in (-1, 1):
        g.box(WHITE, (sx * 330, -90, Z + 60), (10, 10, 120))
    g.box(MAG, (0, -90, Z + 106), (660, 5, 16))
    g.box(DECK_D, (480, -95, Z + 230), (12, 12, 460))                                      # the davit and the bottle
    tilt_box(g, DECK_D, (480, -95, Z + 455), (480, 0, Z + 470), 10, 10)
    bt = sc.part(P.motion(rate=0.45, swing=20.0, pivot=(480.0, 0.0, Z + 468.0), axis=0.0))
    bt.box(DARK, (480, 0, Z + 318), (5, 5, 300))
    bt.cyl(LIME, (480, 0, Z + 120), 14, 50, steps=6)
    bt.cyl(LIME, (480, 0, Z + 170), 6, 16, steps=6)
    naut(sc, r, 420, -200, Z, yaw=160, s=1.25, suit=PINK, arm_l='up', arm_r='up', swing_l=14, swing_r=-14, bob=14, rate=0.45, phase=0.0)
    naut(sc, r, 0, -175, Z, yaw=180, s=1.3, suit=WHITE, band=MAG, arm_r='fwd', hold_r=hold_scissors(), swing_r=10, axis_r='z', arm_l='hip', rate=0.7, phase=0.2)
    v = with_body(sc, 0, -175, Z, 180, 1.3, phase=0.2, rate=0.7)
    v.cyl(DARK, (0, -175, Z + 1.3 * 170), 33, 5, steps=8)
    v.cyl(DARK, (0, -175, Z + 1.3 * 170), 21, 38, steps=8)
    spots = ((-470, -250), (-360, -180), (-250, -262), (-140, -190), (170, -256), (285, -190))
    for i, (x, y) in enumerate(spots):
        d = dict(CHEER[(i * 2 + 3) % len(CHEER)])
        if i in (1, 4):
            d = dict(arm_r='up', hold_r=P.hold_flag(col=(YEL, MAG)[i == 4], w=70.0), swing_r=18, arm_l='hip', bob=20, rate=0.6, react=True)
        naut(sc, r, x, y, Z, yaw=r.uniform(-12, 12), s=r.uniform(1.15, 1.28) if i != 2 else 0.9, build='kid' if i == 2 else 'avg', phase=r.random(), **d)
    for x, col in ((-235, ORANGE), (265, RED)):
        naut(sc, r, x, SY, SZ + SR - 4, yaw=0, s=1.2, suit=col, arm_r='wave', swing_r=28, arm_l='hip', bob=-4, rate=0.7, phase=r.random())


MOVE = dict(grow=(60.0, 40.0, 140.0), grow_down=(60.0, 40.0, 0.0))
JOBS = [
    ('SM_P3B_Wedding', wedding, MOVE),
    ('SM_P3B_PearlMarket', pearl_market, MOVE),
    ('SM_P3B_TreasureHaul', treasure_haul, MOVE),
    ('SM_P3B_SeahorseRace', seahorse_race, MOVE),
    ('SM_P3B_KelpHarvest', kelp_harvest, MOVE),
    ('SM_P3B_SchoolTrip', school_trip, MOVE),
    ('SM_P3B_StatueDig', statue_dig, MOVE),
    ('SM_P3B_BubbleBand', bubble_band, MOVE),
    ('SM_P3B_RescueDrill', rescue_drill, dict(grow=(440.0, 40.0, 140.0), grow_down=(440.0, 40.0, 0.0))),
    ('SM_P3B_SubLaunch', sub_launch, MOVE),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
