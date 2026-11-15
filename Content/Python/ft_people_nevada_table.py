"""FULL THROTTLE (Unreal) the population of Nevada: the writer of its placing table, Source/FullThrottle/FTPeopleL2.inl.

Plain Python (no Unreal):   python Content/Python/ft_people_nevada_table.py      (run in the project folder)

The table is written SLOT BY SLOT (a slot is 45.243 m of road; slot s lies at s * 45.243 m): every row is one Site rule
in a zone of its own, so a piece stands exactly on its slot, on the side and at the distance this file gives it. That
is how the population keeps out of the dressing: this file knows, from FTDressL2.inl, which slots carry a rock cut, a
rock bank, a ventilation head, a mile sign, a roadhouse, a truck stop, a road stop or a casino, and what is free
between them. What the track itself keeps clear (loops, corkscrews, jump gaps, run-ups and landings) is left to the
track: a row there is simply not placed.

The bands: by the road 27 to 29 m (every near piece is TALL: a deck or a bank two metres up; its front stands 24.6 m
out or more); the canyon's rim 33.1 m (inside the 15 m rock cuts, which stand 25 to 35 m out); middle distance 40 to
70 m; far life 85 to 240 m.
"""
import os
import random

SLOT = 0.045243          # km
NV = '/Game/People/Nevada/'
CR = '/Game/People/Crew/'
W = '_Desert'

# name -> (mesh, up m): a set of one piece, or a crew piece on its riser
SETS = [
    ('PplRoadhouse', [(NV + 'SM_RoadhouseDeck', 0.0)]),
    ('PplDiner', [(NV + 'SM_DinerDeck', 0.0)]),
    ('PplHitchA', [(NV + 'SM_Hitchers_A', 0.0)]),
    ('PplHitchB', [(NV + 'SM_Hitchers_B', 0.0)]),
    ('PplProspect', [(NV + 'SM_Prospectors', 0.0)]),
    ('PplLizards', [(NV + 'SM_Lizards', 0.0)]),
    ('PplRiders', [(NV + 'SM_Riders', 0.0)]),
    ('PplStandA', [(NV + 'SM_StripStand_A', 0.0)]),
    ('PplStandB', [(NV + 'SM_StripStand_B', 0.0)]),
    ('PplValets', [(NV + 'SM_Valets', 0.0)]),
    ('PplWedding', [(NV + 'SM_Wedding', 0.0)]),
    ('PplGamblers', [(NV + 'SM_Gamblers', 0.0)]),
    ('PplRimCoyotes', [(NV + 'SM_RimCoyotes', 0.0)]),
    ('PplRimPosse', [(NV + 'SM_RimPosse', 0.0)]),
    ('PplHorses', [(NV + 'SM_WildHorses', 0.0)]),
    ('PplCattle', [(NV + 'SM_Cattle', 0.0)]),
    ('PplCrewCam', [(NV + 'SM_CrewRiser', 0.0), (CR + 'SM_CrewCamera' + W, 2.0)]),
    ('PplCrewClapA', [(NV + 'SM_CrewRiser', 0.0), (CR + 'SM_CrewApplauseA' + W, 2.0)]),
    ('PplCrewClapB', [(NV + 'SM_CrewRiser', 0.0), (CR + 'SM_CrewApplauseB' + W, 2.0)]),
    ('PplCrewStunt', [(NV + 'SM_CrewRiser', 0.0), (CR + 'SM_CrewStunt' + W, 2.0)]),
    ('PplCrewVillage', [(NV + 'SM_CrewRiser', 0.0), (CR + 'SM_CrewVillage' + W, 2.0)]),
    ('PplCrewDolly', [(NV + 'SM_CrewRiser', 0.0), (CR + 'SM_CrewDolly' + W, 2.0)]),
    ('PplCrewCrane', [(CR + 'SM_CrewCrane' + W, 0.0)]),
    ('PplCrewLights', [(CR + 'SM_CrewLights' + W, 0.0)]),
    ('PplCrewSound', [(CR + 'SM_CrewSound' + W, 0.0)]),
    ('PplCrewCatering', [(CR + 'SM_CrewCatering' + W, 0.0)]),
]
NEAR = [n for n, _ in SETS if n not in ('PplRimCoyotes', 'PplRimPosse', 'PplHorses', 'PplCattle')]
# the same sets 7.5 m along their slot: on the side of the road that has the phone line (three poles to a slot, 30 m out,
# on the slot's middle and 15 m either way) a near piece stands between two poles, and smaller
SETS = [(n, p, 0.0) for n, p in SETS] + [(n + 'P', p, 7.5) for n, p in SETS if n in NEAR]
IDX = {n: i for i, (n, _, _) in enumerate(SETS)}
# how far a near piece reaches towards the road from its own middle, metres (as built, with the bounds' margin)
FRONT = {'PplRoadhouse': 2.5, 'PplDiner': 2.7, 'PplHitchA': 3.4, 'PplHitchB': 3.4, 'PplProspect': 3.7, 'PplLizards': 3.8, 'PplRiders': 3.2,
         'PplStandA': 2.4, 'PplStandB': 2.4, 'PplValets': 3.4, 'PplWedding': 3.5, 'PplGamblers': 3.3,
         'PplCrewCam': 2.9, 'PplCrewClapA': 2.9, 'PplCrewClapB': 2.9, 'PplCrewStunt': 2.9, 'PplCrewVillage': 2.9, 'PplCrewDolly': 2.9,
         'PplCrewCrane': 6.0, 'PplCrewLights': 2.2, 'PplCrewSound': 4.0, 'PplCrewCatering': 2.4}
