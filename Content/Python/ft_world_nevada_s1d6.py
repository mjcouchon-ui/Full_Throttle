"""Nevada, Stage 1, builder s1d6: district 6 "Syndicate Range" (14.5 to 18.5 km). /Game/World/Nevada/SM_S1D6_*.
A secret range in poured concrete: battered slit-window bunkers, earth-covered arched magazines, blast walls, panelled
radomes on drum towers, an empty test stand. Syndicate black, pale domes, steady cyan strips, purple doors, white
panels. No chrome, no flames, no vehicles, no weapons, no rockets, no beams. The hero is THE BIG EAR: a 42 m white
panelled dish on a black fork mount that nods slowly.
Placed by the block C:/ftwork/s1/L2_s1d6.inl. Run: import ft_world_nevada_s1d6 as K; K.run()
"""
import math
import ft_art
import ft_people as P
from ft_desert import rod, tilt_box            # tilt_box(m, col, (x,y,z) from, (x,y,z) to, width, depth)

DIR = '/Game/World/Nevada'
STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))        # a piece with no moving part: true bounds
SMALL = dict(grow=(30, 30, 20), grow_down=(30, 30, 0))   # a piece whose moving parts stay close to it

SYN, DOME, CYAN, PURP, WHITE = '#1c1830', '#c9c4d6', '#2ee6ff', '#7b3bd6', '#fff6ea'   # the district's five
EARTH, SAGE = '#c9805a', '#8fb86a'                       # the land a piece stands on (the magazines' cover) and its scrub
FOOT = 200.0                                             # how far a building's footing goes down
GL, GW = 0.6, 0.4                                        # glow: light strips, window bands (steady; they stand 24 m out or more)


# ------------------------------------------------------------------------------------------ helpers
def B(g, col, x0, x1, y0, y1, z0, z1, glow=0.0):
    """A box by its corners."""
    g.box(col, ((x0 + x1) / 2.0, (y0 + y1) / 2.0, (z0 + z1) / 2.0), (abs(x1 - x0), abs(y1 - y0), abs(z1 - z0)), glow=glow)


def ccw(pts):
    a = 0.0
    for (x0, y0), (x1, y1) in zip(pts, pts[1:] + pts[:1]):
        a += x0 * y1 - x1 * y0
    return list(pts) if a >= 0 else list(reversed(pts))


def trap(g, col, x0, x1, z0, z1, y0, y1, batter, glow=0.0):
    """A battered wall seen from the road: x0..x1 at the foot, `batter` narrower each side at the top, y0..y1 deep."""
    g.side(col, ccw([(x0, z0), (x1, z0), (x1 - batter, z1), (x0 + batter, z1)]), y0, y1, glow=glow)


def arch(cx, z0, w, spring, n=8):
    """The outline of an arched front: w wide, straight up to `spring` over z0, a half circle over that."""
    r = w / 2.0
    pts = [(cx - r, z0), (cx + r, z0)]
    for k in range(n + 1):
        a = math.pi * k / float(n)
        pts.append((cx + r * math.cos(a), z0 + spring + r * math.sin(a)))
    if spring <= 0.0:
        pts = pts[2:]
    return ccw(pts)


def mushroom(g, x, y, z, h=80.0, r=28.0):
    """A roof ventilator: a pale stalk under a dark cap."""
    g.cyl(DOME, (x, y, z), r, h, steps=8)
    g.cyl(SYN, (x, y, z + h), r * 1.7, 18, steps=8)


def whip(g, x, y, z, h):
    """A whip aerial with a pale bead on its tip."""
    g.cyl(SYN, (x, y, z), 5, h, steps=6)
    g.sphere(DOME, (x, y, z + h), 13, steps=(3, 5))


def spinner(sc, x, y, z, rate=0.7):
    """A cup anemometer on a short stalk: three arms that go round and round the stalk."""
    sc.still().cyl(SYN, (x, y, z), 5, 60, steps=6)
    sp = sc.part(P.motion(base=(x, y), travel=(-1.0, rate)))
    for j in range(3):
        a = 2.0 * math.pi * j / 3.0
        sp.box(WHITE, (x + 24 * math.cos(a), y + 24 * math.sin(a), z + 62), (48, 8, 8), rot=(0, math.degrees(a), 0))
        sp.box(PURP, (x + 50 * math.cos(a), y + 50 * math.sin(a), z + 62), (18, 18, 18), rot=(0, math.degrees(a), 0))


