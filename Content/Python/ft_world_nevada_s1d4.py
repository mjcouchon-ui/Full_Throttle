"""Nevada, Stage 1, builder s1d4: district 4 "Jackpot Junction and the City Lights" (7.75 to 10.69 km).
/Game/World/Nevada/SM_S1D4_*. Run: import ft_world_nevada_s1d4 as K; K.run()

Atomic-age roadside ("googie"): boomerang and butterfly roofs, folded-plate canopies, glass fronts tilted outward,
starburst spires, a drum lounge on a stalk. Deep violet walls, magenta and cyan neon, gold crowns, white ribs.

  buildings  CoffeeShop (boomerang roof, a giant cup steaming on it), DrumLounge (a windowed drum on a stalk, its
             window band turning slowly), MotelOffice (butterfly roof, a starburst needle spire), BowlingAlley (a giant
             white pin rocking on the roof), DriveIn (a screen tower, turned in the mesh), ArcadeHall (three vaults, a
             marquee of fruit-and-bell shapes, a gold bell swinging on the roof), SodaBar (a walk-up stand under a
             tilted disc, a giant soda glass on top)
  props      StarLamp, ArrowSign, TubeCactus, PhoneKiosk, AtomPylon, FinPylons (tall); KidneyPlanter, RopePodium (low)
  hero       LuckyHorseshoe: a 45 m golden horseshoe, open end up, on a violet plinth; cyan studs; a magenta
             eight-point star rocking inside the arch (far=True: it keeps rocking past 180 m)

No words, no numerals, no dice, no chips, no vehicles. Steady light only (nothing blinks); the two low props carry no
light at all. One material, one section (ft_people.Scene). Where they stand: C:/ftwork/s1/L2_s1d4.inl.
"""
import math
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/Nevada'
VIO, VIO2, MAG, CYAN, GOLD, WHITE = '#3a2a6a', '#4a3a8a', '#ff4fb8', '#2ee6ff', '#ffc93c', '#fff6ea'
SYN, GLASS, WARM, BULB, PAVE, CONC = '#1c1830', '#9ff0ff', '#ffc86a', '#fff3a8', '#5a4a62', '#c9b8a0'
SAGE, SAGE_D, NAVY, COFFEE = '#8fb86a', '#5f8f4a', '#2b2350', '#6a4c36'
G1, G2, G3 = 0.4, 0.9, 1.4                        # glow: lit glass, neon, a lamp
STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))
SMALL = dict(grow=(40, 40, 40), grow_down=(40, 40, 0))


# ------------------------------------------------------------------------------------------ helpers
def pad(g, w, d, top=14.0, foot=200.0, col=PAVE):
    """The paving a piece stands on, with its footing under z = 0."""
    g.box(col, (0, 0, (top - foot) / 2.0), (w, d, top + foot))


def disc(g, col, x, y, z, r, t=20.0, n=10, glow=0.0, sx=1.0):
    """A disc facing the road (its axis along Y), t thick, centred on (x, y, z)."""
    pts = [(x + r * sx * math.cos(2 * math.pi * k / n), z + r * math.sin(2 * math.pi * k / n)) for k in range(n)]
    g.side(col, pts, y - t / 2.0, y + t / 2.0, glow=glow)


def slab(g, col, top, t, y0, y1, glow=0.0):
    """A roof drawn by its top line [(x, z), ...] seen from the front, t thick, from y0 to y1."""
    g.side(col, list(top) + [(x, z - t) for x, z in reversed(top)], y0, y1, glow=glow)


def lerp_line(line, x):
    for (x0, z0), (x1, z1) in zip(line, line[1:]):
        if x0 <= x <= x1:
            return z0 + (z1 - z0) * (x - x0) / float(x1 - x0)
    return line[-1][1]


def burst(g, x, y, z, R, r, n=8, w=16.0, col=GOLD, glow=G2, t=None):
    """A flat starburst facing the road: n rays, long and short by turns."""
    for k in range(n):
        a = 2 * math.pi * k / n + math.pi / 2.0
        L = R if k % 2 == 0 else r
        tilt_box(g, col, (x + math.cos(a) * w * 0.4, y, z + math.sin(a) * w * 0.4), (x + math.cos(a) * L, y, z + math.sin(a) * L), t or w, w, glow=glow)


SPUT = [(0, 0, 1), (0, 0, -1), (1, 0, 0), (-1, 0, 0), (0, 1, 0), (0, -1, 0), (0.58, 0.58, 0.58), (-0.58, 0.58, 0.58), (0.58, -0.58, 0.58),
        (-0.58, -0.58, 0.58), (0.58, 0.58, -0.58), (-0.58, 0.58, -0.58), (0.58, -0.58, -0.58), (-0.58, -0.58, -0.58)]


def sputnik(g, x, y, z, R, core=WARM, ray=WHITE, tip=None, w=7.0, r=None, glow=G3, n=14):
    """A starburst in the round: a lit ball and rays every way, long and short."""
    g.sphere(core, (x, y, z), r or R * 0.26, steps=(4, 6), glow=glow)
    for k, (dx, dy, dz) in enumerate(SPUT[:n]):
        L = R if k < 6 else R * 0.72
        tilt_box(g, ray, (x + dx * R * 0.2, y + dy * R * 0.2, z + dz * R * 0.2), (x + dx * L, y + dy * L, z + dz * L), w, w)
        if tip:
            g.box(tip[k % len(tip)], (x + dx * L, y + dy * L, z + dz * L), (w * 2.2, w * 2.2, w * 2.2), glow=G2)


def arc(g, col, x, y, z, R, a0, a1, n, w, d, glow=0.0, sx=1.0):
    """An arc of n straight lengths in the plane facing the road, w deep (along Y) and d thick."""
    for k in range(n):
        p = [(x + R * sx * math.cos(math.radians(a)), y, z + R * math.sin(math.radians(a))) for a in
             (a0 + (a1 - a0) * (k - 0.06) / n, a0 + (a1 - a0) * (k + 1.06) / n)]
        tilt_box(g, col, p[0], p[1], w, d, glow=glow)


