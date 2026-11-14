"""FULL THROTTLE art kit, run inside the Unreal editor (UE 5.9).

Builds flat-shaded models out of Geometry Script primitives, gives them a cel (toon) material with
three hard light bands, and films them with a SceneCapture2D into raw float buffers. The host-side
script tools/bake.py turns those buffers into outlined, cel-shaded PNG sprites for the web game.

Units are centimetres (UE), X forward, Y right, Z up.
"""
import math, os, struct, array
import unreal

ROOT = os.path.normpath(os.path.join(unreal.Paths.project_dir(), 'Saved', 'FTCaptures'))
MAT_DIR = '/Game/FT/Materials'
GS = unreal.GeometryScript_Primitives
AL = unreal.EditorAssetLibrary
MEL = unreal.MaterialEditingLibrary

# ---------------------------------------------------------------- materials
TOON_CODE = r"""
float3 n = normalize(N);
float3 l = normalize(L);
float d = dot(n, l);
float band = d > 0.45 ? 1.0 : (d > -0.15 ? 0.78 : 0.58);
float3 cool = float3(0.86, 0.84, 1.0);
float3 c = Col * band * lerp(cool, float3(1,1,1), band > 0.9 ? 1.0 : 0.0);
c = c + Col * Glow;
float h = saturate((D - HazeStart) / max(HazeEnd - HazeStart, 1.0));
h = h * h * (3.0 - 2.0 * h) * HazeMax;
return lerp(c, HazeCol, h);
"""


def _ensure_dir(p):
    if not AL.does_directory_exist(p):
        AL.make_directory(p)


def _param_vec(m, name, val, x, y):
    e = MEL.create_material_expression(m, unreal.MaterialExpressionVectorParameter, x, y)
    e.set_editor_property('parameter_name', name)
    e.set_editor_property('default_value', unreal.LinearColor(*val))
    return e


def _param_scalar(m, name, val, x, y):
    e = MEL.create_material_expression(m, unreal.MaterialExpressionScalarParameter, x, y)
    e.set_editor_property('parameter_name', name)
    e.set_editor_property('default_value', val)
    return e


def toon_master(rebuild=False):
    path = MAT_DIR + '/M_Toon'
    if AL.does_asset_exist(path) and not rebuild:
        return AL.load_asset(path)
    _ensure_dir(MAT_DIR)
    if AL.does_asset_exist(path):
        AL.delete_asset(path)
    m = unreal.AssetToolsHelpers.get_asset_tools().create_asset('M_Toon', MAT_DIR, unreal.Material, unreal.MaterialFactoryNew())
    m.set_editor_property('shading_model', unreal.MaterialShadingModel.MSM_UNLIT)
    m.set_editor_property('two_sided', True)
    cu = MEL.create_material_expression(m, unreal.MaterialExpressionCustom, -300, 0)
    cu.set_editor_property('code', TOON_CODE)
    cu.set_editor_property('output_type', unreal.CustomMaterialOutputType.CMOT_FLOAT3)
    names = ['N', 'L', 'Col', 'Glow', 'D', 'HazeStart', 'HazeEnd', 'HazeMax', 'HazeCol']
    ins = []
    for nm in names:
        ci = unreal.CustomInput()
        ci.set_editor_property('input_name', nm)
        ins.append(ci)
    cu.set_editor_property('inputs', ins)
    n = MEL.create_material_expression(m, unreal.MaterialExpressionVertexNormalWS, -700, -200)
    l = _param_vec(m, 'LightDir', (-0.55, -0.35, 0.76, 0), -700, -120)
    col = _param_vec(m, 'Color', (1, 0, 1, 1), -700, -40)
    glow = _param_scalar(m, 'Glow', 0.0, -700, 40)
    dep = MEL.create_material_expression(m, unreal.MaterialExpressionPixelDepth, -700, 120)
    hs = _param_scalar(m, 'HazeStart', 1.0e7, -700, 200)
    he = _param_scalar(m, 'HazeEnd', 2.0e7, -700, 280)
    hm = _param_scalar(m, 'HazeMax', 0.0, -700, 360)
    hc = _param_vec(m, 'HazeColor', (0.85, 0.75, 0.9, 1), -700, 440)
    for src, nm in [(n, 'N'), (l, 'L'), (col, 'Col'), (glow, 'Glow'), (dep, 'D'), (hs, 'HazeStart'), (he, 'HazeEnd'), (hm, 'HazeMax'), (hc, 'HazeCol')]:
        MEL.connect_material_expressions(src, '', cu, nm)
    MEL.connect_material_property(cu, '', unreal.MaterialProperty.MP_EMISSIVE_COLOR)
    MEL.recompile_material(m)
    AL.save_loaded_asset(m)
    return m


