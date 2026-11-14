"""FULL THROTTLE 2.6: build Content/Data/skills.json (format full-throttle.skills/2) from the progression spec pack v6.

Usage:  python -I Tools/spec_to_skills.py <spec dir with 00-README.md ...> <out skills.json>

Every number comes from the spec's Markdown tables (never retyped). The script checks the spec's own totals
(12 §4 read-back table) and stops if one does not match. The game reads only the JSON this writes.
"""
import json
import re
import sys
from pathlib import Path

SPEC = Path(sys.argv[1])
OUT = Path(sys.argv[2])


def doc(name):
    for p in SPEC.glob(name + '*.md'):
        return p.read_text(encoding='utf-8')
    raise SystemExit('missing spec file ' + name)


def section(text, title_re):
    """The text from a heading matching title_re to the next heading of the same or higher level."""
    title_re = re.sub(r'^#+ ', '', title_re)
    m = re.search(r'^(#+) .*' + title_re + r'.*$', text, re.M)
    if not m:
        raise SystemExit('no section ' + title_re)
    lvl = len(m.group(1))
    rest = text[m.end():]
    n = re.search(r'^#{1,%d} ' % lvl, rest, re.M)
    return rest[:n.start()] if n else rest


def tables(text):
    """All Markdown tables in text: list of (header, rows) with cells stripped."""
    out, cur = [], None
    for line in text.splitlines():
        if line.startswith('|'):
            cells = [c.strip() for c in line.strip().strip('|').split('|')]
            if cur is None:
                cur = [cells, []]
            elif all(re.fullmatch(r':?-{2,}:?', c) for c in cells if c):
                continue
            else:
                cur[1].append(cells)
        elif cur is not None:
            out.append(tuple(cur)); cur = None
    if cur is not None:
        out.append(tuple(cur))
    return out


def num(s):
    s = s.replace('$', '').replace(',', '').replace('**', '').replace('+', '').replace('−', '-').strip()
    m = re.search(r'-?\d+(\.\d+)?', s)
    if not m:
        raise ValueError('no number in %r' % s)
    v = float(m.group(0))
    return int(v) if v.is_integer() else v


def arrows(s):
    """Every 'a → b → c[ → d]' run in s, as lists of numbers."""
    runs = []
    for m in re.finditer(r'[+−-]?[\d,.]+(?:\s*(?:su|s|px)?\s*→\s*[+−-]?[\d,.]+)+', s):
        runs.append([num(x) for x in m.group(0).split('→')])
    return runs


def plain(s):
    return s.replace('**', '').replace('`', '').strip()


# ------------------------------------------------------------------------------------------ 01 driver levels
d01 = doc('01-')
lvl_tab = tables(section(d01, r'### Level table'))[0]
driver_levels = []
for r in lvl_tab[1]:
    driver_levels.append({'level': num(r[0]), 'totalXP': num(r[1]), 'xpToNext': 0 if r[2] in ('—', '-') else num(r[2]),
                          'sp': num(r[3]), 'unlock': plain(r[5])})
notes = []
for r in tables(section(d01, r"## 6\. Director's Notes"))[0][1]:
    notes.append({'id': plain(r[0]), 'level': num(r[1]), 'text': plain(r[2])})
xp_sources = {}
xp_rows = {}
for r in tables(section(d01, r'## 2\. XP sources'))[0][1]:
    xp_sources[plain(r[0])] = plain(r[2])
    xp_rows[plain(r[0])] = [plain(c) for c in r]
XP = {'sceneFirstTake': num(xp_rows['Scene wrapped on its first take'][2]),
      'wrap': [num(x) for x in xp_rows['Level wrapped'][2].split('/')],
      'rating': [num(x) for x in xp_rows['Rating bonus'][2].split('/')],
      'firstStunt': num(xp_rows['First time ever landing a stunt type'][2]),
      'note': num(xp_rows["Director's Note completed"][2]),
      'bossFirst': num(re.search(r'\+([\d,]+) XP the first time', xp_rows['Wreck a boss'][3]).group(1))}

