"""Chemical Works, Stage 1, builder s1d6: district 6 "Cooling Field", the spa in the steam (13.4 to 17.4 km).
/Game/World/ChemicalWorks/SM_S1D6_*.  Run: import ft_world_chem_s1d6 as K; K.run()

The works' bath-and-garden quarter, built where the warm cooling water runs: low, wide, terraced and curved, one and
two storeys, colonnades, shallow domes, a glass winter garden, pools with a raised rim, steam everywhere.
Seven buildings, eight props and the hero, THE CASCADE (a stepped cooling pyramid of seven terraces, a sheet of pale
water down the middle of each, a lit hall in a colonnade on top and a slow plume of steam).
Every mesh is an ft_people.Scene: one material (M_People), one section, no collision, movement in the vertex shader.
Palette (the plan's five, and the kit's white and rain-pale for water): lavender white, lilac, periwinkle, purple,
lamp cream. Light is steady and only on pieces whose every rule stands 24 m and more out (the table block says which).
No words, no marks, no figures. Nothing orange or green, nothing that bobs or turns at box size by the road.
"""
import math
import ft_people as P
from ft_desert import rod, tilt_box            # tilt_box(m, col, (x,y,z) from, (x,y,z) to, width, depth)

DIR = '/Game/World/ChemicalWorks'
STONE, LILAC, PERI, PURPLE, CREAM = '#d8d0f0', '#b9a8e0', '#7f8fc6', '#6a5ab0', '#fff1c0'
WHITE, WATER = '#eef6ff', '#dfe8ff'

STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))        # no moving part: true bounds
LOW = dict(grow=(8, 8, 8), grow_down=(8, 8, 0))          # a low prop whose parts move a hand's breadth (bounds stay under 2.5 m)
SMALL = dict(grow=(25, 25, 25), grow_down=(25, 25, 0))   # a building whose moving parts stay close to it


def _ring(r, deg):
    """A point on a circle round the middle: 0 degrees is the front (-Y), 90 is +X."""
    a = math.radians(deg)
    return r * math.sin(a), -r * math.cos(a)


def column(g, x, y, z, h, r=30.0, col=STONE, steps=6):
    """A plain round column with a square capital and a square base."""
    g.cyl(col, (x, y, z), r, h, steps=steps)
    g.box(col, (x, y, z + h - 9.0), (r * 2.7, r * 2.7, 18.0))
    g.box(col, (x, y, z + 7.0), (r * 2.5, r * 2.5, 14.0))


def puffs(sc, pts, pivot_drop=120.0, rate=0.12, bob=-6.0, swing=3.0):
    """Steam: pale balls that float a little and lean, each out of step. pts: (x, y, z, radius)."""
    for k, (x, y, z, r) in enumerate(pts):
        m = sc.part(P.motion(phase=(0.13 + k * 0.37) % 1.0, rate=rate * (1.0 + 0.2 * (k % 3)), bob=bob, swing=swing,
                             pivot=(x, y, z - pivot_drop), axis=20.0 + 55.0 * k))
        m.sphere(STONE if k % 2 == 0 else WHITE, (x, y, z), r, steps=(3, 6))


# ====================================================================================================== buildings
def bath_house(sc):
    """BUILDING: the bath house. A rotunda 15 m across on a paved platform, a ring of twelve columns, a shallow lilac
    dome with a glazed lantern, steam out of the lantern. 10.3 m to the lantern's cap, the steam to 11.9."""
    g = sc.still()
    g.cyl(PERI, (0, 0, -200), 750, 240, steps=12)                    # the paved platform, 2 m of footing
    g.cyl(STONE, (0, 0, 40), 690, 40, steps=12)                      # a step
    g.box(LILAC, (0, -760, -80), (340, 150, 240))                    # the front steps
    g.box(STONE, (0, -720, 20), (300, 110, 80))
    g.cyl(STONE, (0, 0, 80), 470, 470, steps=12)                     # the drum
    for k in range(12):
        x, y = _ring(610, k * 30 + 15)
        column(g, x, y, 80, 440, r=34)
    g.cyl(PURPLE, (0, 0, 520), 665, 60, steps=12)                    # entablature
    for k in range(12):                                              # frieze panels
        x, y = _ring(660, k * 30)
        g.box(PERI, (x, y, 550), (150, 22, 34), rot=(0, k * 30, 0))
    g.cyl(STONE, (0, 0, 580), 705, 30, steps=12)                     # cornice
    g.cyl(STONE, (0, 0, 610), 520, 70, steps=12)                     # attic
    g.sphere(PURPLE, (0, 0, 680), 500, scale=(1, 1, 0.42), steps=(5, 12))   # the shallow dome: 8.9 m
    g.cyl(STONE, (0, 0, 860), 105, 110, steps=8)                     # the lantern
    for k in range(4):
        x, y = _ring(92, k * 90 + 45)
        g.box(CREAM, (x, y, 915), (64, 40, 70), rot=(0, k * 90 + 45, 0), glow=0.8)
    g.cyl(PURPLE, (0, 0, 970), 140, 60, steps=8, top_r=40)           # its cap, open at the top: 10.3 m
    # the door and four tall windows between the columns of the front half
    x, y = _ring(466, 0)
    g.box(PURPLE, (x, y, 215), (140, 26, 270))
    g.box(CREAM, (x, y, 385), (140, 24, 50), glow=0.7)
    for deg in (-60, -30, 30, 60):
        x, y = _ring(464, deg)
        g.box(CREAM, (x, y, 300), (76, 26, 210), rot=(0, deg, 0), glow=0.7)
        g.box(PERI, (x, y, 185), (96, 30, 14), rot=(0, deg, 0))      # sill
    for deg in (-120, -90, 90, 120, 150, 180, 210):                  # shuttered round the back
        x, y = _ring(464, deg)
        g.box(PURPLE, (x, y, 300), (76, 26, 210), rot=(0, deg, 0))
    for sx in (-1, 1):                                               # two urns by the steps
        g.box(STONE, (sx * 230, -700, 110), (60, 60, 60))
        g.cyl(LILAC, (sx * 230, -700, 140), 14, 40, steps=6, top_r=34)
    puffs(sc, [(0, 0, 1050, 46), (30, -20, 1110, 38), (-25, 20, 1160, 30)], pivot_drop=160.0, rate=0.1, bob=-8.0, swing=3.0)


