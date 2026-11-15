"""Justin's approved M2 missile carrier / R1 repair toolbox. UE 5.9, centimetres.

Rebuild only this feature: import ft_powerups; ft_powerups.build()
All geometry is procedural. Existing mesh assets are updated in place, preserving references.
"""
import json
import math
import os
import unreal
import ft_art
import ft_mats

ROOT = '/Game/Characters/Pickups'
MATERIALS = ROOT + '/Materials'
ORANGE, GREEN, INK, IVORY = '#ff8a1e', '#2fbf71', '#1b1033', '#eef6ff'
AMBER, MINT = '#ffc93c', '#76ffae'
AL = unreal.EditorAssetLibrary
MEL = unreal.MaterialEditingLibrary


def bevel_box(m, color, center, size, bevel=3, glow=0.0):
    """Four octagonal rings: broad planar faces and real edge chamfers, 60 triangles."""
    hx, hy, hz = [s * 0.5 for s in size]
    b = min(bevel, hx * 0.45, hy * 0.45, hz * 0.45)
    vertices, faces = [], []
    for z, inset in ((-hz, b), (-hz + b, 0), (hz - b, 0), (hz, b)):
        x, y, c = hx - inset, hy - inset, max(b - inset * 0.5, 0.1)
        ring = [(-x+c, -y), (x-c, -y), (x, -y+c), (x, y-c),
                (x-c, y), (-x+c, y), (-x, y-c), (-x, -y+c)]
        vertices.extend([(a+center[0], d+center[1], z+center[2]) for a, d in ring])
    faces.append(list(reversed(range(8))))
    for row in range(3):
        for j in range(8):
            k = (j + 1) % 8
            faces.append([row*8+j, row*8+k, (row+1)*8+k, (row+1)*8+j])
    faces.append(list(range(24, 32)))
    triangles = []
    for face in faces:
        triangles.extend([unreal.IntVector(face[0], face[j], face[j+1]) for j in range(1, len(face)-1)])
    buffers = unreal.GeometryScriptSimpleMeshBuffers()
    buffers.vertices = [unreal.Vector(*v) for v in vertices]
    buffers.triangles = triangles
    unreal.GeometryScript_MeshEdits.append_buffers_to_mesh(m._mesh(color, glow), buffers)


def crate(m):
    """M2: three pointed missiles, broad orange carrier, open dark guard handles."""
    bevel_box(m, INK, (0, 0, 9), (86, 148, 18), 4)
    bevel_box(m, ORANGE, (0, 0, 29), (76, 138, 30), 5)
    bevel_box(m, INK, (0, 0, 46), (78, 134, 8), 2)
    for x in (-34, 34):
        for y in (-63, 63):
            bevel_box(m, INK, (x, y, 20), (20, 26, 34), 4)
    for y in (-40, 0, 40):
        previous = m.base
        m.base = ft_art._xf((0, y, 0))
        m.cyl(IVORY, (0, 0, 49), 14, 63, steps=10)
        m.cyl(ORANGE, (0, 0, 112), 14, 30, steps=10, top_r=0.6)
        m.cyl(INK, (0, 0, 49), 15, 8, steps=10)
        for yaw in (0, 90, 180, 270):
            m.side(ORANGE, [(9, 53), (25, 50), (16, 79), (9, 74)], -2, 2, rot=(0, yaw, 0))
        m.base = previous
    handle = [(-36, 39), (-26, 112), (14, 112), (36, 39),
              (24, 39), (7, 98), (-14, 98), (-24, 39)]
    for y in (-64, 64):
        m.side(INK, handle, y-5, y+5)
    for x in (-44, 44):
        for y in (-39, 39):
            bevel_box(m, ORANGE, (x, y, 45), (12, 22, 38), 3)
            bevel_box(m, IVORY, (x + math.copysign(7, x), y, 46), (3, 12, 13), 1)
        m.box(AMBER, (math.copysign(39, x), 0, 24), (3, 29, 3), glow=1.0)


