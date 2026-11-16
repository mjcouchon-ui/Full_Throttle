"""Under the Sea, Stage 1, builder s1plants: what grows on the seabed, district by district.
/Game/World/UnderTheSea/SM_S1PLANTS_*. Run: import ft_world_sea_s1plants as K; K.run()

The kit (ft_dress_abyss.py) already has sea grass, anemones, sea whips, kelp forests and rafts, coral gardens and low
reef, and the level has its own SM_Kelp and SM_Coral: none of those is made again. Sixteen pieces, after the zone plan:
  the reef                    SeaFans, TableCoral, BrainCoral, TubeSponges
  the kelp forest             SeaPalm, RibbonHedge, SeaLettuce
  the research station        SeaPens (a planted row, blank tags), SpiralWhip, TubeSponges
  the salvage field           WreckRib (a ship's frame hung with weed), SpiralWhip, SeaLettuce
  the approach (the vents)    TubeWorms (plumes that draw in and out)
  Nereida, the Pearl Quarter  PearlVine (on a trellis), GardenUrn, SeaFans, TableCoral
  Nereida, the Bubble Works   SeaLily (a crown that opens and closes), SeaPalm, TubeWorms
  Nereida, Lamp Row           LanternWeed (steady glow: 24 m and more out), SeaLily, SeaPens, RibbonHedge
  Nereida, the Crown          PearlVine, GardenUrn, BrainCoral, LanternWeed
  everywhere                  StarMoss
Every piece is one ft_people.Scene (one section, M_People). Everything stirs in one current (CUR): stalks and crowns
rock about their feet, the more the higher; hanging weed swings about where it hangs; plumes float up and down. No
actor, no tick: the builders' motion parameters only. Nothing here is orange or mint at 1 to 2 m, nothing is a box,
nothing has a fin or a jaw. LOW pieces (BrainCoral, RibbonHedge, SeaLettuce, StarMoss) are drawn under 1.7 m.
"""
import math
import random
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/UnderTheSea'

# the kit's palette (ft_dress_abyss.py) and the zone plan's district colours, nothing else
SAND_D = '#b9a77e'
ROCK, ROCK_D, ROCK_L = '#9a6a7e', '#6a4562', '#c79aa2'
KELP, KELP_D, KELP_L, DEEP, PALE = '#3f8a4a', '#2a6a3a', '#7fcf6a', '#1f5a3a', '#f1fbe6'
CORAL, PEACH, MAG, LAV, PINK = '#ff7a6a', '#ffb04f', '#ff4fb8', '#b9a4ff', '#ff9ec7'
CREAM, WHITE, MARBLE, GOLD, YEL = '#fff1c0', '#fff6ea', '#f4efe0', '#c9b44a', '#ffd23f'
VIOLET, RED, SOOT, STATION = '#8a5a9a', '#ff3355', '#3a3140', '#cfe3ea'
HULL, HULL_D, WOOD_D = '#8f6450', '#5c3f3a', '#6a4c36'

CUR = 20.0      # the axis everything rocks about (degrees in the ground plane): one current for the whole sea
LOW = dict(grow=(30, 30, 15), grow_down=(30, 30, 0))
TALL = dict(grow=(90, 90, 30), grow_down=(90, 90, 0))


def blob(m, col, c, rx, ry, rz, steps=(4, 6), rot=(0, 0, 0), glow=0.0):
    m.sphere(col, c, 100, scale=(rx / 100.0, ry / 100.0, rz / 100.0), steps=steps, rot=rot, glow=glow)


def sway(swing, pivot, rate=0.3, phase=0.0, axis=CUR):
    return P.motion(swing=swing, pivot=pivot, axis=axis, rate=rate, phase=phase)


def rock(m, x, y, w, d, h, seed, col=ROCK, col2=ROCK_L, sink=40.0):
    """A faceted boulder w x d on the sand, h tall, as much again under it."""
    r = random.Random(seed)
    m.sphere(col, (x, y, (h - sink) / 2.0), 100, scale=(w / 200.0, d / 200.0, (h + sink) / 200.0), steps=(4, 6), rot=(0, r.uniform(0, 60), 0))
    m.sphere(col2, (x + w * 0.3, y - d * 0.14, (h * 0.6 - sink) / 2.0), 100, scale=(w / 330.0, d / 280.0, (h * 0.6 + sink) / 200.0), steps=(4, 5),
             rot=(0, r.uniform(0, 60), 0))


def add(p, dx, dy, dz):
    return (p[0] + dx, p[1] + dy, p[2] + dz)