# ------------------------------------------------------------------------------------------ 02 skill tree
d02 = doc('02-')
tier_tab = tables(section(d02, r'## 1\. Structure'))[0]
tiers = []
for r in tier_tab[1]:
    tiers.append({'tier': num(r[0]), 'sp': num(r[1]), 'level': num(r[2]), 'greenlight': 'greenlight' in r[3]})
nodes, branches = [], []
for m in re.finditer(r'^### (\w[\w ]*?) \(`(\w+)`\) — (.*)$', d02, re.M):
    name, code, about = m.group(1), m.group(2), m.group(3)
    branches.append({'code': code, 'name': name, 'about': about})
    body = d02[m.end():]
    nxt = re.search(r'^#{2,3} ', body, re.M)
    body = body[:nxt.start()] if nxt else body
    for r in tables(body)[0][1]:
        nid = plain(r[0]); t = int(nid[-1])
        tt = tiers[t - 1]
        nm = re.sub(r'\s*\(capstone\)', '', plain(r[1]))
        nodes.append({'id': nid, 'branch': code, 'tier': t, 'name': nm, 'desc': plain(r[2]), 'sp': tt['sp'], 'level': tt['level'], 'greenlight': tt['greenlight']})

# ------------------------------------------------------------------------------------------ 04 garage
d04 = doc('04-')
LINES = {'Armor Plating': 'armor', 'Gun Barrels': 'guns', 'Missile Racks': 'missiles', 'Supercharger': 'super', 'Bull Bar': 'bullbar',
         'Race Tyres': 'tyres', 'Big-Block V8': 'engine', 'Close-Ratio Gearbox': 'gearbox', 'AP Rounds': 'ap', 'HE Rounds': 'he'}
ROW = {'armor': 'ARMOR', 'bullbar': 'ARMOR', 'guns': 'WEAPONS', 'missiles': 'WEAPONS', 'engine': 'ENGINE', 'gearbox': 'ENGINE', 'super': 'ENGINE',
       'tyres': 'TYRES', 'ap': 'AMMO', 'he': 'AMMO'}