def wrench(m):
    """R1: green pitched toolbox, open carrying handle, raised double-ended wrench."""
    bevel_box(m, INK, (0, 0, 8), (88, 140, 16), 4)
    bevel_box(m, GREEN, (0, 0, 42), (80, 132, 60), 7)
    bevel_box(m, INK, (0, 0, 72), (83, 135, 7), 2)
    bevel_box(m, GREEN, (0, 0, 84), (80, 132, 23), 8)
    for x in (-36, 36):
        for y in (-59, 59):
            bevel_box(m, INK, (x, y, 18), (18, 24, 30), 3)
    handle = [(-48, 89), (-45, 110), (-29, 131), (29, 131), (45, 110), (48, 89),
              (34, 89), (32, 105), (23, 118), (-23, 118), (-32, 105), (-34, 89)]
    m.side(GREEN, handle, -9, 9, rot=(0, 90, 0))
    bevel_box(m, INK, (0, 0, 123), (19, 48, 9), 2)
    # A concave open-jaw profile, not a cylinder or a medical cross. Duplicated on both broad faces.
    spanner = [(-38, 10), (-27, 20), (-14, 14), (-10, 5), (10, 5), (14, 14),
               (27, 20), (38, 10), (25, 10), (21, 0), (25, -10), (38, -10),
               (27, -20), (14, -14), (10, -5), (-10, -5), (-14, -14),
               (-27, -20), (-38, -10), (-25, -10), (-21, 0), (-25, 10)]
    a = math.radians(19)
    profile = [(0.85*(x*math.cos(a)-z*math.sin(a)), 47+0.85*(x*math.sin(a)+z*math.cos(a))) for x,z in spanner]
    for x in (-43, 43):
        bevel_box(m, INK, (x, 0, 45), (6, 100, 48), 2)
        lo, hi = (-51, -47) if x > 0 else (47, 51)
        m.side(IVORY, profile, lo, hi, rot=(0, 90, 0))
        for y in (-51, 51):
            bevel_box(m, INK, (x, y, 49), (8, 17, 31), 2)
            bevel_box(m, IVORY, (x + math.copysign(5, x), y, 50), (4, 11, 22), 1)
        m.box(MINT, (math.copysign(41, x), 0, 19), (3, 28, 3), glow=1.0)


def glow_master():
    """Isolated, opaque unlit HDR trim. No change to M_Paint or the shared cel pass."""
    m = ft_mats.new_material(MATERIALS, 'M_PickupGlow', shading=unreal.MaterialShadingModel.MSM_UNLIT)
    color = ft_mats.vparam(m, 'Color', (1, 0.4, 0.02, 1), y=-100)
    gain = ft_mats.sparam(m, 'Gain', 3.0, y=0)
    pulse = ft_mats.sparam(m, 'Pulse', 0.12, y=100)
    time = ft_mats.expr(m, unreal.MaterialExpressionTime, y=200)
    em = ft_mats.custom(m, 'return Color * Gain * (1.0 + Pulse * sin(Time * 4.2));',
                       [('Color', color, ''), ('Gain', gain, ''), ('Pulse', pulse, ''), ('Time', time, '')], desc='Pickup HDR pulse')
    MEL.connect_material_property(em, '', unreal.MaterialProperty.MP_EMISSIVE_COLOR)
    return ft_mats.finish(m)


def material(color, glow, glow_parent):
    name = {ORANGE: 'Orange', GREEN: 'Green', INK: 'Guards', IVORY: 'Ivory', AMBER: 'Amber', MINT: 'Mint'}[color]
    r, g, b = ft_art.hex_lin(color)
    if glow:
        return ft_mats.mi(glow_parent, MATERIALS, 'MI_Pickup_' + name,
                          {'Color': (r, g, b, 1)}, {'Gain': 3.0, 'Pulse': 0.12})
    return ft_mats.mi(AL.load_asset('/Game/Core/Materials/M_Paint'), MATERIALS, 'MI_Pickup_' + name,
                      {'Color': (r, g, b, 1)}, {'Glow': 0.0, 'Metal': 0.18 if color == IVORY else 0.0,
                                              'Rough': 0.48 if color == IVORY else 0.65, 'Soot': 0.0, 'Alert': 0.0})


def _save(name, model, glow_parent):
    """Update source mesh in place; preserve its existing simple collision and asset references."""
    combined = unreal.DynamicMesh()
    materials, slots = [], []
    for index, ((color, glow), mesh) in enumerate(model.meshes.items()):
        unreal.GeometryScript_Normals.set_per_face_normals(mesh)
        unreal.GeometryScript_Materials.enable_material_i_ds(mesh)
        unreal.GeometryScript_Materials.remap_material_i_ds(mesh, 0, index)
        unreal.GeometryScript_MeshEdits.append_mesh(combined, mesh, unreal.Transform())
        materials.append(material(color, glow, glow_parent))
        slots.append(unreal.Name('Pickup_' + str(index)))
    if len(materials) > 4:
        raise RuntimeError('Pickup exceeded four material sections')
    tris = combined.get_triangle_count()
    if tris > 5000:
        raise RuntimeError('Pickup exceeded 5000 triangles')
    path = ROOT + '/' + name
    sm = AL.load_asset(path)
    if sm is None:
        options = unreal.GeometryScriptCreateNewStaticMeshAssetOptions()
        options.enable_nanite = False
        sm, _ = unreal.GeometryScript_NewAssetUtils.create_new_static_mesh_asset_from_mesh(combined, path, options)
    options = unreal.GeometryScriptCopyMeshToAssetOptions()
    options.enable_recompute_normals = False
    options.replace_materials = True
    options.new_materials = materials
    options.new_material_slot_names = slots
    options.emit_transaction = False
    unreal.GeometryScript_AssetUtils.copy_mesh_to_static_mesh(combined, sm, options, unreal.GeometryScriptMeshWriteLOD())
    settings = sm.get_editor_property('nanite_settings')
    settings.enabled = False
    sm.set_editor_property('nanite_settings', settings)
    AL.save_loaded_asset(sm)
    bounds = unreal.GeometryScript_MeshQueries.get_mesh_bounding_box(combined)
    return {'asset': path, 'triangles': tris, 'sections': len(materials), 'nanite': False,
            'bounds_min': [bounds.min.x, bounds.min.y, bounds.min.z],
            'bounds_max': [bounds.max.x, bounds.max.y, bounds.max.z],
            'materials': [m.get_path_name() for m in materials]}


