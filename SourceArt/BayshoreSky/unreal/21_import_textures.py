"""Import every preset's baked textures from SourceArt/BayshoreSky/bakes/<Preset>/ into /BayshoreSky/Textures/<Preset>.
Radiance goes in as HDR (BC6H, no sRGB): the atmosphere strip, the sky light and the photoreal cloud light maps. Opacity
maps are single channel BC4 (TC_Alpha, read from source R). Until a Cycles bake lands, its stand-in from
bakes/<Preset>/_placeholder (blender/make_placeholders.py) is imported in its place, and the log says so."""
import sys
import os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)) if "__file__" in globals() else r"K:\Full Throttle\SkyScratch\SourceArt\BayshoreSky\unreal")
import unreal
from bsky_ue import ROOT, log, import_texture

PRESETS = [p for p in os.environ.get("BSKY_PRESETS", "Day").split(",") if p]


def baked(src, fname):
    real = os.path.join(src, fname)
    if os.path.exists(real):
        return real
    stand_in = os.path.join(src, "_placeholder", fname)
    if os.path.exists(stand_in):
        log("STAND-IN used for", fname)
        return stand_in
    raise RuntimeError("missing bake: " + real)


for P in PRESETS:
    src = os.path.join(ROOT, "bakes", P)
    folder = "/BayshoreSky/Textures/" + P
    T = "T_BSky_%s_" % P
    # sky base: the baked atmosphere strip (-8 to +32 deg) and the sky light cubemap, HDR
    import_texture(os.path.join(src, T + "HorizonAtmo.exr"), folder, T + "HorizonAtmo",
                   "TC_HDR_Compressed", srgb=False, mips=False, address_x="TA_Wrap", address_y="TA_Clamp",
                   lod_group="TEXTUREGROUP_Skybox", never_stream=True, filt="TF_Bilinear")
    import_texture(os.path.join(src, T + "SkyLight.hdr"), folder, T + "SkyLight",
                   "TC_HDR_Compressed", srgb=False, mips=True, lod_group="TEXTUREGROUP_Skybox", cube=True)
    import_texture(os.path.join(src, T + "Horizon.png"), folder, T + "Horizon",
                   "TC_BC7", srgb=False, mips=True, address_x="TA_Wrap", address_y="TA_Clamp",
                   lod_group="TEXTUREGROUP_Skybox", never_stream=True)
    # photoreal cloud layers (blender/bake_layers.py): tile both ways, mips for distance, World group for anisotropy
    for layer in ("Alto", "Cirrus"):
        import_texture(baked(src, T + layer + "Light.exr"), folder, T + layer + "Light", "TC_HDR_Compressed", srgb=False,
                       mips=True, address_x="TA_Wrap", address_y="TA_Wrap", lod_group="TEXTUREGROUP_World", never_stream=True)
        import_texture(baked(src, T + layer + "Alpha.png"), folder, T + layer + "Alpha", "TC_Alpha", srgb=False,
                       mips=True, address_x="TA_Wrap", address_y="TA_Wrap", lod_group="TEXTUREGROUP_World", never_stream=True)
    # photoreal cumulus cards lit from three sun directions (bake_sky.py stage cumulus3)
    import_texture(baked(src, T + "CumulusSun.exr"), folder, T + "CumulusSun", "TC_HDR_Compressed", srgb=False, mips=True,
                   address_x="TA_Clamp", address_y="TA_Clamp", lod_group="TEXTUREGROUP_Skybox", never_stream=True)
    import_texture(baked(src, T + "CumulusAux.png"), folder, T + "CumulusAux", "TC_BC7", srgb=False, mips=True,
                   address_x="TA_Clamp", address_y="TA_Clamp", lod_group="TEXTUREGROUP_Skybox", never_stream=True)
log("done")
