#!/usr/bin/env python3
"""Read-only checks of the completed five-run standalone Brawl sequence.

The engine runner must already have completed each run and copied its save-snapshot.
This reads logs/snapshots only. It prints JSON; --report optionally writes a new report.
"""
import argparse
import hashlib
import json
import re
from pathlib import Path

CASES = [
    ('brawl01-fresh-story', ('day', 0, 0, True)),
    ('brawl02-continue-wave', ('night', 0, 1, False)),
    ('brawl03-retry-outro-cancel', ('nightoutro', 0, 3, False)),
    ('brawl04-discard-unsaved', ('nightoutro', 0, 3, False)),
    ('brawl05-replace-windowclose', ('day', 0, 0, False)),
]
STATE = re.compile(r'FT BRAWL QA STATE screen=(\d+) wave=(\d+) cut=(\d+) time=([-\d.]+) (worldT|cutT)=([-\d.]+) hp=([-\d.]+)')
ACTUAL_SAVE = re.compile(r'LogTemp:\s*FT BRAWL SAVE result=(ok|failed) reason=([^\s]+) kind=([^\s]+) level=(\d+) stage=(-?\d+)')


def digest(data):
    return hashlib.sha256(data).hexdigest()


def load_json(path):
    raw = path.read_bytes()
    return json.loads(raw.decode('utf-8-sig')), digest(raw)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--runs-root', required=True, type=Path)
    parser.add_argument('--report', type=Path)
    args = parser.parse_args()
    root = args.runs_root.resolve()
    report = {'runs_root': str(root), 'passed': False, 'checks': [], 'cases': [], 'limitations': []}

    def check(name, ok, detail=''):
        report['checks'].append({'check': name, 'passed': bool(ok), 'detail': detail})
        return bool(ok)

    records = []
    for i, (name, expected) in enumerate(CASES, 1):
        folder = root / name
        item = {'case': name, 'folder': str(folder)}
        report['cases'].append(item)
        log_path = folder / 'game.log'
        primary_path = folder / 'save-snapshot' / 'brawl-campaign.json'
        settings_path = folder / 'save-snapshot' / 'brawl-campaign.json.settings'
        missing = [str(p) for p in (log_path, primary_path, settings_path) if not p.is_file()]
        if not check(f'{name}: completed log and snapshots present', not missing, '; '.join(missing)):
            records.append(None)
            continue
        log = log_path.read_text(encoding='utf-8-sig', errors='replace')
        try:
            save, primary_hash = load_json(primary_path)
            settings, settings_hash = load_json(settings_path)
        except (ValueError, OSError) as error:
            check(f'{name}: valid snapshot JSON', False, str(error))
            records.append(None)
            continue
        point = (save.get('kind'), save.get('level'), save.get('stage'), save.get('wrap'))
        item.update(checkpoint=point, run_id=save.get('runId'), saved_at=save.get('savedAt'), primary_sha256=primary_hash, settings_sha256=settings_hash)
        failures = re.findall(r'result=timeout|Fatal error:|Assertion failed:|Ensure condition failed:|missing_script=', log)
        check(f'{name}: no harness timeout or engine failure', not failures, ', '.join(failures))
        check(f'{name}: schema and checkpoint', save.get('version') == 1 and point == expected, f'actual={point}; expected={expected}')
        check(f'{name}: retained audio/accessibility preferences', settings.get('muted') is True and settings.get('reducedMotion') is True, repr(settings))
        check(f'{name}: actual desktop menu/native-close exit', 'LogTemp: FT BRAWL SYSTEM exit=desktop discard=0' in log or re.search(r'LogTemp:\s*FT BRAWL SYSTEM exit=desktop discard=0', log), 'Require game-mode exit event, not a harness command echo.')
        fallback = re.findall(r'FT BRAWL QA line=\d+ command=(?:quit|exit)\b[^\r\n]*|FT EXEC line=\d+ cmd=(?:quit|exit)\b[^\r\n]*', log, flags=re.I)
        check(f'{name}: no executed fallback quit', not fallback, '; '.join(fallback))
        check(f'{name}: engine completed shutdown', bool(re.search(r'LogExit: Exiting\.|Engine exit requested|RequestExit\(', log)), 'Requires an engine shutdown event in addition to menu intent.')
        saves = list(ACTUAL_SAVE.finditer(log))
        item['successful_save_events'] = sum(m[1] == 'ok' for m in saves)
        item['failed_save_events'] = sum(m[1] == 'failed' for m in saves)
        if i in (1, 2, 5):
            check(f'{name}: no unexpected save failures', item['failed_save_events'] == 0)
        states = []
        for m in STATE.finditer(log):
            states.append({'screen': int(m[1]), 'wave': int(m[2]), 'cut': int(m[3]), 'time': float(m[4]), m[5]: float(m[6]), 'hp': float(m[7])})
        item['pause_states'] = states
        expected_pairs = 1 if i == 1 else 2 if i == 2 else 0
        if expected_pairs:
            check(f'{name}: complete pause state pairs', len(states) == expected_pairs * 2, f'{len(states)} states')
            for pair in range(min(expected_pairs, len(states) // 2)):
                first, last = states[2 * pair:2 * pair + 2]
                equal = first.keys() == last.keys() and all(abs(first[k] - last[k]) <= .001 for k in first)
                check(f'{name}: pause pair {pair + 1} simulation frozen', first['screen'] == 2 and equal, f'{first} -> {last}')
        backup_path = primary_path.with_suffix(primary_path.suffix + '.bak')
        try:
            backup, backup_hash = load_json(backup_path)
            check(f'{name}: readable compatible backup retained', backup.get('version') == 1)
            item['backup_sha256'] = backup_hash
        except (OSError, ValueError) as error:
            check(f'{name}: readable compatible backup retained', False, str(error))
        records.append({'log': log, 'save': save, 'settings': settings, 'hash': primary_hash, 'settings_hash': settings_hash, 'saves': saves})

    def token(case, text, label):
        record = records[case - 1]
        return check(label, bool(record and text in record['log']), text)

    token(1, 'FT BRAWL SAVE load valid=0 recovered=0 blocked=0', 'fresh launch had no compatible save')
    token(1, 'activate screen=1 row=2', 'mouse selected title Options')
    token(1, 'activate screen=3 row=0', 'mouse toggled audio')
    token(1, 'activate screen=3 row=1', 'mouse toggled reduced motion')
    token(1, 'FTBRAWL cut start 11', 'BEFORE progressed into WRAP')
    token(1, 'command=ftb.WindowClose', 'native close request exercised before cancel')
    token(1, 'FT BRAWL SYSTEM exit=title discard=0', 'successful return to title after close cancellation')
    token(2, 'FTBRAWL start DAY 1 ON SET wrap', 'application restart resumed WRAP')
    token(2, 'FTBRAWL fight begins', 'WRAP continued through night introduction into combat')
    token(2, 'reason=WaveCompleted kind=night level=0 stage=1', 'actual first wave completion saved next wave')
    if records[1]:
        check('wave checkpoint was reached with normal autopilot, not ftb.Win', 'command=ftb.Auto 1' in records[1]['log'] and 'command=ftb.Win' not in records[1]['log'])
    token(3, 'FT BRAWL CONTINUE night=1 wave=1 cut=0 hp=150', 'application restart restored wave2 with full health')
    token(3, 'FT BRAWL SAVE result=failed reason=BossDefeatedTest', 'forced test result reported save failure')
    token(3, 'FT BRAWL SAVE result=failed reason=Exit', 'retry remained failed while write fault active')
    token(3, 'FT BRAWL SAVE result=ok reason=Exit kind=nightoutro level=0 stage=2', 'retry succeeded after clearing write fault')
    token(3, 'FTBRAWL cut start 3', 'saved victory progressed to pier outro')
    token(3, 'activate screen=4 row=0', 'New Game replacement confirmation was cancelled')
    token(4, 'FT BRAWL CONTINUE night=1 wave=3 cut=3 hp=150', 'application restart resumed completed-boss outro')
    token(4, 'FT BRAWL SAVE result=failed reason=EncounterEntered kind=day level=1 stage=0', 'failed Day2 progression was reported')
    token(4, 'FT BRAWL SYSTEM exit=title discard=1', 'failed progression required explicit discard exit')
    token(5, 'FT BRAWL CONTINUE night=1 wave=3 cut=3 hp=150', 'restart after discard restored previous persisted outro')
    token(5, 'activate screen=4 row=1', 'replacement New Game was explicitly confirmed')
    token(5, 'FT BRAWL SAVE result=ok reason=NewGame kind=day level=0 stage=0', 'confirmed replacement saved clean Day1')
    token(5, 'command=ftb.WindowClose', 'native close request exercised for successful desktop exit')

    if all(records):
        ids = [r['save'].get('runId') for r in records]
        check('campaign identity continuous through cases01-04', bool(ids[0]) and len(set(ids[:4])) == 1, repr(ids))
        check('confirmed New Game changes runId', bool(ids[4]) and ids[4] != ids[3])
        check('explicit discard preserves primary bytes', records[2]['hash'] == records[3]['hash'], f"03={records[2]['hash']}; 04={records[3]['hash']}")
        check('all five settings snapshots byte-identical', len({r['settings_hash'] for r in records}) == 1)
        # Cancellation occurs inside case03, between end-of-run snapshots. Do not invent a hash check.
        new_game_in_cancel_case = any(m[1] == 'ok' and m[2] == 'NewGame' for m in records[2]['saves'])
        check('New Game cancellation emits no replacement save and retains runId', not new_game_in_cancel_case and ids[2] == ids[1])
        log3 = records[2]['log']
        opened = log3.find('activate screen=1 row=1')
        cancelled = log3.find('activate screen=4 row=0', max(0, opened))
        returned = log3.find('FT BRAWL SYSTEM screen=1', max(0, cancelled))
        no_write_during_cancel = opened >= 0 and cancelled > opened and returned > cancelled and not ACTUAL_SAVE.search(log3[opened:returned])
        check('cancellation interval contains no save write', no_write_during_cancel)
    report['limitations'].append('New Game cancellation has no intermediate file snapshot. Evidence is safe-cancel input, no write in that interval, unchanged runId and retained outro checkpoint; a before/after cancellation hash is not claimed.')
    report['limitations'].append('Case03 uses ftb.Win only to reach the victory/outro error paths quickly; natural combat checkpoint evidence comes from case02.')
    report['limitations'].append('Visual legibility and audio quality require reviewing the runtime screenshots/audio; this checker validates logs and save files only.')
    report['passed'] = all(c['passed'] for c in report['checks'])
    report['passed_checks'] = sum(c['passed'] for c in report['checks'])
    report['total_checks'] = len(report['checks'])
    text = json.dumps(report, indent=2)
    if args.report:
        args.report.parent.mkdir(parents=True, exist_ok=True)
        args.report.write_text(text + '\n', encoding='utf-8')
    print(text)
    return 0 if report['passed'] else 1


if __name__ == '__main__':
    raise SystemExit(main())
