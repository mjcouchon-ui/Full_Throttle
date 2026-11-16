"""FULL THROTTLE (Unreal) the population, level 7: City Center, pass 3, builder p3a (focus: GAPS).
/Game/People/CityCenter/SM_P3A_*. Placed by the block C:/ftwork/p3/L7a.inl (pasted at the end of FTPeopleL7.inl).

WHERE THE GAPS ARE (read from FTPeopleL7.inl and FTDressL7.inl):
  - SYNDICATE TERRITORY, 7.66 km to the end: 70 % of the road. The first pass stands the Chairman's men in every fourth
    pocket a side and onlookers in every fourth; half the pockets are empty, the blocks between them wholly, and on the
    circuit (13.56 km on) three pieces in four are diced away. Nobody walks there at all. Behind the Syndicate's barrier
    rows (they end 20.4 m out) the pavement is free from 20.6 m to the podiums' fixtures at 24.5 m, the whole length of
    every block: a pocket piece here may also stand in the middle of a block;
  - the six districts before it: the kerb is full and nearly every pocket is taken (free: The Garages left phase 3 and
    right phase 0, Wall Street right phase 2). The strip under the awnings (23.5 to 24.1 m) carries commuters in every
    second or third slot a side; the slots between carry nobody.
WHAT IS MADE FOR THEM (all LOW: nothing here is over 2 m as built, nothing glows, nothing blinks):
  - pocket pieces, at most 6.0 m long and 2.4 m deep (y from -120 to +120), so that one fits both a pocket of the six
    districts (19.4 to 23.1 m out) and the band behind the Syndicate's barriers (20.6 to 23.1 m). The road is a raised
    ribbon behind a metre of barrier and the pavement lies under it: a person on the pavement shows a head. So nearly
    every piece brings a riser of its own (26 cm: pallets, a stage, a stand of two tiers) and its people are under
    1.66 m, which keeps the piece under 2 m with their arms up: press behind a rail, neighbours painting colour over a
    black hoarding, a little grandstand, buskers, chess tables, a flower stall, a household moving out, the street
    cleaners and their machine;
  - strip pieces, 20 m of duckboard walkway 20 cm high with a hand rail on the road's side (the film unit's detour
    round its cables), and the people who use it: errands both ways, a fun run, a marching band, households leaving
    Syndicate Territory with what they can carry. They walk the piece's X and come round again; never across it.
The general cast of the level: people at work, at rest, passing through. Banners and boards carry bars and squares:
never words, never a chevron (the Syndicate's mark). Gold with black is the Syndicate's: nobody here wears both.

Run in the editor:  import ft_people_city_p3a as K; K.run()        (or K.run(['SM_P3A_Press_A']))
"""
import math
import random
import unreal
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/People/CityCenter'
NAVY, DARK, GREY, STEEL, WHITE, CREAM = '#2b2350', '#22202a', '#3f3d50', '#8f9bb3', '#fff6ea', '#fff1d6'
PINK, CYAN, YEL, MINT, RED, ORANGE, VIOLET, BLUE = '#ff5fa2', '#2ee6ff', '#ffc93c', '#7dffb0', '#ff2d3c', '#ff7a3c', '#8f6bff', '#3a5fa8'
WOOD, PLANK, BLACK, GREEN, LILAC, LIME, SKY = '#9a6a44', '#e8c26a', '#1c1a26', '#3fae5a', '#f4a6ff', '#b8ff5a', '#57c7ff'
HOT = (PINK, CYAN, YEL, MINT, RED, ORANGE, VIOLET, WHITE, LILAC, LIME, SKY)      # coats that read at night
RH = 26.0            # a riser's height
SL = 2000.0          # a strip piece's length (and its walkers' travel)


def cit(r, s_max=0.96, **fixed):
    """A citizen in a coat that reads at night. No tall hair, no tall hat, at most 1.66 m: on a 26 cm riser with both
    arms up the piece stays under 2 m."""
    d = P.look(r, tops=HOT)
    d['hair'] = r.choice(('short', 'short', 'long', 'pony', 'bald'))
    d['hat'] = None
    d['s'] = r.uniform(0.88, s_max)
    d.update(fixed)
    if d.get('build') == 'kid' and 's' not in fixed:
        d['s'] = r.uniform(0.62, 0.7)
    return d


def fig(sc, r, x, y, z=0.0, yaw=0.0, detail=1, who=None, **act):
    """One figure. Nobody stands dead still: with no movement given it breathes."""
    d = dict(who) if who else cit(r)
    d.update(act)
    d.setdefault('phase', r.random())
    if not d.get('rate'):
        d['rate'] = 0.3
        d.setdefault('bob', -1.2)
    return P.figure(sc, x, y, z=z, yaw=yaw, detail=detail, **d)


def riser(m, L=580.0, D=190.0, y=15.0, col=GREY, trim=YEL, h=RH):
    """What a pocket piece's people stand on: y from -80 to +110."""
    m.box(col, (0, y, h / 2.0), (L, D, h))
    m.box(trim, (0, y - D / 2.0 - 1.5, h - 6.0), (L, 3, 9))


def rail(m, x0, x1, y, col, z0=0.0, h=104.0, n=4):
    """A barricade in bars of white and a colour: the people stand behind it."""
    seg = (x1 - x0) / float(n)
    for k in range(n + 1):
        m.box(WHITE, (x0 + k * seg, y, z0 + h / 2.0), (7, 7, h))
    for k in range(n):
        m.box(col if k % 2 else WHITE, (x0 + (k + 0.5) * seg, y, z0 + h - 8.0), (seg, 5, 14))
        m.box(WHITE if k % 2 else col, (x0 + (k + 0.5) * seg, y, z0 + h * 0.52), (seg, 4, 8))


# ------------------------------------------------------------------------------------------ what a hand holds
def hold_mic(col=DARK, head=STEEL):
    def f(m, h, side):
        m.box(col, (h[0], h[1] - 3, h[2] + 8), (5, 5, 20))
        m.box(head, (h[0], h[1] - 3, h[2] + 22), (9, 9, 10))
    return f


