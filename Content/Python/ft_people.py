"""FULL THROTTLE (Unreal) the population: people, creatures and film crews (the population pass, 15 Nov 2026).

Looks only. Nothing made here has collision or a game rule, and the shared cel pass is untouched: M_People is an
ordinary lit material, so the cel pass bands and inks a figure like every other prop.

  M_People      ONE material for every population mesh, whatever its colours and however its parts move: a mesh made
                here has a single section (one draw call an instanced mesh). A part's colour and its way of moving are
                written into the mesh itself, the same for every vertex of the part, in the eight UV channels:
                  UV0 (r, g)   UV1 (b, glow)                the part's colour (linear) and how far it glows
                  UV2 (phase, rate)                         turns of phase (+10 = it livens up as the chase comes by)
                                                            and cycles a second
                  UV3 (bob, swing)                          cm it hops (>0: a hop, |sin|; <0: a float, sin) and degrees
                                                            it swings about its pivot
                  UV4 (pivot x, y)  UV5 (pivot z, axis)     the pivot, in the mesh's own space, and the swing's axis: an
                                                            angle in the ground plane in degrees, or 1000 for straight up
                  UV6 (base x, y)                           the figure's own place (travel) or the circle's centre (orbit)
                  UV7 (length, rate)                        >0: it travels a line that long along the mesh's X and comes
                                                            round again; <0: it circles the base. rate: rounds a second
                In the vertex shader, in this order: the swing (an arm waves, a leg strides, a wing beats), the bob (the
                whole figure hops or floats), the travel (a walker goes along the boardwalk, a flock wheels).
  M_PeopleFar   the same, always moving (the track switches vertex movement off past 180 m for roadside pieces).
  Scene         one population piece: a small scene of several figures and their furniture, saved as ONE static mesh.
  figure()      THE figure builder, used for every person of the game: chunky, big head and hands, flat palette colours,
                110 to 300 triangles. Build, height, age, clothes, hair, hats, what is in the hands, what it is doing.
  bird(), quadruped()   the creatures' two builders.

How a piece is drawn (the dressing's own rules, ft_dress.py): cm; the base stands on z = 0; the middle of the footprint
is the origin; the FRONT faces -Y (the placement turns -Y towards the road); long pieces run along X.

Run in the editor:  import ft_people; ft_people.build_core()      (the two masters; every kit calls it itself)
The kits: ft_people_crew.py (the film crew, every level) and ft_people_<level>.py; the placing rules are
Source/FullThrottle/FTPeopleL<n>.inl.
"""
import math
import random
import unreal
import ft_art
import ft_mats
from ft_desert import rod, tilt_box

AL = unreal.EditorAssetLibrary
MEL = unreal.MaterialEditingLibrary
UVS = unreal.GeometryScript_UVs
MQ = unreal.GeometryScript_MeshQueries
F1 = unreal.CustomMaterialOutputType.CMOT_FLOAT1
F3 = unreal.CustomMaterialOutputType.CMOT_FLOAT3

ROOT = '/Game/People'
CORE = ROOT + '/Core'
UP = 1000.0          # the axis code for "straight up"


def log(*a):
    unreal.log('FTPEOPLE ' + ' '.join(str(x) for x in a))


# ------------------------------------------------------------------------------------------ M_People
# What every custom node below starts with: the part's own numbers, read from its UVs, and how lively it is.
READ = r"""
float ph = UV2.x;
float react = step(9.5, ph);
ph -= react * 10.0;
float t = (T * UV2.y + ph + Rnd) * 6.2832;
float k = 1.0;
if (react > 0.5) { k = lerp(React.z, 1.0, 1.0 - smoothstep(React.x, React.y, length(CP - WP))); }
float3 piv = float3(UV4.x, UV4.y, UV5.x);
float3 ax = (UV5.y > 900.0) ? float3(0.0, 0.0, 1.0) : float3(cos(radians(UV5.y)), sin(radians(UV5.y)), 0.0);
float ang = radians(UV3.y) * sin(t) * k;
float ca = cos(ang), sa = sin(ang);
float a2 = (UV7.x < -0.5) ? (T * UV7.y + Rnd) * 6.2832 : 0.0;
float c2 = cos(a2), s2 = sin(a2);
"""
HLSL = {
    'Offset': (READ + r"""
float3 p = L - piv;
float3 q = p * ca + cross(ax, p) * sa + ax * (dot(ax, p) * (1.0 - ca)) + piv;
q.z += (UV3.x >= 0.0) ? UV3.x * abs(sin(t)) * k : -UV3.x * sin(t);
if (UV7.x > 0.5)
{
    float f = frac(UV6.x / UV7.x + 0.5 + T * UV7.y + Rnd);
    float ends = smoothstep(0.0, 0.02, f) * (1.0 - smoothstep(0.98, 1.0, f));   // nothing pops at the ends
    q = piv + (q - piv) * ends;
    q.x += (f - 0.5) * UV7.x - UV6.x;
}
else if (UV7.x < -0.5)
{
    float2 r = q.xy - UV6.xy;
    q.xy = UV6.xy + float2(r.x * c2 - r.y * s2, r.x * s2 + r.y * c2);
}
return q - L;
""", ['L', 'WP', 'CP', 'T', 'Rnd', 'React', 'UV2', 'UV3', 'UV4', 'UV5', 'UV6', 'UV7']),
    'Normal': (READ + r"""
float3 n = Nl * ca + cross(ax, Nl) * sa + ax * (dot(ax, Nl) * (1.0 - ca));
n.xy = float2(n.x * c2 - n.y * s2, n.x * s2 + n.y * c2);
return n;
""", ['Nl', 'WP', 'CP', 'T', 'Rnd', 'React', 'UV2', 'UV3', 'UV4', 'UV5', 'UV7']),
    'Colour': ("return float3(UV0.x, UV0.y, UV1.x);", ['UV0', 'UV1']),
    'Glow': ("return float3(UV0.x, UV0.y, UV1.x) * (UV1.y * Gain);", ['UV0', 'UV1', 'Gain']),
}


def _enum(cls, *names):
    for n in names:
        if hasattr(cls, n):
            return getattr(cls, n)
    raise RuntimeError('none of %s on %s' % (names, cls))


def _connect(a, out, b, pins):
    for pin in pins:
        if MEL.connect_material_expressions(a, out, b, pin):
            return True
    raise RuntimeError('could not connect to any of %s' % (pins,))


