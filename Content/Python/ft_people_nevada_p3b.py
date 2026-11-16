"""FULL THROTTLE (Unreal) the population of Nevada, pass 3, builder p3b: SCENES. /Game/People/Nevada/SM_P3B_*.

Ten story groups that could only stand beside Highway 93: one mesh a scene, 6 to 10 figures each doing something of
their own, with the props that tell the story. The land lies a metre under this road behind a metre of barrier, so
every scene brings its own ground two metres up (a bank, a stage, a deck, a ring): all of them are TALL (24 m or more
from the road; the table stands them 34 to 39 m out, in the gaps between the first cast's decks).

    SM_P3B_Rodeo        a rodeo pen: a bronc bucking under its rider, the clown popping out of his barrel, a roper
                        spinning a loop, a calf trotting the fence, hands on the back rail with their hats in the air
    SM_P3B_SkyWatch     the sky watchers' camp: foil hats, a dish turning, a telescope, a landing ring of painted
                        stones, a child in an alien suit hopping, a lilac alien balloon on a string
    SM_P3B_SaltRecord   the record crew on the salt: a mint streamliner on its stands, mechanics, the driver in
                        helmet and white suit, the flagman, the timekeeper at his table
    SM_P3B_Revue        a casino's outdoor stage: a kick line of five with feather fans, the bandleader, a drummer,
                        a trumpet
    SM_P3B_HoeDown      a barn dance: two squares of four going round (orbits), the fiddler on a bale, the caller
    SM_P3B_Dig          a fossil dig on a rock bench: the great ribs and skull, a pick, a brush, a bone held aloft,
                        a barrow going to and fro, the professor pointing
    SM_P3B_PoolParty    a motel's pool deck: swimmers bobbing, a beach ball going up between them, one bouncing on
                        the board, the lifeguard on her high chair, a flamingo float
    SM_P3B_CookOff      the chili cook-off: three steaming pots, cooks stirring and tasting, judges with score cards,
                        one who took too big a spoonful, the dog under the table
    SM_P3B_PrizeFight   a title fight in an open-air ring: two boxers jabbing, the referee, corner men with towels,
                        the judges' table and its bell, a row hopping behind
    SM_P3B_RimRescue    a rope-rescue drill on the canyon's rim (a 16.5 m column of the cut's own rock, like the
                        first cast's rim pieces): a tripod over the lip, the stretcher swinging on its rope, two
                        hauling, the leader signalling with a flag. Built big: it is seen from far below.

Everything moves through M_People (ft_people.py). No words, no marks, no light on any piece. The placing rules are
C:\\ftwork\\p3\\L2b.inl (pasted by the integrator at the end of FTPeopleL2.inl).
Run in the editor:  import ft_people_nevada_p3b; ft_people_nevada_p3b.run()
"""
import math
import random
import ft_people as P
import ft_people_nevada as N
from ft_desert import rod, tilt_box

DIR = '/Game/People/Nevada'

SAND, SANDST, SANDST_D, SANDST_L = N.SAND, N.SANDST, N.SANDST_D, N.SANDST_L
WOOD, BARK, BARK_D = N.WOOD, N.BARK, N.BARK_D
LEATHER, NAVY, PURP, MAG, TEAL, RED, CYAN = N.LEATHER, N.NAVY, N.PURP, N.MAG, N.TEAL, N.RED, N.CYAN
CREAM, WHITE, GOLD, YEL, STEEL, CHROME, CONC = N.CREAM, N.WHITE, N.GOLD, N.YEL, N.STEEL, N.CHROME, N.CONC
DENIM, RUST, MINT, PINK, LEMON = N.DENIM, N.RUST, N.MINT, N.PINK, N.LEMON
ORANGE, LILAC, LIME, WATER, SALT = '#ff7a3c', '#f4a6ff', '#b8ff5a', '#57c7ff', '#f4f1ea'
HOT = N.HOT
H0 = 200.0           # the floor every scene stands on


# ------------------------------------------------------------------------------------------ what a scene stands on
def plinth(g, L, D, H, top, side, stripe=None, y=0.0):
    """A solid stage: its body from 1.2 m under the ground (the land may fall away) up to a floor at H."""
    g.box(side, (0, y, (H - 14 - 120) / 2.0), (L, D, H - 14 + 120))
    g.box(top, (0, y, H - 7), (L + 12, D + 12, 14))
    if stripe:
        g.box(stripe, (0, y - D / 2.0 - 1.5, H - 52), (L, 3, 26))


def rail(g, x0, x1, y, z, col=CREAM, posts=5, h=100.0, bars=2):
    """A fence along X: posts and thin bars (5 cm: people behind it show)."""
    for k in range(posts):
        g.box(col, (x0 + k * (x1 - x0) / (posts - 1.0), y, z + h / 2.0), (9, 9, h))
    for k in range(bars):
        g.box(col, ((x0 + x1) / 2.0, y, z + h - 6 - k * (h * 0.45)), (abs(x1 - x0), 6, 7))


def who(r, s=1.15, **fixed):
    """A random person of this road, at the size that reads from it (2 m), drawn for the middle distance."""
    return N.person(r, s, **fixed)


def far(r, s=1.15, **fixed):
    """The same, in the plainest drawing (a row at the back, a chorus)."""
    fixed.setdefault('detail', 0)
    return N.person(r, s, **fixed)


def kid(r, **fixed):
    d = N.person(r, **fixed)
    d.update(build='kid', s=0.8)
    return d


# ------------------------------------------------------------------------------------------ what a hand holds
def hold_glove(col=RED):
    def f(m, h, side):
        m.sphere(col, (h[0], h[1] - 3, h[2]), 14, steps=(3, 6))
    return f


def hold_fan(col=MAG, tip=WHITE):
    """A feather fan: a broad blade and its pale tips."""
    def f(m, h, side):
        m.box(col, (h[0], h[1] - 2, h[2] + 30), (62, 4, 52))
        m.box(tip, (h[0], h[1] - 2, h[2] + 60), (70, 4, 12))
    return f


def hold_towel(col=WHITE):
    def f(m, h, side):
        m.box(col, (h[0], h[1], h[2] + 26), (30, 4, 58))
    return f


def hold_horn(col=GOLD):
    """A trumpet, held out ahead."""
    def f(m, h, side):
        m.box(col, (h[0], h[1] - 24, h[2] + 4), (6, 50, 6))
        m.cyl(col, (h[0], h[1] - 48, h[2] + 4), 5, 14, rot=(0, 0, 90), steps=6, top_r=13)
    return f


def hold_bone(col=WHITE):
    """A great thigh bone held up in both hands (drawn once, from the right hand across)."""
    def f(m, h, side):
        if side > 0:
            m.box(col, (h[0] - 30, h[1], h[2] + 12), (120, 11, 11))
            for sx in (-1, 1):
                m.sphere(col, (h[0] - 30 + sx * 62, h[1], h[2] + 12), 14, steps=(3, 5))
    return f


def hold_fiddle(col='#b8553a'):
    def f(m, h, side):
        m.box(col, (h[0], h[1] - 6, h[2] + 10), (20, 9, 40))
        m.box(LEATHER, (h[0], h[1] - 6, h[2] + 40), (5, 5, 30))
    return f


