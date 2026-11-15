"""Regenerate the integrated enemy visuals in the Full Throttle Unreal 5.9 editor.

Run `import ft_enemy_overhaul; ft_enemy_overhaul.rebuild()` in the editor's Python console.
Vehicle bodies keep the checked-in wheel layout and reuse their original packages.
Copy the Brawl material to the standalone project or generate it there with
`import brawl_2d; brawl_2d.m_enemy()`.
"""

import unreal

import brawl_2d
import ft_bikes
import ft_enemy_vehicles
import ft_swat_overhaul


def rebuild():
    paths = ft_bikes.export() + ft_swat_overhaul.export() + ft_enemy_vehicles.rebuild()
    missing = [path for path in paths if not unreal.EditorAssetLibrary.does_asset_exist(path)]
    if missing:
        raise RuntimeError('Enemy asset exports missing: ' + ', '.join(missing))
    material = brawl_2d.m_enemy()
    if not material:
        raise RuntimeError('Brawl opponent material export failed')
    return paths + [material.get_path_name().split('.', 1)[0]]