PHONE = {'state': -1, 'snake': 1, 'joshua': 1, 'gas': -1, 'jackpot': 1, 'city': -1, 'salt': -1, 'vegas': 1}     # the side the phone line runs on


def rng(s0, s1):
    """The slots a dressing zone of (from km, to km) covers."""
    import math
    return range(max(1, int(math.ceil(s0 / SLOT - 1e-6))), int(math.floor(s1 / SLOT + 1e-6)) + 1)


# ---- what the dressing has where (FTDressL2.inl)
CUT_ZONES = [(1.154, 1.335), (1.606, 1.742), (1.787, 2.059), (5.407, 5.452), (5.633, 5.723), (5.995, 6.311), (18.572, 18.708), (18.753, 19.342),
             (19.930, 19.975), (20.020, 20.608), (20.654, 20.699), (21.332, 21.875), (21.920, 22.011)]
CUT = set()
for a, b in CUT_ZONES:
    CUT.update(rng(a, b))
BEND_R = set(rng(5.135, 5.452))     # the cut on the right, the 76 m cliff on the left
BEND_L = set(rng(5.678, 6.040))     # the cut on the left
DIST = [('state', 0.0, 1.17), ('wash', 1.17, 2.43), ('snake', 2.43, 3.83), ('joshua', 3.83, 5.14), ('redrock', 5.14, 6.31), ('gas', 6.31, 7.75),
        ('jackpot', 7.75, 9.05), ('city', 9.05, 10.69), ('salt', 10.69, 14.5), ('range', 14.5, 18.5), ('return', 18.5, 22.0), ('vegas', 22.0, 25.6)]
LAST = int(25.55 / SLOT)


def district(s):
    km = s * SLOT
    for n, a, b in DIST:
        if a <= km < b:
            return n
    return 'vegas'


