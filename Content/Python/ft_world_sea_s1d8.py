"""Under the Sea, Stage 1, builder s1d8: "Nereida: the Crown", the palace gardens (20.6 km to the end).
/Game/World/UnderTheSea/SM_S1D8_*. Run: import ft_world_sea_s1d8 as K; K.run()

Seven buildings, seven props and the hero landmark (THE CROWN), all ft_people.Scene pieces: one material, one section.
Violet walls and gold onion domes (nothing else on the level is violet), lavender crystal, pink blossom, white stone.
No green and no orange near the road, no fin, no jaw, no fish: the district is in the boss's arena.
Placed by C:/ftwork/s1/L6_s1d8.inl.
"""
import math
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/UnderTheSea'
STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))

VIO, LAV, GOLD, PINK, WHT = '#8a5a9a', '#b9a4ff', '#ffd23f', '#ff9ec7', '#f4efe0'
PLUM, INK, GLASS, LEAF = '#6a4562', '#3a3140', '#cfeeff', '#2a6a3a'


# ------------------------------------------------------------------------------------------ shared shapes
def onion(g, col, x, y, z, r, tip=GOLD, steps=8):
    """An onion dome 2r wide standing on z: neck, bulb, point, a spike and a ball. Returns its top."""
    g.cyl(col, (x, y, z), r * 0.62, r * 0.4, steps=steps, top_r=r)
    g.cyl(col, (x, y, z + r * 0.4), r, r * 0.45, steps=steps, top_r=r * 0.8)
    g.cyl(col, (x, y, z + r * 0.85), r * 0.8, r * 0.95, steps=steps, top_r=max(r * 0.08, 5))
    g.cyl(tip, (x, y, z + r * 1.78), max(r * 0.05, 4), r * 0.66, steps=5)
    g.sphere(tip, (x, y, z + r * 2.5), max(r * 0.15, 9), steps=(3, 5))
    return z + r * 2.65


def arch(g, x, y, z, w, h, col=INK, depth=10):
    """A round-headed opening w wide and h tall on a wall face at y (it stands proud of the face by half its depth)."""
    g.box(col, (x, y, z + (h - w / 2.0) / 2.0), (w, depth, h - w / 2.0))
    rod(g, col, (x, y - depth / 2.0, z + h - w / 2.0), (x, y + depth / 2.0, z + h - w / 2.0), w / 2.0, steps=8)


def crystal(g, col, x, y, z, r, h, glow=0.0):
    """A six-sided crystal standing on z, pointed. Returns its top."""
    g.cyl(col, (x, y, z), r, h * 0.68, steps=6, top_r=r * 0.82, glow=glow)
    g.cyl(col, (x, y, z + h * 0.68), r * 0.82, h * 0.32, steps=6, top_r=4, glow=glow)
    return z + h


def ring_boxes(g, col, cx, cy, z, rad, angles, size, glow=0.0):
    """Boxes standing round a centre, each turned to face outwards (size = radial, tangential, tall)."""
    for a in angles:
        ar = math.radians(a)
        g.box(col, (cx + math.cos(ar) * rad, cy + math.sin(ar) * rad, z), size, rot=(0, a, 0), glow=glow)


def pennant(sc, x, y, z, length, col, phase, h=70.0, swing=13.0, rate=0.3):
    """A long swallow-tailed pennant flying along +X from (x, y, z), swinging slowly about its mast. No words."""
    f = sc.part(P.motion(swing=swing, pivot=(x, y, z), axis=P.UP, rate=rate, phase=phase))
    f.box(col, (x + length * 0.4, y, z), (length * 0.8, 5, h))
    for s in (-1, 1):
        f.box(col, (x + length * 0.9, y, z + s * h * 0.3), (length * 0.2, 5, h * 0.4))


