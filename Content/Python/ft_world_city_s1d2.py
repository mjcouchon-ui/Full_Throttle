"""City Center, Stage 1, builder s1d2: district 2 "Theatre Row" (1.10 to 2.10 km). /Game/World/CityCenter/SM_S1D2_*.
Run: import ft_world_city_s1d2 as K; K.run()

Playhouses and music halls in magenta, plum, lilac, warm white, blush and silver. The avenue is a canyon (podiums 25.5 m
out, a tower behind each, a 34 m stage flat in every gap), so:
  BUILDINGS (6)  narrow and TALL, made for the 13 m gap between two towers, one behind the other (58, 76 and 98 m as
                 built; the table stands them 43.5 to 68 m out, the taller the further): what shows from the street is
                 the crown over the flat, so every one carries its idea on top. 3 m of footing each.
                   PuppetHouse   a striped booth tower, two puppets nodding in its stage, a peaked roof
                   RehearsalLoft a blush block of lit studio windows, a band shell and two notes on the roof
                   CostumeHouse  a lilac front with dress forms in its windows; a spool, a needle, a button, a thimble
                   DanceDrum     a lit glass drum, a lilac ribbon spiralling round it, a dancer for a weather vane
                   MusicHall     a sunburst gable, an arcade of lit arches and a magenta onion dome
                   FlyTower      the blank silver box behind a stage: its backdrop door half open, scenery leaning out
  TALL PROPS (3) masts that stand in the gap before the buildings (37.6 m out), 50 to 56 m: MaskMast (a laughing mask
                 and a yawning one, our own drawing), Chandelier (a chandelier swaying under a silver gantry),
                 TicketMast (a roll of blank tickets unreeling down a mast).
  ROOF PROP (1)  MoonFlats: scenery flats (a moon, stars, a cloud) stored on a picture house's roof.
  LOW PROPS (5)  for the pavement behind the parked cars (20.2 to 22.6 m out), under 2 m, unlit: VelvetBenches,
                 StageTrunks (with a wardrobe rail), SpotPlanters, HatBoxKiosk, PosterDrums (pictures, no words).
  HERO (1)       GrandCurtain: a free-standing proscenium arch 100 m tall, a sunburst fan across the top, a magenta
                 curtain in three swags and two drapes that sway slowly, parted on a lit backdrop. Saved far=True (it
                 stands 172 m out: M_People stops moving past 180 m).
Nothing here blinks; glow is steady and only on pieces stood 24 m and more out. No words anywhere.
"""
import math
import ft_art
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/CityCenter'
MAG, PLUM, LILAC, WARM, BLUSH, SILVER = '#ff4fb8', '#4a3a78', '#c391d8', '#fff1c0', '#f3c4dc', '#c9c4d6'
DEEP, PINE = '#2b2350', '#2f6b52'                # the kit's navy (only the dark inside an opening) and its leaf green
G_BULB, G_WIN, G_SOFT, G_CLOTH = 1.5, 0.6, 0.3, 0.08
STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))
SWAY = dict(grow=(20, 20, 0), grow_down=(20, 20, 0))


# ------------------------------------------------------------------------------------------ helpers
def ccw(pts):
    a = 0.0
    for i in range(len(pts)):
        x0, z0 = pts[i]
        x1, z1 = pts[(i + 1) % len(pts)]
        a += x0 * z1 - x1 * z0
    return list(pts) if a > 0 else list(reversed(pts))


def poly(g, col, pts, y0, y1, glow=0.0):
    """A flat shape facing the road: (x, z) points, from y0 to y1."""
    out = []
    for p in pts:
        if not out or abs(p[0] - out[-1][0]) + abs(p[1] - out[-1][1]) > 1.0:
            out.append(p)
    if len(out) > 2 and abs(out[0][0] - out[-1][0]) + abs(out[0][1] - out[-1][1]) <= 1.0:
        out.pop()
    if len(out) >= 3:
        g.side(col, ccw(out), y0, y1, glow=glow)


def arc(g, col, cx, cz, r0, r1, a0, a1, n, y0, y1, glow=0.0, sx=1.0, sz=1.0):
    """A ring's sector facing the road, as n flat pieces. Angles in degrees from +X, counter-clockwise seen from the
    road; r0 / r1 (inner, outer) may be functions of t = 0..1 along the sector (a crescent, a smile)."""
    fi = r0 if callable(r0) else (lambda t: r0)
    fo = r1 if callable(r1) else (lambda t: r1)
    for k in range(n):
        ta, tb = k / float(n), (k + 1) / float(n)
        A, B = math.radians(a0 + (a1 - a0) * ta), math.radians(a0 + (a1 - a0) * tb)
        pts = [(cx + sx * fi(ta) * math.cos(A), cz + sz * fi(ta) * math.sin(A)), (cx + sx * fo(ta) * math.cos(A), cz + sz * fo(ta) * math.sin(A)),
               (cx + sx * fo(tb) * math.cos(B), cz + sz * fo(tb) * math.sin(B)), (cx + sx * fi(tb) * math.cos(B), cz + sz * fi(tb) * math.sin(B))]
        poly(g, col, pts, y0, y1, glow)


def halfdisc(g, col, cx, cz, rx, rz, y0, y1, n=10, glow=0.0):
    poly(g, col, [(cx + rx * math.cos(math.pi * k / n), cz + rz * math.sin(math.pi * k / n)) for k in range(n + 1)], y0, y1, glow)


def disc(g, col, x, y, z, r, t=10.0, steps=10, glow=0.0):
    """A disc facing the road, from y to y + t."""
    g.cyl(col, (x, y + t, z), r, t, rot=(0, 0, 90), steps=steps, glow=glow)


def star(g, col, x, y, z, r, t=12.0, glow=0.0):
    """A six-pointed star facing the road (two triangles), from y to y + t."""
    for a0 in (90.0, 270.0):
        poly(g, col, [(x + r * math.cos(math.radians(a0 + 120.0 * k)), z + r * math.sin(math.radians(a0 + 120.0 * k))) for k in range(3)], y, y + t, glow)


def swag(g, col, x0, x1, ztop, drop, y0, y1, n=6, glow=0.0):
    """A hanging scallop of cloth: straight along the top, a curve below."""
    pts = [(x0, ztop)] + [(x0 + (x1 - x0) * k / float(n), ztop - drop * math.sin(math.pi * k / float(n))) for k in range(1, n)] + [(x1, ztop)]
    poly(g, col, pts, y0, y1, glow)


def bulbs(g, a, b, n, s=36.0, col=WARM, glow=G_BULB):
    for k in range(n):
        t = (k + 0.5) / n
        g.box(col, (a[0] + (b[0] - a[0]) * t, a[1] + (b[1] - a[1]) * t, a[2] + (b[2] - a[2]) * t), (s, s, s), glow=glow)


def onion(g, col, x, y, z, r, tip=SILVER, steps=10, lat=6):
    """An onion dome standing on z: about 2.9 r tall with its finial."""
    g.cyl(col, (x, y, z), r * 0.62, r * 0.36, steps=steps, top_r=r * 0.92)
    g.sphere(col, (x, y, z + r * 0.95), r, scale=(1, 1, 0.9), steps=(lat, steps))
    g.cyl(col, (x, y, z + r * 1.5), r * 0.6, r * 0.95, steps=steps, top_r=r * 0.07)
    g.cyl(tip, (x, y, z + r * 2.4), max(r * 0.045, 5.0), r * 0.35, steps=5)
    g.sphere(tip, (x, y, z + r * 2.8), max(r * 0.12, 9.0), steps=(3, 6))
    return z + r * 2.92


def pennant(sc, x, y, z, h, L, col=MAG, pole=SILVER, ph=0.0, w=None, t=8.0):
    """A pole with a pennant that swings round it."""
    w = w or L * 0.36
    sc.still().cyl(pole, (x, y, z), max(t * 0.9, 6.0), h, steps=5)
    f = sc.part(P.motion(swing=16, pivot=(x, y, z + h), axis=P.UP, rate=0.22, phase=ph))
    f.box(col, (x + L / 2.0 + t, y, z + h - w * 0.6), (L, t, w))
    f.box(WARM, (x + L * 0.22 + t, y, z + h - w * 0.6), (L * 0.1, t + 2.0, w))


