"""BayshoreSky meshes (Blender 5.2, headless). Writes FBX files to SourceArt/BayshoreSky/meshes/.

Run: blender.exe -b --factory-startup --python-exit-code 1 -P make_meshes.py

Units: Blender metres; exported with FBX_SCALE_UNITS so 1 m = 100 Unreal units.
Backdrop meshes are unit-sized and scaled by their components (the materials shade them by view direction).
"""
import bpy
import bmesh
import math
import os
from mathutils import Vector

OUT = r"K:\Full Throttle\SkyScratch\SourceArt\BayshoreSky\meshes"
os.makedirs(OUT, exist_ok=True)


def log(*a):
    print("BSKYM", *a, flush=True)


def clear():
    for ob in list(bpy.data.objects):
        bpy.data.objects.remove(ob, do_unlink=True)
    for me in list(bpy.data.meshes):
        bpy.data.meshes.remove(me)


def to_object(bm, name, uv=True):
    me = bpy.data.meshes.new(name)
    bm.to_mesh(me)
    bm.free()
    ob = bpy.data.objects.new(name, me)
    bpy.context.scene.collection.objects.link(ob)
    return ob


def export(objs, filename):
    for o in bpy.context.scene.objects:
        o.select_set(False)
    for o in objs:
        o.select_set(True)
    bpy.context.view_layer.objects.active = objs[0]
    path = os.path.join(OUT, filename)
    bpy.ops.export_scene.fbx(filepath=path, check_existing=False, use_selection=True, object_types={'MESH', 'EMPTY'},
                             global_scale=1.0, apply_unit_scale=True, apply_scale_options='FBX_SCALE_UNITS',
                             axis_forward='-Z', axis_up='Y', use_space_transform=True, bake_space_transform=False,
                             use_mesh_modifiers=True, mesh_smooth_type='FACE', colors_type='LINEAR',
                             prioritize_active_color=True, use_tspace=False, use_triangles=True,
                             use_custom_props=False, bake_anim=False, add_leaf_bones=False, path_mode='AUTO',
                             embed_textures=False)
    tris = sum(len(o.data.polygons) for o in objs)
    log("exported", filename, "faces", tris)


def tri_count(ob):
    return sum(len(p.vertices) - 2 for p in ob.data.polygons)


def flip_all(bm):
    bmesh.ops.reverse_faces(bm, faces=bm.faces[:])


def add_uv(bm):
    return bm.loops.layers.uv.new("UVMap")


# ------------------------------------------------------------------------------------------------ backdrop shells
def sphere_shell(name, seg=64, rings=32, el_min=-90.0, el_max=90.0, inward=True):
    bm = bmesh.new()
    uvl = add_uv(bm)
    rows = []
    for j in range(rings + 1):
        el = math.radians(el_min + (el_max - el_min) * j / rings)
        row = []
        for i in range(seg):
            a = 2 * math.pi * i / seg
            row.append(bm.verts.new((math.cos(el) * math.cos(a), math.cos(el) * math.sin(a), math.sin(el))))
        rows.append(row)
    for j in range(rings):
        for i in range(seg):
            i2 = (i + 1) % seg
            f = bm.faces.new((rows[j][i], rows[j][i2], rows[j + 1][i2], rows[j + 1][i]))
            for k, l in enumerate(f.loops):
                ii = i if k in (0, 3) else i + 1
                jj = j if k in (0, 1) else j + 1
                l[uvl].uv = (ii / seg, jj / rings)
    bmesh.ops.remove_doubles(bm, verts=bm.verts[:], dist=1e-6)
    bm.normal_update()
    # faces built counter-clockwise seen from outside; flip to face inward
    if inward:
        flip_all(bm)
    return to_object(bm, name)


def cylinder_shell(name, el_min, el_max, seg=96, rows=8, inward=True):
    bm = bmesh.new()
    uvl = add_uv(bm)
    z0, z1 = math.tan(math.radians(el_min)), math.tan(math.radians(el_max))
    grid = []
    for j in range(rows + 1):
        z = z0 + (z1 - z0) * j / rows
        grid.append([bm.verts.new((math.cos(2 * math.pi * i / seg), math.sin(2 * math.pi * i / seg), z)) for i in range(seg)])
    for j in range(rows):
        for i in range(seg):
            i2 = (i + 1) % seg
            f = bm.faces.new((grid[j][i], grid[j][i2], grid[j + 1][i2], grid[j + 1][i]))
            for k, l in enumerate(f.loops):
                ii = i if k in (0, 3) else i + 1
                jj = j if k in (0, 1) else j + 1
                l[uvl].uv = (ii / seg, jj / rows)
    if inward:
        flip_all(bm)
    return to_object(bm, name)


def quad(name, w, h, facing_neg_x=True):
    bm = bmesh.new()
    uvl = add_uv(bm)
    # in the YZ plane, centred, normal -X (toward a viewer on -X)
    vs = [bm.verts.new((0, -w / 2, -h / 2)), bm.verts.new((0, w / 2, -h / 2)),
          bm.verts.new((0, w / 2, h / 2)), bm.verts.new((0, -w / 2, h / 2))]
    f = bm.faces.new(vs)
    uvs = [(0, 0), (1, 0), (1, 1), (0, 1)]
    for l, uv in zip(f.loops, uvs):
        l[uvl].uv = uv
    bm.normal_update()
    if (f.normal.x > 0) == facing_neg_x:
        flip_all(bm)
    return to_object(bm, name)


