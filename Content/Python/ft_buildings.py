"""FULL THROTTLE (Unreal) Level 1 buildings: real glass, living lights and neon behind the cel look.

The cel pass (ft_cel.M_PP_Cel) does not band a pixel whose GBuffer base colour is black. The materials here have a
black base colour and no specular, so only what they emit reaches the screen, in scene-referred HDR. That is how
photoreal glass, lit rooms and neon sit behind cel-shaded walls without changing the shared post-process. They are
lit materials all the same (see _emissive_only): the pass's ink stage reads their normal like any wall's.

  M_BayGlass   windows. Interior mapping (a room behind every pane, no textures): each room has its own wall colour,
               lamp, blinds, curtains, now and then a floor lamp, a TV or somebody standing there. Rooms switch on as
               the evening goes on and change their minds now and then. Over it, the glass reflects the BayshoreSky
               gradient (the sky's own MPC, so it follows any retune) and flashes when a pane catches the low sun.
  M_BayNeon    neon tubes and lit signs: mains shimmer, and here and there a stretch that buzzes and dips.

Meshes carry two UV sets for the glass (see Panes): UV0 = (pane index + across, floor index + up), UV1 = the pane's
size in metres, so one material fits every facade.

Run in the editor (after ft_mats.build_all and with the BayshoreSky plugin in place):
    import ft_buildings; ft_buildings.build_materials()
The meshes are rebuilt by ft_export.run(['bayshore']). Tuning, costs and the reasons: BUILDINGS.md.
"""
import math
import unreal
import ft_mats

AL = unreal.EditorAssetLibrary
MEL = unreal.MaterialEditingLibrary
AT = unreal.AssetToolsHelpers.get_asset_tools()
GS_EDIT = unreal.GeometryScript_MeshEdits
GS_UV = unreal.GeometryScript_UVs
GS_QUERY = unreal.GeometryScript_MeshQueries

MAT = '/Game/Maps/Bayshore/Materials'
MPC = '/BayshoreSky/Materials/MPC_BayshoreSky'


def log(*a):
    unreal.log('FTBLD ' + ' '.join(str(x) for x in a))


def _out(r):
    return r[-1] if isinstance(r, tuple) else r


