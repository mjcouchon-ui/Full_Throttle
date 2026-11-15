"""L_BayshoreSkyTest: the sky, a causeway road with stand-in blocks, a camera on a rail that loops, rolls and corkscrews,
and a looping Level Sequence that drives it. Run in the offscreen editor: ue_run.ps1 -Editor -Script 50_test_map.py
"""
import sys
import os
import math
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)) if "__file__" in globals() else r"K:\Full Throttle\SkyScratch\SourceArt\BayshoreSky\unreal")
import unreal
from bsky_ue import log, enum_entry, ensure_dir, save, EAL, AT

MAP = "/BayshoreSky/Maps/L_BayshoreSkyTest"
SEQ_DIR = "/BayshoreSky/Maps"
SEQ = "LS_BayshoreSkyTest_CameraLoop"
FPS = 30
SPEED = 4500.0  # cm/s along the rail (162 km/h)

les = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

ensure_dir("/BayshoreSky/Maps")
for p in (SEQ_DIR + "/" + SEQ, MAP):
    if EAL.does_asset_exist(p):
        EAL.delete_asset(p)
unreal.SystemLibrary.collect_garbage()
if not les.new_level(MAP):
    raise RuntimeError("new_level failed")
world = unreal.EditorLevelLibrary.get_editor_world()


def spawn(cls, loc=(0, 0, 0), rot=(0, 0, 0), label=None):
    a = eas.spawn_actor_from_class(cls, unreal.Vector(*loc), unreal.Rotator(roll=rot[0], pitch=rot[1], yaw=rot[2]))
    if label:
        a.set_actor_label(label)
    return a


# ------------------------------------------------------------------------------------------------ sky
sky_cls = unreal.load_asset("/BayshoreSky/Blueprints/BP_BayshoreSky").generated_class()
sky = spawn(sky_cls, label="BayshoreSky")

# exposure: manual, so the unlit sky's values map straight to the screen (Matt's port sets the final exposure)
ppv = spawn(unreal.PostProcessVolume, label="PP_TestExposure")
ppv.set_editor_property("unbound", True)
s = ppv.get_editor_property("settings")
s.set_editor_property("override_auto_exposure_method", True)
s.set_editor_property("auto_exposure_method", enum_entry(unreal.AutoExposureMethod, "AEM_Manual"))
s.set_editor_property("override_auto_exposure_apply_physical_camera_exposure", True)
s.set_editor_property("auto_exposure_apply_physical_camera_exposure", False)
s.set_editor_property("override_auto_exposure_bias", True)
s.set_editor_property("auto_exposure_bias", 0.0)
s.set_editor_property("override_vignette_intensity", True)
s.set_editor_property("vignette_intensity", 0.0)
ppv.set_editor_property("settings", s)

# ------------------------------------------------------------------------------------------------ stand-ins (Engine content only)
cube = unreal.load_asset("/Engine/BasicShapes/Cube")
shape_mat = unreal.load_asset("/Engine/BasicShapes/BasicShapeMaterial")


def block(loc, size, label):
    a = spawn(unreal.StaticMeshActor, loc, label=label)
    c = a.static_mesh_component
    c.set_static_mesh(cube)
    c.set_material(0, shape_mat)
    a.set_actor_scale3d(unreal.Vector(size[0] / 100.0, size[1] / 100.0, size[2] / 100.0))
    return a


block((5000, 0, -50), (60000, 1400, 100), "StandIn_Causeway")
import random
rnd = random.Random(42)
for i in range(10):
    x = rnd.uniform(-12000, 22000)
    h = rnd.uniform(800, 2600)
    block((x, rnd.choice([-1, 1]) * rnd.uniform(2200, 4200), h / 2 - 100), (rnd.uniform(900, 2000), rnd.uniform(900, 2000), h),
          "StandIn_Block%02d" % i)

# ------------------------------------------------------------------------------------------------ rail: straight, vertical loop, corkscrew, banked turns
pts = []   # (position, up)
z0 = 600.0
for x in range(-15000, 0, 2500):
    pts.append(((x, 0.0, z0), (0, 0, 1)))
R = 3500.0
for k in range(24):
    ph = math.radians(7.5 + 15.0 * k)
    y = (7.5 + 15.0 * k) / 360.0 * 1600.0
    pts.append(((R * math.sin(ph), y, z0 + R * (1 - math.cos(ph))), (-math.sin(ph), 0.0, math.cos(ph))))
y0 = 1600.0
for x in range(2500, 12000, 2500):
    pts.append(((x, y0, z0), (0, 0, 1)))
rr = 900.0
for k in range(25):
    th = math.radians(15.0 * k + 7.5)
    x = 12000 + k * 750.0
    pts.append(((x, y0 + rr * math.sin(th), z0 + 300 + rr * (1 - math.cos(th))), (0.0, -math.sin(th), math.cos(th))))
