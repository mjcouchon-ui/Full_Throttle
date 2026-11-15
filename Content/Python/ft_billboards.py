"""Bayshore printed billboards (2.5): the 13 adverts of SourceArt/BayshoreBillboards Set01 + Set02 as game assets.

Run in the editor over the Unreal MCP connector, in the K: project (K:/Full Throttle/UE/full-throttle-unreal):
    import ft_billboards; ft_billboards.build()     # textures (imported once), M_BB_Printed, 13 instances, 13 meshes
    ft_billboards.check()                           # prints every asset's settings (face_mesh checks the face's orientation)
    ft_billboards.rebuild_track()                   # rebuilds Track_Bayshore and lists the placed signs
build(reimport=True) reimports the PNGs from SourceArt (the textures keep their source paths, so the editor's
Reimport works too). FTTrack.cpp (DefaultAds / PlanAds) places the meshes; this file never edits a map.

Assets, all under /Game/Maps/Bayshore/Billboards (cooked: /Game/Maps is in DirectoriesToAlwaysCook):
    Textures/T_BB_<Stem>_BC   BC7, sRGB, World group, mips from the group, streamed, clamped; max 1024 (Set01) / 2048 (Set02)
    Materials/M_BB_Printed    opaque Default Lit, one-sided; Face (texture), Tint, Rough 0.75, Specular 0.35, Glow 0
    Materials/MI_BB_<Stem>    one per design, only the Face texture differs
    Meshes/SM_BB_<Stem>       ft_city.ad_billboard() structure (palette materials shared with every prop)
                              + the 1200 x 600 cm face with 0..1 UVs in its own 'Face' slot. save_mesh asks for
                              Nanite, but 5.9's Geometry Script leaves it off unless NaniteSettings.bEnabled is set
                              too, so these are plain static meshes, like every ft_export prop
"""
import os
import unreal
import ft_city
import ft_export
import ft_mats

AL = unreal.EditorAssetLibrary
MEL = unreal.MaterialEditingLibrary
AT = unreal.AssetToolsHelpers.get_asset_tools()
GS_EDIT = unreal.GeometryScript_MeshEdits
GS_UV = unreal.GeometryScript_UVs
GS_QUERY = unreal.GeometryScript_MeshQueries

ROOT = '/Game/Maps/Bayshore/Billboards'
TEX = ROOT + '/Textures'
MAT = ROOT + '/Materials'
MESH = ROOT + '/Meshes'
SRC = os.path.join(unreal.Paths.convert_relative_path_to_full(unreal.Paths.project_dir()), 'SourceArt', 'BayshoreBillboards')

# stem, pack, catwalk accent, what it is. FTTrack.cpp DefaultAds() lists the same stems (kind, district, band).
DESIGNS = [
    ('LenFender', 'Set01', ft_city.YEL, 'Len Fender, injury attorney: STAY DOWN. I\'M ON MY WAY.'),
    ('DickSettle', 'Set01', ft_city.TEAL, 'Dick Settle, personal injury law: YOUR PAIN. MY BOAT.'),
    ('RearEndRick', 'Set01', ft_city.RED, 'Rear-End Rick, accident law: READING THIS WHEN YOU CRASHED?'),
    ('LoosePelican', 'Set01', ft_city.HOTPINK, 'The Loose Pelican, casino cruises: BLOW YOUR SETTLEMENT HERE.'),
    ('SecondMortgage', 'Set01', ft_city.CORAL, 'Second Mortgage Casino: COLLEGE ISN\'T FOR EVERYONE.'),
    ('SinkingFund', 'Set01', ft_city.LAV, 'The Sinking Fund, casino & resort: THE ATM BELIEVES IN YOU.'),
    ('CaptainShucks', 'Set01', ft_city.MINT, 'Captain Shuck\'s, oyster bar: WE SHUCK. YOU SUCK.'),
    ('BurnNotice', 'Set01', ft_city.PEACH, 'Burn Notice, hot wings: YOUR ASS HAS A SEQUEL.'),
    ('BigDicks', 'Set01', ft_city.YEL, 'Big Dick\'s, footlong franks: FINALLY. TWELVE HONEST INCHES.'),
    ('BillableBill', 'Set02', ft_city.YEL, 'Billable Bill, workplace injury law: HURT AT WORK? CONGRATULATIONS.'),
    ('LastDime', 'Set02', ft_city.CORAL, 'Last Dime, casino & resort: LOSE THE HOUSE. KEEP THE MUG.'),
    ('CaptainLiability', 'Set02', ft_city.MINT, 'Captain Liability\'s, seafood & raw bar: YESTERDAY\'S CATCH. TOMORROW\'S LAWSUIT.'),
    ('AlibiInn', 'Set02', ft_city.CORAL, 'Alibi Inn, Bayshore motor lodge: WE CHARGE BY THE LIE.'),
]