# ------------------------------------------------------------------------------------------ glass meshes
class Panes(object):
    """Window quads for M_BayGlass, collected into one DynamicMesh with two UV sets.

    A band is a row of panes on one wall: band(origin, right, up, width, height, panes, floor). Seen from outside,
    `right` is the viewer's right and `up` is up (the shader rebuilds the same frame from the face normal and world
    up, so walls must stand upright). UV0.x runs 0..panes along the band from `pane0`, UV0.y runs floor..floor+1 up it.
    UV1 = (pane width, band height) in metres. Their signs pick the glass:
      room=True    both > 0: a room behind every pane (interior mapping);
      room='flat'  width < 0: every pane lit or dark on its own, nothing behind it (the far skyline, portholes);
      room=False   height < 0: glass block, milky, the whole floor's ring lit as one.
    `base` is the model's extra transform (ft_art.Model.base), applied to every corner."""

    def __init__(self):
        self.mesh = unreal.DynamicMesh()
        GS_UV.set_num_uv_sets(self.mesh, 2)
        self.quads = 0
        self.base = None

    def _p(self, v):
        p = unreal.Vector(*v)
        return self.base.transform_location(p) if self.base is not None else p

    def quad(self, bl, br, tr, tl, uv_bl, uv_tr, dims):
        """Corners as seen from outside. uv_bl / uv_tr: UV0 at the bottom-left and top-right corners."""
        pts = [self._p(c) for c in (tl, tr, br, bl)]
        v = [_out(GS_EDIT.add_vertex_to_mesh(self.mesh, p)) for p in pts]
        (u0, v0), (u1, v1) = uv_bl, uv_tr
        uvs = [(u0, v1), (u1, v1), (u1, v0), (u0, v0)]            # TL, TR, BR, BL
        # outward = up x right (by the cross-product formula; the shader's right is cross(normal, up))
        up = (pts[0].x - pts[3].x, pts[0].y - pts[3].y, pts[0].z - pts[3].z)
        rt = (pts[2].x - pts[3].x, pts[2].y - pts[3].y, pts[2].z - pts[3].z)
        want = (up[1] * rt[2] - up[2] * rt[1], up[2] * rt[0] - up[0] * rt[2], up[0] * rt[1] - up[1] * rt[0])
        for a, b, c in ((0, 2, 1), (0, 3, 2)):                    # the winding ft_billboards.face_mesh proved faces out
            tid = _out(GS_EDIT.add_triangle_to_mesh(self.mesh, unreal.IntVector(v[a], v[b], v[c])))
            if tid < 0:
                raise RuntimeError('Panes: a triangle was refused')
            for uvset, tri in ((0, [uvs[a], uvs[b], uvs[c]]), (1, [dims, dims, dims])):
                t = unreal.GeometryScriptUVTriangle()
                t.set_editor_property('uv0', unreal.Vector2D(*tri[0]))
                t.set_editor_property('uv1', unreal.Vector2D(*tri[1]))
                t.set_editor_property('uv2', unreal.Vector2D(*tri[2]))
                GS_UV.set_mesh_triangle_u_vs(self.mesh, uvset, tid, t)
            r = GS_QUERY.get_triangle_face_normal(self.mesh, tid)
            n = r[0] if isinstance(r, tuple) else r
            if n.x * want[0] + n.y * want[1] + n.z * want[2] <= 0.0:
                raise RuntimeError('Panes: a pane faces into its wall (normal %.2f %.2f %.2f)' % (n.x, n.y, n.z))
        self.quads += 1

    def band(self, origin, right, up, width, height, panes, floor, pane0=0, room=True):
        """A row of `panes` panes starting at `origin` (its bottom-left corner seen from outside)."""
        o = origin
        r = [c * width for c in right]
        u = [c * height for c in up]
        bl = o
        br = (o[0] + r[0], o[1] + r[1], o[2] + r[2])
        tr = (br[0] + u[0], br[1] + u[1], br[2] + u[2])
        tl = (o[0] + u[0], o[1] + u[1], o[2] + u[2])
        e = 0.002                                                  # keeps frac() off the cell edges at the quad's rim
        pane_w, h = width / panes / 100.0, height / 100.0
        dims = (pane_w, h) if room is True else ((-pane_w, h) if room == 'flat' else (pane_w, -h))
        self.quad(bl, br, tr, tl, (pane0 + e, floor + e), (pane0 + panes - e, floor + 1 - e), dims)

    def disc(self, centre, right, up, radius, floor, pane0=0, steps=16):
        """A round pane (a porthole): a fan of triangles, lit as one, with no room behind it."""
        c = self._p(centre)
        vc = _out(GS_EDIT.add_vertex_to_mesh(self.mesh, c))
        ring, uvs = [], []
        for k in range(steps):
            a = 2 * math.pi * k / steps
            ca, sa = math.cos(a), math.sin(a)
            p = (centre[0] + (right[0] * ca + up[0] * sa) * radius, centre[1] + (right[1] * ca + up[1] * sa) * radius,
                 centre[2] + (right[2] * ca + up[2] * sa) * radius)
            ring.append(_out(GS_EDIT.add_vertex_to_mesh(self.mesh, self._p(p))))
            uvs.append((pane0 + 0.5 + 0.49 * ca, floor + 0.5 + 0.49 * sa))
        dims = (-2.0 * radius / 100.0, 2.0 * radius / 100.0)        # 'flat': lit on its own, nothing behind it
        for k in range(steps):
            j = (k + 1) % steps
            tid = _out(GS_EDIT.add_triangle_to_mesh(self.mesh, unreal.IntVector(vc, ring[k], ring[j])))
            if tid < 0:
                raise RuntimeError('Panes: a porthole triangle was refused')
            for uvset, tri in ((0, [(pane0 + 0.5, floor + 0.5), uvs[k], uvs[j]]), (1, [dims, dims, dims])):
                t = unreal.GeometryScriptUVTriangle()
                t.set_editor_property('uv0', unreal.Vector2D(*tri[0]))
                t.set_editor_property('uv1', unreal.Vector2D(*tri[1]))
                t.set_editor_property('uv2', unreal.Vector2D(*tri[2]))
                GS_UV.set_mesh_triangle_u_vs(self.mesh, uvset, tid, t)
        self.quads += steps // 2


def panes(m):
    """The model's glass (made on first use). ft_city draws into it; ft_export.bayshore() exports it."""
    if getattr(m, 'panes', None) is None:
        m.panes = Panes()
    m.panes.base = m.base
    return m.panes


NEON_PART = 'neon'      # ft_art.Model.part while a builder draws neon: those primitives become M_BayNeon sections


