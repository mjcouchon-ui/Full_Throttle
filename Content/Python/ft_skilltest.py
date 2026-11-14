"""2.3 play-test helper (editor, over the Unreal MCP connector): the reel's CONTINUE button and the garage.
Start PIE with the editor launched as -ftauto -ftwrap=40 (the autopilot drives, the level wraps after 40 s),
then drive the screens with the game's own input hooks and read the 'FTFLOW'/'FTSKILLS' lines in the log:
    import ft_skilltest as t; t.press('SpaceBar'); t.click(640, 360); t.shot('reel')"""
import unreal


def _world():
    return unreal.get_editor_subsystem(unreal.UnrealEditorSubsystem).get_game_world()


def cmd(c):
    w = _world()
    if w is None:
        return 'not in PIE'
    unreal.SystemLibrary.execute_console_command(w, c)
    return c


def press(key):
    """Press and release a key, mouse or pad button through the player's input (ft.Press)."""
    return cmd('ft.Press %s' % key)


def click(x, y, right=False):
    """Move the cursor to viewport pixel x,y and click (ft.Click)."""
    return cmd('ft.Click %d %d%s' % (x, y, ' Right' if right else ''))


def viewport():
    w = _world()
    if w is None:
        return None
    s = unreal.WidgetLayoutLibrary.get_viewport_size(w)
    return (int(s.x), int(s.y))


def shot():
    """Screenshot of the PIE viewport with the HUD (the canvas) drawn on it: 'shot showui' writes
    Saved/Screenshots/WindowsEditor/ScreenShotNNNNN.png (HighResShot leaves the HUD out)."""
    cmd('t.IdleWhenNotForeground 0')
    return cmd('shot showui')
