"""Volcano, Stage 1, builder s1d4: district 4 "The Burning Slopes" (11.1 to 13.9 km), THE CLIFF TEMPLE.
/Game/World/Volcano/SM_S1D4_*. Run: import ft_world_volcano_s1d4 as K; K.run()

A mountain monastery stepping up between the lava falls: white walls that lean inwards, jade roofs in tiers with
gold upturned corners, moon doors, long stairs. 8 buildings, 10 props, 1 hero (the Bell Tower: seven tiers, 50 m, one
great gold bell swinging slowly in its open top). Every mesh is an ft_people.Scene: one material, one section.
Nothing here carries a lamp: every colour has the level's small steady lift (0.04; gold 0.10, the bell 0.14), and
every rule that places a piece is 24 m or more out. The stone lanterns' openings are dark (no light, no orange).
Movement is the vertex shader's only: the bell, pennants, banners, robes, a gong, prayer drums, wisps, vanes.
"""
import math
import ft_people as P
from ft_desert import tilt_box

DIR = '/Game/World/Volcano'
STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))

W, J, JD, G, PL = '#f6f3ea', '#2fae7a', '#1d8a5e', '#ffd23f', '#5a2a6a'
ST, STD, PANE = '#b0a088', '#87746d', '#14101a'       # pale stone (kit PALE_L), its shade (kit PALE), kit PANE
R2 = 1.41421
LIFT = {G: 0.10, PANE: 0.0}


def gl(col):
    return LIFT.get(col, 0.04)


def bx(m, col, c, s, rot=(0, 0, 0), glow=None):
    m.box(col, c, s, rot=rot, glow=gl(col) if glow is None else glow)


def cy(m, col, base, r, h, steps=8, top_r=None, rot=(0, 0, 0), glow=None):
    m.cyl(col, base, r, h, rot=rot, steps=steps, top_r=top_r, glow=gl(col) if glow is None else glow)


def sp(m, col, c, r, scale=(1, 1, 1), steps=(4, 8), glow=None):
    m.sphere(col, c, r, scale=scale, steps=steps, glow=gl(col) if glow is None else glow)


def tb(m, col, a, b, w, d=None):
    tilt_box(m, col, a, b, w, d, glow=gl(col))


def sq(m, col, x, y, z, hb, ht, h):
    """A square frustum: half-widths hb at z and ht at z + h (a leaning wall, a hipped roof)."""
    cy(m, col, (x, y, z), hb * R2, h, steps=4, top_r=max(ht, 2.0) * R2, rot=(0, 45, 0))


def trap(m, col, x0, x1, z0, z1, lean, y0, y1):
    """A wall along X whose two ends lean inwards by `lean`."""
    m.side(col, [(x0, z0), (x1, z0), (x1 - lean, z1), (x0 + lean, z1)], y0, y1, glow=gl(col))


def disc(m, col, x, y, z, r, t=10.0, steps=12):
    """A disc standing on the front face (its axis runs along Y), its front at y."""
    cy(m, col, (x, y, z), r, t, steps=steps, rot=(0, 0, 90))


def moon(m, x, y, z, r, inner=PL):
    """A moon door or window: a gold ring and what is seen through it."""
    disc(m, G, x, y, z, r, 10.0)
    disc(m, inner, x, y - 5.0, z, r * 0.82, 10.0)


def horns(m, x, y, z, hx, hy, out=50.0, up=46.0, t=16.0):
    """The four upturned corners of a roof, gold."""
    for sx in (-1, 1):
        for sy in (-1, 1):
            a = (x + sx * hx, y + sy * hy, z)
            tb(m, G, a, (a[0] + sx * out, a[1] + sy * out, z + up), t, t)


def roof_sq(m, x, y, z, half, over=80.0, rise=140.0, top=0.42, col=J):
    """One tier of a square roof: an eave board, a low flare, a steep cap, four gold corners. Returns its top."""
    e = half + over
    bx(m, JD, (x, y, z + 8), (2 * e, 2 * e, 16))
    sq(m, col, x, y, z + 16, e, half * 0.84, rise * 0.4)
    sq(m, col, x, y, z + 16 + rise * 0.4, half * 0.84, half * top, rise * 0.6)
    horns(m, x, y, z + 10, e - 8, e - 8, out=over * 0.55, up=rise * 0.42)
    return z + 16 + rise


def roof_gable(m, x, y, z, hx, hy, over=80.0, rise=180.0, col=J, ribs=5):
    """A roof whose ridge runs along X: flared eaves, a steep upper pitch, tile ribs, plum gables, gold ridge ends
    and corners."""
    ex, ey = hx + over, hy + over
    bx(m, JD, (x, y, z + 8), (2 * ex, 2 * ey, 16))
    for s in (-1, 1):
        tb(m, col, (x, y + s * ey, z + 14), (x, y + s * hy * 0.55, z + 14 + rise * 0.38), 2 * ex, 14)
        tb(m, col, (x, y + s * hy * 0.62, z + rise * 0.32), (x, y, z + rise), 2 * ex - 40, 14)
        for k in range(ribs):
            xr = x + (k - (ribs - 1) / 2.0) * (2 * ex - 90) / float(max(ribs - 1, 1))
            tb(m, JD, (xr, y + s * ey, z + 14), (xr, y + s * hy * 0.55, z + 14 + rise * 0.38), 22, 44)
            tb(m, JD, (xr, y + s * hy * 0.62, z + rise * 0.32), (xr, y, z + rise), 22, 44)
        tb(m, G, (x + s * (ex - 24), y, z + rise + 2), (x + s * (ex + 34), y, z + rise + 58), 22, 22)
    bx(m, PL, (x, y, z + rise * 0.31), (2 * hx + 20, hy * 0.6, rise * 0.62))
    bx(m, JD, (x, y, z + rise + 2), (2 * ex - 30, 30, 28))
    horns(m, x, y, z + 10, ex - 8, ey - 8, out=over * 0.55, up=rise * 0.36)
    return z + rise + 16