# ------------------------------------------------------------------------------------------ buildings
def pavilion(sc):
    """BUILDING 1, the onion-dome pavilion: 10 m square, 18.5 m. A white arcade, violet walls, a lavender drum, the
    great gold onion dome and four pink turret domes; a pink pennant on the spike."""
    g = sc.still()
    g.box(PLUM, (0, 0, -110), (1000, 1000, 300))
    g.box(WHT, (0, 0, 70), (940, 940, 60))
    g.box(WHT, (0, -510, 30), (360, 120, 140))
    g.box(WHT, (0, -590, -5), (440, 120, 90))
    g.box(VIO, (0, 0, 330), (860, 860, 460))
    for x in (-270, 0, 270):
        arch(g, x, -430, 100, 180, 370)
        g.box(GOLD, (x, -437, 492), (40, 8, 46))
    for s in (-1, 1):
        for y in (-200, 200):
            g.box(INK, (s * 430, y, 300), (10, 170, 330))
        for t in (-1, 1):
            g.box(WHT, (s * 415, t * 415, 330), (74, 74, 460))
    g.box(WHT, (0, 0, 585), (960, 960, 50))
    g.box(GOLD, (0, 0, 622), (900, 900, 24))
    g.box(WHT, (0, -440, 680), (880, 14, 16))
    for s in (-1, 1):
        g.box(WHT, (s * 440, 0, 680), (14, 880, 16))
    g.cyl(LAV, (0, 0, 634), 300, 360, steps=8)
    ring_boxes(g, INK, 0, 0, 810, 280, (247.5, 292.5, 202.5, 337.5), (10, 110, 200))
    g.cyl(WHT, (0, 0, 994), 330, 40, steps=8)
    g.cyl(GOLD, (0, 0, 1034), 300, 28, steps=8)
    onion(g, GOLD, 0, 0, 1062, 300, tip=GOLD)
    for s in (-1, 1):
        for t in (-1, 1):
            g.cyl(WHT, (s * 380, t * 380, 634), 62, 170, steps=6)
            onion(g, PINK, s * 380, t * 380, 804, 85, steps=6)
    pennant(sc, 0, 0, 1700, 230, PINK, 0.2, h=60)


def spire_house(sc):
    """BUILDING 2, the crystal spire house: a slender violet tower house 5.2 m wide, 25 m, six storeys of round-headed
    windows, a white balcony, and a crown of lavender and pink crystal that glows steadily."""
    g = sc.still()
    g.box(PLUM, (0, 0, -100), (640, 640, 320))
    g.box(VIO, (0, 0, 460), (520, 520, 800))
    for z in (330, 600):
        g.box(GOLD, (0, 0, z), (540, 540, 22))
    arch(g, 0, -260, 60, 150, 250)
    for z in (370, 640):
        for x in (-130, 130):
            arch(g, x, -260, z, 90, 170)
    for s in (-1, 1):
        for z in (200, 440, 700):
            g.box(INK, (s * 260, 0, z), (10, 110, 150))
    g.box(WHT, (0, -300, 350), (330, 90, 20))
    g.box(WHT, (0, -340, 392), (330, 12, 14))
    for x in (-160, 160):
        g.box(WHT, (x, -340, 372), (12, 12, 44))
    g.box(WHT, (0, 0, 885), (600, 600, 50))
    for s in (-1, 1):
        for t in (-1, 1):
            crystal(g, PINK, s * 255, t * 255, 910, 34, 190, glow=0.35)
    g.cyl(VIO, (0, 0, 910), 235, 620, steps=6, top_r=175)
    ring_boxes(g, INK, 0, 0, 1150, 196, (270, 210, 330), (10, 90, 190))
    ring_boxes(g, INK, 0, 0, 1400, 168, (270, 210, 330), (10, 70, 130))
    g.cyl(GOLD, (0, 0, 1530), 215, 36, steps=6)
    crystal(g, LAV, 0, 0, 1566, 110, 900, glow=0.45)
    for k in range(5):
        a = math.radians(54 + k * 72)
        crystal(g, PINK if k % 2 else LAV, math.cos(a) * 125, math.sin(a) * 125, 1566, 55, 360 + (k * 67) % 190, glow=0.4)


