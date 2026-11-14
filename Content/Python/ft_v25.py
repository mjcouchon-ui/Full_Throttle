"""FULL THROTTLE 2.5: five new levels. Their props, bosses and projectiles (Geometry Script models in the house
style: flat-shaded boxes, cylinders and spheres in a bright palette, glow on the lit bits), their materials (lava,
tower facades) and their maps (track, sky, light, fog, the cel pass).

    L4 VOLCANO       Mount Kaldera: jagged black rock, lava, lightning; boss ENGINE 13, a fire engine that throws fire
    L5 THEME PARK    Coaster Cove at night: tents, carousel, coaster rails; boss MR. CHUCKLES, a clown car under bomb balloons
    L6 ICE MOUNTAIN  Frostbite Peak: a downhill ski run, half pipes; boss THE ABOMINABLE, a yeti
    L7 UNDER THE SEA The road to the sunken city of Nereida; boss THE GREAT WHITE, a shark car that fires fish
    L8 CITY CENTER   Midtown: towers, tight corners, straight through buildings; boss THE CHAIRMAN, a limo gunship

All original designs (no real park, resort, city or film). Units cm, X forward, Y right, Z up. A vehicle's origin is
the middle of its rear on the ground (exported centred); a prop's is the middle of its base, its front facing -X.

Run over the Unreal MCP connector:
    import ft_v25; ft_v25.materials(); ft_v25.export(); ft_v25.build_maps()
"""
import math, random
import unreal
import ft_art
import ft_export
import ft_mats
import ft_level
import ft_cel
from ft_car import wheel, BLACK, GLASS, CHROME, STEEL, TYRE, GOLD, PINK, CYAN, YEL, WHITE
from ft_desert import rod, tilt_box

AL = unreal.EditorAssetLibrary
MEL = unreal.MaterialEditingLibrary
save_mesh, model = ft_export.save_mesh, ft_export.model
hex_lin = ft_cel.hex_lin

RED = '#ff2d3c'
ORANGE = '#ff8a2a'
LAVA = '#ff6a1a'
LAVA_Y = '#ffd23f'
BASALT = '#2a2230'
BASALT_L = '#3e3244'
ASH = '#5a4e5e'
CHAR = '#1e1820'
NAVY = '#2b2350'
MAG = '#ff4fb8'
MINT = '#7fe3c4'
LAV = '#b9a4ff'
SNOW = '#f4f9ff'
ICE = '#bfe3ff'
ICE_D = '#7fb0e0'
PINE = '#2f6b52'
PINE_D = '#234f40'
WOOD = '#9a6a44'
WOOD_D = '#6a4c36'
SAND = '#d9c89a'
SEA = '#1a3f66'
KELP = '#3f8a4a'
KELP_D = '#2a6a3a'
CORAL = '#ff7a6a'
CORAL2 = '#ffb04f'
CONC = '#9a98a8'
CONC_D = '#6a6878'
LITW = ['#ffe28a', '#fff1c0', '#9ff0ff', '#ffc89a']


# =============================================================================================== projectiles
def bomb(m):
    """A cartoon bomb: black ball, a fuse with a spark."""
    m.sphere(BLACK, (0, 0, 0), 70, steps=(8, 12))
    m.cyl('#3a3846', (0, 0, 60), 22, 22, steps=10)
    rod(m, '#c9b48a', (0, 0, 82), (14, 0, 120), 4, steps=6)
    m.sphere(YEL, (16, 0, 124), 12, steps=(4, 6), glow=3.0)
    m.sphere(WHITE, (-30, 26, 30), 12, steps=(4, 6))


def snowball(m):
    m.sphere(SNOW, (0, 0, 0), 110, steps=(8, 12))
    m.sphere(ICE, (30, 40, 40), 50, steps=(5, 8))
    m.sphere(WHITE, (-40, -30, 50), 40, steps=(5, 8))


def fish(m, col='#8fb8e0', belly='#e8f4ff', fin=CYAN, L=160):
    """A flying fish, nose at +X: a fat body, a forked tail, big wing fins."""
    m.sphere(col, (0, 0, 0), L * 0.3, scale=(1.7, 0.75, 0.85), steps=(6, 10))
    m.sphere(belly, (4, 0, -L * 0.07), L * 0.24, scale=(1.6, 0.66, 0.6), steps=(5, 8))
    for s in (-1, 1):
        m.box(fin, (-L * 0.62, 0, s * L * 0.13), (L * 0.32, 6, L * 0.28), rot=(s * 30, 0, 0))
        m.box(fin, (L * 0.05, s * L * 0.38, 4), (L * 0.36, L * 0.5, 4), rot=(0, s * 20, s * 10), glow=0.3)
        m.sphere(BLACK, (L * 0.36, s * L * 0.17, L * 0.06), L * 0.05, steps=(4, 6))
    m.box(fin, (-L * 0.05, 0, L * 0.22), (L * 0.4, 5, L * 0.18), rot=(-12, 0, 0))


def swordfish(m):
    """A swordfish torpedo, nose (the sword) at +X."""
    fish(m, '#3f6fb0', '#cfe6ff', '#ff7ad9', 220)
    rod(m, '#cfe6ff', (60, 0, 6), (210, 0, 6), 6, steps=6)


def fireball(m):
    m.sphere(RED, (0, 0, 0), 80, steps=(6, 10), glow=1.4)
    m.sphere(ORANGE, (6, 0, 0), 66, steps=(6, 10), glow=2.2)
    m.sphere(YEL, (14, 0, 0), 44, steps=(5, 8), glow=3.2)
    for k in range(6):
        a = k * math.pi / 3
        m.sphere(ORANGE, (-70 - 20 * (k % 2), math.cos(a) * 40, math.sin(a) * 40), 34, steps=(4, 6), glow=2.0)


def big_tracer(m):
    m.box('#ffe28a', (0, 0, 0), (260, 20, 20), glow=4.0)
    m.box(ORANGE, (-60, 0, 0), (180, 28, 28), glow=2.0)


def bolt(m, h=4000, seed=5):
    """A lightning bolt from the ground up: a jagged chain of glowing segments with two forks."""
    r = random.Random(seed)
    p = (0.0, 0.0, 0.0)
    pts = [p]
    n = 12
    for k in range(1, n + 1):
        p = (p[0] + r.uniform(-140, 140), p[1] + r.uniform(-260, 260), h * k / n)
        pts.append(p)
    for a, b in zip(pts, pts[1:]):
        tilt_box(m, '#eaf4ff', a, b, 46, 46, glow=6.0)
        tilt_box(m, '#9fd0ff', a, b, 110, 20, glow=2.5)
    for k in (4, 8):
        q = pts[k]
        e = (q[0] + r.uniform(-200, 200), q[1] + r.uniform(400, 700) * (1 if k == 4 else -1), q[2] - r.uniform(300, 500))
        tilt_box(m, '#eaf4ff', q, e, 26, 26, glow=5.0)


# =============================================================================================== bosses
W_BOSS, L_BOSS = 64 * 9.14, 1250.0


