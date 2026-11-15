"""Additional geometry on the existing Full Throttle model generators, in their authored coordinates."""
import math
import random

import unreal
import ft_art
import ft_mats

AL = unreal.EditorAssetLibrary
MEL = unreal.MaterialEditingLibrary
FINISH = '/Game/Core/VisualFinish'
QUERY = unreal.GeometryScript_MeshQueries
EDIT = unreal.GeometryScript_MeshEdits
LIGHT = '#d7e3f0'
DARK = '#22202a'


def master():
    path = FINISH + '/M_VisualFinish'
    if AL.does_asset_exist(path):
        return AL.load_asset(path)
    m = ft_mats.new_material(FINISH, 'M_VisualFinish', shading=unreal.MaterialShadingModel.MSM_DEFAULT_LIT)
    for flag in ('used_with_instanced_static_meshes', 'used_with_geometry_collections'):
        m.set_editor_property(flag, True)
    col = ft_mats.vparam(m, 'Color', (0.3, 0.3, 0.3, 1), y=-400)
    source = ft_mats.expr(m, unreal.MaterialExpressionLocalPosition)
    soot = ft_mats.sparam(m, 'Soot', 0.0, y=100)
    rough = ft_mats.sparam(m, 'Rough', 0.55, y=200)
    metal = ft_mats.sparam(m, 'Metal', 0.1, y=300)
    glow = ft_mats.sparam(m, 'Glow', 0.0, y=400)
    alert = ft_mats.sparam(m, 'Alert', 0.0, y=500)
    gain = ft_mats.sparam(m, 'GlowGain', 10.5, y=600)
    tint = ft_mats.custom(m, """
float3 cell = floor(Local * 0.095);
float fleck = frac(dot(frac(cell * float3(0.0639, 0.1301, 0.0098)), float3(4.527, 8.831, 4.191)));
float3 broad = sin(Local * float3(0.012, 0.016, 0.021));
float grain = lerp(fleck - 0.5, 0.0, saturate(max(length(ddx(Local)), length(ddy(Local))) * 0.11));
float wear = (broad.x + broad.y + broad.z) * 0.023 + grain * 0.095;
float contrast = lerp(1.0, 0.35, saturate(Metal*1.6 + Glow));
return lerp(Col * (1.0 + wear * contrast), float3(0.024, 0.02, 0.028), saturate(Soot * (0.74 + fleck * 0.47)));
""", [('Col', col, ''), ('Local', source, ''), ('Soot', soot, ''),
         ('Metal', metal, ''), ('Glow', glow, '')], desc='Local paint and wear')
    emissive = ft_mats.custom(m, 'return Col * Glow * Gain * (1.0 + Alert * 2.5);',
                             [('Col', col, ''), ('Glow', glow, ''), ('Gain', gain, ''), ('Alert', alert, '')],
                             desc='Signal lights')
    r = ft_mats.custom(m, 'return saturate(max(0.32, Rough) + Soot * 0.36);',
                      [('Rough', rough, ''), ('Soot', soot, '')],
                      out_type=unreal.CustomMaterialOutputType.CMOT_FLOAT1, desc='Surface finish')
    mm = ft_mats.custom(m, 'return Metal * 0.32;', [('Metal', metal, '')],
                       out_type=unreal.CustomMaterialOutputType.CMOT_FLOAT1, desc='Cel-friendly metal')
    for node, pin in ((tint, unreal.MaterialProperty.MP_BASE_COLOR),
                      (emissive, unreal.MaterialProperty.MP_EMISSIVE_COLOR),
                      (r, unreal.MaterialProperty.MP_ROUGHNESS), (mm, unreal.MaterialProperty.MP_METALLIC)):
        MEL.connect_material_property(node, '', pin)
    return ft_mats.finish(m)


