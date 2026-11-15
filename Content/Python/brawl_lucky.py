"""FULL THROTTLE brawl, night 3: THE LUCKY 7 (run 52616). Imports the casino and its new cast and makes its map.
Headless (the module must be built first; run it through wispos --backstage):
  UnrealEditor-Cmd FullThrottleBrawl.uproject -run=pythonscript -script="<abs>/Content/Python/brawl_lucky.py" -AllowCommandletRendering
or in the editor: import brawl_lucky; brawl_lucky.build()

Sources: ArtSource/lucky/*.png (tools/lucky.py, from the Codex originals in lucky/raw/) and ArtSource/frames/
(pitboss_*, dealer_*, the casino pickups item_whiskey|rake|wbottle|cashbox, props prop_cstool|ctable|slot|urn).
Makes:
  /Game/Brawl/Lucky/T_cbg_slots|bar|cage, T_cglow_*, T_cfloor, T_chand, T_laser
  /Game/Brawl/Sprites/Pitboss|Dealer/T_*, the new Items/Props textures
  /Game/Brawl/Maps/L_Lucky7    empty, like L_RustyAnchor (the game mode builds the casino at BeginPlay)
Materials are night 1/2's (M_BrawlWall2D, M_BrawlPaint, M_BrawlOutside, M_BrawlGlow, M_BrawlSprite).
"""
import os
import sys
import unreal
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import brawl_2d

AL = unreal.EditorAssetLibrary
HERE = os.path.dirname(os.path.abspath(__file__))
SRC = os.path.normpath(os.path.join(HERE, '..', '..', 'ArtSource'))
DEST = '/Game/Brawl/Lucky'
FRAMES = ['pitboss_' + n for n in ('idle', 'walk1', 'walk2', 'wind', 'punch', 'bash', 'hurt', 'down', 'draw', 'aim', 'fire', 'taunt')] + \
         ['dealer_' + n for n in ('idle', 'walk1', 'walk2', 'wind', 'punch', 'throw', 'hurt', 'down')] + \
         ['item_whiskey', 'item_rake', 'item_wbottle', 'item_cashbox', 'prop_cstool', 'prop_ctable', 'prop_slot', 'prop_urn']
MASKS = ('T_cglow_slots', 'T_cglow_bar', 'T_cglow_cage', 'T_laser')


def textures():
    d = os.path.join(SRC, 'lucky')
    got = brawl_2d._import(sorted(os.path.join(d, f) for f in os.listdir(d) if f.endswith('.png')), DEST)
    for p in got:
        t = AL.load_asset(p)
        n = p.split('.')[-1]
        t.set_editor_property('lod_group', unreal.TextureGroup.TEXTUREGROUP_WORLD)
        t.set_editor_property('never_stream', True)
        wrap = n == 'T_cfloor'
        t.set_editor_property('address_x', unreal.TextureAddress.TA_WRAP if wrap else unreal.TextureAddress.TA_CLAMP)
        t.set_editor_property('address_y', unreal.TextureAddress.TA_WRAP if wrap else unreal.TextureAddress.TA_CLAMP)
        if n in MASKS:
            t.set_editor_property('srgb', False)
            t.set_editor_property('compression_settings', unreal.TextureCompressionSettings.TC_GRAYSCALE)
        AL.save_loaded_asset(t)
    return got + brawl_2d.only(FRAMES)


def the_map():
    dst = '/Game/Brawl/Maps/L_Lucky7'
    if not AL.does_asset_exist(dst):
        AL.duplicate_asset('/Game/Brawl/Maps/L_RustyAnchor', dst)
    return dst


def build():
    got = textures()
    mp = the_map()
    AL.save_directory(DEST)
    AL.save_directory('/Game/Brawl/Maps')
    unreal.log('BRAWLLUCKY built %d textures, %s' % (len(got), mp))
    print('BRAWLLUCKY ok %d %s' % (len(got), mp))
    return got


if __name__ == '__main__':
    build()