def hold_bow(col=CREAM):
    def f(m, h, side):
        m.box(col, (h[0], h[1] - 4, h[2] + 18), (5, 5, 70))
    return f


def hold_ladle(col=STEEL):
    def f(m, h, side):
        m.box(col, (h[0], h[1], h[2] - 26), (5, 5, 70))
        m.sphere(col, (h[0], h[1], h[2] - 62), 10, steps=(3, 5))
    return f


def hold_brush(col=YEL):
    def f(m, h, side):
        m.box(WOOD, (h[0], h[1], h[2] - 10), (5, 5, 30))
        m.box(col, (h[0], h[1], h[2] - 30), (22, 6, 14))
    return f


def hold_ball(col=WHITE):
    def f(m, h, side):
        m.sphere(col, (h[0], h[1], h[2] + 16), 16, steps=(3, 6))
    return f


# ------------------------------------------------------------------------------------------ 1. the rodeo
def rodeo(sc):
    """A rodeo pen on an earth bank: the bronc bucks under its rider, the clown pops out of his barrel, a roper spins
    his loop over his head, a calf trots the length of the fence, three hands at the back rail wave their hats, and the
    flag judge stands on his crate."""
    r = random.Random(301)
    g = sc.still()
    H, L, D = H0, 1200.0, 500.0
    hx, hy = N.rock_stack(g, r, L + 160, D + 160, H, tones=(SANDST_D, SANDST), band=100.0, top=0.9, cap=SAND, rough=0.06)
    rail(g, -L / 2.0 + 20, L / 2.0 - 20, -D / 2.0 + 20, H, col=CREAM, posts=7, h=92)
    rail(g, -L / 2.0 + 20, L / 2.0 - 20, D / 2.0 - 20, H, col=CREAM, posts=7, h=120)
    N.bunting(g, -L / 2.0 + 20, L / 2.0 - 20, D / 2.0 - 20, H + 330, n=10)
    for sx in (-1, 1):
        g.box(CREAM, (sx * (L / 2.0 - 20), D / 2.0 - 20, H + 225), (8, 8, 210))
    # the bronc and its rider: the whole pair hops, the legs fly
    d = N.cowhand(r, 1.15, top=RED, hat=None)
    d.update(arm_r='up', hold_r=N.hold_hat(CREAM), swing_r=30, axis_r='x', arm_l='fwd')
    N.mount(sc, r, -230, -30, H, yaw=75, s=1.3, coat='#2a2230', mane='#22202a', rider=d, phase=0.0, rate=1.5, bob=26, stride=36, neck_up=30)
    # the clown: he comes up out of the barrel and goes down again
    g.cyl(RED, (150, -70, H), 46, 96, steps=8)
    g.cyl(WHITE, (150, -70, H + 34), 47.5, 20, steps=8)
    P.figure(sc, 150, -70, z=H + 8, yaw=-10, s=1.05, skin=P.SKINS[0], top='#ff5fa2', bottom=YEL, hair='afro', hair_col='#7dffb0', band=CYAN,
             arm_l='vee', arm_r='vee', swing_l=18, swing_r=-18, bob=44, rate=0.7, phase=0.3, detail=1)
    # the roper and his loop (a ring that goes round over his head)
    P.figure(sc, 430, 10, z=H, yaw=-25, arm_r='up', swing_r=16, axis_r='z', arm_l='hip', legs='wide', rate=1.1, phase=0.1,
             **N.cowhand(r, 1.15, top=CYAN, hat_col=CREAM))
    lz = H + 1.15 * 232
    loop = sc.part(P.motion(base=(455.0, 10.0), travel=(-1.0, 1.1)))
    for k in range(6):
        a0, a1 = 2 * math.pi * k / 6.0, 2 * math.pi * (k + 1) / 6.0
        tilt_box(loop, CREAM if k else MAG, (485 + math.cos(a0) * 70, 10 + math.sin(a0) * 70, lz), (485 + math.cos(a1) * 70, 10 + math.sin(a1) * 70, lz), 6, 6)
    # the calf, the length of the pen and round again
    P.quadruped(sc, 0, 120, H, yaw=90, s=0.95, length=150, height=96, girth=50, neck=30, neck_up=14, head=34, body=WHITE, belly='#b8553a',
                ears='#b8553a', tail=40, tail_col='#b8553a', stride=28, wag=18, rate=2.2, bob=5, phase=0.4, travel=(L - 160, 300))
    # the hands at the back rail, up on the bottom bar
    g.box(BARK, (-80, D / 2.0 - 62, H + 20), (620, 40, 40))
    for i, x in enumerate((-300, -90, 130)):
        c = N.cowhand(r, 1.12, detail=0, top=(GOLD, MINT, ORANGE)[i], hat=None)
        c.update(arm_r='wave', hold_r=N.hold_hat(c['hat_col']), swing_r=26, arm_l=('hip', 'vee', 'hip')[i], bob=5 + 2 * i, rate=1.2 + 0.15 * i, react=True)
        P.figure(sc, x, D / 2.0 - 62, z=H + 40, yaw=r.uniform(-12, 12), phase=r.random(), **c)
    # the flag judge on his crate
    g.box(TEAL, (-520, -90, H + 25), (80, 70, 50))
    P.figure(sc, -520, -90, z=H + 50, yaw=14, arm_r='up', hold_r=P.hold_flag(col=MAG, w=90), swing_r=24, rate=0.8, arm_l='hip', phase=0.6,
             **N.cowhand(r, 1.12, top=WHITE, hat_col=LEATHER, band=RED))


