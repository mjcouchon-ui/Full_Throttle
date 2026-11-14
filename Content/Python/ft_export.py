"""Turn the FULL THROTTLE 1.x models (Geometry Script code, original art) into Unreal 5.8 game assets:
Nanite static meshes with PBR palette materials (M_Paint instances), recentred, with simple collision
where physics needs it, and the KESTREL split into body, detachable parts and wheels.

Run in the editor over the Unreal MCP connector:  import ft_export; ft_export.run()   (or run(['kestrel']))
"""
import math
import unreal
import ft_art
import ft_car
import ft_fleet
import ft_city
import ft_chem
import ft_desert
import ft_heli

AL = unreal.EditorAssetLibrary
MEL = unreal.MaterialEditingLibrary
AT = unreal.AssetToolsHelpers.get_asset_tools()
GS = unreal.GeometryScript_Primitives
PALETTE = '/Game/Core/Palette'
_MI = {}

METAL = {'#d7e3f0': (1.0, 0.14), '#8f9bb3': (0.9, 0.3), '#ffc93c': (1.0, 0.24), '#c9c4d6': (0.9, 0.25), '#a39cc2': (0.8, 0.35)}
GLASSES = ('#233a63', '#4a7fc0', '#5ff0ff')
DARK = ('#1c1a26', '#22202a', '#1c1f4a')
TYRES = ('#26232e',)


def finish(col, kind):
    c = col.lower()
    if c in METAL:
        return METAL[c]
    if c in GLASSES:
        return (0.2, 0.04)
    if c in TYRES:
        return (0.0, 0.85)
    if c in DARK:
        return (0.1, 0.5)
    if kind == 'car':
        return (0.45, 0.2)
    return (0.0, 0.6)


def palette_mi(col, glow, kind):
    metal, rough = finish(col, kind)
    key = (col.lower(), round(glow, 2), metal, rough)
    if key in _MI:
        return _MI[key]
    name = 'MI_%s_g%d_m%d_r%d' % (col.lstrip('#').lower(), int(glow * 100), int(metal * 100), int(rough * 100))
    path = PALETTE + '/' + name
    if AL.does_asset_exist(path):
        mi = AL.load_asset(path)
    else:
        if not AL.does_directory_exist(PALETTE):
            AL.make_directory(PALETTE)
        mi = AT.create_asset(name, PALETTE, unreal.MaterialInstanceConstant, unreal.MaterialInstanceConstantFactoryNew())
        mi.set_editor_property('parent', AL.load_asset('/Game/Core/Materials/M_Paint'))
        r, g, b = ft_art.hex_lin(col)
        MEL.set_material_instance_vector_parameter_value(mi, 'Color', unreal.LinearColor(r, g, b, 1))
        MEL.set_material_instance_scalar_parameter_value(mi, 'Glow', glow)
        MEL.set_material_instance_scalar_parameter_value(mi, 'Metal', metal)
        MEL.set_material_instance_scalar_parameter_value(mi, 'Rough', rough)
        MEL.update_material_instance(mi)
        AL.save_loaded_asset(mi)
    _MI[key] = mi
    return mi


