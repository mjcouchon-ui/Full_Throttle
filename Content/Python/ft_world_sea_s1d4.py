"""Under the Sea, Stage 1, builder s1d4: "The salvage field and the approach" (5.0 to 7.94 km), the wreckers' town.
/Game/World/UnderTheSea/SM_S1D4_*. Run: import ft_world_sea_s1d4 as K; K.run()

Everything is made of ships: upturned hulls as roofs on walls of stacked containers, funnels as chimneys and as
houses, crane jibs as ridges, deck rails as balconies, patched sails as awnings. 7 buildings, 8 props, 1 hero.
Every mesh is an ft_people.Scene (one section, M_People); the hero is saved with the always-moving master."""
import math
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/UnderTheSea'
STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))

RUST, DRUST, HULL, HULLD = '#c8683a', '#7a3f2a', '#8f6450', '#5c3f3a'
CREAM, TEAL, SOOT = '#f4efe0', '#3f8a9a', '#3a3140'
LAMP = '#fff1c0'            # lit glass (steady)
SANDD = '#b9a77e'
ROCK, ROCKD, ROCKL = '#9a6a7e', '#6a4562', '#c79aa2'
SMOKE = '#cfc7d6'
LAV, PINK = '#b9a4ff', '#ff9ec7'
EMBER = '#ffb04f'


# ------------------------------------------------------------------------------------------ shared parts
def port(g, x, y, z, r=24.0, glow=0.8, n=(0.0, -1.0)):
    """A porthole on a wall: a cream ring and lit glass. n: the wall's outward direction in the ground plane."""
    a = (x - n[0] * 3, y - n[1] * 3, z)
    b = (x + n[0] * 7, y + n[1] * 7, z)
    c = (x + n[0] * 11, y + n[1] * 11, z)
    rod(g, CREAM, a, b, r + 8, steps=6)
    rod(g, LAMP, b, c, r, steps=6, glow=glow)


def cont(g, col, x, y, z, L=600.0, D=500.0, H=260.0, bars=True):
    """A shipping container standing on z, its long side to the front."""
    g.box(col, (x, y, z + H / 2.0), (L, D, H))
    if bars:
        for dx in (-L / 2.0 + 36, L / 2.0 - 36):
            g.box(SOOT, (x + dx, y - D / 2.0 - 2, z + H / 2.0), (12, 8, H - 24))


def rail(g, x0, x1, y, z, n=5, h=95.0, col=CREAM):
    """A deck rail along X at depth y, standing on z."""
    for k in range(n):
        x = x0 + (x1 - x0) * k / float(n - 1)
        g.box(col, (x, y, z + h / 2.0), (8, 8, h))
    g.box(col, ((x0 + x1) / 2.0, y, z + h), (abs(x1 - x0) + 12, 9, 9))


def funnel(g, x, y, z, r=52.0, h=330.0, col=SOOT, band=CREAM):
    """A ship's funnel as a chimney."""
    g.cyl(col, (x, y, z), r, h, steps=8, top_r=r * 0.9)
    g.cyl(band, (x, y, z + h * 0.72), r * 1.0, h * 0.11, steps=8)
    g.cyl(SOOT, (x, y, z + h), r * 0.98, 16, steps=8)


def bollard(g, x, y, z=0.0, h=90.0, r=16.0, col=SOOT, cap=CREAM):
    g.cyl(col, (x, y, z - 20), r, h + 20, steps=6, top_r=r * 0.8)
    g.cyl(cap, (x, y, z + h), r * 1.25, 12, steps=6)


def small_anchor(g, x, y, z, s=1.0, col=SOOT):
    """An anchor hanging or leaning: a shank, a stock, two arms. About 1 m at s = 1."""
    g.box(col, (x, y, z + 50 * s), (12 * s, 12 * s, 100 * s))
    g.box(col, (x, y, z + 92 * s), (56 * s, 10 * s, 10 * s))
    for k in (-1, 1):
        tilt_box(g, col, (x, y, z + 4 * s), (x + k * 38 * s, y, z + 34 * s), 12 * s, 12 * s)


# ------------------------------------------------------------------------------------------ buildings
def hull_house(sc):
    """BUILDING. An upturned hull as the roof on walls of four containers, two high: 15 x 6 m, 8.6 m to the funnel's top."""
    g = sc.still()
    g.box(DRUST, (0, 0, -95), (1300, 580, 210))                         # plinth of plates, 2 m of footing
    cont(g, RUST, -305, 0, 10)
    cont(g, TEAL, 305, 0, 10)
    cont(g, DRUST, -305, 0, 270)
    cont(g, RUST, 305, 0, 270)
    g.box(CREAM, (0, -254, 268), (1230, 10, 10))
    g.box(DRUST, (0, 0, 532), (1270, 560, 16))                           # the wall plate the hull sits on
    g.sphere(HULL, (0, 0, 536), 300, scale=(2.5, 1.06, 0.78), steps=(6, 10))
    g.box(DRUST, (0, 0, 770), (1040, 20, 26))                            # her keel is the ridge
    for x in (-420, -140, 140, 420):                                      # bilge blocks along the eaves
        g.box(CREAM, (x, -292, 548), (90, 26, 18))
    funnel(g, 400, 70, 640, 52, 250)
    g.box(SOOT, (305, -253, 118), (110, 8, 200))                         # the door, and a second one off the balcony
    g.box(CREAM, (305, -255, 224), (140, 8, 12))
    g.box(SOOT, (-305, -253, 368), (100, 8, 186))
    g.box(HULL, (-305, -300, 264), (540, 100, 12))                       # a deck-rail balcony
    rail(g, -565, -45, -345, 270, n=5)
    tilt_box(g, HULL, (-640, -330, -30), (-592, -338, 266), 54, 8)       # the ladder up to it
    for x, z in ((-440, 150), (-180, 150), (170, 410), (440, 410), (-130, 410)):
        port(g, x, -250, z)
    a = sc.part(P.motion(swing=4, pivot=(305, -252, 262), axis=0.0, rate=0.3, phase=0.2))   # a patched sail over the door
    tilt_box(a, CREAM, (305, -252, 262), (305, -410, 222), 270, 7)
    tilt_box(a, TEAL, (250, -285, 255), (250, -345, 240), 70, 11)
    tilt_box(a, DRUST, (372, -330, 244), (372, -380, 231), 50, 11)