def enabled(path):
    # The Bayshore scenery, M_Paint and billboard frames are shared with ongoing building work.
    return (path.startswith('/Game/Maps/') and not path.startswith('/Game/Maps/Bayshore/')
            or path.startswith('/Game/Characters/') or path == '/Game/FX/SM_Tracer')


THEMES = {
    'ChemicalWorks': ('#234650', '#f4ba32', '#70e9ca'),
    'Nevada': ('#634552', '#e0934e', '#ffdb9d'),
    'Volcano': ('#382833', '#df572b', '#ffb660'),
    'ThemePark': ('#463057', '#fe497f', '#ffd66a'),
    'Glacier': ('#52617b', '#c7deef', '#fd6854'),
    'Abyss': ('#233c54', '#c88a54', '#77eee4'),
    'Metro': ('#262e48', '#87746d', '#99e4fa'),
}
CREW = {
    0: ('#302342', '#b396dc', '#fa6a9e'),
    1: ('#263235', '#c6c455', '#8fed79'),
    2: ('#544338', '#d9ab77', '#e75836'),
    3: ('#3c303b', '#c19ea8', '#ff7a35'),
    4: ('#502d5e', '#d7a98f', '#39deed'),
    5: ('#284764', '#b3d5e7', '#ff9058'),
    6: ('#244655', '#bc935d', '#7cffe0'),
    7: ('#292b3b', '#b2a188', '#69d8ee'),
}


def envelope(meshes):
    boxes = [QUERY.get_mesh_bounding_box(mesh) for mesh in meshes.values() if mesh.get_triangle_count()]
    if not boxes:
        return None
    lo = tuple(min(getattr(b.min, k) for b in boxes) for k in 'xyz')
    hi = tuple(max(getattr(b.max, k) for b in boxes) for k in 'xyz')
    return lo, tuple(b - a for a, b in zip(lo, hi))


def at(lo, size, *p):
    return tuple(a + d * v for a, d, v in zip(lo, size, p))


def dimensions(size, *p):
    return tuple(max(0.5, d * v) for d, v in zip(size, p))


def mesh_box(m, lo, size, col, center, span, rot=(0, 0, 0), glow=0.0):
    m.box(col, at(lo, size, *center), dimensions(size, *span), rot=rot, glow=glow)


def marker(m, lo, size, accent, bright, side=1):
    """Raised mounting bracket, glass optic and segmented edge reflector."""
    center = .70 if side > 0 else .30
    mesh_box(m, lo, size, DARK, (.48, center, .14), (.70, .22, .11))
    mesh_box(m, lo, size, accent, (.47, center, .14), (.75, .12, .045))
    for u in (.22, .46, .70):
        mesh_box(m, lo, size, bright, (u, center, .17), (.07, .115, .027), glow=.32)


def growing_rock(m, lo, size, dark, rim, bright, name):
    # Seeded facets, all tucked into the original placement envelope.
    r = random.Random(sum(map(ord, name)))
    for k in range(6):
        x, y, z = r.uniform(.18, .82), r.uniform(.16, .84), r.uniform(.14, .55)
        q = at(lo, size, x, y, z)
        radius = min(size[0], size[1]) * r.uniform(.105, .18)
        m.sphere(rim if k % 2 else dark, q, radius,
                 scale=(1.02, r.uniform(.55, .9), min(size[2] * .13 / radius, 1.45)), steps=(5, 7),
                 rot=(r.uniform(-12, 12), r.uniform(0, 180), r.uniform(-16, 16)))
    if name in ('SM_Volcano', 'SM_Crag', 'SM_Spike', 'SM_Spike2', 'SM_Boulder'):
        for u in (.31, .46, .64):
            mesh_box(m, lo, size, bright, (.16, u, .25), (.015, .09, .012), rot=(0, 0, 26), glow=.42)