def finial(m, x, y, z, h=90.0, r=24.0):
    cy(m, G, (x, y, z), r * 0.5, h, steps=6)
    cy(m, G, (x, y, z + h * 0.45), r * 1.1, 8, steps=8)
    sp(m, G, (x, y, z + h + r * 0.7), r)
    return z + h + r * 1.7


def planter(m, x, y, z=0.0, r=38.0):
    """A stone tub with a clipped jade bush."""
    bx(m, ST, (x, y, z + 14), (64, 64, 68))
    sp(m, J, (x, y, z + 48 + r * 0.8), r, steps=(3, 6))


def window(m, x, y, z, w=60.0, h=80.0):
    """A plum frame and a dark pane on the front face at y."""
    bx(m, PL, (x, y - 2, z), (w + 30, 8, h + 30))
    bx(m, PANE, (x, y - 5, z), (w, 8, h))


def door(m, x, y, z, w=110.0, h=220.0):
    bx(m, PL, (x, y - 2, z + h / 2.0), (w, 10, h))
    bx(m, G, (x + w * 0.28, y - 8, z + h * 0.5), (12, 6, 12))


def hang(sc, col, x, y, z_top, w, h, swing=6.0, rate=0.3, phase=0.0, axis=0.0, tail=None, t=5.0):
    """A banner hanging from (x, y, z_top), swaying about its top edge."""
    f = sc.part(P.motion(swing=swing, pivot=(x, y, z_top), axis=axis, rate=rate, phase=phase))
    bx(f, col, (x, y, z_top - h / 2.0), (w, t, h))
    if tail:
        bx(f, tail, (x, y, z_top - h + h * 0.07), (w, t + 1.0, h * 0.14))
    return f


# ------------------------------------------------------------------------------------------ buildings
def gate_house(sc):
    """BUILDING 9 x 5.4 m, 10.6 m: the gate of the monastery, a moon door under a two-tier roof, two banners."""
    g = sc.still()
    bx(g, ST, (0, 0, -70), (980, 540, 300))
    trap(g, W, -430, 430, 80, 430, 35, -230, 230)
    moon(g, 0, -234, 255, 168)
    bx(g, ST, (0, -250, 50), (380, 40, 60))
    for x in (-310, 310):
        window(g, x, -230, 300)
        planter(g, x * 1.3, -236, 80)
    z = roof_gable(g, 0, 0, 430, 395, 230, over=88, rise=150)
    trap(g, W, -250, 250, 520, 760, 25, -150, 150)
    bx(g, PL, (0, -152, 660), (380, 8, 90))
    bx(g, PANE, (0, -155, 660), (300, 8, 46))
    z = roof_gable(g, 0, 0, 760, 225, 150, over=84, rise=170)
    finial(g, 0, 0, z - 10, 80, 22)
    for k, x in enumerate((-235, 235)):
        hang(sc, W, x, -246, 400, 60, 230, swing=6, rate=0.28, phase=0.2 + 0.4 * k, tail=PL)


def prayer_hall(sc):
    """BUILDING 16.8 x 7.8 m, 12.6 m: the prayer hall on its plinth, a wide stair, six plum columns, banners between."""
    g = sc.still()
    bx(g, ST, (0, 30, -40), (1640, 640, 380))
    for k in range(3):
        zt = 150.0 - (k + 1) * 38.0
        bx(g, W, (0, -302 - 20 * k, (zt - 100) / 2.0), (620, 24, zt + 100))
    for sx in (-1, 1):
        bx(g, ST, (sx * 350, -310, 120), (56, 56, 240))
        sq(g, J, sx * 350, -310, 240, 46, 8, 44)
    for k in range(6):
        cy(g, PL, (-700 + k * 280, -230, 150), 26, 400, steps=6)
    trap(g, W, -760, 760, 150, 545, 30, -130, 330)
    for x in (-280, 0, 280):
        door(g, x, -130, 150, 150, 270)
    for x in (-560, 560):
        moon(g, x, -134, 330, 62, PANE)
    bx(g, PL, (0, -230, 548), (1500, 50, 40))
    z = roof_gable(g, 0, 30, 566, 760, 290, over=80, rise=200)
    trap(g, W, -460, 460, 690, 860, 28, -110, 170)
    bx(g, PL, (0, -112, 790), (760, 8, 84))
    bx(g, PANE, (0, -115, 790), (680, 8, 40))
    z = roof_gable(g, 0, 30, 860, 430, 150, over=84, rise=230)
    finial(g, 0, 30, z - 10, 110, 28)
    cols = (J, G, W, G, J)
    for k in range(5):
        hang(sc, cols[k], -560 + k * 280, -232, 526, 90, 170, swing=6, rate=0.26 + 0.03 * k, phase=k * 0.23, tail=PL)