# ------------------------------------------------------------------------------------------ buildings
def radome_tower(sc):
    """A panelled radome on a battered drum tower, a stair winding round it, an instrument annex. 12.7 x 8.8 m, 19 m."""
    g = sc.still()
    tx = -190.0
    g.cyl(DOME, (tx, 0, -FOOT), 410, FOOT + 90, steps=12)                    # plinth
    g.cyl(SYN, (tx, 0, 90), 330, 930, steps=12, top_r=300)                   # the drum
    g.cyl(DOME, (tx, 0, 470), 326, 44, steps=12)                             # a pale belt
    g.cyl(CYAN, (tx, 0, 880), 312, 26, steps=12, glow=GL)                    # the light strip, all round
    g.cyl(DOME, (tx, 0, 1020), 470, 34, steps=12)                            # gallery
    g.cyl(SYN, (tx, 0, 1054), 384, 62, steps=12)                             # collar
    g.sphere(WHITE, (tx, 0, 1300), 430, steps=(6, 12))                       # the radome
    g.cyl(DOME, (tx, 0, 1294), 436, 12, steps=12)                            # panel seams
    g.cyl(DOME, (tx, 0, 1509), 379, 12, steps=12)
    g.cyl(DOME, (tx, 0, 1708), 76, 26, steps=8)
    whip(g, tx, 0, 1730, 170)
    for k in range(16):                                                      # the stair: sixteen slabs winding up
        a = math.radians(150 + k * 15)
        g.box(DOME, (tx + 356 * math.cos(a), 356 * math.sin(a), 70 + k * 62), (124, 66, 22), rot=(0, 150 + k * 15, 0))
    B(g, DOME, tx - 86, tx + 86, -342, -326, 90, 372)                        # door frame and the purple door
    B(g, PURP, tx - 64, tx + 64, -350, -338, 90, 340)
    B(g, CYAN, tx - 36, tx + 36, -322, -306, 640, 664, glow=GL)              # a slit over the stair
    # the annex: a low black block, a pale slab roof, a slit window, a door, a conduit to the drum
    B(g, SYN, 200, 650, -200, 200, -FOOT, 300)
    B(g, DOME, 180, 670, -222, 222, 300, 330)
    B(g, CYAN, 250, 480, -206, -200, 196, 222, glow=GL)
    B(g, DOME, 240, 490, -230, -200, 228, 240)
    B(g, PURP, 530, 620, -208, -200, 0, 196)
    B(g, WHITE, 215, 245, -206, -200, 60, 150)
    B(g, DOME, 100, 210, -30, 30, 120, 160)
    B(g, PURP, 200, 650, -204, -200, 268, 290)
    mushroom(g, 300, 90, 330)
    spinner(sc, 560, 110, 330)


def magazines(sc):
    """Three earth-covered magazines in a row: arched concrete headwalls with purple double doors let into one long
    berm of the desert's own earth, a ventilator on each vault, lightning poles, a hand-cart line in front. 27.6 x 9.4 m."""
    g = sc.still()
    trap(g, EARTH, -1380, 1380, -FOOT, 330, -260, 380, 300)                  # the berm
    B(g, DOME, -1380, 1380, -440, -352, -FOOT, 4)                            # the apron
    for cx in (-820, 0, 820):
        g.side(EARTH, arch(cx, 0, 800, 230), -300, 380)                      # the vault under its earth
        g.side(DOME, arch(cx, -FOOT, 660, 190 + FOOT), -352, -300)           # the headwall
        g.side(SYN, arch(cx, 0, 440, 150), -360, -352)                       # the portal
        B(g, PURP, cx - 170, cx - 6, -368, -360, 0, 290)                     # two leaves
        B(g, PURP, cx + 6, cx + 170, -368, -360, 0, 290)
        B(g, WHITE, cx - 150, cx + 150, -373, -368, 128, 150)                # the bar across them
        B(g, CYAN, cx - 150, cx + 150, -358, -352, 420, 444, glow=GL)        # the strip over the portal
        B(g, WHITE, cx + 236, cx + 300, -357, -352, 150, 240)                # a blank number plate
        g.cyl(DOME, (cx, 120, 600), 30, 150, steps=8)                        # the vault's ventilator
        g.cyl(SYN, (cx, 120, 750), 52, 22, steps=8)
    for (x, y, z, r) in ((-410, -150, 340, 44), (410, 60, 340, 40), (-1210, 40, 250, 46), (1190, -90, 260, 42), (-640, 250, 560, 36), (160, 280, 590, 34)):
        g.sphere(SAGE, (x, y, z), r, steps=(3, 6))                           # scrub on the cover
    for x in (-410, 410):                                                    # lightning poles
        g.cyl(SYN, (x, 250, 300), 6, 760, steps=6)
        g.sphere(DOME, (x, 250, 1070), 16, steps=(3, 5))
    B(g, DOME, -1320, 1320, -560, -440, -100, 2)                             # the hand-cart line
    B(g, SYN, -1300, 1300, -478, -470, 2, 12)
    B(g, SYN, -1300, 1300, -536, -528, 2, 12)
    B(g, SYN, 300, 520, -545, -461, 22, 40)                                  # a flat trolley with pale cases
    B(g, WHITE, 320, 400, -533, -473, 40, 110)
    B(g, PURP, 320, 400, -538, -533, 68, 84)
    B(g, DOME, 412, 500, -530, -476, 40, 92)