def foliage(m, lo, size, name):
    leafy = name.startswith(('SM_Joshua', 'SM_Pine', 'SM_Tree'))
    algae = name.startswith('SM_Kelp')
    seed = random.Random(sum(ord(c) for c in name))
    for i in range(7 if algae else 5):
        z = .27 + i * (.085 if algae else .10)
        angle = 2.4 * i + seed.uniform(-.24, .24)
        x, y = .49 + .21 * math.cos(angle), .52 + .22 * math.sin(angle)
        length = .09 + .05 * seed.random()
        if algae:
            blade = [at(lo, size, x, y, z), at(lo, size, x - length, y, z + .16),
                     at(lo, size, x + .03, y, z + .22)]
            m.side('#6b9c56', [(a, c) for a, _, c in blade], blade[0][1] - 1.6, blade[0][1] + 1.6)
        else:
            r = min(size[:2]) * length
            color = '#dbe8ef' if name.startswith('SM_Pine') and i % 2 else ('#769a6a' if leafy else '#9ba25f')
            m.sphere(color, at(lo, size, x, y, z), r, scale=(1, .65, .56), steps=(4, 6),
                     rot=(0, math.degrees(angle), i * 5))


def signature_vehicle(m, lo, size, index, armor, trim, light):
    """Consistent crew hardware with different shapes per theme, read at chase distance."""
    for y in (.18, .82):
        if index in (0, 2):
            mesh_box(m, lo, size, trim, (.30, y, .57), (.25, .085, .085), rot=(0, 0, 12))
            mesh_box(m, lo, size, armor, (.35, y, .61), (.18, .11, .05))
        elif index in (1, 3, 6):
            r = min(size[1] * .07, size[2] * .09)
            m.cyl(trim, at(lo, size, .30, y, .52), r, size[2] * .19, steps=10, top_r=r * .76)
            m.cyl(light, at(lo, size, .30, y, .71), r * .64, size[2] * .024, steps=10, glow=.74)
        elif index in (4, 5):
            for x in (.24, .31, .38):
                mesh_box(m, lo, size, trim, (x, y, .60), (.056, .087, .052), rot=(0, 0, -18))
                mesh_box(m, lo, size, light, (x - .026, y, .61), (.017, .064, .035), glow=.62)
        else:
            mesh_box(m, lo, size, trim, (.29, y, .53), (.28, .075, .041))
            mesh_box(m, lo, size, light, (.18, y, .545), (.10, .041, .020), glow=.68)

def louver(m, lo, size, dark, rim, lamp):
    """A recessed-looking service panel with vents and a tiny lit diagnostic strip."""
    for y in (.21, .71):
        mesh_box(m, lo, size, rim, (.035, y, .29), (.040, .26, .27))
        mesh_box(m, lo, size, dark, (.016, y, .29), (.028, .23, .22))
        for j in range(4):
            z = .224 + .04 * j
            mesh_box(m, lo, size, rim, (.009, y, z), (.016, .18, .012))
        mesh_box(m, lo, size, lamp, (.010, y + .07, .38), (.015, .026, .024), glow=.42)


FIXTURES = ('SM_Clamp', 'SM_Pole', 'SM_Torch', 'SM_Bulbs', 'SM_Gate', 'SM_Buoy', 'SM_Street')
ORGANIC = ('SM_Spike', 'SM_Boulder', 'SM_Crag', 'SM_Volcano', 'SM_DeadTree', 'SM_Rock', 'SM_Butte',
           'SM_Peak', 'SM_Ridge', 'SM_SnowRock', 'SM_SeaRock', 'SM_Pine', 'SM_Kelp', 'SM_Coral',
           'SM_Cactus', 'SM_Joshua', 'SM_Tree')