def _master(name, always=False):
    m = ft_mats.new_material(CORE, name, blend=unreal.BlendMode.BLEND_OPAQUE, shading=unreal.MaterialShadingModel.MSM_DEFAULT_LIT)
    m.set_editor_property('used_with_instanced_static_meshes', True)
    m.set_editor_property('tangent_space_normal', False)
    try:
        m.set_editor_property('always_evaluate_world_position_offset', always)
    except Exception as e:
        unreal.log_warning('FTPEOPLE %s: could not set always_evaluate_world_position_offset (%s)' % (name, e))
    X = -1500
    react = ft_mats.vparam(m, 'React', (2500.0, 11000.0, 0.3, 0.0), x=X, y=-900)   # full within 25 m, at rest past 110 m (30 %)
    gain = ft_mats.sparam(m, 'GlowGain', 10.5, x=X, y=-800)
    rough = ft_mats.sparam(m, 'Rough', 0.78, x=X, y=-700)
    lp = ft_mats.expr(m, unreal.MaterialExpressionLocalPosition, x=X, y=-600)
    t = ft_mats.expr(m, unreal.MaterialExpressionTime, x=X, y=-500)
    rnd = ft_mats.expr(m, unreal.MaterialExpressionPerInstanceRandom, x=X, y=-400)
    wp = ft_mats.expr(m, unreal.MaterialExpressionWorldPosition, x=X, y=-300)
    try:
        wp.set_editor_property('world_position_shader_offset', _enum(unreal.WorldPositionIncludedOffsets, 'WPT_EXCLUDE_ALL_SHADER_OFFSETS'))
    except Exception as e:
        unreal.log_warning('FTPEOPLE world position keeps its offsets (%s)' % e)
    cp = ft_mats.expr(m, unreal.MaterialExpressionCameraPositionWS, x=X, y=-200)
    nws = ft_mats.expr(m, unreal.MaterialExpressionVertexNormalWS, x=X, y=-100)
    uv = []
    for i in range(8):
        e = ft_mats.expr(m, unreal.MaterialExpressionTextureCoordinate, x=X, y=i * 90)
        e.set_editor_property('coordinate_index', i)
        uv.append(e)
    SRC, DST = unreal.MaterialVectorCoordTransformSource, unreal.MaterialVectorCoordTransform

    def xform(src, a, b, y):
        e = ft_mats.expr(m, unreal.MaterialExpressionTransform, x=X + 900, y=y)
        e.set_editor_property('transform_source_type', _enum(SRC, 'TRANSFORMSOURCE_' + a, 'TRANSFORMSOURCE_LOCAL'))
        e.set_editor_property('transform_type', _enum(DST, 'TRANSFORM_' + b, 'TRANSFORM_LOCAL'))
        _connect(src, '', e, ('Input', ''))
        return e

    def interp(src, y):
        e = ft_mats.expr(m, unreal.MaterialExpressionVertexInterpolator, x=X + 1200, y=y)
        _connect(src, '', e, ('Input', ''))
        return e

    n_l = xform(nws, 'WORLD', 'INSTANCE', -100)
    src = {'L': lp, 'T': t, 'Rnd': rnd, 'WP': wp, 'CP': cp, 'Nl': n_l, 'React': react, 'Gain': gain}
    for i in range(8):
        src['UV%d' % i] = uv[i]

    def cu(key, y, out=F3):
        code, names = HLSL[key]
        return ft_mats.custom(m, code, [(n, src[n], '') for n in names], out_type=out, x=X + 600, y=y, desc='People' + key)

    MEL.connect_material_property(xform(cu('Offset', -600), 'INSTANCE', 'WORLD', -600), '', unreal.MaterialProperty.MP_WORLD_POSITION_OFFSET)
    n_w = xform(cu('Normal', -300), 'INSTANCE', 'WORLD', -300)
    n_n = ft_mats.custom(m, 'return normalize(V);', [('V', n_w, '')], x=X + 1050, y=-300, desc='PeopleNormalise')
    MEL.connect_material_property(interp(n_n, -300), '', unreal.MaterialProperty.MP_NORMAL)
    MEL.connect_material_property(interp(cu('Colour', 0), 0), '', unreal.MaterialProperty.MP_BASE_COLOR)
    MEL.connect_material_property(interp(cu('Glow', 200), 200), '', unreal.MaterialProperty.MP_EMISSIVE_COLOR)
    MEL.connect_material_property(rough, '', unreal.MaterialProperty.MP_ROUGHNESS)
    errs = list(MEL.recompile_material(m) or [])
    if errs:
        raise RuntimeError('%s did not compile: %s' % (name, errs))
    AL.save_loaded_asset(m)
    try:
        st = MEL.get_statistics(m)
        log('master', name, 'PS', st.get_editor_property('num_pixel_shader_instructions'), 'VS', st.get_editor_property('num_vertex_shader_instructions'))
    except Exception as e:
        log('master', name, '(no statistics: %s)' % e)
    return m


def build_core(rebuild=False):
    """The two masters. A kit calls this first; they are rebuilt only when asked (rebuild=True) or missing."""
    out = []
    for name, always in (('M_People', False), ('M_PeopleFar', True)):
        path = CORE + '/' + name
        out.append(_master(name, always) if (rebuild or not AL.does_asset_exist(path)) else AL.load_asset(path))
    return out


# ------------------------------------------------------------------------------------------ a piece
def _const_uv(mesh, channel, u, v):
    """Every vertex of a part gets the same UV in a channel (ft_life's way: a projection from a plane ten thousand
    kilometres wide gives one value to the whole part; that value is then slid to (u, v))."""
    sel = unreal.GeometryScriptMeshSelection()
    plane = unreal.Transform(unreal.Vector(0, 0, 0), unreal.Rotator(0, 0, 0), unreal.Vector(1.0e9, 1.0e9, 1.0e9))
    UVS.set_mesh_u_vs_from_planar_projection(mesh, channel, plane, sel)
    r = MQ.get_triangle_u_vs(mesh, channel, 0)
    a = r[0] if isinstance(r, tuple) else r
    UVS.translate_mesh_u_vs(mesh, channel, unreal.Vector2D(u - a.x, v - a.y), sel)


