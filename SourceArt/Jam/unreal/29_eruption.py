"""Mount Kaldera's eruption, second by second: holds MI_Eruption's cycle at chosen seconds (its Scrub parameter) and
films each from the road, so the whole cycle can be judged without waiting for it. Level 3 only.

Frames: HandBack/jam/<JAM_TAG>/e<cone>_t<second>.png (a long lens on the cone from the road) and
        chase<cone>_t<second>.png (the game's chase camera at the same place).

  JAM_TAG      output folder (default eruption)
  JAM_BUILD    1 = run ft_world.build_volcano() first
  JAM_SECONDS  seconds of the 75 s cycle to hold (default: rumble, blast, plume growing, full, aftermath, quiet)

Run: jam_run.ps1 -Script 29_eruption.py -Checkout <project folder>
The material instance is not saved: Scrub goes back to -1 (the clock) at the end.
"""
import os
import sys
import math
import unreal

sys.path.insert(0, r"K:\Full Throttle\SkyScratch\SourceArt\Jam\unreal")
import jam_cam as J

TAG = os.environ.get("JAM_TAG", "eruption")
SECONDS = [float(x) for x in os.environ.get("JAM_SECONDS", "3,8.6,9.5,11,14,19,26,33,40,50,62").split(",") if x]
MEL = unreal.MaterialEditingLibrary
MI = None


def hold(sec):
    def f():
        MEL.set_material_instance_scalar_parameter_value(MI, "Scrub", sec)
        MEL.update_material_instance(MI)
    return f


def done():
    hold(-1.0)()
    unreal.SystemLibrary.quit_editor()


def main():
    global MI
    if os.environ.get("JAM_BUILD") == "1":      # rebuild the eruption's material and mesh first (ft_world.build_volcano)
        import importlib
        import ft_world
        importlib.reload(ft_world)
        ft_world.build_volcano()
        J.log("ft_world.build_volcano done")
    MI = unreal.EditorAssetLibrary.load_asset("/Game/Maps/Volcano/Materials/MI_Eruption")
    tr = J.load(3)
    if MI is None:
        J.log("ERR no MI_Eruption: run ft_world.build_volcano() first")
        unreal.SystemLibrary.quit_editor()
        return
    cones = []
    for c in tr.get_components_by_class(unreal.StaticMeshComponent):
        sm = c.get_editor_property("static_mesh")
        if sm is not None and sm.get_name() == "SM_Eruption":
            cones.append((c.get_world_location(), c.get_world_scale().x))
    J.log("eruption components", len(cones), [(round(p.x), round(p.y), round(p.z), round(s, 2)) for p, s in cones])
    if not cones:
        J.log("ERR the track has no eruption component (is SM_Eruption saved, and is this the build with LifeComps?)")
        unreal.SystemLibrary.quit_editor()
        return
    L = tr.tool_length()
    ds = [i * 5000.0 for i in range(int(L / 5000.0))]
    pts = [tr.tool_frame(d).translation for d in ds]
    shots = []
    for k, (p, sc) in enumerate(cones):
        i = min(range(len(ds)), key=lambda j: (pts[j].x - p.x) ** 2 + (pts[j].y - p.y) ** 2)
        near = math.hypot(pts[i].x - p.x, pts[i].y - p.y)
        d = max(1000.0, ds[i] - max(90000.0, near * 1.6))   # well before the road's nearest point: the cone is ahead and to a side
        pos, fwd, right, up = J.frame(tr, d)
        eye = unreal.Vector(pos.x, pos.y, pos.z + 400.0)
        half = 7400.0 * sc                                    # half of cone and plume together
        target = unreal.Vector(p.x, p.y, p.z + half * 0.92)
        dist = math.hypot(target.x - eye.x, target.y - eye.y)
        fov = max(30.0, min(100.0, 2.0 * math.degrees(math.atan2(half * 1.12 * (1720.0 / 720.0), dist))))
        J.log("cone", k, "scale", round(sc, 2), "nearest road at m", round(ds[i] / 100.0), "which is m away", round(near / 100.0),
              "filmed from m", round(d / 100.0), "lens fov", round(fov))
        secs = SECONDS if k == len(cones) - 1 or len(cones) == 1 else SECONDS[2:7:2]
        for s in secs:
            loc, rot, f = J.free(eye, target, fov)
            shots.append(("e%d_t%04.1f" % (k, s), loc, rot, f, hold(s)))
        for s in (9.5, 19.0, 40.0):
            loc, rot, f = J.chase(tr, d)
            shots.append(("chase%d_t%04.1f" % (k, s), loc, rot, f, hold(s)))
    J.Shooter(TAG, shots, done=done, warm=45, frames=12, size=(1720, 720)).start()


try:
    main()
except Exception as e:  # noqa
    import traceback
    J.log("ERR", repr(e), traceback.format_exc())
    unreal.SystemLibrary.quit_editor()