def terrace_block(sc):
    """BUILDING 3, the terraced garden block: four stepped storeys 24 m long on round-arched arcades, a garden on every
    step (pink and lavender shrubs, trailing blossom, urns), a gold-domed belvedere and two crystal pinnacles on top."""
    g = sc.still()
    g.box(PLUM, (0, 0, -130), (2500, 1500, 340))
    for k in range(4):
        L = 2400 - 380 * k
        yf = -700 + 270 * k
        z0 = 40 + 380 * k
        g.box(WHT if k == 0 else VIO, (0, (yf + 700) / 2.0, z0 + 190), (L, 700 - yf, 380))
        g.box(GOLD, (0, yf - 6, z0 + 368), (L + 12, 14, 24))
        g.box(WHT, (0, yf - 2, z0 + 430), (L, 14, 14))
        n = 7 - k
        for i in range(n):
            x = (i - (n - 1) / 2.0) * (L - 260) / max(n - 1, 1)
            if k % 2 == 0:
                arch(g, x, yf, z0 + 30, 190, 300)
            else:
                g.box(INK, (x, yf, z0 + 190), (120, 10, 200))
        for s in (-1, 1):
            g.box(INK, (s * L / 2.0, 250, z0 + 200), (10, 150, 210))
            g.box(INK, (s * L / 2.0, 520, z0 + 200), (10, 150, 210))
        if k < 3:                                   # the garden on this storey's roof, in front of the next
            m = 5 - k
            yg = yf + 135
            for i in range(m):
                x = (i - (m - 1) / 2.0) * (L - 700) / max(m - 1, 1)
                g.sphere((PINK, LAV, LEAF)[(i + k) % 3], (x, yg, z0 + 430), 62, scale=(1.2, 1, 0.9), steps=(3, 5))
            for i in range(3):
                x = (i - 1) * (L / 3.0) + 130
                g.box(PINK if i % 2 else LAV, (x, yf - 12, z0 + 310), (110, 12, 150))
            for s in (-1, 1):
                x = s * (L / 2.0 - 90)
                g.cyl(WHT, (x, yg, z0 + 380), 26, 50, steps=6, top_r=48)
                g.sphere(PINK, (x, yg, z0 + 450), 34, steps=(3, 5))
    g.box(WHT, (0, -760, 30), (420, 130, 100))
    g.box(WHT, (0, -860, 0), (520, 130, 70))
    zt = 40 + 380 * 4
    for s in (-1, 1):
        for t in (-1, 1):
            g.cyl(WHT, (s * 150, 420 + t * 120, zt), 20, 240, steps=6)
    g.box(WHT, (0, 420, zt + 256), (400, 340, 32))
    onion(g, GOLD, 0, 420, zt + 272, 170)
    for s in (-1, 1):
        crystal(g, LAV, s * 520, 420, zt, 60, 420, glow=0.4)
        pennant(sc, s * 520, 420, zt + 440, 190, PINK if s > 0 else WHT, 0.3 + 0.25 * s, h=50)


def gate_lodge(sc):
    """BUILDING 4, the gate lodge: twin white turrets with violet onion caps, a gold-leaved gate between them under a
    violet gallery, two steady lavender lanterns. The gate faces the road; nothing crosses it."""
    g = sc.still()
    g.box(PLUM, (0, 0, -110), (1500, 560, 300))
    for s in (-1, 1):
        x = s * 520
        g.cyl(WHT, (x, 0, 40), 210, 900, steps=8)
        g.cyl(VIO, (x, 0, 470), 222, 60, steps=8)
        g.cyl(GOLD, (x, 0, 880), 222, 26, steps=8)
        ring_boxes(g, INK, x, 0, 300, 198, (247.5, 292.5), (10, 80, 190))
        ring_boxes(g, INK, x, 0, 700, 198, (247.5, 292.5), (10, 80, 190))
        g.cyl(WHT, (x, 0, 940), 250, 70, steps=8)
        onion(g, VIO, x, 0, 1010, 230)
        g.box(LAV, (s * 250, -170, 400), (44, 44, 70), glow=0.6)
        g.box(GOLD, (s * 250, -170, 446), (60, 60, 20))
    g.box(WHT, (0, 0, 330), (660, 300, 580))
    arch(g, 0, -150, 40, 380, 500)
    for x in (-140, -70, 0, 70, 140):
        g.box(GOLD, (x, -160, 250), (12, 8, 400 if abs(x) < 100 else 340))
    g.box(GOLD, (0, -160, 240), (360, 8, 14))
    g.box(VIO, (0, 0, 680), (720, 340, 120))
    g.box(WHT, (0, -170, 770), (720, 14, 60))
    g.box(GOLD, (0, 0, 748), (740, 360, 16))
    g.sphere(GOLD, (0, 0, 800), 48, steps=(3, 6))
    for s in (-1, 1):
        crystal(g, LAV, s * 110, 0, 750, 26, 150, glow=0.35)
    g.cyl(GOLD, (0, 0, 840), 7, 300, steps=5)
    pennant(sc, 0, 0, 1100, 220, PINK, 0.6, h=60)