def extra_sections(m):
    """[(DynamicMesh, material, slot)] for ft_export.save_mesh: the model's glass, then its neon by colour and glow."""
    out = []
    p = getattr(m, 'panes', None)
    tubes = m.parts.get(NEON_PART, {})
    if (p is not None and p.quads) or tubes:
        # checked before any instance is made: an instance of a missing parent would save and draw the default material
        for path in (MAT + '/MI_BayGlass', MAT + '/M_BayNeon'):
            if AL.load_asset(path) is None:
                raise RuntimeError('ft_buildings: %s is missing (run ft_buildings.build_materials() first)' % path)
    if p is not None and p.quads:
        out.append((p.mesh, AL.load_asset(MAT + '/MI_BayGlass'), 'Glass'))
    for (col, glow), mesh in tubes.items():
        out.append((mesh, neon_mi(col, glow), 'Neon_%s_g%d' % (col.lstrip('#').lower(), int(glow * 100))))
    return out


# ------------------------------------------------------------------------------------------ the glass shader
# A Custom node's body is one function, so the helpers live in a struct declared inside it (Fn.Hash, Fn.Wall, Fn.Sky).
# Fn.Sky is the BayshoreSky dome's gradient (SourceArt/BayshoreSky/unreal/30_materials.py, GRAD), so a reflection shows
# the sky the player sees: keep the two in step. Its inputs are MPC_BayshoreSky parameters.
GLASS_HLSL = r"""
struct BayFn
{
    float Hash(float2 p)
    {
        float3 p3 = frac(float3(p.xyx) * float3(0.1031, 0.1030, 0.0973));
        p3 += dot(p3, p3.yzx + 33.33);
        return frac((p3.x + p3.y) * p3.z);
    }
    float3 Wall(float h)
    {
        // the game's own pastels, as painted walls
        float3 c = float3(1.0, 0.89, 0.67);                             // cream
        c = h > 0.30 ? float3(1.0, 0.56, 0.34) : c;                     // peach
        c = h > 0.50 ? float3(0.21, 0.76, 0.55) : c;                    // mint
        c = h > 0.66 ? float3(0.48, 0.37, 1.0) : c;                     // lavender
        c = h > 0.80 ? float3(0.27, 0.76, 1.0) : c;                     // sky
        c = h > 0.92 ? float3(1.0, 0.19, 0.25) : c;                     // coral
        return lerp(c, float3(0.8, 0.8, 0.8), 0.35);
    }
    float3 Sky(float3 D, float3 Zen, float3 Mid, float3 Hor, float3 Haz, float3 SunD, float3 HaloC, float3 SunC,
               float MidPt, float Haze, float HaloSize)
    {
        float3 d = normalize(D);
        float el = asin(clamp(d.z, -1.0, 1.0));
        float e = saturate(el / 1.5707963);
        float m = clamp(MidPt, 0.02, 0.6);
        float3 c = (e < m) ? lerp(Hor, Mid, smoothstep(0.0, 1.0, e / m)) : lerp(Mid, Zen, smoothstep(0.0, 1.0, saturate((e - m) / (2.0 * m))));
        float2 dh = normalize(d.xy + float2(1e-5, 0.0));
        float2 sh = normalize(SunD.xy + float2(1e-5, 0.0));
        float toward = saturate(dot(dh, sh) * 0.5 + 0.5);
        float lowband = 1.0 - smoothstep(0.0, 0.2, e);
        c = lerp(c, Haz, pow(toward, 3.0) * lowband * 0.45);
        c = lerp(c, Haz, saturate(Haze) * exp(-max(el, 0.0) / 0.07));
        float sang = acos(clamp(dot(d, normalize(SunD)), -1.0, 1.0));
        c += HaloC * 0.25 * exp(-sang / max(radians(HaloSize), 1e-3));
        c = lerp(c, HaloC * 1.15, saturate(pow(toward, 5.0) * exp(-max(el, 0.0) / 0.06)));
        c = lerp(c, SunC * 1.3, saturate(pow(toward, 16.0) * exp(-max(el, 0.0) / 0.025)));
        // below the horizon a window mirrors the street: the city's haze, darkening toward the ground at its foot.
        // Blended over a few degrees: a hard switch drew a razor line through every pane that mirrors the horizon
        float3 g = lerp(Haz * 0.8, Hor * 0.3, saturate(-el / 0.5));
        return lerp(g, c, smoothstep(-0.07, 0.02, el));
    }
};
BayFn Fn;
float3 Vd = -normalize(CamV);                       // camera -> pixel
float3 Nn = normalize(N);
float3 Bw = float3(0.0, 0.0, 1.0);
float3 Tw = normalize(cross(Nn, Bw) + float3(1e-6, 0.0, 0.0));    // the viewer's right, as baked into UV0.x
float blockp = step(Dim.y, -0.001);                 // glass block: milky, lit floor by floor
float flatp = step(Dim.x, -0.001) * (1.0 - blockp); // a pane lit on its own with nothing behind it (skyline, portholes)
float room = step(0.001, Dim.x) * (1.0 - blockp);   // a room behind the pane
float paned = saturate(room + flatp);
float2 cell = floor(UV);
cell.x = paned > 0.5 ? cell.x : 7.0;                // a ring of glass block lights up as one, floor by floor
float2 w = frac(UV);                                // across the pane, up the pane
float seed = Rnd * 113.0;
float h0 = Fn.Hash(cell + seed);
float h1 = Fn.Hash(cell * 1.31 + seed + 17.3);
float h2 = Fn.Hash(cell * 0.73 + seed + 41.9);
float h3 = Fn.Hash(cell * 1.93 + seed + 71.1);
float h4 = Fn.Hash(cell * 2.17 + seed + 101.3);     // curtains
float h5 = Fn.Hash(cell * 0.57 + seed + 133.7);     // a floor lamp, or somebody in the room
float W = max(abs(Dim.x), 0.01);                    // the pane's width and height, metres
float band = max(abs(Dim.y), 0.01);
float fwu = max(abs(Dx.x), abs(Dy.x));              // panes per pixel across, and up
float fwv = max(abs(Dx.y), abs(Dy.y));
float fw = max(fwu, fwv);
float pxm = max(fwu * W, fwv * band);               // metres of wall per pixel
float farK = saturate((fw - 0.2) / 0.5) * paned;    // panes down to a few pixels: fade to the facade's average
float lobby = step(cell.y, -0.5);                   // floors below 0 are the lobby level: always lit

// ---- is the light on? More rooms as the evening goes on. Each room thinks again every four to ten minutes and
// fades over half a second: across a whole skyline that is a light changing here and there, not a glitter of pops
float period = lerp(240.0, 600.0, h1);
float ph = T / period + h0 * 9.0;
float epoch = floor(ph);
float litFrac = lerp(LitStart, LitEnd, saturate(T / max(Dusk, 1.0)));
float rollNow = Fn.Hash(float2(h0 * 91.7 + epoch * 13.37, h2 * 57.3));
float rollWas = Fn.Hash(float2(h0 * 91.7 + (epoch - 1.0) * 13.37, h2 * 57.3));
float onNow = saturate((litFrac - rollNow) / 0.002);
float onWas = saturate((litFrac - rollWas) / 0.002);
float on = max(lerp(onWas, onNow, saturate(frac(ph) * period / 0.5)), lobby);
float tv = (1.0 - on) * step(0.90, h3);             // a dark room with the television on
float warm = Fn.Hash(cell + seed + 5.5);
// saturated lamps: against sunlit pastel walls a lit pane reads by its colour, not by being brighter
float3 lamp = warm < 0.62 ? float3(1.0, 0.5, 0.15) : (warm < 0.88 ? float3(1.0, 0.72, 0.38) : float3(0.5, 0.8, 1.0));
float flick = 0.55 + 0.45 * Fn.Hash(float2(floor(T * 7.0), h0 * 31.0));
float3 light = lamp * on * Interior + float3(0.25, 0.42, 1.0) * tv * flick * Interior * 0.45;
float3 amb = AmbC.rgb * Ambient;                    // what the sky throws into a dark room

float3 inside = float3(0.0, 0.0, 0.0);
[branch]
if (room > 0.5 && LowQ < 0.5)
{
    // ---- the room behind the pane: a box W wide, rh high and depth deep, the pane partway up its front
    float depth = RoomDepth * lerp(0.7, 1.35, h2);
    float rh = max(RoomH, band + 0.2);              // a pane taller than the room's height (the lobby) raises its ceiling
    float y0 = (rh - band) * 0.42;
    float3 o = float3(w.x * W, y0 + w.y * band, 0.0);
    float3 d = float3(dot(Vd, Tw), dot(Vd, Bw), dot(Vd, -Nn));
    d.z = max(d.z, 1e-3);
    float dx = abs(d.x) < 1e-5 ? 1e-5 : d.x;
    float dy = abs(d.y) < 1e-5 ? 1e-5 : d.y;
    float tx = ((dx > 0.0 ? W : 0.0) - o.x) / dx;
    float ty = ((dy > 0.0 ? rh : 0.0) - o.y) / dy;
    float tz = depth / d.z;
    float t = min(tz, min(tx, ty));
    float3 p = o + d * t;
    float3 L = float3(W * 0.5, rh - 0.35, depth * 0.55);           // the ceiling lamp
    float3 wallC = Fn.Wall(h3);
    float3 alb;
    if (t >= tz - 1e-4)
    {
        // back wall: a door or a picture
        alb = wallC;
        float2 q = float2(p.x / W, p.y / rh);
        float door = step(abs(q.x - lerp(0.25, 0.75, h1)), 0.5 / W) * step(q.y, 2.05 / rh);
        float pic = step(abs(q.x - lerp(0.3, 0.7, h2)), 0.35 / W) * step(abs(q.y - 0.55), 0.09);
        alb = lerp(alb, wallC * 0.45, door * step(h0, 0.45));
        alb = lerp(alb, Fn.Wall(frac(h3 + 0.37)) * 1.1, pic * step(0.45, h0));
    }
    else if (tx <= ty)
    {
        alb = wallC * 0.82;
    }
    else
    {
        alb = dy > 0.0 ? float3(0.9, 0.88, 0.84) : lerp(float3(0.42, 0.27, 0.17), float3(0.16, 0.34, 0.36), step(0.5, h1));
        // the lamp's pool on the ceiling
        float pool = saturate(1.0 - length(p.xz - L.xz) / 0.55);
        alb += step(0.0, dy) * pool * pool * 2.0;
    }
    float3 dl = p - L;
    float fall = 1.0 / (1.0 + dot(dl, dl) * 0.22);
    fall = lerp(fall, floor(fall * 4.0 + 0.5) / 4.0, Bands);       // a little of the cel hand inside too
    inside = alb * (light * (0.25 + 1.6 * fall) + amb);

    // ---- halfway in: a floor lamp, or somebody standing there
    float tm = depth * 0.5 / d.z;
    if (tm < t)
    {
        float3 pm = o + d * tm;
        float fx = lerp(0.2, 0.8, h2) * W;
        float lampOn = step(h5, 0.38);
        float shade = step(abs(pm.x - fx), 0.17 - (pm.y - 1.25) * 0.18) * step(1.25, pm.y) * step(pm.y, 1.62);
        float pole = step(abs(pm.x - fx), 0.018) * step(pm.y, 1.25) * saturate(2.0 - pxm / 0.03);   // gone once thinner than a pixel
        inside = lerp(inside, float3(0.02, 0.015, 0.03) + amb, pole * lampOn);
        inside = lerp(inside, lamp * (on * Interior * 2.2 + 0.03), shade * lampOn);
        float px = lerp(0.25, 0.75, h0) * W;
        float body = step(abs(pm.x - px), 0.2 - saturate(pm.y - 1.1) * 0.12) * step(pm.y, 1.52);
        float head = step(length(float2(pm.x - px, pm.y - 1.66)), 0.115);
        float who = step(0.93, h5) * on;
        inside = lerp(inside, wallC * light * 0.05, saturate(body + head) * who);
    }

    // ---- in the window: blinds, curtains. The hotels stand 100 m and more from the road, 6-7 cm of wall to a pixel:
    // slats and folds are drawn as stripes only while a stripe is wider than two pixels, and as their mean beyond
    float drop = h2 > 0.45 ? lerp(0.12, 0.92, Fn.Hash(cell + seed + 9.1)) : 0.0;
    float slats = lerp(step(0.3, frac(w.y * band * 9.0)), 0.7, saturate((pxm * 9.0 - 0.25) / 0.25));
    float slat = step(1.0 - drop, w.y) * slats;
    float3 blindC = float3(0.86, 0.82, 0.74) * (light * 0.55 + amb * 2.0);
    inside = lerp(inside, blindC, slat * Dress);
    float cw = h4 > 0.6 ? lerp(0.06, 0.24, h3) : 0.0;
    float curt = step(min(w.x, 1.0 - w.x), cw);
    float fold = 0.72 + 0.28 * sin(w.x * W * 42.0) * saturate(2.0 - pxm * 6.7 / 0.25);
    float3 curtC = Fn.Wall(frac(h0 + 0.61)) * fold * (light * 0.42 + amb * 2.0);
    inside = lerp(inside, curtC, curt * Dress);
}
else
{
    // glass block, portholes, the far skyline's panes, and the Low path: a lit pane, no room
    float2 g = abs(frac(w * float2(W, band) * 5.0) - 0.5);          // 20 cm blocks
    float joint = lerp(1.0 - smoothstep(0.36, 0.5, max(g.x, g.y)), 0.75, saturate((pxm * 5.0 - 0.25) / 0.25));
    float block = lerp(1.0, 0.7 + 0.3 * joint, blockp);
    inside = (light * 0.75 + amb) * float3(0.92, 0.9, 0.84) * block;
    // glass block is milky: unlit it still shows the day on it (sun side warm, shade side the sky's blue)
    float sunny = saturate(dot(Nn, normalize(SunD.xyz)));
    float3 day = float3(0.62, 0.78, 0.8) * (AmbC.rgb * 1.1 + SunC.rgb * BlockSun * (sunny > 0.25 ? 1.0 : (sunny > 0.02 ? 0.55 : 0.0)));
    inside += day * block * blockp;
}
// far away every pane is a pixel or less: the facade's average, so the lit pattern does not crawl. (Glass block is
// lit a whole floor at a time and needs none; farK is 0 for it.)
float3 avgC = float3(1.0, 0.6, 0.25) * max(litFrac, lobby) * Interior * 0.55 + amb * 0.5;
inside = lerp(inside, lerp(inside, avgC, 0.75), farK);

// ---- the frame, in the cel pass's ink
float fr = min(min(w.x, 1.0 - w.x) * W, min(w.y, 1.0 - w.y) * band);
float frame = (1.0 - smoothstep(Frame, Frame + 0.01 + fw * 0.6, fr)) * (1.0 - farK) * paned;

// ---- the glass: the sky's reflection, and the low sun where a pane catches it. No two panes sit quite true
float3 Nt = normalize(Nn + (Tw * (h1 - 0.5) + Bw * (h3 - 0.5)) * Warp * (1.0 - farK));
float3 R = reflect(Vd, Nt);
float ndv = saturate(dot(-Vd, Nt));
float F = F0 + (1.0 - F0) * pow(1.0 - ndv, 5.0);
float3 sky = Fn.Sky(R, Zen.rgb, Mid.rgb, Hor.rgb, Haz.rgb, SunD.xyz, HaloC.rgb, SunC.rgb, MidPt, Haze, HaloSize) * Bright;
float sdot = saturate(dot(R, normalize(SunD.xyz)));
float3 glint = SunC.rgb * (pow(sdot, GlintPow) * Glint + pow(sdot, 24.0) * Glint * 0.02);
float3 c = inside * (1.0 - F) * Tint.rgb + sky * F * Reflect + glint;
c = lerp(c, InkC.rgb, frame);
return c;
"""