STILL = dict(phase=0.0, rate=0.0, bob=0.0, swing=0.0, pivot=(0.0, 0.0, 0.0), axis=0.0, base=(0.0, 0.0), travel=(0.0, 0.0), react=False)


def motion(**kw):
    """A part's way of moving (see the header). phase: turns; rate: cycles a second; bob: cm (>0 hop, <0 float);
    swing: degrees about pivot (x, y, z in the mesh's space) round axis (degrees in the ground plane, or UP);
    travel: (length cm along the mesh's X, rounds a second: negative goes towards -X) with base = the figure's place;
    orbit: rounds a second about base (give travel=(-1, rate)); react: livelier as the chase comes by."""
    mo = dict(STILL)
    for k2, v in kw.items():
        if k2 not in mo:
            raise ValueError('no such motion value %r' % k2)
        mo[k2] = v
    return mo


class Scene(object):
    """One population piece. Draw into the models that part() and still() hand out, with ft_art.Model's box, cyl,
    sphere, side, plan (and rod, tilt_box); figure(), bird() and quadruped() draw whole creatures into it."""

    def __init__(self):
        self.parts = {}
        self.order = []

    def part(self, mo=None, at=(0.0, 0.0, 0.0), yaw=0.0, s=1.0):
        """The model for parts that move one way. at / yaw / s: where the next primitives are drawn (a figure's own
        frame); the motion's pivot and base are always in the MESH's space."""
        mo = mo or STILL
        key = (round(mo['phase'], 4), round(mo['rate'], 4), round(mo['bob'], 3), round(mo['swing'], 3), tuple(round(v, 2) for v in mo['pivot']),
               round(mo['axis'], 2), tuple(round(v, 2) for v in mo['base']), tuple(round(v, 5) for v in mo['travel']), bool(mo['react']))
        if key not in self.parts:
            self.parts[key] = (ft_art.Model('people'), dict(mo))
            self.order.append(key)
        m = self.parts[key][0]
        m.base = ft_art._xf(at, (0.0, yaw, 0.0), (s, s, s))
        return m

    def still(self, at=(0.0, 0.0, 0.0), yaw=0.0, s=1.0):
        """The model for everything that does not move (furniture, fences, the ground a scene stands on)."""
        return self.part(None, at, yaw, s)

    def save(self, path, far=False, grow=(60.0, 60.0, 60.0), grow_down=(60.0, 60.0, 0.0)):
        """Save as one static mesh with one section. far: the always-moving master (a piece meant for 180 m and
        beyond that must keep moving). grow / grow_down: how far (cm) the parts move beyond the mesh as built."""
        build_core()
        combined = unreal.DynamicMesh()
        UVS.set_num_uv_sets(combined, 8)
        n_parts = 0
        for key in self.order:
            model, mo = self.parts[key]
            for (col, glow), pm in model.meshes.items():
                if MQ.get_num_triangle_i_ds(pm) == 0:
                    continue
                UVS.set_num_uv_sets(pm, 8)
                r, g, b = ft_art.hex_lin(col)
                tl, tr = mo['travel']
                vals = ((r, g), (b, glow), (mo['phase'] % 1.0 + (10.0 if mo['react'] else 0.0), mo['rate']), (mo['bob'], mo['swing']),
                        (mo['pivot'][0], mo['pivot'][1]), (mo['pivot'][2], mo['axis']), (mo['base'][0], mo['base'][1]), (tl, tr))
                for ch, (u, v) in enumerate(vals):
                    _const_uv(pm, ch, float(u), float(v))
                unreal.GeometryScript_Normals.set_per_face_normals(pm)
                unreal.GeometryScript_MeshEdits.append_mesh(combined, pm, unreal.Transform())
                n_parts += 1
        tris = MQ.get_num_triangle_i_ds(combined)
        if tris == 0:
            unreal.log_warning('FTPEOPLE %s drew nothing' % path)
            return None
        mat = AL.load_asset(CORE + ('/M_PeopleFar' if far else '/M_People'))
        folder = path.rsplit('/', 1)[0]
        if not AL.does_directory_exist(folder):
            AL.make_directory(folder)
        if AL.does_asset_exist(path):
            sm = AL.load_asset(path)
            opts = unreal.GeometryScriptCopyMeshToAssetOptions()
            opts.enable_recompute_normals = False
            opts.replace_materials = True
            opts.new_materials = [mat]
            opts.new_material_slot_names = [unreal.Name('People')]
            opts.emit_transaction = False
            unreal.GeometryScript_AssetUtils.copy_mesh_to_static_mesh(combined, sm, opts, unreal.GeometryScriptMeshWriteLOD())
        else:
            opts = unreal.GeometryScriptCreateNewStaticMeshAssetOptions()
            opts.set_editor_property('enable_nanite', False)
            opts.set_editor_property('enable_collision', False)
            opts.set_editor_property('enable_recompute_normals', False)
            sm, _ = unreal.GeometryScript_NewAssetUtils.create_new_static_mesh_asset_from_mesh(combined, path, opts)
            if sm is None:
                raise RuntimeError('could not create ' + path)
            sm.set_editor_property('static_materials', [unreal.StaticMaterial(material_interface=mat, material_slot_name='People')])
        try:    # the pivots and the travel are read from UVs: keep them whole floats
            sub = unreal.get_editor_subsystem(unreal.StaticMeshEditorSubsystem)
            bs = sub.get_lod_build_settings(sm, 0)
            if not bs.get_editor_property('use_full_precision_u_vs'):
                bs.set_editor_property('use_full_precision_u_vs', True)
                bs.set_editor_property('generate_lightmap_u_vs', False)
                sub.set_lod_build_settings(sm, 0, bs)
        except Exception as e:
            unreal.log_warning('FTPEOPLE %s keeps half-float UVs (%s)' % (path, e))
        sm.set_editor_property('positive_bounds_extension', unreal.Vector(*grow))
        sm.set_editor_property('negative_bounds_extension', unreal.Vector(*grow_down))
        try:
            sm.set_editor_property('generate_mesh_distance_field', False)
        except Exception as e:
            unreal.log_warning('FTPEOPLE %s keeps its distance field (%s)' % (path, e))
        AL.save_loaded_asset(sm)
        bb = MQ.get_mesh_bounding_box(combined)      # as built: the asset's own bounds are grown for the movement
        top = bb.max.z / 100.0
        log('piece', path.rsplit('/', 1)[-1], 'triangles', tris, 'parts', n_parts, 'size m', round((bb.max.x - bb.min.x) / 100.0, 1),
            round((bb.max.y - bb.min.y) / 100.0, 1), 'top m', round(top, 2), 'TALL: 24 m or more from the road' if top > 2.0 else 'low')
        return sm


