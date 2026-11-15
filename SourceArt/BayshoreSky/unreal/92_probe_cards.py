"""Probe: why cloud cards still draw with CloudCardsShown 0 (MPC default, preset, the test map's actor, the card instances)."""
import unreal


def log(*a):
    print("BSKY", *[str(x) for x in a], flush=True)


mpc = unreal.load_asset("/BayshoreSky/Materials/MPC_BayshoreSky")
for p in mpc.get_editor_property("ScalarParameters"):
    if str(p.get_editor_property("ParameterName")) in ("CloudCardsShown", "BandGradient", "OutlineWidth"):
        log("mpc", p.get_editor_property("ParameterName"), p.get_editor_property("DefaultValue"))
da = unreal.load_asset("/BayshoreSky/Data/DA_BayshoreSkyPreset_Day")
log("da CloudCardsShown", da.get_editor_property("CloudCardsShown"))
unreal.get_editor_subsystem(unreal.LevelEditorSubsystem).load_level("/BayshoreSky/Maps/L_BayshoreSkyTest")
for a in unreal.get_editor_subsystem(unreal.EditorActorSubsystem).get_all_level_actors():
    if "BayshoreSky" in a.get_class().get_name():
        try:
            log("actor", a.get_actor_label(), "preset", a.get_editor_property("Preset"))
        except Exception as e:  # noqa
            log("actor", a.get_actor_label(), e)
        for c in a.get_components_by_class(unreal.InstancedStaticMeshComponent):
            n = c.get_instance_count()
            log("ism", c.get_name(), n, "custom data floats", c.get_editor_property("num_custom_data_floats"))
            if c.get_name().startswith("Clouds") and n:
                log("cd per instance", c.get_editor_property("per_instance_sm_custom_data")[:15])
