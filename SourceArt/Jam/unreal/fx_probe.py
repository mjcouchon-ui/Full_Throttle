"""Attack effects, step 0: what the stateless Niagara templates and the stock systems really contain.

Dumps every editable property (names and current text values) of the two lightweight templates and a few stock
systems to DevLog/jam/fx_probe.txt, so the effect specs in ft_fx.py use the engine's own property names and formats.
Run: jam_run.ps1 -Script fx_probe.py
"""
import unreal

T = unreal.FTEditorTools
E = 'EmitterHandles[0].StatelessEmitter.'
try:
    out = []
    for p in ('/Niagara/DefaultAssets/Templates/Systems/FountainLightweight', '/Niagara/DefaultAssets/Templates/Systems/DirectionalBurstLightweight',
              '/Game/FX/NS_Sparks', '/Game/FX/NS_TurboFlame', '/Game/FX/NS_Muzzle', '/Game/FX/NS_MissileTrail', '/Game/FX/Pickups/NS_PickupMissile'):
        s = unreal.load_asset(p)
        out.append('#### ' + p)
        if s is None:
            out.append('MISSING')
            continue
        out.append(T.dump_object(s, 6))
        i = 0
        while True:
            v = T.get_path(s, E + 'Modules[%d]' % i)
            if v.startswith('ERROR'):
                break
            out.append('MODULE %d %s enabled=%s' % (i, v, T.get_path(s, E + 'Modules[%d].bModuleEnabled' % i)))
            i += 1
        for k in ('SpawnInfos', 'FixedBounds', 'EmitterState', 'bDeterministic', 'RandomSeed', 'RendererProperties[0].Material', 'RendererProperties[0].Alignment',
                  'RendererProperties[0].FacingMode', 'RendererProperties[0].SortMode', 'RendererProperties[0].PivotInUVSpace', 'RendererProperties[0].SubImageSize'):
            out.append('%s = %s' % (k, T.get_path(s, E + k)))
        for k in ('PoolPrimeSize', 'MaxPoolSize', 'bFixedBounds', 'WarmupTime'):
            out.append('SYS %s = %s' % (k, T.get_path(s, k)))
    path = r'K:\Full Throttle\SkyScratch\DevLog\jam\fx_probe.txt'
    with open(path, 'w', encoding='utf-8') as f:
        f.write('\n'.join(out))
    print('JAM fx probe written', path, len(out))
except Exception as e:  # noqa
    import traceback
    print('JAM ERR', repr(e), traceback.format_exc())
unreal.SystemLibrary.quit_editor()