def export(folder, jobs, only=None):
    """jobs: [(name, build, options)] where build(scene) draws the piece and options go to Scene.save (far=True ...).
    Saves <folder>/<name>. A piece that fails is logged and left out; the rest of the kit is still built."""
    done, failed = [], []
    for job in jobs:
        name, fn = job[0], job[1]
        opts = job[2] if len(job) > 2 else {}
        if only and name not in only:
            continue
        try:
            sc = Scene()
            fn(sc)
            if sc.save(folder + '/' + name, **opts) is not None:
                done.append(name)
        except Exception as e:  # noqa
            import traceback
            unreal.log_error('FTPEOPLE %s/%s FAILED: %r %s' % (folder, name, e, traceback.format_exc()[-1200:]))
            failed.append(name)
    log('kit', folder, 'pieces built', len(done), 'failed', len(failed), ' '.join(failed))
    return done


# ------------------------------------------------------------------------------------------ the palette
# Skins, hair and the clothes most people wear. A level's kit adds its own (uniforms, ski suits, wetsuits).
SKINS = ('#f6d3b4', '#eebf98', '#d9a070', '#b87a50', '#8e5a3a', '#5e3b28')
HAIRS = ('#2a2230', '#3b2a22', '#6a4428', '#a8703a', '#e8c26a', '#c94f2e', '#cfcfd8', '#f4f1ea')
TOPS = ('#ff5fa2', '#2ee6ff', '#ffc93c', '#7dffb0', '#fff6ea', '#ff7a3c', '#8f6bff', '#3a5fa8', '#ff2d3c', '#57c7ff', '#f4a6ff', '#b8ff5a')
BOTTOMS = ('#2b2350', '#3a5fa8', '#f1e4c8', '#5a4a3c', '#22202a', '#7a8fb8', '#c94f6a', '#e8e0f0')
SHOES = ('#22202a', '#fff6ea', '#ff2d3c', '#5a4a3c')
EYE = '#22202a'

BUILDS = {  # shoulder width, hip width, depth, head radius
    'avg': (46.0, 34.0, 26.0, 24.0),
    'slim': (40.0, 30.0, 22.0, 23.0),
    'stocky': (56.0, 44.0, 33.0, 25.0),
    'kid': (40.0, 30.0, 24.0, 28.0),    # use with s about 0.66: a big head on a small body
}
# an arm's pose: (upper arm, forearm), each (lift from hanging in degrees, turn towards the front in degrees)
ARMS = {
    'down': ((7, 0), (9, 40)),
    'up': ((162, 0), (174, 0)),
    'vee': ((138, 0), (150, 0)),
    'wave': ((125, 10), (168, 0)),
    'out': ((90, 0), (90, 0)),
    'fwd': ((82, 78), (88, 86)),         # pointing ahead
    'point': ((95, 55), (98, 60)),       # pointing ahead and out
    'hip': ((38, -25), (72, 150)),
    'hold': ((22, 55), (98, 122)),       # both hands at the chest: a camera, a phone, a tray
    'face': ((58, 72), (165, 110)),      # a hand at the face: a phone call, binoculars, a drink, shading the eyes
    'carry': ((10, 35), (78, 95)),       # forearms level in front: a box, a crate
    'push': ((60, 88), (80, 90)),
    'back': ((20, -50), (30, -60)),
}


def _dir(lift, fwd, side):
    a, f = math.radians(lift), math.radians(fwd)
    return (side * math.sin(a) * math.cos(f), -math.sin(a) * math.sin(f), -math.cos(a))


def look(r, tops=None, bottoms=None, **fixed):
    """A random person: skin, hair, clothes, build. fixed: any of figure()'s own look values, kept as given."""
    d = dict(skin=r.choice(SKINS), hair_col=r.choice(HAIRS), top=r.choice(tops or TOPS), bottom=r.choice(bottoms or BOTTOMS),
             shoes=r.choice(SHOES), build=r.choice(('avg', 'avg', 'slim', 'stocky')), hair=r.choice(('short', 'short', 'long', 'bun', 'afro', 'pony', 'bald')),
             s=r.uniform(0.94, 1.07))
    d.update(fixed)
    return d


