"""Bake reference beside the runtime result, for judging the semi-cel remap against its source.
python -I compare_bake_runtime.py Day  ->  HandBack/compare/Compare_<Preset>_*.png
Reference strips cover azimuth -180..180 (u = (az + 180) / 360) and elevation +32..-8 degrees; screenshots are 90 deg wide.
"""
import os
import sys
from PIL import Image, ImageDraw, ImageFont

P = sys.argv[1] if len(sys.argv) > 1 else "Day"
ROOT = r"K:\Full Throttle\SkyScratch"
REF = os.path.join(ROOT, "SourceArt", "BayshoreSky", "reference", P)
SHOT = os.path.join(ROOT, "HandBack", "screenshots")
OUT = os.path.join(ROOT, "HandBack", "compare")
os.makedirs(OUT, exist_ok=True)
font = ImageFont.load_default()


def ref_crop(strip, az_centre, width_deg=90.0):
    w, h = strip.size
    px = int(width_deg / 360.0 * w)
    cx = int((az_centre + 180.0) / 360.0 * w)
    wide = Image.new("RGB", (w * 2, h))
    wide.paste(strip, (0, 0))
    wide.paste(strip, (w, 0))
    x0 = (cx - px // 2) % w
    return wide.crop((x0, 0, x0 + px, h))


def label(im, text):
    d = ImageDraw.Draw(im)
    d.rectangle([0, 0, 8 + 6 * len(text), 16], fill=(0, 0, 0))
    d.text((4, 3), text, fill=(255, 255, 255), font=font)
    return im


def pair(top, bottom, title, out):
    W = 1200
    t = top.resize((W, int(top.height * W / top.width)))
    b = bottom.resize((W, int(bottom.height * W / bottom.width)))
    im = Image.new("RGB", (W, t.height + b.height + 24), (20, 20, 24))
    ImageDraw.Draw(im).text((6, 6), title, fill=(255, 255, 255), font=font)
    im.paste(label(t, "BAKE REFERENCE (photoreal Cycles render, not shipped)"), (0, 24))
    im.paste(label(b, "RUNTIME (semi-cel, packaged materials)"), (0, 24 + t.height))
    im.save(out)
    print("wrote", out)


strip = Image.open(os.path.join(REF, "Ref_%s_PhotorealStrip.png" % P)).convert("RGB")
pair(ref_crop(strip, 30.0), Image.open(os.path.join(SHOT, "01_toward_sun.png")).convert("RGB"),
     "Preset %s: toward the sun (azimuth 30 deg). Reference strip spans -8..+32 deg elevation; the screenshot is pitched 22 deg up." % P,
     os.path.join(OUT, "Compare_%s_TowardSun.png" % P))
pair(ref_crop(strip, 210.0), Image.open(os.path.join(SHOT, "02_away_from_sun.png")).convert("RGB"),
     "Preset %s: away from the sun (azimuth 210 deg): far cloud bank, hills and skyline." % P,
     os.path.join(OUT, "Compare_%s_AwayFromSun.png" % P))
cards = Image.open(os.path.join(REF, "Ref_%s_CloudCards.png" % P)).convert("RGB")
pair(cards, Image.open(os.path.join(SHOT, "07_closeup_cloud.png")).convert("RGB"),
     "Preset %s: the 8 baked cumulus (photoreal, lit by the sky and sun) vs a runtime card close-up (bands 3, softness 0.12, in-band gradient 0.3)." % P,
     os.path.join(OUT, "Compare_%s_Clouds.png" % P))
