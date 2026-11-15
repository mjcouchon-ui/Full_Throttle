"""FULL THROTTLE (Unreal) the population, level 0: writes Source/FullThrottle/FTPeopleL0.inl.

Plain Python (no editor needed):   python Content/Python/ft_people_bay_table.py
The population table is written by this script and not by hand, for two reasons:
  - the balconies hang on the walls of the dressing's shop rows, and those stand where FTDressL0.inl's Street(...)
    lines say (which slots, which row, how far up). This reads those lines and writes one small district for each,
    with the row's own phase and height. Run it again whenever the dressing's Street lines change.
  - a rule here names the SLOTS it wants (s % every == residue), which is how one sees what shares a slot with what
    in the dressing; the table's own word is a phase, (s + phase) % every == 0, and the script does the sum.
A slot is 45.243 m of road; slot s is at s * 45.243 m. What the dressing has in which slots is noted beside each
district below (read off FTDressL0.inl, 15 Nov 2026).
"""
import os
import re

HERE = os.path.dirname(os.path.abspath(__file__))
SRC = os.path.normpath(os.path.join(HERE, '..', '..', 'Source', 'FullThrottle'))
B = '/Game/People/Bayshore/'
C = '/Game/People/Crew/'
BAY, TOWN = -1, 1
UPS = {'': 0.0, 'B': 0.45, 'C': 0.95}        # the dressing's Front1 / Front1B / Front1C: how far up the row stands
SLOT_KM = 0.045243

out = []


def w(s=''):
    out.append(s)


def f(v):
    s = ('%.3f' % v).rstrip('0').rstrip('.')
    return s + ('.f' if '.' not in s else 'f')


def zone(name, km0, km1):
    w('Zone(TEXT("People, %s"), %s, %s);' % (name, f(km0), f(km1)))


def slot_zone(name, a, b=None):
    b = a if b is None else b
    w('Zone(TEXT("People, %s"), %.5ff, %.5ff);' % (name, (a - 0.5) * SLOT_KM, (b + 0.5) * SLOT_KM))


def ph(every, residue):
    return (every - residue % every) % every


def scatter(mesh, x0, x1, every, residue, side, chance=1.0, scale=1.0, jitter=0.0, yaw=0.0, note=''):
    path = mesh if mesh.startswith('/') else B + mesh
    w('\tScatter(TEXT("%s"), %s, %s, %d, %d, %s, %s, %s, %d, %s);%s' % (path, f(x0), f(x1), every, ph(every, residue), f(chance), f(scale), f(jitter), side, f(yaw),
                                                                         ('   // ' + note) if note else ''))


def site(name, side, x, every, residue=None, phase=None, chance=1.0, note=''):
    p = phase if phase is not None else ph(every, residue)
    w('\tSite(TEXT("%s"), %d, %s, %d, %d, %s);%s' % (name, side, f(x), every, p, f(chance), ('   // ' + note) if note else ''))


def afloat(mesh, d0, d1, every, residue, scale, yaw=20.0, jitter=0.15, note=''):
    w('\tAfloat(TEXT("%s%s"), %s, %s, %d, %d, 1.f, %s, %s, -1, %s);%s' % (B, mesh, f(d0), f(d1), every, ph(every, residue), f(scale), f(jitter), f(yaw),
                                                                        ('   // ' + note) if note else ''))