def command_bunker(sc):
    """The command bunker: a battered black block with one long slit window, buttresses, a blast porch, a smaller
    block on its roof and a periscope that turns slowly. 14 x 8.9 m, 13.5 m with its aerials."""
    g = sc.still()
    trap(g, SYN, -700, 700, -FOOT, 420, -380, 380, 90)
    B(g, DOME, -640, 640, -410, 410, 420, 462)                               # roof slab
    B(g, DOME, -640, 640, -410, -398, 462, 520)                              # parapet
    trap(g, SYN, -360, 360, 462, 790, -230, 280, 46)                         # the upper block
    B(g, DOME, -340, 340, -256, 306, 790, 826)
    B(g, CYAN, -470, 300, -386, -380, 286, 312, glow=GL)                     # the slit, and its hood
    B(g, DOME, -500, 330, -422, -380, 318, 334)
    B(g, CYAN, -270, 270, -236, -230, 660, 690, glow=GL)
    B(g, DOME, -290, 290, -262, -230, 696, 710)
    for x in (-600, -340, -80, 180):                                         # buttresses
        tilt_box(g, SYN, (x, -470, -FOOT), (x, -384, 250), 70, 70)
    B(g, DOME, 360, 600, -470, -380, -FOOT, 250)                             # the blast porch and its door
    B(g, PURP, 420, 540, -478, -470, 0, 210)
    for x in (-520, -260, 0):                                                # blank white plates
        B(g, WHITE, x, x + 80, -386, -380, 140, 220)
    B(g, PURP, -660, 356, -384, -380, 30, 60)                                # a purple band along the foot
    g.cyl(SYN, (200, 40, 826), 24, 300, steps=8)                             # the periscope
    h = sc.part(P.motion(swing=70, pivot=(200, 40, 1150), axis=P.UP, rate=0.06))
    h.box(SYN, (200, 30, 1160), (80, 110, 60))
    h.box(WHITE, (200, -27, 1160), (56, 6, 32))
    h.box(DOME, (200, 20, 1194), (96, 140, 8))
    whip(g, -300, 200, 826, 500)
    whip(g, -240, 250, 826, 380)
    mushroom(g, -520, 150, 462)
    mushroom(g, 520, 200, 462, h=110)


def test_stand(sc):
    """A test stand: a braced gantry of three decks between two battered blast walls, an EMPTY cradle of two clamp
    frames in front of it, a hoist hook swinging under the crane beam, a pale tank on legs. 21.2 x 9.8 m, 17.7 m."""
    OX = -230.0
    g = sc.still(at=(OX, 0, 0))
    for s in (-1, 1):                                                        # the two blast walls
        trap(g, DOME, s * 700 - 130, s * 700 + 130, -FOOT, 940, -380, 380, 50)
        B(g, SYN, s * 700 - 92, s * 700 + 92, -392, 392, 940, 976)
        B(g, PURP, s * 700 - 96, s * 700 + 96, -385, -380, 640, 700)
        B(g, WHITE, s * 700 - 94, s * 700 + 94, -385, -380, 700, 728)
        B(g, CYAN, s * 700 - 12, s * 700 + 12, -386, -380, 200, 560, glow=GL)
    B(g, DOME, -570, 570, -420, 420, -FOOT, 30)                              # the pad
    for sx in (-1, 1):                                                       # the gantry
        for y in (-120, 260):
            B(g, SYN, sx * 240 - 22, sx * 240 + 22, y - 22, y + 22, 30, 1700)
    bays = ((30, 560), (578, 1080), (1098, 1600))
    for i, (z0, z1) in enumerate(bays):
        d = 1 if i % 2 == 0 else -1
        tilt_box(g, SYN, (-240 * d, -120, z0), (240 * d, -120, z1), 16, 16)
        tilt_box(g, SYN, (240 * d, 260, z0), (-240 * d, 260, z1), 16, 16)
        for sx in (-1, 1):
            tilt_box(g, SYN, (sx * 240, -120 if d > 0 else 260, z0), (sx * 240, 260 if d > 0 else -120, z1), 16, 16)
    for z in (560, 1080, 1600):                                              # decks, each with a light strip and a rail
        B(g, DOME, -300, 300, -180, 320, z, z + 18)
        B(g, CYAN, -280, 280, -186, -180, z - 4, z + 14, glow=GL)
        B(g, SYN, -300, 300, -183, -176, z + 96, z + 104)
        for sx in (-1, 1):
            B(g, SYN, sx * 296 - 4, sx * 296 + 4, -184, -176, z + 18, z + 100)
    tilt_box(g, DOME, (292, -100, 30), (292, 240, 578), 70, 12)              # stairs up the right side
    tilt_box(g, DOME, (292, 240, 578), (292, -100, 1098), 70, 12)
    tilt_box(g, DOME, (292, -100, 1098), (292, 240, 1618), 70, 12)
    B(g, SYN, -30, 30, -560, 300, 1700, 1770)                                # the crane beam over the cradle
    B(g, PURP, -50, 50, -470, -390, 1676, 1700)
    hk = sc.part(P.motion(swing=4, pivot=(OX, -430, 1676), axis=0.0, rate=0.22), at=(OX, 0, 0))
    hk.box(SYN, (0, -430, 1546), (8, 8, 260))
    hk.box(PURP, (0, -430, 1396), (44, 30, 52))
    for z in (700, 1240):                                                    # the empty cradle: two clamp frames
        for sx in (-1, 1):
            B(g, PURP, sx * 170 - 20, sx * 170 + 20, -420, -120, z, z + 40)
            B(g, SYN, sx * 132 - 18, sx * 132 + 18, -310, -240, z - 12, z + 52)
        B(g, WHITE, -190, 190, -440, -400, z, z + 40)
    g.sphere(DOME, (1080, 120, 430), 210, steps=(5, 10))                      # the tank on its legs
    g.cyl(PURP, (1080, 120, 420), 214, 20, steps=10)
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.cyl(SYN, (1080 + sx * 130, 120 + sy * 130, -100), 10, 440, steps=6)
    g.cyl(DOME, (1080, 120, 630), 10, 70, steps=6)
    rod(g, DOME, (890, 120, 300), (262, 120, 300), 14, steps=6)
    B(g, PURP, -690, -610, -388, -380, 0, 200)                               # a door in the left wall's end