def fire_truck(m):
    """ENGINE 13: a big red fire engine. Cab at the front, lockers down the sides, a ladder on top, and at the back a
    turret water cannon that throws fire, flames licking out of the nozzle. Chrome bumpers, a red-and-white light bar."""
    W, L = W_BOSS, L_BOSS
    hw = W / 2 - 16
    FR, FR_D = '#e8283c', '#b81c30'
    m.box(BLACK, (L * 0.5, 0, 70), (L * 0.96, hw * 2, 50))
    for s in (-1, 1):
        for x in (0.1, 0.24, 0.84):
            wheel(m, L * x, 52, 52, hw - 46, W / 2.0, s)
    # the cab
    m.side(FR, [(L * 0.76, 110), (L, 110), (L, 300), (L * 0.96, 360), (L * 0.78, 370)], -hw, hw)
    m.box(GLASS, (L * 0.985, 0, 300), (6, hw * 1.6, 90))
    for s in (-1, 1):
        m.box(GLASS, (L * 0.88, s * hw, 300), (150, 4, 80))
    m.box(CHROME, (L + 6, 0, 130), (12, hw * 2 + 10, 40))
    m.box(WHITE, (L + 4, 0, 180), (6, hw * 1.2, 30), glow=0.4)
    # the light bar: red and white
    for k in range(6):
        m.box(RED if k % 2 == 0 else WHITE, (L * 0.9, -hw * 0.7 + k * hw * 0.28, 382), (40, hw * 0.24, 22), glow=2.0)
    # the body: lockers with chrome stripes
    m.box(FR, (L * 0.38, 0, 220), (L * 0.76, hw * 2, 240))
    for s in (-1, 1):
        m.box(CHROME, (L * 0.38, s * (hw + 2), 150), (L * 0.74, 4, 14))
        m.box(WHITE, (L * 0.38, s * (hw + 3), 250), (L * 0.74, 4, 26))
        for k in range(5):
            m.box(FR_D, (L * 0.08 + k * L * 0.135, s * (hw + 2), 210), (L * 0.11, 3, 110))
        m.cyl(GOLD, (L * 0.68, s * (hw + 2), 260), 26, 6, rot=(0, 0, 90), steps=12, glow=0.4)
    # the ladder on top
    for s in (-1, 1):
        m.box(CHROME, (L * 0.45, s * 70, 362), (L * 0.7, 12, 18))
    for k in range(12):
        m.box(CHROME, (L * 0.12 + k * L * 0.058, 0, 362), (8, 140, 8))
    # the fire cannon at the back, pointing back at you
    m.cyl(STEEL, (L * 0.08, 0, 340), 70, 60, steps=14)
    m.cyl(CHROME, (L * 0.08, 0, 380), 22, 260, rot=(90, 0, 0), steps=10)
    m.sphere(ORANGE, (L * 0.08 - 280, 0, 380), 46, steps=(5, 8), glow=2.5)
    m.sphere(YEL, (L * 0.08 - 300, 0, 380), 30, steps=(4, 6), glow=3.5)
    for k in range(4):
        m.sphere(RED, (L * 0.08 - 330 - 26 * k, (k % 2 - 0.5) * 30, 380 + 10 * k), 28 - 4 * k, steps=(4, 6), glow=2.0)
    # hose reels on the back and the number board (two gold bars)
    for s in (-1, 1):
        m.cyl(WHITE, (-4, s * hw * 0.55, 190), 60, 30, rot=(0, 0, 90), steps=14)
        m.box(RED, (-4, s * (hw - 20), 90), (4, 40, 30), glow=1.4)
    m.box(GOLD, (-6, 0, 280), (4, 70, 90))
    m.box(BLACK, (-8, 0, 280), (2, 46, 66))


def clown_car(m):
    """MR. CHUCKLES: an overstuffed little car blown up to bus size, polka dots, a giant clown head through the
    roof (white face, red nose, blue hair tufts, tiny hat), a horn, and a bunch of bomb balloons tied above."""
    W, L = W_BOSS, L_BOSS
    hw = W / 2 - 20
    for s in (-1, 1):
        for x in (0.16, 0.82):
            wheel(m, L * x, 90, 90, hw - 70, W / 2.0, s)
    m.sphere('#ffe14d', (L * 0.5, 0, 260), 1, scale=(L * 0.5, hw, 170), steps=(8, 14))
    m.box('#ffe14d', (L * 0.5, 0, 170), (L * 0.95, hw * 2, 120))
    dots = [MAG, CYAN, '#7b3bd6', '#3dff6a', RED]
    r = random.Random(4)
    for k in range(22):
        a = r.uniform(0.15, 2.95)
        x = r.uniform(0.1, 0.9) * L
        m.sphere(dots[k % 5], (x, math.cos(a) * hw * 1.0, 260 + math.sin(a) * 160), 40, scale=(1, 1, 1), steps=(4, 6))
    # the head
    hz = 520
    m.sphere(WHITE, (L * 0.45, 0, hz), 200, steps=(8, 12))
    m.sphere(RED, (L * 0.45 - 200, 0, hz), 56, steps=(6, 8), glow=0.6)
    for s in (-1, 1):
        m.sphere('#2e6bff', (L * 0.45, s * 210, hz + 60), 90, steps=(6, 8))
        m.sphere(BLACK, (L * 0.45 - 170, s * 70, hz + 70), 22, steps=(4, 6))
        m.box(BLACK, (L * 0.45 - 178, s * 70, hz + 120), (4, 60, 10), rot=(0, 0, s * 15))
    m.box(RED, (L * 0.45 - 186, 0, hz - 90), (4, 180, 24))
    m.cyl(BLACK, (L * 0.45, 0, hz + 190), 70, 90, steps=12)
    m.cyl(BLACK, (L * 0.45, 0, hz + 190), 120, 12, steps=14)
    m.sphere(MAG, (L * 0.45, 0, hz + 290), 22, steps=(4, 6), glow=0.8)
    # the horn and the bumper
    m.cyl(GOLD, (L - 20, hw * 0.6, 420), 30, 160, rot=(90, 0, 0), steps=10, top_r=60)
    m.box(CHROME, (L + 6, 0, 120), (12, hw * 2, 40))
    m.box(RED, (-6, 0, 140), (8, hw * 1.6, 36), glow=1.0)
    # the balloons, tied to the back
    cols = [RED, '#2e6bff', '#3dff6a', MAG, '#ffe14d', '#7b3bd6', ORANGE, CYAN]
    for k, c in enumerate(cols):
        a = k * 2 * math.pi / len(cols)
        bx, by, bz = L * 0.15 + math.cos(a) * 140, math.sin(a) * 230, 1150 + (k % 3) * 120
        rod(m, '#e8e0f0', (L * 0.15, 0, 420), (bx, by, bz - 90), 3, steps=4)
        m.sphere(c, (bx, by, bz), 90, scale=(1, 1, 1.2), steps=(6, 10), glow=0.25)
        m.sphere(BLACK, (bx, by, bz - 130), 34, steps=(4, 6))


def yeti(m):
    """THE ABOMINABLE: a huge shaggy white yeti running downhill, leaning forward, fists the size of cars,
    an icy blue face with angry eyes and fangs, curled horns."""
    FUR, FUR_D, FACE = '#f2f6ff', '#cddcf2', '#7fa0d8'
    x0 = 600
    # legs mid-stride
    rod(m, FUR_D, (x0 - 60, -120, 360), (x0 + 120, -150, 40), 95, steps=10)
    rod(m, FUR_D, (x0 - 60, 120, 360), (x0 - 260, 140, 60), 95, steps=10)
    m.sphere('#9ab0d8', (x0 + 150, -150, 30), 80, scale=(1.4, 1, 0.5), steps=(5, 8))
    m.sphere('#9ab0d8', (x0 - 290, 140, 40), 80, scale=(1.4, 1, 0.5), steps=(5, 8))
    # the body: a big leaning shag
    m.sphere(FUR, (x0, 0, 560), 300, scale=(1.0, 1.0, 1.15), steps=(8, 12))
    for k in range(18):
        a = k * 2 * math.pi / 18
        m.sphere(FUR_D if k % 2 else FUR, (x0 + math.cos(a) * 240, math.sin(a) * 260, 440 + (k % 3) * 120), 110, steps=(5, 7))
    # arms swinging, big fists
    rod(m, FUR, (x0 + 40, -280, 720), (x0 + 300, -360, 420), 90, steps=10)
    rod(m, FUR, (x0 - 40, 280, 720), (x0 - 280, 360, 460), 90, steps=10)
    m.sphere(FUR_D, (x0 + 330, -370, 390), 120, steps=(6, 8))
    m.sphere(FUR_D, (x0 - 310, 370, 430), 120, steps=(6, 8))
    # the head and its face toward the road ahead (+X), so from behind you see the back and the horns
    hx, hz = x0 + 120, 940
    m.sphere(FUR, (hx, 0, hz), 190, steps=(8, 12))
    m.sphere(FACE, (hx + 120, 0, hz - 20), 130, scale=(0.6, 1, 1), steps=(6, 10))
    for s in (-1, 1):
        m.sphere(RED, (hx + 200, s * 50, hz + 20), 22, steps=(4, 6), glow=2.0)
        m.box(WHITE, (hx + 200, s * 40, hz - 80), (10, 18, 50))
        rod(m, '#d9c89a', (hx, s * 140, hz + 120), (hx - 80, s * 260, hz + 260), 30, steps=8)
        rod(m, '#d9c89a', (hx - 80, s * 260, hz + 260), (hx + 20, s * 300, hz + 330), 22, steps=8)
    # a red glare on the back of the head so you see it's angry even from behind
    m.box(FUR_D, (hx - 190, 0, hz + 40), (10, 160, 60))
    # the snow it kicks up
    for k in range(6):
        m.sphere(SNOW, (x0 - 420 - k * 60, (k % 3 - 1) * 160, 60 + k * 20), 90 - k * 8, steps=(5, 7))


