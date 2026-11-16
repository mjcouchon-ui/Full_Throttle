"""FULL THROTTLE boss overhaul (15 Nov 2026): the bosses' own effects, built with ft_fx.py's helpers.

Every system here is /Game/FX/NS_Boss<Name>; FTFx (FTFx.cpp) loads any /Game/FX system by name, and the bosses' rigs
(FTEntity.cpp, "the bosses' rigs") spawn them: bursts attached to the boss (FTFx::Attach), loops held by the rig
(AFTEntityActor::Loop). Like every lightweight emitter they are local space: sprites travel with what they are on,
so nothing here trails down the road toward the player, and nothing lies on the road where a warning would be.
Each belongs to a real cue (a hit, a damage state, a charge, a release, the defeat): none of them is decoration.

Run in the editor:  import ft_fx_boss; ft_fx_boss.build_all()        (or build_all(('NS_BossZapFlash',)))
"""
import unreal
import ft_fx
from ft_fx import fx, heat, cel, SPARK, GLOW, LOOP, BURST
from ft_fx import C_FIRE, C_EMBER, C_ZAP, C_WHITE, C_SMOKE, C_SMOKE_D, C_SMOKE_L, C_GOLD, C_ICE, C_SNOW, C_SEA, C_HE, C_FOE, C_DUST

AL = unreal.EditorAssetLibrary
# 2: sprites sized to be seen from the fight's distance (a boss on his spot is 58 m away: 37 px to the metre at 3440)
# 3: round two: NS_BossPillar, NS_BossWreckFire, NS_BossWake (the entrances, the ends, the shark's wake)
VERSION = 3
ARCS = heat((C_WHITE, 1.0), (C_ZAP, 0.5), ((0.3, 0.5, 1.0), 0.0))
FLAME = heat(((1.0, 0.85, 0.35), 1.0), (C_FIRE, 0.6), (C_EMBER, 0.0))


