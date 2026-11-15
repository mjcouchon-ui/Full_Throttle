"""Shared helpers for the BayshoreSky Unreal build scripts (UE 5.9 editor Python, headless commandlet)."""
import os
import unreal

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))   # SourceArt/BayshoreSky of whichever project runs it
PLUG = "/BayshoreSky"
EAL = unreal.EditorAssetLibrary
MEL = unreal.MaterialEditingLibrary
AT = unreal.AssetToolsHelpers.get_asset_tools()


def log(*a):
    print("BSKY", *[str(x) for x in a], flush=True)


def enum_entry(enum_cls, cpp_name):
    want = cpp_name.replace("_", "").lower()
    for attr in dir(enum_cls):
        if attr.startswith("_"):
            continue
        v = getattr(enum_cls, attr)
        if isinstance(v, enum_cls) and attr.replace("_", "").lower() == want:
            return v
    raise KeyError("%s has no %s" % (enum_cls.__name__, cpp_name))


def ensure_dir(path):
    if not EAL.does_directory_exist(path):
        EAL.make_directory(path)


def save(asset):
    if not EAL.save_loaded_asset(asset, False):
        raise RuntimeError("save failed: %s" % asset.get_path_name())


def load_or_create(folder, name, asset_class, factory):
    ensure_dir(folder)
    path = "%s/%s" % (folder, name)
    if EAL.does_asset_exist(path):
        a = EAL.load_asset(path)
        if a is not None:
            return a
    a = AT.create_asset(name, folder, asset_class, factory)
    if a is None:
        raise RuntimeError("could not create " + path)
    return a


# ------------------------------------------------------------------------------------------------ import
def import_texture(src, folder, name, compression, srgb, mips, address_x="TA_Wrap", address_y="TA_Wrap",
                   lod_group="TEXTUREGROUP_World", cube=False, never_stream=False, filt="TF_Default"):
    ensure_dir(folder)
    pipe = unreal.InterchangeGenericTexturePipeline()
    pipe.set_editor_property("asset_name", name)
    pipe.set_editor_property("detect_normal_map_texture", False)
    pipe.set_editor_property("detect_long_lat_cubemap", False)
    pipe.set_editor_property("file_extensions_to_import_as_long_lat_cubemap", {"hdr"} if cube else set())
    stack = unreal.InterchangePipelineStackOverride()
    stack.add_pipeline(pipe)
    task = unreal.AssetImportTask()
    task.set_editor_property("filename", src)
    task.set_editor_property("destination_path", folder)
    task.set_editor_property("destination_name", name)
    task.set_editor_property("automated", True)
    task.set_editor_property("replace_existing", True)
    task.set_editor_property("save", False)
    task.set_editor_property("options", stack)
    AT.import_asset_tasks([task])
    tex = unreal.load_asset("%s/%s" % (folder, name))
    if tex is None:
        raise RuntimeError("import failed: " + src)
    want = unreal.TextureCube if cube else unreal.Texture2D
    if not isinstance(tex, want):
        raise RuntimeError("%s imported as %s, wanted %s" % (name, type(tex).__name__, want.__name__))
    tex.set_editor_property("lod_group", enum_entry(unreal.TextureGroup, lod_group))
    tex.set_editor_property("compression_settings", enum_entry(unreal.TextureCompressionSettings, compression))
    tex.set_editor_property("srgb", srgb)
    tex.set_editor_property("mip_gen_settings", enum_entry(unreal.TextureMipGenSettings,
                                                           "TMGS_FromTextureGroup" if mips else "TMGS_NoMipmaps"))
    if not cube:
        tex.set_editor_property("address_x", enum_entry(unreal.TextureAddress, address_x))
        tex.set_editor_property("address_y", enum_entry(unreal.TextureAddress, address_y))
    tex.set_editor_property("filter", enum_entry(unreal.TextureFilter, filt))
    tex.set_editor_property("never_stream", never_stream)
    save(tex)
    log("texture", name, type(tex).__name__, compression, "srgb", srgb, "mips", mips)
    return tex