# Set01 was cut from a nine-panel sheet: ~580 x 290 real pixels per sign, Lanczos-upscaled to 2048. A 1024 cap keeps
# every pixel of that detail and a quarter of the memory. Set02 was painted at 1774 x 887, so it keeps 2048.
MAX_SIZE = {'Set01': 1024, 'Set02': 2048}


def _enum(cls, label):
    key = label.replace('_', '').lower()
    for name in dir(cls):
        if name.replace('_', '').lower() == key:
            return getattr(cls, name)
    raise RuntimeError('missing Unreal enum %s.%s' % (cls.__name__, label))


def _dir(p):
    if not AL.does_directory_exist(p):
        AL.make_directory(p)


def _out(r):
    """Geometry Script calls return (mesh, out value) in Python; take the out value."""
    return r[-1] if isinstance(r, tuple) else r


# ------------------------------------------------------------------------------------------ textures
def tex_path(stem):
    return '%s/T_BB_%s_BC' % (TEX, stem)


def import_textures(reimport=False):
    """Import each design's textures_2k PNG (once, or again with reimport=True) and apply the runtime settings."""
    _dir(TEX)
    out = []
    for stem, pack, _, _ in DESIGNS:
        src = os.path.join(SRC, pack, 'textures_2k', 'T_BB_%s_BC.png' % stem)
        if not os.path.isfile(src):
            raise RuntimeError('missing source texture ' + src)
        path = tex_path(stem)
        if reimport or not AL.does_asset_exist(path):
            # the project's proven Interchange route (SourceArt/BayshoreSky/unreal/bsky_ue.py import_texture)
            pipe = unreal.InterchangeGenericTexturePipeline()
            pipe.set_editor_property('asset_name', 'T_BB_%s_BC' % stem)
            pipe.set_editor_property('detect_normal_map_texture', False)
            pipe.set_editor_property('detect_long_lat_cubemap', False)
            pipe.set_editor_property('file_extensions_to_import_as_long_lat_cubemap', set())
            stack = unreal.InterchangePipelineStackOverride()
            stack.add_pipeline(pipe)
            task = unreal.AssetImportTask()
            task.set_editor_property('filename', src)
            task.set_editor_property('destination_path', TEX)
            task.set_editor_property('destination_name', 'T_BB_%s_BC' % stem)
            task.set_editor_property('automated', True)
            task.set_editor_property('replace_existing', True)
            task.set_editor_property('save', False)
            task.set_editor_property('options', stack)
            AT.import_asset_tasks([task])
        tex = unreal.load_asset(path)
        if not isinstance(tex, unreal.Texture2D):
            raise RuntimeError('%s did not import as a Texture2D (%s)' % (path, type(tex).__name__))
        tex.set_editor_property('compression_settings', _enum(unreal.TextureCompressionSettings, 'TC_BC7'))
        tex.set_editor_property('srgb', True)
        tex.set_editor_property('lod_group', _enum(unreal.TextureGroup, 'TEXTUREGROUP_World'))
        tex.set_editor_property('mip_gen_settings', _enum(unreal.TextureMipGenSettings, 'TMGS_FromTextureGroup'))
        tex.set_editor_property('address_x', _enum(unreal.TextureAddress, 'TA_Clamp'))
        tex.set_editor_property('address_y', _enum(unreal.TextureAddress, 'TA_Clamp'))
        tex.set_editor_property('never_stream', False)
        tex.set_editor_property('virtual_texture_streaming', False)
        tex.set_editor_property('lod_bias', 0)
        tex.set_editor_property('max_texture_size', MAX_SIZE[pack])
        AL.save_loaded_asset(tex, only_if_is_dirty=False)
        out.append(tex)
    return out