def shaft(g, w, d, h, wall, trim, bands=(), band_col=BLUSH, foot=300.0):
    """A building's body from its footing to h, a pilaster up each front corner, bands round it."""
    g.box(wall, (0, 0, (h - foot) / 2.0), (w, d, h + foot))
    for sx in (-1, 1):
        g.box(trim, (sx * (w / 2.0 - 36), -d / 2.0 - 10, h / 2.0), (72, 20, h))
    for z in bands:
        g.box(band_col, (0, 0, z), (w + 44, d + 44, 64))


def wins(g, w, d, z0, z1, cols, step, ww, wh, col=WARM, glow=G_WIN, seed=0):
    """Lit windows on the front: `cols` across, a row every `step`; one in five is dark (left out)."""
    n = int((z1 - z0) / step)
    for j in range(n):
        for k in range(cols):
            if (j * 7 + k * 3 + seed) % 5 == 0:
                continue
            g.box(col, (-w / 2.0 + (k + 0.5) * w / cols, -d / 2.0 - 4, z0 + (j + 0.5) * step), (ww, 10, wh), glow=glow)


# ------------------------------------------------------------------------------------------ buildings
def puppet_house(sc):
    """PuppetHouse: 11.2 x 8.4 m, 61 m. A striped booth all the way up; in the stage at its top two puppets nod."""
    g = sc.still()
    W, D, H = 980.0, 640.0, 3900.0
    n = 7
    sw = W / n
    for k in range(n):
        g.box(MAG if k % 2 == 0 else WARM, (-W / 2.0 + (k + 0.5) * sw, 0, (H - 300) / 2.0), (sw, D, H + 300))
    g.box(PLUM, (0, 0, 60), (W + 50, D + 50, 420))
    for z in (1300.0, 2600.0):                                           # two little balconies with a lit door
        g.box(DEEP, (0, -D / 2.0 - 4, z + 190), (300, 10, 380))
        g.box(WARM, (0, -D / 2.0 - 8, z + 170), (230, 6, 300), glow=G_WIN)
        g.box(BLUSH, (0, -D / 2.0 - 30, z - 20), (460, 60, 70))
        for k in range(4):
            g.box(LILAC, (-165 + k * 110, -D / 2.0 - 46, z + 50), (40, 24, 80))
        swag(g, PLUM, -230, 230, z + 470, 90, -D / 2.0 - 16, -D / 2.0)
    bulbs(g, (-W / 2.0 - 8, -D / 2.0 - 12, 700), (-W / 2.0 - 8, -D / 2.0 - 12, H), 9, s=40)
    bulbs(g, (W / 2.0 + 8, -D / 2.0 - 12, 700), (W / 2.0 + 8, -D / 2.0 - 12, H), 9, s=40)
    g.box(SILVER, (0, 0, H + 30), (W + 60, D + 50, 60))                  # the stage's sill
    g.box(LILAC, (0, 0, H + 560), (W + 20, D + 20, 1000))                # the stage box
    yf = -(D + 20) / 2.0
    g.box(DEEP, (0, yf - 5, H + 530), (740, 10, 800))
    g.box(WARM, (0, yf - 12, H + 450), (660, 6, 560), glow=G_SOFT)
    halfdisc(g, BLUSH, 0, H + 180, 250, 250, yf - 18, yf - 14, n=8, glow=G_SOFT)      # a painted sun on the backdrop
    for k in range(3):
        swag(g, MAG, -370 + k * 247, -370 + (k + 1) * 247, H + 930, 150, yf - 44, yf - 24)
    for sx in (-1, 1):
        g.box(MAG, (sx * 340, yf - 30, H + 530), (70, 24, 800))
        g.box(BLUSH, (sx * 340, yf - 44, H + 380), (90, 10, 50))
    bulbs(g, (-400, yf - 50, H + 95), (400, yf - 50, H + 95), 7, s=44)
    for sx, body, hat, ph, rt in ((-150, MAG, BLUSH, 0.0, 0.3), (160, PLUM, WARM, 0.45, 0.24)):
        p = sc.part(P.motion(swing=9, pivot=(sx, yf - 70, H + 120), axis=90.0, rate=rt, phase=ph))
        p.cyl(body, (sx, yf - 70, H + 120), 100, 300, steps=6, top_r=56)
        p.sphere(WARM if body == MAG else LILAC, (sx, yf - 70, H + 505), 100, steps=(4, 6))
        p.cyl(hat, (sx, yf - 70, H + 575), 84, 190, steps=5, top_r=8)
        p.sphere(SILVER, (sx, yf - 70, H + 775), 22, steps=(3, 5))
        for a in (-1, 1):
            tilt_box(p, body, (sx + a * 60, yf - 70, H + 350), (sx + a * 200, yf - 70, H + 440 + a * 40), 44, 44)
            p.box(WARM, (sx + a * 212, yf - 70, H + 448 + a * 40), (50, 50, 50))
    zr = H + 1060.0
    g.box(BLUSH, (0, 0, zr + 24), (W + 110, D + 70, 48))
    poly(g, MAG, [(-W / 2.0 - 70, zr + 48), (W / 2.0 + 70, zr + 48), (0, zr + 640)], -D / 2.0 - 30, D / 2.0 + 30)
    for k in range(5):                                                    # the roof's scalloped edge
        halfdisc(g, WARM, -420 + k * 210, zr + 2, 105, -80, -D / 2.0 - 40, -D / 2.0 - 30, n=5)
    disc(g, WARM, 0, -D / 2.0 - 40, zr + 250, 110, t=10, steps=10, glow=G_WIN)
    star(g, MAG, 0, -D / 2.0 - 48, zr + 250, 78, t=8)
    g.sphere(SILVER, (0, 0, zr + 660), 44, steps=(4, 6))
    pennant(sc, 0, 0, zr + 690, 430, 260, col=LILAC, t=12.0)


def rehearsal_loft(sc):
    """RehearsalLoft: 10.4 x 7 m, 60 m. Lit studio windows up a blush block; on its roof a band shell and two notes."""
    g = sc.still()
    W, D, H = 1040.0, 660.0, 5150.0
    shaft(g, W, D, H, BLUSH, LILAC, bands=(40.0, 1700.0, 3400.0, H - 20), band_col=SILVER)
    for j, z in enumerate((560.0, 1020.0, 2200.0, 2680.0, 3900.0, 4380.0)):      # tall studio windows, in pairs of floors
        for k in range(3):
            if (j + k) % 4 == 3:
                continue
            x = -310 + k * 310
            g.box(WARM, (x, -D / 2.0 - 4, z + 190), (250, 10, 380), glow=G_WIN)
            g.box(PLUM, (x, -D / 2.0 - 8, z + 190), (20, 8, 380))
    bulbs(g, (-W / 2.0 + 60, -D / 2.0 - 30, H + 20), (W / 2.0 - 60, -D / 2.0 - 30, H + 20), 9, s=42)
    zr = H + 12.0
    for sx in (-1, 1):                                                    # the roof's rail
        g.box(SILVER, (sx * (W / 2.0 - 10), 0, zr + 60), (14, D, 14))
        for k in range(3):
            g.box(SILVER, (sx * (W / 2.0 - 10), -D / 2.0 + 20 + k * (D - 40) / 2.0, zr + 30), (14, 14, 60))
    g.box(PLUM, (0, 40, zr + 30), (900, 520, 60))                         # the stage
    for k in range(4):                                                    # the shell: arches stepping back and in
        r = 440.0 - k * 62.0
        y = -230.0 + k * 115.0
        arc(g, LILAC if k % 2 == 0 else BLUSH, 0, zr + 60, r - 78, r, 0, 180, 8, y, y + 115)
    halfdisc(g, WARM, 0, zr + 60, 196, 196, 228, 240, n=8, glow=G_SOFT)
    bulbs(g, (-380, -240, zr + 80), (380, -240, zr + 80), 6, s=36)
    g.box(DEEP, (60, 60, zr + 110), (150, 110, 22))                       # a grand piano under it
    g.box(DEEP, (10, 60, zr + 90), (16, 16, 60))
    tilt_box(g, DEEP, (20, 110, zr + 122), (130, 110, zr + 200), 100, 10)
    for sx, ph in ((-1, 0.0), (1, 0.5)):                                  # two notes on springs at the corners
        x = sx * 455.0
        g.cyl(SILVER, (x, -250, zr), 8, 300, steps=5)
        p = sc.part(P.motion(swing=7, pivot=(x, -250, zr + 300), axis=90.0, rate=0.3, phase=ph))
        p.sphere(MAG, (x, -250, zr + 340), 60, scale=(1.25, 0.8, 0.85), steps=(4, 6))
        p.box(MAG, (x + 62, -250, zr + 480), (16, 16, 290))
        tilt_box(p, MAG, (x + 62, -250, zr + 615), (x + 150, -250, zr + 540), 16, 60)
    pennant(sc, 0, 250, zr + 60, 520, 230, col=MAG, t=10.0, ph=0.3)