# ------------------------------------------------------------------------------------------ 2. the sky watchers
def sky_watch(sc):
    """The sky watchers' camp on a rock bench: foil hats, a dish turning slowly, a telescope, a landing ring of painted
    stones, two in lawn chairs with field glasses, a child in an alien suit hopping, an alien balloon on its string."""
    r = random.Random(302)
    g = sc.still()
    H, L, D = H0, 1150.0, 500.0
    hx, hy = N.rock_stack(g, r, L + 140, D + 140, H, tones=(SANDST_D, SANDST_L), band=100.0, top=0.9, cap=SAND, rough=0.07)
    # the landing ring: painted stones round a cross of tape
    for k in range(10):
        a = 2 * math.pi * k / 10.0
        g.sphere((MAG, WHITE)[k % 2], (-60 + math.cos(a) * 150, 60 + math.sin(a) * 110, H + 8), 20, scale=(1, 1, 0.7), steps=(3, 5))
    g.box(CYAN, (-60, 60, H + 2), (170, 16, 4))
    g.box(CYAN, (-60, 60, H + 2), (16, 130, 4))
    # the dish on its mast, turning to and fro
    dx, dy = 440.0, 110.0
    g.box(STEEL, (dx, dy, H + 110), (10, 10, 220))
    for k in range(3):
        a = 2 * math.pi * k / 3.0
        tilt_box(g, STEEL, (dx, dy, H + 90), (dx + math.cos(a) * 70, dy + math.sin(a) * 70, H), 6, 6)
    dish = sc.part(P.motion(swing=35.0, pivot=(dx, dy, H), axis=P.UP, rate=0.12))
    dish.cyl(CHROME, (dx, dy, H + 215), 16, 46, rot=(-45, -90, 0), steps=8, top_r=105)
    dish.box(MAG, (dx, dy - 52, H + 268), (7, 7, 60), rot=(0, 0, 45))
    # the telescope and its astronomer
    tx, ty = -430.0, -60.0
    for k in range(3):
        a = 2 * math.pi * k / 3.0 + 0.5
        tilt_box(g, LEATHER, (tx, ty, H + 135), (tx + math.cos(a) * 50, ty + math.sin(a) * 50, H), 6, 6)
    rod(g, WHITE, (tx, ty + 46, H + 112), (tx, ty - 60, H + 190), 13, steps=7)
    P.figure(sc, tx, ty + 86, z=H, yaw=0, arm_r='face', arm_l='hip', rock=4, rate=0.35, **who(r, top=PURP, hat='chef', hat_col=CHROME))
    # the believers: both arms to the sky, a striped card, one pointing out the lights
    P.figure(sc, -240, -120, z=H, yaw=-8, arm_l='up', arm_r='up', swing_l=12, swing_r=-12, bob=8, rate=1.1, phase=0.2, react=True,
             **who(r, top=LIME, hat='chef', hat_col=CHROME, hair='long'))
    P.figure(sc, 130, -130, z=H, yaw=6, arm_l='hold', arm_r='up', hold_r=P.hold_sign(col=CREAM, stripe=LILAC, w=110, h2=74), swing_r=10, bob=5, rate=0.9,
             phase=0.5, **who(r, top=ORANGE, hat='chef', hat_col=CHROME))
    P.figure(sc, 300, -70, z=H, yaw=20, arm_r=((150, 30), (160, 30)), swing_r=9, axis_r='x', arm_l='hip', rate=0.6, phase=0.8,
             **who(r, top=CYAN, build='stocky', hat='cap', hat_col=CHROME))
    # two in lawn chairs with field glasses
    for i, x in enumerate((-150, 30)):
        g.box((TEAL, MAG)[i], (x, 190, H + 44), (60, 54, 8))
        g.box((TEAL, MAG)[i], (x, 220, H + 84), (60, 8, 80))
        g.box(STEEL, (x, 190, H + 20), (56, 6, 40))
        P.figure(sc, x, 185, z=H, yaw=(-10, 12)[i], legs='sit', seat=46, arm_l='face', arm_r='face', hold_r=P.hold_camera(LEATHER, CHROME), rock=5,
                 rate=0.3 + 0.1 * i, phase=0.4 * i, **far(r, top=(GOLD, PINK)[i], hat='sun', hat_col=CHROME))
    # the child in the alien suit, and the balloon
    P.figure(sc, 20, 40, z=H, yaw=-5, build='kid', s=0.82, skin=LILAC, top=LILAC, bottom=LILAC, shoes=LILAC, hair='bald', sleeves='long',
             arm_l='vee', arm_r='vee', swing_l=20, swing_r=-20, bob=16, rate=1.7, phase=0.1, react=True, detail=1)
    bx, by = 250.0, 160.0
    g.box(CREAM, (bx, by, H + 150), (4, 4, 300))
    b = sc.part(P.motion(bob=-12.0, rate=0.4, phase=0.3))
    b.sphere(LILAC, (bx, by, H + 372), 66, scale=(1.0, 0.9, 1.2), steps=(4, 8))
    b.sphere(LILAC, (bx, by, H + 300), 30, scale=(1, 1, 1.1), steps=(3, 6))
    for sx in (-1, 1):
        b.box(P.EYE, (bx + sx * 26, by - 56, H + 380), (22, 6, 34), rot=(0, sx * 20, 0))


# ------------------------------------------------------------------------------------------ 3. the record crew
def salt_record(sc):
    """The record crew's pit on the salt: a mint streamliner on its stands, one mechanic polishing its nose and one at
    the tail with a wrench, the driver in helmet and white suit with both arms up, the flagman, the timekeeper at his
    table under a parasol, somebody filming."""
    r = random.Random(303)
    g = sc.still()
    H, L, D = H0, 1200.0, 480.0
    plinth(g, L, D, H, SALT, '#d8d2c6', stripe=MAG)
    for k in range(6):
        g.box((LEATHER, WHITE)[k % 2], (-L / 2.0 + (k + 0.5) * L / 6.0, -D / 2.0 - 3.2, H - 24), (L / 6.0, 3, 20))
    # the car: a cigar 8 m long, a fin, a canopy, four wheels half inside the body
    cx, cy, cz = -60.0, 90.0, H + 62.0
    g.sphere(MINT, (cx, cy, cz), 100, scale=(4.0, 0.58, 0.46), steps=(5, 8))
    g.box(MAG, (cx, cy - 53, cz + 2), (560, 4, 12))
    g.sphere(LEATHER, (cx - 60, cy, cz + 34), 40, scale=(1.5, 0.6, 0.6), steps=(3, 6))
    g.side(MINT, [(cx + 250, cz + 10), (cx + 390, cz + 14), (cx + 380, cz + 150), (cx + 330, cz + 150)], cy - 4, cy + 4)
    for sx in (-250, 230):
        for sy in (-1, 1):
            g.cyl(LEATHER, (cx + sx, cy + sy * 46, cz - 26), 34, 14, rot=(0, 0, 90), steps=8)
    for sx in (-150, 140):
        g.box(STEEL, (cx + sx, cy, H + 12), (60, 80, 24))
    # the crew
    P.figure(sc, cx - 410, cy - 40, z=H, yaw=70, arm_l='push', arm_r='push', swing_l=18, swing_r=18, axis_l='z', axis_r='z', rate=1.2, phase=0.2,
             **who(r, top=ORANGE, bottom=NAVY, hat='cap', hat_col=ORANGE, band=WHITE))
    P.figure(sc, cx + 440, cy - 30, z=H, yaw=-70, arm_r='fwd', hold_r=P.hold_tool(), swing_r=24, axis_r='x', arm_l='hip', rate=0.9, phase=0.6,
             **who(r, top=ORANGE, bottom=NAVY, hat='cap', hat_col=ORANGE, band=WHITE, build='stocky'))
    P.figure(sc, -40, -110, z=H, yaw=-4, arm_l='vee', arm_r='vee', swing_l=10, swing_r=-10, bob=9, rate=1.2, phase=0.1, react=True,
             **who(r, top=WHITE, bottom=WHITE, sleeves='long', hat='helmet', hat_col=MAG, band=MAG, shoes=RED))
    P.figure(sc, 210, -130, z=H, yaw=10, arm_r='up', hold_r=P.hold_flag(col=WHITE, w=110), swing_r=34, rate=1.0, arm_l='hip', phase=0.4, react=True,
             **who(r, top=YEL, bottom=LEATHER, hair='pony'))
    P.figure(sc, -300, -120, z=H, yaw=-16, arm_l='hold', arm_r='hold', hold_r=P.hold_camera(), rock=4, rate=0.4, **who(r, top=CYAN, hat='sun', hat_col=LEMON, wear='shorts'))
    # the timekeeper's table: a clock face with no figures on it, the parasol, the keeper and a helper
    tx = 470.0
    g.box(CREAM, (tx, -80, H + 70), (150, 70, 8))
    for sx in (-1, 1):
        g.box(STEEL, (tx + sx * 66, -80, H + 33), (6, 60, 66))
    g.cyl(WHITE, (tx - 30, -84, H + 76), 22, 30, rot=(0, 0, 90), steps=8)
    N.parasol(g, tx + 20, 0, H, (MAG, CREAM), r=150, h=300)
    P.figure(sc, tx, -20, z=H, yaw=-10, arm_r='face', arm_l='hold', hold_l=P.hold_phone(LEATHER, CREAM), rock=3, rate=0.5,
             **who(r, top=PURP, hat='sun', hat_col=CREAM, wear='dress'))
    P.figure(sc, tx - 130, 130, z=H, yaw=30, arm_r='point', swing_r=10, axis_r='x', arm_l='hip', rate=0.7, phase=0.3, **far(r, top=LIME))
    P.figure(sc, 130, 200, z=H, yaw=0, arm_r='wave', swing_r=24, bob=6, rate=1.3, phase=0.7, react=True, **far(r, top=PINK, hat='cap', hat_col=WHITE))


