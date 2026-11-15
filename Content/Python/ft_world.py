"""FULL THROTTLE (Unreal) the living world: materials and meshes for scenery that moves (the game-jam pass, 3.1).

Everything here is looks only. Nothing has collision, nothing is a hazard, and nothing changes the shared cel pass:
the materials either take the level's normal lighting (and get banded like any wall) or are emissive-only, the trick
ft_buildings uses (a black base colour is not banded, so the colours written here are the colours on screen).

  M_BaySea     Bayshore's sea: the floor AFTTrack lays under the whole level (FTTrack.cpp, "the floor"). Flat
               turquoise tones in steps, broad swell shapes that drift, light crests near the camera, a glitter path
               towards the low sun and a warm sheen from the sky at the horizon. World-space, so it holds still under
               the car; the detail fades with distance, so nothing shimmers far away. No textures.

Run in the editor:  import ft_world; ft_world.build_all()
"""
import math
import unreal
import ft_mats
import ft_buildings

AL = unreal.EditorAssetLibrary
MEL = unreal.MaterialEditingLibrary

BAY = '/Game/Maps/Bayshore/Materials'
MPC = ft_buildings.MPC


def log(*a):
    unreal.log('FTWORLD ' + ' '.join(str(x) for x in a))


# ------------------------------------------------------------------------------------------ Bayshore's sea
SEA_HLSL = r"""
float3 V = WP - CP;
float dist = length(V);
float fd = length(V.xy);
float2 w = normalize(Wind.xy);
float2 c = float2(-w.y, w.x);
float t = T * Speed;
// the swell: long shapes that travel with the wind and bend a little
float2 p = WP.xy / Swell;
float s1 = sin(dot(p, w) * 6.2832 + t + 1.9 * sin(dot(p, c) * 2.1 + t * 0.31));
float s2 = sin(dot(p, w * 0.62 + c * 0.35) * 6.2832 * 1.9 - t * 0.7 + 1.3 * sin(dot(p, c) * 3.7 - t * 0.23));
float swell = s1 * 0.62 + s2 * 0.38;
// the detail holds to about SwellFar, then the sea is one tone (nothing shimmers at the horizon)
float keep = saturate(1.0 - fd / SwellFar);
float tone = 0.5 + 0.5 * swell * keep;
tone = floor(tone * Steps + 0.5) / Steps;
float3 col = lerp(Deep.rgb, Mid.rgb, tone);
// crests: thin light strokes on the swell's leading edge, near the camera only
float2 q = WP.xy / Crest;
float k1 = sin(dot(q, w) * 6.2832 + t * 1.6 + 2.2 * sin(dot(q, c) * 1.7 + t * 0.4) + 1.1 * sin(dot(q, c) * 4.3 - t * 0.5));
float broken = 0.5 + 0.5 * sin(dot(q, c) * 5.3 + sin(dot(q, w) * 2.9 + t * 0.2) * 2.0);
float crest = smoothstep(CrestCut, CrestCut + 0.04, k1) * step(0.42, broken) * saturate(1.0 - fd / CrestFar);
col = lerp(col, Light.rgb, crest * CrestAmt);
// the low sun's path: sparkles in the sun's direction, thicker towards the horizon
float2 sd = normalize(SunD.xy);
float2 vd = V.xy / max(fd, 1.0);
float along = saturate(dot(vd, sd));
float path = pow(along, GlintWidth);
float2 g = WP.xy / Glint;
float cell = sin(g.x * 6.2832 + t * 2.3 + sin(g.y * 4.1) * 2.0) * sin(g.y * 6.2832 * 0.37 - t * 1.1 + sin(g.x * 2.7) * 2.0);
float sparkle = smoothstep(GlintCut, GlintCut + 0.05, cell) * saturate(1.0 - fd / GlintFar) * saturate(fd / 2500.0);
col += SunC.rgb * path * (sparkle * GlintAmt + GlintGlow * saturate(fd / GlintFar));
// the sky lies on the water at a low angle: the horizon's warmth
float graze = 1.0 - saturate(-V.z / max(dist, 1.0));
float sheen = pow(graze, SheenPow) * Sheen;
col = lerp(col, Haz.rgb * Bright, saturate(sheen));
return col * Gain;
"""

SEA_PARAMS = [('Swell', 5200.0), ('SwellFar', 90000.0), ('Steps', 3.0), ('Speed', 0.55), ('Crest', 1500.0), ('CrestCut', 0.86),
              ('CrestFar', 22000.0), ('CrestAmt', 0.85), ('Glint', 900.0), ('GlintCut', 0.72), ('GlintFar', 160000.0),
              ('GlintWidth', 40.0), ('GlintAmt', 2.2), ('GlintGlow', 0.25), ('Sheen', 0.55), ('SheenPow', 14.0), ('Gain', 1.0)]
SEA_VECTORS = [('Deep', (0.035, 0.36, 0.47, 1.0)), ('Mid', (0.065, 0.56, 0.60, 1.0)), ('Light', (0.52, 0.93, 0.88, 1.0)),
               ('Wind', (0.82, 0.57, 0.0, 0.0))]
SEA_SKY = [('SunD', 'SunDirection'), ('SunC', 'SunColor'), ('Haz', 'HazeColor'), ('Bright', 'SkyBrightness')]


def bay_sea():
    """M_BaySea and its instance MI_BaySea (AFTTrack loads the instance by path for Bayshore's floor)."""
    mpc = AL.load_asset(MPC)
    if mpc is None:
        raise RuntimeError('M_BaySea needs the BayshoreSky plugin (%s)' % MPC)
    m = ft_mats.new_material(BAY, 'M_BaySea', blend=unreal.BlendMode.BLEND_OPAQUE, shading=unreal.MaterialShadingModel.MSM_DEFAULT_LIT)
    ft_buildings._emissive_only(m)
    X = -1300
    wp = ft_mats.expr(m, unreal.MaterialExpressionWorldPosition, x=X, y=-700)
    cp = ft_mats.expr(m, unreal.MaterialExpressionCameraPositionWS, x=X, y=-620)
    t = ft_mats.expr(m, unreal.MaterialExpressionTime, x=X, y=-540)
    ins = [('WP', wp, ''), ('CP', cp, ''), ('T', t, '')]
    y = -440
    for nm, v in SEA_PARAMS:
        ins.append((nm, ft_mats.sparam(m, nm, v, x=X, y=y), ''))
        y += 70
    for nm, v in SEA_VECTORS:
        ins.append((nm, ft_mats.vparam(m, nm, v, x=X, y=y), ''))
        y += 70
    for nm, pn in SEA_SKY:
        ins.append((nm, ft_buildings._mpc_param(m, mpc, pn, y), ''))
        y += 70
    cu = ft_mats.custom(m, SEA_HLSL, ins, x=-500, y=0, desc='BaySea')
    MEL.connect_material_property(cu, '', unreal.MaterialProperty.MP_EMISSIVE_COLOR)
    ft_buildings._compile(m)
    return ft_mats.mi(m, BAY, 'MI_BaySea')


# ------------------------------------------------------------------------------------------ Bayshore's clouds
# The sky's own clouds were photographs of clouds (Cycles bakes on layers and cards). Beside a cel-shaded town they
# read as another game. These are the game's kind of thing: a few big, chunky cumulus built from spheres with flat
# bases, lit in three flat tones by the same low sun, far enough away that the cel pass neither bands nor inks them
# (no ink past 1.5 km). They stand in the world (the map, as actors the artist can move), so they keep their place
# through turns and banking and slide a little as the car covers ground.
SKY = '/Game/Maps/Bayshore/Sky'
CLOUD_HLSL = r"""
float3 N = normalize(Nrm);
float3 S = normalize(SunD.xyz);
// the low sun from the side, the sky from above
float lit = dot(N, S) * 0.72 + N.z * 0.38;
float aa = 0.0;
#if PIXELSHADER
aa = fwidth(lit);
#endif
float w = max(Soft, aa);
float t0 = smoothstep(ShadeCut - w, ShadeCut + w, lit);
float t1 = smoothstep(LitCut - w, LitCut + w, lit);
float3 col = lerp(ShadowC.rgb, MidC.rgb, t0);
col = lerp(col, LitC.rgb, t1);
// the flat base sits in its own shade
col = lerp(col, BaseC.rgb, smoothstep(0.55, 0.8, -N.z));
// distance: the far ones sink towards the horizon's colour, so the sky stays quieter than the town
float dist = length(WP - CP);
col = lerp(col, Haz.rgb, saturate(dist / HazeDist) * HazeAmt);
return col * Bright * Gain;
"""
CLOUD_WPO = r"""
// a slow drift down the wind and back (each cloud at its own point of the swing), and puffs that swell a little
float ph = dot(OP.xy, float2(0.000013, 0.000021));
float2 wd = normalize(Wind.xy + float2(1e-5, 0.0));
float drift = sin(T * DriftRate + ph * 6.2832) * DriftAmp;
float swell = sin(T * BreathRate + dot(WP, float3(0.00002, 0.000017, 0.00003)) + ph * 4.0) * BreathAmp;
return float3(wd * drift, 0.0) + normalize(Nrm) * swell;
"""
CLOUD_PARAMS = [('LitCut', 0.34), ('ShadeCut', -0.12), ('Soft', 0.012), ('HazeDist', 4200000.0), ('HazeAmt', 0.6), ('Gain', 1.0),
                ('DriftAmp', 32000.0), ('DriftRate', 0.012), ('BreathAmp', 2200.0), ('BreathRate', 0.16)]
CLOUD_VECTORS = [('LitC', (1.0, 0.80, 0.66, 1.0)), ('MidC', (0.93, 0.47, 0.50, 1.0)), ('ShadowC', (0.33, 0.27, 0.56, 1.0)),
                 ('BaseC', (0.24, 0.21, 0.47, 1.0))]