def gate_tower(sc):
    """The gate: a battered four-storey tower with a lit cab, a lodge with a purple awning, a striped boom standing
    raised (it rocks a little in the wind; no searchlight). 14.8 x 7.4 m, 17.8 m with its aerial."""
    g = sc.still()
    trap(g, SYN, -260, 260, -FOOT, 1050, -240, 240, 40)
    B(g, DOME, -360, 360, -340, 340, 1050, 1082)
    B(g, SYN, -330, 330, -310, 310, 1082, 1340)                              # the cab
    B(g, CYAN, -336, 336, -316, 316, 1200, 1262, glow=GW)                    # its window band, all round
    for sx in (-1, 1):
        for sy in (-1, 1):
            B(g, SYN, sx * 336 - 16, sx * 336 + 16, sy * 316 - 16, sy * 316 + 16, 1200, 1262)
        B(g, SYN, sx * 112 - 10, sx * 112 + 10, -322, -310, 1200, 1262)
    B(g, DOME, -390, 390, -370, 370, 1340, 1376)
    g.cyl(DOME, (-150, 60, 1376), 90, 50, steps=8, top_r=60)
    spinner(sc, -150, 60, 1426)
    whip(g, 180, 120, 1376, 400)
    for z in (300, 560, 820):                                                # stair slits
        B(g, CYAN, -14, 14, -246, -240, z, z + 110, glow=GL)
    B(g, PURP, -70, 70, -248, -240, 0, 220)
    B(g, PURP, -222, 222, -245, -240, 940, 990)
    B(g, WHITE, -222, 222, -245, -240, 990, 1010)
    B(g, SYN, 320, 720, -260, 160, -FOOT, 290)                               # the lodge
    B(g, DOME, 300, 740, -300, 180, 290, 320)
    B(g, CYAN, 380, 560, -266, -260, 150, 230, glow=GW)
    B(g, PURP, 600, 690, -266, -260, 0, 200)
    B(g, PURP, 370, 570, -340, -260, 240, 250)                               # awning, on two posts
    for x in (376, 564):
        B(g, SYN, x - 4, x + 4, -338, -330, 0, 240)
    px, py, pz = -400.0, -330.0, 120.0                                       # the boom, raised
    B(g, DOME, px - 30, px + 30, py - 30, py + 30, -100, pz + 10)
    bm = sc.part(P.motion(swing=2.5, pivot=(px, py, pz), axis=90.0, rate=0.2))
    dx, dz = math.cos(math.radians(118)), math.sin(math.radians(118))
    for k in range(6):
        a0, a1 = k * 110 - 60, k * 110 + 50
        tilt_box(bm, PURP if k % 2 else WHITE, (px + dx * a0, py, pz + dz * a0), (px + dx * a1, py, pz + dz * a1), 16, 16)
    tilt_box(bm, SYN, (px - dx * 150, py, pz - dz * 150), (px - dx * 62, py, pz - dz * 62), 44, 44)
    for x in (-640, -540):                                                   # bollards
        g.cyl(WHITE, (x, -300, -60), 14, 150, steps=6)
        g.cyl(PURP, (x, -300, 60), 15, 18, steps=6)


def aerial_hut(sc):
    """The aerial hut: a black hut under a pale barrel roof, a mast of crossed arms with a loop that turns, inside a
    fence of nine dipoles on a concrete kerb. 15.2 x 7.6 m, 15 m."""
    g = sc.still()
    B(g, SYN, -230, 230, -60, 300, -FOOT, 262)
    g.side(DOME, arch(0, 260, 500, 0.0, 6), -80, 320)
    B(g, CYAN, -150, 30, -66, -60, 150, 180, glow=GL)
    B(g, PURP, 90, 190, -68, -60, 0, 200)
    B(g, WHITE, -205, -175, -66, -60, 60, 130)
    B(g, PURP, -230, 230, -64, -60, 226, 244)
    g.cyl(SYN, (0, 120, 480), 14, 900, steps=6)                              # the mast and its crossed arms
    for (z, w, yaw) in ((800, 360, 0), (980, 300, 60), (1160, 240, 120)):
        g.box(DOME, (0, 120, z), (w, 8, 8), rot=(0, yaw, 0))
    lp = sc.part(P.motion(base=(0.0, 120.0), travel=(-1.0, 0.12)))           # the loop: once round in eight seconds
    lp.box(WHITE, (0, 120, 1386), (130, 8, 8))
    lp.box(WHITE, (0, 120, 1506), (130, 8, 8))
    for sx in (-1, 1):
        lp.box(WHITE, (sx * 61, 120, 1446), (8, 8, 128))
    poles = [(-640, -425), (-320, -425), (0, -425), (320, -425), (640, -425), (-685, -150), (685, -150), (-685, 150), (685, 150)]
    for i, (x, y) in enumerate(poles):                                       # the dipoles
        g.cyl(DOME, (x, y, -100), 7, 620, steps=6)
        g.box(PURP, (x, y, 500), (22, 22, 26))
        g.box(WHITE, (x, y, 518), (240, 8, 8) if i % 2 == 0 else (8, 240, 8))
    B(g, DOME, -700, 700, -440, -410, -60, 24)                               # the kerb
    for sx in (-1, 1):
        B(g, DOME, sx * 685 - 15, sx * 685 + 15, -410, 300, -60, 24)
    tilt_box(g, SYN, (0, 120, 700), (-640, -425, 500), 6, 6)                 # two feeders
    tilt_box(g, SYN, (0, 120, 700), (640, -425, 500), 6, 6)
    mushroom(g, -150, 240, 380, h=110)