# ------------------------------------------------------------------------------------------ 4. the revue
def revue(sc):
    """A casino's outdoor stage: a kick line of five with feather fans, the wave running along it, the bandleader out
    front with his baton, a drummer and a trumpet at the side, a curtain and its pennants behind."""
    r = random.Random(304)
    g = sc.still()
    H, L, D = H0, 1200.0, 440.0
    plinth(g, L, D, H, PURP, NAVY, stripe=GOLD)
    for k in range(8):
        g.box((GOLD, MAG)[k % 2], (-L / 2.0 + (k + 0.5) * L / 8.0, -D / 2.0 - 3.2, H - 26), (L / 8.0 * 0.5, 3, 30))
    # the curtain, in folds
    for k in range(8):
        g.box((MAG, '#c94f6a')[k % 2], (-L / 2.0 + (k + 0.5) * L / 8.0, D / 2.0 - 10, H + 170), (L / 8.0, 14 + 8 * (k % 2), 340))
    g.box(GOLD, (0, D / 2.0 - 12, H + 352), (L + 20, 26, 26))
    N.bunting(g, -L / 2.0, L / 2.0, D / 2.0 - 30, H + 330, n=12)
    # the line: the same step a fifth of a beat apart, so a wave runs along it
    cols = (CYAN, '#ff5fa2', YEL, '#7dffb0', LILAC)
    for i in range(5):
        x = -420 + i * 150
        P.figure(sc, x, 20, z=H, yaw=0, legs='stride', stride=34, arm_l='vee', arm_r='vee', hold_l=hold_fan(cols[(i + 2) % 5]), hold_r=hold_fan(cols[(i + 2) % 5]),
                 swing_l=16, swing_r=-16, bob=7, rate=1.3, phase=i * 0.2,
                 **who(r, 1.15, top=cols[i], bottom=cols[i], wear='dress', hair='bun', hat=None, sleeves='none', shoes=WHITE))
    # the bandleader, the drummer, the trumpet
    P.figure(sc, -40, -150, z=H, yaw=180, arm_r='up', hold_r=hold_bow(WHITE), swing_r=30, axis_r='y', arm_l='out', swing_l=14, rate=1.3, bob=3,
             **who(r, top=WHITE, bottom=LEATHER, sleeves='long', band=MAG, hair='short'))
    dx = 440.0
    g.cyl(RED, (dx, -60, H + 40), 34, 30, steps=8)
    g.cyl(CREAM, (dx, -60, H + 70), 35, 3, steps=8)
    g.box(STEEL, (dx, -60, H + 20), (8, 8, 40))
    g.cyl(GOLD, (dx + 70, -30, H + 110), 26, 3, steps=7)
    g.box(STEEL, (dx + 70, -30, H + 55), (5, 5, 110))
    g.box(LEATHER, (dx, 10, H + 23), (40, 40, 46))
    P.figure(sc, dx, 10, z=H, yaw=0, legs='sit', seat=46, arm_l='carry', arm_r='carry', hold_l=hold_bow(WOOD), hold_r=hold_bow(WOOD), swing_l=22, swing_r=-22,
             axis_l='x', axis_r='x', rate=2.6, **far(r, top=GOLD, bottom=LEATHER, hat='cap', hat_col=LEATHER))
    P.figure(sc, dx + 110, 90, z=H, yaw=-20, arm_r='face', hold_r=hold_horn(), arm_l='hip', rock=7, rate=0.65, phase=0.2, **far(r, top=GOLD, bottom=LEATHER))
    P.figure(sc, -dx - 60, 60, z=H, yaw=20, arm_l='hold', arm_r='hold', swing_l=13, swing_r=-13, axis_l='z', axis_r='z', bob=4, rate=2.4, phase=0.5, react=True,
             **far(r, top=CREAM, bottom=PURP, wear='coat', hat='peak', hat_col=PURP))


# ------------------------------------------------------------------------------------------ 5. the barn dance
def hoe_down(sc):
    """A barn dance on a board floor before a red barn front: two squares of four go round, one each way, the fiddler
    saws away on a bale, the caller waves his hat from a crate."""
    r = random.Random(305)
    g = sc.still()
    H, L, D = H0, 1200.0, 500.0
    plinth(g, L, D, H, WOOD, BARK_D, stripe=CREAM)
    # the barn's front: boards, a cross-braced door, a hay door over it
    yb = D / 2.0 - 12
    g.box('#c9403a', (0, yb, H + 180), (L, 20, 360))
    g.side('#c9403a', [(-L / 2.0, H + 360), (L / 2.0, H + 360), (0, H + 520)], yb - 10, yb + 10)
    g.box(CREAM, (0, yb - 11, H + 120), (220, 4, 240))
    tilt_box(g, '#c9403a', (-100, yb - 14, H + 10), (100, yb - 14, H + 230), 4, 14)
    tilt_box(g, '#c9403a', (-100, yb - 14, H + 230), (100, yb - 14, H + 10), 4, 14)
    g.box(CREAM, (0, yb - 11, H + 420), (90, 4, 80))
    N.bunting(g, -L / 2.0 + 10, L / 2.0 - 10, -D / 2.0 + 14, H + 300, n=12)
    for sx in (-1, 1):
        g.box(CREAM, (sx * (L / 2.0 - 10), -D / 2.0 + 14, H + 150), (8, 8, 300))
    # the two squares
    tops = (RED, CYAN, GOLD, '#ff5fa2', MINT, ORANGE, LILAC, LIME)
    for q, (cx, way) in enumerate(((-300.0, 1.0), (240.0, -1.0))):
        for k in range(4):
            a = 2 * math.pi * k / 4.0 + 0.4 * q
            dress = (k % 2 == 1)
            c = N.cowhand(r, 1.12, detail=0, top=tops[q * 4 + k], hat=(None if dress else 'cowboy'))
            if dress:
                c.update(wear='dress', hair=r.choice(('long', 'pony', 'bun')), sleeves='short')
            P.figure(sc, cx + math.cos(a) * 125, -30 + math.sin(a) * 125, z=H, yaw=math.degrees(a) - 90.0, arm_l='out', arm_r='out', swing_l=10, swing_r=-10,
                     stride=16, bob=7, rate=1.6, phase=0.25 * k, orbit=(cx, -30.0, way * 0.11), **c)
    # the fiddler on a bale, the caller on a crate, bales
    fx = 520.0
    for (bx, by, bz) in ((fx, 90, 0), (fx - 96, 130, 0), (-fx, 120, 0), (-fx, 120, 44)):
        g.box(YEL, (bx, by, H + bz + 22), (90, 56, 44))
    P.figure(sc, fx, 90, z=H, yaw=-25, legs='sit', seat=44, arm_l='hold', hold_l=hold_fiddle(), arm_r='hold', hold_r=hold_bow(), swing_r=26, axis_r='z', rate=2.2,
             rock=0, **N.cowhand(r, 1.12, top=WHITE, hat_col=LEATHER, band=RED))
    g.box(TEAL, (-fx + 120, -60, H + 24), (70, 60, 48))
    c = N.cowhand(r, 1.12, top=PURP, hat=None)
    c.update(arm_r='wave', hold_r=N.hold_hat(CREAM), swing_r=28, arm_l='face', bob=4, rate=1.1, react=True)
    P.figure(sc, -fx + 120, -60, z=H + 48, yaw=12, phase=0.3, **c)
    P.quadruped(sc, 20, 170, H, yaw=-10, s=1.2, length=70, height=48, girth=20, neck=16, neck_up=40, head=18, tail=22, wag=36, rate=2.4, bob=4,
                body='#e8c26a', belly=CREAM)