def glass_front(g, x0, x1, y, z0, top_at, bays, lean=0.33, col=GLASS, glow=G1, skip=()):
    """A glass front tilted outward: panes between white mullions, each as tall as the roof over it allows."""
    w = (x1 - x0) / float(bays)
    for k in range(bays + 1):
        x = x0 + k * w
        zt = top_at(x) - 4.0
        tilt_box(g, WHITE, (x, y - 4, z0), (x, y - 4 - lean * (zt - z0), zt), 14, 16)
        if k < bays and k not in skip:
            xc = x + w / 2.0
            zt = min(top_at(x), top_at(x + w)) - 10.0
            tilt_box(g, col, (xc, y, z0), (xc, y - lean * (zt - z0), zt), w - 8.0, 8, glow=glow)


def shrub(g, x, y, z, r=46.0, col=SAGE):
    g.box(CONC, (x, y, z + 20), (r * 2.0, r * 2.0, 40))
    g.sphere(col, (x, y, z + 40 + r * 0.6), r, scale=(1, 1, 0.8), steps=(4, 6))


# ------------------------------------------------------------------------------------------ buildings
def coffee_shop(sc):
    """16.4 x 9.6 m. A boomerang roof over a glass front that leans out, an A-frame sign of three lit discs through
    the roof's low end, a giant cup on the high end whose steam drifts up."""
    g = sc.still()
    pad(g, 1640, 960)
    roof = [(-800, 480), (-150, 390), (820, 640)]
    under = lambda x: lerp_line(roof, max(-800, min(820, x))) - 46.0
    g.side(VIO, [(-565, 14), (685, 14), (685, under(685) + 4), (-150, under(-150) + 4), (-565, under(-565) + 4)], -90, 420)
    g.box(VIO2, (60, 424, 150), (1250, 10, 272))
    slab(g, WHITE, roof, 46, -430, 440)
    slab(g, VIO2, [(-770, 492), (-150, 404), (800, 648)], 14, -380, 400)
    glass_front(g, -540, 660, -92, 50, under, 5, skip=(4,))
    g.box(MAG, (60, -96, 32), (1210, 26, 38))
    g.box(WARM, (540, -96, 150), (150, 12, 250), glow=G1)                     # the door bay
    g.box(WHITE, (540, -104, 290), (190, 26, 14))
    tilt_box(g, MAG, (-800, -436, 457), (-150, -436, 367), 10, 30, glow=G2)   # neon along the roof's front edge
    tilt_box(g, MAG, (-150, -436, 367), (820, -436, 617), 10, 30, glow=G2)
    tilt_box(g, CYAN, (-800, -436, 420), (-150, -436, 330), 8, 12, glow=G2)
    tilt_box(g, CYAN, (-150, -436, 330), (820, -436, 580), 8, 12, glow=G2)
    for x in (-640, -340, 150, 440, 720):                                    # white beams under the eave
        g.box(WHITE, (x, -300, under(x) - 16), (26, 250, 30))
    for s in (-1, 1):                                                         # the A-frame sign through the low end
        tilt_box(g, WHITE, (-690 + s * 70, -60, 14), (-690, -60, 1260), 44, 44)
    for z, r, col in ((790, 125, MAG), (990, 104, CYAN), (1160, 82, GOLD)):
        disc(g, col, -690, -96, z, r, 30, 10, glow=G2)
        disc(g, WHITE, -690, -116, z, r * 0.5, 12, 8, glow=G1)
    burst(g, -690, -70, 1340, 150, 84, 8, 14)
    z0 = lerp_line(roof, 420)                                                 # the cup
    g.box(VIO, (420, 60, z0 + 10), (270, 270, 110))
    g.cyl(WHITE, (420, 60, z0 + 64), 175, 16, steps=10)
    g.cyl(WHITE, (420, 60, z0 + 80), 96, 176, steps=10, top_r=138)
    g.cyl(MAG, (420, 60, z0 + 150), 122, 26, steps=10, top_r=127)
    g.cyl(COFFEE, (420, 60, z0 + 250), 128, 8, steps=10)
    g.box(WHITE, (574, 60, z0 + 170), (56, 30, 96))
    st = sc.part(P.motion(bob=-16, rate=0.22, phase=0.2))
    for dx, dz, r in ((0, 330, 48), (34, 404, 38), (-12, 468, 28)):
        st.sphere(WHITE, (420 + dx, 60, z0 + dz), r, steps=(3, 6))
    for x, y, col in ((-400, -330, CYAN), (-90, -350, MAG)):                 # two patio tables under parasols
        g.cyl(WHITE, (x, y, 14), 8, 236, steps=5)
        g.cyl(WHITE, (x, y, 80), 52, 8, steps=8)
        g.cyl(col, (x, y, 226), 118, 44, steps=8, top_r=10)
        for dx in (-70, 70):
            g.box(VIO2, (x + dx, y, 40), (34, 34, 52))
    shrub(g, 700, -150, 14)
    shrub(g, -450, 300, under(-450) + 50, 30, SAGE_D)
    g.cyl(WHITE, (150, 250, lerp_line(roof, 150) - 6), 40, 70, steps=6)       # the kitchen's vent
    g.cyl(GOLD, (150, 250, lerp_line(roof, 150) + 64), 60, 24, steps=6, top_r=14)