def store_tower(sc):
    """BUILDING 6.2 x 6.2 m, 16 m: the three-tier store tower, a gold fish vane turning on its top."""
    g = sc.still()
    bx(g, ST, (0, 0, -80), (560, 560, 300))
    sq(g, W, 0, 0, 70, 240, 205, 380)
    door(g, 0, -232, 70, 110, 220)
    z = roof_sq(g, 0, 0, 450, 205, over=80, rise=130)
    sq(g, W, 0, 0, 540, 175, 150, 300)
    bx(g, PL, (0, -166, 700), (210, 12, 84))
    bx(g, PANE, (0, -170, 700), (140, 10, 46))
    for sx in (-1, 1):
        bx(g, PL, (sx * 166, 0, 700), (12, 210, 84))
        bx(g, PANE, (sx * 170, 0, 700), (10, 140, 46))
    z = roof_sq(g, 0, 0, 840, 150, over=75, rise=120)
    sq(g, W, 0, 0, 930, 125, 105, 240)
    moon(g, 0, -120, 1050, 52, PANE)
    z = roof_sq(g, 0, 0, 1170, 105, over=70, rise=190, top=0.2)
    z = finial(g, 0, 0, z - 14, 120, 24)
    v = sc.part(P.motion(swing=38, pivot=(0, 0, z + 20), axis=P.UP, rate=0.11, phase=0.3))
    bx(v, G, (0, 0, z + 8), (8, 8, 36))
    bx(v, G, (34, 0, z + 22), (96, 6, 30))
    bx(v, G, (-44, 0, z + 22), (30, 6, 46))


def kitchen(sc):
    """BUILDING 11.8 x 6 m, 9.2 m: the kitchen, four round chimneys with jade caps, steam off them, an awning."""
    g = sc.still()
    bx(g, ST, (0, 0, -80), (1160, 540, 300))
    trap(g, W, -540, 540, 70, 360, 30, -220, 220)
    for x in (-360, -130):
        window(g, x, -220, 235, 70, 90)
    door(g, 200, -220, 70, 130, 230)
    planter(g, 440, -238, 70, 32)
    tb(g, J, (200, -224, 345), (200, -330, 300), 230, 12)
    for sx in (-1, 1):
        cy(g, PL, (200 + sx * 100, -318, -20), 8, 320, steps=6)
    for k in range(3):
        cy(g, W, (-470 + k * 62, -262, -20), 26, 96, steps=6, top_r=20)
        cy(g, PL, (-470 + k * 62, -262, 76), 22, 10, steps=6)
    z = roof_gable(g, 0, 0, 360, 510, 220, over=80, rise=180)
    for k in range(4):
        x = -360 + k * 240
        cy(g, W, (x, 150, 420), 42, 380, steps=8, top_r=36)
        cy(g, PL, (x, 150, 720), 44, 26, steps=8)
        sq(g, J, x, 150, 800, 62, 14, 52)
        sp(g, G, (x, 150, 862), 12, steps=(3, 6))
        f = sc.part(P.motion(bob=-16, swing=5, pivot=(x, 150, 800), axis=0, rate=0.22 + 0.02 * k, phase=k * 0.27))
        sp(f, W, (x + 10, 150, 892), 30, steps=(3, 6))
        sp(f, W, (x + 34, 150, 926), 20, steps=(3, 6))


def lodge(sc):
    """BUILDING 13.6 x 6.5 m, 10.5 m: the pilgrims' lodge, a gallery on plum posts, robes airing on its rail."""
    g = sc.still()
    bx(g, ST, (0, 0, -80), (1360, 560, 300))
    trap(g, W, -640, 640, 70, 350, 25, -180, 250)
    bx(g, PL, (0, -30, 360), (1300, 520, 24))
    for k in range(6):
        x = -600 + k * 240
        cy(g, PL, (x, -256, 70), 16, 280, steps=6)
        cy(g, PL, (x, -272, 372), 13, 250, steps=6)
    for x in (-360, 0, 360):
        door(g, x, -180, 70, 120, 220)
    bx(g, PL, (0, -184, 318), (160, 8, 40))                       # a sign without words: three gold squares
    for k in (-1, 0, 1):
        bx(g, G, (k * 44, -189, 318), (20, 6, 20))
    for x in (-180, 180):
        planter(g, x, -230, 70, 30)
    trap(g, W, -610, 610, 372, 622, 20, -150, 250)
    bx(g, PL, (0, -280, 446), (1300, 10, 12))
    bx(g, PL, (0, -280, 408), (1300, 8, 8))
    for k in range(5):
        window(g, -480 + k * 240, -150, 500, 70, 100)
    z = roof_gable(g, 0, -20, 622, 600, 250, over=80, rise=160)
    trap(g, W, -300, 300, 740, 860, 20, -100, 100)
    bx(g, PL, (0, -102, 800), (480, 8, 60))
    z = roof_gable(g, 0, 0, 860, 280, 110, over=70, rise=150)
    finial(g, 0, 0, z - 10, 70, 20)
    cols = (PL, W, J, W)
    for k in range(4):
        hang(sc, cols[k], -420 + k * 250 + 30, -294, 446, 70, 92, swing=6, rate=0.34 + 0.03 * k, phase=k * 0.31, t=6.0)