GLASS_PARAMS = [
    # (name, default, what it does)
    # in game (14 Nov): pale lamps at 2.6 and at 3.8 both read as cream, like the sunlit pastel walls (the tonemapper
    # whitens anything bright). The lamps were made saturated instead and the level brought back down
    ('Interior', 2.6, 'brightness of a lit room (scene-linear; the lit wall band of the cel pass is about 1.4-2.4)'),
    ('LitStart', 0.34, 'share of rooms lit when the level starts'),
    ('LitEnd', 0.72, 'share lit once the evening has come on'),
    ('Dusk', 240.0, 'seconds from LitStart to LitEnd'),
    ('RoomDepth', 4.2, 'metres, varied 0.7-1.35x per room'),
    ('RoomH', 3.0, 'metres, floor to ceiling'),
    ('Bands', 0.35, '0 = smooth light falloff in the rooms, 1 = four hard steps'),
    ('Dress', 1.0, 'blinds and curtains (0 = bare windows)'),
    ('Ambient', 0.16, 'sky light in an unlit room, as a share of the sky colour'),
    ('BlockSun', 1.1, 'sunlight on glass block (it is milky, so it shows the day even unlit)'),
    ('Frame', 0.045, 'ink frame round each pane, metres'),
    ('F0', 0.3, 'mirror share head-on (plain glass is 0.04; coated hotel glass more)'),
    ('Reflect', 1.4, 'strength of the sky reflection'),
    ('Warp', 0.035, 'how far panes sit out of true (radians, roughly): each catches its own patch of sky'),
    ('Glint', 26.0, 'HDR peak of the sun in a pane'),
    ('GlintPow', 900.0, 'tightness of that flash'),
]
GLASS_VECTORS = [('Tint', (0.82, 0.93, 1.0, 1)), ('InkC', (0.026, 0.012, 0.079, 1))]
# (Custom-node input, MPC_BayshoreSky parameter)
SKY_INPUTS = [('Zen', 'ZenithColor'), ('Mid', 'MidColor'), ('Hor', 'HorizonColor'), ('Haz', 'HazeColor'), ('SunD', 'SunDirection'),
              ('HaloC', 'SunHaloColor'), ('SunC', 'SunColor'), ('MidPt', 'GradientMidPoint'), ('Haze', 'Haze'),
              ('HaloSize', 'SunHaloSize'), ('Bright', 'SkyBrightness'), ('LowQ', 'LowQuality'), ('AmbC', 'MidColor')]