CLOUD_SKY = [('SunD', 'SunDirection'), ('Haz', 'HazeColor'), ('Bright', 'SkyBrightness'), ('Wind', 'WindDirection')]

# the three shapes: (x, y, z, radius, squash) per puff, in cm, long axis along x, base cut flat at z = 0
CLOUDS = {
    # a long bank, heaped in the middle
    'A': [(-1500, 0, 60, 330, 0.8), (-1080, 60, 120, 420, 0.85), (-600, -40, 190, 520, 0.9), (-80, 30, 280, 640, 0.95),
          (480, -20, 210, 560, 0.9), (960, 50, 130, 440, 0.85), (1400, 0, 70, 340, 0.8), (-340, 260, 120, 380, 0.8),
          (240, -250, 110, 400, 0.8), (-860, -230, 80, 330, 0.8), (760, 240, 80, 320, 0.8)],
    # a tower: three tiers, leaning down the wind
    'B': [(-560, 0, 90, 440, 0.85), (0, 40, 140, 560, 0.9), (560, -30, 90, 430, 0.85), (-200, 0, 520, 440, 0.95),
          (300, 30, 560, 400, 0.95), (90, 0, 900, 330, 1.0), (-900, 40, 40, 300, 0.8), (880, 0, 40, 290, 0.8),
          (0, 280, 100, 380, 0.8), (40, -280, 100, 380, 0.8)],
    # a small one
    'C': [(-420, 0, 70, 300, 0.82), (0, 20, 130, 400, 0.9), (430, -10, 80, 310, 0.82), (60, 200, 60, 250, 0.8), (-40, -200, 60, 250, 0.8)],
}
# where they stand, seen from the middle of the track: (name, shape, azimuth deg, distance km, base height km, width km)
# The sun is at azimuth 30, 6 degrees up: A1 and B1 flank it, low. Above 12 degrees the sky stays clear.
CLOUD_LAYOUT = [('A1', 'A', 6.0, 25.0, 1.7, 13.0), ('B1', 'B', 56.0, 27.0, 2.3, 9.5), ('A2', 'A', 104.0, 21.0, 2.2, 10.0),
                ('C1', 'C', 150.0, 24.0, 3.0, 5.5), ('B2', 'B', 204.0, 22.0, 1.9, 9.0), ('A3', 'A', 262.0, 26.0, 2.7, 12.0),
                ('C2', 'C', 318.0, 22.0, 2.3, 6.0), ('C3', 'C', 344.0, 31.0, 3.4, 5.0)]
CLOUD_CENTRE = (140000.0, 620000.0)  # the middle of Bayshore's track (the sky's own Level Instance stands here too)


def bay_cloud_material():
    mpc = AL.load_asset(MPC)
    if mpc is None:
        raise RuntimeError('M_BayCloud needs the BayshoreSky plugin (%s)' % MPC)
    m = ft_mats.new_material(BAY, 'M_BayCloud', blend=unreal.BlendMode.BLEND_OPAQUE, shading=unreal.MaterialShadingModel.MSM_DEFAULT_LIT)
    ft_buildings._emissive_only(m)
    X = -1300
    nrm = ft_mats.expr(m, unreal.MaterialExpressionVertexNormalWS, x=X, y=-760)
    wp = ft_mats.expr(m, unreal.MaterialExpressionWorldPosition, x=X, y=-680)
    cp = ft_mats.expr(m, unreal.MaterialExpressionCameraPositionWS, x=X, y=-600)
    op = ft_mats.expr(m, unreal.MaterialExpressionObjectPositionWS, x=X, y=-520)
    t = ft_mats.expr(m, unreal.MaterialExpressionTime, x=X, y=-440)
    P = {}
    y = -340
    for nm, v in CLOUD_PARAMS:
        P[nm] = ft_mats.sparam(m, nm, v, x=X, y=y)
        y += 70
    for nm, v in CLOUD_VECTORS:
        P[nm] = ft_mats.vparam(m, nm, v, x=X, y=y)
        y += 70
    for nm, pn in CLOUD_SKY:
        P[nm] = ft_buildings._mpc_param(m, mpc, pn, y)
        y += 70
    shade = ft_mats.custom(m, CLOUD_HLSL, [('Nrm', nrm, ''), ('WP', wp, ''), ('CP', cp, '')] + [(k, P[k], '') for k in (
        'SunD', 'LitCut', 'ShadeCut', 'Soft', 'LitC', 'MidC', 'ShadowC', 'BaseC', 'Haz', 'HazeDist', 'HazeAmt', 'Bright', 'Gain')],
        x=-500, y=0, desc='BayCloud')
    MEL.connect_material_property(shade, '', unreal.MaterialProperty.MP_EMISSIVE_COLOR)
    wpo = ft_mats.custom(m, CLOUD_WPO, [('Nrm', nrm, ''), ('WP', wp, ''), ('OP', op, ''), ('T', t, '')] + [(k, P[k], '') for k in (
        'Wind', 'DriftAmp', 'DriftRate', 'BreathAmp', 'BreathRate')], x=-500, y=500, desc='BayCloudDrift')
    MEL.connect_material_property(wpo, '', unreal.MaterialProperty.MP_WORLD_POSITION_OFFSET)
    ft_buildings._compile(m)
    return ft_mats.mi(m, BAY, 'MI_BayCloud')


def _cloud_mesh(puffs):
    GS = unreal.GeometryScript_Primitives
    m = unreal.DynamicMesh()
    po = unreal.GeometryScriptPrimitiveOptions()
    for x, y, z, r, sq in puffs:
        xf = unreal.Transform(unreal.Vector(x, y, z), unreal.Rotator(0, 0, 0), unreal.Vector(1.0, 1.0, sq))
        GS.append_sphere_lat_long(m, po, xf, r, 12, 18, unreal.GeometryScriptPrimitiveOriginMode.CENTER)
    # the flat base: everything under z = 0 goes, and the cut is capped
    try:
        co = unreal.GeometryScriptMeshPlaneCutOptions()
        co.set_editor_property('fill_holes', True)
        co.set_editor_property('flip_cut_side', True)
        unreal.GeometryScript_MeshBooleans.apply_mesh_plane_cut(m, unreal.Transform(unreal.Vector(0, 0, 0), unreal.Rotator(0, 0, 0), unreal.Vector(1, 1, 1)), co)
        b = unreal.GeometryScript_MeshQueries.get_mesh_bounding_box(m)
        if b.max.z < 50.0:  # it kept the wrong side: build again and cut the other way
            m = unreal.DynamicMesh()
            for x, y, z, r, sq in puffs:
                xf = unreal.Transform(unreal.Vector(x, y, z), unreal.Rotator(0, 0, 0), unreal.Vector(1.0, 1.0, sq))
                GS.append_sphere_lat_long(m, po, xf, r, 12, 18, unreal.GeometryScriptPrimitiveOriginMode.CENTER)
            co.set_editor_property('flip_cut_side', False)
            unreal.GeometryScript_MeshBooleans.apply_mesh_plane_cut(m, unreal.Transform(unreal.Vector(0, 0, 0), unreal.Rotator(0, 0, 0), unreal.Vector(1, 1, 1)), co)
    except Exception as e:
        unreal.log_warning('FTWORLD cloud base not cut flat (%s)' % e)
    # smooth puffs, a hard edge where a puff meets the base
    so = unreal.GeometryScriptSplitNormalsOptions()
    so.set_editor_property('split_by_opening_angle', True)
    so.set_editor_property('opening_angle_deg', 50.0)
    unreal.GeometryScript_Normals.compute_split_normals(m, so, unreal.GeometryScriptCalculateNormalsOptions())
    return m


def _save_static(mesh, path, material, slot):
    """A DynamicMesh as a static mesh with one material; an existing asset is updated in place (actors keep their link)."""
    folder = path.rsplit('/', 1)[0]
    if not AL.does_directory_exist(folder):
        AL.make_directory(folder)
    if AL.does_asset_exist(path):
        sm = AL.load_asset(path)
        opts = unreal.GeometryScriptCopyMeshToAssetOptions()
        opts.enable_recompute_normals = False
        opts.replace_materials = True
        opts.new_materials = [material]
        opts.new_material_slot_names = [unreal.Name(slot)]
        opts.emit_transaction = False
        unreal.GeometryScript_AssetUtils.copy_mesh_to_static_mesh(mesh, sm, opts, unreal.GeometryScriptMeshWriteLOD())
    else:
        opts = unreal.GeometryScriptCreateNewStaticMeshAssetOptions()
        opts.set_editor_property('enable_nanite', False)
        opts.set_editor_property('enable_collision', False)
        opts.set_editor_property('enable_recompute_normals', False)
        sm, _ = unreal.GeometryScript_NewAssetUtils.create_new_static_mesh_asset_from_mesh(mesh, path, opts)
        if sm is None:
            raise RuntimeError('could not create ' + path)
        sm.set_editor_property('static_materials', [unreal.StaticMaterial(material_interface=material, material_slot_name=slot)])
    AL.save_loaded_asset(sm)
    return sm


def bay_cloud_meshes():
    mi = AL.load_asset(BAY + '/MI_BayCloud') or bay_cloud_material()
    out = {}
    for key, puffs in CLOUDS.items():
        m = _cloud_mesh(puffs)
        b = unreal.GeometryScript_MeshQueries.get_mesh_bounding_box(m)
        out[key] = (_save_static(m, '%s/SM_BayCloud_%s' % (SKY, key), mi, 'Cloud'), b.max.x - b.min.x)
        log('cloud', key, 'width', round(b.max.x - b.min.x), 'height', round(b.max.z - b.min.z), 'base', round(b.min.z),
            'triangles', unreal.GeometryScript_MeshQueries.get_num_triangle_i_ds(m))
    return out


