"""FULL THROTTLE 2.6: the SFX DEPT's recoloured effects (spec v6, 07 wave 1).

The stock Niagara systems are stateless (lightweight) emitters with no user parameters, so each colour is a
copy of the stock system with its ScaleColor gradient recoloured (by reflection through UFTEditorTools, as
ft_fx does). Same spawn counts, sizes, lifetimes and timing as stock: looks only (07 §1). The game swaps the
asset for the worn item (FTFlow.cpp ApplyCosmetics / Explode).
Run over the Unreal MCP connector:  import ft_v26; ft_v26.make_tints()
"""
import colorsys
import unreal
import ft_fx

AL = unreal.EditorAssetLibrary
T = unreal.FTEditorTools
FX = '/Game/FX/'
E = ft_fx.E


def lin(hexs):
    c = unreal.LinearColor()
    h = hexs.lstrip('#')
    r, g, b = (int(h[i:i + 2], 16) / 255.0 for i in (0, 2, 4))
    f = lambda v: v / 12.92 if v <= 0.04045 else ((v + 0.055) / 1.055) ** 2.4
    return (f(r), f(g), f(b))


def _scale_color_index(s):
    i = 0
    while True:
        v = T.get_path(s, E + 'Modules[%d]' % i)
        if v.startswith('ERROR'):
            raise RuntimeError('no ScaleColor')
        if 'ScaleColor' in v.split("'")[0]:
            return i
        i += 1


def tint(base, name, keys, material=None):
    path = FX + name
    if AL.does_asset_exist(path):
        AL.delete_asset(path)
    s = AL.duplicate_asset(FX + base, path)
    idx = _scale_color_index(s)
    ft_fx._set(s, 'Modules[%d].ScaleDistribution' % idx, ft_fx.colors(*keys))
    if material:
        ft_fx._set(s, 'RendererProperties[0].Material', ft_fx.mat(material))
    T.compile_niagara(s)
    AL.save_loaded_asset(s)
    return name


def flame(c, core=None):
    core = core or tuple(0.55 + 0.45 * x for x in c)
    return ((core[0] * 11, core[1] * 11, core[2] * 11, 1), (c[0] * 8, c[1] * 8, c[2] * 8, 1), (c[0] * 3, c[1] * 3, c[2] * 3, 0))


def smoke(c):
    return ((c[0] * 0.6, c[1] * 0.6, c[2] * 0.6, 0), (c[0] * 0.55, c[1] * 0.55, c[2] * 0.55, 0.34), (c[0] * 0.5, c[1] * 0.5, c[2] * 0.5, 0))


def make_tints():
    made = []
    # nitro colours (07 SFX DEPT · Nitro colour): turbo flames only
    for cid, hx in (('s_blue_flame', '#3d7bff'), ('s_methanol_green', '#9dff8a'), ('s_purple_haze', '#9b4dff'), ('s_liquid_gold', '#ffc93c')):
        made.append(tint('NS_TurboFlame', 'NS_TurboFlame_' + cid, flame(lin(hx))))
    made.append(tint('NS_TurboFlame', 'NS_TurboFlame_s_white_hot', ((12, 12, 12, 1), (7, 7.5, 9, 1), (1.2, 2.2, 6, 0))))
    hues = [colorsys.hsv_to_rgb(h / 5.0, 0.9, 1.0) for h in range(5)]
    made.append(tint('NS_TurboFlame', 'NS_TurboFlame_s_rainbow_nitro', tuple((r * 8, g * 8, b * 8, 1 if i < 4 else 0) for i, (r, g, b) in enumerate(hues))))
    # tyre smoke (the drift sparks are a separate effect and keep blue / orange / pink)
    for cid, hx in (('s_pink_smoke', '#ff9ccc'), ('s_cyan_smoke', '#9ff3f5'), ('s_yellow_smoke', '#ffe88a'), ('s_green_smoke', '#a8ffb8')):
        made.append(tint('NS_TyreSmoke', 'NS_TyreSmoke_' + cid, smoke(lin(hx))))
    # wreck FX: a tinted copy of the fireball (same size and timing as stock)
    made.append(tint('NS_Explosion', 'NS_Explosion_s_blue_fire', ((4, 7, 12, 1), (0.6, 2.6, 7, 1), (0.12, 0.7, 3, 0.8), (0.04, 0.1, 0.7, 0.4), (0, 0, 0, 0)), '/Game/FX/M_FX_Fire.M_FX_Fire'))
    made.append(tint('NS_Explosion', 'NS_Explosion_s_toxic_blast', ((5, 12, 4, 1), (1.4, 7, 0.8, 1), (0.3, 3, 0.15, 0.8), (0.05, 0.6, 0.05, 0.4), (0, 0, 0, 0)), '/Game/FX/M_FX_Fire.M_FX_Fire'))
    made.append(tint('NS_ExplosionSparks', 'NS_ExplosionSparks_s_confetti_pop', ((10, 2, 8, 1), (2, 9, 9, 1), (9, 8, 1.5, 1), (2, 3, 10, 0))))
    made.append(tint('NS_ExplosionSparks', 'NS_ExplosionSparks_s_gold_rush', ((12, 9, 2.5, 1), (8, 5.5, 1, 1), (3, 1.8, 0.2, 0))))
    return 'built: ' + ', '.join(made)
