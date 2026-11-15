"""Look-dev cameras for the buildings pass: fixed views of real hotel instances in the port's Level 1 (L_Bayshore),
filmed in the off-screen editor with a SceneCapture2D that carries the level's own grade and cel pass (the Grade80s
volume's settings are copied onto the capture, because a capture does not take a volume's post-process materials).

These are for fast iterations on one building. The judge is still the real game: play_uncooked.ps1 / play_port.ps1.
Output: SkyScratch/HandBack/buildings/<tag>/<shot>.png
"""
import os
import math
import unreal

OUT_ROOT = r"K:\Full Throttle\SkyScratch\HandBack\buildings"
MAP = "/Game/Maps/Bayshore/L_Bayshore"
PROPS = "/Game/Maps/Bayshore/Props/"
W, H = 1920, 1080
LES = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
EAS = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
# (width, depth, floors) as ft_export.bayshore() builds them; height = floors * 320 + 120
DIMS = {"SM_HotelPink": (1800, 1200, 7), "SM_HotelMint": (1500, 1000, 5), "SM_HotelLav": (2200, 1200, 9), "SM_HotelPeach": (1400, 900, 4)}
SUN = (0.8613, 0.4973)      # MPC_BayshoreSky.SunDirection, toward the sun


def log(*a):
    print("BLD", *[str(x) for x in a], flush=True)


def actor(label):
    for a in EAS.get_all_level_actors():
        if a.get_actor_label() == label:
            return a
    return None


def instances(mesh):
    """[(index, location, yaw, scale)] of one prop mesh on the track."""
    track = [a for a in EAS.get_all_level_actors() if a.get_actor_label().startswith("Track_")][0]
    out = []
    for c in track.get_components_by_class(unreal.HierarchicalInstancedStaticMeshComponent):
        sm = c.get_editor_property("static_mesh")
        if sm and sm.get_name() == mesh:
            for i in range(c.get_instance_count()):
                t = c.get_instance_transform(i, True)
                out.append((i, t.translation, t.rotation.rotator().yaw, t.scale3d.x))
    return out


def front_to_sun(yaw):
    """How squarely the hotel's front (-X) faces the sun: 1 = full sun, -1 = sun behind it."""
    y = math.radians(yaw)
    return -(math.cos(y) * SUN[0] + math.sin(y) * SUN[1])


def heroes():
    """The buildings to film: near-row hotels (scale exactly 1), by how the sun meets their front."""
    out = {}
    pink = [x for x in instances("SM_HotelPink") if abs(x[3] - 1.0) < 1e-3]
    if pink:
        out["pink_shade"] = ("SM_HotelPink", min(pink, key=lambda x: front_to_sun(x[2])))
        out["pink_sun"] = ("SM_HotelPink", max(pink, key=lambda x: front_to_sun(x[2])))
    lav = [x for x in instances("SM_HotelLav") if abs(x[3] - 1.0) < 1e-3]
    if lav:
        out["lav"] = ("SM_HotelLav", min(lav, key=lambda x: abs(front_to_sun(x[2]))))
    # the road mostly runs so that the road-facing side is the sunlit one: film Mint and Peach that way round
    for key, mesh in (("mint", "SM_HotelMint"), ("peach", "SM_HotelPeach")):
        near = [x for x in instances(mesh) if abs(x[3] - 1.0) < 1e-3]
        if near:
            out[key] = (mesh, min(near, key=lambda x: abs(front_to_sun(x[2]) + 0.5)))
    for k, (m, x) in out.items():
        log("hero", k, m, "inst", x[0], "at", [round(v) for v in (x[1].x, x[1].y, x[1].z)], "yaw %.1f" % x[2], "front to sun %.2f" % front_to_sun(x[2]))
    return out