def place_bay_clouds(save=True):
    """Stand the clouds in the open map (L_Bayshore must be the loaded level). Replaces the ones placed before, so the
    layout above is the source; move them by hand in the editor freely, and re-run only to go back to this layout."""
    import math
    EAS = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
    meshes = bay_cloud_meshes()
    for a in EAS.get_all_level_actors():
        if a.get_actor_label().startswith('BayCloud_'):
            EAS.destroy_actor(a)
    for name, shape, az, dist, alt, width in CLOUD_LAYOUT:
        sm, w = meshes[shape]
        a = math.radians(az)
        loc = unreal.Vector(CLOUD_CENTRE[0] + math.cos(a) * dist * 100000.0, CLOUD_CENTRE[1] + math.sin(a) * dist * 100000.0, alt * 100000.0)
        s = width * 100000.0 / max(w, 1.0)
        actor = EAS.spawn_actor_from_object(sm, loc, unreal.Rotator(roll=0.0, pitch=0.0, yaw=az + 90.0))
        actor.set_actor_label('BayCloud_' + name)
        actor.set_folder_path('Sky')
        actor.set_actor_scale3d(unreal.Vector(s, s, s))
        c = actor.get_component_by_class(unreal.StaticMeshComponent)
        c.set_mobility(unreal.ComponentMobility.STATIC)
        c.set_collision_enabled(unreal.CollisionEnabled.NO_COLLISION)
        c.set_cast_shadow(False)
        c.set_editor_property('bounds_scale', 1.15)       # the drift moves them up to 320 m
        for flag in ('affect_distance_field_lighting', 'affect_dynamic_indirect_lighting', 'visible_in_ray_tracing'):
            try:
                c.set_editor_property(flag, False)
            except Exception as e:
                unreal.log_warning('FTWORLD cloud %s: %s (%s)' % (name, flag, e))
        log('placed', name, shape, 'az', az, 'dist km', dist, 'scale', round(s, 1))
    if save:
        unreal.get_editor_subsystem(unreal.LevelEditorSubsystem).save_current_level()


# ------------------------------------------------------------------------------------------ Under the Sea (Abyss)
# What makes it the sea: light that moves. The surface far overhead ripples (MI_AbyssSurface, a sheet AFTTrack hangs
# above the level), the light it lets through crawls over the sand, the road and the car (a light function on the
# level's key light), shafts of it stand in the water, and things live in it: schools that swim their rounds, a whale
# and a ray far out, strings of bubbles. All of it is looks only and none of it uses a texture.
ABY = '/Game/Maps/Abyss/Materials'
ABYL = '/Game/Maps/Abyss/Life'
GSP = unreal.GeometryScript_Primitives
CENTER = unreal.GeometryScriptPrimitiveOriginMode.CENTER
BASE = unreal.GeometryScriptPrimitiveOriginMode.BASE

CAUSTIC_FN = r"""
struct FtCaustic
{
    // a net of bright lines that swims: two bent line fields, bright where either one crosses zero
    float Net(float2 p, float t, float sharp)
    {
        float2 q = p + 0.32 * float2(sin(p.y * 1.9 + t), cos(p.x * 1.6 - t * 0.7));
        float a = abs(sin(q.x * 3.1 + sin(q.y * 2.3 + t * 0.6) * 1.3));
        float b = abs(sin(q.y * 2.9 - t * 0.5 + sin(q.x * 2.1 - t * 0.4) * 1.3));
        return pow(1.0 - min(a, b), sharp);
    }
};
FtCaustic Ca;
"""
SURFACE_HLSL = CAUSTIC_FN + r"""
float3 V = WP - CP;
float dist = length(V);
float t = T * Speed;
float net = Ca.Net(WP.xy / Scale, t, Sharp) * 0.7 + Ca.Net(WP.xy / (Scale * 2.7) + 3.1, t * 0.6, Sharp * 0.6) * 0.5;
net = floor(saturate(net) * Steps + 0.5) / Steps;
// straight overhead the daylight comes through; towards the edges the surface mirrors the deep
float up = saturate(V.z / max(dist, 1.0));
float window = smoothstep(WinEdge, 1.0, up);
float3 col = lerp(Deep.rgb, Mid.rgb, window) + Light.rgb * net * (0.3 + 0.7 * window);
return col * Gain;
"""
SURFACE_PARAMS = [('Scale', 2600.0), ('Speed', 0.55), ('Sharp', 5.0), ('Steps', 3.0), ('WinEdge', 0.12), ('Gain', 1.0)]
SURFACE_VECTORS = [('Deep', (0.004, 0.05, 0.11, 1.0)), ('Mid', (0.02, 0.26, 0.36, 1.0)), ('Light', (0.30, 0.95, 0.92, 1.0))]

LIGHTFN_HLSL = CAUSTIC_FN + r"""
float net = Ca.Net(WP.xy / Scale + WP.z * 0.0004, T * Speed, Sharp);
// a light function can only take light away: the dim value is the water between the bright lines
return lerp(Dim, 1.0, saturate(net * Amount));
"""
LIGHTFN_PARAMS = [('Scale', 620.0), ('Speed', 0.8), ('Sharp', 6.0), ('Dim', 0.52), ('Amount', 1.6)]

SHAFT_HLSL = r"""
// a cone of light: bright where the view goes through its middle, gone at its edges and at its foot
float3 Vn = normalize(CP - WP);
float facing = abs(dot(normalize(Nrm), Vn));
float edge = smoothstep(0.05, 0.85, facing);
float h = saturate(UV.y);
float along = smoothstep(0.0, 0.3, h) * (0.3 + 0.7 * h);
float ang = UV.x * 6.2832;
float streak = 0.62 + 0.38 * sin(ang * 5.0 + T * 0.33 + Rnd * 20.0) * sin(ang * 3.0 - T * 0.21 + Rnd * 7.0);
float pulse = 0.78 + 0.22 * sin(T * 0.45 + Rnd * 30.0);
// thin near the camera: driving through one never washes the screen
float near = saturate((length(CP - WP) - 500.0) / 2600.0);
return Col.rgb * Gain * edge * along * streak * pulse * near;
"""
SHAFT_PARAMS = [('Gain', 0.5)]
SHAFT_VECTORS = [('Col', (0.25, 0.95, 1.0, 1.0))]

FISH_WPO = r"""
// Local space, nose along +x. A wave runs down every body (vertex alpha: how much a part gives; tails and flukes
// most), then the whole school goes round a circle beside where it stands, heading along the tangent.
float ph = Rnd * 6.2832;
float3 L = Lp;
float wave = sin(L.x * WagK - T * WagRate + ph * 3.0) * WagAmp * VA;
L.y += wave * (1.0 - WagZ);
L.z += wave * WagZ + abs(L.y) * FlapAmp * sin(T * FlapRate + ph);
float ang = T * Speed / max(Radius, 1.0) + ph;
float cs = cos(ang), sn = sin(ang);
float2 r = float2(L.x, L.y - Radius);
float2 rr = float2(r.x * cs - r.y * sn, r.x * sn + r.y * cs);
return float3(rr.x, rr.y + Radius, L.z + sin(ang * 2.0 + ph) * Bob) - Lp;
"""
FISH_PARAMS = [('WagK', 0.11), ('WagRate', 9.0), ('WagAmp', 9.0), ('WagZ', 0.0), ('FlapAmp', 0.0), ('FlapRate', 1.4),
               ('Speed', 260.0), ('Radius', 700.0), ('Bob', 60.0), ('Glow', 0.55), ('Rough', 0.5)]

BUBBLE_HLSL = r"""
// vertex colour: r = which bubble of the string, g = its radius (of MaxR). They are all built at the foot; each one's
// height is its place in the cycle. A bubble swells out of nothing at the bottom and is gone at the top.
float id = VC.r;
float u = frac(id * 7.31 + T * Rise / Height + Rnd);
float life = smoothstep(0.0, 0.06, u) * (1.0 - smoothstep(0.92, 1.0, u));
float2 wob = float2(sin(T * 1.7 + id * 40.0 + u * 9.0), cos(T * 1.3 + id * 23.0 + u * 7.0)) * Wobble * u;
return float3(wob, u * Height) - normalize(Nrm) * VC.g * MaxR * (1.0 - life * lerp(0.6, 1.0, u));
"""
BUBBLE_PARAMS = [('Height', 1900.0), ('Rise', 260.0), ('Wobble', 55.0), ('MaxR', 22.0), ('Gain', 1.0)]
BUBBLE_SHADE = r"""
float3 Vn = normalize(CP - WP);
float rim = pow(1.0 - saturate(dot(normalize(Nrm), Vn)), 2.2);
return (Body.rgb * 0.35 + Rim.rgb * rim) * Gain;
"""


def _local_pos(m, x, y):
    """The vertex in its own instance's space (prop instances are HISM instances)."""
    cls = getattr(unreal, 'MaterialExpressionLocalPosition', None)
    if cls is not None:
        return ft_mats.expr(m, cls, x=x, y=y)
    wp = ft_mats.expr(m, unreal.MaterialExpressionWorldPosition, x=x - 250, y=y)
    tr = ft_mats.expr(m, unreal.MaterialExpressionTransformPosition, x=x, y=y)
    tr.set_editor_property('transform_source_type', unreal.MaterialPositionTransformSource.TRANSFORMPOSSOURCE_WORLD)
    tr.set_editor_property('transform_type', unreal.MaterialPositionTransformSource.TRANSFORMPOSSOURCE_LOCAL)
    MEL.connect_material_expressions(wp, '', tr, '')
    log('no LocalPosition node in this engine: using TransformPosition world->local')
    return tr


