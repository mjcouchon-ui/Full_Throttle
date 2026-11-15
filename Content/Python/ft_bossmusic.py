"""3.8 boss music (owner's queue, 2026-11-15): imports the two boss tracks, saved looping.

  /Game/Audio/Music/M_BossVehicle  = SourceArt/Music/vehicle boss music.mp3
      AFTGameMode::SetBossMusic plays it from a race boss's arrival until the replay (FTGameMode.cpp).
  The nights' boss track (same Bit-Reduced Boss Battle) is Keenan's /Game/Brawl/Audio/S_BossMusic
      (beat-em-up-unreal/Docs/audio/boss-battle.mp3 via brawl_audio.py; Brawl/FTBrawlMusic.cpp).

Run in the editor (not in PIE): import ft_bossmusic; ft_bossmusic.build()
UE 5.9 imports .mp3 directly (no ffmpeg needed).
"""
import os

import unreal

TRACKS = [
    ("vehicle boss music.mp3", "/Game/Audio/Music", "M_BossVehicle"),
]


def build():
    src_dir = os.path.join(unreal.Paths.convert_relative_path_to_full(unreal.Paths.project_dir()), "SourceArt", "Music")
    tasks = []
    for src, dest, name in TRACKS:
        t = unreal.AssetImportTask()
        t.filename = os.path.join(src_dir, src)
        t.destination_path = dest
        t.destination_name = name
        t.automated = True
        t.replace_existing = True
        t.save = False
        tasks.append(t)
    unreal.AssetToolsHelpers.get_asset_tools().import_asset_tasks(tasks)
    out = []
    for _, dest, name in TRACKS:
        path = "%s/%s" % (dest, name)
        wave = unreal.EditorAssetLibrary.load_asset(path)
        if not wave:
            raise RuntimeError("import failed: " + path)
        wave.set_editor_property("looping", True)
        unreal.EditorAssetLibrary.save_asset(path, only_if_is_dirty=False)
        line = "FT_BOSSMUSIC imported %s duration=%.2f looping=%s" % (path, wave.get_editor_property("duration"), wave.get_editor_property("looping"))
        unreal.log(line)
        out.append(line)
    return out


if __name__ == "__main__":
    build()
