"""Imports the brawl's audio into /Game/Brawl/Audio.

S_FightMusic = Matt's fight music (Docs/audio/matt-fight-music.mp3, 2:12), saved looping.
AFTBrawlMusic (Source/.../Brawl/FTBrawlMusic.cpp) plays it for the whole night.

Run in the editor (`import brawl_audio; brawl_audio.build()`) or headless:
  UnrealEditor-Cmd FullThrottleBrawl.uproject -run=pythonscript -script="<abs>/Content/Python/brawl_audio.py"
"""
import os

import unreal

DEST = "/Game/Brawl/Audio"
TRACKS = [("matt-fight-music.mp3", "S_FightMusic")]


def build():
    src_dir = os.path.join(unreal.Paths.convert_relative_path_to_full(unreal.Paths.project_dir()), "Docs", "audio")
    tasks = []
    for src, name in TRACKS:
        t = unreal.AssetImportTask()
        t.filename = os.path.join(src_dir, src)
        t.destination_path = DEST
        t.destination_name = name
        t.automated = True
        t.replace_existing = True
        t.save = False
        tasks.append(t)
    unreal.AssetToolsHelpers.get_asset_tools().import_asset_tasks(tasks)
    for _, name in TRACKS:
        path = "%s/%s" % (DEST, name)
        wave = unreal.EditorAssetLibrary.load_asset(path)
        if not wave:
            raise RuntimeError("import failed: " + path)
        wave.set_editor_property("looping", True)
        unreal.EditorAssetLibrary.save_asset(path, only_if_is_dirty=False)
        unreal.log("BRAWL_AUDIO imported %s duration=%.2f looping=%s" % (path, wave.get_editor_property("duration"), wave.get_editor_property("looping")))


if __name__ == "__main__":
    build()