def _local_to_world(m, vec, x, y):
    tr = ft_mats.expr(m, unreal.MaterialExpressionTransform, x=x, y=y)
    src = unreal.MaterialVectorCoordTransformSource
    tr.set_editor_property('transform_source_type', getattr(src, 'TRANSFORMSOURCE_INSTANCE', src.TRANSFORMSOURCE_LOCAL))
    tr.set_editor_property('transform_type', unreal.MaterialVectorCoordTransform.TRANSFORM_WORLD)
    MEL.connect_material_expressions(vec, '', tr, '')
    return tr


def _params(m, scalars, vectors, x=-1300, y=-300):
    P = {}
    for nm, v in scalars:
        P[nm] = ft_mats.sparam(m, nm, v, x=x, y=y)
        y += 70
    for nm, v in vectors:
        P[nm] = ft_mats.vparam(m, nm, v, x=x, y=y)
        y += 70
    return P


def abyss_surface():
    """MI_AbyssSurface: the sea's surface from below (AFTTrack loads it by path for the sheet over level 6)."""
    m = ft_mats.new_material(ABY, 'M_AbyssSurface', blend=unreal.BlendMode.BLEND_OPAQUE, shading=unreal.MaterialShadingModel.MSM_DEFAULT_LIT, two_sided=True)
    ft_buildings._emissive_only(m)
    wp = ft_mats.expr(m, unreal.MaterialExpressionWorldPosition, x=-1300, y=-700)
    cp = ft_mats.expr(m, unreal.MaterialExpressionCameraPositionWS, x=-1300, y=-620)
    t = ft_mats.expr(m, unreal.MaterialExpressionTime, x=-1300, y=-540)
    P = _params(m, SURFACE_PARAMS, SURFACE_VECTORS)
    cu = ft_mats.custom(m, SURFACE_HLSL, [('WP', wp, ''), ('CP', cp, ''), ('T', t, '')] + [(k, v, '') for k, v in P.items()], x=-500, y=0, desc='AbyssSurface')
    MEL.connect_material_property(cu, '', unreal.MaterialProperty.MP_EMISSIVE_COLOR)
    ft_buildings._compile(m)
    return ft_mats.mi(m, ABY, 'MI_AbyssSurface')


def abyss_caustics():
    """M_AbyssCaustics: a light function for the level's key light (set on the light by abyss_apply)."""
    m = ft_mats.new_material(ABY, 'M_AbyssCaustics')
    m.set_editor_property('material_domain', unreal.MaterialDomain.MD_LIGHT_FUNCTION)
    wp = ft_mats.expr(m, unreal.MaterialExpressionWorldPosition, x=-1300, y=-700)
    t = ft_mats.expr(m, unreal.MaterialExpressionTime, x=-1300, y=-540)
    P = _params(m, LIGHTFN_PARAMS, [])
    cu = ft_mats.custom(m, LIGHTFN_HLSL, [('WP', wp, ''), ('T', t, '')] + [(k, v, '') for k, v in P.items()],
                        out_type=unreal.CustomMaterialOutputType.CMOT_FLOAT1, x=-500, y=0, desc='AbyssCaustics')
    MEL.connect_material_property(cu, '', unreal.MaterialProperty.MP_EMISSIVE_COLOR)
    ft_buildings._compile(m)
    return m


def abyss_shaft_material():
    m = ft_mats.new_material(ABY, 'M_AbyssShaft', blend=unreal.BlendMode.BLEND_ADDITIVE, shading=unreal.MaterialShadingModel.MSM_UNLIT, two_sided=True)
    nrm = ft_mats.expr(m, unreal.MaterialExpressionVertexNormalWS, x=-1300, y=-780)
    wp = ft_mats.expr(m, unreal.MaterialExpressionWorldPosition, x=-1300, y=-700)
    cp = ft_mats.expr(m, unreal.MaterialExpressionCameraPositionWS, x=-1300, y=-620)
    t = ft_mats.expr(m, unreal.MaterialExpressionTime, x=-1300, y=-540)
    uv = ft_mats.expr(m, unreal.MaterialExpressionTextureCoordinate, x=-1300, y=-460)
    rnd = ft_mats.expr(m, unreal.MaterialExpressionPerInstanceRandom, x=-1300, y=-380)
    P = _params(m, SHAFT_PARAMS, SHAFT_VECTORS)
    cu = ft_mats.custom(m, SHAFT_HLSL, [('Nrm', nrm, ''), ('WP', wp, ''), ('CP', cp, ''), ('T', t, ''), ('UV', uv, ''), ('Rnd', rnd, '')] +
                        [(k, v, '') for k, v in P.items()], x=-500, y=0, desc='AbyssShaft')
    MEL.connect_material_property(cu, '', unreal.MaterialProperty.MP_EMISSIVE_COLOR)
    m.set_editor_property('used_with_instanced_static_meshes', True)
    ft_buildings._compile(m)
    return ft_mats.mi(m, ABY, 'MI_AbyssShaft')


def abyss_fish_material():
    """M_AbyssFish: lit (the cel pass bands it like any prop), coloured by vertex colour, with its own glow so a school
    reads in dark water. The swimming is all in the vertex shader. Instances: MI_AbyssFish, MI_AbyssWhale, MI_AbyssManta."""
    m = ft_mats.new_material(ABY, 'M_AbyssFish', blend=unreal.BlendMode.BLEND_OPAQUE, shading=unreal.MaterialShadingModel.MSM_DEFAULT_LIT)
    vc = ft_mats.expr(m, unreal.MaterialExpressionVertexColor, x=-1300, y=-800)
    lp = _local_pos(m, -1300, -700)
    t = ft_mats.expr(m, unreal.MaterialExpressionTime, x=-1300, y=-540)
    rnd = ft_mats.expr(m, unreal.MaterialExpressionPerInstanceRandom, x=-1300, y=-460)
    P = _params(m, FISH_PARAMS, [])
    MEL.connect_material_property(vc, '', unreal.MaterialProperty.MP_BASE_COLOR)
    glow = ft_mats.custom(m, 'return VC.rgb * Glow;', [('VC', vc, ''), ('Glow', P['Glow'], '')], x=-500, y=-300, desc='FishGlow')
    MEL.connect_material_property(glow, '', unreal.MaterialProperty.MP_EMISSIVE_COLOR)
    MEL.connect_material_property(P['Rough'], '', unreal.MaterialProperty.MP_ROUGHNESS)
    off = ft_mats.custom(m, FISH_WPO, [('Lp', lp, ''), ('VA', vc, 'A'), ('T', t, ''), ('Rnd', rnd, '')] + [(k, P[k], '') for k in (
        'WagK', 'WagRate', 'WagAmp', 'WagZ', 'FlapAmp', 'FlapRate', 'Speed', 'Radius', 'Bob')], x=-700, y=300, desc='FishSwim')
    MEL.connect_material_property(_local_to_world(m, off, -350, 300), '', unreal.MaterialProperty.MP_WORLD_POSITION_OFFSET)
    m.set_editor_property('used_with_instanced_static_meshes', True)
    ft_buildings._compile(m)
    return {
        'fish': ft_mats.mi(m, ABY, 'MI_AbyssFish'),
        'whale': ft_mats.mi(m, ABY, 'MI_AbyssWhale', scalars={'WagK': 0.0016, 'WagRate': 1.1, 'WagAmp': 150.0, 'WagZ': 1.0, 'Speed': 320.0,
                                                               'Radius': 7000.0, 'Bob': 250.0, 'Glow': 0.12}),
        'manta': ft_mats.mi(m, ABY, 'MI_AbyssManta', scalars={'WagK': 0.004, 'WagRate': 1.6, 'WagAmp': 30.0, 'WagZ': 1.0, 'FlapAmp': 0.32,
                                                               'FlapRate': 1.5, 'Speed': 230.0, 'Radius': 2600.0, 'Bob': 180.0, 'Glow': 0.18}),
    }


def abyss_bubble_material():
    m = ft_mats.new_material(ABY, 'M_AbyssBubble', blend=unreal.BlendMode.BLEND_OPAQUE, shading=unreal.MaterialShadingModel.MSM_DEFAULT_LIT)
    ft_buildings._emissive_only(m)
    vc = ft_mats.expr(m, unreal.MaterialExpressionVertexColor, x=-1300, y=-800)
    nrm = ft_mats.expr(m, unreal.MaterialExpressionVertexNormalWS, x=-1300, y=-720)
    wp = ft_mats.expr(m, unreal.MaterialExpressionWorldPosition, x=-1300, y=-640)
    cp = ft_mats.expr(m, unreal.MaterialExpressionCameraPositionWS, x=-1300, y=-560)
    t = ft_mats.expr(m, unreal.MaterialExpressionTime, x=-1300, y=-480)
    rnd = ft_mats.expr(m, unreal.MaterialExpressionPerInstanceRandom, x=-1300, y=-400)
    P = _params(m, BUBBLE_PARAMS, [('Body', (0.10, 0.55, 0.70, 1.0)), ('Rim', (0.75, 1.0, 1.0, 1.0))])
    sh = ft_mats.custom(m, BUBBLE_SHADE, [('Nrm', nrm, ''), ('WP', wp, ''), ('CP', cp, ''), ('Body', P['Body'], ''), ('Rim', P['Rim'], ''),
                                          ('Gain', P['Gain'], '')], x=-500, y=0, desc='BubbleShade')
    MEL.connect_material_property(sh, '', unreal.MaterialProperty.MP_EMISSIVE_COLOR)
    off = ft_mats.custom(m, BUBBLE_HLSL, [('VC', vc, ''), ('Nrm', nrm, ''), ('T', t, ''), ('Rnd', rnd, '')] + [(k, P[k], '') for k in (
        'Height', 'Rise', 'Wobble', 'MaxR')], x=-500, y=400, desc='BubbleRise')
    MEL.connect_material_property(off, '', unreal.MaterialProperty.MP_WORLD_POSITION_OFFSET)
    m.set_editor_property('used_with_instanced_static_meshes', True)
    ft_buildings._compile(m)
    return ft_mats.mi(m, ABY, 'MI_AbyssBubble')