_MI = {}
HAZE = {'start': 1.0e7, 'end': 2.0e7, 'max': 0.0, 'col': (0.85, 0.75, 0.9)}


def srgb_to_lin(v):
    v = v / 255.0
    return v / 12.92 if v <= 0.04045 else ((v + 0.055) / 1.055) ** 2.4


def hex_lin(h):
    h = h.lstrip('#')
    return tuple(srgb_to_lin(int(h[i:i + 2], 16)) for i in (0, 2, 4))


def mat(hexcol, glow=0.0):
    """A toon material instance for an sRGB hex colour (cached per colour, glow and haze)."""
    key = (hexcol, round(glow, 3), HAZE['start'], HAZE['end'], HAZE['max'], HAZE['col'])
    if key in _MI:
        return _MI[key]
    name = 'MI_%s_g%d_h%d' % (hexcol.lstrip('#'), int(glow * 100), abs(hash(key[2:])) % 100000)
    path = MAT_DIR + '/' + name
    if AL.does_asset_exist(path):
        mi = AL.load_asset(path)
    else:
        mi = unreal.AssetToolsHelpers.get_asset_tools().create_asset(name, MAT_DIR, unreal.MaterialInstanceConstant, unreal.MaterialInstanceConstantFactoryNew())
        mi.set_editor_property('parent', toon_master())
    r, g, b = hex_lin(hexcol)
    MEL.set_material_instance_vector_parameter_value(mi, 'Color', unreal.LinearColor(r, g, b, 1))
    MEL.set_material_instance_scalar_parameter_value(mi, 'Glow', glow)
    MEL.set_material_instance_scalar_parameter_value(mi, 'HazeStart', HAZE['start'])
    MEL.set_material_instance_scalar_parameter_value(mi, 'HazeEnd', HAZE['end'])
    MEL.set_material_instance_scalar_parameter_value(mi, 'HazeMax', HAZE['max'])
    hr, hg, hb = HAZE['col']
    MEL.set_material_instance_vector_parameter_value(mi, 'HazeColor', unreal.LinearColor(hr, hg, hb, 1))
    MEL.update_material_instance(mi)
    _MI[key] = mi
    return mi


# ---------------------------------------------------------------- models
def _xf(loc=(0, 0, 0), rot=(0, 0, 0), scale=(1, 1, 1)):
    # rot = (pitch, yaw, roll) in degrees
    return unreal.Transform(unreal.Vector(*loc), unreal.Rotator(roll=rot[2], pitch=rot[0], yaw=rot[1]), unreal.Vector(*scale))