def views(mesh, inst):
    """Camera (location, rotation, fov) per view name for one hotel instance."""
    _, pos, yaw, s = inst
    w, d, floors = DIMS[mesh]
    Ht = (floors * 320 + 120) * s
    w, d = w * s, d * s
    y = math.radians(yaw)
    F, R = (math.cos(y), math.sin(y)), (-math.sin(y), math.cos(y))

    def P(f, r, z):
        return unreal.Vector(pos.x + F[0] * f + R[0] * r, pos.y + F[1] * f + R[1] * r, pos.z + z)

    def look(a, b, fov):
        dx, dy, dz = b.x - a.x, b.y - a.y, b.z - a.z
        return a, unreal.Rotator(roll=0.0, pitch=math.degrees(math.atan2(dz, math.hypot(dx, dy))), yaw=math.degrees(math.atan2(dy, dx))), fov

    return {
        # from the road (the hotel stands 100-155 m to its right), as the car comes up to it and as it passes
        "road_far": look(P(-15000, -12500, 350), P(0, 0, Ht * 0.45), 60.0),
        "road_near": look(P(-5000, -12500, 350), P(d * 0.5, 0, Ht * 0.5), 75.0),
        # the front (the windowed face), and the side that looks at the road
        "front": look(P(-4200, 300, 900), P(0, 100, Ht * 0.48), 62.0),
        "side": look(P(d * 0.5 - 600, -w * 0.5 - 4200, 900), P(d * 0.5, -w * 0.5, Ht * 0.48), 62.0),
        # close on a few windows, off-axis so the rooms' depth shows
        "windows": look(P(-1500, -520, 1000), P(0, -100, 1080), 50.0),
    }


class Shooter(object):
    """Films a list of (name, loc, rot, fov, setup) one after another on the editor's tick, then calls done()."""

    def __init__(self, tag, shots, done=None, warm=90, frames=36):
        self.out = os.path.join(OUT_ROOT, tag)
        os.makedirs(self.out, exist_ok=True)
        self.shots, self.done, self.warm, self.frames = shots, done, warm, frames
        self.world = unreal.EditorLevelLibrary.get_editor_world()
        self.rt = unreal.RenderingLibrary.create_render_target2d(self.world, W, H, unreal.TextureRenderTargetFormat.RTF_RGBA8, unreal.LinearColor(0, 0, 0, 1), False)
        self.cap = EAS.spawn_actor_from_class(unreal.SceneCapture2D, unreal.Vector(0, 0, 0), unreal.Rotator(0, 0, 0))
        cc = self.cap.get_editor_property("capture_component2d")
        cc.set_editor_property("texture_target", self.rt)
        cc.set_editor_property("capture_source", unreal.SceneCaptureSource.SCS_FINAL_COLOR_LDR)
        cc.set_editor_property("capture_every_frame", False)
        cc.set_editor_property("capture_on_movement", False)
        cc.set_editor_property("always_persist_rendering_state", True)
        ppv = actor("Grade80s")
        if ppv is not None:
            cc.set_editor_property("post_process_settings", ppv.get_editor_property("settings"))
            cc.set_editor_property("post_process_blend_weight", 1.0)
        self.cc = cc
        self.i, self.f, self.h = 0, -warm, None

    def start(self):
        unreal.EditorPythonScripting.set_keep_python_script_alive(True)
        self.h = unreal.register_slate_post_tick_callback(self.tick)

    def finish(self):
        unreal.unregister_slate_post_tick_callback(self.h)
        EAS.destroy_actor(self.cap)
        log("shots done ->", self.out)
        if self.done:
            self.done()
        else:
            unreal.SystemLibrary.quit_editor()

    def tick(self, dt):
        try:
            if self.i >= len(self.shots):
                self.finish()
                return
            name, loc, rot, fov, setup = self.shots[self.i]
            if self.f == -self.warm or self.f == 0:
                if setup and self.f == 0:
                    setup()
                self.cap.set_actor_location_and_rotation(loc, rot, False, True)
                self.cc.set_editor_property("fov_angle", fov)
            self.cc.capture_scene()
            self.f += 1
            if self.f >= self.frames:
                unreal.RenderingLibrary.export_render_target(self.world, self.rt, self.out, name + ".png")
                log("shot", name)
                self.i += 1
                self.f = 0
        except Exception as e:  # noqa
            import traceback
            log("CAPTURE ERROR", e, traceback.format_exc())
            unreal.unregister_slate_post_tick_callback(self.h)
            unreal.SystemLibrary.quit_editor()


def shot_list(only=None):
    out = []
    for hero, (mesh, inst) in heroes().items():
        for vname, (loc, rot, fov) in views(mesh, inst).items():
            name = "%s_%s" % (hero, vname)
            if only and not any(o in name for o in only):
                continue
            out.append((name, loc, rot, fov, None))
    return out
