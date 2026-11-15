"""FULL THROTTLE brawl, day 1's cutscene BEFORE THE DRIVE (run 73444): the Route 9 film set, Kip Lorenzo, Voss on set.
Headless (the module must be built first; run it through wispos --backstage):
  UnrealEditor-Cmd FullThrottleBrawl.uproject -run=pythonscript -script="<abs>/Content/Python/brawl_day1.py"
or in the editor: import brawl_day1; brawl_day1.build()

Sources: ArtSource/day1/*.png (tools/day1.py, from the Codex originals in day1/raw/) and ArtSource/frames/
(kip_*, voss_mega/shout/frame/order, car_kestrel, prop_camera/lamp/chair/boxes). Makes:
  /Game/Brawl/Day1/T_bg_set_a, T_bg_set_b, T_road        the set's two back panels and the road
  /Game/Brawl/Sprites/Kip/T_kip_*                          Kip Lorenzo (idle, walk1, walk2, wave, me, nervous, ask, smirk)
  /Game/Brawl/Sprites/Voss/T_voss_mega|shout|frame|order   Voss with his megaphone
  /Game/Brawl/Sprites/Props/T_car_kestrel, T_prop_camera|lamp|chair|boxes
  /Game/Brawl/Maps/L_Day1Set                               a copy of the empty L_RustyAnchor (the game mode builds the set)
Touches nothing else. The game mode stages the cutscene from code (FTBrawlCutscene.cpp, cut 10).
"""
import os
import sys
import unreal
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import brawl_2d

AL = unreal.EditorAssetLibrary
HERE = os.path.dirname(os.path.abspath(__file__))
SRC = os.path.normpath(os.path.join(HERE, '..', '..', 'ArtSource'))
DEST = '/Game/Brawl/Day1'
MAP = '/Game/Brawl/Maps/L_Day1Set'
FRAMES = (['kip_' + n for n in ('idle', 'walk1', 'walk2', 'wave', 'me', 'nervous', 'ask', 'smirk')]
          + ['voss_' + n for n in ('mega', 'shout', 'frame', 'order')]
          + ['car_kestrel'] + ['prop_' + n for n in ('camera', 'lamp', 'chair', 'boxes')])


def build():
    d = os.path.join(SRC, 'day1')
    got = brawl_2d._import(sorted(os.path.join(d, f) for f in os.listdir(d) if f.endswith('.png')), DEST)
    for p in got:
        t = AL.load_asset(p)
        t.set_editor_property('lod_group', unreal.TextureGroup.TEXTUREGROUP_WORLD)
        t.set_editor_property('never_stream', True)
        if not p.endswith('T_road'):
            t.set_editor_property('address_x', unreal.TextureAddress.TA_CLAMP)
            t.set_editor_property('address_y', unreal.TextureAddress.TA_CLAMP)
        AL.save_loaded_asset(t)
    got += brawl_2d.only(FRAMES)
    if not AL.does_asset_exist(MAP):
        AL.duplicate_asset('/Game/Brawl/Maps/L_RustyAnchor', MAP)
        AL.save_asset(MAP)
    got.append(MAP)
    unreal.log('BRAWL_DAY1 imported %d: %s' % (len(got), ', '.join(p.split('.')[-1].split('/')[-1] for p in got)))
    return got


if __name__ == '__main__':
    build()
