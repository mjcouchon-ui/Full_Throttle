"""FULL THROTTLE (Unreal) the population of the THEME PARK, pass 3, builder p3a: THE GAPS (/Game/People/ThemePark/SM_P3A_...).

The general cast of the park (people at work, at rest, passing through) for the places the first table left without
anybody: the free slots of the verge, the hills where the verge has no furniture at all, the slots whose only tenant
is a ride of the dressing's (a pirate dock, a funhouse court), the band behind the parade lane, and the lagoon's rails.

    LOW, for the verge (top under 2 m as built, NO glow, figures 0.97 of life size at most, hops of 8 cm at most):
      SM_P3A_Kerb_A     guests at a low rail on a riser: cheering, waving, filming, a child with a flag
      SM_P3A_Kerb_B     the staff at work: a sweeper, one watering the planters, a litter picker, one pushing a bin cart
                        along the piece, one waving the chase by
      SM_P3A_Kerb_C     at rest: two benches of guests with drinks, a parent rocking a pram, a dog, a child
      SM_P3A_Kerb_D     passing through: a family with a pram one way, staff with a box and two friends the other,
                        behind a low hedge (they walk the piece's length and come round again)
    TALL (24 m or more from the centre line; lit like the rest of the park's cast, a steady glow, nothing blinks):
      SM_P3A_Terrace    a cafe terrace on posts 3.3 m up: guests at tables under parasols, three at the rail cheering,
                        a waiter walking the deck with a tray
      SM_P3A_Painters   the staff repainting a show panel from a scaffold of two decks: rollers and brushes going, a
                        bucket hauled up and down, one waving from the scaffold
      SM_P3A_Train      the park's miniature railway on a trestle 2.5 m up: an engine and two open wagons of guests
                        run from shed to shed and come round again; two more wait on the platform
      SM_P3A_Photo      the photo spot: a family jumping together on a round podium before a gold frame, the
                        photographer on a step stool, staff directing, the next group on the steps
      SM_P3A_Prizes     the prize delivery: a lift truck raising a pallet of giant plush animals to the staff on a
                        store's roof, a marshal guiding it, a child jumping at the sight
      SM_P3A_RailGang   for the lagoon: a maintenance tower 25 m high (it stands on the valley floor, its deck at the
                        height of the coaster's rails): the rail gang in hard hats, a flagman, a jib with a spare bogie

No clown and no balloon anywhere (the boss is MR. CHUCKLES, who bombs the road with balloons). No words, no marks.

Run in the editor:  import ft_people_park_p3a; ft_people_park_p3a.run()
The placing rules are this builder's block, C:/ftwork/p3/L4a.inl (the integrator pastes it into FTPeopleL4.inl).
"""
import random
import unreal
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/People/ThemePark'
NAVY, PURPLE, PLUM = '#2b2350', '#3a2f6a', '#4a3a78'
MAG, CYAN, AMBER, CREAM = '#ff4fb8', '#2ee6ff', '#ffd23f', '#fff1d6'
RED, WHITE, LAV, MINT, TEAL = '#ff2d3c', '#fff6ea', '#b9a4ff', '#7fe3c4', '#2fc6c0'
ORANGE, GOLD, WOOD, WOOD_D, GREY_L, DARK = '#ff8a2a', '#ffc93c', '#9a6a44', '#6a4c36', '#9a98a8', '#22202a'
HEDGE, LIME, SAND = '#3f9f5a', '#b8ff5a', '#e8d8b8'
BRIGHT = (MAG, CYAN, AMBER, RED, ORANGE, '#8f6bff', LIME, WHITE)
Z0 = 14.0          # the riser every low piece stands on


# ------------------------------------------------------------------------------------------ helpers
def staff(r, **fixed):
    """The park's staff, as the first kit dressed them: mint shirt, navy trousers, pink cap, a gold sash."""
    d = P.look(r, top=MINT, bottom=NAVY, hat='cap', hat_col=MAG, band=GOLD, sleeves='short', wear='trousers', shoes=WHITE)
    d.update(fixed)
    return d


def guest(r, **fixed):
    d = P.look(r, tops=BRIGHT)
    d.update(fixed)
    return d


def kid(r, **fixed):
    d = P.look(r, tops=BRIGHT, build='kid', s=r.uniform(0.6, 0.7))
    d.update(fixed)
    return d


def low(d):
    """For the verge: nobody over 0.97 of life size and no tall hair, so that the piece stays under 2 m as built."""
    d['s'] = min(d.get('s', 1.0), 0.97)
    if d.get('hair') in ('afro', 'bun', 'mohawk'):
        d['hair'] = 'short'
    if d.get('hat') not in (None, 'cap'):
        d['hat'] = 'cap'
    return d


def together(sc, base, fn):
    """Everything fn() draws that travels takes ONE place on the line, so a train and its riders (or a parent and a
    pram) come round as one thing instead of each wrapping where it stands."""
    before = set(sc.parts)
    out = fn()
    for k in sc.order:
        if k not in before:
            mo = sc.parts[k][1]
            if mo['travel'][0] > 0.5:
                mo['base'] = (float(base[0]), float(base[1]))
    return out


