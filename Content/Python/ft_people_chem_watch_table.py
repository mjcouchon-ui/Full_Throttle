"""FULL THROTTLE (Unreal) the population, level 2, THE WATCHERS: writes the "watch" block of FTPeopleL1.inl.

Plain Python (no editor needed):   python ft_people_chem_watch_table.py > block.txt   and paste the block into
Source/FullThrottle/FTPeopleL1.inl between the BEGIN watch / END watch lines (the block printed includes them).

Why a script: the watchers stand 40 to 41 m from the centre line, in the one band the dressing leaves partly free,
and the dressing (FTDressL1.inl) puts something different into that band in every district. A watcher piece is 14 to
18 m long and 4 to 7 m deep: placed by a plain "every tenth slot" rule it stood INSIDE loading bays (36.5 m, 10 m deep),
on the plinths of the 34 m tank rows, in the flare derricks of the wall turns and in the wall of the mega vat. So the
slots are worked out here, one by one, from the dressing's own rules, and each watcher is written as a rule of its own
(every 1000 slots: a rule that fits exactly one slot).

The game places a rule on slot s when (s + phase) % every == 0; a zone from a km to b km has the slots
ceil(a / SLOT) to floor(b / SLOT); a Site with side 0 takes the left on even s / every and the right on odd.
If the dressing table changes its rules for 30 to 50 m out, change blocked() to match and run this again.
"""
import math

SLOT = 0.045243          # km: 330 * 13.71 cm
LAST = 565               # the last slot of the level's last district (25.6 km)
L, R = -1, 1


def zone(a, b):
    return range(max(1, int(math.ceil(a / SLOT))), int(math.floor(b / SLOT)) + 1)


def on(s, every, phase):
    return (s + phase) % every == 0


GATE, TANKS, PROCESS, REACTOR, ALLEY, LOADING = zone(0.03, 1.23), zone(1.23, 2.6), zone(2.6, 3.98), zone(3.98, 5.35), zone(5.35, 6.72), zone(6.72, 7.94)
WASTE, EXPORT, COOLING, TANKS_B, REACTOR_2 = zone(8.09, 9.46), zone(9.46, 12.72), zone(13.85, 17.4), zone(17.4, 21.4), zone(21.4, 25.6)
DISTRICTS = (('the gate', GATE), ('the tank farm', TANKS), ('the process units', PROCESS), ('the reactor block', REACTOR),
             ('pipe alley', ALLEY), ('the loading yards', LOADING), ('the waste run', WASTE), ('the export terminal', EXPORT),
             ('the cooling field', COOLING), ('tank farm B', TANKS_B), ('reactor two', REACTOR_2))


def blocked(s, side):
    """2 when the dressing has (or may have) a built thing 36 to 44 m out on this side of slot s; 1 when only the band
    behind 42.4 m is taken (a kiln's pad): a mast lift, 4.3 m deep at 40 m, still stands clear there; 0 when free."""
    return 2 if _taken(s, side) else (1 if _kiln(s) else 0)


def _kiln(s):
    """A kiln is 59 m long and its slot is shaken a quarter of a slot: it reaches into the slots beside its own."""
    for slots, every, phase in ((PROCESS, 4, 2), (LOADING, 6, 4), (COOLING, 6, 2), (REACTOR_2, 8, 5)):
        if s in slots and any(on(s + d, every, phase) for d in (-1, 0, 1)):
            return True
    return False