def drum_lounge(sc):
    """11.8 m across, 22 m tall. A lounge like a drum on a white stalk with four flying ribs, its band of lit windows
    turning slowly; a glass lift up the stalk; a gold needle and a starburst on the roof."""
    g = sc.still()
    g.cyl(PAVE, (0, 0, -200), 600, 214, steps=12)
    g.cyl(VIO, (0, 0, 14), 310, 300, steps=10)
    g.cyl(GLASS, (0, 0, 120), 316, 130, steps=10, glow=G1)
    g.cyl(WHITE, (0, 0, 314), 400, 26, steps=10)
    g.cyl(MAG, (0, 0, 300), 408, 12, steps=10, glow=G2)
    g.box(WARM, (0, -312, 110), (130, 14, 200), glow=G1)
    slab(g, WHITE, [(-230, 250), (0, 222), (230, 270)], 20, -520, -300)        # a little boomerang over the door
    g.cyl(WHITE, (0, 0, 340), 150, 860, steps=8, top_r=112)
    for k in range(4):                                                         # the flying ribs
        a = math.radians(45 + k * 90)
        cx, cy = math.cos(a), math.sin(a)
        tilt_box(g, WHITE, (cx * 540, cy * 540, 0), (cx * 300, cy * 300, 560), 36, 60)
        tilt_box(g, WHITE, (cx * 300, cy * 300, 540), (cx * 420, cy * 420, 1230), 36, 50)
        g.sphere(CYAN, (cx * 540, cy * 540, 40), 34, steps=(3, 6), glow=G2)
    g.cyl(GLASS, (176, -70, 14), 58, 1180, steps=6, glow=G1)                   # the lift
    g.cyl(GOLD, (176, -70, 760), 62, 110, steps=6, glow=G1)
    g.cyl(VIO, (0, 0, 1180), 210, 150, steps=12, top_r=580)                    # the drum
    g.cyl(CYAN, (0, 0, 1322), 590, 14, steps=12, glow=G2)
    g.cyl(GLASS, (0, 0, 1336), 566, 176, steps=12, glow=G1)
    g.cyl(WHITE, (0, 0, 1512), 620, 40, steps=12)
    g.cyl(MAG, (0, 0, 1552), 600, 14, steps=12, glow=G2)
    g.cyl(VIO, (0, 0, 1566), 580, 130, steps=12, top_r=200)
    g.cyl(WHITE, (0, 0, 1696), 200, 30, steps=8)
    g.cyl(GOLD, (0, 0, 1726), 26, 420, steps=5, top_r=6)
    for z, r in ((1800, 90), (1880, 60), (1950, 36)):
        g.cyl(GOLD, (0, 0, z), r, 10, steps=8, glow=G1)
    sputnik(g, 0, 0, 2150, 120, core=GOLD, ray=GOLD, tip=(MAG, CYAN), w=8)
    w = sc.part(P.motion(swing=26, pivot=(0, 0, 1400), axis=P.UP, rate=0.05, phase=0.3))   # the window band turns
    for k in range(12):
        a = math.radians(k * 30)
        w.box(WHITE, (math.cos(a) * 574, math.sin(a) * 574, 1424), (20, 20, 180), rot=(0, k * 30, 0))
    for k in range(3):
        a = math.radians(40 + k * 120)
        w.box(GOLD, (math.cos(a) * 590, math.sin(a) * 590, 1424), (16, 150, 60), rot=(0, 40 + k * 120, 0), glow=G2)
    shrub(g, -420, -380, 14)
    shrub(g, 430, -360, 14, 40, SAGE_D)


def motel_office(sc):
    """15.4 x 9.8 m, 24 m to the needle's tip. A butterfly roof over a leaning glass office, a wing of rooms with
    magenta and cyan doors, a folded-plate carport, a luggage cart; on a white tripod a gold needle with three blank
    lit panels and a starburst."""
    g = sc.still()
    pad(g, 1540, 980)
    roof = [(-420, 520), (60, 380), (620, 500)]
    under = lambda x: lerp_line(roof, max(-420, min(620, x))) - 40.0
    g.side(VIO, [(-330, 14), (540, 14), (540, under(540) + 4), (60, under(60) + 4), (-330, under(-330) + 4)], -60, 260)
    slab(g, WHITE, roof, 40, -330, 330)
    slab(g, MAG, [(-420, 474), (60, 334), (620, 454)], 14, -338, -330, glow=G2)
    glass_front(g, -310, 520, -62, 50, under, 4, skip=(3,))
    g.box(WARM, (416, -66, 140), (150, 12, 240), glow=G1)
    g.box(CYAN, (105, -66, 30), (850, 22, 34))
    g.box(VIO2, (150, 370, 164), (1180, 200, 300))                            # the rooms
    g.box(WHITE, (150, 356, 320), (1240, 260, 16))
    for k in range(5):
        x = -330 + k * 240
        g.box(MAG if k % 2 else CYAN, (x, 268, 114), (90, 8, 200))
        g.box(BULB, (x, 262, 250), (30, 10, 22), glow=G2)
    top = [(540 + k * 105, 330 + (70 if k % 2 else 0)) for k in range(3)]     # the carport: folded plates
    slab(g, WHITE, top, 22, -300, 200)
    for y in (-250, 150):
        tilt_box(g, WHITE, (700, y, 14), (640, y, 330), 26, 26)
        tilt_box(g, WHITE, (700, y, 14), (750, y, 330), 26, 26)
    g.box(GOLD, (610, -120, 150), (140, 60, 12))                              # the luggage cart
    for dx in (-64, 64):
        g.box(GOLD, (610 + dx, -120, 90), (8, 8, 160))
        g.cyl(SYN, (610 + dx, -120, 14), 14, 14, steps=6)
    g.box(GOLD, (610, -120, 172), (136, 8, 8))
    g.box(WHITE, (590, -120, 54), (70, 50, 44))
    g.box(MAG, (640, -120, 44), (44, 46, 30))
    for s in (-1, 0, 1):                                                       # the tripod and the needle
        tilt_box(g, WHITE, (-590 + s * 130, 40 - abs(s) * 110 + 60, 14), (-590, 0, 960), 40, 40)
    g.cyl(WHITE, (-590, 0, 940), 46, 60, steps=6)
    g.cyl(GOLD, (-590, 0, 1000), 24, 1380, steps=5, top_r=5)
    for z, col, w in ((640, CYAN, 250), (760, MAG, 300), (880, GOLD, 220)):
        g.box(SYN, (-590, -84, z), (w + 24, 26, 100))
        g.box(col, (-590, -100, z), (w, 10, 76), glow=G2)
    sputnik(g, -590, 0, 1700, 190, core=WARM, ray=WHITE, tip=(MAG, CYAN, GOLD), w=10)
    for z, r in ((1250, 70), (1340, 50), (1420, 34)):
        g.cyl(MAG, (-590, 0, z), r, 10, steps=8, glow=G2)
    shrub(g, -380, -200, 14)
    shrub(g, 240, -220, 14, 38, SAGE_D)