def parse_feat(text):
    """A feat phrase from 04 §3, 06 §6 or 07 §10 -> a structured test the game evaluates (07 §6, 09)."""
    t = plain(text)
    if not t:
        return None
    t2 = re.sub(r'^(Lv|Driver Lv) \d+ \+ ', '', t)
    t2 = t2[0].upper() + t2[1:]
    low = t2.lower()
    m = re.fullmatch(r'([\d,]+) syndicate wrecks', low)
    if m: return {'t': 'wrecks', 'n': num(m.group(1)), 'text': t2}
    m = re.fullmatch(r'wreck (\d+) armored trucks', low)
    if m: return {'t': 'trucks', 'n': num(m.group(1)), 'text': t2}
    if low in ('wreck an armored truck',): return {'t': 'trucks', 'n': 1, 'text': t2}
    m = re.fullmatch(r'(\d+) snake eyes wrecked', low)
    if m: return {'t': 'snakeEyes', 'n': num(m.group(1)), 'text': t2}
    m = re.fullmatch(r"(\d+) director's notes", low)
    if m: return {'t': 'notes', 'n': num(m.group(1)), 'text': t2}
    m = re.fullmatch(r'(\d+)% carnage on any level', low)
    if m: return {'t': 'carnageAny', 'n': num(m.group(1)), 'text': t2}
    if low == 'total carnage on any level': return {'t': 'carnageAny', 'n': 95, 'text': t2}
    if low == 'total carnage on all 3 levels': return {'t': 'carnageAll', 'n': 95, 'text': t2}
    if low == 'rated action hero on all 3 levels': return {'t': 'ratingAll', 'n': 1, 'text': t2}
    m = re.fullmatch(r'blockbuster on level (\d)', low)
    if m: return {'t': 'ratingLevel', 'n': 2, 'k': num(m.group(1)) - 1, 'text': t2}
    if low == 'blockbuster on any level': return {'t': 'ratingAny', 'n': 2, 'text': t2}
    if low == 'blockbuster on all 3 levels': return {'t': 'ratingAll', 'n': 2, 'text': t2}
    if low == 'legendary director on any level': return {'t': 'ratingAny', 'n': 3, 'text': t2}
    if low == 'legendary director on all 3 levels': return {'t': 'ratingAll', 'n': 3, 'text': t2}
    m = re.fullmatch(r'driver level (\d+)', low)
    if m: return {'t': 'driverLevel', 'n': num(m.group(1)), 'text': t2}
    m = re.fullmatch(r'land (\d+) stunt types \(ever\)', low)
    if m: return {'t': 'stuntTypes', 'n': num(m.group(1)), 'text': t2}
    m = re.fullmatch(r'land (?:a )?([A-Z][A-Z \-]+)', t2[0:5].lower() + t2[5:]) if low.startswith('land ') else None
    if low.startswith('land ') and ' and ' in t2:
        names = re.split(r', | and ', t2[5:])
        return {'t': 'featsAll', 'names': [n.strip() for n in names], 'text': t2}
    if low.startswith('land '):
        nm = re.sub(r'^(a |an )', '', t2[5:]).strip()
        return {'t': 'feat', 'names': [nm], 'text': t2}
    m = re.fullmatch(r'leave (\d+) syndicate cars behind', low)
    if m: return {'t': 'leftBehind', 'n': num(m.group(1)), 'text': t2}
    m = re.fullmatch(r'own (.+) rank (\d)', t2.lower())
    if m:
        line = [k for nm, k in LINES.items() if nm.lower() == m.group(1)]
        if not line: raise SystemExit('unknown garage line in feat ' + t)
        return {'t': 'garage', 'line': line[0], 'n': num(m.group(2)), 'text': t2}
    if low == 'run down all 3 bosses': return {'t': 'runDownAll', 'text': t2}
    m = re.fullmatch(r'wrap level (\d)', low)
    if m: return {'t': 'wrapped', 'k': num(m.group(1)) - 1, 'text': t2}
    m = re.fullmatch(r'wrap (\d+) levels', low)
    if m: return {'t': 'wraps', 'n': num(m.group(1)), 'text': t2}
    if low == 'wrap any level under par': return {'t': 'underParAny', 'text': t2}
    if low == 'wrap all 3 levels under par': return {'t': 'underParAll', 'text': t2}
    if low == 'wrap a level with no civilians hit': return {'t': 'cleanWrap', 'text': t2}
    if low == 'wrap a level with 50%+ armor left': return {'t': 'armorWrap', 'text': t2}
    if low in ('wreck all three bosses', 'wreck all 3 bosses'): return {'t': 'bossesAll', 'n': 1, 'text': t2}
    m = re.fullmatch(r'wreck each boss (\d+) times', low)
    if m: return {'t': 'bossesAll', 'n': num(m.group(1)), 'text': t2}
    raise SystemExit('unknown feat phrase: ' + t)


rank_tab = tables(section(d04, r'## 2\. The 30 ranks'))[0]
feat_tab = tables(section(d04, r'## 3\. Feats'))[0]
feats_by = {}
for r in feat_tab[1]:
    m = re.match(r'(.+?) R(\d)', plain(r[0]))
    feats_by[(LINES[m.group(1)], int(m.group(2)))] = parse_feat(r[1])
garage = []
for r in rank_tab[1]:
    nm = plain(r[0])
    if nm not in LINES:
        continue
    key = LINES[nm]
    ranks = []
    for i, c in enumerate(r[2:5]):
        c = plain(c)
        m = re.match(r'(.+?) \$([\d,]+)(?: \(Lv (\d+) \+ feat\))?$', c)
        if not m: raise SystemExit('bad rank cell ' + c)
        level = int(m.group(3)) if m.group(3) else (1, 10, 14)[i]
        ranks.append({'name': m.group(1), 'price': num(m.group(2)), 'level': level, 'feat': feats_by.get((key, i + 1))})
    garage.append({'key': key, 'name': nm, 'row': ROW[key], 'ranks': ranks})
