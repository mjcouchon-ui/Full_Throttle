"""Stand-in textures in the FINAL channel layout of the photoreal bakes (bake_layers.py, bake_sky.py stage cumulus3), made
from the previous deck and cumulus textures, so the runtime shaders can be built and checked before the Cycles bakes land.
Written to bakes/Day/_placeholder/; 21_import_textures.py uses them only when the real file is missing.
Run with Blender's Python: make_placeholders.py"""
import os
import numpy as np
import OpenImageIO as oiio

B = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "bakes", "Day")
OUT = os.path.join(B, "_placeholder")
os.makedirs(OUT, exist_ok=True)


def read(path):
    buf = oiio.ImageBuf(path)
    spec = buf.spec()
    if spec.nchannels == 4:
        spec.attribute("oiio:UnassociatedAlpha", 1)
    return oiio.ImageBuf(path, 0, 0, oiio.ImageSpec(0, 0, 0, oiio.FLOAT) if False else None).get_pixels(oiio.FLOAT) if False else _read_straight(path)


def _read_straight(path):
    cfg = oiio.ImageSpec()
    cfg.attribute("oiio:UnassociatedAlpha", 1)
    inp = oiio.ImageInput.open(path, cfg)
    px = inp.read_image(0, 0, 0, inp.spec().nchannels, oiio.FLOAT)
    inp.close()
    return px


def write(path, arr, fmt):
    h, w = arr.shape[:2]
    c = 1 if arr.ndim == 2 else arr.shape[2]
    spec = oiio.ImageSpec(w, h, c, fmt)
    if c == 4:
        spec.attribute("oiio:UnassociatedAlpha", 1)
    o = oiio.ImageOutput.create(path)
    o.open(path, spec)
    o.write_image(arr if fmt != oiio.UINT8 else (np.clip(arr, 0, 1) * 255 + 0.5).astype(np.uint8))
    o.close()
    print("wrote", path, arr.shape)


deck = _read_straight(os.path.join(B, "T_BSky_Day_Deck.png"))            # R field (edge 0.5), G thickness, B fibres, A cloudlets
R, G, Bf = deck[..., 0], deck[..., 1], deck[..., 2]
alpha = np.clip((R - 0.42) / 0.2, 0, 1) ** 1.2
sun = (0.35 + 0.9 * (1 - G)) * alpha + 0.05
sky = 0.25 + 0.2 * (1 - G)
tau = G * 2.5
write(os.path.join(OUT, "T_BSky_Day_AltoLight.exr"), np.stack([sun, sky, tau], -1).astype(np.float32), oiio.HALF)
write(os.path.join(OUT, "T_BSky_Day_AltoAlpha.png"), alpha.astype(np.float32), oiio.UINT8)
calpha = np.clip((Bf - 0.55) * 2.5, 0, 1) * 0.6
write(os.path.join(OUT, "T_BSky_Day_CirrusLight.exr"), np.stack([0.6 + 0 * Bf, 0.25 + 0 * Bf, calpha * 0.8], -1).astype(np.float32), oiio.HALF)
write(os.path.join(OUT, "T_BSky_Day_CirrusAlpha.png"), calpha.astype(np.float32), oiio.UINT8)

cu = _read_straight(os.path.join(B, "T_BSky_Day_Cumulus.png"))          # R sqrt(sun), G sqrt(sky), B thickness, A opacity
sunL = cu[..., 0] ** 2 * 1.79
skyL = cu[..., 1] ** 2
write(os.path.join(OUT, "T_BSky_Day_CumulusSun.exr"), np.stack([sunL * 0.5, sunL, sunL * 1.2], -1).astype(np.float32), oiio.HALF)
write(os.path.join(OUT, "T_BSky_Day_CumulusAux.png"), np.stack([skyL, cu[..., 3], cu[..., 2], np.ones_like(skyL)], -1).astype(np.float32), oiio.UINT8)