def lift(sc, g):
    """A night level: every colour of a TALL piece gets a steady glow of its own (as the park's first kit does), like
    a figure under stage light. Only for pieces that stand 24 m or more from the centre line; nothing blinks."""
    for key in sc.order:
        model = sc.parts[key][0]
        out = {}
        for (col, glow), pm in model.meshes.items():
            k2 = (col, max(glow, g))
            while k2 in out:
                k2 = (col, k2[1] + 0.0002)
            out[k2] = pm
        model.meshes = out


def lit(fn, g=0.09):
    def build(sc):
        fn(sc)
        lift(sc, g)
    build.__doc__ = fn.__doc__
    return build


def bulbs(m, a, b, n, size=13.0, glow=1.3, col='#ffe28a'):
    """A row of steady lamps from a to b (tall pieces only)."""
    for k in range(n):
        f = (k + 0.5) / n
        m.box(col, (a[0] + (b[0] - a[0]) * f, a[1] + (b[1] - a[1]) * f, a[2] + (b[2] - a[2]) * f), (size, size, size), glow=glow)


def stripes(m, x0, x1, y, z0, z1, cols, n, depth=6.0):
    w = (x1 - x0) / float(n)
    for k in range(n):
        m.box(cols[k % len(cols)], (x0 + (k + 0.5) * w, y, (z0 + z1) / 2.0), (w, depth, z1 - z0))


def tv_of(length, speed):
    return (float(length), float(speed) / float(length))


def riser(g, L, W, col=CREAM, trim=MAG):
    g.box(col, (0, 0, Z0 / 2.0), (L, W, Z0))
    g.box(trim, (0, -W / 2.0 - 2.0, Z0 / 2.0), (L, 4, Z0))


def rail(g, L, y, z0, h=92.0, posts=5, col=MAG, post=PLUM):
    for k in range(posts):
        g.box(post, (-L / 2.0 + 10 + k * (L - 20.0) / (posts - 1), y, z0 + h / 2.0), (8, 8, h))
    g.box(col, (0, y, z0 + h + 2), (L, 7, 7))
    g.box(CREAM, (0, y, z0 + h * 0.55), (L, 5, 5))


def legs(g, xs, ys, H, col=PLUM, band=MAG, w=22.0, deep=0.0):
    for x in xs:
        for y in ys:
            g.box(col, (x, y, (H - deep) / 2.0), (w, w, H + deep))
            g.box(band, (x, y, H * 0.55), (w + 6, w + 6, 40))


def plush(m, x, y, z, col, s=1.0):
    """A giant plush animal (a round body, a round head, two ears, a cream muzzle): a prize, about a metre high."""
    m.sphere(col, (x, y, z + 34 * s), 34 * s, steps=(3, 6))
    m.sphere(col, (x, y, z + 80 * s), 25 * s, steps=(3, 6))
    for sx in (-1, 1):
        m.box(col, (x + sx * 18 * s, y, z + 104 * s), (13 * s, 8 * s, 13 * s))
    m.box(CREAM, (x, y - 22 * s, z + 76 * s), (17 * s, 9 * s, 13 * s))


def hold_broom(col=WOOD, head=MAG):
    def f(m, h, side):
        if side > 0:
            tilt_box(m, col, (h[0] - 10, h[1] + 8, h[2] + 38), (h[0] - 10, h[1] - 42, h[2] - 74), 5, 5)
            m.box(head, (h[0] - 10, h[1] - 46, h[2] - 78), (44, 14, 12))
    return f


def hold_can(col=CYAN):
    def f(m, h, side):
        m.box(col, (h[0], h[1] - 8, h[2] - 8), (20, 28, 22))
        tilt_box(m, col, (h[0], h[1] - 20, h[2] - 8), (h[0], h[1] - 46, h[2] + 4), 6, 6)
    return f


def hold_grabber(col=GREY_L):
    def f(m, h, side):
        tilt_box(m, col, (h[0], h[1], h[2]), (h[0], h[1] - 50, h[2] - 66), 5, 5)
        m.box(RED, (h[0], h[1] - 52, h[2] - 68), (12, 8, 8))
    return f


def hold_tray(col=WHITE):
    def f(m, h, side):
        if side > 0:
            m.box(col, (h[0] - 22, h[1] - 6, h[2] + 10), (50, 34, 4))
            m.box(MAG, (h[0] - 32, h[1] - 6, h[2] + 20), (10, 10, 16))
            m.box(CYAN, (h[0] - 12, h[1] - 6, h[2] + 20), (10, 10, 16))
    return f


def hold_roller(col=MAG):
    """A paint roller on a pole, held up at the wall."""
    def f(m, h, side):
        m.box(CREAM, (h[0], h[1], h[2] + 34), (5, 5, 68))
        m.box(col, (h[0], h[1] - 4, h[2] + 72), (34, 12, 12))
    return f


def hold_brush(col=CYAN):
    def f(m, h, side):
        m.box(WOOD, (h[0], h[1] - 12, h[2] + 4), (6, 26, 6))
        m.box(col, (h[0], h[1] - 30, h[2] + 4), (12, 12, 16))
    return f


def hold_plush(col=CYAN):
    """A plush prize held over the head in both hands (drawn once, from the right hand)."""
    def f(m, h, side):
        if side > 0:
            plush(m, 0.0, h[1], h[2] + 6, col, 0.9)
    return f