def winter_garden(sc):
    """BUILDING: the winter garden. 22 x 9 m: an open arcade with pale trees in tubs inside (their crowns sway), a
    purple glass barrel vault on eight arched ribs, three lamps hanging under it. 9.8 m to the ridge, 10.3 to its finials."""
    g = sc.still()
    g.box(PERI, (0, 0, -70), (2300, 1000, 260))                      # the platform: 2 m of footing, top 0.6 m
    g.box(STONE, (0, 0, 120), (2200, 900, 120))                      # plinth wall, to 1.8 m
    g.box(PURPLE, (0, 300, 320), (2150, 260, 280))                   # the glazed back of the house, 1.8 to 4.6 m
    for k in range(8):                                               # the front arcade: posts and a low rail
        x = -1050 + k * 300
        g.box(STONE, (x, -420, 320), (50, 50, 280))
        if k < 7 and k != 3:
            g.box(LILAC, (x + 150, -425, 205), (250, 20, 50))
    for sx in (-1, 1):                                               # glazed ends
        g.box(PURPLE, (sx * 1065, 0, 320), (40, 800, 280))
    g.box(STONE, (0, 0, 480), (2200, 900, 40))                       # wall plate, 4.6 to 5.0 m
    v = sc.still(yaw=90.0)                                           # the vault's arch is drawn across, then turned along the house
    arch = [(430.0 * math.cos(math.radians(t)), 500.0 + 430.0 * math.sin(math.radians(t))) for t in range(0, 181, 20)]
    v.side(PURPLE, arch, -1060, 1060)
    rib = [(456.0 * math.cos(math.radians(t)), 496.0 + 456.0 * math.sin(math.radians(t))) for t in range(0, 181, 20)]
    for k in range(8):
        x = -1050 + k * 300
        v.side(STONE, rib, x - 22, x + 22)
    g = sc.still()
    g.box(LILAC, (0, 0, 955), (2000, 60, 40))                        # ridge
    for k in range(5):
        g.cyl(STONE, (-900 + k * 450, 0, 975), 22, 50, steps=6, top_r=5)
    g.side(STONE, [(-200, 500), (200, 500), (0, 610)], -480, -400)   # a gable over the middle bay
    g.box(PERI, (0, -404, 345), (200, 12, 250))                      # the open door's dark
    for k in range(3):                                               # lamps under the vault
        g.box(CREAM, (-600 + k * 600, -150, 440), (44, 44, 44), glow=0.9)
    for k in range(6):                                               # pale trees in tubs
        x = -900 + k * 360 + (60 if k % 2 else 0)
        y = -230 if k % 2 == 0 else -60
        g.box(PERI, (x, y, 205), (70, 70, 50))
        g.cyl(LILAC, (x, y, 230), 14, 130, steps=5)
        c = sc.part(P.motion(swing=3.0, pivot=(x, y, 230), axis=20.0 + 40.0 * k, rate=0.3, phase=(k * 0.29) % 1.0))
        c.sphere(WHITE if k % 2 == 0 else STONE, (x, y, 385), 88, scale=(1, 1, 0.8), steps=(3, 6))
    g.box(STONE, (0, -530, 20), (300, 90, 80))                       # the step at the door


