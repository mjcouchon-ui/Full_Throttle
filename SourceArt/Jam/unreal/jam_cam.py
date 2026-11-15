"""Capture tools for the game-jam pass: an editor camera that stands where the game's chase camera does.

`chase(track, d)` rebuilds AFTGameMode::ComputeCamera for a car on the road's centre line at distance d (cm): the
camera rides the road's own frame, so banking and loops roll it exactly as in the game. `Shooter` films a list of
shots with a SceneCapture2D that carries the level's grade and cel pass (copied from the Grade80s volume, because a
capture does not take a volume's post-process materials). Works on any of the eight levels.

These are for finding and isolating things quickly. The judge is still the real game (play_port.ps1 / -ftexec).
Output: SkyScratch/HandBack/jam/<tag>/<shot>.png
"""
import os
import math
import unreal

OUT_ROOT = r"K:\Full Throttle\SkyScratch\HandBack\jam"
MAPS = ["/Game/Maps/Bayshore/L_Bayshore", "/Game/Maps/ChemicalWorks/L_ChemicalWorks", "/Game/Maps/Nevada/L_Nevada",
        "/Game/Maps/Volcano/L_Volcano", "/Game/Maps/ThemePark/L_ThemePark", "/Game/Maps/Glacier/L_Glacier",
        "/Game/Maps/Abyss/L_Abyss", "/Game/Maps/Metro/L_Metro"]
NAMES = ["Bayshore", "ChemicalWorks", "Nevada", "Volcano", "ThemePark", "Glacier", "Abyss", "Metro"]
LES = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
EAS = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
# FTFlag bits (FTCore.h)
DU = 13.71


def log(*a):
    print("JAM", *[str(x) for x in a], flush=True)


def world():
    return unreal.EditorLevelLibrary.get_editor_world()


def _discard():
    """The enum value that lets load_level drop unsaved changes (a capture actor spawned in a map marks it changed,
    and the editor then refuses to load the next map). Its name has moved between engine versions: look it up."""
    for n in dir(unreal):
        if "UnsavedChanges" in n:
            e = getattr(unreal, n)
            if hasattr(e, "DISCARD"):
                return e.DISCARD
    return None


def load(level):
    pol = _discard()
    if pol is not None:
        try:
            LES.load_level(MAPS[level], unsaved_changes_policy=pol)
            return track()
        except TypeError:
            pass
    LES.load_level(MAPS[level])
    return track()


def actors():
    return list(EAS.get_all_level_actors())


def actor(label):
    for a in actors():
        if a.get_actor_label() == label:
            return a
    return None


def track():
    for a in actors():
        if a.get_actor_label().startswith("Track_"):
            return a
    return None


def _v(t):
    return t.translation


def frame(tr, d):
    """(pos, fwd, right, up) of the road at d."""
    t = tr.tool_frame(float(d))
    r = t.rotation
    fwd = unreal.MathLibrary.get_forward_vector(r.rotator())
    right = unreal.MathLibrary.get_right_vector(r.rotator())
    up = unreal.MathLibrary.get_up_vector(r.rotator())
    return t.translation, fwd, right, up


def _add(p, *terms):
    x, y, z = p.x, p.y, p.z
    for v, k in terms:
        x, y, z = x + v.x * k, y + v.y * k, z + v.z * k
    return unreal.Vector(x, y, z)


def _rot_from_xz(fwd, up):
    return unreal.MathLibrary.make_rot_from_xz(fwd, up)


def chase(tr, d, mph=117.0, x=0.0, loop=False):
    """(location, rotation, fov) of the chase camera for a car at distance d, x cm across, at the given speed."""
    pos, fwd, right, up = frame(tr, d)
    car = _add(pos, (right, x), (up, 55.0))
    spd = mph * 44.704
    back = 780.0 + spd * 0.018
    cam = _add(car, (fwd, -back), (up, 210.0 if loop else 290.0))
    look = _add(car, (fwd, 700.0), (up, 90.0))
    f = unreal.Vector(look.x - cam.x, look.y - cam.y, look.z - cam.z)
    n = math.sqrt(f.x * f.x + f.y * f.y + f.z * f.z)
    f = unreal.Vector(f.x / n, f.y / n, f.z / n)
    fov = 78.0 + min(max(spd / (700.0 * DU), 0.0), 1.0) * 14.0
    return cam, _rot_from_xz(f, up), fov


def free(loc, target, fov=70.0):
    dx, dy, dz = target.x - loc.x, target.y - loc.y, target.z - loc.z
    return loc, unreal.Rotator(roll=0.0, pitch=math.degrees(math.atan2(dz, math.hypot(dx, dy))), yaw=math.degrees(math.atan2(dy, dx))), fov


# FTTrack.h flag bits
KINDS = [("gap", 1), ("loop", 2), ("twist", 4), ("pipe", 8), ("wall", 16), ("kicker", 32), ("landing", 64),
         ("inverted", 256), ("halfpipe", 1024), ("coaster", 2048), ("building", 4096)]