# ------------------------------------------------------------------------------------------ the verge (LOW, unlit)
def kerb_a(sc):
    """Guests at a low rail on a riser, eight metres of them: cheering, waving, pointing, one filming the chase, two
    children (one with a flag). They liven up as the chase comes by."""
    r = random.Random(301)
    g = sc.still()
    riser(g, 840, 190)
    rail(g, 840, -84, Z0)
    stripes(g, -420, 420, -90, Z0 + 4, Z0 + 44, (MAG, CREAM, CYAN, CREAM), 8, depth=4)
    acts = (dict(arm_l='up', arm_r='up', swing_l=14, swing_r=-14, bob=8, rate=1.6),
            dict(arm_r='wave', swing_r=28, arm_l='hip', bob=4, rate=1.3),
            dict(arm_r='wave', hold_r=P.hold_flag(col=CYAN, w=50), swing_r=22, bob=8, rate=1.8),
            dict(arm_l='hold', arm_r='hold', hold_r=P.hold_phone(), turn=12, rate=0.4),
            dict(arm_l='vee', arm_r='vee', swing_l=10, swing_r=-10, bob=8, rate=1.4),
            dict(arm_l='up', arm_r='up', swing_l=12, swing_r=-12, bob=8, rate=2.0),
            dict(arm_r='point', arm_l='face', swing_r=10, axis_r='x', rate=0.7))
    for i, x in enumerate((-350, -235, -120, 0, 115, 235, 350)):
        d = low(kid(r) if i in (2, 5) else guest(r))
        d.update(acts[i])
        P.figure(sc, x + r.uniform(-10, 10), -36 + r.uniform(-8, 8), z=Z0, yaw=r.uniform(-14, 14), phase=r.random(), react=True, detail=1, **d)


def kerb_b(sc):
    """The staff at work on the verge: a sweeper, one watering the planters, one waving the chase by, a litter picker
    with a sack, and one pushing a bin cart along the back of the piece (it comes round again)."""
    r = random.Random(303)
    g = sc.still()
    riser(g, 900, 190, col=SAND, trim=CYAN)
    for k, px in enumerate((-300, 0, 300)):
        g.box(WOOD_D, (px, -74, Z0 + 18), (220, 40, 36))
        g.box(HEDGE, (px, -74, Z0 + 42), (210, 34, 14))
        for j in range(4):
            g.box((MAG, AMBER, WHITE, CYAN)[(j + k) % 4], (px - 78 + j * 52, -74, Z0 + 56), (30, 26, 16))
    P.figure(sc, -335, -8, z=Z0, yaw=24, arm_l='carry', arm_r='carry', hold_r=hold_broom(), swing_l=12, swing_r=12, axis_l='x', axis_r='x',
             rate=1.0, phase=r.random(), detail=1, **low(staff(r)))
    P.figure(sc, -150, -22, z=Z0, yaw=-8, arm_r='fwd', arm_l='hip', hold_r=hold_can(), swing_r=8, axis_r='x', rate=0.5, phase=r.random(),
             detail=1, **low(staff(r)))
    P.figure(sc, 60, -26, z=Z0, yaw=6, arm_l='up', arm_r='up', swing_l=14, swing_r=-14, bob=6, rate=1.5, phase=r.random(), react=True,
             detail=1, **low(staff(r)))
    P.figure(sc, 330, -14, z=Z0, yaw=-20, arm_r='fwd', arm_l='down', hold_r=hold_grabber(), hold_l=P.hold_bag(col=AMBER), swing_r=14, axis_r='x',
             rate=0.8, phase=r.random(), detail=1, **low(staff(r)))
    TR = (820.0, 55.0)

    def cart():
        c = sc.part(P.motion(travel=tv_of(*TR), base=(0.0, 52.0), pivot=(74.0, 52.0, Z0 + 50)))
        c.box(MINT, (74, 52, Z0 + 56), (84, 56, 76))
        c.box(MAG, (74, 52, Z0 + 98), (90, 62, 8))
        for sx in (-1, 1):
            c.box(DARK, (74 + sx * 28, 52, Z0 + 10), (20, 62, 20))
        c.box(GREY_L, (28, 52, Z0 + 92), (8, 50, 6))
        P.figure(sc, 0, 52, z=Z0, yaw=90, arm_l='push', arm_r='push', stride=22, rate=1.0, phase=0.3, travel=TR, detail=1, **low(staff(r)))
    together(sc, (0.0, 52.0), cart)