def rotunda(sc):
    """BUILDING 5, the music rotunda: an open ring of eight white columns on a stepped round floor under a gold roof
    and onion, a gold harp and three music stands inside, a rail between the columns, small steady lanterns."""
    g = sc.still()
    g.cyl(PLUM, (0, 0, -260), 620, 300, steps=10)
    g.cyl(WHT, (0, 0, 40), 580, 40, steps=10)
    g.cyl(WHT, (0, 0, 80), 520, 40, steps=10)
    pts = []
    for k in range(8):
        a = math.radians(22.5 + k * 45)
        x, y = math.cos(a) * 440, math.sin(a) * 440
        pts.append((x, y))
        g.cyl(WHT, (x, y, 120), 34, 430, steps=6)
        g.box(GOLD, (x, y, 562), (90, 90, 24))
        g.box(PINK, (x * 0.9, y * 0.9, 500), (22, 22, 34), glow=0.5)
    for k in range(8):
        if k == 5:                              # the bay on the front stays open
            continue
        a, b = pts[k], pts[(k + 1) % 8]
        tilt_box(g, WHT, (a[0], a[1], 215), (b[0], b[1], 215), 12, 14)
        tilt_box(g, VIO, (a[0], a[1], 160), (b[0], b[1], 160), 8, 60)
    g.cyl(WHT, (0, 0, 574), 500, 60, steps=10)
    g.cyl(GOLD, (0, 0, 634), 512, 24, steps=10)
    g.cyl(GOLD, (0, 0, 658), 520, 190, steps=10, top_r=250)
    g.cyl(VIO, (0, 0, 848), 262, 30, steps=10)
    onion(g, GOLD, 0, 0, 878, 235)
    g.cyl(VIO, (0, 80, 120), 90, 50, steps=8)
    g.box(GOLD, (-50, 80, 290), (14, 14, 240))
    tilt_box(g, GOLD, (-50, 80, 410), (20, 80, 440), 14, 16)
    tilt_box(g, GOLD, (20, 80, 440), (70, 80, 400), 14, 16)
    tilt_box(g, GOLD, (70, 80, 400), (-44, 80, 176), 20, 22)
    for i in range(3):
        g.box(LAV, (-28 + i * 24, 80, 300 + i * 22), (4, 4, 190 - i * 50))
    for x, y in ((-230, -40), (0, -140), (230, -40)):
        g.box(INK, (x, y, 185), (5, 5, 130))
        g.box(WHT, (x, y - 6, 262), (56, 6, 40), rot=(0, 0, 20))
    pennant(sc, 0, 0, 1440, 200, LAV, 0.8, h=56)


def orangery(sc):
    """BUILDING 6, the orangery: 18 m of pale glass between violet ribs under a barrel roof with violet hoops and a gold
    ridge, a tall middle bay with a gold onion, fan-lights over every bay, pink coral trees in white tubs outside."""
    g = sc.still()
    g.box(PLUM, (0, 0, -110), (1900, 820, 300))
    g.box(WHT, (0, 0, 70), (1840, 780, 60))
    g.box(GLASS, (0, 0, 330), (1720, 660, 460))
    for i in range(9):
        x = -860 + i * 215
        g.box(VIO, (x, -334, 330), (26, 16, 460))
        if i < 8 and i not in (3, 4):
            rod(g, LAV, (x + 107, -336, 462), (x + 107, -320, 462), 84, steps=8)
    for s in (-1, 1):
        for y in (-220, 0, 220):
            g.box(VIO, (s * 864, y, 330), (16, 26, 460))
    for z in (112, 400, 552):
        g.box(VIO, (0, -336, z), (1730, 14, 22))
    rod(g, GLASS, (-860, 0, 560), (860, 0, 560), 330, steps=10)
    for x in (-760, -480, 480, 760):
        rod(g, VIO, (x - 13, 0, 560), (x + 13, 0, 560), 342, steps=10)
    g.box(GOLD, (0, 0, 898), (1720, 14, 30))
    g.box(GLASS, (0, 0, 520), (420, 760, 840))
    for s in (-1, 1):
        g.box(VIO, (s * 205, -382, 520), (26, 14, 840))
    g.box(VIO, (0, -382, 930), (436, 14, 26))
    g.box(GOLD, (0, 0, 950), (460, 800, 22))
    onion(g, GOLD, 0, 0, 961, 190)
    arch(g, 0, -382, 100, 210, 360)
    g.box(WHT, (0, -440, 30), (300, 100, 140))
    for x in (-760, -330, 330, 760):
        g.cyl(WHT, (x, -470, -40), 46, 150, steps=6, top_r=58)
        g.cyl(PLUM, (x, -470, 110), 9, 120, steps=5)
        g.sphere(PINK, (x, -470, 280), 74, steps=(3, 6))


