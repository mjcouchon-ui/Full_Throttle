"""BP_BayshoreSkyPreset (PrimaryDataAsset class), DA_BayshoreSkyPreset_Day and BP_BayshoreSky (the sky actor).

Run in the offscreen editor: ue_run.ps1 -Editor -Script 40_blueprints.py
Rebuilds the Blueprints from scratch each run (deletes and recreates them).
"""
import sys
import os
import math
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)) if "__file__" in globals() else r"K:\Full Throttle\SkyScratch\SourceArt\BayshoreSky\unreal")
import importlib
import unreal
from bsky_ue import log, enum_entry, ensure_dir, save, sky_component_flags, EAL, AT

from bsky_params import SCALARS as ALL_SCALARS, VECTORS, NOT_IN_PRESET
SCALARS = [s for s in ALL_SCALARS if s[0] not in NOT_IN_PRESET]

BEL = unreal.BlueprintEditorLibrary
BGE = unreal.BlueprintGraphEditor
BPDIR = "/BayshoreSky/Blueprints"
DATADIR = "/BayshoreSky/Data"
MPC_PATH = "/BayshoreSky/Materials/MPC_BayshoreSky.MPC_BayshoreSky"
P = "Day"


def real():
    return BEL.get_basic_type_by_name("real")


def tbool():
    return BEL.get_basic_type_by_name("bool")


def tstr():
    return BEL.get_basic_type_by_name("string")


def tstruct(s):
    return BEL.get_struct_type(s.static_struct())


def tobj(c):
    return BEL.get_object_reference_type(c.static_class())


def fresh_blueprint(path, parent):
    if EAL.does_asset_exist(path):
        EAL.delete_asset(path)
    bp = BEL.create_blueprint_asset_with_parent(path, parent)
    if bp is None:
        raise RuntimeError("could not create " + path)
    return bp


def add_var(bp, name, ptype, category, tooltip=""):
    if not BEL.add_member_variable(bp, name, ptype):
        raise RuntimeError("add var failed " + name)
    BEL.set_blueprint_variable_instance_editable(bp, name, True)
    BEL.set_blueprint_variable_category(bp, name, unreal.Text(category))
    if tooltip:
        BEL.set_blueprint_variable_tooltip(bp, name, tooltip)


def compile_bp(bp):
    ok = BEL.compile_blueprint(bp)
    errs = []
    for gname in BEL.list_graph_names(bp):
        try:
            ed = BGE.get_graph_editor_by_name(bp, gname)
        except Exception:
            continue
        if ed is None:
            continue
        for n in ed.list_nodes_with_errors():
            errs.append("%s: %s: %s" % (gname, n.get_node_title(), n.get_editor_property("error_msg")))
    for e in errs:
        log("BP ERROR", bp.get_name(), e)
    if not ok or errs:
        raise RuntimeError("%s did not compile" % bp.get_name())
    log("compiled", bp.get_name())


# ------------------------------------------------------------------------------------------------ preset class
PRESET_EXTRA = [
    ("PresetDescription", tstr, "Preset", "What this mood is for."),
    ("BakeName", tstr, "Preset", "Name of the bake in BAKE.md (bakes/<BakeName>/) that this preset's textures come from."),
    ("SunAzimuthDeg", real, "Sun", "Sun azimuth in degrees from +X toward +Y. Changing it needs a re-bake (BAKE.md)."),
    ("SunElevationDeg", real, "Sun", "Sun elevation in degrees. Changing it needs a re-bake (BAKE.md)."),
    ("SunLightRotation", lambda: tstruct(unreal.Rotator), "Sun", "Rotation of the directional light; points along -SunDirection."),
    ("SunLightIntensity", real, "Sun", "Directional light intensity (lux)."),
    ("SunLightColor", lambda: tstruct(unreal.LinearColor), "Sun", ""),
    ("SkyLightIntensity", real, "Sky Light", ""),
    ("SkyLightCubemap", lambda: tobj(unreal.TextureCube), "Sky Light", "Baked static cubemap for the Sky Light (BAKE.md target d)."),
    ("SkyMaterial", lambda: tobj(unreal.MaterialInterface), "Baked Materials", "MI_SkyDome_<Bake>: gradient + baked horizon strip + sea"),
    ("CloudMaterial", lambda: tobj(unreal.MaterialInterface), "Baked Materials", "MI_SemiCelCloud_<Bake>"),
    ("HorizonMaterial", lambda: tobj(unreal.MaterialInterface), "Baked Materials", "MI_HorizonBackdrop_<Bake>"),
]