def environment(m, path, lo, size):
    region, leaf = path.split('/')[3], path.rsplit('/', 1)[-1]
    dark, rim, lamp = THEMES[region]
    if leaf.startswith(FIXTURES):
        marker(m, lo, size, rim, lamp, -1 if leaf.endswith('R') else 1)
    elif leaf.startswith(('SM_Joshua', 'SM_Pine', 'SM_Kelp', 'SM_Cactus', 'SM_Tree')):
        foliage(m, lo, size, leaf)
    elif leaf.startswith(ORGANIC):
        growing_rock(m, lo, size, dark, rim, lamp, leaf)
    elif leaf in ('SM_Tank', 'SM_Tank2', 'SM_Sphere', 'SM_Stack', 'SM_Tower', 'SM_Vat', 'SM_Vent', 'SM_Hydrant'):
        for z, radius in ((.092, .44), (.17, .45)):
            m.cyl(rim if z > .1 else dark, at(lo, size, .5, .5, z), min(size[:2])*radius,
                  max(5, size[2]*.022), steps=12)
        mesh_box(m, lo, size, dark, (.12, .5, .46), (.18, .28, .13))
        for y in (.42, .5, .58):
            mesh_box(m, lo, size, lamp, (.045, y, .46), (.025, .032, .037), glow=.36)
    elif leaf in ('SM_Wreck', 'SM_Dome', 'SM_City', 'SM_ParkSkyline', 'SM_Skyline', 'SM_Vegas'):
        for k in range(7):
            f = (k + 1) / 8.0
            mesh_box(m, lo, size, dark, (.20, f, .08), (.22, .095, .10))
            mesh_box(m, lo, size, rim, (.105, f, .12), (.03, .085, .016))
            mesh_box(m, lo, size, lamp, (.095, f, .12), (.016, .030, .012), glow=.30)
        if leaf == 'SM_Dome':
            for k in range(12):
                a = math.tau * k / 12
                m.box(rim, at(lo, size, .5 + .43 * math.cos(a), .5 + .43 * math.sin(a), .57),
                      dimensions(size, .035, .105, .11), rot=(0, math.degrees(a), 0))
    elif leaf == 'SM_Fishes':
        # A few larger silhouettes anchor the original school of small luminous shapes.
        for i, v in enumerate((.18, .37, .58, .77)):
            x, y, z = at(lo, size, .43, v, .25 + i * .11)
            sc = min(size)*.06
            m.sphere(lamp if i % 2 else rim, (x, y, z), sc,
                     scale=(2.2, .5, .78), steps=(5, 7), glow=.27)
            m.side(rim, [(x-sc*1.28, z), (x-sc*3, z+sc*.85), (x-sc*2.72, z), (x-sc*3, z-sc*.78)],
                   y-sc*.10, y+sc*.10)
    else:
        louver(m, lo, size, dark, rim, lamp)
        if leaf.startswith('SM_Tower') or leaf in ('SM_Spire', 'SM_Pylon', 'SM_DropTower'):
            for z in (.28, .53, .78):
                mesh_box(m, lo, size, rim, (.14, .5, z), (.05, .67, .014))
                for v in (.33, .67):
                    mesh_box(m, lo, size, lamp, (.109, v, z + .017), (.015, .06, .014), glow=.40)
        elif leaf in ('SM_Motel', 'SM_Gas', 'SM_Booth', 'SM_Newsstand', 'SM_Chalet', 'SM_Tent', 'SM_Carousel'):
            for v in (.18, .32, .46, .60, .74, .87):
                mesh_box(m, lo, size, rim, (.075, v, .43), (.07, .069, .038))
                mesh_box(m, lo, size, lamp, (.04, v, .439), (.024, .047, .022), glow=.33)


BOSSES = ('SM_Juggernaut', 'SM_Tanker', 'SM_Carrier', 'SM_FireTruck', 'SM_ClownCar',
          'SM_Yeti', 'SM_SharkCar', 'SM_Limo')