def arch_bridge(sc):
    """BUILDING 7, the garden bridge: 38 m of white viaduct on three tall round arches, standing ALONG the road (never
    over it): a violet deck with a balustrade, pearl lamps, trailing blossom, a domed stair tower at each end."""
    g = sc.still()
    for x in (-1500, -500, 500, 1500):
        g.box(WHT, (x, 0, 130), (220, 460, 780))
        g.box(VIO, (x, 0, 535), (260, 500, 30))
    R = 390.0
    bay = [(-500.0, 520.0)]
    for k in range(9):
        a = math.radians(180 - k * 22.5)
        bay.append((R * math.cos(a), 520.0 + R * math.sin(a)))
    bay += [(500.0, 520.0), (500.0, 1000.0), (-500.0, 1000.0)]
    for xc in (-1000, 0, 1000):
        g.side(WHT, bay, -200, 200, loc=(xc, 0, 0))
        for s in (-1, 1):
            g.box(GOLD, (xc, s * 204, 950), (70, 14, 110))
    g.box(VIO, (0, 0, 1030), (3300, 520, 60))
    g.box(GOLD, (0, 0, 1070), (3340, 560, 20))
    for s in (-1, 1):
        g.box(WHT, (0, s * 250, 1170), (3300, 16, 16))
        for i in range(11):
            g.box(WHT, (-1500 + i * 300, s * 250, 1125), (26, 26, 90))
    for x in (-1000, 0, 1000):
        g.cyl(GOLD, (x, 0, 1080), 9, 250, steps=5)
        g.sphere(WHT, (x, 0, 1362), 36, steps=(3, 6), glow=0.7)
    for i, x in enumerate((-1250, -760, -240, 250, 740, 1260)):
        g.box(PINK if i % 2 else LAV, (x, -266, 960), (120, 12, 190 + (i * 53) % 90))
    for s in (-1, 1):
        x = s * 1790
        g.box(WHT, (x, 0, 430), (300, 480, 1380))
        arch(g, x, -240, 40, 150, 280)
        g.box(INK, (x, -240, 800), (90, 10, 170))
        g.box(GOLD, (x, 0, 1130), (330, 510, 20))
        g.cyl(VIO, (x, 0, 1140), 150, 280, steps=8)
        ring_boxes(g, INK, x, 0, 1290, 140, (247.5, 292.5), (10, 60, 150))
        onion(g, PINK, x, 0, 1420, 160)
        pennant(sc, x, 0, 1830, 170, LAV if s > 0 else PINK, 0.45 + 0.2 * s, h=50)


# ------------------------------------------------------------------------------------------ props
def urn(sc):
    """PROP 1, a garden urn on a pedestal: white, gold bands and handles, a spray of pink coral. LOW (1.9 m)."""
    g = sc.still()
    g.box(WHT, (0, 0, -10), (140, 140, 80))
    g.box(WHT, (0, 0, 68), (96, 96, 76))
    g.box(GOLD, (0, 0, 112), (110, 110, 12))
    g.cyl(VIO, (0, 0, 118), 26, 36, steps=8, top_r=56)
    g.cyl(WHT, (0, 0, 154), 56, 14, steps=8, top_r=44)
    g.cyl(GOLD, (0, 0, 168), 48, 8, steps=8)
    for s in (-1, 1):
        g.box(GOLD, (s * 58, 0, 150), (14, 10, 26))
    for k in range(3):
        a = math.radians(30 + k * 120)
        tip = (math.cos(a) * 20, math.sin(a) * 20, 181)
        rod(g, PINK, (0, 0, 172), tip, 5, steps=5)
        g.sphere(PINK, tip, 8, steps=(3, 5))