def build_preset_class():
    path = BPDIR + "/BP_BayshoreSkyPreset"
    bp = fresh_blueprint(path, unreal.PrimaryDataAsset)
    for name, val in SCALARS:
        add_var(bp, name, real(), "Look (live)", "Written to MPC_BayshoreSky." + name)
    for name, val in VECTORS:
        add_var(bp, name, tstruct(unreal.LinearColor), "Look (live)", "Written to MPC_BayshoreSky." + name)
    for name, tf, cat, tip in PRESET_EXTRA:
        add_var(bp, name, tf(), cat, tip)
    compile_bp(bp)
    save(bp)
    return bp


def build_preset_asset(preset_bp):
    path = DATADIR + "/DA_BayshoreSkyPreset_" + P
    ensure_dir(DATADIR)
    if EAL.does_asset_exist(path):
        EAL.delete_asset(path)
    f = unreal.DataAssetFactory()
    f.set_editor_property("data_asset_class", preset_bp.generated_class())
    da = AT.create_asset("DA_BayshoreSkyPreset_" + P, DATADIR, None, f)
    for name, val in SCALARS:
        da.set_editor_property(name, float(val))
    for name, val in VECTORS:
        da.set_editor_property(name, unreal.LinearColor(*val))
    # sun and lights from bsky_params (golden hour, 6 deg), so a rebuild never reverts the look (review finding)
    from bsky_params import SUN_AZ as az, SUN_EL as el, PRESET_EXTRA_VALUES as X
    da.set_editor_property("PresetDescription", "Bayshore golden hour: low sun over the bay, pink cirrus and cumulus, warm haze (Brickell reference).")
    da.set_editor_property("BakeName", P)
    da.set_editor_property("SunAzimuthDeg", az)
    da.set_editor_property("SunElevationDeg", el)
    r = X["SunLightRotation"]
    da.set_editor_property("SunLightRotation", unreal.Rotator(roll=r[1], pitch=r[2], yaw=r[3]))
    da.set_editor_property("SunLightIntensity", X["SunLightIntensity"])
    c = X["SunLightColor"]
    da.set_editor_property("SunLightColor", unreal.LinearColor(c[1], c[2], c[3], 1.0))
    da.set_editor_property("SkyLightIntensity", X["SkyLightIntensity"])
    da.set_editor_property("SkyLightCubemap", unreal.load_asset("/BayshoreSky/Textures/%s/T_BSky_%s_SkyLight" % (P, P)))
    I = "/BayshoreSky/Materials/Instances/"
    da.set_editor_property("SkyMaterial", unreal.load_asset(I + "MI_SkyDome_" + P))
    da.set_editor_property("CloudMaterial", unreal.load_asset(I + "MI_SemiCelCloud_" + P))
    da.set_editor_property("HorizonMaterial", unreal.load_asset(I + "MI_HorizonBackdrop_" + P))
    save(da)
    log("preset asset", da.get_path_name())
    return da


# ------------------------------------------------------------------------------------------------ sky actor
SUB = None


def add_component(bp, parent_handle, cls, name):
    params = unreal.AddNewSubobjectParams(parent_handle=parent_handle, new_class=cls, blueprint_context=bp)
    handle, fail = SUB.add_new_subobject(params)
    if not unreal.SubobjectDataBlueprintFunctionLibrary.is_handle_valid(handle):
        raise RuntimeError("add component %s failed: %s" % (name, fail))
    SUB.rename_subobject(handle, unreal.Text(name))
    data = SUB.k2_find_subobject_data_from_handle(handle)
    tmpl = unreal.SubobjectDataBlueprintFunctionLibrary.get_object_for_blueprint(data, bp)
    return handle, tmpl


def mesh_comp(bp, root, name, mesh, material, scale=1.0, loc=(0, 0, 0), bounds=1.0, ism=False, sort=0):
    cls = unreal.InstancedStaticMeshComponent if ism else unreal.StaticMeshComponent
    h, t = add_component(bp, root, cls, name)
    t.set_editor_property("static_mesh", unreal.load_asset("/BayshoreSky/Meshes/" + mesh))
    t.set_editor_property("override_materials", [unreal.load_asset("/BayshoreSky/Materials/Instances/" + material)])
    t.set_editor_property("relative_location", unreal.Vector(*loc))
    t.set_editor_property("relative_scale3d", unreal.Vector(scale, scale, scale))
    t.set_editor_property("bounds_scale", bounds)
    sky_component_flags(t, sort)
    return h, t