def track_house(sc):
    """The tracking house: a battered blockhouse with a parapet, two white dishes on its roof nodding out of step, a
    small pale dome between them. 10.4 x 6.5 m, 9.3 m."""
    g = sc.still()
    trap(g, SYN, -520, 520, -FOOT, 380, -300, 300, 60)
    B(g, DOME, -480, 480, -320, 320, 380, 412)
    B(g, DOME, -480, 480, -320, -306, 412, 470)
    B(g, CYAN, -330, 300, -306, -300, 250, 276, glow=GL)
    B(g, DOME, -350, 320, -332, -300, 282, 296)
    B(g, PURP, 350, 440, -308, -300, 0, 200)
    B(g, PURP, -480, 330, -304, -300, 30, 58)
    for x in (-420, -300):
        B(g, WHITE, x, x + 70, -306, -300, 110, 190)
    g.sphere(DOME, (0, 120, 412), 110, steps=(5, 10))
    g.cyl(PURP, (0, 120, 412), 114, 16, steps=10)
    whip(g, 420, 240, 412, 420)
    mushroom(g, -420, 220, 412)
    for (x, yaw, ph) in ((-250.0, 18.0, 0.0), (250.0, -24.0, 0.5)):
        g.cyl(DOME, (x, 40, 412), 60, 190, steps=8, top_r=44)
        g.box(SYN, (x, 40, 618), (150, 64, 60), rot=(0, yaw, 0))
        d = sc.part(P.motion(swing=9, pivot=(x, 40.0, 650.0), axis=yaw, rate=0.07, phase=ph))
        d.base = ft_art._xf((x, 40.0, 650.0), (0.0, yaw, -48.0), (1.0, 1.0, 1.0))    # the dish's own frame: +Z is where it looks
        d.box(SYN, (0, 0, 14), (90, 90, 80))
        d.cyl(WHITE, (0, 0, 40), 40, 110, steps=10, top_r=235)
        d.cyl(SYN, (0, 0, 150), 6, 150, steps=6)
        d.box(CYAN, (0, 0, 310), (26, 26, 26), glow=GL)


# ------------------------------------------------------------------------------------------ props
def blast_wall(sc):
    """A length of blast wall: black, buttressed front and back, a pale cap, a cyan strip under it, a purple and white
    band along its foot. 12.6 x 5.4 m, 4.9 m."""
    g = sc.still()
    B(g, SYN, -600, 600, -60, 60, -FOOT, 450)
    B(g, DOME, -620, 620, -84, 84, 450, 482)
    B(g, CYAN, -540, 540, -66, -60, 388, 410, glow=GL)
    for k in range(5):
        x = -480 + k * 240
        tilt_box(g, DOME, (x, -260, -FOOT), (x, -70, 380), 70, 70)
        tilt_box(g, DOME, (x, 260, -FOOT), (x, 70, 380), 70, 70)
    for k in range(8):
        B(g, PURP if k % 2 else WHITE, -600 + k * 150, -450 + k * 150, -64, -60, 40, 86)
    for sx in (-1, 1):
        B(g, DOME, sx * 630 - 50, sx * 630 + 50, -100, 100, -FOOT, 490)


def wind_sock(sc):
    """A sandbag ring round a weather post: a purple and white windsock swinging on its mast, a white louvred screen
    on legs, a cup anemometer going round. 3.1 m across, 5.6 m."""
    g = sc.still()
    g.cyl(DOME, (0, 0, -60), 150, 62, steps=10)
    for c in range(2):
        for k in range(9):
            a = 2.0 * math.pi * (k + 0.5 * c) / 9.0
            g.box(DOME if (k + c) % 2 else WHITE, (112 * math.cos(a), 112 * math.sin(a), 15 + c * 26), (72, 36, 26), rot=(0, math.degrees(a) + 90.0, 0))
    g.cyl(SYN, (0, 0, 0), 7, 560, steps=6)
    s = sc.part(P.motion(swing=16, pivot=(0, 0, 540), axis=P.UP, rate=0.3))
    s.box(SYN, (8, 0, 540), (8, 46, 46))
    tilt_box(s, PURP, (10, 0, 540), (85, 0, 534), 40, 40)
    tilt_box(s, WHITE, (85, 0, 534), (155, 0, 524), 32, 32)
    tilt_box(s, PURP, (155, 0, 524), (215, 0, 510), 24, 24)
    for sx in (-1, 1):                                                       # the screen on its legs
        for sy in (-1, 1):
            B(g, SYN, -50 + sx * 22 - 3, -50 + sx * 22 + 3, -30 + sy * 22 - 3, -30 + sy * 22 + 3, 0, 130)
    B(g, WHITE, -78, -22, -58, -2, 130, 186)
    B(g, DOME, -84, -16, -64, 4, 186, 194)
    g.cyl(SYN, (55, 40, 0), 5, 250, steps=6)
    spinner(sc, 55, 40, 250)


