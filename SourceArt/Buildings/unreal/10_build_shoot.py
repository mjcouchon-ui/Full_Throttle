"""One look-dev iteration in the port's editor: rebuild the buildings' materials and Bayshore's prop meshes from the
port's own scripts (ft_buildings, ft_export), then film the hero hotels in Level 1 (bld_shots).

Environment: BLD_BUILD=0 films only (the "before" frames); BLD_TAG names the output folder; BLD_SHOTS=a,b limits the
shots to names containing a or b; BLD_EVENING=1 films with the evening's share of rooms lit.
Run: bld_run.ps1 -Script 10_build_shoot.py
"""
import os
import sys
import unreal

HERE = os.path.dirname(os.path.abspath(__file__)) if "__file__" in globals() else r"K:\Full Throttle\SkyScratch\SourceArt\Buildings\unreal"
sys.path.insert(0, HERE)
import bld_shots
from bld_shots import log

AL = unreal.EditorAssetLibrary
MEL = unreal.MaterialEditingLibrary
LES = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
BUILD = os.environ.get("BLD_BUILD", "1") == "1"
TAG = os.environ.get("BLD_TAG", "wip")
ONLY = [s for s in os.environ.get("BLD_SHOTS", "").split(",") if s]
EVENING = os.environ.get("BLD_EVENING", "0") == "1"

try:
    if BUILD:
        try:
            LES.load_level("/Engine/Maps/Entry")    # no level holds the meshes while they are deleted and rebuilt
        except Exception as e:
            log("could not leave the level first:", e)
        import ft_buildings
        import ft_export
        log(ft_buildings.build_materials())
        log(ft_export.run(["bayshore"]))
        for p in sorted(AL.list_assets(bld_shots.PROPS, recursive=False)):
            sm = AL.load_asset(p.split(".")[0])
            if isinstance(sm, unreal.StaticMesh):
                try:
                    tris = sm.get_num_triangles(0)
                except Exception:
                    tris = "?"
                try:
                    uvs = sm.get_num_uv_channels(0)
                except Exception:
                    uvs = "?"
                log("mesh", sm.get_name(), "nanite", sm.get_editor_property("nanite_settings").get_editor_property("enabled"), "tris", tris,
                    "uv", uvs, "slots", [str(s.get_editor_property("material_slot_name")) for s in sm.get_editor_property("static_materials")])
    LES.load_level(bld_shots.MAP)
    if EVENING:
        mi = AL.load_asset("/Game/Maps/Bayshore/Materials/MI_BayGlass")
        if mi:
            MEL.set_material_instance_scalar_parameter_value(mi, "LitStart", 0.72)     # not saved: for these frames only
            MEL.update_material_instance(mi)
    shots = bld_shots.shot_list(ONLY)
    log("shots", [s[0] for s in shots])
    bld_shots.Shooter(TAG, shots, warm=240 if BUILD else 90).start()
except Exception as e:  # noqa
    import traceback
    log("ERR", repr(e), traceback.format_exc())
    unreal.SystemLibrary.quit_editor()