vals = {}
for r in tables(section(d04, r'## 4\. What each rank changes'))[0][1]:
    vals[LINES[plain(r[0])]] = plain(r[1])
# the value runs of each line, in the order the spec writes them (04 §4)
VALUE_FIELDS = {
    'armor': ['maxArmor'], 'guns': ['fireInterval'], 'missiles': ['missileStart', 'missileCap'],
    'super': ['turboTank', 'turboRefill', 'burnSeconds', 'flameLength'], 'bullbar': ['ramDamage', 'ramDamageBig', 'bumpDamage'],
    'engine': ['speedUpAdd', 'speedUpTop', 'speedUpMph', 'turboTop', 'turboMph'], 'gearbox': ['accel', 'accelHot', 'punchAdd', 'punchCap'],
    'ap': ['rounds'], 'he': ['blastRadius', 'rounds'],
}
for g in garage:
    if g['key'] == 'tyres':
        continue
    runs = arrows(vals[g['key']])
    fields = VALUE_FIELDS[g['key']]
    if len(runs) < len(fields): raise SystemExit('values missing for ' + g['key'] + ': ' + str(runs))
    v = {}
    for f, run in zip(fields, runs):
        if len(run) == 3: run = [0] + run   # AP/HE: no rank 0 load
        v[f] = run
    g['values'] = v
# Race Tyres are written as +8% / +16% / +25% and −10% / −20% / −30%
ty = vals['tyres']
pcts = [num(x) for x in re.findall(r'[+−-]\d+%', ty)]
gt = [g for g in garage if g['key'] == 'tyres'][0]
gt['values'] = {'driftChargeMult': [1.0] + [round(1 + p / 100, 3) for p in pcts[0:3]], 'bendPullMult': [1.0] + [round(1 + p / 100, 3) for p in pcts[3:6]]}
# HE blast damage: 1, 2 at rank 3 (06 §3)
[g for g in garage if g['key'] == 'he'][0]['values']['blastDamage'] = [0, 1, 1, 2]
extras = {}
for r in tables(section(d04, r'## 6\. Other cash sinks'))[0][1]:
    extras[plain(r[0])] = plain(r[1])
EXTRAS = {'greenlight': num(extras['Capstone greenlight']), 'film': num(extras['Extra reel of film']), 'filmMax': 2,
          'sequel': num(extras['SEQUEL mode']), 'respec': num(re.search(r'\$[\d,]+', extras['Respec ("rewrite the script")']).group(0))}

# ------------------------------------------------------------------------------------------ 05 paces, 08 levels/bosses/kinds
d05 = doc('05-')
d08 = doc('08-')
lt = tables(section(d08, r'## 1\. Level table'))[0]
rows = {plain(r[0]): r[1:] for r in lt[1]}
levels = []
for i in range(3):
    def g(k): return rows[k][i]
    levels.append({'id': 'L%d' % (i + 1), 'title': plain(g('Title')).strip('"'), 'scenes': num(g('Scenes')),
                   'vehicles': num(g('Syndicate vehicles (Carnage denominator, boss included)')), 'henchmen': num(g('Henchmen (2 per chopper; outside Carnage)')),
                   'road': num(g('Road to the boss (road units)')), 'allowance': num(g('Boss-fight allowance in par')),
                   'par': num(re.search(r'\((\d+) s\)', g('Par time')).group(1)), 'fee': num(g('Fee')), 'killFee': num(g('Kill fee (out of film)')),
                   'wrapXP': num(g('Level-wrapped XP')), 'bars': [num(g('Rating bar: Action Hero')) * 1000, num(g('Rating bar: Blockbuster')) * 1000, num(g('Rating bar: Legendary Director')) * 1000],
                   'notes': [n['id'] for n in notes if n['level'] == i + 1]})