def pump_room(sc):
    """BUILDING: the pump room. A small temple front, 9 x 6.6 m: four columns, a pediment, a purple roof, a lit door,
    two steaming spring basins on the platform. 7.6 m to the ridge."""
    g = sc.still()
    g.box(PERI, (0, 0, -80), (900, 660, 240))                        # platform, 2 m of footing, top 0.4 m
    g.box(STONE, (0, -350, 5), (460, 70, 70))                        # steps
    g.box(STONE, (0, -20, 55), (820, 580, 30))                       # stylobate, to 0.7 m
    g.box(STONE, (0, 70, 280), (640, 380, 420))                      # the cella, 0.7 to 4.9 m
    for x in (-285, -95, 95, 285):
        column(g, x, -250, 70, 420, r=36, steps=8)
    g.box(LILAC, (0, -5, 520), (760, 610, 60))                       # entablature, to 5.5 m
    g.side(STONE, [(-400, 550), (400, 550), (0, 700)], -310, 300)    # the pediment
    tilt_box(g, PURPLE, (-425, -5, 548), (0, -5, 714), 640, 18)      # the roof's two slopes
    tilt_box(g, PURPLE, (425, -5, 548), (0, -5, 714), 640, 18)
    g.box(PERI, (0, -314, 605), (96, 10, 56))                        # a plain panel in the gable
    g.box(CREAM, (0, -122, 240), (130, 12, 300), glow=0.7)           # the lit door
    for sx in (-1, 1):
        g.box(CREAM, (sx * 200, -122, 310), (70, 12, 160), glow=0.7)
        for y in (-60, 110, 230):
            g.box(LILAC, (sx * 322, y, 280), (12, 44, 420))          # pilasters
        g.box(CREAM, (sx * 322, 25, 310), (10, 80, 160), glow=0.6)
        g.cyl(STONE, (sx * 400, -5, 552), 22, 50, steps=6, top_r=6)  # acroteria
        g.cyl(PERI, (sx * 380, -270, 40), 46, 76, steps=8)           # a spring basin
        g.cyl(WATER, (sx * 380, -270, 116), 38, 4, steps=8)
    g.cyl(STONE, (0, -5, 712), 22, 50, steps=6, top_r=6)
    puffs(sc, [(380, -270, 160, 30), (-380, -270, 165, 32), (395, -262, 215, 24), (-370, -280, 222, 24)], pivot_drop=60.0, bob=-6.0)


def colonnade(sc):
    """BUILDING (no light): the colonnade walk. 24.6 x 3.4 m: ten columns before a lilac wall with niches and benches,
    urns of pale grass on the parapet (they sway), a little purple-capped block at each end. 6.3 m."""
    g = sc.still()
    g.box(PERI, (0, 0, -75), (2460, 340, 250))                       # the walk, 2 m of footing, top 0.5 m
    for k in range(10):
        column(g, -1080 + k * 240, -110, 50, 380, r=30)
    g.box(PURPLE, (0, 135, 240), (2400, 40, 380))                    # the back wall
    for k in range(5):
        g.box(PERI, (-960 + k * 480, 112, 250), (150, 12, 240))      # niches
        if k < 4:
            g.box(STONE, (-720 + k * 480, 70, 75), (260, 46, 50))    # benches
    g.box(STONE, (0, 5, 460), (2440, 330, 60))                       # entablature, 4.3 to 4.9 m
    g.box(PURPLE, (0, 5, 505), (2380, 280, 30))                      # parapet, to 5.2 m
    for sx in (-1, 1):
        g.box(STONE, (sx * 1110, 5, 545), (220, 300, 50))
        g.cyl(PURPLE, (sx * 1110, 5, 570), 125, 60, steps=8, top_r=14)
    for k, x in enumerate((-720, -240, 240, 720)):
        g.cyl(STONE, (x, -80, 520), 16, 34, steps=6, top_r=34)
        p = sc.part(P.motion(swing=4.0, pivot=(x, -80, 554), axis=10.0, rate=0.3, phase=(0.1 + k * 0.23) % 1.0))
        p.cyl(WHITE, (x, -80, 554), 26, 52, steps=5, top_r=4)


def tea_pavilion(sc):
    """BUILDING: the tea pavilion. An octagon 8.6 m across: eight posts and a rail round a lit room, a purple roof
    stepped three times like a pagoda with cream lamps under the eaves, a lilac pennant turning on the top. 9.2 m."""
    g = sc.still()
    g.cyl(PERI, (0, 0, -200), 430, 240, steps=8)                     # platform, 2 m of footing
    g.cyl(STONE, (0, 0, 40), 390, 30, steps=8)
    g.box(STONE, (0, -430, 0), (240, 90, 80))                        # step
    g.cyl(STONE, (0, 0, 70), 230, 290, steps=8)                      # the room, to 3.6 m
    for k in range(8):
        x, y = _ring(350, k * 45 + 22.5)
        column(g, x, y, 70, 290, r=22)
        if k not in (0, 7):                                          # the rail, open at the front
            x, y = _ring(322, k * 45 + 45)
            g.box(LILAC, (x, y, 135), (230, 14, 60), rot=(0, k * 45 + 45, 0))
        x, y = _ring(418, k * 45 + 22.5)
        g.box(CREAM, (x, y, 340), (26, 26, 34), glow=0.8)            # a lamp under the eave
    x, y = _ring(226, 0)
    g.box(PURPLE, (x, y, 190), (100, 16, 230))                       # door
    for deg in (-90, -45, 45, 90):
        x, y = _ring(224, deg)
        g.box(CREAM, (x, y, 230), (90, 16, 150), rot=(0, deg, 0), glow=0.7)
    g.cyl(PURPLE, (0, 0, 360), 480, 110, steps=8, top_r=250)         # first roof, to 4.7 m
    g.cyl(STONE, (0, 0, 470), 220, 70, steps=8)
    for k in range(4):
        x, y = _ring(216, k * 90)
        g.box(CREAM, (x, y, 505), (70, 14, 36), rot=(0, k * 90, 0), glow=0.7)
    g.cyl(PURPLE, (0, 0, 540), 340, 100, steps=8, top_r=160)         # second, to 6.4 m
    g.cyl(STONE, (0, 0, 640), 140, 60, steps=8)
    g.cyl(PURPLE, (0, 0, 700), 230, 130, steps=8, top_r=24)          # third, to 8.3 m
    g.cyl(LILAC, (0, 0, 830), 6, 92, steps=5)                        # mast
    f = sc.part(P.motion(swing=24.0, pivot=(0, 0, 900), axis=P.UP, rate=0.25, phase=0.3))
    f.box(LILAC, (40, 0, 900), (72, 5, 30))                          # pennant
    f.box(WHITE, (40, 0, 900), (74, 6, 8))