def collect_effects():
    """Eight opaque graphic chips, no textures, translucent overdraw, lights or screen flash."""
    import ft_fx as fx
    path = '/Game/FX/Pickups'
    m = ft_mats.new_material(path, 'M_PickupChip', blend=unreal.BlendMode.BLEND_MASKED,
                             shading=unreal.MaterialShadingModel.MSM_UNLIT, two_sided=True)
    m.set_editor_property('used_with_niagara_sprites', True)
    color = ft_mats.expr(m, unreal.MaterialExpressionParticleColor)
    MEL.connect_material_property(color, 'RGB', unreal.MaterialProperty.MP_EMISSIVE_COLOR)
    MEL.connect_material_property(color, 'Alpha', unreal.MaterialProperty.MP_OPACITY_MASK)
    ft_mats.finish(m)
    for name, rgb in (('NS_PickupMissile', (3.0, 0.76, 0.045)), ('NS_PickupRepair', (0.15, 2.4, 0.7))):
        # Reuse the asset on re-export: fx.make deletes its target, so configure this tiny stack in place.
        full = path + '/' + name
        s = AL.load_asset(full) if AL.does_asset_exist(full) else ft_mats.AT.duplicate_asset(name, path, unreal.load_asset(fx.BURST))
        if s is None:
            raise RuntimeError('Could not create Niagara pickup system: ' + full)
        s.set_editor_property('determinism', True)
        s.set_editor_property('random_seed', 88)
        modules = {
            'InitializeParticle': {'LifetimeDistribution': fx.rng(0.18, 0.36),
                                   'SpriteSizeDistribution': fx.v2rng((14, 14), (22, 22)),
                                   'SpriteRotationDistribution': fx.rng(0, 180)},
            'AddVelocity': {'VelocityType': 'InCone', 'ConeAngle': '180.0', 'ConeVelocityDistribution': fx.rng(200, 360)},
            'Drag': {'DragDistribution': fx.const(1.5)},
            'SolveVelocitiesAndForces': {},
            'ScaleColor': {'ScaleDistribution': fx.colors((*rgb, 1), (*rgb, 1), (*rgb, 0))},
            'ScaleSpriteSize': {'ScaleDistribution': fx.sizes(1, 0.85, 0)},
        }
        for key, index in fx._mods(s).items():
            fx._set(s, 'Modules[%d].bModuleEnabled' % index, 'True' if key in modules else 'False')
            for prop, value in modules.get(key, {}).items():
                fx._set(s, 'Modules[%d].%s' % (index, prop), value)
        fx._set(s, 'SpawnInfos', '((Amount=(Min=8,Max=8)))')
        fx._set(s, 'FixedBounds', '(Min=(X=-200,Y=-200,Z=-200),Max=(X=200,Y=200,Z=200),IsValid=True)')
        fx._set(s, 'EmitterState', '(LoopBehavior=Once,LoopDuration=(Min=0.4,Max=0.4,Mode=UniformConstant))')
        fx._set(s, 'RendererProperties[0].Material', fx.mat(path + '/M_PickupChip.M_PickupChip'))
        s.set_editor_property('pool_prime_size', 2)
        s.set_editor_property('max_pool_size', 8)
        fx.T.compile_niagara(s)
        AL.save_loaded_asset(s)


def build():
    parent = glow_master()
    report = []
    for name, function in (('SM_Crate', crate), ('SM_Wrench', wrench)):
        m = ft_art.Model(name)
        function(m)
        report.append(_save(name, m, parent))
    collect_effects()
    out = os.path.join(unreal.Paths.project_saved_dir(), 'Powerups', 'mesh-report.json')
    os.makedirs(os.path.dirname(out), exist_ok=True)
    with open(out, 'w', encoding='utf-8') as f:
        json.dump(report, f, indent=2)
    unreal.log('POWERUPS ' + json.dumps(report))
    return report