# ------------------------------------------------------------------------------------------ 6. the dig
def dig(sc):
    """A fossil dig on a rock bench: the great ribs, spine and skull of something very old, one swinging a pick, one
    brushing on her knees, one holding a thigh bone over his head, a barrow going to and fro, the professor pointing
    under the sun shade, a sieve rocking on its trestle."""
    r = random.Random(306)
    g = sc.still()
    H, L, D = H0, 1200.0, 520.0
    hx, hy = N.rock_stack(g, r, L + 140, D + 140, H, tones=(N.TALUS, SANDST, SANDST_L), band=70.0, top=0.9, cap=SAND, rough=0.07)
    bone = WHITE
    # the skeleton, standing out of the bench along the back: five ribs each side, the spine, the tail, the skull
    sx0, sy0 = -260.0, 120.0
    for k in range(5):
        x = sx0 + k * 92
        hgt = 170.0 - abs(k - 1.5) * 22
        g.box(bone, (x, sy0, H + hgt), (96, 16, 16))
        for sy in (-1, 1):
            tilt_box(g, bone, (x, sy0 + sy * 6, H + hgt), (x, sy0 + sy * 92, H + hgt * 0.4), 11, 11)
            tilt_box(g, bone, (x, sy0 + sy * 92, H + hgt * 0.4), (x, sy0 + sy * 60, H - 4), 10, 10)
    tilt_box(g, bone, (sx0 + 4 * 92 + 40, sy0, H + 120), (sx0 + 4 * 92 + 330, sy0 + 30, H + 6), 13, 13)
    tilt_box(g, bone, (sx0 - 40, sy0, H + 150), (sx0 - 150, sy0 - 10, H + 200), 15, 15)
    g.box(bone, (sx0 - 215, sy0 - 14, H + 214), (150, 62, 60))
    g.box(bone, (sx0 - 250, sy0 - 14, H + 170), (130, 50, 16))
    g.box(P.EYE, (sx0 - 185, sy0 - 46, H + 226), (26, 4, 22))
    for k in range(4):
        g.box(bone, (sx0 - 300 + k * 30, sy0 - 40, H + 184), (10, 6, 16))
    # the sun shade over the finds table
    tx, ty = 420.0, 110.0
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(BARK, (tx + sx * 150, ty + sy * 90, H + 130), (8, 8, 260))
    for k in range(4):
        g.box((TEAL, CREAM)[k % 2], (tx - 120 + k * 80, ty, H + 262), (80, 200, 6))
    g.box(CREAM, (tx, ty + 20, H + 72), (190, 70, 8))
    g.box(BARK, (tx, ty + 20, H + 34), (170, 8, 68))
    for k in range(3):
        g.box(bone, (tx - 60 + k * 55, ty + 20, H + 82), (36, 14, 10))
    P.figure(sc, tx - 20, ty - 70, z=H, yaw=30, arm_r='point', swing_r=10, axis_r='x', arm_l='hold', hold_l=P.hold_sign(col=CREAM, stripe=TEAL, w=44, h2=56, pole=CREAM),
             rate=0.6, phase=0.2, **who(r, top=CREAM, bottom='#5a4a3c', hat='sun', hat_col=CREAM, sleeves='long', band=TEAL))
    # the pick, the brush, the bone held up
    P.figure(sc, -80, -90, z=H, yaw=160, legs='wide', arm_r=((140, 70), (150, 70)), hold_r=N.hold_pick(), swing_r=42, axis_r='x', arm_l='hip', rate=0.9, phase=0.5,
             **who(r, top=RED, bottom=DENIM, hat='hard', hat_col=YEL, build='stocky'))
    P.figure(sc, 120, -40, z=H - 20, yaw=170, legs='sit', seat=30, arm_r='fwd', hold_r=hold_brush(), swing_r=22, axis_r='z', arm_l='down', rate=1.8,
             **who(r, top=LIME, hat='sun', hat_col=PINK, hair='pony'))
    P.figure(sc, -330, -120, z=H, yaw=-10, arm_l='up', arm_r='up', hold_r=hold_bone(), bob=10, rate=1.2, phase=0.1, react=True,
             **who(r, top=CYAN, wear='shorts', hat='cap', hat_col=ORANGE))
    P.figure(sc, 300, -150, z=H, yaw=-18, arm_l='hold', arm_r='hold', hold_r=P.hold_camera(), rock=4, rate=0.4, **far(r, top=LILAC, hat='sun', hat_col=LEMON))
    # the barrow, the length of the front and round again
    bx, by, tv = 0.0, -200.0, (L - 200, 110)
    P.figure(sc, bx, by, z=H, yaw=90, legs='stride', arm_l='push', arm_r='push', stride=22, rate=1.3, bob=2, travel=tv,
             **who(r, top=ORANGE, bottom=DENIM, hat='hard', hat_col=WHITE))
    bw = sc.part(P.motion(rate=1.3, base=(bx, by), travel=(float(tv[0]), float(tv[1]) / float(tv[0]))))
    bw.box(TEAL, (bx + 100, by, H + 62), (86, 60, 34))
    bw.cyl(LEATHER, (bx + 150, by - 6, H + 22), 22, 12, rot=(0, 0, 90), steps=7)
    bw.sphere(SANDST, (bx + 100, by, H + 84), 34, scale=(1.1, 0.8, 0.6), steps=(3, 6))
    for sy in (-1, 1):
        tilt_box(bw, STEEL, (bx + 44, by + sy * 24, H + 96), (bx + 110, by + sy * 24, H + 50), 5, 5)
    # the sieve on its trestle, rocked by a sixth hand
    vx, vy = -500.0, 60.0
    for sx in (-1, 1):
        g.box(BARK, (vx + sx * 50, vy, H + 50), (8, 60, 100))
    sv = sc.part(P.motion(swing=8.0, pivot=(vx, vy, H + 100), axis=0.0, rate=1.6))
    sv.box(WOOD, (vx, vy, H + 108), (110, 76, 14))
    sv.box(SAND, (vx, vy, H + 116), (96, 62, 4))
    P.figure(sc, vx, vy + 80, z=H, yaw=0, arm_l='carry', arm_r='carry', swing_l=10, swing_r=10, axis_l='x', axis_r='x', rate=1.6,
             **far(r, top=PINK, bottom=DENIM, hat='sun', hat_col=WHITE))
    N.flag(sc, -hx + 40, hy - 40, H, 380, ORANGE, patch=WHITE, w=150, h=100, pole=BARK, phase=0.4)


