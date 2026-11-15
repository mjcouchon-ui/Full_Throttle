"""Screenshots of L_BayshoreSkyTest from the offscreen editor through a SceneCapture2D (final tone-mapped colour),
rendering several frames per shot so TSR and the dithered edges settle. Output: HandBack/screenshots/*.png

Run: ue_run.ps1 -Editor -Script 60_capture.py      (env BSKY_SHOTS=name1,name2 to limit, BSKY_TAG=suffix)
"""
import sys
import os
import math
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)) if "__file__" in globals() else r"K:\Full Throttle\SkyScratch\SourceArt\BayshoreSky\unreal")
import unreal
from bsky_ue import log, enum_entry

OUT = r"K:\Full Throttle\SkyScratch\HandBack\screenshots"
os.makedirs(OUT, exist_ok=True)
MAP = "/BayshoreSky/Maps/L_BayshoreSkyTest"
W, H = 1920, 1080
FRAMES = 40
TAG = os.environ.get("BSKY_TAG", "")
ONLY = [s for s in os.environ.get("BSKY_SHOTS", "").split(",") if s]

les = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
les.load_level(MAP)
world = unreal.EditorLevelLibrary.get_editor_world()
actors = eas.get_all_level_actors()
rail = [a for a in actors if isinstance(a, unreal.CameraRig_Rail)][0]
sky = [a for a in actors if a.get_actor_label() == "BayshoreSky"][0]
spline = rail.get_rail_spline_component()
for _c in sky.get_components_by_class(unreal.PrimitiveComponent):     # debug: BSKY_HIDE=Panorama,Sea,...
    if _c.get_name() in [x for x in os.environ.get("BSKY_HIDE", "").split(",") if x]:
        _c.set_visibility(False, False)
        log("hidden for debugging:", _c.get_name())
L = spline.get_spline_length()


def rail_tf(frac):
    t = spline.get_transform_at_distance_along_spline(frac * L, unreal.SplineCoordinateSpace.WORLD, False)
    return t.translation, t.rotation.rotator()


def find_frac(pred, a=0.0, b=1.0, steps=2000):
    best, bv = None, None
    for i in range(steps + 1):
        f = a + (b - a) * i / steps
        up = spline.get_up_vector_at_distance_along_spline(f * L, unreal.SplineCoordinateSpace.WORLD)
        loc = spline.get_location_at_distance_along_spline(f * L, unreal.SplineCoordinateSpace.WORLD)
        v = pred(up, loc)
        if bv is None or v < bv:
            best, bv = f, v
    return best


f_top = find_frac(lambda up, loc: up.z + (0 if -4000 < loc.x < 4000 else 10))                                         # upside down at the top of the loop
f_cork = find_frac(lambda up, loc: abs(up.z) + (0 if 12000 < loc.x < 30000 and loc.y < 4000 else 10))   # rolled 90 in the corkscrew
f_bank = find_frac(lambda up, loc: -(abs(up.x) + abs(up.y)) + (0 if loc.y > 5000 else 10))             # banked turn
log("rail fractions top %.4f cork %.4f bank %.4f" % (f_top, f_cork, f_bank))

# blimp: the material moves it on a circle; its angle is Time * BlimpSpeed (world time)
blimp_comp = [c for c in sky.get_components_by_class(unreal.StaticMeshComponent) if c.get_name() == "Blimp"][0]
mpc = unreal.load_asset("/BayshoreSky/Materials/MPC_BayshoreSky")
MATLIB = getattr(unreal, "MaterialLibrary", None) or getattr(unreal, "KismetMaterialLibrary")


def blimp_shot(outer=True):
    # freeze the blimp at angle 0 for the shot (world time differs between the editor and a game), so its place is known
    MATLIB.set_scalar_parameter_value(world, mpc, "BlimpSpeed", 0.0)
    radius = MATLIB.get_scalar_parameter_value(world, mpc, "BlimpRadius")
    th = 0.0
    o = blimp_comp.get_world_location()
    c = unreal.Vector(o.x + math.cos(th) * radius, o.y + math.sin(th) * radius, o.z)
    # stand off to one flank, slightly ahead and below; the outer flank faces the sun side
    sgn = 1.0 if outer else -1.0
    side = unreal.Vector(math.cos(th) * sgn, math.sin(th) * sgn, 0.0)
    cam = unreal.Vector(c.x + side.x * 9000 - math.sin(th) * 2500, c.y + side.y * 9000 + math.cos(th) * 2500, c.z - 1500)
    d = unreal.Vector(c.x - cam.x, c.y - cam.y, c.z - cam.z)
    yaw = math.degrees(math.atan2(d.y, d.x))
    pitch = math.degrees(math.atan2(d.z, math.hypot(d.x, d.y)))
    return cam, unreal.Rotator(roll=0.0, pitch=pitch, yaw=yaw)