def hold_paper(col=WHITE):
    def f(m, h, side):
        m.box(col, (h[0] - side * 14, h[1] - 6, h[2] + 10), (42, 2.5, 30))
    return f


def hold_roller(col):
    """A paint roller on a short handle (22 cm over the hand)."""
    def f(m, h, side):
        m.box(WOOD, (h[0], h[1], h[2] + 8), (4.5, 4.5, 18))
        m.box(col, (h[0], h[1] - 2, h[2] + 18), (26, 9, 9))
    return f


def hold_tub(col):
    def f(m, h, side):
        if side > 0:
            m.cyl(col, (h[0] - 22, h[1] - 4, h[2] - 4), 15, 26, steps=6)
    return f


def hold_stick():
    def f(m, h, side):
        tilt_box(m, WOOD, (h[0], h[1], h[2]), (h[0], h[1] - 22, h[2] - 12), 4.5, 4.5)
    return f


def hold_guitar(body=WOOD, neck=DARK):
    def f(m, h, side):
        m.box(body, (h[0] - side * 4, h[1] - 8, h[2] - 4), (40, 10, 48))
        tilt_box(m, neck, (h[0], h[1] - 8, h[2] + 12), (h[0] - side * 60, h[1] - 8, h[2] + 44), 6, 5)
    return f


def hold_sax(col=YEL):
    def f(m, h, side):
        tilt_box(m, col, (h[0], h[1] - 6, h[2] + 24), (h[0], h[1] - 14, h[2] - 32), 7, 7)
        m.box(col, (h[0], h[1] - 22, h[2] - 30), (14, 14, 12))
    return f


def hold_horn(col=YEL, length=44.0):
    """A trumpet: out ahead of the face."""
    def f(m, h, side):
        tilt_box(m, col, (h[0], h[1] - 4, h[2] + 4), (h[0], h[1] - 4 - length, h[2] + 8), 5, 5)
        m.box(col, (h[0], h[1] - 6 - length, h[2] + 8), (15, 5, 15))
    return f


def hold_slide(col=YEL, length=62.0):
    """A trombone's slide: it goes out and in with the arm."""
    def f(m, h, side):
        tilt_box(m, col, (h[0], h[1] + 20, h[2] + 6), (h[0], h[1] - length, h[2] + 2), 5, 5)
        m.box(col, (h[0], h[1] - length - 2, h[2] + 2), (16, 5, 16))
    return f


def hold_bouquet(cols=(PINK, YEL)):
    def f(m, h, side):
        m.cyl(CREAM, (h[0], h[1] - 2, h[2] + 2), 4, 16, steps=5, top_r=9)
        m.sphere(cols[0], (h[0] - 5, h[1] - 3, h[2] + 24), 10, steps=(3, 5))
        m.sphere(cols[1], (h[0] + 6, h[1] - 1, h[2] + 22), 9, steps=(3, 5))
    return f


def hold_case(col=RED, w=46.0, hh=34.0):
    """A suitcase hanging from the hand."""
    def f(m, h, side):
        m.box(col, (h[0], h[1], h[2] - 8 - hh / 2.0), (15, w, hh))
        m.box(DARK, (h[0], h[1], h[2] - 5), (4, 14, 6))
    return f


def hold_plant(pot=ORANGE, leaf=GREEN):
    def f(m, h, side):
        if side > 0:
            m.cyl(pot, (h[0] - 24, h[1] - 8, h[2] + 2), 12, 18, steps=6, top_r=15)
            m.sphere(leaf, (h[0] - 24, h[1] - 8, h[2] + 36), 19, steps=(3, 6))
    return f


def hold_cage(col=YEL, bird=CYAN):
    def f(m, h, side):
        m.box(col, (h[0], h[1], h[2] - 34), (24, 24, 5))
        m.box(col, (h[0], h[1], h[2] - 8), (24, 24, 4))
        for sx, sy in ((-1, -1), (1, -1), (-1, 1), (1, 1)):
            m.box(col, (h[0] + sx * 10, h[1] + sy * 10, h[2] - 21), (4, 4, 26))
        m.box(bird, (h[0], h[1], h[2] - 24), (9, 12, 10))
    return f


def hold_broom(head=RED):
    """A wide push broom, out ahead of the sweeper on the ground (drawn once, in the right hand)."""
    def f(m, h, side):
        if side > 0:
            tilt_box(m, WOOD, (h[0] - 22, h[1] + 16, h[2] + 20), (h[0] - 22, h[1] - 72, 10), 4.5, 4.5)
            m.box(head, (h[0] - 22, h[1] - 76, 7), (64, 16, 13))
    return f


def hold_picker(col=STEEL, grip=CYAN):
    def f(m, h, side):
        tilt_box(m, col, (h[0], h[1] + 6, h[2] + 4), (h[0], h[1] - 52, h[2] - 60), 4.5, 4.5)
        m.box(grip, (h[0], h[1] + 4, h[2] + 4), (6, 12, 6))
    return f


def hold_baton(col=WHITE, knob=PINK):
    def f(m, h, side):
        m.box(col, (h[0], h[1], h[2] + 14), (4.5, 4.5, 26))
        m.box(knob, (h[0], h[1], h[2] + 29), (9, 9, 9))
    return f


def hold_snare(col=WHITE, rim=PINK):
    """A side drum at the belt (drawn in the left arm, which hardly swings)."""
    def f(m, h, side):
        if side < 0:
            m.box(col, (0, -34, 84), (36, 30, 16))
            m.box(rim, (0, -34, 93), (38, 32, 3))
    return f


def hold_bass(col=WHITE, rim=CYAN):
    """A bass drum on the chest, its skins to either side."""
    def f(m, h, side):
        if side < 0:
            rod(m, col, (-15, -42, 98), (15, -42, 98), 33, steps=10)
            rod(m, rim, (-18, -42, 98), (-15, -42, 98), 35, steps=10)
            rod(m, rim, (15, -42, 98), (18, -42, 98), 35, steps=10)
    return f