def site_side(s, every):
    """The side a Site rule with side 0 takes on slot s."""
    return 1 if (s // every) % 2 else -1


def rim_ok(s, side):
    """A rock cut stands on this side of slot s, and no taller wall (CutLineB, every third slot) over it."""
    if s in CUT:
        if side < 0 and (s + 1) % 3 == 0:
            return False
        if side > 0 and (s + 2) % 3 == 0:
            return False
        return True
    if s in BEND_R and side > 0:
        return True
    if s in BEND_L and side < 0:
        return True
    return False


def near_free(s, side):
    """May a piece stand 24.6 to 30 m out on this side of slot s?"""
    d = district(s)
    if s in CUT or s in BEND_R or s in BEND_L or d in ('wash', 'redrock', 'return'):
        return False                                    # rock cuts 25 m out, rock banks 28.5 to 35 m
    if d == 'state' and (s + 2) % 4 == 0:
        return False                                    # mile signs, 24.6 to 27.5 m
    if d == 'gas' and (s + 3) % 7 == 0:
        return False                                    # ventilation heads, 27 to 29 m
    if d == 'jackpot' and s % 3 == 0:
        return False                                    # mile signs
    if d == 'salt' and (s + 2) % 5 == 0:
        return False
    if d == 'range':
        if s % 2 == 1:
            return False                                # rock banks, 28.5 to 40 m
        if (side < 0 and s % 3 == 0) or (side > 0 and (s + 1) % 3 == 0):
            return False                                # ventilation heads
    return True


ROWS = []            # (slot, side, set, x, scale)
TAKEN = set()        # (slot, side, band)


def put(s, side, name, x=None, scale=None, band='near'):
    """A row. Near pieces are sized here: 1.35 times as built (1.25 the crew), so that a figure is 2.3 m and reads at
    145 mph through the cel pass's ink; on the phone line's side 1.2 (1.0), and 7.5 m along, between two poles. The
    distance follows from the size: the front stands 24.7 m out."""
    if s < 1 or s > LAST or (s, side, band) in TAKEN:
        return False
    if band == 'near':
        if not near_free(s, side):
            return False
        crew = name.startswith('PplCrew')
        if PHONE.get(district(s)) == side:
            scale = 1.0 if crew else 1.2
            name += 'P'
        else:
            scale = 1.25 if crew else 1.35
        x = 24.7 + FRONT[name.rstrip('P') if name.endswith('P') else name] * scale
    TAKEN.add((s, side, band))
    ROWS.append((s, side, name, round(x, 1), scale or 1.0))
    return True


def alt(k):
    return -1 if k % 2 == 0 else 1


def build():
    r = random.Random(1993)
    # ------------------------------------------------------------------ the film crew: first, so that the cast gives way
    # the race starts about 120 m in: the unit is drawn up along the first 300 m after that
    start = [(4, 1, 'PplCrewCam'), (4, -1, 'PplCrewClapA'), (5, 1, 'PplCrewCrane'), (5, -1, 'PplCrewLights'), (11, 1, 'PplCrewVillage'), (11, -1, 'PplCrewClapB'),
             (7, -1, 'PplCrewCatering'), (7, 1, 'PplCrewSound'), (8, -1, 'PplCrewDolly'), (8, 1, 'PplCrewStunt'), (9, 1, 'PplCrewClapB'), (9, -1, 'PplCrewCam')]
    for s, side, n in start:
        put(s, side, n)
    # the first loop (3.1 to 3.3 km), the jumps at 4.6, 7.3 and 9.7 km, the second loop and corkscrew (8.1 to 8.6 km),
    # and the run in to the finale, where BIG DEALER's carrier waits (10.2 to 10.7 km)
    spots = [
        (65, -1, 'PplCrewCam'), (65, 1, 'PplCrewLights'), (66, 1, 'PplCrewCrane'), (66, -1, 'PplCrewClapA'), (67, 1, 'PplCrewStunt'), (75, -1, 'PplCrewClapB'), (75, 1, 'PplCrewSound'),
        (96, 1, 'PplCrewDolly'), (96, -1, 'PplCrewLights'), (97, -1, 'PplCrewCam'), (97, 1, 'PplCrewCrane'), (98, 1, 'PplCrewStunt'), (98, -1, 'PplCrewClapA'),
        (106, -1, 'PplCrewClapB'), (106, 1, 'PplCrewSound'), (107, 1, 'PplCrewVillage'),
        (155, -1, 'PplCrewCam'), (155, 1, 'PplCrewLights'), (156, 1, 'PplCrewCrane'), (156, -1, 'PplCrewStunt'), (157, -1, 'PplCrewClapA'), (157, 1, 'PplCrewDolly'),
        (166, 1, 'PplCrewClapB'), (167, -1, 'PplCrewSound'),
        (176, 1, 'PplCrewCam'), (176, -1, 'PplCrewCrane'), (177, -1, 'PplCrewClapA'), (178, 1, 'PplCrewLights'), (191, 1, 'PplCrewClapB'), (192, -1, 'PplCrewVillage'),
        (209, -1, 'PplCrewDolly'), (209, 1, 'PplCrewCrane'), (210, 1, 'PplCrewCam'), (210, -1, 'PplCrewLights'), (211, -1, 'PplCrewStunt'), (211, 1, 'PplCrewClapA'),
        (228, 1, 'PplCrewCatering'), (228, -1, 'PplCrewVillage'), (229, -1, 'PplCrewCam'), (229, 1, 'PplCrewCrane'), (230, 1, 'PplCrewClapB'), (230, -1, 'PplCrewLights'),
        (231, -1, 'PplCrewDolly'), (231, 1, 'PplCrewSound'), (232, 1, 'PplCrewStunt'), (232, -1, 'PplCrewClapA'),
    ]
    for s, side, n in spots:
        put(s, side, n)
    # thinly along the way: one position every 13 slots (590 m), the kinds and the sides in turn
    thin = ('PplCrewClapA', 'PplCrewCam', 'PplCrewCrane', 'PplCrewClapB', 'PplCrewDolly', 'PplCrewLights', 'PplCrewVillage', 'PplCrewStunt')
    k = 0
    for s in range(16, LAST, 13):
        for ds in (0, 1, 2):
            if put(s + ds, alt(k), thin[k % len(thin)]):
                break
        k += 1
    for s, side, n in ((LAST - 6, -1, 'PplCrewCam'), (LAST - 6, 1, 'PplCrewClapA'), (LAST - 4, 1, 'PplCrewCrane'), (LAST - 4, -1, 'PplCrewClapB')):
        put(s, side, n)

    # ------------------------------------------------------------------ the cast, district by district
    for s in range(7, LAST + 1):
        d = district(s)
        a = alt(s // 2)                      # a side that changes every second slot
        stop = site_side(s, 8) if (s + 3) % 8 == 0 else 0        # a truck stop 40 m out on that side (where its zones run)
        if d == 'state':
            if stop and s * SLOT >= 0.3:
                put(s, stop, 'PplDiner')
            m = s % 8
            if m in (0, 4):
                put(s, a, 'PplRiders')
            elif m in (1, 5):
                put(s, -stop if stop else a, 'PplHitchA' if m == 1 else 'PplHitchB')
            elif m == 3:
                put(s, a, 'PplLizards')
            elif m == 7:
                put(s, a, 'PplProspect')
            if s % 4 == 2:
                put(s, a, 'PplRiders', x=42.0, scale=1.5, band='mid')
            if s % 6 == 0:
                put(s, -a, 'PplCattle', x=r.uniform(95, 140), band='far')
            if s % 6 == 3:
                put(s, a, 'PplHorses', x=r.uniform(120, 180), band='far')
        elif d in ('wash', 'redrock', 'return'):
            for side in (-1, 1):
                if rim_ok(s, side):
                    m = (s + (0 if side < 0 else 1)) % 3
                    if m == 0:
                        put(s, side, 'PplRimCoyotes', x=33.1, band='rim')
                    elif m == 1 and r.random() < 0.85:
                        put(s, side, 'PplRimPosse', x=33.1, band='rim')
            if s not in CUT and s not in BEND_R and s not in BEND_L:
                # the open bends between the cuts: hoodoos with coyotes or a posse on them, 46 to 54 m out (odd slots:
                # the dressing's own hoodoos stand there on the even ones), wild horses beyond
                if s % 2 == 1:
                    put(s, a, 'PplRimCoyotes' if s % 4 == 1 else 'PplRimPosse', x=r.uniform(46, 54), band='mid')
                if s % 5 == 0 and d == 'wash':
                    put(s, -a, 'PplHorses', x=r.uniform(120, 170), band='far')
        elif d == 'snake':
            den = site_side(s, 4) if s % 4 == 0 else 0
            m = s % 8
            if den:
                put(s, den, 'PplRoadhouse')
                put(s, -den, 'PplProspect' if m == 0 else 'PplLizards')
            elif stop:
                put(s, stop, 'PplDiner')
                put(s, -stop, 'PplHitchB')
            elif m == 1:
                put(s, a, 'PplLizards')
            elif m == 2:
                put(s, a, 'PplHitchA')
                put(s, -a, 'PplRoadhouse')
            elif m == 3:
                put(s, a, 'PplRiders')
            elif m == 6:
                put(s, a, 'PplHitchB')
                put(s, -a, 'PplRoadhouse')
            elif m == 7:
                put(s, a, 'PplProspect')
            if s % 6 == 2:
                put(s, a, 'PplHorses', x=r.uniform(175, 230), band='far')
        elif d == 'joshua':
            m = s % 6
            if m in (0, 3):
                put(s, a, 'PplRiders')
            elif m == 1:
                put(s, a, 'PplLizards')
                put(s, -a, 'PplRiders', x=42.0, scale=1.5, band='mid')
            elif m == 2:
                put(s, a, 'PplHitchA' if (s // 6) % 2 else 'PplHitchB')
            elif m == 4:
                put(s, a, 'PplProspect')
            elif m == 5:
                put(s, a, 'PplRimCoyotes', x=r.uniform(40, 50), band='mid')
            if s % 7 == 0:
                put(s, -a, 'PplCattle', x=r.uniform(90, 135), band='far')
        elif d == 'gas':
            rs = site_side(s, 4) if s % 4 == 0 else 0            # a road stop (the diner) 64 m out
            m = s % 8
            if rs:
                put(s, rs, 'PplDiner')
                put(s, -rs, 'PplHitchA' if m == 0 else 'PplLizards')
            elif stop:
                put(s, stop, 'PplDiner')
                put(s, -stop, 'PplProspect')
            elif m == 1:
                put(s, a, 'PplProspect')
            elif m == 2:
                put(s, a, 'PplHitchB')
            elif m in (3, 6):
                put(s, a, 'PplRiders')
            elif m == 7:
                put(s, a, 'PplHitchA')
            if s % 9 == 4:
                put(s, a, 'PplCattle', x=r.uniform(110, 145), band='far')
        elif d == 'jackpot':
            m = s % 6
            pair = {1: ('PplStandA', 'PplGamblers'), 2: ('PplValets', 'PplStandB'), 4: ('PplGamblers', 'PplWedding'), 5: ('PplWedding', 'PplValets')}.get(m)
            if stop:
                put(s, stop, 'PplDiner')
            if pair:
                put(s, a, pair[0])
                put(s, -a, pair[1])
            if m == 3 and (s + 1) % 4 != 0:
                put(s, a, 'PplStandB', x=43.0, scale=1.5, band='mid')
        elif d == 'city':
            rs = site_side(s, 5) if s % 5 == 0 else 0
            m = s % 5
            if stop:
                put(s, stop, 'PplDiner')
            if rs:
                put(s, rs, 'PplDiner')
                put(s, -rs, 'PplStandA')
            else:
                pair = {1: ('PplValets', 'PplGamblers'), 2: ('PplWedding', 'PplStandB'), 3: ('PplStandB', 'PplValets'), 4: ('PplGamblers', 'PplHitchA')}[m]
                put(s, a, pair[0])
                put(s, -a, pair[1])
            if s % 4 == 0 and not stop:
                put(s, a, 'PplStandA', x=43.0, scale=1.5, band='mid')
        elif d == 'salt':
            m = s % 5
            c = (s // 5) % 2
            if m == 0:
                put(s, a, 'PplRiders')
            elif m == 1:
                put(s, a, ('PplHitchA', 'PplLizards')[c])
            elif m == 2:
                put(s, a, ('PplProspect', 'PplHitchB')[c])
            elif m == 4:
                put(s, -a, 'PplRiders')
            if m == 3:
                put(s, a, 'PplRiders', x=42.0, scale=1.5, band='mid')
            if s % 4 == 0:      # the freight train runs 80 m out on the left from 10.68 to 11.58 km: the herds keep right there
                put(s, 1 if s * SLOT < 11.7 else a, 'PplHorses', x=r.uniform(115, 200), band='far')
            if s % 4 == 2:
                put(s, 1 if s * SLOT < 11.7 else -a, 'PplCattle', x=r.uniform(95, 160), band='far')
        elif d == 'range':
            m = s % 8
            if m == 0:
                put(s, a, 'PplProspect') or put(s, -a, 'PplProspect')
            elif m == 2:
                put(s, a, 'PplRiders') or put(s, -a, 'PplRiders')
            elif m == 4:
                put(s, a, 'PplLizards') or put(s, -a, 'PplLizards')
            elif m == 6:
                put(s, a, 'PplHitchB') or put(s, -a, 'PplHitchA')
            if s % 6 == 1:
                put(s, a, 'PplRimCoyotes', x=r.uniform(62, 70), scale=1.15, band='mid')
            if s % 6 == 3:
                put(s, -a, 'PplRimPosse', x=r.uniform(62, 70), scale=1.15, band='mid')
            if s % 5 == 0:
                put(s, a, 'PplHorses', x=r.uniform(96, 116), band='far')
            if s % 10 == 7:
                put(s, -a, 'PplCattle', x=r.uniform(96, 116), band='far')
        elif d == 'vegas':
            rs = site_side(s, 5) if s % 5 == 0 else 0
            m = s % 5
            if stop:
                put(s, stop, 'PplDiner')
            if rs:
                put(s, rs, 'PplDiner')
                put(s, -rs, 'PplStandA')
            else:
                pair = {1: ('PplValets', 'PplWedding'), 2: ('PplGamblers', 'PplStandB'), 3: ('PplStandA', 'PplValets'), 4: ('PplWedding', 'PplGamblers')}[m]
                put(s, a, pair[0])
                put(s, -a, pair[1])
            if s % 4 == 1 and not stop:
                put(s, a, 'PplStandB' if s % 8 == 1 else 'PplStandA', x=44.0, scale=1.55, band='mid')
    ROWS.sort()
    return ROWS


HEAD = """// Nevada (level index 2): the population layer. People, creatures and the film crew, placed by rule like the dressing
// (the words and what their numbers mean are in the header of FTTrackDress.cpp; the rule kinds in FTDress.h).
// This table is read after FTDressL2.inl, so the dressing's own placements never move.
//
// WRITTEN BY Content/Python/ft_people_nevada_table.py (plain Python): change that file and run it, not this one.
// The meshes are made by Content/Python/ft_people_nevada.py (/Game/People/Nevada/) and ft_people_crew.py (the crew).
//
// Slot by slot: every row below is one Site rule in a zone of its own (slot s lies s * 45.243 m along the road), so a
// piece stands exactly on its slot, on the side and at the distance the writer gave it, clear of what the dressing has
// there (rock cuts, rock banks, ventilation heads, mile signs; in front of the roadhouses, truck stops, road stops).
// The land lies a metre under this road behind a metre of barrier, so everybody is RAISED: every near piece is a deck,
// a bank or a podium two metres up (TALL: its front stands 24.6 m out or more), the film crew's low pieces stand on a
// scaffold riser (a set: the crew piece 2 m up on SM_CrewRiser), and in the rock cuts the cast is up on the rim (a
// 16.5 m column of the cut's own rock inside the 15 m cut, 33.1 m out). Far life (herds) was saved far=True.
// Rules that bind this layer: nothing over 2 m tall nearer than 24 m to a centre line and nothing at all nearer than
// 17 m; nothing lit; nothing that could be taken for a pickup, an enemy or an attack.
"""


def write(path):
    rows = build()
    out = [HEAD]
    for name, parts, along in SETS:
        out.append('Set(TEXT("%s"), { %s });\n' % (name, ', '.join('{ TEXT("%s"), %s, 0, %s, 0, 1.f }' % (m, ('%.1ff' % along) if along else '0', ('%.1ff' % up) if up else '0') for m, up in parts)))
    out.append('\nstatic const TCHAR* const PplSets[] = { %s };\n' % ', '.join('TEXT("%s")' % n for n, _, _ in SETS))
    out.append('struct FPplRow { int16 Slot; int8 Side; uint8 Set; float X, Scale; };\n')
    out.append('// slot, side (-1 left of travel, +1 right), set, metres from the centre line, scale\n')
    out.append('static const FPplRow PplRows[] = {\n')
    line = []
    for (s, side, name, x, sc) in rows:
        line.append('{%d,%d,%d,%.1ff,%sf}' % (s, side, IDX[name], x, ('%.2f' % sc).rstrip('0').rstrip('.') + ('.' if float(sc) == int(sc) else '')))
        if len(line) == 7:
            out.append('\t' + ', '.join(line) + ',\n')
            line = []
    if line:
        out.append('\t' + ', '.join(line) + ',\n')
    out.append('};\n')
    out.append('for (const FPplRow& R : PplRows)\n{\n\tZone(TEXT("People"), (R.Slot - 0.4f) * %.6ff, (R.Slot + 0.4f) * %.6ff);\n\tSite(PplSets[R.Set], R.Side, R.X, 1, 0, 1.f, R.Scale);\n}\n' % (SLOT, SLOT))
    with open(path, 'w', newline='\n') as f:
        f.write(''.join(out))
    count = {}
    PARTS = {n: p for n, p, _ in SETS}
    pieces = 0
    for (s, side, name, x, sc) in rows:
        count[name] = count.get(name, 0) + 1
        pieces += len(PARTS[name])
    print('rows', len(rows), 'pieces', pieces)
    for n, _, _ in SETS:
        print('  %-16s %d' % (n, count.get(n, 0)))
    return rows


if __name__ == '__main__':
    here = os.path.dirname(os.path.abspath(__file__))
    write(os.path.join(here, '..', '..', 'Source', 'FullThrottle', 'FTPeopleL2.inl'))
