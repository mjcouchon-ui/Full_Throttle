"""Level 1 (Bayshore) asset sheet, part 1 (runs in the port's editor): every asset Level 1 uses, with metadata and a thumbnail.

Roots: the map L_Bayshore, everything under /Game/Maps/Bayshore (props, materials), the game mode (car, HUD and what they
reference) and the assets the C++ loads by path (minus the other two levels). Every dependency (hard and soft) is walked;
Engine and script packages are listed but not walked further.
Thumbnails: textures are exported as PNG; meshes and materials are rendered with a scene capture on a plain backdrop;
other assets get a class tile in part 2. Output: K:\\Full Throttle\\Level1_Asset_Sheet\\_work\\(assets.json, thumbs/*.png)
Part 2 (system Python): build_asset_sheet.py -> Level1_Asset_Sheet.html and .csv
"""
import os
import re
import json
import math
import unreal

WORK = r"K:\Full Throttle\Level1_Asset_Sheet\_work"
THUMBS = os.path.join(WORK, "thumbs")
os.makedirs(THUMBS, exist_ok=True)
PROJ = r"K:\Full Throttle\UE\full-throttle-unreal"
AR = unreal.AssetRegistryHelpers.get_asset_registry()
EAL = unreal.EditorAssetLibrary
LES = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
EAS = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)


def log(*a):
    print("BSKY", *[str(x) for x in a], flush=True)


AR.search_all_assets(True)
opts = unreal.AssetRegistryDependencyOptions(include_soft_package_references=True, include_hard_package_references=True,
                                              include_searchable_names=False, include_soft_management_references=False,
                                              include_hard_management_references=False)

# ---------------------------------------------------------------- roots
roots = {}   # package -> why


def add_root(pkg, why):
    roots.setdefault(pkg, why)


add_root("/Game/Maps/Bayshore/L_Bayshore", "Level 1 map")
for a in AR.get_assets_by_path("/Game/Maps/Bayshore", recursive=True):
    add_root(str(a.package_name), "Level 1 folder (props, materials, sky)")
add_root("/Game/Core/BP_FTGameMode", "game mode (every level)")
src = ""
for dp, dn, fn in os.walk(os.path.join(PROJ, "Source")):
    for f in fn:
        if f.endswith((".cpp", ".h")):
            src += open(os.path.join(dp, f), encoding="utf-8", errors="ignore").read()
for ref in sorted(set(re.findall(r'"(/Game/[A-Za-z0-9_/\.]+)', src))):
    if "/ChemicalWorks" in ref or "/Nevada" in ref:
        continue
    base = ref.split(".")[0]
    if base.endswith("/"):
        for a in AR.get_assets_by_path(base.rstrip("/"), recursive=True):
            add_root(str(a.package_name), "loaded by C++ (every level)")
    elif EAL.does_asset_exist(base):
        add_root(base, "loaded by C++ (every level)")
    else:   # a name prefix such as /Game/Audio/SFX/S_
        folder, prefix = base.rsplit("/", 1)
        for a in AR.get_assets_by_path(folder, recursive=False):
            if str(a.asset_name).startswith(prefix):
                add_root(str(a.package_name), "loaded by C++ (every level)")
# the sky: Level Instance and everything in the plugin it pulls in comes through the map's dependencies

# ---------------------------------------------------------------- walk
seen, order, used_by = {}, [], {}
stack = list(roots.keys())
while stack:
    pkg = stack.pop()
    if pkg in seen:
        continue
    seen[pkg] = roots.get(pkg)
    order.append(pkg)
    if pkg.startswith("/Script") or pkg.startswith("/Engine"):
        continue
    for d in (AR.get_dependencies(pkg, opts) or []):
        d = str(d)
        used_by.setdefault(d, set()).add(pkg)
        if d not in seen:
            stack.append(d)
log("assets found", len(order), "roots", len(roots))


def disk_size(pkg):
    if pkg.startswith("/Game/"):
        base = os.path.join(PROJ, "Content", pkg[len("/Game/"):].replace("/", os.sep))
    elif pkg.startswith("/BayshoreSky/"):
        base = os.path.join(PROJ, "Plugins", "BayshoreSky", "Content", pkg[len("/BayshoreSky/"):].replace("/", os.sep))
    else:
        return None
    total = 0
    for ext in (".uasset", ".umap", ".uexp", ".ubulk"):
        if os.path.exists(base + ext):
            total += os.path.getsize(base + ext)
    return total