def save_mesh(meshes, asset_path, kind='prop', offset=(0, 0, 0), yaw=0.0, nanite=True, collision=None, scale=1.0):
    """meshes: {(col, glow): DynamicMesh}. collision: None, 'box' (fitted to the mesh) or (cx, cy, cz, sx, sy, sz)."""
    if not meshes:
        return None
    combined = unreal.DynamicMesh()
    mats = []
    xf = unreal.Transform(unreal.Vector(*offset), unreal.Rotator(0, 0, 0), unreal.Vector(scale, scale, scale))
    rot = unreal.Transform(unreal.Vector(0, 0, 0), unreal.Rotator(roll=0, pitch=0, yaw=yaw), unreal.Vector(1, 1, 1))
    for i, ((col, glow), m) in enumerate(meshes.items()):
        unreal.GeometryScript_Normals.set_per_face_normals(m)
        unreal.GeometryScript_Materials.enable_material_i_ds(m)
        unreal.GeometryScript_Materials.remap_material_i_ds(m, 0, i)
        unreal.GeometryScript_MeshEdits.append_mesh(combined, m, xf * rot)
        mats.append(palette_mi(col, glow, kind))
    folder, name = asset_path.rsplit('/', 1)
    if not AL.does_directory_exist(folder):
        AL.make_directory(folder)
    if AL.does_asset_exist(asset_path):
        AL.delete_asset(asset_path)
    opts = unreal.GeometryScriptCreateNewStaticMeshAssetOptions()
    opts.set_editor_property('enable_nanite', nanite)
    opts.set_editor_property('enable_collision', False)
    opts.set_editor_property('enable_recompute_normals', False)
    sm, outcome = unreal.GeometryScript_NewAssetUtils.create_new_static_mesh_asset_from_mesh(combined, asset_path, opts)
    if sm is None:
        raise RuntimeError('could not create ' + asset_path)
    sm.set_editor_property('static_materials', [unreal.StaticMaterial(material_interface=mats[i], material_slot_name='M%d' % i) for i in range(len(mats))])
    if collision is not None:
        box = unreal.DynamicMesh()
        if collision == 'box':
            b = unreal.GeometryScript_MeshQueries.get_mesh_bounding_box(combined)
            c = ((b.min.x + b.max.x) / 2, (b.min.y + b.max.y) / 2, (b.min.z + b.max.z) / 2)
            s = (max(b.max.x - b.min.x, 2), max(b.max.y - b.min.y, 2), max(b.max.z - b.min.z, 2))
        else:
            c, s = collision[:3], collision[3:]
        GS.append_box(box, unreal.GeometryScriptPrimitiveOptions(), unreal.Transform(unreal.Vector(*c), unreal.Rotator(0, 0, 0), unreal.Vector(1, 1, 1)), s[0], s[1], s[2], 0, 0, 0, unreal.GeometryScriptPrimitiveOriginMode.CENTER)
        co = unreal.GeometryScriptCollisionFromMeshOptions()
        co.set_editor_property('method', unreal.GeometryScriptCollisionGenerationMethod.ALIGNED_BOXES)
        unreal.GeometryScript_Collision.set_static_mesh_collision_from_mesh(box, sm, co)
    AL.save_loaded_asset(sm)
    return sm


def model(build):
    m = ft_art.Model('x')
    build(m)
    return m


# ------------------------------------------------------------------------------------------- custom bits
def rotor(m):
    m.cyl('#2a2a33', (0, 0, -10), 30, 26, steps=12)
    for a in (0, 90):
        m.box('#3a3846', (0, 0, 8), (1100, 34, 5), rot=(0, a, 0))
    m.cyl('#3a3846', (0, 0, 8), 520, 1, steps=40)  # a faint disc so the blur reads


def henchman(m):
    SUIT, SKIN, BLK, TIE = '#2b2a3a', '#e0a070', '#141218', '#ff3355'
    for s in (-1, 1):
        m.box(BLK, (0, s * 14, 45), (24, 16, 90))
        m.box(SUIT, (0, s * 34, 128), (16, 14, 70), rot=(0, 0, s * 20))
    m.box(SUIT, (0, 0, 125), (30, 46, 70))
    m.box(TIE, (16, 0, 132), (2, 8, 40))
    m.sphere(SKIN, (0, 0, 176), 15)
    m.box(BLK, (13, 0, 179), (4, 26, 6))


def crate(m):
    m.box('#ff8a1e', (0, 0, 55), (110, 110, 110))
    for z in (8, 102):
        m.box('#2b2350', (0, 0, z), (114, 114, 10))
    m.box('#ffe14d', (56, 0, 55), (2, 70, 70), glow=0.9)
    m.box('#2b2350', (57, 0, 55), (2, 46, 12))


def wrench(m):
    m.box('#2fbf71', (0, 0, 50), (100, 100, 100))
    m.box('#eef6ff', (51, 0, 50), (2, 18, 70), glow=0.8)
    m.cyl('#eef6ff', (51, 0, 84), 18, 2, rot=(0, 0, 90), steps=12, glow=0.8)
    m.box('#1f9a62', (0, 0, 2), (106, 106, 6))


def barrel(m, col='#ff3355', glow=0.0):
    m.cyl(col, (0, 0, 0), 42, 120, steps=18, glow=glow)
    for z in (20, 96):
        m.cyl('#2b2350', (0, 0, z), 44, 8, steps=18)
    m.box('#ffd23f', (42, 0, 60), (2, 30, 30), glow=0.6)


def missile(m):
    m.cyl('#8f9bb3', (-60, 0, 0), 9, 120, rot=(0, 0, 90), steps=12)
    m.cyl('#ff2d3c', (60, 0, 0), 9, 30, rot=(0, 0, 90), steps=12, top_r=1.0)
    for a in (0, 90, 180, 270):
        m.box('#1c1a26', (-52, 0, 0), (26, 2, 34), rot=(a, 0, 0))
    m.cyl('#ffae24', (-64, 0, 0), 7, 4, rot=(0, 0, 90), steps=10, glow=2.0)


def tracer(m):
    m.box('#ffd27a', (0, 0, 0), (260, 7, 7), glow=3.0)