def sets():
    w('// ---- Sets. The boardwalk\'s deck is 0.48 m over the sand (ft_dress_bay.boardwalk): what walks on it is drawn from')
    w('// z = 0 and stood up by that much. The pier\'s people are drawn in the pier\'s own space: same anchor, no offsets.')
    w('Set(TEXT("PplWalkA"), { { TEXT("%sSM_Strollers_A"), 0, 0, 0.48f, 0, 1 } });' % B)
    w('Set(TEXT("PplWalkB"), { { TEXT("%sSM_Strollers_B"), 0, 0, 0.48f, 0, 1 } });' % B)
    w('Set(TEXT("PplPier"), { { TEXT("%sSM_Anglers"), 0, 0, 0, 0, 1 } });' % B)
    w('// The film crew (wardrobe Sun). The town-side sets are anchored 21.9 m out, on the pavement between the kerb and')
    w('// the shopfronts, and go in side-street slots only (no shop row at 27.5 m there: the tall pieces stand in the')
    w('// street\'s mouth, 26 m out and more). The bay-side ones stand on the boardwalk\'s deck.')
    w('Set(TEXT("PplShootA"), {      // a camera position and the crane behind it')
    w('\t{ TEXT("%sSM_CrewCamera_Sun"),   -13, 0,    0, 0, 1 },' % C)
    w('\t{ TEXT("%sSM_CrewCrane_Sun"),      1, 8.4f, 0, 0, 1 },' % C)
    w('\t{ TEXT("%sSM_CrewLights_Sun"),   -12, 5.2f, 0, 0, 1 },' % C)
    w('\t{ TEXT("%sSM_CrewApplauseA_Sun"), 13, 0,    0, 0, 1 },' % C)
    w('});')
    w('Set(TEXT("PplShootB"), {      // the dolly on its rails, the director\'s village, sound, catering')
    w('\t{ TEXT("%sSM_CrewDolly_Sun"),      0, 0,    0, 0, 1 },' % C)
    w('\t{ TEXT("%sSM_CrewVillage_Sun"),   14, 0.4f, 0, 0, 1 },' % C)
    w('\t{ TEXT("%sSM_CrewSound_Sun"),    -13, 4.6f, 0, 0, 1 },' % C)
    w('\t{ TEXT("%sSM_CrewCatering_Sun"),   3, 7.8f, 0, 0, 1 },' % C)
    w('});')
    w('Set(TEXT("PplShootC"), {      // the stunt team before a jump')
    w('\t{ TEXT("%sSM_CrewStunt_Sun"),      0, 0.3f, 0, 0, 1 },' % C)
    w('\t{ TEXT("%sSM_CrewApplauseB_Sun"), 13, 0,    0, 0, 1 },' % C)
    w('\t{ TEXT("%sSM_CrewLights_Sun"),   -12, 5.2f, 0, 0, 1 },' % C)
    w('});')
    w('Set(TEXT("PplCrewWalkA"), { { TEXT("%sSM_CrewApplauseA_Sun"), 0, 0, 0.48f, 0, 1 } });' % C)
    w('Set(TEXT("PplCrewWalkB"), { { TEXT("%sSM_CrewApplauseB_Sun"), 0, 0, 0.48f, 0, 1 } });' % C)
    w('Set(TEXT("PplCrewWalkC"), { { TEXT("%sSM_CrewCamera_Sun"), 0, 0, 0.48f, 0, 1 } });' % C)
    w()


WALK = 22.9      # the strollers: the seaward two thirds of the boardwalk (19.9 to 24.1 m)
PAVE = 24.6      # the shoppers: under the awnings of the shop rows (their wall is at 26 m, the awning's edge at 24.2)
DECK = 26.4      # sun terraces, lifeguard towers, bandstands: on the sand right behind the boardwalk (tall: 24 m and more)