def costume_house(sc):
    """CostumeHouse: 9 x 6 m, 76 m. Dress forms in its lit windows; on the roof a spool, a needle, a button, a thimble."""
    g = sc.still()
    W, D, H = 900.0, 580.0, 6300.0
    shaft(g, W, D, H, LILAC, PLUM, bands=(40.0, H - 30), band_col=PLUM)
    for z in (2050.0, 4150.0):                                            # a tape measure round it, twice
        g.box(WARM, (0, 0, z), (W + 30, D + 30, 90))
        for k in range(4):
            g.box(PLUM, (-W / 2.0 + 150 + k * 200.0, -D / 2.0 - 17, z - 10), (16, 6, 70))
    for j, z in enumerate((500.0, 1150.0, 2500.0, 3250.0, 4600.0, 5350.0)):
        for k in range(3):
            if (j * 2 + k) % 5 == 4:
                continue
            x = -270 + k * 270
            g.box(WARM, (x, -D / 2.0 - 4, z + 230), (190, 10, 460), glow=G_WIN)
            if j >= 4:
                halfdisc(g, WARM, x, z + 460, 95, 95, -D / 2.0 - 9, -D / 2.0, n=5, glow=G_WIN)
            if j >= 4:                                                    # a dress form in each top window
                c = (MAG, PLUM, BLUSH)[(j + k) % 3]
                g.cyl(c, (x, -D / 2.0 - 30, z + 90), 60, 150, steps=6, top_r=26)
                g.cyl(c, (x, -D / 2.0 - 30, z + 240), 26, 120, steps=6, top_r=44)
    for sx in (-1, 1):                                                    # a ribbon down each side of the front
        g.box(MAG, (sx * (W / 2.0 - 100), -D / 2.0 - 6, H / 2.0 + 300), (30, 8, H - 700))
    zr = H + 2.0
    g.box(PLUM, (0, 0, zr + 30), (W + 30, D + 30, 60))
    # the spool, standing; thread wound on it
    g.cyl(WARM, (-190, 20, zr + 60), 250, 46, steps=10)
    g.cyl(MAG, (-190, 20, zr + 106), 185, 400, steps=10)
    for k in range(2):
        g.cyl(BLUSH, (-190, 20, zr + 220 + k * 160), 189, 16, steps=10)
    g.cyl(WARM, (-190, 20, zr + 506), 250, 46, steps=10)
    g.cyl(SILVER, (-190, 20, zr + 552), 60, 30, steps=8)
    # the needle, leaning, the thread through its eye
    a, b = (250.0, 60.0, zr + 60.0), (330.0, -40.0, zr + 1250.0)
    rod(g, SILVER, a, b, 17, steps=6)
    g.cyl(SILVER, (b[0], b[1], b[2] - 6), 17, 80, steps=6, top_r=3)
    arc(g, SILVER, 262, zr + 250, 22, 44, 0, 360, 4, 40, 60)
    tilt_box(g, MAG, (-10, 20, zr + 500), (262, 50, zr + 250), 12, 12)
    f = sc.part(P.motion(swing=10, pivot=(262, 50, zr + 250), axis=0.0, rate=0.28, phase=0.2))
    tilt_box(f, MAG, (262, 50, zr + 250), (400, 50, zr + 120), 12, 12)
    tilt_box(f, MAG, (400, 50, zr + 120), (420, 50, zr - 60), 12, 12)
    # the button on the parapet, the thimble on the other corner
    disc(g, BLUSH, 60, -D / 2.0 - 40, zr + 250, 190, t=36, steps=12)
    arc(g, MAG, 60, zr + 250, 150, 190, 0, 360, 8, -D / 2.0 - 50, -D / 2.0 - 40)
    for sx in (-1, 1):
        for sz in (-1, 1):
            disc(g, PLUM, 60 + sx * 52, -D / 2.0 - 46, zr + 250 + sz * 52, 26, t=8, steps=6)
    g.cyl(SILVER, (330, -150, zr + 60), 100, 190, steps=8, top_r=74)
    g.cyl(SILVER, (330, -150, zr + 60), 112, 30, steps=8)
    g.sphere(SILVER, (330, -150, zr + 250), 74, scale=(1, 1, 0.5), steps=(4, 8))
    pennant(sc, -190, 20, zr + 580, 420, 240, col=LILAC, t=10.0, ph=0.6)


def dance_drum(sc):
    """DanceDrum: 7.9 m round, 76 m. A plum drum, its top third a lit glass studio with a lilac ribbon spiralling
    round it; a dancer turns to and fro on the dome."""
    g = sc.still()
    R, H0, H1 = 372.0, 4500.0, 6750.0
    g.cyl(PLUM, (0, 0, -300), R, H0 + 300, steps=12)
    for k, z in enumerate((30.0, 1450.0, 2950.0, H0 - 40)):
        g.cyl(BLUSH if k % 3 else SILVER, (0, 0, z), R + 20, 60, steps=12)
    for z in (760.0, 2200.0, 3700.0):                                    # lit ribbon windows right round
        g.cyl(WARM, (0, 0, z), R + 5, 260, steps=12, glow=G_WIN)
        g.cyl(PLUM, (0, 0, z + 120), R + 9, 22, steps=12)
    g.cyl(WARM, (0, 0, H0 + 20), R - 10, H1 - H0 - 20, steps=12, glow=0.4)
    for z in (H0 + 760.0, H0 + 1500.0):
        g.cyl(SILVER, (0, 0, z), R - 4, 20, steps=12)
    n, turns = 26, 2.2                                                    # the ribbon
    z0, z1, rr = H0 - 300.0, H1 + 80.0, R + 28.0
    for k in range(n):
        a0, a1 = math.radians(360.0 * turns * k / n - 100), math.radians(360.0 * turns * (k + 1) / n - 100)
        tilt_box(g, LILAC if k % 6 else MAG, (rr * math.cos(a0), rr * math.sin(a0), z0 + (z1 - z0) * k / n),
                 (rr * math.cos(a1), rr * math.sin(a1), z0 + (z1 - z0) * (k + 1) / n), 26, 150)
    g.cyl(SILVER, (0, 0, H1), R + 22, 60, steps=12)
    g.sphere(BLUSH, (0, 0, H1 + 60), R - 10, scale=(1, 1, 0.48), steps=(6, 12))
    for k in range(8):
        a = math.radians(45.0 * k + 22.5)
        g.box(WARM, ((R + 26) * math.cos(a), (R + 26) * math.sin(a), H1 + 30), (40, 40, 40), glow=G_BULB)
    zt = H1 + 60 + (R - 10) * 0.48
    g.cyl(SILVER, (0, 0, zt - 20), 10, 300, steps=5)
    d = sc.part(P.motion(swing=55, pivot=(0, 0, zt), axis=P.UP, rate=0.06, phase=0.1))
    z = zt + 280
    d.cyl(MAG, (0, 0, z), 120, 130, steps=8, top_r=30)                    # the dancer: a skirt, a body, a head, arms out
    d.cyl(WARM, (0, 0, z + 130), 30, 130, steps=6, top_r=40)
    d.sphere(WARM, (0, 0, z + 310), 46, steps=(4, 6))
    d.sphere(PLUM, (0, 14, z + 350), 26, steps=(3, 5))
    tilt_box(d, WARM, (-30, 0, z + 240), (-190, 0, z + 330), 22, 22)
    tilt_box(d, WARM, (30, 0, z + 240), (170, 0, z + 420), 22, 22)
    tilt_box(d, LILAC, (170, 0, z + 420), (330, 0, z + 300), 10, 50)      # the ribbon in her hand
    tilt_box(d, LILAC, (330, 0, z + 300), (420, 0, z + 380), 10, 50)


