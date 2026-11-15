"""python jam_pairs.py <out.jpg> <level 0-7> <before tag> <after tag> [pose,pose,...] [tile_w]
A before/after sheet for one level from two runs of 26_tour_editor.py / 28_specials_tour.py: one row per pose, the
before on the left, the after on the right. Tags are folders under HandBack/jam (each holds L<n>/<pose>.png).
Poses default to every frame both runs have; give a list (d02000,high01500_across,...) to pick."""
import os
import sys
import subprocess

ROOT = r"K:\Full Throttle\SkyScratch\HandBack\jam"
NAMES = ["Bayshore", "Chemical Plant", "Nevada", "Volcano", "Theme Park", "Ice Mountain", "Under the Sea", "City Center"]
out, lv, before, after = sys.argv[1], int(sys.argv[2]), sys.argv[3], sys.argv[4]
poses = [p for p in sys.argv[5].split(",") if p] if len(sys.argv) > 5 and sys.argv[5] else None
tw = sys.argv[6] if len(sys.argv) > 6 else "860"
bdir, adir = os.path.join(ROOT, before, "L%d" % lv), os.path.join(ROOT, after, "L%d" % lv)
if poses is None:
    poses = sorted(f[:-4] for f in os.listdir(adir) if f.endswith(".png") and os.path.exists(os.path.join(bdir, f)))
args = []
for p in poses:
    b, a = os.path.join(bdir, p + ".png"), os.path.join(adir, p + ".png")
    if not (os.path.exists(b) and os.path.exists(a)):
        print("missing", p, os.path.exists(b), os.path.exists(a))
        continue
    what = p[1:].lstrip("0") + " m" if p.startswith("d") else p
    args += ["%s, %s: before" % (NAMES[lv], what) + "=" + b, "%s, %s: after" % (NAMES[lv], what) + "=" + a]
subprocess.check_call([sys.executable, os.path.join(os.path.dirname(os.path.abspath(__file__)), "jam_sheet.py"), out, tw, "2"] + args)
