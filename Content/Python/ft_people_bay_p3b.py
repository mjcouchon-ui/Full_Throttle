"""FULL THROTTLE (Unreal) the population, level 0, Bayshore, pass 3, builder p3b: THE SCENES (/Game/People/Bayshore/SM_P3B_...).

Story groups that could only be on a seaside boardwalk town on the afternoon of the chase. One mesh a scene, 8 to 14
figures each doing something different, with the props that tell the story. Placed by the block C:/ftwork/p3/L0b.inl.

  the beach (bay side, 40 m out, on the sand; each at most 14 m along the road and 8 m deep)
    SM_P3B_Wedding       a beach wedding under a flower arch: the couple, the officiant, guests throwing petals,
                         a photographer, a flower child, the cake
    SM_P3B_SandCastle    the sandcastle contest: a towered castle and a sand turtle behind a ribbon, children digging,
                         one running rings round it with a bucket, the judge, the winner with the cup
    SM_P3B_SurfBoat      the lifeguards' drill: a surf boat on its cradle, four rowing in time, the cox calling the
                         stroke, three doing star jumps in a row, the instructor with a flag
    SM_P3B_FishMarket    the catch landed on the beach: a marlin on the weighing frame, the auctioneer on a crate,
                         bidders, the fish stall under its awning, a pelican, gulls, the cat
    SM_P3B_Luau          a limbo party: the dancer rocking under the bar, the queue, three drummers, hula dancers
    SM_P3B_SurfContest   the surf contest's prize-giving: three judges holding up their cards on a scaffold, the
                         winners on the podium with boards and a cup, photographers, fans
    SM_P3B_Yoga          a class on a deck: ten people opening and closing their arms as one (one out of time, one
                         asleep on his mat), the teacher on her block
    SM_P3B_TugOfWar      a tug of war: two teams of four and their rope sliding to and fro, the referee, supporters
  the town (town side, on a low stage of its own on a side street's corner, 29.5 m out; 12.4 m by 5.6 m)
    SM_P3B_Carnival_A    a carnival float: dancers with feather fans before a great shell, drummers, a stilt walker,
                         a flag, confetti and balloons
    SM_P3B_Carnival_B    the steel band's float under a great sun: four on the pans, a singer, shakers, dancers
    SM_P3B_StreetShow    street performers: a juggler, three acrobats in a tower, a golden living statue, a
                         portrait artist and his sitter, a balloon seller, a child on a parent's shoulders
    SM_P3B_Mural         a mural being painted: two painters on a scaffold, one mixing paint, a spray artist, a child
                         painting the bottom, the designer pointing with her plan

All are over 2 m as built (TALL): 24 m or more from the centre line. No glow anywhere, nothing blinks.
Run in the editor:  import ft_people_bay_p3b as K; K.run()        (or K.run(['SM_P3B_Wedding']))
"""
import math
import random
import ft_people as P
from ft_desert import tilt_box
from ft_people_bay import (WOOD, WOOD_D, WHITE, CREAM, SAND, CORAL, TEAL, YEL, HOTPINK, MINT, SKY, NAVY, RED, LAV, HOT, SWIM, LIFT, CHEER,
                           beach_look, town_look)

DIR = '/Game/People/Bayshore'
STEEL, GOLD, SANDW, SAND_D, SILVER, LIME = '#c9c4d6', '#e8c26a', '#e9b96a', '#c98f4a', '#d7e3f0', '#b8ff5a'
FOOT = 220.0         # how far a footing goes down under z = 0 (a piece stands level on land that follows the road)
GUARD = dict(top=YEL, bottom=RED, wear='shorts', hat='cap', hat_col=RED)


def fig(sc, x, y, lk, **kw):
    """One figure from a look and what it does (the two never give the same value twice)."""
    d = dict(lk)
    d.update(kw)
    return P.figure(sc, x, y, **d)


def cheer(sc, r, x, y, z, lk, yaw=0.0, detail=0, k=None, react=True):
    """Someone who has turned to the road to cheer the chase."""
    d = dict(lk)
    d.update(CHEER[r.randrange(len(CHEER)) if k is None else k])
    d.update(z=z, yaw=yaw, detail=detail, phase=r.random(), react=react)
    return P.figure(sc, x, y, **d)


def parasol(g, x, y, z, col, rad=150.0, h=230.0):
    g.cyl(WHITE, (x, y, z - 30), 5, h + 30, steps=6)
    g.cyl(col, (x, y, z + h - 20), rad, 46, steps=8, top_r=8)


def bunting(sc, x0, x1, y, z, cols, n=8, post=True):
    """A line of pennants between two posts; the pennants swing together from the line."""
    g = sc.still()
    if post:
        for x in (x0, x1):
            g.box(WHITE, (x, y, z / 2.0), (8, 8, z))
    g.box(WHITE, ((x0 + x1) / 2.0, y, z - 2), (abs(x1 - x0), 4, 4))
    fl = sc.part(P.motion(swing=20.0, pivot=(0.0, y, z - 2.0), axis=0.0, rate=1.1))
    for k in range(n):
        fl.box(cols[k % len(cols)], (x0 + (k + 0.5) * (x1 - x0) / n, y, z - 22), (34, 4, 36))


def stage(g, w, d, h, top=CREAM, side=TEAL, stripe=WHITE):
    """The town scenes' own floor: h over the land, so that the people on it clear the barrier from the road."""
    g.box(side, (0, 0, (h - 12) / 2.0 - FOOT / 2.0), (w - 16, d - 16, h - 12 + FOOT))
    g.box(top, (0, 0, h - 6), (w, d, 12))
    g.box(stripe, (0, -d / 2.0 + 6, h - 26), (w - 12, 4, 14))


def hold_fan(col, col2):
    def f(m, h, side):
        for j in (-1, 0, 1):
            tilt_box(m, col if j else col2, (h[0], h[1], h[2] + 4), (h[0] + side * 16 + j * 30, h[1], h[2] + 78), 18, 4)
    return f


def hold_thing(col, w=16.0, d=8.0, hgt=40.0, up=16.0):
    """A plain block in the hand: a cup, a fish, a paddle, a plan."""
    def f(m, h, side):
        m.box(col, (h[0], h[1] - 3, h[2] + up), (w, d, hgt))
    return f


def hold_cone(col=WHITE, trim=RED):
    """A megaphone at the face."""
    def f(m, h, side):
        m.box(trim, (h[0], h[1] - 10, h[2] + 4), (12, 16, 12))
        m.box(col, (h[0], h[1] - 30, h[2] + 4), (26, 26, 26))
    return f


