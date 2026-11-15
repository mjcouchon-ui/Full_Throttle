"""FULL THROTTLE (Unreal) the dressing (3.2): modular scenery kits that fill each level's roadside, middle distance
and horizon, beside the locked track.

One module per level builds that level's kit (ft_dress_bay, ft_dress_chem, ft_dress_nevada, ft_dress_volcano,
ft_dress_park, ft_dress_ice, ft_dress_abyss, ft_dress_metro); each has a JOBS list and calls export() here. The pieces
are placed by the level's table, Source/FullThrottle/FTDressL<n>.inl (districts, sets round landmarks, the horizon:
see FTDress.h), never by hand in a map. Run in the editor:

    import ft_dress; ft_dress.run()            # every level's kit
    import ft_dress; ft_dress.run(['chem'])    # one level's

run() exports a kit's JOBS and then calls the kit's after_export(only), if it has one: that is where a kit rebuilds
its moving props (ft_life.Kit pieces are not in JOBS) and puts its motion back on the still pieces.

How a piece is drawn (so the tables can place any of them the same way):
  - with ft_art.Model, like every prop of the game: m.box / m.cyl / m.sphere / m.side / m.plan, a palette colour per
    primitive (hex), glow for anything lit. Chunky, readable, exaggerated shapes: it is seen for a second at 120 mph.
  - units are cm; the base stands on z = 0 (give anything that may stand on falling ground a footing below 0);
    the middle of the footprint is the origin; the FRONT faces -Y (the placement turns -Y towards the road);
    long pieces run along X (X is laid along the road).
  - keep a piece under about 1500 triangles; a district shows hundreds of instances.
  - HORIZON pieces (placed by Far rules, 300 m to 2 km out, past the land's edge) are big: 60 to 250 m. A Far rule
    stands a piece's z = 0 some 25 m below the road's own level (its "drop"), so build it from z = 0 up and make it
    tall enough to stand over the horizon from there.
  - CUT-BANK panels (placed by Bank rules: SM_Cut... in the Chemical Works, Nevada, Volcano and Ice Mountain kits)
    are the one exception to "stands on z = 0, front faces -Y": they are drawn flat, as if the cutting inside a banked
    bend were the floor, and the track turns, pitches and stretches each one onto its own cutting. ft_dress_bank.py
    has the drawing rules and the few helpers they share.
Every level but Bayshore gets the visual-finish pass's materials on export (palette instances of M_VisualFinish), but
not the detail that pass adds to the levels' original props (see export); Bayshore's keep the plain palette (and its
glass and neon sections when a piece uses ft_buildings panes).
"""
import importlib
import unreal
import ft_art
import ft_export

DIRS = {'bayshore': '/Game/Maps/Bayshore/Dress', 'chem': '/Game/Maps/ChemicalWorks/Dress', 'nevada': '/Game/Maps/Nevada/Dress',
        'volcano': '/Game/Maps/Volcano/Dress', 'park': '/Game/Maps/ThemePark/Dress', 'ice': '/Game/Maps/Glacier/Dress',
        'abyss': '/Game/Maps/Abyss/Dress', 'metro': '/Game/Maps/Metro/Dress'}
MODULES = {'bayshore': 'ft_dress_bay', 'chem': 'ft_dress_chem', 'nevada': 'ft_dress_nevada', 'volcano': 'ft_dress_volcano',
           'park': 'ft_dress_park', 'ice': 'ft_dress_ice', 'abyss': 'ft_dress_abyss', 'metro': 'ft_dress_metro'}


def log(*a):
    unreal.log('FTDRESS ' + ' '.join(str(x) for x in a))


def export(level, jobs, only=None):
    """jobs: [(name, build)] where build(m) draws the piece into an ft_art.Model. Saves /Game/Maps/<Level>/Dress/<name>.
    only: a list of names to rebuild (default: all)."""
    folder = DIRS[level]
    done = []
    failed = []
    for name, fn in jobs:
        if only and name not in only:
            continue
        try:    # a piece that fails is logged and left out; the rest of the kit is still built
            mdl = ft_export.model(fn)
            extra = None
            if level == 'bayshore':
                import ft_buildings
                extra = ft_buildings.extra_sections(mdl)
            # the finish pass's added detail (ft_visual_art.refine) is made for the levels' original props: on any
            # other piece it hangs two louvred service panels at fixed fractions of the bounding box, which on a reef,
            # a mesa or a ride is nonsense. Dressing pieces keep the pass's materials and skip that detail.
            import ft_visual_art
            refine = ft_visual_art.refine
            ft_visual_art.refine = lambda meshes, path: None
            try:
                sm = ft_export.save_mesh(mdl.meshes, folder + '/' + name, 'prop', extra=extra, uv_sets=2 if extra else 1,
                                         no_shadow=('Glass', 'Neon'))
            finally:
                ft_visual_art.refine = refine
        except Exception as e:  # noqa
            import traceback
            unreal.log_error('FTDRESS %s/%s FAILED: %r %s' % (level, name, e, traceback.format_exc()[-900:]))
            print('JAM ERR kit piece', level, name, repr(e)[:300], flush=True)
            failed.append(name)
            continue
        if sm is None:
            unreal.log_warning('FTDRESS %s/%s drew nothing' % (level, name))
            continue
        b = sm.get_bounds()
        tris = sm.get_num_triangles(0)
        e = b.box_extent
        log(level, name, 'triangles', tris, 'size m', round(e.x / 50.0, 1), round(e.y / 50.0, 1), round(e.z / 50.0, 1),
            'base z', round(b.origin.z - e.z))
        done.append(name)
    print('JAM kit', level, 'pieces built', len(done), 'failed', len(failed), ' '.join(failed), flush=True)
    return done


def run(levels=None, only=None):
    for key in (levels or list(MODULES)):
        try:
            mod = importlib.import_module(MODULES[key])
        except ImportError:
            log(key, 'has no kit module yet (' + MODULES[key] + ')')
            continue
        importlib.reload(mod)
        export(key, mod.JOBS, only)
        # what moves: an export leaves a still piece with the plain palette, and a kit's moving props are not in JOBS.
        # A kit that has anything moving says how to redo it in after_export(only).
        fin = getattr(mod, 'after_export', None)
        if fin is not None:
            try:
                fin(only)
            except Exception as e:  # noqa
                import traceback
                unreal.log_error('FTDRESS %s after_export FAILED: %r %s' % (key, e, traceback.format_exc()[-900:]))
                print('JAM ERR kit after_export', key, repr(e)[:300], flush=True)
    log('done')
