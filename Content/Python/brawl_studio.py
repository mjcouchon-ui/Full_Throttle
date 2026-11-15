"""Batch import the second Brawl art pass without replacing pre-existing art.

Run with -AllowCommandletRendering when using -run=pythonscript, so the new
effect material can compile. Run only after the source manifests are ready.
FT_BRAWL_STUDIO_REPORT is the
optional output path for the exact package allowlist to mirror and stage.
"""
import json
import os
from pathlib import Path
import unreal


def material():
    al = unreal.EditorAssetLibrary
    mel = unreal.MaterialEditingLibrary
    folder = '/Game/Brawl/Materials'
    name = 'M_BrawlV2Accent'
    path = folder + '/' + name
    mat = al.load_asset(path) if al.does_asset_exist(path) else unreal.AssetToolsHelpers.get_asset_tools().create_asset(
        name, folder, unreal.Material, unreal.MaterialFactoryNew())
    mel.delete_all_material_expressions(mat)
    mat.set_editor_property('blend_mode', unreal.BlendMode.BLEND_TRANSLUCENT)
    mat.set_editor_property('shading_model', unreal.MaterialShadingModel.MSM_UNLIT)
    mat.set_editor_property('two_sided', True)
    def node(cls, x, y, **props):
        result = mel.create_material_expression(mat, cls, x, y)
        for key, value in props.items(): result.set_editor_property(key, value)
        return result
    tex = node(unreal.MaterialExpressionTextureSampleParameter2D, -600, 0, parameter_name='Tex',
               texture=al.load_asset('/Game/Brawl/Art/Studio/T_BrawlV2Dust'))
    glow = node(unreal.MaterialExpressionScalarParameter, -600, 200, parameter_name='Glow', default_value=1.0)
    opacity = node(unreal.MaterialExpressionScalarParameter, -600, 350, parameter_name='Opacity', default_value=1.0)
    color = node(unreal.MaterialExpressionMultiply, -300, 0)
    alpha = node(unreal.MaterialExpressionMultiply, -300, 200)
    mel.connect_material_expressions(tex, 'RGB', color, 'A')
    mel.connect_material_expressions(glow, '', color, 'B')
    mel.connect_material_expressions(tex, 'A', alpha, 'A')
    mel.connect_material_expressions(opacity, '', alpha, 'B')
    mel.connect_material_property(color, '', unreal.MaterialProperty.MP_EMISSIVE_COLOR)
    mel.connect_material_property(alpha, '', unreal.MaterialProperty.MP_OPACITY)
    mel.recompile_material(mat)
    if not al.save_loaded_asset(mat, only_if_is_dirty=False): raise RuntimeError('Accent material save failed')
    return path


def build():
    project = Path(unreal.Paths.convert_relative_path_to_full(unreal.Paths.project_dir()))
    root = project.parent
    source = root/'beat-em-up-unreal/ArtSource/studio-pass-v2'
    manifests = sorted(source.rglob('manifest.json'))
    if len(manifests) < 3: raise RuntimeError('Player, boss and effects source manifests must all exist')
    rows = []
    for path in manifests:
        data = json.loads(path.read_text(encoding='utf-8-sig'))
        if isinstance(data, dict): data = data.get('assets', data.get('frames', []))
        for row in data:
            if 'destination' not in row: raise RuntimeError(f'Invalid import manifest {path}: {row}')
            rows.append(row)
    for name in ('Land','Slash','Pickup','Glass','Wood'):
        rows.append({'source':f'beat-em-up-unreal/ArtSource/studio-pass-v2/effects/S_BrawlV2{name}.wav',
                     'destination':f'/Game/Brawl/Audio/Studio/S_BrawlV2{name}', 'type':'sound'})
    destinations = [row['destination'] for row in rows]
    if len(set(destinations)) != len(destinations): raise RuntimeError('Duplicate import destinations')
    tasks = []
    for row in rows:
        file = Path(row['source'])
        if not file.is_absolute(): file = root / file
        if not file.is_file(): raise RuntimeError(f'Missing authored source {file}')
        folder, name = row['destination'].rsplit('/', 1)
        task = unreal.AssetImportTask()
        task.filename = str(file)
        task.destination_path = folder
        task.destination_name = name
        task.automated = True
        task.replace_existing = True
        task.save = False
        tasks.append(task)
    unreal.AssetToolsHelpers.get_asset_tools().import_asset_tasks(tasks)
    for task, row in zip(tasks, rows):
        asset = unreal.EditorAssetLibrary.load_asset(row['destination'])
        expected = unreal.SoundWave if row.get('type') == 'sound' else unreal.Texture2D
        if not task.imported_object_paths or not isinstance(asset, expected):
            raise RuntimeError(f'Import failed: {row["destination"]}')
        if isinstance(asset, unreal.Texture2D):
            asset.set_editor_property('srgb', True)
            asset.set_editor_property('never_stream', True)
            asset.set_editor_property('compression_settings', unreal.TextureCompressionSettings.TC_DEFAULT)
            asset.set_editor_property('lod_group', unreal.TextureGroup.TEXTUREGROUP_CHARACTER)
            asset.set_editor_property('address_x', unreal.TextureAddress.TA_CLAMP)
            asset.set_editor_property('address_y', unreal.TextureAddress.TA_CLAMP)
        else: asset.set_editor_property('looping', False)
        if not unreal.EditorAssetLibrary.save_loaded_asset(asset, only_if_is_dirty=False):
            raise RuntimeError(f'Cannot save {row["destination"]}')
    destinations.append(material())
    report = os.environ.get('FT_BRAWL_STUDIO_REPORT')
    if report: Path(report).write_text(json.dumps(sorted(destinations), indent=2), encoding='utf-8')
    unreal.log(f'FTBRAWL STUDIO imported {len(destinations)} packages from {len(manifests)} manifests')
    return destinations