# ------------------------------------------------------------------------------------------ the figure
def figure(sc, x, y, z=0.0, yaw=0.0, s=1.0, skin=SKINS[1], top=TOPS[0], bottom=BOTTOMS[0], shoes=SHOES[0], hair='short', hair_col=HAIRS[0],
           build='avg', sleeves='short', legs='stand', wear='trousers', hat=None, hat_col='#ff2d3c', band=None, arm_l='down', arm_r='down',
           hold_l=None, hold_r=None, detail=2, seat=46.0,
           phase=0.0, rate=0.0, bob=0.0, react=False, swing_l=0.0, swing_r=0.0, axis_l='y', axis_r='y', stride=0.0, rock=0.0, turn=0.0,
           travel=None, orbit=None, sway=None, pack=None):
    """One person, standing on (x, y, z), facing -Y at yaw 0 (yaw turns it; front = towards the road as placed).

    Look     skin, top, bottom, shoes, hair_col, hat_col: palette colours. build: 'avg', 'slim', 'stocky', 'kid'
             (a child: s about 0.66). s: size (1 = 1.72 m). hair: 'short', 'long', 'bun', 'afro', 'pony', 'bald',
             'mohawk'. hat: None, 'cap', 'hard', 'cowboy', 'beanie', 'sun', 'helmet', 'chef', 'peak', 'headset'.
             sleeves: 'short', 'long', 'none'. wear: 'trousers', 'shorts', 'skirt', 'dress', 'swim', 'coat'.
             band: a colour for a band across the chest (hi-vis, a sash, a logo-less stripe).
             pack: a colour for a pack on the back (a rucksack, an air tank, a school bag).
             detail: 2 by the road (hands, feet, eyes, bent arms: about 250 triangles), 1 middle distance (about
             200), 0 far or in a big crowd (about 100: a grandstand of thirty).
    Pose     legs: 'stand', 'wide', 'sit' (on a seat `seat` cm high: the body is drawn that much lower), 'stride'
             (one foot ahead). arm_l / arm_r: a name from ARMS, or ((lift, fwd), (lift, fwd)) for upper arm and forearm.
             hold_l / hold_r: draw what the hand holds: f(model, hand, side) with hand = (x, y, z) in the figure's
             frame; it moves with the arm.
    Moving   rate (cycles a second) and phase (turns: give each figure of a crowd its own) drive everything.
             bob: cm (>0 the figure hops, <0 it floats); react: livelier as the chase comes by.
             swing_l / swing_r: degrees an arm swings, about axis_l / axis_r: 'y' (side to side: a wave), 'x' (fore and
             aft: a walk, a hammer), 'z' (round the body). stride: degrees the legs swing fore and aft (opposite ways).
             rock: degrees the whole figure rocks side to side about its feet; turn: degrees it twists (a dance).
             sway: dict(swing=degrees, pivot=(x, y, z) in the MESH's space, axis=degrees or UP): the whole figure
             swings about any pivot (a camera operator panning round the tripod, a rider on a rocking thing; a pivot
             60 m under the ground and 3 degrees slides it 3 m each way along a rail).
             (rock, turn and sway replace the limbs' own swings.)
             travel: (length cm, speed cm/s): it walks along the mesh's X (face it that way: yaw 90 for +X, -90 for
             -X with a negative speed) and comes round again. orbit: (centre x, centre y, rounds a second).
    Returns the top of the head (z, in the mesh's space)."""
    SW, HW, D, HR = BUILDS[build]
    HIP, SH = 66.0, 120.0
    drop = (HIP - seat) if legs == 'sit' else 0.0
    cy, sy = math.cos(math.radians(yaw)), math.sin(math.radians(yaw))

    def W(p):   # the figure's frame -> the mesh's
        return (x + s * (p[0] * cy - p[1] * sy), y + s * (p[0] * sy + p[1] * cy), z + s * p[2])

    tv = (0.0, 0.0)
    base = (x, y)
    if travel:
        tv = (float(travel[0]), float(travel[1]) / float(travel[0]))
    elif orbit:
        base = (orbit[0], orbit[1])
        tv = (-1.0, float(orbit[2]))
    common = dict(phase=phase, rate=rate, bob=bob, react=react, base=base, travel=tv)
    whole = dict(sway) if sway else None
    if whole:
        pass
    elif rock:
        whole = dict(swing=rock, pivot=W((0, 0, 0)), axis=yaw + 90.0)
    elif turn:
        whole = dict(swing=turn, pivot=W((0, 0, 0)), axis=UP)

    def part(swing=0.0, pivot=(0, 0, 100), axis='x'):
        mo = dict(common)
        if whole:
            mo.update(whole)
        else:
            mo.update(swing=swing, pivot=W(pivot), axis=(UP if axis == 'z' else yaw + (90.0 if axis == 'y' else 0.0)))
        return sc.part(motion(**mo), (x, y, z), yaw, s)

    body = part()
    bz = -drop
    # ---- legs
    leg_col = bottom if wear in ('trousers', 'coat') else skin
    fx = HW / 2.0 - 8.0 if legs != 'wide' else HW / 2.0 + 4.0
    lw = 15.0 if build != 'stocky' else 19.0
    for side in (-1, 1):
        m = part(side * stride, (side * (HW / 2.0 - 8.0), 0, HIP), 'x') if stride and legs != 'sit' else body
        hx = side * (HW / 2.0 - 8.0)
        if legs == 'sit':
            knee = (hx, -40.0, seat + 4.0)
            tilt_box(m, bottom if wear in ('trousers', 'coat', 'shorts') else leg_col, (hx, 4.0, seat + 6.0), knee, lw, lw + 1)
            tilt_box(m, leg_col, (hx, -40.0, seat + 6.0), (hx, -42.0, 6.0), lw - 2, lw - 2)
            foot = (hx, -48.0, 5.0)
        else:
            fy = (-14.0 * side) if legs == 'stride' else 0.0
            ft = (side * fx, fy, 0.0)
            if wear == 'shorts':
                mid = ((hx + ft[0]) / 2.0, fy / 2.0, HIP * 0.55)
                tilt_box(m, bottom, (hx, 0, HIP + 2), mid, lw + 2, lw + 3)
                tilt_box(m, skin, mid, (ft[0], fy, 8.0), lw - 3, lw - 2)
            else:
                tilt_box(m, leg_col, (hx, 0, HIP + 2), (ft[0], fy, 8.0), lw, lw + 2)
            foot = (ft[0], fy - 5.0, 5.0)
        if detail >= 1:
            m.box(shoes, foot, (lw + 1, 27.0, 10.0))
    if wear in ('skirt', 'dress') and legs != 'sit':
        body.cyl(bottom if wear == 'skirt' else top, (0, 0, 36.0), HW / 2.0 + 9.0, HIP - 30.0, steps=8, top_r=HW / 2.0 + 1.0)
    elif wear == 'coat':
        body.cyl(top, (0, 0, 40.0), HW / 2.0 + 8.0, HIP - 36.0, steps=8, top_r=HW / 2.0 + 3.0)
    elif wear == 'swim':
        body.box(bottom, (0, 0, bz + HIP - 2.0), (HW + 2, D + 2, 20.0))
    # ---- body
    tcol = skin if (sleeves == 'none' and wear == 'swim' and top is None) else top
    body.box(tcol, (0, 0, bz + (HIP + SH) / 2.0 + 2.0), (SW, D, SH - HIP + 4.0))
    if band:
        body.box(band, (0, 0, bz + SH - 22.0), (SW + 2.0, D + 2.0, 9.0))
    if pack:
        body.box(pack, (0, D / 2.0 + 7.0, bz + SH - 24.0), (SW * 0.62, 15.0, 40.0))
    # ---- head
    hz = bz + SH + HR + 3.0
    # a round head with a flat crown and a narrower jaw (a sphere this coarse comes to a point at the top)
    if detail >= 1:
        body.cyl(skin, (0, 0, hz - HR), HR * 0.96, HR * 1.62, steps=8)
        body.cyl(skin, (0, 0, hz + HR * 0.62), HR * 0.96, HR * 0.38, steps=8, top_r=HR * 0.66)
    else:
        body.cyl(skin, (0, 0, hz - HR), HR * 0.92, HR * 2.0, steps=6)
    if detail >= 2:
        for side in (-1, 1):
            body.box(EYE, (side * HR * 0.38, -HR * 0.9, hz + 1.0), (3.6, 4.0, 5.2))
    top_z = hz + HR
    if hair == 'short':
        body.box(hair_col, (0, 3.0, hz + HR * 0.62), (HR * 1.86, HR * 1.8, HR * 0.8))
    elif hair == 'long':
        body.box(hair_col, (0, 3.0, hz + HR * 0.62), (HR * 1.9, HR * 1.8, HR * 0.8))
        body.box(hair_col, (0, HR * 0.72, hz - HR * 0.6), (HR * 1.9, HR * 0.7, HR * 2.2))
    elif hair == 'bun':
        body.box(hair_col, (0, 3.0, hz + HR * 0.62), (HR * 1.86, HR * 1.8, HR * 0.8))
        body.sphere(hair_col, (0, HR * 0.5, hz + HR * 1.2), HR * 0.45, steps=(3, 6))
        top_z += HR * 0.6
    elif hair == 'afro':
        body.sphere(hair_col, (0, HR * 0.28, hz + HR * 0.42), HR * 1.22, steps=(4, 8))
        top_z += HR * 0.6
    elif hair == 'pony':
        body.box(hair_col, (0, 3.0, hz + HR * 0.62), (HR * 1.86, HR * 1.8, HR * 0.8))
        tilt_box(body, hair_col, (0, HR * 0.9, hz + HR * 0.4), (0, HR * 1.7, hz - HR * 0.8), HR * 0.5, HR * 0.5)
    elif hair == 'mohawk':
        body.box(hair_col, (0, 2.0, hz + HR * 0.95), (HR * 0.4, HR * 1.9, HR * 0.9))
        top_z += HR * 0.4
    if hat == 'cap':
        body.cyl(hat_col, (0, 0, hz + HR * 0.42), HR * 1.04, HR * 0.66, steps=8, top_r=HR * 0.8)
        body.box(hat_col, (0, -HR * 1.2, hz + HR * 0.48), (HR * 1.3, HR * 0.9, 4.0))
    elif hat == 'peak':   # a uniform cap: a flat top and a dark peak
        body.cyl(hat_col, (0, 0, hz + HR * 0.45), HR * 1.02, HR * 0.5, steps=8, top_r=HR * 1.2)
        body.box(EYE, (0, -HR * 1.1, hz + HR * 0.47), (HR * 1.4, HR * 0.7, 4.0))
    elif hat == 'hard':
        body.cyl(hat_col, (0, 0, hz + HR * 0.36), HR * 1.3, 4.0, steps=8)
        body.cyl(hat_col, (0, 0, hz + HR * 0.4), HR * 1.08, HR * 0.78, steps=8, top_r=HR * 0.7)
        top_z += 4.0
    elif hat == 'cowboy':
        body.cyl(hat_col, (0, 0, hz + HR * 0.5), HR * 1.85, 4.0, steps=8)
        body.cyl(hat_col, (0, 0, hz + HR * 0.55), HR * 0.95, HR * 0.8, steps=8, top_r=HR * 0.8)
        top_z += HR * 0.4
    elif hat == 'sun':
        body.cyl(hat_col, (0, 0, hz + HR * 0.45), HR * 2.0, 3.5, steps=8)
        body.cyl(hat_col, (0, 0, hz + HR * 0.5), HR * 1.0, HR * 0.55, steps=8, top_r=HR * 0.85)
    elif hat == 'beanie':
        body.cyl(hat_col, (0, 0, hz + HR * 0.3), HR * 1.06, HR * 0.8, steps=8, top_r=HR * 0.7)
        body.sphere('#fff6ea', (0, 0, hz + HR * 1.2), HR * 0.3, steps=(3, 5))
        top_z += HR * 0.5
    elif hat == 'helmet':
        body.sphere(hat_col, (0, 1.0, hz + HR * 0.18), HR * 1.16, steps=(4, 8))
        body.box(EYE, (0, -HR * 0.98, hz + 2.0), (HR * 1.5, 5.0, HR * 0.5))
        top_z += HR * 0.35
    elif hat == 'chef':
        body.cyl(hat_col, (0, 0, hz + HR * 0.5), HR * 0.95, HR * 1.3, steps=8, top_r=HR * 1.15)
        top_z += HR * 0.9
    elif hat == 'headset':
        body.box(hat_col, (0, 0, hz + HR * 1.0), (HR * 2.2, 5.0, 4.0))
        for side in (-1, 1):
            body.box(hat_col, (side * HR * 1.05, 0, hz), (6.0, 12.0, 15.0))
        body.box(hat_col, (HR * 0.8, -HR * 0.7, hz - HR * 0.45), (3.0, HR * 1.0, 3.0))
    # ---- arms
    aw = 12.0 if build != 'stocky' else 15.0
    for side, pose, hold, swing, axis in ((-1, arm_l, hold_l, swing_l, axis_l), (1, arm_r, hold_r, swing_r, axis_r)):
        up, low = ARMS[pose] if isinstance(pose, str) else pose
        sh = (side * (SW / 2.0 + aw * 0.35), 0.0, bz + SH - 5.0)
        m = part(swing, sh, axis) if swing else body
        d1, d2 = _dir(up[0], up[1], side), _dir(low[0], low[1], side)
        el = (sh[0] + d1[0] * 27.0, sh[1] + d1[1] * 27.0, sh[2] + d1[2] * 27.0)
        hand = (el[0] + d2[0] * 27.0, el[1] + d2[1] * 27.0, el[2] + d2[2] * 27.0)
        c1 = top if sleeves in ('short', 'long') else skin
        c2 = top if sleeves == 'long' else skin
        straight = (d1[0] * d2[0] + d1[1] * d2[1] + d1[2] * d2[2]) > 0.93
        if detail >= 1 and not (straight and c1 == c2):
            tilt_box(m, c1, sh, el, aw, aw)
            tilt_box(m, c2, el, hand, aw - 2.0, aw - 2.0)
        else:
            tilt_box(m, c1, sh, hand, aw, aw)
        if detail >= 2:
            m.box(skin, hand, (15.0, 15.0, 15.0))
        if hold:
            hold(m, hand, side)
    return z + s * top_z