def flare_ghosts(name, count=6):
    bm = bmesh.new()
    uvl = add_uv(bm)
    col = bm.loops.layers.color.new("Col")
    for g in range(count):
        x = g * 0.01  # tiny offsets so the quads are distinct; placement happens in the material
        vs = [bm.verts.new((x, -0.5, -0.5)), bm.verts.new((x, 0.5, -0.5)), bm.verts.new((x, 0.5, 0.5)), bm.verts.new((x, -0.5, 0.5))]
        f = bm.faces.new(vs)
        for l, uv in zip(f.loops, [(0, 0), (1, 0), (1, 1), (0, 1)]):
            l[uvl].uv = uv
            l[col] = (g / max(1, count - 1), 0.0, 0.0, 1.0)
    return to_object(bm, name)


# ------------------------------------------------------------------------------------------------ blimp
L = 60.0          # length (m), nose at +X
HULL_U = 0.9
RMAX = 7.6


def hull_radius(x):
    t = (x + L / 2) / L               # 0 tail .. 1 nose
    t = min(max(t, 0.0), 1.0)
    # blunt nose, long taper to the tail
    nose = math.sin(math.pi * 0.5 * min(1.0, (1.0 - t) / 0.22)) ** 0.55
    tail = math.sin(math.pi * 0.5 * min(1.0, t / 0.62)) ** 0.85
    return RMAX * nose * tail


def blimp_hull(bm, uvl, ring_n, seg_n):
    rings = []
    xs = []
    for j in range(ring_n + 1):
        s = j / ring_n
        x = -L / 2 + L * (0.5 - 0.5 * math.cos(math.pi * s))  # denser at the ends
        xs.append(x)
    tail_v = bm.verts.new((-L / 2, 0, 0))
    nose_v = bm.verts.new((L / 2, 0, 0))
    for j in range(1, ring_n):
        r = hull_radius(xs[j])
        row = []
        for i in range(seg_n):
            th = 2 * math.pi * i / seg_n       # 0 = top (+Z), 90 = +Y side
            row.append(bm.verts.new((xs[j], r * math.sin(th), r * math.cos(th))))
        rows_j = row
        rings.append((xs[j], row))

    def uv_of(x, i):
        u = (x + L / 2) / L * HULL_U  # hull uses u 0..0.9; fins and gondola use the strip above 0.92
        v_ue = i / seg_n                   # Unreal v = theta / 360 (Blender v is flipped on import)
        return (u, 1.0 - v_ue)

    for k in range(len(rings) - 1):
        xa, ra = rings[k]
        xb, rb = rings[k + 1]
        for i in range(seg_n):
            i2 = (i + 1) % seg_n
            f = bm.faces.new((ra[i], ra[i2], rb[i2], rb[i]))
            for l, (x, ii) in zip(f.loops, [(xa, i), (xa, i + 1), (xb, i + 1), (xb, i)]):
                l[uvl].uv = uv_of(x, ii)
    # caps
    xa, ra = rings[0]
    for i in range(seg_n):
        i2 = (i + 1) % seg_n
        f = bm.faces.new((tail_v, ra[i2], ra[i]))
        for l, (x, ii) in zip(f.loops, [(-L / 2, i + 0.5), (xa, i + 1), (xa, i)]):
            l[uvl].uv = uv_of(x, ii)
    xb, rb = rings[-1]
    for i in range(seg_n):
        i2 = (i + 1) % seg_n
        f = bm.faces.new((nose_v, rb[i], rb[i2]))
        for l, (x, ii) in zip(f.loops, [(L / 2, i + 0.5), (xb, i), (xb, i + 1)]):
            l[uvl].uv = uv_of(x, ii)


def box(bm, uvl, centre, size, rot_x=0.0, uv_rect=(0.93, 0.05, 0.99, 0.45), taper=None):
    res = bmesh.ops.create_cube(bm, size=1.0)
    for v in res["verts"]:
        x, y, z = v.co.x * size[0], v.co.y * size[1], v.co.z * size[2]
        if taper is not None and v.co.z > 0:
            x = x * taper + size[0] * (1 - taper) * -0.5   # swept-back top edge
        cy = y * math.cos(rot_x) - z * math.sin(rot_x)
        cz = y * math.sin(rot_x) + z * math.cos(rot_x)
        v.co = Vector((centre[0] + x, centre[1] + cy, centre[2] + cz))
    faces = set()
    for v in res["verts"]:
        faces.update(v.link_faces)
    u0, v0, u1, v1 = uv_rect
    for f in faces:
        for l, uv in zip(f.loops, [(u0, v0), (u1, v0), (u1, v1), (u0, v1)]):
            l[uvl].uv = uv