def definitions():
    d = []

    def add(name, cfg):
        d.append((name, LOOP if 'Type=Rate' in cfg['emitter']['SpawnInfos'] else BURST, cfg))

    # ---- every boss: a hit, a damage state, what a damaged boss trails, the defeat
    add('NS_BossHurtSparks', fx(n=16, life=(0.12, 0.3), size=((14, 60), (24, 110)), mat=SPARK, ramp=[(12, 9, 4, 1), (7, 2.5, 0.5, 1), (2, 0.3, 0.05, 0)],
                                speed=(900, 2600), cone=70.0, axis=(-1, 0, 0.5), gravity=-900.0, stretch=(1200, 9.0), align=True, pool=(2, 8)))
    add('NS_BossBreak', fx(n=2, life=(0.24, 0.32), size=((380, 380), (480, 480)), mat=cel('Burst'),
                           ramp=heat(((1.0, 0.9, 0.6), 1.0), (C_FIRE, 0.65), (C_EMBER, 0.3), ((0.5, 0.06, 0.04), 0.0)), grow=(0.4, 1.0, 1.3), rot=True, sort=True))
    add('NS_BossBreakShards', fx(n=22, life=(0.5, 1.0), size=((30, 70), (50, 130)), mat=cel('Shard'),
                                 ramp=heat(((0.9, 0.9, 1.0), 1.0), ((0.5, 0.5, 0.62), 0.6), ((0.2, 0.2, 0.3), 0.0)),
                                 speed=(900, 2400), cone=75.0, axis=(-0.3, 0, 1), gravity=-1300.0, stretch=(1500, 1.6), align=True, sort=True))
    # smoke and fire stand up off the roof (they never stream back along the road at the car)
    add('NS_BossSmoke', fx(rate=7, life=(0.8, 1.2), size=((140, 140), (220, 220)), mat=cel('Puff'), ramp=heat((C_SMOKE_D, 1.0), (C_SMOKE, 0.6), (C_SMOKE, 0.0)),
                           grow=(0.6, 1.4, 1.9), speed=(260, 480), cone=14.0, axis=(-0.25, 0, 1), drag=0.6, sort=True, owner=False))
    add('NS_BossFlame', fx(rate=12, life=(0.3, 0.5), size=((120, 150), (170, 220)), mat=cel('Blob'), ramp=FLAME, grow=(1.0, 0.8, 0.3),
                           speed=(200, 380), cone=14.0, axis=(-0.3, 0, 1), sphere=24.0, rot=True, sort=True, owner=False))
    add('NS_BossDefeat', fx(n=3, life=(0.3, 0.42), size=((620, 620), (820, 820)), mat=cel('Burst'),
                            ramp=heat(((1.0, 0.95, 0.7), 1.0), ((1.0, 0.6, 0.15), 0.7), (C_EMBER, 0.3), ((0.5, 0.06, 0.04), 0.0)),
                            grow=(0.35, 1.0, 1.35), rot=True, sort=True))
    # ---- round two: the entrances and the ends. A pillar of fire that stands straight up whatever it is on (the
    # Juggernaut's stacks as he arrives, a magazine or a fuel tank going up: 12 m at size 1, tall enough to show over
    # the wrap's card), and the fire a boss's wreck burns with (it rises in the world while the wreck tumbles)
    add('NS_BossPillar', fx(n=14, life=(0.35, 0.6), size=((150, 150), (240, 240)), mat=cel('Blob'), ramp=FLAME, grow=(0.7, 1.2, 0.6),
                            speed=(900, 3400), cone=7.0, axis=(0, 0, 1), world=True, drag=1.2, rot=True, sort=True))
    add('NS_BossWreckFire', fx(rate=14, life=(0.35, 0.6), size=((150, 180), (220, 280)), mat=cel('Blob'), ramp=FLAME, grow=(1.0, 0.9, 0.3),
                               speed=(300, 620), cone=16.0, axis=(0, 0, 1), world=True, sphere=40.0, rot=True, sort=True, owner=False))
    # ---- THE ALCHEMIST: the coil crackles as it fills, lets go in a flash and a spray of arcs, and shorts when hit
    add('NS_BossHurtZap', fx(n=12, life=(0.07, 0.16), size=((22, 90), (32, 160)), mat=cel('Shard'), ramp=ARCS,
                             speed=(1200, 3000), stretch=(1500, 1.8), align=True, sort=True, pool=(2, 8)))
    add('NS_BossZapCrackle', fx(rate=22, life=(0.06, 0.13), size=((20, 90), (30, 170)), mat=cel('Shard'), ramp=ARCS,
                                speed=(900, 2400), sphere=190.0, stretch=(1200, 1.8), align=True, sort=True, owner=False))
    add('NS_BossZapFlash', fx(n=1, life=(0.09, 0.11), size=((560, 560), (640, 640)), mat=cel('Flash'), ramp=heat((C_WHITE, 1.0), (C_ZAP, 0.5), (C_ZAP, 0.0)),
                              grow=(0.55, 1.0, 1.1), rot=True, sort=True, pool=(2, 6)))
    add('NS_BossZapSpray', fx(n=18, life=(0.1, 0.22), size=((28, 140), (42, 260)), mat=cel('Shard'), ramp=ARCS,
                              speed=(1800, 4200), sphere=120.0, stretch=(1800, 1.8), align=True, sort=True, pool=(2, 6)))
    add('NS_BossDefeatZap', fx(n=3, life=(0.3, 0.42), size=((700, 700), (900, 900)), mat=cel('Flash'),
                               ramp=heat((C_WHITE, 1.0), (C_ZAP, 0.6), ((0.25, 0.45, 1.0), 0.3), ((0.1, 0.2, 0.7), 0.0)), grow=(0.35, 1.0, 1.3), rot=True, sort=True))
    # ---- THE BIG DEALER: chips when he is hit, a ring as a rider leaves the lift, dust at the ramp's foot as one
    # rolls off it, tyre smoke through the brake check
    add('NS_BossHurtChips', fx(n=14, life=(0.35, 0.7), size=((46, 46), (70, 70)), mat=cel('Blob'),
                               ramp=heat(((1.0, 0.2, 0.3), 1.0), ((1.0, 0.85, 0.3), 0.7), ((0.95, 0.95, 0.9), 0.4), ((0.9, 0.9, 0.9), 0.0)),
                               speed=(700, 1900), cone=65.0, axis=(-1, 0, 0.7), gravity=-1200.0, rot=True, spin=400.0, sort=True, pool=(2, 8)))
    add('NS_BossLaunch', fx(n=1, life=(0.12, 0.15), size=((300, 300), (340, 340)), mat=cel('Ring'), ramp=heat((C_WHITE, 1.0), ((1.0, 0.8, 0.3), 0.5), (C_FIRE, 0.0)),
                            grow=(0.3, 0.9, 1.0), sort=True))
    add('NS_BossDust', fx(n=10, life=(0.4, 0.7), size=((140, 140), (220, 220)), mat=cel('Puff'), ramp=heat((C_DUST, 1.0), (C_DUST, 0.6), (C_DUST, 0.0)),
                          grow=(0.6, 1.5, 2.0), speed=(300, 900), cone=50.0, axis=(-1, 0, 0.4), drag=3.0, box=(60, 500, 10), sort=True))
    add('NS_BossTyreSmoke', fx(rate=18, life=(0.4, 0.7), size=((110, 110), (170, 170)), mat=cel('Puff'), ramp=heat((C_SMOKE_L, 1.0), (C_SMOKE_L, 0.6), (C_SMOKE, 0.0)),
                               grow=(0.6, 1.5, 2.0), speed=(250, 600), cone=30.0, axis=(-0.6, 0, 1), drag=2.0, box=(60, 40, 6), sort=True, owner=False))
    add('NS_BossDefeatChips', fx(n=3, life=(0.3, 0.42), size=((620, 620), (820, 820)), mat=cel('Burst'),
                                 ramp=heat((C_WHITE, 1.0), (C_GOLD, 0.7), ((1.0, 0.3, 0.35), 0.3), ((0.5, 0.06, 0.1), 0.0)), grow=(0.35, 1.0, 1.35), rot=True, sort=True))
    # ---- THE JUGGERNAUT (and THE CHAIRMAN's turrets): the foes' red at the muzzles as a fan of rounds leaves
    add('NS_BossGunFlash', fx(n=1, life=(0.07, 0.09), size=((230, 230), (280, 280)), mat=cel('Flash'), ramp=heat((C_WHITE, 1.0), (C_FOE, 0.55), (C_FOE, 0.0)),
                              grow=(0.5, 1.0, 1.1), rot=True, sort=True, pool=(2, 8)))
    # ---- ENGINE 13: the jet at a nozzle while its lane burns, steam off the safety valve as a wind-up starts, the
    # ring at the cannon's mouth as it lobs
    add('NS_BossFlameJet', fx(rate=46, life=(0.24, 0.36), size=((90, 90), (150, 150)), mat=cel('Blob'),
                              ramp=heat(((1.0, 0.9, 0.5), 1.0), (C_FIRE, 0.65), (C_EMBER, 0.3), ((0.5, 0.06, 0.04), 0.0)), grow=(0.6, 1.3, 1.8),
                              speed=(3600, 5200), cone=5.0, axis=(-1, 0, 0), rot=True, sort=True, owner=False))
    add('NS_BossSteam', fx(n=8, life=(0.4, 0.7), size=((120, 120), (180, 180)), mat=cel('Puff'),
                           ramp=heat(((0.95, 0.95, 1.0), 1.0), ((0.85, 0.85, 0.95), 0.6), ((0.8, 0.8, 0.9), 0.0)), grow=(0.6, 1.5, 2.1),
                           speed=(500, 1100), cone=20.0, drag=2.5, sort=True))
    add('NS_BossMortar', fx(n=1, life=(0.12, 0.15), size=((320, 320), (380, 380)), mat=cel('Ring'), ramp=heat(((1.0, 0.9, 0.6), 1.0), (C_FIRE, 0.5), (C_EMBER, 0.0)),
                            grow=(0.3, 0.9, 1.0), sort=True))
    add('NS_BossDefeatFire', fx(n=4, life=(0.34, 0.5), size=((640, 640), (900, 900)), mat=cel('Burst'),
                                ramp=heat(((1.0, 0.95, 0.6), 1.0), (C_FIRE, 0.7), (C_EMBER, 0.3), ((0.5, 0.06, 0.04), 0.0)), grow=(0.35, 1.0, 1.4), rot=True, sort=True))
    # ---- MR. CHUCKLES: confetti when he is hit and as his bombs go, a pop for every balloon, cream as the pies leave
    party = heat(((1.0, 0.25, 0.75), 1.0), ((1.0, 0.85, 0.2), 0.75), ((0.25, 0.9, 1.0), 0.4), ((0.3, 0.4, 1.0), 0.0))
    add('NS_BossHurtConfetti', fx(n=18, life=(0.4, 0.8), size=((30, 60), (46, 90)), mat=cel('Shard'), ramp=party,
                                  speed=(700, 2000), cone=70.0, axis=(-1, 0, 0.6), gravity=-900.0, stretch=(1500, 1.6), align=True, sort=True, pool=(2, 8)))
    add('NS_BossConfetti', fx(n=26, life=(0.6, 1.1), size=((34, 70), (50, 100)), mat=cel('Shard'), ramp=party,
                              speed=(500, 1500), cone=80.0, gravity=-600.0, drag=1.5, align=True, sort=True))
    add('NS_BossPop', fx(n=1, life=(0.1, 0.13), size=((240, 240), (300, 300)), mat=cel('Flash'), ramp=heat((C_WHITE, 1.0), ((1.0, 0.3, 0.7), 0.55), ((1.0, 0.3, 0.7), 0.0)),
                         grow=(0.5, 1.0, 1.15), rot=True, sort=True, pool=(3, 9)))
    add('NS_BossPieBlast', fx(n=1, life=(0.1, 0.13), size=((300, 300), (360, 360)), mat=cel('Burst'),
                              ramp=heat(((1.0, 0.98, 0.92), 1.0), ((1.0, 0.86, 0.9), 0.6), (C_FOE, 0.0)), grow=(0.5, 1.0, 1.2), rot=True, sort=True))
    add('NS_BossDefeatConfetti', fx(n=3, life=(0.3, 0.42), size=((620, 620), (820, 820)), mat=cel('Burst'),
                                    ramp=heat(((1.0, 0.95, 0.5), 1.0), ((1.0, 0.3, 0.75), 0.7), ((0.25, 0.9, 1.0), 0.3), ((0.3, 0.4, 1.0), 0.0)),
                                    grow=(0.35, 1.0, 1.35), rot=True, sort=True))
    # ---- THE ABOMINABLE: snow off him when he is hit, kicked up when he runs at the car, thrown up by the pound and
    # the roar; ice in shards when a crust is shot off him
    snow = heat((C_SNOW, 1.0), (C_SNOW, 0.7), (C_ICE, 0.0))
    add('NS_BossHurtSnow', fx(n=8, life=(0.35, 0.6), size=((110, 110), (180, 180)), mat=cel('Puff'), ramp=snow, grow=(0.5, 1.4, 1.9),
                              speed=(300, 900), cone=70.0, axis=(-1, 0, 0.5), drag=2.4, sphere=60.0, sort=True, pool=(2, 8)))
    add('NS_BossSnowKick', fx(rate=14, life=(0.4, 0.7), size=((120, 120), (190, 190)), mat=cel('Puff'), ramp=snow, grow=(0.6, 1.4, 1.9),
                              speed=(400, 900), cone=30.0, axis=(1, 0, 0.6), drag=2.0, box=(60, 400, 10), sort=True, owner=False))
    add('NS_BossSnowSlam', fx(n=14, life=(0.5, 0.9), size=((180, 180), (280, 280)), mat=cel('Puff'), ramp=snow, grow=(0.5, 1.4, 1.9),
                              speed=(400, 1300), cone=85.0, drag=2.2, sphere=120.0, sort=True))
    add('NS_BossIceBreak', fx(n=20, life=(0.4, 0.8), size=((36, 90), (54, 140)), mat=cel('Shard'), ramp=heat((C_WHITE, 1.0), (C_ICE, 0.6), (C_ICE, 0.0)),
                              speed=(1000, 2600), cone=60.0, gravity=-1500.0, stretch=(1500, 1.8), align=True, sort=True))
    add('NS_BossDefeatSnow', fx(n=4, life=(0.34, 0.5), size=((640, 640), (900, 900)), mat=cel('Burst'),
                                ramp=heat((C_SNOW, 1.0), (C_ICE, 0.65), ((0.35, 0.6, 1.0), 0.3), ((0.3, 0.5, 0.9), 0.0)), grow=(0.35, 1.0, 1.4), rot=True, sort=True))
    # ---- THE GREAT WHITE: spray when it is hit and as the flying fish go, a flash in the sea's colour at the
    # launchers (or its mouth) as the swordfish leave
    sea = heat((C_WHITE, 1.0), (C_SEA, 0.6), (C_SEA, 0.0))
    add('NS_BossHurtSpray', fx(n=16, life=(0.3, 0.6), size=((28, 70), (42, 110)), mat=cel('Shard'), ramp=sea,
                               speed=(800, 2200), cone=65.0, axis=(-1, 0, 0.6), gravity=-1400.0, stretch=(1500, 1.8), align=True, sort=True, pool=(2, 8)))
    add('NS_BossSpray', fx(n=22, life=(0.4, 0.75), size=((30, 80), (46, 120)), mat=cel('Shard'), ramp=sea,
                           speed=(900, 2300), cone=40.0, gravity=-1600.0, stretch=(1500, 1.8), align=True, sort=True))
    add('NS_BossFishFlash', fx(n=1, life=(0.08, 0.1), size=((220, 220), (260, 260)), mat=cel('Flash'), ramp=heat((C_WHITE, 1.0), (C_SEA, 0.55), (C_SEA, 0.0)),
                               grow=(0.5, 1.0, 1.1), rot=True, sort=True, pool=(2, 8)))
    # (round two) its wake: a low sheet of spray running back off a flank, knee high and gone in 0.4 s. The rig holds
    # one each side while it swims ahead, and drops both before anything is fired (FTEntity.cpp, AnimShark)
    add('NS_BossWake', fx(rate=30, life=(0.25, 0.42), size=((70, 70), (120, 120)), mat=cel('Puff'), ramp=sea, grow=(0.6, 1.3, 1.8),
                          speed=(700, 1300), cone=14.0, axis=(-1, 0, 0.16), drag=1.6, box=(40, 30, 6), sort=True, owner=False))
    add('NS_BossDefeatSplash', fx(n=4, life=(0.34, 0.5), size=((640, 640), (900, 900)), mat=cel('Burst'),
                                  ramp=heat((C_WHITE, 1.0), (C_SEA, 0.65), ((0.08, 0.45, 0.8), 0.3), ((0.06, 0.3, 0.6), 0.0)), grow=(0.35, 1.0, 1.4), rot=True, sort=True))
    # ---- THE CHAIRMAN: banknotes out of him when he is hit, the sunroof guns' fire while the stream runs, rockets
    # away upward out of the bays, the two jets he hangs on
    cash = heat(((0.5, 1.0, 0.5), 1.0), ((0.3, 0.85, 0.4), 0.6), ((0.2, 0.6, 0.3), 0.0))
    add('NS_BossHurtBills', fx(n=12, life=(0.6, 1.1), size=((60, 32), (90, 48)), mat=cel('Shard'), ramp=cash,
                               speed=(500, 1500), cone=75.0, axis=(-0.6, 0, 1), gravity=-500.0, drag=2.2, rot=True, spin=500.0, sort=True, pool=(2, 8)))
    add('NS_BossDefeatCash', fx(n=4, life=(0.34, 0.5), size=((640, 640), (900, 900)), mat=cel('Burst'),
                                ramp=heat((C_WHITE, 1.0), (C_GOLD, 0.7), ((0.5, 1.0, 0.5), 0.35), ((0.2, 0.6, 0.3), 0.0)), grow=(0.35, 1.0, 1.4), rot=True, sort=True))
    add('NS_BossGunStream', fx(rate=16, life=(0.05, 0.07), size=((120, 120), (170, 170)), mat=cel('Flash'), ramp=heat((C_WHITE, 1.0), (C_FOE, 0.55), (C_FOE, 0.0)),
                               grow=(0.6, 1.0, 1.1), rot=True, sort=True, box=(4, 70, 4), owner=False))
    add('NS_BossRocketUp', fx(n=6, life=(0.3, 0.5), size=((30, 120), (44, 200)), mat=cel('Shard'), ramp=heat((C_WHITE, 1.0), (C_HE, 0.6), (C_SMOKE, 0.0)),
                              speed=(3000, 5200), cone=10.0, axis=(0.25, 0, 1), stretch=(2000, 2.0), align=True, sort=True))
    add('NS_BossJet', fx(rate=40, life=(0.07, 0.12), size=((40, 90), (60, 150)), mat=cel('Shard'),
                         ramp=heat(((0.7, 0.95, 1.0), 1.0), ((1.0, 0.6, 0.15), 0.6), ((1.0, 0.25, 0.05), 0.0)),
                         speed=(1200, 2000), cone=6.0, axis=(-1, 0, -0.15), stretch=(900, 1.6), align=True, sort=True, owner=False))
    return d


def build_all(only=None):
    made = []
    for name, base, cfg in definitions():
        if only and name not in only:
            continue
        made.append(ft_fx.make(name, base, cfg))
    return [s.get_name() for s in made]
