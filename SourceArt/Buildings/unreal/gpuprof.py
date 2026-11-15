"""python -I gpuprof.py <logA> [<logB>] [--depth N] [--min ms] [--tree]: sums UE 5.9 `profilegpu` dumps per pass.

5.9 prints one box-drawn table per pipeline (graphics, compute, copy) with exclusive and inclusive columns and the pass
name, indented by depth, in the last column. This reads every dump in a log, takes each pass's inclusive time, keys it
by its path truncated to --depth levels below the frame's scene render, and averages over the dumps. With two logs it
prints both and the difference, largest first. --tree prints the first dump's hierarchy (to choose a depth)."""
import re
import sys

args = [a for a in sys.argv[1:] if not a.startswith("--")]
opt = {a.split("=")[0]: (a.split("=")[1] if "=" in a else "1") for a in sys.argv[1:] if a.startswith("--")}
DEPTH = int(opt.get("--depth", 3))
MIN = float(opt.get("--min", 0.03))
sys.stdout.reconfigure(encoding="utf-8", errors="replace")
ROW = re.compile(r"LogRHI: Display:\s+┃(.*)┃(.*)┃(.*)┃\s*$")
MS = re.compile(r"([\d\.]+) ms\s*$")
STRIP = re.compile(r"\s+\d+x\d+.*$|\s*\(.*$|\s+Frame \d+.*$")


def dumps(path):
    """[[(depth, name, inclusive_ms, exclusive_ms)]] per pipeline table."""
    out, cur = [], None
    for line in open(path, encoding="utf-8-sig", errors="replace"):
        if "pipeline" in line and "GPU" in line and "LogRHI" in line:
            cur = []
            out.append((line.split("Display:")[1].strip(), cur))
            continue
        m = ROW.search(line.rstrip("\n"))
        if not m or cur is None:
            continue
        ex, inc, name = m.group(1), m.group(2), m.group(3)
        me, mi = MS.search(ex.strip()), MS.search(inc.strip())
        if not me or not mi:
            continue
        name = name.rstrip()
        depth = (len(name) - len(name.lstrip(" ")) - 1) // 3
        cur.append((depth, name.strip(), float(mi.group(1)), float(me.group(1))))
    return out


def summarise(path):
    tables = [t for t in dumps(path) if t[1]]
    frames = max(1, sum(1 for name, _ in tables if "Graphics pipeline" in name))
    acc = {}
    for pipe, rows in tables:
        stack = []
        for depth, name, inc, exc in rows:
            stack = stack[:depth] + [STRIP.sub("", name).strip('"')]
            if "Scene" in stack[:-1]:
                # the passes of the scene: inclusive time of the node DEPTH levels below "Scene"
                rel = stack[stack.index("Scene") + 1:]
                if len(rel) == DEPTH or (len(rel) < DEPTH and exc >= 0.0 and False):
                    key = " / ".join(rel)
                    acc[key] = acc.get(key, 0.0) + inc
                elif len(rel) < DEPTH:
                    key = " / ".join(rel) + " (self)"
                    acc[key] = acc.get(key, 0.0) + exc
            elif len(stack) >= 2 and stack[-2].startswith("RenderGraphExecute") and stack[-1] != "Scene":
                acc["[outside Scene] " + stack[-1]] = acc.get("[outside Scene] " + stack[-1], 0.0) + inc
    return {k: v / frames for k, v in acc.items()}, frames


if "--tree" in opt:
    for pipe, rows in dumps(args[0])[:3]:
        print("==", pipe)
        for depth, name, inc, exc in rows:
            if inc >= MIN and depth <= int(opt.get("--depth", 6)):
                print("%7.3f %7.3f  %s%s" % (inc, exc, "  " * depth, name[:90]))
    sys.exit(0)

a, fa = summarise(args[0])
b, fb = summarise(args[1]) if len(args) > 1 else ({}, 0)
names = sorted(set(a) | set(b), key=lambda k: -abs(b.get(k, 0.0) - a.get(k, 0.0)) if b else -a.get(k, 0.0))
print("%-78s %8s %8s %8s   (dumps %d / %d)" % ("pass, ms per frame", "A", "B", "delta", fa, fb))
ta = tb = 0.0
for k in names:
    va, vb = a.get(k, 0.0), b.get(k, 0.0)
    ta, tb = ta + va, tb + vb
    if max(va, vb) >= MIN:
        print("%-78s %8.3f %8.3f %+8.3f" % (k[-78:], va, vb, vb - va))
print("%-78s %8.3f %8.3f %+8.3f" % ("TOTAL", ta, tb, tb - ta))