def lying(sc, r, x, y, z, yaw, mat):
    m = sc.part(P.motion(bob=-1.2, rate=0.22, phase=r.random()), (x, y, z), yaw)
    skin = r.choice(P.SKINS)
    m.box(mat, (0, 0, 3), (70, 200, 5))
    m.box(skin, (0, 50, 16), (38, 84, 20))
    m.box(r.choice(SWIM), (0, 2, 17), (42, 26, 23))
    m.box(r.choice(HOT), (0, -32, 18), (44, 52, 24))
    m.cyl(skin, (0, -78, 6), 19, 28, steps=6)
    m.box(YEL, (0, -78, 34), (52, 52, 5))                    # the hat over his face


# ------------------------------------------------------------------------------------------ the beach
def wedding(seed=501):
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        z = LIFT
        g.box(WHITE, (0, -40, z + 2), (190, 640, 5))                          # the aisle
        g.box(HOTPINK, (0, -40, z + 3), (44, 640, 5))
        for sx in (-1, 1):                                                   # the arch and its flowers
            g.box(WHITE, (sx * 165, 300, z + 90), (14, 14, 210))
            for k in range(3):
                g.box((HOTPINK, WHITE, YEL)[k], (sx * (96 + k * 4), -300 + k * 190, z + 60), (30, 30, 30))
                g.box(WHITE, (sx * (96 + k * 4), -300 + k * 190, z + 22), (6, 6, 46))
        for k in range(9):
            a = math.radians(k * 22.5)
            g.box((HOTPINK, WHITE, YEL)[k % 3], (math.cos(a) * 165, 298, z + 190 + math.sin(a) * 150), (52, 32, 52), rot=(k * 20.0, 0, 0))
        dr = sc.part(P.motion(swing=7.0, pivot=(0.0, 300.0, z + 330.0), axis=0.0, rate=0.5))
        for sx in (-1, 1):
            dr.box(WHITE, (sx * 128, 306, z + 200), (46, 4, 240))
        fig(sc, -50, 250, P.look(r, top=WHITE, bottom=WHITE, wear='dress', hair='long', sleeves='none', shoes=WHITE, s=0.97), z=z, yaw=90,
            arm_l='fwd', arm_r='fwd', rock=3, rate=0.5)
        g.box(WHITE, (-78, 250, z + 128), (8, 44, 84))                        # the veil
        fig(sc, 50, 250, P.look(r, top=WHITE, bottom=NAVY, wear='trousers', sleeves='long', hair='short', s=1.03), z=z, yaw=-90,
            arm_l='fwd', arm_r='fwd', rock=3, rate=0.5, phase=0.5)
        fig(sc, 0, 350, P.look(r, top=LAV, wear='coat', sleeves='long'), z=z, arm_l='vee', arm_r='vee', swing_l=8, swing_r=-8, rate=0.5, detail=1)
        for ph in (0.0, 0.5):                                                # petals in the air over them
            pt = sc.part(P.motion(bob=-22.0, rate=0.35, phase=ph))
            for k in range(5):
                pt.box((HOTPINK, WHITE)[k % 2], (r.uniform(-150, 150), r.uniform(190, 300), z + r.uniform(215, 300)), (14, 14, 4))
        n = 0
        for sx in (-1, 1):                                                   # the guests, either side of the aisle
            for k in range(4):
                x, y = sx * (250 + (k % 2) * 150 + r.uniform(-20, 20)), -230 + k * 105 + r.uniform(-15, 15)
                lk = beach_look(r, wear=r.choice(('dress', 'skirt', 'trousers', 'shorts')), sleeves='short')
                if n % 3 == 2:
                    cheer(sc, r, x, y, z, lk)
                else:
                    fig(sc, x, y, lk, z=z, yaw=180 - 45 * sx, arm_r='up', arm_l='down', swing_r=20, rate=1.0, bob=4, phase=r.random(), detail=0)
                n += 1
        fig(sc, -190, 110, town_look(r, top=NAVY, hat='cap', hat_col=WHITE), z=z, yaw=115, arm_l='hold', arm_r='hold', hold_r=P.hold_camera(),
            turn=10, rate=0.3, detail=1)
        fig(sc, 70, 30, beach_look(r, build='kid', s=0.66, wear='dress', top=YEL, hat=None), z=z, yaw=180, arm_l='vee', arm_r='vee', bob=12, rate=1.5)
        g.box(WHITE, (470, 290, z + 38), (160, 96, 80))                       # the cake
        for k, (rad, col) in enumerate(((52, HOTPINK), (38, WHITE), (24, HOTPINK))):
            g.cyl(col, (470, 290, z + 78 + k * 26), rad, 26, steps=8)
        parasol(g, -520, 320, z, CORAL)
    return build