# ------------------------------------------------------------------------------------------ 7. the pool party
def pool_party(sc):
    """A motel's pool deck: three swimmers bobbing and a beach ball going up between them, one bouncing on the board,
    the lifeguard on her high chair, a child hopping at the edge, a waiter with a tray going the length of the deck, a
    flamingo float, the motel's doors behind."""
    r = random.Random(307)
    g = sc.still()
    H, L, D = H0, 1200.0, 520.0
    plinth(g, L, D, H, CONC, PINK, stripe=TEAL)
    # the motel's wall and doors
    yb = D / 2.0 - 10
    g.box(CREAM, (0, yb, H + 140), (L, 16, 280))
    g.box(TEAL, (0, yb - 4, H + 290), (L + 30, 60, 14))
    for k in range(5):
        x = -L / 2.0 + (k + 0.5) * L / 5.0
        g.box((MAG, TEAL, ORANGE, TEAL, MAG)[k], (x - 40, yb - 9, H + 100), (80, 3, 200))
        g.box(WATER, (x + 60, yb - 9, H + 150), (70, 3, 70))
    # the pool: water flush in the deck, a coping round it
    px, py, pw, pd = -80.0, -40.0, 760.0, 260.0
    g.box(WATER, (px, py, H + 3), (pw, pd, 6))
    for (cx, cy, w, d) in ((px, py - pd / 2.0, pw + 24, 14), (px, py + pd / 2.0, pw + 24, 14), (px - pw / 2.0, py, 14, pd), (px + pw / 2.0, py, 14, pd)):
        g.box(WHITE, (cx, cy, H + 8), (w, d, 16))
    # the swimmers (chest deep: the rest of them is inside the deck) and the ball
    sw = ((-330, -30, CYAN, 'vee'), (-120, -80, '#ff5fa2', 'up'), (110, 10, YEL, 'vee'))
    for i, (x, y, top, arms) in enumerate(sw):
        P.figure(sc, x, y, z=H + 6 - 100 * 1.1, yaw=(20, -15, 5)[i], arm_l=arms, arm_r=arms, swing_l=14, swing_r=-14, bob=-6, rate=0.9 + 0.15 * i, phase=0.3 * i,
                 react=True, **who(r, 1.1, top=top, wear='swim', sleeves='none', hat=(None, 'cap', None)[i], hat_col=WHITE))
    ball = sc.part(P.motion(bob=170.0, rate=0.42, swing=25.0, pivot=(-110.0, -40.0, H - 300.0), axis=90.0))
    ball.sphere(WHITE, (-110, -40, H + 150), 34, steps=(4, 8))
    ball.box(RED, (-110, -40, H + 150), (70, 70, 12))
    # the flamingo float
    fl = sc.part(P.motion(bob=-5.0, rate=0.5, swing=14.0, pivot=(190.0, -110.0, H), axis=P.UP))
    fl.cyl(PINK, (190, -110, H + 6), 50, 22, steps=8)
    tilt_box(fl, PINK, (190, -150, H + 20), (190, -160, H + 100), 12, 12)
    fl.sphere(PINK, (190, -168, H + 108), 15, steps=(3, 5))
    fl.box(GOLD, (190, -186, H + 104), (8, 22, 8))
    # the board and its diver
    bx = 430.0
    g.box(STEEL, (bx + 60, py, H + 55), (30, 50, 110))
    g.box(CYAN, (bx - 40, py, H + 114), (260, 50, 8))
    P.figure(sc, bx - 130, py, z=H + 118, yaw=-90, arm_l='up', arm_r='up', bob=28, rate=0.9, phase=0.2, **who(r, 1.1, top=ORANGE, wear='swim', sleeves='none'))
    # the lifeguard's chair
    lx, ly = -520.0, 130.0
    for sx in (-1, 1):
        tilt_box(g, WHITE, (lx + sx * 40, ly - 30, H), (lx + sx * 26, ly, H + 170), 8, 8)
        tilt_box(g, WHITE, (lx + sx * 40, ly + 40, H), (lx + sx * 26, ly + 10, H + 170), 8, 8)
    g.box(RED, (lx, ly, H + 174), (70, 60, 8))
    g.box(RED, (lx, ly + 30, H + 220), (70, 8, 90))
    P.figure(sc, lx, ly, z=H + 178 - 46 * 1.1, yaw=10, legs='sit', seat=46, arm_r='point', swing_r=12, axis_r='x', arm_l='face', rate=0.7,
             **who(r, 1.1, top=RED, bottom=RED, wear='shorts', hat='sun', hat_col=WHITE, hair='pony'))
    # the child at the edge, the waiter, somebody under a parasol
    P.figure(sc, -470, -150, z=H, yaw=-10, arm_l='vee', arm_r='vee', bob=15, rate=1.8, phase=0.6, react=True, **kid(r, top=LIME, wear='swim'))
    P.figure(sc, 0, 160, z=H, yaw=90, legs='stride', arm_l='carry', arm_r='carry', hold_r=N.hold_tray(), stride=20, rate=1.2, bob=2, travel=(L - 240, 120),
             **who(r, 1.1, top=WHITE, bottom=LEATHER, band=TEAL, sleeves='long'))
    N.parasol(g, 500, 150, H, (YEL, WHITE), r=150, h=280)
    g.box(YEL, (500, 110, H + 30), (60, 150, 10))
    P.figure(sc, 500, 120, z=H - 14, yaw=-20, legs='sit', seat=46, arm_r='face', hold_r=P.hold_cup(), arm_l='wave', swing_l=18, rate=0.8, phase=0.4,
             **far(r, 1.1, top=LILAC, wear='shorts', hat='sun', hat_col=PINK))