def seahorse(sc):
    """PROP 2, a topiary seahorse (a clipped lavender coral bush on a gold stake, in a white planter): 4 m. A garden
    animal, in profile to the road, with a little pink coronet and blossoms down its back; no fin."""
    g = sc.still()
    g.box(WHT, (0, 0, 5), (170, 170, 130))
    g.box(GOLD, (0, 0, 62), (182, 182, 14))
    g.cyl(GOLD, (20, 0, 70), 7, 150, steps=5)
    g.sphere(LAV, (-34, 0, 112), 27, steps=(3, 6))
    tilt_box(g, LAV, (-34, 0, 92), (26, 0, 96), 30, 30)
    tilt_box(g, LAV, (30, 0, 90), (44, 0, 160), 34, 34)
    g.sphere(LAV, (30, 0, 222), 64, scale=(0.9, 0.7, 1.3), steps=(4, 6))
    g.sphere(LAV, (22, 0, 298), 50, scale=(1, 0.75, 1), steps=(4, 6))
    tilt_box(g, LAV, (14, 0, 305), (34, 0, 356), 46, 44)
    g.sphere(LAV, (48, 0, 366), 40, scale=(1.1, 0.8, 0.9), steps=(4, 6))
    tilt_box(g, LAV, (72, 0, 362), (120, 0, 350), 22, 22)
    g.sphere(LAV, (124, 0, 349), 15, steps=(3, 5))
    for s in (-1, 1):
        g.box(INK, (58, s * 31, 374), (10, 6, 10))
    for i, x in enumerate((26, 44, 62)):
        g.box(PINK, (x, 0, 404 + (6 if i == 1 else 0)), (10, 10, 22))
    for x, z in ((-22, 300), (-32, 250), (-30, 200), (-16, 152)):
        g.sphere(PINK, (x, 0, z), 12, steps=(3, 5))


def lamp_tree(sc):
    """PROP 3, a gold lamp tree: a gold trunk, three curled boughs, three pearl globes that glow steadily (white, not
    amber), a small gold onion at the top. 6 m."""
    g = sc.still()
    g.cyl(PLUM, (0, 0, -60), 60, 80, steps=8)
    g.cyl(GOLD, (0, 0, 20), 44, 60, steps=8, top_r=20)
    g.cyl(GOLD, (0, 0, 80), 14, 330, steps=6)
    g.sphere(GOLD, (0, 0, 250), 24, steps=(3, 6))
    for k in range(3):
        a = math.radians(90 + k * 120)
        c, s = math.cos(a), math.sin(a)
        rod(g, GOLD, (0, 0, 396), (c * 70, s * 70, 470), 9, steps=5)
        rod(g, GOLD, (c * 70, s * 70, 470), (c * 112, s * 112, 522), 9, steps=5)
        g.sphere(WHT, (c * 112, s * 112, 556), 36, steps=(4, 6), glow=0.8)
        g.cyl(GOLD, (c * 112, s * 112, 588), 14, 12, steps=5)
    onion(g, GOLD, 0, 0, 410, 30, steps=6)


def hedge_gate(sc):
    """PROP 4, a clipped hedge and its gate: 10 m of plum-leaved hedge with a lavender trimmed top and pink blossom,
    white piers with gold balls, a little gold wicket gate under a gold bow. LOW (1.9 m)."""
    g = sc.still()
    for s in (-1, 1):
        g.box(PLUM, (s * 295, 0, 40), (370, 90, 180))
        g.box(LAV, (s * 295, 0, 135), (370, 94, 10))
        for i in range(3):
            g.box(PINK, (s * (160 + i * 130), -47, 60 + (i * 37) % 50), (16, 8, 16))
        for x in (s * 100, s * 492):
            g.box(WHT, (x, 0, 55), (40, 100, 210))
            g.sphere(GOLD, (x, 0, 174), 15, steps=(3, 5))
        tilt_box(g, GOLD, (s * 100, 0, 160), (0, 0, 181), 8, 8)
    for z in (36, 120):
        g.box(GOLD, (0, 0, z), (164, 6, 8))
    for x in (-60, -30, 0, 30, 60):
        g.box(GOLD, (x, 0, 78), (6, 6, 84))