def _mpc_param(m, mpc, name, y):
    e = MEL.create_material_expression(m, unreal.MaterialExpressionCollectionParameter, -1300, y)
    e.set_editor_property('collection', mpc)
    e.set_editor_property('parameter_name', name)
    return e


def _usage(m, name):
    # the cooked game falls back to the default material without these (README: material usage flags)
    for flag in ('used_with_instanced_static_meshes', 'used_with_nanite'):
        try:
            m.set_editor_property(flag, True)
        except Exception as e:
            unreal.log_warning('%s: could not set %s (%s)' % (name, flag, e))


def _emissive_only(m):
    """Lit, but black and without specular: nothing but the emissive reaches the screen, and the GBuffer still gets a
    real normal. An Unlit material would do the first half, but it writes no normal: the cel pass's ink stage then
    reads (-1, -1, -1) for its pixels and inks every edge between them and a wall facing +X, +Y or +Z (and not one
    facing the other way), so window rims and thin tubes changed with the road's heading (review, 14 Nov)."""
    base = ft_mats.expr(m, unreal.MaterialExpressionConstant3Vector, x=-500, y=-400)
    base.set_editor_property('constant', unreal.LinearColor(0.0, 0.0, 0.0, 1.0))
    MEL.connect_material_property(base, '', unreal.MaterialProperty.MP_BASE_COLOR)
    for prop, val, y in ((unreal.MaterialProperty.MP_SPECULAR, 0.0, -320), (unreal.MaterialProperty.MP_ROUGHNESS, 1.0, -240),
                         (unreal.MaterialProperty.MP_METALLIC, 0.0, -160)):
        c = ft_mats.expr(m, unreal.MaterialExpressionConstant, x=-500, y=y)
        c.set_editor_property('r', val)
        MEL.connect_material_property(c, '', prop)