def sandcastle(seed=511):
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        z = LIFT
        for k in range(5):                                                   # the ribbon
            g.box(WHITE, (-600 + k * 300, -340, z + 40), (8, 8, 90))
        g.box(HOTPINK, (0, -340, z + 80), (1208, 4, 10))
        g.box(SANDW, (0, 60, z + 45), (300, 300, 110))                        # the castle
        g.box(SAND_D, (0, -92, z + 36), (70, 6, 80))
        g.box(SANDW, (0, 60, z + 150), (170, 170, 100))
        for sx in (-1, 1):
            for sy in (-1, 1):
                g.cyl(SANDW, (sx * 150, 60 + sy * 150, z - 10), 48, 200, steps=8)
                g.cyl(SAND_D, (sx * 150, 60 + sy * 150, z + 190), 56, 14, steps=8)
                g.cyl(SANDW, (sx * 150, 60 + sy * 150, z + 204), 40, 70, steps=8, top_r=4)
        g.cyl(SANDW, (0, 60, z + 200), 50, 90, steps=8)
        g.cyl(SAND_D, (0, 60, z + 290), 58, 80, steps=8, top_r=4)
        g.box(WHITE, (0, 60, z + 400), (5, 5, 80))
        fl = sc.part(P.motion(swing=22.0, pivot=(0.0, 60.0, z + 420.0), axis=P.UP, rate=1.0))
        fl.box(RED, (30, 60, z + 424), (54, 4, 28))
        g.sphere(SANDW, (-430, 150, z + 6), 110, scale=(1.25, 1.0, 0.5), steps=(4, 8))     # the sand turtle
        g.sphere(SANDW, (-580, 150, z + 26), 42, steps=(3, 6))
        for sx in (-1, 1):
            for sy in (-1, 1):
                g.box(SAND_D, (-430 + sx * 110, 150 + sy * 110, z + 8), (80, 44, 16), rot=(0, 30.0 * sx * sy, 0))
        g.cyl(SANDW, (450, 170, z - 6), 96, 76, steps=8, top_r=80)                      # a second, smaller castle
        g.cyl(SANDW, (450, 170, z + 70), 54, 70, steps=8, top_r=44)
        g.cyl(SAND_D, (450, 170, z + 140), 50, 60, steps=8, top_r=4)
        for k, (x, y) in enumerate(((-170, -210), (240, -60), (560, 20))):   # buckets
            g.cyl((RED, TEAL, YEL)[k], (x, y, z), 18, 30, steps=6, top_r=24)
        kid = dict(build='kid', hat='sun')
        fig(sc, -130, -130, beach_look(r, s=0.66, hat_col=YEL, **kid), z=z, yaw=160, arm_l='fwd', arm_r='fwd', swing_l=20, swing_r=20, axis_l='x',
            axis_r='x', rate=1.5)
        fig(sc, 200, -150, beach_look(r, s=0.62, hat_col=HOTPINK, **kid), z=z, yaw=200, legs='sit', seat=8, arm_r='fwd', arm_l='down', swing_r=26,
            axis_r='x', rate=1.3, hold_r=P.hold_tool(RED, YEL, 30))
        fig(sc, 330, 60, beach_look(r, s=0.68, hat_col=TEAL, **kid), z=z, yaw=180, stride=32, arm_l='down', arm_r='carry', swing_l=24, axis_l='x',
            rate=1.9, bob=5, orbit=(0.0, 60.0, 0.09), hold_r=hold_thing(RED, 30, 30, 30, -10))
        fig(sc, -330, -30, beach_look(r, wear='shorts', hat='cap'), z=z, yaw=150, arm_l='push', arm_r='push', swing_l=16, swing_r=16, axis_l='x',
            axis_r='x', rate=0.8, hold_r=P.hold_tool(STEEL, CORAL, 60), detail=1)
        fig(sc, 300, -230, town_look(r, top=WHITE, bottom=NAVY, wear='trousers', hat='sun', hat_col=WHITE, band=RED), z=z, yaw=205, arm_l='hold',
            arm_r='point', swing_r=9, axis_r='x', rate=0.6, hold_l=hold_thing(WHITE, 24, 4, 32, 8), detail=1)
        fig(sc, 560, -90, beach_look(r, build='kid', s=0.7, hat=None, band=RED), z=z, arm_l='up', arm_r='up', hold_r=hold_thing(YEL, 22, 22, 34, 18),
            bob=14, rate=1.6, swing_l=12, swing_r=-12, react=True)
        cheer(sc, r, 640, -200, z, beach_look(r), k=5)
        cheer(sc, r, -600, -200, z, beach_look(r), k=3)
        cheer(sc, r, -520, -260, z, beach_look(r, build='kid', s=0.64), k=1)
        P.quadruped(sc, 60, -250, z=z, yaw=150, length=70, height=48, girth=20, neck=16, neck_up=35, head=18, tail=22, wag=34, rate=2.2,
                    body=GOLD, ears='#5a4a3c', snout='#22202a')
        parasol(g, -260, 340, z, TEAL)
    return build


def surfboat(seed=521):
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        z = LIFT
        g.box(RED, (0, 60, z + 62), (620, 170, 56))                           # the boat, bow to +X, on its cradle
        tilt_box(g, RED, (300, 60, z + 62), (410, 60, z + 80), 124, 50)
        tilt_box(g, RED, (400, 60, z + 78), (466, 60, z + 96), 60, 40)
        tilt_box(g, RED, (-300, 60, z + 62), (-380, 60, z + 74), 130, 50)
        g.box(WHITE, (0, 60, z + 92), (630, 178, 8))
        g.box(YEL, (0, -27, z + 62), (620, 4, 18))
        for sx in (-1, 1):
            g.box(WOOD_D, (sx * 200, 60, z + 16), (40, 240, 40))

        def oar(m, h, side):
            tilt_box(m, WOOD, (h[0] - side * 24, h[1], h[2]), (h[0] + side * 230, h[1] - 30, h[2] - 44), 7, 7)
            m.box(YEL, (h[0] + side * 252, h[1] - 33, h[2] - 48), (64, 8, 24))

        for k in range(4):                                                   # four rowing in time, backs to the bow
            x = -210 + k * 140
            lk = P.look(r, sleeves='none', s=0.96, **GUARD)
            if k % 2:
                fig(sc, x, 60, lk, z=z + 44, yaw=-90, legs='sit', arm_l='fwd', arm_r='fwd', swing_l=22, swing_r=22, axis_l='x', axis_r='x', rate=0.55,
                    hold_r=oar, detail=1)
            else:
                fig(sc, x, 60, lk, z=z + 44, yaw=-90, legs='sit', arm_l='fwd', arm_r='fwd', swing_l=22, swing_r=22, axis_l='x', axis_r='x', rate=0.55,
                    hold_l=oar, detail=1)
        fig(sc, -330, 60, P.look(r, sleeves='short', s=0.95, **GUARD), z=z + 50, yaw=90, arm_r='face', arm_l='up', hold_r=hold_cone(), swing_l=15,
            rate=0.55, detail=1)
        for k in range(3):                                                   # star jumps, as one
            fig(sc, -330 + k * 210, -240, P.look(r, sleeves='none', **GUARD), z=z, legs='wide', arm_l='out', arm_r='out', swing_l=48, swing_r=-48,
                bob=13, rate=1.0, detail=1)
            g.box(CORAL, (-330 + k * 210, -330, z + 12), (70, 22, 22))        # their rescue cans
        fig(sc, 360, -230, P.look(r, sleeves='short', build='stocky', **GUARD), z=z, yaw=-70, arm_r='up', arm_l='hip', hold_r=P.hold_flag(RED, WHITE, 64),
            swing_r=16, rate=1.0, detail=1)
        cheer(sc, r, 560, -150, z, beach_look(r), k=5)
        cheer(sc, r, 640, -60, z, beach_look(r, build='kid', s=0.66), k=0)
        for sx in (-1, 1):                                                   # the bathing flags
            g.box(WHITE, (sx * 640, 280, z + 150), (8, 8, 330))
            fl = sc.part(P.motion(swing=18.0, pivot=(sx * 640.0, 280.0, z + 280.0), axis=P.UP, rate=0.8, phase=0.3 * sx))
            fl.box(RED, (sx * 640 + 50, 280, z + 296), (92, 5, 30))
            fl.box(YEL, (sx * 640 + 50, 280, z + 266), (92, 5, 30))
        tilt_box(g, YEL, (520, 330, z), (560, 250, z + 250), 52, 9)           # a rescue board against the flag pole
    return build


