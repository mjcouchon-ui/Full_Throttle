"""Level 1 sky and sea: build the world materials and cloud meshes, stand the clouds in L_Bayshore, film the result.

  JAM_BUILD=1   rebuild ft_world's Bayshore assets (sea, cloud material and meshes) first
  JAM_PLACE=1   (re)place the clouds from ft_world.CLOUD_LAYOUT and save the map
  JAM_TAG       output folder under HandBack/jam (default sky1)
  JAM_POSES     metres along the road for chase-camera frames

Run: jam_run.ps1 -Script 24_bay_sky.py -Env "JAM_BUILD=1;JAM_PLACE=1;JAM_TAG=sky1"
"""
import os
import sys
import math
import unreal

sys.path.insert(0, r"K:\Full Throttle\SkyScratch\SourceArt\Jam\unreal")
import jam_cam as J

TAG = os.environ.get("JAM_TAG", "sky1")
POSES = [float(x) for x in os.environ.get("JAM_POSES", "200,640,1180,1700,2600,3880,5200,8000,11180,14000,17000,20500").split(",")]
try:
    import importlib
    import ft_world
    importlib.reload(ft_world)
    if os.environ.get("JAM_BUILD") == "1":
        ft_world.build_all()
    tr = J.load(0)
    if os.environ.get("JAM_GROUND") == "1":
        # the level table's ground (ft_cel.LEVELS) onto the map's track; colours first (see ft_cel.apply_level)
        import ft_cel
        importlib.reload(ft_cel)
        g = ft_cel.LEVELS[0]["ground"]
        tr.set_editor_property("ground_colours", [unreal.LinearColor(*(ft_cel.hex_lin(h) + (1,))) for _, _, h in g])
        tr.set_editor_property("ground_profile", [unreal.Vector2D(x, z) for x, z, _ in g])
        J.LES.save_current_level()
        tr = J.track()
        J.log("ground profile applied", len(g), "points")
    if os.environ.get("JAM_PLACE") == "1":
        ft_world.place_bay_clouds(save=True)
    J.log("floor z", tr.tool_floor_z(), "clouds", [a.get_actor_label() for a in J.actors() if a.get_actor_label().startswith("BayCloud_")])
    shots = []
    for d in POSES:
        flags = tr.tool_flags(d * 100.0)
        loc, rot, fov = J.chase(tr, d * 100.0, loop=bool(flags & 3))
        shots.append(("d%05d" % d, loc, rot, fov, None))
    # the sky all round, from over the start line: eight headings, level camera, and one high view of the bay
    pos, fwd, right, up = J.frame(tr, 30000.0)
    eye = unreal.Vector(pos.x, pos.y, pos.z + 400.0)
    for az in (30, 75, 120, 165, 210, 255, 300, 345):
        a = math.radians(az)
        shots.append(("pan%03d" % az, eye, unreal.Rotator(roll=0.0, pitch=7.0, yaw=float(az)), 80.0, None))
    hi = unreal.Vector(pos.x, pos.y, pos.z + 9000.0)
    shots.append(("high_sun", hi, unreal.Rotator(roll=0.0, pitch=-8.0, yaw=30.0), 80.0, None))
    shots.append(("high_bay", hi, unreal.Rotator(roll=0.0, pitch=-14.0, yaw=-70.0), 80.0, None))
    J.Shooter(TAG, shots, warm=50, frames=14, size=(1720, 720)).start()
except Exception as e:  # noqa
    import traceback
    J.log("ERR", repr(e), traceback.format_exc())
    unreal.SystemLibrary.quit_editor()