# ------------------------------------------------------------------------------------------ the reef
def _fan(sc, x, y, yaw, h, w, col, rib, phase, swing=4.5):
    """One flat sea fan facing the road: a scalloped web, five ribs and two veins proud of it, a pale bead at every rib's end."""
    m = sc.part(sway(swing, (x, y, 0), 0.32, phase), (x, y, 0), yaw)
    z0 = h * 0.18
    m.cyl(rib, (0, 0, -120), 10, 120 + z0 + 6, steps=5)
    pts = [(-w * 0.08, z0), (w * 0.08, z0), (w * 0.42, h * 0.5), (w * 0.5, h * 0.8), (w * 0.28, h * 0.97), (0, h), (-w * 0.3, h * 0.95),
           (-w * 0.5, h * 0.74), (-w * 0.4, h * 0.45)]
    m.side(col, pts, -3, 3)
    for k in range(5):
        a = math.radians(90 + (k - 2) * 27)
        tip = (math.cos(a) * w * 0.47, 0, z0 + math.sin(a) * (h - z0) * 0.94)
        tilt_box(m, rib, (0, 0, z0), tip, 10, 8)
        m.box(CREAM, tip, (15, 12, 15))
    m.box(rib, (w * 0.01, 0, h * 0.5), (w * 0.72, 9, 7))
    m.box(rib, (0, 0, h * 0.74), (w * 0.84, 9, 7))


def sea_fans(sc):
    """TALL, 3 m: three lattice fans on a rock, magenta, lavender and pink, each nodding on its own stem."""
    g = sc.still()
    rock(g, 10, 30, 420, 230, 56, 611)
    rock(g, -210, -20, 170, 130, 34, 612, ROCK_L, ROCK)
    _fan(sc, 20, 30, -8, 300, 270, MAG, PINK, 0.0)
    _fan(sc, -185, -10, 26, 215, 200, LAV, CREAM, 0.3, 5.5)
    _fan(sc, 190, -30, -30, 150, 150, PINK, MAG, 0.55, 6.5)
    _fan(sc, 70, -105, 12, 74, 84, PEACH, CORAL, 0.8, 8.0)             # a young one, under a metre
    t = sc.part(sway(9.0, (-60, -90, 20), 0.6, 0.4))
    for k in range(4):                                                  # a tuft of polyps at the foot
        t.cyl(CREAM, (-90 + k * 20, -90 + (k % 2) * 14, 6), 6, 30 + k * 7, steps=5, top_r=9)


def table_coral(sc):
    """TALL, 4.2 m: three tiers of plates on a trunk, polyps trembling on every plate, a small table at its foot."""
    r = random.Random(62)
    g = sc.still()
    rock(g, 0, 10, 360, 270, 54, 621)
    g.cyl(ROCK_L, (0, 0, -150), 50, 300, steps=7, top_r=30)
    tiers = ((0, 0, 150, 215, CORAL), (40, -20, 262, 160, PINK), (-25, 15, 360, 108, CORAL))
    for i, (x, y, z, ra, col) in enumerate(tiers):
        m = sc.part(sway(0.7 + 0.45 * i, (0, 0, 0), 0.22, 0.08 * i))
        m.cyl(ROCK_L, (x, y, z - 92), 22, 92, steps=7, top_r=ra * 0.8)            # the stem widening under the plate
        m.cyl(CREAM, (x, y, z), ra, 12, steps=9)                                    # the pale growing rim
        m.cyl(col, (x, y, z + 12), ra * 0.93, 14, steps=9, top_r=ra * 0.84)
        t = sc.part(sway(7.0, (x, y, z + 26), 0.7, 0.2 * i + 0.1))
        for k in range(5):
            a, d = r.uniform(0, 6.2832), r.uniform(0.15, 0.7) * ra
            t.cyl(MAG if k % 2 else CREAM, (x + math.cos(a) * d, y + math.sin(a) * d, z + 24), 7, r.uniform(20, 34), steps=5, top_r=10)
    s = sc.part(sway(1.6, (-205, -60, 0), 0.26, 0.5))                               # the small table: lavender
    s.cyl(ROCK_L, (-205, -60, -60), 16, 150, steps=6, top_r=62)
    s.cyl(CREAM, (-205, -60, 90), 84, 10, steps=8)
    s.cyl(LAV, (-205, -60, 100), 78, 12, steps=8, top_r=68)
    w = sc.part(sway(8.0, (190, -70, 0), 0.45, 0.2))                                # weed at the rock's foot
    for k in range(3):
        tilt_box(w, KELP_L if k % 2 else KELP, (175 + k * 22, -70, -20), (168 + k * 30, -74, 70 + k * 18), 5, 24)