def music_hall(sc):
    """MusicHall: 10.7 x 8.6 m, 98 m. A plum tower; an arcade of lit arches, a sunburst gable, a magenta onion dome."""
    g = sc.still()
    W, D, H = 1020.0, 640.0, 7300.0
    shaft(g, W, D, H, PLUM, LILAC, bands=(40.0, 2450.0, 4900.0, H - 30))
    wins(g, W, D, 700.0, 7000.0, 3, 900.0, 150, 480, seed=1)
    for sx in (-1, 1):
        bulbs(g, (sx * (W / 2.0 - 36), -D / 2.0 - 30, 2600), (sx * (W / 2.0 - 36), -D / 2.0 - 30, H), 5, s=50)
    for k in range(3):
        swag(g, BLUSH, -450 + k * 300, -450 + (k + 1) * 300, H - 70, 170, -D / 2.0 - 22, -D / 2.0)
    zg = H + 2.0                                                          # the arcade
    g.box(LILAC, (0, 0, zg + 350), (W + 50, D + 50, 700))
    yf = -(D + 50) / 2.0
    for k in range(5):
        x = -380 + k * 190
        g.box(WARM, (x, yf - 4, zg + 270), (120, 10, 380), glow=G_WIN)
        halfdisc(g, WARM, x, zg + 460, 60, 60, yf - 9, yf, n=5, glow=G_WIN)
    g.box(BLUSH, (0, yf - 30, zg + 40), (W + 60, 60, 60))
    g.box(SILVER, (0, 0, zg + 724), (W + 90, D + 90, 48))
    zt = zg + 748.0                                                       # the sunburst gable and the drum
    halfdisc(g, LILAC, 0, zt, 470, 470, yf - 20, yf + 60, n=10)
    for k in range(7):
        a = math.radians(18.0 + k * 24.0)
        tilt_box(g, WARM, (130 * math.cos(a), yf - 30, zt + 130 * math.sin(a)), (430 * math.cos(a), yf - 30, zt + 430 * math.sin(a)), 14, 50, glow=0.9)
    halfdisc(g, MAG, 0, zt, 110, 110, yf - 40, yf - 20, n=6)
    g.cyl(LILAC, (0, 40, zt), 380, 470, steps=10)
    g.cyl(WARM, (0, 40, zt + 300), 386, 90, steps=10, glow=G_WIN)
    g.cyl(BLUSH, (0, 40, zt + 470), 410, 44, steps=10)
    onion(g, MAG, 0, 40, zt + 514, 415, steps=10)
    for sx in (-1, 1):                                                    # a turret at each front corner
        g.cyl(LILAC, (sx * 440, -250, zt), 70, 190, steps=6)
        onion(g, BLUSH, sx * 440, -250, zt + 190, 96, steps=6, lat=4)


def fly_tower(sc):
    """FlyTower: 10.6 x 11.4 m (the cathead overhangs the front), 98 m. The blank ribbed box behind a stage. High on its front the backdrop door is half
    open: a painted sunset on the door, scenery flats leaning out; a star hangs from the cathead and swings."""
    g = sc.still()
    W, D, H0, H = 1000.0, 640.0, 2300.0, 9000.0
    g.box(PLUM, (0, 0, (H0 - 300) / 2.0), (W + 20, D + 20, H0 + 300))
    wins(g, W, D, 500.0, 2100.0, 3, 800.0, 170, 440, seed=2)
    g.box(BLUSH, (0, 0, H0 + 30), (W + 60, D + 50, 60))
    g.box(SILVER, (0, 0, (H0 + H) / 2.0), (W, D, H - H0))
    for k in range(7):
        g.box(LILAC, (-W / 2.0 + 50 + k * 150, -D / 2.0 - 7, (H0 + 5700) / 2.0 + 60), (56, 14, 5700 - H0 - 120))
    g.box(LILAC, (0, 0, 5730), (W + 30, D + 30, 60))
    g.box(BLUSH, (0, 0, H - 30), (W + 60, D + 50, 60))
    for k in range(3):                                                    # louvres
        g.box(PLUM, (-300 + k * 300, -D / 2.0 - 5, H - 370), (190, 10, 220))
        for j in range(3):
            g.box(SILVER, (-300 + k * 300, -D / 2.0 - 12, H - 440 + j * 70), (200, 10, 20))
    yf = -D / 2.0
    zd = 6050.0                                                           # the door: 6.6 x 15.5 m, its right half shut
    g.box(DEEP, (-60, yf - 5, zd + 775), (680, 10, 1550))
    g.box(WARM, (-215, yf - 10, zd + 760), (330, 6, 1480), glow=G_SOFT)
    g.box(BLUSH, (130, yf - 20, zd + 775), (320, 20, 1550))
    g.box(LILAC, (130, yf - 32, zd + 190), (320, 6, 380))                 # the painted sunset: hills, a sun, bars of cloud
    halfdisc(g, WARM, 130, zd + 380, 130, 130, yf - 36, yf - 30, n=6, glow=G_SOFT)
    for j in range(3):
        g.box(MAG, (130 + (j - 1) * 40, yf - 32, zd + 760 + j * 230), (240 - j * 40, 6, 46))
    g.box(SILVER, (-60, yf - 30, zd + 1580), (760, 40, 44))                # the door's rail
    g.box(SILVER, (-60, yf - 90, zd - 24), (900, 180, 30))                 # the loading gallery
    for k in range(5):
        g.box(SILVER, (-490 + k * 215, yf - 172, zd + 40), (14, 14, 110))
    g.box(SILVER, (-60, yf - 172, zd + 96), (900, 14, 14))
    for sx in (-1, 1):
        tilt_box(g, SILVER, (-60 + sx * 380, yf, zd - 260), (-60 + sx * 380, yf - 170, zd - 30), 20, 20)
    # scenery leaning out of the dark half: a cloud, a moon
    for k, (dx, r) in enumerate(((-130, 100), (0, 140), (140, 96))):
        disc(g, LILAC, -250 + dx, yf - 70, zd + 330 + (40 if k == 1 else 0), r, t=14, steps=8)
    g.box(LILAC, (-250, yf - 63, zd + 262), (430, 14, 110))
    arc(g, WARM, -230, zd + 900, lambda t: 250.0 - 120.0 * math.sin(math.pi * t), 250.0, 70, 290, 8, yf - 110, yf - 90, glow=0.7)
    tilt_box(g, SILVER, (-300, yf - 60, zd + 10), (-380, yf - 96, zd + 800), 16, 16)
    for z in range(0, 5):                                                 # sandbags under the gallery
        g.sphere(BLUSH, (-400 + z * 170, yf - 120, zd - 110), 44, scale=(1, 1, 1.3), steps=(3, 5))
        g.box(SILVER, (-400 + z * 170, yf - 120, zd - 50), (8, 8, 50))
    xl = 420.0                                                            # the ladder
    for sx in (-1, 1):
        g.box(PLUM, (xl + sx * 34, yf - 16, (H0 + zd) / 2.0), (14, 14, zd - H0))
    for k in range(13):
        g.box(PLUM, (xl, yf - 16, H0 + 150 + k * (zd - H0 - 200) / 12.0), (70, 12, 14))
    zr = H                                                                # the roof: the cathead and what hangs from it
    for sx in (-1, 1):
        g.box(SILVER, (sx * 150 - 200, 0, zr + 200), (30, 30, 400))
    g.box(SILVER, (-200, -200, zr + 400), (340, D + 500, 40))
    g.cyl(SILVER, (-200, -D / 2.0 - 190, zr + 330), 46, 30, rot=(0, 0, 90), steps=8)
    h = sc.part(P.motion(swing=5, pivot=(-200, -D / 2.0 - 205, zr + 330), axis=90.0, rate=0.14, phase=0.3))
    h.box(SILVER, (-200, -D / 2.0 - 205, zr + 80), (12, 12, 500))
    star(h, WARM, -200, -D / 2.0 - 212, zr - 300, 170, t=14, glow=0.8)
    star(h, MAG, -200, -D / 2.0 - 218, zr - 300, 80, t=8)
    g.box(LILAC, (260, 60, zr + 110), (260, 300, 220))                    # the smoke lantern
    poly(g, BLUSH, [(110, zr + 220), (410, zr + 220), (260, zr + 380)], -100, 220)
    g.box(LILAC, (-30, 180, zr + 60), (200, 160, 120))
    pennant(sc, 380, -200, zr, 760, 300, col=MAG, t=12.0, ph=0.7)