def rock_shrine(sc):
    """BUILDING 4 x 3.6 m, 8.7 m: a small shrine on a tall rock plinth reached by a ladder stair; three flags."""
    g = sc.still()
    cy(g, STD, (0, 10, -150), 165, 430, steps=6, top_r=140)
    cy(g, ST, (10, 0, 262), 140, 200, steps=6, top_r=118)
    for sx in (-1, 1):
        tb(g, PL, (sx * 40, -176, -10), (sx * 40, -112, 462), 8, 8)
    for k in range(5):
        t = (k + 0.7) / 5.5
        bx(g, PL, (0, -176 + 64 * t, -10 + 472 * t), (88, 8, 8))
    bx(g, ST, (0, 0, 472), (250, 250, 24))
    sq(g, W, 0, 0, 484, 86, 72, 150)
    door(g, 0, -80, 484, 56, 104)
    z = roof_sq(g, 0, 0, 634, 72, over=58, rise=112, top=0.2)
    z = finial(g, 0, 0, z - 12, 60, 15)
    cy(g, PL, (100, 60, 484), 6, 308, steps=6)
    tb(g, W, (100, 60, 786), (0, 0, 786), 4, 4)
    ax = math.degrees(math.atan2(60, 100))
    cols = (J, W, G)
    for k in range(3):
        t = (k + 0.8) / 3.6
        hang(sc, cols[k], 100 * (1 - t), 60 * (1 - t), 786, 30, 42, swing=14, rate=0.7 + 0.1 * k, phase=k * 0.33, axis=ax)


def drum_pavilion(sc):
    """BUILDING 6.8 x 6.8 m, 10 m: the drum pavilion, open: four plum posts, a great white drum, tassels at the eaves."""
    g = sc.still()
    bx(g, ST, (0, 0, -60), (560, 560, 320))
    bx(g, W, (0, -292, 15), (240, 40, 170))
    for sx in (-1, 1):
        for sy in (-1, 1):
            cy(g, PL, (sx * 220, sy * 220, 100), 22, 390, steps=6)
        bx(g, PL, (0, sx * 220, 472), (480, 30, 36))
        bx(g, PL, (sx * 220, 0, 472), (30, 480, 36))
    cy(g, W, (-76, 0, 300), 96, 152, steps=10, rot=(-90, 0, 0))
    for sx in (-1, 1):
        cy(g, PL, (sx * 76 - 7, 0, 300), 102, 14, steps=10, rot=(-90, 0, 0))
        tb(g, PL, (sx * 60, -90, 100), (sx * 60, 40, 240), 14, 14)
        tb(g, PL, (sx * 60, 90, 100), (sx * 60, -40, 240), 14, 14)
    cy(g, G, (-7, 0, 300), 99, 14, steps=10, rot=(-90, 0, 0))
    z = roof_sq(g, 0, 0, 490, 250, over=90, rise=190)
    sq(g, W, 0, 0, 650, 110, 96, 100)
    z = roof_sq(g, 0, 0, 750, 96, over=64, rise=130, top=0.2)
    finial(g, 0, 0, z - 12, 80, 20)
    k = 0
    for sx in (-1, 1):
        for sy in (-1, 1):
            f = sc.part(P.motion(swing=13, pivot=(sx * 318, sy * 318, 500), axis=45 * sx * sy, rate=0.5 + 0.06 * k, phase=k * 0.27))
            bx(f, PL, (sx * 318, sy * 318, 468), (10, 10, 64))
            bx(f, W, (sx * 318, sy * 318, 428), (16, 16, 22))
            k += 1


def moon_terrace(sc):
    """BUILDING 14 x 5.8 m, 7.4 m: three stone terraces, a long white stair up the middle to a moon gate in a wall."""
    g = sc.still()
    bx(g, ST, (0, 60, -50), (1400, 460, 360))
    bx(g, STD, (0, 115, 200), (1100, 350, 140))
    bx(g, ST, (0, 170, 335), (800, 240, 130))
    for k in range(8):
        zt = 50.0 * (k + 1)
        y0 = -290.0 + k * 42.5
        bx(g, W, (0, (y0 + 50.0) / 2.0, zt / 2.0 - 20), (260, 50.0 - y0, zt + 40))
    for sx in (-1, 1):
        tb(g, PL, (sx * 146, -288, 46), (sx * 146, 46, 446), 30, 16)
        sp(g, G, (sx * 146, -288, 84), 20)
        bx(g, W, (sx * 270, 250, 530), (260, 40, 260))
        bx(g, JD, (sx * 270, 250, 666), (300, 96, 14))
        bx(g, J, (sx * 270, 250, 684), (280, 44, 24))
        bx(g, ST, (sx * 480, -20, 320), (50, 50, 100))
        sq(g, J, sx * 480, -20, 370, 44, 8, 42)
    n = 12
    for k in range(n):
        a = 2 * math.pi * (k + 0.5) / n
        bx(g, W, (132 * math.cos(a), 250, 540 + 132 * math.sin(a)), (76, 40, 36), rot=(math.degrees(a) + 90, 0, 0))
    bx(g, JD, (0, 250, 692), (380, 110, 14))
    bx(g, J, (0, 250, 712), (320, 56, 28))
    for sx in (-1, 1):
        tb(g, G, (sx * 180, 250, 696), (sx * 226, 250, 740), 16, 16)
        cy(g, PL, (sx * 640, -130, 130), 8, 500, steps=6)
        sp(g, G, (sx * 640, -130, 640), 12, steps=(3, 6))
        hang(sc, W, sx * 610, -130, 620, 44, 250, swing=8, rate=0.3, phase=0.25 + 0.2 * sx, tail=J)