def fishmarket(seed=531):
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        z = LIFT
        BLUE, DARK = '#3a5fa8', NAVY
        g.box(TEAL, (-450, 180, z + 40), (360, 130, 56))                      # the skiff drawn up
        tilt_box(g, TEAL, (-270, 180, z + 40), (-190, 180, z + 62), 96, 46)
        g.box(WHITE, (-450, 180, z + 70), (366, 136, 8))
        g.box(MINT, (-500, 180, z + 84), (150, 90, 26))                       # its net
        for sx in (-1, 1):                                                   # the weighing frame
            tilt_box(g, WOOD_D, (sx * 160, 130, z - 20), (0, 130, z + 400), 14, 14)
        g.box(WOOD_D, (0, 130, z + 392), (90, 20, 16))
        f = sc.part(P.motion(swing=5.0, pivot=(0.0, 130.0, z + 385.0), axis=90.0, rate=0.35))     # the marlin, by the tail
        tilt_box(f, BLUE, (0, 130, z + 345), (0, 130, z + 120), 74, 46)
        f.box(SILVER, (0, 105, z + 228), (50, 6, 190))
        f.box(DARK, (0, 130, z + 362), (160, 10, 40))
        tilt_box(f, DARK, (0, 130, z + 122), (0, 130, z + 26), 9, 9)
        f.box(DARK, (-48, 130, z + 250), (30, 8, 150))
        tx, ty = 400, 190                                                    # the stall under its awning
        g.box(WHITE, (tx, ty, z + 38), (300, 110, 80))
        g.box(SILVER, (tx, ty, z + 82), (290, 100, 8))
        for k in range(6):
            g.box((BLUE, SILVER, CORAL)[k % 3], (tx - 110 + k * 44, ty - 10 + (k % 2) * 24, z + 90), (16, 60, 10))
        for sx in (-1, 1):
            for sy in (-1, 1):
                g.box(WHITE, (tx + sx * 160, ty + sy * 80, z + 130), (8, 8, 270))
        for k in range(6):
            g.box((CORAL, WHITE)[k % 2], (tx - 150 + k * 60, ty, z + 268), (60, 200, 8), rot=(0, 0, -8))
        for k in range(3):
            g.box(WOOD, (190 + (k % 2) * 20, 260, z + 20 + k * 40), (70, 60, 38))
        g.box(WOOD, (150, -30, z + 22), (74, 74, 50))                         # the auctioneer's crate
        fig(sc, -120, 110, P.look(r, top=YEL, bottom=CORAL, wear='trousers', sleeves='long', hat='cap', hat_col=NAVY, build='stocky'), z=z, yaw=-20,
            arm_l='up', arm_r='hip', swing_l=8, rate=0.5, bob=3, detail=1)
        fig(sc, 150, -30, town_look(r, top=WHITE, hat='sun', hat_col=CREAM), z=z + 47, yaw=10, arm_r='point', arm_l='up', swing_r=18, swing_l=14,
            rate=1.6, detail=1, react=True)
        for k, (x, y, yw) in enumerate(((-260, -130, 120), (-170, -240, 150), (310, -180, -140))):      # the bidders
            fig(sc, x, y, town_look(r), z=z, yaw=yw, arm_r=('up', 'wave', 'up')[k], arm_l=('hip', 'down', 'up')[k], swing_r=(22, 26, 14)[k], rate=1.1,
                bob=(5, 3, 8)[k], phase=r.random(), detail=0)
        fig(sc, tx, ty + 80, town_look(r, top=TEAL, band=WHITE, hat='cap', hat_col=WHITE), z=z, arm_r='up', arm_l='hip',
            hold_r=hold_thing(SILVER, 14, 8, 60, 22), swing_r=14, rate=0.8, detail=1)
        fig(sc, tx - 60, ty - 130, town_look(r, wear='dress', hat='sun', hat_col=HOTPINK), z=z, yaw=180, arm_l='carry', arm_r='point', swing_r=8,
            axis_r='x', rate=0.6, hold_l=P.hold_bag(YEL), detail=0)
        fig(sc, 80, 30, beach_look(r, build='kid', s=0.66), z=z, yaw=200, arm_r='up', arm_l='down', swing_r=12, bob=9, rate=1.4)
        g.box(WOOD_D, (-250, -40, z + 60), (16, 16, 150))                     # a pelican on its post
        P.bird(sc, -250, -40, z + 152, heading=-90, span=160, flap=0.0, bob=4, rate=0.7, body=WHITE, wing=STEEL, beak=CORAL, s=1.4)
        P.bird(sc, tx + 250, ty, z + 520, heading=90, span=210, flap=30, rate=1.6, orbit=(tx, ty, 0.1))
        P.bird(sc, tx - 250, ty, z + 640, heading=270, span=210, flap=30, rate=1.5, phase=0.4, orbit=(tx, ty, 0.1))
        P.quadruped(sc, 250, -60, z=z, yaw=160, length=40, height=26, girth=13, neck=8, neck_up=40, head=13, tail=26, wag=40, rate=1.2,
                    body='#22202a', ears='#22202a')
    return build