# ------------------------------------------------------------------------------------------ tall props (masts)
def mask_mast(sc):
    """MaskMast: 11.7 x 2.3 m, 55 m. Two theatre masks on a silver mast, one laughing, one yawning (our own
    drawing), a ring of steady bulbs round each, ribbons swaying under them."""
    g = sc.still()
    g.box(PLUM, (0, 60, 50), (240, 200, 700))
    g.box(BLUSH, (0, 60, 420), (270, 230, 50))
    g.cyl(SILVER, (0, 60, 400), 36, 4700, steps=6)
    for z in (1500.0, 3000.0, 4300.0):
        g.cyl(BLUSH, (0, 60, z), 50, 60, steps=6)
    zc = 4820.0
    g.box(SILVER, (0, 30, zc + 60), (720, 60, 40))
    g.cyl(SILVER, (0, 60, 5100), 22, 330, steps=5)
    g.sphere(MAG, (0, 60, 5460), 60, steps=(4, 6))
    shape = [(-250, 300), (-232, -10), (-150, -250), (0, -340), (150, -250), (232, -10), (250, 300), (0, 345)]
    for sx, face in ((-1, WARM), (1, LILAC)):
        cx = sx * 275.0
        poly(g, face, [(cx + px, zc + pz) for px, pz in shape], -40, 10)
        for k in range(12):                                               # bulbs round it
            a = math.radians(30.0 * k + 15.0)
            g.box(WARM, (cx + 290 * math.cos(a), -30, zc + 10 + 385 * math.sin(a)), (40, 40, 40), glow=G_BULB)
        for a in (-1, 1):
            g.sphere(BLUSH, (cx + a * 165, -44, zc - 60), 38, scale=(1, 0.3, 0.8), steps=(3, 6))    # cheeks
        if sx < 0:      # laughing: eyes shut in arches, a wide grin
            for a in (-1, 1):
                arc(g, PLUM, cx + a * 105, zc + 100, 38, 68, 15, 165, 4, -52, -40)
            arc(g, MAG, cx, zc - 20, lambda t: 190.0 - 95.0 * math.sin(math.pi * t), 190.0, 200, 340, 6, -52, -40)
        else:           # yawning: eyes shut in sagging lines, brows up, a round mouth
            for a in (-1, 1):
                arc(g, PLUM, cx + a * 105, zc + 150, 42, 66, 200, 340, 4, -52, -40)
                tilt_box(g, PLUM, (cx + a * 60, -46, zc + 215), (cx + a * 150, -46, zc + 240), 12, 22)
            disc(g, PLUM, cx, -52, zc - 110, 86, t=12, steps=10)
            disc(g, MAG, cx, -56, zc - 150, 40, t=6, steps=6)
        for a, ph in ((-1, 0.0), (1, 0.35)):                              # ribbons from the chin
            r = sc.part(P.motion(swing=7, pivot=(cx + a * 40, -10, zc - 330), axis=90.0, rate=0.25, phase=ph + (0.2 if sx > 0 else 0.0)))
            tilt_box(r, MAG if sx < 0 else BLUSH, (cx + a * 40, -10, zc - 330), (cx + a * 120, -10, zc - 640), 10, 60)
            tilt_box(r, MAG if sx < 0 else BLUSH, (cx + a * 120, -10, zc - 640), (cx + a * 70, -10, zc - 900), 10, 60)


def chandelier(sc):
    """Chandelier: 9.6 x 3.4 m, 51 m. A silver gantry over the gap; under it a chandelier of three rings, steady bulbs
    and blush drops, swaying a little."""
    g = sc.still()
    zt = 4850.0
    for sx in (-1, 1):
        g.box(PLUM, (sx * 420, 0, 50), (200, 240, 700))
        tilt_box(g, SILVER, (sx * 420, 0, 380), (sx * 150, 0, zt), 50, 50)
        tilt_box(g, LILAC, (sx * 250, 0, zt - 460), (sx * 60, 0, zt - 30), 28, 28)
    for k, z in enumerate((1300.0, 2500.0, 3700.0)):
        w = 2 * (420.0 - 270.0 * (z - 380.0) / (zt - 380.0))
        g.box(SILVER, (0, 0, z), (w, 30, 30))
        g.sphere(BLUSH if k % 2 else MAG, (0, -20, z), 44, steps=(3, 6))
    g.box(SILVER, (0, 0, zt + 20), (420, 70, 70))
    poly(g, MAG, [(-190, zt + 55), (190, zt + 55), (0, zt + 250)], -30, 30)
    g.sphere(SILVER, (0, 0, zt + 270), 34, steps=(3, 6))
    c = sc.part(P.motion(swing=2.5, pivot=(0, 0, zt - 10), axis=90.0, rate=0.11, phase=0.0))
    c.base = ft_art._xf((0, 0, 0), (0, 0, 0), (1.0, 0.5, 1.0))            # an oval chandelier: 6.6 m across, 3.3 m deep
    c.box(SILVER, (0, 0, zt - 260), (18, 36, 500))
    c.cyl(MAG, (0, 0, zt - 640), 150, 150, steps=8, top_r=30)
    c.cyl(SILVER, (0, 0, zt - 1500), 16, 880, steps=5)
    c.sphere(MAG, (0, 0, zt - 1540), 70, steps=(4, 8))
    c.cyl(BLUSH, (0, 0, zt - 1700), 6, 110, steps=5, top_r=40)
    for r, z, n in ((330.0, zt - 1350.0, 10), (240.0, zt - 1020.0, 8), (150.0, zt - 740.0, 6)):
        c.cyl(SILVER, (0, 0, z), r, 26, steps=n)
        c.cyl(SILVER, (0, 0, z + 26), r - 18, 60, steps=n, top_r=24)
        for k in range(n):
            a = math.radians(360.0 * k / n)
            x, y = r * math.cos(a), r * math.sin(a)
            c.box(WARM, (x, y, z + 60), (40, 80, 56), glow=G_BULB)
            c.cyl(BLUSH, (x, y, z - 96), 5, 90, steps=4, top_r=20)


def ticket_mast(sc):
    """TicketMast: 4 x 3.9 m, 53 m. A roll of blank tickets on a silver mast, a strip of them unreeling down it and
    swaying; a star in a ring of bulbs on top. Tickets carry a stamp of a star or a disc: no words, no numbers."""
    g = sc.still()
    my = 120.0
    g.box(PLUM, (0, my, 50), (200, 200, 700))
    g.box(BLUSH, (0, my, 420), (230, 230, 50))
    g.cyl(SILVER, (0, my, 400), 36, 4500, steps=6)
    for z in (1600.0, 3200.0):
        g.cyl(BLUSH, (0, my, z), 50, 60, steps=6)
    zr = 4680.0
    g.box(SILVER, (0, my - 70, zr), (60, 200, 60))
    rod(g, BLUSH, (-170, -10, zr), (170, -10, zr), 122, steps=10)
    for sx in (-1, 1):
        rod(g, MAG, (sx * 170, -10, zr), (sx * 196, -10, zr), 150, steps=10)
        rod(g, SILVER, (sx * 196, -10, zr), (sx * 216, -10, zr), 30, steps=6)
    star(g, WARM, 0, my - 30, 5080, 210, t=24, glow=0.9)
    star(g, MAG, 0, my - 40, 5080, 96, t=10)
    for k in range(8):
        a = math.radians(45.0 * k + 22.5)
        g.box(WARM, (270 * math.cos(a), my - 20, 5080 + 270 * math.sin(a)), (40, 40, 40), glow=G_BULB)
    s = sc.part(P.motion(swing=3, pivot=(0, -134, zr), axis=90.0, rate=0.13, phase=0.0))
    for k in range(7):
        z = zr - 120 - k * 310
        s.box(BLUSH if k % 2 else WARM, (0, -138, z - 150), (300, 12, 290))
        s.box(PLUM, (0, -138, z - 303), (300, 14, 16))
        for sx in (-1, 1):                                                # the punched notch each side
            s.box(PLUM, (sx * 138, -146, z - 150), (26, 6, 60))
        if k % 2:
            star(s, MAG, 0, -152, z - 150, 70, t=6)
        else:
            disc(s, MAG, 0, -152, z - 150, 56, t=6, steps=8)
            disc(s, WARM, 0, -156, z - 150, 26, t=4, steps=6)


