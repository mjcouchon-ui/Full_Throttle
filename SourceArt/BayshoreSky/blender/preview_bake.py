"""Contact sheet of a preset's bake for humans: python -I preview_bake.py <Preset> <out.png>"""
import sys, os
from PIL import Image
ROOT = r"K:\Full Throttle\SkyScratch\SourceArt\BayshoreSky"
P = sys.argv[1]; out = sys.argv[2]
B = os.path.join(ROOT, "bakes", P); R = os.path.join(ROOT, "reference", P)
ch = lambda im, i: im.getchannel(i).convert("RGB")
atlas = Image.open(os.path.join(B, "T_BSky_%s_CloudAtlas.png" % P)); hc = Image.open(os.path.join(B, "T_BSky_%s_HorizonClouds.png" % P))
hz = Image.open(os.path.join(B, "T_BSky_%s_Horizon.png" % P))
ref = Image.open(os.path.join(R, "Ref_%s_PhotorealStrip.png" % P)).convert("RGB")
cards = Image.open(os.path.join(R, "Ref_%s_CloudCards.png" % P)).convert("RGB")
W = 2048
rows = [ref.resize((W, 512))]
rows += [ch(hc, i).resize((W, 160)) for i in (1, 3)]
a = Image.new("RGB", (W, 512))
for k, im in enumerate([ch(atlas, 1), ch(atlas, 2), ch(atlas, 3), cards]):
    a.paste(im.resize((512, 512)), (k * 512, 0))
rows.append(a)
rows += [ch(hz, i).resize((W, 128)) for i in (0, 3)]
sheet = Image.new("RGB", (W, sum(r.height for r in rows)))
y = 0
for r in rows:
    sheet.paste(r, (0, y)); y += r.height
sheet.save(out); print(sheet.size)