def districts():
    # ------------------------------------------------------------------------------------------------ THE STRIP
    w('// ---- 0 to 3.2 km: THE STRIP. The town is out to watch. (The dressing, bay side: an arcade on the walk in slots')
    w('// s%4==2, the walk\'s shops in s%4==0, a beach club 34 m out in s%4==3, a bar 52 m out in s%4==1. Town side: side')
    w('// street s%4==0, the set-back shop row and a kiosk corner s%4==1, the two front rows s%4==2 and 3.)')
    zone('the Strip', 0.0, 3.2)
    site('PplWalkA', BAY, WALK, 4, 0)
    site('PplWalkB', BAY, WALK, 4, 2)
    scatter('SM_SunDeck_A', DECK, DECK, 8, 1, BAY)
    scatter('SM_SunDeck_B', DECK, DECK, 8, 5, BAY)
    scatter('SM_Bandstand', 28.2, 28.2, 16, 7, BAY)
    scatter('SM_GuardTower', 27.0, 27.0, 16, 15, BAY)
    scatter('SM_BeachDay_A', 44, 50, 4, 0, BAY)
    scatter('SM_BeachDay_B', 66, 78, 8, 3, BAY)
    scatter('SM_Volley_A', 74, 82, 8, 1, BAY)
    scatter('SM_SurfWalk', 84, 96, 8, 5, BAY)
    scatter('SM_Gulls', 70, 160, 8, 2, BAY, yaw=180)
    site('PplPier', BAY, 190, 14, phase=9, note='the dressing\'s PierSet: the same slots')
    afloat('SM_SurfRiders', 430, 520, 9, 4, 2.6)
    afloat('SM_Dolphins', 480, 620, 23, 7, 3.0, yaw=180)
    scatter('SM_Shoppers_A', PAVE, PAVE, 4, 2, TOWN)
    scatter('SM_Shoppers_A', PAVE, PAVE, 4, 3, TOWN)
    # ------------------------------------------------------------------------------------------------ THE MARINA
    w('// ---- 3.2 to 6.4 km: THE MARINA. Quieter sand, busier piers: anglers, gulls over the boats, dolphins off the')
    w('// jetties. (Bay side: the arcade s%4==2, the walk\'s shops s%6==3, a pier s%9==5.)')
    zone('the Marina', 3.2, 6.4)
    site('PplWalkB', BAY, WALK, 6, 0)
    site('PplWalkA', BAY, WALK, 6, 3)
    scatter('SM_SunDeck_A', DECK, DECK, 12, 1, BAY)
    scatter('SM_SunDeck_B', DECK, DECK, 12, 7, BAY)
    scatter('SM_GuardTower', 27.0, 27.0, 24, 5, BAY)
    scatter('SM_BeachDay_A', 40, 46, 8, 4, BAY)
    scatter('SM_Gulls', 60, 200, 5, 1, BAY, yaw=180)
    site('PplPier', BAY, 190, 9, phase=4, note='the dressing\'s PierSet')
    afloat('SM_Dolphins', 430, 560, 17, 3, 3.0, yaw=180)
    scatter('SM_Shoppers_A', PAVE, PAVE, 4, 2, TOWN)
    # ------------------------------------------------------------------------------------------------ PLEASURE BEACH
    w('// ---- 6.4 to 9.8 km: PLEASURE BEACH. The beach at its busiest: a band or a terrace of people at every other')
    w('// slot, volleyball, kites, surfers. (Bay side: the arcade s%4==3, the walk\'s shops s%4==1, beach clubs in even')
    w('// slots, the dressing\'s own court 62 m out in s%4==3. Town side: front rows s%4==1 and 3, the set-back row s%4==2.)')
    zone('Pleasure Beach', 6.4, 9.8)
    site('PplWalkA', BAY, WALK, 4, 1)
    site('PplWalkB', BAY, WALK, 4, 3)
    scatter('SM_SunDeck_A', DECK, DECK, 8, 0, BAY)
    scatter('SM_SunDeck_B', DECK, DECK, 8, 4, BAY)
    scatter('SM_Bandstand', 28.2, 28.2, 8, 2, BAY)
    scatter('SM_GuardTower', 27.0, 27.0, 8, 6, BAY)
    scatter('SM_Volley_A', 74, 82, 8, 1, BAY)
    scatter('SM_BeachDay_A', 70, 80, 8, 5, BAY)
    scatter('SM_BeachDay_B', 76, 86, 4, 3, BAY)
    scatter('SM_SurfWalk', 88, 98, 8, 0, BAY)
    scatter('SM_Gulls', 70, 160, 8, 6, BAY, yaw=180)
    site('PplPier', BAY, 190, 12, phase=1, note='the dressing\'s PierSet')
    afloat('SM_SurfRiders', 430, 520, 9, 3, 2.6)
    afloat('SM_Dolphins', 480, 620, 29, 5, 3.0, yaw=180)
    scatter('SM_Shoppers_A', PAVE, PAVE, 4, 1, TOWN)
    scatter('SM_Shoppers_A', PAVE, PAVE, 4, 3, TOWN)
    # ------------------------------------------------------------------------------------------------ HOTEL ROW
    w('// ---- 9.8 to 13.2 km: HOTEL ROW. Joggers on the walk, the hotels\' guests on the balconies, surfers. (Bay side:')
    w('// the arcade s%4==1, the walk\'s shops s%4==3, club decks and lidos 46 to 54 m out in even slots.)')
    zone('Hotel Row', 9.8, 13.2)
    site('PplWalkB', BAY, WALK, 6, 1)
    site('PplWalkA', BAY, WALK, 6, 4)
    scatter('SM_SunDeck_A', DECK, DECK, 8, 0, BAY)
    scatter('SM_SunDeck_B', DECK, DECK, 16, 12, BAY)
    scatter('SM_GuardTower', 27.0, 27.0, 16, 4, BAY)
    scatter('SM_BeachDay_A', 44, 52, 8, 1, BAY)
    scatter('SM_BeachDay_B', 44, 52, 8, 5, BAY)
    scatter('SM_SurfWalk', 70, 90, 8, 3, BAY)
    scatter('SM_Gulls', 70, 160, 8, 7, BAY, yaw=180)
    afloat('SM_SurfRiders', 430, 520, 8, 2, 2.6)
    afloat('SM_Dolphins', 480, 620, 31, 9, 3.0, yaw=180)
    scatter('SM_Shoppers_A', PAVE, PAVE, 4, 2, TOWN)
    # ------------------------------------------------------------------------------------------------ THE POINT
    w('// ---- 13.2 to 16.8 km: THE POINT. The town thins out: gulls, surfers, dolphins off the rocks, a few families.')
    w('// (The boardwalk stands in every third slot here: s%3==0.)')
    zone('the Point', 13.2, 16.8)
    site('PplWalkB', BAY, WALK, 6, 0)
    site('PplWalkA', BAY, WALK, 6, 3)
    scatter('SM_SunDeck_A', DECK, DECK, 16, 1, BAY)
    scatter('SM_GuardTower', 27.0, 27.0, 16, 9, BAY)
    scatter('SM_BeachDay_A', 34, 48, 8, 3, BAY)
    scatter('SM_SurfWalk', 40, 56, 8, 7, BAY)
    scatter('SM_Gulls', 40, 200, 4, 2, BAY, yaw=180)
    afloat('SM_SurfRiders', 420, 520, 6, 1, 2.6)
    afloat('SM_Dolphins', 440, 600, 13, 6, 3.0, yaw=180)
    scatter('SM_Shoppers_A', PAVE, PAVE, 8, 3, TOWN)
    # ------------------------------------------------------------------------------------------------ DOWNTOWN
    w('// ---- 16.8 km to the end: THE FINALE, downtown\'s waterfront. Everything at once. (As the Strip; piers s%11==6.)')
    zone('Downtown waterfront', 16.8, 99.0)
    site('PplWalkA', BAY, WALK, 4, 0)
    site('PplWalkB', BAY, WALK, 4, 2)
    scatter('SM_SunDeck_A', DECK, DECK, 8, 1, BAY)
    scatter('SM_SunDeck_B', DECK, DECK, 8, 5, BAY)
    scatter('SM_Bandstand', 28.2, 28.2, 16, 3, BAY)
    scatter('SM_GuardTower', 27.0, 27.0, 16, 11, BAY)
    scatter('SM_BeachDay_A', 70, 80, 8, 0, BAY)
    scatter('SM_BeachDay_B', 70, 80, 8, 4, BAY)
    scatter('SM_Volley_A', 72, 82, 16, 6, BAY)
    scatter('SM_SurfWalk', 84, 96, 16, 14, BAY)
    scatter('SM_Gulls', 70, 160, 8, 3, BAY, yaw=180)
    site('PplPier', BAY, 190, 11, phase=5, note='the dressing\'s PierSet')
    afloat('SM_SurfRiders', 430, 520, 9, 2, 2.6)
    afloat('SM_Dolphins', 480, 620, 19, 4, 3.0, yaw=180)
    scatter('SM_Shoppers_A', PAVE, PAVE, 4, 2, TOWN)
    w()