def cloud_instances(t):
    import random
    rnd = random.Random(1987)
    t.set_editor_property("num_custom_data_floats", 5)
    order = list(range(32))
    rnd.shuffle(order)
    inst = []
    for i in range(24):
        az = rnd.uniform(-180, 180)
        dist = rnd.uniform(400000, 1400000)
        alt = rnd.uniform(110000, 330000)
        s = rnd.uniform(1100, 2900)
        x, y = math.cos(math.radians(az)) * dist, math.sin(math.radians(az)) * dist
        inst.append(((x, y, alt), s, [float(rnd.randrange(8)), rnd.random(), float(order[i]), 0.0, 400000.0]))
    for i in range(8):
        r = rnd.uniform(0, 260000)
        a = rnd.uniform(0, math.tau)
        s = rnd.uniform(1500, 2600)
        inst.append(((math.cos(a) * r, math.sin(a) * r, rnd.uniform(240000, 340000)), s,
                     [float(rnd.randrange(8)), rnd.random(), float(order[24 + i]), 1.0, 300000.0]))
    for k, (pos, s, data) in enumerate(inst):
        idx = t.add_instance(unreal.Transform(location=unreal.Vector(*pos), rotation=unreal.Rotator(0, 0, 0),
                                              scale=unreal.Vector(s, s, s)), False)
        for j, v in enumerate(data):
            t.set_custom_data_value(idx, j, float(v), False)
    log("cloud instances", t.get_instance_count())


def gull_instances(t):
    import random
    rnd = random.Random(1989)
    t.set_editor_property("num_custom_data_floats", 4)
    for i in range(7):
        pos = unreal.Vector(rnd.uniform(-6000, 6000), rnd.uniform(-6000, 6000), rnd.uniform(-1500, 2500))
        idx = t.add_instance(unreal.Transform(location=pos, rotation=unreal.Rotator(0, 0, 0), scale=unreal.Vector(1, 1, 1)), False)
        for j, v in enumerate([rnd.uniform(1800, 5000), rnd.choice([-1, 1]) * rnd.uniform(0.25, 0.45), rnd.random(),
                               rnd.uniform(100, 400)]):
            t.set_custom_data_value(idx, j, float(v), False)
    log("gull instances", t.get_instance_count())


class GraphHelper:
    def __init__(self, ed):
        self.ed = ed
        self.x = 300

    def place(self, node, dy=0):
        node.set_node_pos(unreal.IntPoint(self.x, dy))
        self.x += 360
        return node

    def call(self, path):
        n = self.ed.add_call_function_node(path)
        if n is None:
            raise RuntimeError("no function " + path)
        return n


def pin(node, name, out=False):
    p = node.find_output_pin(name) if out else node.find_input_pin(name)
    if not p.is_valid():
        names = [x.get_pin_name() for x in (node.list_output_pins() if out else node.list_input_pins())]
        raise RuntimeError("no pin %s on %s (%s)" % (name, node.get_node_title(), names))
    return p


def connect(a, b):
    if not a.try_create_connection(b):
        raise RuntimeError("connect failed %s -> %s" % (a.get_pin_name(), b.get_pin_name()))