# ---- meshes (Geometry Script; vertex colour carries each part's colour, and in alpha how much it wags)
def _xf(loc=(0, 0, 0), rot=(0, 0, 0), scale=(1, 1, 1)):
    return unreal.Transform(unreal.Vector(*loc), unreal.Rotator(roll=rot[0], pitch=rot[1], yaw=rot[2]), unreal.Vector(*scale))


def _tint(mesh, rgba):
    unreal.GeometryScript_VertexColors.set_mesh_constant_vertex_color(mesh, unreal.LinearColor(*rgba), unreal.GeometryScriptColorFlags(), True)


def _new():
    m = unreal.DynamicMesh()
    _tint(m, (1, 1, 1, 1))  # so appended parts keep their colours
    return m


def _ball(dst, col, loc, r, scale=(1, 1, 1), rot=(0, 0, 0), steps=(6, 9)):
    s = unreal.DynamicMesh()
    GSP.append_sphere_lat_long(s, unreal.GeometryScriptPrimitiveOptions(), _xf(scale=scale), r, steps[0], steps[1], CENTER)
    _tint(s, col)
    unreal.GeometryScript_MeshEdits.append_mesh(dst, s, _xf(loc, rot))


def _slab(dst, col, loc, size, rot=(0, 0, 0)):
    s = unreal.DynamicMesh()
    GSP.append_box(s, unreal.GeometryScriptPrimitiveOptions(), _xf(), size[0], size[1], size[2], 0, 0, 0, CENTER)
    _tint(s, col)
    unreal.GeometryScript_MeshEdits.append_mesh(dst, s, _xf(loc, rot))


def _fish(dst, at, body, fin, length=42.0, yaw=0.0, k=1.0):
    """One small fish, nose along +x: a body that wags a little (alpha 0.25) and a tail that wags a lot (alpha 1)."""
    f = _new()
    L = length * k
    _ball(f, body + (0.25,), (0, 0, 0), L * 0.5, scale=(1.0, 0.34, 0.46), steps=(5, 8))
    _slab(f, fin + (1.0,), (-L * 0.62, 0, 0), (L * 0.32, L * 0.05, L * 0.5))
    _slab(f, fin + (0.6,), (-L * 0.42, 0, 0), (L * 0.2, L * 0.06, L * 0.22))
    _slab(f, fin + (0.25,), (L * 0.02, 0, L * 0.25), (L * 0.3, L * 0.04, L * 0.16))
    unreal.GeometryScript_MeshEdits.append_mesh(dst, f, _xf(at, (0, 0, yaw)))


def _school(seed, colours, n=26, centre_z=780.0):
    import random
    r = random.Random(seed)
    m = _new()
    for i in range(n):
        # a loose teardrop: more of them at the front
        u = r.random() ** 0.7
        x = 520.0 - u * 1040.0
        w = 60.0 + 190.0 * math.sin(u * math.pi)
        y, z = r.uniform(-w, w), centre_z + r.uniform(-w, w) * 0.7
        body, fin = r.choice(colours)
        _fish(m, (x, y, z), body, fin, yaw=r.uniform(-9, 9), k=r.uniform(0.8, 1.3))
    return m


def _whale():
    """A whale, 27 m, nose along +x: a silhouette first (it lives far out in the haze)."""
    m = _new()
    back, belly, fin = (0.035, 0.07, 0.16), (0.30, 0.45, 0.58), (0.05, 0.10, 0.22)
    z = 2600.0
    _ball(m, back + (0.2,), (0, 0, z), 1000.0, scale=(1.35, 0.42, 0.40), steps=(8, 12))
    _ball(m, belly + (0.2,), (120, 0, z - 110), 900.0, scale=(1.2, 0.38, 0.30), steps=(6, 10))
    _ball(m, back + (0.6,), (-1250, 0, z + 30), 420.0, scale=(1.5, 0.45, 0.42), steps=(6, 8))
    _slab(m, fin + (1.0,), (-1900, 0, z + 40), (330, 980, 46))
    _slab(m, fin + (1.0,), (-2010, 300, z + 40), (300, 420, 44), rot=(0, 0, 28))
    _slab(m, fin + (1.0,), (-2010, -300, z + 40), (300, 420, 44), rot=(0, 0, -28))
    _slab(m, fin + (0.35,), (350, 470, z - 220), (460, 620, 40), rot=(18, 0, -30))
    _slab(m, fin + (0.35,), (350, -470, z - 220), (460, 620, 40), rot=(-18, 0, 30))
    _slab(m, fin + (0.25,), (-420, 0, z + 420), (300, 40, 170), rot=(0, 25, 0))
    return m


def _manta():
    """A ray, 9 m across: its wings flap by the material (the further out along y, the more)."""
    m = _new()
    top, under = (0.05, 0.12, 0.20), (0.45, 0.62, 0.70)
    z = 1350.0
    _ball(m, top + (0.0,), (0, 0, z), 300.0, scale=(1.1, 0.75, 0.2), steps=(6, 9))
    _slab(m, top + (0.0,), (-20, 0, z), (420, 900, 36), rot=(0, 0, 45))
    _slab(m, under + (0.0,), (-20, 0, z - 22), (380, 820, 14), rot=(0, 0, 45))
    _slab(m, top + (1.0,), (-560, 0, z), (620, 22, 22))
    _slab(m, top + (0.0,), (300, 70, z), (150, 40, 40), rot=(0, 0, 12))
    _slab(m, top + (0.0,), (300, -70, z), (150, 40, 40), rot=(0, 0, -12))
    return m


def _shaft(height, r_top, r_bottom, tilt=13.0, n=1, spread=0.0, seed=0):
    """Cones of light, open at both ends, leaning the way the level's light falls; n of them for the big one."""
    import random
    r = random.Random(seed)
    m = unreal.DynamicMesh()
    for i in range(n):
        s = unreal.DynamicMesh()
        k = r.uniform(0.7, 1.15) if n > 1 else 1.0
        GSP.append_cone(s, unreal.GeometryScriptPrimitiveOptions(), _xf(), r_bottom * k, r_top * k, height * (0.85 + 0.15 * k), 20, 6, False, BASE)
        at = (r.uniform(-spread, spread), r.uniform(-spread, spread), -150.0) if n > 1 else (0, 0, -150.0)
        unreal.GeometryScript_MeshEdits.append_mesh(m, s, _xf(at, (r.uniform(-4, 4) if n > 1 else 0.0, tilt + (r.uniform(-4, 4) if n > 1 else 0.0), 0)))
    return m


def _bubbles(n=16, seed=5):
    import random
    r = random.Random(seed)
    m = _new()
    for i in range(n):
        rad = r.uniform(0.35, 1.0)
        a = r.uniform(0, 6.2832)
        _ball(m, (i / float(n), rad, 0.0, 1.0), (math.cos(a) * r.uniform(0, 70), math.sin(a) * r.uniform(0, 70), 0.0), rad * 22.0, steps=(5, 8))
    return m


def _extend_bounds(sm, x, y, z):
    """A mesh that travels by its material needs bounds that cover its whole round (culling, and the road-clearance test)."""
    sm.set_editor_property('positive_bounds_extension', unreal.Vector(x, y, z))
    sm.set_editor_property('negative_bounds_extension', unreal.Vector(x, y, z))
    AL.save_loaded_asset(sm)


def abyss_meshes():
    fm = abyss_fish_material()
    warm = [((1.0, 0.35, 0.22), (1.0, 0.72, 0.18)), ((1.0, 0.62, 0.12), (1.0, 0.86, 0.35)), ((1.0, 0.32, 0.55), (1.0, 0.7, 0.8))]
    cool = [((0.12, 0.85, 1.0), (0.75, 1.0, 1.0)), ((0.35, 0.55, 1.0), (0.8, 0.9, 1.0)), ((0.9, 0.95, 0.75), (1.0, 0.85, 0.3))]
    for name, mesh, mi, ext in (
            ('SM_FishSchoolA', _school(11, warm), fm['fish'], (1500, 1500, 120)),
            ('SM_FishSchoolB', _school(23, cool, n=34, centre_z=1050.0), fm['fish'], (1500, 1500, 120)),
            ('SM_Whale', _whale(), fm['whale'], (14500, 14500, 400)),
            ('SM_Manta', _manta(), fm['manta'], (5400, 5400, 500))):
        unreal.GeometryScript_Normals.recompute_normals(mesh, unreal.GeometryScriptCalculateNormalsOptions())
        so = unreal.GeometryScriptSplitNormalsOptions()
        so.set_editor_property('split_by_opening_angle', True)
        so.set_editor_property('opening_angle_deg', 40.0)
        unreal.GeometryScript_Normals.compute_split_normals(mesh, so, unreal.GeometryScriptCalculateNormalsOptions())
        sm = _save_static(mesh, '%s/%s' % (ABYL, name), mi, 'Fish')
        _extend_bounds(sm, *ext)
        log('abyss mesh', name, 'triangles', unreal.GeometryScript_MeshQueries.get_num_triangle_i_ds(mesh))
    sh = abyss_shaft_material()
    _save_static(_shaft(4600.0, 110.0, 560.0), ABYL + '/SM_LightShaft', sh, 'Shaft')
    _save_static(_shaft(6200.0, 160.0, 900.0, n=5, spread=900.0, seed=3), ABYL + '/SM_LightShaftBig', sh, 'Shaft')
    bm = abyss_bubble_material()
    bub = _bubbles()
    sm = _save_static(bub, ABYL + '/SM_Bubbles', bm, 'Bubble')
    _extend_bounds(sm, 80, 80, 1950)
    log('abyss meshes built')