def shark_car(m):
    """THE GREAT WHITE: a hot rod built into a great white shark. Grey body tapering to a tail fin at the back,
    a tall dorsal fin, gills, open jaws with teeth at the front, and two fish launchers aimed back at you."""
    W, L = W_BOSS, L_BOSS
    hw = W / 2 - 20
    GREY, GREY_D, BELLY = '#7f8fa8', '#5a6a84', '#e8eef8'
    for s in (-1, 1):
        for x in (0.2, 0.78):
            wheel(m, L * x, 62, 62, hw - 60, W / 2.0, s)
    m.sphere(GREY, (L * 0.52, 0, 250), 1, scale=(L * 0.5, hw, 170), steps=(10, 16))
    m.sphere(BELLY, (L * 0.55, 0, 170), 1, scale=(L * 0.42, hw * 0.9, 90), steps=(8, 12))
    # the dorsal fin and the tail
    m.side(GREY_D, [(L * 0.42, 380), (L * 0.62, 400), (L * 0.46, 720)], -16, 16)
    m.side(GREY_D, [(L * 0.06, 260), (-L * 0.08, 520), (-L * 0.02, 260), (-L * 0.1, 40)], -14, 14)
    for s in (-1, 1):
        m.box(GREY_D, (L * 0.6, s * (hw + 30), 190), (220, 120, 14), rot=(0, s * 15, s * 20))
        for k in range(3):
            m.box(GREY_D, (L * 0.74 + k * 40, s * (hw - 10), 280), (8, 6, 90))
        m.sphere(BLACK, (L * 0.92, s * 150, 320), 26, steps=(4, 6))
    # the jaws
    m.box('#3a1a2a', (L * 1.0, 0, 230), (60, hw * 1.4, 120))
    for k in range(9):
        y = -hw * 0.6 + k * hw * 0.15
        m.cyl(WHITE, (L * 1.02, y, 290), 14, 50, rot=(180, 0, 0), steps=6, top_r=1)
        m.cyl(WHITE, (L * 1.02, y + 20, 170), 14, 50, steps=6, top_r=1)
    # the fish launchers aimed back, glowing
    for s in (-1, 1):
        m.cyl(STEEL, (L * 0.2, s * 120, 400), 44, 260, rot=(90, 0, 0), steps=12)
        m.cyl(CYAN, (L * 0.2 - 262, s * 120, 400), 36, 6, rot=(90, 0, 0), steps=12, glow=1.6)
        m.box(RED, (-30, s * 120, 120), (4, 40, 26), glow=1.4)


def limo(m):
    """THE CHAIRMAN: a black executive stretch limo turned gunship. Long and low, gold trim and a hood ornament,
    tinted windows, four jet pods glowing underneath (it hovers), twin miniguns out of the sunroof and the boot,
    and a little flag on the wing."""
    W, L = W_BOSS * 0.92, L_BOSS
    hw = W / 2 - 10
    LB, LB2 = '#16141e', '#24202e'
    m.box(LB, (L * 0.5, 0, 130), (L, hw * 2, 110))
    m.side(LB2, [(L * 0.12, 185), (L * 0.18, 290), (L * 0.78, 290), (L * 0.84, 185)], -hw + 20, hw - 20)
    for s in (-1, 1):
        m.box('#2a3048', (L * 0.48, s * (hw - 18), 240), (L * 0.58, 4, 70))
        m.box(GOLD, (L * 0.5, s * (hw + 2), 150), (L * 0.98, 4, 10), glow=0.3)
        # jet pods
        for x in (0.15, 0.82):
            m.cyl('#3a3846', (L * x, s * (hw + 40), 60), 70, 160, rot=(0, 0, 90) if False else (90, 0, 0), steps=12)
            m.cyl(ORANGE, (L * x - 2, s * (hw + 40), 60), 50, 6, rot=(90, 0, 0), steps=12, glow=2.5)
            m.cyl(CYAN, (L * x + 80, s * (hw + 40), -10), 40, 20, steps=12, glow=2.0)
    m.box(CHROME, (L + 4, 0, 110), (10, hw * 2, 30))
    m.box(WHITE, (L + 2, 0, 150), (6, hw * 1.5, 20), glow=1.0)
    m.cyl(GOLD, (L * 0.97, 0, 186), 10, 50, steps=6)
    m.sphere(GOLD, (L * 0.97, 0, 240), 16, steps=(4, 6), glow=0.4)
    # the guns: a sunroof turret and a boot turret, barrels back at you
    for (x, z) in ((L * 0.5, 300), (L * 0.06, 190)):
        m.cyl(STEEL, (x, 0, z), 60, 50, steps=12)
        for y in (-26, 26):
            m.cyl(BLACK, (x - 30, y, z + 34), 10, 230, rot=(90, 0, 0), steps=8)
            m.sphere(ORANGE, (x - 266, y, z + 34), 14, steps=(4, 6), glow=2.5)
    for s in (-1, 1):
        m.box(RED, (-6, s * (hw - 30), 130), (4, 50, 22), glow=1.4)
    # the flag
    m.cyl(CHROME, (L * 0.92, hw - 20, 180), 3, 70, steps=4)
    m.box(GOLD, (L * 0.92 - 22, hw - 20, 240), (44, 2, 26))


# =============================================================================================== L4 volcano props
def torch(m, flip=1):
    """A fire basket on an iron post on the barrier, a flame in it."""
    m.cyl(CHAR, (0, 0, 0), 12, 520, steps=8)
    m.box(CHAR, (0, flip * 60, 520), (12, 120, 12))
    m.cyl(BASALT_L, (0, flip * 120, 480), 46, 50, steps=10, top_r=60)
    m.sphere(RED, (0, flip * 120, 545), 42, scale=(1, 1, 1.3), steps=(5, 8), glow=1.8)
    m.sphere(ORANGE, (0, flip * 120, 565), 32, scale=(1, 1, 1.5), steps=(5, 8), glow=2.6)
    m.sphere(YEL, (0, flip * 120, 580), 18, scale=(1, 1, 1.6), steps=(4, 6), glow=3.5)


def spike(m, seed=0, h=1400):
    """A cluster of jagged black rock spires with glowing cracks."""
    r = random.Random(seed)
    for k in range(5):
        x, y = r.uniform(-200, 200), r.uniform(-260, 260)
        hh = h * r.uniform(0.45, 1.0)
        tilt = r.uniform(-14, 14)
        m.cyl(BASALT if k % 2 else BASALT_L, (x, y, 0), r.uniform(110, 190), hh, rot=(tilt, r.uniform(0, 360), 0), steps=5, top_r=8)
        m.box(LAVA, (x - 60, y, hh * 0.3), (6, 14, hh * 0.35), rot=(tilt, 0, r.uniform(-20, 20)), glow=1.6)
    m.sphere(ASH, (0, 0, 0), 300, scale=(1.2, 1.4, 0.35), steps=(5, 8))


def boulder(m, seed=2):
    r = random.Random(seed)
    for k in range(4):
        p = (r.uniform(-120, 120), r.uniform(-160, 160), r.uniform(60, 140))
        rr = r.uniform(90, 170)
        m.sphere(BASALT_L, p, rr, scale=(1.2, 1, 0.8), steps=(5, 7))
        m.box(LAVA, (p[0] - rr * 0.9, p[1], p[2]), (6, rr * 0.9, 10), rot=(0, 0, r.uniform(-30, 30)), glow=1.8)


def dead_tree(m, seed=1):
    r = random.Random(seed)
    rod(m, CHAR, (0, 0, 0), (20, 10, 520), 30, steps=7)
    for k in range(5):
        z = 260 + k * 60
        a = r.uniform(0, 6.28)
        rod(m, CHAR, (8, 4, z), (math.cos(a) * 200, math.sin(a) * 200, z + r.uniform(80, 180)), 10, steps=5)
    m.sphere(LAVA, (10, 0, 280), 8, steps=(4, 4), glow=2.0)


def vent(m):
    """A fissure spitting a lava fountain, a plume of ash above it."""
    m.sphere(BASALT_L, (0, 0, 0), 260, scale=(1.4, 1.2, 0.45), steps=(5, 9))
    m.box(LAVA, (0, 0, 100), (40, 300, 20), glow=2.2)
    for k in range(6):
        m.cyl(ORANGE if k % 2 else LAVA, ((k - 3) * 20, (k % 3 - 1) * 60, 100), 40 - k * 4, 300 + k * 110, rot=((k - 3) * 6, 0, (k % 3 - 1) * 8), steps=6, top_r=4, glow=2.4)
    m.sphere(YEL, (0, 0, 160), 70, steps=(5, 7), glow=3.0)
    for k in range(4):
        m.sphere(ASH, (k * 60, k * 30, 1000 + k * 260), 200 + k * 60, steps=(5, 8))