def camera_mast(sc):
    """A camera mast: a black pole on a concrete foot, a white camera under a sunshade turning slowly at its head, a
    smaller fixed one on the cross arm, a purple panel and a junction box. 7 m."""
    g = sc.still()
    B(g, DOME, -40, 40, -40, 40, -80, 50)
    g.cyl(SYN, (0, 0, 50), 9, 600, steps=6)
    B(g, PURP, -18, 18, -26, -8, 140, 190)
    g.box(PURP, (0, -16, 430), (100, 6, 64), rot=(0, 0, -30))
    B(g, SYN, -60, 60, -6, 6, 596, 606)
    B(g, WHITE, -72, -44, -50, 12, 560, 592)
    B(g, SYN, -66, -50, -56, -50, 566, 586)
    h = sc.part(P.motion(swing=55, pivot=(0, 0, 660), axis=P.UP, rate=0.07))
    h.box(WHITE, (0, -14, 672), (46, 96, 40))
    h.box(DOME, (0, -22, 696), (58, 124, 8))
    h.box(SYN, (0, -64, 672), (30, 6, 26))
    h.box(CYAN, (0, -68, 672), (12, 4, 12))


def warn_board(sc):
    """A warning board with nothing written on it: five purple and white bands on a braced frame, a steady cyan strip
    under its hood, a blank plate below. 3.1 m wide, 3.4 m."""
    g = sc.still()
    for sx in (-1, 1):
        B(g, SYN, sx * 140 - 6, sx * 140 + 6, -6, 6, -80, 330)
        B(g, DOME, sx * 140 - 22, sx * 140 + 22, -22, 22, -80, 16)
        tilt_box(g, SYN, (sx * 140, 96, -60), (sx * 140, 4, 240), 8, 8)
        B(g, DOME, sx * 140 - 16, sx * 140 + 16, 80, 112, -80, 10)
    for k in range(5):
        B(g, PURP if k % 2 == 0 else WHITE, -134, 134, -10, -2, 130 + k * 36, 166 + k * 36)
    B(g, DOME, -150, 150, -40, 8, 330, 340)
    B(g, CYAN, -130, 130, -14, -6, 314, 326, glow=GL)
    B(g, WHITE, -60, 60, -9, -3, 56, 108)


def pylons(sc):
    """Two marker pylons of pale concrete, one tall, one short, each with one steady cyan strip up its road face and a
    purple band at its foot, a conduit between them. 2.6 m wide, 4.5 m."""
    g = sc.still()
    for (x, h) in ((-75, 430), (85, 270)):
        trap(g, DOME, x - 44, x + 44, -100, h, -32, 32, 14)
        B(g, SYN, x - 36, x + 36, -38, 38, h, h + 16)
        B(g, CYAN, x - 9, x + 9, -38, -32, 70, h - 50, glow=GL)
        B(g, PURP, x - 40, x + 40, -36, -32, 24, 50)
    B(g, DOME, -50, 60, -12, 12, 150, 174)
    B(g, WHITE, 62, 108, -36, -32, 196, 236)


def cowls(sc):
    """Three ventilator cowls on a slab: a bent one, a tall one whose head turns with the wind behind its white vane,
    a short one under a cone cap. 3.8 m wide, 4.3 m."""
    g = sc.still()
    B(g, DOME, -190, 190, -80, 80, -80, 30)
    B(g, PURP, -190, 190, -84, -80, 6, 22)
    B(g, SYN, -40, 10, -60, -20, 30, 44)
    g.cyl(DOME, (-110, 0, 30), 34, 260, steps=8)                             # the bent one
    rod(g, DOME, (-110, 20, 300), (-110, -78, 312), 50, steps=8)
    rod(g, SYN, (-110, -78, 312), (-110, -84, 313), 40, steps=8)
    g.cyl(DOME, (20, 20, 30), 34, 340, steps=8)                              # the tall one: its head turns
    t = sc.part(P.motion(swing=35, pivot=(20, 20, 380), axis=P.UP, rate=0.09))
    rod(t, DOME, (20, 60, 380), (20, -60, 392), 52, steps=8)
    rod(t, SYN, (20, -60, 392), (20, -66, 393), 42, steps=8)
    t.box(WHITE, (20, 108, 392), (6, 90, 64))
    g.cyl(DOME, (130, 10, 30), 30, 140, steps=8)                             # the short one
    g.cyl(SYN, (130, 10, 170), 62, 40, steps=8, top_r=10)