def funnel_tower(sc):
    """BUILDING. A great ship's funnel stood up as a four-storey house: 8 m across, 13.4 m, a smoking stovepipe and a
    vane turning on top."""
    g = sc.still()
    g.cyl(DRUST, (0, 0, -160), 390, 210, steps=10)
    g.cyl(RUST, (0, 0, 50), 340, 1150, steps=12, top_r=280)

    def rad(z):
        return 340.0 - 60.0 * (z - 50.0) / 1150.0
    g.cyl(TEAL, (0, 0, 800), rad(800) + 5, 110, steps=12, top_r=rad(910) + 5)
    g.cyl(CREAM, (0, 0, 950), rad(950) + 5, 40, steps=12, top_r=rad(990) + 5)
    g.cyl(SOOT, (0, 0, 1200), 284, 110, steps=12)
    g.cyl(CREAM, (0, 0, 1310), 292, 22, steps=12)
    g.cyl(SOOT, (0, 0, 1332), 250, 90, steps=8, top_r=70)
    g.cyl(HULL, (0, 0, 540), 410, 14, steps=12)                           # a gallery all round at the second floor
    for k in range(8):
        a = math.radians(k * 45.0 + 22.5)
        g.box(CREAM, (400 * math.sin(a), -400 * math.cos(a), 600), (8, 8, 100))
    g.cyl(CREAM, (0, 0, 646), 408, 9, steps=12)
    for z, angs in ((330, (-34, 34)), (690, (-36, 0, 36)), (1080, (-30, 0, 30))):
        for ang in angs:
            a = math.radians(ang)
            n = (math.sin(a), -math.cos(a))
            port(g, rad(z) * n[0], rad(z) * n[1], z, n=n)
    g.box(SOOT, (0, -340, 150), (110, 20, 200))
    g.box(CREAM, (0, -344, 258), (150, 18, 14))
    tilt_box(g, HULL, (-250, -400, -30), (-215, -372, 545), 50, 8)        # ladder to the gallery
    g.cyl(SOOT, (110, 0, 1400), 26, 250, steps=6)                          # stovepipe
    g.cyl(HULL, (-120, 0, 1380), 6, 300, steps=5)                          # vane mast
    s = sc.part(P.motion(bob=-14, rate=0.16, phase=0.1))
    s.sphere(SMOKE, (120, 10, 1700), 46, scale=(1, 1, 0.8), steps=(3, 6))
    s2 = sc.part(P.motion(bob=-20, rate=0.13, phase=0.55))
    s2.sphere(SMOKE, (150, 30, 1810), 64, scale=(1, 1, 0.8), steps=(3, 6))
    v = sc.part(P.motion(base=(-120.0, 0.0), travel=(-1.0, 0.07)))         # the vane turns slowly
    v.box(CREAM, (-90, 0, 1690), (150, 7, 16))
    v.box(TEAL, (-185, 0, 1690), (44, 7, 50))
    a = sc.part(P.motion(swing=4, pivot=(0, -340, 300), axis=0.0, rate=0.27, phase=0.6))
    tilt_box(a, CREAM, (0, -338, 300), (0, -480, 262), 230, 7)
    tilt_box(a, TEAL, (-50, -380, 292), (-50, -440, 276), 60, 11)


def bridge_inn(sc):
    """BUILDING. A wheelhouse on four legs: wide lit windows, a radar bar turning on the roof, a picture sign (a ship's
    wheel) swinging under the deck. 10 x 6.4 m, 10.5 m."""
    g = sc.still()
    for sx in (-1, 1):
        for sy in (-1, 1):
            rod(g, RUST, (sx * 400, sy * 240, -160), (sx * 340, sy * 200, 425), 24, steps=6)
        tilt_box(g, DRUST, (sx * 395, -235, 20), (sx * 345, 205, 400), 14, 14)
    tilt_box(g, DRUST, (-390, -238, 30), (345, -204, 395), 14, 14)
    g.box(HULL, (0, 0, 432), (1000, 640, 36))
    rail(g, -490, 490, -310, 450, n=7)
    for sx in (-1, 1):
        g.box(CREAM, (sx * 494, -60, 545), (9, 500, 9))
    g.box(CREAM, (0, 50, 640), (760, 440, 380))
    g.box(TEAL, (0, 50, 470), (770, 450, 40))
    g.box(LAMP, (0, -172, 700), (700, 8, 150), glow=0.7)                   # the bridge windows
    for k in range(6):
        g.box(TEAL, (-350 + k * 140, -176, 700), (16, 10, 164))
    for sx in (-1, 1):
        g.box(LAMP, (sx * 382, 20, 700), (8, 300, 140), glow=0.7)
        g.box(TEAL, (sx * 385, 20, 700), (10, 16, 150))
    g.box(TEAL, (0, 50, 850), (860, 540, 40))
    g.box(CREAM, (0, 50, 880), (600, 300, 24))
    funnel(g, 260, 170, 870, 58, 210, col=RUST, band=TEAL)
    g.cyl(SOOT, (-120, 60, 890), 10, 150, steps=6)
    r = sc.part(P.motion(base=(-120.0, 60.0), travel=(-1.0, 0.1)))         # the radar bar
    r.box(CREAM, (-120, 60, 1050), (260, 16, 30))
    tilt_box(g, HULL, (-700, -150, -40), (-520, -150, 440), 90, 10)        # the stair
    tilt_box(g, CREAM, (-700, -200, 60), (-520, -200, 540), 8, 8)
    g.cyl(HULL, (-560, -150, -100), 14, 540, steps=5)
    rod(g, CREAM, (300, -316, 500), (300, -326, 500), 36, steps=8)         # a life ring on the rail
    rod(g, TEAL, (300, -326, 500), (300, -330, 500), 18, steps=8)
    s = sc.part(P.motion(swing=6, pivot=(-250, -330, 414), axis=0.0, rate=0.24, phase=0.3))   # the sign: a ship's wheel
    s.box(SOOT, (-250, -330, 330), (130, 8, 130))
    s.box(SOOT, (-250, -330, 404), (8, 8, 20))
    rod(s, CREAM, (-250, -334, 330), (-250, -340, 330), 44, steps=8)
    rod(s, SOOT, (-250, -340, 330), (-250, -343, 330), 30, steps=8)
    s.box(CREAM, (-250, -344, 330), (104, 5, 8))
    s.box(CREAM, (-250, -344, 330), (8, 5, 104))
    g.sphere(LAMP, (120, -250, 380), 20, steps=(3, 6), glow=1.2)           # a lamp under the deck
    g.box(SOOT, (120, -250, 407), (6, 6, 22))


