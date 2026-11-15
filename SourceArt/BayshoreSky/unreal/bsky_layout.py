"""The cumulus card layout, in one place (41_bp_tweaks.py writes it to the Blueprint template, 82_place_cumulus.py to the
placed actors, whose ISM instances do not follow the template). Clusters sit 18-32 km from the sky actor, outside the
~13 km road, so they always stay low over the horizon (about 3-25 deg) and slide past as the car drives. More clusters
toward the sun side and the bay, as in the Brickell reference ("a few more clouds", owner 14 Nov).
Per instance custom data: 0 atlas cell, 1 drift phase, 2 show order, 3 mode (0 = upright billboard), 4 drift span (cm),
5 mirror (fixed: 1 when the sun is on the card's left seen from the sky actor, so the baked +u light faces the sun)."""
import math
import random

from bsky_params import SUN_AZ

# (azimuth deg, distance km, cards); azimuth in Unreal convention (from +X toward +Y)
CLUSTERS = [(-8.0, 26.0, 3), (14.0, 22.0, 4), (38.0, 24.0, 3), (58.0, 19.0, 3), (82.0, 27.0, 2), (112.0, 21.0, 3),
            (140.0, 30.0, 2), (165.0, 20.0, 3), (198.0, 25.0, 3), (228.0, 19.0, 2), (262.0, 28.0, 3), (292.0, 22.0, 2),
            (318.0, 24.0, 3), (342.0, 30.0, 2)]


def show_orders():
    """The show order (custom data 2) of each card, cluster by cluster: dealt round-robin (the first card of every
    cluster, then every second card, ...), so a lower CloudCardsShown (Low effects quality halves it) keeps every cluster
    on screen. Review: in cluster order, Low emptied azimuths 193-346 deg; a shuffle still left a 65 deg gap, round-robin
    leaves the full ring's own widest gap (33 deg)."""
    keys = sorted((k, c) for c, (_, _, n) in enumerate(CLUSTERS) for k in range(n))
    rank = {key: i for i, key in enumerate(keys)}
    return [[float(rank[(k, c)]) for k in range(n)] for c, (_, _, n) in enumerate(CLUSTERS)]


def cards():
    """[(x, y, z, scale, [cell, phase, order, mode, span, mirror])] in cm, relative to the sky actor."""
    rnd = random.Random(2026)
    orders = show_orders()
    out = []
    for c, (az, dist_km, n) in enumerate(CLUSTERS):
        for k in range(n):
            a = az + rnd.uniform(-6.0, 6.0)
            d = (dist_km + rnd.uniform(-3.0, 3.0)) * 100000.0
            alt = rnd.uniform(150000.0, 330000.0)        # the cloud base; the card material lifts the card above it
            s = rnd.uniform(2000.0, 4800.0)
            cell = float(rnd.randrange(8))
            mirror = 1.0 if math.sin(math.radians(SUN_AZ - a)) < 0.0 else 0.0
            out.append((math.cos(math.radians(a)) * d, math.sin(math.radians(a)) * d, alt, s,
                        [cell, rnd.random(), orders[c][k], 0.0, 600000.0, mirror]))
    return out


def apply(t, unreal):
    """Write the layout into an InstancedStaticMeshComponent (template or placed)."""
    t.set_editor_property("hidden_in_game", False)
    t.clear_instances()
    t.set_editor_property("num_custom_data_floats", 6)
    for x, y, z, s, data in cards():
        idx = t.add_instance(unreal.Transform(location=unreal.Vector(x, y, z), rotation=unreal.Rotator(0, 0, 0),
                                              scale=unreal.Vector(s, s, s)), False)
        for j, v in enumerate(data):
            t.set_custom_data_value(idx, j, float(v), False)
    return t.get_instance_count()