def cal_target(sc):
    """A calibration target for the range's cameras: a chequered purple and white board leaning back on an A-frame
    (a pattern, no sign), a white corner reflector on a post beside it. 4 m wide, 4.5 m."""
    g = sc.still()
    for sx in (-1, 1):
        tilt_box(g, SYN, (sx * 140, -72, -60), (sx * 140, -36, 160), 10, 10)
        tilt_box(g, SYN, (sx * 140, 150, -60), (sx * 140, 36, 440), 10, 10)
        B(g, DOME, sx * 140 - 20, sx * 140 + 20, -92, -52, -80, 10)
        B(g, DOME, sx * 140 - 20, sx * 140 + 20, 130, 170, -80, 10)
    g.cyl(SYN, (250, 20, -60), 6, 390, steps=6)                              # the reflector: three plates square to each other
    g.box(WHITE, (250, 65, 375), (90, 6, 90))
    g.box(WHITE, (205, 20, 375), (6, 90, 90))
    g.box(WHITE, (250, 20, 330), (90, 90, 6))
    bd = sc.part(P.motion(phase=0.5))                                        # still (rate 0): only a model of its own, leaning back 15 degrees
    bd.base = ft_art._xf((0.0, 0.0, 300.0), (0.0, 0.0, 15.0), (1.0, 1.0, 1.0))
    bd.box(WHITE, (0, 0, 0), (300, 10, 300))
    bd.box(SYN, (0, 4, 0), (316, 8, 316))
    for i in range(3):
        for j in range(3):
            if (i + j) % 2 == 0:
                bd.box(PURP, ((i - 1) * 100, -7, (j - 1) * 100), (100, 4, 100))


def tank_traps(sc):
    """LOW: five concrete tank-trap pyramids in a row along a black kerb beam, two with purple caps. 9.4 m, 1.3 m."""
    g = sc.still()
    for k in range(5):
        x, y = (k - 2) * 190.0, (k % 2) * 30.0 - 15.0
        g.cyl(DOME if k % 2 else WHITE, (x, y, -50), 82, 170, steps=4, top_r=16, rot=(0, 45 + k * 9, 0))
        if k % 2:
            g.box(PURP, (x, y, 125), (32, 32, 10))
    B(g, SYN, -470, 470, 62, 86, -50, 14)


def cable_drums(sc):
    """LOW: a cable drum wound with purple cable and a strapped stack of conduit on a slab. 5.2 m, 1.7 m."""
    g = sc.still()
    B(g, DOME, -260, 260, -90, 90, -40, 6)
    rod(g, WHITE, (-150, -52, 86), (-150, -40, 86), 80, steps=10)
    rod(g, WHITE, (-150, 40, 86), (-150, 52, 86), 80, steps=10)
    rod(g, PURP, (-150, -40, 86), (-150, 40, 86), 50, steps=10)
    rod(g, SYN, (-150, -58, 86), (-150, 58, 86), 12, steps=6)
    B(g, SYN, -240, -200, -40, 40, 6, 22)
    B(g, SYN, -100, -60, -40, 40, 6, 22)
    for (z, ys) in ((22, (-48, -16, 16, 48)), (50, (-32, 0, 32)), (78, (-16, 16))):
        for y in ys:
            rod(g, DOME, (-30, y, z), (250, y, z), 16, steps=6)
    for x in (40, 180):
        B(g, PURP, x - 6, x + 6, -68, 68, 4, 98)


def hatch(sc):
    """LOW: a shaft hatch: a concrete slab, a round purple lid with a hand wheel, a capped breather pipe, two striped
    bollards. 3.6 m, 1.5 m."""
    g = sc.still()
    B(g, DOME, -130, 130, -110, 110, -50, 46)
    g.cyl(PURP, (-20, 0, 46), 72, 14, steps=10)
    g.cyl(SYN, (-20, 0, 60), 26, 8, steps=8)
    g.cyl(DOME, (90, 60, 46), 14, 96, steps=6)
    g.cyl(SYN, (90, 60, 142), 26, 12, steps=6)
    for x in (-165, 165):
        g.cyl(WHITE, (x, -125, -40), 13, 130, steps=6)
        g.cyl(PURP, (x, -125, 60), 14, 18, steps=6)