def kerb_c(sc):
    """At rest on the verge: two benches of guests with drinks (one waves, one films), a parent rocking a pram to and
    fro, a dog wagging its tail, a child hopping beside it."""
    r = random.Random(307)
    g = sc.still()
    riser(g, 900, 200, col=CREAM, trim=AMBER)
    rail(g, 900, -92, Z0)          # review of pass 3: a rail between these people and the road, like Kerb_A's
    seats = ((-270, (-340, -200)), (170, (100, 240)))
    for bx, _ in seats:
        g.box(WOOD, (bx, 34, Z0 + 42), (290, 52, 8))
        g.box(WOOD, (bx, 62, Z0 + 76), (290, 8, 52))
        for sx in (-1, 1):
            g.box(PLUM, (bx + sx * 130, 40, Z0 + 20), (12, 50, 40))
    acts = (dict(arm_r='face', hold_r=P.hold_cup(col=MAG), swing_r=10, axis_r='x', rate=0.5),
            dict(arm_r='wave', swing_r=26, arm_l='down', rate=1.2, react=True),
            dict(arm_l='hold', arm_r='hold', hold_r=P.hold_phone(), turn=10, rate=0.4),
            dict(arm_l='up', arm_r='up', swing_l=12, swing_r=-12, rate=1.5, react=True))
    k = 0
    for bx, xs in seats:
        for x in xs:
            d = low(guest(r))
            d.update(acts[k])
            P.figure(sc, x, 26, z=Z0, yaw=r.uniform(-10, 10), legs='sit', phase=r.random(), detail=1, **d)
            k += 1
    # the pram slides to and fro (a slow swing about a pivot far under the ground), the parent's arms with it.
    # Review of pass 3: axis 90, so it rolls ALONG the road (axis 0 slid it towards the road and back).
    pr = sc.part(P.motion(rate=0.6, phase=0.1, swing=2.2, pivot=(-40.0, -62.0, -800.0), axis=90.0))
    pr.box(CYAN, (-40, -62, Z0 + 60), (58, 40, 30))
    pr.box(MAG, (-56, -62, Z0 + 86), (28, 42, 26))
    for sx in (-1, 1):
        pr.box(DARK, (-40 + sx * 20, -62, Z0 + 14), (18, 44, 18))
    pr.box(GREY_L, (-40, -40, Z0 + 86), (40, 6, 6))
    P.figure(sc, -40, -4, z=Z0, yaw=0, arm_l='push', arm_r='push', swing_l=9, swing_r=9, axis_l='x', axis_r='x', rate=0.6, phase=0.1,
             detail=1, **low(guest(r)))
    P.quadruped(sc, 356, -30, z=Z0, yaw=50, length=70, height=48, girth=20, neck=16, neck_up=35, head=18, tail=22, wag=30, rate=2.0,
                body='#e8c26a', ears='#8e5a3a')
    d = low(kid(r))
    P.figure(sc, 410, 14, z=Z0, yaw=-12, arm_l='up', arm_r='up', swing_l=12, swing_r=-12, bob=8, rate=1.9, phase=r.random(), react=True,
             detail=1, **d)


def kerb_d(sc):
    """Passing through, behind a low hedge with flowers: a parent pushing a pram, a child skipping and a second
    parent with a bag walk one way; one of the staff with a box and two friends with drinks walk the other. Each group
    travels the piece's length and comes round again."""
    r = random.Random(311)
    g = sc.still()
    riser(g, 1040, 180, col=LAV, trim=CREAM)
    g.box(HEDGE, (0, -76, Z0 + 20), (1040, 26, 40))
    for k in range(8):
        g.box((MAG, AMBER, WHITE, CYAN)[k % 4], (-455 + k * 130, -76, Z0 + 46), (40, 22, 12))
    TA, TB = (1000.0, 72.0), (1000.0, -62.0)

    def family():
        p = sc.part(P.motion(travel=tv_of(*TA), base=(0.0, -14.0), pivot=(66.0, -14.0, Z0 + 50)))
        p.box(MAG, (66, -14, Z0 + 60), (58, 40, 30))
        p.box(CYAN, (82, -14, Z0 + 86), (28, 42, 26))
        for sx in (-1, 1):
            p.box(DARK, (66 + sx * 20, -14, Z0 + 14), (18, 44, 18))
        P.figure(sc, 0, -14, z=Z0, yaw=90, arm_l='push', arm_r='push', stride=24, rate=1.0, phase=0.0, travel=TA, detail=1, **low(guest(r)))
        P.figure(sc, -80, -34, z=Z0, yaw=90, arm_l='up', arm_r='up', swing_l=12, swing_r=-12, stride=28, bob=8, rate=1.6, phase=0.4,
                 travel=TA, detail=1, **low(kid(r)))
        P.figure(sc, -160, -4, z=Z0, yaw=90, arm_r='down', arm_l='wave', hold_r=P.hold_bag(col=CYAN), swing_l=24, stride=24, rate=1.0,
                 phase=0.5, travel=TA, detail=1, **low(guest(r)))

    def others():
        P.figure(sc, 300, 46, z=Z0, yaw=-90, arm_l='carry', arm_r='carry', hold_r=P.hold_box(col=MAG, w=40), stride=22, rate=0.95, phase=0.2,
                 travel=TB, detail=1, **low(staff(r)))
        P.figure(sc, 420, 36, z=Z0, yaw=-90, arm_r='face', hold_r=P.hold_cup(col=AMBER), swing_l=20, axis_l='x', stride=22, rate=0.95,
                 phase=0.7, travel=TB, detail=1, **low(guest(r)))
        P.figure(sc, 480, 58, z=Z0, yaw=-90, arm_r='wave', swing_r=24, swing_l=20, axis_l='x', stride=22, rate=0.95, phase=0.1,
                 travel=TB, detail=1, **low(guest(r)))
    together(sc, (0.0, -14.0), family)
    together(sc, (380.0, 46.0), others)