def vehicle(m, path, lo, size):
    leaf = path.rsplit('/', 1)[-1]
    if '/Themed/' in path:
        index = int(path.split('/')[-2][1:])
    elif '/Bosses/' in path and leaf in BOSSES:
        index = BOSSES.index(leaf)
    elif '/SnakeEyes/' in path:
        index = 2
    else:
        index = 0
    armor, trim, lamp = CREW[index]
    if leaf.startswith('SM_Kestrel_'):
        if leaf == 'SM_Kestrel_Body':
            for s in (.035, .965):
                mesh_box(m, lo, size, '#791b3e', (.42, s, .46), (.26, .04, .11))
                mesh_box(m, lo, size, '#53e9e5', (.42, s, .506), (.21, .041, .018), glow=.35)
                for x in (.34, .39, .44):
                    mesh_box(m, lo, size, '#ebaf76', (x, s, .47), (.015, .043, .024))
        return
    if leaf.startswith('SM_Henchman') or leaf.startswith('SM_Yeti'):
        return
    if leaf in ('SM_Moto', 'SM_Hog', 'SM_Hog2'):
        # ft_bikes models trim against the actual fuel tank and panels.
        return
    if leaf == 'SM_Heli' or leaf in BOSSES or leaf in ('SM_Rammer', 'SM_Suv', 'SM_Truck'):
        # The current enemy exporters attach fitted panels to the authored body.
        return
    if leaf == 'SM_HeliRotor':
        return
    if 'Heli' in leaf:
        for s in (.17, .83):
            mesh_box(m, lo, size, armor, (.64, s, .46), (.20, .12, .12))
            mesh_box(m, lo, size, lamp, (.65, s, .49), (.14, .034, .022), glow=.6)
        return
    is_civilian = '/Traffic/' in path
    if is_civilian:
        armor, trim, lamp = '#293046', '#d7e3f0', '#f1ddab'
    for s in (.025, .975):
        mesh_box(m, lo, size, armor, (.43, s, .41), (.28, .042, .20))
        mesh_box(m, lo, size, trim, (.45, s, .49), (.21, .044, .028))
        for x in (.35, .41, .47):
            mesh_box(m, lo, size, DARK, (x, s, .43), (.022, .054, .070), rot=(0, 0, -12))
        mesh_box(m, lo, size, '#ff584a' if is_civilian else lamp, (.055, s, .52), (.061, .070, .059), glow=.60)
    if not is_civilian:
        signature_vehicle(m, lo, size, index, armor, trim, lamp)
    if '/Bosses/' in path:
        # Broad rear identification panels where the chase camera can pick them out.
        mesh_box(m, lo, size, armor, (.019, .50, .60), (.036, .64, .24))
        for i, y in enumerate((.26, .38, .50, .62, .74)):
            mesh_box(m, lo, size, lamp if i == 2 else trim, (.001, y, .62),
                     (.010, .073, .106), rot=(0, 0, (-24 if i < 2 else 24)), glow=.48 if i == 2 else .15)


def projectile(m, path, lo, size):
    leaf = path.rsplit('/', 1)[-1]
    if leaf in ('SM_BigTracer', 'SM_Tracer', 'SM_Bolt'):
        return
    tone = '#ffe987' if leaf == 'SM_Fireball' else '#d7efff'
    if leaf == 'SM_Bomb':
        tone = '#ff7838'
    for side in (.28, .72):
        mesh_box(m, lo, size, tone, (.54, side, .59), (.22, .07, .065), glow=.46)


def refine(meshes, path):
    """Add readable silhouette details without moving source origins or collision."""
    bounds = envelope(meshes)
    if bounds is None or min(bounds[1]) < 1.0:
        return
    m = ft_art.Model(path.rsplit('/', 1)[-1])
    if path.startswith('/Game/Maps/'):
        environment(m, path, *bounds)
    elif '/Projectiles/' in path or '/SM_Missile' in path or path == '/Game/FX/SM_Tracer':
        projectile(m, path, *bounds)
    elif '/Pickups/' in path:
        marker(m, *bounds, '#23243b', '#ffd23f')
    else:
        vehicle(m, path, *bounds)
    for key, addition in m.meshes.items():
        if key in meshes:
            EDIT.append_mesh(meshes[key], addition, unreal.Transform())
        else:
            meshes[key] = addition
