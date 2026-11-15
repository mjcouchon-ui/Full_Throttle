"""Writes the cumulus card layout (bsky_layout.py) onto the placed BP_BayshoreSky actors, whose ISM instances are saved per
actor and do not follow the Blueprint template: LI_BayshoreSky (the Level Instance the game uses) and, when present, the
test map. Run in each project's editor after 41_bp_tweaks.py: ue_run.ps1 -Editor [-Project ...] -Script 82_place_cumulus.py"""
import sys
import os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)) if "__file__" in globals() else r"K:\Full Throttle\SkyScratch\SourceArt\BayshoreSky\unreal")
import unreal
import bsky_layout

LES = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
EAS = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)


def log(*a):
    print("BSKY", *[str(x) for x in a], flush=True)


for m in ("/BayshoreSky/Maps/LI_BayshoreSky", "/BayshoreSky/Maps/L_BayshoreSkyTest"):
    if not unreal.EditorAssetLibrary.does_asset_exist(m):
        continue
    LES.load_level(m)
    n = 0
    for a in EAS.get_all_level_actors():
        if a.get_class().get_name().startswith("BP_BayshoreSky"):
            for c in a.get_components_by_class(unreal.InstancedStaticMeshComponent):
                if c.get_name().startswith("Clouds"):
                    a.modify()
                    c.modify()
                    log(m, a.get_actor_label(), "cards", bsky_layout.apply(c, unreal))
                    n += 1
    if n:
        LES.save_current_level()
        log("saved", m)
