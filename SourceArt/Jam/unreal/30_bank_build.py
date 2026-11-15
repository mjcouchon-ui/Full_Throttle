"""The cut banks (15 Nov 2026): one editor job for the five levels whose banked bends were bare.

  1. exports the kits' new panels (ft_dress.run([kit], only=[...])) and lists their sizes and materials;
  2. for each level, films the same poses twice from the same build: with ft.DressBank 0 (the cuttings bare: the
     "before") and with ft.DressBank 1. The poses are found from the track itself: the chase camera in the middle of
     the steepest and of some shallow banked bends, in a steep bend's lead-in, and two close looks at a steep
     cutting (from over the road, and along the foot of the bank just over the low barrier) to see how the panels
     sit on it.

  BANK_LEVELS   "1,2,3,5,6" (default)
  BANK_TAG      folder under HandBack/jam (default banks1): <tag>_off/L<n>/..., <tag>/L<n>/...
  BANK_EXPORT   0 = no export (film only)
  BANK_BEFORE   0 = no "before" pass; or the levels to film bare, e.g. "6"

Run: jam_run.ps1 -Checkout <project> -Script <this file> -Env "BANK_TAG=banks1"
The editor log (<project>\\Saved\\AgentTemp\\bank_build.log) has each load's FTTRACK, FTDRESS and FTBANK lines.
"""
import os
import sys
import math
import importlib
import traceback
import unreal

HERE = r"K:\Full Throttle\SkyScratch\SourceArt\Jam\unreal"
sys.path.insert(0, HERE)
import jam_cam as J

LEVELS = [int(x) for x in os.environ.get("BANK_LEVELS", "1,2,3,5,6").split(",") if x != ""]
TAG = os.environ.get("BANK_TAG", "banks1")
EXPORT = os.environ.get("BANK_EXPORT", "1") == "1"
_b = os.environ.get("BANK_BEFORE", "1")
BEFORE = LEVELS if _b == "1" else [int(x) for x in _b.split(",") if x not in ("", "0")]
KEY = {1: "chem", 2: "nevada", 3: "volcano", 5: "ice", 6: "abyss"}
PIECES = {1: ["SM_CutRevet", "SM_CutRevet2", "SM_CutKerb", "SM_CutKerb2"], 2: ["SM_CutStrata", "SM_CutStrata2", "SM_CutLedge"],
          3: ["SM_CutBasalt", "SM_CutBasalt2", "SM_CutAsh"], 5: ["SM_CutIce", "SM_CutIce2", "SM_CutSnow"],
          6: ["SM_CutReef", "SM_CutReef2", "SM_CutShoal"]}
SKIP = 1 | 2 | 4 | 8 | 32 | 64 | 1024 | 2048 | 4096


def export():
    import ft_dress
    import ft_dress_bank
    importlib.reload(ft_dress_bank)
    importlib.reload(ft_dress)
    for lv in LEVELS:
        try:
            only = [n for n in os.environ.get("BANK_ONLY", "").split(",") if n] or PIECES[lv]
            only = [n for n in only if n in PIECES[lv]]
            if not only:
                continue
            ft_dress.run([KEY[lv]], only)
            mats = set()
            for name in PIECES[lv]:
                sm = unreal.EditorAssetLibrary.load_asset(ft_dress.DIRS[KEY[lv]] + "/" + name)
                if not sm:
                    J.log("ERR no mesh", lv, name)
                    continue
                b = sm.get_bounds()
                for i in range(len(sm.get_editor_property("static_materials"))):
                    mi = sm.get_material(i)
                    if mi:
                        mats.add(mi.get_path_name().split(".")[0])
                J.log("piece", lv, name, "triangles", sm.get_num_triangles(0), "size cm", round(b.box_extent.x * 2), round(b.box_extent.y * 2),
                      round(b.box_extent.z * 2), "middle", round(b.origin.x), round(b.origin.y), round(b.origin.z))
            J.log("materials", lv, len(mats), " ".join(sorted(mats)))
        except Exception as e:  # noqa
            J.log("ERR export", lv, repr(e)[:400], traceback.format_exc()[-1200:])


def bends(tr):
    """[(from cm, to cm, largest bank)] of the banked stretches (19 degrees and more) outside special sections."""
    L = tr.tool_length()
    out, cur = [], None
    d = 500.0
    while d < L - 3000.0:
        b, fl = tr.tool_bank(d), tr.tool_flags(d)
        if abs(b) >= 0.33 and not (fl & SKIP):
            if cur is None:
                cur = [d, d, b]
            cur[1] = d
            if abs(b) > abs(cur[2]):
                cur[2] = b
        elif cur is not None:
            out.append(tuple(cur))
            cur = None
        d += 500.0
    if cur:
        out.append(tuple(cur))
    return out