class Model(object):
    """Collects primitives per colour; build() spawns one DynamicMeshActor per colour."""

    def __init__(self, name):
        self.name = name
        self.meshes = {}
        self.actors = []
        self.opts = unreal.GeometryScriptPrimitiveOptions()
        self.opts.set_editor_property('polygroup_mode', unreal.GeometryScriptPrimitivePolygroupMode.PER_FACE)
        self.base = None  # extra transform applied to every primitive (for instancing groups)
        self.part = None  # Unreal 2.0: name of the detachable part being drawn (None = the body)
        self.parts = {}

    def _mesh(self, col, glow=0.0):
        k = (col, glow)
        tgt = self.meshes if not self.part else self.parts.setdefault(self.part, {})
        if k not in tgt:
            tgt[k] = unreal.DynamicMesh()
        return tgt[k]

    def _t(self, loc, rot, scale=(1, 1, 1)):
        t = _xf(loc, rot, scale)
        if self.base is not None:
            t = t * self.base
        return t

    def box(self, col, center, size, rot=(0, 0, 0), glow=0.0):
        """size = (x, y, z); center is the middle of the box."""
        GS.append_box(self._mesh(col, glow), self.opts, self._t(center, rot), size[0], size[1], size[2], 0, 0, 0, unreal.GeometryScriptPrimitiveOriginMode.CENTER)

    def side(self, col, pts, y0, y1, glow=0.0, rot=(0, 0, 0), loc=(0, 0, 0)):
        """A side-profile polygon pts [(x, z), ...] (counter-clockwise seen from +Y... any winding) extruded across y0..y1."""
        poly = [unreal.Vector2D(x, z) for x, z in pts]
        # local XY polygon -> world XZ; local +Z extrusion -> world -Y. Start at y1, extrude to y0.
        t = _xf((loc[0], loc[1] + y1, loc[2]), (0, 0, -90), (1, 1, 1))
        if rot != (0, 0, 0):
            t = t * _xf((0, 0, 0), rot)
        if self.base is not None:
            t = t * self.base
        GS.append_simple_extrude_polygon(self._mesh(col, glow), self.opts, t, poly, y1 - y0, 0, True, unreal.GeometryScriptPrimitiveOriginMode.BASE)

    def plan(self, col, pts, z0, z1, glow=0.0, loc=(0, 0, 0), rot=(0, 0, 0)):
        """A top-view polygon pts [(x, y)] extruded up from z0 to z1."""
        poly = [unreal.Vector2D(x, y) for x, y in pts]
        GS.append_simple_extrude_polygon(self._mesh(col, glow), self.opts, self._t((loc[0], loc[1], loc[2] + z0), rot), poly, z1 - z0, 0, True, unreal.GeometryScriptPrimitiveOriginMode.BASE)

    def cyl(self, col, base, r, h, rot=(0, 0, 0), steps=16, glow=0.0, top_r=None):
        if top_r is None:
            GS.append_cylinder(self._mesh(col, glow), self.opts, self._t(base, rot), r, h, steps, 0, True, unreal.GeometryScriptPrimitiveOriginMode.BASE)
        else:
            GS.append_cone(self._mesh(col, glow), self.opts, self._t(base, rot), r, top_r, h, steps, 0, True, unreal.GeometryScriptPrimitiveOriginMode.BASE)

    def sphere(self, col, center, r, scale=(1, 1, 1), steps=(8, 12), glow=0.0, rot=(0, 0, 0)):
        GS.append_sphere_lat_long(self._mesh(col, glow), self.opts, self._t(center, rot, scale), r, steps[0], steps[1], unreal.GeometryScriptPrimitiveOriginMode.CENTER)

    def build(self, at=(0, 0, 0)):
        sub = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
        for (col, glow), m in self.meshes.items():
            a = sub.spawn_actor_from_class(unreal.DynamicMeshActor, unreal.Vector(*at))
            a.set_actor_label('%s_%s' % (self.name, col.lstrip('#')))
            comp = a.get_editor_property('dynamic_mesh_component')
            unreal.GeometryScript_Normals.set_per_face_normals(m)
            comp.set_dynamic_mesh(m)
            comp.set_material(0, mat(col, glow))
            comp.set_editor_property('cast_shadow', False)
            a.set_folder_path(unreal.Name('FT/' + self.name))
            self.actors.append(a)
        return self.actors

    def destroy(self):
        sub = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
        for a in self.actors:
            sub.destroy_actor(a)
        self.actors = []