def bowling_alley(sc):
    """15.6 x 9.6 m, 16.5 m to the pin's head. A long hall under a folded-plate front, lane lights along the wall, a
    giant white pin with magenta bands rocking on the roof beside a cyan ball, a starburst on a mast."""
    g = sc.still()
    pad(g, 1560, 960)
    g.box(VIO, (0, 130, 250), (1460, 660, 472))
    g.box(VIO2, (0, 130, 500), (1500, 700, 30))
    top = [(-760 + k * 190, 560 + (90 if k % 2 else 0)) for k in range(9)]
    slab(g, WHITE, top, 26, -440, -150)
    slab(g, MAG, [(x, z - 26) for x, z in top], 10, -448, -440, glow=G2)
    for x in (-700, -240, 240, 700):
        tilt_box(g, WHITE, (x, -400, 14), (x + 40, -400, 540), 30, 30)
    g.box(MAG, (0, -204, 440), (1460, 10, 36), glow=G2)
    g.box(CYAN, (0, -204, 392), (1460, 10, 12), glow=G2)
    for k in range(7):                                                         # the lane lights
        disc(g, (CYAN, GOLD)[k % 2], -630 + k * 210, -204, 300, 44, 12, 8, glow=G2)
    g.box(GLASS, (0, -204, 130), (420, 10, 220), glow=G1)
    for x in (-210, 0, 210):
        g.box(WHITE, (x, -208, 130), (16, 16, 232))
    for k in range(3):                                                         # the steps
        g.box(WHITE, (0, -260 - k * 40, 14 + (2 - k) * 12), (520, 44, 14))
    g.box(WHITE, (330, 100, 540), (420, 420, 50))                              # the pin's stand
    g.cyl(MAG, (330, 100, 565), 180, 16, steps=10, glow=G1)
    pin = sc.part(P.motion(swing=4, pivot=(330, 100, 570), axis=90, rate=0.22, phase=0.1))
    for z, r0, r1, h in ((580, 120, 175, 260), (840, 175, 180, 140), (980, 180, 86, 300), (1280, 86, 74, 110)):
        pin.cyl(WHITE, (330, 100, z), r0, h, steps=10, top_r=r1)
    pin.sphere(WHITE, (330, 100, 1500), 130, steps=(5, 10))
    pin.cyl(MAG, (330, 100, 1290), 92, 22, steps=10, top_r=88)
    pin.cyl(MAG, (330, 100, 1340), 86, 22, steps=10, top_r=82)
    g.sphere(CYAN, (-60, 60, 660), 150, steps=(6, 10))                         # the ball
    g.box(WHITE, (-60, 60, 525), (220, 220, 30))
    for dx, dz in ((-40, 60), (20, 78), (-4, 20)):
        g.box(SYN, (-60 + dx, -78 + abs(dx) * 0.2, 660 + dz), (30, 20, 30))
    tilt_box(g, WHITE, (-620, 100, 500), (-640, 100, 1180), 30, 30)            # the mast
    burst(g, -640, 80, 1260, 200, 110, 8, 18)
    g.sphere(MAG, (-640, 80, 1260), 44, steps=(3, 6), glow=G2)
    shrub(g, 560, -300, 14)
    shrub(g, -560, -300, 14, 40, SAGE_D)


def drive_in(sc):
    """About 11 x 11 m as it stands, 19 m tall. A drive-in's screen tower, a quarter turned in the mesh: on the right
    of the road its lit face looks back down the road, on the left the driver sees its ribbed back and its side. A
    ticket booth under a butterfly roof, a row of speaker posts."""
    g = sc.still(yaw=-40.0)
    for x in (-420, 420):                                                      # two tapered legs
        g.side(VIO, [(x - 110, 0), (x + 110, 0), (x + 60, 900), (x - 60, 900)], -20, 160)
        g.box(WHITE, (x, -26, 450), (26, 12, 900))
    g.box(VIO, (0, 80, 1370), (1300, 150, 960))
    g.box(WHITE, (0, 0, 1370), (1340, 30, 1000))
    g.box(BULB, (0, -18, 1370), (1260, 10, 920), glow=0.3)                     # the picture: dusk over a ridge
    g.box(CYAN, (0, -24, 1160), (1260, 6, 20), glow=G2)
    disc(g, MAG, -260, -26, 1420, 190, 8, 12, glow=G2)
    g.side(VIO2, [(-630, 910), (630, 910), (630, 1060), (300, 1150), (60, 1060), (-240, 1180), (-630, 1040)], -30, -22)
    for k in range(6):                                                         # the ribbed back
        g.box(WHITE, (-550 + k * 220, 164, 1370), (24, 22, 960))
    for z in (980, 1370, 1760):
        g.box(WHITE, (0, 170, z), (1300, 14, 22))
    burst(g, 0, 190, 1370, 300, 170, 8, 20, col=MAG, glow=G2)
    disc(g, GOLD, 0, 196, 1370, 60, 12, 8, glow=G2)
    g.box(GOLD, (0, 60, 1868), (1380, 200, 24))
    for x in (-600, 0, 600):
        g.sphere(WARM, (x, 60, 1910), 30, steps=(3, 6), glow=G3)
    g.box(SYN, (0, -10, 830), (900, 44, 120))                                  # a blank marquee under the screen
    g.box(MAG, (0, -36, 830), (840, 8, 70), glow=G2)
    for k in range(9):
        g.box(BULB, (-400 + k * 100, -40, 900), (24, 10, 24), glow=G3)
        g.box(BULB, (-400 + k * 100, -40, 760), (24, 10, 24), glow=G3)
    for s in (-1, 1):                                                          # fins on the screen's ends
        g.side(MAG, [(s * 670, 900), (s * 760, 1500), (s * 670, 1850)], -10, 10, glow=G2)
    g = sc.still()
    pad(g, 1100, 1080)
    g.box(VIO, (-320, -330, 120), (240, 200, 212))                             # the ticket booth
    g.box(GLASS, (-320, -432, 150), (200, 8, 90), glow=G1)
    slab(g, WHITE, [(-500, 300), (-320, 240), (-140, 300)], 18, -480, -200)
    g.box(MAG, (-320, -436, 80), (240, 8, 24), glow=G2)
    g.box(WHITE, (-140, -400, 90), (220, 12, 12))                              # the barrier arm, up
    for k in range(5):                                                         # the speaker posts
        x, y = -60 + k * 110, -450
        g.cyl(WHITE, (x, y, 14), 7, 110, steps=5)
        g.box(CYAN if k % 2 else MAG, (x, y, 130), (44, 20, 26))
    shrub(g, 380, 380, 14)