def fountain(sc):
    """PROP 5, the bubble fountain: three violet bowls with gold rims on white stems over a plum basin, a lavender
    crystal on top, and pale glass bubbles rising and sinking over it. 4.3 m."""
    g = sc.still()
    g.cyl(PLUM, (0, 0, -60), 230, 110, steps=10)
    g.cyl(VIO, (0, 0, 50), 240, 30, steps=10, top_r=250)
    g.cyl(GLASS, (0, 0, 76), 215, 8, steps=10)
    g.cyl(WHT, (0, 0, 80), 40, 110, steps=8, top_r=28)
    g.cyl(VIO, (0, 0, 190), 30, 50, steps=8, top_r=130)
    g.cyl(GOLD, (0, 0, 240), 132, 10, steps=8)
    g.cyl(WHT, (0, 0, 250), 22, 70, steps=6)
    g.cyl(VIO, (0, 0, 320), 20, 34, steps=8, top_r=70)
    g.cyl(GOLD, (0, 0, 354), 72, 8, steps=8)
    crystal(g, LAV, 0, 0, 362, 20, 70)
    for k in range(9):
        a = math.radians(k * 137.5)
        rad = 30 + (k * 29) % 90
        b = sc.part(P.motion(bob=-(18.0 + (k * 7) % 14), rate=0.22 + 0.03 * (k % 5), phase=(k * 0.37) % 1.0))
        b.sphere(GLASS, (math.cos(a) * rad, math.sin(a) * rad, 285 + k * 22), 10 + (k * 5) % 13, steps=(3, 5))


def pennant_mast(sc):
    """PROP 6, the pennant mast: a white mast 9.4 m with gold bands and a gold ball, three long swallow-tailed
    pennants (pink, lavender, white; no words) swinging slowly."""
    g = sc.still()
    g.cyl(PLUM, (0, 0, -60), 55, 90, steps=8)
    g.cyl(WHT, (0, 0, 30), 40, 70, steps=8, top_r=24)
    g.cyl(WHT, (0, 0, 100), 11, 800, steps=6)
    for z in (300, 640, 880):
        g.cyl(GOLD, (0, 0, z), 17, 14, steps=6)
    g.sphere(GOLD, (0, 0, 918), 20, steps=(3, 6))
    pennant(sc, 0, 0, 840, 260, PINK, 0.0, h=64, swing=14, rate=0.3)
    pennant(sc, 0, 0, 750, 220, LAV, 0.35, h=56, swing=12, rate=0.26)
    pennant(sc, 0, 0, 668, 180, WHT, 0.7, h=50, swing=13, rate=0.34)


def parterre(sc):
    """PROP 7, a parterre: a bed of lavender blossom in a low plum border, pink scrolls across it, four small clipped
    cones, and a gold armillary dial on a white pedestal in the middle. LOW (1.9 m)."""
    g = sc.still()
    g.box(LAV, (0, 0, -10), (730, 395, 80))
    for s in (-1, 1):
        g.box(PLUM, (0, s * 215, 0), (800, 36, 100))
        g.box(PLUM, (s * 382, 0, 0), (36, 430, 100))
        tilt_box(g, PINK, (s * 60, -150, 33), (s * 330, 150, 33), 24, 8)
        tilt_box(g, PINK, (s * 60, 150, 33), (s * 330, -150, 33), 24, 8)
        for t in (-1, 1):
            g.cyl(PLUM, (s * 340, t * 170, 30), 34, 120, steps=6, top_r=5)
    g.cyl(WHT, (0, 0, 30), 30, 86, steps=8, top_r=22)
    g.sphere(GOLD, (0, 0, 150), 30, steps=(4, 6))
    g.cyl(GOLD, (0, 0, 147), 44, 6, steps=10)
    rod(g, GOLD, (-44, 0, 128), (44, 0, 176), 4, steps=5)