def _flat(v):
    n = math.hypot(v.x, v.y) or 1.0
    return unreal.Vector(v.x / n, v.y / n, 0.0)


def shots_for(tr):
    bs = bends(tr)
    steep = sorted([b for b in bs if abs(b[2]) >= 0.6], key=lambda b: -abs(b[2]))
    mild = [b for b in bs if abs(b[2]) < 0.6]
    picks = steep[:3] + ([mild[0], mild[len(mild) // 2], mild[-1]] if len(mild) >= 3 else mild)
    shots, seen = [], set()
    for k, (a, b, bank) in enumerate(picks):
        mid = (a + b) / 2.0
        if int(mid) in seen:
            continue
        seen.add(int(mid))
        deg = round(abs(bank) * 57.2958)
        loc, rot, fov = J.chase(tr, mid)
        shots.append(("bank%02ddeg_d%05d" % (deg, mid / 100.0), loc, rot, fov, None))
        if abs(bank) >= 0.6:
            for name, d in (("in", a + 2500.0), ("out", b - 1500.0)):
                loc, rot, fov = J.chase(tr, d)
                shots.append(("bank%02ddeg_d%05d_%s" % (deg, d / 100.0, name), loc, rot, fov, None))
        if k < 2 or abs(bank) < 0.6 and k == len(steep[:3]):
            sd = 1.0 if bank > 0 else -1.0
            pos, fwd, right, up = J.frame(tr, mid)
            g = tr.tool_ground(mid)
            gr = _flat(right)
            # from over the road, looking at the cutting
            eye = J._add(pos, (up, 900.0))
            tgt = J._add(g, (gr, sd * 1250.0), (unreal.Vector(0, 0, 1), -420.0), (_flat(fwd), 500.0))
            shots.append(("look%02ddeg_d%05d" % (deg, mid / 100.0),) + J.free(eye, tgt, 88.0) + (None,))
            # along the foot of the bank, just over the low barrier
            eye = J._add(pos, (right, sd * 1130.0), (up, 190.0))
            p2, f2, r2, u2 = J.frame(tr, mid + 3000.0)
            tgt = J._add(p2, (r2, sd * 1500.0), (u2, 420.0))
            shots.append(("foot%02ddeg_d%05d" % (deg, mid / 100.0),) + J.free(eye, tgt, 88.0) + (None,))
    summary = "%d banked stretches, %d of them 34 degrees and more; filmed %s" % (
        len(bs), len(steep), ", ".join("%.2f km %s%d" % ((a + b) / 200000.0, "R" if bank > 0 else "L", round(abs(bank) * 57.2958)) for a, b, bank in picks))
    return shots, summary


def count(lv, tr, what):
    rows = []
    for c in tr.get_components_by_class(unreal.HierarchicalInstancedStaticMeshComponent):
        sm = c.get_editor_property("static_mesh")
        if sm and sm.get_name().startswith("SM_Cut"):
            rows.append("%s:%d" % (sm.get_name(), c.get_instance_count()))
    J.log("panels", what, "level", lv, " ".join(sorted(rows)) or "none")


JOBS = []
for _lv in LEVELS:
    if _lv in BEFORE:
        JOBS.append((TAG + "_off", 0, _lv))
    JOBS.append((TAG, 1, _lv))
state = {"i": 0}


def one():
    if state["i"] >= len(JOBS):
        unreal.SystemLibrary.quit_editor()
        return
    tag, on, lv = JOBS[state["i"]]
    state["i"] += 1
    try:
        unreal.SystemLibrary.execute_console_command(J.world(), "ft.Dress 1")
        unreal.SystemLibrary.execute_console_command(J.world(), "ft.DressBank %d" % on)
        tr = J.load(lv)
        count(lv, tr, "on" if on else "off")
        shots, summary = shots_for(tr)
        J.log("level", lv, J.NAMES[lv], "banks", "ON" if on else "off (ft.DressBank 0)", "|", summary, "| frames", len(shots), "->", tag)
        J.Shooter(os.path.join(tag, "L%d" % lv), shots, done=one, warm=45, frames=10, size=(1720, 720)).start()
    except Exception as e:  # noqa
        J.log("ERR level", lv, repr(e), traceback.format_exc()[-1500:])
        one()


try:
    if EXPORT:
        export()
    one()
except Exception as e:  # noqa
    J.log("ERR", repr(e), traceback.format_exc()[-1500:])
    unreal.SystemLibrary.quit_editor()