# ------------------------------------------------------------------------------------------ the hero
def bell_tower(sc):
    """HERO 16 x 16 m, 49.7 m: the Bell Tower. A stepped plinth and a stair, six white tiers under jade roofs, the
    seventh open: one great gold bell swinging slowly; a gold spire of rings. Flag lines down to the plinth."""
    g = sc.still()
    bx(g, ST, (0, 0, -50), (1600, 1600, 300))
    bx(g, STD, (0, 0, 160), (1340, 1340, 120))
    bx(g, ST, (0, 0, 275), (1100, 1100, 110))
    for k in range(8):
        zt = 41.25 * (k + 1)
        y0 = -960.0 + k * 51.25
        bx(g, W, (0, (y0 - 550.0) / 2.0, zt / 2.0 - 20), (320, -550.0 - y0, zt + 40))
    for sx in (-1, 1):
        tb(g, PL, (sx * 180, -958, 50), (sx * 180, -552, 384), 36, 18)
        for sy in (-1, 1):
            bx(g, ST, (sx * 720, sy * 720, 180), (70, 70, 160))
            sq(g, J, sx * 720, sy * 720, 260, 58, 10, 56)
    hb = (520, 455, 395, 340, 290, 245)
    bh = (500, 460, 430, 400, 375, 350)
    ov = (130, 120, 110, 105, 100, 95)
    ri = (170, 160, 150, 140, 135, 130)
    nw = (3, 3, 2, 2, 1, 1)
    z = 330.0
    zs = []
    for i in range(6):
        zs.append(z)
        h0, h1 = hb[i], hb[i] - 25
        sq(g, W, 0, 0, z, h0, h1, bh[i])
        zm = z + bh[i] * 0.56
        f = h0 - 25 * 0.56 + 2          # the face at the band's height
        wdt = 2 * (h1 - 50)
        if i == 0:
            moon(g, 0, -(h0 - 8), z + 205, 150)
        else:
            bx(g, PL, (0, -f, zm), (wdt, 14, bh[i] * 0.34))
        for sx in (-1, 1):
            bx(g, PL, (sx * f, 0, zm), (14, wdt, bh[i] * 0.34))
        for k in range(nw[i]):
            u = (k - (nw[i] - 1) / 2.0) * (wdt / float(nw[i]))
            if i > 0:
                bx(g, PANE, (u, -f - 5, zm), (64, 10, bh[i] * 0.22))
            for sx in (-1, 1):
                bx(g, PANE, (sx * (f + 5), u, zm), (10, 64, bh[i] * 0.22))
        bx(g, PL, (0, -(h0 + 26), z + 46), (2 * h0 + 60, 10, 10))
        for sx in (-1, 1):
            bx(g, PL, (sx * (h0 + 26), 0, z + 46), (10, 2 * h0 + 60, 10))
        zr = z + bh[i]
        roof_sq(g, 0, 0, zr, h1, over=ov[i], rise=ri[i], top=0.6)
        e = h1 + ov[i] - 30
        for sx in (-1, 1):
            for sy in (-1, 1):
                bx(g, G, (sx * e, sy * e, zr - 16), (16, 16, 26))
        z = zr + 16 + ri[i] * (0.3 if i < 5 else 0.15)
    # the open seventh tier and the bell
    bx(g, PL, (0, 0, z + 12), (510, 510, 24))
    for sx in (-1, 1):
        for sy in (-1, 1):
            cy(g, PL, (sx * 215, sy * 215, z + 24), 20, 436, steps=6)
        bx(g, PL, (0, sx * 215, z + 86), (430, 10, 10))
        bx(g, PL, (sx * 215, 0, z + 86), (10, 430, 10))
        bx(g, PL, (0, sx * 215, z + 446), (470, 30, 30))
        bx(g, PL, (sx * 215, 0, z + 446), (30, 470, 30))
    zb = z + 440.0
    bx(g, PL, (0, 0, zb), (460, 34, 34))
    b = sc.part(P.motion(swing=11, pivot=(0, 0, zb), axis=20, rate=0.16, phase=0.0))
    cy(b, G, (0, 0, zb - 262), 132, 22, steps=10, glow=0.14)
    cy(b, G, (0, 0, zb - 242), 122, 200, steps=10, top_r=62, glow=0.14)
    cy(b, G, (0, 0, zb - 44), 30, 40, steps=6, glow=0.14)
    cy(b, PL, (0, 0, zb - 276), 16, 60, steps=6)
    sp(b, PL, (0, 0, zb - 286), 26, steps=(3, 6))
    zr = z + 464.0
    zt = roof_sq(g, 0, 0, zr, 255, over=110, rise=260, top=0.12)
    cy(g, G, (0, 0, zt - 30), 20, 660, steps=6)
    for k in range(5):
        cy(g, G, (0, 0, zt + 40 + k * 96), 72 - k * 11, 18, steps=8)
    sp(g, G, (0, 0, zt + 650), 40)
    cy(g, G, (0, 0, zt + 670), 8, 360, steps=6)
    sp(g, G, (0, 0, zt + 1040), 18, steps=(3, 6))
    # two banners on the second tier, two flag lines from its roof down to the plinth's corner posts
    z1 = zs[1]
    for k, sx in enumerate((-1, 1)):
        hang(sc, W, sx * (hb[1] - 70), -(hb[1] + 6), z1 + bh[1] - 40, 70, 300, swing=4, rate=0.22, phase=0.3 * k + 0.1, tail=PL, t=6.0)
        a = (sx * (hb[1] + 60), -(hb[1] + 60), z1 + bh[1] + 20)
        c = (sx * 720, -720, 330)
        tb(g, W, a, c, 5, 5)
        ax = math.degrees(math.atan2(c[1] - a[1], c[0] - a[0]))
        cols = (J, W, G, PL, W)
        for n in range(5):
            t = (n + 1) / 6.0
            p = tuple(a[q] + (c[q] - a[q]) * t for q in range(3))
            hang(sc, cols[n], p[0], p[1], p[2], 56, 70, swing=14, rate=0.5 + 0.05 * n, phase=n * 0.21 + 0.4 * k, axis=ax, t=6.0)


