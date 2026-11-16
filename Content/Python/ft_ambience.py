"""Imports the world's sound (the WAVs Tools/sfx_ambience.py wrote) into /Game/Audio/Ambience.

    import ft_ambience
    ft_ambience.build(r'<folder of the WAVs>')              # every sound named in <folder>/ambience.txt
    ft_ambience.build(r'<folder>', levels=['L0', 'L7'])     # or some levels' only
    ft_ambience.report()                                    # what is in the project now

ambience.txt (written by the generator) says what each sound is: a "bed" and a "spot" are saved looping, a "pass" is
a one-shot. Every sound keeps the project's default sound settings, which are those of the game's own loops (no sound
class, no submix, compression quality 80): Source/FullThrottle/FTAmbience.cpp sets every level itself.
build() returns the lines of a report (the caller logs them): each asset's length, rate, channels, whether it loops,
and the size of its .uasset (a megabyte at most: the generator's loop lengths are chosen for that).
Each sound is original: synthesised by Tools/sfx_ambience.py, nothing downloaded, nothing sampled.
"""
import os

import unreal

DEST = '/Game/Audio/Ambience'
LIMIT = 1000000   # bytes: a bed stays under a megabyte

AL = unreal.EditorAssetLibrary


def _prop(obj, name):
    try:
        return obj.get_editor_property(name)
    except Exception:
        return None


def _file(name):
    return os.path.join(unreal.Paths.convert_relative_path_to_full(unreal.Paths.project_content_dir()), 'Audio', 'Ambience', name + '.uasset')


def _describe(name, wave):
    f = _file(name)
    size = os.path.getsize(f) if os.path.exists(f) else -1
    return '%-18s dur=%.3f rate=%s ch=%s looping=%s bytes=%d%s' % (
        name, _prop(wave, 'duration') or 0.0, _prop(wave, 'sample_rate'), _prop(wave, 'num_channels'), _prop(wave, 'looping'), size,
        '  OVER A MEGABYTE' if size > LIMIT else '')


def kinds(folder):
    """name -> 'bed' | 'spot' | 'pass', from the generator's list."""
    out = {}
    with open(os.path.join(folder, 'ambience.txt')) as fh:
        for line in fh:
            parts = line.split()
            if len(parts) >= 2:
                out[parts[0]] = parts[1]
    return out


def report():
    out = []
    for path in sorted(AL.list_assets(DEST, recursive=False, include_folder=False)):
        name = path.split('/')[-1].split('.')[0]
        wave = AL.load_asset(DEST + '/' + name)
        if wave:
            out.append(_describe(name, wave))
    return out


def build(folder, levels=None):
    kind = kinds(folder)
    names = sorted(n for n in kind if not levels or n.split('_')[1] in levels)
    missing = [n for n in names if not os.path.exists(os.path.join(folder, n + '.wav'))]
    if missing:
        raise RuntimeError('no WAV for: ' + ', '.join(missing))
    new = [n for n in names if not AL.does_asset_exist(DEST + '/' + n)]
    tasks = []
    for n in names:
        t = unreal.AssetImportTask()
        t.set_editor_property('filename', os.path.join(folder, n + '.wav'))
        t.set_editor_property('destination_path', DEST)
        t.set_editor_property('destination_name', n)
        t.set_editor_property('automated', True)
        t.set_editor_property('replace_existing', True)
        t.set_editor_property('save', False)
        tasks.append(t)
    unreal.AssetToolsHelpers.get_asset_tools().import_asset_tasks(tasks)
    out = []
    over = 0
    for n in names:
        path = DEST + '/' + n
        wave = AL.load_asset(path)
        if not wave:
            raise RuntimeError('import failed: ' + path)
        wave.set_editor_property('looping', kind[n] in ('bed', 'spot'))
        AL.save_asset(path, only_if_is_dirty=False)
        line = _describe(n, wave)
        over += 'OVER A MEGABYTE' in line
        out.append('%s %-4s %s' % ('NEW     ' if n in new else 'replaced', kind[n], line))
    out.append('imported %d sounds (%d new, %d replaced) into %s; %d over a megabyte' % (len(names), len(new), len(names) - len(new), DEST, over))
    return out