def cabin_terrace(sc):
    """BUILDING (no light): the changing cabins. Seven gabled cabins, lilac and white by turns with striped fronts, on
    a raised step in a shallow curve whose ends come 1.4 m nearer the road; towels on a rail before them (they sway).
    25 x 5.2 m, 4.3 m tall."""
    g = sc.still()
    g.box(PERI, (0, -30, -85), (2500, 420, 230))                     # the terrace: 2 m of footing, top 0.3 m
    for k in range(7):
        th = (k - 3) * 5.0
        a = math.radians(th)
        c = sc.still(at=(4000.0 * math.sin(a), 4000.0 * (math.cos(a) - 1.0), 0.0), yaw=-th)
        body, stripe = (WHITE, LILAC) if k % 2 == 0 else (LILAC, WHITE)
        c.box(PERI, (0, -20, 45), (330, 300, 30))                    # the raised step
        c.box(body, (0, 30, 190), (270, 200, 260))                   # the cabin, 0.6 to 3.2 m
        c.side(PURPLE if k % 2 == 0 else PERI, [(-160, 320), (160, 320), (0, 410)], -85, 145)
        c.box(PURPLE, (0, -72, 165), (90, 8, 200))                   # door
        c.box(PERI, (0, -72, 290), (44, 8, 24))                      # vent
        for sx in (-1, 1):
            c.box(stripe, (sx * 95, -72, 190), (36, 6, 250))
        tilt_box(c, stripe, (0, -76, 300), (0, -150, 268), 170, 8)   # a little awning over the door
        c.box(WHITE, (0, 30, 418), (14, 14, 22))                     # finial
    g = sc.still()
    for sx in (-1, 1):
        g.box(STONE, (sx * 300, -215, 90), (7, 7, 124))              # the towel rail
    g.box(STONE, (0, -215, 152), (614, 7, 7))
    for k, x in enumerate((-220, -80, 60, 200)):
        p = sc.part(P.motion(swing=5.0, pivot=(x, -215, 152), axis=0.0, rate=0.35, phase=(0.2 + k * 0.27) % 1.0))
        p.box((WHITE, LILAC, STONE, PERI)[k], (x, -215, 112), (64, 6, 74))


def hydro(sc):
    """BUILDING: the hydro, the spa's hotel. 21 x 9.8 m in three stepped terraces: a loggia of eight columns, an upper
    floor set back behind a terrace of planters (they sway) with striped awnings over its windows, an attic with a
    shallow dome and a small lantern; steam from a vent on the roof. 12 m to the lantern's cap."""
    g = sc.still()
    g.box(PERI, (0, 0, -80), (2100, 980, 240))                       # platform, 2 m of footing, top 0.4 m
    g.box(STONE, (0, -520, 0), (460, 100, 80))                       # steps
    g.box(STONE, (0, 60, 230), (1960, 760, 380))                     # ground floor, 0.4 to 4.2 m (front y -320)
    for k in range(8):
        column(g, -910 + k * 260, -420, 40, 340, r=30)
    g.box(PURPLE, (0, -200, 400), (2000, 520, 40))                   # the loggia's roof: the first terrace
    g.box(STONE, (0, -447, 445), (2000, 24, 50))                     # its balustrade
    for k in range(7):
        x = -780 + k * 260
        if k == 3:
            g.box(PURPLE, (x, -322, 190), (150, 10, 300))
            g.box(CREAM, (x, -322, 365), (150, 10, 40), glow=0.7)
        else:
            g.box(CREAM, (x, -322, 230), (110, 10, 200), glow=0.7)
    g.box(STONE, (0, 150, 600), (1500, 580, 360))                    # upper floor, 4.2 to 7.8 m (front y -140)
    for k in range(6):
        x = -625 + k * 250
        g.box(CREAM, (x, -142, 600), (100, 10, 170), glow=0.7)
        tilt_box(g, PURPLE if k % 2 == 0 else WHITE, (x, -144, 715), (x, -236, 672), 130, 8)   # awnings, purple and white by turns
    for sx in (-1, 1):                                               # the stepped wings
        g.box(STONE, (sx * 870, 150, 510), (240, 500, 180))
        g.box(PURPLE, (sx * 870, 150, 610), (260, 520, 20))
        g.box(CREAM, (sx * 870, -102, 510), (90, 10, 100), glow=0.6)
    g.box(PURPLE, (0, 150, 795), (1560, 640, 30))                    # cornice: the second terrace, 8.1 m
    g.box(STONE, (0, 180, 890), (860, 420, 160))                     # attic, to 9.7 m (front y -30)
    for x in (-260, 0, 260):
        g.box(CREAM, (x, -32, 890), (90, 10, 90), glow=0.7)
    g.box(PURPLE, (0, 180, 982), (920, 480, 24))                     # to 9.94 m
    g.sphere(PURPLE, (0, 180, 994), 300, scale=(1, 1, 0.4), steps=(5, 10))   # the shallow dome: 11.1 m
    g.cyl(STONE, (0, 180, 1096), 56, 46, steps=6)
    g.cyl(PURPLE, (0, 180, 1142), 76, 56, steps=6, top_r=10)         # 12.0 m
    for k, x in enumerate((-750, -250, 250, 750)):                   # planters on the first terrace
        g.box(PERI, (x, -385, 445), (130, 60, 50))
        p = sc.part(P.motion(swing=3.0, pivot=(x, -385, 470), axis=0.0, rate=0.28, phase=(0.15 + k * 0.21) % 1.0))
        p.sphere(WHITE if k % 2 else STONE, (x, -385, 512), 50, steps=(3, 6))
    g.box(STONE, (560, 250, 835), (90, 90, 50))                      # the vent on the roof
    puffs(sc, [(560, 250, 905, 40), (575, 240, 965, 34), (550, 262, 1020, 28)], pivot_drop=140.0, rate=0.1, bob=-8.0)