def jib_workshop(sc):
    """BUILDING. The breakers' workshop: container walls two high, a roof of hull plates hung from a crane jib that is
    its ridge and runs on out over the yard, an anchor swinging from its hook. 16 x 6 m, 12.6 m at the jib's head."""
    g = sc.still()
    g.box(DRUST, (0, 0, -95), (960, 600, 210))
    g.box(RUST, (0, 0, 140), (900, 560, 260))
    cont(g, DRUST, -150, 0, 270, L=600, D=560)
    cont(g, TEAL, 300, 0, 270, L=300, D=560)
    g.box(SOOT, (-140, -282, 132), (380, 8, 236))                          # the roller door
    for k in range(4):
        g.box(CREAM, (-140, -286, 50 + k * 56), (380, 6, 8))
    g.box(CREAM, (-140, -288, 258), (430, 12, 16))
    for sy in (-1, 1):                                                     # two slopes of plate
        tilt_box(g, HULL, (0, sy * 310, 520), (0, 0, 700), 980, 16)
    for sx in (-1, 1):
        g.box(DRUST, (sx * 474, 0, 610), (12, 30, 170))
        rod(g, CREAM, (sx * 470, 0, 600), (sx * 482, 0, 600), 44, steps=6)
        rod(g, SOOT, (sx * 482, 0, 600), (sx * 486, 0, 600), 30, steps=6)
    # the jib: two chords and a lattice, from the back gable up and out over the yard at +X
    a0, a1 = (-520.0, 0.0, 700.0), (1040.0, 0.0, 1220.0)
    b0, b1 = (-520.0, 0.0, 790.0), (1040.0, 0.0, 1260.0)
    tilt_box(g, RUST, a0, a1, 26, 26)
    tilt_box(g, RUST, b0, b1, 26, 26)
    for k in range(7):
        t0, t1 = k / 7.0, (k + 1) / 7.0
        p = (a0[0] + (a1[0] - a0[0]) * t0, 0, a0[2] + (a1[2] - a0[2]) * t0)
        q = (b0[0] + (b1[0] - b0[0]) * t1, 0, b0[2] + (b1[2] - b0[2]) * t1)
        tilt_box(g, DRUST, p, q, 12, 12)
    g.box(SOOT, (-470, 0, 760), (130, 150, 170))                           # the jib's foot and its winch house
    tilt_box(g, SOOT, (-470, 0, 845), (-300, 0, 1150), 20, 20)             # the king post and its stay
    rod(g, HULL, (-300, 0, 1150), (1030, 0, 1262), 5, steps=5)
    h = sc.part(P.motion(swing=4, pivot=(1030, 0, 1225), axis=90.0, rate=0.2, phase=0.4))
    rod(h, SOOT, (1030, 0, 1225), (1030, 0, 640), 5, steps=5)
    small_anchor(h, 1030, 0, 520, s=1.3, col=SOOT)
    funnel(g, -330, 180, 560, 40, 250)
    for k in range(3):                                                     # salvaged pipes by the door
        rod(g, TEAL if k != 1 else CREAM, (130, -330 - (k % 2) * 4, 22 + k * 30), (430, -330 - (k % 2) * 4, 22 + k * 30), 17, steps=6)
    port(g, 300, -280, 400)
    port(g, -300, -280, 400)
    port(g, 0, -280, 400)


def lifeboat_row(sc):
    """BUILDING. Lifeboat row houses: three boats on a rack, the middle one a storey higher, a door and portholes in
    each side, davits, a plank walk, a washing line that sways. 20 x 4 m, 7.2 m."""
    g = sc.still()
    boats = ((-650, 190, CREAM, TEAL), (0, 400, TEAL, CREAM), (650, 190, CREAM, HULL))
    for bx, bz, col, top in boats:
        for sx in (-1, 1):
            for sy in (-1, 1):
                g.box(HULL, (bx + sx * 200, sy * 90, bz / 2.0 - 75), (18, 18, bz + 150))
        g.box(HULLD, (bx, 0, bz - 8), (520, 250, 16))
        cz = bz + 96
        g.sphere(col, (bx, 0, cz), 150, scale=(2.0, 0.82, 0.7), steps=(5, 10))
        g.box(top, (bx, 0, cz + 92), (400, 190, 50))                       # the canvas cover, now a roof
        g.box(DRUST, (bx, 0, cz + 4), (604, 30, 14))                       # stem to stern: her rubbing strake
        g.box(SOOT, (bx - 30, -122, cz - 6), (60, 10, 120))
        port(g, bx + 90, -116, cz + 10, r=17)
        port(g, bx - 150, -108, cz + 10, r=17)
        g.cyl(SOOT, (bx + 130, 30, cz + 110), 14, 110, steps=5)
        for sx in (-1, 1):                                                 # davits
            tilt_box(g, SOOT, (bx + sx * 260, 110, bz), (bx + sx * 260, 80, cz + 230), 16, 16)
            tilt_box(g, SOOT, (bx + sx * 260, 80, cz + 230), (bx + sx * 240, -70, cz + 260), 16, 16)
        tilt_box(g, HULL, (bx - 30, -280, bz - 200), (bx - 30, -140, bz), 60, 8)   # steps to the door
    g.box(HULL, (0, -165, 178), (1960, 76, 12))
    rail(g, -960, 960, -200, 184, n=9, h=80)
    g.box(HULL, (0, -150, 390), (560, 60, 12))
    for x in (-720, -240, 240, 720):                                       # lamps on the rail posts
        g.sphere(LAMP, (x, -200, 280), 15, steps=(3, 5), glow=1.0)
    w = sc.part(P.motion(swing=7, pivot=(447, -65, 600), axis=0.0, rate=0.35, phase=0.15))   # washing, davit to davit
    rod(w, HULL, (255, -60, 650), (640, -70, 550), 3, steps=4)
    for k, col in enumerate((CREAM, TEAL, CREAM, DRUST)):
        x = 310 + k * 85
        w.box(col, (x, -66, 650 - (x - 255) / 385.0 * 100 - 36), (60, 5, 62))


