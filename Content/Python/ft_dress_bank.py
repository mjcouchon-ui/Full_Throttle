"""FULL THROTTLE (Unreal) the dressing: what the kits' cut-bank panels share (3.4).

A banked bend sinks the road's inner edge up to 11.5 m, and the track cuts the land down to it: one straight slope from
under the low barrier up to the land's inner edge. The chase camera, rolled with the road, looks straight at that
cutting. A Bank rule in a level's table (FTDress.h, FTTrackDress.cpp) has the track lay a panel on it, panel after
panel along the bend, turned to the slope, pitched to the grade and stretched to fit: along the road to its share of
the bend, up the slope to the cutting's length. So a panel is drawn FLAT, as if the bank were the floor:

    x   along the road (the piece is turned round on the left-hand side, like every dressed piece)
    y   up the slope: the road and its low barrier are on the -y side, the land's edge on the +y side
    z   out of the bank; z = 0 is the bank's own surface

  - a body from BACK to FACE over the whole panel (the track fits a flat panel to a round bank to within 12 cm, so
    the body goes 40 cm in and its face stands 14 cm out), and on it nothing more than 30 cm high;
  - it is stretched: a tall panel (TALL as drawn) lies on runs of 5.5 to 12 m, a low one (LOW) on 2.4 to 5.5 m, and
    11 m of length becomes 10 to 11.3 m. Draw courses and blocks along x and y; a circle would come out an oval;
  - what runs along the road should meet the next panel: band() keeps its edges at their given heights at both ends;
  - nothing lit and no arrows (it lies 9 to 16 m from the centre line), nothing thinner than a hand at that distance
    (the outline pass inks thin parts into scribble), and few triangles: a level has several hundred panels.

Nothing here needs Unreal: a kit's panels can be drawn outside it (dress_tools/kit_dry.py)."""
import random

L = 1100.0       # a panel as drawn: 11 m along the road
TALL = 900.0     # the run up the slope of a tall panel as drawn
LOW = 400.0      # and of a low one
FACE = 14.0      # the body's face, over the bank's surface
BACK = -40.0     # the body's back, in the bank


def ccw(pts):
    """The polygon wound counter-clockwise (m.plan needs it)."""
    a = 0.0
    for i in range(len(pts)):
        (x0, y0), (x1, y1) = pts[i], pts[(i + 1) % len(pts)]
        a += x0 * y1 - x1 * y0
    return list(pts) if a > 0 else list(reversed(pts))


def body(m, col, run, face=FACE):
    """The panel's body: the whole panel, from 40 cm in the bank to its face."""
    m.box(col, (0.0, 0.0, (face + BACK) / 2.0), (L, run, face - BACK))


def block(m, col, x, y, w, h, z, z0=FACE):
    """A block on the face: centre (x, y), w along the road, h up the slope, standing z over the face."""
    m.box(col, (x, y, z0 + z / 2.0), (w, h, z))


def shape(m, col, pts, z, z0=FACE):
    """Any flat shape on the face, [(x, y)], standing z over it."""
    m.plan(col, ccw(pts), z0, z0 + z)


def band(m, col, y0, y1, z, wob=0.0, n=5, seed=0, x0=-L / 2.0, x1=L / 2.0, z0=FACE):
    """A course along the road between y0 and y1, standing z over the face. wob: how far its two long edges wander,
    one number or (lower edge, upper edge); never at the two ends: the same course on the next panel meets it there."""
    r = random.Random(seed)
    wl, wh = wob if isinstance(wob, tuple) else (wob, wob)
    xs = [x0 + (x1 - x0) * i / float(n) for i in range(n + 1)]
    lo = [(x, y0 + (r.uniform(-wl, wl) if 0 < i < n else 0.0)) for i, x in enumerate(xs)]
    hi = [(x, y1 + (r.uniform(-wh, wh) if 0 < i < n else 0.0)) for i, x in enumerate(xs)]
    m.plan(col, ccw(lo + list(reversed(hi))), z0, z0 + z)
