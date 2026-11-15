"""Imports the brawl's audio into /Game/Brawl/Audio.

S_FightMusic = Matt's fight music (Docs/audio/matt-fight-music.mp3, 2:12), saved looping.
S_BossMusic = the boss track (Docs/audio/boss-battle.mp3, "Bit-Reduced Boss Battle", 2:51), saved looping.
AFTBrawlMusic (Source/.../Brawl/FTBrawlMusic.cpp) plays the fight track through the waves and the boss track from
the moment the boss comes out. A track whose mp3 is missing is skipped (its asset is left as it is).
SFX/S_brawl_* = the fight's sounds (run 38222): beat-em-up-unreal/Docs/audio/sfx/*.wav, made by Tools/brawl_sfx_synth.py;
AFTBrawlGameMode::Sfx plays them.
The mp3s and wavs live in beat-em-up-unreal/Docs/audio; full-throttle-unreal imports them from there (same file in both projects).

Run in the editor (`import brawl_audio; brawl_audio.build()`) or headless:
  UnrealEditor-Cmd <project>.uproject -run=pythonscript -script="<abs>/Content/Python/brawl_audio.py"
  (add `-- sfx` to import only the sound effects)
"""
import glob
import os
import sys

import unreal

DEST = "/Game/Brawl/Audio"
TRACKS = [("matt-fight-music.mp3", "S_FightMusic"), ("boss-battle.mp3", "S_BossMusic")]


def _proj():
    return unreal.Paths.convert_relative_path_to_full(unreal.Paths.project_dir())


def _audio_dir():
    d = os.path.join(_proj(), "Docs", "audio")
    if not os.path.isdir(d):   # full-throttle-unreal: the brawl project's copy
        d = os.path.join(_proj(), "..", "beat-em-up-unreal", "Docs", "audio")
    return os.path.normpath(d)


def _sfx_dir():
    for d in (os.path.join(_proj(), "Docs", "audio", "sfx"), os.path.join(_proj(), "..", "beat-em-up-unreal", "Docs", "audio", "sfx")):
        if glob.glob(os.path.join(d, "S_brawl_*.wav")):
            return os.path.normpath(d)
    raise RuntimeError("no S_brawl_*.wav found (run beat-em-up-unreal/Tools/brawl_sfx_synth.py)")


def _import(items):
    tasks = []
    for src, dest, name in items:
        t = unreal.AssetImportTask()
        t.filename = src
        t.destination_path = dest
        t.destination_name = name
        t.automated = True
        t.replace_existing = True
        t.save = False
        tasks.append(t)
    unreal.AssetToolsHelpers.get_asset_tools().import_asset_tasks(tasks)


def _finish(path, looping):
    wave = unreal.EditorAssetLibrary.load_asset(path)
    if not wave:
        raise RuntimeError("import failed: " + path)
    wave.set_editor_property("looping", looping)
    unreal.EditorAssetLibrary.save_asset(path, only_if_is_dirty=False)
    unreal.log("BRAWL_AUDIO imported %s duration=%.2f looping=%s" % (path, wave.get_editor_property("duration"), wave.get_editor_property("looping")))


def build_sfx():
    wavs = sorted(glob.glob(os.path.join(_sfx_dir(), "S_brawl_*.wav")))
    _import([(w, DEST + "/SFX", os.path.splitext(os.path.basename(w))[0]) for w in wavs])
    for w in wavs:
        _finish("%s/SFX/%s" % (DEST, os.path.splitext(os.path.basename(w))[0]), False)


def build_music():
    src_dir = _audio_dir()
    tracks = [(src, name) for src, name in TRACKS if os.path.isfile(os.path.join(src_dir, src))]
    for src, name in TRACKS:
        if (src, name) not in tracks:
            unreal.log_warning("BRAWL_AUDIO skipped %s: no %s in %s" % (name, src, src_dir))
    _import([(os.path.join(src_dir, src), DEST, name) for src, name in tracks])
    for _, name in tracks:
        _finish("%s/%s" % (DEST, name), True)


def build():
    build_music()
    build_sfx()


if __name__ == "__main__":
    build_sfx() if "sfx" in sys.argv[1:] else build()
