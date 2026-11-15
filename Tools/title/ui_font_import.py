"""An Unreal Python job (jam_run.ps1 -Script ui_font_import.py): brings Bungee into the project the way Orbitron and
Rajdhani are there: a font face asset from the TTF (/Game/UI/Fonts/Bungee-Regular) and a runtime font that uses it
(/Game/UI/Fonts/F_Bungee), which is the name FTHUD.cpp and the UI kit load. The TTF is Tools/title/Bungee-Regular.ttf
(SIL Open Font License 1.1: Tools/title/OFL-Bungee.txt, Content/Data/Licenses/OFL-Bungee.txt).

The engine's font file importer makes the runtime font itself when it is asked to (BatchCreateFontAsset), but names
it <face>_Font; that one is duplicated to F_Bungee and never saved."""
import os
import traceback

import unreal

DEST = '/Game/UI/Fonts'
FACE = 'Bungee-Regular'
FONT = 'F_Bungee'


def say(text):
    unreal.log('JAM ' + text)


try:
    eal = unreal.EditorAssetLibrary
    ttf = os.environ.get('FT_BUNGEE_TTF') or os.path.join(unreal.Paths.convert_relative_path_to_full(unreal.Paths.project_dir()), 'Tools', 'title', 'Bungee-Regular.ttf')
    say('ttf %s exists=%s' % (ttf, os.path.exists(ttf)))
    if eal.does_asset_exist(DEST + '/' + FONT):
        say('%s is there already: nothing imported' % FONT)
    else:
        factory = unreal.FontFileImportFactory()
        factory.set_editor_property('batch_create_font_asset', unreal.BatchCreateFontAsset.YES)
        task = unreal.AssetImportTask()
        task.filename = ttf
        task.destination_path = DEST
        task.destination_name = FACE
        task.automated = True
        task.replace_existing = True
        task.save = True
        task.factory = factory
        unreal.AssetToolsHelpers.get_asset_tools().import_asset_tasks([task])
        say('imported: %s' % [str(p) for p in task.imported_object_paths])
        made = DEST + '/' + FACE + '_Font'
        say('face exists=%s  font made by the importer exists=%s' % (eal.does_asset_exist(DEST + '/' + FACE), eal.does_asset_exist(made)))
        font = eal.duplicate_asset(made, DEST + '/' + FONT)
        if not font:
            say('duplicate failed: renaming instead')
            eal.rename_asset(made, DEST + '/' + FONT)
        say('saved face=%s font=%s' % (eal.save_asset(DEST + '/' + FACE, False), eal.save_asset(DEST + '/' + FONT, False)))
    font = unreal.load_asset(DEST + '/' + FONT)
    say('%s class=%s cache=%s' % (FONT, font.get_class().get_name() if font else None, font.get_editor_property('font_cache_type') if font else None))
    for other in ('F_Rajdhani', 'F_Orbitron'):
        o = unreal.load_asset(DEST + '/' + other)
        say('%s class=%s cache=%s' % (other, o.get_class().get_name() if o else None, o.get_editor_property('font_cache_type') if o else None))
    say('DONE' if font else 'FAILED: no F_Bungee')
except Exception:
    for row in traceback.format_exc().splitlines():
        unreal.log_error('JAM ERROR ' + row)
unreal.SystemLibrary.quit_editor()