def forge_rock(sc):
    """BUILDING (the approach). The chimney-rock forge: a boat-roofed smithy clamped with iron bands round a smoking
    vent rock, the forge mouth lit, a bellows beam rocking. 10 x 9 m, 13 m of rock, smoke to 16 m."""
    g = sc.still()
    g.cyl(ROCKD, (0, 120, -220), 430, 1320, steps=7, top_r=150)
    g.cyl(ROCK, (270, 240, -220), 270, 860, steps=6, top_r=70)
    g.cyl(ROCKL, (-300, 260, -220), 200, 520, steps=5, top_r=60)
    g.cyl(SOOT, (0, 120, 1090), 162, 50, steps=7, top_r=152)

    def rad(z):
        return 430.0 - 280.0 * (z + 220.0) / 1320.0
    for z in (640, 900):
        g.cyl(TEAL, (0, 120, z), rad(z) + 10, 34, steps=7)
    g.box(DRUST, (0, -250, -90), (800, 400, 200))
    g.box(RUST, (0, -250, 220), (760, 360, 420))
    g.box(DRUST, (0, -250, 436), (800, 400, 16))
    g.sphere(HULL, (0, -250, 440), 200, scale=(2.0, 1.02, 0.72), steps=(5, 10))
    g.box(DRUST, (0, -250, 588), (560, 18, 22))
    for sx in (-1, 1):                                                     # stays from the bands to the smithy
        tilt_box(g, TEAL, (sx * 240, 60, 650), (sx * 340, -200, 440), 14, 14)
    g.box(SOOT, (-170, -432, 150), (210, 8, 250))                          # the forge mouth
    g.box(EMBER, (-170, -436, 105), (150, 6, 130), glow=1.2)
    g.box(CREAM, (-170, -436, 284), (250, 10, 16))
    g.box(SOOT, (230, -432, 300), (110, 8, 110))                           # the sign: a hammer
    g.box(CREAM, (230, -438, 290), (10, 5, 76))
    g.box(CREAM, (230, -438, 330), (50, 5, 22))
    port(g, 60, -430, 300)
    g.cyl(SOOT, (250, -160, 500), 46, 520, steps=7, top_r=40)              # a funnel strapped to the rock
    g.cyl(CREAM, (250, -160, 880), 48, 40, steps=7)
    g.box(SOOT, (-420, -520, 44), (70, 40, 20))                            # the anvil on its block
    g.box(HULL, (-420, -520, 0), (50, 50, 70))
    g.box(SOOT, (240, -500, 30), (220, 80, 80))                            # the quench trough
    g.box(TEAL, (240, -500, 68), (196, 60, 8))
    g.cyl(HULL, (60, -520, -60), 12, 300, steps=5)                          # the bellows post and beam
    b = sc.part(P.motion(swing=10, pivot=(60, -520, 236), axis=90.0, rate=0.45, phase=0.2))
    b.box(HULL, (60, -520, 236), (360, 12, 14))
    b.box(SOOT, (-110, -520, 200), (8, 8, 60))
    for k, (dx, dz, r, ph) in enumerate(((0, 1260, 70, 0.0), (40, 1420, 96, 0.35), (-30, 1570, 60, 0.7))):
        s = sc.part(P.motion(bob=-16, rate=0.12 + 0.02 * k, phase=ph))
        s.sphere(SMOKE if k != 1 else SOOT, (dx, 120 + dx * 0.5, dz), r, scale=(1, 1, 0.78), steps=(3, 6))


def tenement(sc):
    """BUILDING. A tenement of containers four high, staggered: deck-rail balconies, sail awnings, planters, a washing
    line, a dinghy for a roof, a vane. 13 x 5 m, 12.6 m."""
    g = sc.still()
    g.box(DRUST, (0, 0, -95), (1300, 540, 210))
    cont(g, RUST, -310, 0, 10)
    cont(g, DRUST, 310, 0, 10)
    cont(g, TEAL, -210, 0, 270)
    cont(g, RUST, 400, 0, 270, bars=False)
    cont(g, DRUST, -330, 0, 530)
    cont(g, HULL, 280, 0, 530)
    cont(g, RUST, -80, 0, 790)
    g.box(CREAM, (40, -254, 268), (1260, 10, 10))
    g.box(CREAM, (-20, -254, 528), (1240, 10, 10))
    g.box(SOOT, (-310, -253, 118), (110, 8, 200))
    g.box(SOOT, (310, -253, 118), (110, 8, 200))
    for bx, bz, w in ((400, 270, 560), (-330, 530, 560), (-80, 790, 520)):
        g.box(HULL, (bx, -300, bz - 4), (w, 100, 12))
        rail(g, bx - w / 2.0 + 10, bx + w / 2.0 - 10, -345, bz, n=5)
        g.box(SOOT, (bx - 150, -253, bz + 100), (90, 8, 180))
    for x, z in ((-330, 410), (-90, 410), (580, 410), (420, 410), (-200, 670), (160, 670), (400, 670), (40, 930), (150, 930)):
        port(g, x, -250, z, r=22)
    tilt_box(g, HULL, (120, -330, 266), (150, -338, 526), 50, 8)           # ladders from deck to deck
    tilt_box(g, HULL, (-560, -330, 526), (-330, -338, 786), 50, 8)
    g.box(SOOT, (560, -322, 300), (90, 44, 40))                            # planters on the rails
    g.box(SOOT, (-520, -322, 560), (90, 44, 40))
    g.sphere(HULL, (10, 0, 1056), 130, scale=(1.9, 0.9, 0.62), steps=(4, 8))   # a dinghy keel-up on the roof
    g.box(DRUST, (10, 0, 1138), (380, 14, 16))
    funnel(g, -280, 80, 1050, 40, 190)
    g.cyl(HULL, (560, 150, 790), 6, 330, steps=5)
    v = sc.part(P.motion(base=(560.0, 150.0), travel=(-1.0, 0.06)))
    v.box(CREAM, (590, 150, 1126), (140, 7, 16))
    v.box(TEAL, (500, 150, 1126), (40, 7, 46))
    p = sc.part(P.motion(swing=5, pivot=(560, -322, 320), axis=0.0, rate=0.4, phase=0.3))
    p.sphere(LAV, (545, -322, 345), 26, steps=(3, 5))
    p.sphere(PINK, (585, -322, 340), 20, steps=(3, 5))
    p.sphere(PINK, (-535, -322, 603), 24, steps=(3, 5))
    p.sphere(LAV, (-500, -322, 598), 19, steps=(3, 5))
    a = sc.part(P.motion(swing=4, pivot=(400, -252, 500), axis=0.0, rate=0.28, phase=0.5))
    tilt_box(a, CREAM, (400, -252, 500), (400, -400, 462), 420, 7)
    tilt_box(a, DRUST, (300, -290, 494), (300, -350, 479), 70, 11)
    a2 = sc.part(P.motion(swing=4, pivot=(-80, -252, 1040), axis=0.0, rate=0.31, phase=0.1))
    tilt_box(a2, CREAM, (-80, -252, 1040), (-80, -400, 1000), 400, 7)
    tilt_box(a2, TEAL, (-10, -300, 1031), (-10, -360, 1015), 80, 11)
    w = sc.part(P.motion(swing=7, pivot=(250, -345, 870), axis=0.0, rate=0.36, phase=0.7))   # the washing line
    rod(w, HULL, (180, -345, 880), (580, -345, 800), 3, steps=4)
    for k, col in enumerate((CREAM, TEAL, CREAM)):
        w.box(col, (270 + k * 100, -345, 830 - k * 20), (60, 5, 64))
    g.cyl(HULL, (585, -345, 526), 6, 290, steps=5)