def arcade_hall(sc):
    """15 x 9.4 m, 13 m to the bell's frame. Three white vaults over a violet hall, a leaning marquee of lit shapes
    (cherries, a bell, a star, a lemon, a horseshoe) inside a row of steady bulbs, pinball bumpers by the door, and
    a gold bell swinging in a frame on the roof."""
    g = sc.still()
    pad(g, 1500, 940)
    g.box(VIO, (0, 120, 220), (1400, 640, 412))
    vault = []
    for b in range(3):
        for k in range(1 if b else 0, 7):
            a = math.pi * (1.0 - k / 6.0)
            vault.append((-470 + b * 470 + math.cos(a) * 235, 430 + math.sin(a) * 190))
    g.side(WHITE, vault + [(705, 400), (-705, 400)], -260, 440)
    g.side(VIO2, [(x * 0.94, 414 + (z - 430) * 0.86) for x, z in vault] + [(660, 410), (-660, 410)], -272, -260)
    tilt_box(g, SYN, (0, -230, 300), (0, -330, 600), 1160, 26)                 # the marquee, leaning out
    yq = lambda z: -244 - (z - 300) * 0.333 - 16
    zc = 450
    for dx in (-40, 40):                                                       # cherries
        disc(g, MAG, -440 + dx, yq(zc - 30), zc - 30, 44, 10, 8, glow=G2)
        tilt_box(g, SAGE, (-440 + dx, yq(zc), zc + 10), (-440, yq(zc + 90), zc + 90), 8, 8)
    g.side(GOLD, [(-290, 392), (-150, 392), (-170, 430), (-180, 500), (-220, 530), (-260, 500), (-270, 430)], yq(zc) - 5, yq(zc) + 5, glow=G2)   # bell
    disc(g, WHITE, -220, yq(385), 385, 16, 10, 6, glow=G2)
    burst(g, 0, yq(zc), zc, 86, 46, 8, 14, col=CYAN, glow=G2)                  # star
    disc(g, GOLD, 220, yq(zc), zc, 60, 10, 10, glow=G2, sx=1.35)               # lemon
    disc(g, WHITE, 300, yq(zc), zc, 14, 10, 5, glow=G2)
    arc(g, MAG, 440, yq(zc + 10), zc + 20, 54, 150, 390, 7, 10, 22, glow=G2)   # horseshoe, open end up
    for k in range(11):                                                        # the bulbs
        for z in (322, 578):
            g.box(BULB, (-550 + k * 110, yq(z), z), (26, 12, 26), glow=G3)
    g.box(WARM, (0, -204, 140), (360, 12, 250), glow=G1)                       # the door
    g.box(MAG, (-196, -206, 140), (22, 16, 260), glow=G2)
    g.box(MAG, (196, -206, 140), (22, 16, 260), glow=G2)
    for x, col in ((-420, CYAN), (420, MAG)):                                  # pinball bumpers
        g.cyl(WHITE, (x, -360, 14), 40, 110, steps=8)
        g.cyl(col, (x, -360, 124), 86, 30, steps=8, top_r=60, glow=G1)
        g.cyl(WHITE, (x, -360, 154), 60, 12, steps=8, top_r=20)
    for s in (-1, 1):                                                          # the bell's frame
        tilt_box(g, WHITE, (s * 310, 150, 600), (s * 236, 150, 1300), 30, 30)
    g.box(WHITE, (0, 150, 1290), (520, 40, 30))
    bell = sc.part(P.motion(swing=11, pivot=(0, 150, 1270), axis=90, rate=0.3, phase=0.4))
    bell.cyl(GOLD, (0, 150, 960), 170, 60, steps=10, top_r=150, glow=0.2)
    bell.cyl(GOLD, (0, 150, 1020), 150, 150, steps=10, top_r=84, glow=0.2)
    bell.cyl(GOLD, (0, 150, 1170), 84, 70, steps=10, top_r=30, glow=0.2)
    bell.cyl(WHITE, (0, 150, 1236), 12, 40, steps=5)
    bell.sphere(MAG, (0, 150, 930), 44, steps=(3, 6), glow=G2)
    shrub(g, 640, -330, 14)
    shrub(g, -640, -330, 14, 40, SAGE_D)


def soda_bar(sc):
    """9 x 6 m, 8.8 m tall. A walk-up soda stand: a round-ended counter under a tilted disc on one stalk, stools, and
    on the disc a giant soda glass (cream, magenta, a cyan straw, a cherry) whose bubbles rise."""
    g = sc.still()
    pad(g, 900, 560, foot=160.0)
    g.box(VIO, (0, 60, 74), (560, 200, 120))
    for s in (-1, 1):
        g.cyl(VIO, (s * 280, 60, 14), 100, 120, steps=8)
    g.box(WHITE, (0, 50, 140), (620, 250, 12))
    for s in (-1, 1):
        g.cyl(WHITE, (s * 280, 50, 134), 125, 12, steps=8)
    g.box(MAG, (0, -42, 60), (560, 8, 20), glow=G2)
    for k in range(5):                                                         # stools
        x = -320 + k * 160
        g.cyl(WHITE, (x, -150, 14), 8, 58, steps=5)
        g.cyl(CYAN if k % 2 else MAG, (x, -150, 72), 30, 12, steps=8)
    g.cyl(WHITE, (0, 110, 14), 44, 400, steps=8, top_r=30)                     # the stalk and the disc
    g.cyl(WHITE, (0, 10, 392), 290, 26, steps=12, rot=(0, 0, 9))
    g.cyl(CYAN, (0, 10, 384), 298, 10, steps=12, rot=(0, 0, 9), glow=G2)
    g.cyl(VIO2, (0, 10, 416), 250, 12, steps=12, rot=(0, 0, 9))
    for s in (-1, 1):
        g.box(GLASS, (s * 150, 150, 220), (120, 10, 130), glow=G1)             # the back bar's mirrors
    g.box(VIO, (0, 170, 190), (460, 30, 352))
    g.cyl(WHITE, (40, 80, 420), 70, 16, steps=8)                               # the glass
    g.cyl(WHITE, (40, 80, 436), 40, 60, steps=8)
    g.cyl(WHITE, (40, 80, 490), 60, 240, steps=10, top_r=112)
    g.cyl(MAG, (40, 80, 560), 80, 150, steps=10, top_r=106, glow=0.25)
    g.sphere(WHITE, (40, 80, 744), 96, scale=(1, 1, 0.6), steps=(4, 8))
    g.sphere(MAG, (20, 70, 800), 34, steps=(3, 6), glow=G1)
    tilt_box(g, CYAN, (70, 80, 560), (150, 80, 880), 16, 16, glow=G2)
    b = sc.part(P.motion(bob=-12, rate=0.4, phase=0.5))
    for dx, dz, r in ((-70, 700, 16), (110, 760, 12), (-40, 820, 10)):
        b.sphere(CYAN, (40 + dx, 60, dz), r, steps=(3, 5), glow=G2)