# ------------------------------------------------------------------------------------------ the roof prop
def moon_flats(sc):
    """MoonFlats: 4.4 x 2.4 m, 5.9 m. Scenery stored on a picture house's roof: a crescent moon on its braces (a
    steady glow), two stars on springs, a cloud, a trunk and a rolled backdrop."""
    g = sc.still()
    g.box(SILVER, (0, 0, 6), (440, 240, 12))
    arc(g, WARM, 20, 330, lambda t: 230.0 - 112.0 * math.sin(math.pi * t), 230.0, 65, 295, 8, -20, 4, glow=0.7)
    for x in (-90.0, 20.0):
        tilt_box(g, SILVER, (x, 100, 12), (x - 60, 4, 300), 12, 12)
    g.box(SILVER, (-90, 0, 60), (12, 12, 100))
    for k, (dx, r) in enumerate(((-64, 56), (0, 78), (64, 52))):          # the cloud
        disc(g, LILAC, 100 + dx, -70, 104 + (24 if k == 1 else 0), r, t=10, steps=8)
    g.box(LILAC, (100, -65, 62), (224, 10, 64))
    g.box(SILVER, (100, -40, 30), (10, 60, 40))
    for x, z, col, ph in ((120.0, 430.0, BLUSH, 0.0), (150.0, 250.0, MAG, 0.4)):
        g.box(SILVER, (x, 40, z / 2.0), (8, 8, z))
        p = sc.part(P.motion(swing=6, pivot=(x, 40, 12), axis=90.0, rate=0.3, phase=ph))
        star(p, col, x, 30, z + 40, 62, t=10, glow=G_SOFT)
    g.box(PLUM, (-150, -70, 40), (110, 70, 56))
    g.box(SILVER, (-150, -106, 40), (114, 4, 10))
    rod(g, BLUSH, (-200, 95, 30), (60, 95, 30), 18, steps=6)


# ------------------------------------------------------------------------------------------ low props (the kerb: unlit, under 2 m)
def velvet_benches(sc):
    """VelvetBenches: 4.8 x 1.3 m, 1.5 m. Two plum velvet benches with buttoned backs, silver posts with magenta
    balls, a framed playbill (a picture) between them."""
    g = sc.still()
    for sx in (-1, 1):
        x = sx * 135.0
        g.box(SILVER, (x, 0, -5), (196, 50, 50))
        for e in (-1, 1):
            g.box(SILVER, (x + e * 84, 0, 18), (12, 50, 46))
        g.box(PLUM, (x, -4, 48), (200, 58, 18))
        g.box(PLUM, (x, 28, 88), (200, 14, 66))
        g.box(BLUSH, (x, 28, 124), (204, 18, 8))
        for k in range(5):
            g.box(LILAC, (x - 72 + k * 36, 19, 92), (8, 5, 8))
        for e in (-1, 1):
            rod(g, MAG, (x + e * 100, -30, 64), (x + e * 100, 30, 64), 12, steps=6)
    for x in (-250.0, 0.0, 250.0):
        for y in (-52.0,):
            g.cyl(SILVER, (x, y, -30), 13, 22, steps=6)
            g.cyl(SILVER, (x, y, -8), 5, 100, steps=5)
            g.sphere(MAG, (x, y, 100), 11, steps=(3, 5))
    g.cyl(SILVER, (0, 40, -30), 16, 20, steps=6)                          # the playbill on its stand
    g.box(SILVER, (0, 40, 40), (8, 8, 110))
    g.box(LILAC, (0, 38, 120), (60, 8, 76))
    g.box(WARM, (0, 33, 120), (48, 4, 64))
    disc(g, MAG, -8, 27, 130, 13, t=4, steps=6)
    g.box(PLUM, (10, 29, 104), (20, 4, 20), rot=(45, 0, 0))


def stage_trunks(sc):
    """StageTrunks: 3.6 x 1.5 m, 1.94 m. Touring trunks stacked (plum, lilac, magenta: silver corners, a blush band),
    a top hat on them, and a wardrobe rail of five costumes that sway."""
    g = sc.still()

    def trunk(x, y, z, w, d, h, col):
        g.box(col, (x, y, z + h / 2.0), (w, d, h))
        g.box(BLUSH, (x, y, z + h * 0.55), (w + 3, d + 3, 9))
        for sx in (-1, 1):
            g.box(SILVER, (x + sx * (w / 2.0 - 5), y - d / 2.0 - 1, z + h / 2.0), (10, 4, h + 2))
        g.box(SILVER, (x, y - d / 2.0 - 2, z + h * 0.55), (14, 5, 16))

    trunk(-95, 10, -30, 150, 78, 96, PLUM)
    trunk(-110, 14, 66, 104, 64, 54, LILAC)
    trunk(-40, -52, -30, 70, 44, 76, MAG)
    g.cyl(PLUM, (-118, 14, 120), 26, 4, steps=8)
    g.cyl(PLUM, (-118, 14, 124), 16, 26, steps=8, top_r=18)
    g.cyl(MAG, (-118, 14, 124), 17, 6, steps=8)
    for k in range(4):                                                    # a feather boa over the big trunk
        g.sphere(BLUSH, (-50 + k * 16, -32, 72 - k * 13), 9, steps=(3, 5))
    x0, x1, zr = 25.0, 170.0, 188.0                                       # the rail
    for x in (x0, x1):
        g.box(SILVER, (x, 10, zr / 2.0 - 15), (6, 6, zr + 30))
        g.box(SILVER, (x, 10, -24), (10, 70, 8))
    g.box(SILVER, (97.5, 10, zr), (156, 6, 6))
    for k, col in enumerate((MAG, WARM, LILAC, BLUSH, PLUM)):
        x = 40.0 + k * 29.0
        c = sc.part(P.motion(swing=3, pivot=(x, 10, zr), axis=0.0, rate=0.22, phase=k * 0.17))
        c.box(SILVER, (x, 10, zr - 9), (5, 5, 14))
        c.cyl(col, (x, 10, zr - 44), 13, 28, steps=6, top_r=9)
        c.cyl(col, (x, 10, zr - 150 + (k % 2) * 22), 22, 106 - (k % 2) * 22, steps=6, top_r=12)
        c.box(WARM if col != WARM else MAG, (x, -3, zr - 62), (14, 4, 6))


def spot_planters(sc):
    """SpotPlanters: 3.9 x 1.2 m, 1.8 m. Three tubs shaped like stage spotlights turned up on their yokes (unlit),
    barn doors open, a camellia bush in bloom in each."""
    g = sc.still()
    for k in range(3):
        x = -135.0 + k * 135.0
        col = (PLUM, SILVER, PLUM)[k]
        g.box(SILVER, (x, 0, -10), (92, 60, 40))
        for sx in (-1, 1):
            g.box(SILVER, (x + sx * 50, 0, 46), (8, 22, 84))
        g.cyl(col, (x, 0, 22), 34, 30, steps=8, top_r=43)
        g.cyl(col, (x, 0, 52), 43, 62, steps=8)
        g.cyl(LILAC, (x, 0, 110), 47, 10, steps=8)
        for a in range(4):
            ang = math.radians(90.0 * a)
            cx, cy = x + 52 * math.cos(ang), 52 * math.sin(ang)
            tilt_box(g, BLUSH if k != 1 else MAG, (cx, cy, 112), (cx + 22 * math.cos(ang), cy + 22 * math.sin(ang), 150), 60, 5)
        g.sphere(PINE, (x, 0, 138), 34, scale=(1, 1, 0.7), steps=(3, 6))
        g.sphere(BLUSH if k != 1 else WARM, (x, 0, 152), 30, steps=(4, 6))
        for a in range(5):
            ang = math.radians(72.0 * a + k * 20.0)
            g.sphere(MAG, (x + 24 * math.cos(ang), 24 * math.sin(ang), 156 + (a % 2) * 10), 11, steps=(3, 5))
        g.sphere(MAG, (x, 0, 180), 10, steps=(3, 5))