# banked turn back toward -X
cx, cy, rt = 31000.0, y0 + 8000.0, 8000.0
for k in range(1, 12):
    a = -90.0 + k * 15.0
    ar = math.radians(a)
    bank = 0.45
    inward = (-math.cos(ar), -math.sin(ar), 0.0)
    up = (inward[0] * bank, inward[1] * bank, 1.0)
    pts.append(((cx + rt * math.cos(ar), cy + rt * math.sin(ar), z0 + 400), up))
for x in range(28000, -14000, -4000):
    pts.append(((x, cy + rt, z0 + 400), (0, 0, 1)))
cx2, cy2, rt2 = -16000.0, (cy + rt) / 2.0, (cy + rt) / 2.0
for k in range(1, 12):
    a = 90.0 + k * 15.0
    ar = math.radians(a)
    inward = (-math.cos(ar), -math.sin(ar), 0.0)
    pts.append(((cx2 + rt2 * math.cos(ar), cy2 + rt2 * math.sin(ar), z0 + 200), (inward[0] * 0.45, inward[1] * 0.45, 1.0)))

rail = spawn(unreal.CameraRig_Rail, label="CameraRail")
rail.set_editor_property("lock_orientation_to_rail", True)
spline = rail.get_rail_spline_component()
spline.clear_spline_points(False)
for i, (p, up) in enumerate(pts):
    spline.add_spline_point(unreal.Vector(*p), unreal.SplineCoordinateSpace.LOCAL, False)
spline.set_closed_loop(True, False)
spline.update_spline()
for i, (p, up) in enumerate(pts):
    u = unreal.Vector(*up)
    u.normalize()
    spline.set_up_vector_at_spline_point(i, u, unreal.SplineCoordinateSpace.LOCAL, False)
spline.update_spline()
length = spline.get_spline_length()
duration = length / SPEED
log("rail points", len(pts), "length m", round(length / 100.0), "duration s", round(duration, 1))
# check the frame: the camera must actually be upside down at the top of the loop
for d in (0.0,):
    pass
top_key = None
for i, (p, up) in enumerate(pts):
    if up[2] < -0.95:
        top_key = i
up_top = spline.get_up_vector_at_spline_point(top_key, unreal.SplineCoordinateSpace.WORLD)
log("up vector at the top of the loop", up_top)

cam = spawn(unreal.CineCameraActor, label="RailCamera")
cam.attach_to_actor(rail, "", unreal.AttachmentRule.SNAP_TO_TARGET, unreal.AttachmentRule.SNAP_TO_TARGET,
                    unreal.AttachmentRule.SNAP_TO_TARGET, False)
cc = cam.get_cine_camera_component()
fb = cc.get_editor_property("filmback")
fb.set_editor_property("sensor_width", 36.0)
fb.set_editor_property("sensor_height", 20.25)
cc.set_editor_property("filmback", fb)
cc.set_editor_property("current_focal_length", 18.0)
fs = cc.get_editor_property("focus_settings")
fs.set_editor_property("focus_method", enum_entry(unreal.CameraFocusMethod, "Disable"))
cc.set_editor_property("focus_settings", fs)

# ------------------------------------------------------------------------------------------------ level sequence
seq = AT.create_asset(SEQ, SEQ_DIR, unreal.LevelSequence, unreal.LevelSequenceFactoryNew())
seq.set_display_rate(unreal.FrameRate(FPS, 1))
end = int(round(duration * FPS))
seq.set_playback_start(0)
seq.set_playback_end(end)
rb = seq.add_possessable(rail)
tr = rb.add_track(unreal.MovieSceneFloatTrack)
tr.set_property_name_and_path("CurrentPositionOnRail", "CurrentPositionOnRail")
sec = tr.add_section()
sec.set_range(0, end)
ch = sec.get_channels_by_type(unreal.MovieSceneScriptingFloatChannel)[0]
ch.add_key(unreal.FrameNumber(0), 0.0, interpolation=unreal.MovieSceneKeyInterpolation.LINEAR)
ch.add_key(unreal.FrameNumber(end), 1.0, interpolation=unreal.MovieSceneKeyInterpolation.LINEAR)
cb = seq.add_possessable(cam)
cut = seq.add_track(unreal.MovieSceneCameraCutTrack)
cs = cut.add_section()
cs.set_range(0, end)
cs.set_camera_binding_id(seq.get_binding_id(cb))
save(seq)

lsa = spawn(unreal.LevelSequenceActor, label="CameraLoopSequence")
lsa.set_sequence(seq)
ps = lsa.get_editor_property("playback_settings")
ps.set_editor_property("auto_play", True)
lc = ps.get_editor_property("loop_count")
lc.set_editor_property("value", -1)
ps.set_editor_property("loop_count", lc)
lsa.set_editor_property("playback_settings", ps)

if not les.save_current_level():
    raise RuntimeError("save level failed")
log("map saved", MAP, "sequence frames", end)