def _taken(s, side):
    if s < 1 or s > LAST:
        return True
    if s <= zone(0.03, 12.72)[-1]:                       # steam: blow-down stacks 41 to 43 m, light masts 37 to 40 m
        if on(s, 5, 2) or on(s, 5, 4):
            return True
    elif s < COOLING[0]:                                  # the flare avenue is left to its flames
        return True
    elif on(s, 6, 4) or on(s, 6, 1):
        return True
    if s in GATE:
        if on(s, 3, 2 if side == L else 0):               # tank row at 34 m (to 37.8)
            return True
        if on(s, 4, 1) and (R if (s // 4) % 2 else L) == side:   # the gate house's set at 36 m
            return True
        if on(s, 5, 3):                                   # tanker trailers 30 to 44 m
            return True
    if s in TANKS and (on(s, 2, 1) or on(s, 3, 2)):       # single tanks 40 to 75 m; the main cast's gantry at 38.5 to 39.5 m
        return True
    if s in PROCESS:
        if on(s, 3, 1) or on(s, 4, 0) or on(s, 5, 3):     # compressor sheds (to 39.6), platforms 40 to 55, pump houses to 40
            return True
        if side == L and s in zone(2.6, 3.01) or side == R and s in zone(3.55, 3.96):   # the wall turns' flare derricks, 50 m out, 22 m square
            return True
    if s in REACTOR:
        if on(s, 4, 1 if side == L else 0) or on(s, 6, 4):   # tank row at 34 m; substations 40 to 60
            return True
    if s in ALLEY and (on(s, 4, 2) or on(s, 3, 2)):       # compressor sheds; pump houses 30 to 40
        return True
    if s in LOADING:
        if on(s, 3, 0 if side == L else 1) or on(s, 3, 1):   # the loading bay at 36.5 m (31.5 to 41.5); trailers 28 to 42
            return True
    if LOADING[-1] < s < WASTE[0] + 2 and side == L:      # the mega vat (its wall 41 m out) and the third wall turn's derrick
        return True
    if s in WASTE:
        if on(s, 2, 0 if side == L else 1):               # tank row at 34 m
            return True
        if on(s, 3, 0) and (R if (s // 3) % 2 else L) == side:   # the waste yard's set at 46 m
            return True
    if s in EXPORT:
        if on(s, 4, 3 if side == L else 2) or on(s, 2, 1):   # the loading bay; trailers 28 to 44
            return True
    if s in COOLING:
        if on(s, 5, 3) or on(s, 4, 1):                    # compressor sheds; pump houses 30 to 42
            return True
    if s in REACTOR_2:
        if on(s, 4, 1 if side == L else 0) or on(s, 5, 3) or on(s, 5, 2):   # tank row at 34 m; platforms 40 to 55; substations from 44
            return True
    return False


CAST = ('Stand_A', 'Lift_A', 'Roof', 'Stand_B', 'Lift_B')
GAP = 3                  # a watcher piece on a side at most every third slot (136 m): the main cast and the crew stand between


def plan():
    """[(slot, side, mesh, metres out)]"""
    out = []
    for side, k0 in ((L, 0), (R, 2)):
        last, k = -99, k0
        for s in range(1, LAST + 1):
            b = blocked(s, side)
            if b == 2 or s - last < GAP:
                continue
            mesh, x = CAST[k % len(CAST)], 40.5
            if b == 1 and not mesh.startswith('Lift'):
                mesh = ('Lift_A', 'Lift_B')[k % 2]
            if mesh.startswith('Lift'):
                x = 40.0
            if s in TANKS_B:                              # a tank row at 34 m in every slot (to 37.8): only the shallow pieces, a little further out
                mesh = {'Stand_A': 'Stand_B', 'Roof': 'Stand_B'}.get(mesh, mesh)
                x = 40.8
            out.append((s, side, mesh, x))
            last, k = s, k + 1
    return sorted(out)


def block():
    p = plan()
    o = ['// ---- BEGIN watch (ft_people_chem_watch.py; this block is written by ft_people_chem_watch_table.py) ----',
         '// THE WATCHERS: the night shift, downed tools, massed on raised steel 40 to 41 m from the centre line (all TALL pieces,',
         '// steady light only), and on the roofs of far tanks. The dressing fills that band differently in every district',
         '// (loading bays to 41.5 m, tank rows to 37.8 m, compressor sheds, kilns, blow-down stacks, light masts, the wall',
         "// turns' flare derricks, the mega vat), so every watcher has a rule of its own that fits one slot (every 1000,",
         '// phase 1000 - slot), worked out from the dressing\'s rules by ft_people_chem_watch_table.py: %d pieces, on a side' % len(p),
         '// at most every third slot. The flare avenue (12.72 to 13.85 km) is left to its flames. A lambda of its own, like the',
         '// dressing table\'s passes, so that the level\'s population function stays small enough for the compiler.',
         'auto PplWatch = [&]()', '{', '#define WATCH(n) TEXT("/Game/People/ChemicalWorks/SM_Watch" n)',
         '#define WATCH_AT(n, s, side, x) Scatter(WATCH(n), x, x, 1000, 1000 - (s), 1.f, 1.f, 0.f, side)']
    for name, slots in DISTRICTS + (('round the mega vat', range(LOADING[-1] + 1, WASTE[0])),):
        mine = [q for q in p if q[0] in slots]
        if not mine:
            continue
        o.append('// %s (slots %d to %d): %d' % (name, slots[0], slots[-1], len(mine)))
        line = '\t'
        for (s, side, mesh, x) in mine:
            line += 'WATCH_AT("%s", %d, %d, %.1ff); ' % (mesh, s, side, x)
            if len(line) > 150:
                o.append(line.rstrip())
                line = '\t'
        if line.strip():
            o.append(line.rstrip())
    # the districts are comments only: the rules sit in two zones (a rule's slot decides where it stands)
    first = [i for i, t in enumerate(o) if t.startswith('// the gate')][0]
    o.insert(first, 'Zone(TEXT("People watch: the works"), 0.03f, 99.f);')
    o += ['// the far tanks: Far rules (they stay beside loops and pipes), standing 1.2 m under the road\'s level like the land',
          'Zone(TEXT("People watch: far tanks"), 0.3f, 99.f);',
          '\tFar(WATCH("TankFar"), 70.f, 110.f, 12, 2, 1.f, 1.f, 0.f, -1, 1.2f, 8.f);',
          '\tFar(WATCH("TankFar"), 70.f, 110.f, 12, 8, 1.f, 1.f, 0.f, 1, 1.2f, 8.f);',
          '#undef WATCH_AT', '#undef WATCH', '};', 'PplWatch();', '// ---- END watch ----']
    return '\n'.join(o)


if __name__ == '__main__':
    import collections
    import sys
    p = plan()
    sys.stderr.write('pieces %d %r\n' % (len(p), sorted(collections.Counter(q[2] for q in p).items())))
    for name, slots in DISTRICTS:
        sys.stderr.write('  %-20s slots %3d-%3d  left %2d right %2d\n' % (name, slots[0], slots[-1], sum(1 for q in p if q[0] in slots and q[1] == L),
                                                                    sum(1 for q in p if q[0] in slots and q[1] == R)))
    print(block())
