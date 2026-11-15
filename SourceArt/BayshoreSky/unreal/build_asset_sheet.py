"""Level 1 asset sheet, part 2 (system Python + Pillow): HTML with embedded thumbnails, and a CSV of the same rows.
python -I build_asset_sheet.py   (reads K:\\Full Throttle\\Level1_Asset_Sheet\\_work from 85_asset_sheet.py)
"""
import os
import io
import csv
import json
import base64
import html
from PIL import Image, ImageDraw, ImageFont

ROOT = r"K:\Full Throttle\Level1_Asset_Sheet"
WORK = os.path.join(ROOT, "_work")
rows = json.load(open(os.path.join(WORK, "assets.json"), encoding="utf-8"))
font = ImageFont.load_default()
COLOURS = {"StaticMesh": (64, 96, 160), "Material": (150, 70, 120), "MaterialInstanceConstant": (170, 90, 140),
           "Texture2D": (60, 130, 100), "SoundWave": (180, 120, 40), "NiagaraSystem": (200, 90, 40), "Blueprint": (60, 60, 160),
           "World": (40, 110, 140)}


def tile(cls):
    im = Image.new("RGB", (96, 96), COLOURS.get(cls, (90, 90, 100)))
    d = ImageDraw.Draw(im)
    t = cls[:14]
    d.text((6, 42), t, fill=(255, 255, 255), font=font)
    return im


def thumb_b64(r):
    p = os.path.join(WORK, "thumbs", r["path"].strip("/").replace("/", "__") + ".png")
    try:
        im = Image.open(p).convert("RGB") if os.path.exists(p) else tile(r["type"])
    except Exception:
        im = tile(r["type"])
    im.thumbnail((96, 96))
    buf = io.BytesIO()
    im.save(buf, "PNG")
    r["thumb"] = "thumbs/" + os.path.basename(p) if os.path.exists(p) else "(class tile)"
    return base64.b64encode(buf.getvalue()).decode("ascii")


rows.sort(key=lambda r: (r["source"], r["type"], r["path"]))
total = sum(r["bytes"] or 0 for r in rows)
by_source = {}
for r in rows:
    s = by_source.setdefault(r["source"], [0, 0])
    s[0] += 1
    s[1] += r["bytes"] or 0
cells = []
for r in rows:
    b64 = thumb_b64(r)
    size = "%.2f MB" % (r["bytes"] / 1e6) if r["bytes"] else "engine"
    cells.append("<tr><td><img src='data:image/png;base64,%s' width=96 height=96 alt=''></td><td>%s</td><td><code>%s</code></td>"
                 "<td>%s</td><td class=n>%s</td><td>%s</td><td>%s<br><small>%s</small></td><td>%s</td><td>%s</td></tr>" % (
                     b64, html.escape(r["name"]), html.escape(r["path"]), html.escape(r["type"]), size, html.escape(r["detail"]),
                     html.escape(r["why"]), html.escape(r.get("used_by", "")), html.escape(r["source"]), html.escape(r["licence"])))
summary = "".join("<li>%s: %d assets, %.1f MB</li>" % (html.escape(k), v[0], v[1] / 1e6) for k, v in sorted(by_source.items()))
page = """<!doctype html><html lang=en><head><meta charset=utf-8><meta name=viewport content="width=device-width, initial-scale=1">
<title>Level 1 Asset Sheet</title><style>
:root{--bg:#fbfaf7;--fg:#1c1b20;--mut:#6a6873;--line:#e3e0da;--head:#f1eee8}
@media (prefers-color-scheme:dark){:root{--bg:#16151a;--fg:#ecebef;--mut:#9a98a3;--line:#2c2a31;--head:#1f1d24}}
body{margin:0;padding:24px 16px;background:var(--bg);color:var(--fg);font:14px/1.45 system-ui,sans-serif}
h1{font-size:22px;margin:0 0 4px}p,li{color:var(--mut)}table{border-collapse:collapse;width:100%%;font-size:12.5px}
th,td{border-bottom:1px solid var(--line);padding:6px 8px;text-align:left;vertical-align:top}th{background:var(--head);position:sticky;top:0}
td.n{white-space:nowrap;text-align:right}code{font-size:11.5px;word-break:break-all}img{display:block;border-radius:4px}
.wrap{overflow-x:auto}</style></head><body>
<h1>Full Throttle, Level 1 (Bayshore): asset sheet</h1>
<p>Every asset Level 1 uses: the map, its level folder, the game mode, HUD and player car, the assets the C++ loads by path,
and all their dependencies (asset registry, hard and soft references). %d assets, %.1f MB on disk. Branch <code>level1-sky</code>, with the BayshoreSky sky.</p>
<ul>%s</ul>
<p>"Level 1 scenes": Level 1 is one map and one continuous race, so the column says why an asset is in it (the map itself, the level folder,
the game mode, or C++ that loads it in every level) and which assets reference it. Thumbnails: textures as exported, meshes and materials
rendered on a plain stage, other types as class tiles.</p>
<div class=wrap><table><thead><tr><th>Thumb</th><th>Name</th><th>Path</th><th>Type</th><th>On disk</th><th>Triangles / resolution</th>
<th>Why in Level 1 / used by</th><th>Source</th><th>Licence</th></tr></thead><tbody>%s</tbody></table></div></body></html>""" % (
    len(rows), total / 1e6, summary, "".join(cells))
open(os.path.join(ROOT, "Level1_Asset_Sheet.html"), "w", encoding="utf-8").write(page)
with open(os.path.join(ROOT, "Level1_Asset_Sheet.csv"), "w", newline="", encoding="utf-8") as fh:
    w = csv.writer(fh)
    w.writerow(["name", "path", "type", "bytes_on_disk", "triangles_or_resolution", "why_in_level1", "used_by", "source", "licence", "thumbnail"])
    for r in rows:
        w.writerow([r["name"], r["path"], r["type"], r["bytes"] if r["bytes"] is not None else "", r["detail"], r["why"],
                    r.get("used_by", ""), r["source"], r["licence"], r["thumb"]])
print("sheet:", len(rows), "assets, %.1f MB" % (total / 1e6))