# ------------------------------------------------------------------------------------------ props
def star_lamp(sc):
    """A lamp of three starbursts on a leaning white pole, 8.6 m, in a violet tub. Steady."""
    g = sc.still()
    g.cyl(VIO, (0, 0, -150), 80, 200, steps=8)
    g.cyl(MAG, (0, 0, 50), 86, 10, steps=8)
    tilt_box(g, WHITE, (0, 0, 40), (30, 0, 620), 20, 20)
    for (x, y, z), R in (((30, 0, 760), 96), ((-78, -30, 640), 74), ((112, 30, 560), 64)):
        tilt_box(g, WHITE, (30 if z > 700 else 26, 0, 600 if z > 700 else 480), (x, y, z), 10, 10)
        sputnik(g, x, y, z, R, core=WARM, ray=WHITE, tip=(GOLD,), w=6, n=10)


def arrow_sign(sc):
    """A gold ring on two white legs with a magenta arrow swooping through it down to the door, 10.6 m, a row of
    steady bulbs along the arrow. No words."""
    g = sc.still()
    g.box(VIO, (0, 0, -60), (300, 110, 180))
    g.box(WHITE, (0, 0, 34), (320, 124, 10))
    for s in (-1, 1):
        tilt_box(g, WHITE, (s * 110, 0, 30), (s * 40, 0, 640), 24, 24)
    arc(g, GOLD, 0, 0, 800, 200, 0, 360, 12, 30, 36, glow=G1)
    arc(g, CYAN, 0, -18, 800, 160, 0, 360, 10, 8, 10, glow=G2)
    g.side(MAG, [(-230, 1060), (-150, 1060), (-20, 880), (110, 640), (150, 560), (210, 590), (160, 380), (-10, 500), (50, 520), (20, 620), (-110, 850)],
           -44, -30, glow=G2)
    for k in range(7):
        t = k / 6.0
        g.box(BULB, (-170 + 270 * t + 40 * math.sin(t * 3.0), -50, 1030 - 480 * t), (22, 10, 22), glow=G3)
    for z, col in ((250, CYAN), (160, MAG)):
        g.box(SYN, (0, -14, z), (190, 30, 70))
        g.box(col, (0, -32, z), (166, 8, 50), glow=G2)


def tube_cactus(sc):
    """A saguaro bent out of neon tube, magenta trunk and cyan arms, 7.3 m, on a kidney plinth with a gold rim."""
    g = sc.still()
    kid = [(-130, -60), (-60, -90), (60, -80), (130, -30), (120, 50), (40, 80), (-20, 50), (-90, 80), (-140, 20)]
    g.plan(VIO, kid, -150, 40)
    g.plan(GOLD, [(x * 1.06, y * 1.06) for x, y in kid], 40, 50)
    for dx in (-24, 0, 24):                                                    # the trunk: three tubes
        g.cyl(MAG, (dx, 0, 50), 9, 560 + (60 if dx == 0 else 0), steps=5, glow=G2)
    arc(g, MAG, 0, 0, 610, 24, 0, 180, 4, 16, 16, glow=G2)
    for s, z0, r, h in ((-1, 250, 110, 190), (1, 330, 130, 240)):               # the arms: an elbow and a riser
        for dz in (-12, 12):
            tilt_box(g, CYAN, (s * 30, 0, z0 + dz), (s * (r + 12), 0, z0 + dz), 14, 14, glow=G2)
            g.cyl(CYAN, (s * (r + dz * s), 0, z0 + dz - 6), 8, h - dz, steps=5, glow=G2)
        arc(g, CYAN, s * r, 0, z0 + h, 12, 0, 180, 3, 14, 14, glow=G2)
    burst(g, 0, -14, 700, 44, 26, 8, 8, col=GOLD, glow=G2)                     # the flower
    for x, y in ((-90, -30), (80, 20)):
        g.sphere(SAGE, (x, y, 66), 26, scale=(1, 1, 0.7), steps=(3, 6))


def phone_kiosk(sc):
    """A telephone and news kiosk, 3.4 x 2 m, 4.3 m: a folded-plate roof on white posts, a glass booth, a rack of
    papers and magazines (blank covers), a gold bell on the roof."""
    g = sc.still()
    g.box(PAVE, (0, 0, -70), (340, 200, 160))
    for x in (-150, -20, 150):
        for y in (-80, 80):
            g.box(WHITE, (x, y, 140), (12, 12, 260))
    slab(g, WHITE, [(-190 + k * 76, 290 + (46 if k % 2 else 0)) for k in range(6)], 14, -120, 120)
    slab(g, MAG, [(-190 + k * 76, 276 + (46 if k % 2 else 0)) for k in range(6)], 8, -126, -120, glow=G2)
    g.box(VIO, (-86, 30, 130), (120, 110, 240))                                # the booth
    g.box(GLASS, (-86, -28, 160), (96, 8, 150), glow=G1)
    g.box(SYN, (-86, -34, 150), (30, 8, 70))
    g.box(CYAN, (-86, -30, 256), (110, 10, 18), glow=G2)
    g.box(VIO2, (66, 50, 110), (160, 40, 200))                                 # the rack
    for r in range(3):
        for k in range(4):
            g.box((WHITE, MAG, CYAN, GOLD)[(r + k) % 4], (6 + k * 40, 26, 60 + r * 62), (32, 8, 44), rot=(0, 0, 0))
    g.box(WHITE, (66, -30, 50), (150, 60, 12))
    g.cyl(MAG, (150, -60, 10), 22, 40, steps=6)                                # a stool
    disc(g, GOLD, 60, -60, 390, 40, 12, 8, glow=G2)                            # the bell sign
    g.box(GOLD, (60, -60, 340), (10, 10, 30))
    g.sphere(WARM, (-86, 0, 262), 18, steps=(3, 5), glow=G3)


