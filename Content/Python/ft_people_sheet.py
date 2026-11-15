"""FULL THROTTLE (Unreal) the population: contact sheets. Stands population meshes in an empty level under two plain
lights and renders them to PNG with a scene capture, so a piece can be looked at up close before it is placed.
(No cel pass here: this shows shape, colour and pose. How a piece reads in the game is judged from the game's frames.)

    import ft_people_sheet
    ft_people_sheet.sheet(['/Game/People/Crew/SM_CrewCamera', ...], 'crew', out_dir='C:/somewhere')
"""
import math
import os
import unreal

AL = unreal.EditorAssetLibrary


def _spawn(cls, loc=(0, 0, 0), rot=(0, 0, 0)):
    sub = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
    return sub.spawn_actor_from_class(cls, unreal.Vector(*loc), unreal.Rotator(roll=rot[2], pitch=rot[0], yaw=rot[1]))


def _stage(stage='blank'):
    """stage 'blank': a new empty level, two plain lights and a floor (shape and colour, no cel pass).
    stage 'open': whatever map the editor opened (the game's own: its light and its cel pass), the meshes at its origin."""
    import time
    MEL = unreal.MaterialEditingLibrary
    for nm in ('M_People', 'M_PeopleFar'):   # a capture taken before the shaders are ready shows the grey default material
        m = AL.load_asset('/Game/People/Core/' + nm)
        if m is not None:
            MEL.recompile_material(m)
    if stage != 'blank':
        return
    try:
        unreal.get_editor_subsystem(unreal.LevelEditorSubsystem).new_level('/Temp/PeopleSheet_%d' % int(time.time()))
    except Exception as e:
        unreal.log_warning('FTPEOPLE sheet: no new level (%s): using the open one' % e)
        return
    for rot, lum in (((-48, 35, 0), 6.0), ((-25, -140, 0), 2.2), ((-70, 170, 0), 1.2)):
        a = _spawn(unreal.DirectionalLight, (0, 0, 2000), rot)
        c = a.get_component_by_class(unreal.DirectionalLightComponent)
        c.set_editor_property('intensity', lum)
        c.set_editor_property('cast_shadows', lum > 5.0)
    floor = _spawn(unreal.StaticMeshActor, (0, 0, -1))
    fc = floor.get_component_by_class(unreal.StaticMeshComponent)
    fc.set_static_mesh(AL.load_asset('/Engine/BasicShapes/Plane'))
    floor.set_actor_scale3d(unreal.Vector(400, 400, 1))


def _shoot(path_dir, name, loc, rot, fov, w, h):
    world = unreal.get_editor_subsystem(unreal.UnrealEditorSubsystem).get_editor_world()
    cap = _spawn(unreal.SceneCapture2D, loc, rot)
    cc = cap.get_editor_property('capture_component2d')
    rt = unreal.RenderingLibrary.create_render_target2d(world, w, h, unreal.TextureRenderTargetFormat.RTF_RGBA8_SRGB, unreal.LinearColor(0.5, 0.6, 0.7, 1))
    cc.set_editor_property('texture_target', rt)
    cc.set_editor_property('fov_angle', fov)
    cc.set_editor_property('capture_source', unreal.SceneCaptureSource.SCS_FINAL_TONE_CURVE_HDR)
    cc.set_editor_property('capture_every_frame', False)
    cc.set_editor_property('capture_on_movement', False)
    for _ in range(3):
        cc.capture_scene()
    os.makedirs(path_dir, exist_ok=True)
    unreal.RenderingLibrary.export_render_target(world, rt, path_dir, name + '.png')
    unreal.get_editor_subsystem(unreal.EditorActorSubsystem).destroy_actor(cap)


def sheet(meshes, name, out_dir, window=700.0, w=1600, h=900, max_windows=4, stage='blank'):
    """One mesh at a time, front towards the camera: a three-quarter view of the whole piece, then close views along it
    (a window `window` cm wide each, at most max_windows). PNGs <name>_<mesh>_all.png and <name>_<mesh>_c<k>.png."""
    _stage(stage)
    sub = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
    out = []
    for p in meshes:
        sm = AL.load_asset(p)
        if sm is None:
            unreal.log_warning('FTPEOPLE sheet: no mesh %s' % p)
            continue
        a = _spawn(unreal.StaticMeshActor, (0, 0, 0))
        a.get_component_by_class(unreal.StaticMeshComponent).set_static_mesh(sm)
        b = sm.get_bounds()
        ex, ey = b.box_extent.x, b.box_extent.y
        top = b.origin.z + b.box_extent.z
        cx, cy = b.origin.x, b.origin.y
        short = p.rsplit('/', 1)[-1]
        span = max(2 * ex, top * 1.8, 300.0)
        dist = span * 0.6 / math.tan(math.radians(25.0))
        # the front faces -Y: the camera stands at -Y, a little to one side and above
        _shoot(out_dir, '%s_%s_all' % (name, short), (cx - dist * 0.35, cy - dist * 0.9, top * 0.5 + dist * 0.28), (-15, 68.7, 0), 50.0, w, h)
        out.append(os.path.join(out_dir, '%s_%s_all.png' % (name, short)))
        n = int(min(max_windows, max(1, round(2 * ex / window))))
        for k in range(n):
            wx = cx - ex + (k + 0.5) * (2 * ex / n)
            ww = max(2 * ex / n, 320.0)
            d2 = max(ww, min(top, 400.0) * 1.7) * 0.6 / math.tan(math.radians(20.0))
            _shoot(out_dir, '%s_%s_c%d' % (name, short, k), (wx - d2 * 0.25, cy - ey - d2 * 0.95, min(top, 400.0) * 0.55 + d2 * 0.12), (-6, 75.3, 0), 40.0, w, h)
            out.append(os.path.join(out_dir, '%s_%s_c%d.png' % (name, short, k)))
        sub.destroy_actor(a)
    unreal.log('FTPEOPLE sheet ' + name + ' images ' + str(len(out)) + ' -> ' + out_dir)
    return out
