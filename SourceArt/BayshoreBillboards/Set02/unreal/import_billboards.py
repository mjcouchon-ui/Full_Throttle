"""Optional UE 5.9 import of this pack. Creates assets only; never edits a map.

Use Unreal Editor > Execute Python Script on this file. Existing assets cause
an early stop so an artist's work cannot be silently overwritten.
"""
from pathlib import Path
import unreal

PACK = Path(__file__).resolve().parents[1]
DEST = '/Game/Maps/Bayshore/Billboards/Set02'
STEMS = ('BillableBill', 'LastDime', 'CaptainLiability', 'AlibiInn')


def enum_value(enum_class, label):
    key = label.replace('_', '').lower()
    for name in dir(enum_class):
        if name.replace('_', '').lower() == key:
            return getattr(enum_class, name)
    raise RuntimeError('Missing Unreal enum: {}.{}'.format(enum_class, label))


def run():
    eal = unreal.EditorAssetLibrary
    tools = unreal.AssetToolsHelpers.get_asset_tools()
    mel = unreal.MaterialEditingLibrary
    source_paths = [PACK / 'textures_2k' / ('T_BB_' + s + '_BC.png') for s in STEMS]
    texture_paths = [DEST + '/Textures/T_BB_' + s + '_BC' for s in STEMS]
    parent_path = DEST + '/Materials/M_BB_Printed'
    instance_paths = [DEST + '/Materials/MI_BB_' + s for s in STEMS]

    missing = [str(p) for p in source_paths if not p.is_file()]
    if missing:
        raise RuntimeError('Missing source files: ' + ', '.join(missing))
    existing = [p for p in texture_paths + [parent_path] + instance_paths if eal.does_asset_exist(p)]
    if existing:
        raise RuntimeError('No overwrite performed. Destination assets already exist: ' + ', '.join(existing))

    # Resolve required settings before creating anything.
    settings = {
        'compression_settings': enum_value(unreal.TextureCompressionSettings, 'TC_BC7'),
        'lod_group': enum_value(unreal.TextureGroup, 'TEXTUREGROUP_World'),
        'mip_gen_settings': enum_value(unreal.TextureMipGenSettings, 'TMGS_FromTextureGroup'),
        'address_x': enum_value(unreal.TextureAddress, 'TA_Clamp'),
        'address_y': enum_value(unreal.TextureAddress, 'TA_Clamp'),
        'srgb': True, 'never_stream': False, 'max_texture_size': 2048,
        'lod_bias': 0, 'virtual_texture_streaming': False,
    }
    eal.make_directory(DEST + '/Textures')
    eal.make_directory(DEST + '/Materials')
    textures = []
    for source, target in zip(source_paths, texture_paths):
        # Matches this project's existing Interchange texture import workflow.
        pipeline = unreal.InterchangeGenericTexturePipeline()
        pipeline.set_editor_property('asset_name', source.stem)
        pipeline.set_editor_property('detect_normal_map_texture', False)
        pipeline.set_editor_property('detect_long_lat_cubemap', False)
        pipeline.set_editor_property('file_extensions_to_import_as_long_lat_cubemap', set())
        stack = unreal.InterchangePipelineStackOverride()
        stack.add_pipeline(pipeline)
        task = unreal.AssetImportTask()
        for key, value in {
            'filename': str(source), 'destination_path': DEST + '/Textures',
            'destination_name': source.stem, 'automated': True,
            'replace_existing': False, 'save': False, 'options': stack,
        }.items():
            task.set_editor_property(key, value)
        tools.import_asset_tasks([task])
        texture = unreal.load_asset(target)
        if not isinstance(texture, unreal.Texture2D):
            raise RuntimeError('Expected Texture2D at ' + target)
        for key, value in settings.items():
            texture.set_editor_property(key, value)
        if not eal.save_loaded_asset(texture, only_if_is_dirty=False):
            raise RuntimeError('Texture save failed: ' + target)
        textures.append(texture)

    material = tools.create_asset('M_BB_Printed', DEST + '/Materials', unreal.Material, unreal.MaterialFactoryNew())
    if material is None:
        raise RuntimeError('Could not create billboard material')
    material.set_editor_property('two_sided', False)
    sample = mel.create_material_expression(material, unreal.MaterialExpressionTextureSampleParameter2D, -400, 0)
    sample.set_editor_property('parameter_name', 'BillboardTexture')
    sample.set_editor_property('texture', textures[0])
    mel.connect_material_property(sample, 'RGB', unreal.MaterialProperty.MP_BASE_COLOR)
    roughness = mel.create_material_expression(material, unreal.MaterialExpressionConstant, -250, 200)
    roughness.set_editor_property('r', 0.75)
    mel.connect_material_property(roughness, '', unreal.MaterialProperty.MP_ROUGHNESS)
    metallic = mel.create_material_expression(material, unreal.MaterialExpressionConstant, -250, 300)
    metallic.set_editor_property('r', 0.0)
    mel.connect_material_property(metallic, '', unreal.MaterialProperty.MP_METALLIC)
    mel.recompile_material(material)
    if not eal.save_loaded_asset(material, only_if_is_dirty=False):
        raise RuntimeError('Material save failed')
    for stem, texture in zip(STEMS, textures):
        instance = tools.create_asset('MI_BB_' + stem, DEST + '/Materials', unreal.MaterialInstanceConstant, unreal.MaterialInstanceConstantFactoryNew())
        if instance is None:
            raise RuntimeError('Could not create material instance for ' + stem)
        mel.set_material_instance_parent(instance, material)
        mel.set_material_instance_texture_parameter_value(instance, 'BillboardTexture', texture)
        if not eal.save_loaded_asset(instance, only_if_is_dirty=False):
            raise RuntimeError('Material instance save failed: ' + stem)
    unreal.log('Bayshore Set02: imported four 2K textures, one parent material, four instances. Map unchanged.')


if __name__ == '__main__':
    run()