# ------------------------------------------------------------------------------------------ props
def chain_fence(sc):
    """PROP, LOW (1.05 m). An anchor-chain fence: four bollards, the chain sagging between them, a small anchor
    leaning on the last. 9.6 m long."""
    g = sc.still()
    xs = (-450, -150, 150, 450)
    for x in xs:
        bollard(g, x, 0, h=84)
    for k in range(3):
        x0, x1 = xs[k], xs[k + 1]
        xm = (x0 + x1) / 2.0
        for j in range(4):                                                 # eight links a span, alternate ways up
            t0, t1 = j / 4.0, (j + 1) / 4.0
            za, zb = 70 - 34 * math.sin(math.pi * t0 / 1.0) * 0.5 - 20 * (1 - abs(1 - 2 * t0)), 70 - 34 * math.sin(math.pi * t1) * 0.5 - 20 * (1 - abs(1 - 2 * t1))
            tilt_box(g, HULLD if j % 2 else HULL, (x0 + (x1 - x0) * t0, 0, za), (x0 + (x1 - x0) * t1, 0, zb), 10 if j % 2 else 16, 16 if j % 2 else 10)
        g.box(CREAM, (xm, 0, 30), (16, 16, 16))                            # a mooring float on the low link
    tilt_box(g, SOOT, (455, -30, 0), (480, -22, 84), 12, 12)               # the anchor, leaning
    g.box(SOOT, (478, -22, 78), (60, 10, 10))
    for k in (-1, 1):
        tilt_box(g, SOOT, (455, -30, 4), (455 + k * 34, -34, 30), 11, 11)


def porthole_lamp(sc):
    """PROP, TALL (4.7 m). A porthole lamp post: a pipe, a bracket, a porthole for a lantern (steady light), two
    scraps of sail on the arm."""
    g = sc.still()
    g.cyl(SOOT, (0, 0, -100), 36, 130, steps=6, top_r=24)
    g.cyl(TEAL, (0, 0, 30), 11, 380, steps=6)
    for z in (130, 270):
        g.cyl(CREAM, (0, 0, z), 15, 12, steps=6)
    tilt_box(g, TEAL, (0, 0, 395), (0, -70, 425), 14, 14)
    g.box(TEAL, (0, 30, 360), (150, 10, 10))
    rod(g, CREAM, (0, -40, 420), (0, -96, 420), 46, steps=8)               # the porthole
    rod(g, LAMP, (0, -96, 420), (0, -101, 420), 36, steps=8, glow=1.5)
    rod(g, LAMP, (0, -35, 420), (0, -40, 420), 36, steps=8, glow=1.5)
    for k in range(4):
        a = math.radians(45 + k * 90)
        g.box(SOOT, (50 * math.cos(a), -68, 420 + 50 * math.sin(a)), (10, 60, 10))   # dogs round the rim
    g.cyl(SOOT, (0, -68, 462), 20, 14, steps=6, top_r=6)
    f = sc.part(P.motion(swing=9, pivot=(0, 30, 356), axis=0.0, rate=0.5, phase=0.3))
    f.box(CREAM, (-52, 30, 326), (30, 5, 56))
    f.box(DRUST, (56, 30, 332), (26, 5, 44))


def prop_monument(sc):
    """PROP, TALL (5.4 m). A ship's propeller set up as a monument: three verdigris blades on a cream boss, on a
    plinth of plates between chain posts, a line of pennants (no words) swaying to a post."""
    g = sc.still()
    g.box(DRUST, (0, 0, -30), (380, 300, 180))
    g.box(HULL, (0, 0, 80), (300, 230, 40))
    g.box(CREAM, (0, -118, 30), (150, 8, 50))                              # the plate: a wave, no words
    tilt_box(g, TEAL, (-55, -124, 22), (-18, -124, 40), 6, 9)
    tilt_box(g, TEAL, (-18, -124, 40), (18, -124, 22), 6, 9)
    tilt_box(g, TEAL, (18, -124, 22), (55, -124, 40), 6, 9)
    for sx in (-1, 1):
        tilt_box(g, SOOT, (sx * 110, 30, 100), (0, 30, 330), 34, 34)
    hub = (0.0, 0.0, 330.0)
    rod(g, CREAM, (0, 60, 330), (0, -70, 330), 44, steps=8)
    g.sphere(CREAM, (0, -76, 330), 36, steps=(3, 8))
    for k in range(3):
        a = math.radians(30 + k * 120)                                       # fix round: no blade straight up (it could read as a fin): a Y, two up and one down
        c, s = math.cos(a), math.sin(a)
        tilt_box(g, TEAL, (hub[0] + 30 * c, -10, hub[2] + 30 * s), (hub[0] + 200 * c, -10, hub[2] + 200 * s), 16, 92)
        tilt_box(g, TEAL, (hub[0] + 90 * c, -14, hub[2] + 90 * s), (hub[0] + 180 * c, -14, hub[2] + 180 * s), 14, 132)
    for sx in (-1, 1):
        for sy in (-1, 1):
            bollard(g, sx * 260, sy * 210, h=60, r=12)
    for sx in (-1, 1):
        g.box(HULLD, (sx * 260, 0, 44), (9, 420, 9))
    g.cyl(HULL, (330, 60, -60), 7, 420, steps=5)
    p = sc.part(P.motion(swing=6, pivot=(165, 45, 360), axis=0.0, rate=0.4, phase=0.2))
    rod(p, HULL, (0, 30, 362), (330, 60, 356), 3, steps=4)
    for k, col in enumerate((CREAM, TEAL, CREAM, TEAL)):
        p.box(col, (60 + k * 66, 36 + k * 6, 338), (34, 5, 44))


