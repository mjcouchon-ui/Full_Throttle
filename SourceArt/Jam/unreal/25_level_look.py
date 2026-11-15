"""Any level: (re)build its living-world assets, apply its map changes, film it from the chase camera and a few stands.

  JAM_LEVEL     0-7 (default 6, Under the Sea)
  JAM_BUILD     a ft_world function to call first, e.g. build_abyss  (empty: none)
  JAM_APPLY     a ft_world function to call with the level loaded (it may save the map), e.g. abyss_apply
  JAM_TAG       output folder under HandBack/jam
  JAM_POSES     metres along the road for chase-camera frames
  JAM_TWICE     1 = film every pose twice, a second or so apart (things that move show as a difference)
  JAM_STANDS    extra views "name:d:side_m:up_m:yaw_off:pitch" (a camera standing beside the road at d, looking along it)

Run: jam_run.ps1 -Script 25_level_look.py -Env "JAM_LEVEL=6;JAM_BUILD=build_abyss;JAM_APPLY=abyss_apply;JAM_TAG=abyss1"
"""
import os
import sys
import math
import unreal

sys.path.insert(0, r"K:\Full Throttle\SkyScratch\SourceArt\Jam\unreal")
import jam_cam as J

LEVEL = int(os.environ.get("JAM_LEVEL", "6"))
TAG = os.environ.get("JAM_TAG", "look_L%d" % LEVEL)
POSES = [float(x) for x in os.environ.get("JAM_POSES", "300,900,1500,2400,3300,4500,6000").split(",") if x]
try:
    import importlib
    import ft_world
    importlib.reload(ft_world)
    if os.environ.get("JAM_BUILD"):
        for fn in os.environ["JAM_BUILD"].split(","):
            getattr(ft_world, fn)()
    tr = J.load(LEVEL)
    if os.environ.get("JAM_APPLY"):
        for fn in os.environ["JAM_APPLY"].split(","):
            getattr(ft_world, fn)()
        tr = J.track()
    J.log("level", LEVEL, J.NAMES[LEVEL], "length m", round(tr.tool_length() / 100.0), "floor z", tr.tool_floor_z())
    shots = []
    twice = os.environ.get("JAM_TWICE") == "1"
    for d in POSES:
        flags = tr.tool_flags(d * 100.0)
        loc, rot, fov = J.chase(tr, d * 100.0, loop=bool(flags & 3))
        shots.append(("d%05d" % d, loc, rot, fov, None))
        if twice:
            shots.append(("d%05d_b" % d, loc, rot, fov, None))
    for spec in [s for s in os.environ.get("JAM_STANDS", "").split(";") if s]:
        name, d, side, upm, yaw_off, pitch = spec.split(":")
        pos, fwd, right, up = J.frame(tr, float(d) * 100.0)
        g = tr.tool_ground(float(d) * 100.0)
        rt = unreal.Vector(-fwd.y, fwd.x, 0.0)
        n = math.hypot(rt.x, rt.y) or 1.0
        loc = unreal.Vector(g.x + rt.x / n * float(side) * 100.0, g.y + rt.y / n * float(side) * 100.0, g.z + float(upm) * 100.0)
        yaw = math.degrees(math.atan2(fwd.y, fwd.x)) + float(yaw_off)
        shots.append((name, loc, unreal.Rotator(roll=0.0, pitch=float(pitch), yaw=yaw), 80.0, None))
    J.Shooter(TAG, shots, warm=50, frames=14, size=(1720, 720)).start()
except Exception as e:  # noqa
    import traceback
    J.log("ERR", repr(e), traceback.format_exc())
    unreal.SystemLibrary.quit_editor()