def crag(m, w=4000, h=2600, seed=3):
    """A far ridge of black rock with a jagged skyline and a glowing lava seam."""
    r = random.Random(seed)
    for k in range(9):
        y = -w / 2 + k * w / 8.0
        hh = h * r.uniform(0.45, 1.0)
        m.cyl(BASALT if k % 2 else BASALT_L, (r.uniform(-200, 200), y, 0), r.uniform(500, 800), hh, steps=5, top_r=r.uniform(20, 120))
    m.box(LAVA, (-700, 0, h * 0.15), (20, w * 0.9, 60), glow=1.8)


def volcano(m, R=3000, H=3400):
    """The volcano itself, on the horizon: a banded cone, a glowing crater, lava rivers down its face, an ash plume."""
    m.cyl(BASALT, (0, 0, 0), R, H * 0.55, steps=14, top_r=R * 0.55)
    m.cyl(BASALT_L, (0, 0, H * 0.55), R * 0.55, H * 0.4, steps=14, top_r=R * 0.18)
    m.cyl(LAVA, (0, 0, H * 0.95), R * 0.19, 40, steps=14, glow=3.0)
    m.cyl(YEL, (0, 0, H * 0.96), R * 0.12, 40, steps=14, glow=4.0)
    for k in range(5):
        a = math.radians(-60 + k * 30)
        top = (-R * 0.18 * math.cos(a), R * 0.18 * math.sin(a), H * 0.95)
        bot = (-R * 0.95 * math.cos(a), R * 0.95 * math.sin(a), 80)
        tilt_box(m, LAVA, top, bot, 60, 140, glow=2.5)
    for k in range(6):
        m.sphere(ASH if k % 2 else '#43384a', (k * 150, k * 60, H + 600 + k * 520), 700 + k * 200, steps=(6, 10))


# =============================================================================================== L5 theme park props
def bulbs(m, flip=1):
    """A carnival lamp post: a striped pole, a curly arm, a ring of bulbs."""
    m.cyl(WHITE, (0, 0, 0), 12, 640, steps=8)
    for k in range(6):
        m.cyl(MAG, (0, 0, 60 + k * 100), 13, 30, steps=8)
    m.box(WHITE, (0, flip * 110, 640), (10, 220, 10))
    m.cyl(NAVY, (0, flip * 220, 600), 50, 30, steps=12)
    for k in range(8):
        a = k * math.pi / 4
        m.sphere(['#ffe28a', MAG, CYAN, '#3dff6a'][k % 4], (math.cos(a) * 60, flip * 220 + math.sin(a) * 60, 590), 14, steps=(4, 6), glow=2.2)
    m.sphere('#fff1c0', (0, flip * 220, 586), 30, steps=(5, 8), glow=1.6)