# ------------------------------------------------------------------------------------------- jobs
def kestrel():
    D = '/Game/Characters/Kestrel/'
    ft_car.WHEEL_MODE = 'skip'
    m = ft_art.Model('kestrel')
    ft_car.build_into(m)
    ft_car.WHEEL_MODE = 'normal'
    off = (-215, 0, 0)
    # the chassis box sits above the tyres, so the wheels (Chaos suspension) carry the car
    save_mesh(m.meshes, D + 'SM_Kestrel_Body', 'car', off, collision=(0, 0, 70, 440, 210, 80))
    for part, meshes in m.parts.items():
        save_mesh(meshes, D + 'SM_Kestrel_' + part, 'car', off, collision='box')
    # wheels, built alone at the origin as left wheels (the game mirrors the right ones)
    for name, r, w in (('WheelRear', 40, 56), ('WheelFront', 31, 26)):
        wm = ft_art.Model(name)
        ft_car.wheel(wm, 0, 0, r, -w / 2.0, w / 2.0, -1)
        save_mesh(wm.meshes, D + 'SM_Kestrel_' + name, 'car')
    save_mesh(model(missile).meshes, D + 'SM_Missile', 'car', nanite=False)


def fleet():
    C = '/Game/Characters/'
    save_mesh(model(lambda m: ft_fleet.rammer(m)).meshes, C + 'Syndicate/SM_Rammer', 'car', (-235, 0, 0), collision='box')
    save_mesh(model(lambda m: ft_fleet.suv(m)).meshes, C + 'Syndicate/SM_Suv', 'car', (-260, 0, 0), collision='box')
    save_mesh(model(lambda m: ft_fleet.truck(m)).meshes, C + 'Syndicate/SM_Truck', 'car', (-450, 0, 0), collision='box')
    save_mesh(model(lambda m: ft_fleet.moto(m)).meshes, C + 'Syndicate/SM_Moto', 'car', (-115, 0, 0), collision='box')
    save_mesh(model(henchman).meshes, C + 'Syndicate/SM_Henchman', 'car', collision='box')
    save_mesh(model(lambda m: ft_heli.heli(m)).meshes, C + 'Choppers/SM_Heli', 'car', (0, 0, 0), collision='box')
    save_mesh(model(rotor).meshes, C + 'Choppers/SM_HeliRotor', 'car', nanite=False)
    save_mesh(model(lambda m: ft_desert.hog(m, 1)).meshes, C + 'SnakeEyes/SM_Hog', 'car', (-150, 0, 0), collision='box')
    save_mesh(model(lambda m: ft_desert.hog(m, 2)).meshes, C + 'SnakeEyes/SM_Hog2', 'car', (-150, 0, 0), collision='box')
    civ = [('SM_Sedan0', '#fff1d6', 'sedan'), ('SM_Sedan1', '#8fe3ff', 'sedan'), ('SM_Sedan2', '#ffe14d', 'sedan'),
           ('SM_Hatch', '#7fe3c4', 'hatch'), ('SM_Hatch1', '#b9a4ff', 'hatch'), ('SM_Van', '#ff9ec7', 'van'), ('SM_Van1', '#ffc59e', 'van')]
    for name, col, kind in civ:
        L = {'sedan': 460, 'hatch': 390, 'van': 520}[kind]
        save_mesh(model(lambda m, c=col, k=kind: ft_fleet.sedan(m, c, k)).meshes, C + 'Traffic/' + name, 'car', (-L / 2.0, 0, 0), collision='box')
    save_mesh(model(lambda m: ft_fleet.bus(m)).meshes, C + 'Traffic/SM_Bus', 'car', (-550, 0, 0), collision='box')
    save_mesh(model(lambda m: ft_fleet.ramp(m)).meshes, C + 'Traffic/SM_CarCarrier', 'car', (-565, 0, 0), collision='box')
    save_mesh(model(lambda m: ft_fleet.truck(m, True)).meshes, C + 'Bosses/SM_Juggernaut', 'car', (-625, 0, 0), collision='box')
    save_mesh(model(lambda m: ft_chem.tanker(m)).meshes, C + 'Bosses/SM_Tanker', 'car', (-625, 0, 0), collision='box')
    save_mesh(model(lambda m: ft_desert.carrier(m)).meshes, C + 'Bosses/SM_Carrier', 'car', (-500, 0, 0), collision='box')
    save_mesh(model(crate).meshes, C + 'Pickups/SM_Crate', 'prop', collision='box')
    save_mesh(model(wrench).meshes, C + 'Pickups/SM_Wrench', 'prop', collision='box')
    save_mesh(model(lambda m: barrel(m)).meshes, C + 'Pickups/SM_Barrel', 'prop', collision='box')
    save_mesh(model(lambda m: barrel(m, '#3dff6a', 0.5)).meshes, C + 'Pickups/SM_DrumGreen', 'prop', collision='box')
    save_mesh(model(tracer).meshes, '/Game/FX/SM_Tracer', 'prop', nanite=False)