def wheel_capstan(sc):
    """PROP, LOW (1.75 m). A capstan with its bars, a ship's wheel on a stand, another lying on a coil of rope."""
    g = sc.still()
    g.cyl(HULL, (0, 0, -20), 62, 34, steps=8)                              # the capstan
    g.cyl(SOOT, (0, 0, 14), 44, 70, steps=8, top_r=30)
    g.cyl(CREAM, (0, 0, 84), 50, 24, steps=8)
    for k in range(6):
        a = math.radians(k * 60 + 15)
        rod(g, HULL, (40 * math.cos(a), 40 * math.sin(a), 96), (150 * math.cos(a), 150 * math.sin(a), 100), 5, steps=5)
    for sx in (-1, 1):                                                     # the wheel on its stand, facing the road
        tilt_box(g, SOOT, (-250 + sx * 50, 20, -20), (-250, 20, 96), 12, 12)
    cx, cz = -250.0, 96.0
    rod(g, HULL, (cx, 8, cz), (cx, -2, cz), 60, steps=10)
    rod(g, HULLD, (cx, -2, cz), (cx, -6, cz), 44, steps=10)
    rod(g, CREAM, (cx, -6, cz), (cx, -14, cz), 14, steps=6)
    for k in range(4):
        a = math.radians(k * 45)
        c, s = math.cos(a), math.sin(a)
        tilt_box(g, CREAM, (cx - 78 * c, -9, cz - 78 * s), (cx + 78 * c, -9, cz + 78 * s), 5, 8)
    g.cyl(CREAM, (230, 10, -20), 58, 50, steps=8)                           # the coil, and the wheel lying on it
    g.cyl(SANDD, (230, 10, 30), 40, 8, steps=8)
    g.cyl(HULL, (230, 10, 38), 62, 9, steps=10)
    g.cyl(HULLD, (230, 10, 47), 14, 10, steps=6)
    for k in range(4):
        a = math.radians(k * 45 + 10)
        c, s = math.cos(a), math.sin(a)
        tilt_box(g, CREAM, (230 - 80 * c, 10 - 80 * s, 49), (230 + 80 * c, 10 + 80 * s, 49), 8, 5)


def awning_stall(sc):
    """PROP, TALL (3.4 m). A scrap stall under a patched sail on four oars: plates, pipes and a porthole on the board
    (no boxes), pennants on the front edge; the sail lifts and settles."""
    g = sc.still()
    posts = ((-200, -120, 250), (200, -120, 250), (-200, 120, 320), (200, 120, 320))
    for x, y, h in posts:
        g.cyl(HULL, (x, y, -100), 7, h + 100, steps=5)
        g.box(CREAM, (x, y, h + 20), (18, 7, 48))                          # the oar's blade
    for sx in (-1, 1):
        g.box(SOOT, (sx * 150, -40, 20), (40, 70, 110))
    g.box(HULL, (0, -40, 82), (400, 96, 14))
    rod(g, TEAL, (-170, -60, 100), (-40, -52, 100), 9, steps=6)            # pipes
    rod(g, CREAM, (-160, -30, 98), (-60, -24, 98), 7, steps=6)
    g.box(DRUST, (40, -30, 112), (56, 7, 48), rot=(0, 20, 0))              # plates stood on edge
    g.box(SOOT, (84, -36, 108), (48, 7, 40), rot=(0, -15, 0))
    rod(g, CREAM, (150, -44, 90), (150, -44, 98), 24, steps=8)             # a porthole ring lying flat
    rod(g, SOOT, (150, -44, 98), (150, -44, 101), 15, steps=8)
    g.box(HULLD, (0, 124, 190), (400, 6, 180))                             # the back cloth, scrap hung on it
    g.box(CREAM, (-110, 119, 210), (60, 6, 60))
    g.cyl(TEAL, (40, 112, 150), 22, 8, rot=(0, 0, 90), steps=8)
    small_anchor(g, 130, 117, 150, s=0.7, col=SOOT)
    a = sc.part(P.motion(swing=3, pivot=(0, 130, 336), axis=0.0, rate=0.3, phase=0.4))
    tilt_box(a, CREAM, (0, 140, 338), (0, -150, 262), 460, 7)
    tilt_box(a, TEAL, (-110, 40, 316), (-110, -40, 295), 90, 11)
    tilt_box(a, DRUST, (120, -40, 295), (120, -100, 279), 70, 11)
    for k, col in enumerate((TEAL, CREAM, DRUST, CREAM, TEAL)):
        a.box(col, (-180 + k * 90, -150, 240), (40, 5, 40))


def winch_net(sc):
    """PROP, TALL (6.6 m). A sheer-legs winch: a hook and a net of scrap (plates and pipes, no boxes) swinging
    slowly under it, a drum and crank on a sledge."""
    g = sc.still()
    apex = (0.0, -40.0, 600.0)
    for sx in (-1, 1):
        tilt_box(g, RUST, (sx * 230, 100, -110), apex, 30, 30)
        g.box(HULL, (sx * 230, 130, 6), (40, 380, 22))                     # the sledge runners
    tilt_box(g, DRUST, (0, 330, -110), apex, 24, 24)
    g.box(DRUST, (0, 100, 250), (300, 14, 14))
    tip = (0.0, -250.0, 650.0)
    tilt_box(g, RUST, apex, tip, 24, 24)
    g.box(SOOT, (0, -40, 606), (60, 60, 40))
    rod(g, SOOT, (-70, 210, 70), (70, 210, 70), 40, steps=8)               # the drum
    for sx in (-1, 1):
        g.box(HULL, (sx * 84, 210, 50), (14, 60, 120))
    rod(g, HULLD, (0, 205, 100), (0, -30, 610), 4, steps=4)
    c = sc.part(P.motion(swing=24, pivot=(100, 210, 70), axis=0.0, rate=0.3, phase=0.1))   # the crank rocks
    c.box(SOOT, (100, 210, 96), (10, 10, 60))
    c.box(CREAM, (118, 210, 124), (40, 9, 9))
    n = sc.part(P.motion(swing=4, pivot=tip, axis=90.0, rate=0.2, phase=0.5))
    rod(n, HULLD, tip, (0, -250, 440), 4, steps=4)
    n.box(SOOT, (0, -250, 428), (16, 16, 30))
    n.sphere(HULLD, (0, -250, 300), 96, scale=(1, 1, 0.86), steps=(4, 7))
    for k in range(4):
        a = math.radians(45 + k * 90)
        tilt_box(n, HULL, (0, -250, 418), (92 * math.cos(a), -250 + 92 * math.sin(a), 312), 5, 5)
    rod(n, TEAL, (-120, -260, 300), (110, -230, 350), 12, steps=6)         # what sticks out of the net
    rod(n, CREAM, (30, -350, 270), (-40, -160, 340), 10, steps=6)
    n.box(DRUST, (60, -290, 380), (90, 7, 70), rot=(20, 30, 10))
    n.box(SOOT, (-60, -220, 372), (70, 7, 80), rot=(-15, -40, 0))


