"""BayshoreSky bakes (Blender 5.2, Cycles on the GPU, headless).

Run (from any folder):
  blender.exe -b --factory-startup --python-exit-code 1 -P bake_sky.py -- --preset Day --az 30 --el 35 [--stages atmo,bank,atlas,hills,ref,compose] [--quality 1.0]

Conventions are in SourceArt/BayshoreSky/SPEC.md. Unreal azimuth az_ue is measured from +X toward +Y;
Blender compass azimuth az_b (0 = +Y, +90 = +X) is az_b = 90 + az_ue (Unreal y = -Blender y).
Raw renders go to bakes/<preset>/raw, textures ready to import to bakes/<preset>/, reference images to reference/<preset>/.
"""
import bpy
import sys
import os
import math
import json
import time

import numpy as np
import OpenImageIO as oiio
import openvdb as vdb
from mathutils import Vector

ROOT = r"K:\Full Throttle\SkyScratch\SourceArt\BayshoreSky"

# ------------------------------------------------------------------------------------------------ args
ARGV = sys.argv[sys.argv.index("--") + 1:] if "--" in sys.argv else []


def arg(name, default):
    key = "--" + name
    if key in ARGV:
        return type(default)(ARGV[ARGV.index(key) + 1])
    return default


PRESET = arg("preset", "Day")
AZ_UE = arg("az", 30.0)          # sun azimuth, Unreal convention, degrees
EL = arg("el", 6.0)              # sun elevation, degrees: golden hour (owner, 14 Nov); must match bsky_params.SUN_EL
STAGES = arg("stages", "atmo,bank,atlas,hills,ref,compose").split(",")
QUALITY = arg("quality", 1.0)    # sample multiplier
SEED = arg("seed", 7)
TOWN = arg("town", 0)            # 1 = bake a stand-in skyline into the hills; 0 for the port (Level 1 has its own skyline props)

AZ_B = 90.0 + AZ_UE
OUT = os.path.join(ROOT, "bakes", PRESET)
RAW = os.path.join(OUT, "raw")
REF = os.path.join(ROOT, "reference", PRESET)
for d in (OUT, RAW, REF):
    os.makedirs(d, exist_ok=True)

T0 = time.time()


def log(*a):
    print("BSKYB [%6.1fs]" % (time.time() - T0), *a, flush=True)


def sun_dir_b(az_b_deg=AZ_B, el_deg=EL):
    a, e = math.radians(az_b_deg), math.radians(el_deg)
    return Vector((math.sin(a) * math.cos(e), math.cos(a) * math.cos(e), math.sin(e)))


# ------------------------------------------------------------------------------------------------ image io
def oiio_write(path, data, fmt, attrs=(), names=None):
    data = np.ascontiguousarray(data)
    if data.ndim == 2:
        data = data[:, :, None]
    h, w, c = data.shape
    spec = oiio.ImageSpec(w, h, c, fmt)
    if names:
        spec.channelnames = tuple(names)
    for k, v in attrs:
        spec.attribute(k, v)
    out = oiio.ImageOutput.create(path)
    if out is None or not out.open(path, spec):
        raise RuntimeError("cannot write %s: %s" % (path, oiio.geterror()))
    if not out.write_image(data):
        raise RuntimeError("write failed %s: %s" % (path, out.geterror()))
    out.close()


def write_png(path, rgba01, bits=8):
    x = np.clip(rgba01, 0.0, 1.0)
    if bits == 8:
        oiio_write(path, np.round(x * 255).astype(np.uint8), "uint8", attrs=[("oiio:UnassociatedAlpha", 1)])
    else:
        oiio_write(path, np.round(x * 65535).astype(np.uint16), "uint16", attrs=[("oiio:UnassociatedAlpha", 1)])


def write_exr(path, data, half=True):
    oiio_write(path, data.astype(np.float32), "half" if half else "float", attrs=[("compression", "zip")])


def read_exr_all(path):
    inp = oiio.ImageInput.open(path)
    if inp is None:
        raise RuntimeError("cannot open %s: %s" % (path, oiio.geterror()))
    out = {}
    sub = 0
    while inp.seek_subimage(sub, 0):
        spec = inp.spec()
        px = np.asarray(inp.read_image(sub, 0, 0, spec.nchannels, "float")).reshape(spec.height, spec.width, spec.nchannels)
        for i, n in enumerate(spec.channelnames):
            out[n] = px[:, :, i].astype(np.float32)
        sub += 1
    inp.close()
    return out


def ch(d, pass_name, c):
    suffix = "." + pass_name + "." + c
    for k, v in d.items():
        if k.endswith(suffix) or k == pass_name + "." + c:
            return v
    raise KeyError("no channel %s.%s in %s" % (pass_name, c, sorted(d.keys())[:40]))


def lum(rgb):
    return rgb[..., 0] * 0.2126 + rgb[..., 1] * 0.7152 + rgb[..., 2] * 0.0722


# ------------------------------------------------------------------------------------------------ numpy helpers
def box_blur_axis(a, r, axis, wrap=False):
    if r < 1:
        return a
    n = a.shape[axis]
    pad = [(0, 0)] * a.ndim
    pad[axis] = (r + 1, r)
    a2 = np.pad(a, pad, mode="wrap" if wrap else "edge")
    c = np.cumsum(a2, axis=axis, dtype=np.float64)
    hi = np.take(c, np.arange(2 * r + 1, 2 * r + 1 + n), axis=axis)
    lo = np.take(c, np.arange(0, n), axis=axis)
    return ((hi - lo) / (2 * r + 1)).astype(np.float32)


def gauss_blur(a, sigma, wrap_u=False):
    """Three box blurs approximate a gaussian. a: HxW or HxWxC."""
    if sigma <= 0:
        return a
    r = max(1, int(round(math.sqrt(12.0 * sigma * sigma / 3.0 + 1.0) / 2.0)))
    out = a.astype(np.float32)
    for _ in range(3):
        out = box_blur_axis(out, r, 1, wrap=wrap_u)
        out = box_blur_axis(out, r, 0, wrap=False)
    return out


def pull_push_fill(values, weight, levels=12):
    """Fill pixels with weight ~0 from their neighbourhood (premultiplied pyramid). values HxWxC, weight HxW."""
    w = np.clip(weight, 0, 1).astype(np.float32)
    v = values.astype(np.float32) * w[..., None]
    pyr = [(v, w)]
    for _ in range(levels):
        v, w = pyr[-1]
        h, ww = w.shape
        if h < 2 or ww < 2:
            break
        h2, w2 = h // 2 * 2, ww // 2 * 2
        v = v[:h2, :w2]
        w = w[:h2, :w2]
        v = (v[0::2, 0::2] + v[1::2, 0::2] + v[0::2, 1::2] + v[1::2, 1::2]) * 0.25
        w = (w[0::2, 0::2] + w[1::2, 0::2] + w[0::2, 1::2] + w[1::2, 1::2]) * 0.25
        pyr.append((v, w))
    # push down
    fv, fw = pyr[-1]
    fill = fv / np.maximum(fw, 1e-8)[..., None]
    for v, w in reversed(pyr[:-1]):
        h, ww = w.shape
        up = np.repeat(np.repeat(fill, 2, axis=0), 2, axis=1)
        up = np.pad(up, ((0, max(0, h - up.shape[0])), (0, max(0, ww - up.shape[1])), (0, 0)), mode="edge")[:h, :ww]
        a = np.clip(w * 4.0, 0, 1)[..., None]
        own = v / np.maximum(w, 1e-8)[..., None]
        fill = own * a + up * (1 - a)
    return fill


def resample_rows(img, src_el_of_row, dst_el):
    """img rows sampled at elevations src_el_of_row (descending); returns rows at dst_el (linear interp)."""
    src = np.asarray(src_el_of_row)
    # make ascending for np.interp
    order = np.argsort(src)
    src_sorted = src[order]
    idx = np.interp(dst_el, src_sorted, np.arange(len(src_sorted)))
    i0 = np.floor(idx).astype(int)
    i1 = np.minimum(i0 + 1, len(src_sorted) - 1)
    f = (idx - i0).astype(np.float32)
    r0 = order[i0]
    r1 = order[i1]
    if img.ndim == 3:
        return img[r0] * (1 - f)[:, None, None] + img[r1] * f[:, None, None]
    return img[r0] * (1 - f)[:, None] + img[r1] * f[:, None]


