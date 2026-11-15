"""Apply the visual-art pass to 3.3 vehicle bodies without restoring baked-in wheels.

Run once through Unreal's ExecutePythonScript on the merged checkout. Separate
rotors, wheel rigs, Tesla attachments and clown props retain their existing assets.
"""
import traceback
import unreal
import ft_v33


def rebuild():
    jobs = ft_v33.split_jobs()
    allowed = {path for group in jobs.values() for path, _, _ in group}
    allowed.add('/Game/Characters/Bosses/SM_Tanker')
    allowed.add('/Game/Characters/Choppers/SM_Heli')
    allowed.add('/Game/Characters/Syndicate/SM_Henchman')
    for level in range(1, 8):
        for name in ('SM_Heli', 'SM_Henchman'):
            allowed.add('/Game/Characters/Themed/L%d/%s' % (level, name))

    original_save = ft_v33.save_mesh
    saved = set()

    def save_body(meshes, path, *args, **kwargs):
        if path not in allowed:
            return unreal.EditorAssetLibrary.load_asset(path)
        result = original_save(meshes, path, *args, **kwargs)
        if not result:
            raise RuntimeError('Vehicle art was not saved: ' + path)
        saved.add(path)
        unreal.log('FTINTEGRATE saved ' + path)
        return result

    ft_v33.save_mesh = save_body
    try:
        for group in jobs:
            ft_v33.export_split(group)
        ft_v33.export_heli()
        ft_v33.export_swat()
        ft_v33.export_tesla()
    finally:
        ft_v33.save_mesh = original_save
    missing = sorted(allowed - saved)
    if missing:
        raise RuntimeError('Missing regenerated bodies: ' + ', '.join(missing))
    unreal.log('FTINTEGRATE success bodies=%d' % len(saved))


try:
    rebuild()
except Exception:
    unreal.log_error('FTINTEGRATE failed\n' + traceback.format_exc())
finally:
    unreal.SystemLibrary.quit_editor()
