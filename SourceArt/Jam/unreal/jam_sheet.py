"""python -I jam_sheet.py <out.jpg> <tile_w> <cols> "label=path" ...   A contact sheet: labelled tiles, row by row.
A path may carry a crop as path@x0,y0,x1,y1 (fractions of the frame). A label starting with ! is drawn in red."""
import sys
from PIL import Image, ImageDraw, ImageFont

out, tw, cols = sys.argv[1], int(sys.argv[2]), int(sys.argv[3])
tiles = []
for spec in sys.argv[4:]:
    label, _, path = spec.partition("=")
    path, _, crop = path.partition("@")
    im = Image.open(path).convert("RGB")
    if crop:
        b = [float(v) for v in crop.split(",")]
        im = im.crop((int(b[0] * im.width), int(b[1] * im.height), int(b[2] * im.width), int(b[3] * im.height)))
    tiles.append((label, im.resize((tw, tw * im.height // im.width), Image.LANCZOS)))
try:
    font = ImageFont.truetype("arial.ttf", 17)
except Exception:
    font = ImageFont.load_default()
gap, cap = 6, 26
rows = [tiles[i:i + cols] for i in range(0, len(tiles), cols)]
H = sum(max(t[1].height for t in r) + cap for r in rows) + gap * (len(rows) - 1)
s = Image.new("RGB", (tw * cols + gap * (cols - 1), H), (18, 12, 30))
d = ImageDraw.Draw(s)
y = 0
for r in rows:
    h = max(t[1].height for t in r)
    for i, (label, im) in enumerate(r):
        x = i * (tw + gap)
        red = label.startswith("!")
        d.text((x + 6, y + 4), label.lstrip("!"), fill=(255, 120, 150) if red else (235, 235, 245), font=font)
        s.paste(im, (x, y + cap))
    y += h + cap + gap
s.save(out, quality=88)
print(out, s.size)