# ====================================================================================================== props
def plunge_pool(sc):
    """PROP, LOW: a steaming plunge pool 5.3 m across with a raised rim, two steps, a hand rail and a towel on the
    rim. No light. Steam to 1.9 m."""
    g = sc.still()
    g.cyl(PERI, (0, 0, -40), 265, 100, steps=10)                     # 40 cm of footing, to 0.6 m
    g.cyl(WATER, (0, 0, 60), 215, 5, steps=10)
    for k in range(10):
        x, y = _ring(240, k * 36)
        g.box(STONE, (x, y, 72), (160, 44, 26), rot=(0, k * 36, 0))  # the rim
    g.box(STONE, (0, -300, -5), (150, 70, 70))
    g.box(STONE, (0, -352, -20), (190, 50, 40))
    for sx in (-1, 1):
        g.box(PURPLE, (sx * 60, -272, 112), (6, 6, 60))
    g.box(PURPLE, (0, -272, 140), (126, 6, 6))
    x, y = _ring(240, 108)
    g.box(WHITE, (x, y, 89), (70, 48, 8), rot=(0, 108, 0))
    puffs(sc, [(-80, 30, 120, 38), (60, -50, 135, 42), (90, 80, 112, 34), (-20, -90, 146, 36)], pivot_drop=70.0, bob=-6.0, swing=3.0)


def deck_chairs(sc):
    """PROP, LOW: three striped deck chairs on a strip of paving, a side table with a cup, a folded parasol. 1.9 m."""
    g = sc.still()
    g.box(PERI, (20, 0, -17), (720, 190, 46))                        # paving: 40 cm of footing
    for i, x in enumerate((-250, -60, 130)):
        col = (LILAC, WHITE, PURPLE)[i]
        st = (WHITE, PURPLE, WHITE)[i]
        tilt_box(g, col, (x, -55, 22), (x, 50, 96), 56, 5)           # the canvas
        for sx in (-1, 1):
            tilt_box(g, st, (x + sx * 16, -55, 26), (x + sx * 16, 50, 100), 10, 5)
            tilt_box(g, STONE, (x + sx * 31, -42, 6), (x + sx * 31, 60, 72), 6, 6)     # the frame
            tilt_box(g, STONE, (x + sx * 31, 46, 6), (x + sx * 31, -20, 60), 6, 6)
    g.cyl(STONE, (35, -10, 6), 16, 40, steps=6)                      # side table
    g.cyl(LILAC, (35, -10, 46), 24, 5, steps=6)
    g.cyl(WHITE, (35, -10, 51), 6, 10, steps=5)
    g.cyl(STONE, (310, 0, 6), 24, 10, steps=8)                       # the folded parasol
    g.cyl(LILAC, (310, 0, 6), 5, 150, steps=5)
    g.cyl(PURPLE, (310, 0, 75), 20, 105, steps=6, top_r=7)
    g.cyl(WHITE, (310, 0, 180), 6, 10, steps=5)


def urn(sc):
    """PROP, LOW: a stone urn on a pedestal with a pale plant that sways. 1.9 m."""
    g = sc.still()
    g.box(STONE, (0, 0, -12), (84, 84, 56))                          # 40 cm of footing
    g.box(LILAC, (0, 0, 52), (64, 64, 72))
    g.box(STONE, (0, 0, 94), (80, 80, 12))
    g.cyl(STONE, (0, 0, 100), 14, 14, steps=6)
    g.cyl(STONE, (0, 0, 114), 16, 36, steps=8, top_r=42)
    g.cyl(LILAC, (0, 0, 150), 46, 7, steps=8)
    for sx in (-1, 1):
        g.box(PERI, (sx * 42, 0, 132), (10, 8, 26))
    p = sc.part(P.motion(swing=4.0, pivot=(0, 0, 155), axis=30.0, rate=0.3, phase=0.4))
    p.cyl(WHITE, (0, 0, 155), 22, 32, steps=5, top_r=5)
    tilt_box(p, STONE, (0, 0, 158), (30, 8, 184), 8, 5)
    tilt_box(p, STONE, (0, 0, 158), (-28, -10, 182), 8, 5)
    tilt_box(p, WHITE, (0, 0, 158), (6, 30, 180), 8, 5)


