"""FULL THROTTLE (Unreal) Level 1 sky: the BayshoreSky plugin (Plugins/BayshoreSky) in L_Bayshore.

The plugin's hyper-real golden-hour sky replaces the old NightSky sphere, Sun and SkyLight of Bayshore: it brings its own
dome (HDR gradient and golden-hour glow, baked atmosphere strip, HDR sun disc, photoreal parallax cloud layers, backdrop
sea), photoreal cumulus cards, sun halo and lens ghosts, distant hills, the BAYSHORE blimp, gulls, a golden-hour
directional light (6 deg, light shafts, volumetric scattering) and a sky light from the baked HDR cubemap.
It goes into the map as a Level Instance of /BayshoreSky/Maps/LI_BayshoreSky (one actor, label 'BayshoreSky').
The old NightSky, Sun (or Moonlight) and SkyLight actors stay in the map (ft_cel finds them by label) but are switched off.
The level's HeightFog is matched to the sky: its colour is the sky's lavender horizon, with a warm directional glow
toward the low sun.

Volumetric fog is off by default. Measured on 14 Nov (packaged Development, Epic, 3440x1440, RTX 4080 SUPER, idle GPU)
it cost 1.1 ms GPU: the sky without it adds about 0.1 ms over the original Level 1 (8.6 vs 8.5 ms), and with it 1.2 ms.
In driving views it showed as a slightly hazier veil, not shafts, and 2.6 had already turned Bayshore's volumetric fog
off because it greyed the bands and the ink (ft_v25.py). Set fog['volumetric'] = True below for it: with
r.VolumetricFog.GridPixelSize 16 (the High preset's grid) it costs about 0.35 ms.

Order: ft_level.build(0) clears the map and ft_cel.apply_level(0) restyles the old actors, so run this after both:
    import ft_sky; ft_sky.apply(0)
Undo: ft_sky.remove(0) (switches the old sky back on, resets the fog fields only the sky sets, and reruns
ft_cel.apply_level(0) for the rest of its fog).
Tuning: Plugins/BayshoreSky/INTEGRATION.md (MPC_BayshoreSky, DA_BayshoreSkyPreset_Day).
"""
import unreal

AL = unreal.EditorAssetLibrary
LES = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
EAS = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

SKY_LEVEL = '/BayshoreSky/Maps/LI_BayshoreSky'
LABEL = 'BayshoreSky'
# the sky is centred on the play area: the road stays within X -1.1..3.9 km, Y -0.2..12.6 km of the origin
LEVELS = {
    # 'Moonlight' is the label ft_level.build(0) gives the directional light before ft_cel.apply_level(0) renames it
    # 'Sun': listed too, so applying the sky straight after a build cannot leave the moon lit beside the golden sun
    0: dict(map='/Game/Maps/Bayshore/L_Bayshore', centre=(140000.0, 620000.0, 0.0),
            replaces=('NightSky', 'Sun', 'Moonlight', 'SkyLight'),
            # fog matched to the golden-hour sky (scene-linear luminance, manual exposure): the lavender horizon away
            # from the sun, the orange glow toward it. Volumetric fog off (see the docstring); its settings stay here
            # for when it is switched on
            fog=dict(inscattering=(0.50, 0.40, 0.58), directional=(1.70, 0.48, 0.20), directional_exponent=6.0,
                     directional_start=150000.0, density=0.016, start=4000.0, opacity=0.65, falloff=0.25,
                     volumetric=False, vol_distance=80000.0, vol_extinction=0.55, vol_scattering=0.72,
                     vol_albedo=(255, 236, 222))),
}


def _actor(label):
    for a in EAS.get_all_level_actors():
        if a.get_actor_label() == label:
            return a
    return None


def _switch(a, on):
    """Show or hide an actor and everything on it, in the editor and in game (lights stop lighting when hidden)."""
    a.set_actor_hidden_in_game(not on)
    a.set_is_temporarily_hidden_in_editor(not on)
    for c in a.get_components_by_class(unreal.SceneComponent):
        c.set_visibility(on, False)