# ------------------------------------------------------------------------------------------ materials
def master():
    """M_BB_Printed: printed vinyl. ft_mats.new_material reuses an existing asset so instances and usage flags survive."""
    m = ft_mats.new_material(MAT, 'M_BB_Printed', blend=unreal.BlendMode.BLEND_OPAQUE,
                             shading=unreal.MaterialShadingModel.MSM_DEFAULT_LIT, two_sided=False)
    face = MEL.create_material_expression(m, unreal.MaterialExpressionTextureSampleParameter2D, -900, -200)
    face.set_editor_property('parameter_name', 'Face')
    face.set_editor_property('sampler_type', _enum(unreal.MaterialSamplerType, 'SAMPLERTYPE_Color'))
    face.set_editor_property('texture', unreal.load_asset(tex_path(DESIGNS[0][0])))
    tint = ft_mats.vparam(m, 'Tint', (1, 1, 1, 1), y=0)
    base = ft_mats.expr(m, unreal.MaterialExpressionMultiply, x=-500, y=-150)
    MEL.connect_material_expressions(face, 'RGB', base, 'A')
    MEL.connect_material_expressions(tint, '', base, 'B')
    # Glow 0: lit by the sun like any print. A restrained night/neon accent can raise it per instance.
    glow = ft_mats.sparam(m, 'Glow', 0.0, y=100)
    em = ft_mats.expr(m, unreal.MaterialExpressionMultiply, x=-500, y=50)
    MEL.connect_material_expressions(base, '', em, 'A')
    MEL.connect_material_expressions(glow, '', em, 'B')
    rough = ft_mats.sparam(m, 'Rough', 0.75, y=200)
    spec = ft_mats.sparam(m, 'Specular', 0.35, y=300)  # matte vinyl: no hot sun glint for the bloom to smear over the headline
    metal = ft_mats.expr(m, unreal.MaterialExpressionConstant, y=400)
    metal.set_editor_property('r', 0.0)
    MEL.connect_material_property(base, '', unreal.MaterialProperty.MP_BASE_COLOR)
    MEL.connect_material_property(em, '', unreal.MaterialProperty.MP_EMISSIVE_COLOR)
    MEL.connect_material_property(rough, '', unreal.MaterialProperty.MP_ROUGHNESS)
    MEL.connect_material_property(spec, '', unreal.MaterialProperty.MP_SPECULAR)
    MEL.connect_material_property(metal, '', unreal.MaterialProperty.MP_METALLIC)
    # the cooked game falls back to the default material without these (README: material usage flags)
    for flag in ('used_with_instanced_static_meshes', 'used_with_nanite'):
        try:
            m.set_editor_property(flag, True)
        except Exception as e:  # a renamed flag must not stop the build; check() reports it
            unreal.log_warning('M_BB_Printed: could not set %s (%s)' % (flag, e))
    return ft_mats.finish(m)


def instances(parent):
    out = {}
    for stem, _, _, _ in DESIGNS:
        inst = ft_mats.mi(parent, MAT, 'MI_BB_' + stem)
        MEL.set_material_instance_texture_parameter_value(inst, 'Face', unreal.load_asset(tex_path(stem)))
        MEL.update_material_instance(inst)
        AL.save_loaded_asset(inst, only_if_is_dirty=False)
        out[stem] = inst
    return out


