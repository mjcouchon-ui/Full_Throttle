"""Level 1 sky clipping, step 2b: which part of the sky fills the hole?

Same recorded pose as step 2. final / the sky's backdrop sea switched off (MPC SeaEnabled 0) / the cloud cards and
cloud layers off / the whole BayshoreSky Level Instance hidden. If the dark area changes only with the sea switch and
goes to the empty-frame colour when the instance is hidden, the pixels are the dome's painted sea: sky, not ground.

Run: jam_run.ps1 -Script 22_sky_clip_sky_side.py     Output: HandBack/jam/clip_before/
"""
import os
import sys
import unreal

sys.path.insert(0, r"K:\Full Throttle\SkyScratch\SourceArt\Jam\unreal")
import jam_cam as J

TAG = os.environ.get("JAM_TAG", "clip_before")
try:
    tr = J.load(0)
    W = J.world()
    mpc = unreal.load_asset("/BayshoreSky/Materials/MPC_BayshoreSky")
    li = J.actor("BayshoreSky")
    ML = unreal.MaterialLibrary
    keep = {n: ML.get_scalar_parameter_value(W, mpc, n) for n in ("SeaEnabled", "CloudCardsShown", "AltoCoverage", "CirrusCoverage")}
    J.log("mpc", keep, "li", li.get_class().get_name() if li else None)

    def state(sea=True, clouds=True, sky=True):
        def f():
            ML.set_scalar_parameter_value(W, mpc, "SeaEnabled", keep["SeaEnabled"] if sea else 0.0)
            for n in ("CloudCardsShown", "AltoCoverage", "CirrusCoverage"):
                ML.set_scalar_parameter_value(W, mpc, n, keep[n] if clouds else 0.0)
            li.set_is_temporarily_hidden_in_editor(not sky)
        return f

    shots = []
    for d in (1180.0, 3880.0):
        loc, rot, fov = J.chase(tr, d * 100.0)
        for name, st in (("skyfinal", state()), ("nosea", state(sea=False)), ("noclouds", state(clouds=False)), ("nosky", state(sky=False)), ("skyback", state())):
            shots.append(("d%05d_%s" % (d, name), loc, rot, fov, st))
    J.Shooter(TAG, shots, warm=40, frames=12, size=(1720, 720)).start()
except Exception as e:  # noqa
    import traceback
    J.log("ERR", repr(e), traceback.format_exc())
    unreal.SystemLibrary.quit_editor()