def brain_coral(sc):
    """LOW, 1.4 m: two brain-coral domes with meandering ridges, tufts of weed at their feet, two little feather worms."""
    g = sc.still()
    blob(g, SAND_D, (20, 10, -22), 300, 190, 30, steps=(4, 8))

    def dome(cx, cy, rx, ry, rz, col, rid, n, segs):
        g.sphere(col, (cx, cy, 0), 100, scale=(rx / 100.0, ry / 100.0, rz / 100.0), steps=(6, 9))
        for j in range(n):
            prev = None
            for i in range(segs + 1):
                ph = math.radians(12 + i * (76.0 / segs))
                th = j * 6.2832 / n + (0.2 if i % 2 else -0.2)
                p = (cx + rx * 1.03 * math.sin(ph) * math.cos(th), cy + ry * 1.03 * math.sin(ph) * math.sin(th), rz * 1.03 * math.cos(ph))
                if prev:
                    tilt_box(g, rid, prev, p, 13, 9)
                prev = p

    dome(-45, 15, 152, 126, 128, PINK, MAG, 7, 5)
    dome(160, -45, 88, 76, 72, LAV, VIOLET, 5, 4)
    for n, (x, y) in enumerate(((-215, -60), (60, -120), (250, 40))):
        t = sc.part(sway(9.0, (x, y, 0), 0.5, 0.3 * n))
        for k in range(4):
            tilt_box(t, (KELP_L, KELP, KELP_L, DEEP)[k], (x + k * 13 - 20, y, -15), (x + k * 19 - 30, y - 4, 52 + (k * 37) % 40), 5, 20)
    for n, (x, y, z) in enumerate(((-110, -92, 50), (205, -92, 20))):
        g.cyl(CREAM, (x, y, z - 30), 9, 62, steps=5)
        w = sc.part(P.motion(bob=-7.0, rate=0.3, phase=0.4 * n))
        w.cyl(MAG, (x, y, z + 24), 5, 22, steps=6, top_r=19)


def tube_sponges(sc):
    """TALL, 2.5 m: a cluster of barrel sponges, yellow and lavender, each leaning to the current on its own foot."""
    g = sc.still()
    rock(g, 0, 10, 380, 230, 44, 641)
    spec = ((-95, 20, 36, 236, YEL), (-28, -32, 31, 180, LAV), (44, 26, 40, 214, LAV), (108, -16, 30, 150, YEL), (-152, -26, 25, 110, LAV),
            (162, 30, 23, 96, YEL), (22, -76, 21, 80, PINK))
    for n, (x, y, ra, h, col) in enumerate(spec):
        m = sc.part(sway(1.6 + (n % 3) * 0.7, (x, y, 0), 0.28, 0.17 * n))
        m.cyl(col, (x, y, -40), ra * 0.7, h * 0.55 + 40, steps=7, top_r=ra)
        m.cyl(col, (x, y, h * 0.55), ra, h * 0.45, steps=7, top_r=ra * 1.14)
        m.cyl(CREAM, (x, y, h), ra * 1.2, 8, steps=7)                               # the lip
        m.cyl(ROCK_D, (x, y, h + 4), ra * 0.84, 6, steps=7)                         # the dark mouth
    for n, (x, y) in enumerate(((-205, -40), (215, -30), (70, -100))):              # young ones, crusting the rock
        blob(g, (LAV, YEL, PINK)[n], (x, y, 8), 26, 22, 20)


# ------------------------------------------------------------------------------------------ the kelp forest
def _palm(sc, x, y, s, seed, phase):
    r = random.Random(seed)
    at = (x, y, 0)
    th = max(6.0, 4.6 / s)                      # a blade's thickness: never under 4 cm as built
    g = sc.still(at, 0.0, s)
    rock(g, 10, 0, 250, 200, 48, seed + 1)
    for k in range(5):
        a = math.radians(k * 72 + 10)
        tilt_box(g, KELP_D, (math.cos(a) * 78, math.sin(a) * 78, 6), (math.cos(a) * 14, math.sin(a) * 14, 74), 16, 16)
    pts = ((0, 0, -150), (0, 0, 60), (22, 0, 230), (52, 0, 410), (70, 0, 590), (64, 0, 760))
    rad = (30, 27, 24, 21, 18)
    for i in range(5):
        m = g if i == 0 else sc.part(sway(0.6 * i, at, 0.22, phase + 0.04 * i), at, 0.0, s)
        rod(m, KELP_D if i % 2 == 0 else KELP, pts[i], pts[i + 1], rad[i], steps=6)
    top = pts[5]
    c = sc.part(sway(3.0, at, 0.22, phase + 0.2), at, 0.0, s)
    blob(c, KELP_D, add(top, 0, 0, 10), 44, 44, 36)
    for k in range(7):
        a = math.radians(k * 51.4 + 8)
        blob(c, CREAM if k % 2 else YEL, add(top, math.cos(a) * 54, math.sin(a) * 54, 26 + (k % 2) * 24), 24, 24, 31)
    for k in range(9):
        a = math.radians(k * 40 + r.uniform(-8, 8))
        ca, sa = math.cos(a), math.sin(a)
        p0 = add(top, ca * 30, sa * 30, 20)
        p1 = add(p0, ca * 150, sa * 150, r.uniform(40, 70))
        p2 = add(p1, ca * 110, sa * 110, -r.uniform(150, 200))
        col = KELP if k % 2 else KELP_L
        tilt_box(c, col, p0, p1, 46, th)
        tilt_box(c, col, p1, p2, 36, th)
    for k in range(3):
        a = math.radians(k * 120 + 40)
        tilt_box(c, KELP_L, add(top, math.cos(a) * 16, math.sin(a) * 16, 30), add(top, math.cos(a) * 44, math.sin(a) * 44, 140), 26, th)


