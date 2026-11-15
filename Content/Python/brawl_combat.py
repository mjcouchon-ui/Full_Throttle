"""Import Brawl's deterministic impact cards and one-shot combat cues."""

from pathlib import Path

import unreal


SOUNDS = ('Jab', 'Finish', 'Weapon', 'Throw', 'Hurt', 'Clear')
CARDS = SOUNDS[:-1]


def build():
    project = Path(unreal.Paths.convert_relative_path_to_full(unreal.Paths.project_dir()))
    source = project / 'ArtSource' / 'CombatPolish'
    if not source.is_dir():
        source = project.parent / 'beat-em-up-unreal' / 'ArtSource' / 'CombatPolish'

    files = [(f'T_Brawl{name}', 'png', '/Game/Brawl/Art/Combat') for name in CARDS]
    files += [(f'S_Brawl{name}', 'wav', '/Game/Brawl/Audio/Polish') for name in SOUNDS]
    tasks = []
    for name, extension, folder in files:
        file = source / f'{name}.{extension}'
        if not file.is_file():
            raise RuntimeError(f'Missing combat source {file}')
        task = unreal.AssetImportTask()
        task.filename = str(file)
        task.destination_path = folder
        task.destination_name = name
        task.automated = True
        task.replace_existing = True
        task.save = False
        tasks.append(task)

    unreal.AssetToolsHelpers.get_asset_tools().import_asset_tasks(tasks)
    assets = []
    for (name, extension, folder), task in zip(files, tasks):
        path = f'{folder}/{name}'
        asset = unreal.EditorAssetLibrary.load_asset(path)
        expected = unreal.Texture2D if extension == 'png' else unreal.SoundWave
        if not task.imported_object_paths or not isinstance(asset, expected):
            raise RuntimeError(f'Failed to import {path}')
        if extension == 'png':
            asset.set_editor_property('srgb', True)
            asset.set_editor_property('never_stream', True)
        else:
            asset.set_editor_property('looping', False)
        if not unreal.EditorAssetLibrary.save_loaded_asset(asset, only_if_is_dirty=False):
            raise RuntimeError(f'Failed to save {path}')
        assets.append(path)
    return assets
