"""Import the sky meshes and the blimp mask texture into /BayshoreSky."""
import sys
import os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)) if "__file__" in globals() else r"K:\Full Throttle\SkyScratch\SourceArt\BayshoreSky\unreal")
import unreal
from bsky_ue import ROOT, log, import_mesh, import_texture, save

MESH_DIR = os.path.join(ROOT, "meshes")
FOLDER = "/BayshoreSky/Meshes"
for n in ["SM_BSky_Dome", "SM_BSky_HorizonRing", "SM_BSky_CloudCard",
          "SM_BSky_SunQuad", "SM_BSky_FlareGhosts", "SM_BSky_BlimpOutline", "SM_BSky_Gull"]:
    import_mesh(os.path.join(MESH_DIR, n + ".fbx"), FOLDER, n)

blimp = import_mesh(os.path.join(MESH_DIR, "SM_BSky_Blimp_LOD0.fbx"), FOLDER, "SM_BSky_Blimp")
sms = unreal.get_editor_subsystem(unreal.StaticMeshEditorSubsystem)
for lod in (1, 2):
    r = sms.import_lod(blimp, lod, os.path.join(MESH_DIR, "SM_BSky_Blimp_LOD%d.fbx" % lod))
    log("blimp import_lod", lod, "->", r)
log("blimp lod count", sms.get_lod_count(blimp))
if sms.get_lod_count(blimp) >= 3:
    sms.set_lod_screen_sizes(blimp, [1.0, 0.12, 0.04])
save(blimp)
for i in range(sms.get_lod_count(blimp)):
    log("blimp LOD", i, "verts", sms.get_number_verts(blimp, i))

import_texture(os.path.join(ROOT, "textures_src", "T_BSky_Blimp_Masks.png"), "/BayshoreSky/Textures", "T_BSky_Blimp_Masks",
               "TC_BC7", srgb=False, mips=True, address_x="TA_Clamp", address_y="TA_Wrap", lod_group="TEXTUREGROUP_World", never_stream=True)
log("done")