def downsample_cols(img, factor):
    if factor == 1:
        return img
    h, w = img.shape[:2]
    return img.reshape(h, w // factor, factor, *img.shape[2:]).mean(axis=2)


# ------------------------------------------------------------------------------------------------ procedural noise
def value_noise3(p, lattice):
    n = lattice.shape[0]
    pi = np.floor(p).astype(np.int64)
    f = (p - pi).astype(np.float32)
    f = f * f * (3.0 - 2.0 * f)
    x0, y0, z0 = (pi[..., 0] % n), (pi[..., 1] % n), (pi[..., 2] % n)
    x1, y1, z1 = (x0 + 1) % n, (y0 + 1) % n, (z0 + 1) % n
    fx, fy, fz = f[..., 0], f[..., 1], f[..., 2]
    c00 = lattice[x0, y0, z0] * (1 - fx) + lattice[x1, y0, z0] * fx
    c10 = lattice[x0, y1, z0] * (1 - fx) + lattice[x1, y1, z0] * fx
    c01 = lattice[x0, y0, z1] * (1 - fx) + lattice[x1, y0, z1] * fx
    c11 = lattice[x0, y1, z1] * (1 - fx) + lattice[x1, y1, z1] * fx
    c0 = c00 * (1 - fy) + c10 * fy
    c1 = c01 * (1 - fy) + c11 * fy
    return c0 * (1 - fz) + c1 * fz


def fbm3(p, seed, octaves=5, lac=2.03, gain=0.5):
    rng = np.random.default_rng(seed)
    lattice = rng.random((48, 48, 48), dtype=np.float32)
    total = np.zeros(p.shape[:-1], np.float32)
    amp, norm, freq = 1.0, 0.0, 1.0
    for o in range(octaves):
        off = rng.random(3).astype(np.float32) * 31.0
        total += amp * value_noise3(p * freq + off, lattice)
        norm += amp
        amp *= gain
        freq *= lac
    return total / norm


def value_noise2(p, lattice):
    n = lattice.shape[0]
    pi = np.floor(p).astype(np.int64)
    f = (p - pi).astype(np.float32)
    f = f * f * (3.0 - 2.0 * f)
    x0, y0 = pi[..., 0] % n, pi[..., 1] % n
    x1, y1 = (x0 + 1) % n, (y0 + 1) % n
    a = lattice[x0, y0] * (1 - f[..., 0]) + lattice[x1, y0] * f[..., 0]
    b = lattice[x0, y1] * (1 - f[..., 0]) + lattice[x1, y1] * f[..., 0]
    return a * (1 - f[..., 1]) + b * f[..., 1]


def fbm2(p, seed, octaves=6, lac=2.0, gain=0.5, ridged=False):
    rng = np.random.default_rng(seed)
    lattice = rng.random((256, 256), dtype=np.float32)
    total = np.zeros(p.shape[:-1], np.float32)
    amp, norm, freq = 1.0, 0.0, 1.0
    for o in range(octaves):
        off = rng.random(2).astype(np.float32) * 97.0
        v = value_noise2(p * freq + off, lattice)
        if ridged:
            v = 1.0 - np.abs(v * 2.0 - 1.0)
            v = v * v
        total += amp * v
        norm += amp
        amp *= gain
        freq *= lac
    return total / norm


# ------------------------------------------------------------------------------------------------ cumulus density
def cumulus_density(seed, width, depth, height, res_x, kind="cumulus"):
    """Returns (density float32 [nx,ny,nz] in 0..1, half extents (m), voxel (m)). Domain centred on x/y, z from 0."""
    rng = np.random.default_rng(seed)
    vox = width / res_x
    nx = res_x
    ny = max(16, int(round(depth / vox)))
    nz = max(16, int(round(height / vox)))
    xs = (np.arange(nx, dtype=np.float32) + 0.5) * vox - width / 2
    ys = (np.arange(ny, dtype=np.float32) + 0.5) * vox - depth / 2
    zs = (np.arange(nz, dtype=np.float32) + 0.5) * vox
    X, Y, Z = np.meshgrid(xs, ys, zs, indexing="ij")
    P = np.stack([X, Y, Z], axis=-1)

    s = width / 2000.0  # shape scale relative to the 2 km reference cumulus
    if kind == "cumulus":
        n_lobes, spread, base_r, lift = rng.integers(6, 10), 0.62, 330.0, 300.0
    elif kind == "towering":
        n_lobes, spread, base_r, lift = rng.integers(7, 11), 0.45, 300.0, 430.0
    elif kind == "flat":
        n_lobes, spread, base_r, lift = rng.integers(9, 14), 0.78, 230.0, 120.0
    else:  # puff
        n_lobes, spread, base_r, lift = rng.integers(4, 7), 0.5, 300.0, 230.0

    sdf = np.full(X.shape, 1e9, np.float32)
    zmax_allowed = height * 0.93
    for i in range(n_lobes):
        t = rng.uniform(-1, 1)
        cx = t * spread * width / 2
        cy = rng.uniform(-0.25, 0.25) * depth / 2
        centre_w = 1.0 - abs(t)
        r = (base_r * (0.55 + 0.6 * centre_w) * rng.uniform(0.8, 1.15)) * s
        cz = (150.0 * s + r * 0.55 + lift * s * centre_w * rng.uniform(0.6, 1.1))
        cz = min(cz, zmax_allowed - r * 0.95)
        r_xy = r * rng.uniform(1.0, 1.25)
        d = np.sqrt(((X - cx) / r_xy) ** 2 + ((Y - cy) / (r * 0.95)) ** 2 + ((Z - cz) / r) ** 2) * r - r
        sdf = np.minimum(sdf, d)
        # cauliflower: smaller turrets on the upper surface of each lobe
        if kind != "flat":
            for c in range(int(rng.integers(1, 4))):
                ang = rng.uniform(-1.1, 1.1)
                rc = r * rng.uniform(0.38, 0.58)
                tx = cx + math.sin(ang) * r * 0.75
                tz = min(cz + math.cos(ang) * r * 0.72, zmax_allowed - rc)
                ty = cy + rng.uniform(-0.3, 0.3) * r
                d2 = np.sqrt((X - tx) ** 2 + (Y - ty) ** 2 + (Z - tz) ** 2) - rc
                sdf = np.minimum(sdf, d2)
    # billow displacement
    q = P / (360.0 * s)
    sdf += (fbm3(q, seed * 13 + 1, octaves=4) - 0.5) * 230.0 * s
    sdf += (fbm3(P / (110.0 * s), seed * 13 + 2, octaves=3) - 0.5) * 80.0 * s
    shell = 70.0 * s
    dens = np.clip(-sdf / shell, 0.0, 1.0)
    # flat-ish base and fade at the domain borders
    base = np.clip((Z - 110.0 * s) / (90.0 * s), 0, 1)
    edge = np.minimum.reduce([
        np.clip((X + width / 2) / (3 * vox), 0, 1), np.clip((width / 2 - X) / (3 * vox), 0, 1),
        np.clip((Y + depth / 2) / (3 * vox), 0, 1), np.clip((depth / 2 - Y) / (3 * vox), 0, 1),
        np.clip((height - Z) / (3 * vox), 0, 1)])
    dens = (dens * base * edge).astype(np.float32)
    # wispy interior variation
    dens *= (0.75 + 0.5 * fbm3(P / (200.0 * s), seed * 13 + 3, octaves=3)).astype(np.float32)
    return np.clip(dens, 0, 1).astype(np.float32), (width / 2, depth / 2, height), vox


def write_vdb(path, dens, half, vox):
    grid = vdb.FloatGrid()
    grid.copyFromArray(dens)
    grid.name = "density"
    hx, hy, hz = half
    # voxel centres at index positions: x = -hx + (i + 0.5) * vox
    grid.transform = vdb.createLinearTransform([[vox, 0, 0, 0], [0, vox, 0, 0], [0, 0, vox, 0],
                                                [-hx + vox / 2, -hy + vox / 2, vox / 2, 1]])
    vdb.write(path, grids=[grid])


# ------------------------------------------------------------------------------------------------ scene helpers
def new_scene(name):
    if name in bpy.data.scenes:
        bpy.data.scenes.remove(bpy.data.scenes[name])
    sc = bpy.data.scenes.new(name)
    sc.world = bpy.data.worlds.new(name + "_W")
    return sc


def enable_gpu(sc):
    prefs = bpy.context.preferences.addons["cycles"].preferences
    prefs.compute_device_type = "OPTIX"
    prefs.refresh_devices()
    for d in prefs.devices:
        d.use = (d.type == "OPTIX")
    sc.render.engine = "CYCLES"
    sc.cycles.device = "GPU"


def setup_render(sc, w, h, samples, path):
    enable_gpu(sc)
    r = sc.render
    r.resolution_x, r.resolution_y, r.resolution_percentage = w, h, 100
    r.film_transparent = True
    r.use_file_extension = False
    r.filepath = path
    r.image_settings.media_type = "MULTI_LAYER_IMAGE"
    r.image_settings.file_format = "OPEN_EXR_MULTILAYER"
    r.image_settings.color_depth = "32"
    r.image_settings.exr_codec = "ZIP"
    sc.view_settings.view_transform = "Standard"
    sc.view_settings.look = "None"
    c = sc.cycles
    c.samples = max(1, int(samples * QUALITY))
    c.use_adaptive_sampling = True
    c.adaptive_threshold = 0.004
    c.use_denoising = False
    c.pixel_filter_type = "BOX"
    c.filter_width = 1.0
    c.sample_clamp_indirect = 0.0
    c.max_bounces = 12
    c.volume_bounces = 12
    c.diffuse_bounces = 2
    c.glossy_bounces = 0
    c.transmission_bounces = 0
    c.volume_biased = True
    c.seed = SEED


def world_color(sc, rgb, strength=1.0):
    w = sc.world
    if w.node_tree is None:
        w.use_nodes = True
    nt = w.node_tree
    nt.nodes.clear()
    bg = nt.nodes.new("ShaderNodeBackground")
    bg.inputs["Color"].default_value = (*rgb, 1.0)
    bg.inputs["Strength"].default_value = strength
    out = nt.nodes.new("ShaderNodeOutputWorld")
    nt.links.new(bg.outputs["Background"], out.inputs["Surface"])
    return bg


def world_sky(sc, strength=1.0, sun_disc=False, aerosol=1.6):
    w = sc.world
    if w.node_tree is None:
        w.use_nodes = True
    nt = w.node_tree
    nt.nodes.clear()
    sky = nt.nodes.new("ShaderNodeTexSky")
    sky.sky_type = "MULTIPLE_SCATTERING"
    sky.sun_disc = sun_disc
    sky.sun_size = math.radians(0.545)
    sky.sun_elevation = math.radians(EL)
    sky.sun_rotation = math.radians(AZ_B)
    sky.altitude = 30.0
    sky.air_density = 1.0
    sky.aerosol_density = aerosol
    sky.ozone_density = 1.0
    bg = nt.nodes.new("ShaderNodeBackground")
    bg.inputs["Strength"].default_value = strength
    out = nt.nodes.new("ShaderNodeOutputWorld")
    nt.links.new(sky.outputs["Color"], bg.inputs["Color"])
    nt.links.new(bg.outputs["Background"], out.inputs["Surface"])
    return sky


def add_sun(sc, direction_b, strength, angle_deg=0.53, group=None):
    ld = bpy.data.lights.new(sc.name + "_Sun", "SUN")
    ld.energy = strength
    ld.angle = math.radians(angle_deg)
    ob = bpy.data.objects.new(sc.name + "_Sun", ld)
    sc.collection.objects.link(ob)
    ob.rotation_euler = Vector(direction_b).normalized().to_track_quat("Z", "Y").to_euler()
    if group:
        ob.lightgroup = group
    return ob


def add_pano_camera(sc, lat_min, lat_max, height=30.0):
    cd = bpy.data.cameras.new(sc.name + "_Cam")
    cd.type = "PANO"
    cd.panorama_type = "EQUIRECTANGULAR"
    cd.latitude_min, cd.latitude_max = math.radians(lat_min), math.radians(lat_max)
    cd.longitude_min, cd.longitude_max = -math.pi, math.pi
    cd.clip_start, cd.clip_end = 0.1, 1.0e6
    ob = bpy.data.objects.new(sc.name + "_Cam", cd)
    sc.collection.objects.link(ob)
    # centre the image on compass azimuth +90 (Unreal az 0), so the left edge is Unreal az -180 (SPEC u = (az+180)/360)
    ob.rotation_euler = (math.radians(90), 0.0, -math.radians(90.0))
    ob.location = (0, 0, height)
    sc.camera = ob
    return ob


def volume_material(name, density, anisotropy=0.65):
    m = bpy.data.materials.new(name)
    if m.node_tree is None:
        m.use_nodes = True
    nt = m.node_tree
    nt.nodes.clear()
    pv = nt.nodes.new("ShaderNodeVolumePrincipled")
    pv.inputs["Color"].default_value = (1, 1, 1, 1)
    pv.inputs["Density"].default_value = density
    pv.inputs["Anisotropy"].default_value = anisotropy
    out = nt.nodes.new("ShaderNodeOutputMaterial")
    nt.links.new(pv.outputs["Volume"], out.inputs["Volume"])
    return m


def add_volume(sc, vdb_path, name, mat, location=(0, 0, 0), scale=(1, 1, 1), rot_z=0.0, volume_data=None):
    if volume_data is None:
        volume_data = bpy.data.volumes.new(name)
        volume_data.filepath = vdb_path
        volume_data.grids.load()
        volume_data.materials.append(mat)
    ob = bpy.data.objects.new(name, volume_data)
    sc.collection.objects.link(ob)
    ob.location = location
    ob.scale = scale
    ob.rotation_euler = (0, 0, rot_z)
    return ob


def render(sc):
    log("render", sc.name, "%dx%d" % (sc.render.resolution_x, sc.render.resolution_y), "samples", sc.cycles.samples, "->", sc.render.filepath)
    t = time.time()
    bpy.ops.render.render(write_still=True, scene=sc.name)
    log("  done in %.1fs" % (time.time() - t))


def lightgroups(sc, sun_ob):
    vl = sc.view_layers[0]
    for n in ("sun", "sky"):
        if n not in [g.name for g in vl.lightgroups]:
            vl.lightgroups.add(name=n)
    sun_ob.lightgroup = "sun"
    sc.world.lightgroup = "sky"
    vl.use_pass_environment = True


# ------------------------------------------------------------------------------------------------ stage: atmosphere
def stage_atmo():
    sc = new_scene("Atmo")
    world_sky(sc, strength=1.0, sun_disc=False)
    add_pano_camera(sc, -90, 90)
    setup_render(sc, 8192, 4096, 4, os.path.join(RAW, "atmo_master_equirect.exr"))
    sc.render.film_transparent = False      # a transparent film drops the world from Combined
    sc.cycles.use_adaptive_sampling = False
    render(sc)


# ------------------------------------------------------------------------------------------------ stage: far cloud bank
BANK_KINDS = [("cumulus", 7000, 3200, 2600), ("flat", 9000, 3800, 1700), ("cumulus", 6000, 3000, 2400),
              ("towering", 5200, 3000, 3400), ("flat", 8000, 3600, 1500)]


def build_bank_scene(sc, mat):
    rng = np.random.default_rng(SEED * 31 + 5)
    vols = []
    for i, (kind, w, d, h) in enumerate(BANK_KINDS):
        path = os.path.join(RAW, "bank_%d.vdb" % i)
        if not os.path.exists(path):
            dens, half, vox = cumulus_density(SEED * 100 + i, w, d, h, 128, kind)
            write_vdb(path, dens, half, vox)
            log("vdb", path, dens.shape)
        vd = bpy.data.volumes.new("BankVol%d" % i)
        vd.filepath = path
        vd.grids.load()
        vd.materials.append(mat)
        vols.append(vd)
    # clumps around the horizon; denser groups at a few azimuths, a gap over the sun so the glitter path reads
    centres = rng.uniform(-180, 180, 7)
    placed = 0
    for i in range(64):
        az_ue = float(centres[i % 7] + rng.normal(0, 16))
        rel = (az_ue - AZ_UE + 180) % 360 - 180
        if abs(rel) < 14:
            continue
        dist = float(rng.uniform(16000, 42000))
        az_b = math.radians(90 + az_ue)
        x, y = math.sin(az_b) * dist, math.cos(az_b) * dist
        z = float(rng.uniform(900, 1500))
        s = float(rng.uniform(0.7, 1.35))
        add_volume(sc, None, "Bank%02d" % i, mat, (x, y, z), (s, s, s * rng.uniform(0.85, 1.1)),
                   float(rng.uniform(0, math.tau)), volume_data=vols[i % len(vols)])
        placed += 1
    log("bank clumps placed", placed)


def stage_bank():
    sc = new_scene("Bank")
    mat = volume_material("BankMat", 0.010)
    world_color(sc, (1, 1, 1), 1.0)
    sun = add_sun(sc, sun_dir_b(), 3.0)
    lightgroups(sc, sun)
    add_pano_camera(sc, -8, 32)
    build_bank_scene(sc, mat)
    setup_render(sc, 4096, 1024, 192, os.path.join(RAW, "bank_strip.exr"))
    render(sc)


# ------------------------------------------------------------------------------------------------ stage: cloud card atlas
CARD_KINDS = [("cumulus", 2000, 1300, 950), ("cumulus", 2000, 1200, 950), ("towering", 1700, 1200, 830),
              ("cumulus", 2000, 1300, 950), ("flat", 2000, 1100, 900), ("flat", 2000, 1000, 900),
              ("puff", 2000, 1300, 950), ("puff", 2000, 1200, 950)]
CARD_SUN_VIEW = Vector((0.70, -0.45, 0.55)).normalized()   # camera looks +Y: sun to the right, above, toward the camera


def build_card(sc, i, mat):
    kind, w, d, h = CARD_KINDS[i]
    path = os.path.join(RAW, "card_%d.vdb" % i)
    if not os.path.exists(path):
        dens, half, vox = cumulus_density(SEED * 1000 + i * 17, w, d, h, 192, kind)
        write_vdb(path, dens, half, vox)
        log("vdb", path, dens.shape)
    ob = add_volume(sc, path, "Card%d" % i, mat)
    # ortho camera framing the 2:1 cell
    cd = bpy.data.cameras.new("CardCam%d" % i)
    cd.type = "ORTHO"
    cd.ortho_scale = w * 1.06
    cd.clip_start, cd.clip_end = 1.0, 20000.0
    cam = bpy.data.objects.new("CardCam%d" % i, cd)
    sc.collection.objects.link(cam)
    cam.rotation_euler = (math.radians(90), 0, 0)
    cam.location = (0, -6000, w * 1.06 / 4.0 + 10.0)  # bottom of the frame just under the base
    return ob, cam


def stage_atlas():
    mat = volume_material("CardMat", 0.014)
    for i in range(len(CARD_KINDS)):
        out = os.path.join(RAW, "card_%d.exr" % i)
        sc = new_scene("Card%d" % i)
        world_color(sc, (1, 1, 1), 1.0)
        sun = add_sun(sc, CARD_SUN_VIEW, 3.0)
        lightgroups(sc, sun)
        ob, cam = build_card(sc, i, mat)
        sc.camera = cam
        setup_render(sc, 1024, 512, 384, out)
        render(sc)


# ------------------------------------------------------------------------------------------------ stage: horizon hills and skyline
def land_mask(az_ue_deg, r_m):
    rel = (az_ue_deg - AZ_UE + 180.0) % 360.0 - 180.0
    a = np.abs(rel)
    # open sea toward the sun (+-80 deg), headlands push in further out
    edge = 80.0 - np.clip((r_m - 6000.0) / 9000.0, 0, 1) * 18.0
    return np.clip((a - edge) / 14.0, 0, 1)


def stage_hills():
    sc = new_scene("Hills")
    world_color(sc, (0, 0, 0), 0.0)
    add_sun(sc, sun_dir_b(), math.pi)
    vl = sc.view_layers[0]
    vl.use_pass_diffuse_direct = True
    vl.use_pass_z = True
    vl.use_pass_position = True
    aov = vl.aovs.add()
    aov.name = "kind"
    aov.type = "VALUE"

    def surface_mat(name, kind_value):
        m = bpy.data.materials.new(name)
        if m.node_tree is None:
            m.use_nodes = True
        nt = m.node_tree
        nt.nodes.clear()
        bs = nt.nodes.new("ShaderNodeBsdfDiffuse")
        bs.inputs["Color"].default_value = (0.5, 0.5, 0.5, 1)
        out = nt.nodes.new("ShaderNodeOutputMaterial")
        nt.links.new(bs.outputs["BSDF"], out.inputs["Surface"])
        ao = nt.nodes.new("ShaderNodeOutputAOV")
        ao.aov_name = "kind"
        ao.inputs["Value"].default_value = kind_value
        return m

    hill_mat = surface_mat("HillMat", 0.0)
    town_mat = surface_mat("TownMat", 1.0)

    # polar height field, Unreal azimuth in degrees, radius in metres
    NA, NR = 2048, 180
    az = np.linspace(-180.0, 180.0, NA, endpoint=False, dtype=np.float32)
    rr = (3200.0 + (np.linspace(0, 1, NR, dtype=np.float32) ** 1.6) * 15800.0).astype(np.float32)
    A, R = np.meshgrid(az, rr, indexing="xy")          # [NR, NA]
    az_b = np.radians(90.0 + A)
    X = np.sin(az_b) * R
    Y = np.cos(az_b) * R
    P = np.stack([X, Y], -1) / 2600.0
    ridges = fbm2(P, SEED * 7 + 1, octaves=7, ridged=True)
    rolling = fbm2(P * 0.6, SEED * 7 + 2, octaves=5)
    hmax = np.interp(R, [3200, 5000, 9000, 19000], [70.0, 420.0, 900.0, 1250.0]).astype(np.float32)
    land = land_mask(A, R)
    H = (0.55 * ridges + 0.45 * rolling) * hmax
    H = H * land - (1.0 - land) * 60.0
    # shore: hills meet the water at z = 0 with a short beach
    Z = np.where(land > 0.02, np.maximum(H, -60.0), -60.0).astype(np.float32)

    verts = np.stack([X, Y, Z], -1).reshape(-1, 3)
    faces = []
    idx = np.arange(NR * NA).reshape(NR, NA)
    a0 = idx[:-1, :]
    a1 = np.roll(idx[:-1, :], -1, axis=1)
    b0 = idx[1:, :]
    b1 = np.roll(idx[1:, :], -1, axis=1)
    keep = (land[:-1, :] + np.roll(land[:-1, :], -1, axis=1) + land[1:, :] + np.roll(land[1:, :], -1, axis=1)) > 0.02
    quads = np.stack([a0, a1, b1, b0], -1)[keep]
    me = bpy.data.meshes.new("Hills")
    me.vertices.add(len(verts))
    me.vertices.foreach_set("co", verts.astype(np.float32).ravel())
    me.loops.add(quads.size)
    me.loops.foreach_set("vertex_index", quads.astype(np.int32).ravel())
    me.polygons.add(len(quads))
    me.polygons.foreach_set("loop_start", (np.arange(len(quads)) * 4).astype(np.int32))
    me.polygons.foreach_set("loop_total", np.full(len(quads), 4, np.int32))
    me.update(calc_edges=True)
    me.validate()
    me.materials.append(hill_mat)
    hob = bpy.data.objects.new("Hills", me)
    sc.collection.objects.link(hob)
    # face winding check: make normals point up
    me.update()
    up = np.zeros(len(me.polygons) * 3, np.float32)
    me.polygons.foreach_get("normal", up)
    if up.reshape(-1, 3)[:, 2].mean() < 0:
        for p in me.polygons:
            p.flip()
        me.update()
    log("hills faces", len(quads))

    # skyline: generic low-rise town with a few 80s hotel towers along the far shore, inland side
    rng = np.random.default_rng(SEED * 3 + 11)
    town_az_centre = AZ_UE + 132.0
    import bmesh
    bm = bmesh.new()
    n_bld = 0

    def height_at(x, y):
        a = (math.degrees(math.atan2(x, y)) - 90.0 + 180.0) % 360.0 - 180.0
        r = math.hypot(x, y)
        ai = int(((a + 180.0) / 360.0) * NA) % NA
        ri = int(np.clip(np.searchsorted(rr, r), 0, NR - 1))
        return float(Z[ri, ai]), float(land[ri, ai])

    for k in range(260 if TOWN else 0):
        a = town_az_centre + rng.normal(0, 9.0)
        r = rng.uniform(3600, 6200)
        b = math.radians(90 + a)
        x, y = math.sin(b) * r, math.cos(b) * r
        g, ld = height_at(x, y)
        if ld < 0.6:
            continue
        tall = rng.random() < 0.08
        hgt = rng.uniform(70, 150) if tall else rng.uniform(12, 45)
        wdt = rng.uniform(25, 60) if tall else rng.uniform(25, 110)
        dep = rng.uniform(20, 50) if tall else rng.uniform(20, 80)
        res = bmesh.ops.create_cube(bm, size=1.0)
        rot = rng.uniform(0, math.pi)
        for v in res["verts"]:
            vx, vy, vz = v.co.x * wdt, v.co.y * dep, (v.co.z + 0.5) * hgt
            cx, cy = vx * math.cos(rot) - vy * math.sin(rot), vx * math.sin(rot) + vy * math.cos(rot)
            v.co = Vector((x + cx, y + cy, max(g, 0.0) - 3.0 + vz))
        n_bld += 1
    tme = bpy.data.meshes.new("Town")
    bm.to_mesh(tme)
    bm.free()
    tme.materials.append(town_mat)
    tob = bpy.data.objects.new("Town", tme)
    sc.collection.objects.link(tob)
    log("town buildings", n_bld)

    add_pano_camera(sc, -10, 6.5)
    setup_render(sc, 4096, 512, 48, os.path.join(RAW, "hills_strip.exr"))
    sc.cycles.diffuse_bounces = 0
    sc.cycles.max_bounces = 1
    render(sc)


# ------------------------------------------------------------------------------------------------ stage: photoreal reference
def stage_ref():
    sc = new_scene("Ref")
    sky = world_sky(sc, strength=1.0, sun_disc=True)
    mat = volume_material("RefBankMat", 0.010)
    # reuse the bank volumes and the hills from their scenes by linking objects
    for name in ("Bank",):
        pass
    build_bank_scene(sc, mat)
    if "Hills" in bpy.data.objects:
        sc.collection.objects.link(bpy.data.objects["Hills"])
        sc.collection.objects.link(bpy.data.objects["Town"])
    add_pano_camera(sc, -8, 32)
    setup_render(sc, 2048, 512, 128, os.path.join(RAW, "reference_strip.exr"))
    sc.render.film_transparent = False
    render(sc)
    # cards lit by the real sky
    mat2 = volume_material("RefCardMat", 0.014)
    for i in range(len(CARD_KINDS)):
        sc = new_scene("RefCard%d" % i)
        world_sky(sc, strength=1.0, sun_disc=False)
        # sky texture sun direction differs from the card bake's view-space sun; rotate the sky so its sun matches CARD_SUN_VIEW
        d = CARD_SUN_VIEW
        az_b = math.degrees(math.atan2(d.x, d.y))
        el = math.degrees(math.asin(d.z))
        skyn = [n for n in sc.world.node_tree.nodes if n.type == "TEX_SKY"][0]
        skyn.sun_rotation = math.radians(az_b)
        skyn.sun_elevation = math.radians(el)
        add_sun(sc, CARD_SUN_VIEW, 4.0)
        ob, cam = build_card(sc, i, mat2)
        sc.camera = cam
        setup_render(sc, 512, 256, 96, os.path.join(RAW, "ref_card_%d.exr" % i))
        sc.render.film_transparent = False
        render(sc)


# ------------------------------------------------------------------------------------------------ stage: compose textures
def edge_field(alpha, sigma, wrap_u=False):
    """Soft distance-like field: 0.5 on the alpha = 0.5 contour, toward 0 outside and 1 inside over about 2 sigma."""
    hard = (alpha > 0.5).astype(np.float32)
    return np.clip(gauss_blur(hard, sigma, wrap_u=wrap_u), 0, 1)


def cloud_channels(alpha, sun, amb, sigma, wrap_u=False, smooth=1.0):
    a = np.clip(alpha, 0, 1)
    w = np.clip((a - 0.02) / 0.3, 0, 1)
    sun_u = sun / np.maximum(a, 0.02)
    amb_u = amb / np.maximum(a, 0.02)
    inside = a > 0.5
    s_ref = np.percentile(sun_u[inside], 99.5) if inside.any() else 1.0
    a_ref = np.percentile(amb_u[inside], 99.5) if inside.any() else 1.0
    g = np.clip(sun_u / s_ref * 0.95, 0, 1)
    b = np.clip(amb_u / a_ref * 0.95, 0, 1)
    gb = pull_push_fill(np.stack([g, b], -1), w)
    if smooth > 0:
        gb = gauss_blur(gb, smooth, wrap_u=wrap_u)
    edge = edge_field(a, sigma, wrap_u=wrap_u)
    return np.stack([a, gb[..., 0], gb[..., 1], edge], -1), float(s_ref), float(a_ref)


def stage_compose():
    meta = {"preset": PRESET, "sun_az_ue": AZ_UE, "sun_el": EL,
            "sun_dir_ue": [math.cos(math.radians(EL)) * math.cos(math.radians(AZ_UE)),
                           math.cos(math.radians(EL)) * math.sin(math.radians(AZ_UE)), math.sin(math.radians(EL))]}

    # --- atmosphere strip (HDR) and fitted gradient colours
    d = read_exr_all(os.path.join(RAW, "atmo_master_equirect.exr"))
    rgb = np.stack([ch(d, "Combined", c) for c in "RGB"], -1)
    H, W = rgb.shape[:2]
    el_rows = 90.0 - (np.arange(H) + 0.5) / H * 180.0
    strip_h, strip_w = 1024, 4096
    dst_el = 32.0 - (np.arange(strip_h) + 0.5) / strip_h * 40.0
    strip = resample_rows(rgb, el_rows, dst_el)
    strip = downsample_cols(strip, W // strip_w)
    # below the horizon the model is black: hold the 0.5 deg row instead
    row_hold = int(np.argmin(np.abs(dst_el - 0.5)))
    strip[dst_el < 0.5] = strip[row_hold]
    horizon_row = int(np.argmin(np.abs(dst_el - 2.0)))
    expo = 0.85 / float(lum(strip[horizon_row]).mean())
    meta["panorama_exposure_applied"] = expo
    write_exr(os.path.join(OUT, "T_BSky_%s_HorizonAtmo.exr" % PRESET), strip * expo, half=True)
    # sun column check
    sun_row = int(np.argmin(np.abs(dst_el - min(EL, 31.0))))
    sun_u = (int(np.argmax(lum(strip[sun_row]))) + 0.5) / strip_w
    meta["check_sun_u_measured"] = sun_u
    meta["check_sun_u_expected"] = (AZ_UE + 180.0) / 360.0
    log("sun u measured %.4f expected %.4f" % (sun_u, (AZ_UE + 180.0) / 360.0))

    def band(el0, el1):
        m = (el_rows >= el0) & (el_rows <= el1)
        return (rgb[m].reshape(-1, 3).mean(0) * expo).tolist()

    def band_dir(el0, el1, az_centre, half_width):
        m = (el_rows >= el0) & (el_rows <= el1)
        az = -180.0 + (np.arange(W) + 0.5) / W * 360.0
        rel = (az - az_centre + 180) % 360 - 180
        cm = np.abs(rel) <= half_width
        return (rgb[m][:, cm].reshape(-1, 3).mean(0) * expo).tolist()

    meta["fit"] = {
        "zenith": band(80, 90), "mid": band(40, 50), "seam32": band(30, 34), "horizon": band(1, 4), "haze": band(0, 1),
        "horizon_sunside": band_dir(1, 6, AZ_UE, 30), "horizon_antisun": band_dir(1, 6, AZ_UE + 180, 30)}
    log("fit", json.dumps(meta["fit"]))
    # master kept as source art (full float, 8192 x 4096)
    write_exr(os.path.join(OUT, "Master_%s_Atmosphere_Equirect.exr" % PRESET), rgb * expo, half=True)

    # --- far cloud bank
    d = read_exr_all(os.path.join(RAW, "bank_strip.exr"))
    alpha = ch(d, "Combined", "A")
    sun = lum(np.stack([ch(d, "Combined_sun", c) for c in "RGB"], -1))
    env = lum(np.stack([ch(d, "Environment", c) for c in "RGB"], -1))
    amb = lum(np.stack([ch(d, "Combined_sky", c) for c in "RGB"], -1)) - env
    bank, s_ref, a_ref = cloud_channels(alpha, sun, np.maximum(amb, 0), sigma=2.0, wrap_u=True, smooth=0.8)
    meta["bank_norm"] = [s_ref, a_ref]
    write_png(os.path.join(OUT, "T_BSky_%s_HorizonClouds.png" % PRESET), bank)
    log("bank coverage mean %.3f" % float(alpha.mean()))

    # --- cloud atlas
    atlas = np.zeros((2048, 2048, 4), np.float32)
    for i in range(len(CARD_KINDS)):
        p = os.path.join(RAW, "card_%d.exr" % i)
        d = read_exr_all(p)
        alpha = ch(d, "Combined", "A")
        sun = lum(np.stack([ch(d, "Combined_sun", c) for c in "RGB"], -1))
        env = lum(np.stack([ch(d, "Environment", c) for c in "RGB"], -1))
        amb = lum(np.stack([ch(d, "Combined_sky", c) for c in "RGB"], -1)) - env
        cell, s_ref, a_ref = cloud_channels(alpha, sun, np.maximum(amb, 0), sigma=4.0, smooth=3.0)
        # keep 24 px margins clear
        m = 24
        cell[:m] = cell[-m:] = 0
        cell[:, :m] = cell[:, -m:] = 0
        cell[..., 1:3] = pull_push_fill(cell[..., 1:3], np.clip(cell[..., 0] * 4, 0, 1))
        row, col = i // 2, i % 2
        atlas[row * 512:(row + 1) * 512, col * 1024:(col + 1) * 1024] = cell
        log("card %d coverage %.3f norm %.3f %.3f" % (i, float(alpha.mean()), s_ref, a_ref))
    write_png(os.path.join(OUT, "T_BSky_%s_CloudAtlas.png" % PRESET), atlas)

    # --- horizon hills
    d = read_exr_all(os.path.join(RAW, "hills_strip.exr"))
    alpha = ch(d, "Combined", "A")
    pos_z = ch(d, "Position", "Z")
    under = (pos_z < 0.5) & (alpha > 0)
    alpha = np.where(under, 0.0, alpha).astype(np.float32)
    diff = lum(np.stack([ch(d, "Diffuse Direct", c) for c in "RGB"], -1))
    depth = ch(d, "Depth", "Z")
    try:
        kind = ch(d, "kind", "X")
    except KeyError:
        kind = ch(d, "kind", "V") if any(k.endswith("kind.V") for k in d) else np.zeros_like(alpha)
    a = np.clip(alpha, 0, 1)
    w = np.clip(a * 3, 0, 1)
    shade = np.clip(diff / np.maximum(a, 0.02), 0, 1)
    dist = np.clip((depth - 2500.0) / 16500.0, 0, 1)
    kind_u = np.clip(kind / np.maximum(a, 0.02), 0, 1)
    fill = pull_push_fill(np.stack([shade, dist, kind_u], -1), w)
    hz = np.stack([fill[..., 0], fill[..., 1], fill[..., 2], edge_field(a, 1.6, wrap_u=True)], -1)
    write_png(os.path.join(OUT, "T_BSky_%s_Horizon.png" % PRESET), hz)
    log("hills coverage %.3f" % float(a.mean()))

    # --- sky light HDR (long-lat 2:1): atmosphere + far clouds, ground below the horizon
    sky = rgb * expo
    sky = sky.reshape(512, H // 512, 1024, W // 1024, 3).mean(axis=(1, 3))
    el_s = 90.0 - (np.arange(512) + 0.5) / 512 * 180.0
    fit = meta["fit"]
    sun_col = np.array([1.0, 0.93, 0.82]) * 2.2
    amb_col = np.array(fit["mid"])
    bank_eq = np.zeros((512, 1024, 4), np.float32)
    rows = np.where((el_s <= 32) & (el_s >= -8))[0]
    src_rows_el = 32.0 - (np.arange(1024) + 0.5) / 1024 * 40.0
    bank_small = downsample_cols(bank, 4)
    bank_eq[rows] = resample_rows(bank_small, src_rows_el, el_s[rows])
    cloud_rgb = bank_eq[..., 1:2] * sun_col + (0.35 + 0.65 * bank_eq[..., 2:3]) * amb_col
    ca = np.clip(bank_eq[..., 0:1], 0, 1)
    sky = sky * (1 - ca) + cloud_rgb * ca
    ground = np.array(fit["horizon"]) * np.array([0.35, 0.42, 0.5])
    below = el_s < 0
    sky[below] = ground
    oiio_write(os.path.join(OUT, "T_BSky_%s_SkyLight.hdr" % PRESET), sky.astype(np.float32), "float")

    # --- reference images (tone-mapped previews, not shipped)
    def tonemap(x, ex=1.0):
        x = np.maximum(x * ex, 0)
        y = x / (1.0 + x)
        return np.clip(y ** (1 / 2.2), 0, 1)

    if os.path.exists(os.path.join(RAW, "reference_strip.exr")):
        d = read_exr_all(os.path.join(RAW, "reference_strip.exr"))
        ref = np.stack([ch(d, "Combined", c) for c in "RGB"], -1)
        ex = 0.6 / max(1e-6, float(lum(ref[int(ref.shape[0] * 0.75)]).mean()))
        write_png(os.path.join(REF, "Ref_%s_PhotorealStrip.png" % PRESET), np.concatenate([tonemap(ref, ex), np.ones_like(ref[..., :1])], -1))
    write_png(os.path.join(REF, "Ref_%s_AtmosphereStrip.png" % PRESET),
              np.concatenate([tonemap(strip * expo, 1.2), np.ones_like(strip[..., :1])], -1))
    tiles = []
    for i in range(len(CARD_KINDS)):
        p = os.path.join(RAW, "ref_card_%d.exr" % i)
        if os.path.exists(p):
            d = read_exr_all(p)
            t = np.stack([ch(d, "Combined", c) for c in "RGB"], -1)
            ex = 0.5 / max(1e-6, float(np.percentile(lum(t), 50)))
            tiles.append(tonemap(t, ex))
    if len(tiles) == 8:
        grid = np.concatenate([np.concatenate(tiles[r * 2:r * 2 + 2], axis=1) for r in range(4)], axis=0)
        write_png(os.path.join(REF, "Ref_%s_CloudCards.png" % PRESET), np.concatenate([grid, np.ones_like(grid[..., :1])], -1))
    # previews of the data textures for humans
    write_png(os.path.join(REF, "Data_%s_CloudAtlas_G_sunlit.png" % PRESET), np.repeat(atlas[..., 1:2], 3, -1).tolist() and
              np.concatenate([np.repeat(atlas[..., 1:2], 3, -1), atlas[..., 0:1]], -1))

    with open(os.path.join(OUT, "bake_meta.json"), "w") as f:
        json.dump(meta, f, indent=2)
    log("compose done", OUT)


# ------------------------------------------------------------------------------------------------ stage: golden-hour cumulus cards
# Photoreal cumulus / congestus cards for the low golden-hour sun. Stage "cumulus" builds the VDBs, renders the 8 cards and
# composes; stage "cumuluscompose" only recomposes from the raw renders. Output bakes/<P>/T_BSky_<P>_Cumulus.png (2048 x 2048,
# 2 x 4 cells of 1024 x 512, cell i at column i % 2, row i // 2, row 0 at the top), straight alpha:
#   R = sun-lit radiance per unit opacity (light group "sun", luminance), gamma 2 encoded: linear = R * R
#   G = sky-lit radiance per unit opacity (light group "sky" minus the directly seen sky, luminance), gamma 2: linear = G * G
#   B = optical thickness proxy 1 - exp(-tau / CU_TAU_REF), tau = view-axis optical depth of the VDB (linear, 0..1)
#   A = opacity (1 - view transmittance) from the transparent film, linear
# Every card is lit with the sun on the right (+X in the cell) and slightly behind the cloud; the runtime mirrors a card so its lit side faces the sun.
CU_W, CU_D, CU_H = 2000.0, 1200.0, 1040.0   # VDB domain (m): width x, depth y (the camera looks along +Y), height z (from 0)
CU_RES = 288                                 # voxels across the width
CU_BASE_Z = 110.0                            # flat cloud base height (m)
CU_ORTHO = 2200.0                            # ortho camera width (m); the 2:1 frame spans z CU_FRAME_Z0 .. CU_FRAME_Z0 + 1100
CU_FRAME_Z0 = -60.0
CU_DENSITY = 0.07                          # Principled Volume density (extinction 1/m at grid density 1)
CU_ANISO = 0.6
CU_SUN_EL = 8.0                              # card sun elevation above the cloud's horizon (deg)
CU_SUN_BEHIND = 25.0                         # card sun azimuth from +X (right) toward +Y (behind the cloud) (deg)
CU_SUN_ENERGY = 30.0                         # about 8x the old 4: the sun term now matches the sky term, so neither is noisy
CU_BASE_SHADE = arg("cushade", 1)            # 1 = absorbing haze toward the sun shades the lower cloud from the low sun
CU_SAMPLES = 512
CU_TAU_REF = 40.0
CU_KINDS = ["clusters3", "wide", "towering", "medium", "clusters2", "medium", "congestus", "wide"]
LOOK = arg("look", "")


def cu_sun_dir():
    e, b = math.radians(CU_SUN_EL), math.radians(CU_SUN_BEHIND)
    return Vector((math.cos(e) * math.cos(b), math.cos(e) * math.sin(b), math.sin(e)))


def cu_cells(kind, rng):
    """Convective cells (x, y, radius, top above the base) in metres, before fitting to the domain."""
    W = CU_W
    cells = []
    skirt = True
    if kind in ("clusters3", "clusters2"):
        xs = (-0.31, 0.0, 0.30) if kind == "clusters3" else (-0.2, 0.22)
        for cx in xs:
            for k in range(int(rng.integers(2, 4))):
                cells.append((cx * W + rng.normal(0, 0.035 * W), rng.normal(0, 0.04 * W),
                              0.065 * W * rng.uniform(0.8, 1.2), 0.15 * W * rng.uniform(0.7, 1.35)))
        skirt = False
    elif kind == "wide":
        n = int(rng.integers(6, 9))
        for k in range(n):
            t = (k + rng.uniform(-0.3, 0.3)) / (n - 1) * 2 - 1
            centre = 1.0 - abs(t)
            cells.append((t * 0.38 * W, rng.normal(0, 0.05 * W), 0.08 * W * rng.uniform(0.85, 1.2),
                          (0.07 + 0.09 * centre) * W * rng.uniform(0.75, 1.3)))
    elif kind == "towering":
        cells.append((0.02 * W, 0.0, 0.13 * W, 0.46 * W))
        cells.append((-0.13 * W, rng.normal(0, 0.04 * W), 0.09 * W, 0.24 * W * rng.uniform(0.8, 1.1)))
        cells.append((0.15 * W, rng.normal(0, 0.04 * W), 0.08 * W, 0.17 * W * rng.uniform(0.8, 1.1)))
        cells.append((-0.26 * W, rng.normal(0, 0.04 * W), 0.06 * W, 0.09 * W))
    elif kind == "congestus":
        cells.append((-0.08 * W, 0.0, 0.11 * W, 0.36 * W))
        cells.append((0.1 * W, 0.02 * W, 0.1 * W, 0.30 * W))
        cells.append((0.27 * W, rng.normal(0, 0.04 * W), 0.07 * W, 0.15 * W))
        cells.append((-0.27 * W, rng.normal(0, 0.04 * W), 0.07 * W, 0.12 * W))
    else:  # medium
        n = int(rng.integers(3, 5))
        for k in range(n):
            t = (k + rng.uniform(-0.25, 0.25)) / (n - 1) * 2 - 1
            cells.append((t * 0.22 * W, rng.normal(0, 0.04 * W), 0.09 * W * rng.uniform(0.85, 1.15),
                          (0.15 + 0.1 * (1 - abs(t))) * W * rng.uniform(0.8, 1.2)))
    return cells, skirt


def cu_spheres(kind, rng):
    """Hierarchical blobs: turret stacks per cell, cauliflower children on their upper surfaces, then fit to the domain.
    Returns rows (cx, cy, cz, rx, ry, rz, blend)."""
    cells, skirt = cu_cells(kind, rng)
    base = CU_BASE_Z
    sph = []
    shear = rng.uniform(-0.12, 0.12)
    for (x, y, r, top) in cells:
        z = base + r * 0.45
        rr = r
        level0 = []
        while True:
            level0.append((x + shear * (z - base), y, z, rr * 1.12, rr * 1.08, rr * 0.92, 0.12 * rr))
            if z + rr * 0.92 >= base + top:
                break
            z += rr * 0.72
            rr *= 0.86
        sph.extend(level0)
        # cauliflower: children on the upper and side surfaces, three generations
        gen = [(s[0], s[1], s[2], min(s[3], s[4], s[5])) for s in level0]
        for level, (nmin, nmax, ratio) in enumerate([(6, 9, (0.45, 0.62)), (4, 7, (0.4, 0.56)), (3, 5, (0.38, 0.52))]):
            nxt = []
            for (cx, cy, cz, cr) in gen:
                for _ in range(int(rng.integers(nmin, nmax + 1))):
                    cos_t = rng.uniform(-0.15 if level == 0 else 0.1, 1.0)
                    sin_t = math.sqrt(1 - cos_t * cos_t)
                    ph = rng.uniform(0, math.tau)
                    d = (sin_t * math.cos(ph), sin_t * math.sin(ph), cos_t)
                    rc = cr * rng.uniform(*ratio)
                    off = cr * rng.uniform(0.8, 0.98)
                    c = (cx + d[0] * off, cy + d[1] * off, cz + d[2] * off)
                    sph.append((c[0], c[1], c[2], rc, rc, rc * rng.uniform(0.85, 1.0), 0.15 * rc))
                    nxt.append((c[0], c[1], c[2], rc))
            # only the upward-facing half of the next generation keeps branching
            gen = [g for g in nxt if rng.random() < 0.7]
    if skirt:
        xs = [c[0] for c in cells]
        rs = [c[2] for c in cells]
        x = min(xs) - 0.4 * max(rs)
        while x <= max(xs) + 0.4 * max(rs):
            r = 0.09 * CU_W * rng.uniform(0.8, 1.2)
            sph.append((x, rng.normal(0, 0.03 * CU_W), base + 0.3 * r, r * 1.5, r * 1.2, r * 0.55, 0.3 * r))
            x += 0.1 * CU_W
    a = np.array(sph, np.float64)
    # fit: uniform scale about (x mid, 0, base) so the blobs (plus displacement) stay inside the domain with a margin
    lo_x, hi_x = (a[:, 0] - a[:, 3]).min(), (a[:, 0] + a[:, 3]).max()
    hi_y = np.abs(a[:, 1]) + a[:, 4]
    hi_z = (a[:, 2] + a[:, 5]).max()
    xmid = 0.5 * (lo_x + hi_x)
    f = min((0.45 * CU_W) / (0.5 * (hi_x - lo_x)), (0.9 * CU_H - base) / (hi_z - base), (0.44 * CU_D) / hi_y.max())
    a[:, 0] = (a[:, 0] - xmid) * f
    a[:, 1] *= f
    a[:, 2] = base + (a[:, 2] - base) * f
    a[:, 3:7] *= f
    return a, f


def smin(a, b, k):
    h = np.clip(0.5 + 0.5 * (b - a) / k, 0.0, 1.0)
    return b * (1 - h) + a * h - k * h * (1 - h)


def cu_density(kind, seed):
    """Cauliflower cumulus density in the CU domain. Returns (dens [nx,ny,nz], vox, xs, zs)."""
    t0 = time.time()
    rng = np.random.default_rng(seed)
    sph, f = cu_spheres(kind, rng)
    vox = CU_W / CU_RES
    nx, ny, nz = CU_RES, int(round(CU_D / vox)), int(round(CU_H / vox))
    xs = ((np.arange(nx) + 0.5) * vox - CU_W / 2).astype(np.float32)
    ys = ((np.arange(ny) + 0.5) * vox - CU_D / 2).astype(np.float32)
    zs = ((np.arange(nz) + 0.5) * vox).astype(np.float32)
    sdf = np.full((nx, ny, nz), 1.0e4, np.float32)
    for (cx, cy, cz, rx, ry, rz, kb) in sph:
        pad = max(rx, ry, rz) + kb + 2 * vox
        i0, i1 = np.searchsorted(xs, cx - pad), np.searchsorted(xs, cx + pad)
        j0, j1 = np.searchsorted(ys, cy - pad), np.searchsorted(ys, cy + pad)
        k0, k1 = np.searchsorted(zs, cz - pad), np.searchsorted(zs, cz + pad)
        if i1 <= i0 or j1 <= j0 or k1 <= k0:
            continue
        X = xs[i0:i1, None, None]
        Y = ys[None, j0:j1, None]
        Z = zs[None, None, k0:k1]
        q = np.sqrt(((X - cx) / rx) ** 2 + ((Y - cy) / ry) ** 2 + ((Z - cz) / rz) ** 2)
        d = ((q - 1.0) * min(rx, ry, rz)).astype(np.float32)
        sdf[i0:i1, j0:j1, k0:k1] = smin(sdf[i0:i1, j0:j1, k0:k1], d, kb)
    X, Y, Z = np.meshgrid(xs, ys, zs, indexing="ij")
    P = np.stack([X, Y, Z], axis=-1)
    del X, Y
    # large irregularity, then billowy bumps (rounded peaks, sharp creases) for the cauliflower texture
    sdf += (fbm3(P / 170.0, seed * 7 + 1, octaves=3) - 0.5) * 60.0
    n = fbm3(P / 62.0, seed * 7 + 2, octaves=2)
    sdf -= np.abs(n * 2.0 - 1.0) * 32.0 - 8.0
    n = fbm3(P / 22.0, seed * 7 + 5, octaves=2)
    sdf -= np.abs(n * 2.0 - 1.0) * 11.0 - 3.0
    # flat, slightly ragged base
    XY = np.stack([P[:, :, 0, 0], P[:, :, 0, 1]], -1)
    zb = CU_BASE_Z + (fbm2(XY / 260.0, seed * 7 + 3, octaves=4) - 0.5) * 40.0
    sdf = -smin(-sdf, -(zb[:, :, None] - Z), 18.0)
    # density: crisp shell, softer and wispier toward the base
    inside = -sdf
    soft = 12.0 + 28.0 * np.clip(1.0 - (Z - CU_BASE_Z) / 110.0, 0, 1)
    x = np.clip((inside + 3.0) / soft, 0, 1)
    dens = x * x * (3 - 2 * x)
    wisp = fbm3(P / 75.0, seed * 7 + 4, octaves=3)
    dens *= (0.85 + 0.3 * wisp).astype(np.float32)
    # thin fuzz just outside the surface
    fuzz = np.clip((wisp - 0.55) * 3.0, 0, 1) * np.exp(-np.maximum(sdf, 0) / 12.0) * 0.08
    dens = np.maximum(dens, fuzz * (sdf > 0))
    edge = np.minimum.reduce([
        np.clip(P[..., 0] + CU_W / 2, 0, 3 * vox) / (3 * vox), np.clip(CU_W / 2 - P[..., 0], 0, 3 * vox) / (3 * vox),
        np.clip(P[..., 1] + CU_D / 2, 0, 3 * vox) / (3 * vox), np.clip(CU_D / 2 - P[..., 1], 0, 3 * vox) / (3 * vox),
        np.clip(CU_H - Z, 0, 3 * vox) / (3 * vox)])
    dens = np.clip(dens * edge, 0, 1).astype(np.float32)
    log("cumulus %s: %d blobs, fit %.2f, grid %s, %.1fs" % (kind, len(sph), f, dens.shape, time.time() - t0))
    return dens, vox, xs, zs


def cu_shade_slab(sc, x0, z_top, sigma):
    """Absorbing haze toward the sun (right of the cloud, below z_top), invisible to the camera: the low sun reaches the
    lower cloud through a longer haze path, so tops stay sunlit and bases fall into the sky-lit mauve."""
    import bmesh
    me = bpy.data.meshes.new("CuShade")
    bm = bmesh.new()
    bmesh.ops.create_cube(bm, size=1.0)
    for v in bm.verts:
        v.co = Vector((x0 + (v.co.x + 0.5) * 9000.0, -3000.0 + (v.co.y + 0.5) * 12000.0, z_top - 5000.0 + (v.co.z + 0.5) * 5000.0))
    bm.to_mesh(me)
    bm.free()
    m = bpy.data.materials.new("CuShadeMat")
    if m.node_tree is None:
        m.use_nodes = True
    nt = m.node_tree
    nt.nodes.clear()
    ab = nt.nodes.new("ShaderNodeVolumeAbsorption")
    ab.inputs["Color"].default_value = (0, 0, 0, 1)
    ab.inputs["Density"].default_value = sigma
    out = nt.nodes.new("ShaderNodeOutputMaterial")
    nt.links.new(ab.outputs["Volume"], out.inputs["Volume"])
    me.materials.append(m)
    ob = bpy.data.objects.new("CuShade", me)
    sc.collection.objects.link(ob)
    ob.visible_camera = False
    return ob


def stage_cumulus():
    sun_d = cu_sun_dir()
    mat = volume_material("CuMat", CU_DENSITY, CU_ANISO)
    try:
        mat.cycles.volume_interpolation = "CUBIC"
    except Exception as e:  # noqa: BLE001
        log("cubic interpolation not set:", e)
    times = []
    for i, kind in enumerate(CU_KINDS):
        t0 = time.time()
        dens, vox, xs, zs = cu_density(kind, SEED * 500 + i * 37)
        vdb_path = os.path.join(RAW, "cumulus_%d.vdb" % i)
        write_vdb(vdb_path, dens, (CU_W / 2, CU_D / 2, CU_H), vox)
        tau = dens.sum(axis=1) * vox * CU_DENSITY
        np.savez(os.path.join(RAW, "cumulus_%d_tau.npz" % i), tau=tau, xs=xs, zs=zs)
        col = dens.max(axis=(1, 2))
        x_right = float(xs[np.nonzero(col > 0.01)[0].max()])
        rowz = dens.max(axis=(0, 1))
        z_top = float(zs[np.nonzero(rowz > 0.01)[0].max()])
        del dens
        sc = new_scene("Cumulus%d" % i)
        world_sky(sc, strength=1.0, sun_disc=False)
        skyn = [n for n in sc.world.node_tree.nodes if n.type == "TEX_SKY"][0]
        skyn.sun_rotation = math.atan2(sun_d.x, sun_d.y)
        skyn.sun_elevation = math.radians(CU_SUN_EL)
        sun = add_sun(sc, sun_d, CU_SUN_ENERGY)
        lightgroups(sc, sun)
        add_volume(sc, vdb_path, "Cumulus%d" % i, mat)
        if CU_BASE_SHADE:
            # sun unattenuated above 55 % of the cloud height, about 12 % at the base
            zt = CU_BASE_Z + 0.55 * (z_top - CU_BASE_Z)
            sigma = math.log(1 / 0.12) * math.sin(math.radians(CU_SUN_EL)) / max(50.0, zt - CU_BASE_Z)
            cu_shade_slab(sc, x_right + 40.0, zt, sigma)
        cd = bpy.data.cameras.new("CuCam%d" % i)
        cd.type = "ORTHO"
        cd.ortho_scale = CU_ORTHO
        cd.clip_start, cd.clip_end = 1.0, 20000.0
        cam = bpy.data.objects.new("CuCam%d" % i, cd)
        sc.collection.objects.link(cam)
        cam.rotation_euler = (math.radians(90), 0, 0)
        cam.location = (0, -6000, CU_FRAME_Z0 + CU_ORTHO / 4.0)
        sc.camera = cam
        setup_render(sc, 1024, 512, CU_SAMPLES, os.path.join(RAW, "cumulus_%d.exr" % i))
        sc.cycles.adaptive_threshold = 0.003
        sc.cycles.volume_bounces = 16
        sc.cycles.max_bounces = 16
        render(sc)
        times.append(time.time() - t0)
        log("cumulus card %d (%s) total %.1fs" % (i, kind, times[-1]))
    log("cumulus cards total %.1fs" % sum(times))
    stage_cumuluscompose()


def srgb_encode(x):
    x = np.clip(x, 0, 1)
    return np.where(x <= 0.0031308, x * 12.92, 1.055 * np.power(x, 1 / 2.4) - 0.055)


def srgb_decode_hex(h):
    c = np.array([int(h[k:k + 2], 16) / 255.0 for k in (0, 2, 4)])
    return np.where(c <= 0.04045, c / 12.92, ((c + 0.055) / 1.055) ** 2.4).astype(np.float32)


def stage_cumuluscompose():
    cw, chh = 1024, 512
    cells = []
    for i in range(len(CU_KINDS)):
        d = read_exr_all(os.path.join(RAW, "cumulus_%d.exr" % i))
        alpha = np.clip(ch(d, "Combined", "A"), 0, 1)
        sun = lum(np.stack([ch(d, "Combined_sun", c) for c in "RGB"], -1))
        env = lum(np.stack([ch(d, "Environment", c) for c in "RGB"], -1))
        sky = np.maximum(lum(np.stack([ch(d, "Combined_sky", c) for c in "RGB"], -1)) - env, 0)
        # straight (per unit opacity) radiance; near-transparent texels take their neighbours' values
        w = np.clip((alpha - 0.004) / 0.08, 0, 1)
        su = pull_push_fill(np.stack([sun, sky], -1) / np.maximum(alpha, 1e-4)[..., None], w)
        # optical thickness from the VDB, resampled to the ortho frame
        tz = np.load(os.path.join(RAW, "cumulus_%d_tau.npz" % i))
        px = ((np.arange(cw) + 0.5) / cw - 0.5) * CU_ORTHO
        pz = CU_FRAME_Z0 + CU_ORTHO / 4.0 + (0.5 - (np.arange(chh) + 0.5) / chh) * (CU_ORTHO / 2.0)
        tau = tz["tau"]
        t1 = np.stack([np.interp(px, tz["xs"], tau[:, k], left=0, right=0) for k in range(tau.shape[1])], 0)  # [nz, cw]
        t2 = np.stack([np.interp(pz, tz["zs"], t1[:, j], left=0, right=0) for j in range(cw)], 1)            # [chh, cw]
        border = max(float(alpha[:24].max()), float(alpha[-24:].max()), float(alpha[:, :24].max()), float(alpha[:, -24:].max()))
        log("cumulus cell %d alpha mean %.3f max in 24px border %.4f sun/sky mean %.3g %.3g" % (
            i, float(alpha.mean()), border, float(sun.mean()), float(sky.mean())))
        cells.append((alpha, su[..., 0], su[..., 1], t2.astype(np.float32)))
    ok = np.concatenate([c[0].ravel() > 0.15 for c in cells])
    s_all = np.concatenate([c[1].ravel() for c in cells])[ok]
    g_all = np.concatenate([c[2].ravel() for c in cells])[ok]
    t_all = np.concatenate([c[3].ravel() for c in cells])[np.concatenate([c[0].ravel() > 0.5 for c in cells])]
    s_ref = float(np.percentile(s_all, 99.9))
    g_ref = float(np.percentile(g_all, 99.9))
    tau_ref = float(np.percentile(t_all, 98) / 3.0)
    atlas = np.zeros((2048, 2048, 4), np.float32)
    look = np.zeros((2048, 2048, 3), np.float32)
    sky_c, lit_c, amb_c, mid_c = (srgb_decode_hex(h) for h in ("7895C8", "FFB88C", "8784BA", "F09DB0"))
    for i, (alpha, s, g, t) in enumerate(cells):
        r_lin = np.clip(s / s_ref * 0.95, 0, 1)
        g_lin = np.clip(g / g_ref * 0.95, 0, 1)
        b = np.clip(1.0 - np.exp(-gauss_blur(t, 1.5) / tau_ref), 0, 1)
        a = alpha.copy()
        # 8 px fade at the cell border (the cloud should already be clear of it: see the log line "max in 24px border")
        ramp = np.minimum(np.clip(np.arange(cw) / 8.0, 0, 1), np.clip((cw - 1 - np.arange(cw)) / 8.0, 0, 1))
        rampv = np.minimum(np.clip(np.arange(chh) / 8.0, 0, 1), np.clip((chh - 1 - np.arange(chh)) / 8.0, 0, 1))
        a *= ramp[None, :] * rampv[:, None]
        cell = np.stack([np.sqrt(r_lin), np.sqrt(g_lin), b, a], -1)
        row, col = i // 2, i % 2
        atlas[row * chh:(row + 1) * chh, col * cw:(col + 1) * cw] = cell
        mid = 4.0 * r_lin * (1.0 - r_lin) * 0.25
        cloud = r_lin[..., None] * lit_c * 1.4 + g_lin[..., None] * amb_c + mid[..., None] * mid_c
        look[row * chh:(row + 1) * chh, col * cw:(col + 1) * cw] = sky_c * (1 - a[..., None]) + cloud * a[..., None]
    write_png(os.path.join(OUT, "T_BSky_%s_Cumulus.png" % PRESET), atlas)
    look_rgba = np.concatenate([srgb_encode(look), np.ones_like(look[..., :1])], -1)
    write_png(os.path.join(REF, "Ref_%s_CumulusLook.png" % PRESET), look_rgba)
    if LOOK:
        write_png(LOOK, look_rgba)
    meta = {"texture": "T_BSky_%s_Cumulus" % PRESET, "cells": "2 x 4, 1024 x 512, cell i at column i % 2, row i // 2 (row 0 top)",
            "R": "sun-lit luminance per unit opacity, linear = R^2; linear 0.95 = 99.9th percentile over alpha > 0.15",
            "G": "sky-lit luminance per unit opacity, linear = G^2; linear 0.95 = 99.9th percentile over alpha > 0.15",
            "B": "1 - exp(-tau / tau_ref), tau = view-axis optical depth, linear",
            "A": "opacity, linear, straight alpha", "lit_side": "+u (right) in every cell; sun slightly behind the cloud",
            "sun_norm_ref": s_ref, "sky_norm_ref": g_ref, "tau_ref": tau_ref, "card_sun_el": CU_SUN_EL,
            "card_sun_behind": CU_SUN_BEHIND, "kinds": CU_KINDS, "density": CU_DENSITY, "anisotropy": CU_ANISO,
            "base_shade": CU_BASE_SHADE, "metres_per_cell_width": CU_ORTHO}
    with open(os.path.join(OUT, "cumulus_meta.json"), "w") as fh:
        json.dump(meta, fh, indent=2)
    log("cumulus compose done: s_ref %.4g g_ref %.4g tau_ref %.3g" % (s_ref, g_ref, tau_ref))


# ------------------------------------------------------------------------------------------------ stage: cumulus v2, three sun passes
# Stage "cumulus3" builds the cumulus shapes (cu3_density, round 2: many thermals per card for the 4.8-11.5 km the runtime
# draws them at, near-untapered stacks under domed, sheared heads, flank domes, 3-4 dome generations on the crowns, a ragged
# base with hanging lobes, detached fractus, crisp crowns and soft flanks; see-through holes plugged), renders every card
# three times with a white sun lamp (the same strength in every pass) and once with the multiple-scattering sky and no sun
# disc, each render holding that one light (512 volume bounces: converged multiple scattering; OpenImageDenoise on the
# Combined pass), then composes. Stage "cumulus3compose" rebuilds the textures from the raw renders only.
# Card frame: ortho camera looking along +Y pitched up CU3_PITCH (the card seen from below its base, as the game sees it),
# +u = +X (right), cell v = 0 at the top; the base plane crosses the card plane at v = CU3_BASE_V.
# Sun passes: elevation CU3_SUN_EL, horizontal angle phi between the view direction and the sun, toward +u:
#   BACK phi 20 (sun behind the cloud, a little right), SIDE phi 90 (sun from +u), FRONT phi 160 (behind the camera, a little right).
# Raw (CU3_RAW, default bakes/<P>/raw): cumulus3_<i>.vdb, cumulus3_<i>_tau.npz, cumulus3_<i>_<back|side|front|sky>.exr.
# Textures (CU3_OUT, default bakes/<P>), cells 2 x 4 of 1024 x 512, cell i at column i % 2, row i // 2 (row 0 at the top):
#   T_BSky_<P>_CumulusSun.exr  RGB half: sun-lit luminance per unit opacity (straight, dilated), R BACK, G SIDE, B FRONT, one scale
#   T_BSky_<P>_CumulusAux.png  RGBA8 straight: R sky-lit luminance per unit opacity x sky factor, G opacity, B thickness, A 255
#   cumulus3_meta.json
CU3_SUN_EL = arg("cu3el", 6.0)
CU3_PASSES = (("back", 20.0), ("side", 90.0), ("front", 160.0))
CU3_SKY_PHI = 90.0                           # the sky pass's sky has its sun on the +u side, like the SIDE pass
CU3_SUN = arg("cu3sun", 8.3)                 # white sun lamp strength (W/m^2), the same in every pass: FRONT body ~ 1
CU3_SAMPLES = arg("cu3samples", 256)              # with OpenImageDenoise (round 1: 512, no denoiser)
CU3_SKY_SAMPLES = arg("cu3skysamples", 0)    # samples of the sky pass (0 = CU3_SAMPLES)
CU3_BOUNCES = arg("cu3bounces", 512)             # converged multiple scattering (round 1: 16, which cut it)
CU3_BACK_BOUNCES = arg("cu3backbounces", 0)  # volume bounces of the BACK pass (0 = CU3_BOUNCES)
CU3_ADAPT = arg("cu3adapt", 0.003)           # adaptive sampling noise threshold
CU3_BIASED = arg("cu3biased", 1)             # 1 = ray-marched volumes (as round 1), 0 = Cycles' unbiased null scattering
CU3_DENOISE = arg("cu3denoise", 1)           # 1 = OpenImageDenoise on the Combined pass (each render has one light only)
CU3_PITCH = arg("cu3pitch", 6.0)             # camera pitch above the horizontal (deg): the card seen from below its base
CU3_BASE_V = arg("cu3basev", 0.80)           # image v of the base plane where it crosses the card plane y = 0 (round 1: 0.8455)
CU3_CAM_Y = -6000.0                          # camera distance in front of the card plane (m)
CU3_MARGIN_M = arg("cu3margin", 115.0)       # shape fit: keep every blob this far (m) inside the frame (115 m = 53 px)
CU3_RES = arg("cu3res", 432)                 # voxels across the 2 km domain width (round 1: 288)
CU3_UNDER = arg("cu3under", 0.25)            # sky pass: radiance below the horizon = this x the mirrored sky (0 = model's)
CU3_SCALE = arg("cu3scale", 1.0)             # render resolution scale (1 for the shipped bake; < 1 for quick tests)
CU3_CELLS = [int(c) for c in arg("cu3cells", "0,1,2,3,4,5,6,7").split(",") if c.strip().isdigit()]   # "none" = no cards
CU3_DO = arg("cu3passes", "back,side,front,sky").split(",")
CU3_SHAPES = arg("cu3shapes", 1)             # 1 = build shapes (cu3_density), 0 = reuse the "cumulus" stage's VDBs,
                                             # 2 = reuse cumulus3_<i>.vdb and its _tau.npz already in CU3_RAW
CU3_PHASE = arg("cu3phase", "hg")            # hg (Principled Volume), draine (Mie fit minus its diffraction peak), dual, mie
CU3_DENS = arg("cu3density", 0.10)          # extinction (1/m) at grid density 1 (round 1: CU_DENSITY 0.07); denser reads
                                             # crisper at the 2.2-5x magnification the cards are drawn at
CU3_ANISO = arg("cu3aniso", CU_ANISO)
CU3_DUAL = tuple(float(v) for v in arg("cu3dual", "0.7,0.8,-0.2").split(","))   # phase "dual": weight, g1, g2
CU3_DIAM = arg("cu3diam", 15.0)              # droplet diameter (um) for the Mie fit
CU3_HAZE_T = arg("cu3haze", 0.3)             # haze-block sun transmission at the cloud base (1 = no haze block)
CU3_HAZE_TOP = arg("cu3hazetop", 0.6)        # haze block top, as a fraction of the cloud height above the base
CU3_RENDER = arg("cu3render", 1)             # 0 = build the VDBs and their projections only
CU3_RAW = arg("cu3raw", RAW)
CU3_OUT = arg("cu3out", OUT)
CU3_LOOK = arg("cu3look", "")                # extra path for the look sheet
CU3_SEED = arg("cu3seed", 0)
CU3_EXPOSURE = arg("cu3exposure", 0.0)       # 0 = derive the recommended Exposure from the bake (see stage_cumulus3compose)
CU3_SMOOTH = arg("cu3smooth", 0.0)           # compose: extra alpha-weighted gaussian (px) where alpha > 0.1 (0 = off)
CU3_SUN_SCALE = arg("cu3sunscale", "phys")   # compose: common scale of the sun channels ("phys" or a number)
CU3_SUNTINT = 1.6                            # runtime: SunTint = linear(#FFB88C) * 1.6, AmbTint = linear(#8784BA) * 0.9
CU3_AMBTINT = 0.9


def cu3_towers(kind, rng):
    """Thermals as (x, y, base half-width r, top above the base) in metres, before fitting; whether a low skirt joins
    them; and the card's wind shear (dx/dz toward +x, the downwind side). Sized for how the runtime draws the cards (4.8 to
    11.5 km wide for the 2.2 km bake frame): many thermals of mixed heights rather than the 2-4 of a single 2 km cumulus."""
    W = CU_W
    t = []
    shear = rng.uniform(0.10, 0.22)
    if kind in ("clusters3", "clusters2"):
        xs = (-0.34, -0.02, 0.31) if kind == "clusters3" else (-0.24, 0.22)
        for cx in xs:
            r = 0.075 * W * rng.uniform(0.85, 1.2)
            x0 = cx * W + rng.normal(0, 0.015 * W)
            y0 = rng.normal(0, 0.02 * W)
            t.append((x0, y0, r, r * rng.uniform(0.95, 1.5)))                 # humilis / mediocris: wider than tall
            for s in rng.permutation([-1.0, 1.0])[:int(rng.integers(1, 3))]:   # one or two lower shoulders
                rs = r * rng.uniform(0.5, 0.7)
                t.append((x0 + s * r * rng.uniform(0.6, 0.9), y0 + rng.normal(0, 0.3 * r), rs, rs * rng.uniform(0.8, 1.25)))
        return t, False, shear
    if kind == "wide":
        n = int(rng.integers(9, 13))
        tall = set(rng.choice(n, size=int(rng.integers(2, 4)), replace=False).tolist())
        for k in range(n):
            u = (k + rng.uniform(-0.3, 0.3)) / (n - 1) * 2 - 1
            centre = 1.0 - abs(u)
            r = (0.045 + 0.025 * centre) * W * rng.uniform(0.8, 1.25) * (1.25 if k in tall else 1.0)
            h = r * rng.uniform(0.9, 1.6) * (0.7 + 0.6 * centre) * (rng.uniform(1.15, 1.45) if k in tall else 1.0)
            t.append((u * 0.40 * W, rng.normal(0, 0.05 * W), r, h))
        return t, True, shear
    if kind == "towering":
        t.append((0.0 * W, 0.0, 0.12 * W, 0.5 * W))
        t.append((-0.15 * W, rng.normal(0, 0.03 * W), 0.09 * W, 0.26 * W * rng.uniform(0.9, 1.1)))
        t.append((0.16 * W, rng.normal(0, 0.03 * W), 0.08 * W, 0.20 * W * rng.uniform(0.9, 1.1)))
        t.append((-0.29 * W, rng.normal(0, 0.03 * W), 0.06 * W, 0.10 * W * rng.uniform(0.9, 1.2)))
        t.append((0.30 * W, rng.normal(0, 0.03 * W), 0.055 * W, 0.08 * W * rng.uniform(0.9, 1.2)))
        return t, True, shear
    if kind == "congestus":
        t.append((-0.10 * W, 0.0, 0.11 * W, 0.40 * W))
        t.append((0.11 * W, 0.02 * W, 0.10 * W, 0.33 * W))
        t.append((0.29 * W, rng.normal(0, 0.03 * W), 0.07 * W, 0.15 * W))
        t.append((-0.30 * W, rng.normal(0, 0.03 * W), 0.07 * W, 0.13 * W))
        t.append((0.0, -0.06 * W, 0.06 * W, 0.18 * W * rng.uniform(0.9, 1.1)))
        return t, True, shear
    # medium
    n = int(rng.integers(4, 7))
    for k in range(n):
        u = (k + rng.uniform(-0.25, 0.25)) / (n - 1) * 2 - 1
        centre = 1.0 - abs(u)
        r = (0.065 + 0.03 * centre) * W * rng.uniform(0.85, 1.15)
        t.append((u * 0.30 * W, rng.normal(0, 0.04 * W), r, r * (1.2 + 1.0 * centre) * rng.uniform(0.85, 1.15)))
    return t, True, shear


def cu3_spheres(kind, rng):
    """Dome hierarchy per thermal: a near-untapered bubble stack under a wider head (crown domes reach down to 60 deg below
    its equator, so the head bulges past its stem and undercuts it), sheared downwind; 2-4 domes on each lower bubble's
    flank; three to four generations of smaller domes on the crowns (two on the flanks); no crown dome pointing straight at
    the camera. Also hanging lobes under a third of the thermals and detached fractus pieces, mostly downwind. Then a
    uniform fit (about x mid, y 0, the base) so the pitched card view keeps every part clear of the frame margins.
    Returns (main rows, hanging rows, fractus rows, fit), rows (cx, cy, cz, rx, ry, rz, blend[, density scale])."""
    towers, skirt, shear = cu3_towers(kind, rng)
    base = CU_BASE_Z
    sph, hang, frac = [], [], []

    def turn(ct, ph, crown):
        # no crown dome bulging straight at the camera (-y): such domes, seen head-on, read as faces
        st = math.sqrt(max(0.0, 1.0 - ct * ct))
        if crown and st * math.sin(ph) < -0.8:
            ph = -0.5 * math.pi + math.copysign(1.0, math.cos(ph) if abs(math.cos(ph)) > 1e-6 else 1.0) * 1.0
        return st, ph

    for (x, y, r, top) in towers:
        lean = shear * rng.uniform(0.4, 0.8)
        n = max(1, min(8, int(math.ceil((top - 0.7 * r) / (0.42 * r))) + 1))
        taper = rng.uniform(0.03, 0.18)
        head = rng.uniform(1.0, 1.12) if top > 2.0 * r else 1.0
        stack = []
        for k in range(n):
            u = k / (n - 1) if n > 1 else 0.0
            rr = r * (1.0 - taper * u) * rng.uniform(0.86, 1.14) * (head if (k == n - 1 and n > 1) else 1.0)
            z0 = base + 0.22 * r
            z = z0 + u * max(0.0, (top - 0.85 * rr) - (z0 - base))
            stack.append((x + lean * (z - base) + rng.normal(0, 0.12 * rr), y + rng.normal(0, 0.06 * rr), z, rr))
        for k, (cx, cy, cz, cr) in enumerate(stack):
            sph.append((cx, cy, cz, cr * 1.12, cr * 1.0, cr * (0.78 if k == 0 else 0.9), 0.25 * cr))
        for k, (cx, cy, cz, cr) in enumerate(stack):
            crown = k == len(stack) - 1
            nd = int(rng.integers(6, 11)) if crown else int(rng.integers(4, 9))
            for d_i in range(nd):
                if crown and d_i == 0:          # the crown's main dome, near the top
                    ct = rng.uniform(0.85, 1.0)
                elif crown:                     # mostly upward, a few reaching down to undercut the head
                    ct = 1.0 - 1.5 * rng.random() ** 1.7
                else:
                    ct = rng.uniform(-0.5, 0.6)
                st, ph = turn(ct, rng.uniform(0, math.tau), crown)
                if crown and d_i == 0:
                    rc, off = cr * rng.uniform(0.65, 0.8), cr * rng.uniform(0.55, 0.7)
                else:
                    rc = cr * (rng.uniform(0.5, 0.75) if crown else rng.uniform(0.35, 0.65))
                    off = cr * rng.uniform(0.72, 0.9)
                cz1 = cz + ct * off * 0.9
                cx1 = cx + st * math.cos(ph) * off * 1.1 + (shear * 0.5 * max(0.0, cz1 - cz) if crown else 0.0)
                c1 = (cx1, cy + st * math.sin(ph) * off, cz1)
                sph.append((c1[0], c1[1], c1[2], rc * 1.05, rc, rc * 0.95, 0.12 * rc))
                for _ in range(int(rng.integers(2, 5))):
                    ct2 = rng.uniform(0.05, 1.0)
                    st2, ph2 = turn(ct2, ph + rng.normal(0, 1.1), crown)
                    r2 = rc * rng.uniform(0.38, 0.52)
                    o2 = rc * rng.uniform(0.8, 0.95)
                    c2 = (c1[0] + st2 * math.cos(ph2) * o2, c1[1] + st2 * math.sin(ph2) * o2, c1[2] + ct2 * o2)
                    sph.append((c2[0], c2[1], c2[2], r2, r2, r2 * 0.95, 0.1 * r2))
                    if crown and rng.random() < 0.6:
                        ct3 = rng.uniform(0.25, 1.0)
                        st3, ph3 = turn(ct3, ph2 + rng.normal(0, 1.0), crown)
                        r3 = r2 * rng.uniform(0.4, 0.55)
                        o3 = r2 * rng.uniform(0.8, 0.95)
                        c3 = (c2[0] + st3 * math.cos(ph3) * o3, c2[1] + st3 * math.sin(ph3) * o3, c2[2] + ct3 * o3)
                        sph.append((c3[0], c3[1], c3[2], r3, r3, r3 * 0.95, 0.1 * r3))
                        if ct3 > 0.4 and rng.random() < 0.5:
                            ct4 = rng.uniform(0.3, 1.0)
                            st4, ph4 = turn(ct4, ph3 + rng.normal(0, 1.0), crown)
                            r4 = r3 * rng.uniform(0.42, 0.5)
                            o4 = r3 * rng.uniform(0.8, 0.95)
                            sph.append((c3[0] + st4 * math.cos(ph4) * o4, c3[1] + st4 * math.sin(ph4) * o4,
                                        c3[2] + ct4 * o4, r4, r4, r4 * 0.95, 0.1 * r4))
        # a lobe or two hanging under the stronger thermals (near half, so the pitched view shows them against the base)
        if rng.random() < 0.34:
            for _ in range(int(rng.integers(1, 3))):
                rl = r * rng.uniform(0.15, 0.25)
                hang.append((x + rng.normal(0, 0.3 * r), y - abs(rng.normal(0, 0.25 * r)), base - rl * rng.uniform(0.0, 0.3),
                             rl * 1.3, rl * 1.1, rl * 0.6, 0.5 * rl))
    if skirt:
        xs = [tw[0] for tw in towers]
        rs = [tw[2] for tw in towers]
        x_beg = min(xs) - 0.3 * max(rs)
        x_end = max(xs) + 0.3 * max(rs) + 0.05 * CU_W          # the shelf runs a little further downwind
        x = x_beg
        while x <= x_end:
            u = (x - x_beg) / max(1.0, x_end - x_beg) * 2.0 - 1.0
            r = 0.08 * CU_W * rng.uniform(0.75, 1.15) * (1.0 - 0.45 * u * u)   # thinner toward the ends
            zc = base + 0.28 * r + rng.normal(0, 0.06 * r)
            rz = r * rng.uniform(0.4, 0.65)
            yc = rng.normal(0, 0.03 * CU_W)
            sph.append((x, yc, zc, r * rng.uniform(1.3, 1.8), r * rng.uniform(1.0, 1.4), rz, 0.35 * r))
            for _ in range(int(rng.integers(0, 3))):        # low turrets on the shelf, so it is not a smooth disc
                rd = r * rng.uniform(0.3, 0.5)
                sph.append((x + rng.normal(0, 0.4 * r), yc + rng.normal(0, 0.3 * r), zc + rz * rng.uniform(0.4, 0.8),
                            rd * 1.1, rd, rd * 0.85, 0.15 * rd))
            x += 0.07 * CU_W
    a = np.array(sph, np.float64)
    # detached fractus: 2-4 small, stretched blobs per piece, 70 % on the downwind (+x) side, at 15-70 % of the height,
    # placed 0.03-0.12 W outside the hull at their height
    h_top = float((a[:, 2] + a[:, 5]).max() - base)
    n_f = int(rng.integers(1, 5)) if kind in ("clusters3", "clusters2") else int(rng.integers(2, 7))
    for _ in range(n_f):
        side = 1.0 if rng.random() < 0.7 else -1.0
        zf = base + h_top * rng.uniform(0.15, 0.7)
        q = 1.0 - ((zf - a[:, 2]) / a[:, 5]) ** 2
        ok = q > 0
        if not ok.any():
            continue
        edge = (a[ok, 0] + side * a[ok, 3] * np.sqrt(q[ok])) * side
        xf = side * float(edge.max()) + side * CU_W * rng.uniform(0.03, 0.12)
        yf = rng.normal(0, 0.12 * CU_D)
        dscale = rng.uniform(0.25, 0.5)
        rf0 = CU_W * rng.uniform(0.015, 0.035)
        for _ in range(int(rng.integers(3, 7))):
            rf = rf0 * rng.uniform(0.5, 1.0)
            frac.append((xf + rng.normal(0, 0.9 * rf0), yf + rng.normal(0, 0.7 * rf0), zf + rng.normal(0, 0.5 * rf0),
                         rf * rng.uniform(1.2, 2.0), rf * rng.uniform(0.8, 1.2), rf * rng.uniform(0.5, 0.8), 0.5 * rf, dscale))
    hg = np.array(hang, np.float64).reshape(-1, 7)
    fr = np.array(frac, np.float64).reshape(-1, 8)
    every = np.concatenate([a, hg, fr[:, :7]], 0)
    # fit: horizontal extent, depth, the domain top, and the pitched view's frame (v measured from the base line)
    lo_x, hi_x = (every[:, 0] - every[:, 3]).min(), (every[:, 0] + every[:, 3]).max()
    xmid = 0.5 * (lo_x + hi_x)
    p = math.radians(CU3_PITCH)
    e = np.sqrt((every[:, 4] * math.sin(p)) ** 2 + (every[:, 5] * math.cos(p)) ** 2)
    up = (every[:, 2] - base) * math.cos(p) - every[:, 1] * math.sin(p) + e
    down = -((every[:, 2] - base) * math.cos(p) - every[:, 1] * math.sin(p) - e)
    n_main = len(a)
    # the main union is clipped by the base plane: it reaches below the base line only by the plane's far edge
    down[:n_main] = np.minimum(down[:n_main], (every[:n_main, 1] + every[:n_main, 4]) * math.sin(p))
    v_base_m = (0.5 - CU3_BASE_V) * CU_ORTHO / 2.0
    lim = CU_ORTHO / 4.0 - CU3_MARGIN_M
    f = min((0.44 * CU_W) / (0.5 * (hi_x - lo_x)),
            (0.42 * CU_D) / float((np.abs(every[:, 1]) + every[:, 4]).max()),
            (0.88 * CU_H - base) / float((every[:, 2] + every[:, 5]).max() - base),
            (lim - v_base_m - 25.0) / float(up.max()),
            (lim + v_base_m - 30.0) / max(float(down.max()), 1.0))
    out = []
    for arr in (a, hg, fr):
        arr = arr.copy()
        if len(arr):
            arr[:, 0] = (arr[:, 0] - xmid) * f
            arr[:, 1] *= f
            arr[:, 2] = base + (arr[:, 2] - base) * f
            arr[:, 3:7] *= f
        out.append(arr)
    return out[0], out[1], out[2], f


def cu3_outside(mask_open):
    """Pixels of a 2D boolean 'open' mask that are connected to the image border through open pixels."""
    out = np.zeros_like(mask_open)
    out[0, :] = mask_open[0, :]
    out[-1, :] = mask_open[-1, :]
    out[:, 0] = mask_open[:, 0]
    out[:, -1] = mask_open[:, -1]
    for _ in range(4 * sum(mask_open.shape)):
        g = out.copy()
        g[1:, :] |= out[:-1, :]
        g[:-1, :] |= out[1:, :]
        g[:, 1:] |= out[:, :-1]
        g[:, :-1] |= out[:, 1:]
        g &= mask_open
        if np.array_equal(g, out):
            break
        out = g
    return out


def cu3_ray_z(v_m, ys):
    """Heights where the card camera's ray through image height v_m (metres, up) crosses the depth slices ys."""
    y_c, z_c, p = cu3_cam_params()
    return z_c + v_m / math.cos(p) + (ys - y_c) * math.tan(p)


def cu3_smoothstep(e0, e1, x):
    t = np.clip((x - e0) / (e1 - e0), 0.0, 1.0)
    return t * t * (3.0 - 2.0 * t)


def cu3_density(kind, seed):
    """Round-2 cumulus density in the CU domain, CU3_RES voxels across. Shapes from cu3_spheres; billowy displacement
    strongest on the upper surfaces; a ragged base (two noise scales) with hanging lobes; enclosed see-through holes (in
    the pitched card view) plugged; a shell whose width follows the surface: about 8 m on upward-facing crowns, 30-60 m on
    flanks, the base and the downwind side; erosion 15-90 m deep with wind-combed (x-stretched) noise; detached fractus.
    Returns (dens [nx,ny,nz], vox, xs, zs, fit, filled hole voxels, extra dict)."""
    t0 = time.time()
    rng = np.random.default_rng(seed)
    sph, hang, frac, f = cu3_spheres(kind, rng)
    base = CU_BASE_Z
    vox = CU_W / CU3_RES
    nx, ny, nz = CU3_RES, int(round(CU_D / vox)), int(round(CU_H / vox))
    xs = ((np.arange(nx) + 0.5) * vox - CU_W / 2).astype(np.float32)
    ys = ((np.arange(ny) + 0.5) * vox - CU_D / 2).astype(np.float32)
    zs = ((np.arange(nz) + 0.5) * vox).astype(np.float32)

    def box(cx, cy, cz, pad):
        i0, i1 = np.searchsorted(xs, cx - pad), np.searchsorted(xs, cx + pad)
        j0, j1 = np.searchsorted(ys, cy - pad), np.searchsorted(ys, cy + pad)
        k0, k1 = np.searchsorted(zs, cz - pad), np.searchsorted(zs, cz + pad)
        return i0, i1, j0, j1, k0, k1

    def union(sdf, rows):
        for row in rows:
            cx, cy, cz, rx, ry, rz, kb = row[:7]
            i0, i1, j0, j1, k0, k1 = box(cx, cy, cz, max(rx, ry, rz) + kb + 2 * vox)
            if i1 <= i0 or j1 <= j0 or k1 <= k0:
                continue
            q = np.sqrt(((xs[i0:i1, None, None] - cx) / rx) ** 2 + ((ys[None, j0:j1, None] - cy) / ry) ** 2 +
                        ((zs[None, None, k0:k1] - cz) / rz) ** 2)
            d = ((q - 1.0) * min(rx, ry, rz)).astype(np.float32)
            sdf[i0:i1, j0:j1, k0:k1] = smin(sdf[i0:i1, j0:j1, k0:k1], d, kb)
        return sdf

    X, Y, Z = np.meshgrid(xs, ys, zs, indexing="ij")
    P = np.stack([X, Y, Z], axis=-1)
    del X, Y
    z_hi = float((sph[:, 2] + sph[:, 5]).max())
    hz = np.clip((Z - base) / max(60.0, z_hi - base), 0, 1).astype(np.float32)
    s0 = seed * 7
    disp = (fbm3(P / 260.0, s0 + 1, octaves=3) - 0.5) * 55.0
    n = fbm3(P / 70.0, s0 + 2, octaves=2)
    disp -= (np.abs(n * 2.0 - 1.0) * 16.0 - 5.0) * (0.4 + 0.6 * hz)
    n = fbm3(P / 24.0, s0 + 5, octaves=2)
    disp -= (np.abs(n * 2.0 - 1.0) * 5.5 - 2.0) * hz
    del n
    disp = disp.astype(np.float32)
    XY = np.stack([P[:, :, 0, 0], P[:, :, 0, 1]], -1)
    zb = (base + (fbm2(XY / 240.0, s0 + 3, octaves=4) - 0.5) * 90.0
          + (fbm2(XY / 80.0, s0 + 7, octaves=3) - 0.5) * 30.0)[:, :, None].astype(np.float32)

    def shape(rows):
        sdf = union(np.full((nx, ny, nz), 1.0e4, np.float32), rows) + disp
        sdf = -smin(-sdf, -(zb - Z), 16.0)
        if len(hang):
            sdf = smin(sdf, union(np.full((nx, ny, nz), 1.0e4, np.float32), hang) + 0.5 * disp, 20.0)
        return sdf.astype(np.float32)

    h_hole = 256
    v_rows = (0.5 - (np.arange(h_hole) + 0.5) / h_hole) * (CU_ORTHO / 2.0)

    def proj_rows(field):
        out = np.zeros((h_hole, nx), np.float32)
        for r in range(h_hole):
            k = (cu3_ray_z(v_rows[r], ys) - zs[0]) / vox
            k0 = np.round(k).astype(np.int64)
            ok = (k0 >= 0) & (k0 < nz)
            out[r] = (field[:, np.arange(ny), np.clip(k0, 0, nz - 1)] * ok[None, :]).sum(axis=1)
        return out

    def holes_of(sdf):
        x = np.clip((-sdf + 2.0) / 16.0, 0, 1)
        t2d = proj_rows(x * x * (3 - 2 * x)) * vox * CU3_DENS
        solid = t2d > 1.5
        return ~solid & ~cu3_outside(~solid)

    sdf = shape(sph)
    holes = holes_of(sdf)
    n_holes = int(holes.sum())
    for _ in range(2):
        if not holes.any():
            break
        plugs = []
        hr, hi = np.nonzero(holes)
        inside_y = np.clip(-sdf, 0, None)
        for r, i in zip(hr[::2], hi[::2]):
            zr = cu3_ray_z(v_rows[r], ys)
            k = np.clip(np.round((zr - zs[0]) / vox).astype(np.int64), 0, nz - 1)
            wsum = np.zeros(ny, np.float32)
            for di in range(max(0, i - 4), min(nx, i + 5)):
                wsum += inside_y[di, np.arange(ny), k]
            cy = float((wsum * ys).sum() / wsum.sum()) if wsum.sum() > 0 else 0.0
            cz = float(cu3_ray_z(v_rows[r], np.array([cy]))[0])
            rp = 4.0 * vox
            plugs.append((float(xs[i]), cy, cz, rp, 2.5 * rp, rp, 0.6 * rp))
        sph = np.concatenate([sph, np.array(plugs, np.float64)], 0)
        sdf = shape(sph)
        holes = holes_of(sdf)
    n_left = int(holes.sum())
    del disp, zb
    # surface orientation from the SDF gradient (outward normal): z component (up-facing) and x component (downwind)
    gx = np.zeros_like(sdf)
    gz = np.zeros_like(sdf)
    gx[1:-1] = sdf[2:] - sdf[:-2]
    gz[:, :, 1:-1] = sdf[:, :, 2:] - sdf[:, :, :-2]
    gy = np.zeros_like(sdf)
    gy[:, 1:-1] = sdf[:, 2:] - sdf[:, :-2]
    gl = np.sqrt(gx * gx + gy * gy + gz * gz) + 1e-6
    up = cu3_smoothstep(0.15, 0.65, gz / gl) * cu3_smoothstep(0.3, 0.65, hz)
    dw = cu3_smoothstep(0.1, 0.7, gx / gl)
    del gx, gy, gz, gl, hz
    inside = -sdf
    del sdf
    m1 = fbm3(P / 350.0, s0 + 8, octaves=2)
    soft = 8.0 + (1.0 - up) * (20.0 + 40.0 * np.clip((m1 - 0.3) / 0.4, 0, 1) + 14.0 * dw)
    del m1, dw
    x = np.clip((inside + 2.0) / soft, 0, 1)
    dens = x * x * (3 - 2 * x)
    del x, soft
    m2 = fbm3(P / 300.0, s0 + 9, octaves=2)
    ero_d = 15.0 + (1.0 - up) * 75.0 * np.clip((m2 - 0.3) / 0.4, 0, 1)
    del m2
    ero = fbm3(P * np.array([1 / 120.0, 1 / 40.0, 1 / 40.0], np.float32), s0 + 4, octaves=3)
    rim = 1.0 - np.clip(inside / ero_d, 0, 1)
    dens *= np.clip(1.0 - 0.75 * rim * np.clip((ero - 0.35) * 2.2, 0, 1) * (1.0 - 0.6 * up), 0, 1)
    del rim, ero_d
    dens *= (0.88 + 0.24 * fbm3(P / 140.0, s0 + 6, octaves=2)).astype(np.float32)
    fuzz = np.clip((ero - 0.6) * 3.0, 0, 1) * np.exp(-np.maximum(-inside, 0) / 10.0) * 0.06 * (1.0 - up)
    dens = np.maximum(dens, fuzz * (inside < 0)).astype(np.float32)
    del fuzz, up, inside
    # detached fractus pieces: soft, eroded, thinner than the cloud
    n_frac = 0
    if len(frac):
        for ds in np.unique(frac[:, 7]):
            rows = frac[frac[:, 7] == ds]
            pad = float((rows[:, 3:6].max(axis=1) + rows[:, 6]).max()) + 30.0
            i0 = np.searchsorted(xs, rows[:, 0].min() - pad)
            i1 = np.searchsorted(xs, rows[:, 0].max() + pad)
            j0 = np.searchsorted(ys, rows[:, 1].min() - pad)
            j1 = np.searchsorted(ys, rows[:, 1].max() + pad)
            k0 = np.searchsorted(zs, rows[:, 2].min() - pad)
            k1 = np.searchsorted(zs, rows[:, 2].max() + pad)
            if i1 <= i0 or j1 <= j0 or k1 <= k0:
                continue
            sub = np.full((i1 - i0, j1 - j0, k1 - k0), 1.0e4, np.float32)
            for (cx, cy, cz, rx, ry, rz, kb, _) in rows:
                q = np.sqrt(((xs[i0:i1, None, None] - cx) / rx) ** 2 + ((ys[None, j0:j1, None] - cy) / ry) ** 2 +
                            ((zs[None, None, k0:k1] - cz) / rz) ** 2)
                sub = smin(sub, ((q - 1.0) * min(rx, ry, rz)).astype(np.float32), kb)
            Pl = P[i0:i1, j0:j1, k0:k1]
            sub += (fbm3(Pl / 45.0, s0 + 11, octaves=3) - 0.5) * 30.0
            xx = np.clip((-sub + 2.0) / 22.0, 0, 1)
            dl = xx * xx * (3 - 2 * xx)
            el = fbm3(Pl * np.array([1 / 60.0, 1 / 40.0, 1 / 40.0], np.float32), s0 + 12, octaves=3)
            dl *= np.clip(1.0 - 0.9 * np.clip((el - 0.3) * 2.0, 0, 1), 0, 1) * ds
            dens[i0:i1, j0:j1, k0:k1] = np.maximum(dens[i0:i1, j0:j1, k0:k1], dl.astype(np.float32))
            n_frac += 1
    edge = np.minimum.reduce([
        np.clip(P[..., 0] + CU_W / 2, 0, 3 * vox) / (3 * vox), np.clip(CU_W / 2 - P[..., 0], 0, 3 * vox) / (3 * vox),
        np.clip(P[..., 1] + CU_D / 2, 0, 3 * vox) / (3 * vox), np.clip(CU_D / 2 - P[..., 1], 0, 3 * vox) / (3 * vox),
        np.clip(CU_H - Z, 0, 3 * vox) / (3 * vox), np.clip(Z, 0, 3 * vox) / (3 * vox)])
    dens = np.clip(dens * edge, 0, 1).astype(np.float32)
    log("cumulus3 %s: %d blobs, %d hanging, %d fractus pieces, fit %.2f, hole px %d (left %d), grid %s, %.1fs" % (
        kind, len(sph), len(hang), n_frac, f, n_holes, n_left, dens.shape, time.time() - t0))
    return dens, vox, xs, zs, f, n_holes, {"n_blobs": len(sph), "n_hanging": len(hang), "n_fractus": n_frac}


def cu3_material():
    """White, non-absorbing cloud medium. Density from the VDB grid times CU3_DENS (extinction 1/m)."""
    m = bpy.data.materials.new("Cu3Mat")
    if m.node_tree is None:
        m.use_nodes = True
    nt = m.node_tree
    nt.nodes.clear()
    out = nt.nodes.new("ShaderNodeOutputMaterial")
    if CU3_PHASE == "hg":
        pv = nt.nodes.new("ShaderNodeVolumePrincipled")
        pv.inputs["Color"].default_value = (1, 1, 1, 1)
        pv.inputs["Density"].default_value = CU3_DENS
        pv.inputs["Anisotropy"].default_value = CU3_ANISO
        nt.links.new(pv.outputs["Volume"], out.inputs["Volume"])
    else:
        at = nt.nodes.new("ShaderNodeAttribute")
        at.attribute_name = "density"
        d = float(CU3_DIAM)
        lobes = []
        if CU3_PHASE == "dual":
            # w * HG(g1) + (1 - w) * HG(g2); default 0.7 HG(0.8) + 0.3 HG(-0.2), the HG2 the runtime applies to the layers
            w1, g1, g2 = CU3_DUAL
            lobes = [("HENYEY_GREENSTEIN", w1, {"Anisotropy": g1}), ("HENYEY_GREENSTEIN", 1.0 - w1, {"Anisotropy": g2})]
        elif CU3_PHASE == "draine":
            # Jendersie & d'Eon 2023 Mie fit (5-50 um): w_D * Draine(g_D, alpha) + (1 - w_D) * HG(g_HG ~ 0.99). The HG part
            # is a narrow diffraction peak that only shifts rays by a degree or two: treated as transmission (delta scaling),
            # so the cloud's opacity, the camera alpha and the composite agree. Extinction stays CU3_DENS.
            g_d = math.exp(-2.20679 / (d + 3.91029) - 428.934 / (d + 3.91029) ** 2)
            a_d = math.exp(3.62489 - 8.29288 / (d + 5.52825))
            lobes = [("DRAINE", 1.0, {"Anisotropy": g_d, "Alpha": a_d})]
        else:  # full Mie fit, diffraction peak included
            lobes = [("MIE", 1.0, {"Diameter": d})]
        acc = None
        for phase, wgt, ins in lobes:
            mul = nt.nodes.new("ShaderNodeMath")
            mul.operation = "MULTIPLY"
            mul.inputs[1].default_value = CU3_DENS * wgt
            nt.links.new(at.outputs["Fac"], mul.inputs[0])
            vs = nt.nodes.new("ShaderNodeVolumeScatter")
            vs.phase = phase
            vs.inputs["Color"].default_value = (1, 1, 1, 1)
            for k, v in ins.items():
                vs.inputs[k].default_value = v
            nt.links.new(mul.outputs["Value"], vs.inputs["Density"])
            if acc is None:
                acc = vs.outputs["Volume"]
            else:
                add = nt.nodes.new("ShaderNodeAddShader")
                nt.links.new(acc, add.inputs[0])
                nt.links.new(vs.outputs["Volume"], add.inputs[1])
                acc = add.outputs["Shader"]
        nt.links.new(acc, out.inputs["Volume"])
    try:
        m.cycles.volume_interpolation = "CUBIC"
    except Exception as e:  # noqa: BLE001
        log("cubic interpolation not set:", e)
    return m


def cu3_sun_dir(phi_deg):
    """Direction to the sun in the card frame: horizontal angle phi from the view direction (+Y) toward +u (+X)."""
    e, p = math.radians(CU3_SUN_EL), math.radians(phi_deg)
    return Vector((math.cos(e) * math.sin(p), math.cos(e) * math.cos(p), math.sin(e)))


def cu3_haze_block(sc, sun_d, reach, z_top, sigma):
    """Absorbing haze on the sun's side of the cloud, below z_top, invisible to the camera (see cu_shade_slab): the low sun
    reaches the lower cloud through a longer haze path, so tops stay sunlit and bases fall toward the sky-lit mauve."""
    import bmesh
    h = Vector((sun_d.x, sun_d.y, 0.0)).normalized()
    side = Vector((-h.y, h.x, 0.0))
    o = h * (reach + 40.0)
    me = bpy.data.meshes.new("Cu3Haze")
    bm = bmesh.new()
    bmesh.ops.create_cube(bm, size=1.0)
    for v in bm.verts:
        a, b, c = (v.co.x + 0.5) * 12000.0, v.co.y * 16000.0, z_top - 6000.0 + (v.co.z + 0.5) * 6000.0
        p = o + h * a + side * b
        v.co = Vector((p.x, p.y, c))
    bm.to_mesh(me)
    bm.free()
    m = bpy.data.materials.new("Cu3HazeMat")
    if m.node_tree is None:
        m.use_nodes = True
    nt = m.node_tree
    nt.nodes.clear()
    ab = nt.nodes.new("ShaderNodeVolumeAbsorption")
    ab.inputs["Color"].default_value = (0, 0, 0, 1)
    ab.inputs["Density"].default_value = sigma
    out = nt.nodes.new("ShaderNodeOutputMaterial")
    nt.links.new(ab.outputs["Volume"], out.inputs["Volume"])
    me.materials.append(m)
    ob = bpy.data.objects.new("Cu3Haze", me)
    sc.collection.objects.link(ob)
    ob.visible_camera = False
    return ob


def cu3_sun_sky_balance():
    """Physical sun vs sky light of the same sky model at CU3_SUN_EL: a white diffuse plane facing the sun rendered with
    the sky's sun disc on and off (difference = direct sun E / pi), plus the sky alone on an up-facing plane. Lets the
    meta state how the runtime's SunTint / AmbTint weighting compares with this sky. Writes raw/cumulus3_sunsky.json."""
    import bmesh
    sc = new_scene("Cu3SunSky")
    skyn = world_sky(sc, strength=1.0, sun_disc=True)
    skyn.sun_elevation = math.radians(CU3_SUN_EL)
    skyn.sun_rotation = math.radians(90.0)
    sun_dir = Vector((math.cos(math.radians(CU3_SUN_EL)), 0.0, math.sin(math.radians(CU3_SUN_EL))))
    m = bpy.data.materials.new("Cu3White")
    if m.node_tree is None:
        m.use_nodes = True
    nt = m.node_tree
    nt.nodes.clear()
    df = nt.nodes.new("ShaderNodeBsdfDiffuse")
    df.inputs["Color"].default_value = (1, 1, 1, 1)
    mo = nt.nodes.new("ShaderNodeOutputMaterial")
    nt.links.new(df.outputs["BSDF"], mo.inputs["Surface"])
    me = bpy.data.meshes.new("Cu3Plane")
    bm = bmesh.new()
    bmesh.ops.create_grid(bm, x_segments=1, y_segments=1, size=1.0)
    bm.to_mesh(me)
    bm.free()
    me.materials.append(m)
    pl = bpy.data.objects.new("Cu3Plane", me)
    sc.collection.objects.link(pl)
    cd = bpy.data.cameras.new("Cu3PlaneCam")
    cd.type = "ORTHO"
    cd.ortho_scale = 0.5
    cam = bpy.data.objects.new("Cu3PlaneCam", cd)
    sc.collection.objects.link(cam)
    sc.camera = cam
    res = {}
    for name, normal, disc in (("sun_facing_disc", sun_dir, True), ("sun_facing_sky", sun_dir, False),
                               ("up_sky", Vector((0, 0, 1)), False)):
        skyn.sun_disc = disc
        q = Vector(normal).normalized().to_track_quat("Z", "Y").to_euler()
        pl.rotation_euler = q
        cam.rotation_euler = q
        cam.location = Vector(normal).normalized() * 5.0
        path = os.path.join(CU3_RAW, "cumulus3_sunsky_%s.exr" % name)
        setup_render(sc, 16, 16, 4096, path)
        sc.cycles.use_adaptive_sampling = False
        sc.cycles.max_bounces = 1
        sc.cycles.diffuse_bounces = 0
        sc.render.film_transparent = False
        render(sc)
        d = read_exr_all(path)
        res[name] = [float(np.mean(ch(d, "Combined", c))) for c in "RGB"]
    w709 = np.array([0.2126, 0.7152, 0.0722])
    direct = np.array(res["sun_facing_disc"]) - np.array(res["sun_facing_sky"])
    out = {"sun_elevation_deg": CU3_SUN_EL, "plane_radiance": res, "direct_sun_e_over_pi_rgb": direct.tolist(),
           "direct_sun_irradiance_lum": float(direct @ w709) * math.pi,
           "direct_sun_chroma": (direct / direct.max()).tolist(),
           "sky_on_sun_facing_plane_lum": float(np.array(res["sun_facing_sky"]) @ w709),
           "sky_on_up_plane_lum": float(np.array(res["up_sky"]) @ w709)}
    with open(os.path.join(CU3_RAW, "cumulus3_sunsky.json"), "w") as fh:
        json.dump(out, fh, indent=2)
    log("sun/sky balance: direct sun E (lum) %.2f, sky on a sun-facing plane (lum) %.3f" % (
        out["direct_sun_irradiance_lum"], out["sky_on_sun_facing_plane_lum"]))


def cu3_paths(i):
    return {"vdb": os.path.join(CU3_RAW, "cumulus3_%d.vdb" % i), "tau": os.path.join(CU3_RAW, "cumulus3_%d_tau.npz" % i),
            **{n: os.path.join(CU3_RAW, "cumulus3_%d_%s.exr" % (i, n)) for n in ("back", "side", "front", "sky")}}


def cu3_cam_params():
    """The card camera: orthographic, CU_ORTHO wide, looking along +Y and pitched up by CU3_PITCH, placed so the cloud base
    plane (z = CU_BASE_Z) crosses the card plane y = 0 at image v = CU3_BASE_V. Returns (y_c, z_c, pitch rad).
    A point (x, y, z) lands at u_m = x, v_m = cos(p) * (z - z_c - (y - y_c) * tan(p)) (metres from the image centre, v_m up)."""
    p = math.radians(CU3_PITCH)
    v_m = (0.5 - CU3_BASE_V) * CU_ORTHO / 2.0
    z_c = CU_BASE_Z + CU3_CAM_Y * math.tan(p) - v_m / math.cos(p)
    return CU3_CAM_Y, z_c, p


def cu3_v_of(y, z):
    """Image v (0 at the top, 1 at the bottom) of a point at depth y and height z."""
    y_c, z_c, p = cu3_cam_params()
    v_m = math.cos(p) * (z - z_c - (y - y_c) * math.tan(p))
    return 0.5 - v_m / (CU_ORTHO / 2.0)


def cu3_project(dens, vox, xs, ys, zs, w, h, ext=1.0):
    """Line integral of dens * ext along the card camera's rays (pitched ortho view) for a w x h image. dens [nx, ny, nz]."""
    y_c, z_c, p = cu3_cam_params()
    nx, ny, nz = dens.shape
    v_m = (0.5 - (np.arange(h) + 0.5) / h) * (CU_ORTHO / 2.0)
    u_m = ((np.arange(w) + 0.5) / w - 0.5) * CU_ORTHO
    out = np.zeros((h, nx), np.float32)
    dz_dy = (ys - y_c) * math.tan(p)                                  # [ny]
    jj = np.arange(ny)[None, :]
    for r in range(h):
        z = z_c + v_m[r] / math.cos(p) + dz_dy                         # [ny]
        k = (z - zs[0]) / vox
        k0 = np.floor(k).astype(np.int64)
        fz = (k - k0).astype(np.float32)
        ok0 = (k0 >= 0) & (k0 < nz)
        ok1 = (k0 + 1 >= 0) & (k0 + 1 < nz)
        a = dens[:, jj[0], np.clip(k0, 0, nz - 1)] * (ok0 * (1 - fz))[None, :]
        b = dens[:, jj[0], np.clip(k0 + 1, 0, nz - 1)] * (ok1 * fz)[None, :]
        out[r] = (a + b).sum(axis=1)
    out *= vox / math.cos(p) * ext
    img = np.stack([np.interp(u_m, xs, out[r], left=0.0, right=0.0) for r in range(h)], 0)
    return img.astype(np.float32)


def cu3_world_sky(sc, phi):
    """Multiple-scattering sky without a sun disc, its sun at CU3_SUN_EL and compass azimuth phi (from +Y toward +X). With
    CU3_UNDER > 0 the directions below the horizon see the mirrored sky times CU3_UNDER (a hazy horizon over a dark sea)
    instead of the model's black ground, so the cloud base gets the light from below that a real base gets."""
    skyn = world_sky(sc, strength=1.0, sun_disc=False)
    skyn.sun_elevation = math.radians(CU3_SUN_EL)
    skyn.sun_rotation = math.radians(phi)
    if CU3_UNDER > 0:
        nt = sc.world.node_tree
        bg = next(n for n in nt.nodes if n.type == "BACKGROUND")
        tc = nt.nodes.new("ShaderNodeTexCoord")
        sep = nt.nodes.new("ShaderNodeSeparateXYZ")
        ab = nt.nodes.new("ShaderNodeMath")
        ab.operation = "ABSOLUTE"
        com = nt.nodes.new("ShaderNodeCombineXYZ")
        lt = nt.nodes.new("ShaderNodeMath")
        lt.operation = "LESS_THAN"
        lt.inputs[1].default_value = 0.0
        fac = nt.nodes.new("ShaderNodeMath")
        fac.operation = "MULTIPLY_ADD"
        fac.inputs[1].default_value = CU3_UNDER - 1.0
        fac.inputs[2].default_value = 1.0
        sca = nt.nodes.new("ShaderNodeVectorMath")
        sca.operation = "SCALE"
        L = nt.links
        L.new(tc.outputs["Generated"], sep.inputs[0])
        L.new(sep.outputs["X"], com.inputs["X"])
        L.new(sep.outputs["Y"], com.inputs["Y"])
        L.new(sep.outputs["Z"], ab.inputs[0])
        L.new(ab.outputs[0], com.inputs["Z"])
        L.new(com.outputs[0], skyn.inputs["Vector"])
        L.new(sep.outputs["Z"], lt.inputs[0])
        L.new(lt.outputs[0], fac.inputs[0])
        L.new(skyn.outputs["Color"], sca.inputs["Vector"])
        L.new(fac.outputs[0], sca.inputs["Scale"])
        L.new(sca.outputs["Vector"], bg.inputs["Color"])
    return skyn


def cu3_load_vdb(path, res):
    g = vdb.read(path, "density")
    vox = CU_W / res
    nx, ny, nz = res, int(round(CU_D / vox)), int(round(CU_H / vox))
    dens = np.zeros((nx, ny, nz), np.float32)
    g.copyToArray(dens)
    return dens, vox


def stage_cumulus3():
    os.makedirs(CU3_RAW, exist_ok=True)
    mat = cu3_material()
    w, h = int(round(1024 * CU3_SCALE)), int(round(512 * CU3_SCALE))
    t_all = time.time()
    y_c, z_c, pitch = cu3_cam_params()
    for i in CU3_CELLS:
        kind = CU_KINDS[i]
        p = cu3_paths(i)
        t0 = time.time()
        extra = {}
        if CU3_SHAPES == 1:
            dens, vox, xs, zs, f, n_holes, extra = cu3_density(kind, SEED * 900 + i * 53 + CU3_SEED * 7919)
            write_vdb(p["vdb"], dens, (CU_W / 2, CU_D / 2, CU_H), vox)
        elif CU3_SHAPES == 2:
            prev = np.load(p["tau"])
            res = int(prev["res"]) if "res" in prev else CU_RES
            dens, vox = cu3_load_vdb(p["vdb"], res)
            f, n_holes = float(prev["fit"]), int(prev["holes"])
            extra = {k: prev[k] for k in prev.files if k not in ("tau", "xs", "zs", "fit", "holes", "res", "tau_img")}
        else:
            src = os.path.join(RAW, "cumulus_%d.vdb" % i)
            dens, vox = cu3_load_vdb(src, CU_RES)
            import shutil
            shutil.copyfile(src, p["vdb"])
            f, n_holes = 1.0, -1
        nx, ny, nz = dens.shape
        xs = ((np.arange(nx) + 0.5) * vox - CU_W / 2).astype(np.float32)
        ys = ((np.arange(ny) + 0.5) * vox - CU_D / 2).astype(np.float32)
        zs = ((np.arange(nz) + 0.5) * vox).astype(np.float32)
        # view-axis optical depth along the card camera's (pitched) rays, at the texture resolution
        tau_img = cu3_project(dens, vox, xs, ys, zs, 1024, 512, CU3_DENS)
        np.savez(p["tau"], tau=dens.sum(axis=1) * vox * CU3_DENS, xs=xs, zs=zs, fit=f, holes=n_holes, res=nx,
                 tau_img=tau_img, **extra)
        occ = dens > 0.01
        ix, iy, iz = np.nonzero(occ)
        pts_x = xs[ix]
        pts_y = ys[iy]
        z_top = float(zs[iz].max())
        del dens, occ
        if not CU3_RENDER:
            log("cumulus3 card %d (%s) shape only %.1fs" % (i, kind, time.time() - t0))
            continue
        sc = new_scene("Cumulus3_%d" % i)
        add_volume(sc, p["vdb"], "Cumulus3_%d" % i, mat)
        cd = bpy.data.cameras.new("Cu3Cam%d" % i)
        cd.type = "ORTHO"
        cd.ortho_scale = CU_ORTHO
        cd.clip_start, cd.clip_end = 1.0, 20000.0
        cam = bpy.data.objects.new("Cu3Cam%d" % i, cd)
        sc.collection.objects.link(cam)
        cam.rotation_euler = (math.radians(90) + pitch, 0, 0)
        cam.location = (0, y_c, z_c)
        sc.camera = cam
        for name, phi in list(CU3_PASSES) + [("sky", CU3_SKY_PHI)]:
            if name not in CU3_DO:
                continue
            for ob in [o for o in sc.collection.objects if o.name.startswith(("Cu3Sun", "Cu3Haze"))]:
                bpy.data.objects.remove(ob, do_unlink=True)
            sd = cu3_sun_dir(phi)
            if name == "sky":
                cu3_world_sky(sc, phi)
                sun_ob = None
            else:
                world_color(sc, (0.0, 0.0, 0.0), 0.0)
                sun_ob = add_sun(sc, sd, CU3_SUN)
                sun_ob.name = "Cu3Sun"
                if CU3_HAZE_T < 0.999:
                    hdir = Vector((sd.x, sd.y)).normalized()
                    reach = float((pts_x * hdir.x + pts_y * hdir.y).max())
                    zt = CU_BASE_Z + CU3_HAZE_TOP * (z_top - CU_BASE_Z)
                    sigma = math.log(1.0 / CU3_HAZE_T) * math.sin(math.radians(CU3_SUN_EL)) / max(50.0, zt - CU_BASE_Z)
                    cu3_haze_block(sc, sd, reach, zt, sigma)
            vl = sc.view_layers[0]
            for n in ("sun", "sky"):
                if n not in [g.name for g in vl.lightgroups]:
                    vl.lightgroups.add(name=n)
            if sun_ob is not None:
                sun_ob.lightgroup = "sun"
            sc.world.lightgroup = "sky"
            vl.use_pass_environment = True
            spp = CU3_SKY_SAMPLES if (name == "sky" and CU3_SKY_SAMPLES > 0) else CU3_SAMPLES
            setup_render(sc, w, h, spp, p[name])
            sc.cycles.adaptive_threshold = CU3_ADAPT
            b = CU3_BACK_BOUNCES if (name == "back" and CU3_BACK_BOUNCES > 0) else CU3_BOUNCES
            sc.cycles.volume_bounces = b
            sc.cycles.max_bounces = b
            sc.cycles.volume_biased = bool(CU3_BIASED)
            if CU3_DENOISE:
                # every render here has a single light (one sun lamp, or the sky), so its Combined pass is that light's
                # contribution: OpenImageDenoise on Combined (albedo-guided), the noisy original kept as "Noisy Image"
                sc.cycles.use_denoising = True
                try:
                    sc.cycles.denoiser = "OPENIMAGEDENOISE"
                except Exception as e:  # noqa: BLE001
                    log("denoiser not set:", e)
                sc.cycles.denoising_input_passes = "RGB_ALBEDO"
                sc.cycles.denoising_prefilter = "ACCURATE"
                try:
                    sc.cycles.denoising_quality = "HIGH"
                except Exception:  # noqa: BLE001
                    pass
                vl.cycles.denoising_store_passes = True
            render(sc)
        log("cumulus3 card %d (%s) total %.1fs" % (i, kind, time.time() - t0))
    log("cumulus3 cards total %.1fs" % (time.time() - t_all))
    if CU3_RENDER:
        cu3_sun_sky_balance()
    if CU3_RENDER and all(os.path.exists(cu3_paths(i)[n]) for i in range(len(CU_KINDS)) for n in ("back", "side", "front", "sky")):
        stage_cumulus3compose()


def cu3_read_png(path):
    cfg = oiio.ImageSpec()
    cfg.attribute("oiio:UnassociatedAlpha", 1)
    buf = oiio.ImageBuf(path, 0, 0, cfg)
    s = buf.spec()
    return np.asarray(buf.get_pixels(oiio.UINT8)).reshape(s.height, s.width, s.nchannels)


def cu3_read_exr(path):
    buf = oiio.ImageBuf(path)
    s = buf.spec()
    return np.asarray(buf.get_pixels(oiio.FLOAT)).reshape(s.height, s.width, s.nchannels)


def cu3_upsample2(img, h, w):
    """Bilinear 2x upsample of img [h2, w2, c] to [h, w, c] (pixel centres aligned, edges clamped)."""
    def axis(a, n_out, ax):
        n_in = a.shape[ax]
        pos = (np.arange(n_out) + 0.5) / 2.0 - 0.5
        fl = np.floor(pos)
        i0 = np.clip(fl.astype(np.int64), 0, n_in - 1)
        i1 = np.clip(fl.astype(np.int64) + 1, 0, n_in - 1)
        f = (pos - fl).astype(np.float32)
        shape = [1] * a.ndim
        shape[ax] = n_out
        f = f.reshape(shape)
        return np.take(a, i0, axis=ax) * (1 - f) + np.take(a, i1, axis=ax) * f
    return axis(axis(img, h, 0), w, 1)


def cu3_pull_push(values, weight, levels=12):
    """Fill texels of weight ~0 from their neighbourhood: premultiplied pyramid down (box), then pushed up with bilinear
    upsampling (pull_push_fill pushes nearest-neighbour, which leaves constant rectangles in the filled texels)."""
    w = np.clip(weight, 0, 1).astype(np.float32)
    v = values.astype(np.float32) * w[..., None]
    pyr = [(v, w)]
    for _ in range(levels):
        v, w = pyr[-1]
        h, ww = w.shape
        if h < 2 or ww < 2:
            break
        h2, w2 = h // 2 * 2, ww // 2 * 2
        v = v[:h2, :w2]
        w = w[:h2, :w2]
        v = (v[0::2, 0::2] + v[1::2, 0::2] + v[0::2, 1::2] + v[1::2, 1::2]) * 0.25
        w = (w[0::2, 0::2] + w[1::2, 0::2] + w[0::2, 1::2] + w[1::2, 1::2]) * 0.25
        pyr.append((v, w))
    fv, fw = pyr[-1]
    fill = fv / np.maximum(fw, 1e-8)[..., None]
    for v, w in reversed(pyr[:-1]):
        h, ww = w.shape
        up = cu3_upsample2(fill, h, ww)
        a = np.clip(w * 4.0, 0, 1)[..., None]
        own = v / np.maximum(w, 1e-8)[..., None]
        fill = own * a + up * (1 - a)
    return fill


def cu3_resize(img, h, w):
    """Area-average (down) or bilinear (up) resize of a 2D image to h x w."""
    H, W = img.shape
    if (H, W) == (h, w):
        return img
    if H % h == 0 and W % w == 0:
        return img.reshape(h, H // h, w, W // w).mean(axis=(1, 3))
    ys = np.clip((np.arange(h) + 0.5) * H / h - 0.5, 0, H - 1)
    xs = np.clip((np.arange(w) + 0.5) * W / w - 0.5, 0, W - 1)
    t = np.stack([np.interp(xs, np.arange(W), img[r]) for r in range(H)], 0)
    return np.stack([np.interp(ys, np.arange(H), t[:, c]) for c in range(w)], 1).astype(np.float32)


def cu3_sun_scale(sky_factor):
    """The common scale of the EXR's sun channels. 'phys': the scale that gives the runtime tints this sky model's own
    sun/sky balance at the card sun elevation (probe raw/cumulus3_sunsky.json): with SunTint and AmbTint as specified, the
    sun term then weighs against the sky term as the direct sun's irradiance weighs against the sky's. A number: that
    scale. Returns (scale, info)."""
    sun_tint = srgb_decode_hex("FFB88C") * CU3_SUNTINT
    amb_tint = srgb_decode_hex("8784BA") * CU3_AMBTINT
    ratio_rt = float(lum(sun_tint)) / float(lum(amb_tint))
    info = {"sun_over_sky_weight_runtime_tints": ratio_rt}
    path = os.path.join(CU3_RAW, "cumulus3_sunsky.json")
    if os.path.exists(path):
        with open(path) as fh:
            ss = json.load(fh)
        ratio_phys = ss["direct_sun_irradiance_lum"] * sky_factor / CU3_SUN
        info.update({"probe": ss, "sun_over_sky_weight_physical_in_texture_units": ratio_phys,
                     "scale_for_physical_balance": ratio_phys / ratio_rt})
    if CU3_SUN_SCALE == "phys":
        if "scale_for_physical_balance" not in info:
            raise RuntimeError("sun scale 'phys' needs %s (run stage cumulus3 with rendering once)" % path)
        return float(info["scale_for_physical_balance"]), info
    return float(CU3_SUN_SCALE), info


def stage_cumulus3compose():
    t0 = time.time()
    names = [n for n, _ in CU3_PASSES] + ["sky"]
    cells = []
    for i in range(len(CU_KINDS)):
        p = cu3_paths(i)
        alphas, prem, noisy = [], [], []
        for n in names:
            d = read_exr_all(p[n])
            alphas.append(np.clip(ch(d, "Combined", "A"), 0, 1))
            # every render has one light (a sun lamp, or the sky with a transparent film), so Combined is that light's
            # contribution, premultiplied (denoised with OpenImageDenoise when the render had it on)
            prem.append(np.maximum(lum(np.stack([ch(d, "Combined", c) for c in "RGB"], -1)), 0))
            try:
                noisy.append(np.maximum(lum(np.stack([ch(d, "Noisy Image", c) for c in "RGB"], -1)), 0))
            except KeyError:
                noisy.append(prem[-1])
        alpha = np.mean(alphas, axis=0).astype(np.float32)
        a_spread = float(np.max([np.abs(a - alpha).max() for a in alphas]))
        chh, cw = alpha.shape
        prem = np.stack(prem, -1)
        # straight (per unit opacity) radiance; texels with almost no opacity take the alpha-weighted (premultiplied)
        # pull-push average of their neighbourhood, so the mips of the straight texture stay close to the alpha-weighted ones
        vals = prem / np.maximum(alpha, 1e-4)[..., None]
        fill = cu3_pull_push(vals, alpha)
        keep = np.clip((alpha - 0.002) / 0.02, 0, 1)[..., None]
        vals = vals * keep + fill * (1 - keep)
        if CU3_SMOOTH > 0:
            sm = gauss_blur(vals * alpha[..., None], CU3_SMOOTH) / np.maximum(gauss_blur(alpha, CU3_SMOOTH), 1e-4)[..., None]
            ws = np.clip((alpha - 0.1) / 0.2, 0, 1)[..., None]
            vals = vals * (1 - ws) + sm * ws
        # composite grain left: high-frequency residual of the premultiplied passes in the edge band and the body
        def hf(x):
            return x - gauss_blur(x, 1.5)
        band = (alpha > 0.15) & (alpha < 0.8)
        body = alpha > 0.9
        grain = {n: [float(hf(prem[..., k])[band].std()), float(hf(prem[..., k])[body].std()),
                     float(hf(noisy[k])[band].std()), float(hf(noisy[k])[body].std())] for k, n in enumerate(names)}
        tz = np.load(p["tau"])
        tau = cu3_resize(tz["tau_img"].astype(np.float32), chh, cw)
        nz_cols = np.nonzero(alpha.max(axis=0) > 0.002)[0]
        nz_rows = np.nonzero(alpha.max(axis=1) > 0.002)[0]
        margin = int(min(nz_cols.min(), cw - 1 - nz_cols.max(), nz_rows.min(), chh - 1 - nz_rows.max()))
        m = int(round(24 * cw / 1024))
        border = max(float(alpha[:m].max()), float(alpha[-m:].max()), float(alpha[:, :m].max()), float(alpha[:, -m:].max()))
        cols = np.nonzero(alpha.max(axis=0) > 0.02)[0]
        rows = np.nonzero(alpha.max(axis=1) > 0.02)[0]
        bbox = [float(cols.min() / cw), float((cols.max() + 1) / cw), float(rows.min() / chh), float((rows.max() + 1) / chh)]
        extra = {k: (int(tz[k]) if tz[k].dtype.kind in "iu" else float(tz[k])) for k in tz.files
                 if k.startswith("n_") and tz[k].ndim == 0}
        log("cumulus3 cell %d alpha mean %.3f spread %.4f max in %dpx border %.4f, nearest alpha>0.002 %d px from the edge, "
            "bbox u %.3f-%.3f v %.3f-%.3f" % (i, float(alpha.mean()), a_spread, m, border, margin, *bbox))
        cells.append({"alpha": alpha, "vals": vals.astype(np.float32), "tau": tau, "border": border, "bbox": bbox,
                      "margin_px": margin, "fit": float(tz["fit"]) if "fit" in tz else 1.0,
                      "holes": int(tz["holes"]) if "holes" in tz else -1, "alpha_spread": a_spread, "grain": grain,
                      "extra": extra})
    chh, cw = cells[0]["alpha"].shape
    ok = np.concatenate([c["alpha"].ravel() > 0.15 for c in cells])
    sky_all = np.concatenate([c["vals"][..., 3].ravel() for c in cells])[ok]
    sky_factor = 0.95 / float(np.percentile(sky_all, 99.9))
    t_all = np.concatenate([c["tau"].ravel() for c in cells])[np.concatenate([c["alpha"].ravel() > 0.5 for c in cells])]
    tau_ref = float(np.percentile(t_all, 98) / 3.0)
    # FRONT sunlit body (the lamp calibration): opaque texels in the upper 60 % of the cloud's height
    body = []
    for c in cells:
        rows_v = (np.arange(chh) + 0.5) / chh
        v0, v1 = c["bbox"][2], c["bbox"][3]
        upper = (rows_v < v0 + 0.6 * (v1 - v0))[:, None] & (c["alpha"] > 0.95)
        body.append(c["vals"][..., 2][upper])
    front_body_median = float(np.median(np.concatenate(body)))
    sun_scale, sun_info = cu3_sun_scale(sky_factor)
    log("sky_factor %.4g, FRONT body median as rendered %.3f, sun scale %.4f" % (sky_factor, front_body_median, sun_scale))
    # atlas
    sun_atlas = np.zeros((4 * chh, 2 * cw, 3), np.float32)
    aux = np.zeros((4 * chh, 2 * cw, 4), np.float32)
    ramp = np.minimum(np.clip(np.arange(cw) / 8.0, 0, 1), np.clip((cw - 1 - np.arange(cw)) / 8.0, 0, 1))
    rampv = np.minimum(np.clip(np.arange(chh) / 8.0, 0, 1), np.clip((chh - 1 - np.arange(chh)) / 8.0, 0, 1))
    dither = np.random.default_rng(12345)
    for i, c in enumerate(cells):
        r0, c0 = (i // 2) * chh, (i % 2) * cw
        a = c["alpha"] * ramp[None, :] * rampv[:, None]
        sun_atlas[r0:r0 + chh, c0:c0 + cw] = c["vals"][..., :3] * sun_scale
        # sky: 8-bit linear; a +-1 LSB triangular dither keeps smooth dark bodies from contouring after quantisation
        tri = (dither.random((chh, cw)) + dither.random((chh, cw)) - 1.0) / 255.0
        aux[r0:r0 + chh, c0:c0 + cw, 0] = np.clip(c["vals"][..., 3] * sky_factor + tri, 0, 1)
        aux[r0:r0 + chh, c0:c0 + cw, 1] = a
        aux[r0:r0 + chh, c0:c0 + cw, 2] = np.clip(1.0 - np.exp(-gauss_blur(c["tau"], 1.5) / tau_ref), 0, 1)
        aux[r0:r0 + chh, c0:c0 + cw, 3] = 1.0
    os.makedirs(CU3_OUT, exist_ok=True)
    sun_path = os.path.join(CU3_OUT, "T_BSky_%s_CumulusSun.exr" % PRESET)
    aux_path = os.path.join(CU3_OUT, "T_BSky_%s_CumulusAux.png" % PRESET)
    oiio_write(sun_path, sun_atlas, "half", attrs=[("compression", "zip")], names=("R", "G", "B"))
    write_png(aux_path, aux)
    # round trips: straight alpha (no premultiplication) in the PNG, half precision in the EXR
    aux8 = cu3_read_png(aux_path)
    want8 = np.round(np.clip(aux, 0, 1) * 255).astype(np.uint8)
    rt_png = [int(np.abs(aux8[..., k].astype(int) - want8[..., k]).max()) for k in range(4)]
    sun_rt = cu3_read_exr(sun_path)
    rt_exr = float(np.max(np.abs(sun_rt - sun_atlas) / np.maximum(np.abs(sun_atlas), 1e-3)))
    log("round trip png max |diff| per channel %s, exr max rel diff %.2e" % (rt_png, rt_exr))

    # ---- look sheet: the runtime card formula over the preview sky colours, read back from the written textures
    sun_tint = srgb_decode_hex("FFB88C") * CU3_SUNTINT
    amb_tint = srgb_decode_hex("8784BA") * CU3_AMBTINT
    sky_c, glow_c = srgb_decode_hex("8F94BA"), srgb_decode_hex("E89A7A")
    skyL_t = aux8[..., 0].astype(np.float32) / 255.0
    alpha_t = aux8[..., 1].astype(np.float32) / 255.0
    front_col = sun_rt[..., 2:3] * sun_tint + skyL_t[..., None] * amb_tint
    opaque = alpha_t > 0.95
    target = 1.3 * float(lum(sky_c))                  # the reference's lit cumulus are about 1.3x the sky behind them
    exposure = CU3_EXPOSURE if CU3_EXPOSURE > 0 else round(target / float(np.median(lum(front_col)[opaque])), 3)
    gap = 6
    sheet = np.full((8 * chh + 9 * gap, 3 * cw + 4 * gap, 3), 0.02, np.float32)
    vv = ((np.arange(chh) + 0.5) / chh)[:, None, None]
    stats = {}
    for i in range(len(cells)):
        r0, c0 = (i // 2) * chh, (i % 2) * cw
        sl = (slice(r0, r0 + chh), slice(c0, c0 + cw))
        for k, (n, phi) in enumerate(CU3_PASSES):
            col = exposure * (sun_rt[sl][..., k:k + 1] * sun_tint + skyL_t[sl][..., None] * amb_tint)
            if n == "back":
                tmix = np.clip((vv - 0.35) / 0.65, 0, 1)
                bg = sky_c * (1 - tmix) + glow_c * tmix
            else:
                bg = np.broadcast_to(sky_c, (chh, 1, 3))
            a = alpha_t[sl][..., None]
            img = bg * (1 - a) + col * a
            y0, x0 = gap + i * (chh + gap), gap + k * (cw + gap)
            sheet[y0:y0 + chh, x0:x0 + cw] = img
            stats["%d_%s" % (i, n)] = float(np.median(lum(col)[alpha_t[sl] > 0.95])) if (alpha_t[sl] > 0.95).any() else 0.0
    look_rgba = np.concatenate([srgb_encode(sheet), np.ones_like(sheet[..., :1])], -1)
    look_ref = os.path.join(REF, "cumulus3_look.png") if CU3_OUT == OUT else os.path.join(CU3_OUT, "cumulus3_look.png")
    write_png(look_ref, look_rgba)
    if CU3_LOOK:
        write_png(CU3_LOOK, look_rgba)
    log("look sheet %s (exposure %.3f) body lum per cell/pass %s" % (look_ref, exposure, json.dumps({k: round(v, 3) for k, v in stats.items()})))

    y_c, z_c, pitch = cu3_cam_params()
    meta = {
        "textures": {"sun": os.path.basename(sun_path), "aux": os.path.basename(aux_path)},
        "layout": "2 columns x 4 rows of %d x %d texels; cell i at column i %% 2, row i // 2, row 0 at the top" % (cw, chh),
        "cell_world": "cell width = %.0f m of the bake frame (ortho camera), height = half of that; u to the right, v down. "
                      "Shapes are built for how bsky_layout.py draws a card: %.1f-%.1f km wide (2.2-5.2x the bake frame), so "
                      "a cell holds many thermals and dome generations rather than one 2 km cumulus" % (CU_ORTHO, 4.8, 11.52),
        "intended_card_width_m": [4800.0, 11520.0],
        "sun_exr": {"R": "BACK phi 20", "G": "SIDE phi 90", "B": "FRONT phi 160",
                    "meaning": "sun-lit luminance per unit opacity (straight, divided by alpha, dilated into empty texels), "
                               "white sun lamp, linear, half float, one common scale (sun_scale) for all three channels, not "
                               "clamped",
                    "post": "Combined pass denoised by OpenImageDenoise at render time (albedo-guided; each render holds one "
                            "light); texels with alpha < ~0.02 hold the alpha-weighted pull-push average of their "
                            "neighbourhood, pushed with bilinear upsampling%s" % (
                                "; extra alpha-weighted gaussian sigma %.1f px where alpha > 0.1" % CU3_SMOOTH if CU3_SMOOTH > 0 else ""),
                    "sun_strength_w_m2": CU3_SUN, "front_sunlit_body_median_as_rendered": front_body_median,
                    "sun_scale": sun_scale, "front_sunlit_body_median_stored": front_body_median * sun_scale,
                    "sun_scale_note": "the lamp is set so the FRONT sunlit body renders at about 1.0; the stored channels are "
                                      "the rendered values times sun_scale, one scale for all three, chosen so SunTint and "
                                      "AmbTint as specified weigh sun and sky as this sky model's direct sun and sky do",
                    "sun_scale_basis": sun_info},
        "aux_png": {"R": "sky-lit luminance per unit opacity x sky_factor (linear; 0.95 = 99.9th percentile over alpha > 0.15), "
                         "+-1 LSB triangular dither before 8-bit quantisation",
                    "G": "opacity (linear, straight alpha)", "B": "thickness 1 - exp(-tau / tau_ref), tau = optical depth "
                    "along the card camera's (pitched) rays", "A": "255", "sky_factor": sky_factor, "tau_ref": tau_ref},
        "passes": {n: {"phi_deg": phi, "sun_dir_card": list(cu3_sun_dir(phi))} for n, phi in CU3_PASSES},
        "sky_pass": {"world": "MULTIPLE_SCATTERING sky, no sun disc, strength 1, its sun at elevation %.1f deg and phi %.0f (+u "
                              "side)" % (CU3_SUN_EL, CU3_SKY_PHI),
                     "below_horizon": ("mirrored sky x %.2f (sea and haze under the cloud base)" % CU3_UNDER) if CU3_UNDER > 0
                     else "model (black)", "environment_excluded": "transparent film: Combined holds the cloud's light only"},
        "phi_definition": "horizontal angle between the camera view direction and the sun direction; 0 = sun straight behind "
                          "the card, 180 = sun straight behind the camera",
        "lit_side_in_side_pass": "+u (right). Every pass has the sun on the +u side; mirror u when the sun is on the card's other side",
        "sun_elevation_deg": CU3_SUN_EL,
        "camera": {"type": "orthographic, %.0f m wide, looking along +Y pitched up %.1f deg (the card seen from below its base, "
                           "as the game sees cards 3-12 deg up)" % (CU_ORTHO, CU3_PITCH), "location_m": [0.0, y_c, z_c]},
        "cloud_base_v": CU3_BASE_V,
        "cloud_base_note": "v where the cloud base plane (%.0f m) crosses the card plane (the cloud's depth centre). The pitched "
                           "view shows the base from below as a band: near edge above this v, far edge below it (about +-%.3f "
                           "for a cloud 1 km deep). The runtime places atlas v 0.85 at the instance height (Ht * 0.35); for "
                           "this bake use Ht * %.3f" % (CU_BASE_Z, 500.0 * math.sin(pitch) / (CU_ORTHO / 2.0), CU3_BASE_V - 0.5),
        "cells": [{"index": i, "kind": CU_KINDS[i], "fit_factor": c["fit"], "bbox_u0_u1_v0_v1": c["bbox"],
                   "nearest_alpha_px_from_cell_edge": c["margin_px"], "max_alpha_in_24px_border": c["border"],
                   "filled_hole_px": c["holes"], **c["extra"]} for i, c in enumerate(cells)],
        "medium": {"density_per_m": CU3_DENS, "phase": CU3_PHASE,
                   **({"anisotropy": CU3_ANISO} if CU3_PHASE == "hg" else
                      {"dual_w_g1_g2": list(CU3_DUAL)} if CU3_PHASE == "dual" else {"mie_diameter_um": CU3_DIAM}),
                   "volume_bounces": CU3_BOUNCES, "samples": CU3_SAMPLES, "sky_samples": CU3_SKY_SAMPLES or CU3_SAMPLES,
                   "denoiser": "OpenImageDenoise (albedo)" if CU3_DENOISE else "none",
                   "voxels_across_2km": CU3_RES, "haze_block_base_transmission": CU3_HAZE_T,
                   "haze_block_top_fraction": CU3_HAZE_TOP,
                   "notes": ["512 bounces: multiple scattering converges there (1024 identical); 16 bounces left BACK bodies "
                             "at about 12 % of their converged sun light",
                             "extinction 0.10/m (round 1: 0.07): crisper domes at the 2.2-5x magnification the cards are drawn at; "
                             "0.07, 0.095 and 0.12 were compared",
                             "HG 0.6 kept: the dual lobe 0.7 HG(0.8) + 0.3 HG(-0.2) darkened BACK bodies by about 15 % and turned "
                             "them bluer (B/R 1.08 -> 1.18 on cell 6) for under 2 % change in FRONT"]},
        "runtime": {"formula": "rgb = Exposure * (sunL(phi) * SunTint + skyL * AmbTint); alpha = aux.G",
                    "sunL_phi_as_built": "phi <= 20: R; 20..90: lerp(R, G, (phi - 20) / 70); 90..160: lerp(G, B, (phi - 90) / 70); "
                                         ">= 160: B (M_SemiCelCloud today)",
                    "sunL_phi_recommended": "20..90: lerp(R, G, 1 - (1 - t)^2), t = (phi - 20) / 70, so the BACK rim fades as fast "
                                            "as forward scattering does (at phi 40 the rim keeps 51 % instead of 71 %); 90..160 "
                                            "linear. In M_SemiCelCloud: wB = 1 - saturate((phi - 20) / 70); wB = wB * wB",
                    "SunTint": "linear(#FFB88C) * %.2f" % CU3_SUNTINT, "AmbTint": "linear(#8784BA) * %.2f" % CU3_AMBTINT,
                    "Exposure_recommended": exposure,
                    "Exposure_basis": "median front-lit opaque cloud luminance = 1.3 x the #8F94BA preview sky (the reference's ratio)",
                    "CumulusSkyNorm": 1.0,
                    "notes": ["set CardExposure to Exposure_recommended (round 1 shipped 0.58 for unscaled sun channels)",
                              "keep CumulusSkyNorm 1.0: the physical sun/sky balance is baked into sun_scale",
                              "M_SemiCelCloud also lerps toward HazeColor by Haze * (1 - exp(-dist / HazeDistance)) * 0.85, "
                              "which this formula and the look sheet leave out; it moves cards 15 km and further toward pink"],
                    "skyL": "aux.R as stored (already scaled by sky_factor)",
                    "mirror": "sample u' = 1 - u inside the cell when the sun is on the card's -u side"},
        "body_luminance_median_per_cell_pass": stats,
        "grain_hf_std_premult_band_body_denoised_vs_noisy": {str(i): c["grain"] for i, c in enumerate(cells)},
        "round_trip": {"png_max_abs_diff_per_channel": rt_png, "exr_half_max_rel_diff": rt_exr},
    }
    with open(os.path.join(CU3_OUT, "cumulus3_meta.json"), "w") as fh:
        json.dump(meta, fh, indent=2)
    log("cumulus3 compose done in %.1fs: sky_factor %.4g tau_ref %.3g front body median %.3f sun scale %.4f exposure %.3f" % (
        time.time() - t0, sky_factor, tau_ref, front_body_median, sun_scale, exposure))


# ------------------------------------------------------------------------------------------------ main
log("preset", PRESET, "az_ue", AZ_UE, "el", EL, "stages", STAGES, "quality", QUALITY)
for st in ("atmo", "bank", "atlas", "hills", "ref", "compose", "cumulus", "cumuluscompose", "cumulus3", "cumulus3compose"):
    if st in STAGES:
        log("=== stage", st)
        globals()["stage_" + st]()
log("all done")
