"""Level 1 sky clipping, step 1: find it. Walks the whole Bayshore track with the chase camera, logs every stretch
where the road banks or stands high over the land (where the camera can see past the land's edge), and films those.

Run: bld_run.ps1 -Script <this>     Output: HandBack/jam/clip_sweep/  and the JAM log lines.
"""
import os
import sys
import math
import unreal

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)) if "__file__" in globals() else r"K:\Full Throttle\SkyScratch\SourceArt\Jam\unreal")
import jam_cam as J

try:
    tr = J.load(0)
    L = tr.tool_length()
    J.log("track length m", round(L / 100.0), "actors", [a.get_actor_label() for a in J.actors()])
    cands = []
    d = 3000.0
    while d < L - 3000.0:
        pos, fwd, right, up = J.frame(tr, d)
        g = tr.tool_ground(d)
        bank = tr.tool_bank(d)
        flags = tr.tool_flags(d)
        height = pos.z - g.z
        tilt = math.degrees(math.acos(max(-1.0, min(1.0, up.z))))
        cands.append((d, bank, height, tilt, flags, pos))
        d += 2500.0
    # the banked or raised stretches, strongest first, at least 150 m apart
    cands.sort(key=lambda c: -(abs(c[3]) + max(0.0, c[2]) / 150.0))
    picked = []
    for c in cands:
        if all(abs(c[0] - p[0]) > 15000.0 for p in picked):
            picked.append(c)
        if len(picked) >= 28:
            break
    picked.sort(key=lambda c: c[0])
    shots = []
    for d, bank, height, tilt, flags, pos in picked:
        J.log("cand d=%.0f m bank=%.2f height=%.1f m tilt=%.1f flags=%d pos=(%.0f,%.0f,%.0f)" % (d / 100.0, bank, height / 100.0, tilt, flags, pos.x, pos.y, pos.z))
        loc, rot, fov = J.chase(tr, d, loop=bool(flags & 3))
        shots.append(("d%05d_t%02d_h%03d" % (d / 100.0, tilt, max(0, height / 100.0)), loc, rot, fov, None))
    J.Shooter("clip_sweep", shots, warm=40, frames=10, size=(860, 360)).start()
except Exception as e:  # noqa
    import traceback
    J.log("ERR", repr(e), traceback.format_exc())
    unreal.SystemLibrary.quit_editor()
