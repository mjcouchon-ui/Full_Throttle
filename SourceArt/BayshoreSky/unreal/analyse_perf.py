"""python -I analyse_perf.py <perf dir>: compare run A (sky on) with run B (sky meshes hidden).
Reads <run>/stats.json written by Engine/Binaries/DotNET/CsvTools/csvinfo.exe -showAllStats -toJson."""
import json, sys, os, statistics as st
D = sys.argv[1]
SKIP = 240   # first 4 s at the fixed 60 Hz step: loading and PSO warm-up
def load(run):
    d = json.load(open(os.path.join(D, run, "stats.json")))
    return {k: v["all"] for k, v in d["stats"].items() if "all" in v}, d.get("metadata", {})
A, mA = load("A"); B, mB = load("B")
def s(v):
    v = sorted(v[SKIP:]); n = len(v)
    return st.mean(v), v[n // 2], v[int(n * 0.95)], v[-1]
print("GPU %s, driver %s, %sx%s, frames %d (first %d skipped)" % (mA.get("gpu"), mA.get("gpudriver"), mA.get("systemresolution.resx"),
      mA.get("systemresolution.resy"), len(A["FrameTime"]), SKIP))
print("%-36s %30s   %30s   %9s" % ("stat (ms or count)", "A sky on: mean / med / p95 / max", "B sky off: mean / med / p95 / max", "A-B mean"))
keys = ["FrameTime", "GPUTime", "GameThreadTime", "RenderThreadTime", "RHIThreadTime", "RHI/DrawCalls", "RHI/PrimitivesDrawn"]
keys += sorted(k for k in A if k.startswith("GPU/"))
for k in keys:
    if k not in A or k not in B:
        continue
    a, b = s(A[k]), s(B[k])
    if k.startswith("GPU/") and max(a[0], b[0]) < 0.005:
        continue
    print("%-36s %7.3f %7.3f %7.3f %7.2f   %7.3f %7.3f %7.3f %7.2f   %+9.3f" % ((k,) + a + b + (a[0] - b[0],)))
print()
for k in sorted(x for x in A if x.startswith("PSO/")):
    v = A[k]
    late = [(i, x) for i, x in enumerate(v) if i >= SKIP and x > 0]
    print("%-36s all-frames total %8.2f   frames>0 after warm-up: %d %s" % (k, sum(v), len(late), late[:6]))