def abyss_apply(save=True):
    """The caustics onto L_Abyss's key light, and a brighter, greener near water (L_Abyss must be the loaded level)."""
    EAS = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
    lf = AL.load_asset(ABY + '/M_AbyssCaustics') or abyss_caustics()
    for a in EAS.get_all_level_actors():
        if a.get_actor_label() == 'Moonlight':
            c = a.get_component_by_class(unreal.DirectionalLightComponent)
            c.set_editor_property('light_function_material', lf)
            c.set_editor_property('light_function_fade_distance', 60000.0)
            c.set_editor_property('disabled_brightness', 0.75)
            c.set_editor_property('intensity', 6.2)   # the function only takes light away: 4.5 lux before
            log('caustics on', a.get_actor_label())
    if save:
        unreal.get_editor_subsystem(unreal.LevelEditorSubsystem).save_current_level()


def build_abyss():
    abyss_surface()
    abyss_caustics()
    abyss_meshes()


# ------------------------------------------------------------------------------------------ Mount Kaldera erupts
# One mesh stands over each volcano on the horizon (AFTTrack puts a component of it on every cone) and its material
# plays the eruption on a cycle, from the engine's clock alone: no actor ticks, no particles, the same cost at every
# quality level. A cycle (Period seconds): the crater flickers and the lowest puffs cough (the rumble), the blast (a
# ring of fire thrown out from the rim), then the main event (puffs leave the crater one after another and swell into
# a leaning plume with a cap, lit from below by the fire and from the side by the level's own light, lightning
# flickering in it; five lava fountains pulse; fragments are thrown on their own arcs; lava runs down the flanks),
# then the aftermath (the fountains sink, the plume is fed no more, drifts down the wind and thins from the bottom
# up, the flows cool to a dull red), then a quiet spell. Each cone starts at its own second (custom primitive data 0,
# set by AFTTrack), so the one beside the early road blows as the player comes up to it.
# Vertex colour says what a part is: r = kind (0 plume, 0.18 fountain, 0.4 lava flow, 0.6 fragment, 0.8 the blast
# ring, 1 the crater's light), g = its own number, b = its radius (of MaxR), a = how high up (or how far down a flow)
# it belongs, 0 to 1.
# The first version (15 Nov, early) was judged from the road in editor captures: the plume was ash-dark on a
# near-black sky and its undersides one flat red, so from below it read as a red blob. Hence the three tones, the
# side light, the fire's glow only on the lowest puffs, and everything else that is new here.
VOL = '/Game/Maps/Volcano/Materials'
VOLL = '/Game/Maps/Volcano/Life'
ERUPT_TIME = r"""
float kind = VC.r;
float id = VC.g;
float ord = VA;
float t = Scrub >= 0.0 ? Scrub : fmod(T + Phase + 1000.0 * Period, Period);   // Scrub: hold the cycle at one second (stills, tuning)
float mainEv = smoothstep(Ignite, Ignite + 0.6, t) * (1.0 - smoothstep(MainEnd, MainEnd + 6.0, t));
"""
ERUPT_WPO = ERUPT_TIME + r"""
float rad = VC.b * MaxR * 1.15;
float3 N = normalize(Nl);
float3 crater = float3(0.0, 0.0, CraterZ);
float3 off = float3(0.0, 0.0, 0.0);
if (kind < 0.1)
{
    // the plume: each puff leaves the crater in turn and swells into its place; later it drifts off and thins
    float on = Ignite + ord * Feed;
    float e = saturate((t - on) / Rise);
    e = 1.0 - (1.0 - e) * (1.0 - e);
    float cough = (1.0 - smoothstep(0.0, 0.25, ord)) * 0.35 * saturate(sin(t * 2.1 + id * 30.0)) * (1.0 - smoothstep(Ignite - 0.5, Ignite, t));
    float offT = MainEnd + 2.0 + ord * Fade;
    float gone = smoothstep(offT, offT + 3.5, t);
    float s = max(e, cough) * (1.0 - gone);
    float3 c = Lp - N * rad / 1.15;
    float away = max(t - on, 0.0);
    float3 drift = float3(Wind.xy * away * DriftSpeed * (0.3 + ord), away * 22.0 * (0.4 + ord) + 900.0 * gone);
    off = (crater - c) * (1.0 - max(e, cough * 0.6)) + drift * step(on, t) - N * rad * (1.0 - s) + N * rad * 0.06 * sin(t * 1.1 + id * 40.0) * s;
}
else if (kind < 0.3)
{
    // the fountains: up in half a second, pulsing while the main event lasts, sinking after
    float h = mainEv * (0.72 + 0.28 * sin(t * 7.0 + id * 31.0));
    off = float3((Lp.xy - crater.xy) * (h - 1.0) * 0.3, (Lp.z - CraterZ) * (h - 1.0)) - N * rad * (1.0 - saturate(h * 3.0));
}
else if (kind < 0.5)
{
    // the lava flows lie still on the flanks: only their light moves (the shading below)
    off = float3(0.0, 0.0, 0.0);
}
else if (kind < 0.7)
{
    // the fragments: thrown one after another, each on its own arc
    float tau = fmod(t - Ignite + id * BombEvery * 7.0 + 1000.0 * BombEvery, BombEvery);
    float a = id * 97.0;
    float3 v = float3(cos(a), sin(a), 0.0) * (500.0 + 900.0 * frac(id * 13.7)) + float3(0.0, 0.0, 2300.0 + 1200.0 * frac(id * 7.3));
    float live = mainEv * (1.0 - smoothstep(BombEvery * 0.8, BombEvery * 0.95, tau));
    off = v * tau + float3(0.0, 0.0, -750.0 * tau * tau) - N * rad * (1.0 - live);
}
else if (kind < 0.9)
{
    // the blast: a ring thrown out from the rim at the first second, rising a little, gone in RingTime seconds
    float tau = (t - Ignite) / RingTime;
    float act = step(0.0, tau) * step(tau, 1.0);
    float k = 1.0 + RingReach * pow(saturate(tau), 0.6);
    float3 d = Lp - crater;
    off = lerp(-d, float3(d.xy * (k - 1.0), d.z * (k - 1.0) * 0.3 + 500.0 * saturate(tau)), act);
}
else
{
    // the crater's light: flickering in the rumble, a burst at the blast, a glow through the aftermath
    float burst = smoothstep(Ignite - 0.1, Ignite + 0.25, t) * (1.0 - smoothstep(Ignite + 0.3, Ignite + 2.5, t));
    float rumble = (0.25 + 0.15 * sin(t * 9.0) * sin(t * 2.3)) * (1.0 - smoothstep(Quiet, Quiet + 4.0, t));
    float s = max(rumble + 0.35 * mainEv, burst * 1.5);
    off = (Lp - crater) * (s - 1.0);
}
return off;
"""
ERUPT_SHADE = ERUPT_TIME + r"""
float3 N = normalize(Nw);
float3 col;
if (kind < 0.1)
{
    // ash in three flat tones: lit where it faces the level's light, dark underneath, a middle tone between
    float sun = dot(N, normalize(ToSun.xyz));
    col = lerp(AshDark.rgb, AshMid.rgb, smoothstep(-0.32, -0.26, sun));
    col = lerp(col, AshLit.rgb, smoothstep(0.24, 0.30, sun));
    // the fire below lights the undersides of the lowest puffs, hot yellow nearest the crater, then orange, then red
    float low = 1.0 - smoothstep(0.0, 0.42, ord);
    float under = smoothstep(0.18, -0.2, N.z) * low * (0.3 + 0.7 * mainEv);
    float3 fire = lerp(Fire.rgb * 1.3, Core.rgb * 2.4, smoothstep(0.55, 0.8, low * (0.6 + 0.4 * sin(t * 5.0 + id * 9.0))));
    col = lerp(col, fire, step(0.35, under));
    col = lerp(col, Fire.rgb * 0.55, step(0.12, under) * (1.0 - step(0.35, under)));
    // lightning in the ash: now one puff, now another, for a sixth of a second
    float tick = floor(t * 6.0);
    float flash = step(1.0 - Lightning, frac(sin(tick * 12.9898 + id * 78.233) * 43758.5453)) * mainEv * step(0.25, ord);
    col = lerp(col, float3(1.6, 1.5, 2.2), flash * 0.85);
}
else if (kind < 0.3)
{
    // the fountains: yellow-white at the vent, orange higher, red at the tips
    float3 c = lerp(Core.rgb * 5.0, Fire.rgb * 3.0, smoothstep(0.3, 0.5, ord));
    c = lerp(c, Fire.rgb * float3(1.3, 0.5, 0.5), smoothstep(0.7, 0.85, ord));
    col = c * (0.8 + 0.2 * sin(t * 13.0 + id * 20.0));
}
else if (kind < 0.5)
{
    // lava flows: the front runs down the flank in FlowTime seconds, bright pulses following it down; after the main
    // event it cools to a dull red and holds that through the quiet spell (dark before the first eruption's front)
    float front = saturate((t - Ignite - 1.0) / FlowTime);
    float reached = step(ord, front);
    float pulse = smoothstep(0.55, 0.75, frac(ord * 3.0 - t * 0.22 + id * 5.0));
    float hot = 1.0 - smoothstep(MainEnd, Quiet, t);
    float3 lava = lerp(Fire.rgb * 1.6, Core.rgb * 3.2, pulse * hot);
    float3 cool = Fire.rgb * float3(0.42, 0.22, 0.2);
    float past = step(t, Ignite + 1.0);                      // before this cycle's front: last cycle's cooling lava
    col = lerp(lerp(cool, lerp(cool, lava, hot), reached), cool, past);
    col = lerp(col, Core.rgb * 4.0, reached * step(front - 0.04, ord) * step(front, 0.999));   // the front itself
}
else if (kind < 0.7) { col = lerp(Core.rgb * 4.0, Fire.rgb * 3.0, frac(id * 3.3)); }
else if (kind < 0.9)
{
    float tau = saturate((t - Ignite) / RingTime);
    col = lerp(Core.rgb * 6.0, Fire.rgb * 1.5, smoothstep(0.15, 0.5, tau)) * (1.0 - smoothstep(0.7, 1.0, tau));
}
else { col = lerp(Fire.rgb * 2.0, Core.rgb * 5.0, saturate(0.25 + mainEv)) ; }
return col * Gain;
"""
ERUPT_PARAMS = [('Period', 75.0), ('Ignite', 9.0), ('MainEnd', 33.0), ('Quiet', 58.0), ('Feed', 9.0), ('Rise', 4.0), ('Fade', 16.0),
                ('DriftSpeed', 26.0), ('BombEvery', 3.6), ('MaxR', 2000.0), ('CraterZ', 3400.0), ('Gain', 1.0), ('Scrub', -1.0),
                ('RingTime', 2.6), ('RingReach', 7.0), ('FlowTime', 14.0), ('Lightning', 0.012)]
