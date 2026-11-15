"""FULL THROTTLE (Unreal) dressing kit, level 4: the Theme Park (Coaster Cove, at night). Themed lands, one after the
other: the front gate and its plaza, the midway of stalls, Pirate Cove, the lagoon under the coaster, Rocket Land,
the funhouse and the back lot, and the promenade of the finale. Placed by Source/FullThrottle/FTDressL4.inl.
See ft_dress.py for how a piece is drawn (front faces -Y, base on z = 0, long axis along X, cm).

    import ft_dress; ft_dress.run(['park'])

All of it is our own: the park's name is the game's, the mascot (STARRY, a star in a ringmaster's coat) and every
ride, sign and land are invented here; nothing is taken from a real park, brand or film.

MOTION. A ride's moving parts are drawn in "mover" colours (mv(): the same colour one step off, so those parts get
material slots of their own) and MOVERS below says, for each such mesh, what should move, about which axis and through
which point of the mesh. animate() at the bottom hands that to the living-world pass (ft_life.animate: it puts M_Life
on those slots only, so the hub turns and the deck stands still). Until it is run the rides stand in a mid-ride pose.
Run animate() again after every export: the export puts the plain palette back.

UNDER EVERY PIECE (_std): a dark foundation slab 1.6 m deep under the footprint, so a piece that stands where the land
tilts shows a plinth instead of daylight; and a thin hidden post going down 0.85 of the piece's height. The post is
there for the visual-finish pass every level but Bayshore gets on export (ft_visual_art.refine): it adds a service
panel just inside the -X face of a piece's bounding box, between 15% and 43% of its height. On a plain building that
is a welcome detail on the end wall; on a stall, a wheel or a sign on posts it would hang in the air. With the post the
box is deep enough that the panel lies under the ground.

THE LAGOON (SM_Lake, SM_Lake2) is not stood on the land like the rest: every lake is placed at one height above the
level's floor (SiteAfloat in the table), which is the lagoon's water level, so the humps of land under the coaster
rise out of one sheet of water and the hollows between them hold it.

THIRD PASS (15 Nov 2026, the overhaul): dense, distinctive and alive at racing speed.
  - the verge (17 to 24 m) reads now: the fence is a striped show hoarding under a line of pennants that flutter, the
    queue rails and the benches have guests in them (_guest), the stalls and gates have a crowd before them (SM_Crowd);
  - the cut banks are faced by a Bank rule (SM_CutLawn, SM_CutLawnLow: flat panels, ft_dress_bank.py) instead of the
    two tilted beds, which were loud and left most of each bend bare;
  - things that travel are ft_life.Kit props, built by life_props() below and saved beside the rest of the kit: the
    sky ride's gondolas (SM_SkyRide), a train over a wooden coaster's hill (SM_WoodCoaster, a stage flat 120 m long),
    the log flume's boats and spray (SM_Flume), swan boats on the lagoon (SM_SwanBoats);
  - mechanical figures: STARRY waves and nods, Rocket Land's tin robot waves and turns his head (SM_Robot), the
    cove's kraken has a galleon in its arms (SM_Galleon); a second mover shade (mv2) lets one mesh carry two motions;
  - the horizon has two rows of flat skyline cards behind the lit rides (SM_Sky1, SM_Sky2) and the far wheel turns.
animate() calls life_props() first: one call after an export sets everything going.
"""
import math
import random
import unreal
import ft_art
import ft_city
import ft_dress_bank as B
import ft_visual_art
from ft_desert import rod, tilt_box

NAVY, DEEP, PURPLE, PLUM = '#2b2350', '#2a2546', '#3a2f6a', '#4a3a78'
MAG, CYAN, AMBER, BULB, CREAM = '#ff4fb8', '#2ee6ff', '#ffd23f', '#ffe28a', '#fff1d6'
RED, WHITE, LAV, MINT, TEAL = '#ff2d3c', '#fff6ea', '#b9a4ff', '#7fe3c4', '#2fc6c0'
WOOD, WOOD_D, PINE, GREY, GREY_L, ROCK, ROCK_D = '#9a6a44', '#6a4c36', '#2f6b52', '#6a6878', '#9a98a8', '#4a4466', '#3a3556'
POOL = '#3a5fa8'      # a paddling pool's water: unlit (the lagoon's WATER glows, and stands far from the road)
ORANGE, GOLD, SAND, WATER, SHED, SHED_D, GEN = '#ff8a2a', '#ffc93c', '#efcf9f', '#1c5f8a', '#5a5870', '#4a4860', '#3f6a5a'
G1, G2, G3, G4 = 0.35, 0.9, 1.4, 2.0          # glow: soft, a lit window, a sign, a bulb
GW = 0.2                                      # lit water
BULBS = [BULB, MAG, CYAN, AMBER]
DIR = '/Game/Maps/ThemePark/Dress/'

# what moves (see animate()): mesh -> the M_Life parameters for its mover slots. Axes and pivots are in the mesh's own space (cm).
MOVERS = {
    'SM_JetRide': dict(what='the hub, its six arms and rockets, the planet on top: turns about Z',
                       SpinAxis=(0, 0, 1), SpinPivot=(0, 0, 0), SpinRate=0.7),
    'SM_SwingRide': dict(what='the crown and its ten chairs: turns about Z, faster',
                         SpinAxis=(0, 0, 1), SpinPivot=(0, 0, 0), SpinRate=1.25),
    'SM_PirateShip': dict(what='the ship and its hangers: swings 38 degrees each way about Y through the axle',
                          SpinAxis=(0, 1, 0), SpinPivot=(0, 0, 1350), SpinRate=1.4, SpinSwing=38.0),
    'SM_BigWheel': dict(what='the rims, spokes and gondolas: turns slowly about Y through the hub', far=True,
                        SpinAxis=(0, 1, 0), SpinPivot=(0, 0, 3400), SpinRate=0.1),
    'SM_FarWheel': dict(what='the giant wheel on the skyline: the same, slower still', far=True,
                        SpinAxis=(0, 1, 0), SpinPivot=(0, 0, 6300), SpinRate=0.05),
    'SM_MarqueeTower': dict(what='the star on the marquee tower: turns about Z', SpinAxis=(0, 0, 1), SpinPivot=(0, 0, 0), SpinRate=0.9),
    # mechanical figures: an arm that waves from the shoulder (SpinSwing: it rocks instead of turning)
    'SM_Mascot': dict(what="STARRY's raised arm: waves from the shoulder", SpinAxis=(0, 1, 0), SpinPivot=(130, 0, 450), SpinRate=3.2, SpinSwing=22.0),
    'SM_Robot': dict(what="the robot's raised arm: waves from the shoulder", SpinAxis=(0, 1, 0), SpinPivot=(300, 0, 930), SpinRate=2.2, SpinSwing=24.0),
}
# a second motion on the same mesh, on the mv2 shades
MOVERS2 = {
    'SM_Mascot': dict(what='his star head and hat: nod from the neck', SpinAxis=(0, 1, 0), SpinPivot=(0, 0, 540), SpinRate=1.3, SpinSwing=6.0),
    'SM_Robot': dict(what='his head: turns left and right on the neck', SpinAxis=(0, 0, 1), SpinPivot=(0, 0, 1160), SpinRate=0.8, SpinSwing=38.0),
}
TIN, TIN_D, PEACH, KRAKEN, SPRAY, LAWN = '#a8b4d0', '#7f8aa6', '#ffc59e', '#c2408f', '#dff6ff', '#4f9a6a'
_MOVER_BASE = (NAVY, PURPLE, PLUM, MAG, CYAN, AMBER, BULB, CREAM, RED, WHITE, LAV, MINT, TEAL, WOOD, WOOD_D, GREY_L, ORANGE, GOLD, TIN, TIN_D, KRAKEN)
SKIN = ('#f2c8a0', '#b87a50')                     # guests: two skins, five shirts, one pair of trousers (few material slots)
SHIRT = (MAG, CYAN, AMBER, MINT, WHITE)


def mv(col):
    """The mover's shade of a colour: one step off in blue, so the part gets its own material slot (see MOTION)."""
    b = int(col[5:7], 16)
    return col[:5] + '%02x' % (b - 1 if b == 255 else b + 1)


def mv2(col):
    """A second mover's shade: two steps off in blue (a mesh with two motions: an arm that waves, a head that turns)."""
    b = int(col[5:7], 16)
    return col[:5] + '%02x' % (b - 2 if b >= 254 else b + 2)


def mover_hexes(shade=mv):
    """The mover shades, as ft_life names a slot's colour (hex without #)."""
    return set(shade(c).lstrip('#').lower() for c in _MOVER_BASE)


def life_props():
    """The kit's props whose parts travel (ft_life.Kit: a Conveyor, Plume or Burst part is told apart by its UVs, which
    an exported piece does not have). Built here and saved in the Dress folder with M_Life already on them; the table
    places them like any other piece. Returns their names."""
    import ft_life
    done = []
    for name, fn in KITS:
        try:        # one prop that fails must not stop the others, nor the lights animate() sets after them
            k = ft_life.Kit()
            kw = fn(k) or {}
            k.save(DIR + name, **kw)
            done.append(name)
        except Exception as e:  # noqa
            import traceback
            unreal.log_error('FTDRESS park: kit prop %s FAILED: %r %s' % (name, e, traceback.format_exc()[-900:]))
            print('JAM ERR kit prop', 'park', name, repr(e)[:300], flush=True)
    return done


def animate():
    """Set the park going. Builds the travelling props (life_props), then puts M_Life on the mover slots of the pieces
    that turn, wave or flutter and on the lights that run (ft_life.py, the living-world pass). Run in the editor after
    ft_dress.run(['park']): an export puts the plain palette back. It changes material slots only and saves each
    mesh in place."""
    import ft_life
    life_props()
    A = ft_life.animate
    movers, movers2 = mover_hexes(), mover_hexes(mv2)
    n = 0
    for table, shades in ((MOVERS, movers), (MOVERS2, movers2)):
        for name, p in table.items():
            kw = dict((k, v) for k, v in p.items() if k not in ('what', 'far'))
            n += A(DIR + name, 'Spin', slots=lambda slot, col, glow, shades=shades: col in shades, far=bool(p.get('far')), SpinRand=1.0, **kw)
    # the bulbs of the stalls and signs run in a chase; the lagoon's lights shimmer
    for name in ('SM_GameStall', 'SM_GameStall2', 'SM_GameStall3', 'SM_TicketGate', 'SM_SignMidway', 'SM_SignPirate',
                 'SM_SignLagoon', 'SM_SignRocket', 'SM_SignFun', 'SM_SignRide', 'SM_FacadeRow'):
        n += A(DIR + name, 'Light', slots=lambda slot, col, glow: glow >= 1.9, LightAxis=(1, 0, 0), LightLen=260.0, LightRate=0.8,
               LightDuty=0.6, LightMin=0.25)
    for name in ('SM_Lake', 'SM_Lake2', 'SM_Pond'):
        n += A(DIR + name, 'Light', slots=lambda slot, col, glow: 0.8 < glow < 1.0, LightHash=0.35, LightCell=300.0, LightRate=1.5,
               LightMin=0.2)
    # third pass. Marquee lights that climb: the high striker's scale (they were steady: the old filter picked nothing),
    # the bulbs up the marquee towers; the lights of the coasters run along their rails
    n += A(DIR + 'SM_HighStriker', 'Light', slots=lambda slot, col, glow: 0.8 < glow < 1.5, LightAxis=(0, 0, 1), LightLen=756.0, LightRate=0.9,
           LightDuty=0.5, LightMin=0.3, LightRand=1.0)
    n += A(DIR + 'SM_MarqueeTower', 'Light', slots=lambda slot, col, glow: glow >= 1.9 and col not in movers, LightAxis=(0, 0, 1), LightLen=660.0,
           LightRate=0.7, LightDuty=0.5, LightMin=0.3, LightRand=1.0)
    for name, ln in (('SM_CoasterHill', 1600.0), ('SM_FarCoaster', 6500.0)):
        n += A(DIR + name, 'Light', slots=[MAG + '*'], far=(name == 'SM_FarCoaster'), LightAxis=(1, 0, 0), LightLen=ln, LightRate=0.45,
               LightDuty=0.6, LightSoft=0.15, LightMin=0.4, LightRand=1.0)
    n += A(DIR + 'SM_Saucer', 'Light', slots=lambda slot, col, glow: glow >= 1.9, LightHash=0.4, LightCell=70.0, LightRate=3.0, LightMin=0.3, LightRand=1.0)
    n += A(DIR + 'SM_Robot', 'Light', slots=lambda slot, col, glow: abs(glow - G3) < 0.011, LightHash=0.4, LightCell=80.0, LightRate=2.0, LightMin=0.25,
           LightRand=1.0)
    # cloth: the pennants over the hoarding (small, unlit: they stand right behind the barrier), the galleon's flag
    n += A(DIR + 'SM_ParkFence', 'Flutter', slots=lambda slot, col, glow: col in movers, FlutterPin=(0.0, 0.0, 192.0), FlutterAxis=(0.0, 0.0, -1.0),
           FlutterDir=(0.0, 1.0, 0.0), FlutterSpan=46.0, FlutterLen=130.0, FlutterRate=6.0, FlutterAmp=7.0, FlutterPhase=(0.004, 0.0, 0.0))
    n += A(DIR + 'SM_Galleon', 'Flutter', slots=lambda slot, col, glow: col in movers2, FlutterPin=(150.0, 0.0, 3800.0), FlutterAxis=(1.0, 0.0, 0.0),
           FlutterDir=(0.0, 1.0, 0.0), FlutterSpan=400.0, FlutterLen=380.0, FlutterRate=5.0, FlutterAmp=26.0)
    # the kraken's arms sway from the water up; the crowd bobs, person after person
    n += A(DIR + 'SM_Galleon', 'Sway', slots=lambda slot, col, glow: col in movers, far=True, SwayBase=40.0, SwayHeight=2300.0, SwayAmp=240.0,
           SwayPow=1.5, SwayRate=0.8, SwayLean=0.2)
    n += A(DIR + 'SM_Galleon', 'Light', slots=lambda slot, col, glow: glow >= 1.9, LightHash=0.3, LightCell=90.0, LightRate=1.5, LightMin=0.4, LightRand=1.0)
    n += A(DIR + 'SM_Balloon', 'Bob', far=True, BobAmp=220.0, BobRate=0.05, BobRand=1.0)
    n += A(DIR + 'SM_Crowd', 'Bob', slots=lambda slot, col, glow: col != GREY_L.lstrip('#'), BobAmp=5.0, BobRate=1.1, BobRand=1.0,
           BobPhase=(0.0021, 0.0013, 0.0))
    # the swinging ship leaves the box it was built in: 4 m more of bounds along X, or it is culled at the screen's edge
    try:
        sm = unreal.EditorAssetLibrary.load_asset(DIR + 'SM_PirateShip')
        if sm and abs(sm.get_editor_property('positive_bounds_extension').x - 400.0) > 1.0:
            sm.set_editor_property('positive_bounds_extension', unreal.Vector(400.0, 0.0, 0.0))
            sm.set_editor_property('negative_bounds_extension', unreal.Vector(400.0, 0.0, 0.0))
            unreal.EditorAssetLibrary.save_loaded_asset(sm)
    except Exception as e:  # noqa
        unreal.log_warning('FTDRESS park: the pirate ship keeps its built bounds (%r)' % (e,))
    return n


def _ccw(pts):
    """The polygon counter-clockwise, whichever way it was written (an extrusion written clockwise comes out inside out)."""
    a = 0.0
    for (x0, y0), (x1, y1) in zip(pts, pts[1:] + pts[:1]):
        a += x0 * y1 - x1 * y0
    return list(pts) if a >= 0 else list(reversed(pts))


def _side(m, col, pts, y0, y1, glow=0.0):
    m.side(col, _ccw(pts), y0, y1, glow=glow)