def _runs(mask, flags):
    """[(first index, last index)] of consecutive samples that have the bit."""
    out, a = [], None
    for i, f in enumerate(flags):
        if f & mask:
            if a is None:
                a = i
        elif a is not None:
            out.append((a, i - 1))
            a = None
    if a is not None:
        out.append((a, len(flags) - 1))
    return out


def _peaks(vals, n, apart):
    """Indices of the n largest values at least `apart` samples from each other."""
    got = []
    for i in sorted(range(len(vals)), key=lambda j: -vals[j]):
        if all(abs(i - j) >= apart for j in got):
            got.append(i)
            if len(got) >= n:
                break
    return got


def specials(tr, per=2, step=1000.0):
    """Chase-camera shots of a level's special sections (jumps and their run-ups, loops, corkscrews, pipes, half-pipes,
    wall rides, coaster track, building runs, the highest elevated stretches and the steepest banked turns): the
    clearance check for scenery. Returns (shots, summary): shots as Shooter takes them, named s_<kind>_d<metres>."""
    L = tr.tool_length()
    ds = [1500.0 + i * step for i in range(int((L - 4000.0) / step))]
    flags = [tr.tool_flags(d) for d in ds]
    high = [tr.tool_frame(d).translation.z - tr.tool_ground(d).z for d in ds]
    bank = [abs(tr.tool_bank(d)) for d in ds]
    picks, found = [], []
    for name, mask in KINDS:
        rs = _runs(mask, flags)
        if not rs:
            continue
        found.append("%s x%d (first at %d m)" % (name, len(rs), ds[rs[0][0]] / 100.0))
        for a, b in ([rs[0]] if len(rs) == 1 or per < 2 else [rs[0], rs[-1]]):
            picks.append((name, (a + b) // 2))
            if name in ("gap", "kicker"):   # and the run-up: what the player sees as the jump comes
                picks.append((name + "_runup", max(0, a - 8)))
    for i in _peaks(high, per, 150):
        if high[i] > 800.0:
            picks.append(("high%dm" % round(high[i] / 100.0), i))
    for i in _peaks(bank, per, 150):
        if bank[i] > 0.35:
            picks.append(("bank%ddeg" % round(bank[i] * 57.2958), i))
    shots, seen = [], set()
    for name, i in picks:
        key = (name.split("_")[0], i // 6)
        if key in seen:
            continue
        seen.add(key)
        loc, rot, fov = chase(tr, ds[i], loop=bool(flags[i] & 10))
        shots.append(("s_%s_d%05d" % (name, ds[i] / 100.0), loc, rot, fov, None))
    summary = "%s | highest road over the land %d m | steepest bank %d deg" % (
        "; ".join(found) or "no special sections", round(max(high) / 100.0), round(max(bank) * 57.2958))
    return shots, summary


class Shooter(object):
    """Films [(name, loc, rot, fov, setup)] on the editor's tick. setup() runs when its shot starts (use it to hide or
    show things); `frames` captures per shot let TSR and Lumen settle."""

    def __init__(self, tag, shots, done=None, warm=60, frames=24, size=(1720, 720), grade=True, source=None):
        self.out = os.path.join(OUT_ROOT, tag)
        os.makedirs(self.out, exist_ok=True)
        self.shots, self.done, self.warm, self.frames = shots, done, warm, frames
        self.world = world()
        fmt = unreal.TextureRenderTargetFormat.RTF_RGBA8
        self.rt = unreal.RenderingLibrary.create_render_target2d(self.world, size[0], size[1], fmt, unreal.LinearColor(0, 0, 0, 1), False)
        self.cap = EAS.spawn_actor_from_class(unreal.SceneCapture2D, unreal.Vector(0, 0, 0), unreal.Rotator(0, 0, 0))
        cc = self.cap.get_editor_property("capture_component2d")
        cc.set_editor_property("texture_target", self.rt)
        cc.set_editor_property("capture_source", source or unreal.SceneCaptureSource.SCS_FINAL_COLOR_LDR)
        cc.set_editor_property("capture_every_frame", False)
        cc.set_editor_property("capture_on_movement", False)
        cc.set_editor_property("always_persist_rendering_state", True)
        ppv = actor("Grade80s")
        if grade and ppv is not None:
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
                if fov < 0.0:  # a negative "fov" is an orthographic view that many cm wide
                    self.cc.set_editor_property("projection_type", unreal.CameraProjectionMode.ORTHOGRAPHIC)
                    self.cc.set_editor_property("ortho_width", -fov)
                else:
                    self.cc.set_editor_property("projection_type", unreal.CameraProjectionMode.PERSPECTIVE)
                    self.cc.set_editor_property("fov_angle", fov)
            self.cc.capture_scene()
            self.f += 1
            if self.f >= self.frames:
                unreal.RenderingLibrary.export_render_target(self.world, self.rt, self.out, name + ".png")
                self.i += 1
                self.f = 0
        except Exception as e:  # noqa
            import traceback
            log("CAPTURE ERROR", e, traceback.format_exc())
            unreal.unregister_slate_post_tick_callback(self.h)
            unreal.SystemLibrary.quit_editor()
