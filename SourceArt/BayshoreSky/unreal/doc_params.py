"""Writes the parameter table of Plugins/BayshoreSky/INTEGRATION.md from bsky_params.py, so the doc never drifts from
the values the build writes (review finding: the table still showed the old cel-look values).
python -I doc_params.py <INTEGRATION.md>   (replaces the line PARAM_TABLE, or the table under '## 6.' on later runs)"""
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from bsky_params import SCALARS, COLORS, MPC_ONLY, NOT_IN_PRESET, SUN_AZ, SUN_EL, PRESET_EXTRA_VALUES  # noqa: E402

DESC = {
    "CloudExposure": "exposure of the photoreal altocumulus deck (HDR)",
    "CirrusExposure": "exposure of the photoreal cirrus layer (HDR)",
    "AltoCoverage": "share of the sky the deck's cloud fields cover (clear blue between them; 1 = the bake everywhere)",
    "CirrusCoverage": "share of the sky the cirrus bundles cover (1 = the bake everywhere)",
    "HillsHeight": "the distant hills drawn at this share of their baked height (a flat bay shore, as in the reference)",
    "CardExposure": "exposure of the cumulus cards (HDR)",
    "CloudTauScale": "optical depth at which a layer stops following the phase function (thin cloud glows toward the sun, thick cloud scatters evenly)",
    "CumulusSkyNorm": "extra gain on the cumulus sky light (1: the bake's sun/sky balance as stored, cumulus3_meta.json)",
    "CloudDriftSpeed": "cloud drift, cm/s, along WindDirection (cirrus 2.2x)",
    "CloudCardsShown": "how many cumulus cards are drawn (dealt round-robin across the clusters)",
    "Haze": "horizon haze; distant cloud fades toward the sky behind it at twice this",
    "HazeDistance": "distance over which clouds, blimp and gulls fade into the haze (cm)",
    "SkyBrightness": "overall sky emissive multiplier (tuned in game against Level 1's manual exposure, bias 0)",
    "PanoramaExposure": "brightness of the baked atmosphere strip",
    "PanoramaStylize": "how far the atmosphere strip is pulled toward the art-directed gradient (1 = gradient only)",
    "GradientMidPoint": "elevation (0-1 of 90 deg) of the mid colour; the zenith colour is reached at 3x it",
    "SunDiscSize": "angular radius of the sun disc (deg)",
    "SunHaloSize": "angular size of the halo (deg)",
    "SunIntensity": "sun disc brightness (HDR; about 20x SDR white)",
    "SunRayStrength": "painted rays in the halo (0: the light shafts do this now)",
    "FlareStrength": "lens ghosts",
    "SeaGlitterStrength": "glitter path and glints on the backdrop sea",
    "SeaEnabled": "the backdrop sea on or off (written by ShowSea)",
    "LowQuality": "set to 1 by the scalability hook",
    "BlimpSpeed": "blimp angular speed (rad/s)",
    "BlimpRadius": "blimp path radius (cm)",
    "GullFlapSpeed": "gull wing beat",
    "SunDistance": "distance of the halo and ghost quads (cm)",
    "BandCount": "semi-cel bands (inactive: BandGradient 1)",
    "BandSoftness": "semi-cel band softness (inactive)",
    "BandGradient": "1 = no bands: the sky is not cel shaded",
    "OutlineWidth": "ink outline width (0: no ink on the sky)",
    "BlimpOutlineWidth": "blimp ink hull width (0; the hull is retired)",
    "CloudEdgeSoftness": "soft edge of the hills, in pixels",
    "CloudRimStrength": "(legacy) rim strength of the old procedural deck",
    "CloudAmbientGradient": "(legacy) ambient gradient of the old cel cards",
}

rows = ["| Parameter | Value | What it does |", "|---|---|---|"]
for name, val in SCALARS:
    tag = " (MPC-only)" if name in MPC_ONLY else (" (set by the Blueprint)" if name in NOT_IN_PRESET else "")
    rows.append("| `%s`%s | %s | %s |" % (name, tag, ("%g" % val), DESC.get(name, "")))
rows.append("| Colours | | " + ", ".join("`%s` %s" % (k, v) for k, v in COLORS.items()) + " |")
rows.append("| `SunDirection` | azimuth %g deg, elevation %g deg | must match the bakes |" % (SUN_AZ, SUN_EL))
rows.append("| `WindDirection` | (1, 0.3) | the deck drifts along it; the cirrus 15 deg off it |")
x = PRESET_EXTRA_VALUES
rows.append("| Lights (preset) | sun %g lux, colour %s, rotation pitch %g yaw %g; sky light %g | |" % (
    x["SunLightIntensity"], tuple(x["SunLightColor"][1:]), x["SunLightRotation"][2], x["SunLightRotation"][3], x["SkyLightIntensity"]))
table = "\n".join(rows)

path = sys.argv[1]
doc = open(path, encoding="utf-8").read()
if "PARAM_TABLE" in doc:
    doc = doc.replace("PARAM_TABLE", table)
else:
    doc = re.sub(r"\| Parameter \| Value \| What it does \|\n(\|.*\|\n)+", table + "\n", doc)
open(path, "w", encoding="utf-8", newline="\n").write(doc)
print("parameter table:", len(rows) - 2, "rows")
