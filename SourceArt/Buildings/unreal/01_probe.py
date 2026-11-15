"""Probe of the port before the buildings pass: Nanite state and size of Bayshore's props, M_Paint's usage flags, the
sky's MPC, Level 1's actors and its cel instance, and the Python API names this pass needs (UE 5.9)."""
import unreal

AL = unreal.EditorAssetLibrary
MEL = unreal.MaterialEditingLibrary
LES = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
EAS = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
SMS = unreal.get_editor_subsystem(unreal.StaticMeshEditorSubsystem)


def log(*a):
    print("BLD", *[str(x) for x in a], flush=True)


try:
    for p in sorted(AL.list_assets('/Game/Maps/Bayshore/Props', recursive=False)):
        sm = AL.load_asset(p.split('.')[0])
        if not isinstance(sm, unreal.StaticMesh):
            continue
        ns = sm.get_editor_property('nanite_settings')
        b = sm.get_bounds()
        log('mesh', sm.get_name(), 'nanite', ns.get_editor_property('enabled'), 'tris', SMS.get_number_triangles(sm, 0) if hasattr(SMS, 'get_number_triangles') else '?',
            'sections', len(sm.get_editor_property('static_materials')), 'extent', [round(v) for v in (b.box_extent.x, b.box_extent.y, b.box_extent.z)])
    hp = AL.load_asset('/Game/Maps/Bayshore/Props/SM_HotelPink')
    for s in hp.get_editor_property('static_materials'):
        log('pink slot', s.get_editor_property('material_slot_name'), s.get_editor_property('material_interface').get_name())
    log('nanite settings props', [x for x in dir(unreal.MeshNaniteSettings) if not x.startswith('_')][:60])
except Exception as e:
    log('ERR meshes', repr(e))

try:
    m = AL.load_asset('/Game/Core/Materials/M_Paint')
    for f in ('used_with_nanite', 'used_with_instanced_static_meshes', 'used_with_geometry_collections', 'used_with_static_lighting'):
        try:
            log('M_Paint', f, m.get_editor_property(f))
        except Exception as e:
            log('M_Paint', f, 'ERR', e)
    log('usage enum', [x for x in dir(unreal.MaterialUsage) if x.startswith('MATUSAGE')])
except Exception as e:
    log('ERR paint', repr(e))

try:
    mpc = AL.load_asset('/BayshoreSky/Materials/MPC_BayshoreSky')
    log('mpc scalars', [(str(p.get_editor_property('parameter_name')), round(p.get_editor_property('default_value'), 4)) for p in mpc.get_editor_property('scalar_parameters')])
    for p in mpc.get_editor_property('vector_parameters'):
        v = p.get_editor_property('default_value')
        log('mpc vec', p.get_editor_property('parameter_name'), [round(x, 4) for x in (v.r, v.g, v.b, v.a)])
except Exception as e:
    log('ERR mpc', repr(e))

try:
    LES.load_level('/Game/Maps/Bayshore/L_Bayshore')
    for a in EAS.get_all_level_actors():
        log('actor', a.get_actor_label(), a.get_class().get_name(), a.is_hidden_ed(), [round(v) for v in (a.get_actor_location().x, a.get_actor_location().y, a.get_actor_location().z)],
            [round(v, 1) for v in (a.get_actor_rotation().pitch, a.get_actor_rotation().yaw, a.get_actor_rotation().roll)])
    world = unreal.get_editor_subsystem(unreal.UnrealEditorSubsystem).get_editor_world()
    ws = world.get_world_settings()
    log('gamemode override', ws.get_editor_property('default_game_mode'))
    mi = AL.load_asset('/Game/Maps/Bayshore/Materials/MI_PP_Cel_Bayshore')
    for nm in ('Key', 'Bright', 'Th1', 'Th2', 'Hot', 'HotLevel', 'EmisCut', 'MidLevel', 'ShadowLevel', 'HueKeep', 'FadeNear', 'FadeFar', 'InkFadeNear', 'InkFadeFar', 'InkAmount', 'SkyDepth'):
        log('cel', nm, round(MEL.get_material_instance_scalar_parameter_value(mi, nm), 4))
    for nm in ('LitTint', 'ShadowTint', 'InkCol'):
        v = MEL.get_material_instance_vector_parameter_value(mi, nm)
        log('cel', nm, [round(x, 4) for x in (v.r, v.g, v.b)])
    ppv = [a for a in EAS.get_all_level_actors() if a.get_actor_label() == 'Grade80s'][0]
    s = ppv.get_editor_property('settings')
    for k in ('auto_exposure_bias', 'bloom_intensity', 'bloom_threshold', 'vignette_intensity', 'film_grain_intensity', 'lens_flare_intensity'):
        log('ppv', k, s.get_editor_property(k))
    track = [a for a in EAS.get_all_level_actors() if a.get_actor_label().startswith('Track_')][0]
    for c in track.get_components_by_class(unreal.HierarchicalInstancedStaticMeshComponent):
        sm = c.get_editor_property('static_mesh')
        log('hism', sm.get_name() if sm else None, c.get_instance_count(), 'cull', c.get_editor_property('instance_start_cull_distance'), c.get_editor_property('instance_end_cull_distance'),
            'shadow', c.get_editor_property('cast_shadow'))
    # where the near hotel row stands relative to the road, for the look-dev cameras
    for c in track.get_components_by_class(unreal.HierarchicalInstancedStaticMeshComponent):
        sm = c.get_editor_property('static_mesh')
        if sm and sm.get_name() == 'SM_HotelPink':
            for i in range(0, min(c.get_instance_count(), 6)):
                t = c.get_instance_transform(i, True)
                log('pink inst', i, [round(v) for v in (t.translation.x, t.translation.y, t.translation.z)], round(t.rotation.rotator().yaw, 1), round(t.scale3d.x, 2))
except Exception as e:
    import traceback
    log('ERR level', repr(e), traceback.format_exc())

log('probe done')
