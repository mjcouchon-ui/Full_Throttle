"""FULL THROTTLE (Unreal) level builder: one map per level under Content/Maps/<Level>/.

Each map holds the procedural track (AFTTrack), a night sky (sphere with the M_Sky material: gradient,
stars, moon), moonlight, a real-time sky light for Lumen, volumetric height fog and an unbound
post-process volume with the 80s grade. Run over the Unreal MCP connector:
    import ft_level; ft_level.build(0)   # 0 Bayshore, 1 Chemical Works, 2 Nevada
"""
import math
import unreal

AL = unreal.EditorAssetLibrary
MEL = unreal.MaterialEditingLibrary
AT = unreal.AssetToolsHelpers.get_asset_tools()
LES = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
EAS = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

LEVELS = [
    dict(name='Bayshore', map='/Game/Maps/Bayshore/L_Bayshore',
         sky=((0.004, 0.003, 0.02), (0.05, 0.015, 0.14), (0.85, 0.12, 0.42)), moon=(-35.0, 150.0), moon_col=(0.62, 0.7, 1.0), moon_lux=5.0,
         fog=(0.05, 0.015, 0.08), fog_density=0.03, fog_height=-400.0, tint=(1.0, 0.94, 1.06), sky_gain=1.0, exposure=0.4),
    dict(name='ChemicalWorks', map='/Game/Maps/ChemicalWorks/L_ChemicalWorks',
         sky=((0.002, 0.002, 0.012), (0.03, 0.02, 0.1), (0.5, 0.15, 0.6)), moon=(-42.0, -30.0), moon_col=(0.55, 0.75, 1.0), moon_lux=4.0,
         fog=(0.03, 0.015, 0.07), fog_density=0.04, fog_height=-300.0, tint=(0.96, 1.0, 1.06), sky_gain=1.0, exposure=0.4),
    dict(name='Nevada', map='/Game/Maps/Nevada/L_Nevada',
         sky=((0.006, 0.004, 0.03), (0.11, 0.03, 0.18), (1.0, 0.32, 0.22)), moon=(-8.0, 45.0), moon_col=(1.0, 0.55, 0.45), moon_lux=6.0,
         fog=(0.08, 0.03, 0.05), fog_density=0.025, fog_height=-500.0, tint=(1.06, 0.98, 0.96), sky_gain=1.1, exposure=0.1),
]


def _sky_mi(i, L):
    path = '/Game/Maps/%s/Materials' % L['name']
    if not AL.does_directory_exist(path):
        AL.make_directory(path)
    name = 'MI_Sky_' + L['name']
    full = path + '/' + name
    mi = AL.load_asset(full) if AL.does_asset_exist(full) else AT.create_asset(name, path, unreal.MaterialInstanceConstant, unreal.MaterialInstanceConstantFactoryNew())
    mi.set_editor_property('parent', AL.load_asset('/Game/Core/Materials/M_Sky'))
    for k, c in zip(('Sky0', 'Sky1', 'Sky2'), L['sky']):
        MEL.set_material_instance_vector_parameter_value(mi, k, unreal.LinearColor(c[0], c[1], c[2], 1))
    pitch, yaw = L['moon']
    # direction toward the moon = opposite of the light's travel
    p, y = math.radians(-pitch), math.radians(yaw)
    d = (math.cos(p) * math.cos(y), math.cos(p) * math.sin(y), math.sin(p))
    MEL.set_material_instance_vector_parameter_value(mi, 'MoonDir', unreal.LinearColor(-d[0] * -1, -d[1] * -1, d[2], 0))
    MEL.set_material_instance_scalar_parameter_value(mi, 'Gain', L['sky_gain'])
    MEL.update_material_instance(mi)
    AL.save_loaded_asset(mi)
    return mi