def signal_mast(sc):
    """PROP, TALL (7.9 m). The town's sign mast: a spar with a crow's-nest barrel, three picture boards (a porthole,
    a wave, an anchor: no words) swinging from the yard, a vane turning on top."""
    g = sc.still()
    g.box(DRUST, (0, 0, -40), (130, 110, 150))
    g.box(HULL, (0, 0, 44), (96, 80, 22))
    g.cyl(HULL, (0, 0, 40), 12, 700, steps=6, top_r=8)
    g.cyl(HULLD, (0, 0, 440), 44, 64, steps=8, top_r=50)
    g.cyl(CREAM, (0, 0, 470), 52, 12, steps=8)
    g.box(HULL, (0, 0, 380), (330, 9, 9))
    for sx in (-1, 1):
        rod(g, HULLD, (sx * 160, 0, 384), (0, 0, 620), 3, steps=4)
    boards = ((-120, SOOT, 0.1), (0, TEAL, 0.45), (120, CREAM, 0.8))
    for x, col, ph in boards:
        b = sc.part(P.motion(swing=7, pivot=(x, 0, 376), axis=0.0, rate=0.33, phase=ph))
        b.box(HULLD, (x, 0, 352), (6, 6, 44))
        b.box(col, (x, 0, 290), (86, 7, 86))
        if x < 0:                                                          # an anchor
            small_anchor(b, x, -6, 258, s=0.62, col=CREAM)
        elif x == 0:                                                       # a wave
            tilt_box(b, CREAM, (x - 30, -6, 280), (x - 8, -6, 300), 5, 8)
            tilt_box(b, CREAM, (x - 8, -6, 300), (x + 12, -6, 280), 5, 8)
            tilt_box(b, CREAM, (x + 12, -6, 280), (x + 32, -6, 300), 5, 8)
        else:                                                              # a porthole
            rod(b, TEAL, (x, -4, 290), (x, -9, 290), 28, steps=8)
            rod(b, SOOT, (x, -9, 290), (x, -12, 290), 17, steps=8)
    v = sc.part(P.motion(base=(0.0, 0.0), travel=(-1.0, 0.06)))
    v.box(CREAM, (40, 0, 760), (170, 7, 14))
    v.box(TEAL, (-66, 0, 760), (46, 7, 56))
    v.box(CREAM, (130, 0, 760), (26, 7, 30))
    g.cyl(SOOT, (0, 0, 738), 5, 44, steps=5)


def boiler_planter(sc):
    """PROP, LOW (1.25 m). Half a boiler drum as a planter, pale tufts nodding in it, and a plank bench on two
    bollards. 5.8 m long."""
    g = sc.still()
    rod(g, SOOT, (-290, 0, 30), (10, 0, 30), 56, steps=10)
    for x in (-240, -140, -40):
        rod(g, CREAM, (x - 5, 0, 30), (x + 5, 0, 30), 59, steps=10)
    for x in (-250, -30):
        g.box(HULL, (x, 0, -10), (26, 130, 60))
    g.box(SANDD, (-140, 0, 80), (270, 62, 12))
    t = sc.part(P.motion(swing=6, pivot=(-140, 0, 84), axis=0.0, rate=0.4, phase=0.2))
    for k, (x, col, r) in enumerate(((-240, LAV, 24), (-190, CREAM, 18), (-140, PINK, 26), (-85, CREAM, 17), (-40, LAV, 22))):
        t.sphere(col, (x, (k % 2) * 10 - 5, 100 + r * 0.5), r, steps=(3, 5))
    bollard(g, 110, 0, h=40, r=15)
    bollard(g, 270, 0, h=40, r=15)
    g.box(HULL, (190, 0, 58), (230, 46, 10))


