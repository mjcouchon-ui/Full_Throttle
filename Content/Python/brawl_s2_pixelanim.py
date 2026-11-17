"""FULL THROTTLE brawl, stage 2 fix round: the fighter sprite materials say that their picture changes without moving.

A fighter is a flat card whose drawing is swapped up to 30 times a second. The renderer's temporal anti-aliasing keeps
history per pixel and, with no motion to explain the change, blended the previous drawing into the new one: every
in-between came out smeared in the game (seen in the stage 2 review films). "Has Pixel Animation" is the engine's
material switch for exactly this (a flipbook on still geometry). Nothing else in the materials is touched.

Headless:
  powershell -NoProfile -ExecutionPolicy Bypass -File C:\ftwork\tools\run_job.ps1 -Job <abs>/Content/Python/brawl_s2_pixelanim.py -Project <project> -Tag s2_pixelanim
Log line per material: FTPEOPLE FTBRAWL PIXELANIM material=<name> blend=<mode> before=<bool> after=<bool> saved=<bool>
(the FTPEOPLE word only makes the runner print the line)
"""
import unreal

AL = unreal.EditorAssetLibrary
MEL = unreal.MaterialEditingLibrary
for name in ('M_BrawlSprite', 'M_BrawlEnemy', 'M_BrawlEnemyV'):
    path = '/Game/Brawl/Materials/' + name
    m = AL.load_asset(path)
    if not m:
        unreal.log('FTPEOPLE FTBRAWL PIXELANIM material=%s missing' % name)
        continue
    try:
        before = m.get_editor_property('has_pixel_animation')
        m.set_editor_property('has_pixel_animation', True)
        MEL.recompile_material(m)
        after = m.get_editor_property('has_pixel_animation')
        saved = AL.save_asset(path, only_if_is_dirty=False)
        unreal.log('FTPEOPLE FTBRAWL PIXELANIM material=%s blend=%s before=%s after=%s saved=%s' % (name, m.get_editor_property('blend_mode'), before, after, saved))
    except Exception as e:
        unreal.log('FTPEOPLE FTBRAWL PIXELANIM material=%s failed=%s' % (name, e))
