"""LI_BayshoreSky: a level that holds only BP_BayshoreSky, for adding the sky to Level 1 as a Level Instance
(one small change to Matt's map). Also checks that a Level Instance actor loads it, in a throwaway map that is not saved.
Run: ue_run.ps1 -Editor -Script 52_level_instance.py
"""
import sys
import os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)) if "__file__" in globals() else r"K:\Full Throttle\SkyScratch\SourceArt\BayshoreSky\unreal")
import unreal
from bsky_ue import log, EAL

LI = "/BayshoreSky/Maps/LI_BayshoreSky"
les = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
sky_cls = unreal.load_asset("/BayshoreSky/Blueprints/BP_BayshoreSky").generated_class()
if not EAL.does_asset_exist(LI):   # to rebuild it, delete Content/Maps/LI_BayshoreSky.umap first (deleted packages stay in memory)
    if not les.new_level(LI):
        raise RuntimeError("new_level failed")
    a = eas.spawn_actor_from_class(sky_cls, unreal.Vector(0, 0, 0), unreal.Rotator(0, 0, 0))
    a.set_actor_label("BayshoreSky")
    if not les.save_current_level():
        raise RuntimeError("save failed")
log("level instance source saved", LI)

# check: a throwaway map with a Level Instance actor pointing at it
world = unreal.EditorLoadingAndSavingUtils.new_blank_map(False)   # unsaved, empty
before = [x for x in eas.get_all_level_actors() if x.get_class() == sky_cls]
log("blank map sky actors before:", len(before))
li = eas.spawn_actor_from_class(unreal.LevelInstance, unreal.Vector(0, 0, 0), unreal.Rotator(0, 0, 0))
ok = li.set_world_asset(unreal.load_asset(LI))
li.load_level_instance()
skies = [x for x in eas.get_all_level_actors() if x.get_class() == sky_cls]
log("LI check: set_world_asset", ok, "is_loaded", li.is_loaded(), "loaded level", li.get_loaded_level(),
    "-> sky actors in the editor world:", len(skies))

# loading is asynchronous: tick the editor a little, then count again and quit
state = {"n": 0, "h": None}


def _tick(dt):
    state["n"] += 1
    if state["n"] < 120 and not li.is_loaded():
        return
    n = len(unreal.GameplayStatics.get_all_actors_of_class(unreal.EditorLevelLibrary.get_editor_world(), sky_cls))
    log("LI check after %d ticks: is_loaded %s, sky actors in the world %d" % (state["n"], li.is_loaded(), n))
    unreal.unregister_slate_post_tick_callback(state["h"])
    unreal.SystemLibrary.quit_editor()


unreal.EditorPythonScripting.set_keep_python_script_alive(True)
state["h"] = unreal.register_slate_post_tick_callback(_tick)