def hat_box_kiosk(sc):
    """HatBoxKiosk: 2.3 x 2.4 m, 1.95 m. A ticket kiosk shaped like a round hat box: blush with magenta stripes and
    a ribbon, a lilac lid with a bow, a shuttered window under a striped awning (unlit)."""
    g = sc.still()
    R = 100.0
    g.cyl(PLUM, (0, 10, -30), R + 6, 40, steps=12)
    g.cyl(BLUSH, (0, 10, 10), R, 134, steps=12)
    for k in range(6):
        a = math.radians(60.0 * k + 30.0)
        g.box(MAG, ((R - 1) * math.cos(a), 10 + (R - 1) * math.sin(a), 77), (16, 8, 134), rot=(0, 60.0 * k + 120.0, 0))
    g.cyl(MAG, (0, 10, 56), R + 3, 20, steps=12)
    g.cyl(LILAC, (0, 10, 144), R + 8, 26, steps=12)
    g.cyl(PLUM, (0, 10, 150), R + 10, 8, steps=12)
    for sx in (-1, 1):                                                    # the bow
        g.sphere(MAG, (sx * 26, 10, 180), 20, scale=(1.3, 0.6, 0.75), steps=(3, 6))
        tilt_box(g, MAG, (sx * 10, 10, 172), (sx * 46, -20, 172), 12, 4)
    g.sphere(WARM, (0, 10, 180), 11, steps=(3, 5))
    yw = 10.0 - R                                                         # the drum's front
    g.box(DEEP, (0, yw - 2, 104), (72, 8, 50))                            # the window, its sill, the awning
    g.box(WARM, (0, yw - 8, 76), (86, 20, 7))
    for k in range(5):
        tilt_box(g, MAG if k % 2 == 0 else WARM, (-36 + k * 18, yw + 2, 136), (-36 + k * 18, yw - 24, 124), 18, 5)
    g.cyl(BLUSH, (24, yw - 10, 80), 6, 12, steps=6)
    g.box(SILVER, (-26, yw - 10, 84), (16, 10, 8))


def poster_drums(sc):
    """PosterDrums: 2.6 x 1.3 m, 1.9 m. Two poster drums under magenta caps. The posters are pictures (a mask, a
    star, a dancer): no words."""
    g = sc.still()
    for sx in (-1, 1):
        x = sx * 68.0
        g.cyl(PLUM, (x, 0, -30), 60, 44, steps=10)
        g.cyl(WARM, (x, 0, 14), 52, 136, steps=10)
        g.cyl(LILAC, (x, 0, 150), 60, 12, steps=10)
        g.sphere(MAG, (x, 0, 160), 54, scale=(1, 1, 0.5), steps=(4, 10))
        for k, ang in enumerate((-126.0, -90.0, -54.0)):
            a = math.radians(ang)
            px, py = x + 53 * math.cos(a), 53 * math.sin(a)
            yaw = ang + 90.0
            col = (BLUSH, LILAC, PLUM)[(k + (1 if sx > 0 else 0)) % 3]
            g.box(col, (px, py, 86), (30, 4, 96), rot=(0, yaw, 0))
            qx, qy = x + 56 * math.cos(a), 56 * math.sin(a)
            kind = (k + (0 if sx < 0 else 1)) % 3
            if kind == 0:       # a mask: a pale face, two dark eyes
                g.box(WARM, (qx, qy, 98), (20, 4, 26), rot=(0, yaw, 0))
                g.box(DEEP, (qx, qy, 102), (14, 6, 5), rot=(0, yaw, 0))
            elif kind == 1:     # a star: a square on its corner
                g.box(MAG, (qx, qy, 96), (18, 4, 18), rot=(45, yaw, 0))
                g.box(WARM, (qx, qy, 64), (20, 4, 6), rot=(0, yaw, 0))
            else:               # a dancer: a skirt and a head
                g.cyl(MAG, (qx, qy, 78), 10, 22, steps=5, top_r=4)
                g.sphere(WARM, (qx, qy, 108), 6, steps=(3, 5))