def fountain(sc):
    """PROP, LOW: a drinking fountain on three round steps: a bowl on a pedestal, three spouts running into the steps,
    a little dome of water that rises and falls 3 cm. No light. 1.8 m."""
    g = sc.still()
    g.cyl(PERI, (0, 0, -40), 150, 58, steps=8)                       # 40 cm of footing
    g.cyl(STONE, (0, 0, 18), 115, 18, steps=8)
    g.cyl(PERI, (0, 0, 36), 80, 18, steps=8)
    g.cyl(STONE, (0, 0, 54), 22, 62, steps=8)                        # pedestal
    g.cyl(STONE, (0, 0, 116), 24, 22, steps=8, top_r=62)             # bowl
    g.cyl(LILAC, (0, 0, 138), 66, 6, steps=8)
    g.cyl(WATER, (0, 0, 144), 54, 4, steps=8)
    g.cyl(STONE, (0, 0, 144), 7, 18, steps=5)
    for deg in (0, 120, 240):
        x, y = _ring(64, deg)
        g.box(PURPLE, (x, y, 132), (12, 18, 8), rot=(0, deg, 0))     # spout
        x2, y2 = _ring(74, deg)
        x3, y3 = _ring(80, deg)
        tilt_box(g, WATER, (x2, y2, 130), (x3, y3, 56), 6, 6)        # its thread of water
        x4, y4 = _ring(100, deg + 60)
        g.cyl(WHITE, (x4, y4, 36), 6, 10, steps=5)                   # a cup on the step
    p = sc.part(P.motion(bob=-3.0, rate=0.8, phase=0.2))
    p.sphere(WHITE, (0, 0, 166), 11, steps=(3, 6))


def globe_lamp(sc):
    """PROP, TALL, steady light: a lamp of five cream globes: four hang from curved arms, one stands under a pointed
    purple cap on a plain lilac post. 5.4 m. Only 24 m and more out."""
    g = sc.still()
    g.cyl(STONE, (0, 0, -150), 30, 180, steps=8)                     # 1.5 m of footing
    g.cyl(PERI, (0, 0, 0), 40, 12, steps=8)
    g.cyl(LILAC, (0, 0, 30), 11, 410, steps=6)                       # the post, to 4.4 m
    for k in range(4):
        deg = k * 90 + 45
        x, y = _ring(72, deg)
        x1, y1 = _ring(36, deg)
        tilt_box(g, LILAC, (0, 0, 392), (x1, y1, 428), 7, 7)
        tilt_box(g, LILAC, (x1, y1, 428), (x, y, 424), 7, 7)
        g.box(LILAC, (x, y, 416), (5, 5, 14))
        g.cyl(PURPLE, (x, y, 404), 15, 9, steps=6, top_r=4)
        g.sphere(CREAM, (x, y, 384), 24, steps=(4, 6), glow=0.9)
    g.cyl(LILAC, (0, 0, 440), 8, 14, steps=6, top_r=20)
    g.sphere(CREAM, (0, 0, 480), 30, steps=(4, 6), glow=0.9)
    g.cyl(PURPLE, (0, 0, 504), 25, 36, steps=6, top_r=3)


def towel_cart(sc):
    """PROP, LOW: a towel cart with folded towels and a rack of four robes that sway on their rail. 1.9 m."""
    g = sc.still()
    g.box(PERI, (0, 0, -17), (440, 150, 46))                         # paving: 40 cm of footing
    g.box(LILAC, (-90, 0, 70), (150, 70, 50))                        # the cart
    for sx in (-1, 1):
        for sy in (-1, 1):
            rod(g, PURPLE, (-90 + sx * 50, sy * 33, 32), (-90 + sx * 50, sy * 41, 32), 26, steps=8)
        tilt_box(g, STONE, (-165, sx * 30, 90), (-192, sx * 30, 122), 6, 6)
    g.box(STONE, (-192, 0, 122), (6, 66, 6))
    g.box(WHITE, (-125, 0, 105), (60, 60, 20))
    g.box(STONE, (-125, 0, 124), (58, 58, 18))
    g.box(PERI, (-58, 0, 105), (60, 60, 20))
    g.box(WHITE, (-58, 0, 123), (56, 56, 16))
    for x in (20, 200):                                              # the rack
        g.box(STONE, (x, 0, 96), (7, 7, 180))
        g.box(STONE, (x, 0, 9), (10, 60, 6))
    g.box(STONE, (110, 0, 184), (190, 7, 7))
    for k, x in enumerate((50, 90, 130, 170)):
        p = sc.part(P.motion(swing=4.0, pivot=(x, 0, 181), axis=0.0, rate=0.3, phase=(0.1 + k * 0.27) % 1.0))
        p.box((WHITE, LILAC, WHITE, STONE)[k], (x, 0, 125), (30, 12, 100))
        p.box(PERI, (x, 0, 128), (32, 14, 7))
        p.box(STONE, (x, 0, 178), (22, 5, 5))