def build_apply_preset(bp, preset_class_path):
    ed = BGE.create_and_edit_function_graph(bp, "ApplyPreset")
    ed.set_function_category(unreal.Text("Bayshore Sky"))
    ed.set_function_tooltip(unreal.Text("Writes a preset's live values to MPC_BayshoreSky and sets the baked materials, sun and sky light. Event-driven; nothing ticks."))
    preset_type = BEL.get_object_reference_type(unreal.load_asset(BPDIR + "/BP_BayshoreSkyPreset").generated_class())
    in_pin = ed.add_graph_input_parameter("InPreset", preset_type, "")
    entry_then = ed.find_graph_entry_pin()
    g = GraphHelper(ed)
    last = entry_then

    def chain(node):
        nonlocal last
        connect(last, node.find_execute_pin())
        last = node.find_then_pin()

    def preset_get(name):
        n = ed.add_get_member_variable_node(name, preset_class_path)
        connect(in_pin, n.find_self_pin())
        return pin(n, name, out=True)

    def comp_get(name):
        n = ed.add_get_member_variable_node(name, "")
        return pin(n, name, out=True)

    # if not valid, stop
    isv = g.call("/Script/Engine.KismetSystemLibrary.IsValid")
    connect(in_pin, pin(isv, "Object"))
    br = ed.add_branch_node()
    chain(br)
    connect(pin(isv, "ReturnValue", out=True), br.find_condition_pin())
    last = br.find_then_pin()
    # remember it
    setp = ed.add_set_member_variable_node("Preset", "")
    chain(setp)
    connect(in_pin, pin(setp, "Preset"))
    for name, _ in SCALARS:
        n = g.place(g.call("/Script/Engine.KismetMaterialLibrary.SetScalarParameterValue"))
        chain(n)
        pin(n, "Collection").set_pin_value(MPC_PATH)
        pin(n, "ParameterName").set_pin_value(name)
        connect(preset_get(name), pin(n, "ParameterValue"))
    for name, _ in VECTORS:
        n = g.place(g.call("/Script/Engine.KismetMaterialLibrary.SetVectorParameterValue"))
        chain(n)
        pin(n, "Collection").set_pin_value(MPC_PATH)
        pin(n, "ParameterName").set_pin_value(name)
        connect(preset_get(name), pin(n, "ParameterValue"))
    for comp, var in (("SkyDome", "SkyMaterial"), ("Clouds", "CloudMaterial"), ("HorizonHills", "HorizonMaterial")):
        n = g.place(g.call("/Script/Engine.PrimitiveComponent.SetMaterial"))
        chain(n)
        connect(comp_get(comp), n.find_self_pin())
        pin(n, "ElementIndex").set_pin_value("0")
        connect(preset_get(var), pin(n, "Material"))
    n = g.place(g.call("/Script/Engine.SceneComponent.K2_SetWorldRotation"))
    chain(n)
    connect(comp_get("SunLight"), n.find_self_pin())
    connect(preset_get("SunLightRotation"), pin(n, "NewRotation"))
    n = g.place(g.call("/Script/Engine.LightComponent.SetIntensity"))
    chain(n)
    connect(comp_get("SunLight"), n.find_self_pin())
    connect(preset_get("SunLightIntensity"), pin(n, "NewIntensity"))
    n = g.place(g.call("/Script/Engine.LightComponent.SetLightColor"))
    chain(n)
    connect(comp_get("SunLight"), n.find_self_pin())
    connect(preset_get("SunLightColor"), pin(n, "NewLightColor"))
    n = g.place(g.call("/Script/Engine.SkyLightComponent.SetCubemap"))
    chain(n)
    connect(comp_get("SkyLight"), n.find_self_pin())
    connect(preset_get("SkyLightCubemap"), pin(n, "NewCubemap"))
    n = g.place(g.call("/Script/Engine.SkyLightComponent.SetIntensity"))
    chain(n)
    connect(comp_get("SkyLight"), n.find_self_pin())
    connect(preset_get("SkyLightIntensity"), pin(n, "NewIntensity"))
    # sea on or off: it is part of the dome material, switched by MPC SeaEnabled
    n = g.place(g.call("/Script/Engine.KismetMaterialLibrary.SetScalarParameterValue"))
    chain(n)
    pin(n, "Collection").set_pin_value(MPC_PATH)
    pin(n, "ParameterName").set_pin_value("SeaEnabled")
    conv = g.call("/Script/Engine.KismetMathLibrary.Conv_BoolToDouble")
    connect(comp_get("ShowSea"), pin(conv, "InBool"))
    connect(pin(conv, "ReturnValue", out=True), pin(n, "ParameterValue"))
    for comp, flag in (("HorizonHills", "ShowHorizonHills"), ("Blimp", "ShowBlimp"),
                       ("BlimpOutline", "ShowBlimp"), ("Gulls", "ShowGulls"), ("SunLight", "UseBuiltInSunLight"),
                       ("SkyLight", "UseBuiltInSkyLight")):
        n = g.place(g.call("/Script/Engine.SceneComponent.SetVisibility"))
        chain(n)
        connect(comp_get(comp), n.find_self_pin())
        connect(comp_get(flag), pin(n, "bNewVisibility"))
    n = g.place(g.call("/Script/Engine.SkyLightComponent.RecaptureSky"))
    chain(n)
    connect(comp_get("SkyLight"), n.find_self_pin())
    log("ApplyPreset nodes", len(ed.list_all_nodes()))