# ------------------------------------------------------------------------------------------ props, low
def lantern_row(sc):
    """PROP, LOW (1.9 m): three stone lanterns on a kerb. Pale stone, dark openings: no light in them."""
    g = sc.still()
    bx(g, ST, (0, 0, -5), (640, 50, 30))
    for x in (-250, 0, 250):
        bx(g, ST, (x, 0, 20), (64, 64, 26))
        cy(g, ST, (x, 0, 30), 14, 72, steps=6)
        bx(g, STD, (x, 0, 108), (60, 60, 14))
        bx(g, W, (x, 0, 137), (44, 44, 44))
        for sy in (-1, 1):
            bx(g, PANE, (x, sy * 21, 137), (22, 6, 24))
            bx(g, PANE, (x + sy * 21, 0, 137), (6, 22, 24))
        sq(g, STD, x, 0, 159, 42, 8, 31)


def incense_urn(sc):
    """PROP, LOW (1.9 m): the great incense urn on three feet, a thin pale wisp over it."""
    g = sc.still()
    bx(g, ST, (0, 0, 0), (120, 106, 40))
    for k in range(3):
        a = math.radians(90 + k * 120)
        cy(g, PL, (32 * math.cos(a), 32 * math.sin(a), 20), 9, 42, steps=6)
    cy(g, ST, (0, 0, 58), 30, 60, steps=8, top_r=52)
    cy(g, PL, (0, 0, 96), 48, 10, steps=8)
    cy(g, G, (0, 0, 118), 53, 8, steps=8)
    sp(g, W, (0, 0, 124), 40, scale=(1, 1, 0.3), steps=(3, 8))
    for sx in (-1, 1):
        tb(g, ST, (sx * 46, 0, 100), (sx * 66, 0, 132), 10, 10)
        bx(g, PL, (sx * 9, 5 * sx, 150), (4, 4, 44))
    for k, (zz, r) in enumerate(((148, 9), (163, 11), (178, 12))):
        f = sc.part(P.motion(bob=-4, swing=7, pivot=(0, 0, 126), axis=0, rate=0.3 + 0.04 * k, phase=k * 0.3))
        sp(f, W, (0, 0, zz), r, steps=(3, 6))


def prayer_drums(sc):
    """PROP, LOW (1.96 m): five plum prayer drums on a rail under a little jade ridge; each turns to and fro."""
    g = sc.still()
    bx(g, ST, (0, 0, 10), (520, 60, 60))
    for sx in (-1, 1):
        bx(g, PL, (sx * 246, 0, 105), (22, 22, 130))
        tb(g, G, (sx * 276, 0, 178), (sx * 298, 0, 192), 8, 8)
    bx(g, PL, (0, 0, 165), (540, 30, 14))
    bx(g, JD, (0, 0, 176), (560, 70, 8))
    bx(g, J, (0, 0, 185), (540, 36, 10))
    for k in range(5):
        x = -180 + k * 90
        cy(g, ST, (x, 0, 40), 5, 118, steps=6)
        d = sc.part(P.motion(swing=170, pivot=(x, 0, 95), axis=P.UP, rate=0.16 + 0.03 * k, phase=k * 0.21))
        cy(d, PL, (x, 0, 60), 26, 70, steps=8)
        cy(d, G, (x, 0, 68), 27.5, 8, steps=8)
        cy(d, G, (x, 0, 114), 27.5, 8, steps=8)
        bx(d, W, (x, -25, 95), (18, 6, 30))


def stone_tortoise(sc):
    """PROP, LOW (1.95 m): a stone tortoise on a plinth with a blank white tablet on its back; its head nods."""
    g = sc.still()
    bx(g, ST, (0, 0, 5), (250, 100, 50))
    sp(g, STD, (0, 0, 62), 60, scale=(1.3, 0.75, 0.6), steps=(4, 8))
    bx(g, PL, (0, 0, 96), (64, 72, 8))
    bx(g, G, (0, 0, 101), (56, 22, 4))
    for sx in (-1, 1):
        for sy in (-1, 1):
            bx(g, ST, (sx * 52, sy * 38, 37), (30, 24, 20))
    tb(g, ST, (-70, 0, 60), (-104, 0, 44), 10, 10)
    bx(g, W, (0, 0, 146), (50, 16, 90))
    bx(g, G, (0, 0, 193), (58, 22, 5))
    h = sc.part(P.motion(swing=5, pivot=(70, 0, 60), axis=90, rate=0.15, phase=0.2))
    tb(h, ST, (64, 0, 60), (116, 0, 92), 22, 22)
    bx(h, ST, (128, 0, 98), (34, 28, 24))
    for sy in (-1, 1):
        bx(h, PANE, (138, sy * 13, 103), (5, 5, 5))