ERUPT_VECTORS = [('AshDark', (0.10, 0.05, 0.08, 1.0)), ('AshMid', (0.34, 0.17, 0.20, 1.0)), ('AshLit', (0.85, 0.50, 0.42, 1.0)),
                 ('Fire', (1.0, 0.25, 0.03, 1.0)), ('Core', (1.0, 0.72, 0.16, 1.0)), ('Wind', (1.0, 0.25, 0.0, 0.0)),
                 # towards the level's key light (L_Volcano's 'Moonlight': pitch -40, yaw 70; ft_v25.LEVEL_SETUP)
                 ('ToSun', (-0.26, -0.72, 0.64, 0.0))]


def eruption_material():
    m = ft_mats.new_material(VOL, 'M_Eruption', blend=unreal.BlendMode.BLEND_OPAQUE, shading=unreal.MaterialShadingModel.MSM_DEFAULT_LIT)
    ft_buildings._emissive_only(m)
    X = -1300
    vc = ft_mats.expr(m, unreal.MaterialExpressionVertexColor, x=X, y=-900)
    nw = ft_mats.expr(m, unreal.MaterialExpressionVertexNormalWS, x=X, y=-820)
    lp = _local_pos(m, X, -740)
    t = ft_mats.expr(m, unreal.MaterialExpressionTime, x=X, y=-660)
    P = _params(m, ERUPT_PARAMS, ERUPT_VECTORS)
    phase = ft_mats.sparam(m, 'Phase', 0.0, x=X, y=-580)
    phase.set_editor_property('use_custom_primitive_data', True)   # each cone its own second (AFTTrack sets it)
    phase.set_editor_property('primitive_data_index', 0)
    # the normal in the mesh's own space, for the vertex work (uniform scale and a turn about z only)
    nl = ft_mats.expr(m, unreal.MaterialExpressionTransform, x=X + 250, y=-820)
    nl.set_editor_property('transform_source_type', unreal.MaterialVectorCoordTransformSource.TRANSFORMSOURCE_WORLD)
    nl.set_editor_property('transform_type', unreal.MaterialVectorCoordTransform.TRANSFORM_LOCAL)
    MEL.connect_material_expressions(nw, '', nl, '')
    common = [('VC', vc, ''), ('VA', vc, 'A'), ('T', t, ''), ('Phase', phase, '')] + [(k, P[k], '') for k in ('Period', 'Ignite', 'MainEnd', 'Scrub')]
    sh = ft_mats.custom(m, ERUPT_SHADE, common + [('Nw', nw, '')] + [(k, P[k], '') for k in (
        'AshDark', 'AshMid', 'AshLit', 'Fire', 'Core', 'ToSun', 'Gain', 'Quiet', 'RingTime', 'FlowTime', 'Lightning')],
                        x=-500, y=0, desc='EruptionShade')
    MEL.connect_material_property(sh, '', unreal.MaterialProperty.MP_EMISSIVE_COLOR)
    off = ft_mats.custom(m, ERUPT_WPO, common + [('Nl', nl, ''), ('Lp', lp, '')] + [(k, P[k], '') for k in (
        'Quiet', 'Feed', 'Rise', 'Fade', 'DriftSpeed', 'BombEvery', 'MaxR', 'CraterZ', 'Wind', 'RingTime', 'RingReach')], x=-700, y=500, desc='EruptionMove')
    MEL.connect_material_property(_local_to_world(m, off, -350, 500), '', unreal.MaterialProperty.MP_WORLD_POSITION_OFFSET)
    ft_buildings._compile(m)
    return ft_mats.mi(m, VOL, 'MI_Eruption')


def _strut(dst, col, a, b, w, d):
    """A box from a to b, w wide and d deep (for the lava flows and the blast ring)."""
    dx, dy, dz = b[0] - a[0], b[1] - a[1], b[2] - a[2]
    L = math.sqrt(dx * dx + dy * dy + dz * dz) or 1.0
    yaw = math.degrees(math.atan2(dy, dx))
    pitch = math.degrees(math.atan2(dz, math.hypot(dx, dy)))
    _slab(dst, col, ((a[0] + b[0]) / 2.0, (a[1] + b[1]) / 2.0, (a[2] + b[2]) / 2.0), (L, w, d), rot=(0, pitch, yaw))   # this module's _xf takes (roll, pitch, yaw)


def _eruption_mesh(H=3400.0, maxr=2000.0, seed=31):
    """In the volcano mesh's own space (ft_v25.volcano: a cone 3000 in radius at the base, a shoulder of 1650 at
    z = 1870, the rim 540 at z = 3230; its front, where its own painted lava runs, is -X)."""
    import random
    r = random.Random(seed)
    m = _new()
    n = 24
    for k in range(n):                                    # the plume's column, as it stands at its fullest
        u = (k + 0.5) / n
        z = H + 350 + (u ** 0.85) * 9000.0
        spread = 220 + u * 1900.0
        rad = 900 + u * 1100.0 + r.uniform(-120, 160)   # big enough to close over the cone's own painted plume
        a = r.uniform(0, 6.2832)
        x = u * u * 2600.0 + math.cos(a) * spread * r.uniform(0.2, 1.0)
        y = math.sin(a) * spread * r.uniform(0.2, 1.0)
        _ball(m, (0.0, r.random(), min(rad / maxr, 1.0), u * 0.86), (x, y, z), rad, scale=(1.0, 1.0, r.uniform(0.75, 0.95)), steps=(6, 9))
    for k in range(8):                                    # and its cap, spreading where the column stops rising
        a = k * 0.7854 + 0.3
        rad = r.uniform(1300, 1750)
        _ball(m, (0.0, r.random(), min(rad / maxr, 1.0), 0.88 + 0.015 * k), (2600 + math.cos(a) * 2300, math.sin(a) * 2300, H + 9300 + r.uniform(-300, 500)),
              rad, scale=(1.15, 1.15, 0.62), steps=(6, 9))
    for j in range(5):                                    # five fountains, each four cones one on another
        a = j * 1.2566 + 0.4
        hj = r.uniform(2600, 3800)
        for sg in range(4):
            v0, v1 = sg / 4.0, (sg + 1) / 4.0
            r0, r1 = 260.0 * (1.0 - v0) + 30.0, 260.0 * (1.0 - v1) + 30.0
            s = unreal.DynamicMesh()
            GSP.append_cone(s, unreal.GeometryScriptPrimitiveOptions(), _xf(), r0, r1, hj / 4.0, 8, 0, sg == 3, BASE)
            _tint(s, (0.18, j / 5.0 + 0.05, r0 / maxr, (v0 + v1) / 2.0))
            unreal.GeometryScript_MeshEdits.append_mesh(m, s, _xf((math.cos(a) * (820 + 240 * v0), math.sin(a) * (820 + 240 * v0), H - 120 + v0 * hj)))   # a ring outside the painted plume
    for j in range(12):                                   # lava flows down the flanks, all the way round
        a = math.radians(15 + j * 30)
        ca, sa = -math.cos(a), math.sin(a)
        prof = [(550.0, 3225.0), (1100.0, 2550.0), (1664.0, 1870.0), (2330.0, 940.0), (2996.0, 30.0)]
        segs = 12
        for q in range(segs):
            f0, f1 = q / float(segs), (q + 1) / float(segs)

            def at(f):
                g = f * (len(prof) - 1)
                i = min(int(g), len(prof) - 2)
                w = g - i
                rr = prof[i][0] + (prof[i + 1][0] - prof[i][0]) * w - 10.0   # the cone has 14 flat sides: sunk enough to touch their middles
                return (ca * rr - sa * 60.0 * math.sin(f * 7.0 + j * 2.0), sa * rr + ca * 60.0 * math.sin(f * 7.0 + j * 2.0),
                        prof[i][1] + (prof[i + 1][1] - prof[i][1]) * w)
            s = _new()
            _strut(s, (0.4, (j + 0.5) / 12.0, 0.05, (f0 + f1) / 2.0), at(f0), at(f1), 70.0 + 90.0 * f0, 130.0)
            unreal.GeometryScript_MeshEdits.append_mesh(m, s, _xf())
    for k in range(20):                                   # fragments, waiting in the crater
        _ball(m, (0.6, (k + 0.5) / 20.0, 150.0 / maxr, 0.0), (0, 0, H + 80), 150.0, steps=(4, 6))
    for k in range(18):                                   # the blast ring, at the rim
        a0, a1 = k * 0.34907, (k + 1) * 0.34907
        s = _new()
        _strut(s, (0.8, k / 18.0, 0.05, 0.0), (math.cos(a0) * 620, math.sin(a0) * 620, H - 60), (math.cos(a1) * 620, math.sin(a1) * 620, H - 60), 110.0, 70.0)
        unreal.GeometryScript_MeshEdits.append_mesh(m, s, _xf())
    _ball(m, (1.0, 0.0, 0.0, 0.0), (0, 0, H - 40), 520.0, scale=(1.0, 1.0, 0.4), steps=(6, 10))   # the crater's light
    return m