def luau(seed=541):
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        z = LIFT
        for sy in (-1, 1):                                                   # the limbo bar, end on to the road
            g.box(WOOD_D, (0, sy * 135, z + 80), (12, 12, 190))
        g.box(YEL, (0, 0, z + 120), (8, 280, 8))
        fig(sc, -80, 0, beach_look(r, wear='shorts', hat=None), z=z, yaw=90, legs='wide', arm_l='out', arm_r='out',
            sway=dict(swing=17.0, pivot=(-80.0, 0.0, z), axis=90.0), rate=0.5, detail=1)
        d = dict(beach_look(r))
        d.update(CHEER[6])
        fig(sc, -270, 30, d, z=z, yaw=90, phase=0.2, detail=1)
        d = dict(beach_look(r))
        d.update(CHEER[1])
        fig(sc, -410, -30, d, z=z, yaw=90, phase=0.6, detail=0)
        fig(sc, 210, -40, beach_look(r), z=z, yaw=-90, arm_l='up', arm_r='up', swing_l=14, swing_r=-14, bob=11, rate=1.5, detail=1)
        for k in range(3):                                                   # the drummers
            x = -160 + k * 160
            g.box(WOOD, (x, 320, z + 20), (56, 56, 46))
            g.cyl((CORAL, TEAL, YEL)[k], (x, 262, z - 4), 24, 76, steps=8, top_r=31)
            g.cyl(WHITE, (x, 262, z + 72), 32, 5, steps=8)
            fig(sc, x, 320, beach_look(r, wear='shorts', hat='sun', hat_col=CREAM), z=z, legs='sit', arm_l='fwd', arm_r='fwd', swing_l=26, swing_r=-26,
                axis_l='x', axis_r='x', rate=2.4, phase=k / 3.0, detail=1)
        for k, (x, y) in enumerate(((340, 130), (480, 30))):                 # hula
            fig(sc, x, y, P.look(r, top=(HOTPINK, YEL)[k], bottom=LIME, wear='skirt', sleeves='none', hair='long', band=WHITE), z=z, yaw=-20,
                arm_l='out', arm_r='out', turn=28, rate=0.8, bob=4, phase=0.5 * k, detail=1)
        for k, (x, y) in enumerate(((-560, 200), (-470, 290), (-620, 60))):
            cheer(sc, r, x, y, z, beach_look(r), k=(5, 2, 7)[k])
        for sx in (-1, 1):                                                   # carved posts, a line of pennants between
            for k in range(6):
                g.box((WOOD_D, RED, WOOD_D, YEL, WOOD_D, TEAL)[k], (sx * 650, 380, z + 25 + k * 50), (44 - (k % 2) * 8, 44 - (k % 2) * 8, 50))
        bunting(sc, -650, 650, 380, z + 330, (HOTPINK, YEL, TEAL, WHITE), n=12, post=False)
        g.box(WOOD, (560, 280, z + 36), (150, 80, 72))                        # the table: a pineapple, cups
        g.cyl(YEL, (530, 280, z + 72), 18, 34, steps=6)
        g.cyl(MINT, (530, 280, z + 106), 14, 22, steps=6, top_r=2)
        for k in range(3):
            g.cyl((HOTPINK, WHITE, TEAL)[k], (575 + k * 20, 270, z + 72), 6, 15, steps=6)
    return build


def surfcontest(seed=551):
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        z = LIFT
        D = z + 230.0                                                        # the judges' scaffold
        g.box(WOOD, (0, 250, D - 8), (440, 220, 16))
        for sx in (-1, 1):
            for sy in (-1, 1):
                g.box(STEEL, (sx * 200, 250 + sy * 90, D / 2.0 - 30), (12, 12, D + 60))
            g.box(STEEL, (sx * 200, 340, D + 125), (8, 8, 250))
        for k in range(6):
            g.box((HOTPINK, WHITE)[k % 2], (-183 + k * 73, 138, D - 60), (73, 4, 100))
        g.box(YEL, (0, 250, D + 252), (480, 270, 12), rot=(0, 0, -6))
        g.box(WHITE, (0, 170, D + 36), (400, 30, 72))
        for k in range(3):
            fig(sc, -130 + k * 130, 240, town_look(r, hat='sun', hat_col=(CREAM, YEL, WHITE)[k]), z=D, legs='sit', arm_r='up', arm_l='down',
                hold_r=hold_thing((HOTPINK, TEAL, LIME)[k], 46, 4, 46, 34), swing_r=16, rate=0.7, phase=k / 3.0, detail=1)
        fig(sc, 330, 80, town_look(r, top=WHITE, hat='cap', hat_col=HOTPINK), z=z, yaw=-25, arm_r='face', arm_l='up', hold_r=hold_cone(YEL, NAVY),
            swing_l=16, rate=1.2, detail=1)
        for k, (x, h) in enumerate(((0, 90), (-116, 60), (116, 40))):        # the podium and the winners
            g.box(WHITE, (x, -120, z + h / 2.0 - 10), (112, 96, h + 20))
            g.box((YEL, STEEL, CORAL)[k], (x, -170, z + h - 20), (112, 4, 22))
            lk = beach_look(r, wear='swim' if k else 'shorts', sleeves='none', hat=None)
            if k == 0:
                fig(sc, x, -120, lk, z=z + h, arm_l='up', arm_r='up', hold_r=hold_thing(YEL, 26, 26, 40, 18), swing_l=12, swing_r=-12, bob=6, rate=1.5,
                    detail=1, react=True)
            else:
                fig(sc, x, -120, lk, z=z + h, arm_l='down', arm_r='wave', hold_l=P.hold_board((HOTPINK, MINT)[k - 1], WHITE, 190), swing_r=26, rate=1.2,
                    phase=0.3 * k, detail=1)
        for k in range(5):                                                   # boards stood in the sand
            g.box((YEL, HOTPINK, MINT, SKY, CORAL)[k], (-620 + k * 62, 170, z + 100), (50, 9, 230), rot=(0, 0, 7))
        fig(sc, -260, -210, town_look(r, top=NAVY), z=z, yaw=125, arm_l='hold', arm_r='hold', hold_r=P.hold_camera(), turn=8, rate=0.4, detail=0)
        fig(sc, 270, -230, town_look(r, top=WHITE), z=z, yaw=-125, arm_l='hold', arm_r='hold', hold_r=P.hold_camera(), turn=8, rate=0.4, phase=0.5, detail=0)
        for k, (x, y) in enumerate(((480, -120), (580, -30), (520, 110), (640, 180))):
            cheer(sc, r, x, y, z, beach_look(r, **(dict(build='kid', s=0.66) if k == 1 else {})), yaw=-35)
        bunting(sc, -650, -250, 330, z + 300, (HOTPINK, YEL, TEAL, WHITE), n=6)
    return build


