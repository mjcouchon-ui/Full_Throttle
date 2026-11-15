"""Import the authored Brawl boss poses and animated venue masks."""

from pathlib import Path

import unreal

import brawl_2d


SOURCE = Path(__file__).resolve().parents[3] / 'beat-em-up-unreal' / 'ArtSource' / 'boss-polish'
BOSSES = ('Earl', 'Vinnie', 'Pitboss', 'Frankie', 'Kingpin', 'Sven', 'Commodore')
POSES = ('intro', 'intro2', 'signatureprep', 'signaturefollow', 'defeat')
VENUES = ('harbour', 'disco', 'casino', 'embers', 'lanes', 'snow', 'deck')


def _import(files, destination, mask=False):
    missing = [path.name for path in files if not path.is_file()]
    if missing:
        raise RuntimeError('Missing Brawl source art: ' + ', '.join(missing))
    result = brawl_2d._import([str(path) for path in files], destination)
    if len(result) != len(files):
        raise RuntimeError(f'Brawl import incomplete at {destination}: {len(result)}/{len(files)}')
    for path in result:
        texture = brawl_2d.AL.load_asset(path)
        texture.set_editor_property('lod_group', unreal.TextureGroup.TEXTUREGROUP_WORLD if mask
                                    else unreal.TextureGroup.TEXTUREGROUP_CHARACTER)
        texture.set_editor_property('never_stream', True)
        texture.set_editor_property('address_x', unreal.TextureAddress.TA_CLAMP)
        texture.set_editor_property('address_y', unreal.TextureAddress.TA_CLAMP)
        if mask:
            texture.set_editor_property('srgb', False)
            texture.set_editor_property('compression_settings', unreal.TextureCompressionSettings.TC_GRAYSCALE)
        if not brawl_2d.AL.save_loaded_asset(texture):
            raise RuntimeError(f'Could not save {path}')
    return result


def build():
    paths = []
    for boss in BOSSES:
        paths += _import([SOURCE / f'{boss.lower()}_{pose}.png' for pose in POSES],
                         '/Game/Brawl/Sprites/' + boss)
    paths += _import([SOURCE / f'venue_{venue}.png' for venue in VENUES], '/Game/Brawl/Polish', True)
    unreal.log(f'BRAWL BOSS POLISH imported {len(paths)} textures')
    return paths


if __name__ == '__main__':
    build()
