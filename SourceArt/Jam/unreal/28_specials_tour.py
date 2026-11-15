"""A chase-camera run of each level's SPECIAL sections: jumps, loops, corkscrews, pipes, half-pipes, wall rides,
coaster track, building runs, the highest elevated stretches and the steepest banked turns. This is the clearance
check for scenery: the camera rides the road's own frame there, as the game's does, so anything that stands in the
car's way, in the camera's way, or in the open under an elevated or inverted camera shows in these frames.

Frames go to HandBack/jam/<JAM_TAG>/L<n>/s_<kind>_d<metres>.png; the log lists what was found where.

  JAM_LEVELS    "0,1,2,3,4,5,6,7" (default all)
  JAM_TAG       output folder under HandBack/jam (default specials)
  JAM_NODRESS   1 = build the levels without the dressing (ft.Dress 0)
  JAM_PER       frames per kind of section (default 2: the first and the last of its kind)

Run: jam_run.ps1 -Script 28_specials_tour.py -Env "JAM_TAG=specials_after" [-Checkout <project folder>]
"""
import os
import sys
import unreal

sys.path.insert(0, r"K:\Full Throttle\SkyScratch\SourceArt\Jam\unreal")
import jam_cam as J

LEVELS = [int(x) for x in os.environ.get("JAM_LEVELS", "0,1,2,3,4,5,6,7").split(",") if x != ""]
TAG = os.environ.get("JAM_TAG", "specials")
PER = int(os.environ.get("JAM_PER", "2"))
state = {"i": 0}
if os.environ.get("JAM_NODRESS") == "1":
    unreal.SystemLibrary.execute_console_command(J.world(), "ft.Dress 0")
    J.log("ft.Dress 0: levels build without the dressing")


def one():
    if state["i"] >= len(LEVELS):
        unreal.SystemLibrary.quit_editor()
        return
    lv = LEVELS[state["i"]]
    state["i"] += 1
    try:
        tr = J.load(lv)
        shots, summary = J.specials(tr, PER)
        J.log("level", lv, J.NAMES[lv], "specials:", summary, "| frames", len(shots))
        if not shots:
            one()
            return
        J.Shooter(os.path.join(TAG, "L%d" % lv), shots, done=one, warm=45, frames=10, size=(1720, 720)).start()
    except Exception as e:  # noqa
        import traceback
        J.log("ERR level", lv, repr(e), traceback.format_exc())
        one()


one()