def yoga(seed=561):
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        z = LIFT
        T = z + 30.0
        g.box(WOOD_D, (0, 0, T - 8 - FOOT / 2.0), (1220, 700, FOOT))
        g.box(WOOD, (0, 0, T - 8), (1260, 740, 16))
        n = 0
        for row, cnt in enumerate((4, 3, 3)):
            for k in range(cnt):
                x = (k - (cnt - 1) / 2.0) * 270 + r.uniform(-10, 10)
                y = -60 + row * 170
                g.box(HOT[(n * 3) % len(HOT)], (x, y, T + 2), (70, 170, 4))
                fig(sc, x, y, P.look(r, tops=HOT, bottoms=SWIM, wear=r.choice(('shorts', 'trousers')), sleeves='none',
                                     hair=r.choice(('bun', 'pony', 'short', 'afro'))),
                    z=T, arm_l='vee', arm_r='vee', swing_l=34, swing_r=-34, rate=0.25, phase=0.35 if n == 5 else 0.0, detail=0)
                n += 1
        g.box(TEAL, (0, -280, T + 15), (130, 130, 30))                        # the teacher, facing her class
        fig(sc, 0, -280, P.look(r, top=WHITE, bottom=HOTPINK, wear='trousers', sleeves='none', hair='bun'), z=T + 30, yaw=180, arm_l='vee', arm_r='vee',
            swing_l=34, swing_r=-34, rate=0.25, detail=1)
        lying(sc, r, 540, 290, T, 20.0, LAV)
        P.quadruped(sc, -560, -250, z=T, yaw=30, length=70, height=48, girth=20, neck=16, neck_up=35, head=18, tail=22, wag=30, rate=1.8,
                    body=WHITE, ears='#5a4a3c', snout='#22202a')
        parasol(g, 560, 120, T, YEL)
        g.box(SKY, (500, -300, T + 30), (50, 50, 60))                         # the water
        for sx in (-1, 1):                                                   # feather flags at the front corners
            g.box(WHITE, (sx * 610, -350, T + 150), (6, 6, 300))
            fl = sc.part(P.motion(swing=16.0, pivot=(sx * 610.0, -350.0, T + 200.0), axis=P.UP, rate=0.7, phase=0.4 * sx))
            fl.box((HOTPINK, TEAL)[sx > 0], (sx * 610 + 22, -350, T + 190), (38, 4, 210))
    return build


def tugofwar(seed=571):
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        z = LIFT
        slide = dict(swing=1.4, pivot=(0.0, 0.0, -3000.0), axis=90.0)        # 76 cm each way along the rope
        rope = sc.part(P.motion(rate=0.3, **slide))
        rope.box(WOOD, (0, 0, z + 100), (940, 6, 6))
        rope.box(RED, (0, 0, z + 78), (20, 4, 40))
        g.box(WHITE, (0, 0, z + 2), (12, 320, 4))
        for sy in (-1, 1):
            g.cyl(CORAL, (0, sy * 170, z), 18, 46, steps=6, top_r=4)
        for team, (d, col) in enumerate(((-1, HOTPINK), (1, TEAL))):
            for k in range(4):
                x = d * (150 + k * 92)
                fig(sc, x, (k % 2) * 30 - 15, P.look(r, top=col, bottom=(WHITE, NAVY)[team], wear='shorts', sleeves='short',
                                                     build=r.choice(('avg', 'stocky', 'slim'))),
                    z=z, yaw=-90.0 * d, legs='stride', arm_l='fwd', arm_r='fwd', sway=slide, rate=0.3, detail=1)
        fig(sc, 0, 190, town_look(r, top=WHITE, bottom=NAVY, wear='trousers', hat='cap', hat_col=NAVY, band=NAVY), z=z, arm_l='out', arm_r='out',
            hold_l=P.hold_flag(HOTPINK, WHITE, 50), hold_r=P.hold_flag(TEAL, WHITE, 50), swing_l=22, swing_r=-22, bob=6, rate=1.2, detail=1)
        for k, (x, y, col) in enumerate(((-560, 220, HOTPINK), (-440, 290, HOTPINK), (-300, 250, HOTPINK), (330, 260, TEAL), (470, 300, TEAL),
                                         (590, 210, TEAL))):
            cheer(sc, r, x, y, z, beach_look(r, top=col, **(dict(build='kid', s=0.66) if k in (2, 3) else {})), k=(0, 1, 3, 7, 0, 1)[k], react=(k % 2 == 0))
        g.box(WHITE, (0, 340, z + 36), (90, 70, 72))                          # the cup on its table
        g.cyl(YEL, (0, 340, z + 72), 16, 40, steps=6, top_r=24)
        bunting(sc, -650, 650, 390, z + 300, (HOTPINK, WHITE, TEAL, WHITE), n=12)
    return build


# ------------------------------------------------------------------------------------------ the town
ST_W, ST_D = 1240.0, 560.0


def carnival(seed=581, variant=0):
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        H = 80.0
        stage(g, ST_W, ST_D, H, CREAM, (TEAL, CORAL)[variant])
        F = H + 100.0                                                        # the float's deck
        body = (HOTPINK, TEAL)[variant]
        g.box(body, (0, 130, H + 50), (920, 260, 100))
        for k in range(10):
            g.box((YEL, WHITE)[k % 2], (-414 + k * 92, -2, H + 24), (92, 4, 36))
        for sx in (-1, 1):
            g.cyl(NAVY, (sx * 300, -4, H + 30), 30, 8, rot=(0, 0, 90), steps=8)
        if variant == 0:
            for k in range(-3, 4):                                           # the shell and its pearl
                a = math.radians(k * 24.0)
                tilt_box(g, (LAV, WHITE)[k % 2], (0, 245, F), (math.sin(a) * 330, 245, F + math.cos(a) * 330), 92, 14)
            g.sphere(WHITE, (0, 215, F + 50), 46, steps=(4, 8))
            for k in range(3):
                fig(sc, -280 + k * 280, 110, P.look(r, top=(YEL, HOTPINK, LIME)[k], bottom=(HOTPINK, YEL, HOTPINK)[k], wear='skirt', sleeves='none',
                                                    hat='sun', hat_col=(HOTPINK, YEL, TEAL)[k]),
                    z=F, arm_l='vee', arm_r='vee', hold_l=hold_fan((HOTPINK, YEL, TEAL)[k], WHITE), hold_r=hold_fan((HOTPINK, YEL, TEAL)[k], WHITE),
                    turn=30, bob=8, rate=1.1, phase=k / 3.0, detail=1)
            for sx in (-1, 1):
                fig(sc, sx * 410, 150, town_look(r, top=WHITE, hat='sun', hat_col=YEL), z=F, arm_l='carry', arm_r='carry', hold_r=P.hold_box(RED, 44),
                    swing_l=18, swing_r=-18, axis_l='x', axis_r='x', rate=2.2, phase=0.25 * sx, detail=1)
        else:
            g.cyl(YEL, (0, 250, F + 240), 190, 14, rot=(0, 0, 90), steps=10)              # the sun
            for k in range(8):
                g.box(CORAL, (0, 258, F + 240), (560, 10, 36), rot=(k * 22.5, 0, 0))
            for k in range(4):                                               # the pans
                x = -300 + k * 200
                g.box(NAVY, (x, 70, F + 40), (8, 8, 80))
                g.cyl(STEEL, (x, 70, F + 78), 36, 22, steps=8)
                fig(sc, x, 140, town_look(r, top=(YEL, HOTPINK, LIME, WHITE)[k], hat='sun', hat_col=CREAM), z=F, arm_l='fwd', arm_r='fwd', swing_l=22,
                    swing_r=-22, axis_l='x', axis_r='x', rate=2.6, phase=k / 4.0, bob=3, detail=1)
        sx0 = -440                                                           # on the stage before it: the stilt walker
        for sx in (-1, 1):
            g.box((RED, WHITE)[sx > 0], (sx0 + sx * 11, -150, H + 85), (16, 16, 170))
        fig(sc, sx0, -150, P.look(r, top=YEL, bottom=RED, wear='shorts', sleeves='long', hat='chef', hat_col=RED, s=1.0), z=H + 150, arm_l='out',
            arm_r='out', swing_l=20, swing_r=-20, rate=0.9, detail=1)
        fig(sc, 440, -150, town_look(r, top=(LIME, YEL)[variant]), z=H, arm_r='up', arm_l='hip', hold_r=P.hold_flag((YEL, HOTPINK)[variant], WHITE, 120),
            swing_r=26, rate=0.8, bob=4, detail=1)
        for k in range(3):
            x = -210 + k * 210
            if variant == 1 and k == 1:                                      # the singer
                fig(sc, x, -170, P.look(r, top=HOTPINK, wear='dress', hair='afro', sleeves='none'), z=H, arm_r='face', arm_l='up',
                    hold_r=hold_thing(NAVY, 8, 8, 20, 10), swing_l=18, rock=0, rate=1.0, bob=5, detail=1)
            else:
                fig(sc, x, -170, beach_look(r, wear='skirt', sleeves='none', hat='sun'), z=H, yaw=r.uniform(-25, 25), arm_l='wave', arm_r='wave',
                    hold_l=hold_thing(RED, 12, 12, 26, 14), hold_r=hold_thing(YEL, 12, 12, 26, 14), turn=24, bob=9, rate=1.2, phase=r.random(),
                    detail=0)
        for ph in (0.0, 0.5):                                                # confetti
            cf = sc.part(P.motion(bob=-30.0, rate=0.4, phase=ph))
            for k in range(7):
                cf.box(HOT[(k * 3 + int(ph * 2)) % len(HOT)], (r.uniform(-520, 520), r.uniform(-200, 120), H + r.uniform(300, 430)), (14, 14, 4))
        for sx in (-1, 1):                                                   # balloons at the float's ends
            bl = sc.part(P.motion(bob=-9.0, rate=0.5, phase=0.3 * sx))
            bl.box(WHITE, (sx * 452, 240, F + 110), (4, 4, 220))
            for k in range(3):
                bl.sphere((RED, YEL, SKY)[(k + (sx > 0)) % 3], (sx * 452 + (k - 1) * 34, 240, F + 240 + (k % 2) * 40), 28, steps=(4, 6))
    return build