def vane_kiosk(sc):
    """PROP, TALL (no light): a well-head kiosk over a warm spring: four slim columns, a lilac and purple cap, a wisp of
    steam off the basin, and on the top a weather vane, a pale swan on an arrow, turning 30 degrees each way. 4.7 m."""
    g = sc.still()
    g.cyl(PERI, (0, 0, -150), 125, 175, steps=8)                     # 1.5 m of footing
    g.cyl(STONE, (0, 0, 25), 105, 12, steps=8)
    g.cyl(PERI, (0, 0, 37), 46, 70, steps=8)                         # the basin
    g.cyl(WATER, (0, 0, 107), 38, 4, steps=8)
    for sx in (-1, 1):
        for sy in (-1, 1):
            column(g, sx * 68, sy * 68, 37, 225, r=9)
    g.cyl(STONE, (0, 0, 262), 122, 18, steps=8)
    g.cyl(LILAC, (0, 0, 280), 116, 45, steps=8, top_r=58)
    g.cyl(PURPLE, (0, 0, 325), 58, 50, steps=8, top_r=7)
    g.cyl(STONE, (0, 0, 375), 5, 62, steps=5)                        # the vane's mast
    g.box(STONE, (0, 0, 402), (60, 5, 5))
    g.box(STONE, (0, 0, 402), (5, 60, 5))
    v = sc.part(P.motion(swing=30.0, pivot=(0, 0, 425), axis=P.UP, rate=0.12, phase=0.1))
    v.box(LILAC, (0, 0, 425), (96, 5, 7))
    v.box(LILAC, (-44, 0, 425), (14, 5, 26))
    v.box(WHITE, (16, 0, 438), (40, 5, 18))                          # the swan
    tilt_box(v, WHITE, (32, 0, 444), (42, 0, 466), 5, 6)
    v.box(WHITE, (47, 0, 467), (14, 5, 8))
    puffs(sc, [(0, 0, 140, 22), (14, -10, 172, 18)], pivot_drop=60.0, bob=-5.0)


def chess_table(sc):
    """PROP, LOW: a stone chess table with eight dark squares and seven big pieces, two cushioned stools, a third
    stool with a tray of cups. 1.2 m."""
    g = sc.still()
    g.box(PERI, (20, 0, -17), (360, 250, 46))                        # paving: 40 cm of footing
    g.cyl(STONE, (0, 0, 6), 20, 66, steps=8)
    g.box(STONE, (0, 0, 77), (112, 112, 10))
    for i in range(4):
        for j in range(4):
            if (i + j) % 2 == 0:
                g.box(PURPLE, (-39 + i * 26, -39 + j * 26, 84), (26, 26, 4))
    for x, y, col, h in ((-39, -13, WHITE, 26), (13, -39, WHITE, 34), (39, -13, WHITE, 22), (-13, -39, WHITE, 20),
                         (-13, 39, PERI, 30), (39, 13, PERI, 24), (13, 13, PERI, 36)):
        g.cyl(col, (x, y, 82), 8, h, steps=5, top_r=3)
    for sy in (-1, 1):
        g.cyl(STONE, (0, sy * 100, 6), 22, 40, steps=8)
        g.cyl(LILAC, (0, sy * 100, 46), 24, 7, steps=8)
    g.cyl(STONE, (140, 20, 6), 20, 44, steps=8)
    g.box(LILAC, (140, 20, 53), (50, 36, 6))
    for dx in (-12, 12):
        g.cyl(WHITE, (140 + dx, 20, 56), 6, 10, steps=5)


