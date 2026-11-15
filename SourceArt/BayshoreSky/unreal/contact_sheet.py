"""python -I contact_sheet.py <out.png> <img1> <img2> ... : 2-column sheet of 960x540 tiles."""
import sys
from PIL import Image
out, files = sys.argv[1], sys.argv[2:]
ims = [Image.open(f).convert("RGB").resize((960, 540)) for f in files]
rows = (len(ims) + 1) // 2
s = Image.new("RGB", (1920, 540 * rows))
for i, im in enumerate(ims):
    s.paste(im, ((i % 2) * 960, (i // 2) * 540))
s.save(out)