# ------------------------------------------------------------------------------------------ the hero
def grand_curtain(sc):
    """GrandCurtain: 83 x 13 m, 100 m. A proscenium arch standing free over the roofs: two plum towers with boxes
    and onion cupolas, a lilac sunburst fan across the top, a magenta curtain (three swags that billow, two drapes
    whose tails sway) parted on a lit backdrop with a painted sun."""
    g = sc.still()
    TX, TW, TD, TH = 3500.0, 1300.0, 1100.0, 8200.0
    XI = TX - TW / 2.0                                                    # the opening's edge: 28.5 m from the middle
    ZL = 7400.0                                                           # the opening's top
    yf = -TD / 2.0
    for sx in (-1, 1):
        g.box(PLUM, (sx * TX, 0, (TH - 300) / 2.0), (TW, TD, TH + 300))
        for z in (60.0, 2600.0, 5000.0, TH - 30):
            g.box(BLUSH, (sx * TX, 0, z), (TW + 70, TD + 70, 90))
        g.box(LILAC, (sx * (TX + TW / 2.0 - 70), yf - 16, TH / 2.0), (140, 32, TH))
        bulbs(g, (sx * (XI + 60), yf - 40, 600), (sx * (XI + 60), yf - 40, ZL), 8, s=80)
        for j, z in enumerate((900.0, 1700.0, 4100.0, 6500.0)):
            for k in range(2):
                if (j + k) % 4 == 3:
                    continue
                g.box(WARM, (sx * (TX - 50 + (k * 2 - 1) * 280), yf - 5, z + 200), (220, 12, 420), glow=G_WIN)
        for z in (2900.0, 5300.0):                                        # a box (a balcony) with a lit door and a swag
            x = sx * (TX - 60)
            g.box(DEEP, (x, yf - 6, z + 330), (620, 12, 660))
            g.box(WARM, (x, yf - 14, z + 300), (520, 8, 560), glow=G_WIN)
            g.box(BLUSH, (x, yf - 130, z - 20), (820, 260, 150))
            for k in range(3):
                g.box(WARM, (x - 270 + k * 270, yf - 220, z + 130), (100, 70, 150))
            g.box(SILVER, (x, yf - 220, z + 225), (840, 90, 40))
            swag(g, MAG, x - 330, x + 330, z + 720, 190, yf - 40, yf - 10, glow=G_CLOTH)
        # the cupola
        g.box(SILVER, (sx * TX, 0, TH + 30), (TW + 130, TD + 130, 60))
        g.cyl(LILAC, (sx * TX, 0, TH + 60), 500, 300, steps=10)
        g.cyl(WARM, (sx * TX, 0, TH + 140), 506, 110, steps=10, glow=G_WIN)
        top = onion(g, MAG, sx * TX, 0, TH + 360, 400, steps=10)
        pennant(sc, sx * TX, 0, top - 40, 420, 420, col=BLUSH, t=24.0, ph=0.25 * (sx + 1))
    # the entablature and the fan
    g.box(PLUM, (0, 0, ZL + 250), (2 * XI + 40, 760, 500))
    g.box(BLUSH, (0, 0, ZL + 30), (2 * XI + 40, 820, 60))
    g.box(SILVER, (0, 0, ZL + 520), (2 * XI + 300, 860, 60))
    bulbs(g, (-XI + 100, -445, ZL + 30), (XI - 100, -445, ZL + 30), 13, s=70)
    ZF, RX, RZ = ZL + 550.0, 3300.0, 1880.0
    halfdisc(g, LILAC, 0, ZF, RX, RZ, -200, 200, n=14)
    for k in range(9):
        a = math.radians(14.0 + k * 19.0)
        tilt_box(g, WARM, (RX * 0.27 * math.cos(a), -220, ZF + RZ * 0.27 * math.sin(a)), (RX * 0.9 * math.cos(a), -220, ZF + RZ * 0.9 * math.sin(a)), 40, 150, glow=0.9)
    halfdisc(g, MAG, 0, ZF, RX * 0.22, RZ * 0.22, -250, -200, n=8)
    halfdisc(g, WARM, 0, ZF, RX * 0.1, RZ * 0.1, -270, -250, n=6, glow=G_WIN)
    for k in range(11):
        a = math.radians(7.5 + k * 16.5)
        g.box(WARM, (RX * 0.97 * math.cos(a), -230, ZF + RZ * 0.97 * math.sin(a)), (76, 76, 76), glow=G_BULB)
    g.sphere(SILVER, (0, 0, ZF + RZ + 60), 110, steps=(4, 8))
    # the stage, its steps, the footlights, the prompt box
    g.box(PLUM, (0, 100, 50), (2 * XI + 40, 1300, 700))
    for k in range(3):
        top = 290.0 - k * 110.0
        g.box(BLUSH if k == 0 else PLUM, (0, -550 - k * 130 - 65, (top - 300) / 2.0), (4200 + k * 300, 130, top + 300))
    for k in range(7):
        x = -2400.0 + k * 800.0
        if k == 3:
            halfdisc(g, LILAC, 0, 400, 330, 260, -560, -470, n=6)
            continue
        halfdisc(g, LILAC, x, 400, 200, 170, -540, -500, n=5)
        g.box(WARM, (x, -470, 470), (90, 60, 90), glow=G_BULB)
    # the backdrop: lit, a sun over two hills
    yb = 360.0
    g.box(WARM, (0, yb + 10, (ZL + 400) / 2.0), (2 * XI, 20, ZL - 400), glow=0.32)
    halfdisc(g, BLUSH, 0, 1900, 1500, 1500, yb - 20, yb, n=12, glow=0.45)
    halfdisc(g, LILAC, -1500, 400, 1700, 1500, yb - 40, yb - 20, n=8, glow=0.12)
    halfdisc(g, LILAC, 1700, 400, 1500, 1150, yb - 44, yb - 24, n=8, glow=0.12)
    for k, (x, z) in enumerate(((-1900.0, 5600.0), (-600.0, 6300.0), (1000.0, 5800.0))):
        star(g, WARM, x, yb - 26, z, 130 + (k % 2) * 50, t=20, glow=0.9)
    # the curtain. Three swags across the top, each billowing on its own time; tassels and rosettes at the joins
    yc = -200.0
    xs = (-XI, -XI / 3.0, XI / 3.0, XI)
    for k in range(3):
        v = sc.part(P.motion(swing=1.0, pivot=((xs[k] + xs[k + 1]) / 2.0, yc, ZL), axis=0.0, rate=0.07, phase=k * 0.33))
        swag(v, MAG, xs[k] - 30, xs[k + 1] + 30, ZL + 20, 1350, yc - 60, yc + 60, n=8, glow=G_CLOTH)
        swag(v, BLUSH, xs[k] + 150, xs[k + 1] - 150, ZL + 20, 620, yc - 90, yc - 60, n=6, glow=G_CLOTH)
        for j in range(3):                                                # the fringe
            t = (j + 1) / 4.0
            v.box(WARM, (xs[k] + (xs[k + 1] - xs[k]) * t, yc - 70, ZL + 20 - 1350 * math.sin(math.pi * t) - 20), (150, 30, 90), glow=G_SOFT)
    for k in range(4):
        g.cyl(WARM, (xs[k] * 0.985, yc - 110, ZL - 620), 130, 560, steps=6, top_r=36, glow=G_SOFT)
        disc(g, MAG, xs[k] * 0.985, -425, ZL + 250, 210, t=45, steps=10)
        disc(g, WARM, xs[k] * 0.985, -445, ZL + 250, 90, t=20, steps=8, glow=G_WIN)
    # two drapes, parted: gathered at a tie 27 m up; the upper lengths billow, the tails under the tie sway
    ZT = 2700.0
    for sx, ph in ((-1, 0.0), (1, 0.5)):
        up = sc.part(P.motion(swing=0.6, pivot=(sx * 1950.0, yc + 80, ZL), axis=0.0, rate=0.08, phase=ph))
        lo = sc.part(P.motion(swing=2.5, pivot=(sx * 2540.0, yc + 80, ZT), axis=90.0, rate=0.08, phase=ph + 0.15))
        for i in range(5):
            xt = sx * (XI - 190.0 - i * 360.0)
            xg = sx * (2700.0 - i * 80.0)
            xb = sx * (2780.0 - i * 190.0)
            tilt_box(up, MAG, (xt, yc + 70 - i * 6, ZL - 20), (xg, yc + 70 - i * 6, ZT), 60, 300, glow=G_CLOTH)
            tilt_box(lo, MAG, (xg, yc + 70 - i * 6, ZT), (xb, yc + 70 - i * 6, 420), 60, 230, glow=G_CLOTH)
        poly(up, PLUM, [(sx * (XI + 10), ZL - 20), (sx * 1010.0, ZL - 20), (sx * 2230.0, ZT), (sx * (XI + 10), ZT)], yc + 100, yc + 130)   # the shadow between the folds
        poly(lo, PLUM, [(sx * (XI + 10), ZT), (sx * 2230.0, ZT), (sx * 1880.0, 420), (sx * (XI + 10), 420)], yc + 100, yc + 130)
        g.box(WARM, (sx * 2540.0, yc + 10, ZT), (620, 120, 130), glow=G_SOFT)       # the tie, its tassel, its boss on the tower
        g.cyl(WARM, (sx * 2290.0, yc - 20, ZT - 520), 100, 480, steps=6, top_r=26, glow=G_SOFT)
        g.sphere(SILVER, (sx * (XI - 20), yc, ZT), 110, steps=(4, 6))
    # the cartouche over the middle: a rosette between two scrolls
    disc(g, MAG, 0, -440, ZL + 250, 300, t=50, steps=12)
    disc(g, WARM, 0, -460, ZL + 250, 140, t=20, steps=8, glow=G_WIN)
    for sx in (-1, 1):
        arc(g, SILVER, sx * 520, ZL + 250, 120, 190, 0, 360, 6, -420, -390)


JOBS = [
    ('SM_S1D2_PuppetHouse', puppet_house, dict(grow=(40, 40, 30), grow_down=(40, 40, 0))),
    ('SM_S1D2_RehearsalLoft', rehearsal_loft, dict(grow=(40, 40, 30), grow_down=(40, 40, 0))),
    ('SM_S1D2_CostumeHouse', costume_house, dict(grow=(40, 40, 30), grow_down=(40, 40, 0))),
    ('SM_S1D2_DanceDrum', dance_drum, dict(grow=(80, 80, 30), grow_down=(80, 80, 0))),
    ('SM_S1D2_MusicHall', music_hall, STILL),
    ('SM_S1D2_FlyTower', fly_tower, dict(grow=(60, 60, 30), grow_down=(60, 60, 0))),
    ('SM_S1D2_MaskMast', mask_mast, dict(grow=(60, 30, 0), grow_down=(60, 30, 0))),
    ('SM_S1D2_Chandelier', chandelier, dict(grow=(90, 30, 0), grow_down=(90, 30, 0))),
    ('SM_S1D2_TicketMast', ticket_mast, dict(grow=(130, 30, 0), grow_down=(130, 30, 0))),
    ('SM_S1D2_MoonFlats', moon_flats, dict(grow=(50, 20, 10), grow_down=(50, 20, 0))),
    ('SM_S1D2_VelvetBenches', velvet_benches, STILL),
    ('SM_S1D2_StageTrunks', stage_trunks, dict(grow=(5, 12, 0), grow_down=(5, 12, 0))),
    ('SM_S1D2_SpotPlanters', spot_planters, STILL),
    ('SM_S1D2_HatBoxKiosk', hat_box_kiosk, STILL),
    ('SM_S1D2_PosterDrums', poster_drums, STILL),
    ('SM_S1D2_GrandCurtain', grand_curtain, dict(far=True, grow=(160, 120, 40), grow_down=(160, 120, 0))),
]


def run(only=None):
    P.build_core()                              # loads the two masters; never build_core(rebuild=True)
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