def atom_pylon(sc):
    """A white tripod with a gold ball and three crossed rings (the atom of every roadside sign of its day) turning
    a little to and fro, 9.4 m, cyan and magenta beads on the rings."""
    g = sc.still()
    g.cyl(VIO, (0, 0, -150), 150, 180, steps=8)
    g.cyl(GOLD, (0, 0, 30), 156, 10, steps=8)
    for k in range(3):
        a = math.radians(90 + k * 120)
        tilt_box(g, WHITE, (math.cos(a) * 120, math.sin(a) * 120, 30), (0, 0, 640), 22, 22)
    g.cyl(WHITE, (0, 0, 620), 14, 120, steps=5)
    g.sphere(GOLD, (0, 0, 780), 62, steps=(4, 8), glow=G1)
    for k in range(3):
        m = sc.part(P.motion(swing=16, pivot=(0, 0, 780), axis=P.UP, rate=0.1, phase=0.2))
        m.base = P.ft_art._xf((0, 0, 780), (k * 60, 0, 0), (1, 1, 1))
        arc(m, WHITE, 0, 0, 0, 160, 0, 360, 12, 10, 10, sx=0.4)
        m.sphere((CYAN, MAG, CYAN)[k], (0, 0, 160 if k != 1 else -160), 20, steps=(3, 5), glow=G2)


def fin_pylons(sc):
    """Three swept fins in a row, 4.4 m long, 5.6 m: violet with a white leading edge and a lit diamond each
    (magenta, cyan, gold), a low white rail between them."""
    g = sc.still()
    g.box(PAVE, (0, 0, -70), (460, 90, 160))
    for k, col in enumerate((MAG, CYAN, GOLD)):
        x = -160 + k * 160
        h = 440 + k * 60
        g.side(VIO, [(x - 50, 10), (x + 50, 10), (x + 16, h), (x - 70, h + 40)], -14, 14)
        g.side(WHITE, [(x - 62, 10), (x - 50, 10), (x - 70, h + 40), (x - 84, h + 48)], -18, 18)
        g.box(col, (x - 26, 0, h - 90), (44, 40, 44), rot=(45, 0, 0), glow=G2)
    g.box(WHITE, (0, 0, 60), (400, 10, 10))
    g.box(WHITE, (0, 0, 120), (400, 10, 10))
    burst(g, -32, 0, 640, 60, 34, 8, 10, col=GOLD, glow=G2)
    for k in range(2):                                                         # a shrub between each pair
        g.sphere(SAGE if k else SAGE_D, (-100 + k * 160, 0, 30), 30, scale=(1, 1, 0.7), steps=(3, 6))


def kidney_planter(sc):
    """LOW (1.8 m). A kidney-shaped planter, violet with a white rim, a fan of agave and a small bloom. No light."""
    g = sc.still()
    kid = [(-150, -50), (-70, -85), (60, -80), (150, -30), (140, 50), (50, 85), (-20, 50), (-100, 80), (-160, 20)]
    g.plan(VIO, kid, -60, 50)
    g.plan(WHITE, [(x * 1.05, y * 1.06) for x, y in kid], 50, 60)
    g.plan(COFFEE, [(x * 0.9, y * 0.88) for x, y in kid], 60, 64)
    for k in range(9):                                                         # the agave's fan
        a = math.radians(k * 40)
        L = 86 if k % 2 else 104
        tilt_box(g, SAGE if k % 2 else SAGE_D, (40 + math.cos(a) * 10, math.sin(a) * 8, 62), (40 + math.cos(a) * L * 0.62, math.sin(a) * L * 0.5, 62 + L), 18, 9)
    g.box(SAGE_D, (40, 0, 120), (9, 9, 110))
    g.sphere(MAG, (40, 0, 172), 13, steps=(3, 5))
    for x, y, r in ((-90, 10, 24), (-60, -40, 18)):
        g.sphere(SAGE, (x, y, 70 + r * 0.5), r, scale=(1, 1, 0.7), steps=(3, 6))
        g.sphere(GOLD, (x, y, 70 + r * 1.1), 7, steps=(3, 4))


def rope_podium(sc):
    """LOW (1.4 m). A doorman's podium, violet with a gold top and a white ledger, on a magenta carpet between gold
    rope posts with magenta ropes. No light."""
    g = sc.still()
    g.box(PAVE, (0, 0, -24), (420, 150, 60))
    g.box(MAG, (0, 0, 8), (400, 90, 5))
    g.box(VIO, (150, 30, 60), (56, 46, 108))
    g.box(GOLD, (150, 24, 118), (70, 60, 8), rot=(0, 0, -14))
    g.box(WHITE, (150, 20, 126), (40, 30, 5), rot=(0, 0, -14))
    xs = (-190, -90, 10)
    for y in (-52, 52):
        for x in xs:
            g.cyl(GOLD, (x, y, 6), 14, 6, steps=6)
            g.cyl(GOLD, (x, y, 12), 5, 82, steps=5)
            g.sphere(GOLD, (x, y, 100), 9, steps=(3, 5))
        for a, b in zip(xs, xs[1:]):
            mid = (a + b) / 2.0
            tilt_box(g, MAG, (a, y, 92), (mid, y, 74), 6, 6)
            tilt_box(g, MAG, (mid, y, 74), (b, y, 92), 6, 6)