# ------------------------------------------------------------------------------------------ at rest (TALL)
def terrace(sc):
    """A cafe terrace on banded posts, its deck 3.3 m up: two tables under parasols with guests sitting over drinks,
    three at the rail cheering the chase, a waiter walking the back of the deck with a tray; a striped fascia with
    a row of steady lamps, a stair at one end."""
    r = random.Random(313)
    H = 330.0
    g = sc.still()
    legs(g, (-460, 460), (-150, 150), H)
    g.box(CREAM, (0, 0, H - 8), (1000, 380, 16))
    stripes(g, -500, 500, -193, H - 62, H - 16, (MAG, CREAM), 8)
    bulbs(g, (-470, -198, H - 76), (470, -198, H - 76), 7)
    for x in (-494, -165, 165, 494):
        g.box(PLUM, (x, -184, H + 50), (8, 8, 100))
    g.box(GOLD, (0, -184, H + 100), (1000, 7, 7))
    for sx in (-1, 1):
        g.box(GOLD, (sx * 494, 0, H + 100), (7, 368, 7))
    tilt_box(g, WOOD, (660, 60, 0), (505, 60, H - 10), 110, 14)
    acts = (dict(arm_r='face', hold_r=P.hold_cup(col=WHITE), swing_r=10, axis_r='x', rate=0.5),
            dict(arm_r='wave', swing_r=26, rate=1.2, react=True),
            dict(arm_l='up', arm_r='up', swing_l=12, swing_r=-12, rate=1.5, react=True),
            dict(arm_r='point', arm_l='hip', swing_r=10, axis_r='x', rate=0.7))
    k = 0
    for j, tx in enumerate((-300, 260)):
        g = sc.still()
        g.cyl(PLUM, (tx, 70, H), 6, 70, steps=6)
        g.cyl((CYAN, AMBER)[j], (tx, 70, H + 70), 52, 6, steps=8)
        g.box(CREAM, (tx, 70, H + 190), (5, 5, 240))
        g.cyl((MAG, CYAN)[j], (tx, 70, H + 285), 150, 50, steps=8, top_r=8)
        for sx in (-1, 1):
            g = sc.still()
            g.box(PLUM, (tx + sx * 86, 60, H + 23), (36, 36, 46))
            d = guest(r)
            d.update(acts[k % 4])
            P.figure(sc, tx + sx * 86, 54, z=H, yaw=sx * -14, legs='sit', phase=r.random(), detail=0, **d)
            k += 1
    stand = (dict(arm_l='up', arm_r='up', swing_l=14, swing_r=-14, bob=12, rate=1.6), dict(arm_r='wave', swing_r=28, arm_l='hip', bob=5, rate=1.3),
             dict(arm_l='vee', arm_r='vee', swing_l=10, swing_r=-10, bob=14, rate=1.9))
    for i, x in enumerate((-70, 25, 110)):
        d = kid(r) if i == 2 else guest(r)
        d.update(stand[i])
        P.figure(sc, x, -140, z=H, yaw=r.uniform(-12, 12), phase=r.random(), react=True, detail=1, **d)
    P.figure(sc, 0, 150, z=H, yaw=90, arm_r='hold', arm_l='down', hold_r=hold_tray(), stride=22, swing_l=18, axis_l='x', rate=0.9, phase=0.2,
             travel=(840.0, 50.0), detail=0, **staff(r))


# ------------------------------------------------------------------------------------------ at work (TALL)
def painters(sc):
    """The staff repainting a show panel 6 m high in candy stripes (half of it done) from a scaffold of two decks: two
    with rollers, one with a brush, one turned round waving at the chase; on the ground one hauls a bucket up on a
    rope and one sorts the tins. A row of steady work lamps along the top."""
    r = random.Random(317)
    g = sc.still()
    g.box(CREAM, (0, 100, 320), (780, 14, 600))
    g.box(GOLD, (0, 100, 628), (800, 20, 16))
    for sx in (-1, 1):
        g.box(PLUM, (sx * 395, 100, 320), (16, 22, 640))
    w = 760.0 / 8
    for k in range(5):
        full = k < 4
        g.box((MAG, CYAN)[k % 2], (-380 + (k + 0.5) * w, 91, 320 if full else 480), (w - 6, 6, 580 if full else 260))
    for sx in (-1, 0, 1):
        for sy in (-1, 1):
            g.box(GREY_L, (sx * 370, sy * 55 - 10, 330), (9, 9, 660))
    for hz in (220, 440):
        g.box(WOOD, (0, -10, hz), (780, 120, 10))
        g.box(AMBER, (0, -68, hz + 96), (780, 6, 7))
    tilt_box(g, GREY_L, (-370, -66, 10), (0, -66, 220), 7, 7)
    tilt_box(g, GREY_L, (0, -66, 10), (370, -66, 220), 7, 7)
    g.box(GREY_L, (0, -10, 656), (780, 9, 9))
    bulbs(g, (-360, -72, 652), (360, -72, 652), 6)
    g.box(GREY_L, (330, -85, 660), (9, 70, 9))
    g.box(CREAM, (330, -116, 330), (5, 5, 650))
    for k in range(3):
        g.cyl((MAG, CYAN, AMBER)[k], (-250 - k * 42, -150, 0), 16, 30, steps=6)
    bk = sc.part(P.motion(rate=0.25, bob=-80.0, pivot=(330.0, -128.0, 330.0)))
    bk.cyl(AMBER, (330, -128, 300), 22, 34, steps=6, top_r=26)
    crew = lambda **kw: staff(r, hat='hard', hat_col=AMBER, **kw)
    P.figure(sc, -250, -6, z=445, yaw=180, arm_r='up', arm_l='hip', hold_r=hold_roller(MAG), swing_r=14, axis_r='x', rate=0.9, phase=0.1, detail=1, **crew())
    P.figure(sc, 70, -6, z=445, yaw=180, arm_r='fwd', arm_l='down', hold_r=hold_brush(CYAN), swing_r=18, axis_r='y', rate=1.3, phase=0.5, detail=1, **crew())
    P.figure(sc, -110, -6, z=225, yaw=180, arm_r='up', arm_l='hip', hold_r=hold_roller(CYAN), swing_r=14, axis_r='x', rate=0.8, phase=0.7, detail=1, **crew())
    P.figure(sc, 240, -20, z=225, yaw=0, arm_l='up', arm_r='up', swing_l=14, swing_r=-14, bob=5, rate=1.5, phase=0.3, react=True, detail=1, **crew())
    P.figure(sc, 300, -160, z=0, yaw=-70, arm_l='up', arm_r='up', swing_l=20, swing_r=20, axis_l='x', axis_r='x', rate=0.5, phase=0.0, detail=0, **crew())
    P.figure(sc, -200, -165, z=0, yaw=20, arm_r='point', arm_l='hip', swing_r=10, axis_r='x', rate=0.7, phase=0.6, detail=0, **crew())