def build_volcano():
    mi = eruption_material()
    mesh = _eruption_mesh()
    sm = _save_static(mesh, VOLL + '/SM_Eruption', mi, 'Eruption')
    _extend_bounds(sm, 6500, 6500, 3500)
    log('eruption mesh triangles', unreal.GeometryScript_MeshQueries.get_num_triangle_i_ds(mesh))


# ------------------------------------------------------------------------------------------ the land's own surface
# The land beside the road was one flat colour per band with faint washes (M_Ground). On banked turns, elevated
# stretches, loops and corkscrews the chase camera sees mostly that land, and nothing tall may stand near the road
# there: those frames were a bare slab. So the land gets a surface of its own, level by level, drawn in the material
# over the level's own ground colours (the vertex colours, as before): slabs with joints, wind or tide ripples, or
# cracked ground, in two or three flat tones like everything else in the game. It costs nothing to place, follows
# every bank and cutting, and is never in the way of the road.
# AFTTrack uses /Game/Maps/<Level>/Materials/MI_Land for the land and the bed when it exists (ft.Land 0: the plain
# ground, for a before and an after).
LAND = '/Game/Maps/Land'
LAND_HLSL = r"""
// VC: the level's ground colour. WP: world position, cm. The washes are M_Ground's own, kept so the far land is unchanged.
float2 w = WP.xy * 0.0013;
float ridges = sin(w.x + 1.2 * sin(w.y * 1.4)) * cos(w.y * 0.95 + w.x * 0.33);
float washes = sin(w.x * 0.42 - w.y * 0.65) * sin(w.x * 0.8 + w.y * 0.38);
float layers = floor((ridges * 0.58 + washes * 0.42 + 1.0) * 2.0) * 0.25;
float2 gritCell = floor(WP.xy * 0.036);
float grit = frac(dot(frac(gritCell * float2(0.1273, 0.3129)), float2(7.13, 8.63)));
float blur = saturate(length(fwidth(WP.xy)) * 0.023);
float3 col = VC.rgb * (0.86 + 0.26 * layers + lerp(grit - 0.5, 0.0, blur) * 0.09);

// which surface: Kind, or KindWarm where the ground is a warm colour (more green than blue: sand beside a town's stone)
float kind = (KindWarm >= 0.0 && VC.g > VC.b) ? KindWarm : Kind;
float ca = cos(Angle), sa = sin(Angle);
float2 q = float2(WP.x * ca + WP.y * sa, WP.y * ca - WP.x * sa) / Cell;     // pattern space, in cells
float px = length(fwidth(q));                                               // cells to a pixel
float keep = (1.0 - smoothstep(0.10, 0.36, px)) * Amount;                   // gone before it can shimmer
float tone = 0.0;
// (the derivatives are taken above: none inside these branches)
BRANCH
if (kind < 0.5)
{
    // slabs: a grid of them, each one of three shades, a joint between
    float2 f = abs(frac(q) - 0.5);
    float joint = smoothstep(0.5 - Joint - px, 0.5 - Joint, max(f.x, f.y));
    float2 id = floor(q);
    float h = frac(sin(dot(id, float2(127.1, 311.7))) * 43758.5453);
    tone = lerp((floor(h * 3.0) - 1.0) * Vary, -Dark, joint);
}
else
{
BRANCH
if (kind < 1.5)
{
    // ripples: bands that wander (sand under wind or tide, drifted snow, mown stripes)
    float wob = Warp * sin(q.y * 0.37 + 1.7 * sin(q.x * 0.21 + q.y * 0.13));
    float s = frac(q.x + wob);
    float band = smoothstep(Duty - px, Duty, s) * (1.0 - smoothstep(1.0 - px, 1.0, s));
    tone = lerp(Vary, -Dark, band);
}
else
{
    // cracked ground: plates, each one of three shades, a crack between
    float2 ip = floor(q), fp = frac(q);
    float f1 = 8.0, f2 = 8.0;
    float2 idc = ip;
    for (int j = -1; j <= 1; j++)
    for (int i = -1; i <= 1; i++)
    {
        float2 g = float2(i, j);
        float2 o = frac(sin(float2(dot(ip + g, float2(127.1, 311.7)), dot(ip + g, float2(269.5, 183.3)))) * 43758.5453);
        float2 r = g + o - fp;
        float d = dot(r, r);
        if (d < f1) { f2 = f1; f1 = d; idc = ip + g; }
        else if (d < f2) { f2 = d; }
    }
    float crack = 1.0 - smoothstep(Joint, Joint + px * 1.5 + 0.02, sqrt(f2) - sqrt(f1));
    float h = frac(sin(dot(idc, float2(12.9898, 78.233))) * 43758.5453);
    tone = lerp((floor(h * 3.0) - 1.0) * Vary, -Dark, crack);
}
}
return col * (1.0 + keep * tone);
"""
# Kind: 0 slabs, 1 ripples, 2 cracked ground. KindWarm: the same choice for ground of a warm colour (more green than
# blue), or -1 for no difference: one material serves a level whose land is two things. Cell: the size of one slab, ripple or plate, cm. Angle: radians, the
# pattern's turn in the world. Joint: the joint's or crack's width, in cells. Dark: how much darker the joint, crack or
# band is. Vary: how far the shades of the slabs or plates stand apart. Warp: how far the ripples wander, in cells.
# Duty: the share of a ripple that is the light part. Amount: the whole pattern's strength.
LAND_PARAMS = [('Kind', 0.0), ('KindWarm', -1.0), ('Cell', 600.0), ('Angle', 0.0), ('Joint', 0.035), ('Dark', 0.16), ('Vary', 0.05), ('Warp', 0.5), ('Duty', 0.6),
               ('Amount', 1.0)]
# One surface per level (its folder under /Game/Maps). The beach and the town of Bayshore share one material: the
# land's colour says which is which (KindWarm), so the sand is rippled and the town's ground is laid stone.
LAND_LEVELS = {
    'Bayshore':      dict(Kind=0, KindWarm=1, Cell=460.0, Angle=0.5, Joint=0.03, Dark=0.12, Vary=0.05, Warp=0.55, Duty=0.58),
    'ChemicalWorks': dict(Kind=0, Cell=900.0, Angle=0.26, Joint=0.022, Dark=0.30, Vary=0.07),
    'Nevada':        dict(Kind=2, Cell=1500.0, Joint=0.035, Dark=0.20, Vary=0.05),
    'Volcano':       dict(Kind=2, Cell=1100.0, Joint=0.05, Dark=0.34, Vary=0.08),
    'ThemePark':     dict(Kind=0, Cell=560.0, Angle=0.785, Joint=0.03, Dark=0.15, Vary=0.085),
    'Glacier':       dict(Kind=1, Cell=780.0, Angle=1.1, Dark=0.055, Vary=0.02, Warp=0.8, Duty=0.66),
    'Abyss':         dict(Kind=1, Cell=380.0, Angle=0.2, Dark=0.12, Vary=0.04, Warp=0.7, Duty=0.5),
    'Metro':         dict(Kind=0, Cell=480.0, Angle=0.0, Joint=0.03, Dark=0.26, Vary=0.06),
}


def land_materials(levels=None):
    """M_Land and one MI_Land per level (AFTTrack loads /Game/Maps/<Level>/Materials/MI_Land by path)."""
    m = ft_mats.new_material(LAND, 'M_Land')
    vc = ft_mats.expr(m, unreal.MaterialExpressionVertexColor, x=-1300, y=-500)
    wp = ft_mats.expr(m, unreal.MaterialExpressionWorldPosition, x=-1300, y=-420)
    P = _params(m, LAND_PARAMS, [])
    cu = ft_mats.custom(m, LAND_HLSL, [('VC', vc, ''), ('WP', wp, '')] + [(k, v, '') for k, v in P.items()], x=-500, y=0, desc='Land')
    MEL.connect_material_property(cu, '', unreal.MaterialProperty.MP_BASE_COLOR)
    MEL.connect_material_property(ft_mats.sparam(m, 'Rough', 0.9, x=-500, y=260), '', unreal.MaterialProperty.MP_ROUGHNESS)
    MEL.connect_material_property(ft_mats.sparam(m, 'Metal', 0.0, x=-500, y=340), '', unreal.MaterialProperty.MP_METALLIC)
    ft_buildings._compile(m)
    for name, values in LAND_LEVELS.items():
        if levels and name not in levels:
            continue
        ft_mats.mi(m, '/Game/Maps/%s/Materials' % name, 'MI_Land', scalars={k: float(v) for k, v in values.items()})
        log('land', name, values)
    return m


def build_all():
    bay_sea()
    bay_cloud_material()
    bay_cloud_meshes()
    build_abyss()
    build_volcano()
    log('built')
