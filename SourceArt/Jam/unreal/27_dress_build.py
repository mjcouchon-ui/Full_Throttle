"""Build dressing kits (and any ft_world assets), apply map changes, then film the chase-camera run: one editor job.

  JAM_KITS      "bayshore,abyss" ...   kits to export with ft_dress.run (empty: none)
  JAM_ONLY      piece names to rebuild within those kits (default: all)
  JAM_WORLD     ft_world functions to call first, e.g. "build_abyss"
  JAM_APPLY     "6:abyss_apply;0:place_bay_clouds"   level:ft_world function pairs, run with that level loaded
  then everything 26_tour_editor.py takes (JAM_LEVELS, JAM_TAG, JAM_POSES, JAM_HIGH)

Run: jam_run.ps1 -Checkout <project> -Script 27_dress_build.py -Env "JAM_KITS=abyss;JAM_LEVELS=6;JAM_TAG=dress_after1"
"""
import os
import sys
import importlib
import traceback
import unreal

sys.path.insert(0, r"K:\Full Throttle\SkyScratch\SourceArt\Jam\unreal")
import jam_cam as J

errors = []


def step(what, fn):
    """One build step on its own: a failure is logged and the rest still runs (a broken piece must not cost the
    whole job its camera run)."""
    try:
        fn()
        J.log(what, "done")
    except Exception as e:  # noqa
        errors.append(what)
        J.log("ERR build", what, repr(e)[:900], traceback.format_exc()[-1500:])


if os.environ.get("JAM_WORLD"):
    import ft_world
    importlib.reload(ft_world)
    for fn in [f for f in os.environ["JAM_WORLD"].split(",") if f]:
        step("ft_world." + fn, getattr(ft_world, fn))
kits = [k for k in os.environ.get("JAM_KITS", "").split(",") if k]
if kits:
    import ft_dress
    importlib.reload(ft_dress)
    only = [n for n in os.environ.get("JAM_ONLY", "").split(",") if n] or None
    for kit in kits:
        step("kit " + kit, lambda kit=kit: ft_dress.run([kit], only))
for pair in [p for p in os.environ.get("JAM_APPLY", "").split(";") if p]:
    lv, fn = pair.split(":")

    def apply(lv=lv, fn=fn):
        import ft_world
        J.load(int(lv))
        getattr(ft_world, fn)()
    step("apply %s to level %s" % (fn, lv), apply)
J.log("build steps failed:", ", ".join(errors) if errors else "none")
ok = not errors
if ok or os.environ.get("JAM_TOUR_ANYWAY", "1") == "1":   # the run is filmed anyway unless JAM_TOUR_ANYWAY=0
    exec(compile(open(r"K:\Full Throttle\SkyScratch\SourceArt\Jam\unreal\26_tour_editor.py").read(), "26_tour_editor.py", "exec"))
else:
    unreal.SystemLibrary.quit_editor()