def _compile(m):
    MEL.layout_material_expressions(m)
    errs = list(MEL.recompile_material(m) or [])
    if errs:
        raise RuntimeError('%s did not compile: %s' % (m.get_name(), errs))
    AL.save_loaded_asset(m)
    try:
        st = MEL.get_statistics(m)
        ps = st.get_editor_property('num_pixel_shader_instructions')
        log('compiled', m.get_name(), 'PS', ps, 'VS', st.get_editor_property('num_vertex_shader_instructions'),
            'samplers', st.get_editor_property('num_samplers'))
        if not ps:
            unreal.log_warning('%s reports no pixel-shader instructions: check that it compiled' % m.get_name())
    except Exception as e:
        log('compiled', m.get_name(), '(no statistics: %s)' % e)
    return m


def glass():
    """M_BayGlass and the hotels' instance MI_BayGlass."""
    mpc = AL.load_asset(MPC)
    if mpc is None:
        raise RuntimeError('M_BayGlass needs the BayshoreSky plugin (%s)' % MPC)
    m = ft_mats.new_material(MAT, 'M_BayGlass', blend=unreal.BlendMode.BLEND_OPAQUE, shading=unreal.MaterialShadingModel.MSM_DEFAULT_LIT)
    _emissive_only(m)
    X = -1300
    uv = ft_mats.expr(m, unreal.MaterialExpressionTextureCoordinate, x=X, y=-800)
    dim = ft_mats.expr(m, unreal.MaterialExpressionTextureCoordinate, x=X, y=-720)
    dim.set_editor_property('coordinate_index', 1)
    # derivatives as nodes, not ddx() in the Custom node (the translator can then give a Nanite mesh its analytic ones)
    dx = ft_mats.expr(m, unreal.MaterialExpressionDDX, x=X + 250, y=-880)
    dy = ft_mats.expr(m, unreal.MaterialExpressionDDY, x=X + 250, y=-960)
    MEL.connect_material_expressions(uv, '', dx, '')
    MEL.connect_material_expressions(uv, '', dy, '')
    n = ft_mats.expr(m, unreal.MaterialExpressionVertexNormalWS, x=X, y=-640)
    cam = ft_mats.expr(m, unreal.MaterialExpressionCameraVectorWS, x=X, y=-560)
    t = ft_mats.expr(m, unreal.MaterialExpressionTime, x=X, y=-480)
    rnd = ft_mats.expr(m, unreal.MaterialExpressionPerInstanceRandom, x=X, y=-400)
    ins = [('UV', uv, ''), ('Dim', dim, ''), ('Dx', dx, ''), ('Dy', dy, ''), ('N', n, ''), ('CamV', cam, ''), ('T', t, ''), ('Rnd', rnd, '')]
    y = -300
    for nm, v, _ in GLASS_PARAMS:
        ins.append((nm, ft_mats.sparam(m, nm, v, x=X, y=y), ''))
        y += 70
    for nm, v in GLASS_VECTORS:
        ins.append((nm, ft_mats.vparam(m, nm, v, x=X, y=y), ''))
        y += 70
    for nm, pn in SKY_INPUTS:
        ins.append((nm, _mpc_param(m, mpc, pn, y), ''))
        y += 70
    cu = ft_mats.custom(m, GLASS_HLSL, ins, x=-500, y=0, desc='BayGlass')
    MEL.connect_material_property(cu, '', unreal.MaterialProperty.MP_EMISSIVE_COLOR)
    _usage(m, 'M_BayGlass')
    _compile(m)
    return ft_mats.mi(m, MAT, 'MI_BayGlass')