def sea_palm(sc):
    """TALL, 9 m: one thick stalk on a clawed holdfast, a crown of pale float bladders and long blades that droop;
    a young one a third its height beside it."""
    _palm(sc, -40, 20, 1.0, 65, 0.0)
    _palm(sc, 250, -110, 0.42, 165, 0.4)


def ribbon_hedge(sc):
    """LOW, 1.6 m: a hedge of ribbon weed 6.4 m long, four clumps of blades broad to the road, seed beads on some."""
    r = random.Random(66)
    g = sc.still()
    blob(g, SAND_D, (0, 0, -18), 340, 74, 28, steps=(4, 8))
    cols = (KELP, KELP_D, KELP_L, KELP, DEEP)
    for c in range(4):
        cx = -228 + c * 152
        lo = sc.part(sway(3.0, (cx, 0, 0), 0.4, 0.21 * c))
        hi = sc.part(sway(6.0, (cx, 0, 0), 0.4, 0.21 * c + 0.05))
        for k in range(5):
            x, y, h, lean = cx - 62 + k * 31 + r.uniform(-6, 6), r.uniform(-22, 22), r.uniform(104, 146), r.uniform(-16, 16)
            col = cols[(c + k) % 5]
            mid, tip = (x + lean * 0.4, y, h * 0.55), (x + lean * 1.6, y, h)
            tilt_box(lo, col, (x, y, -20), mid, 5, 30)
            tilt_box(hi, col, mid, tip, 5, 23)
            if k % 2 == 0:
                blob(hi, PINK if c % 2 else CREAM, add(tip, 0, 0, 2), 9, 9, 10, steps=(4, 5))
    for n, (x, y) in enumerate(((-300, -50), (-40, -58), (210, -52))):
        blob(g, WHITE, (x, y, 2), 14, 11, 8, steps=(4, 5))                           # shells