def hold_cymbal(col=YEL):
    def f(m, h, side):
        m.box(col, (h[0] - side * 5, h[1] - 4, h[2] + 4), (3, 28, 28))
    return f


CHEER = (
    dict(arm_l='up', arm_r='up', swing_l=14, swing_r=-14, bob=6, rate=1.6),
    dict(arm_l='hold', arm_r='hold', swing_l=13, swing_r=-13, axis_l='z', axis_r='z', rate=2.4),          # clapping
    dict(arm_r='point', arm_l='hip', swing_r=10, axis_r='x', rate=0.7),
    dict(arm_r='wave', swing_r=26, rate=1.3, bob=3),
    dict(arm_l='vee', arm_r='vee', swing_l=10, swing_r=-10, bob=7, rate=1.4),
    dict(arm_l='wave', arm_r='wave', swing_l=-22, swing_r=22, rate=1.1, bob=5),
    dict(arm_r='up', arm_l='down', swing_r=18, rate=1.8, bob=5),                                           # a fist pumping
)


# ------------------------------------------------------------------------------------------ pocket pieces (6.0 m by 2.4 m, low)
def press(seed, vest=CYAN, kind=0):
    """LOW. The press behind a rail, on a riser: a television camera panning on a low tripod and its operator, a
    reporter with a microphone, photographers (two holding their cameras over the others' heads), one on the phone.
    kind 1: the reporter is interviewing someone. Press wear a band in `vest` and never the crew's pink."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        riser(g, col=NAVY, trim=vest)
        rail(g, -290.0, 290.0, -112.0, vest)
        tx, ty = -205.0, -38.0
        for a in (90, 210, 330):
            tilt_box(g, STEEL, (tx, ty, RH + 92), (tx + math.cos(math.radians(a)) * 40, ty + math.sin(math.radians(a)) * 40, RH), 4.5, 4.5)
        pan = dict(swing=18.0, pivot=(tx, ty, 0.0), axis=P.UP)
        cam = sc.part(P.motion(rate=0.14, phase=0.3, **pan))
        cam.box(DARK, (tx, ty, RH + 109), (30, 62, 34))
        cam.box(GREY, (tx, ty + 6, RH + 136), (12, 44, 20))
        cam.box(STEEL, (tx, ty - 42, RH + 109), (18, 22, 18))
        cam.box(vest, (tx + 16.5, ty + 4, RH + 112), (3, 36, 12))
        fig(sc, r, tx, ty + 74, z=RH, who=cit(r, band=vest, hat='cap', hat_col=WHITE), arm_l='push', arm_r='push', rate=0.14, phase=0.3, sway=pan)
        if kind == 0:
            fig(sc, r, -85, -44, z=RH, yaw=-20, who=cit(r, top=PINK, wear='coat', sleeves='long'), arm_r='face', hold_r=hold_mic(),
                arm_l='point', swing_l=12, axis_l='x', rate=0.8, react=True)
        else:
            fig(sc, r, -112, -40, z=RH, yaw=75, who=cit(r, top=YEL, wear='coat', sleeves='long'), arm_r='fwd', hold_r=hold_mic(),
                arm_l='hip', rock=2, rate=0.4)
            fig(sc, r, -40, -40, z=RH, yaw=-75, detail=0, who=cit(r), arm_r='point', swing_r=14, axis_r='x', arm_l='out', swing_l=10, rate=0.9)
        for k, x in enumerate((50, 140, 232)):
            fig(sc, r, x, -40 + r.uniform(-6, 6), z=RH, yaw=r.uniform(-15, 15), detail=1 if k < 2 else 0, who=cit(r, band=vest),
                arm_l='hold', arm_r='hold', hold_r=P.hold_camera(col=DARK, lens=STEEL), turn=24, rate=0.3 + 0.07 * k)
        high = ((110, 30), (150, 20))
        for k, x in enumerate((-95, 10, 115, 215)):
            if k in (0, 2):
                act = dict(arm_l=high, arm_r=high, hold_r=P.hold_camera(col=DARK, lens=STEEL), bob=4, rate=1.1, react=True)
            elif k == 1:
                act = dict(arm_r='face', hold_r=P.hold_phone(), arm_l='hip', turn=14, rate=0.3)
            else:
                act = dict(arm_r='wave', swing_r=24, bob=4, rate=1.3, react=True)
            fig(sc, r, x, 62, z=RH, yaw=r.uniform(-12, 12), detail=0, who=cit(r, band=vest), **act)
    return build


def mural(seed, kind=0):
    """LOW. Neighbours painting colour over a black hoarding (bars and squares: never words, never a chevron): rollers
    going up and down, one standing back to look, a child with a tub, one waving a roller at the chase."""
    def build(sc):
        r = random.Random(seed)
        cols = list(HOT)
        r.shuffle(cols)
        g = sc.still()
        riser(g, col=GREY, trim=PINK)
        by = 102.0
        g.box(BLACK, (0, by, RH + 86), (570, 8, 164))                       # the hoarding: 30 cm to 1.94 m
        for sx in (-1, 1):
            g.box(WOOD, (sx * 270, by + 9, RH + 60), (8, 8, 120))
        f = by - 5.2
        if kind == 0:
            for k in range(5):
                g.box(cols[k], (-248 + k * 72, f, RH + 86), (64, 2.4, 150))
            g.box(cols[5], (150, f, RH + 58), (118, 2.4, 92))
        else:
            for k in range(4):
                g.box(cols[k], (-84, f, RH + 26 + k * 39), (384, 2.4, 33))
            g.box(cols[4], (186, f, RH + 112), (92, 2.4, 92))
        g.box(CREAM, (0, 10, RH + 1.2), (430, 150, 2.4))                    # the drop cloth
        for k in range(5):
            g.cyl(cols[k], (-200 + k * 100, -62, RH), 15, 28, steps=6)
        reach, low = ((120, 80), (140, 85)), ((70, 80), (95, 88))
        fig(sc, r, -230, 46, z=RH, yaw=180, who=cit(r, s_max=0.93, hat='cap', hat_col=WHITE), arm_r=reach, swing_r=14, axis_r='x',
            hold_r=hold_roller(cols[0]), arm_l='hip', rate=0.7)
        fig(sc, r, -120, 50, z=RH, yaw=180, detail=0, who=cit(r, s_max=0.93), arm_r=low, swing_r=16, axis_r='y', hold_r=hold_roller(cols[1]), rate=0.9)
        fig(sc, r, -10, 44, z=RH, yaw=180, who=cit(r, s_max=0.93), arm_l=reach, swing_l=12, axis_l='x', hold_l=hold_roller(cols[2]), rate=0.6)
        fig(sc, r, 110, 22, z=RH, yaw=200, detail=0, who=cit(r), arm_l='hip', arm_r='hip', rock=4, rate=0.3)
        fig(sc, r, 205, -34, z=RH, yaw=10, who=cit(r, s_max=0.92), arm_r='wave', swing_r=24, hold_r=hold_roller(cols[3]), bob=0, rate=1.3, react=True)
        fig(sc, r, 262, 40, z=RH, yaw=-20, detail=0, who=cit(r, build='kid'), arm_l='carry', arm_r='carry', hold_r=hold_tub(cols[4]), bob=3, rate=0.8)
        fig(sc, r, -262, -34, z=RH, yaw=-10, detail=0, who=cit(r), arm_l='vee', arm_r='vee', swing_l=10, swing_r=-10, bob=6, rate=1.4, react=True)
    return build


def bleacher(seed, cols=(PINK, CYAN, YEL)):
    """LOW. A little grandstand of painted pallets, two tiers: eleven neighbours sitting on it to watch the last scene,
    cheering, clapping, pointing, out of step. The back row's heads are 1.9 m up."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        g.box(GREY, (0, 0, RH / 2.0), (580, 220, RH))
        g.box(cols[0], (0, -111.5, RH - 6), (580, 3, 10))
        g.box(cols[1], (0, 45, RH + 17.5), (580, 130, 35))                  # the front bench: 61 cm
        g.box(cols[2], (0, 80, RH + 52.5), (580, 60, 35))                   # the back bench: 96 cm
        g.box(WHITE, (0, -21.5, RH + 29), (580, 3, 8))
        g.box(WHITE, (0, 48.5, RH + 64), (580, 3, 8))
        seated = (
            dict(arm_l='up', arm_r='up', swing_l=14, swing_r=-14, bob=4, rate=1.6),
            dict(arm_l='hold', arm_r='hold', swing_l=13, swing_r=-13, axis_l='z', axis_r='z', rate=2.4),
            dict(arm_r='point', arm_l='down', swing_r=10, axis_r='x', rate=0.7),
            dict(arm_r='wave', swing_r=26, rate=1.3),
            dict(arm_l='vee', arm_r='vee', swing_l=10, swing_r=-10, bob=5, rate=1.4),
            dict(arm_r='up', arm_l='hold', hold_l=P.hold_cup(), swing_r=18, rate=1.8),
        )
        for i in range(6):
            d = cit(r, build='kid') if i == 4 else cit(r)
            d.update(seated[(i * 5 + seed) % 6])
            fig(sc, r, -240 + i * 96 + r.uniform(-6, 6), -5, z=RH, yaw=r.uniform(-10, 10), detail=1 if i in (1, 3) else 0, who=d,
                legs='sit', seat=35, react=True)
        for i in range(5):
            d = cit(r, s_max=0.93)
            d.update(seated[(i * 3 + seed + 1) % 6])
            fig(sc, r, -192 + i * 96 + r.uniform(-6, 6), 62, z=RH + 35, yaw=r.uniform(-10, 10), detail=0, who=d, legs='sit', seat=35, react=True)
    return build