def build_events(bp):
    # construction script: ApplyPreset(Preset)
    cs = BGE.get_graph_editor_by_name(bp, "UserConstructionScript")
    call = cs.add_call_function_node("ApplyPreset")
    get = cs.add_get_member_variable_node("Preset", "")
    connect(cs.find_graph_entry_pin(), call.find_execute_pin())
    connect(pin(get, "Preset", out=True), pin(call, "InPreset"))
    # BeginPlay: ApplyPreset, then the scalability hook
    eg = BGE.get_graph_editor_by_name(bp, "EventGraph")
    ev = BEL.add_event_override(bp, "ReceiveBeginPlay", unreal.IntPoint(0, 0))
    a = eg.add_call_function_node("ApplyPreset")
    g2 = eg.add_get_member_variable_node("Preset", "")
    connect(ev.find_then_pin(), a.find_execute_pin())
    connect(pin(g2, "Preset", out=True), pin(a, "InPreset"))
    gus = eg.add_call_function_node("/Script/Engine.GameUserSettings.GetGameUserSettings")
    q = eg.add_call_function_node("/Script/Engine.GameUserSettings.GetVisualEffectQuality")
    connect(pin(gus, "ReturnValue", out=True), q.find_self_pin())
    le = eg.add_call_function_node("/Script/Engine.KismetMathLibrary.LessEqual_IntInt")
    connect(pin(q, "ReturnValue", out=True), pin(le, "A"))
    pin(le, "B").set_pin_value("0")
    br = eg.add_branch_node()
    if gus.find_execute_pin().is_valid():          # impure: it must be on the exec chain or it is pruned
        connect(a.find_then_pin(), gus.find_execute_pin())
        connect(gus.find_then_pin(), br.find_execute_pin())
    else:
        connect(a.find_then_pin(), br.find_execute_pin())
    connect(pin(le, "ReturnValue", out=True), br.find_condition_pin())
    s1 = eg.add_call_function_node("/Script/Engine.KismetMaterialLibrary.SetScalarParameterValue")
    connect(br.find_then_pin(), s1.find_execute_pin())
    pin(s1, "Collection").set_pin_value(MPC_PATH)
    pin(s1, "ParameterName").set_pin_value("LowQuality")
    pin(s1, "ParameterValue").set_pin_value("1.0")
    s2 = eg.add_call_function_node("/Script/Engine.KismetMaterialLibrary.SetScalarParameterValue")
    connect(s1.find_then_pin(), s2.find_execute_pin())
    pin(s2, "Collection").set_pin_value(MPC_PATH)
    pin(s2, "ParameterName").set_pin_value("CloudCardsShown")
    getn = eg.add_get_member_variable_node("CloudCardsShown", BPDIR + "/BP_BayshoreSkyPreset.BP_BayshoreSkyPreset_C")
    g3 = eg.add_get_member_variable_node("Preset", "")
    connect(pin(g3, "Preset", out=True), getn.find_self_pin())
    mul = eg.add_call_function_node("/Script/Engine.KismetMathLibrary.Multiply_DoubleDouble")
    connect(pin(getn, "CloudCardsShown", out=True), pin(mul, "A"))
    pin(mul, "B").set_pin_value("0.5")
    connect(pin(mul, "ReturnValue", out=True), pin(s2, "ParameterValue"))
    for comp in ("FlareGhosts",):
        v = eg.add_call_function_node("/Script/Engine.SceneComponent.SetVisibility")
        connect(s2.find_then_pin(), v.find_execute_pin())
        gc = eg.add_get_member_variable_node(comp, "")
        connect(pin(gc, comp, out=True), v.find_self_pin())
        pin(v, "bNewVisibility").set_pin_value("false")