bt = tables(section(d08, r'## 3\. Bosses'))[0]
brow = {plain(r[0]): r[1:] for r in bt[1]}
bosses = []
for i, nm in enumerate(bt[0][1:]):
    bosses.append({'name': plain(nm), 'level': num(brow['Level'][i]), 'armor': num(brow['Armor'][i]), 'spot': num(brow['Screen spot'][i]),
                   'escape': num(brow['Escape clock'][i]), 'pace': num(brow['Road pace'][i]), 'flee': num(brow['LAST CHANCE flee speed'][i]), 'points': 3000})
kt = tables(section(d08, r'### Syndicate kinds'))[0]
KIND = {'Biker': 'Moto', 'Snake Eyes': 'Hog', 'Rammer': 'Rammer', 'Gunship SUV': 'Suv', 'Armored truck': 'Truck', 'Syndicate chopper': 'Heli', 'Henchman': 'Henchman'}
pt = {plain(r[0]): r for r in tables(section(d05, r'### Paces'))[0][1]}
PT = {'Moto': 'BIKER', 'Hog': 'SNAKE EYES (level 3)', 'Rammer': 'RAMMER', 'Suv': 'GUNSHIP SUV', 'Truck': 'ARMORED TRUCK', 'Heli': 'SYNDICATE CHOPPER'}
kinds = []
for r in kt[1]:
    k = KIND[plain(r[0])]
    e = {'kind': k, 'name': plain(r[0]), 'armor': num(r[1]), 'class': plain(r[2]).replace('—', 'NONE'), 'points': num(r[3]), 'xp': num(r[4])}
    if k in PT:
        p = pt[PT[k]]
        e['keepsUp'] = 'keeps up' in p[1]
        e['pace'] = 0 if e['keepsUp'] else num(p[1])
        e['heightPx'] = num(p[3]) if re.search(r'\d', p[3]) else 0
    kinds.append(e)

# ------------------------------------------------------------------------------------------ 07 cosmetics
d07 = doc('07-')
cat = section(d07, r'## 10\. The full catalog')
SLOT = {'Paint Shop': 'paint', 'Rims': 'rims', 'Neon underglow': 'neon', 'Nitro colour': 'nitro', 'Exhaust': 'exhaust', 'Trails': 'trail',
        'Tyre smoke': 'smoke', 'Wreck FX': 'wreck', 'Boss finisher': 'finisher'}
TIER = {'STOCK': 0, 'B-MOVIE': 1, 'ACTION HERO': 2, 'BLOCKBUSTER': 3, 'LEGENDARY': 4}
cosmetics = []
for m in re.finditer(r'^### (Paint Shop|Rims|SFX DEPT) · (.+?) \(\d+\)$', cat, re.M):
    shop, group = m.group(1), m.group(2)
    body = cat[m.end():]
    nxt = re.search(r'^### ', body, re.M)
    body = body[:nxt.start()] if nxt else body
    slot = SLOT[shop] if shop != 'SFX DEPT' else SLOT[group]
    for r in tables(body)[0][1]:
        how = plain(r[6])
        wave = 1
        if shop in ('Paint Shop', 'Rims') and 'New renders' in how:
            wave = 2 if shop == 'Rims' else 3
        cosmetics.append({'id': plain(r[0]), 'name': plain(r[1]), 'shop': 'SFX' if shop == 'SFX DEPT' else ('PAINT' if shop == 'Paint Shop' else 'RIMS'),
                          'group': group, 'slot': slot, 'tier': TIER[plain(r[2])], 'price': num(r[3]), 'level': num(r[4]),
                          'feat': parse_feat(r[5]), 'how': how, 'look': plain(r[7]), 'swatch': plain(r[8]) or None, 'wave': wave})

