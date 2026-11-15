"""Write the live look values (bsky_params) into DA_BayshoreSkyPreset_<P> and the MPC defaults, without rebuilding Blueprints.
Run: ue_run.ps1 -Editor -Script 45_preset_values.py"""
import sys
import os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)) if "__file__" in globals() else r"K:\Full Throttle\SkyScratch\SourceArt\BayshoreSky\unreal")
import unreal
from bsky_ue import log, save
from bsky_params import SCALARS, VECTORS, PRESET_EXTRA_VALUES, NOT_IN_PRESET

P = "Day"
da = unreal.load_asset("/BayshoreSky/Data/DA_BayshoreSkyPreset_" + P)
for n, v in SCALARS:
    if n not in NOT_IN_PRESET:
        da.set_editor_property(n, float(v))
for n, v in VECTORS:
    da.set_editor_property(n, unreal.LinearColor(*v))
for n, v in PRESET_EXTRA_VALUES.items():
    if isinstance(v, tuple) and v[0] == "rotator":
        v = unreal.Rotator(roll=v[1], pitch=v[2], yaw=v[3])
    elif isinstance(v, tuple) and v[0] == "color":
        v = unreal.LinearColor(v[1], v[2], v[3], 1.0)
    da.set_editor_property(n, v)
save(da)
mpc = unreal.load_asset("/BayshoreSky/Materials/MPC_BayshoreSky")
sp = list(mpc.get_editor_property("ScalarParameters"))
sv = dict(SCALARS)
for p in sp:
    n = str(p.get_editor_property("ParameterName"))
    if n in sv:
        p.set_editor_property("DefaultValue", float(sv[n]))
mpc.set_editor_property("ScalarParameters", sp)
vp = list(mpc.get_editor_property("VectorParameters"))
vv = dict(VECTORS)
for p in vp:
    n = str(p.get_editor_property("ParameterName"))
    if n in vv:
        p.set_editor_property("DefaultValue", unreal.LinearColor(*vv[n]))
mpc.set_editor_property("VectorParameters", vp)
save(mpc)
log("preset values written", da.get_name())