def streetshow(seed=591):
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        H = 80.0
        stage(g, ST_W, ST_D, H, CREAM, CORAL)
        fig(sc, -440, -60, P.look(r, top=YEL, bottom=RED, wear='trousers', sleeves='long', hat='chef', hat_col=RED, band=RED), z=H, yaw=10,
            arm_l='carry', arm_r='carry', swing_l=16, swing_r=-16, axis_l='x', axis_r='x', rate=1.2, detail=1)       # the juggler
        for k in range(3):
            b = sc.part(P.motion(bob=100.0, rate=0.6, phase=k / 3.0))
            b.sphere((HOTPINK, TEAL, YEL)[k], (-440 + (k - 1) * 30, -90, H + 150), 13, steps=(3, 6))
        acro = dict(top=TEAL, bottom=TEAL, wear='trousers', sleeves='none', hat=None)
        for sx in (-1, 1):                                                   # the acrobats' tower
            fig(sc, sx * 58, 40, P.look(r, build='stocky', s=1.0, **acro), z=H, legs='wide', arm_l='up', arm_r='up', detail=1)
        fig(sc, 0, 40, P.look(r, build='slim', s=0.9, **acro), z=H + 142, arm_l='vee', arm_r='vee', rock=4, rate=0.6, detail=1)
        g.box(STEEL, (260, 160, H + 30), (84, 84, 60))                        # the living statue, gold from head to foot
        fig(sc, 260, 160, dict(skin=GOLD, top=GOLD, bottom=GOLD, shoes=GOLD, hair_col=GOLD, hair='short', hat='sun', hat_col=GOLD, wear='coat',
                               sleeves='long', s=1.0),
            z=H + 60, arm_r='up', arm_l='hip', turn=14, rate=0.12, detail=1)
        g.box(WOOD, (440, -40, H + 23), (40, 40, 46))                         # the portrait artist, the easel, the sitter
        fig(sc, 440, -40, town_look(r, top=WHITE, hat='sun', hat_col=NAVY), z=H, yaw=90, legs='sit', arm_r='fwd', arm_l='down', swing_r=12, axis_r='z',
            rate=1.4, detail=1)
        g.box(WOOD_D, (548, -40, H + 60), (8, 70, 120))
        g.box(WHITE, (540, -40, H + 150), (8, 96, 116))
        g.sphere(CORAL, (533, -40, H + 165), 26, scale=(0.2, 1.0, 1.0), steps=(3, 6))
        g.box(NAVY, (534, -40, H + 120), (4, 50, 30))
        g.box(WOOD, (350, -180, H + 23), (40, 40, 46))
        fig(sc, 350, -180, town_look(r, wear='dress', hat='sun', hat_col=HOTPINK), z=H, yaw=70, legs='sit', arm_l='hip', arm_r='down', rate=0.3,
            bob=-1.0, detail=1)
        fig(sc, -250, 190, town_look(r, top=WHITE, hat='cap', hat_col=RED), z=H, arm_r='up', arm_l='hip', swing_r=6, rate=0.5, detail=1)   # balloons
        bl = sc.part(P.motion(bob=-10.0, rate=0.4))
        bl.box(WHITE, (-225, 190, H + 255), (4, 4, 150))
        for k in range(6):
            bl.sphere(HOT[(k * 2) % len(HOT)], (-225 + ((k % 3) - 1) * 46, 190 + (k // 3) * 20 - 10, H + 340 + (k % 2) * 46 + (k // 3) * 30), 30, steps=(4, 6))
        d = dict(town_look(r))
        d.update(CHEER[6])
        fig(sc, -570, -200, d, z=H, yaw=60, phase=0.3, detail=0)
        d = dict(town_look(r))
        d.update(CHEER[0])
        fig(sc, 570, -220, d, z=H, yaw=-20, phase=0.7, detail=0, react=True)
        lk = town_look(r, hat=None, s=1.0)
        fig(sc, 170, -200, lk, z=H, yaw=150, arm_l='up', arm_r='up', detail=1)                                        # a child on shoulders
        fig(sc, 170, -200, town_look(r, build='kid', s=0.66, hat='cap', hat_col=YEL), z=H + 96, yaw=150, legs='sit', arm_l='vee', arm_r='vee',
            swing_l=14, swing_r=-14, rate=1.5, detail=1)
        g.cyl(NAVY, (-360, -180, H), 26, 8, steps=8)                          # the hat for coins
        g.cyl(NAVY, (-360, -180, H), 16, 26, steps=8)
        bunting(sc, -600, 600, 262, H + 330, (HOTPINK, YEL, TEAL, WHITE), n=12)
    return build


def mural(seed=601):
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        H = 60.0
        BLUE = '#3a5fa8'
        stage(g, ST_W, ST_D, H, CREAM, LAV)
        g.box(CREAM, (0, 250, H + 210), (1180, 30, 420))                      # the wall; two thirds painted
        g.box(SKY, (-200, 232, H + 300), (760, 5, 230))
        g.box(BLUE, (-200, 231, H + 120), (760, 6, 130))
        g.cyl(YEL, (-380, 230, H + 310), 84, 6, rot=(0, 0, 90), steps=10)
        for k in range(4):
            g.box(WHITE, (-500 + k * 200, 227, H + 150 + (k % 2) * 26), (130, 5, 16), rot=((-10, 10)[k % 2], 0, 0))
        tilt_box(g, WOOD_D, (60, 228, H + 60), (90, 228, H + 300), 22, 5)     # a palm
        for k in range(4):
            g.box(MINT, (90, 226, H + 306), (190, 5, 34), rot=(-50 + k * 34, 0, 0))
        g.box(HOTPINK, (-140, 227, H + 250), (90, 5, 90), rot=(45, 0, 0))     # a sail
        for k in range(3):                                                   # the part still to do: the drawing
            g.box(NAVY, (390 + k * 60, 233, H + 120 + k * 90), (150, 4, 6), rot=(20 - k * 20, 0, 0))
        PL = H + 190.0                                                       # the scaffold
        g.box(WOOD, (140, 160, PL), (660, 76, 10))
        for x in (-180, 460):
            for y in (126, 194):
                g.box(STEEL, (x, y, H + 200), (10, 10, 400))
        g.box(STEEL, (140, 194, PL + 100), (660, 6, 6))
        for sy in (-1, 1):                                                   # the ladder
            tilt_box(g, WOOD_D, (540 + sy * 20, 60, H), (540 + sy * 20, 150, PL), 6, 6)
        for k in range(4):
            g.box(WOOD_D, (540, 72 + k * 20, H + 30 + k * 42), (46, 5, 5))

        def roller(col):
            def f(m, h, side):
                m.box(WOOD, (h[0], h[1], h[2] + 22), (5, 5, 44))
                m.box(col, (h[0], h[1] - 4, h[2] + 48), (34, 10, 10))
            return f

        over = dict(top=WHITE, bottom=WHITE, wear='trousers', sleeves='short', hat='cap')
        for k, x in enumerate((-40, 250)):
            fig(sc, x, 150, P.look(r, hat_col=(HOTPINK, TEAL)[k], band=(HOTPINK, TEAL)[k], **over), z=PL + 5, yaw=180, arm_r='up', arm_l='hip',
                hold_r=roller((HOTPINK, SKY)[k]), swing_r=26, axis_r='x', rate=0.7, phase=0.5 * k, detail=1)
        g.box(WHITE, (-420, 20, H + 2), (280, 220, 4))                        # the drop cloth, the tins
        for k in range(4):
            g.cyl((HOTPINK, YEL, TEAL, BLUE)[k], (-520 + k * 62, 60 - (k % 2) * 80, H + 4), 22, 34, steps=6)
            g.box((HOTPINK, YEL, TEAL, BLUE)[k], (-500 + k * 62, 10 - (k % 2) * 40, H + 5), (40, 30, 3))
        fig(sc, -400, 0, P.look(r, hat_col=YEL, band=YEL, **over), z=H, yaw=30, arm_l='push', arm_r='push', swing_l=14, swing_r=14, axis_l='x',
            axis_r='x', rate=1.0, hold_r=P.hold_tool(WOOD, YEL, 50), detail=1)
        fig(sc, 440, 20, town_look(r, top=NAVY, hat='cap', hat_col=LIME), z=H, yaw=150, arm_r='fwd', arm_l='hip', swing_r=20, axis_r='z', rate=0.9,
            hold_r=hold_thing(LIME, 10, 10, 22, 6), detail=1)
        fig(sc, -140, -170, town_look(r, top=HOTPINK, wear='dress', hat='sun', hat_col=WHITE), z=H, yaw=165, arm_l='hold', arm_r='up',
            hold_l=hold_thing(WHITE, 40, 4, 30, 10), swing_r=9, rate=0.5, detail=1)
        fig(sc, -540, 190, town_look(r, build='kid', s=0.66, top=YEL, hat=None), z=H, yaw=180, arm_r='fwd', arm_l='down', swing_r=24, axis_r='x',
            rate=1.3, hold_r=roller(YEL))
        cheer(sc, r, 250, -200, H, town_look(r), k=5)
        cheer(sc, r, 570, -190, H, town_look(r), k=3)
        P.quadruped(sc, 90, -210, z=H, yaw=170, length=70, height=48, girth=20, neck=16, neck_up=35, head=18, tail=22, wag=30, rate=2.0,
                    body='#5a4a3c', ears='#22202a', snout='#22202a')
    return build


WIDE = dict(grow=(120.0, 80.0, 80.0), grow_down=(120.0, 80.0, 0.0))
JOBS = [
    ('SM_P3B_Wedding', wedding(501)),
    ('SM_P3B_SandCastle', sandcastle(511), WIDE),
    ('SM_P3B_SurfBoat', surfboat(521)),
    ('SM_P3B_FishMarket', fishmarket(531), WIDE),
    ('SM_P3B_Luau', luau(541)),
    ('SM_P3B_SurfContest', surfcontest(551)),
    ('SM_P3B_Yoga', yoga(561)),
    ('SM_P3B_TugOfWar', tugofwar(571), WIDE),
    ('SM_P3B_Carnival_A', carnival(581, 0)),
    ('SM_P3B_Carnival_B', carnival(582, 1)),
    ('SM_P3B_StreetShow', streetshow(591)),
    ('SM_P3B_Mural', mural(601)),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths(only=None):
    return [DIR + '/' + j[0] for j in JOBS if not only or j[0] in only]