# ------------------------------------------------------------------------------------------ 8. the cook-off
def cook_off(sc):
    """The chili cook-off: three great pots steaming on a long table, one cook stirring, one tasting, one beating the
    dinner iron, two judges holding up score cards, one who took too big a spoonful hopping and fanning his mouth, a
    dog begging at the table's end."""
    r = random.Random(308)
    g = sc.still()
    H, L, D = H0, 1200.0, 460.0
    plinth(g, L, D, H, WOOD, RUST, stripe=YEL)
    N.bunting(g, -L / 2.0 + 10, L / 2.0 - 10, D / 2.0 - 16, H + 330, n=12)
    for sx in (-1, 0, 1):
        g.box(CREAM, (sx * (L / 2.0 - 10), D / 2.0 - 16, H + 165), (8, 8, 330))
    # the table: a checked cloth (squares, never a mark)
    ty = 40.0
    g.box(CREAM, (-60, ty, H + 78), (820, 110, 10))
    g.box(RED, (-60, ty - 56, H + 52), (820, 3, 60))
    for k in range(9):
        g.box(WHITE, (-60 - 364 + k * 91, ty - 58, H + 52), (44, 2, 60))
    for sx in (-1, 1):
        g.box(BARK, (-60 + sx * 380, ty, H + 38), (10, 90, 76))
    # the pots and their steam
    pots = ((-330.0, RUST, RED), (-60.0, STEEL, ORANGE), (210.0, LEATHER, '#c9403a'))
    for i, (x, col, chili) in enumerate(pots):
        g.cyl(col, (x, ty, H + 83), 52, 70, steps=8, top_r=58)
        g.cyl(chili, (x, ty, H + 150), 52, 5, steps=8)
        st = sc.part(P.motion(bob=-34.0, rate=0.3, phase=0.33 * i))
        st.sphere(WHITE, (x, ty, H + 230), 34, scale=(1, 1, 1.3), steps=(3, 6))
        st.sphere(WHITE, (x + 26, ty + 10, H + 300), 24, scale=(1, 1, 1.2), steps=(3, 5))
    # the cooks behind the table
    P.figure(sc, -330, ty + 100, z=H, yaw=0, arm_r='fwd', hold_r=hold_ladle(), swing_r=24, axis_r='z', arm_l='hip', rate=1.1,
             **who(r, top=WHITE, hat='chef', hat_col=WHITE, band=RED, build='stocky'))
    P.figure(sc, -60, ty + 100, z=H, yaw=8, arm_r='face', hold_r=P.hold_cup(RED), arm_l='hip', rock=4, rate=0.5, phase=0.3,
             **who(r, top=WHITE, hat='chef', hat_col=WHITE, band=ORANGE, hair='long'))
    ix, iy = 430.0, 130.0
    g.box(BARK, (ix, iy, H + 150), (8, 8, 300))
    g.box(BARK, (ix - 50, iy, H + 296), (110, 8, 8))
    tri = sc.part(P.motion(swing=10.0, pivot=(ix - 96, iy, H + 292), axis=90.0, rate=2.2))
    for (a, b) in (((-40, 0), (40, 0)), ((40, 0), (0, 70)), ((0, 70), (-40, 0))):
        tilt_box(tri, STEEL, (ix - 96 + a[0], iy, H + 200 + a[1]), (ix - 96 + b[0], iy, H + 200 + b[1]), 6, 6)
    P.figure(sc, ix - 170, iy, z=H, yaw=80, arm_r='fwd', hold_r=hold_bow(STEEL), swing_r=26, axis_r='x', arm_l='hip', rate=2.2, bob=3,
             **who(r, top=YEL, bottom=DENIM, hat='cowboy', hat_col=CREAM))
    # out front: the judges and their cards, the one with the burnt tongue, the queue
    for i, x in enumerate((-430, -250)):
        P.figure(sc, x, -120, z=H, yaw=(-12, 6)[i], arm_r='up', hold_r=P.hold_sign(col=(MINT, CREAM)[i], stripe=(MAG, RED)[i], w=64, h2=64), swing_r=8, bob=5,
                 rate=1.0, phase=0.4 * i, arm_l='hip', **who(r, top=(PURP, TEAL)[i], band=GOLD, hat=('sun', 'cowboy')[i], hat_col=CREAM))
    P.figure(sc, 40, -130, z=H, yaw=5, arm_l='face', arm_r='face', swing_l=24, swing_r=-24, axis_l='y', axis_r='y', bob=18, rate=2.1, phase=0.1, react=True,
             **who(r, top=RED, wear='shorts', hair='bald', build='stocky'))
    P.figure(sc, 230, -110, z=H, yaw=-30, arm_r='point', swing_r=10, axis_r='x', arm_l='hold', hold_l=P.hold_cup(CREAM), rock=5, rate=0.7,
             **far(r, top=CYAN, hat='cap', hat_col=RED))
    P.figure(sc, 380, -130, z=H, yaw=-10, arm_l='vee', arm_r='vee', bob=13, rate=1.7, phase=0.5, react=True, **kid(r, top=LIME, detail=0))
    P.quadruped(sc, 470, -40, H, yaw=110, s=1.25, length=70, height=48, girth=20, neck=16, neck_up=55, head=18, tail=22, wag=40, rate=2.6, bob=5,
                body=LEATHER, belly=WHITE)


# ------------------------------------------------------------------------------------------ 9. the prize fight
def prize_fight(sc):
    """A title fight in an open-air ring: two boxers bouncing and jabbing, the referee, a corner man each side with a
    towel, the judges' table with its bell, a row hopping behind them."""
    r = random.Random(309)
    g = sc.still()
    H, L, D = H0, 1200.0, 520.0
    plinth(g, L, D, H - 60, '#5a4a3c', NAVY, stripe=GOLD)
    # the ring: a floor 60 cm over the stage, its apron, four posts, three ropes a side
    rx, rw = -170.0, 300.0           # the ring's middle and its half size (x); half depth 230
    rd = 230.0
    g.box(TEAL, (rx, 0, H - 30), (2 * rw + 40, 2 * rd + 40, 60))
    g.box(CREAM, (rx, 0, H + 2), (2 * rw, 2 * rd, 4))
    pc = (RED, CYAN, WHITE, WHITE)
    for k, (sx, sy) in enumerate(((-1, -1), (1, 1), (1, -1), (-1, 1))):
        g.box(pc[k], (rx + sx * rw, sy * rd, H + 70), (16, 16, 140))
    for k in range(3):
        z = H + 44 + k * 40
        col = (RED, WHITE, CYAN)[k]
        for sy in (-1, 1):
            g.box(col, (rx, sy * rd, z), (2 * rw, 5, 5))
        for sx in (-1, 1):
            g.box(col, (rx + sx * rw, 0, z), (5, 2 * rd, 5))
    # the boxers: singlets and shorts in their corners' colours, gloves, always moving
    for i, (x, yaw, col, glove) in enumerate(((rx - 62, 90, RED, RED), (rx + 62, -90, CYAN, NAVY))):
        P.figure(sc, x, 10 - 20 * i, z=H + 4, yaw=yaw, legs='stride', wear='shorts', sleeves='none', top=col, bottom=(WHITE, GOLD)[i], shoes=col,
                 skin=P.SKINS[(4, 1)[i]], hair=('short', 'bald')[i], hair_col=P.HAIRS[0], build='stocky', s=1.15,
                 arm_l='hold', arm_r='hold', hold_l=hold_glove(glove), hold_r=hold_glove(glove), swing_l=30, swing_r=-30, axis_l='x', axis_r='x',
                 stride=10, bob=6, rate=2.0, phase=0.5 * i, detail=1)
    P.figure(sc, rx + 10, 130, z=H + 4, yaw=0, arm_l='out', arm_r='out', swing_l=14, swing_r=-14, rock=0, bob=3, rate=1.0, phase=0.3,
             **who(r, top=WHITE, bottom=LEATHER, sleeves='long', band=LEATHER, hair='short'))
    # the corner men, on the apron outside the ropes
    for i, (sx, col) in enumerate(((-1, RED), (1, CYAN))):
        P.figure(sc, rx + sx * (rw - 40), -rd - 34, z=H - 60, yaw=sx * -25, arm_r='up', hold_r=hold_towel(WHITE), swing_r=28, arm_l='face', bob=5, rate=1.4,
                 phase=0.4 * i, react=True, **far(r, top=col, bottom=LEATHER, hat='cap', hat_col=col, build='stocky'))
    # the judges' table and its bell, the row behind
    jx = 400.0
    g.box(CREAM, (jx, -150, H - 60 + 74), (300, 60, 8))
    g.box(MAG, (jx, -181, H - 60 + 40), (300, 3, 76))
    g.cyl(GOLD, (jx + 110, -150, H - 60 + 80), 20, 18, steps=8, top_r=8)
    for i, x in enumerate((jx - 80, jx + 20)):
        P.figure(sc, x, -100, z=H - 60, yaw=(-30, -20)[i], legs='sit', seat=46, arm_r=('hold', 'point')[i], arm_l='hold', swing_r=(0, 9)[i], axis_r='x',
                 rate=0.6, phase=0.5 * i, **far(r, top=(LILAC, YEL)[i], hat=('headset', 'sun')[i], hat_col=(LEATHER, WHITE)[i]))
        g.box(LEATHER, (x, -100, H - 60 + 22), (40, 40, 44))
    P.figure(sc, jx + 130, -90, z=H - 60, yaw=-30, arm_r='fwd', hold_r=P.hold_tool(STEEL, RED, 20), swing_r=30, axis_r='x', arm_l='hip', rate=0.5, phase=0.2,
             **far(r, top=ORANGE, hat='peak', hat_col=NAVY))
    g.box(PURP, (jx, 110, H - 60 + 30), (330, 80, 60))
    acts = (dict(arm_l='up', arm_r='up', swing_l=14, swing_r=-14, bob=9, rate=1.6), dict(arm_r='wave', swing_r=26, rate=1.3, bob=5),
            dict(arm_l='vee', arm_r='vee', swing_l=10, swing_r=-10, bob=11, rate=1.4), dict(arm_r='up', hold_r=P.hold_flag(col=CYAN, w=60), swing_r=22, rate=1.0, bob=6))
    for i in range(3):
        d = far(r)
        d.update(acts[i])
        P.figure(sc, jx - 100 + i * 100, 110, z=H, yaw=-28 + r.uniform(-8, 8), phase=r.random(), react=True, **d)
    N.flag(sc, L / 2.0 - 20, D / 2.0 - 30, H - 60, 470, MAG, patch=GOLD, w=190, h=120, phase=0.2)
    N.flag(sc, -L / 2.0 + 20, D / 2.0 - 30, H - 60, 470, CYAN, patch=WHITE, w=190, h=120, phase=0.6)


