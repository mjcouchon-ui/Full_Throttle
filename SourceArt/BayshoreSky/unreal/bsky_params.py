"""Live parameters of MPC_BayshoreSky and the Day preset defaults (shared by the build scripts and preview_deck.py)."""
import math


def srgb(hexstr):
    h = hexstr.lstrip("#")
    out = []
    for i in (0, 2, 4):
        c = int(h[i:i + 2], 16) / 255.0
        out.append(c / 12.92 if c <= 0.04045 else ((c + 0.055) / 1.055) ** 2.4)
    return out


# Look (owner, 14 Nov): hyper-real, no cel look on the sky (BandGradient 1 = no bands, OutlineWidth 0 = no ink), alive while
# driving. Target: the Brickell golden-hour shot (HandBack/refs/R7_brickell_target.webp): muted blue zenith, lavender mid
# sky, an orange-to-pink glow on the sun side only, mauve-pink cloud lit orange toward the sun, pink cirrus streaks and
# cumulus clusters over about 30% of the sky. Sun at golden hour, 6 deg, over the bay (owner's choice).
SUN_AZ, SUN_EL = 30.0, 6.0
SCALARS = [
    ("BandCount", 3.0), ("BandSoftness", 0.4), ("BandGradient", 1.0), ("CloudDriftSpeed", 600.0),
    ("OutlineWidth", 0.0), ("Haze", 0.3), ("CloudCardsShown", 48.0), ("SunDiscSize", 0.9),
    ("SunHaloSize", 22.0), ("SunRayStrength", 0.0), ("FlareStrength", 0.2), ("GradientMidPoint", 0.12),
    ("PanoramaExposure", 0.7), ("PanoramaStylize", 0.9), ("CloudRimStrength", 0.8), ("SeaGlitterStrength", 1.0),
    ("LowQuality", 0.0), ("CloudEdgeSoftness", 3.0), ("SkyBrightness", 0.9), ("BlimpSpeed", 0.012),
    ("BlimpRadius", 250000.0), ("BlimpOutlineWidth", 0.0), ("HazeDistance", 2500000.0),
    ("CloudAmbientGradient", 0.7), ("SunDistance", 4000000.0), ("GullFlapSpeed", 7.0), ("SunIntensity", 20.0), ("SeaEnabled", 1.0),
    # photoreal clouds and HDR (MPC only, see MPC_ONLY): exposure of the Cycles-baked deck, cirrus and cards (each its
    # own, layers_meta.json / cumulus3_meta.json recommendations), the optical depth scale of the phase-function blend,
    # the cumulus sky-light normalisation, and the share of the sky the deck and cirrus fields cover (1 = everywhere)
    # round 2 bakes (14 Nov, 17:14): both layers share one physical scale (Exposure 0.45) and carry their clear sky
    # themselves, so the runtime cloud fields are off (1); cards per cumulus3_meta.json (1.058)
    ("CloudExposure", 0.45), ("CirrusExposure", 0.45), ("CardExposure", 1.058), ("CloudTauScale", 1.0), ("CumulusSkyNorm", 1.0),
    ("AltoCoverage", 1.0), ("CirrusCoverage", 1.0),
    # the baked hills ring drawn at this share of its height: Miami is flat, R7 shows a thin far shore across the bay
    ("HillsHeight", 0.35),
]
COLORS = {
    # gradient measured on R7 (14 Nov, away from the sun): ~#8C8CA9-#B0909D at the horizon, #6E87B3 ~10 deg up,
    # #4E72A8 ~20 deg up; the chase camera sees 0-25 deg, so the blue arrives low (GradientMidPoint 0.12 = 11 deg)
    "ZenithColor": "#44659F", "MidColor": "#6E87B6", "HorizonColor": "#9A96B4", "HazeColor": "#DDA0A6",
    # CloudRimColor is the sun's tint on every photoreal cloud (layers and cards): salmon, as R7's lit cumulus are
    # (linear G/R 0.47, B/R 0.59; the layers review found #FFB88C could not reach it)
    "SunColor": "#FFC98A", "SunHaloColor": "#FF7E4C", "CloudLitColor": "#F09DB0", "CloudShadowColor": "#8784BA",
    "CloudRimColor": "#FFA28E", "OutlineColor": "#6A4E7C", "HillNearColor": "#5F6280", "HillFarColor": "#9C90AE",
    "SkylineColor": "#E7CFC6", "SeaDeepColor": "#1B2D3A", "SeaHorizonColor": "#4A5566", "GlitterColor": "#FFD7A8",
    "BlimpBodyColor": "#F2EFF2", "BlimpAccentColor": "#E8456B", "BlimpLetterColor": "#1E3F8A", "GullColor": "#FBF6F2",
}
_a, _e = math.radians(SUN_AZ), math.radians(SUN_EL)
VECTORS = [("SunDirection", [math.cos(_e) * math.cos(_a), math.cos(_e) * math.sin(_a), math.sin(_e), 0.0]),
           ("WindDirection", [1.0, 0.3, 0.0, 0.0])]
VECTORS += [(k, srgb(v) + [1.0]) for k, v in COLORS.items()]

# preset values that are not MPC parameters (lights). Plain values here (no unreal types) so the preview can import this;
# 45_preset_values.py converts them. Golden hour: a warm, lower sun light pointing along -SunDirection.
PRESET_EXTRA_VALUES = {
    "SunLightIntensity": 7.0, "SkyLightIntensity": 1.2,
    "SunLightRotation": ("rotator", 0.0, -SUN_EL, SUN_AZ + 180.0),      # roll, pitch, yaw
    "SunLightColor": ("color", 1.0, 0.74, 0.52),
}

# scalars written by the Blueprint itself (options and scalability), and MPC-only values, not stored in presets: adding an
# MPC-only scalar needs 30_materials.py (which builds the MPC) but no Blueprint or preset rebuild
MPC_ONLY = ("CloudExposure", "CirrusExposure", "CardExposure", "CloudTauScale", "CumulusSkyNorm", "AltoCoverage", "CirrusCoverage",
            "HillsHeight")
NOT_IN_PRESET = ("SeaEnabled", "LowQuality") + MPC_ONLY