# the track's set pieces, by the slots the dressing table flags them on (FTDressL0.inl: LoopSite, TwistSite, JumpSite)
JUMPS = (38, 77, 119, 189, 256, 323)
LOOPS = (43, 94, 153, 229, 282, 328, 360, 382, 414, 445, 477, 509, 541)
TWISTS = (64, 165, 224, 290, 346, 397, 428, 460, 492, 524, 556)
LAST = 560


def side_street_before(s, back=1):
    """The last side-street slot (s % 4 == 0) at least `back` slots before slot s."""
    t = s - back
    return t - t % 4


def crew():
    w('// ---- The film crew (/Game/People/Crew/, wardrobe Sun). The level is a shoot: a unit by the start, the stunt team')
    w('// before every jump, a camera and its crane before every loop, a camera before every corkscrew, crew applauding on')
    w('// the boardwalk after them, a unit before the boss, and a camera every two kilometres between. One slot a district.')
    used = set()

    def unit(name, s, set_name):
        if s in used or s < 1:
            return
        used.add(s)
        slot_zone(name, s)
        site(set_name, TOWN, 21.9, 1, 0)

    def low(name, s, piece, side=TOWN):
        key = (s, side)
        if key in used or s in used or s < 1:
            return
        used.add(key)
        slot_zone(name, s)
        if side == TOWN:
            w('\tScatter(TEXT("%sSM_Crew%s_Sun"), 21.9f, 21.9f, 1, 0, 1.f, 1.f, 0.f, 1, 0.f);' % (C, piece))
        else:
            site({'ApplauseA': 'PplCrewWalkA', 'ApplauseB': 'PplCrewWalkB', 'Camera': 'PplCrewWalkC'}[piece], BAY, 22.6, 1, 0)

    # the start: the unit is all here
    unit('the crew at the start', 4, 'PplShootA')
    unit('the crew at the start', 8, 'PplShootB')
    low('the crew at the start', 1, 'Camera')
    low('the crew at the start', 2, 'ApplauseB')
    low('the crew at the start', 3, 'Village')
    low('the crew at the start', 6, 'ApplauseA')
    low('the crew at the start', 1, 'ApplauseA', BAY)
    low('the crew at the start', 3, 'Camera', BAY)
    low('the crew at the start', 5, 'ApplauseB', BAY)
    for k, j in enumerate(JUMPS):
        unit('the stunt team before a jump', side_street_before(j, 2), 'PplShootC')
        low('a camera on the jump', side_street_before(j, 2) - 1, 'Camera')
        low('crew past the landing', j + 4, 'ApplauseA' if k % 2 else 'ApplauseB', BAY)
    for k, a in enumerate(LOOPS):
        unit('a unit before a loop', side_street_before(a, 1), 'PplShootA' if k % 2 == 0 else 'PplShootB')
        low('crew before a loop', a - 1, 'ApplauseB' if k % 2 else 'ApplauseA', BAY)
    for k, a in enumerate(TWISTS):
        low('a camera on a corkscrew', a - 1, 'Camera' if k % 2 == 0 else 'Dolly')
        low('crew before a corkscrew', a - 2, 'ApplauseA' if k % 2 else 'ApplauseB', BAY)
    # before the boss: the last kilometre
    for s, nm in ((532, 'PplShootB'), (536, 'PplShootC'), (548, 'PplShootA'), (552, 'PplShootB')):
        unit('the unit before the boss', s, nm)
    for s, p in ((534, 'Camera'), (538, 'ApplauseA'), (546, 'ApplauseB'), (550, 'Camera'), (554, 'Village')):
        low('the unit before the boss', s, p)
    for s in range(22, LAST, 44):
        low('a camera along the way', s, 'Camera' if (s // 44) % 2 == 0 else 'ApplauseA')
    w()


def streets():
    res = []
    pat = re.compile(r'^Street\((\d+), (\d+), TEXT\("ShopRow(\w?)"\), TEXT\("Front1(\w?)"\), TEXT\("Front2(\w?)"\), (\d+), (\d+), (\d+), ([\d.]+)f, ([\d.]+)f, ([\d.]+)f')
    for line in open(os.path.join(SRC, 'FTDressL0.inl')):
        m = pat.match(line)
        if m:
            a, b, _, v1, v2, _, ph_a, ph_b, _, ch_a, ch_b = m.groups()
            res.append((int(a), int(b), v1, v2, int(ph_a), int(ph_b), float(ch_a), float(ch_b)))
    return res


def balconies():
    w('// ---- The balconies, on the walls of the dressing\'s two front shop rows (27.5 m): one district for each of its')
    w('// Street(...) lines, the row\'s own slots, phase and height. Front1 has the surf shop\'s upper windows 5.8 m back')
    w('// along the row; Front2 the sweet shop\'s 4.6 m on. Front2\'s are left out from the Marina to Hotel Row (a')
    w('// balcony at every row read as a pattern).')
    for v in ('', 'B', 'C'):
        w('Set(TEXT("PplBalc1%s"), { { TEXT("%sSM_Balcony_A"), -5.8f, 0, %s, 0, 1 } });' % (v, B, f(UPS[v])))
        w('Set(TEXT("PplBalc2%s"), { { TEXT("%sSM_Balcony_A"), 4.6f, 0, %s, 0, 1 } });' % (v, B, f(UPS[v])))
    n = 0
    for a, b, v1, v2, ph_a, ph_b, ch_a, ch_b in streets():
        km = a * SLOT_KM
        second = km < 3.2 or km >= 13.2
        if ch_a <= 0 and not (second and ch_b > 0):
            continue
        slot_zone('balconies', a, b)
        if ch_a > 0:
            site('PplBalc1' + v1, TOWN, 27.5, 4, phase=ph_a)
        if ch_b > 0 and second:
            site('PplBalc2' + v2, TOWN, 27.5, 4, phase=ph_b)
        n += 1
    return n


def main():
    w('// Bayshore (level index 0): the population layer. People, creatures and film crews, placed by rule like the dressing')
    w('// (the words and what their numbers mean are in the header of FTTrackDress.cpp; the rule kinds in FTDress.h).')
    w('// WRITTEN BY Content/Python/ft_people_bay_table.py: change that file and run it, never this one. Its header says why.')
    w('// The meshes: ft_people_bay.py and ft_people_bay_beach.py (/Game/People/Bayshore/), and the film crew in wardrobe')
    w('// Sun (ft_people_crew.py, /Game/People/Crew/). This table is read after FTDressL0.inl, so no dressing piece moves.')
    w('// The sight rule as the track applies it: a piece whose top stands 2 m over the road is pushed out to 24 m.')
    w()
    sets()
    districts()
    crew()
    n = balconies()
    only = os.environ.get('FT_ONLY')        # for counting one mesh's pieces in the game's log: keep only its rules
    if only:
        keys = only.split(',')
        out[:] = [ln for ln in out if not ln.startswith('	') or ln.rstrip().endswith('},') or any(k in ln for k in keys)]
    open(os.path.join(SRC, 'FTPeopleL0.inl'), 'w', newline='\n').write('\n'.join(out) + '\n')
    print('lines', len(out), 'balcony districts', n)


if __name__ == '__main__':
    main()
