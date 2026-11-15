"""FULL THROTTLE brawl, night 1's cutscenes (run 49531): the pier outside THE RUSTY ANCHOR and Harlan Voss.
Headless (the module must be built first; run it through wispos --backstage):
  UnrealEditor-Cmd FullThrottleBrawl.uproject -run=pythonscript -script="<abs>/Content/Python/brawl_night1.py"
or in the editor: import brawl_night1; brawl_night1.build()

Sources: ArtSource/night1/*.png (tools/night1.py, from the Codex original in night1/raw/) and ArtSource/frames/
(voss_*, car_limo). Makes:
  /Game/Brawl/Night1/T_bg_anchor_out, T_glow_anchor_out   the bar's outside wall on the pier + its neon mask
  /Game/Brawl/Sprites/Voss/T_voss_*                        Harlan Voss (idle, walk1, walk2, point, give, laugh, smoke, away)
  /Game/Brawl/Sprites/Props/T_car_limo                     his limousine
Touches nothing else. The game mode stages the cutscenes from code (FTBrawlCutscene.cpp).
"""
import os
import sys
import unreal
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import brawl_2d

AL = unreal.EditorAssetLibrary
HERE = os.path.dirname(os.path.abspath(__file__))
SRC = os.path.normpath(os.path.join(HERE, '..', '..', 'ArtSource'))
DEST = '/Game/Brawl/Night1'
FRAMES = ['voss_' + n for n in ('idle', 'walk1', 'walk2', 'point', 'give', 'laugh', 'smoke', 'away')] + ['car_limo']


def build():
    d = os.path.join(SRC, 'night1')
    got = brawl_2d._import(sorted(os.path.join(d, f) for f in os.listdir(d) if f.endswith('.png')), DEST)
    for p in got:
        t = AL.load_asset(p)
        n = p.split('.')[-1]
        t.set_editor_property('lod_group', unreal.TextureGroup.TEXTUREGROUP_WORLD)
        t.set_editor_property('never_stream', True)
        t.set_editor_property('address_x', unreal.TextureAddress.TA_CLAMP)
        t.set_editor_property('address_y', unreal.TextureAddress.TA_CLAMP)
        if n.startswith('T_glow'):
            t.set_editor_property('srgb', False)
            t.set_editor_property('compression_settings', unreal.TextureCompressionSettings.TC_GRAYSCALE)
        AL.save_loaded_asset(t)
    got += brawl_2d.only(FRAMES)
    unreal.log('BRAWL_NIGHT1 imported %d: %s' % (len(got), ', '.join(p.split('.')[-1] for p in got)))
    return got


if __name__ == '__main__':
    build()