# ------------------------------------------------------------------------------------------ passing through (TALL)
def train(sc):
    """The park's miniature railway on a trestle 2.5 m up, 22 m of it: an engine (its driver waving) and two open
    wagons of guests run from one shed to the other and come round again; two guests wait on the platform and wave.
    A row of steady lamps under the beam."""
    r = random.Random(331)
    H = 250.0
    g = sc.still()
    g.box(CREAM, (0, 0, H - 12), (2200, 110, 24))
    for sy in (-1, 1):
        g.box(GOLD, (0, sy * 34, H + 3), (2200, 6, 6))
    for k in range(5):
        x = -880 + k * 440
        for sy in (-1, 1):
            tilt_box(g, PLUM, (x, sy * 120, 0), (x, sy * 30, H - 24), 20, 20)
        g.box(MAG, (x, 0, H * 0.5), (24, 150, 16))
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box((MAG, CYAN)[sx > 0], (sx * 990, sy * 78, H + 110), (220, 14, 220))
            g.box(PLUM, (sx * 990, sy * 78, H / 2.0), (18, 18, H))
        g.box(CREAM, (sx * 990, 0, H + 232), (250, 190, 24))
        g.box(GOLD, (sx * 990, 0, H + 252), (120, 100, 16))
    bulbs(g, (-850, -62, H - 34), (850, -62, H - 34), 8)
    g.box(CREAM, (-700, -122, H - 6), (360, 124, 12))
    for sx in (-1, 1):
        g.box(PLUM, (-700 + sx * 160, -170, (H - 12) / 2.0), (14, 14, H - 12))
    for i, x in enumerate((-780, -640)):
        d = kid(r) if i else guest(r)
        P.figure(sc, x, -130, z=H, yaw=r.uniform(-10, 10), arm_l='up', arm_r='up', swing_l=14, swing_r=-14, bob=10, rate=1.6, phase=r.random(),
                 react=True, detail=0, **d)
    TR = (1300.0, 140.0)
    F = H + 22.0          # the wagons' floor

    def consist():
        t = sc.part(P.motion(travel=tv_of(*TR), base=(0.0, 0.0), pivot=(0.0, 0.0, H + 60)))
        # the engine
        t.box(MAG, (320, 0, H + 45), (190, 86, 50))
        t.box(CYAN, (360, 0, H + 95), (100, 58, 50))
        t.cyl(AMBER, (392, 0, H + 120), 14, 50, steps=6, top_r=20)
        t.box(GOLD, (420, 0, H + 40), (14, 70, 30))
        t.box(CREAM, (236, 0, H + 96), (10, 84, 52))
        for sx in (-1, 0, 1):
            t.box(DARK, (320 + sx * 62, 0, H + 16), (30, 94, 30))
        # two open wagons
        for j, cx in enumerate((40, -230)):
            t.box((AMBER, CYAN)[j], (cx, 0, H + 40), (224, 92, 44))
            t.box(GOLD, (cx, 0, H + 64), (230, 98, 6))
            for sx in (-1, 1):
                t.box(DARK, (cx + sx * 70, 0, H + 14), (28, 98, 28))
            t.box(DARK, (cx + 125, 0, H + 30), (30, 10, 8))
        P.figure(sc, 270, 0, z=H + 70, yaw=20, arm_r='wave', arm_l='hip', swing_r=28, rate=1.2, phase=0.2, travel=TR, detail=0, **staff(r))
        acts = (dict(arm_l='up', arm_r='up', swing_l=14, swing_r=-14, rate=1.6), dict(arm_r='wave', swing_r=26, rate=1.3),
                dict(arm_l='vee', arm_r='vee', swing_l=10, swing_r=-10, rate=1.4))
        k = 0
        for cx in (40, -230):
            for dx in (-72, 0, 72):
                d = kid(r) if k in (1, 4) else guest(r)
                d.update(acts[k % 3])
                P.figure(sc, cx + dx, 6 if k % 2 else 0, z=F, yaw=(8, 90, -8)[k % 3], legs='sit', phase=r.random(), travel=TR, detail=0, **d)
                k += 1
    together(sc, (0.0, 0.0), consist)


