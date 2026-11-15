"""L_BayshoreSkyBake_Day under /BayshoreSky/Bake (never cooked): a live, physically based sky for reference renders only
(Sky Atmosphere, Volumetric Cloud, Exponential Height Fog, a Directional Light at the preset's sun, a real-time Sky Light).
It is the in-engine counterpart of the Blender bake (BAKE.md). Run: ue_run.ps1 -Editor -Script 55_bake_map.py
"""
import sys
import os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)) if "__file__" in globals() else r"K:\Full Throttle\SkyScratch\SourceArt\BayshoreSky\unreal")
import unreal
from bsky_ue import log, enum_entry, ensure_dir, EAL

P, AZ, EL = "Day", 30.0, 35.0
MAP = "/BayshoreSky/Bake/L_BayshoreSkyBake_" + P
les = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
ensure_dir("/BayshoreSky/Bake")
if EAL.does_asset_exist(MAP):
    EAL.delete_asset(MAP)
unreal.SystemLibrary.collect_garbage()
if not les.new_level(MAP):
    raise RuntimeError("new_level failed")


def spawn(cls, label, loc=(0, 0, 0), rot=None):
    a = eas.spawn_actor_from_class(cls, unreal.Vector(*loc), rot or unreal.Rotator(0, 0, 0))
    a.set_actor_label(label)
    return a


sun = spawn(unreal.DirectionalLight, "Sun_" + P, rot=unreal.Rotator(roll=0.0, pitch=-EL, yaw=AZ + 180.0))
lc = sun.get_component_by_class(unreal.DirectionalLightComponent)
lc.set_editor_property("intensity", 10.0)
lc.set_editor_property("atmosphere_sun_light", True)
lc.set_editor_property("mobility", unreal.ComponentMobility.MOVABLE)
spawn(unreal.SkyAtmosphere, "SkyAtmosphere")
vc = spawn(unreal.VolumetricCloud, "VolumetricCloud")
fog = spawn(unreal.ExponentialHeightFog, "HeightFog")
sky = spawn(unreal.SkyLight, "SkyLight_RealTime")
sc = sky.get_component_by_class(unreal.SkyLightComponent)
sc.set_editor_property("mobility", unreal.ComponentMobility.MOVABLE)
sc.set_editor_property("real_time_capture", True)
ppv = spawn(unreal.PostProcessVolume, "PP_Bake")
ppv.set_editor_property("unbound", True)
s = ppv.get_editor_property("settings")
s.set_editor_property("override_auto_exposure_method", True)
s.set_editor_property("auto_exposure_method", enum_entry(unreal.AutoExposureMethod, "AEM_Manual"))
s.set_editor_property("override_auto_exposure_bias", True)
s.set_editor_property("auto_exposure_bias", 10.0)
s.set_editor_property("override_vignette_intensity", True)
s.set_editor_property("vignette_intensity", 0.0)
ppv.set_editor_property("settings", s)
if not les.save_current_level():
    raise RuntimeError("save failed")
log("bake map saved", MAP)
