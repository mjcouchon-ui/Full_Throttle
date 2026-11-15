"""python -I ingame_sheet.py <folder> <out.jpg> [cols] [tile_w] [every]: sheet of in-game frames (3440x1440), every Nth frame."""
import sys, os
from PIL import Image, ImageDraw
folder, out = sys.argv[1], sys.argv[2]
cols = int(sys.argv[3]) if len(sys.argv) > 3 else 2
tw = int(sys.argv[4]) if len(sys.argv) > 4 else 1147
every = int(sys.argv[5]) if len(sys.argv) > 5 else 1
files = sorted(f for f in os.listdir(folder) if f.lower().endswith(".png"))[::every]
ims = [Image.open(os.path.join(folder, f)).convert("RGB") for f in files]
th = tw * ims[0].height // ims[0].width
rows = (len(ims) + cols - 1) // cols
s = Image.new("RGB", (tw * cols, th * rows))
d = ImageDraw.Draw(s)
for i, (f, im) in enumerate(zip(files, ims)):
    x, y = (i % cols) * tw, (i // cols) * th
    s.paste(im.resize((tw, th), Image.LANCZOS), (x, y))
    d.rectangle([x, y, x + 8 * len(f) + 8, y + 16], fill=(0, 0, 0))
    d.text((x + 4, y + 2), f[:-4], fill=(255, 255, 255))
s.save(out, quality=88)
print(len(files), "frames")