def crowd_line(sc, r, n, length, y=0.0, depth=60.0, yaw=0.0, kid_every=5, acts=None, **kw):
    """n people along X, two deep, each a random person out of step with the next. acts: a list of dicts of
    figure() values to pick from for each (poses, props); kw: figure() values for all of them."""
    acts = acts or [dict()]
    for i in range(n):
        d = look(r)
        if kid_every and i % kid_every == kid_every - 2:
            d.update(build='kid', s=r.uniform(0.6, 0.7))
        d.update(kw)
        d.update(r.choice(acts))
        px = -length / 2.0 + (i + 0.5) * length / n + r.uniform(-12, 12)
        py = y + (depth / 2.0 if i % 2 else -depth / 2.0) + r.uniform(-10, 10)
        d.setdefault('phase', r.random())
        figure(sc, px, py, yaw=yaw + r.uniform(-14, 14), **d)


# ------------------------------------------------------------------------------------------ what a hand holds
def hold_phone(col='#22202a', screen='#9ff0ff', glow=0.0):
    """glow: only on a piece that stands 24 m or more from the road (the light rule)."""
    def f(m, h, side):
        m.box(col, (h[0], h[1] - 4, h[2] + 11), (9, 3, 17))
        m.box(screen, (h[0], h[1] - 2.2, h[2] + 11), (7, 1, 14), glow=glow)
    return f


