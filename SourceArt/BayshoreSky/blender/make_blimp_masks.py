"""Blimp mask texture T_BSky_Blimp_Masks (1024 x 512), system Python + Pillow.
R lettering (BAYSHORE, Bungee, SIL OFL), G accent (stripes, fins, gondola), B soft occlusion, A 1.
UV (Unreal): u = 0.9 * along the hull, tail 0 -> nose 0.9; v = theta / 360 with theta 0 on top, 90 on Blender +Y side.
Strip u 0.93..0.99 is the fins (v 0.55..0.95) and gondola (v 0.05..0.45) in Unreal v.
Run: python -I make_blimp_masks.py
"""
from PIL import Image, ImageDraw, ImageFont, ImageFilter, ImageOps
import math, os
ROOT = r"K:\Full Throttle\SkyScratch\SourceArt\BayshoreSky"
W, H = 2048, 1024
L, R, HULL_U = 60.0, 7.6, 0.9
font = ImageFont.truetype(os.path.join(ROOT, "fonts", "Bungee", "Bungee-Regular.ttf"), 400)
text = "BAYSHORE"
bb = font.getbbox(text)
tw, th = bb[2] - bb[0], bb[3] - bb[1]
glyph = Image.new("L", (tw + 40, th + 40), 0)
ImageDraw.Draw(glyph).text((20 - bb[0], 20 - bb[1]), text, font=font, fill=255)
glyph = glyph.crop(glyph.getbbox())
aspect = glyph.width / glyph.height
# physical size on the hull: letters 5.6 m tall, at most 36 m long
h_m = 6.4
w_m = min(36.0, h_m * aspect)
h_m = w_m / aspect
m_per_px_u = L / (W / HULL_U)                 # metres per pixel along the hull
m_per_px_v = 2 * math.pi * R / H             # metres per pixel around the hull (at max radius)
w_px, h_px = int(round(w_m / m_per_px_u)), int(round(h_m / m_per_px_v))
g = glyph.resize((w_px, h_px), Image.LANCZOS)
centre_u = 0.47 * W                           # a little ahead of mid-hull (hull centre is u 0.45)
R_ch = Image.new("L", (W, H), 0)
# side A, theta 90 (y = H/4): reads nose->tail on screen, so mirror horizontally
ga = ImageOps.mirror(g)
R_ch.paste(ga, (int(centre_u - w_px / 2), int(H * 0.25 - h_px / 2)))
# side B, theta 270 (y = 3H/4): text up is +v, so mirror vertically
gb = ImageOps.flip(g)
R_ch.paste(gb, (int(centre_u - w_px / 2), int(H * 0.75 - h_px / 2)))
R_ch = R_ch.filter(ImageFilter.GaussianBlur(1.2))   # soft edge field: the material thresholds it at 0.5 with fwidth
# accent: two stripes along the lower flanks, the nose cap, fins and gondola
G_ch = Image.new("L", (W, H), 0)
d = ImageDraw.Draw(G_ch)
for th0, th1 in ((118, 127), (233, 242)):
    d.rectangle([0, int(th0 / 360 * H), int(HULL_U * W), int(th1 / 360 * H)], fill=255)
d.rectangle([int(0.885 * W), 0, int(HULL_U * W) + 2, H], fill=255)            # nose cap
d.rectangle([int(0.93 * W), int(0.05 * H), int(0.99 * W), int(0.95 * H)], fill=255)  # fins + gondola
G_ch = G_ch.filter(ImageFilter.GaussianBlur(1.2))
# occlusion: darker belly and toward the tail
B_ch = Image.new("L", (W, H), 0)
px = B_ch.load()
for y in range(H):
    th = (y + 0.5) / H * 360.0
    belly = max(0.0, -math.cos(math.radians(th)))          # 1 at the bottom
    for x in range(W):
        u = (x + 0.5) / W
        tail = max(0.0, 1.0 - u / 0.15) if u < HULL_U else 0.0
        px[x, y] = int(255 * max(0.0, 1.0 - 0.35 * belly ** 1.5 - 0.2 * tail))
A_ch = Image.new("L", (W, H), 255)
out = Image.merge("RGBA", (R_ch, G_ch, B_ch, A_ch))
dst = os.path.join(ROOT, "textures_src", "T_BSky_Blimp_Masks.png")
out.save(dst)
print("BSKYT wrote", dst, "letters px", w_px, h_px, "metres", round(w_m, 1), round(h_m, 2))