def build(i):
    L = LEVELS[i]
    if AL.does_asset_exist(L['map']):
        LES.load_level(L['map'])
        for a in EAS.get_all_level_actors():
            EAS.destroy_actor(a)
    else:
        folder = L['map'].rsplit('/', 1)[0]
        if not AL.does_directory_exist(folder):
            AL.make_directory(folder)
        LES.new_level(L['map'])

    track = EAS.spawn_actor_from_class(unreal.FTTrack, unreal.Vector(0, 0, 0))
    track.set_actor_label('Track_' + L['name'])
    track.set_editor_property('level_index', i)
    track.set_editor_property('build_in_editor', True)

    sky = EAS.spawn_actor_from_class(unreal.StaticMeshActor, unreal.Vector(0, 0, 0))
    sky.set_actor_label('NightSky')
    smc = sky.static_mesh_component
    smc.set_static_mesh(AL.load_asset('/Engine/BasicShapes/Sphere'))
    smc.set_material(0, _sky_mi(i, L))
    smc.set_editor_property('cast_shadow', False)
    smc.set_collision_profile_name('NoCollision')
    sky.set_actor_scale3d(unreal.Vector(4000, 4000, 4000))

    moon = EAS.spawn_actor_from_class(unreal.DirectionalLight, unreal.Vector(0, 0, 10000))
    moon.set_actor_label('Moonlight')
    pitch, yaw = L['moon']
    moon.set_actor_rotation(unreal.Rotator(roll=0, pitch=pitch, yaw=yaw), False)
    lc = moon.get_component_by_class(unreal.DirectionalLightComponent)
    lc.set_intensity(L['moon_lux'])
    lc.set_light_color(unreal.LinearColor(*L['moon_col'], 1))
    lc.set_editor_property('atmosphere_sun_light', False)
    lc.set_editor_property('dynamic_shadow_distance_movable_light', 20000.0)

    sl = EAS.spawn_actor_from_class(unreal.SkyLight, unreal.Vector(0, 0, 5000))
    sl.set_actor_label('SkyLight')
    slc = sl.get_component_by_class(unreal.SkyLightComponent)
    slc.set_editor_property('real_time_capture', True)
    slc.set_editor_property('mobility', unreal.ComponentMobility.MOVABLE)
    slc.set_intensity(2.0)

    fog = EAS.spawn_actor_from_class(unreal.ExponentialHeightFog, unreal.Vector(0, 0, L['fog_height']))
    fog.set_actor_label('HeightFog')
    fc = fog.get_component_by_class(unreal.ExponentialHeightFogComponent)
    fc.set_fog_density(L['fog_density'])
    fc.set_fog_height_falloff(0.25)
    fc.set_editor_property('fog_max_opacity', 0.9)
    fc.set_fog_inscattering_color(unreal.LinearColor(*L['fog'], 1))
    fc.set_editor_property('start_distance', 3000.0)
    fc.set_volumetric_fog(True)
    fc.set_editor_property('volumetric_fog_scattering_distribution', 0.6)
    fc.set_editor_property('volumetric_fog_extinction_scale', 0.6)

    ppv = EAS.spawn_actor_from_class(unreal.PostProcessVolume, unreal.Vector(0, 0, 0))
    ppv.set_actor_label('Grade80s')
    ppv.set_editor_property('unbound', True)
    s = ppv.get_editor_property('settings')
    def ov(k, v):
        s.set_editor_property('override_' + k, True)
        s.set_editor_property(k, v)
    ov('auto_exposure_method', unreal.AutoExposureMethod.AEM_MANUAL)
    ov('auto_exposure_bias', L['exposure'])
    ov('auto_exposure_apply_physical_camera_exposure', False)
    ov('bloom_intensity', 1.15)
    ov('bloom_threshold', 1.0)
    ov('vignette_intensity', 0.45)
    ov('film_grain_intensity', 0.22)
    ov('scene_fringe_intensity', 0.6)
    ov('lens_flare_intensity', 0.6)
    ov('motion_blur_amount', 0.2)
    ov('motion_blur_max', 4.0)
    ov('color_saturation', unreal.Vector4(1.12, 1.08, 1.15, 1.0))
    ov('color_contrast', unreal.Vector4(1.08, 1.08, 1.1, 1.0))
    ov('scene_color_tint', unreal.LinearColor(*L['tint'], 1))
    ov('lumen_scene_lighting_quality', 1.0)
    ov('lumen_final_gather_quality', 1.0)
    ov('lumen_reflection_quality', 1.0)
    ppv.set_editor_property('settings', s)

    LES.save_current_level()
    return 'built %s: %d actors' % (L['map'], len(EAS.get_all_level_actors()))