def build_sky_actor(preset_bp, preset_asset):
    global SUB
    SUB = unreal.get_engine_subsystem(unreal.SubobjectDataSubsystem)
    path = BPDIR + "/BP_BayshoreSky"
    bp = fresh_blueprint(path, unreal.Actor)
    handles = SUB.k2_gather_subobject_data_for_blueprint(bp)
    root = handles[0]
    mesh_comp(bp, root, "SkyDome", "SM_BSky_Dome", "MI_SkyDome_" + P, scale=60000.0)
    mesh_comp(bp, root, "HorizonHills", "SM_BSky_HorizonRing", "MI_HorizonBackdrop_" + P, scale=20000.0)
    mesh_comp(bp, root, "SunDisc", "SM_BSky_SunQuad", "MI_SunDisc", bounds=100000.0)
    mesh_comp(bp, root, "SunGlow", "SM_BSky_SunQuad", "MI_SunGlow", bounds=100000.0, sort=10)
    mesh_comp(bp, root, "FlareGhosts", "SM_BSky_FlareGhosts", "MI_FlareGhosts", bounds=100000.0, sort=20)
    _, clouds = mesh_comp(bp, root, "Clouds", "SM_BSky_CloudCard", "MI_SemiCelCloud_" + P, ism=True)
    cloud_instances(clouds)
    mesh_comp(bp, root, "Blimp", "SM_BSky_Blimp", "MI_SemiCelBlimp", loc=(0, 0, 30000))
    mesh_comp(bp, root, "BlimpOutline", "SM_BSky_BlimpOutline", "MI_InkOutlineBlimp", loc=(0, 0, 30000))
    _, gulls = mesh_comp(bp, root, "Gulls", "SM_BSky_Gull", "MI_SemiCelGull", loc=(0, 0, 6000), ism=True)
    gull_instances(gulls)
    _, sun = add_component(bp, root, unreal.DirectionalLightComponent, "SunLight")
    sun.set_editor_property("mobility", unreal.ComponentMobility.MOVABLE)
    sun.set_editor_property("intensity", 3.0)
    sun.set_editor_property("relative_rotation", unreal.Rotator(roll=0.0, pitch=-35.0, yaw=210.0))
    _, sky = add_component(bp, root, unreal.SkyLightComponent, "SkyLight")
    sky.set_editor_property("mobility", unreal.ComponentMobility.MOVABLE)
    sky.set_editor_property("source_type", enum_entry(unreal.SkyLightSourceType, "SLS_SpecifiedCubemap"))
    sky.set_editor_property("cubemap", preset_asset.get_editor_property("SkyLightCubemap"))
    sky.set_editor_property("real_time_capture", False)
    # variables
    preset_type = BEL.get_object_reference_type(preset_bp.generated_class())
    add_var(bp, "Preset", preset_type, "Bayshore Sky", "The active look. Change it here or call ApplyPreset at runtime.")
    for name, tip in (("ShowSea", "Turn off if the level already has its own sea (sets MPC SeaEnabled)."),
                      ("ShowHorizonHills", "Hide if the level already has its own horizon hills and skyline."),
                      ("ShowBlimp", ""), ("ShowGulls", ""),
                      ("UseBuiltInSunLight", "Turn off to use the level's own directional light (match its rotation to the preset)."),
                      ("UseBuiltInSkyLight", "Turn off to use the level's own sky light.")):
        add_var(bp, name, tbool(), "Bayshore Sky|Options", tip)
    compile_bp(bp)
    cdo = unreal.get_default_object(bp.generated_class())
    cdo.set_editor_property("Preset", preset_asset)
    for name in ("ShowSea", "ShowHorizonHills", "ShowBlimp", "ShowGulls", "UseBuiltInSunLight", "UseBuiltInSkyLight"):
        cdo.set_editor_property(name, True)
    build_apply_preset(bp, BPDIR + "/BP_BayshoreSkyPreset.BP_BayshoreSkyPreset_C")
    compile_bp(bp)
    build_events(bp)
    compile_bp(bp)
    save(bp)
    return bp


def main():
    ensure_dir(BPDIR)
    # delete dependents before the class they depend on (deleting a data asset whose class was replaced crashes)
    for path in (BPDIR + "/BP_BayshoreSky", DATADIR + "/DA_BayshoreSkyPreset_" + P, BPDIR + "/BP_BayshoreSkyPreset"):
        if EAL.does_asset_exist(path):
            if not EAL.delete_asset(path):
                raise RuntimeError("could not delete " + path)
            log("deleted", path)
    unreal.SystemLibrary.collect_garbage()
    pbp = build_preset_class()
    da = build_preset_asset(pbp)
    build_sky_actor(pbp, da)
    log("blueprints done")


main()
