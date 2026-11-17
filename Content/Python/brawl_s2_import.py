"""Import new brawl drawings for one or more characters into /Game/Brawl/Sprites/<Who>.

A drawing is a 1024x1024 RGBA PNG named T_<who>_s2_<state>_<nn>.png (feet at y 1000, chest at x 512), in a
folder per character. Each becomes the texture /Game/Brawl/Sprites/<Who>/T_<who>_s2_<state>_<nn> with the same
settings as that character's existing T_<who>_idle (read from it and copied: sRGB, never streamed, compression,
texture group, clamping, mip and filter settings). Existing textures that are not named _s2_ are never touched.

Which folders: the environment variable FT_S2_IMPORT, "Who=folder;Who2=folder2". Without it, every
C:/ftwork/final/brawl/art/<Who>/frames that exists. Run headless through the job runner:

    $env:FT_S2_IMPORT = "Waiter=C:/ftwork/final/brawl/art/Waiter/frames"
    powershell -NoProfile -ExecutionPolicy Bypass -File C:\\ftwork\\tools\\run_job.ps1 `
        -Job C:\\ftwork\\wt\\brawl\\full-throttle-unreal\\Content\\Python\\brawl_s2_import.py `
        -Project C:\\ftwork\\wt\\brawl\\full-throttle-unreal -Tag s2_import_Waiter

Writes <folder>/../import-report.json (per file: package, size, settings, or the reason it was refused) and logs
one line "FTBRAWL S2IMPORT who=... imported=N refused=M". A refused file stops nothing else. Then the chart file
Source/FullThrottle/Brawl/Charts/FTBrawlChart_<Who>.inl decides when each drawing is shown (compile after editing).
"""
import json
import os
import re
from pathlib import Path

import unreal

ART_ROOT = Path('C:/ftwork/final/brawl/art')
COPIED = ('srgb', 'never_stream', 'compression_settings', 'lod_group', 'address_x', 'address_y',
          'mip_gen_settings', 'filter', 'lod_bias', 'max_texture_size', 'compression_no_alpha')


def jobs():
    spec = os.environ.get('FT_S2_IMPORT', '').strip()
    if spec:
        out = []
        for part in spec.split(';'):
            if part.strip():
                who, folder = part.split('=', 1)
                out.append((who.strip(), Path(folder.strip())))
        return out
    return [(d.name, d / 'frames') for d in sorted(ART_ROOT.iterdir()) if (d / 'frames').is_dir()] if ART_ROOT.is_dir() else []


def settings_of(tex):
    got = {}
    for key in COPIED:
        try:
            got[key] = tex.get_editor_property(key)
        except Exception:
            pass
    return got


def import_one(who, folder):
    al = unreal.EditorAssetLibrary
    lo = who.lower()
    dest = '/Game/Brawl/Sprites/' + who
    model = al.load_asset('%s/T_%s_idle' % (dest, lo))
    report = {'who': who, 'folder': str(folder), 'destination': dest, 'files': []}
    if not isinstance(model, unreal.Texture2D):
        report['error'] = 'no existing texture %s/T_%s_idle to copy the settings from' % (dest, lo)
        return report
    wanted = settings_of(model)
    report['settings_copied_from'] = '%s/T_%s_idle' % (dest, lo)
    report['settings'] = {k: str(v) for k, v in wanted.items()}
    pattern = re.compile(r'T_%s_s2_[a-z0-9]+(_[a-z0-9]+)*\.png' % re.escape(lo))
    tasks, rows = [], []
    for png in sorted(folder.glob('*.png')):
        row = {'file': png.name}
        report['files'].append(row)
        if not pattern.fullmatch(png.name):
            row['refused'] = 'name must be T_%s_s2_<state>_<nn>.png, lower case' % lo
            continue
        task = unreal.AssetImportTask()
        task.filename = str(png)
        task.destination_path = dest
        task.destination_name = png.stem
        task.automated = True
        task.replace_existing = True
        task.save = False
        tasks.append(task); rows.append(row)
    if tasks:
        unreal.AssetToolsHelpers.get_asset_tools().import_asset_tasks(tasks)
    for task, row in zip(tasks, rows):
        path = '%s/%s' % (dest, task.destination_name)
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
        row['size'] = list(size)
    return report


def main():
    todo = jobs()
    if not todo:
        unreal.log_error('FTBRAWL S2IMPORT nothing to do: set FT_S2_IMPORT or make %s/<Who>/frames' % ART_ROOT)
        return
    for who, folder in todo:
        if not folder.is_dir():
            unreal.log_error('FTBRAWL S2IMPORT who=%s no folder %s' % (who, folder))
            continue
        report = import_one(who, folder)
        done = [f for f in report['files'] if 'package' in f]
        bad = [f for f in report['files'] if 'refused' in f]
        (folder.parent / 'import-report.json').write_text(json.dumps(report, indent=1), encoding='utf-8')
        line = 'FTBRAWL S2IMPORT who=%s imported=%d refused=%d %s' % (who, len(done), len(bad), report.get('error', ''))
        (unreal.log_error if bad or 'error' in report else unreal.log)(line)
        for f in bad:
            unreal.log_error('FTBRAWL S2IMPORT refused %s: %s' % (f['file'], f['refused']))


main()