# ------------------------------------------------------------------------------------------ the photo spot (TALL)
def photo(sc):
    """The park's photo spot: a round podium before a gold frame and a striped backdrop with a star; a family of four
    jumps together for the picture (in step, again and again), the photographer shoots from a step stool, one of the
    staff directs, the next two wait on the steps (one waves at the chase). A row of steady lamps along the frame."""
    r = random.Random(337)
    H = 110.0
    g = sc.still()
    g.cyl(PLUM, (0, 0, 0), 300, H - 12, steps=10)
    g.cyl(CREAM, (0, 0, H - 12), 310, 12, steps=10)
    g.box(CREAM, (355, -10, 36), (130, 230, 72))
    stripes(g, -330, 330, 156, H, H + 470, (CYAN, CREAM, MAG, CREAM), 6, depth=10)
    for sx in (-1, 1):
        g.box(GOLD, (sx * 345, 140, H + 240), (26, 16, 480))
    g.box(GOLD, (0, 140, H + 485), (716, 16, 26))
    g.box(AMBER, (0, 140, H + 545), (84, 12, 84), rot=(45, 0, 0), glow=1.0)
    bulbs(g, (-320, 128, H + 462), (320, 128, H + 462), 8)
    g.box(MINT, (-10, -238, 40), (70, 60, 80))
    g.box(CREAM, (-10, -238, 84), (82, 72, 8))
    for i, x in enumerate((-160, -55, 50, 155)):
        d = kid(r) if i in (1, 2) else guest(r)
        d.update((dict(arm_l='vee', arm_r='vee', swing_l=10, swing_r=-10), dict(arm_l='up', arm_r='up', swing_l=12, swing_r=-12))[i % 2])
        P.figure(sc, x, 20, z=H, yaw=r.uniform(-8, 8), bob=16, rate=0.7, phase=0.0, detail=1, **d)
    P.figure(sc, -10, -238, z=88, yaw=180, arm_l='hold', arm_r='hold', hold_r=P.hold_camera(), rate=0.7, phase=0.5, turn=6, detail=1, **staff(r))
    P.figure(sc, -235, -70, z=H, yaw=150, arm_r='point', arm_l='hip', swing_r=12, axis_r='x', rate=0.7, phase=0.2, detail=0, **staff(r))
    P.figure(sc, 355, -80, z=72, yaw=-10, arm_r='wave', arm_l='hip', swing_r=28, bob=5, rate=1.3, phase=r.random(), react=True, detail=0, **guest(r))
    P.figure(sc, 355, 40, z=72, yaw=110, arm_l='hold', arm_r='hold', swing_l=13, swing_r=-13, axis_l='z', axis_r='z', rate=2.2, phase=r.random(),
             detail=0, **guest(r))


# ------------------------------------------------------------------------------------------ the prize delivery (TALL)
def prizes(sc):
    """The prize delivery: a lift truck raises a pallet of giant plush animals (it goes up and down) to the staff on a
    store's flat roof: one reaches for them, one holds a plush over the head and hops; a marshal on the ground guides
    the truck with both arms, the driver sits at the wheel; a child jumps at the sight, a parent points."""
    r = random.Random(347)
    HD = 260.0
    g = sc.still()
    g.box(LAV, (330, 60, HD / 2.0), (360, 240, HD))
    stripes(g, 150, 510, -63, HD - 60, HD, (MAG, CREAM), 6)
    g.box(CREAM, (330, 60, HD + 5), (380, 260, 10))
    g.box(GOLD, (330, 184, HD + 100), (380, 7, 7))
    for sx in (-1, 1):
        g.box(PLUM, (330 + sx * 186, 184, HD + 55), (8, 8, 96))
    bulbs(g, (170, -70, HD - 74), (490, -70, HD - 74), 5)
    plush(g, 450, 80, HD + 10, AMBER, 1.1)
    # the lift truck
    g.box(MINT, (-250, 0, 62), (260, 130, 70))
    for sx in (-1, 1):
        g.box(DARK, (-250 + sx * 85, 0, 28), (56, 144, 56))
    g.box(CREAM, (-330, 0, 120), (90, 120, 46))
    for sy in (-1, 1):
        g.box(AMBER, (-110, sy * 48, 240), (12, 12, 440))
    g.box(AMBER, (-110, 0, 456), (12, 108, 12))
    up = sc.part(P.motion(rate=0.12, bob=-105.0, pivot=(-40.0, 0.0, 300.0)))
    up.box(WOOD, (-40, 0, 250), (150, 130, 12))
    up.box(AMBER, (-104, 0, 262), (8, 120, 36))
    plush(up, -78, -22, 256, MAG, 1.1)
    plush(up, -8, -30, 256, CYAN, 1.1)
    plush(up, -40, 34, 256, LIME, 1.0)
    P.figure(sc, -330, -6, z=97, yaw=90, legs='sit', arm_l='push', arm_r='push', rate=0.12, detail=0, **staff(r))
    P.figure(sc, 190, -10, z=HD + 10, yaw=-90, arm_l='carry', arm_r='carry', swing_l=12, swing_r=12, axis_l='x', axis_r='x', rate=0.6, phase=0.2,
             detail=1, **staff(r))
    P.figure(sc, 330, -20, z=HD + 10, yaw=4, arm_l='up', arm_r='up', hold_r=hold_plush(CYAN), bob=6, rate=1.4, phase=0.6, react=True, detail=1, **staff(r))
    P.figure(sc, -60, -150, z=0, yaw=110, arm_l='up', arm_r='up', swing_l=16, swing_r=-16, rate=0.8, phase=0.1, detail=1,
             **staff(r, hat='hard', hat_col=AMBER, band=LIME))
    P.figure(sc, 120, -150, z=0, yaw=-30, arm_r='point', arm_l='hip', swing_r=10, axis_r='x', rate=0.7, phase=0.4, detail=0, **guest(r))
    P.figure(sc, 200, -140, z=0, yaw=-20, arm_l='up', arm_r='up', swing_l=12, swing_r=-12, bob=14, rate=1.9, phase=0.8, react=True, detail=0, **kid(r))


