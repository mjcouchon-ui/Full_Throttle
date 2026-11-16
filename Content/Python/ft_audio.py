"""Imports the sound effects Tools/sfx_synth.py wrote (16-bit mono WAVs) into /Game/Audio/SFX.

    import ft_audio
    ft_audio.build(r'<folder of the WAVs>')                      # the names in <folder>/overhaul.txt (the 15 Nov overhaul's NEW sounds)
    ft_audio.build(r'<folder>', names=['S_Explode', 'S_Gun'])    # or any names
    ft_audio.report(['S_Explode', 'S_Near'])                     # what is in the project now

A new sound takes the settings of the 2.x effects (those of S_Pickup, read at run time: its sound class, compression
and loading), so the new ones sit in the mix exactly as the old ones do; a second or third take (S_Explode_B) takes
those of the sound it is a take of (the takes are in the project; since the revert of 15 Nov the game plays S_<Name>
only). A sound that replaces one keeps that asset's
own settings: they are read before the import and put back after it (the report says which, if any, the import had
changed). Loops (LOOPS) are saved looping (the title logo's neon hum, S_TitleNeonHum, is one).
A sound the game already has never changes (owner's rule): build() without names leaves out the ten the overhaul once
replaced (KEPT; put back by commit ff773438) even if an old overhaul.txt still lists them, and says so in its report.
sfx_synth.py now writes its heavier versions of those to <folder>/offered/, which nothing here reads.
Each sound is original: synthesised by Tools/sfx_synth.py, nothing downloaded.
"""
import os

import unreal

DEST = '/Game/Audio/SFX'
LOOPS = ('S_EngineLoop', 'S_Rotor', 'S_Skid', 'S_PropBuzz', 'S_TitleNeonHum')
MODEL = 'S_Pickup'
# existing sounds the overhaul's heavier versions must never replace: left out of the default (overhaul.txt) import
KEPT = ('S_Explode', 'S_Wreck', 'S_HeliDown', 'S_Missile', 'S_Hit', 'S_PlayerHit', 'S_Bump', 'S_Land', 'S_BossIntro', 'S_BossPhase')
# what a new sound copies from the model (editor property names; one missing from this engine version is skipped)
COPIED = ('sound_class_object', 'sound_group', 'compression_quality', 'loading_behavior_override', 'volume', 'pitch',
          'sound_submix_object', 'virtualization_mode', 'priority', 'attenuation_settings', 'concurrency_set')

AL = unreal.EditorAssetLibrary


def _prop(obj, name):
    try:
        return obj.get_editor_property(name)
    except Exception:
        return None


def describe(wave):
    return 'dur=%.3f s  rate=%s  ch=%s  looping=%s  class=%s  group=%s  quality=%s  loading=%s  volume=%s' % (
        _prop(wave, 'duration') or 0.0, _prop(wave, 'sample_rate'), _prop(wave, 'num_channels'), _prop(wave, 'looping'),
        (_prop(wave, 'sound_class_object').get_name() if _prop(wave, 'sound_class_object') else None), _prop(wave, 'sound_group'),
        _prop(wave, 'compression_quality'), _prop(wave, 'loading_behavior_override'), _prop(wave, 'volume'))


def report(names):
    out = []
    for n in names:
        path = DEST + '/' + n
        wave = AL.load_asset(path) if AL.does_asset_exist(path) else None
        out.append('%-16s %s' % (n, describe(wave) if wave else 'not in the project'))
    return out


def build(folder, names=None):
    """Returns the lines of a report (the caller logs them)."""
    out = []
    if names is None:
        with open(os.path.join(folder, 'overhaul.txt')) as f:
            names = [line.strip() for line in f if line.strip()]
        held = [n for n in names if n in KEPT]
        if held:   # a list from before the revert: those WAVs are the heavier versions of sounds the game already has
            names = [n for n in names if n not in KEPT]
            out.append('NOT imported (existing sounds, never replaced): ' + ', '.join(held))
    missing = [n for n in names if not os.path.exists(os.path.join(folder, n + '.wav'))]
    if missing:
        raise RuntimeError('no WAV for: ' + ', '.join(missing))
    new = [n for n in names if not AL.does_asset_exist(DEST + '/' + n)]
    model = AL.load_asset(DEST + '/' + MODEL)
    out.append('model %s: %s' % (MODEL, describe(model) if model else 'MISSING'))
    # a replaced sound's own settings, read before the import (an unattended import may put the defaults back)
    kept = {}
    for n in names:
        if n not in new:
            was = AL.load_asset(DEST + '/' + n)
            kept[n] = dict((prop, _prop(was, prop)) for prop in COPIED)
    # a new sound's settings: a second or third take (S_Explode_B) takes those of the sound it is a take of, read now,
    # before the import touches it; any other new sound takes the model's
    given = {}
    for n in new:
        base = n[:-2] if n[-2:] in ('_B', '_C') else None
        source = AL.load_asset(DEST + '/' + base) if base and AL.does_asset_exist(DEST + '/' + base) else model
        given[n] = dict((prop, _prop(source, prop)) for prop in COPIED) if source else {}
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
    for n in names:
        path = DEST + '/' + n
        wave = AL.load_asset(path)
        if not wave:
            raise RuntimeError('import failed: ' + path)
        back = []
        for prop in COPIED:
            value = given[n].get(prop) if n in new else kept[n].get(prop)
            if value is None:
                continue
            if n not in new and _prop(wave, prop) != value:
                back.append(prop)
            try:
                wave.set_editor_property(prop, value)
            except Exception:
                pass
        wave.set_editor_property('looping', n in LOOPS)
        AL.save_asset(path, only_if_is_dirty=False)
        out.append('%-16s %s  %s%s' % (n, 'NEW     ' if n in new else 'replaced', describe(wave),
                                       ('  (put back: ' + ', '.join(back) + ')') if back else ''))
    out.append('imported %d sounds (%d new, %d replaced) into %s' % (len(names), len(new), len(names) - len(new), DEST))
    return out