def cloud_shot():
    ism = [c for c in sky.get_components_by_class(unreal.InstancedStaticMeshComponent) if c.get_name() == "Clouds"][0]
    best = None
    for i in range(ism.get_instance_count()):
        tf = ism.get_instance_transform(i, True)
        p = tf.translation
        if p.z > 230000:
            continue  # skip the flat overhead cards
        dist = math.hypot(p.x, p.y)
        if best is None or dist < best[0]:
            best = (dist, p, tf.scale3d.x)
    _, p, s = best
    w = 100.0 * s
    dirv = unreal.Vector(p.x, p.y, 0.0)
    dirv.normalize()
    cam = unreal.Vector(p.x - dirv.x * w * 1.3, p.y - dirv.y * w * 1.3, p.z - w * 0.15)
    d = unreal.Vector(p.x - cam.x, p.y - cam.y, p.z - cam.z)
    return cam, unreal.Rotator(roll=0.0, pitch=math.degrees(math.atan2(d.z, math.hypot(d.x, d.y))),
                               yaw=math.degrees(math.atan2(d.y, d.x)))


SHOTS = [
    ("01_toward_sun", lambda: (unreal.Vector(-9000, -1500, 700), unreal.Rotator(roll=0, pitch=22, yaw=30)), 90.0),
    ("02_away_from_sun", lambda: (unreal.Vector(-9000, -1500, 700), unreal.Rotator(roll=0, pitch=6, yaw=210)), 90.0),
    ("03_upside_down_loop_top", lambda: rail_tf(f_top), 90.0),
    ("04_scene_causeway", lambda: rail_tf(0.06), 90.0),
    ("05_scene_corkscrew_roll", lambda: rail_tf(f_cork), 90.0),
    ("06_scene_banked_turn", lambda: rail_tf(f_bank), 90.0),
    ("07_closeup_cloud", cloud_shot, 40.0),
    ("08_closeup_blimp", lambda: blimp_shot(True), 30.0),
    ("08b_closeup_blimp_other_side", lambda: blimp_shot(False), 30.0),
    ("09_straight_up", lambda: (unreal.Vector(0, 0, 700), unreal.Rotator(roll=0, pitch=89, yaw=30)), 100.0),
    ("10_sea_glitter", lambda: (unreal.Vector(-9000, -1500, 700), unreal.Rotator(roll=0, pitch=-6, yaw=30)), 70.0),
]
if ONLY:
    SHOTS = [s for s in SHOTS if s[0] in ONLY]

rt = unreal.RenderingLibrary.create_render_target2d(world, W, H, enum_entry(unreal.TextureRenderTargetFormat, "RTF_RGBA8"),
                                                    unreal.LinearColor(0, 0, 0, 1), False)
cap = eas.spawn_actor_from_class(unreal.SceneCapture2D, unreal.Vector(0, 0, 0), unreal.Rotator(0, 0, 0))
cc = cap.get_editor_property("capture_component2d")
cc.set_editor_property("texture_target", rt)
cc.set_editor_property("capture_source", enum_entry(unreal.SceneCaptureSource, "SCS_FinalColorLDR"))
cc.set_editor_property("capture_every_frame", False)
cc.set_editor_property("capture_on_movement", False)
cc.set_editor_property("always_persist_rendering_state", True)

state = {"shot": 0, "frame": 0, "handle": None}


def tick(dt):
    try:
        if state["shot"] >= len(SHOTS):
            unreal.unregister_slate_post_tick_callback(state["handle"])
            eas.destroy_actor(cap)
            log("captures done", len(SHOTS))
            unreal.SystemLibrary.quit_editor()
            return
        name, where, fov = SHOTS[state["shot"]]
        if state["frame"] == 0:
            loc, rot = where()
            cap.set_actor_location_and_rotation(loc, rot, False, True)
            cc.set_editor_property("fov_angle", fov)
            log("shot", name, "at", loc, rot)
        cc.capture_scene()
        state["frame"] += 1
        if state["frame"] >= FRAMES:
            unreal.RenderingLibrary.export_render_target(world, rt, OUT, name + TAG + ".png")
            log("wrote", name + TAG + ".png")
            state["shot"] += 1
            state["frame"] = 0
    except Exception as e:  # noqa
        log("CAPTURE ERROR", e)
        unreal.unregister_slate_post_tick_callback(state["handle"])
        unreal.SystemLibrary.quit_editor()


for _lib in ("EditorPythonScripting", "PythonScriptLibrary", "EditorPythonScriptingLibrary"):
    if hasattr(unreal, _lib) and hasattr(getattr(unreal, _lib), "set_keep_python_script_alive"):
        getattr(unreal, _lib).set_keep_python_script_alive(True)
        break
else:
    raise RuntimeError("no set_keep_python_script_alive")
state["handle"] = unreal.register_slate_post_tick_callback(tick)