# ------------------------------------------------------------------------------------------ the hero
def crown(sc):
    """THE CROWN: seven violet spires of different heights in a ring 40 m across on a garden mound, each with a gold
    onion dome, joined by a gold circlet 40 m up set with pearls and crystal points; seven long pennants turn slowly
    round the circlet. 85 m at the tallest. Stands 185 m out: saved far=True so the pennants keep turning."""
    g = sc.still()
    g.cyl(PLUM, (0, 0, -300), 2750, 500, steps=14, top_r=2550)
    g.cyl(WHT, (0, 0, 200), 2480, 60, steps=12)
    g.cyl(LAV, (0, 0, 260), 1450, 26, steps=12)
    g.sphere(GLASS, (0, 0, 280), 620, scale=(1, 1, 0.55), steps=(4, 10))
    ring_boxes(g, INK, 0, 0, 100, 2530, [k * 360.0 / 14 + 12.86 for k in range(14)], (10, 260, 190))
    g.box(WHT, (0, -2640, 100), (520, 260, 200))
    RING, ZC = 2000.0, 4000.0
    heights = (8500, 7200, 6100, 5200, 5600, 6600, 7700)
    pos = []
    for k in range(7):
        a = math.radians(90 + k * 360.0 / 7)
        x, y = math.cos(a) * RING, math.sin(a) * RING
        pos.append((x, y))
        H = heights[k]
        g.cyl(WHT, (x, y, 260), 330, 500, steps=8)
        g.cyl(VIO, (x, y, 760), 290, ZC - 100 - 760, steps=8, top_r=220)
        g.cyl(LAV, (x, y, 2300), 262, 70, steps=8)
        g.cyl(GOLD, (x, y, ZC - 100), 262, 200, steps=8)
        top = H - 230 * 2.65
        g.cyl(VIO, (x, y, ZC + 100), 215, top - 60 - (ZC + 100), steps=8, top_r=150)
        g.cyl(WHT, (x, y, top - 60), 200, 60, steps=8)
        onion(g, GOLD, x, y, top, 230, steps=6)
        out = math.degrees(a)
        ring_boxes(g, INK, x, y, 1500, 272, (out, 270), (10, 110, 300))
        ring_boxes(g, '#fff1c0', x, y, 3100, 240, (out, 270), (10, 90, 240), glow=0.6)
    for k in range(7):
        a, b = pos[k], pos[(k + 1) % 7]
        for i in range(3):
            p = (a[0] + (b[0] - a[0]) * i / 3.0, a[1] + (b[1] - a[1]) * i / 3.0, ZC)
            q = (a[0] + (b[0] - a[0]) * (i + 1) / 3.0, a[1] + (b[1] - a[1]) * (i + 1) / 3.0, ZC)
            tilt_box(g, GOLD, p, q, 150, 200)
        mx, my = (a[0] + b[0]) / 2.0, (a[1] + b[1]) / 2.0
        crystal(g, LAV, mx, my, ZC + 100, 80, 420, glow=0.4)
        g.sphere(WHT, (mx * 1.045, my * 1.045, ZC), 70, steps=(3, 5), glow=0.5)
    for k in range(7):
        a = math.radians(k * 360.0 / 7 + 20)
        x, y = math.cos(a) * 2350, math.sin(a) * 2350
        f = sc.part(P.motion(base=(0.0, 0.0), travel=(-1.0, 0.02), phase=k / 7.0))
        col = (PINK, LAV, WHT)[k % 3]
        tang = math.degrees(a) + 90
        f.box(col, (x, y, ZC + 60), (420, 8, 110), rot=(0, tang, 0))
        ta = math.radians(tang)
        for s in (-1, 1):
            f.box(col, (x - math.cos(ta) * 280, y - math.sin(ta) * 280, ZC + 60 + s * 34), (140, 8, 42), rot=(0, tang, 0))


JOBS = [('SM_S1D8_OnionPavilion', pavilion), ('SM_S1D8_CrystalSpireHouse', spire_house, STILL),
        ('SM_S1D8_TerraceBlock', terrace_block), ('SM_S1D8_GateLodge', gate_lodge), ('SM_S1D8_MusicRotunda', rotunda),
        ('SM_S1D8_Orangery', orangery, STILL), ('SM_S1D8_ArchBridge', arch_bridge),
        ('SM_S1D8_Urn', urn, STILL), ('SM_S1D8_SeahorseTopiary', seahorse, STILL), ('SM_S1D8_LampTree', lamp_tree, STILL),
        ('SM_S1D8_HedgeGate', hedge_gate, STILL), ('SM_S1D8_BubbleFountain', fountain, dict(grow=(10, 10, 40), grow_down=(10, 10, 0))),
        ('SM_S1D8_PennantMast', pennant_mast, dict(grow=(40, 90, 20), grow_down=(40, 90, 0))),
        ('SM_S1D8_Parterre', parterre, STILL),
        ('SM_S1D8_Crown', crown, dict(far=True, grow=(100, 100, 100), grow_down=(100, 100, 0)))]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