# ------------------------------------------------------------------------------------------ meshes
def face_mesh():
    """The printed face: a 1200 x 600 cm quad in the plane x = ft_city.AD_FACE_X, its front toward -X.

    Seen from the front (camera on -X looking +X, so +Y is the viewer's right) the image's top-left, UV (0, 0),
    is at y = -600, z = top. One winding only: a dynamic mesh refuses a reversed second copy of the quad (its
    edges would be non-manifold), so the first version's "both windings" kept only the copy facing +X, into the
    backing panel, and the print never showed from the road. TL, BR, TR is the winding whose face normal points
    -X (Geometry Script's normals are the side Unreal draws); the build stops if it ever does not."""
    W, H, z0, x = ft_city.AD_W, ft_city.AD_H, ft_city.AD_Z0, ft_city.AD_FACE_X
    corners = [(x, -W / 2, z0 + H), (x, W / 2, z0 + H), (x, W / 2, z0), (x, -W / 2, z0)]  # TL, TR, BR, BL
    uvs = [(0.0, 0.0), (1.0, 0.0), (1.0, 1.0), (0.0, 1.0)]
    m = unreal.DynamicMesh()
    v = [_out(GS_EDIT.add_vertex_to_mesh(m, unreal.Vector(*c))) for c in corners]
    tris = [(0, 2, 1), (0, 3, 2)]
    ids = [_out(GS_EDIT.add_triangle_to_mesh(m, unreal.IntVector(v[a], v[b], v[c]))) for a, b, c in tris]
    if min(ids) < 0:
        raise RuntimeError('face_mesh: a triangle was refused (ids %s)' % ids)
    GS_UV.set_num_uv_sets(m, 1)
    for tid, (a, b, c) in zip(ids, tris):
        t = unreal.GeometryScriptUVTriangle()
        t.set_editor_property('uv0', unreal.Vector2D(*uvs[a]))
        t.set_editor_property('uv1', unreal.Vector2D(*uvs[b]))
        t.set_editor_property('uv2', unreal.Vector2D(*uvs[c]))
        GS_UV.set_mesh_triangle_u_vs(m, 0, tid, t)
    for tid in ids:
        r = GS_QUERY.get_triangle_face_normal(m, tid)
        n = r[0] if isinstance(r, tuple) else r
        if n.x > -0.99:
            raise RuntimeError('face_mesh: triangle %d faces (%.2f, %.2f, %.2f), not -X' % (tid, n.x, n.y, n.z))
    return m


def meshes(mis):
    _dir(MESH)
    out = []
    for stem, _, accent, _ in DESIGNS:
        model = ft_export.model(lambda mm, a=accent: ft_city.ad_billboard(mm, a))
        sm = ft_export.save_mesh(model.meshes, MESH + '/SM_BB_' + stem, 'prop', extra=[(face_mesh(), mis[stem], 'Face')])
        out.append(sm)
    return out


def build(reimport=False, textures=True):
    """Everything, in order. Safe to run again: textures are imported only once unless reimport=True."""
    with unreal.ScopedSlowTask(4, 'Bayshore billboards') as task:
        task.make_dialog(False)
        task.enter_progress_frame(1, 'textures')
        if textures:
            import_textures(reimport)
        task.enter_progress_frame(1, 'M_BB_Printed')
        parent = master()
        task.enter_progress_frame(1, 'instances')
        mis = instances(parent)
        task.enter_progress_frame(1, 'meshes')
        meshes(mis)
    return 'Bayshore billboards: %d textures, M_BB_Printed, %d instances, %d meshes under %s' % (len(DESIGNS), len(mis), len(DESIGNS), ROOT)