def clear(prefix=''):
    sub = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
    n = 0
    for a in sub.get_all_level_actors():
        if isinstance(a, (unreal.DynamicMeshActor, unreal.SceneCapture2D)) and a.get_actor_label().startswith(prefix):
            sub.destroy_actor(a)
            n += 1
    return n


# ---------------------------------------------------------------- capture
_CAP = {}


def _capture_actor():
    sub = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
    for a in sub.get_all_level_actors():
        if isinstance(a, unreal.SceneCapture2D) and a.get_actor_label() == 'FT_Capture':
            return a
    a = sub.spawn_actor_from_class(unreal.SceneCapture2D, unreal.Vector(0, 0, 0))
    a.set_actor_label('FT_Capture')
    return a


def capture(actors, name, loc, rot, w, h, fov=None, ortho=None, depth=False):
    """Film only `actors` from loc/rot into Saved/FTCaptures/<name>.rgba (float32 RGBA, A = coverage)
    and optionally <name>.depth (float32). rot = (pitch, yaw, roll)."""
    world = unreal.get_editor_subsystem(unreal.UnrealEditorSubsystem).get_editor_world()
    cap = _capture_actor()
    cap.set_actor_location_and_rotation(unreal.Vector(*loc), unreal.Rotator(roll=rot[2], pitch=rot[0], yaw=rot[1]), False, False)
    cc = cap.get_editor_property('capture_component2d')
    key = (w, h)
    if key not in _CAP:
        _CAP[key] = (unreal.RenderingLibrary.create_render_target2d(world, w, h, unreal.TextureRenderTargetFormat.RTF_RGBA32F, unreal.LinearColor(0, 0, 0, 1)),
                     unreal.RenderingLibrary.create_render_target2d(world, w, h, unreal.TextureRenderTargetFormat.RTF_RGBA32F, unreal.LinearColor(0, 0, 0, 1)))
    rt, rtd = _CAP[key]
    if ortho:
        cc.set_editor_property('projection_type', unreal.CameraProjectionMode.ORTHOGRAPHIC)
        cc.set_editor_property('ortho_width', ortho)
    else:
        cc.set_editor_property('projection_type', unreal.CameraProjectionMode.PERSPECTIVE)
        cc.set_editor_property('fov_angle', fov or 30.0)
    cc.set_editor_property('primitive_render_mode', unreal.SceneCapturePrimitiveRenderMode.PRM_USE_SHOW_ONLY_LIST)
    cc.clear_show_only_components()
    for a in actors:
        cc.show_only_actor_components(a, False)
    cc.set_editor_property('capture_every_frame', False)
    cc.set_editor_property('capture_on_movement', False)
    cc.set_editor_property('always_persist_rendering_state', True)
    os.makedirs(ROOT, exist_ok=True)
    out = {}
    passes = [('rgba', rt, unreal.SceneCaptureSource.SCS_SCENE_COLOR_HDR)]
    if depth:
        passes.append(('depth', rtd, unreal.SceneCaptureSource.SCS_SCENE_DEPTH))
    for tag, target, src in passes:
        cc.set_editor_property('texture_target', target)
        cc.set_editor_property('capture_source', src)
        cc.capture_scene()
        px = unreal.RenderingLibrary.read_render_target_raw(world, target, False)
        buf = array.array('f')
        if tag == 'rgba':
            for p in px:
                buf.extend((p.r, p.g, p.b, 1.0 - p.a))
        else:
            for p in px:
                buf.append(p.r)
        fn = os.path.join(ROOT, '%s.%s' % (name, tag))
        with open(fn, 'wb') as f:
            f.write(struct.pack('<ii', w, h))
            buf.tofile(f)
        out[tag] = fn
    return out
