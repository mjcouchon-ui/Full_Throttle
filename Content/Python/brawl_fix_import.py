"""Re-import corrected brawl KEY drawings over their existing textures, keeping every texture setting.

brawl_s2_import.py only takes the in-betweens (T_<who>_s2_*). The key drawings (T_<who>_idle, T_punk_swalk01,
T_earl_bkneel ...) came in through brawl_2d.py, brawl_studio.py and brawl_boss_polish.py with settings of their own, so
a corrected key is brought in here: the PNG replaces the pixels of the texture that already exists and nothing else.

The list: the environment variable FT_FIX_IMPORT names a text file, one drawing per line,

    <path of the PNG>|/Game/Brawl/Sprites/<Who>/T_<who>_<frame>

(empty lines and lines starting with # are skipped). The texture must already exist: this script never makes a new
one and never touches a texture that is not on the list. Run headless through the job runner:

    $env:FT_FIX_IMPORT = "C:/ftwork/final/brawl/fixin/keys.txt"
    powershell -NoProfile -ExecutionPolicy Bypass -File C:\\ftwork\\tools\\run_job.ps1 `
        -Job C:\\ftwork\\wt\\brawl\\full-throttle-unreal\\Content\\Python\\brawl_fix_import.py `
        -Project C:\\ftwork\\wt\\brawl\\full-throttle-unreal -Tag fix_import

For every texture it reads the settings first (size, compression, filter, mips, sRGB, group and the rest of CHECKED),
imports with replace_existing, puts back any setting the import changed, reads them again and logs one line:

    FTBRAWL FIXIMPORT tex=<package> size=1024x1024 compression=... filter=... mip=... srgb=... group=... settings=unchanged
    FTBRAWL FIXIMPORT REFUSED <package>: <why>            (an error line; the others still run)

and at the end "FTBRAWL FIXIMPORT done imported=N refused=M". A report with the settings before and after is written
beside the list as <list>.report.json. To put one drawing back, run this script with the original PNG on the list (or
copy the original .uasset back while no editor is open).
"""
import json
import os
import re

import unreal

CHECKED = ('srgb', 'never_stream', 'compression_settings', 'lod_group', 'address_x', 'address_y', 'mip_gen_settings',
           'filter', 'lod_bias', 'max_texture_size', 'compression_no_alpha', 'compression_quality', 'power_of_two_mode',
           'downscale', 'defer_compression', 'lossy_compression_amount', 'virtual_texture_streaming', 'flip_green_channel',
           'preserve_border', 'adjust_brightness', 'adjust_saturation', 'adjust_vibrance', 'adjust_hue', 'chroma_key_texture',
           'global_force_mip_levels_to_be_resident', 'num_cinematic_mip_levels', 'mip_load_options', 'alpha_coverage_thresholds')


def settings_of(tex):
    got = {}
    for key in CHECKED:
        try:
            got[key] = tex.get_editor_property(key)
        except Exception:
            pass
    return got


def text(values):
    return {k: plain(v) for k, v in values.items()}


def size_of(tex):
    return [tex.blueprint_get_size_x(), tex.blueprint_get_size_y()]


def saved_size(package):
    """size of the texture as saved (the asset registry's Dimensions tag): a texture just loaded reports 32x32 until
    the editor has prepared it, so the size before the import cannot be asked of the texture itself"""
    data = unreal.EditorAssetLibrary.find_asset_data(package)
    tag = str(data.get_tag_value('Dimensions') or '')
    try:
        x, y = tag.lower().split('x')
        return [int(x), int(y)]
    except Exception:
        return [0, 0]


def plain(value):
    return re.sub(r' \(0x[0-9A-Fa-f]+\)', '', str(value))


def short(value):
    return str(value).split('.')[-1].replace('>', '').split(':')[0].strip()


def main():
    listing = os.environ.get('FT_FIX_IMPORT', '').strip()
    if not listing or not os.path.isfile(listing):
        unreal.log_error('FTBRAWL FIXIMPORT nothing to do: set FT_FIX_IMPORT to a list file (png|package per line)')
        return
    al = unreal.EditorAssetLibrary
    tools = unreal.AssetToolsHelpers.get_asset_tools()
    report, done, bad = [], 0, 0

    def refuse(row, why):
        row['refused'] = why
        unreal.log_error('FTBRAWL FIXIMPORT REFUSED %s: %s' % (row['package'], why))

    for raw in open(listing, encoding='utf-8'):
        line = raw.strip()
        if not line or line.startswith('#'):
            continue
        png, _, package = line.partition('|')
        png, package = png.strip(), package.strip()
        row = {'png': png, 'package': package}
        report.append(row)
        if not package.startswith('/Game/Brawl/Sprites/') or '_s2_' in package.rsplit('/', 1)[-1]:
            refuse(row, 'only key drawings under /Game/Brawl/Sprites (the in-betweens go through brawl_s2_import.py)'); bad += 1; continue
        if not os.path.isfile(png):
            refuse(row, 'no such PNG'); bad += 1; continue
        if not al.does_asset_exist(package):
            refuse(row, 'no existing texture: this script only replaces'); bad += 1; continue
        tex = al.load_asset(package)
        if not isinstance(tex, unreal.Texture2D):
            refuse(row, 'not a Texture2D'); bad += 1; continue
        size_before = saved_size(package)
        before = settings_of(tex)
        task = unreal.AssetImportTask()
        task.filename = png
        task.destination_path = package.rsplit('/', 1)[0]
        task.destination_name = package.rsplit('/', 1)[1]
        task.automated = True
        task.replace_existing = True
        task.save = False
        tools.import_asset_tasks([task])
        tex = al.load_asset(package)
        if not isinstance(tex, unreal.Texture2D):
            refuse(row, 'the editor did not import it'); bad += 1; continue
        put_back = []
        for key, value in before.items():
            try:
                if tex.get_editor_property(key) != value:
                    tex.set_editor_property(key, value)
                    put_back.append(key)
            except Exception:
                pass
        after, size_after = settings_of(tex), size_of(tex)
        row.update(size_before=size_before, size_after=size_after, settings_before=text(before), settings_after=text(after),
                   put_back=put_back)
        differ = [k for k in before if plain(before[k]) != plain(after.get(k))]
        if size_after != size_before:
            refuse(row, 'size was %dx%d, the PNG gave %dx%d (NOT saved; the asset on disk is still the old one)' % tuple(size_before + size_after)); bad += 1; continue
        if differ:
            refuse(row, 'settings differ after the import and could not be put back: %s (NOT saved)' % ', '.join(differ)); bad += 1; continue
        if not al.save_loaded_asset(tex, only_if_is_dirty=False):
            refuse(row, 'cannot save'); bad += 1; continue
        done += 1
        unreal.log('FTBRAWL FIXIMPORT tex=%s size=%dx%d compression=%s filter=%s mip=%s srgb=%s group=%s neverstream=%s settings=unchanged%s' % (
            package, size_after[0], size_after[1], short(after.get('compression_settings')), short(after.get('filter')),
            short(after.get('mip_gen_settings')), after.get('srgb'), short(after.get('lod_group')), after.get('never_stream'),
            (' (put back after the import: %s)' % ','.join(put_back)) if put_back else ''))
    with open(listing + '.report.json', 'w', encoding='utf-8') as f:
        json.dump(report, f, indent=1)
    (unreal.log_error if bad else unreal.log)('FTBRAWL FIXIMPORT done imported=%d refused=%d' % (done, bad))


main()