def water_jars(sc):
    """PROP, LOW (1.7 m): four white water jars with plum lids on a stone bench, a carrying yoke and two pails."""
    g = sc.still()
    bx(g, ST, (0, 0, 15), (420, 80, 70))
    for k, x in enumerate((-150, -50, 50, 150)):
        s = (1.0, 0.86, 1.08, 0.9)[k]
        cy(g, W, (x, 0, 50), 30 * s, 40 * s, steps=8, top_r=36 * s)
        cy(g, J, (x, 0, 50 + 40 * s), 37 * s, 8, steps=8)
        cy(g, W, (x, 0, 58 + 40 * s), 36 * s, 45 * s, steps=8, top_r=22 * s)
        cy(g, PL, (x, 0, 58 + 85 * s), 24 * s, 9, steps=8)
        sp(g, G, (x, 0, 72 + 85 * s), 6, steps=(3, 6))
    tb(g, PL, (222, -30, -10), (236, 24, 166), 7, 7)
    for (x, y) in ((252, -22), (262, 20)):
        cy(g, W, (x, y, -20), 14, 46, steps=6, top_r=17)
        cy(g, PL, (x, y, 26), 18, 5, steps=6)


# ------------------------------------------------------------------------------------------ props, tall and open
def pennant_line(sc):
    """PROP, TALL, OPEN (8.8 m): two thin plum poles 9 m apart, a line of nine pennants waving between them."""
    g = sc.still()
    pts = ((-450, 806), (-150, 748), (150, 748), (450, 806))
    for sx in (-1, 1):
        bx(g, ST, (sx * 450, 0, 10), (50, 50, 60))
        cy(g, PL, (sx * 450, 0, 40), 7, 812, steps=6)
        sp(g, G, (sx * 450, 0, 864), 13, steps=(3, 6))
    for a, b in zip(pts, pts[1:]):
        tb(g, W, (a[0], 0, a[1]), (b[0], 0, b[1]), 4, 4)
    cols = (W, J, G, PL)
    for k in range(9):
        x = -400 + k * 100
        seg = 0 if x < -150 else (1 if x < 150 else 2)
        a, b = pts[seg], pts[seg + 1]
        zl = a[1] + (b[1] - a[1]) * (x - a[0]) / float(b[0] - a[0])
        f = sc.part(P.motion(swing=16, pivot=(x, 0, zl), axis=0, rate=0.6 + 0.04 * (k % 4), phase=k * 0.17))
        bx(f, cols[k % 4], (x, 0, zl - 33), (44, 4, 44), rot=(45, 0, 0))


def banner_mast(sc):
    """PROP, TALL, OPEN (10.3 m): one mast with a three-drum crown, two long banners swaying, a gold vane turning."""
    g = sc.still()
    cy(g, ST, (0, 0, -20), 32, 60, steps=8, top_r=26)
    cy(g, PL, (0, 0, 40), 8, 950, steps=6)
    cy(g, G, (0, 0, 796), 36, 6, steps=8)
    cy(g, W, (0, 0, 802), 33, 48, steps=8)
    cy(g, J, (0, 0, 852), 29, 48, steps=8)
    cy(g, PL, (0, 0, 902), 25, 44, steps=8)
    sp(g, G, (0, 0, 998), 12, steps=(3, 6))
    bx(g, PL, (0, 0, 760), (150, 6, 6))
    for k, sx in enumerate((-1, 1)):
        hang(sc, W, sx * 60, 0, 757, 46, 390, swing=5, rate=0.3, phase=0.15 + 0.45 * k, tail=PL)
    v = sc.part(P.motion(swing=40, pivot=(0, 0, 1016), axis=P.UP, rate=0.1, phase=0.5))
    bx(v, G, (22, 0, 1018), (64, 5, 18))
    bx(v, G, (-22, 0, 1018), (20, 5, 28))


def moon_arch(sc):
    """PROP, TALL, OPEN (6.9 m): a moon GATE, a half arch of pale stone standing on two solid square piers joined by a
    stone sill, under a jade cap, a tassel. (Review round: it was a whole ring on two thin legs and read as a floating
    ring beside the road where the boss marks rings; now it is a gateway with a solid foot.)"""
    g = sc.still()
    bx(g, STD, (0, 0, 0), (560, 96, 60))                                  # the sill, footed 30 cm into the ground
    for sx in (-1, 1):
        bx(g, ST, (sx * 220, 0, 195), (112, 84, 390))                     # a solid pier up to the arch's spring
        bx(g, W, (sx * 220, 0, 398), (132, 96, 24))
        tb(g, G, (sx * 118, 0, 666), (sx * 164, 0, 710), 14, 14)
    n, R = 7, 220.0
    for k in range(n):                                                    # the upper half only
        a = math.pi * (k + 0.5) / n
        bx(g, ST if k % 2 else W, (R * math.cos(a), 0, 410 + R * math.sin(a)), (108, 44, 50), rot=(math.degrees(a) + 90, 0, 0))
    bx(g, JD, (0, 0, 660), (240, 90, 12))
    bx(g, J, (0, 0, 676), (200, 50, 22))
    f = sc.part(P.motion(swing=10, pivot=(0, 0, 606), axis=0, rate=0.32, phase=0.1))
    bx(f, PL, (0, 0, 560), (10, 10, 92))
    bx(f, W, (0, 0, 506), (18, 18, 26))