# ====================================================================================================== the hero
def cascade(sc):
    """HERO: THE CASCADE. A stepped cooling pyramid, 64 x 46 m at the foot: seven lavender terraces 7.6 m each with a
    lilac lip, rows of periwinkle and lit cream niches, and down the middle of every terrace a sheet of pale water
    between lilac cheeks: three white bands on each sheet rise and fall 70 cm, each a beat behind the one above it
    (a wave that runs down the pyramid), foam where each sheet lands, a pool at the foot. On top (53 m) a lit hall
    in a colonnade of fourteen columns under a stepped purple roof (61 m), and out of the roof a slow plume of steam
    to 89 m. Saved on the far master so it moves at any distance."""
    g = sc.still()
    N, H = 7, 760.0
    hw = [3200.0 - 420.0 * k for k in range(N)]
    hd = [2300.0 - 300.0 * k for k in range(N)]
    g.box(STONE, (0, 0, -150), (6400, 4600, 300))                    # 3 m of footing
    g.box(PERI, (0, -2560, 10), (1200, 480, 60))                     # the pool at the foot
    g.box(WATER, (0, -2560, 42), (1100, 400, 8))
    for sx in (-1, 1):
        g.box(LILAC, (sx * 760, -2420, 220), (200, 200, 440))        # two piers by the pool
        g.cyl(STONE, (sx * 760, -2420, 440), 60, 70, steps=6, top_r=110)
    for k in range(N):
        z0 = k * H
        g.box(STONE, (0, 0, z0 + H / 2.0), (2 * hw[k], 2 * hd[k], H))
        g.box(LILAC, (0, 0, z0 + H - 30.0), (2 * hw[k] + 80, 2 * hd[k] + 80, 60))       # the lip
        cw = 380.0 - 30.0 * k                                        # half the water's width
        m = (9 - k - 1) // 2                                         # niches on each side of the water
        span = hw[k] - cw - 260.0
        wn = min(300.0, span / m * 0.62)
        for sx in (-1, 1):
            for i in range(m):
                x = sx * (cw + 140.0 + (i + 0.5) * span / m)
                lit = (i + k) % 2 == 0
                g.box(CREAM if lit else PERI, (x, -hd[k] - 8.0, z0 + 320.0), (wn, 24, 430), glow=0.6 if lit else 0.0)
            g.box(LILAC, (sx * (cw + 34.0), -hd[k] - 16.0, z0 + 350.0), (68, 40, 700))   # the water's cheeks
            if k < 5:
                for y in (-hd[k] * 0.45, hd[k] * 0.25):
                    g.box(PERI, (sx * (hw[k] + 8.0), y, z0 + 320.0), (24, 300, 430))
        g.box(WATER, (0, -hd[k] - 10.0, z0 + 350.0), (2 * cw, 20, 700))                 # the sheet of water
        for j in range(3):                                           # its bands: the lower, the later
            o = (N - 1 - k) * 3 + (2 - j)
            b = sc.part(P.motion(bob=-70.0, rate=0.3, phase=(1.0 - o * 0.11) % 1.0))
            b.box(WHITE, (0, -hd[k] - 24.0, z0 + 150.0 + j * 220.0), (2 * cw - 70.0, 16, 70))
        f = sc.part(P.motion(bob=-14.0, rate=0.5, phase=(0.3 + k * 0.17) % 1.0))
        f.sphere(WHITE, (0, -hd[k] - 100.0, z0 + 36.0), cw * 0.9, scale=(1, 0.24, 0.16), steps=(3, 6))   # foam where it lands
        if k in (0, 2, 4):                                           # small round turrets on three terraces' front corners
            for sx in (-1, 1):
                tx, ty = sx * (hw[k] - 150.0), -(hd[k] - 150.0)
                g.cyl(STONE, (tx, ty, z0 + H), 110, 260, steps=6)
                g.cyl(PURPLE, (tx, ty, z0 + H + 260.0), 140, 120, steps=6, top_r=14)
    zt = N * H                                                       # 53.2 m: the top terrace's floor
    g.box(CREAM, (0, 20, zt + 250.0), (900, 560, 500), glow=0.5)     # the lit hall
    spots = [(x, sy * 440.0) for x in (-600, -300, 0, 300, 600) for sy in (-1, 1)] + [(sx * 600.0, y) for sx in (-1, 1) for y in (-150, 150)]
    for x, y in spots:
        column(g, x, y, zt, 500, r=46)
    g.box(LILAC, (0, 0, zt + 540.0), (1400, 1040, 80))
    g.box(STONE, (0, 0, zt + 610.0), (1160, 800, 60))
    g.box(LILAC, (0, 0, zt + 665.0), (880, 560, 50))
    g.cyl(PURPLE, (0, 0, zt + 690.0), 300, 90, steps=8, top_r=170)   # the roof's open crown: 61 m
    top = zt + 780.0
    for k, (x, y, dz, r) in enumerate(((0, 0, 220, 250), (120, 40, 520, 330), (-150, -30, 860, 400), (100, 80, 1260, 470),
                                       (-60, -60, 1720, 540), (180, 20, 2220, 600))):
        p = sc.part(P.motion(phase=(0.07 + k * 0.19) % 1.0, rate=0.07 + 0.01 * k, bob=-(60.0 + 30.0 * k), swing=1.2,
                             pivot=(0, 0, top), axis=90.0))
        p.sphere(STONE if k % 2 == 0 else WHITE, (x, y, top + dz), r, steps=(4, 7))


JOBS = [
    ('SM_S1D6_BathHouse', bath_house, SMALL),
    ('SM_S1D6_WinterGarden', winter_garden, SMALL),
    ('SM_S1D6_PumpRoom', pump_room, SMALL),
    ('SM_S1D6_Colonnade', colonnade, SMALL),
    ('SM_S1D6_TeaPavilion', tea_pavilion, SMALL),
    ('SM_S1D6_CabinTerrace', cabin_terrace, SMALL),
    ('SM_S1D6_Hydro', hydro, SMALL),
    ('SM_S1D6_PlungePool', plunge_pool, LOW),
    ('SM_S1D6_DeckChairs', deck_chairs, STILL),
    ('SM_S1D6_Urn', urn, LOW),
    ('SM_S1D6_Fountain', fountain, LOW),
    ('SM_S1D6_GlobeLamp', globe_lamp, STILL),
    ('SM_S1D6_TowelCart', towel_cart, LOW),
    ('SM_S1D6_VaneKiosk', vane_kiosk, LOW),
    ('SM_S1D6_ChessTable', chess_table, STILL),
    ('SM_S1D6_Cascade', cascade, dict(far=True, grow=(60, 60, 300), grow_down=(60, 60, 0))),
]


def run(only=None):
    P.build_core()                              # loads the two masters; never rebuilds them
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