def blimp(name, ring_n, seg_n, fins=True, gondola=True):
    bm = bmesh.new()
    uvl = add_uv(bm)
    blimp_hull(bm, uvl, ring_n, seg_n)
    if fins:
        # cruciform tail fins, accent-coloured region of the mask (u 0.94..0.99, v 0.02..0.06)
        for k in range(4):
            ang = math.pi / 2 * k
            r = hull_radius(-L / 2 + 7.0)
            c = (-L / 2 + 6.5, 0.0, 0.0)
            # fin as a thin tapered slab sticking out radially
            out = r + 3.0
            cy, cz = math.sin(ang) * out * 0.5, math.cos(ang) * out * 0.5
            box(bm, uvl, (c[0], cy, cz), (9.0, 0.35, out), rot_x=-ang, taper=0.55)
    if gondola:
        box(bm, uvl, (6.0, 0.0, -RMAX - 0.9), (9.0, 2.6, 2.2), uv_rect=(0.93, 0.55, 0.99, 0.95))
    bm.normal_update()
    bmesh.ops.recalc_face_normals(bm, faces=bm.faces[:])
    return to_object(bm, name)


# ------------------------------------------------------------------------------------------------ gull
def gull(name):
    bm = bmesh.new()
    uvl = add_uv(bm)
    col = bm.loops.layers.color.new("Col")
    span = 1.2
    # body: elongated double cone along X
    pts = [(-0.30, 0, 0), (0.0, 0.05, 0.02), (0.0, -0.05, 0.02), (0.0, 0, -0.04), (0.0, 0, 0.06), (0.25, 0, 0.01)]
    v = [bm.verts.new(p) for p in pts]
    body_faces = [(0, 1, 4), (0, 4, 2), (0, 2, 3), (0, 3, 1), (5, 4, 1), (5, 2, 4), (5, 3, 2), (5, 1, 3)]
    for f in body_faces:
        fc = bm.faces.new([v[i] for i in f])
        for l in fc.loops:
            l[col] = (0.0, 0.0, 0.0, 1.0)
            l[uvl].uv = (0.5, 0.5)
    # wings: inner and outer panels with an upward elbow ("gull wing")
    for side in (1, -1):
        root_f = bm.verts.new((0.08, side * 0.04, 0.03))
        root_b = bm.verts.new((-0.10, side * 0.04, 0.03))
        elbow_f = bm.verts.new((0.05, side * span * 0.25, 0.09))
        elbow_b = bm.verts.new((-0.14, side * span * 0.25, 0.08))
        tip = bm.verts.new((-0.12, side * span * 0.5, 0.0))
        tip_b = bm.verts.new((-0.20, side * span * 0.46, 0.0))
        panels = [(root_f, elbow_f, elbow_b, root_b), (elbow_f, tip, tip_b, elbow_b)]
        for p in panels:
            vs = p if side > 0 else tuple(reversed(p))
            fc = bm.faces.new(vs)
            for l in fc.loops:
                w = abs(l.vert.co.y) / (span * 0.5)
                l[col] = (min(1.0, w), 1.0 if w > 0.8 else 0.0, 0.0, 1.0)
                l[uvl].uv = (0.5, 0.5)
            # single-sided: M_BSky_Gull is two-sided
    bm.normal_update()
    return to_object(bm, name)


# ------------------------------------------------------------------------------------------------ build and export
clear()
ob = sphere_shell("SM_BSky_Dome", 64, 32)   # full sphere: M_SkyDome paints gradient, horizon strip and sea by view direction
export([ob], "SM_BSky_Dome.fbx")
clear()
ob = cylinder_shell("SM_BSky_HorizonRing", -2.5, 6.5, 128, 2)   # below -2.5 deg the level's own terrain fills the view
export([ob], "SM_BSky_HorizonRing.fbx")
clear()
ob = quad("SM_BSky_CloudCard", 1.0, 0.5)
export([ob], "SM_BSky_CloudCard.fbx")
clear()
ob = quad("SM_BSky_SunQuad", 1.0, 1.0)
export([ob], "SM_BSky_SunQuad.fbx")
clear()
ob = flare_ghosts("SM_BSky_FlareGhosts", 6)
export([ob], "SM_BSky_FlareGhosts.fbx")
clear()
for lod, (rn, sn) in enumerate([(30, 32), (16, 16), (8, 10)]):
    ob = blimp("SM_BSky_Blimp_LOD%d" % lod, rn, sn, fins=True, gondola=lod < 2)
    log("blimp LOD%d tris %d" % (lod, tri_count(ob)))
    export([ob], "SM_BSky_Blimp_LOD%d.fbx" % lod)
    clear()
ob = blimp("SM_BSky_BlimpOutline", 16, 16, fins=True, gondola=True)
bm = bmesh.new()
bm.from_mesh(ob.data)
flip_all(bm)
bm.to_mesh(ob.data)
bm.free()
log("outline tris %d" % tri_count(ob))
export([ob], "SM_BSky_BlimpOutline.fbx")
clear()
ob = gull("SM_BSky_Gull")
log("gull tris %d" % tri_count(ob))
export([ob], "SM_BSky_Gull.fbx")
log("all meshes done")