# ------------------------------------------------------------------------------------------ neon
NEON_HLSL = r"""
// Mains shimmer on every tube. Here and there a stretch buzzes and dips: stretches are slabs of the world about a
// metre thick (a phase per building and per slab), not single tubes, so on a sign a few letters dip together.
float ph = Rnd * 61.0 + floor(dot(WP, float3(0.0053, 0.0047, 0.0061)));
float hum = 1.0 + Hum * sin(T * 119.0 + ph * 3.0) * sin(T * 7.3 + ph * 1.7);
float faulty = step(1.0 - Faulty, frac(sin(ph * 12.9898) * 43758.5453));
float e = floor(T * 9.0);
float r = frac(sin((e + ph) * 78.233) * 43758.5453);
float slow = 0.5 + 0.5 * sin(T * 0.6 + ph);              // it comes and goes in spells
float dropout = step(1.0 - 0.45 * slow, r) * faulty;
float lvl = lerp(1.0, 0.3, dropout);
return Col.rgb * Glow * Gain * hum * lvl;
"""


def neon():
    """M_BayNeon: neon for the buildings' tubes and signs, emissive only (the palette's Glow sections on M_Paint have
    a base colour, so the cel pass bands them unless they stay above its EmisCut)."""
    m = ft_mats.new_material(MAT, 'M_BayNeon', blend=unreal.BlendMode.BLEND_OPAQUE, shading=unreal.MaterialShadingModel.MSM_DEFAULT_LIT)
    _emissive_only(m)
    col = ft_mats.vparam(m, 'Color', (1.0, 0.08, 0.48, 1), y=-200)
    glow = ft_mats.sparam(m, 'Glow', 0.7, y=-100)
    gain = ft_mats.sparam(m, 'GlowGain', 14.0, y=0)
    hum = ft_mats.sparam(m, 'Hum', 0.07, y=100)
    faulty = ft_mats.sparam(m, 'Faulty', 0.14, y=200)
    t = ft_mats.expr(m, unreal.MaterialExpressionTime, y=300)
    rnd = ft_mats.expr(m, unreal.MaterialExpressionPerInstanceRandom, y=400)
    wp = ft_mats.expr(m, unreal.MaterialExpressionWorldPosition, y=500)
    cu = ft_mats.custom(m, NEON_HLSL, [('Col', col, ''), ('Glow', glow, ''), ('Gain', gain, ''), ('Hum', hum, ''), ('Faulty', faulty, ''),
                                       ('T', t, ''), ('Rnd', rnd, ''), ('WP', wp, '')], desc='BayNeon')
    MEL.connect_material_property(cu, '', unreal.MaterialProperty.MP_EMISSIVE_COLOR)
    _usage(m, 'M_BayNeon')
    return _compile(m)


