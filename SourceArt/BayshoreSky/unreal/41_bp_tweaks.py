"""Small edits to BP_BayshoreSky's component templates without rebuilding it (keeps the placed actors valid). Rerun after
any 40_blueprints.py run, then 82_place_cumulus.py for the placed actors.
- no world-position-offset velocity for slow or camera-relative sky parts (saves the velocity pass; gulls keep it);
- retired parts hidden in game (no draw call): the blimp's ink hull (no cel look) and the SunDisc quad (the disc is drawn
  inside the dome now, so cloud can cross it);
- the cumulus card layout (bsky_layout.py);
- volumetric lighting on the sky's sun (owner, 14 Nov): light-shaft bloom (rays from the bright sky round the sun, broken
  up by cloud and buildings), volumetric-fog scattering with volumetric shadows (shafts between buildings when the level's
  height fog has volumetric fog on; ft_sky.py sets that up), and the sky light's share of the volumetric fog."""
import sys
import os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)) if "__file__" in globals() else r"K:\Full Throttle\SkyScratch\SourceArt\BayshoreSky\unreal")
import unreal
from bsky_ue import log, save
import bsky_layout

bp = unreal.load_asset("/BayshoreSky/Blueprints/BP_BayshoreSky")
sub = unreal.get_engine_subsystem(unreal.SubobjectDataSubsystem)
RETIRED = {"BlimpOutline", "SunDisc"}
NO_VELOCITY = {"Clouds", "SunDisc", "SunGlow", "FlareGhosts", "Blimp", "BlimpOutline"}
SUN_LIGHT = {
    "enable_light_shaft_bloom": True, "bloom_scale": 0.16, "bloom_threshold": 1.2, "bloom_max_brightness": 40.0,
    "bloom_tint": unreal.Color(r=255, g=214, b=176, a=255), "enable_light_shaft_occlusion": False,
    "volumetric_scattering_intensity": 1.6, "cast_volumetric_shadow": True,
}
SKY_LIGHT = {"volumetric_scattering_intensity": 1.0}

for h in sub.k2_gather_subobject_data_for_blueprint(bp):
    data = sub.k2_find_subobject_data_from_handle(h)
    obj = unreal.SubobjectDataBlueprintFunctionLibrary.get_object_for_blueprint(data, bp)
    if obj is None:
        continue
    name = obj.get_name().replace("_GEN_VARIABLE", "")
    if name == "HorizonHills":
        obj.set_editor_property("render_in_depth_pass", True)    # finite distance: must sort against the clouds
        obj.set_editor_property("relative_scale3d", unreal.Vector(20000.0, 20000.0, 20000.0))   # 20 km: Level 1's road spans ~13 km
    if name == "SkyDome":
        obj.set_editor_property("render_in_depth_pass", False)   # drawn in the sky pass after opaque geometry; no prepass needed
    if name == "Clouds" and isinstance(obj, unreal.InstancedStaticMeshComponent):
        log("cumulus cards (template)", bsky_layout.apply(obj, unreal))
    if name in RETIRED:
        # ApplyPreset's SetVisibility does not touch bHiddenInGame; untick "Hidden in Game" to bring them back
        obj.set_editor_property("hidden_in_game", True)
    if name in NO_VELOCITY:
        obj.set_editor_property("world_position_offset_writes_velocity", False)
    if name == "SunLight" and isinstance(obj, unreal.DirectionalLightComponent):
        for k, v in SUN_LIGHT.items():
            obj.set_editor_property(k, v)
        log("sun light: light shafts and volumetric scattering")
    if name == "SkyLight" and isinstance(obj, unreal.SkyLightComponent):
        for k, v in SKY_LIGHT.items():
            obj.set_editor_property(k, v)
unreal.BlueprintEditorLibrary.compile_blueprint(bp)
save(bp)
log("bp tweaks done")