def _foot(m, top):
    """The hidden post (see the module note)."""
    d = top * 0.85
    m.box(NAVY, (0, 0, -d / 2.0), (14, 14, d))


def _std(fn, slab=160.0, post=True):
    """A piece as the table stands it: fn's drawing, the foundation slab under its footprint, the hidden post. The
    third pass's pieces are made without the post (post=False): the finish pass's panels it hid are no longer added
    to dressing pieces (ft_dress.export), and it made a piece's box, and so its cull distance, twice as deep."""
    def build(m):
        fn(m)
        env = ft_visual_art.envelope(m.meshes)
        if env is None:
            return
        lo, size = env
        if slab:
            m.box(DEEP, (lo[0] + size[0] / 2.0, lo[1] + size[1] / 2.0, -slab / 2.0 - 2.0), (size[0] * 0.97, size[1] * 0.97, slab))
        if post:
            _foot(m, lo[2] + size[2])
    return build


def _awn(deg):
    """The roll that drops an awning's road-side (-Y) edge by deg."""
    return -ft_city._roll_sign() * deg


def _bulbs(m, a, b, n, cols=None, r=12, glow=G4):
    """n bulbs strung from a to b."""
    cols = cols or BULBS
    for k in range(n):
        t = (k + 0.5) / n
        m.sphere(cols[k % len(cols)], (a[0] + (b[0] - a[0]) * t, a[1] + (b[1] - a[1]) * t, a[2] + (b[2] - a[2]) * t), r, steps=(3, 5), glow=glow)


def _ring(m, col, centre, major, minor, rot=(0, 0, 90), steps=24, tube=4, glow=0.0):
    """A torus: rot (0, 0, 90) stands it up facing the road (its axis along Y); (0, 0, 0) lays it flat."""
    unreal.GeometryScript_Primitives.append_torus(m._mesh(col, glow), m.opts, m._t(centre, rot), unreal.GeometryScriptRevolveOptions(),
                                                 major, minor, steps, tube, unreal.GeometryScriptPrimitiveOriginMode.CENTER)


# the block capitals of the game's neon signs (ft_city.GLYPHS: strokes on a 4 x 6 grid) and the letters it lacks
_GLYPHS = dict(ft_city.GLYPHS)
_GLYPHS.update({
    'F': [(0, 0, 0, 6), (0, 6, 4, 6), (0, 3, 3, 3)],
    'W': [(0, 6, 1, 0), (1, 0, 2, 4), (2, 4, 3, 0), (3, 0, 4, 6)],
    'X': [(0, 0, 4, 6), (0, 6, 4, 0)],
    'Z': [(0, 6, 4, 6), (4, 6, 0, 0), (0, 0, 4, 0)],
    'J': [(4, 6, 4, 1), (4, 1, 3, 0), (3, 0, 1, 0), (1, 0, 0, 1)],
})


def _letters(m, word, y, z0, h, col, glow=G3, x=0.0):
    """Block capitals h high that face the road, centred on x, their feet at z0, standing 12 cm proud of the plane y.
    Drawn as ft_city.text draws them (one box a stroke), with the fuller alphabet above."""
    unit = h / 6.0
    t = unit * 0.8
    adv = 5.6 * unit
    width = len(word) * adv
    wall = ft_city.Wall(m, None, (x + width / 2.0, y, 0), (-1, 0, 0), (0, -1, 0), width)   # a wall facing -Y reads from +X to -X
    u = width / 2.0 - (len(word) * adv - 1.6 * unit) / 2.0
    for ch in word:
        for x0, y0, x1, y1 in _GLYPHS.get(ch, ()):
            du, dz = (x1 - x0) * unit, (y1 - y0) * unit
            roll = ft_city._roll_sign() * math.degrees(math.atan2(dz, du))
            m.box(col, wall.pt(u + (x0 + x1) / 2.0 * unit, 6.0, z0 + (y0 + y1) / 2.0 * unit), (12, math.hypot(du, dz) + t, t), rot=(0, wall.yaw, roll),
                  glow=glow)
        u += adv
    return width


def _stripes(m, x0, x1, y, z, depth, thick, cols, n, roll=0.0):
    """A striped canvas (an awning, a valance, a canopy): n strips side by side along X."""
    w = (x1 - x0) / float(n)
    for k in range(n):
        m.box(cols[k % len(cols)], (x0 + w * (k + 0.5), y, z), (w, depth, thick), rot=(0, 0, roll))


def _pennant(m, at, h=260, col=MAG):
    m.cyl(GOLD, at, 5, h, steps=5)
    _side(m, col, [(at[0], at[2] + h), (at[0] + 110, at[2] + h - 35), (at[0], at[2] + h - 70)], at[1] - 3, at[1] + 3)


def _star(m, col, x, z, R, y0, y1, glow=G1):
    pts = []
    for k in range(10):
        a = math.pi / 2.0 + k * math.pi / 5.0
        rr = R if k % 2 == 0 else R * 0.44
        pts.append((x + math.cos(a) * rr, z + math.sin(a) * rr))
    _side(m, col, pts, y0, y1, glow=glow)


# ------------------------------------------------------------------------------------------ the verge (low, unlit)
def _guest(m, x, y, r, z=0.0, s=1.0, arm=False, hat=None, balloon=None, sit=False):
    """One park guest, chunky and unlit: trousers, a shirt, a round head; s is the size (a child is 0.7). arm: one arm
    up; hat: a cap of that colour; balloon: one on a string, its top 1.95 m up at most (the verge's limit); sit: seated
    (the legs are left out, the rest drops by a leg)."""
    shirt, skin = r.choice(SHIRT), r.choice(SKIN)
    if sit:
        z -= 30.0 * s
    else:
        m.box(NAVY, (x, y, z + 38 * s), (34 * s, 26 * s, 76 * s))
    m.box(shirt, (x, y, z + 104 * s), (46 * s, 30 * s, 60 * s))
    m.sphere(skin, (x, y, z + 152 * s), 19 * s, steps=(3, 5))
    if arm:
        rod(m, shirt, (x - 22 * s, y, z + 124 * s), (x - 46 * s, y, z + 176 * s), 7 * s, steps=4)
    if hat:
        m.cyl(hat, (x, y, z + 164 * s), 21 * s, 13 * s, steps=6)
    if balloon:
        top = min(z + 232 * s, 195.0)
        m.box(GREY_L, (x + 24 * s, y, (z + 120 * s + top - 36) / 2.0), (3, 3, max(top - 36 - (z + 120 * s), 4.0)))
        m.sphere(balloon, (x + 24 * s, y, top - 19), 19, steps=(3, 5))


def queue_rails(m, seed=31):
    """Switchback queue rails under a striped canvas shade, the queue standing in them (eleven guests, two children, a
    balloon), a height-check post at the entrance: low and unlit."""
    r = random.Random(seed)
    L = 2200
    for j in range(3):
        y = -110 + j * 150
        m.box(GREY_L, (0, y, 104), (L, 8, 8))
        m.box(GREY_L, (0, y, 58), (L, 6, 6))
        for k in range(8):
            m.box(PURPLE, (-L / 2.0 + 40 + k * (L - 80) / 7.0, y, 54), (10, 10, 108))
    for sx in (-1, 1):
        m.box(WOOD_D, (560 + sx * 330, -110, 92), (12, 12, 184))
    _stripes(m, 230, 890, -110, 172, 14, 26, [MAG, CREAM], 6)
    m.box(CREAM, (-980, -190, 90), (18, 18, 180))
    m.box(RED, (-980, -200, 160), (120, 8, 36))
    for k in range(11):
        lane = k % 2
        x = -900 + k * 172 + r.uniform(-30, 30)
        _guest(m, x, -35 + lane * 150 + r.uniform(-16, 16), r, s=0.7 if k in (3, 8) else r.uniform(0.95, 1.05), arm=(k == 5),
               hat=(GOLD if k % 4 == 1 else None), balloon=(RED if k == 8 else None))


def park_fence(m):
    """The park's show hoarding, twenty metres of it: boards in candy stripes with a gold star on every other one, a
    gold rail, plum posts with gold knobs, a line of pennants between the posts (mover shades: they flutter), and a
    planter of flowers along its foot on the road's side. Like everything on the verge it is under 2 m and unlit:
    nothing nearer the road than 24 m may be taller or lit. It must read by its colours under the barrier's lamps."""
    L, n = 2000.0, 10
    w = L / n
    m.box(PURPLE, (0, 0, 18), (L, 34, 36))
    for k in range(n):
        x = -L / 2.0 + w * (k + 0.5)
        m.box(MAG if k % 2 == 0 else CREAM, (x, 0, 98), (w - 8, 16, 124))
        if k % 2 == 0:
            _star(m, AMBER, x, 100, 46, -14, -8, glow=0.0)
    m.box(GOLD, (0, 0, 166), (L, 22, 12))
    for k in range(n + 1):
        x = -L / 2.0 + k * w
        m.box(PLUM, (x, 0, 92), (20, 24, 184))
        if k % 2 == 0:
            m.sphere(GOLD, (x, 0, 184), 11, steps=(3, 5))
    cols = (mv(CYAN), mv(AMBER), mv(WHITE), mv(MINT))
    for k in range(2 * n):
        x = -L / 2.0 + w * (k + 0.5) / 2.0
        _side(m, cols[k % 4], [(x - 30, 192), (x, 146), (x + 30, 192)], -24, -18)
    m.box(GREY_L, (0, -62, 22), (L - 160, 56, 44))
    for k in range(8):
        x = -L / 2.0 + 200 + k * (L - 400) / 7.0
        m.sphere((MAG, AMBER, WHITE, PINE)[k % 4], (x, -62, 54), 40, scale=(1.5, 0.7, 0.6), steps=(3, 5))


