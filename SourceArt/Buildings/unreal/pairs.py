"""python -I pairs.py <before_dir> <after_dir> <out.jpg> <tile_w> name1 name2 ...: before | after rows of the same shots.
A name may carry a crop as name@x0,y0,x1,y1 (fractions of the frame), applied to both sides."""
import sys, os
from PIL import Image, ImageDraw

before, after, out, tw = sys.argv[1], sys.argv[2], sys.argv[3], int(sys.argv[4])
rows = []
for spec in sys.argv[5:]:
    name, _, crop = spec.partition("@")
    box = [float(v) for v in crop.split(",")] if crop else None
    pair = []
    for d in (before, after):
        im = Image.open(os.path.join(d, name + ".png")).convert("RGB")
        if box:
            im = im.crop((int(box[0] * im.width), int(box[1] * im.height), int(box[2] * im.width), int(box[3] * im.height)))
        pair.append(im.resize((tw, tw * im.height // im.width), Image.LANCZOS))
    rows.append((name, pair))
gap = 6
H = sum(p[0].height for _, p in rows) + gap * (len(rows) - 1)
s = Image.new("RGB", (tw * 2 + gap, H), (18, 12, 30))
d = ImageDraw.Draw(s)
y = 0
for name, (a, b) in rows:
    s.paste(a, (0, y))
    s.paste(b, (tw + gap, y))
    for x, label in ((0, "BEFORE  " + name), (tw + gap, "AFTER  " + name)):
        d.rectangle([x, y, x + 7 * len(label) + 10, y + 16], fill=(0, 0, 0))
        d.text((x + 5, y + 2), label, fill=(255, 255, 255))
    y += a.height + gap
s.save(out, quality=90)
print(s.size)