def tent(m, R=700, h=500):
    """A striped big-top tent with a pennant."""
    cols = [RED, WHITE]
    n = 16
    for k in range(n):
        a = k * 2 * math.pi / n
        m.box(cols[k % 2], (math.cos(a) * R, math.sin(a) * R, h / 2), (30, 2 * math.pi * R / n + 4, h), rot=(0, math.degrees(a), 0))
    m.cyl(WHITE, (0, 0, h), R + 20, R * 0.9, steps=n, top_r=20)
    for k in range(n // 2):
        a = k * 4 * math.pi / n
        tilt_box(m, RED, (math.cos(a) * (R + 10), math.sin(a) * (R + 10), h + 4), (0, 0, h + R * 0.9), 60, 8)
    m.cyl(GOLD, (0, 0, h + R * 0.9), 6, 200, steps=6)
    m.box(MAG, (0, 30, h + R * 0.9 + 170), (6, 60, 40), glow=0.5)
    m.box(NAVY, (-R - 10, 0, 150), (20, 220, 300))
    for k in range(10):
        a = k * 2 * math.pi / 10
        m.sphere('#ffe28a', (math.cos(a) * (R + 30), math.sin(a) * (R + 30), h + 10), 16, steps=(4, 6), glow=2.0)


def carousel(m, R=520):
    m.cyl('#3a2f6a', (0, 0, 0), R, 60, steps=20)
    m.cyl(GOLD, (0, 0, 0), 60, 520, steps=10)
    m.cyl(MAG, (0, 0, 470), R + 40, 60, steps=20)
    m.cyl(WHITE, (0, 0, 530), R + 40, 220, steps=20, top_r=40)
    for k in range(10):
        a = k * 2 * math.pi / 10
        x, y = math.cos(a) * R * 0.75, math.sin(a) * R * 0.75
        m.cyl(GOLD, (x, y, 60), 6, 410, steps=6)
        m.box([WHITE, '#ffe14d', CYAN, MAG][k % 4], (x, y, 200 + (k % 2) * 60), (110, 34, 70), rot=(0, math.degrees(a) + 90, 0))
        m.sphere('#ffe28a', (math.cos(a) * (R + 40), math.sin(a) * (R + 40), 500), 16, steps=(4, 6), glow=2.2)


def booth(m):
    m.box('#ff9ec7', (0, 0, 130), (220, 220, 260))
    m.box(WHITE, (0, 0, 270), (260, 260, 20))
    m.cyl(MAG, (0, 0, 280), 160, 90, steps=8, top_r=10)
    m.box('#fff1c0', (-112, 0, 160), (4, 140, 80), glow=1.2)
    m.box(YEL, (-116, 0, 230), (4, 180, 30), glow=1.0)


def balloons(m):
    m.box(WHITE, (0, 0, 60), (160, 100, 100))
    for k in range(9):
        a = k * 2 * math.pi / 9
        bx, by, bz = math.cos(a) * 80, math.sin(a) * 80, 330 + (k % 3) * 50
        rod(m, '#e8e0f0', (0, 0, 110), (bx, by, bz - 50), 2, steps=4)
        m.sphere([RED, '#2e6bff', '#3dff6a', MAG, '#ffe14d', CYAN][k % 6], (bx, by, bz), 46, scale=(1, 1, 1.2), steps=(5, 8), glow=0.35)


def topiary(m, seed=1):
    m.cyl(WOOD_D, (0, 0, 0), 20, 200, steps=6)
    m.sphere(PINE, (0, 0, 300), 150, steps=(6, 8))
    m.sphere('#3f8a62', (20, 30, 420), 90, steps=(5, 8))


def drop_tower(m, h=4200):
    m.box(WHITE, (0, 0, h / 2), (160, 160, h))
    for k in range(12):
        m.box([MAG, CYAN, '#ffe14d'][k % 3], (-82, 0, 200 + k * h / 12.5), (4, 170, 30), glow=1.6)
    m.cyl('#3a2f6a', (0, 0, h * 0.62), 300, 160, steps=16)
    for k in range(12):
        a = k * 2 * math.pi / 12
        m.box(RED if k % 2 else '#ffe14d', (math.cos(a) * 290, math.sin(a) * 290, h * 0.62 + 40), (60, 60, 80))
    m.sphere(RED, (0, 0, h + 60), 60, steps=(5, 8), glow=2.0)


def lattice(m, L=3600, seed=4):
    """A wooden roller coaster hill in the background: a white track over criss-cross supports."""
    r = random.Random(seed)
    pts = []
    n = 18
    for k in range(n + 1):
        y = -L / 2 + k * L / n
        z = 400 + 900 * (0.5 - 0.5 * math.cos(2 * math.pi * k / n)) + 300 * math.sin(4 * math.pi * k / n)
        pts.append((0, y, z))
    for a, b in zip(pts, pts[1:]):
        tilt_box(m, WHITE, a, b, 30, 120)
        tilt_box(m, MAG, (a[0], a[1], a[2] + 26), (b[0], b[1], b[2] + 26), 10, 130, glow=0.8)
    for p in pts[::2]:
        m.box(WOOD, (0, p[1], p[2] / 2), (24, 24, p[2]))
        tilt_box(m, WOOD_D, (0, p[1] - 100, 0), (0, p[1] + 100, p[2] * 0.7), 12, 12)


def park_skyline(m, width=12000, seed=9):
    """Coaster Cove on the horizon: the big top, the drop tower, a giant wheel, coaster loops and a ring of lights."""
    r = random.Random(seed)
    tent(m, 1600, 1200)
    old = m.base
    for (y, fn) in ((-4200, lambda: drop_tower(m, 6000)), (3800, lambda: lattice(m, 5200))):
        m.base = ft_art._xf((800, y, 0))
        fn()
    m.base = old
    ro = unreal.GeometryScriptRevolveOptions()
    for k, (y, rr) in enumerate(((-2200, 900), (2000, 1100), (5200, 700))):
        unreal.GeometryScript_Primitives.append_torus(m._mesh([MAG, CYAN, '#ffe14d'][k], 0.6), m.opts, m._t((1200, y, rr + 300), (0, 0, 90)), ro, rr, 40, 24, 6, unreal.GeometryScriptPrimitiveOriginMode.CENTER)
    m.box('#1e1a36', (-200, 0, 200), (300, width, 400))
    for k in range(50):
        m.box(r.choice(LITW + [MAG, CYAN]), (-352, -width / 2 + (k + 0.5) * width / 50.0, 120 + (k % 3) * 100), (4, width / 70.0, 40), glow=1.4)


# =============================================================================================== L6 glacier props
def gate(m, flip=1):
    """A slalom gate on the barrier: a striped pole and a flag panel out over the road."""
    col = RED if flip > 0 else '#2e6bff'
    m.cyl(WHITE, (0, 0, 0), 8, 420, steps=6)
    for k in range(4):
        m.cyl(col, (0, 0, 60 + k * 90), 9, 40, steps=6)
    m.box(col, (0, flip * 90, 330), (6, 160, 140))
    m.box(WHITE, (-4, flip * 90, 330), (2, 90, 40), glow=0.3)
    m.sphere('#fff1c0', (0, 0, 430), 16, steps=(4, 6), glow=1.6)


def pine(m, h=900, seed=0):
    r = random.Random(seed)
    m.cyl(WOOD_D, (0, 0, 0), 26, h * 0.25, steps=6)
    for k in range(4):
        z = h * (0.15 + k * 0.2)
        rr = h * (0.36 - k * 0.07)
        m.cyl(PINE if k % 2 else PINE_D, (0, 0, z), rr, h * 0.32, steps=8, top_r=rr * 0.2)
        m.cyl(SNOW, (0, 0, z + h * 0.18), rr * 0.62, h * 0.14, steps=8, top_r=rr * 0.12)
    m.cyl(SNOW, (0, 0, 0), h * 0.32, 20, steps=8)


def snow_rock(m, seed=1):
    r = random.Random(seed)
    for k in range(3):
        p = (r.uniform(-100, 100), r.uniform(-150, 150), r.uniform(60, 130))
        rr = r.uniform(90, 160)
        m.sphere('#6a7488', p, rr, scale=(1.2, 1, 0.8), steps=(5, 7))
        m.sphere(SNOW, (p[0], p[1], p[2] + rr * 0.5), rr * 0.85, scale=(1.2, 1, 0.4), steps=(5, 7))


def lift(m, h=1400):
    """A ski-lift pylon: a lattice tower, a crossarm with sheaves, two chairs hanging off the cable."""
    m.cyl('#9aa4b8', (0, 0, 0), 40, h, steps=8, top_r=26)
    m.box('#5a6478', (0, 0, h), (40, 700, 40))
    for s in (-1, 1):
        m.cyl(BLACK, (0, s * 330, h - 30), 30, 20, rot=(0, 0, 90), steps=10)
        rod(m, '#3a3846', (-600, s * 330, h - 40), (600, s * 330, h - 40), 4, steps=4)
        for x in (-380, 300):
            rod(m, '#5a6478', (x, s * 330, h - 40), (x, s * 330, h - 300), 5, steps=4)
            m.box(RED if s > 0 else '#2e6bff', (x, s * 330, h - 320), (60, 120, 14))
            m.box(RED if s > 0 else '#2e6bff', (x + 26, s * 330, h - 280), (8, 120, 70))
    m.sphere(RED, (0, 0, h + 40), 20, steps=(4, 6), glow=1.6)


def chalet(m):
    m.box(WOOD, (0, 0, 200), (700, 900, 400))
    m.side(SNOW, [(-420, 380), (420, 380), (0, 760)], -480, 480)
    m.side(WOOD_D, [(-440, 370), (440, 370), (0, 740)], -470, 470)
    for y in (-260, 0, 260):
        m.box('#ffe28a', (-352, y, 220), (4, 140, 150), glow=1.4)
        m.box(WOOD_D, (-354, y, 220), (4, 10, 160))
    m.cyl('#6a7488', (100, 250, 500), 50, 380, steps=8)
    m.sphere(SNOW, (100, 250, 880), 60, steps=(4, 6))


def ridge(m, w=4400, h=2800, seed=6):
    r = random.Random(seed)
    for k in range(7):
        y = -w / 2 + k * w / 6.0
        hh = h * r.uniform(0.5, 1.0)
        rr = r.uniform(700, 1000)
        m.cyl('#6a7488', (r.uniform(-200, 200), y, 0), rr, hh, steps=6, top_r=40)
        m.cyl(SNOW, (r.uniform(-200, 200), y, hh * 0.45), rr * 0.6, hh * 0.56, steps=6, top_r=30)


def peak(m, R=3000, H=4600):
    """The mountain on the horizon: rock below, a white cap, blue ice shadows, a cornice flag of snow blowing off."""
    m.cyl('#5a6478', (0, 0, 0), R, H * 0.5, steps=9, top_r=R * 0.5)
    m.cyl(SNOW, (0, 0, H * 0.45), R * 0.56, H * 0.55, steps=9, top_r=30)
    for k in range(5):
        a = math.radians(-70 + k * 35)
        tilt_box(m, ICE_D, (-R * 0.5 * math.cos(a), R * 0.5 * math.sin(a), H * 0.45), (-R * 0.05 * math.cos(a), R * 0.05 * math.sin(a), H * 0.95), 120, 300)
    for k in range(4):
        m.sphere(WHITE, (-200 + k * 80, 300 + k * 400, H + 100 + k * 60), 400 - k * 70, scale=(1, 1.6, 0.5), steps=(5, 8))


# =============================================================================================== L7 underwater props
def buoy(m, flip=1):
    """A seabed marker: a chain post with a glowing orb out over the road."""
    m.cyl(STEEL, (0, 0, 0), 12, 480, steps=8)
    m.box(STEEL, (0, flip * 80, 480), (10, 160, 10))
    m.sphere(CYAN, (0, flip * 160, 440), 50, steps=(6, 8), glow=2.4)
    m.sphere(WHITE, (0, flip * 160, 440), 26, steps=(4, 6), glow=3.0)
    for k in range(4):
        m.sphere('#bfe3ff', (6 * k, flip * 40, 520 + k * 70), 10 + k * 3, steps=(4, 4), glow=0.4)


def kelp(m, h=1100, seed=0):
    r = random.Random(seed)
    for s in range(4):
        x0, y0 = r.uniform(-120, 120), r.uniform(-160, 160)
        p = (x0, y0, 0)
        n = 7
        for k in range(1, n + 1):
            q = (x0 + 70 * math.sin(k * 0.9 + s), y0 + 50 * math.cos(k * 0.7 + s), h * k / n * r.uniform(0.8, 1.0))
            tilt_box(m, KELP if k % 2 else KELP_D, p, q, 40, 14)
            if k % 2 == 0:
                m.sphere('#c9b44a', q, 18, steps=(4, 6))
            p = q


def coral(m, seed=0):
    r = random.Random(seed)
    m.sphere('#ff9ec7', (0, 0, 60), 120, scale=(1, 1, 0.7), steps=(6, 9))
    for k in range(9):
        a = r.uniform(0, 6.28)
        b = (math.cos(a) * 80, math.sin(a) * 80, 60)
        t = (math.cos(a) * 200, math.sin(a) * 200, r.uniform(200, 420))
        rod(m, CORAL if k % 2 else CORAL2, b, t, 18, steps=6)
        m.sphere(CORAL2 if k % 2 else MAG, t, 26, steps=(4, 6), glow=0.5)


def coral_fan(m, seed=1):
    r = random.Random(seed)
    m.side('#b9a4ff', [(-20, 0), (20, 0), (240, 420), (0, 520), (-240, 420)], -8, 8)
    for k in range(5):
        m.sphere(CYAN, (r.uniform(-150, 150), 0, r.uniform(150, 460)), 16, steps=(4, 6), glow=1.4)
    m.sphere(SAND, (0, 0, 0), 110, scale=(1, 1, 0.4), steps=(5, 7))


def fishes(m, seed=3):
    """A school of small glowing fish hanging over the roadside."""
    r = random.Random(seed)
    cx, cz = 0, 700
    for k in range(26):
        a = k * 0.5
        x, y, z = cx + math.cos(a) * r.uniform(80, 400), math.sin(a) * r.uniform(200, 600), cz + r.uniform(-260, 260)
        m.sphere(r.choice(['#ffd23f', '#2ee6ff', '#ff7ad9', '#fff1c0']), (x, y, z), 26, scale=(1.8, 0.6, 0.8), steps=(4, 6), glow=0.8)


def wreck(m):
    """A sunken galleon on its side in the sand, a broken mast."""
    m.side(WOOD_D, [(-900, 100), (900, 100), (1100, 500), (-1000, 520)], -260, 260, rot=(0, 0, 25))
    m.side(WOOD, [(-860, 480), (1050, 480), (1060, 540), (-880, 560)], -240, 240, rot=(0, 0, 25))
    rod(m, WOOD_D, (0, 0, 400), (300, 600, 1300), 30, steps=6)
    m.box('#d9c89a', (150, 300, 900), (20, 500, 300), rot=(0, 0, 30))
    m.sphere(SAND, (0, 0, 0), 900, scale=(1.4, 0.8, 0.15), steps=(5, 9))
    m.sphere(CYAN, (800, 0, 400), 40, steps=(4, 6), glow=1.8)


def dome(m, R=900, seed=2):
    """A glass dome of the sunken city, lit inside: towers under a ribbed bubble."""
    r = random.Random(seed)
    for k in range(5):
        h = r.uniform(400, R * 0.9)
        m.box('#3a6a9a', (r.uniform(-R * 0.4, R * 0.4), r.uniform(-R * 0.4, R * 0.4), h / 2), (200, 200, h))
        m.box(r.choice(LITW), (r.uniform(-R * 0.4, R * 0.4) - 102, r.uniform(-R * 0.3, R * 0.3), h * 0.6), (4, 120, 40), glow=1.6)
    m.sphere('#9ff0ff', (0, 0, 0), R, scale=(1, 1, 0.9), steps=(8, 14), glow=0.25)
    for k in range(8):
        a = k * math.pi / 4
        m.box(CYAN, (math.cos(a) * R * 0.7, math.sin(a) * R * 0.7, R * 0.62), (20, 20, R * 0.6), rot=(0, math.degrees(a), 0), glow=1.2)
    m.cyl('#2a4a6a', (0, 0, 0), R + 40, 80, steps=16)


def sea_rock(m, w=4200, h=2200, seed=7):
    r = random.Random(seed)
    for k in range(7):
        y = -w / 2 + k * w / 6.0
        hh = h * r.uniform(0.4, 1.0)
        m.cyl('#24486e' if k % 2 else '#1a3a5a', (r.uniform(-200, 200), y, 0), r.uniform(500, 800), hh, steps=6, top_r=r.uniform(100, 300))
        m.sphere(CORAL if k % 2 else '#ff9ec7', (r.uniform(-500, -300), y, hh * 0.4), 90, steps=(4, 6), glow=0.4)


def sunken_city(m, width=12000, seed=13):
    """NEREIDA on the horizon: domes, spires with lit windows, a great central tower with a glowing crown."""
    r = random.Random(seed)
    old = m.base
    for k in range(9):
        y = -width / 2 + (k + 0.5) * width / 9.0
        m.base = ft_art._xf((r.uniform(0, 1200), y, 0), (0, 0, 0), (1.4, 1.4, 1.4))
        dome(m, r.uniform(700, 1100), seed + k)
    m.base = old
    m.cyl('#3a6a9a', (600, 0, 0), 420, 5000, steps=10, top_r=180)
    for k in range(6):
        m.cyl(CYAN, (600, 0, 600 + k * 700), 430 - k * 30, 40, steps=10, glow=1.4)
    m.sphere('#ff7ad9', (600, 0, 5200), 400, steps=(6, 10), glow=1.2)
    for (y, h) in ((-2600, 3200), (2400, 3600), (-4800, 2400), (4800, 2600)):
        m.cyl('#2a4a6a', (900, y, 0), 220, h, steps=8, top_r=40)
        m.sphere(CYAN, (900, y, h), 90, steps=(4, 6), glow=2.0)
    m.box('#14304a', (-200, 0, 200), (300, width, 400))
    for k in range(40):
        m.box(r.choice([CYAN, '#9ff0ff', '#ff7ad9', '#fff1c0']), (-352, -width / 2 + (k + 0.5) * width / 40.0, 120 + (k % 3) * 100), (4, width / 60.0, 40), glow=1.5)


# =============================================================================================== L8 midtown props
def street(m, flip=1):
    """A city street lamp: a dark green fluted pole, a long arm, a cobra head."""
    m.cyl('#24403a', (0, 0, 0), 16, 760, steps=8, top_r=11)
    m.box('#24403a', (0, flip * 150, 760), (12, 300, 12))
    m.box('#24403a', (0, flip * 300, 748), (70, 110, 26))
    m.box('#fff1c0', (0, flip * 300, 734), (60, 96, 4), glow=1.5)
    m.box(GOLD, (0, 0, 300), (4, 30, 40))


TOWER_STYLES = [
    # body, trim, window lit colour, width, depth, height, crown
    ('#3a3858', '#8a88a8', '#ffe28a', 1900, 1900, 6200, 'spire'),
    ('#4a4058', '#c9a46a', '#fff1c0', 2100, 1700, 4600, 'stepped'),
    ('#2a3a54', '#7fa0d8', '#9ff0ff', 1800, 1800, 8200, 'flat'),
    ('#54404a', '#e0b07a', '#ffc89a', 2200, 2000, 5400, 'stepped'),
]


def tower(m, style=0, seed=0):
    """A midtown tower: a slab with rows of lit windows on every face, set-backs, a crown (spire, steps or a flat
    top with a water tower)."""
    body, trim, lit, w, d, h, crown = TOWER_STYLES[style]
    r = random.Random(seed + style * 7)
    m.box(body, (0, 0, h / 2), (d, w, h))
    m.box(trim, (0, 0, 220), (d + 40, w + 40, 440))
    m.box('#fff1c0', (-d / 2 - 22, 0, 180), (4, w * 0.5, 200), glow=1.2)
    floors = int(h / 330)
    for f in range(2, floors):
        z = f * 330
        for (cx, cy, sx, sy) in ((-d / 2 - 2, 0, 4, w * 0.9), (d / 2 + 2, 0, 4, w * 0.9), (0, -w / 2 - 2, d * 0.9, 4), (0, w / 2 + 2, d * 0.9, 4)):
            if r.random() < 0.72:
                m.box(lit if r.random() < 0.7 else r.choice(LITW), (cx, cy, z), (sx, sy, 120), glow=0.9)
    if crown == 'spire':
        m.box(trim, (0, 0, h + 300), (d * 0.6, w * 0.6, 600))
        m.cyl(CHROME, (0, 0, h + 600), 120, 1400, steps=8, top_r=4)
        m.sphere(RED, (0, 0, h + 2000), 30, steps=(4, 6), glow=2.0)
    elif crown == 'stepped':
        for k in range(3):
            m.box(trim if k % 2 else body, (0, 0, h + 200 + k * 400), (d * (0.8 - k * 0.2), w * (0.8 - k * 0.2), 400))
        m.box(MAG, (0, 0, h + 1420), (d * 0.25, w * 0.25, 40), glow=1.4)
    else:
        m.cyl(WOOD, (d * 0.2, w * 0.2, h), 160, 300, steps=10)
        m.cyl(WOOD_D, (d * 0.2, w * 0.2, h + 300), 180, 140, steps=10, top_r=10)
        m.box(CYAN, (0, 0, h + 20), (d + 10, w + 10, 30), glow=1.0)


def hydrant(m):
    m.cyl(RED, (0, 0, 0), 22, 70, steps=8)
    m.sphere(RED, (0, 0, 70), 22, steps=(4, 8))
    for s in (-1, 1):
        m.cyl(CHROME, (0, 0, 46), 8, 34 * s if s > 0 else 34, rot=(0, 0, 90 * s), steps=6)


def newsstand(m):
    m.box('#2a5a3a', (0, 0, 150), (300, 400, 300))
    m.box('#fff1c0', (-152, 0, 170), (4, 320, 140), glow=1.2)
    m.box(RED, (-154, 0, 270), (4, 380, 40), glow=0.8)
    m.box('#2a5a3a', (-40, 0, 310), (420, 460, 20))


def city_skyline(m, width=24000, seed=17):
    r = random.Random(seed)
    y = -width / 2
    while y < width / 2:
        w = r.choice([900, 1200, 1500])
        h = r.choice([2600, 3400, 4400, 5600, 7000])
        m.box(r.choice(['#2a2840', '#33304e', '#24203a', '#3a3456']), (0, y + w / 2, h / 2), (900, w, h))
        for k in range(int(h / 500)):
            if r.random() < 0.7:
                m.box(r.choice(LITW), (-452, y + w / 2, 300 + k * 500), (4, w * 0.8, 120), glow=0.9)
        y += w + r.choice([0, 100, 300])


def spire(m):
    """THE CHAIRMAN's tower on the horizon: a tall art-deco needle with gold chevrons and a beacon."""
    m.box('#2a2840', (0, 0, 3500), (2400, 2400, 7000))
    for k in range(4):
        m.box('#3a3456', (0, 0, 7000 + k * 700 + 350), (1800 - k * 380, 1800 - k * 380, 700))
        for s in (-1, 1):
            m.box(GOLD, (-(900 - k * 190) - 4, s * (300 - k * 50), 7000 + k * 700 + 350), (6, 160, 500), rot=(s * 20, 0, 0), glow=1.2)
    m.cyl(CHROME, (0, 0, 9800), 160, 3200, steps=8, top_r=6)
    m.sphere(RED, (0, 0, 13000), 120, steps=(5, 8), glow=3.0)
    r = random.Random(3)
    for k in range(18):
        m.box(r.choice(LITW), (-1204, r.uniform(-1000, 1000), 400 + k * 360), (4, 900, 120), glow=0.9)


# =============================================================================================== export
def export():
    C = '/Game/Characters/'
    P = C + 'Projectiles/'
    save_mesh(model(bomb).meshes, P + 'SM_Bomb', 'prop', nanite=False)
    save_mesh(model(snowball).meshes, P + 'SM_Snowball', 'prop', nanite=False)
    save_mesh(model(lambda m: fish(m)).meshes, P + 'SM_Fish', 'prop', nanite=False)
    save_mesh(model(swordfish).meshes, P + 'SM_Swordfish', 'prop', nanite=False)
    save_mesh(model(fireball).meshes, P + 'SM_Fireball', 'prop', nanite=False)
    save_mesh(model(big_tracer).meshes, P + 'SM_BigTracer', 'prop', nanite=False)
    save_mesh(model(lambda m: bolt(m)).meshes, P + 'SM_Bolt', 'prop', nanite=False)
    save_mesh(model(fire_truck).meshes, C + 'Bosses/SM_FireTruck', 'car', (-625, 0, 0), collision='box')
    save_mesh(model(clown_car).meshes, C + 'Bosses/SM_ClownCar', 'car', (-625, 0, 0), collision='box')
    save_mesh(model(yeti).meshes, C + 'Bosses/SM_Yeti', 'car', (-625, 0, 0), collision='box')
    save_mesh(model(shark_car).meshes, C + 'Bosses/SM_SharkCar', 'car', (-625, 0, 0), collision='box')
    save_mesh(model(limo).meshes, C + 'Bosses/SM_Limo', 'car', (-625, 0, 0), collision='box')
    groups = {
        '/Game/Maps/Volcano/Props/': [
            ('SM_Torch', lambda m: torch(m)), ('SM_TorchR', lambda m: torch(m, -1)), ('SM_Spike', lambda m: spike(m, 0)),
            ('SM_Spike2', lambda m: spike(m, 5, 1800)), ('SM_Boulder', lambda m: boulder(m)), ('SM_DeadTree', lambda m: dead_tree(m)),
            ('SM_Vent', vent), ('SM_Crag', lambda m: crag(m)), ('SM_Volcano', lambda m: volcano(m))],
        '/Game/Maps/ThemePark/Props/': [
            ('SM_Bulbs', lambda m: bulbs(m)), ('SM_BulbsR', lambda m: bulbs(m, -1)), ('SM_Tent', lambda m: tent(m)),
            ('SM_Carousel', lambda m: carousel(m)), ('SM_Booth', booth), ('SM_Balloons', balloons), ('SM_Tree', lambda m: topiary(m)),
            ('SM_DropTower', lambda m: drop_tower(m)), ('SM_Lattice', lambda m: lattice(m)), ('SM_ParkSkyline', lambda m: park_skyline(m))],
        '/Game/Maps/Glacier/Props/': [
            ('SM_Gate', lambda m: gate(m)), ('SM_GateR', lambda m: gate(m, -1)), ('SM_Pine', lambda m: pine(m)),
            ('SM_Pine2', lambda m: pine(m, 1200, 3)), ('SM_SnowRock', lambda m: snow_rock(m)), ('SM_Lift', lambda m: lift(m)),
            ('SM_Chalet', chalet), ('SM_Ridge', lambda m: ridge(m)), ('SM_Peak', lambda m: peak(m))],
        '/Game/Maps/Abyss/Props/': [
            ('SM_Buoy', lambda m: buoy(m)), ('SM_BuoyR', lambda m: buoy(m, -1)), ('SM_Kelp', lambda m: kelp(m)),
            ('SM_Kelp2', lambda m: kelp(m, 1400, 4)), ('SM_Coral', lambda m: coral(m)), ('SM_Coral2', lambda m: coral_fan(m)),
            ('SM_Fishes', lambda m: fishes(m)), ('SM_Wreck', wreck), ('SM_Dome', lambda m: dome(m)), ('SM_SeaRock', lambda m: sea_rock(m)),
            ('SM_City', lambda m: sunken_city(m))],
        '/Game/Maps/Metro/Props/': [
            ('SM_Street', lambda m: street(m)), ('SM_StreetR', lambda m: street(m, -1)), ('SM_Tower1', lambda m: tower(m, 0, 1)),
            ('SM_Tower2', lambda m: tower(m, 1, 2)), ('SM_Tower3', lambda m: tower(m, 2, 3)), ('SM_Tower4', lambda m: tower(m, 3, 4)),
            ('SM_Hydrant', hydrant), ('SM_Newsstand', newsstand), ('SM_Skyline', lambda m: city_skyline(m)), ('SM_Spire', spire)],
    }
    n = 12
    for d, jobs in groups.items():
        for name, fn in jobs:
            save_mesh(model(fn).meshes, d + name, 'prop')
            n += 1
    return 'exported %d meshes' % n


# =============================================================================================== materials
LAVA_HLSL = """
float2 p = WP.xy / 900.0;
float t = Time * 0.35;
float n = sin(p.x * 1.7 + t) * sin(p.y * 1.3 - t * 0.8) + 0.5 * sin(p.x * 3.1 - p.y * 2.3 + t * 1.6);
n = n * 0.5 + 0.5;
float b = n > 0.72 ? 1.0 : (n > 0.45 ? 0.62 : 0.34);
float3 c = b > 0.9 ? float3(1.0, 0.85, 0.3) : (b > 0.5 ? float3(1.0, 0.42, 0.06) : float3(0.55, 0.08, 0.04));
return c * Gain;
"""


def materials():
    """MI_Lava (banded cel lava that crawls) and MI_Facade (the towers over the road: lit windows)."""
    m = ft_mats.new_material(ft_mats.CORE, 'M_Lava', shading=unreal.MaterialShadingModel.MSM_UNLIT)
    wp = ft_mats.expr(m, unreal.MaterialExpressionWorldPosition)
    tm = ft_mats.expr(m, unreal.MaterialExpressionTime, y=100)
    g = ft_mats.sparam(m, 'Gain', 6.0, y=200)
    e = ft_mats.custom(m, LAVA_HLSL, [('WP', wp, ''), ('Time', tm, ''), ('Gain', g, '')], desc='Lava')
    MEL.connect_material_property(e, '', unreal.MaterialProperty.MP_EMISSIVE_COLOR)
    ft_mats.finish(m)
    ft_mats.mi(m, ft_mats.CORE, 'MI_Lava', None, {'Gain': 6.0})
    ft_mats.mi(AL.load_asset(ft_mats.CORE + '/M_Windows'), ft_mats.CORE, 'MI_Facade', {'Color': (0.05, 0.045, 0.09, 1), 'Lit': (1.0, 0.78, 0.45, 1)}, {'Intensity': 5.0})
    return 'M_Lava, MI_Lava, MI_Facade'


# =============================================================================================== maps
# 2.6: level one's cel look at night. 2.5's Key 0.8 / Bright 1.0 left the moonlit side of everything dark and the
# headlights and explosions blew out to white (over EmisCut); Key 1.1 puts the moonlit side in the lit band (measured
# with Debug=1 in Volcano), Bright 1.3 makes the flat colour read like Bayshore's, EmisCut 6 keeps lit-up road banded.
NIGHT_CEL = dict(Key=1.1, Bright=1.3, Hot=2.6, EmisCut=6.0, HueKeep=0.6, ShadowLevel=0.5)


def night_fog(col, density, start=4000.0, opacity=0.6):
    """2.6: Bayshore's fog at night: no volumetric fog (2.0's 0.9-opacity volumetric haze greyed the bands and the ink)."""
    return dict(col=col, density=density, start=start, volumetric=False, opacity=opacity)
DAY_CEL = dict(Key=3.0, Bright=0.8, Hot=1.9, HueKeep=0.4)

LEVEL_SETUP = [
    # ft_level (map, first light and fog), then ft_cel (the 2.2+ look)
    (dict(name='Volcano', map='/Game/Maps/Volcano/L_Volcano',
          sky=((0.01, 0.002, 0.004), (0.12, 0.02, 0.01), (1.0, 0.18, 0.05)), moon=(-14.0, 30.0), moon_col=(1.0, 0.45, 0.25), moon_lux=4.0,
          fog=(0.12, 0.03, 0.02), fog_density=0.035, fog_height=-300.0, tint=(1.06, 0.97, 0.94), sky_gain=1.0, exposure=0.3),
     dict(name='Volcano', map='/Game/Maps/Volcano/L_Volcano', day=False, light_label='Moonlight',
          sky=dict(ft_cel.MOON_1X, Sky0=hex_lin('140608'), Sky1=hex_lin('4a1410'), Sky2=hex_lin('ff5a1f'), DiscCol=hex_lin('ffb070'), DiscCol2=hex_lin('ff5a1f'),
                   HaloCol=hex_lin('ff7a3a'), Stars=0.0, MoonDir=ft_cel.disc_dir(60.0, 10.0)),
          sun=dict(pitch=-40.0, yaw=70.0, lux=5.0, col=hex_lin('ffb08a')), sky_light=2.0,
          fog=night_fog((0.12, 0.03, 0.02), 0.02), road=dict(Grain=0.02, Wet=0.3, Puddles=0.0), cel=NIGHT_CEL)),
    (dict(name='ThemePark', map='/Game/Maps/ThemePark/L_ThemePark',
          sky=((0.004, 0.003, 0.02), (0.05, 0.015, 0.14), (0.85, 0.12, 0.42)), moon=(-35.0, 150.0), moon_col=(0.62, 0.7, 1.0), moon_lux=5.0,
          fog=(0.05, 0.015, 0.08), fog_density=0.03, fog_height=-400.0, tint=(1.0, 0.94, 1.06), sky_gain=1.0, exposure=0.4),
     dict(name='ThemePark', map='/Game/Maps/ThemePark/L_ThemePark', day=False, light_label='Moonlight',
          sky=dict(ft_cel.MOON_1X, Sky0=(0.004, 0.004, 0.03), Sky1=(0.06, 0.02, 0.16), Sky2=(0.7, 0.12, 0.4), Stars=1.0, MoonDir=ft_cel.disc_dir(170.0, 20.0)),
          sun=dict(pitch=-40.0, yaw=150.0, lux=5.0, col=hex_lin('c8c0ff')), sky_light=2.0,
          fog=night_fog((0.05, 0.015, 0.08), 0.018), road=dict(Grain=0.015, Wet=0.5, Puddles=0.0), cel=NIGHT_CEL)),
    (dict(name='Glacier', map='/Game/Maps/Glacier/L_Glacier',
          sky=((0.1, 0.25, 0.6), (0.5, 0.7, 0.9), (1.0, 0.95, 0.9)), moon=(-50.0, 160.0), moon_col=(1.0, 0.97, 0.92), moon_lux=9.0,
          fog=(0.8, 0.88, 1.0), fog_density=0.015, fog_height=-500.0, tint=(1.0, 1.0, 1.02), sky_gain=1.0, exposure=0.0),
     dict(name='Glacier', map='/Game/Maps/Glacier/L_Glacier', day=True, light_label='Sun',
          sky=dict(ft_cel.SUN_1X, Sky0=hex_lin('3f8fdf'), Sky1=hex_lin('bfe3ff'), Sky2=hex_lin('fff4f0'), MoonDir=ft_cel.disc_dir(30.0, 22.0)),
          sun=dict(pitch=-50.0, yaw=20.0, lux=9.0, col=hex_lin('fff8f0')), sky_light=1.3,
          fog=dict(col=hex_lin('e4f0ff'), density=0.014, start=5000.0, volumetric=False, opacity=0.6),
          exposure=-0.3, road=dict(RoadPastel=hex_lin('5a6888'), RoadLift=1.0, Grain=0.03, Wet=0.3, Puddles=0.0),
          wall=dict(Color=hex_lin('e8f4ff'), Rough=0.6, Metal=0.0), cel=DAY_CEL)),
    (dict(name='Abyss', map='/Game/Maps/Abyss/L_Abyss',
          sky=((0.002, 0.01, 0.03), (0.01, 0.06, 0.12), (0.05, 0.3, 0.45)), moon=(-80.0, 0.0), moon_col=(0.5, 0.85, 1.0), moon_lux=4.0,
          fog=(0.01, 0.08, 0.14), fog_density=0.06, fog_height=-300.0, tint=(0.94, 1.0, 1.06), sky_gain=1.0, exposure=0.4),
     dict(name='Abyss', map='/Game/Maps/Abyss/L_Abyss', day=False, light_label='Moonlight',
          sky=dict(ft_cel.MOON_1X, Sky0=hex_lin('010612'), Sky1=hex_lin('06305a'), Sky2=hex_lin('2a9ac0'), DiscCol=hex_lin('bff4ff'), DiscCol2=hex_lin('5fd0f0'),
                   HaloCol=hex_lin('5fd0f0'), Halo=0.5, Glow=0.6, Rays=0.25, Stripes=0.0, Stars=0.0, MoonDir=ft_cel.disc_dir(0.0, 70.0)),
          sun=dict(pitch=-75.0, yaw=20.0, lux=4.5, col=hex_lin('8fe0ff')), sky_light=2.4,
          fog=night_fog(hex_lin('0a4a6a'), 0.04, start=2500.0, opacity=0.7),
          road=dict(Grain=0.02, Wet=0.8, Puddles=0.0), cel=NIGHT_CEL)),
    (dict(name='Metro', map='/Game/Maps/Metro/L_Metro',
          sky=((0.004, 0.003, 0.012), (0.03, 0.02, 0.08), (1.0, 0.3, 0.15)), moon=(-40.0, 210.0), moon_col=(0.62, 0.7, 1.0), moon_lux=4.0,
          fog=(0.05, 0.03, 0.07), fog_density=0.03, fog_height=-400.0, tint=(1.02, 0.98, 1.04), sky_gain=1.0, exposure=0.4),
     dict(name='Metro', map='/Game/Maps/Metro/L_Metro', day=False, light_label='Moonlight',
          sky=dict(ft_cel.MOON_1X, Sky0=hex_lin('080614'), Sky1=hex_lin('2a1f4a'), Sky2=hex_lin('ff6a3d'), Stars=0.4, MoonDir=ft_cel.disc_dir(150.0, 24.0)),
          sun=dict(pitch=-42.0, yaw=140.0, lux=5.0, col=hex_lin('c0d0ff')), sky_light=2.0,
          fog=night_fog((0.05, 0.03, 0.07), 0.018), road=dict(Grain=0.015, Wet=0.6, Puddles=0.0), cel=NIGHT_CEL)),
]


def register():
    """Add the five levels to ft_level.LEVELS and ft_cel.LEVELS (indices 3..7) for this editor session."""
    while len(ft_level.LEVELS) > 3:
        ft_level.LEVELS.pop()
    while len(ft_cel.LEVELS) > 3:
        ft_cel.LEVELS.pop()
    for a, b in LEVEL_SETUP:
        ft_level.LEVELS.append(a)
        ft_cel.LEVELS.append(b)


def build_map(i):
    register()
    r1 = ft_level.build(i)
    r2 = ft_cel.apply_level(i)
    return r1 + ' | ' + r2


def build_maps(only=None):
    out = []
    for i in range(3, 8):
        if only and i not in only:
            continue
        out.append(build_map(i))
    return out
