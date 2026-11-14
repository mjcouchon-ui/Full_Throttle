"""Editor views for checking the look: a SceneCapture2D renders a camera to a PNG in Saved/Shots
(works even when the editor window is in the background). import ft_shot; ft_shot.shot('name', loc, rot)"""
import os
import unreal

EAS = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
OUT = os.path.join(unreal.Paths.project_saved_dir(), 'Shots')


def _cap():
    for a in EAS.get_all_level_actors():
        if isinstance(a, unreal.SceneCapture2D) and a.get_actor_label() == 'FT_ShotCam':
            return a
    a = EAS.spawn_actor_from_class(unreal.SceneCapture2D, unreal.Vector(0, 0, 0))
    a.set_actor_label('FT_ShotCam')
    return a


def shot(name, loc, rot, fov=80.0, w=1280, h=720, keep=False):
    world = unreal.get_editor_subsystem(unreal.UnrealEditorSubsystem).get_editor_world()
    cap = _cap()
    cap.set_actor_location_and_rotation(unreal.Vector(*loc), unreal.Rotator(roll=rot[2], pitch=rot[0], yaw=rot[1]), False, False)
    cc = cap.get_editor_property('capture_component2d')
    rt = unreal.RenderingLibrary.create_render_target2d(world, w, h, unreal.TextureRenderTargetFormat.RTF_RGBA8_SRGB, unreal.LinearColor(0, 0, 0, 1))
    cc.set_editor_property('texture_target', rt)
    cc.set_editor_property('fov_angle', fov)
    cc.set_editor_property('capture_source', unreal.SceneCaptureSource.SCS_FINAL_TONE_CURVE_HDR)
    cc.set_editor_property('capture_every_frame', False)
    cc.set_editor_property('capture_on_movement', False)
    cc.capture_scene()
    cc.capture_scene()
    os.makedirs(OUT, exist_ok=True)
    unreal.RenderingLibrary.export_render_target(world, rt, OUT, name + '.png')
    if not keep:
        EAS.destroy_actor(cap)
    return os.path.join(OUT, name + '.png')


def game_shot(name, w=1280, h=720):
    """Capture what the player camera sees in the running PIE world."""
    world = unreal.get_editor_subsystem(unreal.UnrealEditorSubsystem).get_game_world()
    pcm = unreal.GameplayStatics.get_player_camera_manager(world, 0)
    loc, rot, fov = pcm.get_camera_location(), pcm.get_camera_rotation(), pcm.get_fov_angle()
    xf = unreal.Transform(loc, rot, unreal.Vector(1, 1, 1))
    cap = unreal.GameplayStatics.begin_deferred_actor_spawn_from_class(world, unreal.SceneCapture2D, xf)
    cap = unreal.GameplayStatics.finish_spawning_actor(cap, xf)
    cc = cap.get_editor_property('capture_component2d')
    rt = unreal.RenderingLibrary.create_render_target2d(world, w, h, unreal.TextureRenderTargetFormat.RTF_RGBA8_SRGB, unreal.LinearColor(0, 0, 0, 1))
    cc.set_editor_property('texture_target', rt)
    cc.set_editor_property('fov_angle', fov)
    cc.set_editor_property('capture_source', unreal.SceneCaptureSource.SCS_FINAL_TONE_CURVE_HDR)
    cc.capture_scene()
    os.makedirs(OUT, exist_ok=True)
    unreal.RenderingLibrary.export_render_target(world, rt, OUT, name + '.png')
    cap.k2_destroy_actor()
    return '%s loc=%s rot=%s' % (name, loc, rot)