def buskers(seed, kind=0):
    """LOW. A street band on a stage of pallets and a striped rug, an open case for coins on the pavement.
    kind 0: a guitar, a bucket drummer, a saxophone, a singer; kind 1: two trumpets, a trombone, a bass drum, a
    leader. Three listeners: one dancing, one clapping, a child hopping."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        riser(g, col=WOOD, trim=RED)
        for k in range(6):
            g.box(RED if k % 2 else CREAM, (-250 + k * 100, 15, RH + 1.2), (100, 176, 2.4))
        g.box(DARK, (0, -100, 6), (120, 36, 12))
        g.box(YEL, (0, -100, 13.5), (90, 20, 3))
        if kind == 0:
            g.box(BLUE, (0, 62, RH + 22), (46, 40, 44))
            g.cyl(WHITE, (-30, -14, RH), 20, 46, steps=7)
            g.cyl(CYAN, (30, -14, RH), 20, 46, steps=7)
            fig(sc, r, 0, 62, z=RH, legs='sit', seat=44, who=cit(r, hat='cap', hat_col=RED), arm_l='fwd', arm_r='fwd', swing_l=16, swing_r=-16,
                axis_l='x', axis_r='x', hold_l=hold_stick(), hold_r=hold_stick(), rate=2.4)
            fig(sc, r, -125, 30, z=RH, yaw=10, who=cit(r, top=VIOLET), arm_r='hold', swing_r=10, axis_r='x', hold_r=hold_guitar(),
                arm_l=((50, 40), (80, 70)), rate=2.2)
            fig(sc, r, 120, 30, z=RH, yaw=-10, who=cit(r, top=MINT), arm_l='hold', arm_r='hold', hold_r=hold_sax(), rock=6, rate=0.8)
            fig(sc, r, -228, -30, z=RH, yaw=15, detail=0, who=cit(r, top=PINK, wear='dress'), arm_r='face', hold_r=hold_mic(), arm_l='out',
                swing_l=16, rate=0.8, bob=2)
        else:
            uni = dict(top=RED, bottom=WHITE, sleeves='long', wear='trousers', hat='cap', hat_col=WHITE)
            fig(sc, r, -135, 30, z=RH, yaw=8, who=cit(r, **uni), arm_r='face', arm_l='hip', hold_r=hold_horn(), bob=3, rate=1.0)
            fig(sc, r, -45, 50, z=RH, yaw=-4, detail=0, who=cit(r, **uni), arm_r='face', arm_l='face', hold_r=hold_horn(), rock=4, rate=0.9)
            fig(sc, r, 50, 28, z=RH, yaw=-8, who=cit(r, **uni), arm_l='face', arm_r='fwd', swing_r=16, axis_r='x', hold_r=hold_slide(), rate=0.9)
            fig(sc, r, 140, 52, z=RH, detail=0, who=cit(r, **uni), arm_l=((40, 60), (80, 80)), arm_r=((40, 60), (80, 80)), swing_r=20, axis_r='x',
                hold_l=hold_bass(), rate=1.8)
            fig(sc, r, -228, -30, z=RH, yaw=15, detail=0, who=cit(r, **uni), arm_r=((100, 20), (140, 10)), swing_r=16, axis_r='x', hold_r=hold_baton(),
                arm_l='hip', rate=1.8)
        fig(sc, r, 212, -36, z=RH, yaw=-12, who=cit(r), arm_l='out', arm_r='up', turn=24, rate=0.8)
        fig(sc, r, 262, 46, z=RH, yaw=-25, detail=0, who=cit(r), arm_l='hold', arm_r='hold', swing_l=13, swing_r=-13, axis_l='z', axis_r='z', rate=2.4, react=True)
        fig(sc, r, 196, 64, z=RH, yaw=-15, detail=0, who=cit(r, build='kid'), arm_l='vee', arm_r='vee', swing_l=10, swing_r=-10, bob=8, rate=1.5)
    return build


def chess(seed):
    """LOW. Two chess tables on a riser: four players on crates (one moving a piece, one with a hand at the chin),
    three watching over their shoulders (one has seen the chase and points), someone behind a newspaper."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        riser(g, col=BLUE, trim=WHITE)
        for tx in (-170.0, 90.0):
            g.cyl(STEEL, (tx, -20, RH), 5, 66, steps=5)
            g.box(CREAM, (tx, -20, RH + 68), (84, 70, 5))
            g.box(WHITE, (tx, -20, RH + 71.5), (44, 44, 2))
            for a, b in ((-11, -11), (11, 11), (-11, 11), (11, -11)):
                g.box(DARK, (tx + a, -20 + b, RH + 73), (11, 11, 1.5))
            g.box(RED, (tx, 8, RH + 76), (16, 8, 10))
            for sx in (-1, 1):
                g.box(WOOD, (tx + sx * 68, -20, RH + 22), (40, 40, 44))
        g.box(WOOD, (245, -45, RH + 22), (40, 40, 44))
        for k, tx in enumerate((-170.0, 90.0)):
            fig(sc, r, tx - 68, -20, z=RH, yaw=90, detail=1 if k == 0 else 0, who=cit(r), legs='sit', seat=44, arm_r='fwd', swing_r=9, axis_r='x',
                arm_l='hip', rate=0.3)
            fig(sc, r, tx + 68, -20, z=RH, yaw=-90, detail=0 if k == 0 else 1, who=cit(r, hat='cap', hat_col=RED), legs='sit', seat=44, arm_r='face',
                arm_l='down', turn=8, rate=0.25)
        fig(sc, r, -170, 64, z=RH, yaw=5, detail=0, who=cit(r), arm_r='face', arm_l='hip', rock=3, rate=0.3)
        fig(sc, r, -30, 60, z=RH, yaw=-10, detail=0, who=cit(r), arm_r='point', arm_l='hip', swing_r=12, axis_r='x', rate=0.8, react=True)
        fig(sc, r, 95, 66, z=RH, yaw=8, detail=0, who=cit(r), arm_l='hold', arm_r='hold', turn=16, rate=0.3)
        fig(sc, r, 245, -45, z=RH, yaw=-8, who=cit(r, wear='coat', sleeves='long'), legs='sit', seat=44, arm_l='hold', arm_r='hold', hold_r=hold_paper(),
            rock=2, rate=0.25)
    return build