def hold_camera(col='#22202a', lens='#8f9bb3'):
    def f(m, h, side):
        m.box(col, (h[0] - side * 8, h[1] - 4, h[2] + 8), (26, 14, 17))
        m.cyl(lens, (h[0] - side * 8, h[1] - 10, h[2] + 8), 6.5, 10, rot=(0, 0, 90), steps=6)
    return f


def hold_sign(col='#fff6ea', stripe='#ff2d3c', w=70.0, h2=46.0, pole='#8f6b4a'):
    """A placard on a stick, a stripe across it (never words, never a real mark)."""
    def f(m, h, side):
        m.box(pole, (h[0], h[1], h[2] + 40), (4, 4, 96))
        m.box(col, (h[0], h[1] - 2, h[2] + 96), (w, 4, h2))
        m.box(stripe, (h[0], h[1] - 4.4, h[2] + 96), (w * 0.72, 1.5, h2 * 0.3))
    return f


def hold_flag(col='#ff2d3c', pole='#c9c4d6', w=60.0):
    def f(m, h, side):
        m.box(pole, (h[0], h[1], h[2] + 45), (3.5, 3.5, 110))
        m.box(col, (h[0] + side * w / 2.0, h[1], h[2] + 84), (w, 2.5, 32))
    return f


def hold_cup(col='#fff6ea'):
    def f(m, h, side):
        m.cyl(col, (h[0], h[1] - 2, h[2] + 4), 5.5, 15, steps=6, top_r=7.0)
    return f


def hold_bag(col='#ff5fa2'):
    def f(m, h, side):
        m.box(col, (h[0], h[1], h[2] - 26), (30, 12, 34))
        m.box(col, (h[0], h[1], h[2] - 6), (3, 3, 10))
    return f


def hold_box(col='#c9a070', w=46.0):
    def f(m, h, side):
        if side > 0:
            m.box(col, (h[0] - 24, h[1] - 6, h[2] + 12), (w, 34, 30))
    return f


def hold_tool(col='#8f9bb3', grip='#ff7a3c', length=42.0):
    def f(m, h, side):
        m.box(grip, (h[0], h[1], h[2] + 4), (5, 5, 16))
        m.box(col, (h[0], h[1], h[2] + 10 + length / 2.0), (5, 5, length))
        m.box(col, (h[0], h[1] - 6, h[2] + 10 + length), (8, 20, 9))
    return f


def hold_rod(col='#5a4a3c', length=260.0):
    """A long pole held out ahead and up: a fishing rod, a boom, a ski pole (turn it with the arm)."""
    def f(m, h, side):
        tilt_box(m, col, (h[0], h[1] + 20, h[2] - 8), (h[0], h[1] - length * 0.8, h[2] + length * 0.55), 3.5, 3.5)
    return f


def hold_board(col='#ffc93c', stripe='#ff2d3c', length=190.0):
    """A board under the arm, upright beside the body: a surfboard, a snowboard, a plank."""
    def f(m, h, side):
        m.box(col, (h[0] + side * 9, h[1], h[2] + 30), (7, 44, length))
        m.box(stripe, (h[0] + side * 13, h[1], h[2] + 30), (1.5, 9, length * 0.86))
    return f


# ------------------------------------------------------------------------------------------ creatures
def bird(sc, x, y, z, heading=0.0, span=110.0, body='#f4f9ff', wing='#c9c4d6', beak='#ffc93c', phase=0.0, rate=2.2, flap=32.0,
         travel=None, orbit=None, bob=0.0, s=1.0):
    """A bird in the air at (x, y, z), flying along `heading` (degrees in the ground plane, 0 = +X). Its wings beat
    about its own line of flight. span: wing tip to wing tip, cm. flap 0 with a small bob: a bird standing
    (give z = what it stands on). travel / orbit as figure()'s."""
    hd = math.radians(heading)
    tx, ty = math.cos(hd), math.sin(hd)
    rx, ry = -ty, tx
    tv, base = (0.0, 0.0), (x, y)
    if travel:
        tv = (float(travel[0]), float(travel[1]) / float(travel[0]))
    elif orbit:
        base, tv = (orbit[0], orbit[1]), (-1.0, float(orbit[2]))
    common = dict(phase=phase, rate=rate, bob=bob, base=base, travel=tv)
    L = span * 0.42 * s
    b = sc.part(motion(pivot=(x, y, z), **common))
    b.base = None
    tilt_box(b, body, (x - tx * L * 0.5, y - ty * L * 0.5, z), (x + tx * L * 0.5, y + ty * L * 0.5, z + L * 0.06), L * 0.36, L * 0.34)
    b.sphere(body, (x + tx * L * 0.56, y + ty * L * 0.56, z + L * 0.14), L * 0.2, steps=(3, 5))
    tilt_box(b, beak, (x + tx * L * 0.7, y + ty * L * 0.7, z + L * 0.12), (x + tx * L * 0.95, y + ty * L * 0.95, z + L * 0.08), L * 0.08, L * 0.08)
    tilt_box(b, wing, (x - tx * L * 0.45, y - ty * L * 0.45, z), (x - tx * L * 0.85, y - ty * L * 0.85, z + L * 0.03), L * 0.3, L * 0.05)
    for side in (-1, 1):
        w = sc.part(motion(swing=side * flap, pivot=(x, y, z + L * 0.1), axis=heading, **common))
        w.base = None
        half = span * 0.5 * s
        a = (x + side * rx * L * 0.12, y + side * ry * L * 0.12, z + L * 0.1)
        mid = (x + side * rx * half * 0.55, y + side * ry * half * 0.55, z + L * 0.2)
        tip = (x + side * rx * half - tx * L * 0.2, y + side * ry * half - ty * L * 0.2, z + L * 0.12)
        _wing(w, body, a, mid, tx, ty, L * 0.42)
        _wing(w, wing, mid, tip, tx, ty, L * 0.3)