# ------------------------------------------------------------------------------------------ 10. the rim rescue
def rim_rescue(sc):
    """A rope-rescue drill on the canyon's rim (the first cast's rim column: 16.5 m of the cut's own rock, its cap over
    the rim): a tripod at the lip, the stretcher swinging on its rope under it, two hauling in step, a third steadying
    the line, the leader signalling down the canyon with a flag. Orange helmets, lime bands. Built at twice life: it is
    seen from the road far below, against the sky."""
    r = random.Random(310)
    g = sc.still()
    hx, hy = N.rim_column(g, r, 1900, 520)
    H = N.RIM_H
    S = 1.9
    # the tripod over the lip, its head 4.6 m up
    tx, ty = -300.0, -hy + 60.0
    head = (tx, ty - 40, H + 460)
    for (fx, fy) in ((tx - 200, ty + 40), (tx + 200, ty + 40), (tx, ty + 330)):
        tilt_box(g, YEL, (fx, fy, H), head, 14, 14)
    g.box(STEEL, (head[0], head[1], head[2] - 12), (40, 40, 30))
    # the stretcher on its rope, swinging along the face
    st = sc.part(P.motion(swing=7.0, pivot=head, axis=90.0, rate=0.3, bob=-30.0))
    st.box(CREAM, (tx, ty - 40, H + 330), (7, 7, 240))
    for sx in (-1, 1):
        tilt_box(st, CREAM, (tx, ty - 40, H + 215), (tx + sx * 150, ty - 40, H + 120), 6, 6)
    st.box(ORANGE, (tx, ty - 40, H + 100), (360, 90, 34))
    st.box(WHITE, (tx, ty - 40, H + 124), (300, 70, 16))
    st.sphere(P.SKINS[2], (tx - 120, ty - 40, H + 146), 30, steps=(3, 6))
    st.box(RED, (tx + 60, ty - 86, H + 100), (90, 4, 28))
    st.box(WHITE, (tx + 60, ty - 88.5, H + 100), (60, 2, 10))
    st.box(WHITE, (tx + 60, ty - 88.5, H + 100), (22, 2, 26))
    # the haul line back from the tripod's head, and the two on it
    hl = sc.part(P.motion(swing=2.0, pivot=head, axis=90.0, rate=0.5))
    tilt_box(hl, CREAM, head, (tx + 520, ty + 240, H + 220), 7, 7)
    team = dict(hat='hard', hat_col=ORANGE, band=LIME, sleeves='long', bottom=NAVY, shoes=LEATHER, detail=0)
    for i, (x, y) in enumerate(((tx + 420, ty + 250), (tx + 620, ty + 300))):
        P.figure(sc, x, y, z=H, yaw=-65, legs='stride', arm_l='push', arm_r='push', swing_l=20, swing_r=20, axis_l='x', axis_r='x', rock=0, rate=0.5,
                 phase=0.0, **N.person(r, S, top=(RED, CYAN)[i], **team))
    P.figure(sc, tx + 170, ty + 120, z=H, yaw=-40, arm_r='up', arm_l='fwd', swing_r=10, axis_r='x', rate=0.5, phase=0.25, **N.person(r, S, top=YEL, **team))
    # the leader with the flag, one with a radio, the rope bags
    P.figure(sc, 420, -hy + 150, z=H, yaw=10, arm_r='up', hold_r=P.hold_flag(col=ORANGE, w=120), swing_r=30, rate=0.8, arm_l='hip', phase=0.5,
             **N.person(r, S, top=WHITE, **team))
    P.figure(sc, 700, -hy + 190, z=H, yaw=-12, arm_r='face', arm_l='point', swing_l=10, axis_l='x', rock=0, rate=0.6, phase=0.1, **N.person(r, S, top=MAG, **team))
    P.figure(sc, -700, -hy + 170, z=H, yaw=14, arm_l='vee', arm_r='vee', swing_l=12, swing_r=-12, bob=12, rate=1.2, phase=0.7, react=True,
             **N.person(r, S, top=LIME, **dict(team, band=WHITE)))
    for (bx, by, col) in ((120, 120, RED), (200, 170, CYAN), (-560, 60, YEL)):
        g.box(col, (bx, by, H + 40), (90, 70, 80))
    N.flag(sc, hx - 60, hy - 60, H, 520, ORANGE, patch=WHITE, w=260, h=170, phase=0.3)


JOBS = [
    ('SM_P3B_Rodeo', rodeo),
    ('SM_P3B_SkyWatch', sky_watch),
    ('SM_P3B_SaltRecord', salt_record),
    ('SM_P3B_Revue', revue),
    ('SM_P3B_HoeDown', hoe_down),
    ('SM_P3B_Dig', dig),
    ('SM_P3B_PoolParty', pool_party),
    ('SM_P3B_CookOff', cook_off),
    ('SM_P3B_PrizeFight', prize_fight),
    ('SM_P3B_RimRescue', rim_rescue),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