# ------------------------------------------------------------------------------------------ the hero
def standing_stern(sc):
    """HERO. THE STANDING STERN: the back half of a great ship on end, stern up, 69 m; her deck faces the road and
    is the town's high street (balconies, lit doors and windows, the old deckhouse and its funnel standing out
    level), her keel side is rust; her propeller turns slowly at the top. Blunt on top: no rudder, no fin."""
    g = sc.still()
    g.sphere(SANDD, (0, 0, -150), 1500, scale=(1.15, 1.0, 0.24), steps=(4, 8))
    for k, (x, y, yaw, h) in enumerate(((-960, -520, 20, 700), (990, -400, -30, 560), (-760, 700, 60, 620), (880, 640, -50, 800), (140, -900, 5, 420))):
        g.box(DRUST if k % 2 else RUST, (x, y, h / 2.0 - 200), (520, 34, h), rot=(10, yaw, 8))
    g.box(HULL, (0, -80, 1250), (1600, 940, 3100))                         # the deck half (front)
    g.box(HULL, (0, -80, 3900), (1540, 900, 2200))
    g.box(RUST, (0, 320, 1250), (1620, 460, 3100))                         # the keel half (back)
    g.box(RUST, (0, 310, 3900), (1560, 440, 2200))
    g.box(CREAM, (0, 40, 2350), (1636, 120, 5300))                          # her boot-top, now a stripe up her side
    g.sphere(HULL, (0, 0, 5000), 770, scale=(1.0, 0.68, 1.75), steps=(8, 12))
    g.sphere(RUST, (0, 90, 4990), 775, scale=(1.0, 0.6, 1.72), steps=(6, 10))
    g.box(CREAM, (0, -545, 2350), (1440, 30, 4900))                        # the deck
    # fix round: four broad bands round the hull, cream and teal, between the rows of ports: from the road her sides were one dark mass
    for z, col, wx in ((1200, CREAM, 1660), (2240, TEAL, 1660), (3280, CREAM, 1600), (4320, TEAL, 1600)):
        g.box(col, (0, 10, z), (wx, 1060, 400))
    # the propeller on its boss, at the top (the keel side)
    hx, hy = 0.0, 330.0
    g.box(DRUST, (hx, hy, 6030), (320, 380, 740))
    g.cyl(SOOT, (hx, hy, 6400), 74, 220, steps=8)
    g.cyl(CREAM, (hx, hy, 6600), 124, 170, steps=8)
    g.cyl(CREAM, (hx, hy, 6770), 124, 110, steps=8, top_r=44)
    p = sc.part(P.motion(base=(hx, hy), travel=(-1.0, 0.05)))
    for k in range(4):
        a = math.radians(k * 90 + 20)
        c, s = math.cos(a), math.sin(a)
        tilt_box(p, TEAL, (hx + 90 * c, hy + 90 * s, 6690), (hx + 560 * c, hy + 560 * s, 6690), 230, 36)
        tilt_box(p, TEAL, (hx + 300 * c, hy + 300 * s, 6692), (hx + 520 * c, hy + 520 * s, 6692), 330, 30)
    # the look-out on the stern, beside the boss
    g.box(HULL, (0, -160, 6250), (560, 420, 16))
    rail(g, -270, 270, -360, 6258, n=5)
    g.box(HULL, (0, -160, 5800), (60, 60, 900))
    g.sphere(LAMP, (0, -330, 6420), 40, steps=(4, 6), glow=1.4)
    g.box(SOOT, (0, -330, 6330), (10, 10, 150))
    # hatches, now the town's gates
    for z in (520, 1750):
        g.box(DRUST, (0, -566, z), (720, 20, 520))
        g.box(SOOT, (0, -578, z - 60), (220, 10, 380))
        g.box(LAMP, (0, -584, z + 180), (150, 8, 60), glow=0.9)
    # balconies up the deck
    for k, z in enumerate((1180, 2420, 4120, 4720)):
        g.box(HULL, (0, -650, z), (940, 190, 16))
        rail(g, -460, 460, -738, z + 8, n=6)
        g.box(SOOT, (0, -563, z + 108), (90, 10, 190))
        for sx in (-1, 1):
            g.box(LAMP, (sx * 300, -563, z + 120), (80, 8, 80), glow=0.9)
            tilt_box(g, DRUST, (sx * 440, -562, z - 150), (sx * 440, -730, z), 14, 14)
    for k, (z0, z1) in enumerate(((1196, 2412), (2436, 2980))):             # ramps of plank from deck to deck
        sx = 1 if k % 2 == 0 else -1
        tilt_box(g, HULL, (-sx * 420, -640, z0), (sx * 420, -640, z1), 80, 12)
    # the old deckhouse stands out level, her funnel pointing at the road
    g.box(CREAM, (0, -800, 3400), (1000, 500, 760))
    g.box(TEAL, (0, -1056, 3400), (1040, 14, 800))
    for sx in (-1, 1):
        for z in (3180, 3440, 3640):
            g.box(LAMP, (sx * 503, -800, z), (8, 300, 90), glow=0.9)
    for x in (-320, 320):
        g.box(LAMP, (x, -1066, 3560), (180, 8, 150), glow=0.9)
    rod(g, RUST, (0, -1050, 3300), (0, -1560, 3300), 170, steps=10)
    rod(g, TEAL, (0, -1330, 3300), (0, -1420, 3300), 176, steps=10)
    rod(g, SOOT, (0, -1560, 3300), (0, -1610, 3300), 172, steps=10)
    # lit ports up both her sides
    for sx in (-1, 1):
        for j in range(9):
            z = 420 + j * 520
            xw = 803 if z < 2800 else 773
            for y in (-420, -220):
                g.box(LAMP, (sx * xw, y, z), (8, 46, 62), glow=0.9)
    # a derrick with a net of scrap, and a line of washing from the top balcony
    tip = (640.0, -1500.0, 5150.0)
    tilt_box(g, SOOT, (500, -560, 4300), tip, 44, 44)
    rod(g, HULLD, tip, (300, -440, 5300), 6, steps=4)
    n = sc.part(P.motion(swing=3, pivot=tip, axis=90.0, rate=0.15, phase=0.3))
    rod(n, HULLD, tip, (640, -1500, 4640), 6, steps=4)
    n.sphere(HULLD, (640, -1500, 4500), 150, scale=(1, 1, 0.86), steps=(4, 7))
    rod(n, TEAL, (480, -1520, 4500), (800, -1470, 4580), 18, steps=6)
    n.box(DRUST, (700, -1560, 4620), (140, 10, 110), rot=(20, 30, 10))
    w = sc.part(P.motion(swing=5, pivot=(0, -740, 4820), axis=0.0, rate=0.25, phase=0.6))
    rod(w, HULL, (-450, -740, 4830), (450, -740, 4810), 5, steps=4)
    for k, col in enumerate((CREAM, TEAL, CREAM, DRUST, CREAM, TEAL)):
        w.box(col, (-360 + k * 144, -740, 4740), (100, 7, 130))
    a = sc.part(P.motion(swing=3, pivot=(0, -562, 2700), axis=0.0, rate=0.2, phase=0.1))   # a sail over the second balcony
    tilt_box(a, CREAM, (0, -562, 2700), (0, -800, 2640), 700, 9)
    tilt_box(a, TEAL, (-160, -620, 2692), (-160, -720, 2667), 140, 14)


JOBS = [
    ('SM_S1D4_HullHouse', hull_house),
    ('SM_S1D4_FunnelTower', funnel_tower),
    ('SM_S1D4_BridgeInn', bridge_inn),
    ('SM_S1D4_JibWorkshop', jib_workshop),
    ('SM_S1D4_LifeboatRow', lifeboat_row),
    ('SM_S1D4_ForgeRock', forge_rock),
    ('SM_S1D4_Tenement', tenement),
    ('SM_S1D4_ChainFence', chain_fence, STILL),
    ('SM_S1D4_PortholeLamp', porthole_lamp),
    ('SM_S1D4_PropMonument', prop_monument),
    ('SM_S1D4_WheelCapstan', wheel_capstan, STILL),
    ('SM_S1D4_AwningStall', awning_stall),
    ('SM_S1D4_WinchNet', winch_net),
    ('SM_S1D4_SignalMast', signal_mast),
    ('SM_S1D4_BoilerPlanter', boiler_planter, dict(grow=(20, 20, 20), grow_down=(20, 20, 0))),
    ('SM_S1D4_StandingStern', standing_stern, dict(far=True, grow=(300, 300, 60), grow_down=(300, 300, 0))),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
