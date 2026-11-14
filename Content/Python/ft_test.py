"""Play-test helper (editor, over the Unreal MCP connector): start PIE with the autopilot and test cvars.
import ft_test; ft_test.go(level=0, arcade=63, shots=5)   then read 'FTTEL'/'FTCAR' lines in the log."""
import glob
import os
import unreal

MAPS = ['/Game/Maps/Bayshore/L_Bayshore', '/Game/Maps/ChemicalWorks/L_ChemicalWorks', '/Game/Maps/Nevada/L_Nevada']


def go(level=0, arcade=63, shots=5.0, autopilot=1):
    les = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
    if les.is_in_play_in_editor():
        les.editor_request_end_play()
        return 'was running: stopped, call again'
    for f in glob.glob(os.path.join(unreal.Paths.project_saved_dir(), 'Shots', 'ft_*.png')):
        os.remove(f)
    world = unreal.get_editor_subsystem(unreal.UnrealEditorSubsystem).get_editor_world()
    if not world.get_path_name().startswith(MAPS[level]):
        les.load_level(MAPS[level])
    for c in ('ft.Autopilot %d' % autopilot, 'ft.Shots %g' % shots, 'ft.Arcade %d' % arcade):
        unreal.SystemLibrary.execute_console_command(None, c)
    les.editor_request_begin_play()
    return 'PIE level %d arcade %d' % (level, arcade)


def stop():
    unreal.get_editor_subsystem(unreal.LevelEditorSubsystem).editor_request_end_play()
    return 'stopped'
