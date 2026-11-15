"""Screenshots of the sky in the port's Level 1 (L_Bayshore) through a SceneCapture2D, from points on the road.
Road points come from the track's procedural mesh chunks (centre of chunk k, looking at chunk k+3).
Note: scene captures skip post-process materials, so the port's cel pass (M_PP_Cel) is not in these images;
it leaves the unlit sky untouched anyway. Output: HandBack/port_screenshots/*.png
Run in the port's editor: ue_run.ps1 -Editor -Project <FullThrottle.uproject> -Script 81_capture_port.py
"""
import os
import math
import unreal

OUT = r"K:\Full Throttle\SkyScratch\HandBack\port_screenshots"
os.makedirs(OUT, exist_ok=True)
W, H, FRAMES = 1920, 1080, 40
LES = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
EAS = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
LES.load_level("/Game/Maps/Bayshore/L_Bayshore")
world = unreal.EditorLevelLibrary.get_editor_world()


def log(*a):
    print("BSKY", *[str(x) for x in a], flush=True)


track = [a for a in EAS.get_all_level_actors() if a.get_actor_label().startswith("Track_")][0]
chunks = [c for c in track.get_components_by_class(unreal.ProceduralMeshComponent)]
pts = []
for c in chunks:
    origin, extent, radius = unreal.SystemLibrary.get_component_bounds(c)
    pts.append(origin)
log("track chunks", len(pts))


def look(a, b, up=6.0, roll=0.0, pitch_add=0.0):
    d = unreal.Vector(b.x - a.x, b.y - a.y, b.z - a.z)
    yaw = math.degrees(math.atan2(d.y, d.x))
    pitch = math.degrees(math.atan2(d.z, math.hypot(d.x, d.y))) + pitch_add
    return unreal.Vector(a.x, a.y, a.z + up * 100.0), unreal.Rotator(roll=roll, pitch=pitch, yaw=yaw)


def road(frac, pitch_add=4.0, roll=0.0, up=6.0):
    n = len(pts)
    k = min(int(frac * n), n - 4)
    return look(pts[k], pts[k + 3], up, roll, pitch_add)


SUN_AZ, SUN_EL = 30.0, 35.0
SHOTS = [
    ("P1_road_start", lambda: road(0.03), 90.0),
    ("P2_road_middle", lambda: road(0.45), 90.0),
    ("P3_road_late", lambda: road(0.8), 90.0),
    ("P4_toward_sun", lambda: (road(0.2)[0], unreal.Rotator(roll=0.0, pitch=20.0, yaw=SUN_AZ)), 90.0),
    ("P5_away_from_sun", lambda: (road(0.2)[0], unreal.Rotator(roll=0.0, pitch=6.0, yaw=SUN_AZ + 180.0)), 90.0),
    ("P6_upside_down", lambda: (road(0.3)[0], unreal.Rotator(roll=180.0, pitch=-10.0, yaw=SUN_AZ + 40.0)), 90.0),
    ("P7_straight_up", lambda: (road(0.5)[0], unreal.Rotator(roll=0.0, pitch=89.0, yaw=0.0)), 100.0),
]
rt = unreal.RenderingLibrary.create_render_target2d(world, W, H, unreal.TextureRenderTargetFormat.RTF_RGBA8, unreal.LinearColor(0, 0, 0, 1), False)
cap = EAS.spawn_actor_from_class(unreal.SceneCapture2D, unreal.Vector(0, 0, 0), unreal.Rotator(0, 0, 0))
cc = cap.get_editor_property("capture_component2d")
cc.set_editor_property("texture_target", rt)
cc.set_editor_property("capture_source", unreal.SceneCaptureSource.SCS_FINAL_COLOR_LDR)
cc.set_editor_property("capture_every_frame", False)
cc.set_editor_property("capture_on_movement", False)
cc.set_editor_property("always_persist_rendering_state", True)
state = {"shot": 0, "frame": 0, "h": None}


def tick(dt):
    try:
        if state["shot"] >= len(SHOTS):
            unreal.unregister_slate_post_tick_callback(state["h"])
            EAS.destroy_actor(cap)
            log("captures done")
            unreal.SystemLibrary.quit_editor()
            return
        name, where, fov = SHOTS[state["shot"]]
        if state["frame"] == 0:
            loc, rot = where()
            cap.set_actor_location_and_rotation(loc, rot, False, True)
            cc.set_editor_property("fov_angle", fov)
            log("shot", name, loc, rot)
        cc.capture_scene()
        state["frame"] += 1
        if state["frame"] >= FRAMES:
            unreal.RenderingLibrary.export_render_target(world, rt, OUT, name + ".png")
            state["shot"] += 1
            state["frame"] = 0
    except Exception as e:  # noqa
        log("CAPTURE ERROR", e)
        unreal.unregister_slate_post_tick_callback(state["h"])
        unreal.SystemLibrary.quit_editor()


unreal.EditorPythonScripting.set_keep_python_script_alive(True)
state["h"] = unreal.register_slate_post_tick_callback(tick)