# ------------------------------------------------------------------------------------------ the lagoon (TALL, on the valley floor)
def rail_gang(sc):
    """For the lagoon, where the road is the coaster 26 m over the valley: a maintenance tower 25 m high on four legs
    banded amber and dark, its deck at the height of the rails. On the deck, 1.5 times life size: the rail gang in hard
    hats: a flagman waving a pink flag, one cheering, one pointing at the rails, one guiding the load on the jib (a
    spare bogie that sways on its cable), one with a box, one waving. A hazard-striped fascia with steady lamps. The
    legs go 6 m under z = 0 so that it stands in a hollow as well as on a hump."""
    r = random.Random(349)
    H = 2500.0
    g = sc.still()
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(PLUM, (sx * 520, sy * 170, (H - 600) / 2.0), (50, 50, H + 600))
            for k in range(4):
                g.box(AMBER if k % 2 == 0 else DARK, (sx * 520, sy * 170, 500 + k * 560), (58, 58, 150))
    for k in range(4):
        z0, z1 = 150 + k * 580, 150 + (k + 1) * 580
        tilt_box(g, GREY_L, (-520 if k % 2 == 0 else 520, -170, z0), (520 if k % 2 == 0 else -520, -170, z1), 18, 18)
    g.box(CREAM, (0, 0, H - 10), (1280, 460, 20))
    stripes(g, -640, 640, -232, H - 100, H - 20, (AMBER, DARK), 12)
    bulbs(g, (-610, -240, H - 122), (610, -240, H - 122), 10, size=16.0)
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(PLUM, (sx * 632, sy * 222, H + 78), (12, 12, 156))
        g.box(AMBER, (sx * 632, 0, H + 156), (9, 452, 9))
    g.box(AMBER, (0, -222, H + 156), (1280, 9, 9))
    # the jib and its load
    g.box(PLUM, (470, 90, H + 360), (22, 22, 720))
    tilt_box(g, AMBER, (470, 90, H + 700), (150, -50, H + 780), 18, 18)
    ld = sc.part(P.motion(rate=0.15, swing=5.0, pivot=(150.0, -50.0, H + 780.0), axis=0.0))
    ld.box(CREAM, (150, -50, H + 620), (6, 6, 320))
    ld.box(MAG, (150, -50, H + 430), (150, 70, 56))
    for sx in (-1, 1):
        ld.box(DARK, (150 + sx * 50, -50, H + 392), (44, 84, 44))
    crew = lambda **kw: staff(r, hat='hard', hat_col=AMBER, band=LIME, s=1.5, **kw)
    P.figure(sc, -500, -150, z=H, yaw=8, arm_r='wave', arm_l='hip', hold_r=P.hold_flag(col=MAG, w=70), swing_r=30, rate=1.0, phase=0.1, react=True,
             detail=0, **crew())
    P.figure(sc, -300, -150, z=H, yaw=-6, arm_l='up', arm_r='up', swing_l=14, swing_r=-14, bob=12, rate=1.6, phase=0.5, react=True, detail=0, **crew())
    P.figure(sc, -100, -150, z=H, yaw=-20, arm_r='point', arm_l='hip', swing_r=10, axis_r='x', rate=0.7, phase=0.3, detail=0, **crew())
    P.figure(sc, 150, 70, z=H, yaw=0, arm_l='up', arm_r='up', swing_l=8, swing_r=8, axis_l='x', axis_r='x', rate=0.15, phase=0.0, detail=0, **crew())
    P.figure(sc, 340, -150, z=H, yaw=10, arm_l='carry', arm_r='carry', hold_r=P.hold_box(col=MAG, w=44), rock=4, rate=0.5, phase=0.7, detail=0, **crew())
    P.figure(sc, 540, -150, z=H, yaw=-8, arm_r='wave', arm_l='down', swing_r=28, bob=5, rate=1.3, phase=0.9, react=True, detail=0, **crew())


# ------------------------------------------------------------------------------------------ the kit
LOW = dict(grow=(60.0, 30.0, 30.0), grow_down=(60.0, 30.0, 0.0))
JOBS = [
    ('SM_P3A_Kerb_A', kerb_a, LOW),
    ('SM_P3A_Kerb_B', kerb_b, LOW),
    ('SM_P3A_Kerb_C', kerb_c, LOW),
    ('SM_P3A_Kerb_D', kerb_d, dict(grow=(260.0, 30.0, 30.0), grow_down=(260.0, 30.0, 0.0))),
    ('SM_P3A_Terrace', lit(terrace)),
    ('SM_P3A_Painters', lit(painters), dict(grow=(60.0, 60.0, 120.0))),
    ('SM_P3A_Train', lit(train)),
    ('SM_P3A_Photo', lit(photo)),
    ('SM_P3A_Prizes', lit(prizes), dict(grow=(60.0, 60.0, 140.0))),
    ('SM_P3A_RailGang', lit(rail_gang, 0.1)),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths(only=None):
    return [DIR + '/' + j[0] for j in JOBS if not only or j[0] in only]
