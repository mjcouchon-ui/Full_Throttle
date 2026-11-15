"""A chase-camera run of one or more levels in the editor: the same poses every time, so a run before and a run
after a scenery change pair up frame for frame. Each level's frames go to HandBack/jam/<JAM_TAG>/L<n>/d<metres>.png.

  JAM_LEVELS    "0,1,2,3,4,5,6,7" (default all)
  JAM_TAG       output folder under HandBack/jam (default tour_ed)
  JAM_POSES     metres along the road (default: 14 spread from the start to past the finale's start)
  JAM_NODRESS   1 = build the levels WITHOUT the dressing (console variable ft.Dress 0): the before of a before/after,
                from the same build, at any time
  JAM_HIGH      1 = also two high views per level (60 m up, looking along and across the road): what an elevated or
                inverted camera sees of the land
  JAM_SPECIALS  1 = also the level's special sections (jam_cam.specials: jumps, loops, pipes, the highest and the
                steepest stretches), named s_<kind>_d<metres>
  JAM_PASSES    several runs in one editor job, "tag:dress:levels+..." e.g. "dress_before:0:6+dress_after1:1:0,6"
                (dress 0 = without the dressing). Replaces JAM_TAG, JAM_LEVELS and JAM_NODRESS.

Run: jam_run.ps1 -Script 26_tour_editor.py -Env "JAM_TAG=dress_before" [-Checkout <project folder>]
"""
import os
import sys
import math
import unreal

sys.path.insert(0, r"K:\Full Throttle\SkyScratch\SourceArt\Jam\unreal")
import jam_cam as J

LEVELS = [int(x) for x in os.environ.get("JAM_LEVELS", "0,1,2,3,4,5,6,7").split(",") if x != ""]
TAG = os.environ.get("JAM_TAG", "tour_ed")
POSES = [float(x) for x in os.environ.get("JAM_POSES", "250,700,1300,2000,2900,3900,5000,6400,8000,9800,11800,14000,16500,19000").split(",") if x]
HIGH = os.environ.get("JAM_HIGH", "1") == "1"
SPECIALS = os.environ.get("JAM_SPECIALS", "0") == "1"
state = {"i": 0}
JOBS = []   # (tag, dressed, level)
if os.environ.get("JAM_PASSES"):
    for p in [x for x in os.environ["JAM_PASSES"].replace("+", ";").split(";") if x]:
        ptag, pdress, plevels = p.split(":")
        JOBS += [(ptag, pdress != "0", int(x)) for x in plevels.split(",") if x != ""]
else:
    JOBS = [(TAG, os.environ.get("JAM_NODRESS") != "1", lv) for lv in LEVELS]


def one():
    if state["i"] >= len(JOBS):
        unreal.SystemLibrary.quit_editor()
        return
    tag, dressed, lv = JOBS[state["i"]]
    state["i"] += 1
    try:
        # the track reads ft.Dress when the level loads and builds itself
        unreal.SystemLibrary.execute_console_command(J.world(), "ft.Dress %d" % (1 if dressed else 0))
        tr = J.load(lv)
        L = tr.tool_length()
        shots = []
        for d in POSES:
            if d * 100.0 > L - 3000.0:
                continue
            flags = tr.tool_flags(d * 100.0)
            loc, rot, fov = J.chase(tr, d * 100.0, loop=bool(flags & 3))
            shots.append(("d%05d" % d, loc, rot, fov, None))
        if HIGH:
            for d in (1500.0, 6000.0):
                pos, fwd, right, up = J.frame(tr, d * 100.0)
                g = tr.tool_ground(d * 100.0)
                yaw = math.degrees(math.atan2(fwd.y, fwd.x))
                eye = unreal.Vector(g.x, g.y, g.z + 6000.0)
                shots.append(("high%05d_along" % d, eye, unreal.Rotator(roll=0.0, pitch=-12.0, yaw=yaw), 85.0, None))
                shots.append(("high%05d_across" % d, eye, unreal.Rotator(roll=0.0, pitch=-14.0, yaw=yaw + 80.0), 85.0, None))
        if SPECIALS:
            more, summary = J.specials(tr)
            shots += more
            J.log("level", lv, J.NAMES[lv], "specials:", summary)
        J.log("level", lv, J.NAMES[lv], "dressed" if dressed else "NOT dressed (ft.Dress 0)", "length m", round(L / 100.0), "frames", len(shots), "->", tag)
        J.Shooter(os.path.join(tag, "L%d" % lv), shots, done=one, warm=45, frames=10, size=(1720, 720)).start()
    except Exception as e:  # noqa
        import traceback
        J.log("ERR level", lv, repr(e), traceback.format_exc())
        one()


one()