# ------------------------------------------------------------------------------------------ 12 tuning
d12 = doc('12-')
tun = {}
for r in tables(section(d12, r'4\. Data Tables'))[-1][1]:
    names = [n.strip() for n in plain(r[0]).split(',')]
    vs = re.findall(r'[\d,]*\.?\d+', plain(r[1]).replace(',000', '000'))
    if names == ['RatingMult (B-Movie … Legendary)']:
        tun['RatingMult'] = [num(v) for v in plain(r[1]).split(',')]
        continue
    if names == ['PxToUU']:
        continue
    vals_ = [num(v) for v in re.split(r',\s(?=\d)', plain(r[1]))]
    for n, v in zip(names, vals_):
        tun[n] = v
tun['PxToUU'] = 13.71  # the project's own value: FT::DU, cm per 1.x road unit / screen px along the road (00, 12 §4)

# ------------------------------------------------------------------------------------------ checks (12 §4 read-back table)
def check(what, got, want):
    if got != want:
        raise SystemExit('CHECK FAILED %s: got %r want %r' % (what, got, want))
    print('ok  %-40s %r' % (what, got))

check('driver levels rows', len(driver_levels), 30)
check('level 30 total XP', driver_levels[-1]['totalXP'], 448920)
check('SP from levels', sum(l['sp'] for l in driver_levels), 35)
check('director notes', [n['id'] for n in notes][0::8], ['L1-boss40', 'L3-boss60'])
check('skill nodes', len(nodes), 30)
check('skill SP sum', sum(n['sp'] for n in nodes), 75)
check('greenlight nodes', sum(1 for n in nodes if n['greenlight']), 5)
check('garage ranks', sum(len(g['ranks']) for g in garage), 30)
check('garage price sum', sum(r['price'] for g in garage for r in g['ranks']), 1770000)
check('cosmetics rows', len(cosmetics), 108)
check('cosmetics for sale', sum(1 for c in cosmetics if c['price'] > 0), 104)
check('cosmetics price sum', sum(c['price'] for c in cosmetics), 6378000)
check('level vehicles', [l['vehicles'] for l in levels], [114, 51, 66])
check('level par', [l['par'] for l in levels], [300, 185, 220])
check('boss armor', [b['armor'] for b in bosses], [120, 120, 200])
check('kinds armor', [k['armor'] for k in kinds], [2, 3, 5, 10, 26, 40, 1])
check('ammo rounds', [g['values']['rounds'] for g in garage if g['key'] in ('ap', 'he')], [[0, 40, 60, 80], [0, 40, 60, 80]])
check('engine speedUpAdd', [g['values']['speedUpAdd'] for g in garage if g['key'] == 'engine'][0], [90, 115, 140, 170])
check('xp bonuses', (XP['sceneFirstTake'], XP['rating'], XP['firstStunt'], XP['note'], XP['bossFirst']), (50, [0, 100, 250, 500], 100, 300, 500))
check('tuning CarnageFactor', (tun['CarnageFactor'], tun['CarnageCap'], tun['ScheduleCap'], tun['MphPerSu']), (2, 3, 20000, 0.31))

data = {
    'format': 'full-throttle.skills/2',
    'about': "FULL THROTTLE progression (design v6, 14 Nov 2026): driver levels and XP, the skill tree, the garage, the asset library, "
             "the paycheck tuning and per-level data. Written by Tools/spec_to_skills.py from the spec pack; the Unreal build reads it from "
             "Content/Data/skills.json. Format 2 replaces format 1 (2.3's four point-bought upgrades): skill points now buy the tree, cash buys the garage.",
    'tuning': tun, 'extras': EXTRAS, 'tiers': tiers, 'driverLevels': driver_levels, 'notes': notes, 'branches': branches, 'skillNodes': nodes,
    'garage': garage, 'cosmetics': cosmetics, 'levels': levels, 'bosses': bosses, 'kinds': kinds, 'xp': XP,
}
OUT.write_text(json.dumps(data, indent=1, ensure_ascii=False), encoding='utf-8')
print('wrote', OUT, OUT.stat().st_size, 'bytes')