# ------------------------------------------------------------------------------------------ the hero
def big_ear(sc):
    """THE BIG EAR: a white panelled dish 38 m across on a black fork mount, a turntable and a pale concrete drum,
    its rim 42 m up. A cyan feed horn at its focus on four struts. The dish nods slowly (6 degrees each way, once in
    twenty seconds). A stair winds round the drum; a control hut stands beside it. Faces the road."""
    g = sc.still()
    ZT = 2300.0                                                              # the trunnion: where the dish nods
    g.cyl(DOME, (0, 0, -FOOT), 900, FOOT + 300, steps=14)                    # the drum base
    g.cyl(SYN, (0, 0, 300), 912, 30, steps=14)
    g.cyl(DOME, (0, 0, 330), 640, 600, steps=14, top_r=520)                  # the tower
    g.cyl(CYAN, (0, 0, 760), 560, 30, steps=14, glow=GL)                     # its light strip
    g.cyl(DOME, (0, 0, 930), 760, 22, steps=14)                              # catwalk
    g.cyl(SYN, (0, 0, 952), 540, 190, steps=14)                              # turntable
    B(g, SYN, -580, 580, -240, 240, 1142, 1400)                              # the yoke and the two arms of the fork
    for sx in (-1, 1):
        B(g, SYN, sx * 470 - 95, sx * 470 + 95, -210, 210, 1400, ZT + 150)
        B(g, PURP, sx * 470 - 99, sx * 470 + 99, -214, 214, 1700, 1760)
        rod(g, DOME, (sx * 372, 0, ZT), (sx * 600, 0, ZT), 120, steps=10)    # trunnion bearings
    for k in range(14):                                                      # the stair round the drum's tower
        a = math.radians(170 + k * 14)
        rr = 710.0 - 120.0 * k * 44.0 / 600.0                               # the tower narrows: the stair keeps to it
        g.box(DOME, (rr * math.cos(a), rr * math.sin(a), 330 + k * 44), (190, 90, 22), rot=(0, 170 + k * 14, 0))
    B(g, DOME, -160, 160, -960, -860, -FOOT, 330)                            # porch and purple door
    B(g, PURP, -100, 100, -972, -960, 0, 260)
    for a in (40, 140, 220, 320):                                            # blank white plates round the drum
        ra = math.radians(a)
        g.box(WHITE, (905 * math.cos(ra), 905 * math.sin(ra), 170), (16, 130, 130), rot=(0, a, 0))
    B(g, SYN, 1100, 1560, 0, 420, -FOOT, 300)                                # the control hut
    B(g, DOME, 1080, 1580, -24, 444, 300, 332)
    B(g, CYAN, 1160, 1420, -6, 0, 190, 218, glow=GL)
    B(g, PURP, 1450, 1540, -8, 0, 0, 200)
    B(g, DOME, 880, 1110, 180, 240, 60, 100)
    whip(g, 1500, 360, 332, 420)
    # ---- the dish, drawn looking straight up in its own frame (origin = the trunnion), then tipped 40 degrees to the road
    d = sc.part(P.motion(swing=6, pivot=(0.0, 0.0, ZT), axis=0.0, rate=0.05))
    d.base = ft_art._xf((0.0, 0.0, ZT), (0.0, 0.0, -40.0), (1.0, 1.0, 1.0))
    d.box(SYN, (0, 0, 140), (700, 520, 540))                                 # the hub box between the fork's arms
    d.box(SYN, (0, 0, -260), (520, 300, 260))                                # counterweight
    d.cyl(SYN, (0, 0, 340), 280, 60, steps=12)
    N = 24
    prof = ((150.0, 425.0), (760.0, 492.0), (1340.0, 644.0), (1900.0, 870.0))    # (radius, height): the bowl
    for j in range(3):
        (r0, z0), (r1, z1) = prof[j], prof[j + 1]
        w = 2.0 * r1 * math.tan(math.pi / N) + 4.0
        for k in range(N):
            a = 2.0 * math.pi * (k + 0.5 * (j % 2)) / N
            ca, sa = math.cos(a), math.sin(a)
            tilt_box(d, WHITE if (k + j) % 2 else DOME, (r0 * ca, r0 * sa, z0), (r1 * ca, r1 * sa, z1), w, 10)
    d.cyl(WHITE, (0, 0, 418), 170, 14, steps=12)
    for k in range(12):                                                      # the ribs behind
        a = 2.0 * math.pi * k / 12.0
        ca, sa = math.cos(a), math.sin(a)
        tilt_box(d, SYN, (240 * ca, 240 * sa, 250), (1850 * ca, 1850 * sa, 760), 26, 60)
    for k in range(4):                                                       # four struts to the feed
        a = math.radians(45 + k * 90)
        ca, sa = math.cos(a), math.sin(a)
        tilt_box(d, SYN, (1400 * ca, 1400 * sa, 670), (70 * ca, 70 * sa, 1760), 18, 18)
    d.box(SYN, (0, 0, 1800), (190, 190, 130))
    d.cyl(CYAN, (0, 0, 1560), 110, 180, steps=10, top_r=60, glow=GL)         # the feed horn, its mouth to the dish


# name, build, Scene.save options
JOBS = [
    ('SM_S1D6_RadomeTower', radome_tower, SMALL),
    ('SM_S1D6_Magazines', magazines, STILL),
    ('SM_S1D6_CommandBunker', command_bunker, SMALL),
    ('SM_S1D6_TestStand', test_stand, SMALL),
    ('SM_S1D6_GateTower', gate_tower, dict(grow=(45, 30, 45), grow_down=(45, 30, 0))),
    ('SM_S1D6_AerialHut', aerial_hut, SMALL),
    ('SM_S1D6_TrackHouse', track_house, dict(grow=(40, 40, 40), grow_down=(40, 40, 0))),
    ('SM_S1D6_BlastWall', blast_wall, STILL),
    ('SM_S1D6_WindSock', wind_sock, dict(grow=(60, 70, 20), grow_down=(60, 70, 0))),
    ('SM_S1D6_CameraMast', camera_mast, SMALL),
    ('SM_S1D6_WarnBoard', warn_board, STILL),
    ('SM_S1D6_Pylons', pylons, STILL),
    ('SM_S1D6_Cowls', cowls, SMALL),
    ('SM_S1D6_CalTarget', cal_target, STILL),
    ('SM_S1D6_TankTraps', tank_traps, STILL),
    ('SM_S1D6_CableDrums', cable_drums, STILL),
    ('SM_S1D6_Hatch', hatch, STILL),
    ('SM_S1D6_BigEar', big_ear, dict(grow=(60, 320, 320), grow_down=(60, 320, 0))),
]


def run(only=None):
    P.build_core()                              # loads the two masters; never build_core(rebuild=True)
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