# ------------------------------------------------------------------------------------------ the hero
def lucky_horseshoe(sc):
    """THE LUCKY HORSESHOE. 35 m wide, 45 m tall: a golden horseshoe standing open end up on a violet plinth with
    white ribs, seven cyan studs a side on both faces, and a magenta eight-point star rocking slowly inside the arch."""
    g = sc.still()
    g.box(VIO, (0, 0, 150), (1700, 1000, 900))                                 # the plinth: 3 m of footing
    g.box(VIO2, (0, 0, 640), (1500, 860, 80))
    g.box(GOLD, (0, 0, 596), (1740, 1040, 28), glow=0.2)
    for k in range(9):
        x = -800 + k * 200
        tilt_box(g, WHITE, (x, -520, 0), (x, -500, 600), 30, 30)
        tilt_box(g, WHITE, (x, 520, 0), (x, 500, 600), 30, 30)
    for k in range(8):
        g.box(CYAN, (-700 + k * 200, -506, 60), (60, 12, 26), glow=G2)
    g.box(WARM, (0, -504, 150), (150, 12, 260), glow=G1)                       # a door, for the size of it
    for k in range(4):
        g.box(WHITE, (0, -540 - k * 40, 14 + (3 - k) * 10 - 20), (300, 44, 20))
    g.box(MAG, (0, -508, 330), (360, 12, 30), glow=G2)
    for k in range(17):                                                        # steady bulbs round the plinth's crown
        g.box(BULB, (-800 + k * 100, -526, 560), (30, 12, 30), glow=G3)
    for s in (-1, 1):                                                          # two swept buttress fins
        g.side(VIO2, [(s * 850, 0), (s * 1250, 0), (s * 900, 1000)], -40, 40)
        tilt_box(g, WHITE, (s * 1250, 0, 0), (s * 900, 0, 1000), 100, 30)
        tilt_box(g, MAG, (s * 1290, 0, 0), (s * 940, 0, 1000), 20, 20, glow=G2)
    cx, cz, R, W, D = 0.0, 2520.0, 1450.0, 360.0, 440.0
    g.box(VIO, (0, 0, 800), (700, 420, 340))                                   # the saddle the shoe sits in
    g.box(GOLD, (0, 0, 968), (760, 470, 24), glow=0.2)
    arc(g, GOLD, cx, 0, cz, R, 180, 360, 16, W, D, glow=0.22)                  # the bow
    arc(g, WHITE, cx, 0, cz, R - D / 2.0 - 8, 180, 360, 16, W + 30, 20)        # a white rib inside it
    arms = [(1450, 2520), (1440, 3000), (1400, 3480), (1320, 3950), (1250, 4300)]
    for s in (-1, 1):
        for (x0, z0), (x1, z1) in zip(arms, arms[1:]):
            tilt_box(g, GOLD, (s * x0, 0, z0 - 14), (s * x1, 0, z1 + 14), W, D, glow=0.22)
            tilt_box(g, WHITE, (s * (x0 - D / 2.0 - 8), 0, z0), (s * (x1 - D / 2.0 - 8), 0, z1), W + 30, 20)
        g.box(GOLD, (s * 1215, 0, 4400), (560, W + 70, 200), glow=0.22)        # the heel
        g.box(WHITE, (s * 1215, 0, 4290), (580, W + 90, 20))
        sputnik(g, s * 1215, 0, 4600, 110, core=WARM, ray=WHITE, tip=(CYAN,), w=12)
    studs = [(a, R * math.cos(math.radians(a)), cz + R * math.sin(math.radians(a))) for a in (200, 225, 250)]
    pts = [(x, z) for _, x, z in studs] + [(1445, 2760), (1420, 3240), (1360, 3720), (1285, 4130)]
    for s in (-1, 1):
        for x, z in pts:
            for f in (-1, 1):
                g.box(CYAN, (s * abs(x), f * (W / 2.0 + 8), z), (150, 16, 110), glow=G3)
    disc(g, CYAN, 0, -(W / 2.0 + 8), cz - R, 80, 16, 8, glow=G3)
    disc(g, CYAN, 0, (W / 2.0 + 8), cz - R, 80, 16, 8, glow=G3)
    st = sc.part(P.motion(swing=11, pivot=(0, 0, 2760), axis=90, rate=0.08, phase=0.25))   # the star
    def star(R1, R2, r):
        out = []
        for k in range(16):
            a = math.radians(90 + k * 22.5)
            rr = R1 if k % 4 == 0 else (R2 if k % 4 == 2 else r)
            out.append((math.cos(a) * rr, 2760 + math.sin(a) * rr))
        return out
    st.side(MAG, star(900, 600, 250), -70, 70, glow=G2)
    st.side(WHITE, star(620, 400, 170), -90, 90, glow=G1)
    st.side(GOLD, star(330, 210, 100), -106, 106, glow=G2)
    disc(st, CYAN, 0, 0, 2760, 70, 230, 8, glow=G3)
    for s in (-1, 1):                                                          # the bosses it turns between
        g.box(GOLD, (s * 1120, 0, 2760), (240, 120, 120), glow=0.22)
        g.box(WHITE, (s * 990, 0, 2760), (30, 170, 170))


JOBS = [('SM_S1D4_CoffeeShop', coffee_shop, SMALL), ('SM_S1D4_DrumLounge', drum_lounge, SMALL), ('SM_S1D4_MotelOffice', motel_office, STILL),
        ('SM_S1D4_BowlingAlley', bowling_alley, dict(grow=(90, 40, 40), grow_down=(90, 40, 0))), ('SM_S1D4_DriveIn', drive_in, STILL), ('SM_S1D4_ArcadeHall', arcade_hall, dict(grow=(80, 40, 40), grow_down=(80, 40, 0))),
        ('SM_S1D4_SodaBar', soda_bar, SMALL), ('SM_S1D4_StarLamp', star_lamp, STILL), ('SM_S1D4_ArrowSign', arrow_sign, STILL),
        ('SM_S1D4_TubeCactus', tube_cactus, STILL), ('SM_S1D4_PhoneKiosk', phone_kiosk, STILL), ('SM_S1D4_AtomPylon', atom_pylon, SMALL),
        ('SM_S1D4_FinPylons', fin_pylons, STILL), ('SM_S1D4_KidneyPlanter', kidney_planter, STILL), ('SM_S1D4_RopePodium', rope_podium, STILL),
        ('SM_S1D4_LuckyHorseshoe', lucky_horseshoe, dict(far=True, grow=(60, 60, 60), grow_down=(60, 60, 0)))]


def run(only=None):
    P.build_core()                              # loads the two masters; never build_core(rebuild=True)
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
