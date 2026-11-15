"""Level 1 sky clipping, step 2: prove which thing is responsible.

At recorded chase-camera poses on the banked turns, film the same frame with one thing changed at a time:
  final      the shipped frame (grade and cel pass on)
  base       GBuffer base colour: opaque geometry shows its colour, anything that is only sky is black
  nodome     the sky's SkyDome hidden          (is the dark area drawn by the dome?)
  nohills    HorizonHills hidden
  noclouds   cloud cards, SunGlow, FlareGhosts hidden
  nosky      the whole BayshoreSky instance hidden
  dye        a bright magenta floor far below everything: every pixel that turns magenta has NO opaque
             ground between the camera and the floor
  noground   the track's land hidden (+ dye): what the land covers today
  top / topwide   orthographic views straight down with the dye floor: the land's real footprint

Run: jam_run.ps1 -Script 21_sky_clip_isolate.py   [env JAM_TAG=clip_before]   Output: HandBack/jam/<tag>/
"""
import os
import sys
import math
import unreal

sys.path.insert(0, r"K:\Full Throttle\SkyScratch\SourceArt\Jam\unreal")
import jam_cam as J

TAG = os.environ.get("JAM_TAG", "clip_before")
POSES = [float(x) for x in os.environ.get("JAM_POSES", "1180,3880,11180,640").split(",")]  # metres along the road
VARIANTS = os.environ.get("JAM_VARIANTS", "final,nodome,nohills,noclouds,nosky,dye,noground,top,topwide").split(",")

try:
    if os.environ.get("JAM_BUILD") == "1":  # (re)build the world materials first
        import importlib
        import ft_world
        importlib.reload(ft_world)
        ft_world.build_all()
    tr = J.load(0)
    W = J.world()
    GS = unreal.GameplayStatics
    try:
        J.log("floor z", tr.tool_floor_z(), "length m", round(tr.tool_length() / 100.0))
    except Exception as e:  # an older binary
        J.log("no floor in this build:", e)
    # everything in the world, including what the BayshoreSky Level Instance streamed in
    sky_comps = {}
    everyone = list(GS.get_all_actors_of_class(W, unreal.Actor))
    J.log("world actors", [(a.get_name(), a.get_class().get_name()) for a in everyone])
    for a in everyone:
        cn = a.get_class().get_name()
        if cn.startswith("BP_BayshoreSky"):
            comps = a.get_components_by_class(unreal.SceneComponent)
            J.log("sky actor", a.get_name(), cn, [c.get_name() for c in comps])
            for c in comps:
                sky_comps.setdefault(c.get_name(), []).append(c)
    chunks = list(tr.get_components_by_class(unreal.ProceduralMeshComponent))
    J.log("track chunks", len(chunks))

    # the dye floor
    floor = J.EAS.spawn_actor_from_class(unreal.StaticMeshActor, unreal.Vector(150000.0, 600000.0, -9000.0), unreal.Rotator(0, 0, 0))
    smc = floor.get_component_by_class(unreal.StaticMeshComponent)
    smc.set_static_mesh(unreal.load_asset("/Engine/BasicShapes/Plane"))
    floor.set_actor_scale3d(unreal.Vector(60000.0, 60000.0, 1.0))
    mid = unreal.MaterialLibrary.create_dynamic_material_instance(W, unreal.load_asset("/Game/FX/M_Tracer"))
    mid.set_vector_parameter_value("Color", unreal.LinearColor(1.0, 0.0, 0.6, 1.0))
    smc.set_material(0, mid)
    smc.set_cast_shadow(False)

    def vis(names, on):
        for n, cs in sky_comps.items():
            if any(k.lower() in n.lower() for k in names):
                for c in cs:
                    c.set_visibility(on, True)

    def state(dome=True, hills=True, clouds=True, sky=True, dye=False, ground=True):
        def f():
            for n, cs in sky_comps.items():
                for c in cs:
                    c.set_visibility(sky, False)
            if sky:
                vis(["SkyDome"], dome)
                vis(["Hills"], hills)
                vis(["Cloud", "SunGlow", "Flare"], clouds)
            floor.set_is_temporarily_hidden_in_editor(not dye)
            smc.set_visibility(dye, True)
            for c in chunks:
                c.set_mesh_section_visible(6, ground)
        return f

    V = {"final": state(), "nodome": state(dome=False), "nohills": state(hills=False), "noclouds": state(clouds=False),
         "nosky": state(sky=False), "dye": state(dye=True), "noground": state(dye=True, ground=False),
         "top": state(dye=True), "topwide": state(dye=True)}
    shots = []
    for d in POSES:
        pos, fwd, right, up = J.frame(tr, d * 100.0)
        flags = tr.tool_flags(d * 100.0)
        loc, rot, fov = J.chase(tr, d * 100.0, loop=bool(flags & 3))
        J.log("POSE d=%.0f m  bank=%.2f rad  road=(%.0f, %.0f, %.0f)  cam loc=(%.0f, %.0f, %.0f) rot=(pitch %.2f, yaw %.2f, roll %.2f) fov=%.1f"
              % (d, tr.tool_bank(d * 100.0), pos.x, pos.y, pos.z, loc.x, loc.y, loc.z, rot.pitch, rot.yaw, rot.roll, fov))
        yaw = math.degrees(math.atan2(fwd.y, fwd.x))
        for v in VARIANTS:
            if v == "top":
                shots.append(("d%05d_%s" % (d, v), unreal.Vector(pos.x, pos.y, pos.z + 40000.0), unreal.Rotator(roll=0.0, pitch=-90.0, yaw=yaw), -14000.0, V[v]))
            elif v == "topwide":
                shots.append(("d%05d_%s" % (d, v), unreal.Vector(pos.x, pos.y, pos.z + 40000.0), unreal.Rotator(roll=0.0, pitch=-90.0, yaw=yaw), -90000.0, V[v]))
            else:
                shots.append(("d%05d_%s" % (d, v), loc, rot, fov, V[v]))

    def base_pass():
        b = [("%s_base" % n.rsplit("_", 1)[0], l, r, f, state()) for (n, l, r, f, s) in shots if n.endswith("_final")]
        b += [("%s_basedye" % n.rsplit("_", 1)[0], l, r, f, state(dye=True)) for (n, l, r, f, s) in shots if n.endswith("_final")]
        J.Shooter(TAG, b, warm=8, frames=6, size=(1720, 720), grade=False, source=unreal.SceneCaptureSource.SCS_BASE_COLOR).start()

    J.Shooter(TAG, shots, done=base_pass, warm=40, frames=12, size=(1720, 720)).start()
except Exception as e:  # noqa
    import traceback
    J.log("ERR", repr(e), traceback.format_exc())
    unreal.SystemLibrary.quit_editor()