# ------------------------------------------------------------------------------------------ checks
def check():
    """One line per asset with the settings that matter; returns the lines (also logged)."""
    lines = []
    for stem, pack, _, _ in DESIGNS:
        t = unreal.load_asset(tex_path(stem))
        if t is None:
            lines.append('MISSING texture ' + stem)
            continue
        lines.append('%s %s %dx%d max %d %s srgb=%s group=%s mips=%s stream=%s addr=%s/%s src=%s' % (
            t.get_name(), pack, t.blueprint_get_size_x(), t.blueprint_get_size_y(), t.get_editor_property('max_texture_size'),
            t.get_editor_property('compression_settings'), t.get_editor_property('srgb'), t.get_editor_property('lod_group'),
            t.get_editor_property('mip_gen_settings'), not t.get_editor_property('never_stream'),
            t.get_editor_property('address_x'), t.get_editor_property('address_y'),
            ';'.join(t.get_editor_property('asset_import_data').extract_filenames()) if t.get_editor_property('asset_import_data') else '-'))
    m = unreal.load_asset(MAT + '/M_BB_Printed')
    if m:
        flags = []
        for f in ('used_with_instanced_static_meshes', 'used_with_nanite'):
            try:
                flags.append('%s=%s' % (f, m.get_editor_property(f)))
            except Exception:
                flags.append('%s=?' % f)
        lines.append('M_BB_Printed blend=%s shading=%s two_sided=%s %s' % (
            m.get_editor_property('blend_mode'), m.get_editor_property('shading_model'), m.get_editor_property('two_sided'), ' '.join(flags)))
    else:
        lines.append('MISSING M_BB_Printed')
    for stem, _, _, _ in DESIGNS:
        mi = unreal.load_asset(MAT + '/MI_BB_' + stem)
        sm = unreal.load_asset(MESH + '/SM_BB_' + stem)
        if mi is None or sm is None:
            lines.append('MISSING MI/SM ' + stem)
            continue
        tex = MEL.get_material_instance_texture_parameter_value(mi, 'Face')
        b = sm.get_bounding_box()
        slots = [s.get_editor_property('material_slot_name') for s in sm.get_editor_property('static_materials')]
        nan = sm.get_editor_property('nanite_settings').get_editor_property('enabled')
        lines.append('SM_BB_%s nanite=%s bounds x %.0f..%.0f y %.0f..%.0f z %.0f..%.0f slots=%d face=%s tex=%s' % (
            stem, nan, b.min.x, b.max.x, b.min.y, b.max.y, b.min.z, b.max.z, len(slots), 'Face' in [str(s) for s in slots],
            tex.get_name() if tex else None))
    for ln in lines:
        unreal.log('FTADS ' + ln)
    return lines


def track(level=0):
    names = {0: 'Bayshore'}
    EAS = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
    for a in EAS.get_all_level_actors():
        if a.get_class().get_name() == 'FTTrack' and a.get_editor_property('level_index') == level:
            return a
    raise RuntimeError('no FTTrack for level %d (%s) in the open map' % (level, names.get(level, '?')))


def layout(level=0):
    """{mesh name: [(x, y, z, yaw, scale)]} for every placed billboard, rounded, so two rebuilds can be compared."""
    out = {}
    for c in track(level).get_components_by_class(unreal.HierarchicalInstancedStaticMeshComponent):
        sm = c.get_editor_property('static_mesh')
        if sm is None or not sm.get_name().startswith('SM_BB_'):
            continue
        rows = []
        for i in range(c.get_instance_count()):
            t = _out(c.get_instance_transform(i, True))
            rows.append((round(t.translation.x), round(t.translation.y), round(t.translation.z), round(t.rotation.rotator().yaw, 1), round(t.scale3d.x, 2)))
        out[sm.get_name()] = rows
    return out


def rerun(a):
    """Rerun the track's construction (OnConstruction -> Rebuild). 5.9's Python has no rerun_construction_scripts,
    but an edited property reruns it, so build_in_editor is set to its own value (this can mark the map as modified;
    nothing saved in it changes, the track's components are transient)."""
    a.set_editor_property('build_in_editor', a.get_editor_property('build_in_editor'), unreal.PropertyAccessChangeNotifyMode.ALWAYS)


def rebuild_track(level=0):
    """Rebuild the track twice and confirm the signs come back identical. Returns (count per design, same?)."""
    a = track(level)
    rerun(a)
    first = layout(level)
    rerun(a)
    second = layout(level)
    counts = dict((k, len(v)) for k, v in sorted(first.items()))
    unreal.log('FTADS rebuild: %d signs %s identical=%s' % (sum(counts.values()), counts, first == second))
    return counts, first == second