def flowers(seed):
    """LOW (the canopy's top is 1.92 m). A flower stall: three tiers of buckets under a low striped canopy, tubs of
    flowers along the front, the seller wrapping a bunch, customers choosing and carrying, one waving hers at the chase."""
    def build(sc):
        r = random.Random(seed)
        cols = list(HOT)
        r.shuffle(cols)
        g = sc.still()
        sx0 = -150.0
        for k in range(3):
            top = 40.0 + k * 40.0
            g.box(WOOD, (sx0, 10 + k * 36, top / 2.0), (270, 36, top))
            for j in range(2):
                bx = sx0 - 62 + j * 124 + (k % 2) * 30 - 15
                g.cyl(STEEL, (bx, 10 + k * 36, top), 13, 20, steps=6)
                g.sphere(cols[(k * 2 + j) % len(cols)], (bx, 10 + k * 36, top + 36), 24, scale=(1.4, 0.8, 0.8), steps=(3, 6))
        for px in (sx0 - 140, sx0 + 140):
            for py in (-12, 100):
                g.box(STEEL, (px, py, 93), (5, 5, 186))
        for k in range(6):
            g.box(MINT if k % 2 else WHITE, (sx0 - 117.5 + k * 47, 44, 189), (47, 124, 6))
        for k in range(3):
            g.cyl(DARK, (-262 + k * 84, -96, 0), 20, 28, steps=6)
            g.sphere(cols[(k + 6) % len(cols)], (-262 + k * 84, -96, 46), 26, scale=(1.1, 0.8, 0.8), steps=(3, 6))
        fig(sc, r, 24, 52, yaw=-30, who=cit(r, s_max=1.0, top=MINT, band=WHITE), arm_l='hold', arm_r='hold', hold_r=hold_bouquet((cols[0], cols[1])),
            rock=3, rate=0.4)
        fig(sc, r, 100, -20, yaw=70, who=cit(r, s_max=1.0), arm_r='fwd', swing_r=8, axis_r='x', arm_l='hip', rate=0.5)
        fig(sc, r, 165, 44, yaw=55, detail=0, who=cit(r, s_max=1.0), arm_r='point', swing_r=10, axis_r='x', rate=0.6)
        fig(sc, r, 228, -34, yaw=0, who=cit(r, s_max=0.95), arm_r='wave', swing_r=24, hold_r=hold_bouquet((cols[2], cols[3])), bob=4, rate=1.3, react=True)
        fig(sc, r, 266, 46, yaw=-15, detail=0, who=cit(r, build='kid'), arm_r='up', swing_r=16, hold_r=hold_bouquet((cols[4], cols[5])), bob=5, rate=1.4)
        fig(sc, r, -20, -74, yaw=10, detail=0, who=cit(r, s_max=1.0, top=MINT, band=WHITE), arm_l='carry', arm_r='carry', hold_r=hold_tub(STEEL), bob=-1.5, rate=0.5)
    return build