def _fog(L, log):
    fa = _actor('HeightFog')
    if fa is None or 'fog' not in L:
        return
    fc = fa.get_component_by_class(unreal.ExponentialHeightFogComponent)
    f = L['fog']
    fc.set_editor_property('fog_inscattering_luminance', unreal.LinearColor(*f['inscattering'], 1.0))
    fc.set_editor_property('directional_inscattering_luminance', unreal.LinearColor(*f['directional'], 1.0))
    fc.set_editor_property('directional_inscattering_exponent', f['directional_exponent'])
    fc.set_editor_property('directional_inscattering_start_distance', f['directional_start'])
    fc.set_fog_density(f['density'])
    fc.set_fog_height_falloff(f['falloff'])
    fc.set_editor_property('start_distance', f['start'])
    fc.set_editor_property('fog_max_opacity', f['opacity'])
    fc.set_editor_property('fog_cutoff_distance', 5000000.0)    # the sky dome (60 km) keeps its own colours
    fc.set_volumetric_fog(f['volumetric'])
    fc.set_editor_property('volumetric_fog_distance', f['vol_distance'])
    fc.set_editor_property('volumetric_fog_extinction_scale', f['vol_extinction'])
    fc.set_editor_property('volumetric_fog_scattering_distribution', f['vol_scattering'])
    fc.set_editor_property('volumetric_fog_albedo', unreal.Color(r=f['vol_albedo'][0], g=f['vol_albedo'][1], b=f['vol_albedo'][2], a=255))
    log.append('fog matched to the sky, volumetric %s' % f['volumetric'])


def apply(i=0, save=True):
    L = LEVELS[i]
    LES.load_level(L['map'])
    li = _actor(LABEL)
    if li is None:
        li = EAS.spawn_actor_from_class(unreal.LevelInstance, unreal.Vector(*L['centre']), unreal.Rotator(0, 0, 0))
        li.set_actor_label(LABEL)
    li.set_world_asset(AL.load_asset(SKY_LEVEL))
    li.set_actor_location(unreal.Vector(*L['centre']), False, False)
    li.set_actor_rotation(unreal.Rotator(0, 0, 0), False)   # the blimp, gull and cloud paths assume an unrotated sky
    log = ['%s -> %s at %s' % (LABEL, SKY_LEVEL, L['centre'])]
    for label in L['replaces']:
        a = _actor(label)
        if a:
            _switch(a, False)
            log.append('off: ' + label)
    _fog(L, log)
    if save:
        LES.save_current_level()
    unreal.log('ft_sky: ' + '; '.join(log))
    return log


def _fog_defaults():
    """Put back the fog properties apply() sets and ft_cel.apply_level() does not: the directional inscattering and the
    volumetric fog's shape go back to the values ft_level.build() and the engine give them."""
    fa = _actor('HeightFog')
    if fa is None:
        return
    fc = fa.get_component_by_class(unreal.ExponentialHeightFogComponent)
    fc.set_editor_property('directional_inscattering_luminance', unreal.LinearColor(0.0, 0.0, 0.0, 1.0))
    fc.set_editor_property('directional_inscattering_exponent', 4.0)
    fc.set_editor_property('directional_inscattering_start_distance', 10000.0)
    fc.set_fog_height_falloff(0.25)
    fc.set_editor_property('volumetric_fog_distance', 6000.0)
    fc.set_editor_property('volumetric_fog_extinction_scale', 0.6)
    fc.set_editor_property('volumetric_fog_scattering_distribution', 0.6)
    fc.set_editor_property('volumetric_fog_albedo', unreal.Color(r=255, g=255, b=255, a=255))


def remove(i=0, save=True):
    """Back to the level's own sky: delete the Level Instance, switch the old actors on again, reset the fog
    properties only the sky sets, and let ft_cel restyle the level (the rest of its fog included)."""
    L = LEVELS[i]
    LES.load_level(L['map'])
    li = _actor(LABEL)
    if li:
        EAS.destroy_actor(li)
    for label in L['replaces']:
        a = _actor(label)
        if a:
            _switch(a, True)
    _fog_defaults()
    import ft_cel
    ft_cel.apply_level(i, save=False)    # a failure here is raised: the level would keep the sky's fog
    if save:
        LES.save_current_level()
