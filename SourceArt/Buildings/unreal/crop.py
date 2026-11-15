"""python -I crop.py <image> <out.jpg> x0 y0 x1 y1 [scale]: a crop of one frame (fractions of the frame), optionally scaled."""
import sys
from PIL import Image
im = Image.open(sys.argv[1]).convert("RGB")
x0, y0, x1, y1 = [float(v) for v in sys.argv[3:7]]
sc = float(sys.argv[7]) if len(sys.argv) > 7 else 1.0
c = im.crop((int(x0 * im.width), int(y0 * im.height), int(x1 * im.width), int(y1 * im.height)))
if sc != 1.0:
    c = c.resize((int(c.width * sc), int(c.height * sc)), Image.LANCZOS)
c.save(sys.argv[2], quality=90)
print(c.size)