def porters(seed):
    """LOW. A household moving out: a hand cart loaded with a sofa, a rolled rug, a standing lamp (unlit) and the cat;
    two porters carrying an armchair between them, one lifting a box on, the owner with the bird cage pointing the
    way, a child on a suitcase waving."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        cx, cy = -110.0, 34.0
        g.box(WOOD, (cx, cy, 44), (300, 110, 8))
        for sx in (-1, 1):
            for sy in (-1, 1):
                rod(g, DARK, (cx + sx * 110, cy + sy * 48, 20), (cx + sx * 110, cy + sy * 57, 20), 20, steps=8)
        g.box(PINK, (cx - 30, cy, 68), (200, 76, 40))
        g.box(PINK, (cx - 30, cy + 32, 112), (200, 14, 48))
        for sx in (-1, 1):
            g.box(LILAC, (cx - 30 + sx * 100, cy, 96), (16, 76, 24))
        rod(g, VIOLET, (cx - 122, cy - 18, 101), (cx - 10, cy - 18, 101), 13, steps=6)
        g.box(STEEL, (cx + 118, cy + 20, 108), (5, 5, 120))
        g.cyl(CREAM, (cx + 118, cy + 20, 160), 24, 30, steps=6, top_r=13)
        g.box(ORANGE, (78, 84, 30), (52, 34, 60))
        g.box(CYAN, (78, 84, 72), (46, 30, 24))
        g.box(YEL, (196, -72, 13), (60, 36, 26))
        porter = dict(top=BLUE, bottom=NAVY, sleeves='long', wear='trousers', hat='cap', hat_col=BLUE, band=WHITE)
        ph = r.random()
        fig(sc, r, 118, 14, yaw=90, who=cit(r, s_max=1.0, **porter), arm_l='carry', arm_r='carry', bob=-2.0, rate=0.5, phase=ph)
        fig(sc, r, 262, 14, yaw=-90, detail=0, who=cit(r, s_max=1.0, **porter), arm_l='carry', arm_r='carry', bob=-2.0, rate=0.5, phase=ph)
        fig(sc, r, cx + 10, cy - 96, yaw=180, detail=0, who=cit(r, s_max=1.0, **porter), arm_l='carry', arm_r='carry', hold_r=P.hold_box(col=WOOD),
            bob=-2.0, rate=0.6)
        fig(sc, r, 66, -70, yaw=0, who=cit(r, s_max=1.0, wear='coat', sleeves='long'), arm_r='point', swing_r=12, axis_r='x', hold_l=hold_cage(),
            rate=0.8, react=True)
        fig(sc, r, 196, -66, yaw=0, detail=0, who=cit(r, build='kid'), legs='sit', seat=40, arm_r='wave', swing_r=26, rate=1.3, react=True)
        ch = sc.part(P.motion(bob=-2.0, rate=0.5, phase=ph))
        ch.box(MINT, (190, 14, 95), (70, 64, 40))
        ch.box(MINT, (190, 44, 132), (70, 12, 40))
        P.quadruped(sc, cx + 50, cy - 4, z=88, yaw=-70, length=34, height=22, girth=12, neck=6, neck_up=40, head=12, tail=20, wag=25, rate=1.2,
                    body=WHITE, ears=WHITE, tail_col=ORANGE)
    return build


def sweepers(seed):
    """LOW (the machine's roof is 1.92 m). The night's street cleaners in orange: a ride-on sweeper sliding to and
    fro with its driver, two with wide push brooms, one picking litter into a bag, one who has stopped to wave; their
    bin cart and three cones."""
    def build(sc):
        r = random.Random(seed)

        def crew(**k):
            return cit(r, s_max=1.0, top=ORANGE, bottom=NAVY, sleeves='long', wear='trousers', band=WHITE, hat='cap', hat_col=ORANGE, **k)

        g = sc.still()
        bx, by = 250.0, 62.0
        g.cyl(MINT, (bx, by, 18), 30, 92, steps=8)
        g.cyl(DARK, (bx, by, 110), 32, 6, steps=8)
        for sy in (-1, 1):
            rod(g, DARK, (bx, by + sy * 28, 18), (bx, by + sy * 35, 18), 18, steps=8)
        tilt_box(g, STEEL, (bx - 28, by, 100), (bx - 58, by, 118), 5, 5)
        for k in range(3):
            g.cyl(ORANGE, (60 + k * 45, 102, 0), 16, 52, steps=6, top_r=4)
        mx, my = -125.0, 34.0
        slide = dict(swing=1.8, pivot=(mx, my, -2600.0), axis=90.0)
        mc = sc.part(P.motion(rate=0.18, phase=0.1, **slide))
        mc.box(YEL, (mx, my, 50), (170, 100, 60))
        mc.box(ORANGE, (mx + 55, my, 95), (60, 100, 30))
        mc.box(DARK, (mx - 40, my, 84), (44, 60, 8))
        for sx in (-1, 1):
            rod(mc, DARK, (mx + sx * 55, my - 53, 22), (mx + sx * 55, my + 53, 22), 22, steps=8)
        rod(mc, VIOLET, (mx + 108, my - 50, 24), (mx + 108, my + 50, 24), 24, steps=8)
        for px, py in ((mx - 80, my - 46), (mx - 80, my + 46), (mx + 22, my - 46), (mx + 22, my + 46)):
            mc.box(DARK, (px, py, 133), (5, 5, 106))
        mc.box(WHITE, (mx - 29, my, 189), (114, 104, 6))
        fig(sc, r, mx - 40, my, z=52, yaw=90, who=crew(s=0.88), legs='sit', seat=40, arm_l='fwd', arm_r='fwd', rate=0.18, phase=0.1, sway=slide)
        fig(sc, r, -60, -86, yaw=90, who=crew(), arm_l='push', arm_r='push', swing_l=10, swing_r=10, axis_l='x', axis_r='x', hold_r=hold_broom(RED), rate=0.9)
        fig(sc, r, 118, -80, yaw=90, detail=0, who=crew(), arm_l='push', arm_r='push', swing_l=10, swing_r=10, axis_l='x', axis_r='x',
            hold_r=hold_broom(CYAN), rate=0.8)
        fig(sc, r, 150, 30, yaw=0, detail=0, who=crew(), arm_r='fwd', swing_r=18, axis_r='x', hold_r=hold_picker(), hold_l=P.hold_bag(LIME), rate=0.6)
        fig(sc, r, 268, -22, yaw=0, who=crew(), arm_r='wave', swing_r=26, arm_l='hip', bob=3, rate=1.3, react=True)
    return build


# ------------------------------------------------------------------------------------------ strip pieces (20 m by 1 m, low)
def walkway(m, L=SL, col=PLANK, trim=ORANGE):
    """20 m of duckboards 20 cm high, a ramp at each end, a hand rail on the road's side (y = -50)."""
    m.box(col, (0, 0, 10), (L, 100, 20))
    for sx in (-1, 1):
        tilt_box(m, col, (sx * L / 2.0, 0, 18), (sx * (L / 2.0 + 70), 0, 1), 100, 4)
    for k in range(5):
        m.box(trim, (-L / 2.0 + 3 + k * (L - 6) / 4.0, -50, 68), (6, 6, 96))
    m.box(WHITE, (0, -50, 112), (L, 5, 8))
    m.box(trim, (0, -50, 72), (L, 5, 6))


def walker(sc, r, x, lane, v, detail=1, who=None, **act):
    """Someone walking the walkway at v cm/s (negative: towards -X), coming round again."""
    way = 1 if v > 0 else -1
    d = dict(stride=30 if abs(v) > 250 else 24, swing_l=18, swing_r=-18, axis_l='x', axis_r='x', rate=abs(v) / 150.0, bob=3, phase=r.random(),
             travel=(SL, float(v)))
    d.update(who if who else cit(r, s_max=0.98))
    d.update(act)
    P.figure(sc, x, lane, z=20.0, yaw=90 * way, detail=detail, **d)


def tprop(sc, x, y, v, z=20.0):
    """The model of a thing that travels with a walker (a pram, a hand truck): draw it round (x, y, z)."""
    return sc.part(P.motion(pivot=(x, y, z), base=(x, y), travel=(SL, float(v) / SL)))


def strip_errands(seed):
    """LOW. The walkway on an ordinary night: someone with the dog, a courier and his hand truck of parcels, a pram, a
    pair of runners, a couple, someone late with a case. Both ways."""
    def build(sc):
        r = random.Random(seed)
        walkway(sc.still())
        walker(sc, r, -720, 18, 120, who=cit(r, s_max=0.98, wear='coat', sleeves='long'))
        P.quadruped(sc, -610, 16, z=20, yaw=90, length=92, height=66, girth=26, neck=20, neck_up=38, head=24, tail=28, wag=30, stride=22, rate=2.0,
                    travel=(SL, 120), body=PLANK, ears=WOOD, snout=WOOD)
        walker(sc, r, 420, -18, -140, who=cit(r, s_max=0.98, top=LIME, hat='cap', hat_col=LIME, band=DARK), arm_l='push', arm_r='push', swing_l=0, swing_r=0)
        t = tprop(sc, 350, -18, -140)
        t.box(STEEL, (362, -18, 80), (6, 44, 120))
        t.box(STEEL, (340, -18, 26), (44, 44, 5))
        rod(t, DARK, (356, -42, 32), (356, 6, 32), 12, steps=6)
        for k, c in enumerate((ORANGE, CYAN, PINK)):
            t.box(c, (338, -18, 46 + k * 34), (40, 40 - k * 4, 32))
        walker(sc, r, 160, 18, 105, who=cit(r, s_max=0.98, wear='dress'), arm_l='push', arm_r='push', swing_l=0, swing_r=0)
        t = tprop(sc, 226, 18, 105)
        t.box(MINT, (226, 18, 76), (62, 40, 30))
        t.box(WHITE, (210, 18, 100), (30, 40, 22))
        rod(t, DARK, (206, -4, 34), (206, 40, 34), 14, steps=6)
        rod(t, DARK, (246, -4, 34), (246, 40, 34), 14, steps=6)
        for k in range(2):
            walker(sc, r, -320 + k * 70, -20 + k * 10, -335 - k * 12, detail=0, who=cit(r, s_max=0.98, wear='shorts', sleeves='short', band=WHITE),
                   swing_l=26, swing_r=-26, bob=5)
        walker(sc, r, -160, 8, 112, detail=0)
        walker(sc, r, -110, 28, 112, detail=0, who=cit(r, s_max=0.98, wear='skirt'))
        walker(sc, r, 800, 14, 175, detail=0, who=cit(r, s_max=0.98, wear='coat', sleeves='long'), hold_r=hold_case(RED), swing_r=0)
    return build


def strip_run(seed):
    """LOW. A night fun run along the walkway: nine runners in bright vests with a white band for a number, each at
    their own pace, all one way."""
    def build(sc):
        r = random.Random(seed)
        walkway(sc.still(), trim=PINK)
        for i in range(9):
            v = r.uniform(300, 370)
            walker(sc, r, -900 + i * 215 + r.uniform(-40, 40), r.choice((-20, 0, 20)), v, detail=1 if i % 3 == 0 else 0,
                   who=cit(r, s_max=0.98, top=HOT[(i * 4 + seed) % len(HOT)], wear='shorts', sleeves='short', band=WHITE), swing_l=26, swing_r=-26, bob=5)
    return build


def strip_leavers(seed):
    """LOW. Households leaving Syndicate Territory with what they can carry, all one way: suitcases, a box, a pot
    plant, the bird in its cage, a hand cart with a chair and a lamp on it."""
    def build(sc):
        r = random.Random(seed)
        walkway(sc.still(), trim=CYAN)
        carry = (
            dict(hold_r=hold_case(RED), swing_r=0), dict(arm_l='carry', arm_r='carry', hold_r=P.hold_box(col=WOOD), swing_l=0, swing_r=0),
            dict(hold_l=hold_case(CYAN), hold_r=hold_case(ORANGE, w=38.0, hh=28.0), swing_l=0, swing_r=0),
            dict(arm_l='carry', arm_r='carry', hold_r=hold_plant(), swing_l=0, swing_r=0), dict(hold_r=hold_cage(), swing_r=0),
            dict(hold_l=hold_case(VIOLET), swing_l=0), dict(arm_l='carry', arm_r='carry', hold_r=P.hold_box(col=PINK, w=40.0), swing_l=0, swing_r=0),
        )
        for i in range(7):
            v = r.uniform(96, 124)
            d = cit(r, build='kid') if i == 5 else cit(r, s_max=0.98, wear=r.choice(('coat', 'coat', 'trousers', 'dress')), sleeves='long')
            d.update(carry[i])
            walker(sc, r, -860 + i * 250 + r.uniform(-40, 40), r.choice((-18, 0, 18)), v, detail=1 if i % 3 == 0 else 0, who=d,
                   pack=r.choice((None, PINK, YEL)))
        walker(sc, r, 820, 0, 100, who=cit(r, s_max=0.98, wear='coat', sleeves='long'), arm_l='back', arm_r='back', swing_l=0, swing_r=0)
        t = tprop(sc, 720, 0, 100)
        t.box(WOOD, (715, 0, 52), (130, 70, 8))
        rod(t, DARK, (715, -40, 38), (715, 40, 38), 18, steps=8)
        tilt_box(t, WOOD, (780, 0, 54), (806, 0, 100), 5, 5)
        t.box(LILAC, (690, 0, 80), (56, 60, 48))
        t.box(LILAC, (668, 0, 122), (12, 60, 40))
        t.box(STEEL, (752, 10, 106), (5, 5, 100))
        t.cyl(CREAM, (752, 10, 150), 20, 26, steps=6, top_r=11)
    return build


def strip_band(seed):
    """LOW. A marching band along the walkway in cyan jackets, white trousers and pink caps, in step: the leader's
    baton, two trumpets, a trombone, two side drums, the bass drum, the cymbals."""
    def build(sc):
        r = random.Random(seed)
        walkway(sc.still(), trim=CYAN)
        v = 105.0
        acts = (
            dict(arm_l='hold', arm_r='hold', swing_l=14, swing_r=-14, axis_l='z', axis_r='z', hold_l=hold_cymbal(), hold_r=hold_cymbal()),
            dict(arm_l=((40, 60), (80, 80)), arm_r=((40, 60), (80, 80)), swing_l=0, swing_r=20, hold_l=hold_bass()),
            dict(arm_l='fwd', arm_r='fwd', swing_l=4, swing_r=-14, hold_l=hold_snare(), hold_r=hold_stick()),
            dict(arm_l='fwd', arm_r='fwd', swing_l=4, swing_r=14, hold_l=hold_snare(), hold_r=hold_stick()),
            dict(arm_l='face', arm_r='fwd', swing_l=0, swing_r=16, hold_r=hold_slide()),
            dict(arm_l='face', arm_r='face', swing_l=0, swing_r=0, hold_r=hold_horn()),
            dict(arm_l='hip', arm_r='face', swing_l=0, swing_r=0, hold_r=hold_horn()),
            dict(arm_l='hip', arm_r=((100, 20), (140, 10)), swing_l=0, swing_r=16, hold_r=hold_baton()),
        )
        for i in range(8):
            d = cit(r, top=CYAN, bottom=WHITE, sleeves='long', wear='trousers', hat='peak', hat_col=PINK, band=WHITE, shoes=DARK)
            d.update(acts[i])
            walker(sc, r, -430 + i * 122, 16 if i % 2 else -16, v, detail=1 if i % 2 else 0, who=d, phase=0.5 * (i % 2), rate=0.9, stride=22)
    return build


JOBS = [
    ('SM_P3A_Press_A', press(31, CYAN, 0)),
    ('SM_P3A_Press_B', press(32, LIME, 1)),
    ('SM_P3A_Mural_A', mural(33, 0)),
    ('SM_P3A_Mural_B', mural(34, 1)),
    ('SM_P3A_Bleacher_A', bleacher(35)),
    ('SM_P3A_Bleacher_B', bleacher(36, cols=(YEL, VIOLET, MINT))),
    ('SM_P3A_Buskers_A', buskers(37, 0)),
    ('SM_P3A_Buskers_B', buskers(38, 1)),
    ('SM_P3A_Chess_A', chess(39)),
    ('SM_P3A_Flowers_A', flowers(40)),
    ('SM_P3A_Porters_A', porters(41)),
    ('SM_P3A_Sweepers_A', sweepers(42)),
    ('SM_P3A_Strip_A', strip_errands(43)),
    ('SM_P3A_Strip_B', strip_run(44)),
    ('SM_P3A_Strip_C', strip_leavers(45)),
    ('SM_P3A_Strip_D', strip_band(46)),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths(only=None):
    return [DIR + '/' + j[0] for j in JOBS if not only or j[0] in only]