rows = []
to_render = []
for pkg in order:
    if pkg.startswith("/Script"):
        continue
    data = AR.get_assets_by_package_name(pkg)
    cls = str(data[0].asset_class_path.asset_name) if data else "?"
    name = pkg.rsplit("/", 1)[1]
    r = dict(name=name, path=pkg, type=cls, bytes=disk_size(pkg), detail="", why=roots.get(pkg) or "", thumb="")
    if pkg.startswith("/Engine"):
        r["source"], r["licence"] = "Unreal Engine content", "Unreal Engine EULA"
    elif pkg.startswith("/BayshoreSky"):
        bake = "/Textures/Day/" in pkg
        r["source"] = "BayshoreSky sky (bake, BAKE.md)" if bake else "BayshoreSky sky (this work)"
        r["licence"] = "ours; credits in SkyScratch/CREDITS.md"
    else:
        r["source"], r["licence"] = "Matt's port", "port (owner to declare)"
    users = sorted(u for u in used_by.get(pkg, ()) if not u.startswith("/Script"))
    r["used_by"] = "; ".join(users[:6]) + (" (+%d)" % (len(users) - 6) if len(users) > 6 else "")
    if not r["why"]:
        r["why"] = "dependency"
    if not pkg.startswith("/Engine") and data:
        try:
            obj = unreal.load_asset(pkg)
        except Exception:
            obj = None
        if isinstance(obj, unreal.StaticMesh):
            r["detail"] = "%d tris LOD0, %d LODs%s" % (obj.get_num_triangles(0), obj.get_num_lods(),
                                                       ", Nanite %d tris" % obj.get_num_nanite_triangles() if obj.get_num_nanite_triangles() else "")
            to_render.append((pkg, "mesh"))
        elif isinstance(obj, unreal.Texture2D):
            r["detail"] = "%d x %d, %s" % (obj.blueprint_get_size_x(), obj.blueprint_get_size_y(),
                                          str(obj.get_editor_property("compression_settings")).split(".")[-1])
            to_render.append((pkg, "texture"))
        elif isinstance(obj, unreal.TextureCube):
            r["detail"] = "cube"
        elif isinstance(obj, unreal.MaterialInterface):
            to_render.append((pkg, "material"))
        elif isinstance(obj, unreal.SoundWave):
            r["detail"] = "%.1f s" % obj.get_editor_property("duration")
    rows.append(r)

json.dump(rows, open(os.path.join(WORK, "assets.json"), "w", encoding="utf-8"), indent=1)
log("rows", len(rows), "to render", len(to_render))

# ---------------------------------------------------------------- thumbnails
for pkg, kind in [t for t in to_render if t[1] == "texture"]:
    try:
        task = unreal.AssetExportTask()
        task.set_editor_property("object", unreal.load_asset(pkg))
        task.set_editor_property("filename", os.path.join(THUMBS, pkg.strip("/").replace("/", "__") + ".png"))
        task.set_editor_property("automated", True)
        task.set_editor_property("replace_identical", True)
        task.set_editor_property("prompt", False)
        unreal.Exporter.run_asset_export_task(task)
    except Exception as e:  # noqa
        log("texture export failed", pkg, e)

renders = [t for t in to_render if t[1] in ("mesh", "material")]
world = unreal.EditorLoadingAndSavingUtils.new_blank_map(False)
world = unreal.EditorLevelLibrary.get_editor_world()
rt = unreal.RenderingLibrary.create_render_target2d(world, 256, 256, unreal.TextureRenderTargetFormat.RTF_RGBA8, unreal.LinearColor(0, 0, 0, 1), False)
light = EAS.spawn_actor_from_class(unreal.DirectionalLight, unreal.Vector(0, 0, 1000), unreal.Rotator(roll=0, pitch=-45, yaw=135))
sky = EAS.spawn_actor_from_class(unreal.SkyLight, unreal.Vector(0, 0, 500), unreal.Rotator(0, 0, 0))
holder = EAS.spawn_actor_from_class(unreal.StaticMeshActor, unreal.Vector(0, 0, 0), unreal.Rotator(0, 0, 0))
smc = holder.static_mesh_component
smc.set_mobility(unreal.ComponentMobility.MOVABLE)
sphere = unreal.load_asset("/Engine/BasicShapes/Sphere")
cap = EAS.spawn_actor_from_class(unreal.SceneCapture2D, unreal.Vector(0, 0, 0), unreal.Rotator(0, 0, 0))
cc = cap.get_editor_property("capture_component2d")
cc.set_editor_property("texture_target", rt)
cc.set_editor_property("capture_source", unreal.SceneCaptureSource.SCS_FINAL_COLOR_LDR)
cc.set_editor_property("capture_every_frame", False)
cc.set_editor_property("always_persist_rendering_state", True)
cc.set_editor_property("fov_angle", 30.0)
state = {"i": 0, "f": 0, "h": None}


def setup(pkg, kind):
    obj = unreal.load_asset(pkg)
    if kind == "mesh":
        smc.set_static_mesh(obj)
        smc.set_editor_property("override_materials", [])
    else:
        smc.set_static_mesh(sphere)
        smc.set_editor_property("override_materials", [obj])
    o, e, rad = unreal.SystemLibrary.get_component_bounds(smc)
    dist = max(rad, 1.0) / math.tan(math.radians(15.0)) * 1.05
    d = unreal.Vector(1.0, -0.8, 0.55)
    d.normalize()
    loc = unreal.Vector(o.x + d.x * dist, o.y + d.y * dist, o.z + d.z * dist)
    yaw = math.degrees(math.atan2(-d.y, -d.x))
    pitch = math.degrees(math.atan2(-d.z, math.hypot(d.x, d.y)))
    cap.set_actor_location_and_rotation(loc, unreal.Rotator(roll=0, pitch=pitch, yaw=yaw), False, True)


def tick(dt):
    try:
        if state["i"] >= len(renders):
            unreal.unregister_slate_post_tick_callback(state["h"])
            log("thumbnails done", len(renders))
            unreal.SystemLibrary.quit_editor()
            return
        pkg, kind = renders[state["i"]]
        if state["f"] == 0:
            setup(pkg, kind)
        cc.capture_scene()
        state["f"] += 1
        if state["f"] >= 6:
            unreal.RenderingLibrary.export_render_target(world, rt, THUMBS, pkg.strip("/").replace("/", "__") + ".png")
            state["i"] += 1
            state["f"] = 0
    except Exception as e:  # noqa
        log("THUMB ERROR", renders[state["i"]], e)
        state["i"] += 1
        state["f"] = 0


unreal.EditorPythonScripting.set_keep_python_script_alive(True)
state["h"] = unreal.register_slate_post_tick_callback(tick)
