"""Build a portable, labeled review from unmodified Unreal viewport captures.

Only presentation derivatives are resized/desaturated/compressed. Runtime art and
raw PNGs are never changed. Requires ffmpeg on PATH; Python standard library only.
"""
import argparse
import hashlib
import html
import json
import re
import subprocess
from pathlib import Path

NAMES = ['Bayshore', 'Chemical Plant', 'Nevada Desert', 'Volcano',
         'Theme Park', 'Ice Mountain', 'Under the Sea', 'City Center']
FONT = "C\\:/Windows/Fonts/arial.ttf"
RECORDS = []
PREVIOUS = {}


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def ffmpeg(args):
    subprocess.run(['ffmpeg', '-hide_banner', '-loglevel', 'error', '-y',
                    '-threads', '2', *map(str, args)], check=True)


def grid(items, target, cols=2, gray=False):
    if not all(p.exists() for p, _ in items):
        return False
    sources = [{'path': str(p), 'sha256': sha(p), 'label': l} for p, l in items]
    previous = PREVIOUS.get(target.name)
    if (target.exists() and previous and previous.get('sources') == sources
            and previous.get('grayscale') == gray and previous.get('sha256') == sha(target)):
        RECORDS.append(previous)
        return True
    args, filters, layout = [], [], []
    for i, (p, label) in enumerate(items):
        args += ['-i', p]
        label = label.replace(':', ' -').replace("'", '')
        filters.append(f"[{i}:v]scale=860:360:force_original_aspect_ratio=decrease,pad=860:360:(ow-iw)/2:(oh-ih)/2" + (',hue=s=0' if gray else '') +
                       f",drawtext=fontfile='{FONT}':text='{label}':fontsize=18:"
                       f"fontcolor=white:x=10:y=10:box=1:boxcolor=black@0.75[v{i}]")
        layout.append(f'{(i % cols) * 860}_{(i // cols) * 360}')
    filters.append(''.join(f'[v{i}]' for i in range(len(items))) +
                   f"xstack=inputs={len(items)}:layout={'|'.join(layout)}:fill=black[out]")
    ffmpeg([*args, '-filter_complex_threads', '2', '-filter_complex', ';'.join(filters),
            '-map', '[out]', '-frames:v', '1', '-q:v', '2', target])
    RECORDS.append({'file': target.name, 'sha256': sha(target), 'type': 'review composite',
                    'grayscale': gray, 'sources': sources})
    return True


def shot(root, level, quality, number):
    return root / f'L{level}-Q{quality}' / 'shots' / f'ft_L{level}_{number:03d}.png'


def native_frame(source, target):
    if not source.exists():
        return
    source_hash = sha(source)
    previous = PREVIOUS.get(target.name)
    if not (target.exists() and previous and previous.get('source_sha256') == source_hash
            and previous.get('sha256') == sha(target)):
        ffmpeg(['-i', source, '-frames:v', '1', '-c:v', 'libwebp', '-lossless', '1',
                '-compression_level', '6', '-pix_fmt', 'bgra', target])
    RECORDS.append({'file': target.name, 'sha256': sha(target), 'type': 'native-resolution lossless WebP',
                    'source': str(source), 'source_sha256': source_hash})