def gong_frame(sc):
    """PROP, TALL, OPEN (4.2 m): a gold gong hung in a plum frame under a little jade roof, swinging a little."""
    g = sc.still()
    bx(g, ST, (0, 0, 5), (380, 90, 50))
    for sx in (-1, 1):
        cy(g, PL, (sx * 150, 0, 30), 13, 330, steps=6)
        tb(g, G, (sx * 200, 0, 350), (sx * 240, 0, 386), 16, 16)
        tb(g, J, (0, sx * 56, 378), (0, 0, 412), 410, 10)
    bx(g, PL, (0, 0, 350), (400, 24, 24))
    bx(g, JD, (0, 0, 372), (430, 116, 10))
    bx(g, PL, (120, -12, 205), (8, 8, 90))
    bx(g, W, (120, -12, 152), (16, 16, 18))
    f = sc.part(P.motion(swing=4, pivot=(0, 0, 340), axis=0, rate=0.22, phase=0.0))
    disc(f, G, 0, -5, 215, 82, 10.0)
    disc(f, W, 0, -10, 215, 15, 6.0, steps=8)                              # a small pale boss (review round: the dark centre read as a bull's-eye)
    for sx in (-1, 1):
        tb(f, W, (sx * 40, 0, 338), (sx * 40, 0, 286), 4, 4)


def robe_line(sc):
    """PROP, TALL, OPEN (3.3 m): a washing line of six robes between two posts, plum, white and jade, swaying."""
    g = sc.still()
    pts = ((-330, 312), (-110, 290), (110, 290), (330, 312))
    for sx in (-1, 1):
        bx(g, ST, (sx * 330, 0, 0), (44, 44, 40))
        cy(g, PL, (sx * 330, 0, 20), 8, 300, steps=6)
        bx(g, PL, (sx * 330, 0, 316), (8, 60, 8))
    for a, b in zip(pts, pts[1:]):
        tb(g, W, (a[0], 0, a[1]), (b[0], 0, b[1]), 4, 4)
    cols = (PL, W, J, PL, W, J)
    for k in range(6):
        x = -260 + k * 104
        zl = 290 + 22 * max(0.0, (abs(x) - 110) / 220.0)
        f = sc.part(P.motion(swing=8, pivot=(x, 0, zl), axis=0, rate=0.34 + 0.03 * (k % 3), phase=k * 0.19))
        bx(f, cols[k], (x, 0, zl - 60), (56, 5, 112))
        bx(f, cols[k], (x, 0, zl - 16), (104, 5, 26))
        bx(f, G if cols[k] == PL else PL, (x, 0, zl - 62), (58, 6, 10))


SWAY = dict(grow=(10, 30, 10), grow_down=(10, 30, 0))          # what a banner or a pennant sweeps beyond the mesh
SMALL = dict(grow=(6, 8, 6), grow_down=(6, 8, 0))
JOBS = [
    ('SM_S1D4_GateHouse', gate_house, SWAY), ('SM_S1D4_PrayerHall', prayer_hall, SWAY), ('SM_S1D4_StoreTower', store_tower, SMALL),
    ('SM_S1D4_Kitchen', kitchen, dict(grow=(10, 10, 30), grow_down=(10, 10, 0))), ('SM_S1D4_Lodge', lodge, SWAY),
    ('SM_S1D4_RockShrine', rock_shrine, SWAY), ('SM_S1D4_DrumPavilion', drum_pavilion, SWAY), ('SM_S1D4_MoonTerrace', moon_terrace, SWAY),
    ('SM_S1D4_BellTower', bell_tower, dict(far=True, grow=(30, 30, 0), grow_down=(30, 30, 0))),
    ('SM_S1D4_LanternRow', lantern_row, STILL), ('SM_S1D4_IncenseUrn', incense_urn, SMALL), ('SM_S1D4_PrayerDrums', prayer_drums, SMALL),
    ('SM_S1D4_StoneTortoise', stone_tortoise, SMALL), ('SM_S1D4_WaterJars', water_jars, STILL),
    ('SM_S1D4_PennantLine', pennant_line, dict(grow=(8, 24, 8), grow_down=(8, 24, 0))),
    ('SM_S1D4_BannerMast', banner_mast, dict(grow=(10, 36, 6), grow_down=(10, 36, 0))),
    ('SM_S1D4_MoonArch', moon_arch, dict(grow=(6, 16, 0), grow_down=(6, 16, 0))),
    ('SM_S1D4_GongFrame', gong_frame, dict(grow=(6, 18, 0), grow_down=(6, 18, 0))),
    ('SM_S1D4_RobeLine', robe_line, dict(grow=(8, 22, 0), grow_down=(8, 22, 0))),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