def mesh_pipeline():
    pipe = unreal.InterchangeGenericAssetsPipeline()
    pipe.set_editor_property("use_source_name_for_asset", True)
    cm = pipe.get_editor_property("common_meshes_properties")
    cm.set_editor_property("force_all_mesh_as_type", enum_entry(unreal.InterchangeForceMeshType, "IFMT_StaticMesh"))
    cm.set_editor_property("import_lods", True)
    cm.set_editor_property("bake_meshes", True)
    cm.set_editor_property("recompute_normals", False)
    cm.set_editor_property("recompute_tangents", True)
    cm.set_editor_property("vertex_color_import_option", enum_entry(unreal.InterchangeVertexColorImportOption, "IVCIO_Replace"))
    cm.set_editor_property("import_sockets", False)
    mp = pipe.get_editor_property("mesh_pipeline")
    mp.set_editor_property("import_static_meshes", True)
    mp.set_editor_property("import_skeletal_meshes", False)
    mp.set_editor_property("collision", False)
    mp.set_editor_property("build_nanite", False)
    mp.set_editor_property("generate_lightmap_u_vs", False)
    mp.set_editor_property("distance_field_resolution_scale", 0.0)
    mp.set_editor_property("max_lumen_mesh_cards", 0)
    mat = pipe.get_editor_property("material_pipeline")
    mat.set_editor_property("import_materials", False)
    mat.get_editor_property("texture_pipeline").set_editor_property("import_textures", False)
    pipe.get_editor_property("animation_pipeline").set_editor_property("import_animations", False)
    return pipe


def import_mesh(src, folder, name):
    ensure_dir(folder)
    stack = unreal.InterchangePipelineStackOverride()
    pipe = mesh_pipeline()
    pipe.set_editor_property("asset_name", name)
    pipe.set_editor_property("use_source_name_for_asset", False)
    stack.add_pipeline(pipe)
    task = unreal.AssetImportTask()
    task.set_editor_property("filename", src)
    task.set_editor_property("destination_path", folder)
    task.set_editor_property("destination_name", name)
    task.set_editor_property("automated", True)
    task.set_editor_property("replace_existing", True)
    task.set_editor_property("save", False)
    task.set_editor_property("options", stack)
    AT.import_asset_tasks([task])
    paths = list(task.get_editor_property("imported_object_paths"))
    mesh = None
    for p in paths:
        a = unreal.load_asset(p)
        if isinstance(a, unreal.StaticMesh):
            mesh = a
    if mesh is None:
        raise RuntimeError("no static mesh from %s (%s)" % (src, paths))
    if mesh.get_name() != name:
        log("WARNING mesh imported as", mesh.get_path_name(), "wanted", name)
    sms = unreal.get_editor_subsystem(unreal.StaticMeshEditorSubsystem)
    sms.remove_collisions(mesh)
    mesh.set_editor_property("generate_mesh_distance_field", False)
    mesh.set_editor_property("support_ray_tracing", False)
    bs = mesh.get_editor_property("body_setup")
    if bs is not None:
        bs.set_editor_property("collision_trace_flag", enum_entry(unreal.CollisionTraceFlag, "CTF_UseSimpleAsComplex"))
    save(mesh)
    b = mesh.get_bounds()
    log("mesh", mesh.get_name(), "verts", sms.get_number_verts(mesh, 0), "lods", sms.get_lod_count(mesh),
        "bounds origin", b.origin, "extent", b.box_extent)
    return mesh


# ------------------------------------------------------------------------------------------------ components
def sky_component_flags(comp, translucent_sort=0):
    """Settings every sky primitive gets: no collision, no shadows, no DF lighting, no navigation, no decals."""
    comp.set_collision_profile_name("NoCollision")
    comp.set_editor_property("cast_shadow", False)
    for prop, val in (("affect_distance_field_lighting", False), ("affect_dynamic_indirect_lighting", False),
                      ("receives_decals", False), ("visible_in_ray_tracing", False),
                      ("can_ever_affect_navigation", False), ("render_custom_depth", False),
                      ("never_distance_cull", True), ("use_as_occluder", False),
                      ("cast_dynamic_shadow", False), ("cast_static_shadow", False)):
        try:
            comp.set_editor_property(prop, val)
        except Exception as e:  # noqa
            log("WARNING", comp.get_name(), prop, e)
    if translucent_sort:
        comp.set_editor_property("translucency_sort_priority", translucent_sort)