def events(log):
    return [line.split('LogFT: Display: ', 1)[1] for line in log.read_text(errors='replace').splitlines()
            if 'LogFT: Display: ' in line and 'FTBENCH ' not in line]


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--captures', type=Path, required=True)
    ap.add_argument('--output', type=Path, required=True)
    args = ap.parse_args()
    root, out = args.captures, args.output
    out.mkdir(parents=True, exist_ok=True)
    if (out / 'manifest.json').exists():
        PREVIOUS.update({r['file']: r for r in json.loads((out / 'manifest.json').read_text(encoding='utf-8'))['derivatives']})
    sections, runs = [], []
    before, after = root / 'baseline-270', root / 'after-270'
    for q in [3, 0]:
        for level, name in enumerate(NAMES):
            quality = 'Epic' if q == 3 else 'Low'
            staged = root / 'staged-280'
            if not staged.exists():
                staged = root / f'staged-280-Q{q}'
            items = []
            for n in [16, 32, 48, 64, 80, 96, 108]:
                for state, folder in [('Before', before), ('After', after)]:
                    items.append((shot(folder, level, q, n), f'{state} / {name} / {quality} / frame {n:03d}'))
            dest = out / f'natural-L{level}-Q{q}.jpg'
            if grid(items, dest):
                sections.append((f'{name} / {quality} / same-frame 2.7 comparison', dest.name,
                                 'Before left, after right. Raw capture numbers match; simulation is fixed at 60 Hz.'))
            for state, folder in [('before-2.7', before), ('after-2.7', after),
                                  ('staged-2.8', staged)]:
                run = folder / f'L{level}-Q{q}'
                if (run / 'manifest.json').exists():
                    entry = json.loads((run / 'manifest.json').read_text(encoding='utf-8-sig'))
                    entry.update(set=state, png_count=len(list((run / 'shots').glob('*.png'))))
                    log = run / 'game.log'
                    entry['normal_exit'] = log.exists() and 'LogExit: Exiting.' in log.read_text(errors='replace')
                    if state == 'after-2.7' and entry['normal_exit']:
                        entry['gameplay_log_matches_baseline'] = events(log) == events(before / f'L{level}-Q{q}' / 'game.log')
                    runs.append(entry)
            dest = out / f'staged-L{level}-Q{q}.jpg'
            stages = [(2, '100 m'), (10, 'approaching'), (17, 'close'), (20, 'collection demonstration')]
            if grid([(shot(staged, level, q, n), f'{name} / {quality} / {label} / frame {n:03d}')
                     for n, label in stages], dest):
                native_frame(shot(staged, level, q, 2), out / f'far-native-L{level}-Q{q}.webp')
                sections.append((f'{name} / {quality} / 2.8 inspection', dest.name,
                                 'Staged visual proxies: 121 mph approach and collection FX. Not a reward or timing test.'))
    for q in [3, 0]:
        for level in [0, 1]:
            dest = out / f'turntable-L{level}-Q{q}.jpg'
            items = [(shot(root / 'turntables-v2', level, q, n), f'{NAMES[level]} / Q{q} / frame {n:03d}')
                     for n in [2, 10, 18, 26]]
            if grid(items, dest):
                sections.append((f'{NAMES[level]} / Q{q} / turntable on 2.7', dest.name,
                                 'Actual production light and cel pass. End-on overlap is shown rather than hidden.'))
    items = [(shot(root / 'turntables-v2', 0, 3, n), f'Grayscale / Bayshore / Epic / frame {n:03d}') for n in [2, 10, 18, 26]]
    if grid(items, out / 'grayscale-close.jpg', gray=True):
        sections.append(('Grayscale form review', 'grayscale-close.jpg',
                         'Desaturated copies of actual captures; silhouettes and bright/dark separation can be compared without color.'))
    art = out.parent
    refs = [(art / 'concepts/powerups-directions-v01.png', 'Original concepts / Justin chose R1 and M2'),
            (shot(root / 'turntables-v2', 0, 3, 2), 'Actual game / R1 and M2 / daytime'),
            (art / 'references/03-kestrel-body.png', 'Existing KESTREL / angular planes and navy breaks'),
            (shot(root / 'turntables-v2', 1, 3, 2), 'Actual game / R1 and M2 / night'),
            (art / 'references/01-current-missile-crate.png', 'Original missile crate'),
            (art / 'references/02-current-repair-kit.png', 'Original repair cube')]
    if grid(refs, out / 'reference-contact.jpg'):
        sections.insert(0, ('Runtime models against approved references', 'reference-contact.jpg',
                            'Concept illustration and existing game assets are labeled separately from actual runtime captures.'))
    for q in [3, 0]:
        far = [(shot(root / 'far-v2', level, q, 2), f'{NAMES[level]} / Q{q} / exact 100 m') for level in [0, 1]]
        if grid(far, out / f'far-Q{q}.jpg'):
            sections.append((f'100 m / Q{q}', f'far-Q{q}.jpg', 'Uncropped frames at 86 degree horizontal field of view.'))
    for q in [3, 0]:
        quality = 'Epic' if q == 3 else 'Low'
        items = []
        revisions = set()
        for folder, label in [('sky-toward', 'toward sun'), ('sky-away', 'away from sun'),
                              ('sky-far', '100 m along road')]:
            sky_root = root / folder
            if not sky_root.exists():
                sky_root = root / f'{folder}-Q{q}'
            run = sky_root / f'L0-Q{q}'
            # The uncooked Level Instance streams in after the first screenshots.
            source_frame = shot(sky_root, 0, q, 17)
            items.append((source_frame, f'Golden hour / {quality} / {label} / frame 017'))
            if source_frame.exists() and not (run / 'manifest.json').exists():
                raise ValueError(f'Uncooked capture has no manifest: {run}')
            if (run / 'manifest.json').exists():
                entry = json.loads((run / 'manifest.json').read_text(encoding='utf-8-sig'))
                if not entry.get('source_revision'):
                    raise ValueError(f'Uncooked capture has no source revision: {run}')
                revisions.add(entry['source_revision'])
                log = run / 'game.log'
                entry.update(set=folder, png_count=len(list((run / 'shots').glob('*.png'))),
                             normal_exit=log.exists() and 'LogExit: Exiting.' in log.read_text(errors='replace'))
                runs.append(entry)
        dest = out / f'golden-hour-Q{q}.jpg'
        if all(p.exists() for p, _ in items) and len(revisions) != 1:
            raise ValueError(f'Golden-hour captures mix source revisions: {revisions}')
        if grid(items, dest):
            native_frame(items[-1][0], out / f'golden-hour-far-Q{q}.webp')
            sections.append((f'Bayshore golden hour / {quality}', dest.name,
                             f'Uncooked game viewport on temporary integration {next(iter(revisions))[:7]}; source parents and working changes are recorded in the capture manifest. '
                             'Frame 017 is selected after the Level Instance streams in. Close views face toward/away the rendered sun; far view follows the road. Compatibility evidence only; '
                             'sky/building changes are not part of the powerups branch.'))
    movie = out / 'turntable-day.mp4'
    source = root / 'turntables-v2/L0-Q3/shots/ft_L0_%03d.png'
    if shot(root / 'turntables-v2', 0, 3, 49).exists():
        ffmpeg(['-framerate', '4', '-i', source, '-vf', 'scale=1720:720', '-c:v', 'libx264',
                '-crf', '18', '-pix_fmt', 'yuv420p', '-r', '24', '-movflags', '+faststart', movie])
        RECORDS.append({'file': movie.name, 'sha256': sha(movie), 'type': '4 fps screenshot sequence at original cadence',
                        'source_pattern': str(source)})
    payload = {'raw_capture_root': str(root), 'timings': 'Deferred by Justin; screenshot runs are not performance evidence.',
               'runs': runs, 'derivatives': RECORDS}
    (out / 'manifest.json').write_text(json.dumps(payload, indent=2) + '\n', encoding='utf-8')
    figures = ''
    for title, file, note in sections:
        native = (file.replace('golden-hour-', 'golden-hour-far-') if file.startswith('golden-hour-')
                  else file.replace('staged-', 'far-native-')).replace('.jpg', '.webp')
        native_link = f'<p><a href="{native}">Open the 100 m frame at native resolution (lossless)</a></p>' if (out / native).exists() else ''
        figures += (f'<figure><h2>{html.escape(title)}</h2><p>{html.escape(note)}</p>{native_link}'
                    f'<a href="{file}"><img loading="lazy" src="{file}" alt="{html.escape(title)}"></a></figure>')
    page = '''<!doctype html><html lang="en"><meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1">
<title>Full Throttle power-ups: visual evidence</title><style>
body{margin:0;background:#140f20;color:#eef6ff;font:16px/1.55 system-ui,sans-serif}main{max-width:1500px;margin:auto;padding:28px}
h1{font-size:40px;line-height:1.15}h2{font-size:23px;margin:0}p{color:#c4b6d6;max-width:950px}a{color:#76ffae}img{display:block;width:100%;height:auto}
figure{margin:36px 0;padding:20px;background:#21182e;border:1px solid #463254}.notice{border-left:4px solid #ff8a1e;padding:12px 20px;background:#21182e}
</style><main><p>FULL THROTTLE / JUSTIN / APPROVED R1 + M2</p><h1>Power-up visual evidence</h1>
<p class="notice">GPU and first-use timings remain pending at Justin's request. These are actual Unreal viewport captures, resized and compressed for review. Staged inspection views are labeled separately from natural gameplay. The original 2.7 comparisons are separate from the final 2.8 integration checks.</p>
<p class="notice">Art review: close forms preserve R1 + M2. At 100 m the pickups remain detectable through color, but function recognition from geometry alone does not pass. Native lossless frames are linked below; final runtime art acceptance remains with Justin.</p>
<p><a href="../REVIEW.html">Approved concepts and exact game references</a> · <a href="../../../POWERUPS.md">Implementation and tuning</a> · <a href="manifest.json">Capture manifests and source hashes</a></p>'''
    video = '<h2>Turntable</h2><p>Original capture cadence: four samples per second.</p><video controls loop preload="metadata" style="width:100%" src="turntable-day.mp4"></video>' if movie.exists() else ''
    (out / 'index.html').write_text(page + video + figures + '</main></html>\n', encoding='utf-8')
    print(json.dumps({'composites': len(RECORDS), 'runs': len(runs), 'output': str(out)}))


if __name__ == '__main__':
    main()
