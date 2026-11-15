"""python -I sheet.py <folder> <out.jpg> [cols] [tile_w] [name filters...]: labelled contact sheet of a shots folder."""
import sys, os
from PIL import Image, ImageDraw
folder, out = sys.argv[1], sys.argv[2]
cols = int(sys.argv[3]) if len(sys.argv) > 3 else 3
tw = int(sys.argv[4]) if len(sys.argv) > 4 else 640
filt = sys.argv[5:]
files = sorted(f for f in os.listdir(folder) if f.lower().endswith(".png") and (not filt or any(x in f for x in filt)))
th = tw * 9 // 16
rows = (len(files) + cols - 1) // cols
s = Image.new("RGB", (tw * cols, th * rows))
d = ImageDraw.Draw(s)
for i, f in enumerate(files):
    im = Image.open(os.path.join(folder, f)).convert("RGB")
    ex = im.getextrema()
    im = im.resize((tw, th), Image.LANCZOS)
    x, y = (i % cols) * tw, (i // cols) * th
    s.paste(im, (x, y))
    d.rectangle([x, y, x + 8 * len(f) + 8, y + 16], fill=(0, 0, 0))
    d.text((x + 4, y + 2), f[:-4], fill=(255, 255, 255))
    print(f, ex)
s.save(out, quality=88)