_NEON = {}


def neon_mi(col, glow):
    """An M_BayNeon instance for a palette colour and glow, named like the palette's own."""
    import ft_art
    key = (col.lower(), round(glow, 2))
    if key in _NEON:
        return _NEON[key]
    r, g, b = ft_art.hex_lin(col)
    name = 'MI_BayNeon_%s_g%d' % (col.lstrip('#').lower(), int(glow * 100))
    inst = ft_mats.mi(AL.load_asset(MAT + '/M_BayNeon'), MAT, name, vectors={'Color': (r, g, b, 1)}, scalars={'Glow': glow})
    _NEON[key] = inst
    return inst


def paint_nanite():
    """Only for whoever exports a mesh with save_mesh(nanite=True): the palette's parent then needs the Nanite usage
    flag, or a cooked game draws the default material. Sets it on the existing M_Paint without rebuilding its graph.
    Not called by build_materials(): Bayshore's props are not Nanite (measured slower, see ft_export.save_mesh)."""
    m = AL.load_asset('/Game/Core/Materials/M_Paint')
    if m.get_editor_property('used_with_nanite'):
        return 'M_Paint already has the Nanite flag'
    m.set_editor_property('used_with_nanite', True)
    MEL.recompile_material(m)
    AL.save_loaded_asset(m)
    return 'M_Paint: Nanite usage flag set'


def build_materials():
    g = glass()
    n = neon()
    return 'buildings: %s, %s' % (g.get_name(), n.get_name())