def bayshore():
    D = '/Game/Maps/Bayshore/Props/'
    jobs = [
        ('SM_Lamp', lambda m: ft_city.lamp(m)), ('SM_LampR', lambda m: ft_city.lamp(m, -1)),
        ('SM_Palm', lambda m: ft_city.palm(m)), ('SM_Palm2', lambda m: ft_city.palm(m, h=700, lean=-16, seed=3)),
        ('SM_HotelPink', lambda m: ft_city.hotel(m)),
        ('SM_HotelMint', lambda m: ft_city.hotel(m, 1500, 1000, 5, ft_city.MINT, ft_city.PEACH, ft_city.HOTPINK, False, ft_city.SKY)),
        ('SM_HotelLav', lambda m: ft_city.hotel(m, 2200, 1200, 9, ft_city.LAV, ft_city.CREAM, ft_city.TEAL, True, ft_city.HOTPINK)),
        ('SM_HotelPeach', lambda m: ft_city.hotel(m, 1400, 900, 4, ft_city.PEACH, ft_city.SKY, ft_city.HOTPINK, True, ft_city.MINT)),
        ('SM_Lifeguard', lambda m: ft_city.lifeguard(m)), ('SM_Lifeguard2', lambda m: ft_city.lifeguard(m, ft_city.TEAL, ft_city.YEL)),
        ('SM_Umbrella', lambda m: ft_city.umbrella(m)), ('SM_Umbrella2', lambda m: ft_city.umbrella(m, ft_city.TEAL, ft_city.YEL)),
        ('SM_Billboard', lambda m: ft_city.billboard(m)), ('SM_Ferris', lambda m: ft_city.ferris(m)),
        ('SM_Lighthouse', lambda m: ft_city.lighthouse(m)), ('SM_Skyline', lambda m: ft_city.skyline(m)),
    ]
    for name, fn in jobs:
        save_mesh(model(fn).meshes, D + name, 'prop')


def chemworks():
    D = '/Game/Maps/ChemicalWorks/Props/'
    jobs = [
        ('SM_Clamp', lambda m: ft_chem.lamp(m)), ('SM_ClampR', lambda m: ft_chem.lamp(m, -1)),
        ('SM_Tank', lambda m: ft_chem.tank(m)), ('SM_Tank2', lambda m: ft_chem.tank(m, 420, 1100, ft_chem.STEELB, ft_chem.MAG)),
        ('SM_Sphere', lambda m: ft_chem.sphere(m)), ('SM_Stack', lambda m: ft_chem.stack(m)), ('SM_Tower', lambda m: ft_chem.tower(m)),
        ('SM_Pipework', lambda m: ft_chem.pipework(m)), ('SM_Vat', lambda m: ft_chem.vat(m)), ('SM_Skyline', lambda m: ft_chem.skyline(m)),
    ]
    for name, fn in jobs:
        save_mesh(model(fn).meshes, D + name, 'prop')


def nevada():
    D = '/Game/Maps/Nevada/Props/'
    jobs = [
        ('SM_Pole', lambda m: ft_desert.pole(m)), ('SM_PoleR', lambda m: ft_desert.pole(m, -1)),
        ('SM_Joshua', lambda m: ft_desert.joshua(m, 620, 1)), ('SM_Joshua2', lambda m: ft_desert.joshua(m, 780, 4)),
        ('SM_Rock', lambda m: ft_desert.rock(m, 0)), ('SM_Rock2', lambda m: ft_desert.rock(m, 1)), ('SM_Butte', lambda m: ft_desert.butte(m)),
        ('SM_Cactus', lambda m: ft_desert.cactus(m)), ('SM_Motel', lambda m: ft_desert.motel(m)), ('SM_Pylon', lambda m: ft_desert.pylon(m)),
        ('SM_Gas', lambda m: ft_desert.gas(m)), ('SM_Vegas', lambda m: ft_desert.vegas(m)),
    ]
    for name, fn in jobs:
        save_mesh(model(fn).meshes, D + name, 'prop')


GROUPS = {'kestrel': kestrel, 'fleet': fleet, 'bayshore': bayshore, 'chemworks': chemworks, 'nevada': nevada}


def run(only=None):
    done = []
    with unreal.ScopedSlowTask(len(GROUPS), 'FULL THROTTLE meshes') as task:
        for name, fn in GROUPS.items():
            if only and name not in only:
                continue
            task.enter_progress_frame(1, name)
            fn()
            done.append(name)
    return 'exported: ' + ', '.join(done)