def _wing(m, col, a, b, tx, ty, chord):
    """A flat wing panel from a to b, `chord` deep along the line of flight."""
    dx, dy, dz = b[0] - a[0], b[1] - a[1], b[2] - a[2]
    L = math.sqrt(dx * dx + dy * dy + dz * dz) or 1.0
    yaw = math.degrees(math.atan2(dy, dx))
    pitch = math.degrees(math.atan2(dz, math.hypot(dx, dy)))
    m.box(col, ((a[0] + b[0]) / 2.0, (a[1] + b[1]) / 2.0, (a[2] + b[2]) / 2.0), (L, chord, max(chord * 0.09, 2.0)), rot=(pitch, yaw, 0))


def quadruped(sc, x, y, z=0.0, yaw=0.0, s=1.0, body='#8e5a3a', belly=None, mane=None, length=150.0, height=120.0, girth=42.0, neck=50.0,
              neck_up=55.0, head=34.0, ears='#8e5a3a', horns=None, tail=40.0, tail_col=None, legs_col=None, snout=None,
              phase=0.0, rate=0.0, bob=0.0, stride=0.0, graze=0.0, wag=0.0, travel=None, orbit=None):
    """A four-legged animal standing on (x, y, z), its head towards -Y at yaw 0 (so yaw 90 heads it along +X).
    length: chest to rump; height: the back; girth: the body's width and depth; neck / neck_up: the neck's length and
    its angle above level (a horse 55, a grazing cow -20, a dog 30); head: its length; horns: a colour, or None;
    mane: a colour (a horse); tail: its length. A horse: the defaults. A cow: length 190, height 125, girth 62,
    neck 34, neck_up 10. A dog: 70, 48, 20, neck 16, neck_up 35, head 18. A goat: 80, 62, 26, horns.
    stride: degrees the legs swing; graze: degrees the neck and head nod; wag: degrees the tail swings.
    travel / orbit as figure()'s (face it along the mesh's X to travel)."""
    cy, sy = math.cos(math.radians(yaw)), math.sin(math.radians(yaw))

    def W(p):
        return (x + s * (p[0] * cy - p[1] * sy), y + s * (p[0] * sy + p[1] * cy), z + s * p[2])

    tv, base = (0.0, 0.0), (x, y)
    if travel:
        tv = (float(travel[0]), float(travel[1]) / float(travel[0]))
    elif orbit:
        base, tv = (orbit[0], orbit[1]), (-1.0, float(orbit[2]))
    common = dict(phase=phase, rate=rate, bob=bob, base=base, travel=tv)

    def part(swing=0.0, pivot=(0, 0, height), axis=0.0):
        return sc.part(motion(swing=swing, pivot=W(pivot), axis=(UP if axis == UP else yaw + axis), **common), (x, y, z), yaw, s)

    legs_col = legs_col or body
    b = part()
    hl = length / 2.0
    b.box(body, (0, 0, height - girth / 2.0), (girth, length, girth))
    if belly:
        b.box(belly, (0, 0, height - girth * 0.86), (girth * 0.9, length * 0.8, girth * 0.3))
    lw = max(girth * 0.26, 5.0)
    leg_h = height - girth * 0.8
    for k, (ly, sgn) in enumerate(((-hl + lw, 1), (-hl + lw, -1), (hl - lw, -1), (hl - lw, 1))):
        side = -1 if k % 2 else 1
        m = part(sgn * stride, (side * (girth / 2.0 - lw / 2.0), ly, leg_h), 0.0) if stride else b
        m.box(legs_col, (side * (girth / 2.0 - lw / 2.0), ly, leg_h / 2.0), (lw, lw * 1.1, leg_h + 4.0))
    # neck and head nod together about the shoulder
    nk = part(graze, (0, -hl + girth * 0.2, height - girth * 0.3), 0.0) if graze else b
    a = math.radians(neck_up)
    n0 = (0, -hl + girth * 0.25, height - girth * 0.35)
    n1 = (0, n0[1] - math.cos(a) * neck, n0[2] + math.sin(a) * neck)
    tilt_box(nk, body, n0, n1, girth * 0.5, girth * 0.55)
    hd = (0, n1[1] - head * 0.55, n1[2] - head * 0.12)
    nk.box(body, (0, n1[1] - head * 0.3, n1[2] + head * 0.05), (girth * 0.46, head, head * 0.56))
    nk.box(snout or body, (0, hd[1] - head * 0.3, hd[2] - head * 0.05), (girth * 0.36, head * 0.42, head * 0.4))
    for side in (-1, 1):
        nk.box(ears, (side * girth * 0.2, n1[1] + head * 0.05, n1[2] + head * 0.42), (head * 0.16, head * 0.12, head * 0.36))
        if horns:
            tilt_box(nk, horns, (side * girth * 0.14, n1[1] - head * 0.05, n1[2] + head * 0.3), (side * girth * 0.42, n1[1] + head * 0.1, n1[2] + head * 0.8), head * 0.13, head * 0.13)
    if mane:
        tilt_box(nk, mane, (0, n0[1] + girth * 0.16, n0[2] + girth * 0.3), (0, n1[1] + girth * 0.12, n1[2] + girth * 0.28), girth * 0.2, girth * 0.3)
    if tail:
        tl = part(wag, (0, hl, height - girth * 0.1), 90.0) if wag else b
        tilt_box(tl, tail_col or mane or body, (0, hl - 2.0, height - girth * 0.12), (0, hl + tail * 0.35, height - girth * 0.2 - tail * 0.9), lw * 0.7, lw * 0.7)
    return z + s * (n1[2] + head * 0.6)
