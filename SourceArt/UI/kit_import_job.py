# -*- coding: utf-8 -*-
"""Queue job: import the UI kit's 56 textures, then let the game module look for them.

  1. Content/Python/ft_ui_kit_import.run(): SourceArt/UI/Kit/T_UI_*.png -> /Game/UI/Kit (textures only; the Bungee
     font is already in the project and is not touched).
  2. The console command FTSkinPreload (FTHUDSkin.cpp): the log then holds
     "FT SKIN kit textures=56 of 56 bungee=..." when every name the skin asks for is there.

Run:  powershell -File SourceArt/Jam/unreal/jam_run.ps1 -Checkout <project> -Script <project>/SourceArt/UI/kit_import_job.py
      (JAM_ONLY=Icons,Digits in -Env imports only the textures whose names hold one of those fragments)
The whole log: <project>/Saved/AgentTemp/kit_import_job.log
"""
import os
import sys
import traceback
import unreal


def log(*a):
    print("JAM KITJOB", *[str(x) for x in a], flush=True)


try:
    project = unreal.Paths.convert_relative_path_to_full(unreal.Paths.project_dir())
    scripts = os.path.join(project, "Content", "Python")
    if scripts not in sys.path:
        sys.path.insert(0, scripts)
    try:
        import ft_ui_kit_import
        only = [n for n in os.environ.get("JAM_ONLY", "").split(",") if n] or None
        report = ft_ui_kit_import.run(only)
        log("imported", report["textures"], "textures,", "%.2f MB as RGBA8" % (report["bytes"] / 1048576.0))
    except Exception:  # noqa
        log("ERR import:", traceback.format_exc()[-3000:].replace("\n", " | "))
    # whatever the import said: count what is there, and let the game module look for every name its table asks for
    made = list(unreal.EditorAssetLibrary.list_assets("/Game/UI/Kit", recursive=False, include_folder=False))
    log("assets in /Game/UI/Kit:", len(made))
    unreal.SystemLibrary.execute_console_command(unreal.EditorLevelLibrary.get_editor_world(), "FTSkinPreload")
    log("FTSkinPreload sent: its answer is the log line 'FT SKIN kit textures=N of 56'")
except Exception:  # noqa
    log("ERR", traceback.format_exc()[-3000:].replace("\n", " | "))
finally:
    unreal.SystemLibrary.quit_editor()
