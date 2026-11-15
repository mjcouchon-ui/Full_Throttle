"""Compare the baked Nishita golden-hour atmosphere (Master_Day_Atmosphere_Equirect.exr) with the R7 reference colours.
Run with Blender's Python: analyse_atmo.py <master.exr>. Prints linear radiance and an exposure-matched sRGB at chosen
(azimuth, elevation) points; azimuths are Unreal azimuths (u = (az + 180) / 360, SPEC.md), elevation from the top row."""
import sys
import numpy as np
import OpenImageIO as oiio

img = oiio.ImageBuf(sys.argv[1]).get_pixels(oiio.FLOAT)[..., :3]
H, W = img.shape[:2]


def at(az, el):
    u = ((az + 180.0) / 360.0) % 1.0
    v = (90.0 - el) / 180.0
    x, y = int(u * W) % W, min(int(v * H), H - 1)
    return img[max(y - 2, 0):y + 3, max(x - 2, 0):x + 3].reshape(-1, 3).mean(0)


def srgb(c):
    c = np.clip(c, 0, None)
    c = c / (1 + c * 0.0)
    return np.where(c <= 0.0031308, c * 12.92, 1.055 * np.power(c, 1 / 2.4) - 0.055)


pts = [("zenith", 0, 89), ("el45 sun side", 30, 45), ("el45 anti", 210, 45), ("el20 sun", 30, 20), ("el20 side", 120, 20),
       ("el20 anti", 210, 20), ("el5 sun", 30, 5), ("el5 sun+40", 70, 5), ("el5 side", 120, 5), ("el5 anti", 210, 5),
       ("el1 sun", 30, 1), ("el1 anti", 210, 1)]
ref = {"zenith": "#5C79AC", "el45 sun side": "#9296B7", "el20 side": "#948FAA", "el5 anti": "#908BA2", "el1 sun": "#FA9F72",
       "el5 sun": "#D78480"}
vals = {n: at(a, e) for n, a, e in pts}
k = 0.62 / max(vals["el45 sun side"].max(), 1e-6)       # expose so the el-45 sky matches R7's mid-sky value
for n, a, e in pts:
    c = vals[n]
    s = np.clip(srgb(c * k), 0, 1)
    print("%-15s az %4d el %3d  lin %s  sRGB(k) #%02X%02X%02X  ref %s" % (n, a, e, np.round(c, 4), *(s * 255 + 0.5).astype(int), ref.get(n, "")))
print("exposure k", k, "shape", img.shape)
