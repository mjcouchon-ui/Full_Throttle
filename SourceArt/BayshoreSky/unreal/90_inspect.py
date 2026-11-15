"""Print the sky components' render flags on the placed actor in the test map and in the Blueprint template."""
import sys
import os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)) if "__file__" in globals() else r"K:\Full Throttle\SkyScratch\SourceArt\BayshoreSky\unreal")
import unreal
from bsky_ue import log
les = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
les.load_level("/BayshoreSky/Maps/L_BayshoreSkyTest")
sky = [a for a in eas.get_all_level_actors() if a.get_actor_label() == "BayshoreSky"][0]
for c in sky.get_components_by_class(unreal.StaticMeshComponent):
    m = c.get_material(0) if hasattr(c, "get_material") else None
    log("placed", c.get_name(), "depth_pass", c.get_editor_property("render_in_depth_pass"), "main_pass", c.get_editor_property("render_in_main_pass"),
        "wpo_vel", c.get_editor_property("world_position_offset_writes_velocity"), "mat", m.get_name() if m else None,
        "blend", m.get_base_material().get_editor_property("blend_mode") if m else None, "sky", m.get_base_material().get_editor_property("is_sky") if m else None)