def plaza_seats(m, seed=33):
    """Benches round a planter with a clipped cone tree, guests on them, a balloon seller with his bunch, a barrel bin,
    a finger post: the verge's furniture, under 2 m and unlit."""
    r = random.Random(seed)
    m.cyl(GREY_L, (0, 40, 0), 150, 50, steps=10)
    m.cyl(PINE, (0, 40, 50), 110, 140, steps=8, top_r=12)
    for s in (-1, 1):
        m.box(WOOD, (s * 330, 20, 46), (300, 56, 10))
        m.box(WOOD, (s * 330, 46, 80), (300, 10, 60))
        for e in (-1, 1):
            m.box(PURPLE, (s * 330 + e * 130, 24, 22), (12, 50, 44))
        for e in (-1, 1):
            _guest(m, s * 330 + e * 70, 14, r, z=52, sit=True, hat=(MAG if s * e > 0 else None))
    m.cyl(WOOD_D, (-560, 20, 0), 34, 90, steps=8)
    m.cyl(PLUM, (-560, 20, 90), 38, 12, steps=8)
    m.box(CREAM, (600, 30, 95), (14, 14, 190))
    for k, c in enumerate((PLUM, WOOD, TEAL)):
        m.box(c, (640 if k % 2 else 560, 30, 170 - k * 44), (150, 8, 34))
    _guest(m, -90, -70, r, s=1.02, arm=True)
    for k, c in enumerate((RED, AMBER, CYAN, MAG, MINT)):       # the seller's bunch, held high
        m.sphere(c, (-150 + (k % 3) * 34, -70 + (k // 3) * 24, 162 + (k % 2) * 16), 17, steps=(3, 5))


def cove_barrels(m, seed=4):
    """Barrels, crates, a rowing boat hauled up and an open chest of gold: Pirate Cove's clutter, low and unlit."""
    r = random.Random(seed)
    for k in range(5):
        x, y = -360 + k * 150 + r.uniform(-20, 20), r.uniform(-60, 60)
        m.cyl(WOOD, (x, y, 0), 44, 110, steps=8)
        m.cyl(WOOD_D, (x, y, 30), 46, 10, steps=8)
        m.cyl(WOOD_D, (x, y, 80), 46, 10, steps=8)
    for k in range(3):
        m.box(WOOD_D if k % 2 else WOOD, (430 + (k % 2) * 110, r.uniform(-40, 40), 50 + (k // 2) * 90), (100, 100, 100), rot=(0, r.uniform(-15, 15), 0))
    _side(m, WOOD, [(-200, 0), (180, 0), (260, 80), (-240, 80)], 150, 290)
    m.box(WOOD_D, (10, 220, 60), (300, 120, 12))
    m.cyl(WOOD_D, (-520, 60, 0), 10, 190, steps=5)
    _side(m, NAVY, [(-510, 186), (-330, 160), (-510, 134)], 57, 63)
    m.box(RED, (-460, -80, 36), (150, 90, 72))                  # the chest: red, banded in gold, heaped with it
    m.box(GOLD, (-460, -80, 74), (158, 98, 8))
    for k in range(4):
        m.sphere(GOLD, (-510 + k * 34, -80 + (k % 2) * 20, 90), 20, steps=(3, 5))
    tilt_box(m, RED, (-460, -35, 78), (-460, -5, 150), 150, 10)


def crowd(m, seed=21, n=16, L=1100.0):
    """A crowd before a stall or a gate, eleven metres of it: guests two deep, arms up here and there, children, a few
    balloons. Unlit and under 2.4 m (it throws no shadow). The guests bob, one after the next (animate())."""
    r = random.Random(seed)
    m.box(GREY_L, (0, 0, -15), (L + 60, 230, 30))
    for k in range(n):
        x = -L / 2.0 + (k + 0.5) * L / n + r.uniform(-18, 18)
        y = (-48 if k % 2 else 48) + r.uniform(-14, 14)
        _guest(m, x, y, r, s=0.72 if k % 5 == 3 else r.uniform(0.94, 1.06), arm=(k % 4 == 1), hat=(GOLD if k % 6 == 2 else None),
               balloon=((MAG, CYAN, AMBER)[k % 3] if k % 5 == 0 else None))


# ------------------------------------------------------------------------------------------ the midway
def game_stall(m, col=MAG, trim=CREAM, sign=AMBER, seed=1):
    """A midway game stall: an open front, a counter, a striped valance, prizes along the back wall under a strip
    light, a sign board edged with bulbs on the roof."""
    r = random.Random(seed)
    W, D, H = 640, 420, 300
    m.box(col, (0, D / 2.0 - 15, H / 2.0), (W, 30, H))
    for s in (-1, 1):
        m.box(col, (s * (W / 2.0 - 15), 0, H / 2.0), (30, D, H))
    m.box(NAVY, (0, 0, 10), (W, D, 20))
    m.box(trim, (0, -D / 2.0 + 20, 95), (W - 60, 56, 14))
    m.box(col, (0, -D / 2.0 + 22, 45), (W - 70, 36, 90))
    _stripes(m, -W / 2.0, W / 2.0, -D / 2.0 - 8, H - 44, 12, 88, [trim, col], 8)
    m.box(trim, (0, -40, H + 12), (W + 60, D + 140, 24))
    prizes = [CYAN, AMBER, MINT, RED, LAV, WHITE]
    for j in range(2):
        for k in range(6):
            m.sphere(r.choice(prizes), (-W / 2.0 + 90 + k * (W - 180) / 5.0, D / 2.0 - 56, 136 + j * 84), 28, steps=(3, 6))
    m.box(BULB, (0, D / 2.0 - 36, 262), (W - 90, 6, 22), glow=G2)
    m.box(NAVY, (0, -D / 2.0 - 30, H + 96), (W - 110, 16, 136))
    m.box(sign, (0, -D / 2.0 - 40, H + 96), (W - 200, 6, 64), glow=G3)
    _bulbs(m, (-W / 2.0 + 70, -D / 2.0 - 42, H + 170), (W / 2.0 - 70, -D / 2.0 - 42, H + 170), 9)


def food_stand(m):
    """A food stand under a striped box of popcorn the size of a car."""
    W, D, H = 520, 400, 280
    m.box(CREAM, (0, 30, H / 2.0), (W, D - 60, H))
    m.box(NAVY, (0, -D / 2.0 + 58, 170), (W - 120, 6, 130))
    m.box(BULB, (0, -D / 2.0 + 54, 170), (W - 150, 4, 100), glow=G2)
    m.box(RED, (0, -D / 2.0 + 30, 96), (W - 40, 70, 12))
    _stripes(m, -W / 2.0 - 20, W / 2.0 + 20, -D / 2.0 - 30, H - 12, 190, 12, [RED, CREAM], 8, roll=_awn(14))
    m.box(RED, (0, 30, H + 10), (W + 40, D - 20, 20))
    bz = H + 20
    for k in range(5):
        m.box(RED if k % 2 else WHITE, (-120 + k * 60, 40, bz + 130), (60, 200, 260))
    r = random.Random(7)
    for k in range(12):
        m.sphere(BULB if k % 3 else WHITE, (r.uniform(-130, 130), 40 + r.uniform(-80, 80), bz + 270 + r.uniform(0, 90)), r.uniform(40, 62), steps=(3, 6),
                 glow=G1)
    _bulbs(m, (-W / 2.0, -D / 2.0 - 110, H - 40), (W / 2.0, -D / 2.0 - 110, H - 40), 8)


def candy_cart(m):
    """A candy-floss cart on two big wheels, a parasol over it."""
    m.box(MAG, (0, 0, 130), (300, 170, 120))
    m.box(CREAM, (0, 0, 196), (320, 190, 12))
    for s in (-1, 1):
        rod(m, NAVY, (s * 90, -92, 70), (s * 90, -104, 70), 70, steps=10)
        rod(m, GOLD, (s * 90, -104, 70), (s * 90, -110, 70), 18, steps=6)
    m.box(WOOD_D, (-190, 0, 130), (110, 12, 12))
    m.cyl(GOLD, (60, 0, 200), 6, 230, steps=5)
    m.cyl(MAG, (60, 0, 400), 190, 60, steps=10, top_r=10)
    m.cyl(CREAM, (60, 0, 394), 194, 8, steps=10)
    for k, c in enumerate((MAG, CYAN, LAV)):
        m.cyl(WOOD, (-90 + k * 60, 20, 200), 3, 70, steps=4)
        m.sphere(c, (-90 + k * 60, 20, 300), 44, scale=(1, 1, 1.2), steps=(4, 6), glow=G1)


def high_striker(m):
    """A test-your-strength tower: lights that climb the scale to a gold bell and a burst of rays."""
    H = 920
    m.box(NAVY, (0, 0, 20), (280, 220, 40))
    m.box(RED, (0, 40, H / 2.0 + 20), (84, 30, H))
    m.box(CREAM, (0, 22, H / 2.0 + 20), (44, 6, H - 90))
    cols = [MINT, MINT, MINT, AMBER, AMBER, AMBER, ORANGE, ORANGE, RED]
    for k in range(9):
        m.box(cols[k], (0, 16, 120 + k * 84), (60, 6, 28), glow=G2 if k < 6 else G3)
    m.sphere(GOLD, (0, 30, H + 86), 62, steps=(5, 8), glow=G1)
    for k in range(8):
        a = k * math.pi / 4.0
        tilt_box(m, AMBER, (math.cos(a) * 90, 30, H + 86 + math.sin(a) * 90), (math.cos(a) * 190, 30, H + 86 + math.sin(a) * 190), 16, 10, glow=G3)
    m.box(WOOD_D, (0, -70, 52), (96, 70, 26))
    rod(m, WOOD, (130, -90, 0), (160, -60, 170), 6, steps=5)
    m.box(GREY, (160, -60, 184), (44, 64, 44))


def bandstand(m):
    """An eight-sided bandstand: a domed striped roof on thin posts, a ring of bulbs under the eaves."""
    R = 420
    m.cyl(GREY_L, (0, 0, 0), R + 40, 70, steps=8)
    m.cyl(PLUM, (0, 0, 70), R, 14, steps=8)
    for k in range(8):
        a = k * math.pi / 4.0 + math.pi / 8.0
        m.cyl(CREAM, (math.cos(a) * (R - 30), math.sin(a) * (R - 30), 84), 9, 330, steps=6)
        m.sphere(BULB, (math.cos(a) * (R + 10), math.sin(a) * (R + 10), 406), 16, steps=(3, 5), glow=G4)
    m.cyl(MAG, (0, 0, 414), R + 40, 40, steps=8)
    m.cyl(CREAM, (0, 0, 454), R + 20, 210, steps=8, top_r=70)
    m.cyl(MAG, (0, 0, 664), 70, 60, steps=8, top_r=8)
    _pennant(m, (0, 0, 724), 200, AMBER)
    m.box(GREY_L, (0, -R - 70, 30), (220, 120, 40))


def pond(m):
    """A round ornamental pond in a stone rim: lit water, a jet, lily lights. Small enough to stand on a slope."""
    m.cyl(GREY_L, (0, 0, -300), 760, 330, steps=12)
    m.cyl(WATER, (0, 0, -290), 700, 335, steps=12, glow=GW)
    m.cyl(CYAN, (0, 0, 45), 12, 240, steps=5, top_r=2, glow=G2)
    for k in range(5):
        a = k * 2 * math.pi / 5.0 + 0.4
        m.sphere([MAG, AMBER, CYAN][k % 3], (math.cos(a) * 430, math.sin(a) * 430, 52), 22, steps=(3, 5), glow=G4)
        m.cyl(PINE, (math.cos(a) * 430, math.sin(a) * 430, 45), 60, 5, steps=6)
    for k in range(3):
        a = k * 2 * math.pi / 3.0 + 1.3
        m.sphere(PINE, (math.cos(a) * 800, math.sin(a) * 800, 60), 80, steps=(4, 6))


# ------------------------------------------------------------------------------------------ the front gate and its plaza
def ticket_gate(m):
    """The park's gate: two striped towers, the name in lights on the arch between them, ticket booths under it."""
    S = 1150
    for s in (-1, 1):
        m.box(CREAM, (s * S, 0, 450), (300, 300, 900))
        for k in range(4):
            m.box(MAG, (s * S, 0, 150 + k * 210), (306, 306, 70))
        m.cyl(MAG, (s * S, 0, 900), 230, 330, steps=8, top_r=10)
        m.cyl(CREAM, (s * S, 0, 892), 236, 10, steps=8)
        _pennant(m, (s * S, 0, 1230), 240, AMBER if s < 0 else CYAN)
        m.box(BULB, (s * S, -152, 560), (120, 4, 200), glow=G2)
    m.box(NAVY, (0, 0, 700), (2 * S - 300, 110, 260))
    m.box(MAG, (0, 0, 838), (2 * S - 300, 130, 16))
    m.box(MAG, (0, 0, 562), (2 * S - 300, 130, 16))
    _letters(m, 'COASTER COVE', -55, 612, 170, AMBER)
    _bulbs(m, (-S + 170, -62, 858), (S - 170, -62, 858), 11)
    _bulbs(m, (-S + 170, -62, 544), (S - 170, -62, 544), 11)
    for k in range(3):
        x = -560 + k * 560
        m.box(PLUM, (x, 20, 130), (200, 200, 260))
        m.box(BULB, (x, -82, 160), (130, 4, 90), glow=G2)
        m.cyl(MAG, (x, 20, 260), 150, 90, steps=8, top_r=10)
        for e in (-1, 1):
            m.box(GREY_L, (x + e * 190, -40, 50), (8, 120, 8))
            m.box(GREY_L, (x + e * 190, -40, 100), (8, 120, 8))


def fountain(m):
    """The plaza's fountain: three tiers, lit water, a ring of jets."""
    m.cyl(GREY_L, (0, 0, 0), 460, 70, steps=14)
    m.cyl(CYAN, (0, 0, 70), 420, 6, steps=14, glow=G1)
    m.cyl(GREY_L, (0, 0, 70), 90, 170, steps=8)
    m.cyl(GREY_L, (0, 0, 240), 230, 30, steps=12, top_r=250)
    m.cyl(CYAN, (0, 0, 270), 236, 6, steps=12, glow=G1)
    m.cyl(GREY_L, (0, 0, 270), 50, 150, steps=8)
    m.cyl(GREY_L, (0, 0, 420), 110, 24, steps=10, top_r=124)
    m.cyl(CYAN, (0, 0, 444), 10, 240, steps=5, top_r=2, glow=G2)
    for k in range(8):
        a = k * math.pi / 4.0
        tilt_box(m, CYAN, (math.cos(a) * 330, math.sin(a) * 330, 76), (math.cos(a) * 150, math.sin(a) * 150, 300), 10, 10, glow=G2)
        m.sphere(MAG if k % 2 else AMBER, (math.cos(a) * 480, math.sin(a) * 480, 60), 16, steps=(3, 5), glow=G4)


def mascot(m):
    """STARRY, the park's own mascot: a star in a ringmaster's coat and top hat, waving from a striped drum. A mechanical
    figure: his raised arm is in the mover shade (it waves from the shoulder) and his head and hat in the second one
    (they nod from the neck)."""
    m.cyl(CREAM, (0, 0, 0), 190, 130, steps=12)
    for k in range(6):
        a = k * math.pi / 3.0
        m.box(MAG, (math.cos(a) * 186, math.sin(a) * 186, 65), (10, 90, 130), rot=(0, math.degrees(a), 0))
    for s in (-1, 1):
        m.cyl(NAVY, (s * 60, 0, 130), 34, 150, steps=6)
        m.box(WHITE, (s * 60, -20, 140), (80, 120, 30))
    m.sphere(MAG, (0, 0, 380), 150, scale=(1.0, 0.8, 1.15), steps=(5, 8))
    for k in range(3):
        m.sphere(GOLD, (0, -118, 320 + k * 60), 16, steps=(3, 5))
    m.box(WHITE, (0, -110, 500), (120, 30, 40))
    rod(m, MAG, (-130, 0, 440), (-250, -20, 330), 30, steps=6)
    rod(m, mv(MAG), (130, 0, 450), (270, -20, 620), 30, steps=6)
    m.sphere(WHITE, (-262, -22, 318), 40, steps=(4, 6))
    m.sphere(mv(WHITE), (282, -22, 640), 44, steps=(4, 6))
    _star(m, mv2(AMBER), 0, 720, 230, -45, 45)
    for s in (-1, 1):
        m.sphere(mv2(NAVY), (s * 50, -50, 740), 20, steps=(3, 5))
    m.box(mv2(NAVY), (0, -48, 672), (90, 8, 14))
    m.cyl(mv2(NAVY), (0, 0, 900), 96, 14, steps=10)
    m.cyl(mv2(NAVY), (0, 0, 910), 66, 130, steps=10)
    m.cyl(mv2(MAG), (0, 0, 922), 68, 24, steps=10)
    for a in (-1, 1):
        m.sphere(CYAN if a < 0 else AMBER, (a * 260, -200, 20), 26, steps=(3, 5), glow=G4)


def big_wheel(m, R=3000):
    """The observation wheel: legs, axle and loading deck, and the wheel itself (two lit rims, spokes, twelve
    gondolas) in mover colours: it turns about Y through the hub."""
    hub = R + 400
    for sy in (-1, 1):
        for sx in (-1, 1):
            tilt_box(m, CREAM, (sx * 1500, sy * 290, 0), (0, sy * 190, hub), 110, 90)
        tilt_box(m, CREAM, (-760, sy * 240, hub / 2.0), (760, sy * 240, hub / 2.0), 70, 60)
    rod(m, GOLD, (0, -300, hub), (0, 300, hub), 110, steps=10)
    m.box(PURPLE, (0, 0, 40), (3400, 900, 80))
    m.box(PLUM, (0, -420, 190), (900, 220, 300))
    m.box(BULB, (0, -532, 200), (600, 4, 140), glow=G2)
    _stripes(m, -500, 500, -470, 350, 320, 12, [MAG, CREAM], 8, roll=_awn(12))
    _bulbs(m, (-1650, -440, 100), (1650, -440, 100), 12)
    for y in (-120, 120):
        _ring(m, mv(MAG) if y < 0 else mv(CYAN), (0, y, hub), R, 34, steps=24, glow=G2)
    _ring(m, mv(CREAM), (0, 0, hub), R * 0.55, 22, steps=20)
    n = 12
    for k in range(n):
        a = 2 * math.pi * k / n + 0.2
        cx, cz = math.cos(a), math.sin(a)
        tilt_box(m, mv(CREAM), (cx * 130, 0, hub + cz * 130), (cx * R, 0, hub + cz * R), 26, 26)
        m.box(mv([AMBER, MAG, CYAN, CREAM][k % 4]), (cx * R, 0, hub + cz * R - 130), (190, 200, 170), glow=G1)
        m.box(mv(NAVY), (cx * R, -102, hub + cz * R - 110), (150, 4, 70))
    m.sphere(mv(AMBER), (0, -150, hub), 150, scale=(1, 0.5, 1), steps=(5, 8), glow=G3)


# ------------------------------------------------------------------------------------------ Pirate Cove
def pirate_ship(m, axle=1350, drop=1080):
    """The swinging ship: two A-frames, the axle between them, a boarding deck with rope rails; and the ship itself
    (hull, striped rail, benches, mast and flag, hung from the axle) in mover colours: it swings about Y through
    the axle. Drawn at the middle of its swing."""
    for sy in (-1, 1):
        for sx in (-1, 1):
            tilt_box(m, WOOD, (sx * 620, sy * 330, 0), (0, sy * 300, axle), 60, 60)
        tilt_box(m, WOOD_D, (-330, sy * 318, axle * 0.47), (330, sy * 318, axle * 0.47), 40, 40)
        m.sphere(GOLD, (0, sy * 300, axle + 40), 46, steps=(4, 6))
    rod(m, WOOD_D, (0, -330, axle), (0, 330, axle), 34, steps=8)
    m.box(WOOD_D, (0, -420, 100), (1500, 170, 20))
    for k in range(6):
        x = -700 + k * 280
        m.box(WOOD, (x, -500, 160), (14, 14, 120))
        m.box(WOOD_D, (x, -420, 50), (20, 150, 100))
    m.box(CREAM, (0, -500, 214), (1420, 8, 8))
    m.box(NAVY, (-900, -420, 250), (30, 30, 500))
    m.box(RED, (-900, -438, 440), (260, 8, 110))
    m.box(AMBER, (-900, -444, 440), (200, 4, 50), glow=G3)
    _bulbs(m, (-620, -334, 40), (0, -304, axle), 7, [BULB, AMBER])
    _bulbs(m, (620, -334, 40), (0, -304, axle), 7, [BULB, AMBER])
    z = axle - drop
    _side(m, mv(WOOD_D), [(-860, z + 250), (-700, z), (640, z), (900, z + 250), (1040, z + 330), (880, z + 330)], -170, 170)
    _side(m, mv(RED), [(-860, z + 250), (900, z + 250), (900, z + 300), (-860, z + 300)], -178, 178)
    m.box(mv(AMBER), (0, -180, z + 276), (1500, 4, 16), glow=G2)
    for k in range(6):
        m.box(mv(CREAM), (-600 + k * 240, 0, z + 270), (70, 300, 30))
    for sy in (-1, 1):
        rod(m, mv(WOOD), (0, sy * 150, axle), (-420, sy * 150, z + 280), 20, steps=6)
        rod(m, mv(WOOD), (0, sy * 150, axle), (420, sy * 150, z + 280), 20, steps=6)
    m.cyl(mv(WOOD), (40, 0, z + 250), 16, 560, steps=6)
    _side(m, mv(NAVY), [(60, z + 800), (330, z + 730), (60, z + 650)], -3, 3)
    m.sphere(mv(GOLD), (1050, 0, z + 350), 40, steps=(4, 6), glow=G1)
    _bulbs(m, (-820, -182, z + 320), (880, -182, z + 320), 10, [mv(BULB), mv(MAG)])


def skull_rock(m, seed=3):
    """The cove's landmark: a rock with two lit caves for eyes and a row of pale stones for teeth over a lit cave
    mouth, torches either side, a strip of falling water down its flank."""
    r = random.Random(seed)
    m.sphere(ROCK, (0, 120, 420), 760, scale=(1.15, 0.9, 0.95), steps=(6, 9))
    m.sphere(ROCK_D, (-520, 200, 200), 520, scale=(1.2, 1.0, 0.8), steps=(5, 8))
    m.sphere(ROCK_D, (560, 240, 240), 560, scale=(1.1, 1.0, 0.85), steps=(5, 8))
    for s in (-1, 1):
        rod(m, NAVY, (s * 270, -540, 620), (s * 270, -440, 620), 150, steps=10)
        rod(m, AMBER, (s * 270, -546, 620), (s * 270, -540, 620), 104, steps=10, glow=G2)
    _side(m, NAVY, [(-60, 400), (60, 400), (0, 520)], -610, -520)
    for k in range(7):
        m.box(CREAM, (-270 + k * 90, -560, 240 + (10 if k % 2 else 0)), (64, 140, 110))
    m.box(NAVY, (0, -540, 100), (560, 130, 190))
    m.box(AMBER, (0, -607, 96), (420, 4, 120), glow=G1)
    for s in (-1, 1):
        m.cyl(WOOD_D, (s * 620, -600, 0), 12, 300, steps=5)
        m.sphere(ORANGE, (s * 620, -600, 330), 40, scale=(1, 1, 1.4), steps=(4, 6), glow=G4)
    m.box(CYAN, (700, -322, 215), (120, 10, 420), rot=(0, -30, 0), glow=G2)
    for k in range(5):
        m.sphere(ROCK_D, (r.uniform(-900, 900), -620 + r.uniform(-60, 60), 40), r.uniform(70, 130), scale=(1.2, 1, 0.7), steps=(4, 6))


# ------------------------------------------------------------------------------------------ Rocket Land
def jet_ride(m, hub=900):
    """The rocket ride: a round deck, a fence, the tower, the operator's booth; and what turns on the tower (the hub,
    six arms at different heights with a rocket on each, a ringed planet on top) in mover colours: about Z."""
    m.cyl(PURPLE, (0, 0, 0), 760, 50, steps=16)
    m.cyl(CYAN, (0, 0, 50), 700, 6, steps=16, glow=G1)
    m.cyl(CREAM, (0, 0, 50), 120, hub - 110, steps=10)
    for k in range(4):
        m.cyl(MAG, (0, 0, 160 + k * 170), 126, 50, steps=10)
    for k in range(12):
        a = k * math.pi / 6.0
        m.box(GREY_L, (math.cos(a) * 760, math.sin(a) * 760, 100), (10, 10, 110))
    m.box(PLUM, (-520, -520, 150), (220, 220, 200))
    m.box(BULB, (-520, -632, 170), (140, 4, 90), glow=G2)
    m.box(MAG, (-520, -520, 258), (260, 260, 16))
    m.cyl(mv(CYAN), (0, 0, hub - 60), 170, 120, steps=10, glow=G1)
    m.sphere(mv(MAG), (0, 0, hub + 230), 170, steps=(5, 8), glow=G1)
    m.cyl(mv(AMBER), (0, 0, hub + 222), 270, 14, steps=14, glow=G2)
    cols = [RED, CYAN, AMBER, MINT, WHITE, LAV]
    for k in range(6):
        a = k * math.pi / 3.0
        cx, cy = math.cos(a), math.sin(a)
        z = 420 + (k % 3) * 170
        rod(m, mv(CREAM), (cx * 150, cy * 150, hub - 10), (cx * 640, cy * 640, z), 22, steps=6)
        tx, ty = -cy, cx
        tail = (cx * 640 - tx * 170, cy * 640 - ty * 170, z)
        nose = (cx * 640 + tx * 170, cy * 640 + ty * 170, z)
        rod(m, mv(cols[k]), tail, nose, 62, steps=8)
        rod(m, mv(CREAM), nose, (nose[0] + tx * 120, nose[1] + ty * 120, z), 44, steps=8)
        m.sphere(mv(RED), (nose[0] + tx * 140, nose[1] + ty * 140, z), 34, steps=(3, 6))
        m.box(mv(MAG), (tail[0], tail[1], z + 60), (120, 12, 110), rot=(0, math.degrees(a) + 90, 0))
        m.sphere(mv(ORANGE), (tail[0] - tx * 30, tail[1] - ty * 30, z), 40, steps=(3, 6), glow=G4)


def saucer(m):
    """A flying-saucer pavilion on three legs: a lit dome, a ring of lights round its rim, a ramp down to the ground."""
    for k in range(3):
        a = k * 2 * math.pi / 3.0 + 0.5
        rod(m, GREY_L, (math.cos(a) * 520, math.sin(a) * 520, 0), (math.cos(a) * 300, math.sin(a) * 300, 330), 26, steps=6)
        m.cyl(GREY, (math.cos(a) * 520, math.sin(a) * 520, 0), 70, 16, steps=8)
    m.cyl(GREY_L, (0, 0, 300), 300, 90, steps=14, top_r=640)
    m.cyl(LAV, (0, 0, 390), 640, 60, steps=14)
    m.cyl(GREY_L, (0, 0, 450), 640, 70, steps=14, top_r=320)
    m.sphere(CYAN, (0, 0, 510), 300, scale=(1, 1, 0.75), steps=(5, 10), glow=G1)
    for k in range(14):
        a = k * 2 * math.pi / 14.0
        m.sphere([AMBER, MAG, CYAN][k % 3], (math.cos(a) * 650, math.sin(a) * 650, 420), 24, steps=(3, 5), glow=G4)
    tilt_box(m, GREY, (0, -200, 300), (0, -760, 0), 200, 16)
    m.box(BULB, (0, -296, 240), (140, 4, 120), glow=G2)
    m.cyl(GREY, (0, 0, 730), 6, 220, steps=5)
    m.sphere(RED, (0, 0, 960), 26, steps=(3, 5), glow=G4)


def planet_posts(m):
    """Planets on poles: three globes at different heights, one ringed, small lights between."""
    for x, h, r_, c in ((-360, 520, 130, MAG), (0, 760, 170, AMBER), (380, 420, 110, CYAN)):
        m.cyl(GREY_L, (x, 0, 0), 12, h, steps=6)
        m.sphere(c, (x, 0, h + r_ * 0.8), r_, steps=(5, 8), glow=G1)
        m.cyl(GREY, (x, 0, 0), 60, 14, steps=8)
    m.cyl(CREAM, (0, 0, 760 + 126), 260, 12, steps=14)
    _bulbs(m, (-360, -10, 300), (380, -10, 300), 6, [BULB, CYAN], r=10)


# ------------------------------------------------------------------------------------------ the funhouse and the back lot
def funhouse(m):
    """A funhouse: a front in patchwork panels, a grinning doorway with teeth, round eye windows, a barrel to walk
    through, a turret gone askew, its name on a board across the roof."""
    W, D, H = 1000, 700, 820
    m.box(LAV, (0, 120, H / 2.0), (W, D, H))
    cols = [MINT, MAG, AMBER, CYAN, CREAM, PLUM]
    for j in range(3):
        for k in range(4):
            if (j + k) % 2 == 0:
                m.box(cols[(j * 2 + k) % 6], (-W / 2.0 + 125 + k * 250, -233, 140 + j * 260), (240, 8, 250))
    _side(m, MAG, [(-300, 0), (300, 0), (340, 240), (200, 400), (-200, 400), (-340, 240)], -252, -236)
    _side(m, NAVY, [(-230, 0), (230, 0), (260, 220), (150, 330), (-150, 330), (-260, 220)], -262, -252)
    for k in range(5):
        m.box(CREAM, (-200 + k * 100, -268, 296), (70, 10, 70))
        m.box(CREAM, (-200 + k * 100, -268, 34), (70, 10, 60))
    rod(m, PLUM, (0, -430, 170), (0, -272, 170), 150, steps=10)
    for k in range(4):
        rod(m, AMBER, (0, -431 + k * 40, 170), (0, -421 + k * 40, 170), 154, steps=10)
    for s in (-1, 1):
        rod(m, CREAM, (s * 300, -250, 590), (s * 300, -234, 590), 110, steps=10)
        rod(m, CYAN, (s * 300, -254, 590), (s * 300, -250, 590), 70, steps=10, glow=G2)
        rod(m, NAVY, (s * 300 + 20, -258, 590), (s * 300 + 20, -254, 590), 28, steps=8)
    m.box(MINT, (330, 220, H + 230), (260, 260, 460), rot=(-9, 0, 0))
    m.cyl(MAG, (296, 220, H + 440), 220, 320, steps=8, top_r=8, rot=(-9, 0, 0))
    m.box(NAVY, (-120, -150, H + 100), (720, 20, 170))
    _letters(m, 'FUNHOUSE', -160, H + 58, 90, AMBER, x=-120.0)
    _bulbs(m, (-470, -250, H + 20), (-470, -250, 60), 6, [BULB, MAG])
    _bulbs(m, (470, -250, H + 20), (470, -250, 60), 6, [BULB, CYAN])


def swing_ride(m, top=1050):
    """The chair swing: a deck, a fence, the striped column; and what turns on it (the crown, its ring of bulbs, ten
    chairs flung out on their chains) in mover colours: about Z."""
    m.cyl(PURPLE, (0, 0, 0), 620, 50, steps=14)
    m.cyl(CREAM, (0, 0, 50), 100, top - 50, steps=10)
    for k in range(5):
        m.cyl(MAG if k % 2 else TEAL, (0, 0, 150 + k * 180), 106, 60, steps=10)
    for k in range(10):
        a = k * math.pi / 5.0
        m.box(GREY_L, (math.cos(a) * 620, math.sin(a) * 620, 100), (10, 10, 110))
    m.cyl(mv(MAG), (0, 0, top), 560, 60, steps=14)
    m.cyl(mv(CREAM), (0, 0, top + 60), 560, 250, steps=14, top_r=60)
    m.cyl(mv(MAG), (0, 0, top + 310), 60, 70, steps=8, top_r=6)
    m.sphere(mv(AMBER), (0, 0, top + 400), 40, steps=(4, 6), glow=G4)
    for k in range(14):
        a = k * 2 * math.pi / 14.0
        m.sphere(mv([BULB, MAG, CYAN][k % 3]), (math.cos(a) * 566, math.sin(a) * 566, top + 30), 20, steps=(3, 5), glow=G4)
    cols = [RED, CYAN, AMBER, MINT, WHITE]
    for k in range(10):
        a = k * math.pi / 5.0
        cx, cy = math.cos(a), math.sin(a)
        seat = (cx * 860, cy * 860, top - 480 + (k % 2) * 60)
        rod(m, mv(GREY_L), (cx * 520, cy * 520, top), seat, 5, steps=4)
        m.box(mv(cols[k % 5]), seat, (70, 70, 50), rot=(0, math.degrees(a), 0))
        m.box(mv(NAVY), (seat[0], seat[1], seat[2] + 60), (40, 40, 70), rot=(0, math.degrees(a), 0))


def maintenance(m, seed=5):
    """The back lot: a ribbed shed with a roller door, spare coaster cars on a rack, a generator, cable drums, bins,
    a ladder left against the wall, one amber lamp over the door. No colour: this is where the park is not on show."""
    W, D, H = 1100, 700, 430
    m.box(SHED, (0, 150, H / 2.0), (W, D, H))
    for k in range(11):
        m.box(SHED_D, (-W / 2.0 + 50 + k * 100, -202, H / 2.0), (16, 8, H))
    _side(m, SHED_D, [(-W / 2.0 - 30, H), (W / 2.0 + 30, H), (0, H + 170)], -210, 510)
    m.box(NAVY, (-200, -206, 170), (420, 8, 340))
    for k in range(6):
        m.box(GREY, (-200, -212, 40 + k * 56), (410, 4, 8))
    m.box(GREY_L, (330, -206, 110), (110, 8, 220))
    m.sphere(AMBER, (-200, -230, 380), 22, steps=(3, 5), glow=G3)
    for k in range(3):
        z = 40 + k * 130
        m.box([RED, TEAL, AMBER][k], (-900, 60, z + 50), (260, 150, 90))
        m.box(NAVY, (-900, 60, z + 104), (200, 110, 30))
        m.box(GREY, (-900, 60, z - 4), (300, 180, 10))
    for s in (-1, 1):
        m.box(GREY, (-900 + s * 150, 60, 200), (10, 10, 420))
    m.box(GEN, (840, -40, 110), (360, 200, 220))
    m.cyl(GREY, (960, -40, 220), 18, 200, steps=6)
    m.box(AMBER, (840, -142, 160), (120, 4, 40), glow=G1)
    for k in range(3):
        rod(m, WOOD if k % 2 else WOOD_D, (520 + k * 120, -330, 60), (520 + k * 120, -250, 60), 60, steps=8)
    for k in range(3):
        m.cyl([GEN, SHED, PLUM][k], (-470 + k * 80, -300, 0), 34, 100, steps=7)
    tilt_box(m, GREY_L, (620, -250, 0), (600, -206, 380), 50, 6)
    for k in range(5):
        m.box(GREY_L, (-600 + k * 300, -420, 110), (8, 8, 220))
    for z in (60, 130, 200):
        m.box(GREY, (0, -420, z), (1220, 4, 6))


# ------------------------------------------------------------------------------------------ the lagoon under the coaster
def lake(m, seed=6, serpent=False, L=4400, W=7000):
    """One lake of the lagoon the coaster flies over, 44 m along the rails and 70 m across them: dark water with streaks
    of the park's lights on it inside a sand bank, an island with a lamp, pedal boats, a humped bridge strung with bulbs,
    jets, floating lanterns; the second kind has the lagoon's sea serpent arching out of it. Its z = 0 is the hollow's
    own ground; the water stands 1.3 m over that, and the bank and the water go down well below it (see THE LAGOON)."""
    r = random.Random(seed)
    zw = 130.0
    hx, hy, c = L / 2.0, W / 2.0, 700.0
    out = [(-hx + c, -hy), (hx - c, -hy), (hx, -hy + c), (hx, hy - c), (hx - c, hy), (-hx + c, hy), (-hx, hy - c), (-hx, -hy + c)]
    m.plan(WATER, out, -380, zw, glow=GW)
    for a, b in zip(out, out[1:] + out[:1]):
        tilt_box(m, SAND, (a[0], a[1], zw - 250), (b[0], b[1], zw - 250), 280, 580)
    for k in range(10):
        m.box([MAG, AMBER, CYAN][k % 3], (r.uniform(-1500, 1500), r.uniform(-2900, 2900), zw + 3), (r.uniform(260, 620), 26, 4), glow=G2)
    s = 1 if seed % 2 else -1
    ix, iy = r.uniform(-400, 400), s * 1500
    m.sphere(PINE, (ix, iy, zw), 420, scale=(1.3, 1.0, 0.3), steps=(4, 8))
    m.cyl(CREAM, (ix, iy, zw + 110), 8, 330, steps=5)
    m.sphere(BULB, (ix, iy, zw + 460), 34, steps=(4, 6), glow=G4)
    m.sphere(PINE, (ix - 160, iy + 90, zw + 170), 110, steps=(4, 6))
    m.sphere(PINE, (ix + 190, iy - 60, zw + 150), 90, steps=(4, 6))
    for x, y, col in ((-900, -s * 700, MAG), (500, -s * 1900, CYAN), (-300, s * 2700, AMBER)):
        m.box(WHITE, (x, y, zw + 26), (200, 110, 50))
        m.box(col, (x, y, zw + 126), (190, 120, 10))
        for sx in (-1, 1):
            m.box(WHITE, (x + sx * 85, y, zw + 76), (8, 8, 100))
    arch = [(-700, 30), (-520, 160), (-260, 250), (0, 280), (260, 250), (520, 160), (700, 30), (560, 30), (430, 110), (220, 180), (0, 202), (-220, 180),
            (-430, 110), (-560, 30)]
    by = -s * 2500
    _side(m, CREAM, [(x, z + zw - 40) for x, z in arch], by - 60, by + 60)
    for k in range(7):
        t = (k + 0.5) / 7.0
        m.sphere(BULB if k % 2 else MAG, (-620 + 1240 * t, by - 72, zw + 250 * math.sin(math.pi * t) + 50), 16, steps=(3, 5), glow=G4)
    for k in range(3):
        m.cyl(CYAN, (r.uniform(-1600, 1600), r.uniform(-600, 600), zw), 14, r.uniform(260, 420), steps=5, top_r=2, glow=G2)
    for k in range(8):
        m.sphere([BULB, MAG, CYAN, AMBER][k % 4], (r.uniform(-1900, 1900), r.uniform(-3100, 3100), zw + 14), 18, steps=(3, 5), glow=G4)
    if serpent:
        sy = s * 300
        for k, x in enumerate((-700, -60, 520)):
            _ring(m, MINT if k % 2 else TEAL, (x, sy, zw - 60), 300 - k * 30, 70 - k * 8, steps=12, tube=5)
        rod(m, TEAL, (900, sy, zw - 60), (1120, sy, zw + 260), 62, steps=7)
        m.sphere(TEAL, (1200, sy, zw + 300), 110, scale=(1.5, 1.0, 0.9), steps=(4, 7))
        for e in (-1, 1):
            m.sphere(AMBER, (1260, sy + e * 70, zw + 340), 22, steps=(3, 5), glow=G4)
        _side(m, MAG, [(1040, zw + 330), (1120, zw + 470), (1160, zw + 340)], sy - 6, sy + 6)


def path_lights(m, seed=8):
    """A stretch of lit footpath: pale slabs, lamps on tall poles with strings of bulbs between them, flower beds."""
    r = random.Random(seed)
    for k in range(4):
        m.box(SAND, (-1200 + k * 800, 60 * math.sin(k * 1.3), 20), (860, 260, 40), rot=(0, 8 * math.cos(k * 1.1), 0))
    poles = [(-1500, 190, 520), (-500, -190, 560), (500, 190, 520), (1500, -190, 560)]
    for x, y, h in poles:
        m.cyl(CREAM, (x, y, -200), 9, h + 200, steps=5)
        m.sphere(BULB, (x, y, h + 26), 30, steps=(4, 6), glow=G4)
    for a, b in zip(poles, poles[1:]):
        mid = ((a[0] + b[0]) / 2.0, (a[1] + b[1]) / 2.0, min(a[2], b[2]) - 90)
        _bulbs(m, a, mid, 4, [MAG, AMBER, CYAN], r=10)
        _bulbs(m, mid, b, 4, [CYAN, AMBER, MAG], r=10)
    for k in range(5):
        x, y = r.uniform(-1500, 1500), r.choice((-1, 1)) * r.uniform(260, 420)
        m.sphere(PINE, (x, y, 20), 120, scale=(1.4, 1.0, 0.35), steps=(4, 6))
        m.sphere([MAG, AMBER, LAV][k % 3], (x, y, 60), 60, scale=(1.5, 1.1, 0.4), steps=(3, 6))


def coaster_hill(m, L=6400, H=3000):
    """A length of the park's own coaster: hills and a loop on a forest of white supports, the rail picked out in
    pink, a train on the first climb, a station under an awning. Runs along X; its supports go on down below z = 0."""
    pts = []
    n = 24
    for k in range(n + 1):
        t = k / float(n)
        z = 500 + (H - 500) * (0.5 - 0.5 * math.cos(2 * math.pi * t * 1.5)) * (0.55 + 0.45 * math.cos(math.pi * t * 0.9))
        pts.append((-L / 2.0 + L * t, 0.0, max(z, 420.0)))
    for a, b in zip(pts, pts[1:]):
        tilt_box(m, WHITE, a, b, 130, 40)
        tilt_box(m, MAG, (a[0], a[1] - 70, a[2] + 30), (b[0], b[1] - 70, b[2] + 30), 12, 12, glow=G2)
    for p in pts[::2]:
        m.box(CREAM, (p[0], 0, (p[2] - 500) / 2.0), (34, 34, p[2] + 500))
        tilt_box(m, CREAM, (p[0] - 150, 110, -200), (p[0] + 150, -110, p[2] * 0.72), 18, 18)
    _ring(m, WHITE, (L * 0.36, 0, 1500), 1080, 44, steps=24)
    _ring(m, CYAN, (L * 0.36, -60, 1500), 1080, 12, steps=24, glow=G2)
    for k in range(4):
        m.box([RED, AMBER, CYAN, MAG][k], (pts[4][0] + k * 190, 0, pts[4][2] + 110 + k * 70), (170, 150, 110))
    m.box(PLUM, (-L / 2.0 + 500, -300, 150), (900, 400, 300))
    m.box(BULB, (-L / 2.0 + 500, -502, 170), (600, 4, 130), glow=G2)
    _stripes(m, -L / 2.0 + 50, -L / 2.0 + 950, -360, 330, 560, 12, [MAG, CREAM], 8, roll=_awn(10))


# ------------------------------------------------------------------------------------------ the names of the lands
def land_sign(m, word, col=AMBER, trim=MAG, emblem=None):
    """A land's name in lights: a board on two striped posts, bulbs along its top, the land's emblem over it."""
    h = 120.0
    w = len(word) * 5.6 * h / 6.0 + 160.0
    half = w / 2.0
    for s in (-1, 1):
        m.box(CREAM, (s * (half + 30), 0, 330), (60, 60, 660))
        for k in range(3):
            m.box(trim, (s * (half + 30), 0, 120 + k * 200), (64, 64, 60))
        m.sphere(GOLD, (s * (half + 30), 0, 696), 44, steps=(4, 6))
    m.box(NAVY, (0, 0, 560), (w, 30, 230))
    m.box(trim, (0, 0, 684), (w + 60, 44, 18))
    m.box(trim, (0, 0, 438), (w + 60, 44, 18))
    _letters(m, word, -15, 500, h, col)
    _bulbs(m, (-half, -26, 706), (half, -26, 706), max(6, int(w / 120)))
    if emblem:
        emblem(m, 0.0, 820.0)


def _em_star(m, x, z):
    _star(m, AMBER, x, z, 120, -14, 14, glow=G3)


def _em_skull(m, x, z):
    tilt_box(m, CREAM, (x - 130, 0, z - 80), (x + 130, 0, z + 80), 16, 26)
    tilt_box(m, CREAM, (x - 130, 0, z + 80), (x + 130, 0, z - 80), 16, 26)
    m.sphere(CREAM, (x, -10, z + 10), 80, scale=(1, 0.7, 1.05), steps=(4, 7))
    for e in (-1, 1):
        m.sphere(NAVY, (x + e * 30, -62, z + 22), 20, steps=(3, 5))


def _em_wave(m, x, z):
    for k in range(3):
        _ring(m, CYAN if k % 2 == 0 else TEAL, (x - 150 + k * 150, 0, z - 90), 90, 14, steps=12, tube=4, glow=G2)
    m.sphere(AMBER, (x, 0, z + 60), 36, steps=(4, 6), glow=G4)


def _em_rocket(m, x, z):
    rod(m, CREAM, (x - 110, 0, z - 60), (x + 70, 0, z + 60), 40, steps=8)
    rod(m, RED, (x + 70, 0, z + 60), (x + 130, 0, z + 100), 26, steps=6)
    m.sphere(ORANGE, (x - 135, 0, z - 78), 36, steps=(3, 6), glow=G4)
    tilt_box(m, MAG, (x - 120, 0, z - 10), (x - 60, 0, z - 110), 12, 70)


def _em_hat(m, x, z):
    m.cyl(NAVY, (x, 0, z - 80), 120, 16, steps=10)
    m.cyl(NAVY, (x, 0, z - 70), 80, 160, steps=10)
    m.cyl(MAG, (x, 0, z - 54), 82, 30, steps=10)
    _star(m, AMBER, x, z + 20, 44, -90, -80, glow=G3)


def _em_nose(m, x, z):
    m.cyl(CREAM, (x, 0, z - 90), 150, 26, steps=12)
    m.cyl(CYAN, (x, 0, z - 64), 110, 20, steps=12)
    m.sphere(RED, (x, 0, z + 40), 86, steps=(5, 8), glow=G2)


# ------------------------------------------------------------------------------------------ the horizon (built from z = 0 up)
def far_coaster(m, L=26000, H=8200):
    """The big coaster on the skyline: a quarter of a kilometre of hills and a loop."""
    pts = []
    n = 28
    for k in range(n + 1):
        t = k / float(n)
        z = 1400 + (H - 1400) * (0.5 - 0.5 * math.cos(2 * math.pi * t * 2.5)) * (1.0 - 0.55 * t)
        pts.append((-L / 2.0 + L * t, 0.0, z))
    for a, b in zip(pts, pts[1:]):
        tilt_box(m, WHITE, a, b, 420, 150)
        tilt_box(m, MAG, (a[0], -240, a[2] + 110), (b[0], -240, b[2] + 110), 60, 60, glow=G2)
    for p in pts[::2]:
        m.box(CREAM, (p[0], 0, p[2] / 2.0), (130, 130, p[2]))
        tilt_box(m, CREAM, (p[0] - 500, 300, 0), (p[0] + 500, -300, p[2] * 0.7), 70, 70)
    _ring(m, WHITE, (L * 0.3, 0, 3600), 2900, 150, steps=24)
    _ring(m, CYAN, (L * 0.3, -200, 3600), 2900, 50, steps=24, glow=G2)
    for k in range(7):
        m.sphere(BULB if k % 2 else AMBER, (-L / 2.0 + (k + 0.5) * L / 7.0, -320, 420), 130, steps=(3, 6), glow=G4)


def far_wheel(m, R=5600):
    """A giant wheel on the skyline, a hundred and twenty metres of it, lit round both rims. What turns (rims, spokes,
    gondolas, the hub's boss) is in mover shades: it turns about Y through the hub, slowly (MOVERS)."""
    hub = R + 700
    for sy in (-1, 1):
        for sx in (-1, 1):
            tilt_box(m, CREAM, (sx * 2800, sy * 600, 0), (0, sy * 320, hub), 320, 260)
        _ring(m, mv(MAG) if sy < 0 else mv(CYAN), (0, sy * 260, hub), R, 110, steps=28, glow=G2)
    _ring(m, mv(CREAM), (0, 0, hub), R * 0.5, 70, steps=20)
    n = 16
    for k in range(n):
        a = 2 * math.pi * k / n
        cx, cz = math.cos(a), math.sin(a)
        tilt_box(m, mv(CREAM), (cx * 300, 0, hub + cz * 300), (cx * R, 0, hub + cz * R), 80, 80)
        m.box(mv([AMBER, MAG, CYAN, CREAM][k % 4]), (cx * R, 0, hub + cz * R - 300), (520, 620, 440), glow=G2)
    m.sphere(mv(AMBER), (0, -420, hub), 420, scale=(1, 0.4, 1), steps=(5, 8), glow=G3)
    m.box(PURPLE, (0, 0, 200), (7000, 2000, 400))
    _bulbs(m, (-3300, -1020, 460), (3300, -1020, 460), 10, [BULB, MAG, CYAN], r=110)


def far_chute(m, H=15000):
    """A parachute tower: a lattice mast a hundred and fifty metres tall, a crown of arms, canopies at every height."""
    for sx in (-1, 1):
        for sy in (-1, 1):
            tilt_box(m, CREAM, (sx * 1500, sy * 1500, 0), (sx * 300, sy * 300, H), 260, 260)
    for k in range(1, 8):
        z = k * H / 8.0
        w = 1500 - (1200.0 * k / 8.0)
        for s in (-1, 1):
            m.box(CREAM, (0, s * w, z), (2 * w, 110, 110))
            m.box(CREAM, (s * w, 0, z), (110, 2 * w, 110))
        m.box(MAG if k % 2 else CYAN, (0, -w - 80, z), (2 * w, 40, 90), glow=G2)
    m.cyl(MAG, (0, 0, H), 2700, 260, steps=12)
    m.cyl(CREAM, (0, 0, H + 260), 2700, 900, steps=12, top_r=200)
    m.sphere(RED, (0, 0, H + 1400), 220, steps=(4, 6), glow=G4)
    cols = [AMBER, MAG, CREAM, CYAN, RED, MINT]
    for k in range(6):
        a = k * math.pi / 3.0 + 0.3
        x, y = math.cos(a) * 2500, math.sin(a) * 2500
        z = H * (0.35 + 0.1 * ((k * 3) % 5))
        rod(m, GREY_L, (x, y, z + 900), (x, y, H), 14, steps=4)
        m.cyl(cols[k], (x, y, z + 300), 800, 600, steps=10, top_r=80)
        m.box(NAVY, (x, y, z - 150), (220, 220, 200))
        rod(m, GREY_L, (x, y, z - 60), (x - 600, y, z + 320), 10, steps=4)
        rod(m, GREY_L, (x, y, z - 60), (x + 600, y, z + 320), 10, steps=4)


def far_bigtops(m):
    """Three big tops on the skyline, striped, each with its pennant, bulbs strung from peak to peak."""
    peaks = []
    for x, R, h, c in ((-4200, 2600, 1700, RED), (300, 3400, 2300, MAG), (4700, 2300, 1500, TEAL)):
        n = 12
        for k in range(n):
            a = k * 2 * math.pi / n
            m.box(c if k % 2 else WHITE, (x + math.cos(a) * R, math.sin(a) * R, h / 2.0), (120, 2 * math.pi * R / n + 20, h), rot=(0, math.degrees(a), 0))
        m.cyl(WHITE, (x, 0, h), R + 60, R * 0.95, steps=n, top_r=60)
        for k in range(0, n, 2):
            a = k * 2 * math.pi / n
            tilt_box(m, c, (x + math.cos(a) * (R + 40), math.sin(a) * (R + 40), h + 20), (x, 0, h + R * 0.95), 260, 40)
        top = h + R * 0.95
        m.cyl(GOLD, (x, 0, top), 30, 900, steps=5)
        _side(m, MAG if c != MAG else AMBER, [(x, top + 900), (x + 520, top + 740), (x, top + 580)], -12, 12, glow=G1)
        peaks.append((x, -200.0, top))
        m.box(NAVY, (x, -R - 30, 500), (900, 80, 1000))
        m.box(BULB, (x, -R - 80, 520), (640, 20, 700), glow=G2)
    for a, b in zip(peaks, peaks[1:]):
        mid = ((a[0] + b[0]) / 2.0, -300.0, min(a[2], b[2]) - 1500)
        _bulbs(m, a, mid, 6, [BULB, MAG, CYAN], r=90)
        _bulbs(m, mid, b, 6, [CYAN, AMBER, BULB], r=90)


def far_rocket(m, H=13000):
    """Rocket Land's skyline: a striped rocket a hundred and thirty metres tall on its pad, a gantry beside it."""
    m.cyl(GREY, (0, 0, 0), 3000, 300, steps=12)
    m.cyl(CREAM, (0, 0, 900), 1000, H * 0.62, steps=14)
    m.cyl(MAG, (0, 0, 900 + H * 0.2), 1010, H * 0.07, steps=14)
    m.cyl(MAG, (0, 0, 900 + H * 0.45), 1010, H * 0.05, steps=14)
    m.cyl(CREAM, (0, 0, 900 + H * 0.62), 1000, H * 0.26, steps=14, top_r=40)
    m.cyl(RED, (0, 0, 900 + H * 0.8), 430, H * 0.09, steps=10, top_r=20)
    rod(m, CYAN, (0, -1000, 900 + H * 0.5), (0, -1030, 900 + H * 0.5), 360, steps=12, glow=G2)
    for k in range(4):
        a = k * math.pi / 2.0 + math.pi / 4.0
        cx, cy = math.cos(a), math.sin(a)
        tilt_box(m, MAG, (cx * 2100, cy * 2100, 300), (cx * 900, cy * 900, 3600), 120, 900)
    m.cyl(ORANGE, (0, 0, 300), 700, 600, steps=10, top_r=420, glow=G3)
    gx = 2900
    m.box(GREY_L, (gx, 0, H * 0.36), (700, 700, H * 0.72))
    for k in range(9):
        z = 500 + k * H * 0.075
        m.box(NAVY, (gx, -356, z), (560, 10, H * 0.05))
        m.box(AMBER if k % 3 else CYAN, (gx, -366, z + H * 0.03), (300, 10, 90), glow=G2)
    for z in (H * 0.3, H * 0.5, H * 0.66):
        m.box(GREY_L, (gx / 2.0 + 400, 0, 900 + z), (gx - 900, 220, 180))
    m.sphere(RED, (gx, 0, H * 0.72 + 200), 200, steps=(4, 6), glow=G4)
    for s in (-1, 1):
        m.sphere(BULB, (s * 2600, -2200, 420), 190, steps=(4, 6), glow=G4)


def far_fort(m):
    """Pirate Cove's skyline: a fort on its rock, round towers under pointed roofs, the masts of a ship behind it."""
    stone, stone_l = '#5a5478', '#6a6490'
    m.sphere(ROCK, (0, 600, 600), 5200, scale=(1.3, 0.8, 0.5), steps=(6, 10))
    m.box(stone, (0, 0, 3300), (6200, 3200, 2600))
    for k in range(9):
        m.box(stone, (-2800 + k * 700, -1500, 4800), (380, 260, 420))
    for k in range(6):
        m.box(AMBER, (-2300 + k * 920, -1612, 3400 + (k % 2) * 500), (300, 20, 460), glow=G2)
    for x, h, r_ in ((-3600, 6400, 1000), (3400, 7600, 1150), (200, 5200, 800)):
        m.cyl(stone_l, (x, -400, 1500), r_, h - 1500, steps=10)
        m.cyl(MAG, (x, -400, h), r_ + 160, r_ * 1.9, steps=10, top_r=30)
        m.box(AMBER, (x, -400 - r_ - 8, h - 700), (260, 20, 420), glow=G2)
        m.cyl(GOLD, (x, -400, h + r_ * 1.9), 26, 800, steps=5)
        _side(m, RED, [(x, h + r_ * 1.9 + 800), (x + 620, h + r_ * 1.9 + 620), (x, h + r_ * 1.9 + 440)], -412, -388)
    for x, h in ((-1700, 9800), (-300, 11200), (1100, 9200)):
        m.cyl(WOOD_D, (x, 2400, 2000), 90, h - 2000, steps=6)
        for j in range(3):
            z = h - 900 - j * 2100
            m.box(CREAM, (x, 2380, z), (2400 - j * 100, 40, 1500))
            m.box(WOOD_D, (x, 2400, z + 800), (2700 - j * 100, 60, 60))
    for s in (-1, 1):
        m.sphere(ORANGE, (s * 3200, -1700, 2300), 200, scale=(1, 1, 1.4), steps=(4, 6), glow=G4)


# ------------------------------------------------------------------------------------------ the park's floor (second pass)
# What the land is made of where a banked, looping or flying camera looks down on it. Every piece here is FLAT (nothing
# over 1.9 m, so the track's sight-line rule leaves it alone) and UNLIT but for the pool's water (placed 30 m out and
# beyond). The track sinks a piece 15 cm into the land, so each has a kerb and skirt going 1.4 m down and its top 25 to
# 45 cm up; each kind has its own top height, so two that overlap lie one over the other instead of flickering.
# They are made for LEVEL ground: the table keeps them off the hills, the jumps and the coaster's valleys.
def _disc(m, col, r, z0, z1, at=(0.0, 0.0), steps=20, glow=0.0):
    m.cyl(col, (at[0], at[1], z0), r, z1 - z0, steps=steps, glow=glow)


def _starpts(R, r, n, turn=0.0, at=(0.0, 0.0)):
    """A star's outline in plan: n points of radius R, the notches between them at r."""
    pts = []
    for k in range(2 * n):
        a = turn + k * math.pi / n
        rr = R if k % 2 == 0 else r
        pts.append((at[0] + math.cos(a) * rr, at[1] + math.sin(a) * rr))
    return _ccw(pts)


def _stadium(hl, r, n=10):
    """A running-track outline in plan: two half circles of radius r, their centres hl either side of the middle."""
    pts = []
    for k in range(n + 1):
        a = -math.pi / 2.0 + math.pi * k / n
        pts.append((hl + math.cos(a) * r, math.sin(a) * r))
    for k in range(n + 1):
        a = math.pi / 2.0 + math.pi * k / n
        pts.append((-hl + math.cos(a) * r, math.sin(a) * r))
    return _ccw(pts)


def floor_plaza(m, R=1300.0):
    """A round plaza 26 m across: a stone kerb, pale paving, a pink ring, a compass star in two colours, planters
    round the rim. The forecourt of a gate, the pad under a rocket ride, the hub the paths run to."""
    _disc(m, GREY_L, R, -140, 30, steps=24)
    _disc(m, CREAM, R - 70, 27, 34, steps=24)
    _disc(m, MAG, R * 0.80, 32, 36, steps=24)
    _disc(m, CREAM, R * 0.72, 34, 38, steps=24)
    m.plan(TEAL, _starpts(R * 0.66, R * 0.20, 4, math.pi / 4.0), 36, 40)
    m.plan(AMBER, _starpts(R * 0.70, R * 0.22, 4, 0.0), 38, 42)
    _disc(m, PLUM, R * 0.15, 40, 45, steps=12)
    for k in range(8):
        a = (k + 0.5) * math.pi / 4.0
        x, y = math.cos(a) * (R - 150), math.sin(a) * (R - 150)
        m.cyl(PLUM, (x, y, 27), 70, 45, steps=8)
        m.sphere(PINE if k % 2 else MAG, (x, y, 102), 62, scale=(1, 1, 0.8), steps=(4, 6))


def floor_spoke(m, L=3000.0, W=600.0):
    """A footpath 30 m long running AWAY from the road (along Y): pale slabs between pink bands, coloured diamonds down
    the middle, a round node at each end. Paths leave the promenade for the rides behind it."""
    m.box(GREY_L, (0, 0, -55), (W + 90, L, 170))
    m.box(CREAM, (0, 0, 29), (W, L, 8))
    for s in (-1, 1):
        m.box(MAG, (s * (W / 2.0 - 50), 0, 32), (56, L, 6))
    for k in range(5):
        m.box(AMBER if k % 2 else TEAL, (0, -L / 2.0 + 500 + k * (L - 1000) / 4.0, 33), (170, 170, 6), rot=(0, 45, 0))
    for s in (-1, 1):
        at = (0, s * L / 2.0)
        _disc(m, GREY_L, 430, -140, 31, at=at, steps=16)
        _disc(m, CREAM, 370, 29, 36, at=at, steps=16)
        _disc(m, MAG if s < 0 else TEAL, 190, 34, 39, at=at, steps=12)
        _disc(m, CREAM, 110, 37, 41, at=at, steps=10)


def floor_pave(m, L=3400.0, W=900.0):
    """The paved terrace a row of stalls stands on, 34 m by 9 m: a line of big diamonds along the front, where the
    crowd would stand, and a gold line along each edge."""
    m.box(GREY_L, (0, 0, -57), (L, W, 166))
    m.box(CREAM, (0, 0, 24), (L - 80, W - 80, 8))
    for k in range(10):
        m.box(LAV if k % 2 else MAG, (-L / 2.0 + 260 + k * (L - 520) / 9.0, -W / 2.0 + 150, 27), (150, 150, 5), rot=(0, 45, 0))
    for s in (-1, 1):
        m.box(AMBER, (0, s * (W / 2.0 - 70), 27), (L - 160, 40, 5))


def floor_lawn(m, L=2600.0, W=1600.0):
    """A lawn panel 26 m by 16 m inside a pale kerb: a star bed of pink flowers in the middle, a lavender bed with
    yellow and white clumps at each end, a clipped hedge along both long sides with a cone tree at every corner."""
    m.box(CREAM, (0, 0, -58), (L, W, 164))
    m.box(MINT, (0, 0, 23), (L - 100, W - 100, 8))
    m.plan(MAG, _starpts(430, 190, 5, math.pi / 2.0), 25, 36)
    _disc(m, AMBER, 130, 34, 42, steps=10)
    for sx in (-1, 1):
        m.box(LAV, (sx * 880, 0, 30), (240, W - 420, 12))
        for k in range(4):
            m.sphere(AMBER if k % 2 else WHITE, (sx * 880, -420 + k * 280, 40), 70, scale=(1, 1, 0.5), steps=(3, 6))
    for sy in (-1, 1):
        m.box(PINE, (0, sy * (W / 2.0 - 110), 51), (L - 260, 90, 60))
        for sx in (-1, 1):
            m.cyl(PINE, (sx * (L / 2.0 - 130), sy * (W / 2.0 - 110), 25), 80, 150, steps=8, top_r=10)


def floor_games(m, L=2200.0, W=1400.0):
    """A court of painted games, 22 m by 14 m: a giant chessboard, a hopscotch, a target of rings."""
    m.box(CREAM, (0, 0, -56), (L, W, 168))
    m.box(PLUM, (0, 0, 26), (L - 70, W - 70, 8))
    q = 150.0
    m.box(MAG, (-560, 0, 30), (6 * q, 6 * q, 6))
    for i in range(6):
        for j in range(6):
            if (i + j) % 2 == 0:
                m.box(CREAM, (-560 - 2.5 * q + i * q, -2.5 * q + j * q, 32), (q, q, 5))
    for k in range(7):
        off = (0.0, 62.0, -62.0)[k % 3]
        m.box((AMBER, CYAN, MINT)[k % 3], (230 + k * 118, -440 + off, 31), (108, 108, 5))
    for k, (rr, c) in enumerate(((330, RED), (250, CREAM), (170, RED), (90, AMBER))):
        _disc(m, c, rr, 28 + k, 32 + 1.5 * k, at=(620, 250), steps=16)


def floor_queue(m, L=2000.0, W=1100.0):
    """A queue yard, 20 m by 11 m: switchback lanes painted in yellow, a stout post with a pink knob at every turn, a
    wait-time board lying low at the way in."""
    m.box(GREY_L, (0, 0, -57), (L, W, 166))
    m.box(PURPLE, (0, 0, 25), (L - 60, W - 60, 8))
    n = 5
    gap = (W - 320) / (n - 1.0)
    for k in range(n):
        y = -W / 2.0 + 160 + k * gap
        m.box(AMBER, (0, y, 30), (L - 360, 46, 5))
        e = 1 if k % 2 == 0 else -1
        if k < n - 1:
            m.box(AMBER, (e * (L / 2.0 - 180), y + gap / 2.0, 30), (46, gap + 46, 5))
        m.cyl(CREAM, (-e * (L / 2.0 - 150), y, 27), 26, 95, steps=6)
        m.sphere(MAG, (-e * (L / 2.0 - 150), y, 136), 34, steps=(3, 6))
    m.box(TEAL, (-L / 2.0 + 150, -W / 2.0 + 60, 75), (220, 30, 92))
    m.box(CREAM, (-L / 2.0 + 150, -W / 2.0 + 44, 84), (150, 6, 44))


def floor_kart(m):
    """A go-kart track, 41 m by 21 m: an oval of dark tarmac round a lawn, red and white kerbs on both bends, a
    chequered start line, tyre stacks, three karts on the grid."""
    hl, ro, ri = 1000.0, 1000.0, 470.0
    m.plan(GREY_L, _stadium(hl, ro + 60), -140, 22)
    m.plan(PURPLE, _stadium(hl, ro), 20, 25)
    m.plan(CREAM, _stadium(hl, ri + 44), 23, 27)
    m.plan(MINT, _stadium(hl, ri), 25, 29)
    for s in (-1, 1):
        for k in range(9):
            a = -math.pi / 2.0 + math.pi * (k + 0.5) / 9.0
            m.box(RED if k % 2 else WHITE, (s * (hl + math.cos(a) * (ro - 70)), math.sin(a) * (ro - 70), 26), (120, 170, 8),
                  rot=(0, s * math.degrees(a), 0))
    for k in range(6):
        m.box(WHITE if k % 2 else NAVY, (-40 + (k % 2) * 80, -ro + 60 + k * 76, 27), (80, 76, 4))
    for k, (x, y) in enumerate(((-hl, 0), (hl, 0), (-300, 0), (300, 0), (0, 300), (0, -300))):
        m.cyl(NAVY, (x, y, 29), 52, 70, steps=8)
        m.cyl((MAG, AMBER, CYAN)[k % 3], (x, y, 99), 40, 10, steps=8)
    for k, c in enumerate((RED, CYAN, AMBER)):
        x, y = 260 + k * 300, -ro + 200 + (k % 2) * 260
        m.box(c, (x, y, 60), (190, 100, 46))
        m.box(NAVY, (x - 20, y, 92), (70, 80, 30))
        for sx in (-1, 1):
            m.box(NAVY, (x + sx * 70, y, 45), (56, 130, 44))


def floor_pool(m):
    """A paddling pool in two lobes, 23 m by 13 m: blue water inside a pale kerb, stepping stones across the waist,
    lily pads; unlit. Stood under the pirate ships (the swinging boat over water) and out on the lawns."""
    lobes = ((-420, 60, 640), (430, -80, 520))
    for x, y, r in lobes:
        _disc(m, CREAM, r, -140, 34, at=(x, y), steps=18)
    m.box(CREAM, (0, 0, -53), (860, 760, 174))
    for x, y, r in lobes:
        _disc(m, POOL, r - 70, 26, 37, at=(x, y), steps=18)
    m.box(POOL, (0, -10, 32), (860, 620, 10))
    for k in range(5):
        _disc(m, GREY_L, 58, 30, 43, at=(-10 + (k % 2) * 60, -250 + k * 125), steps=8)
    for k, (x, y) in enumerate(((-640, 260), (-300, -260), (520, -260), (640, 120), (-700, -120))):
        _disc(m, PINE, 64, 36, 40, at=(x, y), steps=8)
        m.sphere(MAG if k % 2 else WHITE, (x + 14, y + 10, 48), 22, steps=(3, 5))


def floor_yard(m, seed=11):
    """Backstage: a concrete service yard, 24 m by 16 m, with a hatched keep-clear bay, parking bays and a service cart
    in one, crates on pallets, a skip, cable drums, bins and a spare coaster car on a trolley. Nothing over 1.9 m and
    no colour to speak of: it stands behind the stall rows and in the back lot, where the park is not on show."""
    r = random.Random(seed)
    L, W = 2400.0, 1600.0
    m.box(GREY, (0, 0, -59), (L, W, 162))
    m.box(GREY_L, (0, 0, 21), (L - 60, W - 60, 8))
    for k in range(7):
        m.box(AMBER, (-960 + k * 110, -430, 27), (36, 500, 4), rot=(0, 35, 0))
    for k in range(5):
        m.box(CREAM, (120 + k * 250, -470, 27), (30, 520, 4))
    m.box(GEN, (245, -470, 78), (170, 360, 96))
    m.box(NAVY, (245, -570, 146), (150, 120, 40))
    for sy in (-1, 1):
        m.box(NAVY, (245, -470 + sy * 120, 50), (190, 70, 50))
    for k in range(6):
        m.box(WOOD if k % 2 else WOOD_D, (-960 + (k % 3) * 124, 380, 67 + (k // 3) * 82), (104, 104, 80), rot=(0, r.uniform(-10, 10), 0))
    m.box(WOOD_D, (-836, 380, 34), (400, 130, 18))
    for k in range(3):
        m.box(WOOD_D, (-420 + k * 170, 470, 34), (140, 110, 18))
    m.box(SHED, (520, 430, 86), (380, 180, 120))
    m.box(SHED_D, (520, 430, 150), (400, 200, 10))
    for k in range(2):
        rod(m, WOOD, (930, 230 + k * 170, 88), (1010, 230 + k * 170, 88), 62, steps=8)
    for k in range(4):
        m.cyl((GEN, PLUM, SHED, RED)[k], (-80 + k * 84, 600, 25), 34, 100, steps=7)
    m.box(GREY, (-860, -20, 46), (300, 170, 14))
    m.box(RED, (-860, -20, 100), (260, 150, 90))
    m.box(NAVY, (-860, -20, 156), (200, 110, 28))


# ------------------------------------------------------------------------------------------ the banks of the banked bends
# A banked bend sinks the road's inner edge up to 11 m; the track cuts the land down to it, and from the road that
# cutting fills a third of the frame. The second pass stood two tilted beds on it by hand (SM_BankStar, SM_BankBands):
# loud (a mint face with pink, gold and lavender shapes) and only where the bank held its full angle. They are gone.
# The track now faces every cutting itself (a Bank rule, FTDress.h) with these two panels, drawn FLAT as
# ft_dress_bank.py says: x along the road, y up the slope, z out of the bank. A clipped hedge bank in the lawn's own
# dark green, a pale stone kerb at its foot and its head, ribbons of flowers that run on from panel to panel. Unlit.
def cut_lawn(m):
    """The tall panel (5.5 m of cutting and more: bends banked 32 degrees and up)."""
    h = B.TALL / 2.0
    B.body(m, PINE, B.TALL)
    B.band(m, CREAM, -h, -h + 64.0, 10.0)
    B.band(m, CREAM, h - 56.0, h, 10.0)
    B.band(m, MAG, -h + 210.0, -h + 280.0, 8.0, wob=26.0, n=5, seed=3)
    B.band(m, AMBER, h - 330.0, h - 270.0, 8.0, wob=26.0, n=5, seed=5)
    for k in range(4):                                          # clipped bushes between the ribbons, in the lawn's own green
        B.block(m, LAWN, -B.L / 2.0 + (k + 0.5) * B.L / 4.0, (-50.0 if k % 2 else 70.0), 170.0, 110.0, 24.0)


def cut_lawn_low(m):
    """The low panel (2.4 to 5.5 m of cutting: the gentler bends and every bend's lead-in)."""
    h = B.LOW / 2.0
    B.body(m, PINE, B.LOW)
    B.band(m, CREAM, -h, -h + 50.0, 10.0)
    B.band(m, CREAM, h - 44.0, h, 10.0)
    B.band(m, MAG, -34.0, 34.0, 8.0, wob=14.0, n=5, seed=7)


# ------------------------------------------------------------------------------------------ third pass: the middle distance
def _lamps(m, a, b, n, cols=None, s=26.0, glow=G4):
    """n square bulbs strung from a to b: twelve triangles each (a round one is thirty), for pieces seen from 40 m on."""
    cols = cols or BULBS
    for k in range(n):
        t = (k + 0.5) / n
        m.box(cols[k % len(cols)], (a[0] + (b[0] - a[0]) * t, a[1] + (b[1] - a[1]) * t, a[2] + (b[2] - a[2]) * t), (s, s, s), glow=glow)


def marquee_tower(m, H=2400.0):
    """A marquee tower, one each side of the road where a land begins: a striped shaft on a ticket kiosk, bulbs climbing
    its two road-side corners (they chase upward), COVE down its face in lights, and on top the park's star, in the
    mover shade (it turns)."""
    m.box(NAVY, (0, 0, 20), (560, 560, 40))
    m.box(PLUM, (0, 0, 190), (420, 420, 300))
    m.box(BULB, (0, -212, 200), (240, 4, 150), glow=G2)
    m.box(MAG, (0, 0, 348), (470, 470, 16))
    m.box(CREAM, (0, 0, 356 + (H - 356) / 2.0), (260, 260, H - 356))
    for k in range(6):
        m.box(MAG, (0, 0, 520 + k * 340), (268, 268, 110))
    for sx in (-1, 1):
        _lamps(m, (sx * 142, -142, 400), (sx * 142, -142, H - 20), 12, s=30.0)
    m.box(NAVY, (0, -134, 1500), (150, 8, 880))
    for k, ch in enumerate('COVE'):
        _letters(m, ch, -140, 1790 - k * 215, 150, AMBER, x=0.0)
    m.box(MAG, (0, 0, H + 20), (430, 430, 40))
    _lamps(m, (-200, -222, H + 20), (200, -222, H + 20), 6, s=30.0)
    m.cyl(GOLD, (0, 0, H + 40), 16, 190, steps=6)
    _star(m, mv(AMBER), 0, H + 430, 250, -24, 24, glow=G3)
    m.base = ft_art._xf((0, 0, 0), (0, 90, 0))                  # and the same star across it: a turning star needs two faces
    try:
        _star(m, mv(AMBER), 0, H + 430, 250, -24, 24, glow=G3)
    finally:
        m.base = None


def facade_row(m):
    """Sixty metres of the park's main street as a stage flat (2.5 m thick: at 60 m and beyond it reads as buildings):
    four plaster fronts in the park's pastels, each with its own skyline (a gable with a star, steps, a dome, a
    turret), lit windows, a striped awning over a lit doorway, bulbs along the parapet (they chase)."""
    fronts = ((1500, 1450, LAV, MAG, 'gable'), (1300, 1100, MINT, AMBER, 'step'), (1700, 1800, CREAM, TEAL, 'dome'), (1500, 1250, PEACH, MAG, 'turret'))
    x = -3000.0
    for k, (w, h, body, trim, top) in enumerate(fronts):
        xc = x + w / 2.0
        m.box(body, (xc, 0, h / 2.0 - 100), (w - 16, 250, h + 200))
        m.box(trim, (xc, -12, h + 22), (w + 10, 290, 44))
        m.box(NAVY, (xc, -128, 150), (300, 6, 300))
        m.box(BULB, (xc, -131, 190), (190, 4, 180), glow=G2)
        _stripes(m, x + 70, x + w - 70, -190, 350, 150, 10, [trim, CREAM], 6, roll=_awn(16))
        for j in range(int((h - 520) // 430)):
            for i in (-1, 1):
                m.box(CYAN if (i + j + k) % 3 == 0 else BULB, (xc + i * w * 0.24, -128, 640 + j * 430), (w * 0.27, 6, 270), glow=G2)
        _lamps(m, (x + 60, -150, h + 66), (x + w - 60, -150, h + 66), 5)
        if top == 'gable':
            _side(m, body, [(x + 8, h + 44), (x + w - 8, h + 44), (xc, h + 480)], -125, 125)
            _star(m, AMBER, xc, h + 200, 100, -134, -125, glow=G3)
        elif top == 'step':
            m.box(body, (xc, 0, h + 150), (w * 0.6, 250, 212))
            m.box(trim, (xc, 0, h + 320), (w * 0.3, 250, 128))
        elif top == 'dome':
            m.sphere(trim, (xc, 0, h + 44), w * 0.3, scale=(1.0, 0.24, 1.0), steps=(5, 8))
            m.cyl(GOLD, (xc, 0, h + 44 + w * 0.3), 12, 220, steps=5)
            _side(m, MAG, [(xc, h + w * 0.3 + 264), (xc + 150, h + w * 0.3 + 214), (xc, h + w * 0.3 + 164)], -4, 4)
        else:
            m.box(body, (x + w * 0.74, 0, h + 260), (300, 250, 432))
            m.cyl(trim, (x + w * 0.74, 0, h + 476), 210, 330, steps=6, top_r=8)
            m.box(BULB, (x + w * 0.74, -128, h + 250), (110, 6, 200), glow=G2)
        x += w


def galleon(m):
    """Pirate Cove's landmark, dead ahead of the big jump: a galleon aground on a rock in its own pool, three masts of
    sail braced fore and aft (so they show from the road), her stern windows lit, lamps in the rigging; and the cove's
    kraken has her: four arms out of the water, in the mover shade (they sway). Her flag is the second mover (it
    flutters). 52 m across, 39 m to the truck."""
    m.cyl(GREY_L, (0, 0, -300), 2600, 330, steps=14)
    m.cyl(WATER, (0, 0, -290), 2500, 335, steps=14, glow=GW)
    m.sphere(ROCK, (0, 200, 150), 1300, scale=(1.5, 0.8, 0.5), steps=(5, 9))
    hull = [(-1500, 700), (-1700, 1500), (-1100, 1500), (-1000, 1250), (1200, 1250), (1500, 1500), (1900, 1650), (1500, 800), (900, 600), (-1100, 600)]
    _side(m, WOOD_D, hull, -330, 330)
    m.box(RED, (0, 0, 1280), (2300, 680, 70))
    m.box(WOOD, (-1400, 0, 1700), (620, 600, 420))
    for k in range(3):
        m.box(AMBER, (-1580 + k * 180, -302, 1720), (110, 6, 130), glow=G2)
        m.box(AMBER, (-1712, -180 + k * 180, 1700), (6, 120, 160), glow=G2)
    tilt_box(m, WOOD, (1500, 0, 1550), (2500, 0, 1900), 36, 36)                 # the bowsprit
    for x, h in ((-700, 3300), (150, 3900), (1000, 3000)):
        m.cyl(WOOD, (x, 0, 1250), 36, h - 1250, steps=6)
        for zz, w in ((h - 520, 900), (h - 1320, 1250)):
            m.box(CREAM, (x, 0, zz), (w, 30, 640))
            m.box(WOOD_D, (x, 0, zz + 330), (w + 120, 44, 44))
        m.cyl(WOOD_D, (x, 0, h - 150), 90, 60, steps=6)
    _side(m, mv2(NAVY), [(150, 3900), (560, 3800), (150, 3700)], -4, 4)
    tops = ((-1650, 0, 1950), (-700, 0, 3300), (150, 0, 3900), (1000, 0, 3000), (2450, 0, 1900))
    for a, b in zip(tops, tops[1:]):
        _lamps(m, (a[0], -40, a[2]), (b[0], -40, b[2]), 5, [BULB, AMBER])
    for x0, y0, lx, ly, h in ((-2000, -500, 420, 120, 2100), (2150, -350, -480, 160, 2500), (600, -1000, 160, 300, 1500), (-800, 950, -260, -200, 1800)):
        p = (x0, y0, -60.0)
        for k in range(1, 6):
            f = k / 5.0
            q = (x0 + lx * f * f + 150 * math.sin(f * 3.0), y0 + ly * f * f, h * f)
            rod(m, mv(KRAKEN), p, q, 125 - 17 * k, steps=6)
            p = q
        m.sphere(mv(LAV), p, 60, steps=(3, 5))


def balloon(m):
    """A hot-air balloon, 14 m of it: a cream envelope hooped in the park's colours, a basket under it, the burner lit.
    The table hangs them in the air beside both jumps' flights and far out over the park (a Far rule with a drop below
    zero); they rise and sink a little, each in its own time (animate())."""
    m.sphere(CREAM, (0, 0, 900), 460, scale=(1.0, 1.0, 1.15), steps=(6, 10))
    for z, r_, c in ((1100, 432, MAG), (900, 468, AMBER), (700, 432, CYAN)):
        m.cyl(c, (0, 0, z - 50), r_, 100, steps=12)
    m.cyl(CREAM, (0, 0, 290), 110, 180, steps=8, top_r=250)
    m.box(WOOD, (0, 0, 80), (150, 150, 120))
    for sx in (-1, 1):
        for sy in (-1, 1):
            tilt_box(m, WOOD_D, (sx * 70, sy * 70, 140), (sx * 100, sy * 100, 300), 10, 10)
    m.sphere(ORANGE, (0, 0, 250), 40, steps=(3, 5), glow=G3)


def robot(m):
    """Rocket Land's tin robot, 14 m of him: boots, a boxy chest with a panel of lights (they blink), a square head
    with lamp eyes and an aerial. A mechanical figure: his raised arm is the mover (it waves from the shoulder), his
    head the second mover (it turns left and right). In his other hand, a planet on a stick."""
    m.box(NAVY, (0, 0, 20), (700, 520, 40))
    for sx in (-1, 1):
        m.box(TIN_D, (sx * 150, 0, 250), (170, 200, 420))
        m.box(RED, (sx * 150, -20, 80), (210, 260, 80))
    m.box(TIN, (0, 0, 740), (560, 320, 560))
    m.box(CYAN, (0, 0, 472), (580, 340, 24))
    m.box(NAVY, (0, -164, 760), (360, 6, 300))
    for j in range(3):
        for i in range(4):
            m.box((CYAN, AMBER, MAG)[(i + j) % 3], (-135 + i * 90, -170, 670 + j * 90), (50, 6, 50), glow=G3)
    m.cyl(TIN_D, (0, 0, 1020), 60, 40, steps=8)
    m.box(mv2(TIN), (0, 0, 1190), (380, 300, 260))
    for sx in (-1, 1):
        m.sphere(mv2(AMBER), (sx * 90, -152, 1210), 44, steps=(3, 6), glow=G4)
    m.box(mv2(RED), (0, -152, 1110), (180, 6, 30))
    m.cyl(mv2(TIN_D), (0, 0, 1320), 9, 110, steps=5)
    m.sphere(mv2(RED), (0, 0, 1450), 30, steps=(3, 5), glow=G4)
    m.box(TIN_D, (-350, 0, 760), (120, 130, 400))               # the arm that hangs, and the planet it holds
    m.cyl(GREY_L, (-350, -80, 560), 10, 420, steps=5)
    m.sphere(MAG, (-350, -80, 1040), 90, steps=(4, 7), glow=G1)
    m.cyl(AMBER, (-350, -80, 1034), 150, 12, steps=10, glow=G2)
    m.sphere(TIN_D, (300, 0, 930), 80, steps=(3, 6))            # the shoulder, and the arm that waves
    rod(m, mv(TIN_D), (300, 0, 930), (520, -20, 1250), 56, steps=6)
    m.sphere(mv(RED), (540, -22, 1290), 80, steps=(4, 6))


# ------------------------------------------------------------------------------------------ third pass: the horizon's cards
def _skyline(feats, x0, x1, step=500.0, floor=600.0):
    """The outline of a row of shapes as one polygon [(x, z)]: the highest of them every `step` cm. feats: (kind, middle,
    half width, height): 'tent' a peak, 'hump' a coaster's hill, 'dome', 'block', 'wheel' (a disc on a drum)."""
    def top(x):
        z = floor
        for kind, xm, hw, h in feats:
            u = (x - xm) / float(hw)
            if abs(u) >= 1.0:
                continue
            if kind == 'tent':
                z = max(z, h * (1.0 - abs(u)) ** 0.8)
            elif kind == 'hump':
                z = max(z, h * math.cos(u * math.pi / 2.0))
            elif kind == 'dome':
                z = max(z, h * math.sqrt(1.0 - u * u))
            elif kind == 'wheel':
                z = max(z, h * 0.5 + h * 0.5 * math.sqrt(1.0 - u * u))
            else:
                z = max(z, h)
        return z
    n = int(round((x1 - x0) / step))
    return [(x0, -2500.0)] + [(x0 + (x1 - x0) * i / float(n), top(x0 + (x1 - x0) * i / float(n))) for i in range(n + 1)] + [(x1, -2500.0)]


def sky_card(m, feats, width, col, glow, dots, seed):
    """A flat skyline card for the horizon: one silhouette in one tone (a little self-lit, so the night does not turn
    it black), a few lights on it. 60 cm thick; its face is to the road."""
    r = random.Random(seed)
    _side(m, col, _skyline(feats, -width / 2.0, width / 2.0), -30, 30, glow=glow)
    for k in range(dots):
        kind, xm, hw, h = feats[r.randrange(len(feats))]
        m.box((BULB, MAG, CYAN)[k % 3], (xm + r.uniform(-0.5, 0.5) * hw, -34, r.uniform(0.25, 0.6) * h), (240, 8, 240), glow=G2)


SKY1 = (('tent', -17500, 3400, 5200), ('tent', -12500, 2600, 4000), ('wheel', -7000, 3600, 8600), ('hump', -1500, 3800, 5600),
        ('hump', 3800, 3000, 7400), ('block', 8300, 700, 9800), ('dome', 8300, 1800, 5200), ('tent', 13200, 3600, 5600), ('hump', 18200, 2800, 4200))
SKY2 = (('hump', -22000, 5200, 6800), ('hump', -13500, 4200, 9800), ('tent', -7000, 3800, 6000), ('block', -1500, 900, 13000),
        ('dome', -1500, 2800, 7200), ('hump', 5600, 5200, 8200), ('wheel', 13500, 4600, 11500), ('tent', 20500, 4400, 6400))


# ------------------------------------------------------------------------------------------ third pass: what travels (ft_life.Kit)
# Each of these draws into an ft_life.Kit: groups of parts with a way of moving. A Conveyor part is drawn about the
# origin and sent along a line by the material (ConvStart, ConvDir a UNIT vector, ConvLen, ConvArc for a hill), its
# place on the line set by the number given to piece(a=...). They return what Kit.save() needs to know of how far the
# parts leave the mesh as built (so the prop is not culled while they are in view, and the track's road test knows).
def sky_ride(k, L=4524.0, H=1900.0):
    """One span of the sky ride: two pylons, 45 m of twin cable 19 m up (its gondolas clear every ride that stands under it), gondolas going out on one and back on the
    other. A span is one 45 m slot of road long and every span is in step (ConvRand 0), so a row of them down a
    straight is one line: a gondola that leaves a span enters the next. The table stands the spans (at three times
    the size over the lagoon)."""
    s = k.group()
    for x in (-L / 2.0, L / 2.0):      # a pylon at each end: down a straight the next span's stands in the same place
        s.cyl(CREAM, (x, 0, -200), 46, H + 200, steps=8, top_r=30)
        for j in range(3):
            s.cyl(MAG, (x, 0, 300 + j * 480), 50, 90, steps=8)
        s.box(PLUM, (x, 0, H), (80, 580, 60))
        s.sphere(BULB, (x, 0, H + 70), 30, steps=(3, 5), glow=G4)
    for sy in (-1, 1):
        s.box(GREY_L, (0, sy * 230, H - 20), (L, 14, 14))
    for sy, d, cols in ((-1, 1.0, (RED, AMBER)), (1, -1.0, (CYAN, MAG))):
        g = k.group('Conveyor', ConvStart=(-d * L / 2.0, sy * 230.0, H - 20.0), ConvDir=(d, 0.0, 0.0), ConvLen=L, ConvRate=0.1, ConvRand=0.0)
        for i in range(3):
            def cabin(m, c=cols[i % 2]):
                m.box(GREY_L, (0, 0, -75), (12, 12, 150))
                m.box(c, (0, 0, -250), (230, 160, 200))
                m.box(BULB, (0, 0, -232), (234, 164, 64), glow=G1)
                m.box(CREAM, (0, 0, -146), (250, 180, 16))
            g.piece(cabin, a=i / 3.0)
    return dict(grow=(0.0, 0.0, 0.0), grow_down=(0.0, 0.0, 0.0))


def wood_coaster(k, L=12000.0, base=700.0, peak=3300.0):
    """A wooden coaster's big hill as a stage flat, 120 m long and 33 m to the crest: a plum infill, white posts and
    cross braces half a metre wide (a real lattice would ink to scribble at 100 m), the rail picked out in pink light;
    and a train of five cars that goes over the hill and comes round again (the hill is the Conveyor's own arc)."""
    s = k.group()
    n = 24
    top = lambda x: base + (peak - base) * math.sin(math.pi * (x + L / 2.0) / L)
    xs = [-L / 2.0 + L * i / n for i in range(n + 1)]
    _side(s, PLUM, [(-L / 2.0, -300.0)] + [(x, top(x)) for x in xs] + [(L / 2.0, -300.0)], -40, 40)
    for i in range(0, n + 1, 2):
        s.box(WHITE, (xs[i], -20, (top(xs[i]) - 300) / 2.0), (56, 110, top(xs[i]) + 300))
    for i in range(0, n, 2):
        h = min(top(xs[i]), top(xs[i + 2])) * 0.92
        tilt_box(s, WHITE, (xs[i], -44, 0), (xs[i + 2], -44, h), 44, 56)
        tilt_box(s, WHITE, (xs[i + 2], -44, 0), (xs[i], -44, h), 44, 56)
    for a, b in zip(xs, xs[1:]):
        tilt_box(s, WHITE, (a, -30, top(a)), (b, -30, top(b)), 170, 50)
        tilt_box(s, MAG, (a, -124, top(a) + 44), (b, -124, top(b) + 44), 18, 18, glow=G2)
    for sx in (-1, 1):
        s.box(TEAL, (sx * (L / 2.0 - 330), -90, base / 2.0 + 40), (700, 320, base + 280))
        s.box(CREAM, (sx * (L / 2.0 - 330), -90, base + 200), (780, 400, 40))
        s.box(BULB, (sx * (L / 2.0 - 330), -252, base * 0.5), (420, 4, 220), glow=G2)
    g = k.group('Conveyor', ConvStart=(-L / 2.0, -40.0, base + 70.0), ConvDir=(1.0, 0.0, 0.0), ConvLen=L, ConvArc=peak - base, ConvRate=0.085, ConvRand=1.0)
    for i, c in enumerate((AMBER, RED, CYAN, RED, AMBER)):
        def car(m, c=c, lead=(i == 4)):
            m.box(c, (0, 0, 70), (300, 190, 130))
            m.box(NAVY, (0, 0, 152), (220, 150, 40))
            if lead:
                m.box(BULB, (156, 0, 90), (14, 120, 60), glow=G4)
        g.piece(car, a=i * 0.028)
    return dict(grow=(0.0, 0.0, 300.0), grow_down=(0.0, 0.0, 0.0))


def flume(k, H=5200.0):
    """LOG FALLS, the lagoon's landmark: a mountain of the cove's purple rock 52 m tall, pines on its shoulders, and a
    log flume down its face: water running in the trough (a Light chase down it), log boats sliding down one after
    the other (Conveyor), spray standing over the pool at its foot (Plume), the ride's name in lights."""
    s = k.group()
    s.sphere(ROCK, (-400, 300, 1900), 2100, scale=(1.3, 0.85, 1.05), steps=(6, 9))
    s.cyl(ROCK, (-600, 400, 2400), 1300, H - 2400, steps=7, top_r=240)
    s.sphere(ROCK_D, (1700, 500, 800), 1500, scale=(1.2, 0.9, 0.8), steps=(5, 8))
    s.sphere(ROCK_D, (-2600, 300, 700), 1300, scale=(1.1, 0.9, 0.85), steps=(5, 8))
    s.cyl(CREAM, (-600, 400, H - 800), 520, 830, steps=7, top_r=40)
    A, Bt = (-1500.0, -1500.0, 3300.0), (1700.0, -1500.0, 420.0)
    ln = math.sqrt(sum((q - p) ** 2 for p, q in zip(A, Bt)))
    d = tuple((q - p) / ln for p, q in zip(A, Bt))
    tilt_box(s, WOOD_D, A, Bt, 340, 60)                         # the trough's bed and its far board: the road's side is open,
    tilt_box(s, WOOD, (A[0], A[1] + 170, A[2] + 70), (Bt[0], Bt[1] + 170, Bt[2] + 70), 30, 130)      # so the water shows as a lit band
    for i in range(3, 6):                                       # trestles under the lower half (the upper half lies on the rock)
        x = A[0] + (Bt[0] - A[0]) * i / 6.0
        z = A[2] + (Bt[2] - A[2]) * i / 6.0
        s.box(WOOD_D, (x, A[1], z / 2.0 - 100), (44, 300, z + 200))
    s.box(WOOD, (-1700, -1150, 3400), (480, 940, 520))          # the lift house the boats come out of, on the rock's face
    s.box(RED, (-1700, -1150, 3690), (560, 1020, 60))
    s.box(NAVY, (-1470, -1500, 3380), (30, 300, 300))
    s.cyl(GREY_L, (2350, -1620, -300), 900, 360, steps=12)
    s.cyl(WATER, (2350, -1620, -290), 820, 366, steps=12, glow=GW)
    for sx in (-1, 1):
        s.box(WOOD, (-2100 + sx * 1150, -2300, 180), (40, 40, 1160))
    s.box(NAVY, (-2100, -2300, 640), (2400, 30, 300))
    _letters(s, 'LOG FALLS', -2316, 560, 150, AMBER, x=-2100.0)
    for x, y, z, r_ in ((1700, 300, 1900, 250), (-2600, 200, 1700, 240), (-3300, -1300, 0, 250), (3000, -500, 0, 240), (700, -1700, 0, 200),
                        (-700, -1900, 0, 220)):
        s.cyl(PINE, (x, y, z - 250), r_, r_ * 2.8 + 190, steps=6, top_r=8)
    w = k.group('Light', LightAxis=d, LightLen=640.0, LightRate=1.5, LightDuty=0.55, LightSoft=0.2, LightMin=0.45)
    tilt_box(w, CYAN, (A[0], A[1] - 20, A[2] + 68), (Bt[0], Bt[1] - 20, Bt[2] + 68), 300, 76, glow=G2)
    g = k.group('Conveyor', ConvStart=(A[0], A[1], A[2] + 110.0), ConvDir=d, ConvLen=ln, ConvRate=0.1, ConvRand=1.0)
    pitch = math.degrees(math.atan2(d[2], d[0]))
    for i in range(2):
        def log(m, i=i):
            m.base = ft_art._xf((0, 0, 0), (pitch, 0, 0))
            try:
                m.box(WOOD, (0, 0, 46), (430, 170, 92))
                m.box(WOOD_D, (196, 0, 70), (60, 180, 140))
                for j in range(3):
                    m.sphere(SHIRT[(i + j) % len(SHIRT)], (-130 + j * 110, 0, 130), 36, steps=(3, 5))
            finally:
                m.base = None
        g.piece(log, a=i * 0.5)
    p = k.group('Plume', rough=0.9, PlumeBase=(Bt[0] + 420.0, Bt[1] - 60.0, 120.0), PlumeRise=760.0, PlumeDrift=140.0, PlumeSize0=0.5, PlumeSize1=1.9,
                PlumeRate=0.7, PlumeSpread=280.0, PlumeEase=0.6)
    for i in range(7):
        p.piece(lambda m: m.sphere(SPRAY, (0, 0, 0), 70, steps=(3, 5), glow=0.12), a=i / 7.0, b=0.8 + 0.4 * ((i * 3) % 4) / 3.0, c=i * 0.41)
    return dict(grow=(300.0, 300.0, 300.0), grow_down=(300.0, 300.0, 0.0))


def swan_boats(k, run=5200.0):
    """Four swan boats in a slow line across a lake of the lagoon, and round again: the table stands them at the
    lagoon's water level with the lakes (their z = 0 is the water)."""
    g = k.group('Conveyor', ConvStart=(0.0, -run / 2.0, 0.0), ConvDir=(0.0, 1.0, 0.0), ConvLen=run, ConvRate=0.022, ConvRand=1.0)
    for i in range(4):
        def swan(m, c=(MAG, CYAN, AMBER, MINT)[i], x=(i % 2) * 300.0 - 150.0):
            m.sphere(WHITE, (x, 0, 40), 120, scale=(0.8, 1.3, 0.45), steps=(4, 7))
            rod(m, WHITE, (x, 90, 60), (x, 150, 210), 22, steps=5)
            m.sphere(WHITE, (x, 172, 226), 34, steps=(3, 5))
            m.box(ORANGE, (x, 210, 220), (16, 44, 14))
            m.box(c, (x, -40, 124), (150, 124, 12))
            for sx in (-1, 1):
                m.box(c, (x + sx * 68, -40, 92), (8, 8, 60))
        g.piece(swan, a=i / 4.0 + 0.03 * i)
    return dict(grow=(320.0, run / 2.0 + 240.0, 280.0), grow_down=(320.0, run / 2.0 + 240.0, 0.0))


KITS = [('SM_SkyRide', sky_ride), ('SM_WoodCoaster', wood_coaster), ('SM_Flume', flume), ('SM_SwanBoats', swan_boats)]

_FLOOR = [('SM_FloorPlaza', floor_plaza), ('SM_FloorSpoke', floor_spoke), ('SM_FloorPave', floor_pave), ('SM_FloorLawn', floor_lawn),
          ('SM_FloorGames', floor_games), ('SM_FloorQueue', floor_queue), ('SM_FloorKart', floor_kart), ('SM_FloorPool', floor_pool),
          ('SM_FloorYard', floor_yard), ('SM_CutLawn', cut_lawn), ('SM_CutLawnLow', cut_lawn_low), ('SM_Crowd', crowd),
          ('SM_Sky1', lambda m: sky_card(m, SKY1, 42000.0, '#5a2f8e', 0.06, 12, 41)),
          ('SM_Sky2', lambda m: sky_card(m, SKY2, 52000.0, '#8a3a98', 0.07, 8, 43))]

_NEAR = [
    ('SM_QueueRails', queue_rails), ('SM_ParkFence', park_fence), ('SM_PlazaSeats', plaza_seats), ('SM_CoveBarrels', cove_barrels),
    ('SM_GameStall', game_stall), ('SM_GameStall2', lambda m: game_stall(m, TEAL, CREAM, MAG, seed=2)),
    ('SM_GameStall3', lambda m: game_stall(m, PLUM, AMBER, CYAN, seed=3)), ('SM_FoodStand', food_stand), ('SM_CandyCart', candy_cart),
    ('SM_HighStriker', high_striker), ('SM_Bandstand', bandstand),
    ('SM_TicketGate', ticket_gate), ('SM_Fountain', fountain), ('SM_Mascot', mascot), ('SM_BigWheel', big_wheel),
    ('SM_PirateShip', pirate_ship), ('SM_SkullRock', skull_rock),
    ('SM_JetRide', jet_ride), ('SM_Saucer', saucer), ('SM_PlanetPosts', planet_posts),
    ('SM_Funhouse', funhouse), ('SM_SwingRide', swing_ride), ('SM_Maintenance', maintenance),
    ('SM_PathLights', path_lights), ('SM_CoasterHill', coaster_hill),
    ('SM_SignMidway', lambda m: land_sign(m, 'THE MIDWAY', AMBER, MAG, _em_star)),
    ('SM_SignPirate', lambda m: land_sign(m, 'PIRATE COVE', CREAM, RED, _em_skull)),
    ('SM_SignLagoon', lambda m: land_sign(m, 'THE LAGOON', CYAN, TEAL, _em_wave)),
    ('SM_SignRocket', lambda m: land_sign(m, 'ROCKET LAND', CYAN, MAG, _em_rocket)),
    ('SM_SignFun', lambda m: land_sign(m, 'FUNHOUSE', MAG, TEAL, _em_hat)),
    ('SM_SignRide', lambda m: land_sign(m, 'THE LAST RIDE', AMBER, RED, _em_nose)),
]
_NEW = [('SM_MarqueeTower', marquee_tower), ('SM_Robot', robot)]     # third pass: a slab, no post
_WATER = [('SM_Pond', pond), ('SM_Lake', lake), ('SM_Lake2', lambda m: lake(m, seed=13, serpent=True))]
_FAR = [('SM_FarCoaster', far_coaster), ('SM_FarWheel', far_wheel), ('SM_FarChute', far_chute), ('SM_FarBigTops', far_bigtops),
        ('SM_FarRocket', far_rocket), ('SM_FarFort', far_fort)]
_FLAT = [('SM_FacadeRow', facade_row), ('SM_Galleon', galleon), ('SM_Balloon', balloon)]         # third pass: drawn with their own footing (the balloon flies)
# the floor, bank, crowd and card pieces are drawn with their own skirts and take no hidden post (it would double their
# bounds); the travelling props are not jobs at all: life_props() builds them (KITS)
JOBS = ([(n, _std(f)) for n, f in _NEAR] + [(n, _std(f, post=False)) for n, f in _NEW] + [(n, _std(f, slab=0.0)) for n, f in _WATER + _FAR]
        + list(_FLAT) + list(_FLOOR))
