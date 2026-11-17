"""Import the brawl's stage drawings (breakable scenery states, onlookers) into /Game/Brawl/Stage.

A drawing is a 1024x1024 RGBA PNG named T_stage_<name>.png (its base at y 1000, its middle at x 512), cut from a
generated sheet by gen/cut_sheet.py (wrapper: C:/ftwork/final/brawl/code/stages/tools/cut_all.py). Each becomes the
texture /Game/Brawl/Stage/T_stage_<name> with the settings of an existing painted prop (read from
/Game/Brawl/Sprites/Props/T_prop_stool and copied: sRGB, never streamed, compression, texture group, clamping, mip
and filter settings). Nothing outside /Game/Brawl/Stage is touched. The game reads them in FTBrawlStage.cpp; the
sizes it needs are in Source/FullThrottle/Brawl/FTBrawlStageArt.inl (written by cut_all.py --inl).

Folder: the environment variable FT_STAGE_IMPORT, else C:/ftwork/final/brawl/code/stages/art/frames. Run headless:

    powershell -NoProfile -ExecutionPolicy Bypass -File C:\\ftwork\\tools\\run_job.ps1 `
        -Job C:\\ftwork\\wt\\brawl\\full-throttle-unreal\\Content\\Python\\brawl_stage_import.py `
        -Project C:\\ftwork\\wt\\brawl\\full-throttle-unreal -Tag s2_stage_import

Writes <folder>/../import-report.json and logs "FTBRAWL STAGEIMPORT imported=N refused=M".
"""
import json
import os
import re
from pathlib import Path

import unreal

FOLDER = Path(os.environ.get('FT_STAGE_IMPORT', '').strip() or 'C:/ftwork/final/brawl/code/stages/art/frames')
DEST = '/Game/Brawl/Stage'
MODEL = '/Game/Brawl/Sprites/Props/T_prop_stool'
COPIED = ('srgb', 'never_stream', 'compression_settings', 'lod_group', 'address_x', 'address_y',
          'mip_gen_settings', 'filter', 'lod_bias', 'max_texture_size', 'compression_no_alpha')


def main():
    al = unreal.EditorAssetLibrary
    report = {'folder': str(FOLDER), 'destination': DEST, 'settings_copied_from': MODEL, 'files': []}
    model = al.load_asset(MODEL)
    if not isinstance(model, unreal.Texture2D) or not FOLDER.is_dir():
        unreal.log_error('FTBRAWL STAGEIMPORT imported=0 refused=0 no model texture %s or no folder %s' % (MODEL, FOLDER))
        return
    wanted = {}
    for key in COPIED:
        try:
            wanted[key] = model.get_editor_property(key)
        except Exception:
            pass
    report['settings'] = {k: str(v) for k, v in wanted.items()}
    pattern = re.compile(r'T_stage_[a-z0-9]+(_[a-z0-9]+)*\.png')
    tasks, rows = [], []
    for png in sorted(FOLDER.glob('*.png')):
        row = {'file': png.name}
        report['files'].append(row)
        if not pattern.fullmatch(png.name):
            row['refused'] = 'name must be T_stage_<name>.png, lower case'
            continue
        task = unreal.AssetImportTask()
        task.filename = str(png)
        task.destination_path = DEST
        task.destination_name = png.stem
        task.automated = True
        task.replace_existing = True
        task.save = False
        tasks.append(task); rows.append(row)
    if tasks:
        unreal.AssetToolsHelpers.get_asset_tools().import_asset_tasks(tasks)
    for task, row in zip(tasks, rows):
        path = '%s/%s' % (DEST, task.destination_name)
        tex = al.load_asset(path)
        if not isinstance(tex, unreal.Texture2D):
            row['refused'] = 'the editor did not import it'
            continue
        size = (tex.blueprint_get_size_x(), tex.blueprint_get_size_y())
        if size != (1024, 1024):
            row['refused'] = 'must be 1024x1024, is %dx%d (asset deleted)' % size
            al.delete_asset(path)
            continue
        for key, value in wanted.items():
            tex.set_editor_property(key, value)
        if not al.save_loaded_asset(tex, only_if_is_dirty=False):
            row['refused'] = 'cannot save ' + path
            continue
        row['package'] = path
    done = [f for f in report['files'] if 'package' in f]
    bad = [f for f in report['files'] if 'refused' in f]
    (FOLDER.parent / 'import-report.json').write_text(json.dumps(report, indent=1), encoding='utf-8')
    (unreal.log_error if bad else unreal.log)('FTBRAWL STAGEIMPORT imported=%d refused=%d' % (len(done), len(bad)))
    for f in bad:
        unreal.log_error('FTBRAWL STAGEIMPORT refused %s: %s' % (f['file'], f['refused']))


main()