def sea_lettuce(sc):
    """LOW, 0.8 m: a bed of sea lettuce, nine ruffled heads, pale green with paler hearts, each nodding."""
    r = random.Random(67)
    g = sc.still()
    blob(g, SAND_D, (0, 0, -20), 310, 150, 30, steps=(4, 8))
    for n in range(9):
        x, y, s = -244 + n * 61 + r.uniform(-12, 12), (n % 3 - 1) * 64 + r.uniform(-14, 14), r.uniform(0.8, 1.22)
        m = sc.part(sway(5.0, (x, y, 0), 0.5, (n % 3) * 0.3 + 0.1 * (n // 3)))
        m.cyl(KELP_L, (x, y, -8), 16 * s, 58 * s + 8, steps=7, top_r=60 * s)
        m.cyl(PALE, (x, y, 16 * s), 12 * s, 44 * s, steps=6, top_r=38 * s)
        for k in range(3):
            a = math.radians(k * 120 + n * 37)
            tilt_box(m, KELP if k else KELP_L, (x + math.cos(a) * 20 * s, y + math.sin(a) * 20 * s, 8),
                     (x + math.cos(a) * 78 * s, y + math.sin(a) * 78 * s, 40 * s), 36 * s, 5)


# ------------------------------------------------------------------------------------------ the research station
def sea_pens(sc):
    """TALL, 2.5 m: a planted row of five sea pens, quills with barbs to both sides, a blank tag on a stake by each."""
    g = sc.still()
    blob(g, SAND_D, (0, 0, -20), 340, 84, 30, steps=(4, 8))
    cols = ((PINK, MAG), (WHITE, LAV), (LAV, VIOLET), (PINK, MAG), (WHITE, LAV))
    for n in range(5):
        x, y, h = -240 + n * 120, (n % 2) * 24 - 12, (210, 250, 190, 240, 205)[n]
        quill, barb = cols[n]
        m = sc.part(sway(4.5, (x, y, 0), 0.3, 0.19 * n))
        m.cyl(CREAM, (x, y, -120), 11, 120 + h * 0.3, steps=5, top_r=8)
        blob(m, quill, (x, y, h * 0.17), 16, 16, 27)
        rod(m, CREAM, (x, y, h * 0.3), (x, y, h), 6, steps=5)
        for k in range(6):
            z, L = h * (0.34 + 0.1 * k), h * 0.3 * (1.0 - 0.12 * abs(k - 2.2))
            for sd in (-1, 1):
                tilt_box(m, barb if k % 2 else quill, (x, y, z), (x + sd * L, y, z + L * 0.55), 5, 20)
        g.box(WOOD_D, (x + 40, y - 36, 14), (5, 5, 62))
        g.box(WHITE, (x + 40, y - 39, 40), (26, 5, 18))                              # the tag: blank


def _spiral(sc, x0, y0, h, r0, turns, col, tip, nseg, thick, phase):
    """A whip coral wound like a corkscrew, opening as it climbs; four lengths of it, the higher the more they rock."""
    g = sc.still()
    prev = (x0, y0, -120)
    for i in range(nseg + 1):
        t = i / float(nseg)
        a = 6.2832 * turns * t + phase * 6.2832
        ra = r0 * (0.45 + 0.55 * t)
        p = (x0 + ra * math.cos(a), y0 + ra * math.sin(a), h * t)
        grp = min(3, int(t * 4 - 0.001)) if i else 0
        m = g if grp == 0 else sc.part(sway(0.9 * grp, (x0, y0, 0), 0.26, phase + 0.04 * grp))
        rod(m, col, prev, p, thick * (1.0 - 0.35 * t), steps=5)
        prev = p
    m = sc.part(sway(2.7, (x0, y0, 0), 0.26, phase + 0.12))
    blob(m, tip, prev, thick * 1.5, thick * 1.5, thick * 1.5, steps=(4, 5))


def spiral_whip(sc):
    """TALL, 5 m: three spiral whip corals on a rock, coral red, yellow and pink, white beads at their tips."""
    g = sc.still()
    rock(g, 0, 0, 330, 230, 50, 691)
    _spiral(sc, -40, 10, 490, 62, 3.2, CORAL, WHITE, 16, 12, 0.0)
    _spiral(sc, 100, -30, 330, 44, 2.4, YEL, WHITE, 12, 10, 0.35)
    _spiral(sc, -150, -44, 172, 27, 1.6, PINK, CREAM, 8, 8, 0.7)
    blob(g, LAV, (150, 40, 12), 30, 24, 20)
    blob(g, MAG, (-90, 70, 10), 24, 20, 16)


# ------------------------------------------------------------------------------------------ the salvage field, the vents
def wreck_rib(sc):
    """TALL, 6 m: five frames of a lost ship still standing on her keel, two planks holding them, weed hanging from
    every timber. Blunt, broken tops, joined along their length: a hull's side, nothing else."""
    r = random.Random(70)
    g = sc.still()
    g.box(HULL_D, (0, 44, 2), (780, 64, 60), rot=(1.5, 0, 0))
    hang = []
    for i, x in enumerate((-300, -150, 0, 150, 300)):
        hgt = (470, 560, 600, 540, 420)[i]
        pts = ((x, 44, -60), (x, -40, hgt * 0.22), (x, -110, hgt * 0.5), (x, -140, hgt * 0.78), (x, -122, hgt))
        for k in range(4):
            tilt_box(g, HULL if (i + k) % 2 else WOOD_D, pts[k], pts[k + 1], 26, 30)
        hang.append((x + 4, -126, hgt - 14))
        hang.append((x - 6, -118, hgt * 0.5 - 16))
    g.box(HULL, (0, -128, 272), (660, 14, 30))                                       # the stringers
    g.box(WOOD_D, (20, -152, 440), (350, 14, 26))
    g.box(HULL, (-60, -58, 70), (470, 12, 76), rot=(0, 0, 0))                        # what is left of her planking
    g.box(HULL_D, (150, -60, 150), (240, 12, 60))
    hang += [(-220, -134, 258), (80, -134, 258), (230, -134, 258), (-60, -158, 428)]
    for n, hp in enumerate(hang):
        L = r.uniform(90, 170) if hp[2] > 200 else r.uniform(60, 110)
        col = (KELP, KELP_D, KELP_L)[n % 3]
        w = sc.part(sway(10.0, hp, 0.35, (n % 5) * 0.2))
        tilt_box(w, col, hp, add(hp, 4, -3, -L * 0.55), 5, 26)
        tilt_box(w, col, add(hp, 4, -3, -L * 0.55), add(hp, -5, -5, -L), 5, 19)
    for n, (x, y, z) in enumerate(((-150, -128, 300), (0, -138, 480), (150, -58, 120), (-300, -30, 110), (300, -132, 330), (-20, -66, 80))):
        blob(g, CREAM if n % 2 else WHITE, (x, y, z), 15, 9, 13, steps=(4, 5))       # barnacles


def tube_worms(sc):
    """TALL, 3.2 m: a stack of white tubes on dark vent rock, red plumes that draw in and come out again."""
    r = random.Random(71)
    g = sc.still()
    rock(g, 0, 0, 300, 230, 70, 711, ROCK_D, SOOT)
    g.cyl(SOOT, (112, 40, 0), 46, 92, steps=6, top_r=24)                              # a small chimney
    for k in range(10):
        a, d = k * 2.4, 12 + (k * 37) % 88
        x, y = math.cos(a) * d - 22, math.sin(a) * d * 0.7
        h = 292 - d * 1.5 + r.uniform(-18, 18)
        top = (x + math.cos(a) * h * 0.12, y + math.sin(a) * h * 0.12, h)
        rod(g, WHITE if k % 3 else STATION, (x, y, -40), top, 17, steps=6)
        pl = sc.part(P.motion(bob=-16.0, rate=0.2, phase=(k % 4) * 0.25))
        pl.cyl(RED, add(top, 0, 0, -22), 7, 46, steps=6, top_r=27)
        blob(pl, PINK if k % 2 else RED, add(top, 0, 0, 27), 25, 25, 9, steps=(4, 6))


# ------------------------------------------------------------------------------------------ Nereida
def pearl_vine(sc):
    """TALL, 4.3 m: a trellis of marble posts and gold lattice, two vines climbing it, strings of pale pearl bladders
    swinging from its rails."""
    g = sc.still()
    W, H = 420.0, 380.0
    for sx in (-1, 1):
        g.box(MARBLE, (sx * W / 2, 0, H / 2 - 60), (22, 22, H + 120))
        g.box(ROCK_D, (sx * W / 2, 0, 8), (42, 42, 16))
        blob(g, PINK, (sx * W / 2, 0, H + 16), 17, 17, 17)
    for z in (120, 250, 372):
        g.box(GOLD, (0, 0, z), (W, 9, 9))
    for a, b in (((-W / 2, 0, 120), (0, 0, 372)), ((0, 0, 120), (W / 2, 0, 372)), ((W / 2, 0, 120), (0, 0, 372)), ((0, 0, 120), (-W / 2, 0, 372))):
        tilt_box(g, GOLD, a, b, 7, 7)
    tilt_box(g, MARBLE, (-W / 2, 0, H), (0, 0, H + 44), 14, 14)
    tilt_box(g, MARBLE, (0, 0, H + 44), (W / 2, 0, H), 14, 14)
    vines = (((-170, -12, -30), (-150, -12, 110), (-60, -12, 190), (-92, -12, 300), (20, -12, 372), (120, -12, 334), (160, -12, 404)),
             ((182, -12, -30), (150, -12, 130), (60, -12, 240), (112, -12, 330)))
    n = 0
    for v in vines:
        for i in range(len(v) - 1):
            rod(g, KELP_D, v[i], v[i + 1], 7, steps=5)
            p = v[i + 1]
            lv = sc.part(sway(12.0, p, 0.6, 0.17 * n))
            for sd in (-1, 1):
                tilt_box(lv, KELP if sd > 0 else KELP_L, p, add(p, sd * 36, -8, 18), 20, 5)
            n += 1
    for n, (x, z, L) in enumerate(((-150, 372, 70), (-60, 250, 60), (50, 372, 96), (150, 250, 54), (-120, 120, 44), (90, 120, 50), (0, 250, 40))):
        hp = (x, -10, z - 4)
        t = sc.part(sway(9.0, hp, 0.4, 0.23 * n))
        t.box(KELP_D, add(hp, 0, 0, -L / 2.0), (5, 5, L))
        for k in range(3):
            blob(t, CREAM if (n + k) % 2 else PINK, add(hp, (k - 1) * 9, -4, -L * (0.4 + 0.3 * k)), 15 + k * 2, 15 + k * 2, 16 + k * 2)


def garden_urn(sc):
    """TALL, 2.2 m: a rose-stone urn on a marble pedestal, fronds standing in it, weed trailing over its rim."""
    r = random.Random(73)
    g = sc.still()
    g.box(ROCK_D, (0, 0, -40), (124, 124, 120))
    g.box(MARBLE, (0, 0, 55), (96, 96, 70))
    g.box(ROCK_L, (0, 0, 95), (114, 114, 12))
    for sx in (-1, 1):
        for sy in (-1, 1):
            blob(g, CREAM, (sx * 48, sy * 48, 106), 9, 9, 9, steps=(4, 5))
        g.box(GOLD, (sx * 74, 0, 150), (12, 10, 34))
    g.cyl(ROCK_L, (0, 0, 101), 30, 16, steps=8)
    g.cyl(ROCK_L, (0, 0, 117), 22, 14, steps=8, top_r=58)
    g.cyl(MARBLE, (0, 0, 131), 58, 40, steps=8, top_r=66)
    g.cyl(GOLD, (0, 0, 148), 68, 7, steps=8)
    g.cyl(ROCK_L, (0, 0, 171), 72, 9, steps=8)
    g.cyl(KELP_D, (0, 0, 176), 60, 6, steps=8)
    f = sc.part(sway(6.0, (0, 0, 178), 0.4, 0.0))
    for k in range(6):
        a = math.radians(k * 60 + 15)
        tip = (math.cos(a) * 46, math.sin(a) * 46, 180 + r.uniform(28, 38))
        tilt_box(f, KELP_L if k % 2 else KELP, (math.cos(a) * 20, math.sin(a) * 20, 176), tip, 20, 5)
        if k % 2 == 0:
            blob(f, PINK if k % 4 else CREAM, tip, 10, 10, 10, steps=(4, 5))
    for k in range(6):
        a = math.radians(k * 60 - 12)
        ca, sa = math.cos(a), math.sin(a)
        hp = (ca * 68, sa * 68, 180)
        t = sc.part(sway(7.0, hp, 0.35, 0.16 * k))
        e = (ca * 86, sa * 86, 148)
        tilt_box(t, KELP, hp, e, 20, 5)
        tilt_box(t, KELP_L if k % 2 else KELP, e, (ca * 90, sa * 90, r.uniform(50, 96)), 16, 5)


def sea_lily(sc):
    """TALL, 8.3 m: a sea lily like a tree: a jointed stem of cream and rose, a crown of ten feathered arms that opens
    and closes all together."""
    g = sc.still()
    rock(g, 0, 10, 250, 200, 50, 741)
    for k in range(6):                                                              # the roots that grip
        a = math.radians(k * 60 + 20)
        tilt_box(g, CREAM, (math.cos(a) * 12, math.sin(a) * 12, 62), (math.cos(a) * 84, math.sin(a) * 84, 10), 10, 10)
    pts = ((0, 0, -150), (0, 0, 70), (10, 0, 160), (24, 0, 250), (34, 0, 340), (36, 0, 430), (28, 0, 510), (16, 0, 580))
    for i in range(7):
        rod(g, CREAM if i % 2 == 0 else ROCK_L, pts[i], pts[i + 1], 15 - i * 0.6, steps=6)
        p = pts[i + 1]
        g.cyl(PINK, add(p, 0, 0, -5), 20 - i * 0.6, 10, steps=6)
        if i in (2, 4):
            for k in range(4):
                a = math.radians(k * 90 + i * 20)
                tilt_box(g, CREAM, p, add(p, math.cos(a) * 44, math.sin(a) * 44, 18), 8, 8)
    c = pts[7]
    g.cyl(MAG, c, 16, 46, steps=7, top_r=52)
    piv = add(c, 0, 0, 40)
    for k in range(10):
        ad = k * 36.0
        a = math.radians(ad)
        ca, sa = math.cos(a), math.sin(a)
        m = sc.part(P.motion(swing=13.0, pivot=piv, axis=ad + 90.0, rate=0.16, phase=0.0))
        b = add(c, ca * 44, sa * 44, 44)
        mid = add(b, ca * 120, sa * 120, 90)
        tip = add(mid, ca * 40, sa * 40, 110)
        tilt_box(m, CREAM, b, mid, 13, 11)
        tilt_box(m, CREAM, mid, tip, 10, 9)
        for q, (f0, pa, pb, L) in enumerate(((0.45, b, mid, 66), (0.9, b, mid, 60), (0.4, mid, tip, 48), (0.85, mid, tip, 36))):
            p = (pa[0] + (pb[0] - pa[0]) * f0, pa[1] + (pb[1] - pa[1]) * f0, pa[2] + (pb[2] - pa[2]) * f0)
            m.box(MAG if (k + q) % 2 else PINK, p, (L, 7, 9), rot=(0, ad + 90.0, 0))


def lantern_weed(sc):
    """TALL, 4 m: three stalks of lantern weed, bulbs of warm white and pink hanging from them with a steady soft glow (the table block
    keeps it 24 m and more from the road)."""
    g = sc.still()
    blob(g, SAND_D, (20, 0, -20), 230, 130, 30, steps=(4, 8))
    rock(g, -150, 30, 130, 100, 30, 751, ROCK_L, ROCK)

    def bulb(m, at, col, glow):
        rod(m, KELP_D, at, add(at, 0, 0, -24), 4, steps=4)
        m.cyl(KELP_D, add(at, 0, 0, -30), 13, 8, steps=6)
        blob(m, col, add(at, 0, 0, -52), 21, 21, 27, glow=glow)

    for n, (x, y, h, sd) in enumerate(((-70, 10, 400, 1), (44, -20, 330, -1), (124, 30, 250, 1))):
        p0, p1, p2, p3 = (x, y, -120), (x + sd * 10, y, h * 0.4), (x + sd * 34, y, h * 0.75), (x + sd * 70, y, h)
        p4 = add(p3, sd * 42, 0, -26)
        rod(g, KELP_D, p0, p1, 9, steps=5)
        mid = sc.part(sway(2.5, (x, y, 0), 0.3, 0.2 * n))
        up = sc.part(sway(4.5, (x, y, 0), 0.3, 0.2 * n + 0.04))
        rod(mid, KELP_D, p1, p2, 8, steps=5)
        rod(up, KELP, p2, p3, 6, steps=5)
        rod(up, KELP, p3, p4, 5, steps=5)
        bulb(up, p4, WHITE, 0.5)
        bulb(up, add(p2, -sd * 34, -6, 30), PINK if n % 2 else WHITE, 0.45)
        tilt_box(up, KELP, p2, add(p2, -sd * 34, -6, 30), 6, 6)
        bulb(mid, add(p1, -sd * 38, 4, 34), PINK, 0.45)
        tilt_box(mid, KELP_D, p1, add(p1, -sd * 38, 4, 34), 7, 7)
        if h > 300:
            bulb(mid, add(p1, sd * 44, -6, 70), WHITE, 0.5)
            tilt_box(mid, KELP_D, add(p1, sd * 8, 0, 40), add(p1, sd * 44, -6, 70), 7, 7)
        tilt_box(g, KELP_L, (x - sd * 6, y - 10, -10), (x - sd * 44, y - 20, 62), 5, 22)   # two leaves at the foot
        tilt_box(g, KELP, (x + sd * 8, y + 8, -10), (x + sd * 40, y + 14, 46), 5, 20)


def star_moss(sc):
    """LOW, 0.45 m: cushions of star moss, small star flowers trembling on them, white, pink, yellow and lavender."""
    r = random.Random(76)
    g = sc.still()
    cush = ((-200, 10, 96, 70, 30), (-78, -32, 80, 62, 26), (28, 26, 112, 78, 34), (152, -12, 86, 64, 28), (240, 32, 62, 50, 22))
    for n, (x, y, rx, ry, rz) in enumerate(cush):
        blob(g, KELP if n % 2 == 0 else DEEP, (x, y, 0), rx, ry, rz, steps=(4, 7))
        fl = sc.part(sway(7.0, (x, y, rz), 0.8, 0.23 * n))
        for k in range(3 + n % 2):
            a, d = r.uniform(0, 6.2832), r.uniform(0.1, 0.62)
            px, py = x + math.cos(a) * d * rx, y + math.sin(a) * d * ry
            pz = rz * math.sqrt(max(0.05, 1.0 - d * d)) + 3
            col = (WHITE, PINK, YEL, LAV)[(n + k) % 4]
            yw = r.uniform(0, 90)
            fl.box(col, (px, py, pz), (23, 7, 5), rot=(0, yw, 0))
            fl.box(col, (px, py, pz), (7, 23, 5), rot=(0, yw, 0))
    blob(g, SAND_D, (20, 0, -24), 300, 110, 30, steps=(4, 8))


JOBS = [
    ('SM_S1PLANTS_SeaFans', sea_fans, TALL), ('SM_S1PLANTS_TableCoral', table_coral, TALL), ('SM_S1PLANTS_BrainCoral', brain_coral, LOW),
    ('SM_S1PLANTS_TubeSponges', tube_sponges, TALL), ('SM_S1PLANTS_SeaPalm', sea_palm, TALL), ('SM_S1PLANTS_RibbonHedge', ribbon_hedge, LOW),
    ('SM_S1PLANTS_SeaLettuce', sea_lettuce, LOW), ('SM_S1PLANTS_SeaPens', sea_pens, TALL), ('SM_S1PLANTS_SpiralWhip', spiral_whip, TALL),
    ('SM_S1PLANTS_WreckRib', wreck_rib, TALL), ('SM_S1PLANTS_TubeWorms', tube_worms, TALL), ('SM_S1PLANTS_PearlVine', pearl_vine, TALL),
    ('SM_S1PLANTS_GardenUrn', garden_urn, TALL), ('SM_S1PLANTS_SeaLily', sea_lily, TALL), ('SM_S1PLANTS_LanternWeed', lantern_weed, TALL),
    ('SM_S1PLANTS_StarMoss', star_moss, LOW),
]


def run(only=None):
    P.build_core()                              # loads the two masters; never build_core(rebuild=True)
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
