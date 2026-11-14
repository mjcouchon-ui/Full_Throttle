/* FULL THROTTLE — everything you see. Reads the engine's state, never changes the rules.
   The camera rides behind and above the KESTREL-88: the engine's strip of road (x across, y down
   the strip) is filmed in pseudo-3D, the highway running away to the horizon and bending.
   1.0: the look is the cel-shaded 80s beach town of Bayshore. Every car, prop and the skyline are
   sprites rendered in Unreal Engine 5.8 (projects/set-to-fail/full-throttle-unreal) and listed in
   assets/art/sprites.js; the road, sky, sea and effects are drawn here in the same flat-band style.
   1.2: the road is lit (sun or moon on its face, haze with distance, floodlight and headlight pools),
   cars carry glowing lights, the effects glow, and the frame goes through a post pass (bloom, a zoom
   blur at speed, a colour grade, vignette and grain). The Alchemist's lightning is drawn here too.
   1.3: the Syndicate chopper (an Unreal sprite, its rotor drawn spinning here) and the henchmen it
   drops; explosions that are mostly fire and light (a flash, a climbing fireball, flaming debris,
   sparks and embers, a burning patch, light thrown over everything round them); the KESTREL's damage
   in three Unreal-rendered stages plus soot, smoke, fire, sparks and parts falling off; and the
   highlight reel's cameras (RCAM: low, alongside, crane, aerial, roadside, kill cam, dolly) and titles.
   1.5: level 3, the Nevada desert at dusk (DUSK): a synthwave sunset, far ridges, Las Vegas lit up dead
   ahead on the horizon (an Unreal panorama that grows as you near it, with searchlights and a beam off the
   pyramid), sandstone, Joshua trees, poles, a motel, a gas stop and casino pylons; the SNAKE EYES on
   their choppers (whirling chains, flying off the carrier) and THE BIG DEALER's motorcycle carrier. */
(function () {
  'use strict';
  var E = window.FT_ENGINE, H = 600, EDGE = E.EDGE, RH = E.ROAD_HALF;
  var REDUCED = !!(window.matchMedia && matchMedia('(prefers-reduced-motion: reduce)').matches);
  var cv, c, dpr = 1, W = 960, CH = 600, scale = 1, viewW = 960, viewH = 600, oy = 0;
  var parts = [], pops = [], delayed = [];
  var CIV = ['#d9d4c7', '#3f6fb5', '#c9a227', '#6b8f71', '#9b3d3d', '#5a5f6b', '#e07a3c', '#7a5ba8'];
  var FONT = 'system-ui,-apple-system,"Segoe UI",Roboto,sans-serif';
  var MONO = 'ui-monospace,"Cascadia Mono",Consolas,Menlo,monospace';
  function font(px, w, mono) { return (w || 800) + ' ' + px + 'px ' + (mono ? MONO : FONT); }
  function clamp(v, a, b) { return v < a ? a : v > b ? b : v; }
  function easeOutBack(t) { t = clamp(t, 0, 1); var c1 = 1.70158, c3 = c1 + 1; return 1 + c3 * Math.pow(t - 1, 3) + c1 * Math.pow(t - 1, 2); }
  function easeOut(t) { t = clamp(t, 0, 1); return 1 - Math.pow(1 - t, 3); }
  function rr(x, y, w, h, r) {
    r = Math.max(0, Math.min(r, w / 2, h / 2));
    c.beginPath(); c.moveTo(x + r, y); c.lineTo(x + w - r, y); c.quadraticCurveTo(x + w, y, x + w, y + r);
    c.lineTo(x + w, y + h - r); c.quadraticCurveTo(x + w, y + h, x + w - r, y + h); c.lineTo(x + r, y + h);
    c.quadraticCurveTo(x, y + h, x, y + h - r); c.lineTo(x, y + r); c.quadraticCurveTo(x, y, x + r, y); c.closePath();
  }

  function init(canvas) {
    cv = canvas; c = cv.getContext('2d');
    resize();
  }
  function resize() {
    dpr = Math.min(window.devicePixelRatio || 1, 2.5);
    W = window.innerWidth; CH = window.innerHeight;
    cv.width = Math.round(W * dpr); cv.height = Math.round(CH * dpr);
    cv.style.width = W + 'px'; cv.style.height = CH + 'px';
    scale = Math.min(CH / H, W / 340);
    viewW = W / scale; viewH = CH / scale; oy = viewH - H;
  }
  function layout() { return { viewW: viewW, viewH: viewH, scale: scale, oy: oy, wide: viewW >= 760 }; }

  // ---------- the chase camera ----------
  // The track (track.js) is real 3D: every point along it has a position and its own forward, right
  // and up, so the road can climb, bank, loop and roll over. Things on the road are placed in road
  // terms (x across, height up off the road, Z = world units ahead of the camera along the road) and
  // proj() carries that onto the track and then onto the screen. One engine unit is ZK world units.
  var T = window.FT_TRACK, TF = null;
  var ZK = T.ZK, CAMB = 140, NEAR = 24, DRAW = 5600, SEG = 36, JUMP = 58, OBJ_FAR = 2900;
  // ZN: how far behind the camera's own spot on the road things are still drawn. The chase camera
  // looks straight down the road (NEAR); a reel camera can stand off to the side or high up and look
  // back across the road, so it draws from well behind itself (whatever is behind the lens is culled).
  var ZN = NEAR, RCAM = null, REELON = false;
  var NS = Math.ceil(DRAW / SEG) + 2;
  var V = { hY: 270, k: 3.6, f: 756, ch: 75, cx: 480, zp: 210, camX: 0, camY: 0, dc: 0, s0: 0, px: 480, py: 540,
    ox: 0, oy: 0, oz: 0, Fx: 0, Fy: 0, Fz: 1, Rx: 1, Ry: 0, Rz: 0, Ux: 0, Uy: 1, Uz: 0 };
  var cam = { on: false, y: 0, x: 0, lift: 0, fov: 1, roll: 0 };
  // 1.7.2: the projector's results live on an object, not in closure variables: a double stored in a
  // closure slot is boxed (a fresh heap number every store), and proj() runs tens of thousands of times a frame
  var PJ = { x: 0.5, y: 0.5, s: 1.5, z: 1.5 }, PBAD = false;
  // the track at world distance s from the start line, between samples
  var TP = { x: 0, y: 0, z: 0, rx: 1, ry: 0, rz: 0, ux: 0, uy: 1, uz: 0, fx: 0, fy: 0, fz: 1, corner: 0 };
  function trackAt(s) {
    var u = (s + TF.pre) / TF.step;
    if (u < 0) u = 0;
    var i = u | 0;
    if (i > TF.n - 2) { i = TF.n - 2; u = i + 1; }
    var f = u - i, g = 1 - f, j = i + 1;
    TP.x = TF.px[i] * g + TF.px[j] * f; TP.y = TF.py[i] * g + TF.py[j] * f; TP.z = TF.pz[i] * g + TF.pz[j] * f;
    TP.rx = TF.rx[i] * g + TF.rx[j] * f; TP.ry = TF.ry[i] * g + TF.ry[j] * f; TP.rz = TF.rz[i] * g + TF.rz[j] * f;
    TP.ux = TF.ux[i] * g + TF.ux[j] * f; TP.uy = TF.uy[i] * g + TF.uy[j] * f; TP.uz = TF.uz[i] * g + TF.uz[j] * f;
    TP.fx = TF.fx[i] * g + TF.fx[j] * f; TP.fy = TF.fy[i] * g + TF.fy[j] * f; TP.fz = TF.fz[i] * g + TF.fz[j] * f;
    TP.corner = TF.corner[i] * g + TF.corner[j] * f;
  }
  // a world point onto the screen; PBAD when it is behind the camera
  function projW(wx, wy, wz) {
    wx -= V.ox; wy -= V.oy; wz -= V.oz;
    var z = wx * V.Fx + wy * V.Fy + wz * V.Fz;
    PBAD = z < 2; if (z < 2) z = 2;
    PJ.z = z; PJ.s = V.f / z;
    PJ.x = V.cx + (wx * V.Rx + wy * V.Ry + wz * V.Rz) * PJ.s;
    PJ.y = V.hY - (wx * V.Ux + wy * V.Uy + wz * V.Uz) * PJ.s;
  }
  function proj(X, Yh, Z) {
    trackAt(V.s0 + Z);
    projW(TP.x + TP.rx * X + TP.ux * Yh, TP.y + TP.ry * X + TP.uy * Yh, TP.z + TP.rz * X + TP.uz * Yh);
  }
  function zOf(y) { return (V.camY - y) * ZK; }
  function dOfZ(Z) { return V.dc + Z / ZK; }

  var TF_SEEN = null;
  function useTrack() {
    TF = T.build();
    if (TF !== TF_SEEN) { TF_SEEN = TF; PCACHE = {}; PGRID = null; GCOL = null; GANT = {}; FLAT = T.level === 2 ? CHEMF : T.level === 3 ? DESF : BAYF; setLook(); }
  }
  function buildView(S, rdt) {
    useTrack();
    var p = S.p;
    if (RCAM) { reelView(S, RCAM); buildLights(S, rdt); return; }
    ZN = NEAR;
    V.hY = Math.round(viewH * 0.45);
    // the lens widens with speed and punches out when turbo or a drift boost lights
    var spd = clamp((S.speed - 380) / 420, 0, 1);
    cam.fov += ((1 - 0.11 * spd - 0.07 * (p.kick || 0)) - cam.fov) * (1 - Math.exp(-rdt * 4));
    V.k = Math.min(3.6 * viewH / 600, viewW / 112) * cam.fov;
    V.zp = CAMB * ZK; V.f = V.k * V.zp; V.cx = viewW / 2;
    var lift = p.liftH || 0;
    var tY = p.y + p.h / 2 + CAMB, tX = p.x * 0.72 - (p.yaw || 0) * 24;
    if (!cam.on || Math.abs(cam.y - tY) > 220) { cam.on = true; cam.y = tY; cam.x = tX; cam.lift = 0; }
    var a = 1 - Math.exp(-rdt * 4), b = 1 - Math.exp(-rdt * 8);
    cam.y += (tY - cam.y) * a; cam.x += (tX - cam.x) * b; cam.lift += (lift * (p.gapJ ? 0.9 : 0.6) - cam.lift) * b; // over a gap the camera flies with you
    V.ch = (viewH * 0.9 - V.hY) / V.k + cam.lift;
    V.camY = cam.y; V.camX = cam.x;
    V.dc = S.dist + H - cam.y;
    V.s0 = V.dc * ZK;
    // inside a loop the road between camera and car curls up, so the camera sits lower to keep
    // looking at the back of the car rather than down on its roof
    trackAt(V.s0 + V.zp); var f1x = TP.fx, f1y = TP.fy, f1z = TP.fz;
    trackAt(V.s0);
    var curl = clamp((1 - (TP.fx * f1x + TP.fy * f1y + TP.fz * f1z)) / 0.3, 0, 1);
    V.ch *= 1 - 0.45 * curl;
    // the camera rides behind the car, raised off the road along the road's own up
    V.ox = TP.x + TP.rx * V.camX + TP.ux * V.ch; V.oy = TP.y + TP.ry * V.camX + TP.uy * V.ch; V.oz = TP.z + TP.rz * V.camX + TP.uz * V.ch;
    // It aims at the spot level with its own height above the car, so on a straight the framing is
    // the classic chase view and in a loop it still looks at the car. It leans with only part of a
    // banked corner (so the bank shows) but all of a loop or a corkscrew (so you go round with it).
    trackAt(V.s0 + V.zp);
    var fx = TP.x + TP.rx * V.camX + TP.ux * V.ch - V.ox, fy = TP.y + TP.ry * V.camX + TP.uy * V.ch - V.oy, fz = TP.z + TP.rz * V.camX + TP.uz * V.ch - V.oz;
    var ux = 0, uy = 0, uz = 0;
    for (var n = 0; n < 4; n++) {
      trackAt(V.s0 + V.zp * (0.5 + n * 0.17));
      var al = -0.4 * TP.corner, ca = Math.cos(al), sa = Math.sin(al);
      ux += TP.ux * ca + TP.rx * sa; uy += TP.uy * ca + TP.ry * sa; uz += TP.uz * ca + TP.rz * sa;
    }
    var m = Math.hypot(fx, fy, fz) || 1; fx /= m; fy /= m; fz /= m;
    var rx = uy * fz - uz * fy, ry = uz * fx - ux * fz, rz = ux * fy - uy * fx;
    m = Math.hypot(rx, ry, rz) || 1; rx /= m; ry /= m; rz /= m;
    V.Fx = fx; V.Fy = fy; V.Fz = fz; V.Rx = rx; V.Ry = ry; V.Rz = rz;
    V.Ux = fy * rz - fz * ry; V.Uy = fz * rx - fx * rz; V.Uz = fx * ry - fy * rx;
    // a little roll into the turn (more in a drift), eased so it never snaps
    var rollT = REDUCED ? 0 : clamp(-(p.vx || 0) / 300 * 0.028 - (p.drift ? p.drift.dir * 0.035 : 0), -0.08, 0.08);
    cam.roll += (rollT - cam.roll) * (1 - Math.exp(-rdt * 4));
    var ca = Math.cos(cam.roll), sa = Math.sin(cam.roll);
    var Rx2 = V.Rx * ca + V.Ux * sa, Ry2 = V.Ry * ca + V.Uy * sa, Rz2 = V.Rz * ca + V.Uz * sa;
    V.Ux = V.Ux * ca - V.Rx * sa; V.Uy = V.Uy * ca - V.Ry * sa; V.Uz = V.Uz * ca - V.Rz * sa;
    V.Rx = Rx2; V.Ry = Ry2; V.Rz = Rz2;
    buildLights(S, rdt);
  }

  // 1.3: a reel camera, given in road terms: cd/td = road distance of the lens and of what it looks
  // at, cx/tx across, ch/th height above the road, zoom (1 = the chase lens), roll.
  function reelView(S, C) {
    ZN = -330;
    V.hY = Math.round(viewH * 0.5);
    V.k = Math.min(3.6 * viewH / 600, viewW / 112) * (C.zoom || 1);
    V.zp = CAMB * ZK; V.f = V.k * V.zp; V.cx = viewW / 2;
    V.dc = C.cd; V.s0 = C.cd * ZK; V.camY = S.dist + H - C.cd; V.camX = C.cx; V.ch = C.ch;
    cam.on = false;
    trackAt(V.s0);
    // a crane or an aerial rises straight up in the world (so a loop or a corkscrew can't tip it over);
    // the low cameras ride the road's own up, like the chase camera
    var W = !!C.wup, ux = W ? 0 : TP.ux, uy = W ? 1 : TP.uy, uz = W ? 0 : TP.uz;
    V.ox = TP.x + TP.rx * C.cx + ux * C.ch; V.oy = TP.y + TP.ry * C.cx + uy * C.ch; V.oz = TP.z + TP.rz * C.cx + uz * C.ch;
    trackAt(C.td * ZK);
    if (!W) { ux += TP.ux; uy += TP.uy; uz += TP.uz; }
    var fx = TP.x + TP.rx * C.tx + TP.ux * C.th - V.ox, fy = TP.y + TP.ry * C.tx + TP.uy * C.th - V.oy, fz = TP.z + TP.rz * C.tx + TP.uz * C.th - V.oz;
    var m = Math.hypot(fx, fy, fz) || 1; fx /= m; fy /= m; fz /= m;
    var rx = uy * fz - uz * fy, ry = uz * fx - ux * fz, rz = ux * fy - uy * fx;
    m = Math.hypot(rx, ry, rz) || 1; rx /= m; ry /= m; rz /= m;
    V.Fx = fx; V.Fy = fy; V.Fz = fz; V.Rx = rx; V.Ry = ry; V.Rz = rz;
    V.Ux = fy * rz - fz * ry; V.Uy = fz * rx - fx * rz; V.Uz = fx * ry - fy * rx;
    var ca = Math.cos(C.roll || 0), sa = Math.sin(C.roll || 0);
    var Rx2 = V.Rx * ca + V.Ux * sa, Ry2 = V.Ry * ca + V.Uy * sa, Rz2 = V.Rz * ca + V.Uz * sa;
    V.Ux = V.Ux * ca - V.Rx * sa; V.Uy = V.Uy * ca - V.Ry * sa; V.Uz = V.Uz * ca - V.Rz * sa;
    V.Rx = Rx2; V.Ry = Ry2; V.Rz = Rz2;
  }
  // the angle a reel camera sees a sprite from, off the road's own line (the sprites were filmed from
  // behind at a range of angles, so a camera off to one side picks the frame that shows that side)
  function viewPsi(X, Z) { return RCAM ? clamp(Math.atan2(X - V.camX, Math.max(30, Z)) * 57.3, -70, 70) : 0; }

  // ---------- particles (live in the engine's road space, plus a height) ----------
  // 1.7.2: the cap is kept by stepFX (once a frame, before anything is drawn), not by a shift() per new particle
  function partCap() { return REDUCED ? 300 : LOWFX ? 650 : 1200; }
  function trimParts() { var cap = partCap() + 1; if (parts.length > cap) parts.splice(0, parts.length - cap); }
  function P(o) {
    o.age = 0; o.h = o.h || 0; o.vh = o.vh || 0; parts.push(o); return o;
  }
  // 1.7.2: effects that used to roll a chance once per drawn frame roll it per 60th of a second instead
  // (FR = this frame's length in 60ths), so a 120 or 144 Hz screen gets the same smoke and sparks as 60 Hz
  var FR = 1;
  function hit(p) { return Math.random() < p * FR; }
  function easeK(k) { return 1 - Math.pow(1 - k, FR); }
  // 1.3 explosions: a white-hot flash, a fireball of flame puffs that swell and climb and burn from white
  // through yellow and orange to deep red, a column of fire out of the big ones, flaming chunks, a
  // fountain of sparks, embers drifting up for seconds, a patch left burning on the road, and only a
  // little smoke, late. Each one throws light: on the road (flashLight), over everything round it
  // (BOOMS, added over the scene) and on the lens (FX.boom).
  var BOOMS = [];
  var COLK = { hog: '#ff3355', rammer: '#7b3bd6', suv: '#23202c', truck: '#5d6b4c', boss: '#3a3d45', moto: '#3a3e4c', heli: '#7b3bd6', sedan: '#d9d4c7', hatch: '#3f6fb5', van: '#c9a227', bus: '#e8b923', crash: '#7b3bd6' };
  function burst(x, y, size, kind, alt) {
    if (!(size > 0)) return;
    var n = REDUCED ? 0.4 : LOWFX ? 0.65 : 1, i, a, s, sq = Math.sqrt(size), h0 = alt || 0;
    P({ t: 'flash', x: x, y: y, h: h0 + 14 * sq, life: 0.2, r: 44 * size, g: 0.5 });
    P({ t: 'glow', x: x, y: y, h: h0 + 16 * sq, life: 0.9, r: 70 * size, g: 0.7, col: '255,120,40' });
    P({ t: 'ring', x: x, y: y, h: h0, life: 0.42, r: 14, R: 90 * size, g: 0.8, col: '255,190,90', add: true });
    if (h0 < 20) { P({ t: 'scorch', x: x, y: y, h: 0, life: 5, r: 24 * sq, g: 1 }); P({ t: 'pyre', x: x, y: y, h: 0, life: 1.4 + 1.3 * sq, r: 12 * sq, g: 1 }); }
    flashLight(x, y, 160 * sq, 3, FIREL, 0.75 + 0.15 * size);
    BOOMS.push({ x: x, y: y, h: h0 + 18 * sq, size: size, age: 0, life: 0.55 + 0.2 * size });
    if (BOOMS.length > 10) BOOMS.shift();
    FX.boom = Math.max(FX.boom, Math.min(0.8, 0.2 + 0.2 * size));
    // 1.4: the fire itself is Unreal flipbooks (billowing fireballs that boil out, climb and burn down
    // to soot, and a ragged star of flame tongues for the first instant), several at once, each with
    // its own variant, size, turn, mirroring and timing, so no two blasts look alike
    var FB = SP.fx_fireball && IMG.fx_fireball;
    if (FB) {
      var rows = SP.fx_fireball.rows, v0 = Math.floor(Math.random() * rows), life0 = 0.8 + 0.22 * sq, sp6 = Math.pow(size, 0.6);
      if (SP.fx_burst && IMG.fx_burst) P({ t: 'fxb', sh: 'fx_burst', v: Math.floor(Math.random() * SP.fx_burst.rows), x: x, y: y + 1, h: h0 + 3 * sq, life: 0.24 + 0.04 * sq, mul: 0.95 * sp6, rot: (Math.random() - 0.5) * 0.7, flip: Math.random() < 0.5, end: 0.7, g: 0.5 });
      P({ t: 'fxb', sh: 'fx_fireball', v: v0, x: x, y: y, h: h0, life: life0, mul: (0.95 + Math.random() * 0.2) * sp6, rot: (Math.random() - 0.5) * 0.3, flip: Math.random() < 0.5, g: 0.55, vh: 8, drag: 1 });
      var nsec = Math.max(1, Math.round((size >= 1.5 ? 3 : size >= 0.8 ? 2 : 1) * (REDUCED ? 0.5 : 1)));
      for (i = 0; i < nsec; i++) {
        a = Math.random() * 6.283; var rr0 = (9 + Math.random() * 14) * sq;
        P({ t: 'fxb', sh: 'fx_fireball', v: (v0 + 1 + i) % rows, x: x + Math.cos(a) * rr0, y: y + Math.sin(a) * rr0 * 0.6, h: h0 + Math.random() * 8 * sq, life: life0 * (0.65 + Math.random() * 0.25),
          mul: (0.42 + Math.random() * 0.25) * sp6, rot: (Math.random() - 0.5) * 0.6, flip: Math.random() < 0.5, g: 0.6, vx: Math.cos(a) * 40 * sq, vy: Math.sin(a) * 26 * sq, vh: 20 + Math.random() * 40, drag: 2.2, dl: 0.03 + Math.random() * 0.12 });
      }
      // a big one throws a column of fire up out of the middle
      if (size >= 1.5) for (i = 0; i < 2; i++) P({ t: 'fxb', sh: 'fx_fireball', v: (v0 + 2 + i) % rows, x: x + (Math.random() - 0.5) * 10, y: y, h: h0 + 12, life: life0 * 0.75, mul: (0.5 - 0.1 * i) * sp6,
        rot: (Math.random() - 0.5) * 0.3, flip: Math.random() < 0.5, g: 0.6, vh: 120 + 70 * i, grav: 70, drag: 0.7, dl: 0.07 + i * 0.1 });
    } else {
      for (i = 0; i < 22 * size * n; i++) {
        a = Math.random() * 6.283; s = (30 + Math.random() * 110) * sq;
        P({ t: 'fball', x: x + Math.cos(a) * 6 * size, y: y + Math.sin(a) * 6 * size, h: h0 + 6 + Math.random() * 16 * sq, vx: Math.cos(a) * s, vy: Math.sin(a) * s * 0.7,
          vh: (40 + Math.random() * 120) * sq, grav: -40, life: 0.45 + Math.random() * 0.5, r: (7 + Math.random() * 9) * sq, grow: 22 * sq, g: 0.5, drag: 3.2, dl: Math.random() * 0.08 });
      }
      if (size >= 1.5) for (i = 0; i < 9 * size * n; i++) {
        P({ t: 'fball', x: x + (Math.random() - 0.5) * 16, y: y + (Math.random() - 0.5) * 16, h: h0 + 10, vx: (Math.random() - 0.5) * 30, vy: (Math.random() - 0.5) * 20,
          vh: 120 + Math.random() * 160, grav: -30, life: 0.7 + Math.random() * 0.5, r: (8 + Math.random() * 6) * sq, grow: 30, g: 0.6, drag: 1.4, dl: 0.05 + Math.random() * 0.25 });
      }
    }
    for (i = 0; i < 7 * size * n; i++) {
      a = Math.random() * 6.283; s = 110 + Math.random() * 240;
      P({ t: 'debris', dk: i % 3, x: x, y: y, h: h0 + 10, vx: Math.cos(a) * s, vy: Math.sin(a) * s, vh: 150 + Math.random() * 260, grav: 650, life: 0.9 + Math.random() * 0.8, r: 2 + Math.random() * 3.5,
        rot: Math.random() * 6, vr: (Math.random() - 0.5) * 20, g: 0.95, drag: 1.1, col: Math.random() < 0.5 ? '#2b2b2b' : '#5a4a55', burn: Math.random() < 0.7 });
    }
    // body panels in the colour of what blew up
    var pc = COLK[kind];
    if (pc) for (i = 0; i < Math.min(5, 2 + size * 1.5) * n; i++) {
      a = Math.random() * 6.283; s = 90 + Math.random() * 180;
      P({ t: 'part', x: x, y: y, h: h0 + 12, vx: Math.cos(a) * s, vy: Math.sin(a) * s, vh: 180 + Math.random() * 240, grav: 650, life: 1.6 + Math.random(), rot: Math.random() * 6,
        vr: (Math.random() - 0.5) * 22, g: 0.95, drag: 0.8, col: pc, w: 4 + Math.random() * 5, l: 2.5 + Math.random() * 3, burn: Math.random() < 0.5 });
    }
    for (i = 0; i < 30 * size * n; i++) {
      a = Math.random() * 6.283; s = 160 + Math.random() * 340;
      P({ t: 'spark', x: x, y: y, h: h0 + 10, vx: Math.cos(a) * s, vy: Math.sin(a) * s, vh: 80 + Math.random() * 300, grav: 520, life: 0.3 + Math.random() * 0.45, g: 0.5, drag: 1.6,
        col: Math.random() < 0.3 ? '#ffffff' : Math.random() < 0.5 ? '#ffd23f' : '#ff9e3d' });
    }
    for (i = 0; i < 26 * size * n; i++) {
      a = Math.random() * 6.283; s = 20 + Math.random() * 90;
      P({ t: 'ember', x: x, y: y, h: h0 + 10 + Math.random() * 30, vx: Math.cos(a) * s, vy: Math.sin(a) * s, vh: 30 + Math.random() * 90, grav: -8, life: 1.4 + Math.random() * 1.8,
        r: 1 + Math.random() * 1.4, g: 0.8, drag: 0.9, ph: Math.random() * 6.283 });
    }
    for (i = 0; i < (FB ? 2 : 4) * size * n; i++) {
      a = Math.random() * 6.283; s = Math.random() * 40;
      P({ t: 'smoke', x: x, y: y, h: h0 + 20 * sq, vx: Math.cos(a) * s, vy: Math.sin(a) * s, vh: 40 + Math.random() * 40, life: 1.2 + Math.random(), r: (6 + Math.random() * 6) * sq,
        grow: 20, g: 0.85, drag: 1.5, dark: true, dl: 0.35 + Math.random() * 0.3 });
    }
    if (size >= 1.8 && !REDUCED) P({ t: 'rays', x: x, y: y, h: h0 + 16 * sq, life: 0.34, r: 130 * size, g: 0.5, rot: Math.random() * 6 });
  }
  // a piece of the KESTREL coming off: a mirror, a hubcap, the aerial, a wing endplate, a panel, the bumper, the plate
  var PARTS = [['#ff2d3c', 4, 3], ['#ffc93c', 3.6, 3.6, 1], ['#1c1a26', 1, 9], ['#1c1a26', 2.2, 7], ['#ff2d3c', 7, 4], ['#1c1a26', 10, 2.6], ['#ffe14d', 5, 2.4]];
  function partOff(n, pl) {
    var q = PARTS[n % PARTS.length], sd = Math.random() < 0.5 ? -1 : 1;
    P({ t: 'part', x: pl.x + sd * 12, y: pl.y + 8, h: 11 + (pl.liftH || 0), vx: sd * (60 + Math.random() * 90), vy: 110 + Math.random() * 120, vh: 150 + Math.random() * 130, grav: 650,
      life: 2.6, rot: Math.random() * 6, vr: (Math.random() - 0.5) * 26, g: 1, drag: 0.5, col: q[0], w: q[1], l: q[2], disc: !!q[3] });
    sparks(pl.x + sd * 12, pl.y + 8, 12, null, 10);
  }
  // a henchman leaving the car (or the rope), as a tumbling rag doll
  function ragdoll(e, S) {
    var how = e.how, sd = e.side || (Math.random() < 0.5 ? -1 : 1), pl = S.p;
    var o = { t: 'rag', x: e.x, y: e.y, h: e.alt || 8, vx: 0, vy: 0, vh: 0, grav: 520, life: 1.8, rot: 0, vr: sd * 10, g: 1, drag: 0.3 };
    if (how === 'wall') {
      o.x = pl.x + sd * 18; o.y = pl.y; o.h = 10; o.vx = sd * 170; o.vy = 150; o.vh = 230;
      var wx = sd * (EDGE + 2);
      sparks(wx, pl.y, 26, '#ffd23f', 8); sparks(wx, pl.y + 10, 14, null, 4);
      flashLight(wx, pl.y, 80, 1.6, FIREL, 0.35);
      P({ t: 'ring', x: wx, y: pl.y, h: 8, life: 0.35, r: 6, R: 40, g: 1, col: '255,220,140', add: true });
      for (var i = 0; i < 6; i++) P({ t: 'puff', x: wx, y: pl.y + (Math.random() - 0.5) * 20, h: 6, vx: -sd * Math.random() * 40, vy: 60, vh: 20, life: 0.6, r: 4, grow: 30, g: 0.95, drag: 2 });
    } else if (how === 'swipe') { o.x = pl.x + sd * 18; o.y = pl.y; o.h = 10; o.vx = sd * 210; o.vy = 120; o.vh = 170; sparks(o.x, o.y, 16, null, 8); }
    else if (how === 'fall') { o.x = pl.x + sd * 14; o.y = pl.y; o.h = 12; o.vx = sd * 50; o.vy = 60; o.vh = 80; o.grav = S.inverted ? -300 : 420; o.g = S.inverted ? 0.7 : 1; o.life = 1.5; }
    else if (how === 'wreck') { o.x = pl.x + sd * 16; o.y = pl.y; o.h = 10; o.vx = sd * 160; o.vy = 80; o.vh = 260; }
    else if (how === 'miss') { o.vx = (Math.random() - 0.5) * 60; o.vh = 40; o.life = 1.6; for (var j = 0; j < 5; j++) P({ t: 'puff', x: e.x, y: e.y, h: 2, vx: (Math.random() - 0.5) * 60, vy: 40, vh: 15, life: 0.5, r: 4, grow: 28, g: 1, drag: 2 }); }
    else { o.vh = -20; o.g = 0.9; }
    P(o);
  }
  function sparks(x, y, n, col, h) {
    for (var i = 0; i < n; i++) {
      var a = Math.random() * 6.283, s = 80 + Math.random() * 200;
      P({ t: 'spark', x: x, y: y, h: h || 8, vx: Math.cos(a) * s, vy: Math.sin(a) * s, vh: 40 + Math.random() * 160, grav: 600, life: 0.15 + Math.random() * 0.2, g: 0.6, drag: 2, col: col });
    }
  }
  var DCOL = [null, '#5fd0ff', '#ffa53d', '#ff4fb8'], DRGB = ['255,255,255', '95,208,255', '255,165,61', '255,79,184'];
  function eburst(x, y, n, h) { // an electric splash: cyan sparks and little arcs
    sparks(x, y, n, '#9fe8ff', h || 8);
    for (var i = 0; i < n / 3; i++) P({ t: 'arc', x: x + (Math.random() - 0.5) * 30, y: y + (Math.random() - 0.5) * 30, h: 4 + Math.random() * 20, life: 0.18 + Math.random() * 0.2, r: 10 + Math.random() * 16, g: 0.8 });
  }
  function consume(S, evs) {
    for (var i = 0; i < evs.length; i++) {
      var e = evs[i];
      switch (e.t) {
        case 'driftStart': for (var d0 = 0; d0 < 8; d0++) P({ t: 'puff', x: S.p.x + (Math.random() - 0.5) * 30, y: S.p.y + 22, h: 2, vx: (Math.random() - 0.5) * 80, vy: 20 + Math.random() * 40, vh: 20, life: 0.6, r: 5, grow: 36, g: 0.92, drag: 2 }); break;
        case 'driftLvl':
          P({ t: 'ring', x: S.p.x, y: S.p.y + 20, life: 0.3, r: 8, R: 30, g: 0.5, col: DRGB[e.lvl], add: true });
          for (var d1 = 0; d1 < 10; d1++) sparks(S.p.x + (d1 % 2 ? 12 : -12), S.p.y + 24, 1, DCOL[e.lvl], 3);
          break;
        case 'boost':
          FX.boostT = 0.5; FX.boostLvl = e.lvl;
          P({ t: 'ring', x: S.p.x, y: S.p.y + 26, life: 0.4, r: 10, R: 48, g: 0.4, col: DRGB[e.lvl], add: true });
          break;
        case 'turbo': P({ t: 'ring', x: S.p.x, y: S.p.y + 26, life: 0.35, r: 10, R: 44, g: 0.4, col: '54,241,255', add: true }); break;
        case 'zap':
          FX.flash = Math.max(FX.flash, 0.7);
          for (var zy = (S.boss ? S.boss.y + S.boss.h / 2 : 100); zy < 590; zy += 70) { eburst(e.x + (Math.random() - 0.5) * 30, zy, 4, 4); flashLight(e.x, zy, 60, 1.2, ZAPL, 0.5); }
          break;
        case 'strike':
          FX.flash = Math.max(FX.flash, 0.9);
          eburst(e.x, e.y, 22, 6); flashLight(e.x, e.y, 110, 2, ZAPL, 0.6);
          P({ t: 'ring', x: e.x, y: e.y, life: 0.45, r: 10, R: 80, g: 0, col: '150,235,255', add: true });
          P({ t: 'scorch', x: e.x, y: e.y, h: 0, life: 3, r: 26, g: 1 });
          break;
        case 'ringOut':
          FX.flash = Math.max(FX.flash, 0.6);
          if (S.boss) { flashLight(S.boss.x, S.boss.y + 60, 160, 2, ZAPL, 0.6); P({ t: 'ring', x: S.boss.x, y: S.boss.y + 70, life: 0.5, r: 30, R: 200, g: 0, col: '110,225,255', add: true }); }
          break;
        case 'bossPhase': FX.flash = 1; if (S.boss) eburst(S.boss.x, S.boss.y + 40, 30, 30); break;
        case 'shock': FX.shock = 0.8; eburst(S.p.x, S.p.y, 14, 10); break;
        case 'dodge': P({ t: 'ring', x: S.p.x, y: S.p.y, life: 0.4, r: 8, R: 50, g: 0, col: '125,255,155', add: true }); break;
        case 'explode':
          if (e.delay) delayed.push({ at: e.delay, e: e }); else burst(e.x, e.y, e.size, e.kind, e.alt);
          break;
        case 'hit':
          sparks(e.x, e.y, e.big ? 4 : 3, e.big ? '#9fe8ff' : null, 12);
          // a missile tears panels off what it hits
          if (e.d >= 6 && COLK[e.kind]) for (var hp2 = 0; hp2 < 2; hp2++) P({ t: 'part', x: e.x, y: e.y, h: 14, vx: (Math.random() - 0.5) * 200, vy: 60 + Math.random() * 120, vh: 160 + Math.random() * 160,
            grav: 650, life: 1.5, rot: Math.random() * 6, vr: (Math.random() - 0.5) * 22, g: 0.95, drag: 0.8, col: COLK[e.kind], w: 4 + Math.random() * 4, l: 2.5 + Math.random() * 2 });
          break;
        case 'partOff': partOff(e.n, S.p); jolt(PSUS, -4, (Math.random() - 0.5) * 1.2); break;
        case 'latch':
          sparks(S.p.x + e.side * 14, S.p.y, 12, null, 10); FX.flinch = 1; jolt(PSUS, -9, e.side * 1.4);
          P({ t: 'ring', x: S.p.x + e.side * 14, y: S.p.y, h: 12, life: 0.3, r: 4, R: 26, g: 0.2, col: '255,79,184', add: true });
          break;
        case 'henchHit': sparks(S.p.x + e.side * 10, S.p.y - 4, 7, '#cfe8ff', 13); FX.flinch = 0.8; jolt(PSUS, -3, e.side * 0.8); break;
        case 'henchOff': ragdoll(e, S); break;
        case 'heliDown':
          P({ t: 'hwreck', x: e.x, y: e.y, h: e.alt, vx: (e.vx || 0) * 0.4, vy: 100, vh: 10, grav: 300, rot: 0, vr: (e.lean >= 0 ? 1 : -1) * 5, life: 0.72, g: 0 });
          FX.boom = 1;
          break;
        case 'spark': sparks(e.x, e.y, 3, '#9fe8ff', 12); break;
        case 'rail': sparks(e.x, e.y, 14, null, 6); flashLight(e.x, e.y, 40, 0.8, FIREL, 0.25); if (S.p) jolt(PSUS, 3, (e.x > S.p.x ? -1 : 1) * 1.6); break;
        case 'bump': sparks(e.x, e.y, 8, null, 8); if (S.p) jolt(PSUS, 4, (e.x > S.p.x ? -1 : 1) * 1.3); break;
        case 'playerHit': jolt(PSUS, -5 - Math.min(8, (e.d || 0) * 0.4), (Math.random() - 0.5) * 1.5); sparks(e.x, e.y, 8, null, 10); FX.hurt = Math.max(FX.hurt, 0.7); FX.flinch = Math.max(FX.flinch, 0.6 + Math.min(0.4, (e.d || 0) / 30)); break;
        case 'gun': P({ t: 'muzzle', x: e.x, y: e.y, h: 20, life: 0.05, g: 0 }); break;
        case 'missile': flashLight(e.x, e.y, 50, 0.9, FIREL, 0.25); for (var k = 0; k < 6; k++) P({ t: 'smoke', x: e.x, y: e.y + 10, h: 12, vx: (Math.random() - 0.5) * 60, vy: 40 + Math.random() * 40, vh: 10, life: 0.6, r: 5, grow: 20, g: 0.9, drag: 2 }); break;
        case 'popup': pops.push({ x: e.x, y: e.y, text: e.text, color: e.color, big: e.big, t: 0 }); if (pops.length > 12) pops.shift(); break;
        case 'land': jolt(PSUS, -16, (Math.random() - 0.5) * 0.8); for (var l = 0; l < 14; l++) P({ t: 'smoke', x: S.p.x + (Math.random() - 0.5) * 30, y: S.p.y + 20, h: 2, vx: (Math.random() - 0.5) * 120, vy: Math.random() * 60, vh: 20, life: 0.6, r: 6, grow: 30, g: 0.95, drag: 3 }); sparks(S.p.x, S.p.y + 26, 12, null, 2); break;
        case 'pickup': P({ t: 'ring', x: e.x, y: e.y, life: 0.35, r: 6, R: 40, g: 0.5, col: '120,255,160', add: true }); break;
        // 1.5: the SNAKE EYES
        case 'chain':
          if (e.hit) { sparks(S.p.x - e.side * 12, S.p.y - 4, 14, '#ffffff', 12); FX.flinch = 0.9; jolt(PSUS, -4, -e.side * 1.6); }
          else P({ t: 'ring', x: e.x - e.side * 14, y: e.y, h: 14, life: 0.25, r: 4, R: 22, g: 0.5, col: '215,227,240', add: true });
          break;
        case 'hogOff':
          sparks(e.x, e.y + 6, 10, '#ffd23f', 2);
          for (var ho2 = 0; ho2 < 5; ho2++) P({ t: 'puff', x: e.x + (Math.random() - 0.5) * 16, y: e.y + 10, h: 2, vx: (Math.random() - 0.5) * 50, vy: 30, vh: 14, life: 0.6, r: 4, grow: 26, g: 0.9, drag: 2 });
          break;
        case 'hogJump': for (var hj = 0; hj < 6; hj++) P({ t: 'puff', x: e.x + (Math.random() - 0.5) * 18, y: e.y, h: 30, vx: (Math.random() - 0.5) * 60, vy: 20, vh: 10, life: 0.6, r: 4, grow: 26, g: 0.9, drag: 2 }); break;
        case 'hogLand':
          sparks(e.x, e.y + 8, 12, '#ffd23f', 2);
          P({ t: 'ring', x: e.x, y: e.y, h: 1, life: 0.35, r: 6, R: 32, g: 0.9, col: '255,200,140', add: true });
          for (var hl = 0; hl < 8; hl++) P({ t: 'puff', x: e.x + (Math.random() - 0.5) * 24, y: e.y + 8, h: 2, vx: (Math.random() - 0.5) * 120, vy: Math.random() * 60, vh: 18, life: 0.6, r: 5, grow: 28, g: 0.95, drag: 3 });
          if (e.crush) { FX.flinch = 1; jolt(PSUS, -12, (Math.random() - 0.5) * 2); }
          break;
        case 'brakeWarn': if (S.boss) flashLight(S.boss.x, S.boss.y + S.boss.h / 2 + 20, 120, 2, [255, 40, 50], 2.2); break;
      }
    }
  }

  function stepFX(S, rdt) {
    var ts = S ? S.timeScale : 1, dt = rdt * ts, sp = S ? S.speed * dt : 0;
    for (var i = delayed.length - 1; i >= 0; i--) {
      delayed[i].at -= dt;
      if (delayed[i].at <= 0) { var e = delayed[i].e; burst(e.x, e.y, e.size, e.kind, e.alt); delayed.splice(i, 1); }
    }
    // 1.7.2: one pass that keeps the live ones in order (a splice per dead particle was O(n) each);
    // particles born during the pass are appended past n0 and kept, unstepped, as before
    trimParts();
    var n0 = parts.length, w = 0;
    for (i = 0; i < n0; i++) {
      var p = parts[i];
      p.age += dt;
      if (p.age >= p.life) continue;
      parts[w++] = p;
      if (p.drag) { var k = Math.exp(-p.drag * dt); p.vx *= k; p.vy *= k; }
      p.x += (p.vx || 0) * dt; p.y += (p.vy || 0) * dt + sp * p.g;
      if (p.grav) p.vh -= p.grav * dt;
      p.h += p.vh * dt;
      if (p.h < 0) { p.h = 0; p.vh = -p.vh * 0.3; if (p.vx) { p.vx *= 0.6; p.vy *= 0.6; } }
      if (p.vr) p.rot += p.vr * dt;
      if (p.grow) p.r += p.grow * dt;
      if (p.ph !== undefined) p.vx += Math.sin(p.age * 3.1 + p.ph) * 40 * dt; // embers wander as they rise
      // burning chunks trail fire; a burning patch keeps licking flames up; a falling chopper trails fire and smoke
      if (p.burn && p.h > 1 && hit(0.5 * ts)) P({ t: 'fire', x: p.x, y: p.y, h: p.h, vx: 0, vy: 0, vh: 10, life: 0.15, r: 1.7, g: p.g, drag: 1, nk: 1 });
      if (p.t === 'pyre') {
        var pk = p.age / p.life;
        if (hit(0.85 * (1 - pk) * (IMG.fx_flame ? 0.5 : 1) * ts)) P({ t: 'fire', x: p.x + (Math.random() - 0.5) * p.r * 2, y: p.y + (Math.random() - 0.5) * p.r, h: 1, vx: 0, vy: 0, vh: 40 + Math.random() * 50, life: 0.3 + Math.random() * 0.25, r: 2.5 + Math.random() * 4 * (1 - pk), g: 1, drag: 1 });
        if (hit(0.06 * ts)) flashLight(p.x, p.y, 60, 0.9 * (1 - pk), FIREL, 0.3);
      }
      if (p.t === 'hwreck') {
        for (var hw2 = 0; hw2 < 2; hw2++) if (hit(1)) P({ t: 'fire', x: p.x + (Math.random() - 0.5) * 16, y: p.y, h: p.h + Math.random() * 10, vx: 0, vy: -40, vh: 30, life: 0.35, r: 6, g: 0, drag: 1 });
        if (hit(0.5)) P({ t: 'smoke', x: p.x, y: p.y, h: p.h + 10, vx: 0, vy: -60, vh: 20, life: 0.8, r: 6, grow: 20, g: 0, drag: 1, dark: true });
      }
    }
    for (i = n0; i < parts.length; i++) parts[w++] = parts[i];
    parts.length = w;
    for (i = BOOMS.length - 1; i >= 0; i--) { BOOMS[i].age += dt; BOOMS[i].y += sp * 0.6; if (BOOMS[i].age >= BOOMS[i].life) BOOMS.splice(i, 1); }
    for (i = pops.length - 1; i >= 0; i--) { pops[i].t += rdt * Math.max(0.35, ts); if (pops[i].t > 1.3) pops.splice(i, 1); }
    if (S && S.p) {
      var hp = S.p.hp, pl = S.p;
      // 1.3 damage you can see: the blower steams, then smokes black, then burns; the torn bumper drags
      // sparks along the road; loose bits spark at the wheels
      var bx = pl.x - (pl.yaw || 0) * 6, by = pl.y + 14, bh = (pl.liftH || 0) + 15, live = S.phase === 'play' || REELON;
      if (live && hp < 82 && hit((hp < 45 ? 0.75 : hp < 65 ? 0.4 : 0.15) * ts)) P({ t: 'smoke', x: bx + (Math.random() - 0.5) * 10, y: by, h: bh, vx: (Math.random() - 0.5) * 20, vy: 40, vh: 32, life: 0.9 + Math.random() * 0.6, r: 3.5, grow: 17, g: 0.9, drag: 1, dark: hp < 50 });
      if (live && hp < 35 && hit((hp < 18 ? 0.95 : 0.6) * (IMG.fx_flame ? 0.4 : 1) * ts)) P({ t: 'fire', x: bx + (Math.random() - 0.5) * 12, y: by, h: bh - 2, vx: (Math.random() - 0.5) * 30, vy: 50, vh: 50, life: 0.32, r: hp < 18 ? 6 : 4.5, g: 0.6, drag: 1 });
      if (live && hp < 35 && hit(0.08 * ts)) flashLight(pl.x, pl.y, 55, 0.8, FIREL, 0.3);
      if (live && hp <= 40 && pl.air <= 0 && hit(0.75 * ts)) P({ t: 'spark', x: pl.x - 11 - (pl.yaw || 0) * 10, y: pl.y + 27, h: 1, vx: (Math.random() - 0.5) * 60, vy: 180 + Math.random() * 160, vh: 30 + Math.random() * 80, grav: 600, life: 0.2 + Math.random() * 0.2, g: 0.6, drag: 2 });
      if (live && hp < 55 && hit(0.12 * ts)) sparks(pl.x + (Math.random() < 0.5 ? -14 : 14), pl.y + 20, 2, null, 2);
      // henchmen on the doors: their shoes scrape the road
      for (var lc = 0; lc < (pl.latch || []).length; lc++) if (hit(0.5 * ts)) P({ t: 'spark', x: pl.x + pl.latch[lc].side * 21, y: pl.y + 6, h: 1, vx: pl.latch[lc].side * 30, vy: 150 + Math.random() * 100, vh: 30 + Math.random() * 50, grav: 600, life: 0.18, g: 0.6, drag: 2, col: '#ffd23f' });
      // drifting: tyre smoke, skid marks, and sparks in the colour of the charge
      if (pl.drift && S.phase === 'play' && pl.air <= 0) {
        var dr = pl.drift, ry = pl.y + pl.h / 2 - 6, sw = -(pl.yaw || 0) * 16;
        FX.skidT = (FX.skidT || 0) - dt;
        if (FX.skidT <= 0) { FX.skidT = 1 / 40; for (var s2 = -1; s2 <= 1; s2 += 2) P({ t: 'skid', x: pl.x + s2 * 11 + sw, y: ry, h: 0, life: 1.5, g: 1 }); }
        if (hit(0.7 * ts)) P({ t: 'puff', x: pl.x + (Math.random() < 0.5 ? -12 : 12) + sw, y: ry + 4, h: 2, vx: -dr.dir * 50 * Math.random(), vy: 30 + Math.random() * 50, vh: 14 + Math.random() * 22, life: 0.75, r: 4, grow: 34, g: 0.92, drag: 2 });
        if (dr.lvl > 0) for (var q2 = 0; q2 < 2; q2++) if (hit(0.85 * ts)) P({ t: 'spark', x: pl.x + (q2 ? 12 : -12) + sw, y: ry + 2, h: 2, vx: (Math.random() - 0.5) * 160 - dr.dir * 70, vy: 60 + Math.random() * 150, vh: 50 + Math.random() * 120, grav: 600, life: 0.22 + Math.random() * 0.2, g: 0.6, drag: 2, col: DCOL[dr.lvl] });
      }
      // turbo and drift boosts: flames off the four pipes
      if ((pl.turboOn || pl.boostT > 0) && S.phase === 'play') {
        var bc = pl.turboOn ? null : DCOL[pl.boostLvl || 1];
        for (var f2 = 0; f2 < 2; f2++) if (hit(0.9 * ts)) P({ t: 'fire', x: pl.x + (f2 ? 1 : -1) * (4 + Math.random() * 5) - (pl.yaw || 0) * 12, y: pl.y + pl.h / 2 + 2, h: (pl.liftH || 0) + 3.5, vx: (Math.random() - 0.5) * 20, vy: 30 + Math.random() * 40, vh: 6, life: 0.16 + Math.random() * 0.1, r: 2.6, g: 0.9, drag: 1, col: bc, ex: 1 });
      }
      // at speed the air streams past in lines
      if (S.phase === 'play' && !REDUCED) {
        var hot2 = clamp((S.speed - 470) / 250, 0, 1);
        if (hot2 > 0 && hit(hot2 * 0.9 * ts)) {
          var sx3 = (Math.random() < 0.5 ? -1 : 1) * (EDGE + 20 + Math.random() * 300);
          P({ t: 'streak', x: sx3, y: -150 - Math.random() * 350, h: 8 + Math.random() * 110, life: 1.4, g: 1.05 });
        }
        if (DUSK && hit(0.012 * ts)) {
          var wsd = Math.random() < 0.5 ? -1 : 1;
          P({ t: 'weed', x: wsd * (EDGE + 40 + Math.random() * 80), y: -260 + Math.random() * 300, h: 0.5, vx: -wsd * (70 + Math.random() * 70), vy: 0, life: 5, g: 1, r: 5 + Math.random() * 3, rot: 0, vr: -wsd * 7 });
        }
        if (DUSK && hit(0.1 * ts)) P({ t: 'mote', x: (Math.random() - 0.5) * 700, y: -250 + Math.random() * 500, h: 5 + Math.random() * 60, vh: 2 + Math.random() * 6, life: 3, g: 1, r: 1.2 + Math.random() * 1.5, col: '255,190,140' });
        if (LV2 && hit(0.18 * ts)) P({ t: 'mote', x: (Math.random() - 0.5) * 700, y: -250 + Math.random() * 500, h: 15 + Math.random() * 90, vh: 4 + Math.random() * 10, life: 3, g: 1, r: 1.5 + Math.random() * 2, col: Math.random() < 0.5 ? '125,255,155' : '232,74,255' });
      }
      for (var m = 0; m < S.missiles.length; m++) {
        var ms = S.missiles[m];
        if (hit(1)) P({ t: 'smoke', x: ms.x, y: ms.y + 8, h: 12, vx: (Math.random() - 0.5) * 16, vy: 0, vh: 6, life: 0.5, r: 3, grow: 16, g: 0.9, drag: 1 });
      }
      for (m = 0; m < S.ents.length; m++) {
        var en = S.ents[m];
        // a damaged foe smokes, then smokes black and throws sparks, then burns
        if (E.KIND[en.kind].foe && en.kind !== 'hench' && en.hp < en.max * 0.7) {
          var efr = en.hp / en.max, eh = (en.alt || 0) + 16;
          if (hit((efr < 0.4 ? 0.45 : 0.15) * ts)) P({ t: 'smoke', x: en.x, y: en.y - en.h * 0.2, h: eh, vx: 0, vy: 20, vh: 30, life: 0.7, r: 4, grow: 16, g: 0.9, drag: 1, dark: efr < 0.45 });
          if (efr < 0.3 && hit(0.55 * (IMG.fx_flame ? 0.45 : 1) * ts)) P({ t: 'fire', x: en.x + (Math.random() - 0.5) * en.w * 0.5, y: en.y - en.h * 0.2, h: eh - 4, vx: 0, vy: 30, vh: 40, life: 0.3, r: 4.5, g: 0.9, drag: 1 });
          if (efr < 0.45 && hit(0.08 * ts)) sparks(en.x + (Math.random() - 0.5) * en.w, en.y + en.h * 0.3, 3, null, eh - 10);
        }
      }
    }
    trimParts();
  }
  function clearFX() { parts.length = 0; pops.length = 0; delayed.length = 0; cam.on = false; FLASHES.length = 0; BOOMS.length = 0; FX.shock = FX.hurt = FX.flash = FX.boom = FX.flinch = 0; }

  // ======================= 1.2: light, haze, glow and post-processing =======================
  // The road and rails are lit per segment: the sun (Bayshore) or moon (the plant) on the face of the
  // road, so banks and loops shade as they turn; the haze that thickens with distance; and placed
  // lights (plant floodlights, the tube lamps, the KESTREL's headlights, explosions and lightning)
  // that pool across the lanes. Colours go through lit(), cached by their light.
  var TNOW = 0, LV2 = false, DUSK = false, LOWFX = false, FRAME_MS = 0;
  var FX = { shock: 0, hurt: 0, flash: 0, sun: null, boostT: 0, boostLvl: 0, sky: 0, skyT: 3, skyX: 0, boom: 0, flinch: 0 };
  var FOGC = [246, 214, 236], FOG0 = 1300, FOGK = 0.5, AMB0 = 0.74, AMB1 = 0.4, LDIR = [0, 1, 0];
  var CC = {}, CCN = 0, HEXC = {}, FOGI = {};
  function setLook() {
    LV2 = T.level === 2; DUSK = T.level === 3;
    if (DUSK) { FOGC = [168, 84, 128]; FOGK = 0.5; AMB0 = 0.8; AMB1 = 0.34; LDIR = [Math.sin(SUN3) * 0.9, 0.32, Math.cos(SUN3) * 0.9]; }
    else if (LV2) { FOGC = [58, 36, 118]; FOGK = 0.62; AMB0 = 0.84; AMB1 = 0.18; LDIR = [Math.sin(0.6) * 0.6, 0.8, Math.cos(0.6) * 0.6]; }
    else { FOGC = [246, 212, 236]; FOGK = 0.52; AMB0 = 0.72; AMB1 = 0.42; LDIR = [Math.sin(SUN_HEADING) * 0.8, 0.6, Math.cos(SUN_HEADING) * 0.8]; }
    CC = {}; CCN = 0; FOGI = {};
  }
  function rgbOf(col) {
    var v = HEXC[col];
    if (!v) { var n = parseInt(col.slice(1), 16); v = HEXC[col] = [n >> 16 & 255, n >> 8 & 255, n & 255]; }
    return v;
  }
  // col under ambient amb, plus added light (lr, lg, lb: 0..255, tinted by the surface), through haze fog
  // 1.7.2: the cache is keyed by numbers (one Map per colour), not by a string built on every call:
  // the road asks for ~2,000 colours a frame and the string keys were most of the page's garbage
  function lit(col, amb, lr, lg, lb, fog) {
    return litQ(col, Math.round(amb * 20) | 0, Math.min(15, lr / 16 | 0), Math.min(15, lg / 16 | 0), Math.min(15, lb / 16 | 0), Math.round(fog * 14) | 0);
  }
  // the same, with the light already in its cache steps (small integers, so nothing is boxed on the way in)
  function litQ(col, ai, ri, gi, bi, fi) {
    var m = CC[col] || (CC[col] = new Map()), key = (((ai * 16 + ri) * 16 + gi) * 16 + bi) * 16 + fi;
    var v = m.get(key);
    if (v !== undefined) return v;
    if (++CCN > 9000) { CC = {}; CCN = 0; m = CC[col] = new Map(); }
    var q = rgbOf(col), a = ai / 20, f = fi / 14;
    v = 'rgb(' + litCh(q[0], a, ri, FOGC[0], f) + ',' + litCh(q[1], a, gi, FOGC[1], f) + ',' + litCh(q[2], a, bi, FOGC[2], f) + ')';
    m.set(key, v);
    return v;
  }
  function litCh(base, a, li, fc, f) { var x = Math.min(255, base * a + li * 16 * (0.22 + base / 255 * 1.05)); return Math.round(x * (1 - f) + fc * f); }
  function fogAt(Z) { var fz = clamp((Z - FOG0) / (DRAW - FOG0), 0, 1); return FOGK * fz * (2 - fz); }

  // placed and passing lights, in road terms: X across, Z ahead of the camera (world units)
  var LITE = [], FLASHES = [], LJ = { r: 0.5, g: 0.5, b: 0.5, a: 1.5, f: 0.5 };
  var SODIUM = [255, 160, 64], TUBEL = [70, 235, 255], HEADL = [215, 235, 255], FIREL = [255, 150, 60], ZAPL = [120, 230, 255];
  function flashLight(x, y, r, k, col, life) {
    FLASHES.push({ x: x, y: y, r: r, k: k, c: col, life: life, age: 0 });
    if (FLASHES.length > 28) FLASHES.shift();
  }
  function addLite(X, Z, rx, rz, k, col) { LITE.push({ X: X, Z: Z, rx: rx, rz: rz, k: k, r: col[0], g: col[1], b: col[2] }); }
  function buildLights(S, rdt) {
    LITE.length = 0;
    var ts = S ? S.timeScale : 1;
    for (var i = FLASHES.length - 1; i >= 0; i--) {
      var F = FLASHES[i];
      F.age += rdt * Math.max(0.3, ts);
      if (F.age >= F.life) { FLASHES.splice(i, 1); continue; }
      var Zf = zOf(F.y);
      if (Zf < -300 || Zf > DRAW) continue;
      var u = F.age / F.life;
      addLite(F.x, Zf, F.r, F.r * ZK, F.k * (1 - u) * (1 - u), F.c);
    }
    // the chopper's searchlight on the KESTREL (strong at night, a hint by day)
    if (S) for (var hi = 0; hi < S.ents.length; hi++) {
      var he = S.ents[hi];
      if (he.kind !== 'heli' || he.st !== 'hover') continue;
      addLite(S.p.x + Math.sin(TNOW * 1.7 + he.id) * 14, zOf(S.p.y - 8), 46, 80, LV2 ? 1.3 : DUSK ? 0.8 : 0.35, [255, 240, 200]);
    }
    if (DUSK && S) {
      // dusk: the poles' sodium lamps along the shoulders, and the KESTREL's headlights
      var d0 = Math.floor(V.s0 / PSLOT), d1 = Math.floor((V.s0 + 3000) / PSLOT);
      for (var dk = d0; dk <= d1; dk++) { var Zd = dk * PSLOT - V.s0; if (Zd > -250) addLite((dk % 2 === 0 ? -1 : 1) * (EDGE - 4), Zd, 150, 110, 0.55, SODIUM); }
      if (S.phase !== 'wreck' && S.outcome !== 'wrecked') addLite(S.p.x, zOf(S.p.y - S.p.h / 2) + 210, 95, 240, 0.45, HEADL);
      return;
    }
    if (!LV2 || !S) return;
    var k0 = Math.floor(V.s0 / PSLOT), k1 = Math.floor((V.s0 + 3400) / PSLOT);
    for (var k = k0; k <= k1; k++) {
      var Z = k * PSLOT - V.s0;
      if (Z < -250) continue;
      if (T.inPipe(k * PSLOT / ZK)) { addLite(0, Z, 120, 90, 0.85, TUBEL); continue; }
      addLite((k % 2 === 0 ? -1 : 1) * (EDGE - 20), Z, 200, 125, 0.95, SODIUM);
    }
    var p = S.p;
    if (S.phase !== 'wreck' && S.outcome !== 'wrecked') addLite(p.x, zOf(p.y - p.h / 2) + 210, 95, 240, 0.6, HEADL);
  }
  function lightAt(Z, X) {
    LJ.r = LJ.g = LJ.b = 0;
    for (var i = 0; i < LITE.length; i++) {
      var L = LITE[i], dz = (Z - L.Z) / L.rz, dx = (X - L.X) / L.rx, q = dz * dz + dx * dx;
      if (q > 5) continue;
      var k = L.k * Math.exp(-q);
      LJ.r += L.r * k; LJ.g += L.g * k; LJ.b += L.b * k;
    }
  }
  var SL = new Float64Array(18), LX = [-RH - 12, -96, -32, 32, 96, RH + 12];
  // 1.7.2: the segment's light is put into lit()'s cache steps once here (LQ: ambient, fog, then r,g,b per
  // lane), so the dozen colour lookups a segment makes pass small integers instead of boxed doubles
  var LQ = new Int32Array(20);
  function segLight(n) {
    var Z = SB.Z[n];
    LJ.f = fogAt(Z);
    var d = SB.wux[n] * LDIR[0] + SB.wuy[n] * LDIR[1] + SB.wuz[n] * LDIR[2];
    LJ.a = AMB0 + AMB1 * Math.max(0, d);
    LQ[0] = Math.round(LJ.a * 20); LQ[1] = Math.round(LJ.f * 14);
    var any = false;
    if (LITE.length && Z < 3600) {
      for (var j = 0; j < 6; j++) {
        lightAt(Z, LX[j]); SL[j * 3] = LJ.r; SL[j * 3 + 1] = LJ.g; SL[j * 3 + 2] = LJ.b;
        LQ[2 + j * 3] = Math.min(15, LJ.r / 16 | 0); LQ[3 + j * 3] = Math.min(15, LJ.g / 16 | 0); LQ[4 + j * 3] = Math.min(15, LJ.b / 16 | 0);
        if (LJ.r + LJ.g + LJ.b > 10) any = true;
      }
    }
    return any;
  }
  function segCol(col, j) { return j < 0 ? litQ(col, LQ[0], 0, 0, 0, LQ[1]) : litQ(col, LQ[0], LQ[2 + j * 3], LQ[3 + j * 3], LQ[4 + j * 3], LQ[1]); }

  // ---------- glow: soft round lights, added over the frame ----------
  var GLOWC = {};
  function glowImg(col) {
    var g = GLOWC[col];
    if (g) return g;
    var cv2 = document.createElement('canvas'); cv2.width = cv2.height = 64;
    var x = cv2.getContext('2d'), gr = x.createRadialGradient(32, 32, 0, 32, 32, 32);
    gr.addColorStop(0, 'rgba(' + col + ',1)'); gr.addColorStop(0.18, 'rgba(' + col + ',0.7)');
    gr.addColorStop(0.45, 'rgba(' + col + ',0.22)'); gr.addColorStop(1, 'rgba(' + col + ',0)');
    x.fillStyle = gr; x.fillRect(0, 0, 64, 64);
    return (GLOWC[col] = cv2);
  }
  // a glow at x,y of radius r; sy squashes it (a pool on the road). Call inside addOn()/addOff().
  function glow(x, y, r, col, a, sy) {
    if (r < 0.6 || a <= 0.01) return;
    sy = sy || 1;
    c.globalAlpha = Math.min(1, a);
    c.drawImage(glowImg(col), x - r, y - r * sy, r * 2, r * 2 * sy);
  }
  function addOn() { c.save(); c.globalCompositeOperation = 'lighter'; }
  function addOff() { c.restore(); }
  var SHADOWI = null;
  function shadowImg() {
    if (SHADOWI) return SHADOWI;
    var cv2 = document.createElement('canvas'); cv2.width = cv2.height = 64;
    var x = cv2.getContext('2d'), gr = x.createRadialGradient(32, 32, 4, 32, 32, 32);
    gr.addColorStop(0, 'rgba(30,12,60,0.55)'); gr.addColorStop(0.55, 'rgba(30,12,60,0.3)'); gr.addColorStop(1, 'rgba(30,12,60,0)');
    x.fillStyle = gr; x.fillRect(0, 0, 64, 64);
    return (SHADOWI = cv2);
  }

  // ---------- lightning: a jagged line between two screen points, drawn as glow, colour and a hot core ----------
  var BP = [];
  function boltPath(x0, y0, x1, y1, jag, segs) {
    BP.length = 0;
    var dx = x1 - x0, dy = y1 - y0, len = Math.hypot(dx, dy) || 1, nx = -dy / len, ny = dx / len, off = 0;
    for (var i = 0; i <= segs; i++) {
      var u = i / segs;
      off = i === 0 || i === segs ? 0 : off * 0.45 + (Math.random() - 0.5) * jag * 1.6;
      BP.push(x0 + dx * u + nx * off * Math.sin(u * Math.PI), y0 + dy * u + ny * off * Math.sin(u * Math.PI));
    }
  }
  function strokeBolt(w, col, a) {
    if (BP.length < 4 || a <= 0.01) return;
    c.save(); c.globalCompositeOperation = 'lighter'; c.lineJoin = 'round'; c.lineCap = 'round';
    c.beginPath(); c.moveTo(BP[0], BP[1]);
    for (var i = 2; i < BP.length; i += 2) c.lineTo(BP[i], BP[i + 1]);
    c.strokeStyle = 'rgb(' + col + ')';
    c.globalAlpha = 0.22 * a; c.lineWidth = w * 5; c.stroke();
    c.globalAlpha = 0.75 * a; c.lineWidth = w * 1.8; c.stroke();
    c.globalAlpha = Math.min(1, a); c.strokeStyle = '#ffffff'; c.lineWidth = Math.max(0.8, w * 0.6); c.stroke();
    c.restore();
  }
  function arcBetween(x0, y0, x1, y1, w, col, a, jag) {
    boltPath(x0, y0, x1, y1, jag || Math.hypot(x1 - x0, y1 - y0) * 0.25, Math.max(4, Math.min(16, Math.hypot(x1 - x0, y1 - y0) / 9 | 0)));
    strokeBolt(w, col, a);
  }

  // ---------- the Alchemist's electricity on the road ----------
  var HZN = null, HZB = null, COILS = [0, 0, 0, 0], COILOK = false;
  function hazardStrips(n) {
    var y = V.camY - SB.Z[n] / ZK, idx = SB.idx[n];
    for (var i = 0; i < HZN.length; i++) {
      var h = HZN[i];
      if (h.k === 'lane') {
        var top = HZB ? HZB.y + HZB.h / 2 - 10 : -400;
        if (y < top || y > H + 80) continue;
        var u = clamp(h.age / h.warn, 0, 1), live = h.age >= h.warn, fl = Math.sin(TNOW * (16 + u * 30)) * 0.5 + 0.5;
        if (live) { strip(n, h.x - 31, h.x + 31, 'rgba(225,255,255,' + (0.75 * (1 - (h.age - h.warn) / h.live) + 0.15).toFixed(3) + ')'); continue; }
        var late = u > 0.7 && fl > 0.5;
        strip(n, h.x - 31, h.x + 31, late ? 'rgba(255,255,255,0.45)' : 'rgba(255,214,40,' + (0.16 + 0.3 * u).toFixed(3) + ')');
        var ea = ((0.55 + 0.45 * u) * (0.7 + 0.3 * fl)).toFixed(3);
        strip(n, h.x - 33, h.x - 27, 'rgba(95,240,255,' + ea + ')'); strip(n, h.x + 27, h.x + 33, 'rgba(95,240,255,' + ea + ')');
        // chevrons marching down the lane at you
        var ph = (idx + Math.floor(TNOW * 16)) % 8;
        if (ph < 2) { strip(n, h.x - 24 + ph * 8, h.x - 8 + ph * 8, 'rgba(28,31,74,0.85)'); strip(n, h.x + 8 - ph * 8, h.x + 24 - ph * 8, 'rgba(28,31,74,0.85)'); }
      } else if (h.k === 'ring') {
        var y0 = h.age >= h.warn ? h.y : (HZB ? HZB.y + HZB.h / 2 : -300);
        if (y < y0 || y > H + 60) continue;
        var ga = (0.14 + 0.08 * Math.sin(TNOW * 10)).toFixed(3), x0 = h.gx - h.gw / 2, x1 = h.gx + h.gw / 2;
        strip(n, x0, x1, 'rgba(125,255,155,' + ga + ')');
        strip(n, x0, x0 + 3, 'rgba(125,255,155,0.75)'); strip(n, x1 - 3, x1, 'rgba(125,255,155,0.75)');
        if ((idx + Math.floor(TNOW * 10)) % 5 === 0) strip(n, h.gx - 14, h.gx + 14, 'rgba(125,255,155,0.45)');
      }
    }
  }
  // the lock-on reticle on the road, and the strike when it lands
  function drawZone(h) {
    var u = clamp(h.age / h.warn, 0, 1), live = h.age >= h.warn, locked = h.age >= h.track;
    var col = live ? '210,255,255' : locked ? '255,80,62' : '255,214,70', ok = true, i, a;
    c.beginPath();
    for (i = 0; i <= 24; i++) {
      a = i / 24 * 6.283;
      proj(h.x + Math.cos(a) * h.r, 0.6, zOf(h.y + Math.sin(a) * h.r));
      if (PBAD) ok = false;
      if (i) c.lineTo(PJ.x, PJ.y); else c.moveTo(PJ.x, PJ.y);
    }
    if (!ok) return;
    c.fillStyle = 'rgba(' + col + ',' + (live ? 0.45 : 0.08 + 0.22 * u).toFixed(3) + ')'; c.fill();
    c.setLineDash([7, 5]); c.lineDashOffset = -TNOW * 50; c.strokeStyle = 'rgba(' + col + ',0.95)'; c.lineWidth = 2.2; c.stroke(); c.setLineDash([]);
    proj(h.x, 0.6, zOf(h.y)); var cx = PJ.x, cy = PJ.y, s = PJ.s;
    if (!live) {
      // the inner ring closes in as it locks; the crosshair spins
      var rin = h.r * (1 - u * 0.75);
      c.beginPath();
      for (i = 0; i <= 16; i++) { a = i / 16 * 6.283; proj(h.x + Math.cos(a) * rin, 0.6, zOf(h.y + Math.sin(a) * rin)); if (i) c.lineTo(PJ.x, PJ.y); else c.moveTo(PJ.x, PJ.y); }
      c.strokeStyle = 'rgba(' + col + ',0.8)'; c.lineWidth = 1.5; c.stroke();
      c.save(); c.translate(cx, cy); c.rotate(TNOW * (locked ? 6 : 2)); c.strokeStyle = 'rgba(' + col + ',0.9)'; c.lineWidth = 2;
      var L1 = h.r * s * 0.55, L0 = h.r * s * 0.2;
      c.beginPath(); c.moveTo(-L1, 0); c.lineTo(-L0, 0); c.moveTo(L0, 0); c.lineTo(L1, 0); c.moveTo(0, -L1 * 0.5); c.lineTo(0, -L0 * 0.5); c.moveTo(0, L0 * 0.5); c.lineTo(0, L1 * 0.5); c.stroke();
      c.restore();
      if (locked) { addOn(); glow(cx, cy, h.r * s * 1.2, '255,80,62', 0.25 + 0.2 * Math.sin(TNOW * 30), 0.45); addOff(); }
    } else {
      var k = 1 - (h.age - h.warn) / h.live;
      addOn(); glow(cx, cy, h.r * s * 2.6, '150,235,255', 0.9 * k, 0.5); glow(cx, cy - h.r * s, h.r * s * 1.5, '255,255,255', 0.6 * k); addOff();
      for (var b = 0; b < 2; b++) { boltPath(cx + (Math.random() - 0.5) * 80, -10, cx + (Math.random() - 0.5) * 8, cy, 34, 16); strokeBolt(b ? 1.6 : 3, '150,230,255', k); }
    }
  }
  // the shock wave: a crackling fence across the road, with its gap left dark and marked green
  function drawRing(h) {
    var Zr = zOf(h.y), g0 = h.gx - h.gw / 2, g1 = h.gx + h.gw / 2, side;
    for (side = 0; side < 2; side++) {
      var xa = side ? g1 : -EDGE - 8, xb = side ? EDGE + 8 : g0;
      if (xb - xa < 4) continue;
      BP.length = 0;
      for (var x = xa; x <= xb + 0.1; x += Math.max(6, (xb - xa) / 14)) {
        proj(x, 3 + Math.random() * 10, Zr + (Math.random() - 0.5) * 10);
        if (!PBAD) BP.push(PJ.x, PJ.y);
      }
      strokeBolt(2.6, '110,225,255', 1);
      BP.length = 0;
      for (x = xa; x <= xb + 0.1; x += Math.max(8, (xb - xa) / 10)) { proj(x, 1 + Math.random() * 18, Zr); if (!PBAD) BP.push(PJ.x, PJ.y); }
      strokeBolt(1.2, '200,160,255', 0.7);
    }
    // the gap's two green posts
    for (side = 0; side < 2; side++) {
      var gx = side ? g1 : g0;
      proj(gx, 0, Zr); var ax = PJ.x, ay = PJ.y, s = PJ.s, bad = PBAD; proj(gx, 26, Zr);
      if (bad || PBAD) continue;
      c.strokeStyle = '#7dff9b'; c.lineWidth = Math.max(2, 3 * s); c.beginPath(); c.moveTo(ax, ay); c.lineTo(PJ.x, PJ.y); c.stroke();
      addOn(); glow(PJ.x, PJ.y, 10 * s + 4, '125,255,155', 0.8); addOff();
    }
    proj(h.gx, 0, Zr);
    if (!PBAD) { addOn(); glow(PJ.x, PJ.y, h.gw * PJ.s * 0.7, '125,255,155', 0.35, 0.4); addOff(); }
  }
  // the bolt down a lane, from the coil to past the car
  function laneBolts(S) {
    for (var i = 0; i < HZN.length; i++) {
      var h = HZN[i];
      if (h.k !== 'lane' || h.age < h.warn) continue;
      var k = 1 - (h.age - h.warn) / h.live, top = HZB ? HZB.y + HZB.h / 2 : 0, zA = zOf(top), zB = NEAR + 30;
      for (var b = 0; b < 2; b++) {
        BP.length = 0;
        if (COILOK) BP.push(COILS[h.side > 0 ? 2 : 0], COILS[h.side > 0 ? 3 : 1]);
        for (var Zq = zA; Zq > zB; Zq -= 50 + Math.random() * 30) {
          proj(h.x + (Math.random() - 0.5) * 26, 6 + Math.random() * 16, Zq);
          if (!PBAD) BP.push(PJ.x, PJ.y);
        }
        strokeBolt(b ? 1.6 : 3.4, b ? '200,170,255' : '130,230,255', k);
      }
    }
  }
  // the tanker's two tesla coils (drawn here over the Unreal tanker), crackling as they charge
  function drawCoils(e, o) {
    var L = o.L, w = e.w, top = (SPR_TOP.boss || 72) * 0.92, ch = e.charge > 0;
    if (e.charge > (e._cm || 0) + 0.01) e._cm = e.charge;
    if (!ch) e._cm = 0;
    var k = ch && e._cm ? 1 - e.charge / e._cm : 0, rage = e.hp < e.max * 0.33;
    COILOK = false;
    for (var sd = 0; sd < 2; sd++) {
      var X = o.x + (sd ? 1 : -1) * w * 0.3;
      var rot = upAt(X, o.Z + L * 0.22);
      proj(X, top, o.Z + L * 0.22);
      if (PBAD) return;
      var bx = PJ.x, by = PJ.y, s = PJ.s;
      c.save(); c.translate(bx, by); c.rotate(rot); c.scale(s, s);
      c.lineWidth = 1.2 / Math.max(0.3, s) * 0.8; c.strokeStyle = INK;
      for (var r = 0; r < 4; r++) {
        c.fillStyle = r % 2 ? '#c06a2a' : '#e8a050';
        c.beginPath(); c.ellipse(0, -r * 4.2, 6.5 - r * 0.9, 2.2, 0, 0, 6.283); c.fill(); c.stroke();
      }
      c.fillStyle = '#3b3f6e'; c.fillRect(-1.2, -20, 2.4, 6);
      c.fillStyle = k > 0.05 || rage ? '#e6f6ff' : '#9fb0ff'; c.beginPath(); c.arc(0, -22, 4.2, 0, 6.283); c.fill(); c.stroke();
      c.restore();
      var sx = bx + Math.sin(rot) * 22 * s, sy = by - Math.cos(rot) * 22 * s;
      COILS[sd * 2] = sx; COILS[sd * 2 + 1] = sy;
      addOn(); glow(sx, sy, (10 + 26 * k + (rage ? 8 : 0)) * s + 3, '140,220,255', 0.45 + 0.5 * k + 0.1 * Math.sin(TNOW * 40)); addOff();
    }
    COILOK = true;
    var n = Math.random() < 0.35 + k * 0.6 ? 1 + (k > 0.5 ? 2 : 0) + (rage ? 1 : 0) : 0;
    if (n) arcBetween(COILS[0], COILS[1], COILS[2], COILS[3], 1 + k * 1.5, '140,220,255', 0.6 + 0.4 * k);
    for (var j = 1; j < n; j++) {
      var sd2 = Math.random() < 0.5 ? 0 : 2, ang = Math.random() * 6.283, ln = (16 + Math.random() * 40) * (0.5 + k) * Math.max(0.6, PJ.s);
      arcBetween(COILS[sd2], COILS[sd2 + 1], COILS[sd2] + Math.cos(ang) * ln, COILS[sd2 + 1] + Math.sin(ang) * ln * 0.7, 1.1, '190,160,255', 0.7);
    }
    // aiming: while a lane arc charges, a thin feeler reaches from the coil down toward it
    if (e.chargeK === 'lane' && ch && HZN) {
      for (var i = 0; i < HZN.length; i++) {
        var h = HZN[i]; if (h.k !== 'lane' || h.age >= h.warn) continue;
        proj(h.x, 4, o.Z - 120 - 200 * k); if (PBAD) continue;
        var ci = h.side > 0 ? 2 : 0;
        if (Math.random() < 0.5 + k * 0.5) arcBetween(COILS[ci], COILS[ci + 1], PJ.x, PJ.y, 0.8 + k, '255,236,110', 0.4 + 0.5 * k);
      }
    }
  }

  // ---------- 1.2 sky detail: Bayshore's sun rays, sailboats, gulls and the blimp; the plant's storm ----------
  var BOATS = [[-0.7, 0.25, 1, 0], [0.1, 0.55, 0.8, 1], [0.9, 0.15, 1.2, 2], [1.7, 0.4, 0.9, 0], [2.6, 0.7, 1.1, 1], [3.4, 0.3, 0.8, 2], [4.3, 0.5, 1.3, 0], [5.2, 0.2, 0.9, 1]];
  var SAILS = [['#ff4fb8', '#fffaf0'], ['#2ee6ff', '#fffaf0'], ['#ffd23f', '#ff7a3d']];
  function boat(x, y, s, kind, t) {
    var bob = Math.sin(t * 1.6 + x * 0.01) * s * 0.6;
    y += bob;
    c.strokeStyle = INK; c.lineWidth = 1.1;
    c.fillStyle = '#fffaf0';
    c.beginPath(); c.moveTo(x - 9 * s, y - 2 * s); c.lineTo(x + 9 * s, y - 2 * s); c.lineTo(x + 6 * s, y + 1.5 * s); c.lineTo(x - 6 * s, y + 1.5 * s); c.closePath(); c.fill(); c.stroke();
    var sc = SAILS[kind % 3];
    c.fillStyle = sc[0]; c.beginPath(); c.moveTo(x, y - 2.5 * s); c.lineTo(x, y - 17 * s); c.lineTo(x + 7 * s, y - 2.5 * s); c.closePath(); c.fill(); c.stroke();
    c.fillStyle = sc[1]; c.beginPath(); c.moveTo(x - 0.6 * s, y - 3 * s); c.lineTo(x - 0.6 * s, y - 13 * s); c.lineTo(x - 6 * s, y - 3 * s); c.closePath(); c.fill(); c.stroke();
    c.fillStyle = 'rgba(255,255,255,0.6)'; c.fillRect(x - 11 * s, y + 2 * s, 22 * s, 0.8 * s);
  }
  function gull(x, y, s, t, ph) {
    var f = Math.sin(t * 7 + ph) * 0.5;
    c.strokeStyle = INK; c.lineWidth = 1.4; c.beginPath();
    c.moveTo(x - 7 * s, y - (2 + f * 4) * s); c.quadraticCurveTo(x - 3 * s, y - (3 + f * 3) * s, x, y);
    c.quadraticCurveTo(x + 3 * s, y - (3 + f * 3) * s, x + 7 * s, y - (2 + f * 4) * s); c.stroke();
  }
  function blimp(x, y, s) {
    c.strokeStyle = INK; c.lineWidth = 1.3;
    c.fillStyle = '#d9ccf5'; c.beginPath(); c.moveTo(x + 24 * s, y - 6 * s); c.lineTo(x + 32 * s, y - 13 * s); c.lineTo(x + 32 * s, y + 1 * s); c.closePath(); c.fill(); c.stroke();
    c.fillStyle = '#fffaf0'; c.beginPath(); c.ellipse(x, y - 6 * s, 28 * s, 9 * s, 0, 0, 6.283); c.fill(); c.stroke();
    c.fillStyle = '#ff4fb8'; c.fillRect(x - 20 * s, y - 7.5 * s, 40 * s, 3 * s);
    c.fillStyle = '#2b2350'; c.fillRect(x - 5 * s, y + 2 * s, 10 * s, 3 * s);
    c.fillStyle = '#2b2350'; c.font = font(Math.max(5, 5.2 * s), 900); c.textAlign = 'center'; c.textBaseline = 'middle';
    c.fillText('BAYSHORE', x, y - 2.6 * s);
  }
  // after the sun is drawn: rays, then (in the sea) a glitter path under it
  function sunRays(sx, sy, sr, t) {
    addOn();
    glow(sx, sy, sr * 4.2, '255,236,170', 0.45);
    c.globalAlpha = 0.07; c.fillStyle = '#fff6d0';
    for (var i = 0; i < 12; i++) {
      var a = t * 0.05 + i * 0.5236, a2 = a + 0.09;
      c.beginPath(); c.moveTo(sx, sy); c.lineTo(sx + Math.cos(a) * sr * 9, sy + Math.sin(a) * sr * 9); c.lineTo(sx + Math.cos(a2) * sr * 9, sy + Math.sin(a2) * sr * 9); c.closePath(); c.fill();
    }
    addOff();
  }
  function glitter(sx, t, R, col) {
    c.fillStyle = col;
    for (var i = 0; i < 46; i++) {
      var u = ((i * 0.618034) % 1), wy = 6 + u * u * 150, spread = 14 + wy * 0.9;
      var wx = sx + (((i * 0.7548777 + t * 0.12 * (1 + i % 3)) % 1) - 0.5) * spread * 2;
      var fl = Math.sin(t * 6 + i * 1.7);
      if (fl < 0) continue;
      c.globalAlpha = 0.35 + 0.6 * fl;
      c.fillRect(wx - (3 + wy * 0.06), wy, 6 + wy * 0.12, 1.2 + wy * 0.012);
    }
    c.globalAlpha = 1;
  }
  // the plant's sky: a storm bank with lightning in it now and then, and searchlights sweeping
  var STORM = [[-1.0, 1.4], [-0.2, 1.1], [0.6, 1.6], [1.5, 1.2], [2.3, 1.5], [3.1, 1.0], [3.9, 1.4], [4.8, 1.2], [5.6, 1.5]];
  function stormClouds(heading, PW, R, t, lit) {
    for (var i = 0; i < STORM.length; i++) {
      var sd = STORM[i], x = ((((sd[0] - heading * 0.9 + t * 0.003) % 6.283) + 6.283) % 6.283 - 3.14) * PW * 0.5;
      if (Math.abs(x) > R) continue;
      var r = viewH * 0.06 * sd[1], y = -viewH * 0.5;
      c.fillStyle = '#1a1446'; c.strokeStyle = lit > 0 ? 'rgba(190,220,255,' + (0.4 + lit * 0.6) + ')' : '#3a2f7a'; c.lineWidth = 1.6;
      c.beginPath();
      c.arc(x - r * 1.2, y, r * 0.7, Math.PI * 0.5, Math.PI * 1.5); c.arc(x - r * 0.4, y - r * 0.5, r * 0.8, Math.PI, Math.PI * 1.9);
      c.arc(x + r * 0.5, y - r * 0.55, r * 0.9, Math.PI * 1.1, Math.PI * 1.95); c.arc(x + r * 1.3, y, r * 0.7, Math.PI * 1.5, Math.PI * 0.5);
      c.closePath(); c.fill(); c.stroke();
    }
  }
  function searchlights(R, t) {
    addOn();
    for (var i = 0; i < 3; i++) {
      var bx = (i - 1) * R * 0.55 + Math.sin(i * 7.1) * 60, a = Math.sin(t * (0.25 + i * 0.07) + i * 2) * 0.55;
      var len = viewH * 1.1, wdt = 0.05, g = c.createLinearGradient(bx, 0, bx + Math.sin(a) * len, -Math.cos(a) * len);
      g.addColorStop(0, 'rgba(180,200,255,0.22)'); g.addColorStop(1, 'rgba(180,200,255,0)');
      c.fillStyle = g; c.globalAlpha = 1;
      c.beginPath(); c.moveTo(bx - 3, -2); c.lineTo(bx + Math.sin(a - wdt) * len, -Math.cos(a - wdt) * len); c.lineTo(bx + Math.sin(a + wdt) * len, -Math.cos(a + wdt) * len); c.lineTo(bx + 3, -2); c.closePath(); c.fill();
    }
    addOff();
  }

  // ---------- 1.2 roadside detail drawn here in the same cel style: sign gantries over the road ----------
  // Bayshore gets green highway signs and neon; the plant gets hazard-striped pipe bridges with beacons.
  var GANT = {}, GTEXT1 = ['BAYSHORE  ↑', 'PIER  2 MI', 'SUNSET STRIP', 'ROUTE 9 NORTH', 'MARINA  EXIT 4', 'NEON MILE', 'LIGHTHOUSE PT', 'CAUSEWAY'];
  var GTEXT3 = ['LAS VEGAS  38', 'US 93  NORTH', 'LAST GAS  50 MI', 'JACKPOT JUNCTION', 'LOOSEST SLOTS IN NEVADA', 'LAS VEGAS  12', 'WELCOME TO NEVADA', 'THE STRIP  EXIT 7'];
  var GTEXT2 = ['VOLTA CHEMICAL WORKS', 'DANGER  ⚡  HIGH VOLTAGE', 'REACTOR  ↑', 'NO SMOKING', 'TANK FARM  B', 'AUTHORISED ONLY'];
  function gantryAt(k) {
    if (GANT[k] !== undefined) return GANT[k];
    var g = null;
    if (k % 4 === 2 && hash(k * 13 + 7) < 0.55) {
      var s = k * PSLOT + PSLOT * 0.5, d = s / ZK, ok = !T.inPipe(d) && !T.gapAt(d), F = T.FEATS;
      for (var i = 0; ok && i < F.length; i++) if (d > F[i].d0 - 500 && d < F[i].d1 + 400) ok = false;
      if (ok) { trackAt(s); if (TP.uy < 0.97) ok = false; }
      if (ok) g = { s: s, text: LV2 ? GTEXT2[k % GTEXT2.length] : DUSK ? GTEXT3[k % GTEXT3.length] : GTEXT1[k % GTEXT1.length], neon: !LV2 && hash(k * 3 + 1) < 0.4 };
    }
    return (GANT[k] = g);
  }
  function drawGantry(g, t) {
    var Z = g.s - V.s0, o = { x: 0, Z: Z, L: 8, yaw: 0, lift: 0, fl: false };
    var a = fadeFor(Z);
    if (a <= 0.01) return;
    c.globalAlpha = a;
    var postC = LV2 ? '#4a5ca8' : '#8f86c9', beamC = LV2 ? '#ffd23f' : '#8f86c9';
    box(o, -EDGE - 18, -EDGE - 11, 0, 150, 1, 7, postC); box(o, EDGE + 11, EDGE + 18, 0, 150, 1, 7, postC);
    box(o, -EDGE - 18, EDGE + 18, 142, 150, 0, 8, beamC);
    if (LV2) {
      // hazard stripes on the beam, and a fat pipe along it
      for (var sx = -EDGE - 12; sx < EDGE + 12; sx += 24) back(clamp((sx + EDGE + 18) / (EDGE * 2 + 36), 0, 1), clamp((sx + 12 + EDGE + 18) / (EDGE * 2 + 36), 0, 1), 0, 1, '#1c1f4a');
      box(o, -EDGE - 18, EDGE + 18, 150, 156, 2, 7, '#5a6fc0');
    }
    var pw = LV2 ? 92 : 78, py0 = LV2 ? 118 : 114, py1 = 141;
    box(o, -pw, pw, py0, py1, -1, 1, LV2 ? '#1c1f4a' : g.neon ? '#2b2350' : '#1f8a5a');
    proj(-pw, py0, Z - 1); var x0 = PJ.x, y0 = PJ.y, bad = PBAD; proj(pw, py1, Z - 1); var x1 = PJ.x, y1 = PJ.y;
    if (!bad && !PBAD && Math.abs(x1 - x0) > 14) {
      var cx = (x0 + x1) / 2, cy = (y0 + y1) / 2, ww = Math.abs(x1 - x0), hh = Math.abs(y1 - y0), rot = Math.atan2(y1 - y0, x1 - x0) + Math.atan2(hh, ww);
      c.save(); c.translate(cx, cy);
      c.strokeStyle = LV2 ? '#ffd23f' : g.neon ? '#ff4fb8' : '#fffaf0'; c.lineWidth = Math.max(1, hh * 0.05); c.strokeRect(-ww / 2 + hh * 0.08, -hh / 2 + hh * 0.08, ww - hh * 0.16, hh - hh * 0.16);
      c.font = font(Math.max(4, hh * 0.42), 900); c.textAlign = 'center'; c.textBaseline = 'middle';
      c.fillStyle = LV2 ? '#ffd23f' : g.neon ? '#ff7ad0' : '#fffaf0';
      c.fillText(g.text, 0, hh * 0.03, ww * 0.88);
      if (g.neon || LV2) { addOn(); c.globalAlpha = a * 0.5; c.fillStyle = LV2 ? '#ffd23f' : '#ff4fb8'; c.fillText(g.text, 0, hh * 0.03, ww * 0.88); addOff(); }
      c.restore();
    }
    if (LV2) {
      // beacons at each end, blinking out of step
      for (var sd = -1; sd <= 1; sd += 2) {
        proj(sd * (EDGE + 14), 160, Z + 4);
        if (PBAD) continue;
        var on = Math.sin(t * 5 + sd * 1.7 + g.s) > 0.2;
        c.fillStyle = on ? '#ff5a4e' : '#5a1a2a'; c.beginPath(); c.arc(PJ.x, PJ.y, Math.max(1.5, 3 * PJ.s), 0, 6.283); c.fill();
        if (on) { addOn(); glow(PJ.x, PJ.y, 26 * PJ.s + 6, '255,70,60', 0.75); addOff(); }
      }
    }
    c.globalAlpha = 1;
  }

  // ---------- 1.2 post-processing: bloom, a zoom blur at speed, the grade, vignette, grain, flares ----------
  var PB = null, PBX = null, VIG = null, VIGR = null, VIGC = null, GRN = null, POSTKEY = '';
  // 1.7.2: the bloom filter string is set only when it changes, and the grain pattern and the grade's
  // gradient are made once (per size and level) instead of every frame
  var PBXF = '', GRNP = null, GRADE = null, GRADEK = '';
  function vignette(col, a) {
    var v = document.createElement('canvas'); v.width = 256; v.height = 160;
    var x = v.getContext('2d'), g = x.createRadialGradient(128, 80, 30, 128, 80, 152);
    g.addColorStop(0, 'rgba(' + col + ',0)'); g.addColorStop(0.6, 'rgba(' + col + ',' + (a * 0.22) + ')'); g.addColorStop(1, 'rgba(' + col + ',' + a + ')');
    x.fillStyle = g; x.fillRect(0, 0, 256, 160);
    return v;
  }
  function postSetup() {
    var key = cv.width + 'x' + cv.height;
    if (POSTKEY === key) return;
    POSTKEY = key;
    if (!PB) { PB = document.createElement('canvas'); PBX = PB.getContext('2d'); }
    PB.width = Math.max(64, cv.width >> 2); PB.height = Math.max(40, cv.height >> 2); PBXF = ''; // a resize resets the context
    if (!VIG) {
      VIG = vignette('10,0,30', 0.62); VIGR = vignette('255,30,60', 0.9); VIGC = vignette('90,230,255', 0.9);
      GRN = document.createElement('canvas'); GRN.width = GRN.height = 128;
      var gx = GRN.getContext('2d'), id = gx.createImageData(128, 128);
      for (var i = 0; i < id.data.length; i += 4) { var v = Math.random() * 255 | 0; id.data[i] = id.data[i + 1] = id.data[i + 2] = v; id.data[i + 3] = 255; }
      gx.putImageData(id, 0, 0);
    }
  }
  function postFX(S, ui, t) {
    postSetup();
    var W2 = cv.width, H2 = cv.height, k = dpr * scale;
    var hot = S && S.p ? clamp((S.speed - 480) / 260, 0, 1) : 0;
    c.save(); c.setTransform(1, 0, 0, 1, 0, 0);
    if (!LOWFX) {
      var bf = LV2 ? 'brightness(0.6) contrast(2.5) saturate(1.7) blur(3px)' : DUSK ? 'brightness(0.56) contrast(2.6) saturate(1.7) blur(3px)' : 'brightness(0.52) contrast(2.7) saturate(1.5) blur(3px)';
      if (PBXF !== bf) { PBX.globalCompositeOperation = 'copy'; PBX.filter = bf; PBXF = bf; }
      PBX.drawImage(cv, 0, 0, PB.width, PB.height);
      c.globalCompositeOperation = 'screen';
      c.globalAlpha = LV2 ? 0.85 : DUSK ? 0.62 : 0.38;
      c.drawImage(PB, 0, 0, W2, H2);
      if (hot > 0.01 && !REDUCED) {
        var zx = V.cx * k, zy = V.hY * k;
        for (var z = 1; z <= 2; z++) {
          var zk = 1 + (0.035 + 0.03 * hot) * z;
          c.globalAlpha = 0.28 * hot / z;
          c.drawImage(PB, zx - zx * zk, zy - zy * zk, W2 * zk, H2 * zk);
        }
      }
    }
    // the grade
    c.globalCompositeOperation = 'soft-light'; c.globalAlpha = 1;
    var gk = H2 + (LV2 ? 'c' : DUSK ? 'd' : 'b'), g = GRADE;
    if (GRADEK !== gk) {
      g = GRADE = c.createLinearGradient(0, 0, 0, H2); GRADEK = gk;
      if (LV2) { g.addColorStop(0, 'rgba(120,60,255,0.6)'); g.addColorStop(0.5, 'rgba(255,80,200,0.25)'); g.addColorStop(1, 'rgba(40,220,255,0.4)'); }
      else if (DUSK) { g.addColorStop(0, 'rgba(150,60,230,0.5)'); g.addColorStop(0.45, 'rgba(255,110,150,0.28)'); g.addColorStop(1, 'rgba(255,160,70,0.38)'); }
      else { g.addColorStop(0, 'rgba(255,130,200,0.5)'); g.addColorStop(0.45, 'rgba(255,225,170,0.25)'); g.addColorStop(1, 'rgba(60,160,255,0.4)'); }
    }
    c.fillStyle = g; c.fillRect(0, 0, W2, H2);
    c.globalCompositeOperation = 'source-over';
    c.globalAlpha = LV2 ? 0.95 : DUSK ? 0.7 : 0.42; c.drawImage(VIG, 0, 0, W2, H2);
    if (S && S.p) {
      var hurt = Math.max(FX.hurt, S.p.hp < 30 && S.phase === 'play' && !REELON ? 0.35 + 0.25 * Math.sin(t * 6) : 0);
      if (hurt > 0.01) { c.globalAlpha = clamp(hurt, 0, 1) * 0.75; c.drawImage(VIGR, 0, 0, W2, H2); }
      if (FX.shock > 0.01) { c.globalAlpha = clamp(FX.shock, 0, 1) * (0.55 + 0.45 * Math.random()); c.drawImage(VIGC, 0, 0, W2, H2); }
      if (FX.boom > 0.01 && !REDUCED) { c.globalCompositeOperation = 'lighter'; c.globalAlpha = FX.boom * 0.12; c.fillStyle = '#ff9a40'; c.fillRect(0, 0, W2, H2); c.globalCompositeOperation = 'source-over'; }
      if (FX.flash > 0.01 && !REDUCED) { c.globalCompositeOperation = 'lighter'; c.globalAlpha = FX.flash * 0.22; c.fillStyle = '#9fd8ff'; c.fillRect(0, 0, W2, H2); c.globalCompositeOperation = 'source-over'; }
    }
    if (!REDUCED && !LOWFX) {
      c.globalCompositeOperation = 'overlay'; c.globalAlpha = LV2 ? 0.09 : DUSK ? 0.075 : 0.06;
      c.fillStyle = GRNP || (GRNP = c.createPattern(GRN, 'repeat'));
      var ox = Math.random() * 128 | 0, oy = Math.random() * 128 | 0;
      c.translate(-ox, -oy); c.fillRect(0, 0, W2 + 128, H2 + 128);
    }
    c.restore();
    c.globalAlpha = 1; c.globalCompositeOperation = 'source-over';
    // speed lines out of the vanishing point
    if (hot > 0.05 && !REDUCED) {
      addOn();
      var dg = Math.hypot(viewW, viewH);
      c.strokeStyle = '#ffffff'; c.lineWidth = 1.4;
      for (var i = 0; i < 28; i++) {
        var a = (i * 2.399 + Math.floor(t * 30) * 0.7) % 6.283, r0 = dg * (0.32 + ((i * 0.37 + t * 3.1) % 1) * 0.25), r1 = r0 + dg * (0.08 + 0.1 * hot);
        c.globalAlpha = 0.16 * hot;
        c.beginPath(); c.moveTo(V.cx + Math.cos(a) * r0, V.hY + Math.sin(a) * r0 * 0.8); c.lineTo(V.cx + Math.cos(a) * r1, V.hY + Math.sin(a) * r1 * 0.8); c.stroke();
      }
      addOff();
    }
    // the sun's lens flare: ghosts on the line through the middle of the screen
    if (FX.sun && !LV2) {
      var sx = FX.sun.x, sy = FX.sun.y, mx = viewW / 2, my = viewH / 2, vis = clamp(1 - Math.hypot(sx - mx, sy - my) / (viewW * 0.9), 0, 1);
      if (vis > 0.02 && sy > -viewH * 0.4 && sy < viewH) {
        addOn();
        var GH = [[0.35, 18, '255,200,120', 0.2], [0.62, 9, '120,220,255', 0.3], [0.95, 26, '255,120,200', 0.12], [1.3, 14, '255,240,160', 0.22], [1.7, 40, '140,160,255', 0.08]];
        for (var j = 0; j < GH.length; j++) {
          var q = GH[j], fx = sx + (mx - sx) * q[0] * 2, fy = sy + (my - sy) * q[0] * 2;
          glow(fx, fy, q[1] * viewH / 600 * 1.4, q[2], q[3] * vis);
        }
        addOff();
      }
    }
  }

  // ---------- Bayshore: sky, sea, skyline (the art pass, 1.0) ----------
  var INK = '#1b1033';
  var FLAT = { road: '#6e6a8c', road2: '#66628a', line: '#fffaf0', kerbA: '#ff4fb8', kerbB: '#fffaf0', rail: '#2ee6ff',
    post: '#2b2350', warn: '#ffd23f', warnDark: '#2b2350', kick: '#ff4fb8', deck: '#d9ccf5', deckSide: '#a796d6',
    pillar: '#f3e8ff', sand: '#ffe2a8', wet: '#f4cf8e', prom: '#ffc1dc', lawn: '#8fe8c4',
    sea: '#25bfc4', seaFar: '#7fe0dc', sky0: '#5fb8ff', sky1: '#a9dcff', sky2: '#ffe0ef' };
  var BAYF = FLAT;
  // level 2, the Volta Chemical Works at night: blue steel deck, hazard kerbs, sodium rails, a violet lake
  var CHEMF = { road: '#3b4a8a', road2: '#34437e', line: '#ffd23f', kerbA: '#ffd23f', kerbB: '#1c1f4a', rail: '#ff9e3d',
    post: '#1c1f4a', warn: '#ff4fb8', warnDark: '#1c1f4a', kick: '#e84aff', deck: '#8fa3e0', deckSide: '#4a5ca8',
    pillar: '#7f93d6', sand: '#2b3466', wet: '#5a2a9a', prom: '#24305c', lawn: '#2b3466',
    sea: '#8a2fd6', seaFar: '#d77bff', sky0: '#0d0b2e', sky1: '#2a2470', sky2: '#c25bd6',
    glassA: '#5ff0ff', glassB: '#19b5c9', rib: '#2a3f8f', ribLite: '#ffd23f' };
  // 1.5, level 3, the Nevada desert at dusk: dusky asphalt, sunset-orange kerbs, neon rails, red sand
  var DESF = { road: '#4a3f63', road2: '#443a5c', line: '#ffd23f', kerbA: '#ff7a3d', kerbB: '#fff1d6', rail: '#2ee6ff',
    post: '#2b2350', warn: '#ffd23f', warnDark: '#2b2350', kick: '#ff4fb8', deck: '#e8b08a', deckSide: '#b8704a',
    pillar: '#f2c58a', sand: '#e89a68', wet: '#d9845a', prom: '#e89a68', lawn: '#d9845a',
    sea: '#d98a5e', seaFar: '#a2507a', sky0: '#1d1450', sky1: '#7a2f9a', sky2: '#ff9a5a' };
  // the art made in Unreal: one strip of frames per thing, the frame picked by the angle it is seen at
  var SP = window.FT_SPRITES || {}, IMG = {}, WHT = {}, CMU = 9.14; // centimetres per game unit
  var ART_BAD = {}, ART_T0 = performance.now();
  // 1.7.2: ORIG holds each sheet's <img>; PIN its decoded ImageBitmap while the current level uses it (see warm)
  var ORIG = {}, PIN = {}, WANT = {}, PINQ = [];
  (function loadArt() {
    // decoded up front (1.4), so the first explosion or wheel frame drawn doesn't stall on a decode
    for (var k in SP) (function (k) {
      var im = new Image();
      im.onload = function () { ORIG[k] = im; if (im.decode) im.decode().then(function () { IMG[k] = IMG[k] || im; }, function () { IMG[k] = IMG[k] || im; }); else IMG[k] = im; };
      im.onerror = function () { ART_BAD[k] = 1; }; // 1.7.2: a sheet that can't load never holds up the title
      im.src = (window.FT_ART_BASE || '') + SP[k].src;
    })(k);
  })();
  function whiteOf(k) { // the strip as a white silhouette, for hit flashes
    if (WHT[k] || !IMG[k]) return WHT[k];
    var im = IMG[k], cv2 = document.createElement('canvas'); cv2.width = im.width; cv2.height = im.height;
    var g = cv2.getContext('2d'); g.drawImage(im, 0, 0); g.globalCompositeOperation = 'source-in'; g.fillStyle = '#fff'; g.fillRect(0, 0, im.width, im.height);
    return (WHT[k] = cv2);
  }
  // 1.3: the strip as soot, scorch and bare-metal scratches only, laid over a damaged vehicle
  var DMG = {};
  function dmgOf(k) {
    if (DMG[k] || !IMG[k]) return DMG[k];
    var im = IMG[k], cv2 = document.createElement('canvas'); cv2.width = im.width; cv2.height = im.height;
    var g = cv2.getContext('2d'), area = im.width * im.height, i, x, y, r;
    for (i = 0; i < area / 700; i++) {
      x = Math.random() * im.width; y = Math.random() * im.height; r = 3 + Math.random() * 14;
      g.fillStyle = 'rgba(28,16,32,' + (0.35 + Math.random() * 0.45) + ')'; g.beginPath(); g.ellipse(x, y, r * 1.5, r, Math.random() * 3, 0, 6.283); g.fill();
    }
    g.lineCap = 'round';
    for (i = 0; i < area / 1800; i++) {
      x = Math.random() * im.width; y = Math.random() * im.height; var a = (Math.random() - 0.5) * 0.7, l = 8 + Math.random() * 18;
      g.strokeStyle = 'rgba(225,230,245,0.8)'; g.lineWidth = 1 + Math.random(); g.beginPath(); g.moveTo(x, y); g.lineTo(x + Math.cos(a) * l, y + Math.sin(a) * l); g.stroke();
    }
    g.globalCompositeOperation = 'destination-in'; g.drawImage(im, 0, 0);
    return (DMG[k] = cv2);
  }
  // sprite k, the frame nearest psi (degrees, + = nose to the right), anchored at x,y; sz = screen px per game unit
  function fogOf(k) { // the strip as a silhouette in the haze colour, laid over far props
    if (FOGI[k] || !IMG[k]) return FOGI[k];
    var im = IMG[k], cv2 = document.createElement('canvas'); cv2.width = im.width; cv2.height = im.height;
    var g = cv2.getContext('2d'); g.drawImage(im, 0, 0); g.globalCompositeOperation = 'source-in'; g.fillStyle = 'rgb(' + FOGC.join(',') + ')'; g.fillRect(0, 0, im.width, im.height);
    return (FOGI[k] = cv2);
  }
  function sprite(k, psi, x, y, sz, rot, fl, mul, sqy) {
    var s = SP[k], im = fl === 3 ? dmgOf(k) : fl === 2 ? fogOf(k) : fl ? whiteOf(k) : IMG[k];
    if (!s || !im || !s.frames) return false;
    var fr = s.frames, best = fr[0];
    for (var i = 1; i < fr.length; i++) if (Math.abs(fr[i].psi - psi) < Math.abs(best.psi - psi)) best = fr[i];
    var sc = sz * s.cmPerPx / CMU * (mul || 1);
    if (sc * s.fh < 0.6) return true;
    // 1.7.2: nothing is drawn wholly off screen (a fifth of all sprite draws were: props round a bend, the
    // far side of a loop). The reach is generous: any point of the frame, from an anchor anywhere, plus 40 px
    var reach = (s.fw + s.fh + Math.abs(best.ax) + Math.abs(best.ay)) * sc * Math.max(1, sqy || 1) + 40;
    if (x + reach < 0 || x - reach > viewW || y + reach < 0 || y - reach > viewH) return true;
    c.save(); c.translate(x, y); if (rot) c.rotate(rot); c.scale(sc, sc * (sqy || 1));
    c.drawImage(im, best.x, 0, s.fw, s.fh, -best.ax, -best.ay, s.fw, s.fh);
    c.restore();
    return true;
  }
  function frameNear(k, psi) {
    var fr = SP[k] && SP[k].frames, best = fr && fr[0];
    if (!fr) return null;
    for (var i = 1; i < fr.length; i++) if (Math.abs(fr[i].psi - psi) < Math.abs(best.psi - psi)) best = fr[i];
    return best;
  }

  // ======================= 1.4: cars in parts, on springs =======================
  // The KESTREL, the rammer and the gunship SUV are no longer one baked picture. Unreal films the body
  // with holes cut where a wheel stands in front of it, and every wheel on its own, in 8 spin frames at
  // every angle (tread blocks and a five-hole rim, so the turning shows from behind). They are put back
  // together here: wheels underneath, turning with the road speed (the KESTREL's fronts steer, and
  // countersteer in a drift), and the body on top riding a light suspension: it bobs over the road's
  // bumps, squats when you floor it, dives when you brake, leans out of corners and drifts, and
  // compresses and rebounds on landings and hits. A right wheel is a left wheel at -psi, mirrored.
  var WHEEL_OF = { player: ['wheel_kr', 'wheel_kf'], player_d1: ['wheel_kr', 'wheel_kf'], player_d2: ['wheel_kr', 'wheel_kf'],
    rammer: ['wheel_rm', 'wheel_rm'], rammer_rev: ['wheel_rm', 'wheel_rm'], suv: ['wheel_suv', 'wheel_suv'] };
  var WROWS = {}, HJ = { x: 0.5, y: 0.5 }, RDT = 1 / 60, PSUS = null;
  function wheelRows(k) {
    if (WROWS[k]) return WROWS[k];
    var s = SP[k]; if (!s || !s.frames) return null;
    var rows = [];
    for (var i = 0; i < s.frames.length; i++) if (s.frames[i].c === 0) rows.push(s.frames[i]);
    rows.sort(function (a, b) { return a.psi - b.psi; });
    return (WROWS[k] = rows);
  }
  function partsReady(k) { var w = WHEEL_OF[k]; return !!(w && IMG[k] && IMG[w[0]] && IMG[w[1]] && SP[k].frames[0].wh); }
  // wheel sheet k with its hub at x,y: psi its heading, spin its turn in frames (blended between the two
  // nearest, so it turns smoothly), sc screen px per sheet px; mirror for a right-hand wheel
  function wheelAt(k, psi, spin, x, y, sc, rot, mirror, fl) {
    var s = SP[k], im = fl ? whiteOf(k) : IMG[k], rows = wheelRows(k);
    if (!s || !im || !rows) return;
    var want = mirror ? -psi : psi, best = rows[0];
    for (var i = 1; i < rows.length; i++) if (Math.abs(rows[i].psi - want) < Math.abs(best.psi - want)) best = rows[i];
    var n = s.cols, f = ((spin % n) + n) % n, i0 = Math.floor(f), i1 = (i0 + 1) % n, fr = f - i0;
    c.save(); c.translate(x, y); if (rot) c.rotate(rot); c.scale(mirror ? -sc : sc, sc);
    var a0 = c.globalAlpha;
    c.drawImage(im, i0 * s.fw, best.r * s.fh, s.fw, s.fh, -best.ax, -best.ay, s.fw, s.fh);
    if (fr > 0.04) { c.globalAlpha = a0 * fr; c.drawImage(im, i1 * s.fw, best.r * s.fh, s.fw, s.fh, -best.ax, -best.ay, s.fw, s.fh); c.globalAlpha = a0; }
    c.restore();
  }
  // where wheel i's hub sits on screen for body frame fr anchored at x,y (into HX, HY)
  function hubAt(fr, i, x, y, sc, rot) {
    var dx = (fr.wh[i][0] - fr.ax) * sc, dy = (fr.wh[i][1] - fr.ay) * sc, cs = Math.cos(rot), sn = Math.sin(rot);
    HJ.x = x + dx * cs - dy * sn; HJ.y = y + dx * sn + dy * cs;
  }
  // the road's own little bumps, in game units (a few centimetres), smooth, with now and then a seam
  function bumpAt(s) {
    var u = s / 23, i = Math.floor(u), f = u - i, a = hash(i * 7 + 3), b = hash(i * 7 + 10);
    f = f * f * (3 - 2 * f);
    var v = (a + (b - a) * f - 0.5) * 1.15, j = Math.floor(s / 150), g = (s - j * 150) / 18;
    if (hash(j * 13 + 5) > 0.68 && g < 1) v += Math.sin(g * Math.PI) * (1.2 + 0.9 * hash(j * 13 + 6));
    return v;
  }
  function newSus() { return { h: 0, vh: 0, r: 0, vr: 0, q: 0, vq: 0, w: [0, 0, 0, 0], spin: Math.random() * 8, spd: -1, acc: 0 }; }
  PSUS = newSus();
  // a jolt to a car's springs: dh heave (units/s, - = slammed down), dr roll (radians/s)
  function jolt(u, dh, dr) { if (u) { u.vh += dh; u.vr += dr; } }
  function spring(x, v, target, om, ze, dt) { return v + (om * om * (target - x) - 2 * ze * om * v) * dt; }
  // one step of a car's springs. s: where it is along the road; spd: road speed; latT: lean target from
  // cornering (radians); air: off the ground; brake: braking hard
  function stepSus(u, dt, s, spd, latT, air, brake, wb) {
    if (dt <= 0) return;
    var n = Math.max(1, Math.ceil(dt / (1 / 120))), h = Math.min(dt, 0.1) / n;
    if (u.spd < 0) u.spd = spd;
    var accNow = (spd - u.spd) / dt; u.spd = spd;
    u.acc += (clamp(accNow, -2400, 2400) - u.acc) * Math.min(1, dt * 8);
    var bk = 0.55 + clamp(spd / 700, 0, 1) * 0.6, i;
    var tw = [bumpAt(s) * bk, bumpAt(s + 6.7) * bk, bumpAt(s + wb) * bk, bumpAt(s + wb + 6.7) * bk];
    for (i = 0; i < 4; i++) u.w[i] += ((air ? -1.25 : tw[i]) - u.w[i]) * Math.min(1, dt * (air ? 7 : 40));
    // the body follows the wheels through its springs: heave from all four, roll from left against
    // right plus the lean out of the turn, pitch from front against back plus squat and dive
    var hT = air ? 0.3 : (u.w[0] + u.w[1] + u.w[2] + u.w[3]) * 0.21;
    var rT = latT + (air ? 0 : (u.w[0] + u.w[2] - u.w[1] - u.w[3]) * 0.008);
    var qT = clamp(u.acc / 800, -1, 1.2) - (brake ? 0.8 : 0) + (air ? 0 : ((u.w[0] + u.w[1]) - (u.w[2] + u.w[3])) * 0.15);
    for (var k = 0; k < n; k++) {
      u.vh = spring(u.h, u.vh, hT, 15, 0.32, h); u.h += u.vh * h;
      u.vr = spring(u.r, u.vr, rT, 12, 0.36, h); u.r += u.vr * h;
      u.vq = spring(u.q, u.vq, qT, 11, 0.4, h); u.q += u.vq * h;
    }
    u.h = clamp(u.h, -2.4, 1.8); u.r = clamp(u.r, -0.1, 0.1); u.q = clamp(u.q, -1.6, 1.6);
  }
  // a car in parts at road anchor x,y (sz screen px per game unit, rot = the road's up on screen).
  // spin: wheel turn in frames; steer: front wheel angle (degrees); wob: a bent wheel's wobble
  var BODY = { x: 0, y: 0, rot: 0 };
  function carParts(k, psi, x, y, sz, rot, u, steer, fl, sqy, wob) {
    var fr = frameNear(k, psi), wk = WHEEL_OF[k], s = SP[k], sc = sz * s.cmPerPx / CMU;
    var upx = Math.sin(rot), upy = -Math.cos(rot), i;
    var sR = SP[wk[0]], sF = SP[wk[1]], scR = sz * sR.cmPerPx / CMU, scF = sz * sF.cmPerPx / CMU;
    // the wheels, the far (front) pair first; each rides up and down on its own travel
    var order = [2, 3, 0, 1];
    for (var j = 0; j < 4; j++) {
      i = order[j];
      hubAt(fr, i, x, y, sc, rot);
      var tr = u.w[i] * sz, front = i >= 2, right = i % 2 === 1;
      var wr = rot + (wob && i === 1 ? Math.sin(u.spin * 0.785) * wob : 0);
      wheelAt(front ? wk[1] : wk[0], psi + (front ? steer : 0), front ? u.spin * (sR.fh / Math.max(1, sF.fh)) : u.spin,
        HJ.x + upx * tr, HJ.y + upy * tr, front ? scF : scR, wr, right, fl);
    }
    // the body on its springs: heave, plus the rear dropping in a squat (and rising in a dive), and roll
    var lift = (u.h - u.q * 0.75) * sz;
    BODY.x = x + upx * lift; BODY.y = y + upy * lift; BODY.rot = rot + u.r;
    sprite(k, psi, BODY.x, BODY.y, sz, BODY.rot, fl, 1, (sqy || 1) * (1 + u.q * 0.02));
    return fr;
  }

  var SUN_HEADING = -0.35, CLOUDS = [[-0.9, 0.62, 1.2], [-0.35, 0.78, 0.8], [0.25, 0.55, 1.0], [0.8, 0.7, 1.4], [1.5, 0.6, 0.9], [2.3, 0.74, 1.1], [3.0, 0.58, 0.8], [3.8, 0.68, 1.3], [4.6, 0.6, 1.0], [5.4, 0.72, 0.9]];
  function cloud(x, y, r) {
    c.fillStyle = '#ffffff'; c.strokeStyle = INK; c.lineWidth = 1.4;
    c.beginPath();
    c.arc(x - r * 1.1, y, r * 0.62, Math.PI * 0.5, Math.PI * 1.5); c.arc(x - r * 0.45, y - r * 0.42, r * 0.7, Math.PI, Math.PI * 1.85);
    c.arc(x + r * 0.35, y - r * 0.5, r * 0.82, Math.PI * 1.1, Math.PI * 1.95); c.arc(x + r * 1.15, y, r * 0.6, Math.PI * 1.5, Math.PI * 0.5);
    c.closePath(); c.fill(); c.stroke();
    c.fillStyle = '#e4dcff'; c.fillRect(x - r * 1.1, y - r * 0.1, r * 2.25, r * 0.1);
  }
  // The horizon is where the camera sees the world's level, wherever it has pitched or rolled to
  // (in a loop it turns right round). Sky above, Bayshore bay below.
  var STARS = [];
  for (var si0 = 0; si0 < 70; si0++) STARS.push([(si0 * 0.618034) % 1, ((si0 * 0.7548777) % 1) * 0.9 + 0.08, (si0 % 5) * 0.3 + 0.8]);
  function drawSky(S, t) {
    if (T.level === 2) { drawNight(S, t); return; }
    if (T.level === 3) { drawDusk(S, t); return; }
    var gx = V.Ry, gy = V.Uy, n = Math.hypot(gx, gy) || 1e-6, g0 = V.Fy * V.f;
    var d = -g0 / n, ux = gx / n, uy = -gy / n;
    var hx = V.cx + ux * d, hy = V.hY + uy * d, th = Math.atan2(gx, gy);
    var heading = Math.atan2(V.Fx, V.Fz), R = Math.hypot(viewW, viewH) + 80;
    c.fillStyle = FLAT.sky1; c.fillRect(-60, -60, viewW + 120, viewH + 120);
    c.save(); c.translate(hx, hy); c.rotate(th);
    var sh = viewH * 0.75, gr = c.createLinearGradient(0, -sh, 0, 0);
    gr.addColorStop(0, FLAT.sky0); gr.addColorStop(0.6, FLAT.sky1); gr.addColorStop(1, FLAT.sky2);
    c.fillStyle = FLAT.sky0; c.fillRect(-R, -R * 3, R * 2, R * 3 - sh + 1);
    c.fillStyle = gr; c.fillRect(-R, -sh, R * 2, sh + 1);
    // the sun: a big 80s disc with bands cut out of its lower half
    var ds = ((SUN_HEADING - heading + Math.PI * 3) % (Math.PI * 2)) - Math.PI;
    FX.sun = null;
    if (Math.abs(ds) < 1.2) {
      var sx = Math.tan(ds) * V.f, sy = -viewH * 0.3, sr = viewH * 0.11;
      sunRays(sx, sy, sr, t);
      FX.sun = { x: hx + sx * Math.cos(th) - sy * Math.sin(th), y: hy + sx * Math.sin(th) + sy * Math.cos(th) };
      c.fillStyle = 'rgba(255,244,190,0.45)'; c.beginPath(); c.arc(sx, sy, sr * 1.45, 0, 6.283); c.fill();
      c.fillStyle = '#fff3a8'; c.beginPath(); c.arc(sx, sy, sr, 0, 6.283); c.fill();
      c.fillStyle = '#ffc94d'; c.beginPath(); c.arc(sx, sy, sr, 0.15, Math.PI - 0.15); c.fill();
      c.fillStyle = FLAT.sky1;
      for (var b = 0; b < 4; b++) c.fillRect(sx - sr, sy + sr * (0.22 + b * 0.2), sr * 2, sr * (0.05 + b * 0.025));
    }
    // cel clouds, drifting
    var PW = V.f * 2.2;
    for (var ci = 0; ci < CLOUDS.length; ci++) {
      var cd = CLOUDS[ci], cx2 = ((((cd[0] - heading * 0.9 + t * 0.004) % 6.283) + 6.283) % 6.283 - 3.14) * PW * 0.5;
      if (Math.abs(cx2) < R) cloud(cx2, -viewH * cd[1] * 0.62, viewH * 0.045 * cd[2]);
    }
    // the Bayshore blimp, and gulls wheeling over the bay
    var bpx = ((((2.0 - heading * 0.9 + t * 0.003) % 6.283) + 6.283) % 6.283 - 3.14) * PW * 0.5;
    if (Math.abs(bpx) < R) blimp(bpx, -viewH * 0.47, viewH / 600 * 1.3);
    for (var gi = 0; gi < 6; gi++) {
      var gpx = ((((gi * 1.1 - heading * 0.95 + t * (0.012 + gi * 0.003)) % 6.283) + 6.283) % 6.283 - 3.14) * PW * 0.5;
      if (Math.abs(gpx) < R) gull(gpx, -viewH * (0.2 + 0.07 * Math.sin(t * 0.4 + gi * 2.1)) - gi * 6, viewH / 600 * (0.9 + (gi % 3) * 0.25), t, gi);
    }
    // far lilac hills, then the Bayshore skyline across the bay (one tile per third of the compass)
    c.fillStyle = '#c9b6f0'; c.strokeStyle = INK; c.lineWidth = 1.2;
    var hw = V.f * 1.4, ho = -((heading / 6.283) * 3 * hw) % hw, HILL = [26, 44, 30, 58, 36, 22];
    c.beginPath(); c.moveTo(-R, 2);
    for (var u = Math.floor((-R - ho) / (hw / 6)) - 1; ; u++) {
      var hxp = ho + u * hw / 6;
      c.lineTo(hxp, -HILL[((u % 6) + 6) % 6] * viewH / 600);
      if (hxp > R) break;
    }
    c.lineTo(R, 2); c.closePath(); c.fill(); c.stroke();
    var sk = SP.skyline, im = IMG.skyline;
    if (sk && im) {
      var dh = viewH * 0.2, sc = dh / sk.h, tw = sk.w * sc, off = -((heading / 6.283) * 3 * tw) % tw;
      for (var x0 = off - tw * 2; x0 < R; x0 += tw) if (x0 + tw > -R) c.drawImage(im, x0, -dh + 3, tw, dh);
    }
    // the bay, with sparkles
    var gs = c.createLinearGradient(0, 0, 0, viewH * 0.6);
    gs.addColorStop(0, FLAT.seaFar); gs.addColorStop(1, FLAT.sea);
    c.fillStyle = gs; c.fillRect(-R, 2, R * 2, R * 2);
    c.fillStyle = INK; c.fillRect(-R, 1, R * 2, 2);
    c.fillStyle = 'rgba(255,255,255,0.75)';
    for (var w = 0; w < 26; w++) {
      var wy = 8 + ((w * 37) % 120) * (1 + w * 0.05), wx = ((w * 211 - heading * V.f * 1.5 + t * (6 + w % 3)) % (R * 2) + R * 2) % (R * 2) - R;
      c.fillRect(wx, wy, 10 + (w % 4) * 6 + wy * 0.08, 1.5 + wy * 0.01);
    }
    // the sun's glitter path on the water, and sailboats out on the bay
    if (Math.abs(ds) < 1.2) glitter(Math.tan(ds) * V.f, t, R, '#fffbe0');
    for (var bi = 0; bi < BOATS.length; bi++) {
      var bt = BOATS[bi], bd = ((bt[0] - heading + Math.PI * 3) % (Math.PI * 2)) - Math.PI;
      if (Math.abs(bd) > 1.25) continue;
      var bsc = (0.7 + bt[1] * 1.6) * bt[2] * viewH / 600;
      boat(Math.tan(bd) * V.f + Math.sin(t * 0.05 + bi) * 30, 5 + bt[1] * 34 * viewH / 600, bsc, bt[3], t);
    }
    c.restore();
  }

  // The plant at night: a deep violet sky with stars and a big striped moon, the far side of the works
  // across the lake, and the lake itself, glowing violet chemical with drifting bubbles.
  function drawNight(S, t) {
    var gx = V.Ry, gy = V.Uy, n = Math.hypot(gx, gy) || 1e-6, g0 = V.Fy * V.f;
    var d = -g0 / n, ux = gx / n, uy = -gy / n;
    var hx = V.cx + ux * d, hy = V.hY + uy * d, th = Math.atan2(gx, gy);
    var heading = Math.atan2(V.Fx, V.Fz), R = Math.hypot(viewW, viewH) + 80;
    c.fillStyle = FLAT.sky1; c.fillRect(-60, -60, viewW + 120, viewH + 120);
    c.save(); c.translate(hx, hy); c.rotate(th);
    var sh = viewH * 0.75, gr = c.createLinearGradient(0, -sh, 0, 0);
    gr.addColorStop(0, FLAT.sky0); gr.addColorStop(0.62, FLAT.sky1); gr.addColorStop(1, FLAT.sky2);
    c.fillStyle = FLAT.sky0; c.fillRect(-R, -R * 3, R * 2, R * 3 - sh + 1);
    c.fillStyle = gr; c.fillRect(-R, -sh, R * 2, sh + 1);
    var PW = V.f * 2.2;
    c.fillStyle = '#ffffff';
    for (var si = 0; si < STARS.length; si++) {
      var st = STARS[si], sx0 = ((((st[0] * 6.283 - heading * 0.9) % 6.283) + 6.283) % 6.283 - 3.14) * PW * 0.5;
      if (Math.abs(sx0) > R) continue;
      c.globalAlpha = 0.5 + 0.5 * Math.sin(t * 2 + si);
      c.fillRect(sx0, -viewH * st[1] * 0.8, st[2] * 1.4, st[2] * 1.4);
    }
    c.globalAlpha = 1;
    // the storm over the works, lit from inside by lightning now and then
    stormClouds(heading, PW, R, t, FX.sky);
    if (FX.sky > 0.02) {
      var lx0 = FX.skyX * R;
      boltPath(lx0, -viewH * 0.48, lx0 + (Math.random() - 0.5) * 80, -viewH * 0.03, 40, 14); strokeBolt(2.2, '190,210,255', FX.sky);
    }
    // the moon: a pale disc cut by bands, like the beach sun at night
    var ds = ((0.6 - heading + Math.PI * 3) % (Math.PI * 2)) - Math.PI;
    if (Math.abs(ds) < 1.2) {
      var mx = Math.tan(ds) * V.f, my = -viewH * 0.36, mr = viewH * 0.09;
      c.fillStyle = 'rgba(160,240,255,0.25)'; c.beginPath(); c.arc(mx, my, mr * 1.5, 0, 6.283); c.fill();
      c.fillStyle = '#e8fbff'; c.beginPath(); c.arc(mx, my, mr, 0, 6.283); c.fill();
      c.fillStyle = '#9fe8ff'; c.beginPath(); c.arc(mx, my, mr, 0.15, Math.PI - 0.15); c.fill();
      c.fillStyle = FLAT.sky1;
      for (var b = 0; b < 4; b++) c.fillRect(mx - mr, my + mr * (0.22 + b * 0.2), mr * 2, mr * (0.05 + b * 0.025));
    }
    searchlights(R, t);
    // the far side of the works across the lake
    var sk = SP.chem_skyline, im = IMG.chem_skyline;
    c.fillStyle = '#3a2f7a'; c.fillRect(-R, -viewH * 0.05, R * 2, viewH * 0.05 + 2);
    if (sk && im) {
      var dh = viewH * 0.24, sc = dh / sk.h, tw = sk.w * sc, off = -((heading / 6.283) * 3 * tw) % tw;
      for (var x0 = off - tw * 2; x0 < R; x0 += tw) if (x0 + tw > -R) c.drawImage(im, x0, -dh + 3, tw, dh);
    }
    // the lake of violet chemical, with bubbles drifting by
    var gs = c.createLinearGradient(0, 0, 0, viewH * 0.6);
    gs.addColorStop(0, FLAT.seaFar); gs.addColorStop(1, FLAT.sea);
    c.fillStyle = gs; c.fillRect(-R, 2, R * 2, R * 2);
    c.fillStyle = INK; c.fillRect(-R, 1, R * 2, 2);
    for (var w = 0; w < 30; w++) {
      var wy = 8 + ((w * 37) % 120) * (1 + w * 0.05), wx = ((w * 211 - heading * V.f * 1.5 + t * (6 + w % 3)) % (R * 2) + R * 2) % (R * 2) - R;
      if (w % 3 === 0) { c.fillStyle = 'rgba(255,190,255,0.8)'; c.beginPath(); c.arc(wx, wy, 2 + wy * 0.03 + Math.sin(t * 3 + w) * 1, 0, 6.283); c.fill(); }
      else { c.fillStyle = 'rgba(255,220,255,0.55)'; c.fillRect(wx, wy, 10 + (w % 4) * 6 + wy * 0.08, 1.5 + wy * 0.01); }
    }
    if (Math.abs(ds) < 1.2) glitter(Math.tan(ds) * V.f, t, R, '#f0e0ff');
    c.restore();
  }

  // ---------- 1.5, level 3: the desert at dusk, with Las Vegas on the horizon ----------
  // A synthwave sunset: violet overhead, magenta to orange at the horizon, the first stars, a big banded
  // sun sinking off to the left behind two ranges of far ridges, and the city dead ahead up the highway
  // (VEGAS_H, the way the road set off), lit up and glowing, growing as the level goes on. A pyramid
  // throws a beam into the sky and searchlights sweep over the Strip. Below it all the desert floor.
  var SUN3 = -0.78, VEGAS_H = 0, RIDGE1 = [44, 72, 50, 92, 60, 38, 80, 52], RIDGE2 = [16, 30, 20, 38, 24, 14, 34, 22];
  function ridge(hw, ho, R, H, col, ink) {
    c.fillStyle = col; c.strokeStyle = INK; c.lineWidth = 1.2;
    c.beginPath(); c.moveTo(-R, 2);
    for (var u = Math.floor((-R - ho) / (hw / H.length)) - 1; ; u++) {
      var x = ho + u * hw / H.length;
      c.lineTo(x, -H[((u % H.length) + H.length) % H.length] * viewH / 600);
      if (x > R) break;
    }
    c.lineTo(R, 2); c.closePath(); c.fill(); if (ink) c.stroke();
  }
  function drawDusk(S, t) {
    var gx = V.Ry, gy = V.Uy, n = Math.hypot(gx, gy) || 1e-6, g0 = V.Fy * V.f;
    var d = -g0 / n, ux = gx / n, uy = -gy / n;
    var hx = V.cx + ux * d, hy = V.hY + uy * d, th = Math.atan2(gx, gy);
    var heading = Math.atan2(V.Fx, V.Fz), R = Math.hypot(viewW, viewH) + 80;
    c.fillStyle = FLAT.sky1; c.fillRect(-60, -60, viewW + 120, viewH + 120);
    c.save(); c.translate(hx, hy); c.rotate(th);
    var sh = viewH * 0.8, gr = c.createLinearGradient(0, -sh, 0, 0);
    gr.addColorStop(0, FLAT.sky0); gr.addColorStop(0.5, FLAT.sky1); gr.addColorStop(0.82, '#e0508a'); gr.addColorStop(1, FLAT.sky2);
    c.fillStyle = FLAT.sky0; c.fillRect(-R, -R * 3, R * 2, R * 3 - sh + 1);
    c.fillStyle = gr; c.fillRect(-R, -sh, R * 2, sh + 1);
    var PW = V.f * 2.2;
    // the first stars, high up
    c.fillStyle = '#ffffff';
    for (var si = 0; si < STARS.length; si++) {
      var st = STARS[si];
      if (st[1] < 0.45) continue;
      var sx0 = ((((st[0] * 6.283 - heading * 0.9) % 6.283) + 6.283) % 6.283 - 3.14) * PW * 0.5;
      if (Math.abs(sx0) > R) continue;
      c.globalAlpha = (0.25 + 0.35 * Math.sin(t * 2 + si)) * (st[1] - 0.4) * 1.6;
      c.fillRect(sx0, -viewH * st[1] * 0.85, st[2] * 1.2, st[2] * 1.2);
    }
    c.globalAlpha = 1;
    // the sun, sinking: a banded disc going down behind the ridges
    var ds = ((SUN3 - heading + Math.PI * 3) % (Math.PI * 2)) - Math.PI;
    FX.sun = null;
    if (Math.abs(ds) < 1.2) {
      var sx = Math.tan(ds) * V.f, sy = -viewH * 0.035, sr = viewH * 0.15;
      FX.sun = { x: hx + sx * Math.cos(th) - sy * Math.sin(th), y: hy + sx * Math.sin(th) + sy * Math.cos(th) };
      addOn(); glow(sx, sy, sr * 3.2, '255,120,90', 0.45, 0.6); addOff();
      var sg = c.createLinearGradient(0, sy - sr, 0, sy + sr);
      sg.addColorStop(0, '#fff3a0'); sg.addColorStop(0.5, '#ffb84d'); sg.addColorStop(1, '#ff4f8a');
      c.fillStyle = sg; c.beginPath(); c.arc(sx, sy, sr, 0, 6.283); c.fill();
      c.fillStyle = '#e0508a';
      for (var b = 0; b < 5; b++) c.fillRect(sx - sr, sy - sr * 0.1 + b * sr * 0.2, sr * 2, sr * (0.035 + b * 0.02));
    }
    // two ranges of far ridges
    var hw = V.f * 1.6;
    ridge(hw, -((heading / 6.283) * 3 * hw) % hw, R, RIDGE1, '#8a4a96', false);
    var hw2 = V.f * 1.1;
    ridge(hw2, -((heading / 6.283) * 4 * hw2) % hw2, R, RIDGE2, '#5a2c6e', true);
    // Las Vegas, dead ahead up the highway
    var dv = ((VEGAS_H - heading + Math.PI * 3) % (Math.PI * 2)) - Math.PI, vk = SP.vegas, vim = IMG.vegas;
    if (Math.abs(dv) < 1.15 && vk) {
      var prog = clamp(V.dc / 80000, 0, 1), dh = viewH * (0.2 + 0.17 * prog), vsc = dh / vk.h, tw = vk.w * vsc, vx = Math.tan(dv) * V.f;
      if (vx + tw > -R && vx - tw < R) {
        addOn();
        glow(vx, -dh * 0.25, tw * 0.95, '255,80,170', 0.4, 0.45);
        glow(vx, -dh * 0.05, tw * 0.6, '255,190,110', 0.35, 0.3);
        // the beam off the pyramid, straight up, and searchlights sweeping over the Strip
        var bx = vx - tw / 2 + tw * 0.216, by = -dh + 3 + dh * 0.7, bl = viewH * 1.1;
        var bg = c.createLinearGradient(bx, by, bx, by - bl);
        bg.addColorStop(0, 'rgba(220,240,255,0.75)'); bg.addColorStop(1, 'rgba(220,240,255,0)');
        c.fillStyle = bg; c.beginPath(); c.moveTo(bx - 1.5, by); c.lineTo(bx - 4, by - bl); c.lineTo(bx + 4, by - bl); c.lineTo(bx + 1.5, by); c.closePath(); c.fill();
        for (var li = 0; li < 3; li++) {
          var lx = vx + (li - 1) * tw * 0.32, a = Math.sin(t * (0.3 + li * 0.08) + li * 2.1) * 0.6, len = viewH * 0.95, wd = 0.045;
          var lg = c.createLinearGradient(lx, -dh * 0.5, lx + Math.sin(a) * len, -dh * 0.5 - Math.cos(a) * len);
          lg.addColorStop(0, 'rgba(255,200,240,0.24)'); lg.addColorStop(1, 'rgba(255,200,240,0)');
          c.fillStyle = lg; c.beginPath(); c.moveTo(lx - 2, -dh * 0.5); c.lineTo(lx + Math.sin(a - wd) * len, -dh * 0.5 - Math.cos(a - wd) * len);
          c.lineTo(lx + Math.sin(a + wd) * len, -dh * 0.5 - Math.cos(a + wd) * len); c.lineTo(lx + 2, -dh * 0.5); c.closePath(); c.fill();
        }
        addOff();
        if (vim) c.drawImage(vim, vx - tw / 2, -dh + 3, tw, dh);
        // the city's neon pulses
        addOn(); glow(vx, -dh * 0.15, tw * 0.5, '255,79,184', 0.12 + 0.06 * Math.sin(t * 2.3), 0.4); addOff();
      }
    }
    // the desert floor, fading into the dusk, with scrub on it
    var gs = c.createLinearGradient(0, 0, 0, viewH * 0.6);
    gs.addColorStop(0, FLAT.seaFar); gs.addColorStop(1, FLAT.sea);
    c.fillStyle = gs; c.fillRect(-R, 2, R * 2, R * 2);
    c.fillStyle = INK; c.fillRect(-R, 1, R * 2, 2);
    c.fillStyle = '#6a3a5a';
    for (var w = 0; w < 34; w++) {
      var wy = 6 + ((w * 37) % 120) * (1 + w * 0.05), wx = ((w * 211 - heading * V.f * 1.5) % (R * 2) + R * 2) % (R * 2) - R, wr = 1.5 + wy * 0.05;
      c.globalAlpha = 0.55; c.beginPath(); c.ellipse(wx, wy, wr * 2.2, wr, 0, 0, 6.283); c.fill();
    }
    c.globalAlpha = 1;
    c.restore();
  }

  // ---------- Bayshore on the ground: the beach on the left, the promenade and lawns on the right ----------
  // Flat bands at ground level, following the road's heading, from far to near.
  var HR = { x: 1, z: 0 }, GSTEP = 150, GFAR = 15000;
  function hRight() { var m = Math.hypot(TP.fx, TP.fz); if (m > 0.25) { HR.x = TP.fz / m; HR.z = -TP.fx / m; } }
  var GXS = [-1400, -175, 175, 700, 6000, -1500, -1440];
  var GX = new Float64Array(7), GY = new Float64Array(7), GOK = new Uint8Array(7);
  var GPV = { x: new Float64Array(7), y: new Float64Array(7), ok: new Uint8Array(7) };
  var GCOL = null;
  function drawGround() {
    if (!GCOL) GCOL = [[5, 0, FLAT.wet], [0, 1, FLAT.sand], [2, 3, FLAT.prom], [3, 4, FLAT.lawn]];
    var edgeCol = T.level === 2 ? '#ffd23f' : T.level === 3 ? null : '#ffffff';
    var gy = TF.minY - 30, s, i, first = true;
    for (s = V.s0 + GFAR; s > V.s0 + (RCAM ? -1300 : 40); s -= GSTEP) {
      trackAt(s); hRight();
      for (i = 0; i < 7; i++) { projW(TP.x + HR.x * GXS[i], gy, TP.z + HR.z * GXS[i]); GX[i] = PJ.x; GY[i] = PJ.y; GOK[i] = PBAD ? 0 : 1; }
      if (!first) {
        for (var j = 0; j < GCOL.length; j++) {
          var a = GCOL[j][0], b = GCOL[j][1];
          if (!(GOK[a] && GOK[b] && GPV.ok[a] && GPV.ok[b])) continue;
          c.fillStyle = GCOL[j][2]; c.beginPath();
          c.moveTo(GPV.x[a], GPV.y[a]); c.lineTo(GPV.x[b], GPV.y[b]); c.lineTo(GX[b], GY[b]); c.lineTo(GX[a], GY[a]); c.closePath(); c.fill();
        }
        if (edgeCol && GOK[6] && GPV.ok[6]) { c.strokeStyle = edgeCol; c.lineWidth = 2; c.beginPath(); c.moveTo(GPV.x[6], GPV.y[6]); c.lineTo(GX[6], GY[6]); c.stroke(); }
      }
      first = false;
      for (i = 0; i < 7; i++) { GPV.x[i] = GX[i]; GPV.y[i] = GY[i]; GPV.ok[i] = GOK[i]; }
    }
  }

  // ---------- the props of Bayshore, placed by a hash of the slot, so every take has the same town ----------
  var PROPS = [], PSLOT = 330, PFAR = 11000, SLOT = [];
  function hash(n) { n = (n ^ 61) ^ (n >>> 16); n = n + (n << 3); n = n ^ (n >>> 4); n = Math.imul(n, 0x27d4eb2d); return ((n ^ (n >>> 15)) >>> 0) / 4294967296; }
  var HOTELS = ['hotel_pink', 'hotel_mint', 'hotel_lav', 'hotel_peach'];
  // [sprite, across (- = beach side), along the slot (0..1), size]
  function slotProps(k, out) {
    var h1 = hash(k * 7 + 1), h2 = hash(k * 7 + 2), h3 = hash(k * 7 + 3), h4 = hash(k * 7 + 4), h5 = hash(k * 7 + 5);
    if (k % 2 === 0) out.push(['lamp', -EDGE - 26, 0]); else out.push(['lamp_r', EDGE + 26, 0]);
    if (h1 < 0.7) out.push([h2 < 0.5 ? 'palm' : 'palm2', -EDGE - 90 - h3 * 80, 0.1]);
    if (h2 < 0.55) out.push([h1 < 0.5 ? 'palm2' : 'palm', EDGE + 95 + h4 * 70, 0.1]);
    if (h3 < 0.4) out.push([h4 < 0.5 ? 'umbrella' : 'umbrella2', -420 - h5 * 700, 0.2]);
    if (h5 < 0.3) out.push([h1 < 0.5 ? 'umbrella2' : 'umbrella', -360 - h2 * 500, 0.3]);
    if (k % 7 === 3) out.push([h2 < 0.5 ? 'lifeguard' : 'lifeguard2', -900 - h3 * 300, 0.4]);
    if (k % 11 === 5) out.push(['billboard', EDGE + 420, 0.2]);
    else out.push([HOTELS[Math.floor(h4 * 4)], 1050 + h5 * 700, 0.5, 1.35]);
    if (k % 2 === 1) out.push([HOTELS[Math.floor(h3 * 4)], 2600 + h1 * 900, 0.6, 1.5]);
    if (k % 23 === 9) out.push(['ferris', -2300, 0.5, 1.2]);
    if (k % 29 === 17) out.push(['lighthouse', -2400, 0.5, 1.2]);
  }
  // the Volta Chemical Works: plant floodlights by the rails, tanks and pipework close in, spheres,
  // towers and stacks further out, vats of the violet stuff here and there
  function chemProps(k, out) {
    var h1 = hash(k * 7 + 1), h2 = hash(k * 7 + 2), h3 = hash(k * 7 + 3), h4 = hash(k * 7 + 4), h5 = hash(k * 7 + 5);
    if (k % 2 === 0) out.push(['clamp', -EDGE - 26, 0]); else out.push(['clamp_r', EDGE + 26, 0]);
    if (h1 < 0.5) out.push([h2 < 0.5 ? 'tank' : 'tank2', -EDGE - 230 - h3 * 200, 0.2]);
    if (h2 < 0.5) out.push([h4 < 0.55 ? 'pipework' : 'vat', EDGE + 240 + h4 * 200, 0.3]);
    if (h3 < 0.25) out.push(['vat', -700 - h1 * 300, 0.7]);
    if (k % 5 === 2) out.push(['sphere', -1000 - h3 * 400, 0.5, 1.2]);
    if (k % 4 === 1) out.push([h5 < 0.5 ? 'tower' : 'stack', 1000 + h5 * 800, 0.5, 1.3]);
    if (k % 3 === 0) out.push([h3 < 0.5 ? 'tank' : 'sphere', 2100 + h1 * 900, 0.6, 1.4]);
    if (k % 7 === 4) out.push(['stack', -2300 - h2 * 500, 0.4, 1.4]);
    if (k % 9 === 6) out.push(['tower', -1700 - h4 * 400, 0.8, 1.3]);
  }
  // 1.5, the Nevada desert: telephone poles with sodium lamps along the shoulders, Joshua trees, barrel
  // cacti and red sandstone close in, a motel, a gas stop and casino pylons now and then, buttes far out
  function desertProps(k, out) {
    var h1 = hash(k * 7 + 1), h2 = hash(k * 7 + 2), h3 = hash(k * 7 + 3), h4 = hash(k * 7 + 4), h5 = hash(k * 7 + 5);
    if (k % 2 === 0) out.push(['pole', -EDGE - 30, 0]); else out.push(['pole_r', EDGE + 30, 0]);
    if (h1 < 0.55) out.push([h2 < 0.5 ? 'joshua' : 'joshua2', -EDGE - 110 - h3 * 260, 0.2]);
    if (h2 < 0.5) out.push([h4 < 0.5 ? 'joshua2' : 'joshua', EDGE + 120 + h5 * 240, 0.45]);
    if (h3 < 0.45) out.push(['cactus', (h1 < 0.5 ? -1 : 1) * (EDGE + 70 + h4 * 120), 0.7]);
    if (h4 < 0.35) out.push([h5 < 0.5 ? 'rock' : 'rock2', (h2 < 0.5 ? -1 : 1) * (600 + h1 * 900), 0.5, 1.2]);
    if (k % 13 === 4) out.push(['motel', EDGE + 560, 0.5]);
    else if (k % 13 === 10) out.push(['gas', -EDGE - 500, 0.5]);
    if (k % 9 === 7) out.push(['pylon', (h3 < 0.5 ? -1 : 1) * (EDGE + 430), 0.3, 1.1]);
    if (k % 5 === 1) out.push(['butte', (h5 < 0.5 ? -1 : 1) * (2600 + h2 * 1600), 0.5, 1.6]);
    if (k % 3 === 0) out.push([h1 < 0.5 ? 'rock2' : 'joshua2', (h4 < 0.5 ? -1 : 1) * (1500 + h3 * 900), 0.6, 1.4]);
  }
  function propsOf(k, out) { if (T.level === 2) chemProps(k, out); else if (T.level === 3) desertProps(k, out); else slotProps(k, out); }
  // The road can come back past a prop: the far side of a loop lands a road-width over, an S-bend or
  // a helix swings back. A grid of the whole track's footprint (the ground plan) lets each prop check
  // it stands clear of every stretch of road but its own, so no palm or tank ends up on a road.
  var PCACHE = {}, PGRID = null, PCELL = 320;
  function grid() {
    if (PGRID) return PGRID;
    PGRID = {};
    for (var i = 0; i < TF.n; i += 2) {
      var key = Math.floor(TF.px[i] / PCELL) + ',' + Math.floor(TF.pz[i] / PCELL);
      (PGRID[key] || (PGRID[key] = [])).push(i);
    }
    return PGRID;
  }
  function clearOfRoad(wx, wz, s, r) {
    var G = grid(), cx = Math.floor(wx / PCELL), cz = Math.floor(wz / PCELL), need = EDGE + 30 + r, n = Math.ceil(need / PCELL);
    for (var a = -n; a <= n; a++) for (var b = -n; b <= n; b++) {
      var L = G[(cx + a) + ',' + (cz + b)];
      if (!L) continue;
      for (var j = 0; j < L.length; j++) {
        var i = L[j], si = i * TF.step - TF.pre;
        if (Math.abs(si - s) < 420) continue; // its own stretch of road: placed beside it on purpose
        var dx = TF.px[i] - wx, dz = TF.pz[i] - wz;
        if (dx * dx + dz * dz < need * need) return false;
      }
    }
    return true;
  }
  function slotList(k) {
    var L = PCACHE[k];
    if (L) return L;
    L = PCACHE[k] = [];
    SLOT.length = 0;
    propsOf(k, SLOT);
    for (var i = 0; i < SLOT.length; i++) {
      var p = SLOT[i], s = k * PSLOT + p[2] * PSLOT, sp = SP[p[0]], dd = s / ZK;
      // nothing close in stands along a pipe: it would poke through the tube
      if (Math.abs(p[1]) < 800 && (T.inPipe(dd) || T.inPipe(dd + 250) || T.inPipe(dd - 250))) continue;
      trackAt(s); hRight();
      var wx = TP.x + HR.x * p[1], wz = TP.z + HR.z * p[1];
      var r = sp ? sp.fw * sp.cmPerPx / CMU * 0.5 * (p[3] || 1) * 0.8 : 20;
      if (!clearOfRoad(wx, wz, s, r)) continue;
      L.push({ k: p[0], wx: wx, wz: wz, side: p[1] < 0 ? 1 : -1, mul: p[3] || 1 });
    }
    return L;
  }
  var PROPP = []; // 1.7.2: the frame's prop records are reused, not made new every frame
  function gatherProps() {
    PROPS.length = 0;
    var gy = TF.minY - 30, k0 = Math.floor((V.s0 + (RCAM ? -1300 : 60)) / PSLOT), k1 = Math.floor((V.s0 + PFAR) / PSLOT);
    for (var k = k0; k <= k1; k++) {
      var L = slotList(k);
      for (var i = 0; i < L.length; i++) {
        var p = L[i];
        projW(p.wx, gy, p.wz);
        if (PBAD || PJ.z < 30) continue;
        var q = PROPP[PROPS.length] || (PROPP[PROPS.length] = { k: '', x: 0.5, y: 0.5, s: 0.5, z: 0.5, side: 0, mul: 1, a: 0.5 });
        q.k = p.k; q.x = PJ.x; q.y = PJ.y; q.s = PJ.s; q.z = PJ.z; q.side = p.side; q.mul = p.mul; q.a = clamp((PFAR - PJ.z) / 1800, 0, 1);
        PROPS.push(q);
      }
    }
  }
  function drawProp(p) {
    if (p.a <= 0.01) return;
    c.globalAlpha = p.a;
    var rot = Math.atan2(V.Ry, V.Uy), k = p.k;
    if (k === 'palm' || k === 'palm2') rot += Math.sin(TNOW * 1.3 + p.x * 0.01 + p.z * 0.002) * 0.03;
    sprite(k, p.side * 28, p.x, p.y, p.s, rot, false, p.mul);
    var f = fogAt(p.z);
    if (f > 0.05) { c.globalAlpha = p.a * f; sprite(k, p.side * 28, p.x, p.y, p.s, rot, 2, p.mul); }
    c.globalAlpha = 1;
    if (LV2) propLights(p, rot, f);
    else if (DUSK) duskLights(p, rot, f);
  }
  // the desert's lights: sodium lamps on the poles, the motel's neon and its blinking star, a casino
  // pylon's rings chasing up to its dice, the gas stop's canopy
  var DQ = new Float64Array(2), DLS = { x: 0.5, y: 0.5, cs: 0.5, sn: 0.5, u: 0.5 };
  function at(across, up) { var d = DLS; DQ[0] = d.x + d.cs * across * d.u + d.sn * up * d.u; DQ[1] = d.y + d.sn * across * d.u - d.cs * up * d.u; return DQ; }
  function duskLights(p, rot, f) {
    var k = p.k, s = SP[k];
    if (!s || !s.frames) return;
    var u = p.s * p.mul, sn = Math.sin(rot), cs = Math.cos(rot), a = p.a * (1 - f * 0.5), ph = p.x * 0.011 + p.z * 0.003;
    var q = DQ;
    DLS.x = p.x; DLS.y = p.y; DLS.cs = cs; DLS.sn = sn; DLS.u = u;
    addOn();
    if (k === 'pole' || k === 'pole_r') {
      q = at((k === 'pole' ? 1 : -1) * 15, 63);
      glow(q[0], q[1], Math.max(4, 26 * u), '255,170,80', a * 0.75); glow(q[0], q[1], Math.max(1.5, 6 * u), '255,240,200', a);
    } else if (k === 'motel') {
      var on = Math.sin(TNOW * 7 + ph) > -0.6;
      q = at(-55, 98); if (on) glow(q[0], q[1], Math.max(6, 40 * u), '255,79,184', a * 0.7, 0.55);
      q = at(-55, 122); if (Math.sin(TNOW * 3 + ph) > 0) glow(q[0], q[1], Math.max(4, 16 * u), '255,230,80', a * 0.9);
      q = at(-40, 77); glow(q[0], q[1], Math.max(4, 20 * u), '255,230,80', a * 0.6, 0.4);
    } else if (k === 'pylon') {
      for (var i = 0; i < 9; i++) {
        var lit2 = ((i - TNOW * 8) % 9 + 9) % 9 < 3;
        q = at(0, (240 + i * 140) / 9.14);
        glow(q[0], q[1], Math.max(3, 16 * u), i % 3 === 0 ? '255,79,184' : i % 3 === 1 ? '46,230,255' : '255,201,60', a * (lit2 ? 0.9 : 0.35), 0.5);
      }
      q = at(0, 178); glow(q[0], q[1], Math.max(5, 34 * u), '255,230,140', a * 0.55, 0.4);
    } else if (k === 'gas') {
      q = at(0, 50); glow(q[0], q[1], Math.max(6, 56 * u), '255,250,230', a * 0.5, 0.35);
      q = at(0, 51); glow(q[0], q[1], Math.max(4, 50 * u), '255,60,70', a * 0.35, 0.2);
    }
    addOff();
  }
  function propLights(p, rot, f) {
    var s = SP[p.k];
    if (!s || !s.frames) return;
    var sc = p.s * s.cmPerPx / CMU * p.mul, hgt = s.frames[0].ay * sc, sn = Math.sin(rot), cs = Math.cos(rot);
    var tx = p.x + sn * hgt * 0.93, ty = p.y - cs * hgt * 0.93, a = p.a * (1 - f * 0.5), k = p.k, ph = p.x * 0.013 + hgt;
    if (k === 'stack' || k === 'tower') {
      if (k === 'stack' && hgt > 6) {
        // steam rolling off the top
        var r0 = Math.max(2, s.fw * sc * 0.12);
        for (var i = 0; i < 5; i++) {
          var u = (i + ((TNOW * 0.4 + ph) % 1)) / 5, rr = r0 * (0.8 + u * 2.2);
          c.globalAlpha = a * 0.5 * (1 - u);
          c.fillStyle = '#c9c0f5'; c.beginPath(); c.arc(tx + sn * u * r0 * 9 + u * r0 * 2, ty - cs * u * r0 * 9, rr, 0, 6.283); c.fill();
        }
        c.globalAlpha = 1;
      }
      var on = Math.sin(TNOW * 3 + ph) > 0;
      if (on) { addOn(); glow(tx, ty, Math.max(4, hgt * 0.12), '255,60,60', a * 0.9); addOff(); }
    } else if (k === 'vat') {
      addOn(); glow(tx, ty + hgt * 0.15, Math.max(5, hgt * 0.5), '200,90,255', a * (0.45 + 0.15 * Math.sin(TNOW * 2 + ph)), 0.5); addOff();
    } else if (k === 'sphere' || k === 'tank' || k === 'tank2') {
      if (Math.sin(TNOW * 2.2 + ph) > 0.3) { addOn(); glow(tx, ty, Math.max(3, hgt * 0.08), '255,210,63', a * 0.8); addOff(); }
    } else if (k === 'clamp' || k === 'clamp_r') {
      addOn(); glow(tx, ty, Math.max(5, hgt * 0.35), '255,170,80', a * 0.8); glow(tx, ty, Math.max(2, hgt * 0.1), '255,240,200', a); addOff();
    }
  }

  // ---------- the road: one strip per segment, sorted in with everything else ----------
  // Each segment boundary keeps its centre, right and up in camera terms, so a point across the
  // road is a few multiplies away.
  function F64() { return new Float64Array(NS + 26); }
  var SB = { n: 0, Z: F64(), d: F64(), idx: new Int32Array(NS + 26), px: F64(), py: F64(), pz: F64(),
    rx: F64(), ry: F64(), rz: F64(), ux: F64(), uy: F64(), uz: F64(), wy: F64(), up: F64(), zone: [], gap: new Uint8Array(NS + 26),
    wux: F64(), wuy: F64(), wuz: F64() };
  function boundaries() {
    var base = V.s0, frac = ((base % SEG) + SEG) % SEG, idx0 = Math.floor(base / SEG), n = 0;
    for (var k = Math.floor(ZN / SEG) - 1; k < NS + 1; k++) {
      var Z = k * SEG - frac;
      if (Z + SEG < ZN) continue;
      if (Z < ZN) Z = ZN;
      if (Z > DRAW) break;
      trackAt(V.s0 + Z);
      var wx = TP.x - V.ox, wy = TP.y - V.oy, wz = TP.z - V.oz;
      SB.Z[n] = Z; SB.idx[n] = idx0 + k; SB.d[n] = dOfZ(Z);
      SB.px[n] = wx * V.Rx + wy * V.Ry + wz * V.Rz; SB.py[n] = wx * V.Ux + wy * V.Uy + wz * V.Uz; SB.pz[n] = wx * V.Fx + wy * V.Fy + wz * V.Fz;
      SB.rx[n] = TP.rx * V.Rx + TP.ry * V.Ry + TP.rz * V.Rz; SB.ry[n] = TP.rx * V.Ux + TP.ry * V.Uy + TP.rz * V.Uz; SB.rz[n] = TP.rx * V.Fx + TP.ry * V.Fy + TP.rz * V.Fz;
      SB.ux[n] = TP.ux * V.Rx + TP.uy * V.Ry + TP.uz * V.Rz; SB.uy[n] = TP.ux * V.Ux + TP.uy * V.Uy + TP.uz * V.Uz; SB.uz[n] = TP.ux * V.Fx + TP.uy * V.Fy + TP.uz * V.Fz;
      SB.wy[n] = TP.y - TF.minY; SB.up[n] = TP.uy; SB.wux[n] = TP.ux; SB.wuy[n] = TP.uy; SB.wuz[n] = TP.uz;
      SB.zone[n] = T.zoneAt(SB.d[n]); SB.gap[n] = SB.zone[n] === 'gap' ? 1 : 0;
      n++;
    }
    SB.n = n;
  }
  // a point X across, h up, at boundary n
  function bp(n, X, h) {
    var x = SB.px[n] + SB.rx[n] * X + SB.ux[n] * h, y = SB.py[n] + SB.ry[n] * X + SB.uy[n] * h, z = SB.pz[n] + SB.rz[n] * X + SB.uz[n] * h;
    PBAD = z < 2; if (z < 2) z = 2;
    PJ.s = V.f / z; PJ.x = V.cx + x * PJ.s; PJ.y = V.hY - y * PJ.s;
  }
  var QX = new Float64Array(4), QY = new Float64Array(4);
  function quad(col) {
    c.fillStyle = col; c.beginPath(); c.moveTo(QX[0], QY[0]); c.lineTo(QX[1], QY[1]); c.lineTo(QX[2], QY[2]); c.lineTo(QX[3], QY[3]); c.closePath(); c.fill();
  }
  // four boundary points into the quad; false if any is behind the camera
  function q4(n0, X0, h0, n1, X1, h1, n2, X2, h2, n3, X3, h3) {
    bp(n0, X0, h0); QX[0] = PJ.x; QY[0] = PJ.y; var bad = PBAD;
    bp(n1, X1, h1); QX[1] = PJ.x; QY[1] = PJ.y; bad = bad || PBAD;
    bp(n2, X2, h2); QX[2] = PJ.x; QY[2] = PJ.y; bad = bad || PBAD;
    bp(n3, X3, h3); QX[3] = PJ.x; QY[3] = PJ.y; bad = bad || PBAD;
    return !bad;
  }
  // a flat strip from x0 to x1 across, between boundaries n and n+1
  function strip(n, x0, x1, col) { if (q4(n, x0, 0, n, x1, 0, n + 1, x1, 0, n + 1, x0, 0)) quad(col); }
  // an upright strip (a rail) at X, from h0 to h1
  function wall(n, X, h0, h1, col) { if (q4(n, X, h0, n, X, h1, n + 1, X, h1, n + 1, X, h0)) quad(col); }
  function post(n, X) { if (q4(n, X - 1.4, 0, n, X - 1.4, 11, n, X + 1.4, 11, n, X + 1.4, 0)) quad(FLAT.post); }
  // the road, cel style: an ink edge, pink-and-white kerbs, banded lavender asphalt, cyan rails
  function drawSegment(n) {
    var Z = SB.Z[n], idx = SB.idx[n], zone = SB.zone[n];
    var lights = segLight(n), jl = lights ? 0 : -1, jr = lights ? 5 : -1, ink = segCol(INK, -1);
    if (SB.wy[n] > 8) { wall(n, -EDGE - 8, -16, 0, segCol(FLAT.deckSide, jl)); wall(n, EDGE + 8, -16, 0, segCol(FLAT.deckSide, jr)); }
    strip(n, -EDGE - 9, EDGE + 9, ink);
    var kb = zone === 'warn' ? (idx & 1 ? FLAT.warn : FLAT.warnDark) : ((idx >> 1) & 1 ? FLAT.kerbA : FLAT.kerbB);
    strip(n, -EDGE - 6, -RH, segCol(kb, jl)); strip(n, RH, EDGE + 6, segCol(kb, jr));
    strip(n, -RH - 2, -RH, ink); strip(n, RH, RH + 2, ink);
    var rc = zone === 'kick' && (idx >> 1) & 1 ? FLAT.kick : ((idx >> 2) & 1 ? FLAT.road : FLAT.road2);
    if (lights) { strip(n, -RH, -63, segCol(rc, 1)); strip(n, -65, 1, segCol(rc, 2)); strip(n, -1, 65, segCol(rc, 3)); strip(n, 63, RH, segCol(rc, 4)); }
    else strip(n, -RH, RH, segCol(rc, -1));
    if ((idx & 3) < 2 && Z < 3600 && zone !== 'kick') { var lc = segCol(FLAT.line, lights ? 2 : -1); strip(n, -66, -62, lc); strip(n, -2, 2, lc); strip(n, 62, 66, lc); }
    if (Z < 3800) {
      wall(n, -EDGE - 3, 5, 10, segCol(FLAT.rail, jl)); wall(n, EDGE + 3, 5, 10, segCol(FLAT.rail, jr));
      wall(n, -EDGE - 3, 10, 11.2, ink); wall(n, EDGE + 3, 10, 11.2, ink);
      if (idx % 3 === 0) { post(n, -EDGE - 3); post(n, EDGE + 3); }
    }
    if (HZN) hazardStrips(n);
    // where the road starts again after a gap, its cut edge faces you
    if (SB.gap[n] && !SB.gap[n + 1] && q4(n + 1, -EDGE - 6, 0, n + 1, EDGE + 6, 0, n + 1, EDGE + 6, -22, n + 1, -EDGE - 6, -22)) quad(segCol(FLAT.deck, -1));
    if (zone === 'pipe') drawTube(n);
    // night: cat's eyes along the lane lines, and the rail lamps on the posts
    if ((LV2 || DUSK) && Z < 2600 && zone !== 'pipe' && (idx & 3) === 0) {
      addOn();
      for (var ce = -1; ce <= 1; ce++) { bp(n, ce * 64, 0.6); if (!PBAD) glow(PJ.x, PJ.y, Math.max(1.6, 3.2 * PJ.s), '255,214,90', 0.85 * (1 - LJ.f)); }
      if (idx % 12 === 0) for (var rs = -1; rs <= 1; rs += 2) { bp(n, rs * (EDGE + 3), 12); if (!PBAD) glow(PJ.x, PJ.y, Math.max(2, 9 * PJ.s), '255,158,61', 0.7 * (1 - LJ.f)); }
      addOff();
    }
  }
  // A pipe: a glass tube round the road (a circle through both road edges, its axis PIPE_C above the
  // road), cut into panels with a steel rib every few segments and a lit stripe along the crown. It
  // is built in the road's own frame, so as the road rolls round the tube the tube rolls with it.
  var PIPE_K = 12, PIPE_R = 0, PIPE_A = 0, PCX = new Float64Array(PIPE_K + 1), PCH = new Float64Array(PIPE_K + 1);
  function pipeShape() {
    if (PIPE_R) return;
    PIPE_R = Math.hypot(EDGE + 9, T.PIPE_C); PIPE_A = Math.atan2(EDGE + 9, T.PIPE_C);
    for (var j = 0; j <= PIPE_K; j++) {
      var a = PIPE_A + (Math.PI * 2 - PIPE_A * 2) * j / PIPE_K;
      PCX[j] = PIPE_R * Math.sin(a); PCH[j] = T.PIPE_C - PIPE_R * Math.cos(a);
    }
  }
  function drawTube(n) {
    pipeShape();
    var idx = SB.idx[n];
    c.globalAlpha = 0.26;
    for (var j = 0; j < PIPE_K; j++) {
      if (q4(n, PCX[j], PCH[j], n, PCX[j + 1], PCH[j + 1], n + 1, PCX[j + 1], PCH[j + 1], n + 1, PCX[j], PCH[j])) quad((j + (idx >> 1)) & 1 ? FLAT.glassA : FLAT.glassB);
    }
    c.globalAlpha = 1;
    // the lit stripe along the crown of the tube
    var m = PIPE_K >> 1;
    if (q4(n, PCX[m] - 10, PCH[m] + 1, n, PCX[m] + 10, PCH[m] + 1, n + 1, PCX[m] + 10, PCH[m] + 1, n + 1, PCX[m] - 10, PCH[m] + 1)) quad(idx & 1 ? '#ff4fb8' : '#ffd23f');
    // a steel rib every fourth segment, and a fat collar at each mouth of the tube
    var mouth = SB.zone[n - 1] !== 'pipe' && n > 0 || SB.zone[n + 1] !== 'pipe';
    if (idx % 4 === 0 || mouth) ring(mouth && SB.zone[n + 1] === 'pipe' ? n : n, mouth ? 16 : 5, mouth);
    // lamps along the crown, and a pulse of light racing through the tube
    if (idx % 8 === 0 && SB.Z[n] < 3600) { bp(n, PCX[m], PCH[m] - 2); if (!PBAD) { addOn(); glow(PJ.x, PJ.y, Math.max(3, 30 * PJ.s), '95,240,255', 0.8 * (1 - LJ.f)); addOff(); } }
    if ((idx + Math.floor(TNOW * 22)) % 30 === 0) { c.save(); c.globalCompositeOperation = 'lighter'; c.globalAlpha = 0.7 * (1 - LJ.f); ringGlow(n); c.restore(); }
  }
  function ringGlow(n) {
    c.beginPath();
    for (var j = 0; j <= PIPE_K; j++) { bp(n, PCX[j], PCH[j]); if (PBAD) return; if (j) c.lineTo(PJ.x, PJ.y); else c.moveTo(PJ.x, PJ.y); }
    c.strokeStyle = '#7ff7ff'; c.lineWidth = Math.max(2, 10 * PJ.s); c.stroke();
    c.strokeStyle = '#ffffff'; c.lineWidth = Math.max(1, 3 * PJ.s); c.stroke();
  }
  function ring(n, w, mouth) {
    var bad = false;
    c.beginPath();
    for (var j = -1; j <= PIPE_K + 1; j++) {
      var jj = clamp(j, 0, PIPE_K), X = PCX[jj] * (j < 0 || j > PIPE_K ? 1 : 1), Hh = PCH[jj];
      if (j < 0) { X = -EDGE - 9; Hh = -6; } else if (j > PIPE_K) { X = EDGE + 9; Hh = -6; }
      bp(n, X, Hh); if (PBAD) bad = true;
      if (j < 0) c.moveTo(PJ.x, PJ.y); else c.lineTo(PJ.x, PJ.y);
    }
    if (bad) return;
    var lw = Math.max(1.5, w * PJ.s);
    c.lineJoin = 'round';
    c.strokeStyle = INK; c.lineWidth = lw + 3; c.stroke();
    c.strokeStyle = FLAT.rib; c.lineWidth = lw; c.stroke();
    if (mouth) { c.strokeStyle = FLAT.ribLite; c.lineWidth = Math.max(1, lw * 0.3); c.setLineDash([lw * 1.2, lw * 1.2]); c.stroke(); c.setLineDash([]); }
  }

  // a plain pillar under raised road, down to the ground
  function drawPillar(n) {
    trackAt(V.s0 + SB.Z[n]);
    var gy = TF.minY, w = 18;
    var ax = TP.x - TP.rx * w, az = TP.z - TP.rz * w, ay = TP.y - TP.ry * w - 4;
    var bx = TP.x + TP.rx * w, bz = TP.z + TP.rz * w, by = TP.y + TP.ry * w - 4;
    projW(ax, ay, az); QX[0] = PJ.x; QY[0] = PJ.y; var bad = PBAD;
    projW(bx, by, bz); QX[1] = PJ.x; QY[1] = PJ.y; bad = bad || PBAD;
    projW(bx, gy, bz); QX[2] = PJ.x; QY[2] = PJ.y; bad = bad || PBAD;
    projW(ax, gy, az); QX[3] = PJ.x; QY[3] = PJ.y; bad = bad || PBAD;
    if (!bad) { quad(lit(FLAT.pillar, LJ.a, 0, 0, 0, fogAt(SB.Z[n]))); c.strokeStyle = INK; c.lineWidth = 1.2; c.stroke(); if (T.level === 2) { c.strokeStyle = '#ffd23f'; c.beginPath(); c.moveTo((QX[0] + QX[3]) / 2, (QY[0] + QY[3]) / 2); c.lineTo((QX[1] + QX[2]) / 2, (QY[1] + QY[2]) / 2); c.stroke(); } }
  }

  // ---------- boxes in perspective: every vehicle is built from a few ----------
  var BX = new Float64Array(8), BY = new Float64Array(8), BK = { l: 0, r: 0, t: 0, b: 0 };
  var PAL = {};
  function shade(hex, k) {
    var n = parseInt(hex.slice(1), 16), r = n >> 16 & 255, g = n >> 8 & 255, b = n & 255;
    function f(v) { return Math.round(clamp(k > 1 ? v + (255 - v) * (k - 1) : v * k, 0, 255)); }
    return 'rgb(' + f(r) + ',' + f(g) + ',' + f(b) + ')';
  }
  function pal(hex) {
    var q = PAL[hex];
    if (!q) q = PAL[hex] = { back: shade(hex, 0.82), top: shade(hex, 1.12), side: shade(hex, 0.55) };
    return q;
  }
  var WHITE = { back: '#ffffff', top: '#ffffff', side: '#f0f0f0' };
  // o = { x, Z (rear), L, yaw, lift, fl }; local box x0..x1 across, y0..y1 up, z0..z1 forward from the rear
  // Faces are drawn only when they face the camera (by their winding on screen), so a box reads
  // right from behind, from above, from below in a loop or upside down in a corkscrew.
  function box(o, x0, x1, y0, y1, z0, z1, col) {
    var P2 = o.fl ? WHITE : (typeof col === 'string' ? pal(col) : col), bad = false;
    for (var i = 0; i < 8; i++) {
      var lx = i & 1 ? x1 : x0, ly = i & 2 ? y1 : y0, lz = i & 4 ? z1 : z0;
      proj(o.x + lx + (lz - o.L * 0.5) * o.yaw, ly + o.lift, o.Z + lz);
      BX[i] = PJ.x; BY[i] = PJ.y; if (PBAD) bad = true;
    }
    BK.vis = false;
    if (bad) return;
    face(0, 4, 6, 2, P2.side, 1);   // left
    face(1, 5, 7, 3, P2.side, -1);  // right
    face(2, 3, 7, 6, P2.top, -1);   // top
    face(0, 1, 5, 4, P2.side, 1);   // underside
    face(4, 5, 7, 6, P2.back, 1);   // front
    BK.vis = face(0, 1, 3, 2, P2.back, -1); // back
  }
  function face(a, b, d, e, col, sgn) {
    var ax = BX[a], ay = BY[a], bx = BX[b] - ax, by = BY[b] - ay, dx = BX[d] - ax, dy = BY[d] - ay, ex = BX[e] - ax, ey = BY[e] - ay;
    var area = bx * dy - by * dx + dx * ey - dy * ex;
    if (area * sgn <= 0) return false;
    c.fillStyle = col; c.beginPath(); c.moveTo(ax, ay); c.lineTo(BX[b], BY[b]); c.lineTo(BX[d], BY[d]); c.lineTo(BX[e], BY[e]); c.closePath(); c.fill();
    return true;
  }
  // a rectangle on the last box's back face: u across (0 left), v up (0 bottom)
  function back(u0, u1, v0, v1, col) { if (BK.vis) patch(0, 1, 3, 2, u0, u1, v0, v1, col); }
  // a patch on one face of the last box, by bilinear corners (a: u0v0, b: u1v0, d: u1v1, e: u0v1)
  function patch(a, b, d, e, u0, u1, v0, v1, col) {
    function pt(u, v) {
      return [(1 - u) * (1 - v) * BX[a] + u * (1 - v) * BX[b] + u * v * BX[d] + (1 - u) * v * BX[e],
              (1 - u) * (1 - v) * BY[a] + u * (1 - v) * BY[b] + u * v * BY[d] + (1 - u) * v * BY[e]];
    }
    var p1 = pt(u0, v0), p2 = pt(u1, v0), p3 = pt(u1, v1), p4 = pt(u0, v1);
    c.fillStyle = col; c.beginPath(); c.moveTo(p1[0], p1[1]); c.lineTo(p2[0], p2[1]); c.lineTo(p3[0], p3[1]); c.lineTo(p4[0], p4[1]); c.closePath(); c.fill();
  }
  var TOP = [2, 3, 7, 6], LEFT = [0, 4, 6, 2], RIGHT = [1, 5, 7, 3];
  function topPatch(u0, u1, v0, v1, col) { if (V.ch > 0) patch(2, 3, 7, 6, u0, u1, v0, v1, col); }
  function shadowOf(o, w, L) {
    if (T.gapAt(dOfZ(o.Z + L * 0.5))) return; // nothing under it to throw a shadow on
    proj(o.x - w / 2 - 2, 0, o.Z - 2); var ax = PJ.x, ay = PJ.y; proj(o.x + w / 2 + 2, 0, o.Z - 2); var bx = PJ.x;
    proj(o.x + w / 2 + 2 + L * o.yaw, 0, o.Z + L); var cx2 = PJ.x, cy = PJ.y; proj(o.x - w / 2 - 2 + L * o.yaw, 0, o.Z + L); var dx2 = PJ.x;
    var sa0 = c.globalAlpha;
    c.drawImage(shadowImg(), Math.min(ax, dx2) - (bx - ax) * 0.25, cy - (ay - cy) * 0.35, Math.max(bx, cx2) - Math.min(ax, dx2) + (bx - ax) * 0.5, (ay - cy) * 1.7);
    c.globalAlpha = sa0 * 0.6;
    c.fillStyle = 'rgba(40,18,80,0.4)'; c.beginPath(); c.moveTo(ax, ay); c.lineTo(bx, ay); c.lineTo(cx2, cy); c.lineTo(dx2, cy); c.closePath(); c.fill();
    c.globalAlpha = sa0;
  }
  function wheels4(o, w, L, inset) {
    var hw = w / 2, t = inset || 0;
    box(o, -hw - 1, -hw + 5, 0, 8, L * 0.1, L * 0.26, '#0d0d0f'); box(o, hw - 5, hw + 1, 0, 8, L * 0.1, L * 0.26, '#0d0d0f');
    box(o, -hw - 1 + t, -hw + 5, 0, 8, L * 0.72, L * 0.88, '#0d0d0f'); box(o, hw - 5, hw + 1 - t, 0, 8, L * 0.72, L * 0.88, '#0d0d0f');
  }
  // flat red blocks on the back: no glow
  function taillights(u0, u1, v0, v1) {
    back(u0, u1, v0, v1, '#d83a3a'); back(1 - u1, 1 - u0, v0, v1, '#d83a3a');
  }

  // ---------- the vehicles, seen from behind: plain boxes, flat colours ----------
  function frameOf(e, rdt) {
    var L = e.h * ZK, Z = zOf(e.y + e.h / 2);
    if (e._px === undefined) e._px = e.x;
    var vx = rdt > 0 ? (e.x - e._px) / rdt : 0; e._px = e.x;
    e._yaw = (e._yaw || 0) + (clamp(vx / 900, -0.18, 0.18) - (e._yaw || 0)) * Math.min(1, rdt * 10);
    return { x: e.x, Z: Z, L: L, yaw: e._yaw, lift: 0, fl: e.flash > 0 };
  }
  function fadeFor(Z, age) {
    var a = clamp((OBJ_FAR - Z) / 500, 0, 1);
    if (age !== undefined && !RCAM) a *= clamp(age * 3, 0, 1);
    if (!RCAM && Z < V.zp * 0.75) a *= clamp(Z / (V.zp * 0.75), 0.25, 1);
    return a;
  }

  function drawEnt(e, t, rdt) {
    var k = e.kind, o = frameOf(e, rdt), L = o.L, w = e.w, hw = w / 2;
    if (o.Z < ZN + 6) return;
    var al = fadeFor(o.Z, e.age);
    if (al <= 0.01) return;
    c.globalAlpha = al;
    if (k === 'crate' || k === 'wrench') { pickup(e, o, t); c.globalAlpha = 1; return; }
    if (k === 'barrel') { barrel(e, o); c.globalAlpha = 1; return; }
    if (k === 'heli') { drawHeli(e, o, t, al); c.globalAlpha = 1; return; }
    if (k === 'hench') { drawHenchDrop(e, o, t, al); c.globalAlpha = 1; return; }
    if (k === 'hog') { drawHog(e, o, t, al); c.globalAlpha = 1; return; }
    var sk = spriteKey(e, t);
    if (sk && IMG[sk]) {
      if (k !== 'moto') shadowOf({ x: o.x, Z: o.Z + L * 0.18, yaw: o.yaw }, w, L * 0.64);
      var rot = upAt(o.x, o.Z + L * 0.5);
      if (!PBAD) {
        var psi = clamp(Math.atan(o.yaw) * 57.3 * 3, -30, 30);
        if (k === 'moto') { psi = clamp(e.lean * 30, -30, 30); rot += e.lean * 0.3; }
        psi += viewPsi(o.x, o.Z + L * 0.5);
        var ebx = PJ.x, eby = PJ.y, ebr = rot;
        if (partsReady(sk)) {
          // 1.4: the rammer and the gunship in parts too: spinning wheels, a body on springs that
          // leans as they swerve, kicks when they're hit and shudders while a rammer revs to charge
          var su = e._sus || (e._sus = newSus()), edt = Math.min(0.05, RDT * ((CURS && CURS.timeScale) || 1)), spd = CURS ? CURS.speed : 400;
          if (o.fl && !e._fl0) jolt(su, -6, (Math.random() - 0.5) * 1.6);
          e._fl0 = o.fl;
          if (e.st === 'rev') { su.vr += (Math.random() - 0.5) * 0.7; su.vh += (Math.random() - 0.5) * 2; }
          su.spin -= (9 + spd * 0.03) * edt;
          stepSus(su, edt, (CURS ? CURS.dist : 0) - e.y, spd, -o.yaw * 0.3, false, e.st === 'brake', 30);
          carParts(sk, psi, PJ.x, PJ.y, PJ.s, rot, su, clamp(o.yaw * 80, -12, 12), o.fl, 1, 0);
          ebx = BODY.x; eby = BODY.y; ebr = BODY.rot;
        } else sprite(sk, psi, PJ.x, PJ.y, PJ.s, rot, o.fl);
        // soot and scratches build up as a foe takes damage
        if (E.KIND[k].foe && e.hp < e.max && k !== 'moto' && !o.fl) { c.globalAlpha = al * clamp((1 - e.hp / e.max) * 1.15, 0, 0.85); sprite(sk, psi, ebx, eby, PJ.s, ebr, 3); c.globalAlpha = al; }
        carLights(e, o, k, al);
        if (k === 'boss' && T.level === 2 && e.st !== 'flee') drawCoils(e, o);
        if (k === 'boss' && T.level === 3) drawCarrier(e, o, t, al);
      }
      hpBar(e, k, w, SPR_TOP[k] || 26, o);
      c.globalAlpha = 1; return;
    }
    if (k === 'moto') { moto(e, o); c.globalAlpha = 1; return; }
    shadowOf(o, w, L);
    var top = 22;
    if (k === 'truck' || k === 'boss') {
      var big = k === 'boss', bodyH = big ? 58 : 44;
      top = bodyH;
      wheels4(o, w, L);
      box(o, -hw + 2, hw - 2, 5, big ? 40 : 32, L * 0.8, L, big ? '#3a3d45' : '#5d6b4c'); // cab
      box(o, -hw, hw, 6, bodyH, 0, L * 0.78, big ? '#2c2f36' : '#4c5a3f');
      taillights(0.06, 0.18, 0.06, 0.14);
      if (big) {
        // the turret, aimed back at you
        box(o, -14, 14, bodyH, bodyH + 10, L * 0.18, L * 0.42, '#4a4e58');
        o.yaw = -(e.aim || 0) * 0.5;
        box(o, -10, -6, bodyH + 4, bodyH + 7, L * 0.18 - 46, L * 0.2, '#6d717a');
        box(o, 6, 10, bodyH + 4, bodyH + 7, L * 0.18 - 46, L * 0.2, '#6d717a');
        o.yaw = e._yaw;
      }
    } else if (k === 'ramp') {
      top = 32;
      wheels4(o, w, L);
      box(o, -hw + 2, hw - 2, 4, 32, L * 0.9, L, '#e0a526'); // cab
      box(o, -hw, hw, 8, 15, L * 0.24, L * 0.9, '#6a6f7a'); // deck
      wedge(o, -hw, hw, o.Z, o.Z + L * 0.24, 15);           // the ramp itself
    } else {
      var col = CIV[e.col % 8], bodyTop = 13, cabTop = 22, cz0 = 0.28, cz1 = 0.7, cw = 0.4;
      if (k === 'rammer') {
        // revving to ram: the body blinks pale, so you can still read the threat
        col = (e.st === 'rev' && Math.floor(t * 16) % 2) || e.st === 'charge' ? '#ff9a9e' : '#e8262f';
        bodyTop = 10; cabTop = 17; cz0 = 0.36;
      }
      else if (k === 'suv') { col = '#2a2d35'; bodyTop = 17; cabTop = 28; cz0 = 0.18; cz1 = 0.86; cw = 0.44; }
      else if (k === 'van') { bodyTop = 15; cabTop = 31; cz0 = 0.02; cz1 = 0.8; cw = 0.46; }
      else if (k === 'hatch') { cz0 = 0.1; cz1 = 0.66; cabTop = 23; }
      else if (k === 'bus') { col = '#e8b923'; bodyTop = 46; }
      top = k === 'bus' ? bodyTop : cabTop;
      wheels4(o, w, L);
      box(o, -hw, hw, 4, bodyTop, 0, L, col);
      taillights(0.05, 0.2, k === 'bus' ? 0.12 : 0.5, k === 'bus' ? 0.22 : 0.8);
      if (k !== 'bus') {
        box(o, -hw * cw * 2, hw * cw * 2, bodyTop, cabTop, L * cz0, L * cz1, col);
        back(0.12, 0.88, 0.18, 0.82, '#2b3340'); // rear window
        if (k === 'suv') {
          box(o, -5, 5, cabTop, cabTop + 6, L * 0.5, L * 0.62, '#4a4f5a');
          var oy2 = o.yaw; o.yaw = -(e.aim || 0) * 0.6;
          box(o, -1.5, 1.5, cabTop + 3, cabTop + 5, L * 0.5 - 20, L * 0.52, '#16181c');
          o.yaw = oy2;
        }
      }
    }
    hpBar(e, k, w, top, o);
    c.globalAlpha = 1;
  }
  // ---------- 1.3: the Syndicate chopper and its henchmen ----------
  var CURS = null;
  function findEnt(id) { if (!CURS) return null; for (var i = 0; i < CURS.ents.length; i++) if (CURS.ents[i].id === id) return CURS.ents[i]; return null; }
  var SUIT = '#2b1d4a', SKIN = '#f2b48f', TIE = '#ff4fb8', SHIRT = '#fff6ea';
  function limb(x0, y0, x1, y1, w, col) {
    c.strokeStyle = INK; c.lineWidth = w + 1.3; c.beginPath(); c.moveTo(x0, y0); c.lineTo(x1, y1); c.stroke();
    c.strokeStyle = col; c.lineWidth = w; c.stroke();
  }
  // a Syndicate henchman (dark purple suit, pink tie, shades), in game units at s px per unit.
  // pose 'rope': hanging from a rope, hands at x,y. 'cling': hands on the KESTREL's roof edge at x,y,
  // body swinging out on side k, legs kicking at the road (punch: one fist through the window).
  // 'rag': tumbling, centred on x,y.
  function hench(x, y, s, pose, k, t, rot, punch) {
    if (s < 0.25) return;
    c.save(); c.translate(x, y); if (rot) c.rotate(rot); c.scale(s * 0.9, s * 0.9);
    c.lineCap = 'round'; c.lineJoin = 'round';
    var sw = Math.sin(t * 7), sh, hip, hd, hands, feet;
    k = k || 1;
    if (pose === 'rope') {
      sh = [0, 3.2]; hip = [sw * 0.5, 10.5]; hd = [0.3, 1.2];
      hands = [[-0.8, 0], [0.8, -0.6]]; feet = [[-1.6 + sw, 18.5], [1.4 + sw * 0.6, 18.2]];
    } else if (pose === 'cling') {
      sh = [k * 2.6, 3.2]; hip = [k * 5.2, 10.5]; hd = [k * 1.3, 1.0];
      hands = [[k * -0.6, 0], punch ? [k * -6.5, 4.5] : [k * 0.8, -0.2]];
      feet = [[k * (6.5 + sw * 1.8), 18.2], [k * (8.8 - sw * 1.8), 17.4]];
    } else {
      sh = [0, -3.5]; hip = [0, 3.5]; hd = [0, -6.6];
      hands = [[-5.5, -6.5 + sw], [5.6, -2 - sw]]; feet = [[-4, 10 + sw], [4.5, 9.5 - sw]];
    }
    var i;
    for (i = 0; i < 2; i++) limb(hip[0], hip[1], feet[i][0], feet[i][1], 2.3, SUIT);
    c.fillStyle = INK;
    for (i = 0; i < 2; i++) { c.beginPath(); c.arc(feet[i][0], feet[i][1], 1.3, 0, 6.283); c.fill(); }
    limb(sh[0], sh[1], hip[0], hip[1], 4.6, SUIT);
    c.strokeStyle = SHIRT; c.lineWidth = 1.3; c.beginPath(); c.moveTo(sh[0], sh[1] + 0.3); c.lineTo(sh[0] + (hip[0] - sh[0]) * 0.4, sh[1] + (hip[1] - sh[1]) * 0.4); c.stroke();
    c.strokeStyle = TIE; c.lineWidth = 0.7; c.stroke();
    for (i = 0; i < 2; i++) limb(sh[0], sh[1], hands[i][0], hands[i][1], 1.9, SUIT);
    c.strokeStyle = INK; c.lineWidth = 0.6;
    for (i = 0; i < 2; i++) { c.fillStyle = SKIN; c.beginPath(); c.arc(hands[i][0], hands[i][1], 1.05, 0, 6.283); c.fill(); c.stroke(); }
    c.fillStyle = SKIN; c.lineWidth = 0.8; c.beginPath(); c.arc(hd[0], hd[1], 2.2, 0, 6.283); c.fill(); c.stroke();
    c.fillStyle = INK; c.beginPath(); c.arc(hd[0], hd[1] - 0.5, 2.3, Math.PI * 1.05, Math.PI * 1.95); c.fill();
    c.fillStyle = '#0d0b14'; c.fillRect(hd[0] - 1.8, hd[1] - 0.1, 3.6, 0.9);
    c.fillStyle = '#7ff7ff'; c.fillRect(hd[0] - 1.4, hd[1], 0.8, 0.3);
    c.restore();
  }
  // a pulsing pink target on the road where a henchman is about to land
  function roadMark(X, Z, r, a) {
    proj(X, 0.6, Z); if (PBAD) return; var x = PJ.x, y = PJ.y;
    proj(X + r, 0.6, Z); var rx = Math.hypot(PJ.x - x, PJ.y - y);
    proj(X, 0.6, Z + r * ZK); var ry = Math.max(1, Math.hypot(PJ.x - x, PJ.y - y));
    var ro = Math.atan2(PJ.y - y, PJ.x - x) + Math.PI / 2;
    c.save(); c.translate(x, y); c.rotate(ro - Math.PI / 2 + Math.PI / 2);
    c.globalAlpha = a; c.strokeStyle = '#ff4fb8'; c.lineWidth = Math.max(1.5, rx * 0.12);
    c.beginPath(); c.ellipse(0, 0, rx, ry, 0, 0, 6.283); c.stroke();
    c.beginPath(); c.moveTo(-rx * 1.3, 0); c.lineTo(-rx * 0.5, 0); c.moveTo(rx * 0.5, 0); c.lineTo(rx * 1.3, 0); c.stroke();
    c.restore();
    addOn(); glow(x, y, rx * 1.6, '255,79,184', a * 0.5, ry / Math.max(rx, 1)); addOff();
    c.globalAlpha = 1;
  }
  function drawHenchDrop(e, o, t, al) {
    var Z = zOf(e.y), u = clamp(e.t / (e.T || 1.25), 0, 1);
    if (e.st === 'drop' && e.tx !== undefined) roadMark(e.tx, zOf(e.ty2), 17 - 7 * u, al * (0.55 + 0.45 * Math.sin(t * 16)));
    var rot = upAt(e.x, Z);
    proj(e.x, e.alt || 0, Z); if (PBAD) return;
    var x = PJ.x, y = PJ.y, s = PJ.s, heli = findEnt(e.heli);
    if (heli) {
      proj(heli.x, (heli.alt || 0) + 2, zOf(heli.y + heli.h / 2) + heli.h * ZK * 0.5);
      if (!PBAD) { c.globalAlpha = al; c.strokeStyle = INK; c.lineWidth = Math.max(1, 0.9 * s); c.beginPath(); c.moveTo(PJ.x, PJ.y); c.lineTo(x, y); c.stroke(); }
    }
    c.globalAlpha = al;
    hench(x, y, s, 'rope', 1, t + e.id, rot);
  }
  function drawHeli(e, o, t, al) {
    var Zc = o.Z + o.L * 0.5, alt = e.alt || 0;
    // the shadow on the road under it, fainter the higher it flies
    if (!T.gapAt(dOfZ(Zc))) {
      proj(e.x, 0.3, Zc);
      if (!PBAD) { c.globalAlpha = al * clamp(1 - alt / 260, 0.15, 0.6); c.drawImage(shadowImg(), PJ.x - 46 * PJ.s, PJ.y - 10 * PJ.s, 92 * PJ.s, 20 * PJ.s); }
    }
    var rot = upAt(e.x, Zc) + (e.lean || 0) * 0.22;
    var want = (e.st === 'hover' ? (e.cx > 0 ? -38 : 38) : 0) + (e.lean || 0) * 14;
    e._psi = e._psi === undefined ? want : e._psi + (want - e._psi) * easeK(0.06);
    var psi = e._psi + viewPsi(e.x, Zc);
    proj(e.x, alt, Zc); if (PBAD) return;
    var x = PJ.x, y = PJ.y, s = PJ.s;
    c.globalAlpha = al;
    if (IMG.heli) {
      sprite('heli', psi, x, y, s, rot, o.fl);
      if (e.hp < e.max && !o.fl) { c.globalAlpha = al * clamp((1 - e.hp / e.max) * 1.1, 0, 0.85); sprite('heli', psi, x, y, s, rot, 3); c.globalAlpha = al; }
    } else {
      c.fillStyle = '#7b3bd6'; c.strokeStyle = INK; c.lineWidth = 2; c.beginPath(); c.ellipse(x, y - 10 * s, 24 * s, 10 * s, 0, 0, 6.283); c.fill(); c.stroke();
    }
    // the rotor: a blurred disc with two smeared blades and yellow tips
    proj(e.x, alt + 18, Zc); var hx = PJ.x, hy = PJ.y;
    proj(e.x + 46, alt + 18, Zc); var rx = Math.hypot(PJ.x - hx, PJ.y - hy);
    proj(e.x, alt + 18, Zc + 46); var ry = Math.max(rx * 0.06, Math.hypot(PJ.x - hx, PJ.y - hy));
    if (!PBAD && rx > 2) {
      c.save(); c.translate(hx, hy); c.rotate(rot);
      c.globalAlpha = al * 0.2; c.fillStyle = INK; c.beginPath(); c.ellipse(0, 0, rx, ry, 0, 0, 6.283); c.fill();
      c.lineCap = 'round';
      for (var b = 0; b < 2; b++) for (var sm = 0; sm < 3; sm++) {
        var ang = t * 31 + b * Math.PI - sm * 0.16;
        c.globalAlpha = al * (0.55 - sm * 0.15); c.strokeStyle = INK; c.lineWidth = Math.max(1.2, 2.4 * s);
        c.beginPath(); c.moveTo(-Math.cos(ang) * rx, -Math.sin(ang) * ry); c.lineTo(Math.cos(ang) * rx, Math.sin(ang) * ry); c.stroke();
      }
      c.globalAlpha = al * 0.4; c.strokeStyle = '#ffd23f'; c.lineWidth = Math.max(1, 1.1 * s); c.beginPath(); c.ellipse(0, 0, rx * 0.97, ry * 0.97, 0, 0, 6.283); c.stroke();
      c.restore();
    }
    // the red beacon, the strobe, and at night a searchlight cone onto the KESTREL
    addOn();
    proj(e.x, alt - 1, Zc); if (!PBAD && Math.sin(t * 9 + e.id) > 0.2) glow(PJ.x, PJ.y, 12 * s + 3, '255,40,60', al * 0.9);
    if (Math.sin(t * 13 + e.id * 2) > 0.85) { proj(e.x, alt + 12, Zc - 30); if (!PBAD) glow(PJ.x, PJ.y, 10 * s + 3, '255,255,255', al); }
    if (e.st === 'hover' && CURS && CURS.p) {
      var pz = zOf(CURS.p.y - 8); proj(e.x, alt - 4, Zc - 6); var sx0 = PJ.x, sy0 = PJ.y, sbad = PBAD;
      proj(CURS.p.x + Math.sin(TNOW * 1.7 + e.id) * 14, 0.5, pz);
      if (!sbad && !PBAD) {
        var tx = PJ.x, ty = PJ.y, ww = Math.max(6, 30 * PJ.s);
        var gr = c.createLinearGradient(sx0, sy0, tx, ty);
        gr.addColorStop(0, 'rgba(255,245,210,' + (LV2 ? 0.5 : DUSK ? 0.35 : 0.18) * al + ')'); gr.addColorStop(1, 'rgba(255,245,210,0)');
        c.fillStyle = gr; c.beginPath(); c.moveTo(sx0 - 2, sy0); c.lineTo(tx - ww, ty); c.lineTo(tx + ww, ty); c.lineTo(sx0 + 2, sy0); c.closePath(); c.fill();
        glow(tx, ty, ww * 1.2, '255,245,210', (LV2 ? 0.45 : DUSK ? 0.3 : 0.15) * al, 0.35);
      }
    }
    addOff();
    // armor bar once it's been hit
    if (e.hp < e.max) {
      proj(e.x, alt + 34, Zc);
      if (!PBAD) { var bw2 = Math.max(24, 40 * PJ.s), bh = Math.max(3, 1.6 * PJ.s); c.globalAlpha = al; c.fillStyle = INK; c.fillRect(PJ.x - bw2 / 2 - 1, PJ.y - bh - 1, bw2 + 2, bh + 2); c.fillStyle = '#ff4fb8'; c.fillRect(PJ.x - bw2 / 2, PJ.y - bh, bw2 * Math.max(0, e.hp) / e.max, bh); }
    }
  }
  // 1.5: a SNAKE EYES biker on his chopper (an Unreal sprite). It leans as he swerves; flying off the
  // carrier it rides up off the road over a pulsing landing mark; winding up, a chain whirls over his head
  function drawHog(e, o, t, al) {
    var Zc = o.Z + o.L * 0.5, alt = e.alt || 0, fly = e.st === 'fly', sk = spriteKey(e, t);
    if (fly && e.tx !== undefined) roadMark(e.tx, zOf(e.ty2), 15, al * (0.55 + 0.45 * Math.sin(t * 16)));
    if (!T.gapAt(dOfZ(Zc))) {
      proj(e.x, 0.3, Zc);
      if (!PBAD) { c.globalAlpha = al * clamp(0.6 - alt / 110, 0.15, 0.6); c.drawImage(shadowImg(), PJ.x - 11 * PJ.s, PJ.y - 5 * PJ.s, 22 * PJ.s, 10 * PJ.s); }
    }
    var rot = upAt(e.x, Zc) + (e.lean || 0) * 0.3 + (fly ? Math.sin(t * 8 + e.id) * 0.1 : 0);
    var psi = clamp((e.lean || 0) * 38, -45, 45) + viewPsi(e.x, Zc);
    proj(e.x, alt, Zc); if (PBAD) return;
    var x = PJ.x, y = PJ.y, s = PJ.s;
    c.globalAlpha = al;
    if (sk && IMG[sk]) {
      sprite(sk, psi, x, y, s, rot, o.fl);
      if (e.hp < e.max && !o.fl) { c.globalAlpha = al * 0.6; sprite(sk, psi, x, y, s, rot, 3); c.globalAlpha = al; }
    } else { moto(e, o); return; }
    // his tail light
    addOn(); proj(e.x, alt + 7, o.Z + 1); if (!PBAD) glow(PJ.x, PJ.y, 8 * PJ.s + 1.5, '255,50,70', al * 0.75); addOff();
    if (e.st === 'wind') {
      // the chain: a whirling loop over his head, a glint at its end, a red warning round him
      proj(e.x, alt + 27, Zc); if (PBAD) return;
      var hx = PJ.x, hy = PJ.y, r = 10 * s, ang = t * 22 + e.id, pulse = 0.5 + 0.5 * Math.sin(t * 20);
      addOn(); glow(x, y - 12 * s, 26 * s + 4, '255,40,60', al * (0.3 + 0.35 * pulse), 0.7); addOff();
      c.save(); c.translate(hx, hy); c.rotate(rot);
      c.strokeStyle = INK; c.lineWidth = Math.max(1.5, 1.6 * s); c.beginPath(); c.ellipse(0, 0, r, r * 0.32, 0, 0, 6.283); c.stroke();
      c.strokeStyle = '#d7e3f0'; c.lineWidth = Math.max(1, 0.9 * s); c.setLineDash([Math.max(1.5, 1.4 * s), Math.max(1, 0.8 * s)]);
      c.beginPath(); c.ellipse(0, 0, r, r * 0.32, 0, ang % 6.283, ang % 6.283 + 4.2); c.stroke(); c.setLineDash([]);
      var ex = Math.cos(ang) * r, ey = Math.sin(ang) * r * 0.32;
      c.strokeStyle = '#d7e3f0'; c.beginPath(); c.moveTo(e.side < 0 ? 3 * s : -3 * s, 4 * s); c.lineTo(ex, ey); c.stroke();
      addOn(); glow(ex, ey, 5 * s + 2, '255,255,255', al * 0.9); addOff();
      c.restore();
      c.globalAlpha = al * (0.6 + 0.4 * pulse); c.fillStyle = '#ff3355'; c.font = font(Math.max(10, 9 * s), 900); c.textAlign = 'center';
      c.fillText('!', hx, hy - 8 * s);
    }
  }
  // the carrier's lights: its tail lights flare and blink when it brakes, then glare while it drops back
  // onto you; the decks' neon pulses; a light at the top of the kicker the jumpers fly off
  function drawCarrier(e, o, t, al) {
    var bt = typeof e.brake === 'number' ? e.brake : e.brake ? e.brake.t : -1, Zr = o.Z + 20 * ZK;
    addOn();
    for (var sd = -1; sd <= 1; sd += 2) {
      proj(o.x + sd * 27, 8, Zr); if (PBAD) continue;
      var on = bt >= 0 && (bt > 0.8 || Math.floor(t * 12) % 2 === 0);
      glow(PJ.x, PJ.y, (on ? 34 : 12) * PJ.s + 2, on ? '255,30,40' : '255,50,70', al * (on ? 1 : 0.6));
    }
    if (bt >= 0) { proj(o.x, 2, Zr); if (!PBAD) glow(PJ.x, PJ.y, 90 * PJ.s, '255,30,40', al * (bt > 0.8 ? 0.45 : 0.3), 0.35); }
    var pz = 0.55 + 0.45 * Math.sin(t * 4);
    for (var k = -1; k <= 1; k += 2) { proj(o.x + k * 30, 33, Zr + 40 * ZK); if (!PBAD) glow(PJ.x, PJ.y, 18 * PJ.s + 2, '255,79,184', al * 0.4 * pz, 0.5); }
    proj(o.x, 37, Zr); if (!PBAD) glow(PJ.x, PJ.y, 14 * PJ.s + 2, '255,210,63', al * (0.4 + 0.3 * Math.sin(t * 9)), 0.6);
    addOff();
    if (bt > 0.8 && bt < 2.2 && hit(0.6)) sparks(e.x + (Math.random() - 0.5) * 40, e.y + e.h / 2 + 24, 2, '#ffd23f', 1);
  }
  // henchmen hanging off the KESTREL's doors
  function drawClingers(p, Z, L, lift, prot, t) {
    var Lt = p.latch;
    if (!Lt || !Lt.length) return;
    for (var i = 0; i < Lt.length; i++) {
      var q = Lt[i], sd = q.side;
      proj(p.x + sd * 13 - (p.yaw || 0) * 8, lift + 12.5, Z + L * 0.52);
      if (PBAD) continue;
      hench(PJ.x, PJ.y, PJ.s, 'cling', sd, t * 1.3 + i * 2, prot + (p.roll || 0), q.hitT < 0.3);
    }
  }

  // tail lights (and a rammer's angry glow as it revs)
  function carLights(e, o, k, al) {
    if (k === 'ramp') return;
    var top = SPR_TOP[k] || 26, hy = top * (k === 'bus' ? 0.18 : k === 'truck' || k === 'boss' ? 0.2 : 0.4), sp = k === 'moto' ? 0 : e.w * 0.36;
    var rev = k === 'rammer' && (e.st === 'rev' || e.st === 'charge'), night = LV2 ? 1 : DUSK ? 0.85 : 0.55, a = al * night;
    addOn();
    for (var sd = -1; sd <= 1; sd += 2) {
      if (!sp && sd > 0) break;
      proj(o.x + sd * sp, hy, o.Z + 1);
      if (PBAD) continue;
      glow(PJ.x, PJ.y, (rev ? 22 : 9) * PJ.s + 1.5, rev ? '255,40,40' : '255,50,70', rev ? al * 0.95 : a * 0.75);
    }
    if (rev) { proj(o.x, 0, o.Z + o.L * 0.5); if (!PBAD) glow(PJ.x, PJ.y, 40 * PJ.s, '255,40,40', al * 0.35, 0.35); }
    if (k === 'suv' && e.burst > 0) { proj(o.x, top + 4, o.Z + o.L * 0.5); if (!PBAD) glow(PJ.x, PJ.y, 16 * PJ.s, '255,220,120', al * 0.8); }
    addOff();
  }
  // health bar for foes that took a hit
  function hpBar(e, k, w, top, o) {
    if (!(E.KIND[k].foe && e.hp < e.max && k !== 'boss')) return;
    proj(e.x, top + 10, o.Z);
    var bw2 = Math.max(18, w * PJ.s), bh = Math.max(3, 3 * PJ.s * 0.5);
    c.fillStyle = INK; c.fillRect(PJ.x - bw2 / 2 - 1, PJ.y - bh - 1, bw2 + 2, bh + 2);
    c.fillStyle = '#ff4fb8'; c.fillRect(PJ.x - bw2 / 2, PJ.y - bh, bw2 * Math.max(0, e.hp) / e.max, bh);
  }
  // which Unreal sprite a road user wears
  var SPR_TOP = { rammer: 22, suv: 36, truck: 50, boss: 72, bus: 48, ramp: 34, moto: 40, hog: 30 };
  var CIV_OF = { sedan: [0, 3, 6], hatch: [1, 4, 7], van: [2, 5] };
  function spriteKey(e, t) {
    var k = e.kind;
    if (k === 'rammer') return (e.st === 'rev' && Math.floor(t * 16) % 2) || e.st === 'charge' ? 'rammer_rev' : 'rammer';
    if (CIV_OF[k]) { var l = CIV_OF[k]; return 'civ' + l[(e.col || 0) % l.length]; }
    if (k === 'boss' && T.level === 2 && SP.boss2) return 'boss2';
    if (k === 'boss' && T.level === 3 && SP.boss3) return 'boss3';
    if (k === 'hog') return (e.col || 0) % 2 && SP.hog2 ? 'hog2' : SP.hog ? 'hog' : null;
    if (SP[k]) return k;
    return null;
  }
  function wedge(o, x0, x1, Za, Zb, h) {
    proj(o.x + x0, 0, Za); var a1 = PJ.x, a2 = PJ.y; proj(o.x + x1, 0, Za); var b1 = PJ.x;
    proj(o.x + x1, h, Zb); var c1 = PJ.x, c2 = PJ.y; proj(o.x + x0, h, Zb); var d1 = PJ.x;
    c.fillStyle = o.fl ? '#fff' : '#ffd23f'; c.beginPath(); c.moveTo(a1, a2); c.lineTo(b1, a2); c.lineTo(c1, c2); c.lineTo(d1, c2); c.closePath(); c.fill();
  }
  // which way is up on screen at a spot on the road (the road can be banked, looped or upside down)
  function upAt(X, Z) {
    proj(X, 10, Z); var bx = PJ.x, by = PJ.y;
    proj(X, 0, Z);
    return Math.atan2(bx - PJ.x, -(by - PJ.y));
  }
  // a biker: a tyre, a body and a round head
  function moto(e, o) {
    var rot = upAt(o.x, o.Z);
    if (PBAD) return;
    var s = PJ.s, x = PJ.x, y = PJ.y;
    c.save(); c.translate(x, y); c.rotate(rot);
    c.fillStyle = 'rgba(0,0,0,0.4)'; c.beginPath(); c.ellipse(0, 0, 8 * s, 2.2 * s, 0, 0, 6.283); c.fill();
    c.scale(s, s); c.rotate(e.lean * 0.35);
    var fl = e.flash > 0;
    c.fillStyle = fl ? '#fff' : '#1a1a1d'; c.fillRect(-2.6, -14, 5.2, 14);   // tyre
    c.fillStyle = fl ? '#fff' : '#3a3e46'; c.fillRect(-5, -21, 10, 9);      // bike
    c.fillStyle = fl ? '#fff' : '#b8212b'; c.fillRect(-7, -37, 14, 17);     // rider
    c.fillStyle = fl ? '#fff' : '#22242a'; c.beginPath(); c.arc(0, -41, 5.2, 0, 6.283); c.fill(); // head
    c.restore();
  }
  function barrel(e, o) {
    var rot = upAt(o.x, o.Z);
    if (PBAD) return;
    var s = PJ.s;
    c.save(); c.translate(PJ.x, PJ.y); c.rotate(rot);
    c.fillStyle = 'rgba(0,0,0,0.35)'; c.beginPath(); c.ellipse(0, 0, 11 * s, 3 * s, 0, 0, 6.283); c.fill();
    c.fillStyle = e.flash > 0 ? '#fff' : T.level === 2 ? '#3fd86a' : '#c0392b'; c.fillRect(-9 * s, -22 * s, 18 * s, 22 * s);
    if (T.level === 2 && !(e.flash > 0)) { c.fillStyle = '#1c1f4a'; c.fillRect(-9 * s, -16 * s, 18 * s, 3 * s); c.fillRect(-9 * s, -8 * s, 18 * s, 3 * s); }
    c.restore();
  }
  function pickup(e, o, t) {
    var bob = 6 + Math.sin(t * 5 + e.id) * 3;
    var rot = upAt(o.x, o.Z);
    if (PBAD) return;
    var s = PJ.s;
    c.save(); c.translate(PJ.x, PJ.y); c.rotate(rot);
    c.fillStyle = 'rgba(0,0,0,0.3)'; c.beginPath(); c.ellipse(0, 0, 11 * s, 3 * s, 0, 0, 6.283); c.fill();
    c.translate(0, -(bob + 11) * s); c.scale(s, s);
    c.fillStyle = e.kind === 'crate' ? '#ff9e3d' : '#57e38a'; c.fillRect(-11, -11, 22, 22);
    c.fillStyle = '#16181c'; c.font = font(14, 900); c.textAlign = 'center'; c.textBaseline = 'middle';
    c.fillText(e.kind === 'crate' ? 'M' : '+', 0, 1);
    c.restore();
  }

  // ---------- the KESTREL-88, from behind: a body, a cabin, two tail lights ----------
  function drawPlayer(S, t) {
    var p = S.p, w = p.w, hw = w / 2, L = p.h * ZK;
    var Z = zOf(p.y + p.h / 2);
    var lift = p.liftH || 0;
    var wrecked = S.phase === 'wreck' || S.outcome === 'wrecked';
    var yawv = p.vx / 300 * 0.12 + (p.drift ? p.drift.dir * 0.1 : 0);
    var o = { x: p.x, Z: Z, L: L, yaw: wrecked ? 0.45 : yawv, lift: lift, fl: false };
    proj(p.x, 0, Z); V.px = PJ.x; V.py = PJ.y;
    // the shadow stays on the road
    c.globalAlpha = Math.max(0.2, 1 - (lift / JUMP) * 0.5); shadowOf({ x: p.x, Z: Z + L * 0.2, yaw: o.yaw }, w + lift * 0.2, L * 0.6); c.globalAlpha = 1;
    var prot = upAt(p.x, Z + L * 0.5), pbad = PBAD;
    // the pink underglow, pooled on the road
    if (!wrecked && !pbad) { proj(p.x, 0, Z + L * 0.45); if (!PBAD) { addOn(); glow(PJ.x, PJ.y, 36 * PJ.s, '255,79,184', (LV2 ? 0.8 : DUSK ? 0.65 : 0.45) * Math.max(0.25, 1 - lift / 60), 0.33); addOff(); } }
    proj(p.x, lift, Z + L * 0.5);
    if (IMG.player && !pbad && !PBAD) {
      var ax = PJ.x, ay = PJ.y, as = PJ.s;
      if (wrecked) {
        sprite('wreck', 0, ax, ay, as, prot);
        if (hit(0.5)) P({ t: 'fire', x: p.x + (Math.random() - 0.5) * 20, y: p.y + (Math.random() - 0.5) * 30, h: 10, vx: 0, vy: -10, vh: 40, life: 0.4, r: 6, g: 0.9 });
        return;
      }
      if (p.inv > 0 && Math.floor(p.inv * 20) % 2 === 0 && FX.shock <= 0) { drawClingers(p, Z, L, lift, prot, t); return; }
      var psi = clamp((p.yaw || 0) * 57.3 * 1.5 + viewPsi(p.x, Z + L * 0.5), -36, 36), rot = prot + (p.roll || 0);
      var sq = p.squash > 0 ? 1 - 0.14 * p.squash * Math.cos((1 - p.squash) * 9) : 1;
      // 1.3: the KESTREL as it takes damage: dented below 70% armor, trashed below 40%, with soot building up
      var pk = p.hp <= 40 && IMG.player_d2 ? 'player_d2' : p.hp <= 70 && IMG.player_d1 ? 'player_d1' : 'player';
      if (FX.flinch > 0.02 && !REDUCED) { ax += Math.sin(t * 97) * 2.4 * FX.flinch * as; ay += Math.cos(t * 83) * 1.2 * FX.flinch * as; }
      var bx = ax, by = ay, brot = rot;
      if (partsReady(pk)) {
        // 1.4: the KESTREL in parts: wheels turning with the road and steering, the body on springs
        var u = PSUS, dts = Math.min(0.05, RDT * (S.timeScale || 1));
        var sIn = p.steerIn !== undefined ? p.steerIn : clamp(p.vx / 300, -1, 1);
        var steer = p.drift ? -p.drift.dir * 14 + sIn * 6 : sIn * 17;
        u.spin -= (p.air > 0 ? 7 : 9 + S.speed * 0.03) * (p.drift || p.boostT > 0 ? 1.3 : 1) * dts;
        stepSus(u, dts, S.dist - p.y, S.speed, (p.roll || 0) * 0.85 + (p.drift ? -p.drift.dir * 0.03 : 0), p.air > 0 || lift > 0.5, p.vy > 60, 31);
        carParts(pk, psi, ax, ay, as, prot, u, steer, false, sq, pk === 'player_d2' ? 0.06 : 0);
        bx = BODY.x; by = BODY.y; brot = BODY.rot;
      } else sprite(pk, psi, ax, ay, as, rot, false, 1, sq);
      if (p.hp < 85) { c.globalAlpha = clamp((85 - p.hp) / 85, 0, 1) * 0.8; sprite(pk, psi, bx, by, as, brot, 3, 1, sq); c.globalAlpha = 1; }
      if (!LOWFX) playerSheen(psi, bx, by, as, brot, sq, p, pk);
      if (FX.shock > 0) { c.globalAlpha = FX.shock * (0.3 + 0.4 * Math.random()); sprite(pk, psi, bx, by, as, brot, true, 1, sq); c.globalAlpha = 1; }
      // the lamps ride on the body
      var hot = p.turboOn || p.boostT > 0, brake = p.vy > 40, tx0 = -(p.yaw || 0) * 14, odx = bx - ax, ody = by - ay;
      addOn();
      for (var tl = -1; tl <= 1; tl += 2) {
        proj(p.x + tl * 10 + tx0, lift + 7, Z + 2);
        if (!PBAD) glow(PJ.x + odx, PJ.y + ody, (brake ? 15 : 8) * PJ.s + 2, '255,40,70', brake ? 0.95 : 0.6);
      }
      // exhaust glow, and the drift charge glowing at the rear wheels
      if (hot) {
        var gcol = p.turboOn ? '255,120,220' : DRGB[p.boostLvl || 1];
        proj(p.x + tx0, lift + 4, Z - 6); if (!PBAD) glow(PJ.x + odx, PJ.y + ody, 26 * PJ.s + 4, gcol, 0.75);
      }
      if (p.drift && p.drift.lvl > 0) {
        for (var dw = -1; dw <= 1; dw += 2) { proj(p.x + dw * 12 + tx0, 1, Z + 4); if (!PBAD) glow(PJ.x, PJ.y, (8 + 4 * p.drift.lvl) * PJ.s, DRGB[p.drift.lvl], 0.8 + 0.2 * Math.random()); }
      }
      addOff();
      // turbo: cel flames out of the four pipes (a drift boost burns in its charge colour)
      if (hot) {
        var fk = 0.75 + Math.random() * 0.5, rz = Z + L * 0.5 - 25, fcol = p.turboOn ? '#ff4fb8' : DCOL[p.boostLvl || 1];
        for (var xi = -1; xi <= 1; xi += 2) for (var xo = 0; xo < 2; xo++) {
          var X = p.x + xi * (3.5 + xo * 2) + tx0;
          proj(X, lift + 3.3, rz); var fx0 = PJ.x + odx, fy0 = PJ.y + ody, fs = PJ.s; if (PBAD) continue;
          proj(X, lift + 3.3, rz - 10 * fk); var fx1 = PJ.x + odx, fy1 = PJ.y + ody;
          var r0 = 1.3 * fs;
          c.fillStyle = fcol; c.strokeStyle = INK; c.lineWidth = 1;
          c.beginPath(); c.ellipse((fx0 + fx1) / 2, (fy0 + fy1) / 2 + r0 * 0.6, r0 * 1.2, Math.max(r0 * 1.2, Math.abs(fy1 - fy0) * 0.5 + r0), 0, 0, 6.283); c.fill(); c.stroke();
          c.fillStyle = '#fff3a8'; c.beginPath(); c.ellipse((fx0 + fx1) / 2, (fy0 + fy1) / 2 + r0 * 0.6, r0 * 0.6, Math.max(r0 * 0.6, Math.abs(fy1 - fy0) * 0.3), 0, 0, 6.283); c.fill();
        }
      }
      // fried: arcs crawl over the car
      if (FX.shock > 0.05) for (var sa = 0; sa < 3; sa++) {
        var spx = 44 * as, spy = 22 * as;
        arcBetween(ax + (Math.random() - 0.5) * spx, ay - Math.random() * spy, ax + (Math.random() - 0.5) * spx, ay - Math.random() * spy, 1.2, '150,230,255', FX.shock);
      }
      // a burning car glows from the engine deck
      if (p.hp < 35) { proj(p.x, lift + 15, Z + L * 0.25); if (!PBAD) { addOn(); glow(PJ.x, PJ.y, (18 + (35 - p.hp)) * PJ.s * 0.5, '255,140,50', 0.55 + 0.25 * Math.random()); addOff(); } }
      drawClingers(p, Z, L, lift, prot, t);
      return;
    }
    if (wrecked) {
      wheels4(o, w, L);
      box(o, -hw, hw, 4, 11, 0, L, '#2a2a2c'); box(o, -hw * 0.76, hw * 0.76, 11, 17, L * 0.3, L * 0.66, '#3a3a3d');
      if (hit(0.5)) P({ t: 'fire', x: p.x + (Math.random() - 0.5) * 20, y: p.y + (Math.random() - 0.5) * 30, h: 10, vx: 0, vy: -10, vh: 40, life: 0.4, r: 6, g: 0.9 });
      return;
    }
    if (p.inv > 0 && Math.floor(p.inv * 20) % 2 === 0) return;
    wheels4(o, w, L);
    box(o, -hw, hw, 3, 12, 0, L, '#3f7fe0');
    var rear = [BX[0], BY[0], BX[1], BY[1], BX[3], BY[3], BX[2], BY[2]], rearVis = BK.vis;
    taillights(0.05, 0.28, 0.5, 0.8);
    box(o, -hw * 0.76, hw * 0.76, 12, 20, L * 0.3, L * 0.66, '#3f7fe0');
    back(0.1, 0.9, 0.16, 0.84, '#2b3340'); // rear window
    // turbo: two plain orange blocks out of the exhausts
    if (p.turboOn && rearVis) {
      BX[0] = rear[0]; BY[0] = rear[1]; BX[1] = rear[2]; BY[1] = rear[3]; BX[3] = rear[4]; BY[3] = rear[5]; BX[2] = rear[6]; BY[2] = rear[7];
      var fk = 0.5 + Math.random() * 0.3;
      patch(0, 1, 3, 2, 0.11, 0.21, -0.7 * fk, 0.3 * fk, '#ffa53d');
      patch(0, 1, 3, 2, 0.79, 0.89, -0.7 * fk, 0.3 * fk, '#ffa53d');
    }
  }

  // the paint: a sheen that sweeps over the body as lights go by, and a rim of the nearest light
  var SHC = null, SHX = null;
  function playerSheen(psi, x, y, sz, rot, sqy, p, key) {
    var s = SP[key || 'player'], im = IMG[key || 'player'];
    if (!s || !im || !s.frames) return;
    var fr = s.frames, best = fr[0];
    for (var i = 1; i < fr.length; i++) if (Math.abs(fr[i].psi - psi) < Math.abs(best.psi - psi)) best = fr[i];
    var sc = sz * s.cmPerPx / CMU, w = Math.ceil(s.fw * sc) + 2, hh = Math.ceil(s.fh * sc) + 2;
    if (w < 10 || w > 1400) return;
    if (!SHC) { SHC = document.createElement('canvas'); SHX = SHC.getContext('2d'); }
    if (SHC.width < w || SHC.height < hh) { SHC.width = Math.max(w, SHC.width); SHC.height = Math.max(hh, SHC.height); }
    SHX.setTransform(1, 0, 0, 1, 0, 0); SHX.globalCompositeOperation = 'source-over'; SHX.globalAlpha = 1;
    SHX.clearRect(0, 0, w, hh);
    SHX.drawImage(im, best.x, 0, s.fw, s.fh, 0, 0, s.fw * sc, s.fh * sc);
    SHX.globalCompositeOperation = 'source-in';
    var band = LV2 ? ((V.s0 % PSLOT) + PSLOT) % PSLOT / PSLOT * 1.8 - 0.4 : ((TNOW * 0.32) % 2.2) - 0.6;
    var g = SHX.createLinearGradient(band * w - w * 0.25, 0, band * w + w * 0.05, hh * 0.9);
    g.addColorStop(0, 'rgba(255,255,255,0)'); g.addColorStop(0.5, LV2 ? 'rgba(255,200,140,0.5)' : 'rgba(255,255,255,0.5)'); g.addColorStop(1, 'rgba(255,255,255,0)');
    SHX.fillStyle = g; SHX.fillRect(0, 0, w, hh);
    // the rim: the sun's side by day, the floodlight side at night
    SHX.globalCompositeOperation = 'source-atop';
    var side = LV2 ? ((Math.floor(V.s0 / PSLOT) % 2) ? 1 : -1) : -1, rg = SHX.createLinearGradient(side < 0 ? 0 : w, 0, w * 0.5, 0);
    rg.addColorStop(0, LV2 ? 'rgba(255,160,64,0.55)' : 'rgba(255,225,170,0.4)'); rg.addColorStop(1, 'rgba(255,200,120,0)');
    SHX.fillStyle = rg; SHX.fillRect(0, 0, w, hh);
    c.save(); c.translate(x, y); if (rot) c.rotate(rot); c.scale(1, sqy || 1);
    c.globalCompositeOperation = 'screen';
    c.drawImage(SHC, 0, 0, w, hh, -best.ax * sc, -best.ay * sc, w, hh);
    c.restore();
  }

  // ---------- one sorted list: far to near ----------
  var ITEMS = [], pool = [];
  function byZ(a, b) { return b.z > a.z ? 1 : b.z < a.z ? -1 : 0; } // a small-integer answer: a double one is boxed per compare
  function item(z, k, o) {
    var it = pool[ITEMS.length] || (pool[ITEMS.length] = {});
    it.z = z; it.k = k; it.o = o; ITEMS.push(it);
  }
  // Everything (road, pillars, cars, smoke) goes in one list by its distance from the camera and is
  // painted far to near, which is what makes a crest hide the road beyond it and a loop read right.
  function drawScene(S, t, rdt) {
    ITEMS.length = 0; CURS = S;
    var i, e, Z;
    drawGround();
    gatherProps();
    for (i = 0; i < PROPS.length; i++) item(PROPS[i].z + 40, 5, PROPS[i]);
    boundaries();
    for (i = 0; i < SB.n - 1; i++) {
      if (SB.gap[i] && SB.gap[i + 1]) continue;
      var zc = (SB.pz[i] + SB.pz[i + 1]) / 2;
      if (zc < 2) continue;
      item(zc + SEG * 1.5, 3, i);
      if ((SB.idx[i] % 7 === 0 || SB.gap[i] !== SB.gap[i + 1]) && SB.wy[i] > 40 && SB.up[i] > 0.75) item(zc + SEG * 2.5, 4, SB.gap[i] ? i + 1 : i);
    }
    HZN = S.hz && S.hz.length ? S.hz : null; HZB = S.boss; COILOK = false;
    var gk0 = Math.floor((V.s0 + (RCAM ? -600 : 60)) / PSLOT), gk1 = Math.floor((V.s0 + OBJ_FAR + 900) / PSLOT);
    for (var gk = gk0; gk <= gk1; gk++) {
      var gg = gantryAt(gk);
      if (!gg) continue;
      Z = gg.s - V.s0;
      if (Z > ZN + 20 && Z < OBJ_FAR + 900) { proj(0, 60, Z); if (!PBAD) item(PJ.z, 7, gg); }
    }
    if (HZN) for (i = 0; i < HZN.length; i++) {
      var hz = HZN[i];
      if (hz.k === 'zone') { Z = zOf(hz.y); proj(hz.x, 0, Z); if (!PBAD) item(PJ.z + 30, 6, hz); }
      else if (hz.k === 'ring' && hz.age >= hz.warn) { Z = zOf(hz.y); if (Z > ZN) { proj(0, 0, Z); if (!PBAD) item(PJ.z, 6, hz); } }
    }
    for (i = 0; i < S.ents.length; i++) {
      e = S.ents[i]; Z = zOf(e.y + e.h / 2);
      if (Z > ZN && Z < OBJ_FAR) { proj(e.x, e.alt ? e.alt * 0.5 : 0, Z); if (!PBAD) item(PJ.z, 0, e); }
    }
    Z = zOf(S.p.y + S.p.h / 2); proj(S.p.x, 0, Z); item(PJ.z - 0.5, 1, S.p);
    for (i = 0; i < parts.length; i++) {
      Z = zOf(parts[i].y);
      if (Z > ZN && Z < OBJ_FAR) { proj(parts[i].x, parts[i].h, Z); if (!PBAD) { parts[i]._Z = Z; item(PJ.z - 1, 2, parts[i]); } }
    }
    ITEMS.sort(byZ);
    for (i = 0; i < ITEMS.length; i++) {
      var it = ITEMS[i];
      switch (it.k) {
        case 0: aim(S, it.o); drawEnt(it.o, t, rdt); break;
        case 1: drawPlayer(S, t); break;
        case 2: drawPart(it.o, it.o._Z); break;
        case 3: drawSegment(it.o); break;
        case 4: drawPillar(it.o); break;
        case 5: drawProp(it.o); break;
        case 6: if (it.o.k === 'zone') drawZone(it.o); else drawRing(it.o); break;
        case 7: drawGantry(it.o, t); break;
      }
    }
    c.globalAlpha = 1; c.globalCompositeOperation = 'source-over';
    drawBooms();
  }
  // an explosion's light over everything round it: a big warm glow that blooms and dies, hot in the middle
  function drawBooms() {
    if (!BOOMS.length) return;
    addOn();
    for (var i = 0; i < BOOMS.length; i++) {
      var b = BOOMS[i], u = b.age / b.life, Z = zOf(b.y);
      if (Z < ZN || Z > DRAW) continue;
      proj(b.x, b.h, Z); if (PBAD) continue;
      var R = Math.min(viewH * 0.5, 125 * Math.sqrt(b.size) * PJ.s * (0.7 + 0.5 * easeOut(u * 2)));
      var fl = 0.85 + 0.15 * Math.sin(b.age * 60);
      glow(PJ.x, PJ.y, R, '255,120,30', (1 - u) * (1 - u) * 0.5 * fl);
      glow(PJ.x, PJ.y, R * 0.32, '255,220,150', (1 - u) * (1 - u) * (1 - u) * 0.55);
      glow(PJ.x, PJ.y + R * 0.15, R * 1.3, '255,80,20', (1 - u) * 0.25 * fl, 0.3);
    }
    addOff();
  }
  function aim(S, e) {
    if (e.kind === 'suv' || e.kind === 'boss') {
      var want = -Math.atan2(S.p.x - e.x, S.p.y - e.y);
      e.aim = (e.aim || 0) + (want - (e.aim || 0)) * easeK(0.15);
    }
  }

  // explosions, smoke and sparks as plain flat shapes: no glow, no blending
  function drawPart(p, Z) {
    var k = p.age / p.life;
    if (p.dl && p.age < p.dl) return;
    proj(p.x, p.h, Z);
    if (RCAM && PJ.z < 90) return; // nothing smothers a reel camera's lens
    var x = PJ.x, y = PJ.y, s = PJ.s, rr;
    switch (p.t) {
      case 'smoke': case 'puff':
        rr = Math.max(0.5, p.r * s);
        c.globalAlpha = (p.t === 'puff' ? 0.55 : p.dl ? 0.5 : 0.8) * (1 - k); c.fillStyle = p.dark ? '#4a3a5e' : LV2 ? '#cfc6f0' : '#efe8ff';
        c.beginPath(); c.arc(x, y, rr, 0, 6.283); c.fill();
        if (rr > 5) {
          // cel shading: a soft shadowed underside and a light ink rim
          c.globalAlpha *= 0.55;
          c.fillStyle = p.dark ? '#3a2d4e' : LV2 ? '#a99ee0' : '#d8ccf5'; c.beginPath(); c.arc(x, y, rr, 0.35, Math.PI - 0.35); c.arc(x, y - rr * 0.45, rr * 0.9, Math.PI - 0.5, 0.5, true); c.fill();
          c.strokeStyle = 'rgba(27,16,51,' + 0.3 * (1 - k) + ')'; c.lineWidth = 1; c.beginPath(); c.arc(x, y, rr, 0, 6.283); c.stroke();
        } break;
      case 'glow':
        c.globalCompositeOperation = 'lighter'; glow(x, y, Math.min(viewH * 0.32, p.r * s * (0.8 + k * 0.6)), p.col, (1 - k) * 0.75); c.globalCompositeOperation = 'source-over'; break;
      case 'ember':
        var ea = (1 - k) * (0.6 + 0.4 * Math.sin(p.age * 30 + (p.ph || p.x)));
        c.globalCompositeOperation = 'lighter'; glow(x, y, Math.max(1.5, p.r * 3 * s), k < 0.5 ? '255,220,120' : '255,110,60', ea);
        if (k < 0.6) { c.globalAlpha = ea; c.fillStyle = '#fff6d0'; c.fillRect(x - 0.7, y - 0.7, 1.4, 1.4); }
        c.globalCompositeOperation = 'source-over'; break;
      case 'weed': {
        // a tumbleweed bouncing across the road: a ball of dry scribble, turning
        proj(p.x, p.h + Math.abs(Math.sin(p.age * 4.5 + p.r)) * 8, Z); if (PBAD) break;
        x = PJ.x; y = PJ.y; s = PJ.s; rr = Math.max(1.2, p.r * s);
        c.globalAlpha = Math.min(1, (1 - k) * 3);
        c.save(); c.translate(x, y - rr); c.rotate(p.rot);
        c.strokeStyle = INK; c.lineWidth = Math.max(1, rr * 0.22); c.beginPath(); c.arc(0, 0, rr, 0, 6.283); c.stroke();
        c.strokeStyle = '#c99a5a'; c.lineWidth = Math.max(0.7, rr * 0.12);
        for (var wi = 0; wi < 5; wi++) { c.beginPath(); c.ellipse(0, 0, rr * 0.95, rr * (0.3 + wi * 0.14), wi * 0.7, 0, 6.283); c.stroke(); }
        c.restore(); c.globalAlpha = 1;
        break;
      }
      case 'mote':
        c.globalCompositeOperation = 'lighter'; glow(x, y, Math.max(1.2, p.r * 2.4 * s), p.col, Math.sin(k * Math.PI) * 0.7); c.globalCompositeOperation = 'source-over'; break;
      case 'scorch': case 'skid':
        var y2 = zOf(p.y + (p.t === 'skid' ? 7 : p.r));
        proj(p.x, 0.2, y2); var ex = PJ.x, ey = PJ.y; proj(p.x, 0.2, Z);
        if (p.t === 'skid') { c.globalAlpha = 0.5 * (1 - k); c.strokeStyle = '#1b1033'; c.lineWidth = Math.max(1, 3.2 * s); c.beginPath(); c.moveTo(ex, ey); c.lineTo(PJ.x, PJ.y); c.stroke(); }
        else { proj(p.x, 0.2, zOf(p.y - p.r)); var fy = PJ.y; c.globalAlpha = 0.45 * Math.min(1, (1 - k) * 2); c.fillStyle = '#1b1033'; c.beginPath(); c.ellipse(x, (ey + fy) / 2, Math.max(1, p.r * s), Math.max(0.5, Math.abs(ey - fy) / 2), 0, 0, 6.283); c.fill(); }
        break;
      case 'streak':
        proj(p.x, p.h, zOf(p.y - 70)); if (PBAD) break;
        c.globalAlpha = 0.35 * Math.sin(k * Math.PI); c.strokeStyle = LV2 ? '#d7c8ff' : '#ffffff'; c.lineWidth = Math.max(1, 1.2 * s);
        c.beginPath(); c.moveTo(x, y); c.lineTo(PJ.x, PJ.y); c.stroke(); break;
      case 'arc':
        arcBetween(x, y, x + (Math.random() - 0.5) * p.r * s * 2, y - Math.random() * p.r * s, 1, '150,230,255', 1 - k); break;
      case 'debris':
        var ds = SP.fx_debris, dim = IMG.fx_debris;
        if (ds && dim && p.r * s > 1.5) {
          // 1.4: a charred chunk of car (Unreal), tumbling through its rotation frames
          var dj = Math.floor(((p.rot % 6.283) + 6.283) % 6.283 / 6.283 * ds.cols) % ds.cols, dfr = ds.frames[(p.dk || 0) * ds.cols + dj], dsc = s * ds.cmPerPx / CMU * p.r * 0.32;
          c.save(); c.translate(x, y); c.rotate(p.rot * 0.3); c.scale(dsc, dsc);
          c.drawImage(dim, dj * ds.fw, (p.dk || 0) * ds.fh, ds.fw, ds.fh, -dfr.ax, -dfr.ay, ds.fw, ds.fh); c.restore();
        } else { c.save(); c.translate(x, y); c.rotate(p.rot); c.fillStyle = p.col; c.fillRect(-p.r * s, -p.r * 0.6 * s, p.r * 2 * s, p.r * 1.2 * s); c.restore(); }
        break;
      case 'fxb':
        // 1.4: a flipbook of real fire, played through once (fireballs in real time, the burst fast)
        var fs = SP[p.sh], fim = IMG[p.sh];
        if (!fs || !fim) break;
        var nf = fs.cols, fk = clamp(k * (p.end || 1), 0, 0.999), fj = Math.min(nf - 1, Math.floor(fk * nf));
        var fsc = s * fs.cmPerPx / CMU * p.mul * (1 + 0.12 * k), ffr = fs.frames[p.v * nf + fj];
        if (fs.fh * fsc < 1) break;
        if (k < 0.5) { c.globalCompositeOperation = 'lighter'; glow(x, y - fs.fh * 0.25 * fsc, Math.min(viewH * 0.3, fs.fw * fsc * 0.55), '255,150,60', (0.5 - k) * 0.55); c.globalCompositeOperation = 'source-over'; }
        c.save(); c.translate(x, y); if (p.rot) c.rotate(p.rot); c.scale(p.flip ? -fsc : fsc, fsc);
        // the soot at the end thins away rather than hanging there, and nothing smothers the lens
        c.globalAlpha = (k > 0.6 ? Math.pow((1 - k) / 0.4, 1.4) : 1) * clamp((PJ.z - 60) / 220, 0, 1);
        c.drawImage(fim, fj * fs.fw, p.v * fs.fh, fs.fw, fs.fh, -ffr.ax, -ffr.ay, fs.fw, fs.fh);
        c.restore();
        break;
      case 'fire':
        rr = Math.max(0.5, p.r * (1 + k * 0.6) * s);
        if (!p.col && !p.nk && !p.ex && rr > 2.5 && IMG.fx_flame && SP.fx_flame) {
          // 1.4: a lick of real fire (an Unreal flipbook, looping) instead of a disc
          var ls = SP.fx_flame, lv = p.fv !== undefined ? p.fv : (p.fv = Math.floor(Math.random() * ls.rows)), lp = p.fph !== undefined ? p.fph : (p.fph = Math.random() * ls.cols);
          var lj = Math.floor(p.age * 15 + lp) % ls.cols, lfr = ls.frames[lv * ls.cols + lj], lsc = s * ls.cmPerPx / CMU * p.r * 0.05 * (1 + k * 0.4);
          c.globalCompositeOperation = 'lighter'; glow(x, y - p.r * s * 0.6, rr * 2.4, '255,150,60', (1 - k) * 0.55); c.globalCompositeOperation = 'source-over';
          c.save(); c.translate(x, y); c.scale(lv ? -lsc : lsc, lsc * (1 + 0.15 * Math.sin(p.age * 20 + lp)));
          c.globalAlpha = k > 0.55 ? (1 - k) / 0.45 : 1;
          c.drawImage(IMG.fx_flame, lj * ls.fw, lv * ls.fh, ls.fw, ls.fh, -lfr.ax, -lfr.ay, ls.fw, ls.fh);
          c.restore();
          break;
        }
        c.globalAlpha = 1 - k; c.fillStyle = p.col ? (k < 0.35 ? '#ffffff' : p.col) : k < 0.25 ? '#ffe14d' : k < 0.55 ? '#ffa21f' : '#ff4a2a';
        c.beginPath(); c.arc(x, y, rr, 0, 6.283); c.fill();
        if (rr > 3 && !p.nk) {
          c.strokeStyle = INK; c.lineWidth = 1.2; c.stroke();
          if (k < 0.5) { c.fillStyle = p.col ? '#fffbe8' : '#fff3a8'; c.beginPath(); c.arc(x - rr * 0.2, y - rr * 0.25, rr * 0.45, 0, 6.283); c.fill(); }
        }
        if (k < 0.6 && rr > 1.5) { c.globalCompositeOperation = 'lighter'; glow(x, y, rr * 2.6, p.col ? '160,210,255' : '255,160,70', (0.6 - k) * 0.9); c.globalCompositeOperation = 'source-over'; }
        break;
      case 'spark':
        proj(p.x - p.vx * 0.03, p.h - p.vh * 0.03, zOf(p.y - p.vy * 0.03));
        c.globalCompositeOperation = 'lighter';
        c.strokeStyle = p.col || '#ffd23f'; c.lineWidth = Math.max(1.2, 0.7 * s); c.globalAlpha = 1 - k * 0.5;
        c.beginPath(); c.moveTo(x, y); c.lineTo(PJ.x, PJ.y); c.stroke();
        c.strokeStyle = '#fffbe8'; c.lineWidth = Math.max(0.6, 0.3 * s); c.stroke();
        c.globalCompositeOperation = 'source-over'; break;
      case 'flash':
        c.globalCompositeOperation = 'lighter'; glow(x, y, Math.min(viewH * 0.25, Math.max(2, p.r * 0.9 * s)), '255,240,200', 1 - k);
        c.globalCompositeOperation = 'source-over';
        c.globalAlpha = 1 - k; c.fillStyle = '#fff3c4';
        c.beginPath(); c.arc(x, y, Math.max(0.5, p.r * 0.35 * s), 0, 6.283); c.fill(); break;
      case 'ring':
        if (p.add) c.globalCompositeOperation = 'lighter';
        c.globalAlpha = (1 - k) * 0.8;
        c.strokeStyle = 'rgb(' + (p.col || '255,220,160') + ')'; c.lineWidth = (3 * (1 - k) + 1) * Math.min(2, s * 0.5);
        var rr2 = (p.r + (p.R - p.r) * easeOut(k)) * s;
        c.beginPath(); c.ellipse(x, y, rr2, rr2 * Math.min(0.6, V.ch / Math.max(Z, 1) * 2.2), 0, 0, 6.283); c.stroke();
        c.globalCompositeOperation = 'source-over'; break;
      case 'fball':
        // a puff of the fireball: white-hot, then yellow, orange and a dying deep red, inked like the rest
        rr = Math.max(0.5, p.r * s);
        var fc = k < 0.06 ? '#ffffff' : k < 0.22 ? '#fff07a' : k < 0.42 ? '#ffb21f' : k < 0.66 ? '#ff6a1f' : '#c41e3a';
        c.globalAlpha = k < 0.75 ? 1 : (1 - k) / 0.25;
        c.fillStyle = fc; c.beginPath(); c.arc(x, y, rr, 0, 6.283); c.fill();
        if (rr > 4) { c.strokeStyle = INK; c.lineWidth = 1.2; c.globalAlpha *= 0.7; c.stroke(); }
        if (k < 0.6 && rr > 2.5) { c.globalAlpha = 1 - k; c.fillStyle = k < 0.25 ? '#fffbe0' : '#ffe14d'; c.beginPath(); c.arc(x - rr * 0.22, y - rr * 0.28, rr * 0.48, 0, 6.283); c.fill(); }
        c.globalCompositeOperation = 'lighter'; glow(x, y, Math.min(viewH * 0.18, rr * 2.4), k < 0.5 ? '255,190,90' : '255,110,40', (1 - k) * 0.7); c.globalCompositeOperation = 'source-over';
        break;
      case 'pyre':
        c.globalCompositeOperation = 'lighter';
        glow(x, y, Math.max(3, p.r * 3 * s), '255,120,40', (1 - k) * 0.75 * (0.8 + 0.2 * Math.sin(p.age * 23)), 0.35);
        c.globalCompositeOperation = 'source-over'; break;
      case 'rays':
        c.globalCompositeOperation = 'lighter'; c.lineCap = 'round';
        var rl = p.r * s * (0.35 + 0.65 * easeOut(k * 1.5));
        for (var ri = 0; ri < 12; ri++) {
          var ra = p.rot + ri * 0.5236 + (ri % 2) * 0.12, rw = ri % 2 ? 0.55 : 1;
          c.globalAlpha = (1 - k) * 0.45 * rw; c.strokeStyle = '#ffd9a0'; c.lineWidth = Math.max(1.5, 4 * s * rw);
          c.beginPath(); c.moveTo(x + Math.cos(ra) * rl * 0.15, y + Math.sin(ra) * rl * 0.15); c.lineTo(x + Math.cos(ra) * rl * rw, y + Math.sin(ra) * rl * rw); c.stroke();
        }
        c.globalCompositeOperation = 'source-over'; break;
      case 'part':
        c.save(); c.translate(x, y); c.rotate(p.rot); c.globalAlpha = k > 0.8 ? (1 - k) / 0.2 : 1;
        c.fillStyle = p.col; c.strokeStyle = INK; c.lineWidth = Math.max(0.8, 0.5 * s);
        if (p.disc) { c.beginPath(); c.ellipse(0, 0, p.w * s * 0.5, p.l * s * 0.5 * Math.abs(Math.cos(p.rot * 2)) + 0.5, 0, 0, 6.283); c.fill(); c.stroke(); }
        else { c.fillRect(-p.w * s / 2, -p.l * s / 2, p.w * s, p.l * s); c.strokeRect(-p.w * s / 2, -p.l * s / 2, p.w * s, p.l * s); }
        c.restore(); break;
      case 'rag':
        c.globalAlpha = k > 0.75 ? (1 - k) / 0.25 : 1;
        hench(x, y, s, 'rag', 1, p.age * 2, p.rot);
        break;
      case 'hwreck':
        if (IMG.heli) {
          var hpsi = ((p.age * 520) % 180) - 90;
          sprite('heli', hpsi, x, y, s, p.rot * 0.3);
          c.globalAlpha = 0.75; sprite('heli', hpsi, x, y, s, p.rot * 0.3, 3); c.globalAlpha = 1;
        }
        c.globalCompositeOperation = 'lighter'; glow(x, y - 8 * s, 40 * s, '255,140,50', 0.8); c.globalCompositeOperation = 'source-over';
        break;
      case 'muzzle':
        c.globalCompositeOperation = 'lighter'; glow(x, y, Math.max(3, 12 * s), '255,210,90', 0.9); c.globalCompositeOperation = 'source-over';
        c.fillStyle = '#fff3a8'; c.save(); c.translate(x, y); c.rotate(p.x); c.fillRect(-4 * s, -1 * s, 8 * s, 2 * s); c.fillRect(-1 * s, -4 * s, 2 * s, 8 * s); c.restore(); break;
    }
    c.globalAlpha = 1;
  }

  function drawShots(S) {
    var i, b, Z;
    c.strokeStyle = '#ffd23f'; c.lineCap = 'butt';
    for (i = 0; i < S.bullets.length; i++) {
      b = S.bullets[i]; Z = zOf(b.y);
      if (Z < ZN || Z > OBJ_FAR) continue;
      proj(b.x, 16, Z); var x1 = PJ.x, y1 = PJ.y, s = PJ.s, b1 = PBAD;
      proj(b.x, 16, zOf(b.y + 34));
      if (b1 || PBAD) continue;
      c.globalCompositeOperation = 'lighter';
      c.strokeStyle = 'rgba(255,190,60,0.45)'; c.lineWidth = Math.max(2, 4.5 * s);
      c.beginPath(); c.moveTo(x1, y1); c.lineTo(PJ.x, PJ.y); c.stroke();
      c.strokeStyle = '#fff3a8'; c.lineWidth = Math.max(1, 1.6 * s); c.stroke();
      c.globalCompositeOperation = 'source-over';
    }
    for (i = 0; i < S.ebullets.length; i++) {
      b = S.ebullets[i]; Z = zOf(b.y);
      if (Z < ZN) continue;
      proj(b.x, 10, Z); if (PBAD) continue;
      addOn(); glow(PJ.x, PJ.y, Math.max(6, 12 * PJ.s), '255,60,60', 0.8); addOff();
      c.fillStyle = '#ff3b3b'; c.beginPath(); c.arc(PJ.x, PJ.y, Math.max(2.5, 4 * PJ.s), 0, 6.283); c.fill();
      c.fillStyle = '#ffe0e0'; c.beginPath(); c.arc(PJ.x, PJ.y, Math.max(1, 1.6 * PJ.s), 0, 6.283); c.fill();
    }
    for (i = 0; i < S.missiles.length; i++) {
      var m = S.missiles[i]; Z = zOf(m.y);
      if (Z < ZN) continue;
      proj(m.x, 12, Z); if (PBAD) continue; var mx = PJ.x, my = PJ.y, ms = PJ.s;
      addOn(); glow(mx, my, Math.max(5, 16 * ms), '255,170,80', 0.9); addOff();
      c.fillStyle = '#f0f0f0'; c.beginPath(); c.arc(mx, my, Math.max(1.5, 3.2 * ms), 0, 6.283); c.fill();
    }
  }

  function drawPops() {
    c.textAlign = 'center'; c.textBaseline = 'middle';
    for (var i = 0; i < pops.length; i++) {
      var p = pops[i], s = easeOutBack(p.t / 0.25), a = p.t > 0.9 ? 1 - (p.t - 0.9) / 0.4 : 1;
      var Z = Math.max(V.zp, zOf(p.y));
      proj(p.x, 34, Math.min(Z, 1800));
      var x = clamp(PJ.x, 90, viewW - 90), y = clamp(PJ.y, V.hY * 0.5, viewH - 40);
      c.save(); c.translate(x, y - p.t * 40); c.scale(s, s); c.globalAlpha = clamp(a, 0, 1);
      c.font = font(p.big ? 19 : 14, 900);
      c.lineWidth = 4; c.strokeStyle = 'rgba(0,0,0,0.75)'; c.strokeText(p.text, 0, 0);
      c.fillStyle = p.color; c.fillText(p.text, 0, 0);
      c.restore();
    }
  }

  // ---------- the whole frame ----------
  function shakeOffset(S, t) {
    if (REDUCED || !S) return { x: 0, y: 0, r: 0 };
    var a = S.trauma * S.trauma, hum = S.p && (S.p.turboOn || S.p.boostT > 0) ? 1.1 : S.p && S.p.drift ? 0.6 : 0;
    return {
      x: a * 16 * (Math.sin(t * 37.1) * 0.6 + Math.sin(t * 61.7 + 1.3) * 0.4) + hum * Math.sin(t * 83.3),
      y: a * 12 * (Math.sin(t * 43.3 + 2.1) * 0.6 + Math.sin(t * 71.9) * 0.4) + hum * Math.sin(t * 97.1 + 0.5),
      r: a * 0.025 * Math.sin(t * 29.3 + 0.7)
    };
  }

  function world(S, t, rdt) {
    RDT = rdt;
    buildView(S, rdt);
    var sh = shakeOffset(S, t);
    c.save();
    c.translate(viewW / 2 + sh.x, viewH / 2 + sh.y); c.rotate(sh.r); c.translate(-viewW / 2, -viewH / 2);
    drawSky(S, t);
    drawScene(S, t, rdt);
    if (HZN) laneBolts(S);
    drawShots(S);
    drawPops();
    c.restore();
  }

  function filmLook(S) {
    // slow motion squeezes in plain letterbox bars (no grade, grain, vignette or flashes in the demo)
    var slow = S ? clamp((1 - S.timeScale) / 0.7, 0, 1) : 0;
    if (slow > 0.01) {
      var bar = 46 * easeOut(slow);
      c.fillStyle = '#000'; c.fillRect(0, 0, viewW, bar); c.fillRect(0, viewH - bar, viewW, bar);
      if (slow > 0.5) {
        c.fillStyle = 'rgba(255,255,255,' + (slow - 0.5) * 2 + ')'; c.font = font(13, 800, true); c.textAlign = 'right'; c.textBaseline = 'middle';
        c.fillText('▶▶  SLOW MOTION', viewW - 24, viewH - bar / 2);
      }
    }
  }

  // ---------- HUD: spy dashboard + film production overlay ----------
  function panel(x, y, w, h) {
    c.fillStyle = 'rgba(8,12,18,0.78)'; rr(x, y, w, h, 8); c.fill();
    c.strokeStyle = 'rgba(54,241,255,0.35)'; c.lineWidth = 1; rr(x + 0.5, y + 0.5, w - 1, h - 1, 8); c.stroke();
    c.fillStyle = 'rgba(54,241,255,0.8)'; c.fillRect(x + 10, y, 30, 2);
  }
  function label(t, x, y, col, px, align) {
    c.fillStyle = col || '#7fa9b8'; c.font = font(px || 10, 800, true); c.textAlign = align || 'left'; c.textBaseline = 'alphabetic';
    c.fillText(t, x, y);
  }
  function armorBar(S, x, y, w, t) {
    var hp = S.p.hp, seg = 10, gw = (w - (seg - 1) * 3) / seg;
    for (var i = 0; i < seg; i++) {
      var on = hp > i * 10 + 0.5, col = hp > 60 ? '#4cd07d' : hp > 30 ? '#ffc844' : '#ff4a3e';
      if (hp <= 25 && on && Math.floor(t * 6) % 2) col = '#ff9a8f';
      c.fillStyle = on ? col : 'rgba(255,255,255,0.08)'; c.fillRect(x + i * (gw + 3), y, gw, 10);
    }
  }
  function missilePips(S, x, y, w) {
    var cap = Math.max(9, S.p.maxMissiles || 9, S.p.missiles); // 1.8: WEAPONS ranks add pods
    for (var i = 0; i < cap; i++) {
      var on = i < S.p.missiles, px = x + i * 14;
      if (px + 10 > x + w) break;
      c.fillStyle = on ? '#ff9e3d' : 'rgba(255,255,255,0.1)';
      rr(px, y, 8, 18, 4); c.fill();
      if (on) { c.fillStyle = '#ffe2c0'; c.fillRect(px, y, 8, 4); }
    }
  }
  function turboBar(S, x, y, w, t) {
    var p = S.p, f = p.turbo / 100;
    c.fillStyle = 'rgba(255,255,255,0.08)'; c.fillRect(x, y, w, 10);
    c.fillStyle = p.turboLock ? '#5d6169' : '#36f1ff'; c.fillRect(x, y, w * f, 10);
    if (p.turboOn) { c.fillStyle = 'rgba(255,255,255,' + (0.3 + 0.3 * Math.sin(t * 30)) + ')'; c.fillRect(x, y, w * f, 10); }
  }

  // the drift charge: three pips that light blue, orange and pink, and a bar filling to the next
  function driftMeter(S, x, y, w, ui, t) {
    var p = S.p, d = p.drift, lvl = d ? d.lvl : 0;
    label(ui.touch ? 'DRIFT' : 'DRIFT  [' + (ui.keyOne ? ui.keyOne('drift') : 'SHIFT') + ']', x, y);
    label(d ? (lvl ? ['', 'BLUE', 'ORANGE', 'PINK!'][lvl] : 'SLIDING') : p.boostT > 0 ? 'BOOST!' : 'HOLD + STEER', x + w, y, d ? DCOL[lvl] || '#e8f3f7' : p.boostT > 0 ? '#36f1ff' : '#5f7c88', 9, 'right');
    var AT = [0, 0.65, 1.45, 2.35], gw = (w - 6) / 3;
    for (var i = 0; i < 3; i++) {
      var on = lvl > i, f = d ? clamp((d.charge - AT[i]) / (AT[i + 1] - AT[i]), 0, 1) : 0;
      c.fillStyle = 'rgba(255,255,255,0.08)'; c.fillRect(x + i * (gw + 3), y + 6, gw, 8);
      c.fillStyle = DCOL[i + 1]; c.globalAlpha = on ? 1 : 0.55; c.fillRect(x + i * (gw + 3), y + 6, gw * (on ? 1 : f), 8); c.globalAlpha = 1;
      if (on && Math.floor(t * 12) % 2 && lvl === i + 1) { c.fillStyle = 'rgba(255,255,255,0.4)'; c.fillRect(x + i * (gw + 3), y + 6, gw, 8); }
    }
  }
  function hud(S, ui, t) {
    var L = { wide: viewW >= 760 }, p = S.p, mph = Math.round(S.speed * 0.31);
    var side = (viewW - EDGE * 2) / 2;
    var safeT = ui.safe.t / scale + 8, safeL = ui.safe.l / scale + 8, safeR = ui.safe.r / scale + 8;
    // progress bar: a strip of film across the top of the road
    var bw = Math.min(EDGE * 2 - 20, 300), bx = viewW / 2 - bw / 2, by = safeT;
    c.fillStyle = 'rgba(8,12,18,0.82)'; rr(bx - 8, by - 4, bw + 16, 30, 6); c.fill();
    c.fillStyle = '#16181c'; c.fillRect(bx, by + 6, bw, 12);
    c.fillStyle = 'rgba(255,255,255,0.14)';
    for (var sx = bx + 3; sx < bx + bw - 3; sx += 9) { c.fillRect(sx, by + 1, 4, 3); c.fillRect(sx, by + 20, 4, 3); }
    var pg = S.progress;
    c.fillStyle = '#ffd23f'; c.fillRect(bx, by + 6, bw * pg, 12);
    for (var mi = 1; mi < E.SCENES.length - 1; mi++) { var mx = bx + bw * 0.85 * E.SCENES[mi].d / E.SCENE_LEN; c.fillStyle = mi <= S.scene ? 'rgba(11,13,18,0.7)' : 'rgba(255,255,255,0.5)'; c.fillRect(mx, by + 6, 1, 12); }
    c.fillStyle = '#ff3ea5'; c.fillRect(bx + bw * 0.85, by + 4, 2, 16);
    c.fillStyle = '#fff'; c.beginPath(); var cx = bx + bw * pg; c.moveTo(cx, by + 3); c.lineTo(cx + 5, by - 2); c.lineTo(cx - 5, by - 2); c.fill();
    label('SCENE ' + (S.scene + 1) + '/' + E.SCENES.length, bx + 4, by + 16, pg > 0.12 ? '#0b0d12' : '#ffd23f', 9);
    label('★ FINALE', bx + bw - 3, by + 16, pg > 0.84 ? '#0b0d12' : '#ffd23f', 9, 'right');

    // boss bar
    if (S.boss && !S.boss.dead && S.boss.hp > 0 && S.phase === 'play') {
      var bb = S.boss, bbw = Math.min(260, EDGE * 2 - 30);
      label(E.KIND.boss.name, viewW / 2, by + 44, '#ff5a4e', 11, 'center');
      c.fillStyle = 'rgba(0,0,0,0.6)'; c.fillRect(viewW / 2 - bbw / 2, by + 48, bbw, 7);
      c.fillStyle = '#ff2a3a'; c.fillRect(viewW / 2 - bbw / 2, by + 48, bbw * bb.hp / bb.max, 7);
    }

    if (L.wide) {
      var pw = Math.min(236, side - 28);
      // --- left: the spy dashboard
      var lx = safeL, ly = safeT, lh = ui.playT < 10 || ui.screen !== 'play' ? 326 : 274;
      panel(lx, ly, pw, lh);
      label('◉ KESTREL-88', lx + 12, ly + 20, '#36f1ff', 10);
      label('AGENT N. KANE', lx + pw - 12, ly + 20, '#7fa9b8', 10, 'right');
      label('ARMOR', lx + 12, ly + 46);
      label(Math.round(p.hp) + '%', lx + pw - 12, ly + 46, p.hp > 30 ? '#e8f3f7' : '#ff6a5e', 11, 'right');
      armorBar(S, lx + 12, ly + 52, pw - 24, t);
      var dmgTxt = p.hp > 79 ? 'ALL SYSTEMS NOMINAL' : p.hp > 59 ? 'BODYWORK DAMAGED' : p.hp > 39 ? 'WINDSHIELD CRACKED' : p.hp > 24 ? 'ENGINE SMOKING' : 'CRITICAL — ON FIRE';
      label(dmgTxt, lx + 12, ly + 76, p.hp > 59 ? '#4cd07d' : p.hp > 24 ? '#ffc844' : (Math.floor(t * 4) % 2 ? '#ff4a3e' : '#ffd0cc'), 9);
      label(ui.touch ? 'MISSILES' : ui.pad ? 'MISSILES  [B]' : 'MISSILES  [' + ui.keyOne('missile') + ' / R-CLICK]', lx + 12, ly + 102);
      label(String(p.missiles), lx + pw - 12, ly + 102, p.missiles ? '#ff9e3d' : '#ff5a4e', 11, 'right');
      missilePips(S, lx + 12, ly + 108, pw - 24);
      label(ui.touch ? 'TURBO' : 'TURBO  [' + ui.keyOne('turbo') + ']', lx + 12, ly + 148);
      label(p.turboOn ? 'BURN' : p.boostT > 0 ? 'BOOST' : p.turboLock ? 'RECHARGING' : p.turbo > 99 ? 'READY' : 'CHARGING', lx + pw - 12, ly + 148, p.turboOn || p.boostT > 0 ? '#36f1ff' : p.turboLock ? '#ff6a5e' : '#7fa9b8', 10, 'right');
      turboBar(S, lx + 12, ly + 154, pw - 24, t);
      driftMeter(S, lx + 12, ly + 182, pw - 24, ui, t);
      label('SPEED', lx + 12, ly + 220);
      c.fillStyle = p.turboOn || p.boostT > 0 ? '#36f1ff' : '#e8f3f7'; c.font = font(32, 800, true); c.textAlign = 'left'; c.fillText(String(mph), lx + 12, ly + 254);
      label('MPH', lx + 14 + c.measureText(String(mph)).width + 4, ly + 254, '#7fa9b8', 10);
      // tacho arc
      var ax = lx + pw - 48, ay = ly + 248;
      c.strokeStyle = 'rgba(255,255,255,0.12)'; c.lineWidth = 6; c.beginPath(); c.arc(ax, ay, 26, Math.PI * 0.8, Math.PI * 2.2); c.stroke();
      c.strokeStyle = p.turboOn ? '#36f1ff' : '#ff3ea5'; c.beginPath(); c.arc(ax, ay, 26, Math.PI * 0.8, Math.PI * (0.8 + 1.4 * clamp(S.speed / 760, 0, 1))); c.stroke();
      c.fillStyle = 'rgba(54,241,255,0.12)'; c.fillRect(lx + 12, ly + 270, pw - 24, 1);
      if (lh < 326) { /* the controls hint leaves after ten seconds, to free the view */ } else if (!ui.touch) {
        // 1.7: the player's own keys (or the pad's buttons)
        if (ui.pad) {
          label('STICK DRIVE  A/RB DRIFT  LB/RT TURBO', lx + 12, ly + 290, '#5f7c88', 9);
          label('X GUNS  B MISSILE  START PAUSE', lx + 12, ly + 306, '#5f7c88', 9);
        } else {
          // 1.7.1: the layout in use (WASD: J/K shoot; arrows: Z/X shoot)
          label(ui.keyText('steer').replace('ARROW KEYS', '←→↑↓') + ' DRIVE  ' + ui.keyOne('drift') + ' DRIFT  ' + ui.keyOne('turbo') + ' TURBO', lx + 12, ly + 290, '#5f7c88', 9);
          label(ui.keyOne('fire') + '/L-CLICK GUNS  ' + ui.keyOne('missile') + '/R-CLICK MISSILE', lx + 12, ly + 306, '#5f7c88', 9);
        }
      } else label('LEFT THUMB DRIVES · BUTTONS FIRE', lx + 12, ly + 296, '#5f7c88', 9);

      // --- right: the film production overlay
      var rx = viewW - safeR - pw, ry = safeT, rh = 214;
      panel(rx, ry, pw, rh);
      var blink = Math.floor(t * 2) % 2 === 0 || S.phase !== 'play';
      c.fillStyle = '#c0182a'; rr(rx + 12, ry + 10, 122, 22, 4); c.fill();
      if (blink) { c.fillStyle = '#fff'; c.beginPath(); c.arc(rx + 24, ry + 21, 5, 0, 6.283); c.fill(); }
      label('NOW FILMING', rx + 34, ry + 26, '#fff', 11);
      label(timecode(S.t), rx + pw - 12, ry + 54, '#ffd23f', 10, 'right');
      label('SCENE ' + (S.scene + 1) + ' · TAKE ' + S.sceneTake, rx + 12, ry + 54, '#e8f3f7', 11);
      label(S.beat, rx + 12, ry + 70, '#ff3ea5', 10);
      var reels = S.reels - S.retakes;
      label('FILM LEFT', rx + 12, ry + 88, '#7fa9b8', 9);
      for (var rl = 0; rl < S.reels; rl++) {
        c.strokeStyle = rl < reels ? '#ffd23f' : '#3a3f48'; c.lineWidth = 2;
        c.beginPath(); c.arc(rx + 80 + rl * 16, ry + 85, 5, 0, 6.283); c.stroke();
        if (rl < reels) { c.fillStyle = '#ffd23f'; c.beginPath(); c.arc(rx + 80 + rl * 16, ry + 85, 1.6, 0, 6.283); c.fill(); }
      }
      c.fillStyle = 'rgba(54,241,255,0.12)'; c.fillRect(rx + 12, ry + 98, pw - 24, 1);
      label('BOX OFFICE', rx + 12, ry + 118);
      c.fillStyle = '#ffd23f'; c.font = font(30, 800, true); c.textAlign = 'right'; c.fillText(String(S.score).replace(/\B(?=(\d{3})+(?!\d))/g, ','), rx + pw - 12, ry + 152);
      if (S.combo > 1) {
        var cs = 1 + 0.15 * Math.max(0, S.comboT - 2.6) / 0.4;
        c.save(); c.translate(rx + 12, ry + 182); c.scale(cs, cs);
        label('COMBO ×' + S.combo, 0, 0, '#7ff7ff', 13); c.restore();
        c.fillStyle = 'rgba(127,247,255,0.6)'; c.fillRect(rx + 12, ry + 190, (pw - 24) * clamp(S.comboT / 3, 0, 1), 3);
      } else label('KILLS ' + S.counts.kills + '  ·  NEAR MISSES ' + S.counts.near, rx + 12, ry + 186, '#5f7c88', 9);
      label('STUNTS ' + S.counts.stunts + '  ·  COLLATERAL ' + S.counts.civ, rx + 12, ry + 204, '#5f7c88', 9);
    } else {
      // compact: two small blocks in the corners
      var cw = 134;
      panel(safeL, safeT + 34, cw, 84);
      label('ARMOR ' + Math.round(p.hp) + '%', safeL + 8, safeT + 52, p.hp > 30 ? '#e8f3f7' : '#ff6a5e', 10);
      armorBar(S, safeL + 8, safeT + 58, cw - 16, t);
      label('MSL ' + p.missiles, safeL + 8, safeT + 86, '#ff9e3d', 10);
      label(Math.round(S.speed * 0.31) + ' MPH', safeL + cw - 8, safeT + 86, '#e8f3f7', 10, 'right');
      turboBar(S, safeL + 8, safeT + 96, cw - 16, t);
      if (p.drift) label('DRIFT ' + ['·', '●', '●●', '●●●'][p.drift.lvl], safeL + 8, safeT + 130, DCOL[p.drift.lvl] || '#e8f3f7', 11);
      panel(viewW - safeR - cw, safeT + 34, cw, 62);
      c.fillStyle = '#c0182a'; rr(viewW - safeR - cw + 8, safeT + 42, 84, 16, 3); c.fill();
      label((Math.floor(t * 2) % 2 ? '● ' : '  ') + 'NOW FILMING', viewW - safeR - cw + 12, safeT + 54, '#fff', 9);
      c.fillStyle = '#ffd23f'; c.font = font(18, 800, true); c.textAlign = 'right'; c.fillText(String(S.score), viewW - safeR - 8, safeT + 84);
      if (S.combo > 1) label('×' + S.combo, viewW - safeR - cw + 8, safeT + 84, '#7ff7ff', 12);
      else label('SC ' + (S.scene + 1) + ' TK ' + S.sceneTake, viewW - safeR - cw + 8, safeT + 84, '#ff3ea5', 9);
    }

    if (p.drift && S.phase === 'play' && !REDUCED) {
      for (var dp = 0; dp < 3; dp++) {
        c.fillStyle = p.drift.lvl > dp ? DCOL[dp + 1] : 'rgba(255,255,255,0.25)';
        c.beginPath(); c.arc(V.px + (dp - 1) * 12, V.py + 16, 4, 0, 6.283); c.fill();
      }
    }
    // viewfinder corners
    c.strokeStyle = 'rgba(255,255,255,0.35)'; c.lineWidth = 2;
    var m = 10, ln = 26;
    [[m, m + 40, 1, 1], [viewW - m, m + 40, -1, 1], [m, viewH - m, 1, -1], [viewW - m, viewH - m, -1, -1]].forEach(function (q) {
      c.beginPath(); c.moveTo(q[0], q[1] + ln * q[3]); c.lineTo(q[0], q[1]); c.lineTo(q[0] + ln * q[2], q[1]); c.stroke();
    });

    // the director, on the megaphone the whole way through
    if (ui.dir) director(S, ui, t, L.wide ? Math.min(236, side - 28) : 134);
    // CUT!
    if (S.phase === 'wrap' || S.phase === 'wreck' || S.phase === 'done') {
      var k = clamp((S.phase === 'done' ? 1 : (S.outcome === 'wrecked' || S.cutAgain ? 2.8 : S.outcome === 'escaped' ? 3 : 3.6) - S.wrapT) / 0.5, 0, 1);
      var s = 0.6 + 0.4 * easeOutBack(k);
      c.save(); c.translate(viewW / 2, viewH * 0.42); c.scale(s, s); c.globalAlpha = k;
      c.font = font(86, 900); c.textAlign = 'center'; c.textBaseline = 'middle';
      c.lineWidth = 10; c.strokeStyle = '#000'; c.strokeText('CUT!', 0, 0);
      c.fillStyle = S.outcome === 'wrapped' ? '#ffd23f' : '#ff4a3e'; c.fillText('CUT!', 0, 0);
      c.font = font(18, 800, true);
      var sub = S.cutAgain ? 'AGAIN! FROM THE TOP OF SCENE ' + (S.scene + 1) : S.outcome === 'wrapped' ? 'PRINT IT. THAT’S A WRAP!' : S.outcome === 'wrecked' ? 'OUT OF FILM. WE LOST THE STAR CAR!' : 'THE TARGET GOT AWAY!';
      c.lineWidth = 5; c.strokeText(sub, 0, 62, viewW / s - 20); c.fillStyle = '#fff'; c.fillText(sub, 0, 62, viewW / s - 20);
      if (S.cutAgain) { var nx = 'TAKE ' + (S.sceneTake + 1) + ' COMING UP'; c.font = font(15, 800, true); c.strokeText(nx, 0, 92); c.fillStyle = '#ffd23f'; c.fillText(nx, 0, 92); }
      c.restore();
    }
  }
  function wrapText(text, maxW) {
    var words = text.split(' '), out = [], cur = '';
    for (var i = 0; i < words.length; i++) {
      var next = cur ? cur + ' ' + words[i] : words[i];
      if (cur && c.measureText(next).width > maxW) { out.push(cur); cur = words[i]; } else cur = next;
    }
    if (cur) out.push(cur);
    return out;
  }
  function megaphone(x, y, col) {
    c.fillStyle = col;
    c.beginPath(); c.moveTo(x, y - 3); c.lineTo(x + 5, y - 3); c.lineTo(x + 13, y - 8); c.lineTo(x + 13, y + 8); c.lineTo(x + 5, y + 3); c.lineTo(x, y + 3); c.closePath(); c.fill();
    c.fillRect(x + 4, y + 3, 2, 5);
  }
  // the director's line (a bar under the film strip), the scene board, and one big shouted word
  function director(S, ui, t, pw) {
    var D = ui.dir, L = D.line, safeT = ui.safe.t / scale + 8, wide = viewW >= 760;
    var room = viewW - 2 * (pw + ui.safe.l / scale + (wide ? 24 : 20));
    var top = safeT + (S.boss && !S.boss.dead && S.phase === 'play' ? 62 : 36);
    if (room < 250) { room = viewW - 24; top = safeT + 124; }
    var bw = Math.min(room, 560), bx = viewW / 2 - bw / 2, barBottom = top;
    if (L) {
      var fs = viewW < 600 ? 15 : 17;
      c.font = font(fs, 800);
      var lines = wrapText('“' + L.text + '”', bw - 30), lh = fs + 5;
      var bh = 26 + lines.length * lh;
      barBottom = top + bh;
      var pop = L.pri >= 2 && L.age < 0.18 ? 1 + 0.08 * (1 - L.age / 0.18) : 1;
      c.save(); c.translate(viewW / 2, top + bh / 2); c.scale(pop, pop); c.translate(-viewW / 2, -(top + bh / 2));
      c.globalAlpha = L.age > L.hold ? 0.62 : 1;
      c.fillStyle = 'rgba(6,8,12,0.82)'; rr(bx, top, bw, bh, 8); c.fill();
      var hot = L.pri >= 3 ? '#ffd23f' : L.pri >= 2 ? '#ff3ea5' : '#36f1ff';
      c.fillStyle = hot; c.fillRect(bx, top + 6, 3, bh - 12);
      megaphone(bx + 12, top + 12, hot);
      label('DIRECTOR', bx + 30, top + 16, hot, 9);
      label('SCENE ' + (S.scene + 1) + ' · TAKE ' + S.sceneTake, bx + bw - 12, top + 16, '#5f7c88', 9, 'right');
      // typed out as he says it
      c.font = font(fs, 800); c.textAlign = 'left'; c.textBaseline = 'alphabetic';
      var left = Math.max(2, Math.floor(L.age * 70) + 1);
      for (var i = 0; i < lines.length && left > 0; i++) {
        c.fillStyle = L.pri >= 3 ? '#fff3c4' : '#ffffff';
        c.fillText(lines[i].slice(0, left), bx + 16, top + 18 + (i + 1) * lh);
        left -= lines[i].length + 1;
      }
      c.globalAlpha = 1; c.restore();
    }
    var sl = D.slate;
    if (sl) {
      var a = sl.age, k = a < 0.3 ? easeOutBack(a / 0.3) : 1, fade = a > 2.4 ? clamp(1 - (a - 2.4) / 0.4, 0, 1) : 1;
      var sw = Math.min(300, viewW - 40), sh = 74, sx = viewW / 2 - sw / 2, sy = Math.max(viewH * 0.31 - sh / 2, barBottom + 34);
      c.save(); c.globalAlpha = fade; c.translate(viewW / 2, sy + sh / 2); c.scale(k, k); c.rotate(-0.03); c.translate(-viewW / 2, -(sy + sh / 2));
      c.fillStyle = '#16181c'; c.fillRect(sx, sy, sw, sh);
      c.strokeStyle = '#e8e8e8'; c.lineWidth = 2; c.strokeRect(sx + 4, sy + 18, sw - 8, sh - 22);
      var ang = a < 0.25 ? -0.35 * (1 - a / 0.25) : 0;
      c.save(); c.translate(sx, sy + 14); c.rotate(ang);
      c.fillStyle = '#16181c'; c.fillRect(0, -14, sw, 14); c.fillStyle = '#e8e8e8';
      for (var j = 0; j < sw / 34; j++) { c.beginPath(); c.moveTo(6 + j * 34, -14); c.lineTo(22 + j * 34, -14); c.lineTo(12 + j * 34, 0); c.lineTo(-4 + j * 34, 0); c.closePath(); c.fill(); }
      c.restore();
      c.textAlign = 'center'; c.textBaseline = 'middle';
      c.fillStyle = '#ff3ea5'; c.font = font(11, 800, true);
      c.fillText('SCENE ' + (sl.n + 1) + '   ·   TAKE ' + sl.take, viewW / 2, sy + 32);
      c.fillStyle = '#fff'; c.font = font(22, 900);
      c.fillText(sl.name, viewW / 2, sy + 55, sw - 20);
      c.restore();
    }
    var sh2 = D.shout, shoutY = sl ? Math.max(viewH * 0.5, barBottom + 34 + 74 + 46) : viewH * 0.5;
    if (sh2) {
      var T = sh2.age, kk = T < 0.15 ? easeOutBack(T / 0.15) : 1, aa = T > sh2.life - 0.35 ? (sh2.life - T) / 0.35 : 1;
      c.save(); c.translate(viewW / 2, shoutY); c.scale(kk, kk); c.globalAlpha = clamp(aa, 0, 1);
      c.font = font(Math.min(76, viewW / 9), 900); c.textAlign = 'center'; c.textBaseline = 'middle';
      c.lineWidth = 10; c.strokeStyle = '#000'; c.strokeText(sh2.text, 0, 0);
      c.fillStyle = sh2.col; c.fillText(sh2.text, 0, 0);
      c.restore();
    }
  }
  function timecode(t) {
    var f = Math.floor(t * 24) % 24, s = Math.floor(t) % 60, m = Math.floor(t / 60);
    function z(n) { return (n < 10 ? '0' : '') + n; }
    return 'TC 00:' + z(m) + ':' + z(s) + ':' + z(f);
  }

  // ---------- touch controls ----------
  function touchUI(ui) {
    if (!ui.touch) return;
    var b = ui.buttons;
    for (var k in b) {
      var q = b[k], on = ui.held[k];
      c.globalAlpha = on ? 0.9 : 0.55;
      c.fillStyle = on ? q.col : 'rgba(10,14,20,0.7)'; c.beginPath(); c.arc(q.x, q.y, q.r, 0, 6.283); c.fill();
      c.strokeStyle = q.col; c.lineWidth = 3; c.beginPath(); c.arc(q.x, q.y, q.r, 0, 6.283); c.stroke();
      c.fillStyle = on ? '#0b0d12' : '#fff'; c.font = font(q.r > 40 ? 15 : 12, 900); c.textAlign = 'center'; c.textBaseline = 'middle';
      c.fillText(q.label, q.x, q.y);
      c.globalAlpha = 1;
    }
    var j = ui.stick;
    if (j) {
      c.globalAlpha = 0.5; c.strokeStyle = '#36f1ff'; c.lineWidth = 3;
      c.beginPath(); c.arc(j.ox, j.oy, 56, 0, 6.283); c.stroke();
      c.fillStyle = '#36f1ff'; c.beginPath(); c.arc(j.ox + j.dx * 46, j.oy + j.dy * 46, 22, 0, 6.283); c.fill();
      c.globalAlpha = 1;
    } else {
      c.globalAlpha = 0.35; c.fillStyle = '#fff'; c.font = font(12, 800); c.textAlign = 'center';
      c.fillText('◀  DRAG HERE TO DRIVE  ▶', ui.stickHome.x, ui.stickHome.y);
      c.globalAlpha = 1;
    }
  }

  // ---------- screens ----------
  function title(ui, t) {
    var T = ui.t;
    c.fillStyle = 'rgba(27,16,51,0.38)'; c.fillRect(0, 0, viewW, viewH);
    var bar = Math.min(70, viewH * 0.1);
    c.fillStyle = '#000'; c.fillRect(0, 0, viewW, bar); c.fillRect(0, viewH - bar, viewW, bar);
    var cx = viewW / 2, cy = viewH * 0.36;
    var a1 = easeOut((T - 0.2) / 0.6);
    c.globalAlpha = a1; c.font = font(13, 800, true); c.textAlign = 'center'; c.textBaseline = 'middle';
    c.fillStyle = '#c9b48a'; c.fillText('A  ROADBURN  PICTURES  PRODUCTION', cx, cy - 92);
    c.globalAlpha = 1;
    var s = easeOutBack((T - 0.6) / 0.5), fs = Math.min(110, viewW / 6.4);
    if (T > 0.6) {
      c.save(); c.translate(cx, cy); c.scale(s, s); c.transform(1, 0, -0.18, 1, 0, 0);
      c.font = font(fs, 900); c.textAlign = 'center'; c.textBaseline = 'middle';
      c.lineWidth = 12; c.strokeStyle = '#120a1a'; c.strokeText('FULL THROTTLE', 0, 0);
      var tg = c.createLinearGradient(0, -fs * 0.4, 0, fs * 0.4);
      tg.addColorStop(0, '#fff3a8'); tg.addColorStop(0.5, '#ffc93c'); tg.addColorStop(0.52, '#ff4fb8'); tg.addColorStop(1, '#b44dff');
      c.fillStyle = tg; c.fillText('FULL THROTTLE', 0, 0);
      c.restore();
    }
    var a2 = easeOut((T - 1.1) / 0.6);
    c.globalAlpha = a2;
    c.fillStyle = '#36f1ff'; c.font = font(16, 800, true); c.fillText('THIS SUMMER, THE ROAD IS THE WEAPON.', cx, cy + fs * 0.62);
    c.fillStyle = '#e8f3f7'; c.font = font(13, 700, true);
    c.fillText('STARRING  YOU  AS  AGENT NOVA KANE', cx, cy + fs * 0.62 + 28);
    // the level picker: arrows (or a tap on it) switch between the two
    var lv = E.LEVELS[ui.level || 0], ly2 = cy + fs * 0.62 + 62;
    c.fillStyle = 'rgba(8,12,18,0.78)'; rr(cx - 230, ly2 - 17, 460, 34, 8); c.fill();
    c.strokeStyle = ui.level ? '#e84aff' : '#36f1ff'; c.lineWidth = 1.5; rr(cx - 230, ly2 - 17, 460, 34, 8); c.stroke();
    c.fillStyle = '#ffd23f'; c.font = font(16, 900);
    c.fillText('◀   LEVEL ' + lv.n + ':  ' + lv.zone + '  ·  ' + lv.title + '   ▶', cx, ly2 + 1, 450);
    ui.levelBox = { x: cx - 230, y: ly2 - 17, w: 460, h: 34, k: scale };
    var ky = viewH - bar - (ui.touch ? 70 : 114);
    // 1.7: the controls button sits under the level picker (or just above the instructions if space is short)
    // (on a short screen there's no room below, so it goes beside the picker)
    if (ly2 + 38 <= ky - 36 || viewW < cx + 230 + 180) ctlButton(ui, cx, Math.min(ly2 + 38, ky - 36));
    else ctlButton(ui, cx + 230 + 16 + (ui.touch ? 64 : 82), ly2);
    // a dark band behind the instructions so they read over the bright attract scene
    c.globalAlpha = a2 * 0.7; c.fillStyle = 'rgba(8,6,20,0.75)'; rr(cx - 360, ky - 22, 720, ui.touch ? 78 : 144, 10); c.fill(); c.globalAlpha = a2;
    c.textAlign = 'center'; c.textBaseline = 'middle';
    if (Math.floor(t * 1.6) % 2 === 0 || T < 2) {
      c.fillStyle = '#ffd23f'; c.font = font(20, 900);
      c.fillText(ui.touch ? 'TAP TO ROLL CAMERA' : ui.pad ? 'PRESS A TO ROLL CAMERA' : 'PRESS ENTER TO ROLL CAMERA', cx, ky);
    }
    var K = ui.keyText || function () { return ''; };
    if (!ui.touch) {
      // 1.7: the lines read the player's own bindings (or the pad's buttons when a pad was used last)
      // 1.7.1: the hands are split. WASD layout: the left hand drives (WASD, Shift drift, Space turbo),
      // the right hand shoots (J, K or the mouse). The arrows layout shoots with Z and X instead.
      c.fillStyle = '#c8dbe3'; c.font = font(12, 700, true);
      var wasd = !ui.pad && (ui.layout || 0) === 0, hands = wasd && ui.keyText('steer') === 'WASD';
      c.fillText((hands ? 'LEFT HAND:  ' : '') + K('steer') + ' STEER  ·  HOLD ' + K('drift') + ' + STEER TO DRIFT, LET GO TO BOOST  ·  ' + K('turbo') + ' TURBO', cx, ky + 31, 700);
      c.fillText(ui.pad ? K('fire') + '  MACHINE GUNS     ' + K('missile') + '  MISSILES     START  PAUSE' :
        (hands ? 'RIGHT HAND:  ' : '') + K('fire') + ' / LEFT CLICK  MACHINE GUNS  ·  ' + K('missile') + ' / RIGHT CLICK  MISSILES', cx, ky + 49, 700);
      if (!ui.pad) {
        var o = 1 - (ui.layout || 0);
        c.fillStyle = '#9fb3bd';
        c.fillText(K('steer', o) + ' INSTEAD?  STEER WITH ' + (o ? 'THEM' : 'IT') + ' AND ' + K('fire', o) + ' FIRES THE GUNS, ' + K('missile', o) + ' A MISSILE', cx, ky + 67, 700);
      }
      c.fillStyle = '#7f95a0'; c.fillText('NEAR MISSES, STUNTS AND WRECKAGE PAY.  CIVILIANS DON’T.', cx, ky + 86, 700);
      c.fillStyle = '#ff9ccf'; c.fillText(ui.level === 2 ? 'A BIKER WHIRLING A CHAIN?  GET CLEAR, OR SLAM HIM OFF HIS CHOPPER.' : 'HENCHMEN ON THE CAR?  SLAM THEIR SIDE INTO THE WALL, OR GO UPSIDE DOWN.', cx, ky + 103, 700);
    } else {
      c.fillStyle = '#9fb3bd'; c.font = font(12, 700, true);
      c.fillText('LEFT THUMB: DRAG TO DRIVE   ·   HOLD DRIFT + STEER, LET GO TO BOOST   ·   FIRE, MSL, TURBO', cx, ky + 32, 700);
      c.fillStyle = '#ff9ccf'; c.fillText(ui.level === 2 ? 'CHAIN WHIRLING? SLAM THE BIKER OFF' : 'HENCHMEN ON THE CAR? SLAM THEM INTO THE WALL', cx, ky + 50);
    }
    if (ui.best) {
      c.fillStyle = '#c9b48a'; c.font = font(11, 800, true);
      c.fillText('BEST TAKE: ' + ui.best.score + '  ·  ' + ui.best.title, cx, viewH - bar / 2);
    }
    c.globalAlpha = 1;
  }

  function sceneCard(ui, t) {
    var T = ui.t;
    c.fillStyle = '#05060a'; c.fillRect(0, 0, viewW, viewH);
    var cx = viewW / 2, cy = viewH * 0.4, s = Math.min(1, viewW / 760);
    // the clapperboard
    var slide = easeOut(T / 0.45), bx = cx - 170 * s, by = cy - 120 * s + (1 - slide) * -300;
    c.save(); c.translate(bx, by); c.scale(s, s);
    c.fillStyle = '#16181c'; c.fillRect(0, 40, 340, 170);
    c.strokeStyle = '#e8e8e8'; c.lineWidth = 2; c.strokeRect(6, 46, 328, 158);
    c.beginPath(); c.moveTo(6, 100); c.lineTo(334, 100); c.moveTo(6, 152); c.lineTo(334, 152); c.moveTo(170, 100); c.lineTo(170, 204); c.stroke();
    c.fillStyle = '#e8e8e8'; c.font = font(11, 800, true); c.textAlign = 'left'; c.textBaseline = 'alphabetic';
    c.fillText('PROD.', 14, 64); c.fillText('SCENE', 14, 116); c.fillText('TAKE', 178, 116); c.fillText('STARRING', 14, 168); c.fillText('ROLL', 178, 168);
    c.font = font(26, 900); c.fillText('FULL THROTTLE', 14, 92);
    var cd = ui.card || { n: 0, name: E.SCENES[0].name, take: 1 };
    c.fillText(String(cd.n + 1), 14, 144); c.fillText(String(cd.take), 178, 144); c.font = font(20, 900); c.fillText('YOU', 14, 194); c.fillText('A-' + (40 + ui.take), 178, 194);
    // the sticks: open, then clack shut at 1.0 s
    var ang = T < 0.7 ? -0.42 : T < 1.0 ? -0.42 * (1 - easeOut((T - 0.7) / 0.3)) : 0;
    c.save(); c.translate(0, 38); c.rotate(ang);
    c.fillStyle = '#16181c'; c.fillRect(0, -26, 340, 26);
    c.fillStyle = '#e8e8e8';
    for (var i = 0; i < 7; i++) { c.beginPath(); c.moveTo(10 + i * 50, -26); c.lineTo(36 + i * 50, -26); c.lineTo(20 + i * 50, 0); c.lineTo(-6 + i * 50, 0); c.closePath(); c.fill(); }
    c.restore();
    c.fillStyle = '#16181c'; c.fillRect(0, 12, 340, 26);
    c.fillStyle = '#e8e8e8';
    for (i = 0; i < 7; i++) { c.beginPath(); c.moveTo(-6 + i * 50, 12); c.lineTo(20 + i * 50, 12); c.lineTo(36 + i * 50, 38); c.lineTo(10 + i * 50, 38); c.closePath(); c.fill(); }
    c.restore();
    // the scene title
    var a = easeOut((T - 1.15) / 0.5);
    c.globalAlpha = a; c.textAlign = 'center'; c.textBaseline = 'middle';
    c.fillStyle = '#ff3ea5'; c.font = font(Math.min(22, viewW / 26), 900, true);
    c.fillText(cd.retake ? 'CUT! AGAIN.  SCENE ' + (cd.n + 1) + ',  TAKE ' + cd.take : 'SCENE ' + (cd.n + 1) + ' OF ' + E.SCENES.length, cx, cy + 140 * s + 10);
    c.fillStyle = '#fff'; c.font = font(Math.min(40, viewW / 17), 900);
    c.save(); c.translate(cx, cy + 140 * s + 50); c.scale(1 + (1 - a) * 0.15, 1 + (1 - a) * 0.15);
    c.fillText(cd.name, 0, 0, viewW - 30); c.restore();
    c.fillStyle = '#9fb3bd'; c.font = font(13, 700, true);
    c.fillText(cd.retake ? 'RESET THE CARS.  FRESH PAINT ON THE KESTREL.  ROLLING...' : 'LEVEL ' + E.SCENE.level + ':  ' + E.SCENE.title + '  ·  ' + E.SCENE.place, cx, cy + 140 * s + 88, viewW - 30);
    c.globalAlpha = 1;
    // fade to the road and shout ACTION
    if (T > 2.9) { c.fillStyle = 'rgba(0,0,0,' + clamp((T - 2.9) / 0.3, 0, 1) + ')'; c.fillRect(0, 0, viewW, viewH); }
  }

  function action(ui) {
    // a quick fade up from black as the camera rolls; the director shouts ACTION!
    var T = ui.playT;
    if (T < 0.25) { c.fillStyle = 'rgba(0,0,0,' + (1 - T / 0.25) + ')'; c.fillRect(0, 0, viewW, viewH); }
  }

  function ratingScreen(S, ui, t) {
    var T = ui.t, cx = viewW / 2;
    c.fillStyle = 'rgba(3,4,8,' + clamp(T / 0.4, 0, 0.86) + ')'; c.fillRect(0, 0, viewW, viewH);
    var pw = Math.min(560, viewW - 30), ph = Math.min(520, viewH - 30), px = cx - pw / 2, py = viewH / 2 - ph / 2;
    var s = easeOutBack(T / 0.4);
    c.save(); c.translate(cx, viewH / 2); c.scale(s, s); c.translate(-cx, -viewH / 2);
    panel(px, py, pw, ph);
    var sc = Math.min(1, ph / 520);
    c.textAlign = 'center'; c.textBaseline = 'middle';
    c.fillStyle = '#ff3ea5'; c.font = font(12, 800, true);
    c.fillText('RUN ' + S.take + ' · ' + S.retakes + (S.retakes === 1 ? ' RETAKE' : ' RETAKES') + ' · ' + (S.outcome === 'wrapped' ? 'IN THE CAN' : S.outcome === 'wrecked' ? 'STAR CAR WRECKED' : 'TARGET ESCAPED'), cx, py + 26 * sc);
    c.fillStyle = '#fff'; c.font = font(26 * sc + 4, 900);
    c.fillText('THE DAILIES', cx, py + 56 * sc);
    var rows = [
      ['DESTRUCTION', S.counts.kills + ' wrecked', S.tally.destruction],
      ['NEAR MISSES', S.counts.near + ' close shaves', S.tally.near],
      ['STUNTS', S.counts.stunts + ' landed', S.tally.stunts],
      ['ARMOR BONUS', Math.round(Math.max(0, S.p.hp)) + '% left', S.tally.armor],
      ['COLLATERAL', S.counts.civ + ' civilians', S.tally.collateral]
    ];
    var ry = py + 92 * sc, rh = 28 * sc, lx = px + 26, rx = px + pw - 26;
    for (var i = 0; i < rows.length; i++) {
      var k = clamp((T - 0.5 - i * 0.28) / 0.25, 0, 1);
      if (k <= 0) break;
      c.globalAlpha = k;
      c.textAlign = 'left'; c.fillStyle = '#9fb3bd'; c.font = font(13, 800, true); c.fillText(rows[i][0], lx, ry + i * rh);
      c.fillStyle = '#5f7c88'; c.font = font(11, 700, true); c.fillText(rows[i][1], lx + 150, ry + i * rh);
      c.textAlign = 'right'; c.fillStyle = rows[i][2] < 0 ? '#ff5a4e' : '#e8f3f7'; c.font = font(15, 800, true);
      c.fillText((rows[i][2] > 0 ? '+' : '') + Math.round(rows[i][2] * k), rx, ry + i * rh);
      c.globalAlpha = 1;
    }
    var ty = ry + rows.length * rh + 8 * sc;
    c.fillStyle = 'rgba(54,241,255,0.25)'; c.fillRect(lx, ty - 10, pw - 52, 1);
    var tk = clamp((T - 2) / 0.6, 0, 1);
    c.textAlign = 'left'; c.fillStyle = '#ffd23f'; c.font = font(15, 900, true); c.fillText('BOX OFFICE', lx, ty + 12);
    c.textAlign = 'right'; c.font = font(26, 900, true); c.fillText(String(Math.round(S.score * tk)), rx, ty + 12);
    // the stamp
    if (T > 2.7) {
      var st = easeOutBack((T - 2.7) / 0.3), R = S.rating, sy0 = ty + 100 * sc;
      c.save(); c.translate(cx, sy0); c.rotate(-0.06); c.scale(2.2 - 1.2 * st, 2.2 - 1.2 * st); c.globalAlpha = clamp(st, 0, 1);
      c.fillStyle = '#9fb3bd'; c.font = font(12, 800, true); c.textAlign = 'center'; c.fillText('DIRECTOR’S RATING', 0, -38 * sc);
      var col = R.key === 'legend' ? '#ffd23f' : R.key === 'blockbuster' ? '#ff3ea5' : R.key === 'hero' ? '#36f1ff' : '#c9b48a';
      c.font = font(Math.min(40, pw / 11), 900);
      var tw = c.measureText(R.title).width + 36;
      c.strokeStyle = col; c.lineWidth = 4; rr(-tw / 2, -24, tw, 48, 6); c.stroke();
      c.fillStyle = col; c.fillText(R.title, 0, 2);
      c.font = font(18, 900); var stars = '';
      for (var si = 0; si < 4; si++) stars += si < R.stars ? '★' : '☆';
      c.fillText(stars, 0, 42 * sc);
      c.restore();
      c.globalAlpha = clamp((T - 3.1) / 0.4, 0, 1);
      c.fillStyle = '#e8f3f7'; c.font = font(13, 700); c.textAlign = 'center';
      c.fillText(R.quip, cx, sy0 + 72 * sc, pw - 30);
      if (S.outcome === 'wrecked') { c.fillStyle = '#ff6a5e'; c.font = font(11, 800, true); c.fillText('A WRECKED STAR CAR CAPS THE RATING AT B-MOVIE', cx, sy0 + 92 * sc); }
      else if (S.outcome === 'escaped') { c.fillStyle = '#ff6a5e'; c.font = font(11, 800, true); c.fillText(E.KIND.boss.name + ' ESCAPED: RATING CAPPED AT ACTION HERO', cx, sy0 + 92 * sc); }
      else if (ui.newBest) { c.fillStyle = '#ffd23f'; c.font = font(11, 800, true); c.fillText('★ NEW BEST TAKE ★', cx, sy0 + 92 * sc); }
      c.globalAlpha = 1;
    }
    // 1.8: the skill points this level paid, banked for the GARAGE
    if (T > 3.2 && ui.earn) {
      var ek = clamp((T - 3.2) / 0.3, 0, 1), er = ui.earn, ey = py + ph - 52 * sc;
      c.globalAlpha = ek; c.textAlign = 'center'; c.textBaseline = 'middle';
      if (er.total > 0) {
        c.fillStyle = '#5bff73'; c.font = font(17, 900, true);
        c.fillText('+' + er.total + ' SKILL POINT' + (er.total === 1 ? '' : 'S'), cx, ey - 9);
        c.fillStyle = '#9fb3bd'; c.font = font(10, 800, true);
        c.fillText(er.lines.map(function (l) { return l.label + ' +' + l.n; }).join('   \u00b7   '), cx, ey + 9, pw - 30);
      } else { c.fillStyle = '#ff6a5e'; c.font = font(12, 800, true); c.fillText('NO SKILL POINTS: THE TAKE NEVER MADE IT TO THE CAN', cx, ey, pw - 30); }
      c.globalAlpha = 1;
    }
    if (T > 3.4) {
      c.fillStyle = Math.floor(t * 1.6) % 2 ? '#ffd23f' : '#fff'; c.font = font(14, 900); c.textAlign = 'center';
      var gar = !!(ui.earn && ui.earn.total > 0);
      c.fillText(ui.touch ? (gar ? 'TAP: TO THE GARAGE' : 'TAP: ANOTHER TAKE') : ui.pad ? (gar ? 'A: TO THE GARAGE   ·   Y: RESHOOT   ·   B: TITLE' : 'A: ANOTHER TAKE   ·   B: TITLE') : (gar ? 'ENTER: TO THE GARAGE   ·   R: RESHOOT   ·   ESC: TITLE' : 'ENTER: ANOTHER TAKE   ·   ESC: TITLE'), cx, py + ph - 22 * sc);
    }
    c.restore();
  }

  // ---------- 1.3: the highlight reel's titles, in scope letterbox ----------
  function reelOverlay(ui, t) {
    var sh = ui.reel, T = ui.t, bar = Math.max(viewH * 0.1, (viewH - viewW / 2.39) / 2);
    // the film: a flicker, and now and then a scratch down the frame
    if (!REDUCED) {
      c.fillStyle = 'rgba(255,236,210,' + (Math.random() * 0.035) + ')'; c.fillRect(0, 0, viewW, viewH);
      if (Math.random() < 0.08) { c.fillStyle = 'rgba(255,255,255,0.25)'; c.fillRect(Math.random() * viewW, bar, 1, viewH - bar * 2); }
    }
    c.fillStyle = '#000'; c.fillRect(0, 0, viewW, bar); c.fillRect(0, viewH - bar, viewW, bar);
    if (!sh) return;
    var st = sh.shotT, lv = E.LEVELS[E.level || 0] || E.LEVELS[0];
    // a hard cut: one white frame
    if (st < 0.1 && sh.index > 0 && !REDUCED) { c.fillStyle = 'rgba(255,255,255,' + (1 - st / 0.1) * 0.5 + ')'; c.fillRect(0, bar, viewW, viewH - bar * 2); }
    c.textBaseline = 'middle';
    // the production strip in the bars
    c.font = font(11, 800, true); c.textAlign = 'left'; c.fillStyle = '#ff3ea5';
    c.fillText('\u25cf DAILIES  \u00b7  HIGHLIGHTS', 18, bar / 2);
    c.fillStyle = '#9fb3bd'; c.fillText('SHOT ' + (sh.index + 1) + ' / ' + sh.count, 190, bar / 2);
    c.textAlign = 'right'; c.fillStyle = '#e8f3f7';
    c.fillText(sh.camName + '     ' + timecode(T + 3600 * 0), viewW - 18, bar / 2);
    c.textAlign = 'left'; c.fillStyle = '#7f95a0';
    c.fillText('LEVEL ' + lv.n + '  \u00b7  ' + lv.title, 18, viewH - bar / 2);
    if (sh.slow) { c.textAlign = 'right'; c.fillStyle = '#e8f3f7'; c.fillText('\u25b6\u25b6  SLOW MOTION', viewW - 18, viewH - bar / 2); }
    continueButton(ui, t, bar);
    // 1.8: the reel plays on, round again, until CONTINUE
    if (sh.loops > 0 && sh.index === 0 && sh.shotT < 2.2) {
      var fa = clamp(sh.shotT / 0.25, 0, 1) * clamp((2.2 - sh.shotT) / 0.5, 0, 1);
      c.globalAlpha = fa; c.textAlign = 'center'; c.font = font(Math.min(30, viewW / 22), 900);
      c.lineWidth = 6; c.strokeStyle = '#120a1a'; c.strokeText('FROM THE TOP', viewW / 2, bar + 40);
      c.fillStyle = '#ffd23f'; c.fillText('FROM THE TOP', viewW / 2, bar + 40); c.globalAlpha = 1;
    }
    // the opening title
    if (sh.index === 0 && T < 3.4) {
      var a = clamp(T / 0.5, 0, 1) * clamp((3.4 - T) / 0.6, 0, 1), cx = viewW / 2, cy = viewH * 0.42;
      c.globalAlpha = a * 0.55; c.fillStyle = '#05060a'; c.fillRect(0, cy - 70, viewW, 150);
      c.globalAlpha = a; c.textAlign = 'center';
      c.fillStyle = '#c9b48a'; c.font = font(13, 800, true); c.fillText('ROADBURN  PICTURES  PRESENTS', cx, cy - 44);
      var fs = Math.min(64, viewW / 11);
      c.save(); c.translate(cx, cy + 4); c.transform(1, 0, -0.18, 1, 0, 0);
      c.font = font(fs, 900); c.lineWidth = 9; c.strokeStyle = '#120a1a'; c.strokeText('THE HIGHLIGHTS', 0, 0);
      var tg = c.createLinearGradient(0, -fs * 0.4, 0, fs * 0.4);
      tg.addColorStop(0, '#fff3a8'); tg.addColorStop(0.5, '#ffc93c'); tg.addColorStop(0.52, '#ff4fb8'); tg.addColorStop(1, '#b44dff');
      c.fillStyle = tg; c.fillText('THE HIGHLIGHTS', 0, 0); c.restore();
      c.fillStyle = '#36f1ff'; c.font = font(14, 800, true); c.fillText('LEVEL ' + lv.n + ':  ' + lv.zone + '  \u00b7  ' + lv.title, cx, cy + 50);
      c.globalAlpha = 1;
      continueButton(ui, t, bar);
      return;
    }
    // the lower third: the scene, and what happened
    if (st > 0.2) {
      var k = easeOut((st - 0.2) / 0.35), lx = 26 - (1 - k) * 60, ly = viewH - bar - 64;
      c.globalAlpha = k;
      c.fillStyle = 'rgba(5,6,10,0.72)'; c.fillRect(lx - 10, ly - 18, Math.min(viewW * 0.62, 470), 62);
      c.fillStyle = '#ff3ea5'; c.fillRect(lx - 10, ly - 18, 4, 62);
      c.textAlign = 'left'; c.fillStyle = '#ff9ccf'; c.font = font(11, 800, true);
      c.fillText(sh.again ? 'ANOTHER ANGLE' : 'SCENE ' + (sh.scene + 1) + '  \u00b7  ' + (sh.sceneName || ''), lx, ly - 4);
      c.fillStyle = '#ffffff'; c.font = font(Math.min(30, viewW / 22), 900);
      c.save(); c.translate(lx, ly + 22); c.transform(1, 0, -0.14, 1, 0, 0); c.fillText(sh.label || '', 0, 0, Math.min(viewW * 0.6, 450)); c.restore();
      c.globalAlpha = 1;
    }
    // the last shot fades out on the wrap
    if (sh.index === sh.count - 1 && sh.left < 1.4 && !sh.loops) {
      var f = clamp((1.4 - sh.left) / 1.1, 0, 1);
      c.fillStyle = 'rgba(0,0,0,' + f * 0.85 + ')'; c.fillRect(0, bar, viewW, viewH - bar * 2);
      c.globalAlpha = f; c.textAlign = 'center'; c.fillStyle = '#ffd23f'; c.font = font(Math.min(40, viewW / 16), 900);
      c.fillText('PRINT IT. THAT\u2019S A WRAP.', viewW / 2, viewH / 2); c.globalAlpha = 1;
    }
  }

  // 1.8: the one way out of the highlight reel: a button pressed on purpose (a click elsewhere, Space or Esc
  // do nothing). ENTER, the pad's START, or a click on the button itself. Armed after 0.8 s.
  function continueButton(ui, t, bar) {
    var armed = ui.t > 0.8, x = viewW / 2, y = viewH - bar / 2, h = Math.max(24, Math.min(34, bar - 10)), w = 250;
    var hint = ui.touch ? 'TAP' : 'ENTER · START';
    var hot = armed && ui.reelHover;
    c.globalAlpha = armed ? 1 : 0.4;
    c.fillStyle = hot ? 'rgba(255,79,184,0.5)' : 'rgba(30,8,36,0.95)'; rr(x - w / 2, y - h / 2, w, h, h / 2); c.fill();
    c.strokeStyle = !armed ? '#7f95a0' : hot ? '#ffd23f' : (Math.floor(t * 1.6) % 2 ? '#ff4fb8' : '#ffd23f'); c.lineWidth = 2; rr(x - w / 2, y - h / 2, w, h, h / 2); c.stroke();
    c.textAlign = 'center'; c.textBaseline = 'middle';
    c.fillStyle = '#ffffff'; c.font = font(15, 900); c.fillText('CONTINUE ▶', x - (ui.touch ? 24 : 48), y + 1);
    c.fillStyle = '#ffd23f'; c.font = font(10, 800, true); c.fillText('[' + hint + ']', x + (ui.touch ? 60 : 64), y + 1);
    c.globalAlpha = 1;
    ui.reelBtn = { x: x - w / 2, y: y - h / 2, w: w, h: h, k: scale };
  }

  // ---------- 1.8: THE GARAGE, between levels: spend the skill points ----------
  // Everything on it comes from skills.js (FT_SKILLS): one card per upgrade, then the button that starts the
  // next level. The coming skill tree adds upgrades (with 'requires'); a locked one draws dimmed.
  function garageScreen(S, ui, t) {
    var K = window.FT_SKILL, G = ui.gar, P = ui.sk, D = K.data, ups = D.upgrades, n = ups.length;
    c.fillStyle = 'rgba(3,4,10,' + clamp(ui.t / 0.3, 0, 0.88) + ')'; c.fillRect(0, 0, viewW, viewH);
    var cols = viewW >= 760 ? Math.min(4, n) : 2, rows = Math.ceil(n / cols), gap = 12;
    G.cols = cols;
    var pw = Math.min(940, viewW - 20), cw = (pw - 36 - gap * (cols - 1)) / cols, ch = 200, HEAD = 142, FOOT = 104;
    var ph = HEAD + rows * ch + (rows - 1) * gap + FOOT, px = viewW / 2 - pw / 2;
    if (ph > viewH - 8) { ch = Math.max(150, (viewH - 8 - HEAD - FOOT - (rows - 1) * gap) / rows); ph = HEAD + rows * ch + (rows - 1) * gap + FOOT; }
    var py = Math.max(4, viewH / 2 - ph / 2), cx = viewW / 2;
    panel(px, py, pw, ph);
    c.textAlign = 'center'; c.textBaseline = 'middle';
    c.fillStyle = '#ff4fb8'; c.font = font(11, 800, true);
    c.fillText(G.subtitle, cx, py + 20, pw - 30);
    c.save(); c.translate(cx, py + 52); c.transform(1, 0, -0.16, 1, 0, 0);
    c.font = font(34, 900); c.lineWidth = 6; c.strokeStyle = '#120a1a'; c.strokeText('THE GARAGE', 0, 0);
    var tg = c.createLinearGradient(0, -14, 0, 14); tg.addColorStop(0, '#fff3a8'); tg.addColorStop(0.5, '#ffc93c'); tg.addColorStop(0.52, '#ff4fb8'); tg.addColorStop(1, '#b44dff');
    c.fillStyle = tg; c.fillText('THE GARAGE', 0, 0); c.restore();
    // the bank
    var by = py + 94;
    c.fillStyle = 'rgba(91,255,115,0.10)'; rr(cx - 160, by - 19, 320, 38, 8); c.fill();
    c.strokeStyle = 'rgba(91,255,115,0.6)'; c.lineWidth = 1.5; rr(cx - 160, by - 19, 320, 38, 8); c.stroke();
    c.fillStyle = '#9fb3bd'; c.font = font(13, 800, true); c.textAlign = 'right'; c.fillText('SKILL POINTS', cx + 22, by + 1);
    c.fillStyle = '#5bff73'; c.font = font(28, 900, true); c.textAlign = 'left'; c.fillText(String(P.points), cx + 36, by + 2);
    c.textAlign = 'center'; c.fillStyle = '#7fa9b8'; c.font = font(10, 800, true);
    c.fillText(G.earned > 0 ? '+' + G.earned + ' EARNED THIS LEVEL' + (G.earnLine ? '   (' + G.earnLine + ')' : '') : 'NONE EARNED THIS LEVEL', cx, by + 30, pw - 30);
    // the cards
    var boxes = [], top = py + HEAD;
    for (var i = 0; i < n; i++) {
      var u = ups[i], r = K.rank(P, u.id), lk = K.locked(P, u), cost = K.nextCost(P, u), maxed = r >= u.maxRank;
      var col = u.color || '#36f1ff', sel = G.sel === i, x = px + 18 + (i % cols) * (cw + gap), y = top + Math.floor(i / cols) * (ch + gap);
      var afford = !lk && !maxed && P.points >= cost;
      c.fillStyle = sel ? 'rgba(255,79,184,0.16)' : 'rgba(255,255,255,0.05)'; rr(x, y, cw, ch, 8); c.fill();
      c.strokeStyle = sel ? '#ff4fb8' : 'rgba(54,241,255,0.28)'; c.lineWidth = sel ? 2.5 : 1; rr(x, y, cw, ch, 8); c.stroke();
      c.fillStyle = col; c.fillRect(x + 12, y + 10, 30, 3);
      c.globalAlpha = lk ? 0.45 : 1;
      c.textAlign = 'left'; c.textBaseline = 'alphabetic';
      c.fillStyle = col; c.font = font(19, 900); c.fillText(u.name, x + 12, y + 36, cw - 24);
      c.fillStyle = '#c8dbe3'; c.font = font(11, 700);
      var dl = wrapText(u.desc || '', cw - 24), nd = Math.min(3, dl.length);
      for (var di = 0; di < nd; di++) c.fillText(dl[di], x + 12, y + 54 + di * 14);
      // rank pips
      var pyy = y + 50 + nd * 14, pwid = Math.min(26, (cw - 80 - (u.maxRank - 1) * 4) / u.maxRank);
      for (var k = 0; k < u.maxRank; k++) { c.fillStyle = k < r ? col : 'rgba(255,255,255,0.12)'; rr(x + 12 + k * (pwid + 4), pyy, pwid, 9, 3); c.fill(); }
      c.fillStyle = '#7fa9b8'; c.font = font(9, 800, true); c.textAlign = 'right'; c.fillText('RANK ' + r + '/' + u.maxRank, x + cw - 12, pyy + 8);
      // what it does, now and at the next rank
      var ey = pyy + 28;
      c.textAlign = 'left';
      (u.effects || []).forEach(function (e) {
        var st = D.stats[e.stat] || { label: e.stat };
        var now = K.effectText(e, r).split(' ')[0], nx = K.effectText(e, r + 1).split(' ')[0];
        c.fillStyle = '#7fa9b8'; c.font = font(9, 800, true); c.fillText(String(st.label).toUpperCase(), x + 12, ey);
        c.font = font(12, 900, true); c.fillStyle = r ? '#e8f3f7' : '#5f7c88'; c.fillText(now, x + 12, ey + 15);
        if (!maxed) {
          var nw = c.measureText(now).width, ar = '  →  ';
          c.fillStyle = '#5f7c88'; c.fillText(ar, x + 12 + nw, ey + 15);
          c.fillStyle = col; c.fillText(nx, x + 12 + nw + c.measureText(ar).width, ey + 15);
        }
        ey += 31;
      });
      c.globalAlpha = 1;
      // the price
      var fy = y + ch - 14;
      c.textAlign = 'center'; c.font = font(11, 900, true);
      if (lk) { var rq = K.byId(lk.id); c.fillStyle = '#ff6a5e'; c.fillText('LOCKED: NEEDS ' + (rq ? rq.name : lk.id) + ' RANK ' + lk.rank, x + cw / 2, fy, cw - 16); }
      else if (maxed) { c.fillStyle = '#ffd23f'; c.fillText('★ MAXED ★', x + cw / 2, fy); }
      else { c.fillStyle = afford ? '#5bff73' : '#ff9e3d'; c.fillText('NEXT RANK: ' + cost + ' PT' + (cost === 1 ? '' : 'S') + (afford ? '' : '  (NEED ' + (cost - P.points) + ' MORE)'), x + cw / 2, fy, cw - 16); }
      if (G.flash && G.flash.i === i && G.flash.t > 0) { c.globalAlpha = clamp(G.flash.t / 0.35, 0, 1) * 0.45; c.fillStyle = G.flash.ok ? col : '#ff3a3a'; rr(x, y, cw, ch, 8); c.fill(); c.globalAlpha = 1; }
      boxes.push({ x: x, y: y, w: cw, h: ch, k: scale, i: i });
    }
    // the confirm button
    var bw = 330, bh = 44, bx = cx - bw / 2, byy = top + rows * ch + (rows - 1) * gap + 16, bsel = G.sel === n;
    var pulse = 0.5 + 0.5 * Math.sin(t * 4);
    c.fillStyle = bsel ? 'rgba(255,79,184,' + (0.35 + 0.2 * pulse) + ')' : 'rgba(30,8,36,0.95)'; rr(bx, byy, bw, bh, bh / 2); c.fill();
    c.strokeStyle = bsel ? '#ffd23f' : '#ff4fb8'; c.lineWidth = bsel ? 3 : 2; rr(bx, byy, bw, bh, bh / 2); c.stroke();
    c.textAlign = 'center'; c.textBaseline = 'middle'; c.fillStyle = '#fff'; c.font = font(18, 900);
    c.fillText(G.confirm + '  ▶', cx, byy + bh / 2 + 1, bw - 20);
    boxes.push({ x: bx, y: byy, w: bw, h: bh, k: scale, i: n });
    G.boxes = boxes;
    // what just happened, then the keys
    var hy = byy + bh + 16;
    c.font = font(11, 800, true);
    if (G.msgT > 0) { c.globalAlpha = clamp(G.msgT, 0, 1); c.fillStyle = G.msgCol || '#ffd23f'; c.fillText(G.msg, cx, hy, pw - 30); c.globalAlpha = 1; }
    c.fillStyle = '#7fa9b8'; c.font = font(10, 800, true);
    c.fillText(ui.touch ? 'TAP A CARD TO BUY A RANK  ·  TAP THE BUTTON WHEN YOU ARE DONE' :
      ui.pad ? 'D-PAD / STICK PICK  ·  A BUY  ·  X REFUND  ·  START: ' + G.confirm :
      '←→ PICK  ·  ENTER / A BUY  ·  BACKSPACE / X REFUND  ·  THE BUTTON OR GAMEPAD START GOES ON  ·  MOUSE: CLICK A CARD TO BUY, RIGHT-CLICK TO REFUND', cx, hy + 20, pw - 24);
  }

  function paused(ui) {
    c.fillStyle = 'rgba(0,0,0,0.6)'; c.fillRect(0, 0, viewW, viewH);
    c.textAlign = 'center'; c.textBaseline = 'middle';
    c.fillStyle = '#ffd23f'; c.font = font(42, 900); c.fillText('CAMERAS HOLDING', viewW / 2, viewH / 2 - 20);
    c.fillStyle = '#e8f3f7'; c.font = font(14, 800, true);
    c.fillText(ui.touch ? 'TAP TO RESUME' : ui.pad ? 'START / A TO RESUME  ·  B OR BACK FOR TITLE' : 'P / ENTER TO RESUME  ·  ESC FOR TITLE  ·  M MUTE', viewW / 2, viewH / 2 + 26, viewW - 20);
    ctlButton(ui, viewW / 2, viewH / 2 + 66);
  }

  // ---------- 1.7: the controls screen ----------
  // a button that opens it, on the title and the pause screen
  function ctlButton(ui, x, y) {
    var tx = '⚙  CONTROLS' + (ui.touch ? '' : ui.pad ? '   [Y]' : '   [C]');
    c.font = font(13, 900, true); c.textAlign = 'center'; c.textBaseline = 'middle';
    var w = c.measureText(tx).width + 28, h = 26;
    c.fillStyle = 'rgba(8,12,18,0.82)'; rr(x - w / 2, y - h / 2, w, h, 6); c.fill();
    c.strokeStyle = '#ff4fb8'; c.lineWidth = 1.5; rr(x - w / 2, y - h / 2, w, h, 6); c.stroke();
    c.fillStyle = '#ffd0ec'; c.fillText(tx, x, y + 1);
    ui.ctlBtn = { x: x - w / 2, y: y - h / 2, w: w, h: h, k: scale };
  }
  function controlsScreen(ui, t) {
    var C = ui.ctl, rows = C.rows, B = C.binds, kn = C.keyName;
    c.fillStyle = 'rgba(3,4,10,0.72)'; c.fillRect(0, 0, viewW, viewH);
    var rh = 25, pw = Math.min(700, viewW - 24), ph = 176 + rows.length * rh, px = viewW / 2 - pw / 2, py = Math.max(8, viewH / 2 - ph / 2);
    panel(px, py, pw, ph);
    c.textAlign = 'center'; c.textBaseline = 'middle';
    c.fillStyle = '#ffd23f'; c.font = font(26, 900); c.fillText('CONTROLS', viewW / 2, py + 26);
    c.fillStyle = '#9fb3bd'; c.font = font(11, 700, true);
    c.fillText(C.wait ? '' : ui.touch ? 'ON A PHONE: DRAG ON THE LEFT TO DRIVE, HOLD DRIFT AND STEER, LET GO TO BOOST' :
      ui.pad && !ui.ctlKeyboard ? 'THE PAD IS FIXED (RIGHT COLUMN)  ·  KEYS ARE SET FROM THE KEYBOARD' : '↑↓ PICK  ·  ←→ LAYOUT  ·  ENTER OR CLICK, THEN PRESS THE NEW KEY  ·  DEL CLEARS', viewW / 2, py + 50, pw - 20);
    // the columns: action, the WASD layout's key, the arrows layout's key (1.7.1), then the fixed mouse and pad
    var cA = px + 18, kw = Math.min(118, (pw - 36) * 0.18), c1 = px + Math.max(150, pw * 0.3), c2 = c1 + kw + 10, cP = c2 + kw + 18, top = py + 82;
    var LY = C.layouts || ['KEY', 'OR'], inUse = ui.layout || 0;
    label('ACTION', cA, top - 8, '#5f7c88', 9);
    for (var hs = 0; hs < 2; hs++) label(LY[hs] + (hs === inUse ? '  ● IN USE' : ''), hs ? c2 : c1, top - 8, hs === inUse ? '#36f1ff' : '#5f7c88', 9);
    label(pw > 560 ? 'MOUSE  ·  GAMEPAD (FIXED)' : 'PAD (FIXED)', cP, top - 8, '#5f7c88', 9);
    var boxes = [];
    for (var i = 0; i < rows.length; i++) {
      var r = rows[i], y = top + i * rh, on = C.sel === i;
      if (on) { c.fillStyle = 'rgba(255,79,184,0.12)'; c.fillRect(px + 8, y - 2, pw - 16, rh - 2); }
      label(r.name, cA, y + 15, on ? '#ffffff' : '#c8dbe3', 11);
      for (var s = 0; s < 2; s++) {
        var bx = s ? c2 : c1, hot = on && C.slot === s, waiting = hot && C.wait;
        c.fillStyle = waiting ? (Math.floor(t * 4) % 2 ? '#ff4fb8' : '#7a1e58') : hot ? 'rgba(255,79,184,0.35)' : 'rgba(255,255,255,0.07)';
        rr(bx, y, kw, rh - 6, 4); c.fill();
        if (hot) { c.strokeStyle = '#ff4fb8'; c.lineWidth = 1.5; rr(bx, y, kw, rh - 6, 4); c.stroke(); }
        var code = B[r.id][s];
        label(waiting ? 'PRESS A KEY' : kn(code), bx + kw / 2, y + 14, waiting ? '#ffffff' : code ? '#ffd23f' : '#5f7c88', waiting ? 9 : 11, 'center');
        boxes.push({ x: bx, y: y, w: kw, h: rh - 6, k: scale, row: i, slot: s });
      }
      label((r.mouse && pw > 560 ? r.mouse + '  ·  ' : '') + r.pad, cP, y + 15, '#7fa9b8', 10);
    }
    // RESET and BACK
    var by = top + rows.length * rh + 12, bw = 190, gap = 16, bl = viewW / 2 - bw - gap / 2, br = viewW / 2 + gap / 2;
    [[bl, 'RESET TO DEFAULTS', rows.length], [br, ui.touch ? 'BACK' : ui.pad && !ui.ctlKeyboard ? 'BACK  [B]' : 'BACK  [ESC]', rows.length + 1]].forEach(function (q) {
      var on = C.sel === q[2];
      c.fillStyle = on ? 'rgba(255,79,184,0.35)' : 'rgba(255,255,255,0.07)'; rr(q[0], by, bw, 28, 6); c.fill();
      c.strokeStyle = on ? '#ff4fb8' : 'rgba(54,241,255,0.35)'; c.lineWidth = 1.5; rr(q[0], by, bw, 28, 6); c.stroke();
      c.fillStyle = on ? '#fff' : '#c8dbe3'; c.font = font(12, 900, true); c.textAlign = 'center'; c.textBaseline = 'middle'; c.fillText(q[1], q[0] + bw / 2, by + 15);
      boxes.push({ x: q[0], y: by, w: bw, h: 28, k: scale, row: q[2] });
    });
    ui.ctlBoxes = boxes;
    // what just happened, or a reminder of the fixed keys
    c.textAlign = 'center'; c.textBaseline = 'middle'; c.font = font(11, 800, true);
    if (C.msgT > 0) { c.globalAlpha = clamp(C.msgT, 0, 1); c.fillStyle = '#ffd23f'; c.fillText(C.msg, viewW / 2, by + 48, pw - 20); c.globalAlpha = 1; }
    else { c.fillStyle = '#5f7c88'; c.fillText(ui.touch ? 'FIRE · MSL · TURBO · DRIFT ARE THE BUTTONS ON THE RIGHT  ·  II PAUSES' : 'THE LAYOUT FOLLOWS THE KEYS YOU STEER WITH  ·  P / ESC PAUSE  ·  M SOUND  ·  ENTER / SPACE GO ON IN MENUS', viewW / 2, by + 48, pw - 20); }
    if (ui.padName && !ui.touch) { c.fillStyle = '#4cd07d'; c.font = font(9, 700, true); c.fillText('GAMEPAD CONNECTED: ' + String(ui.padName).slice(0, 60).toUpperCase(), viewW / 2, py + ph - 8, pw - 20); }
  }

  // screen text never drops below a readable size on a small phone
  function overlay(fn) {
    var k = scale < 0.8 ? 0.8 / scale : 1, vw = viewW, vh = viewH, sc = scale;
    if (k === 1) { fn(); return; }
    c.save(); c.scale(k, k); viewW = vw / k; viewH = vh / k; scale = sc * k;
    try { fn(); } finally { viewW = vw; viewH = vh; scale = sc; c.restore(); }
  }

  function frame(S, ui, t, rdt) {
    var t0 = performance.now();
    TNOW = t;
    var frozen = ui.screen === 'paused' || (ui.screen === 'controls' && ui.ctl && ui.ctl.from === 'paused');
    var fd = frozen ? 0 : rdt;
    FR = fd * 60;
    FX.hurt = Math.max(0, FX.hurt - fd * 1.5); FX.shock = Math.max(0, FX.shock - fd * 1.3); FX.flash = Math.max(0, FX.flash - fd * 5);
    FX.sky = Math.max(0, FX.sky - fd * 3.5);
    FX.boom = Math.max(0, FX.boom - fd * 3.2); FX.flinch = Math.max(0, FX.flinch - fd * 4);
    REELON = ui.screen === 'reel';
    if (LV2 && !REDUCED) {
      FX.skyT -= fd;
      if (FX.skyT <= 0) { FX.sky = 1; FX.skyX = Math.random() * 1.6 - 0.8; FX.skyT = (S && S.boss ? 1.5 : 3.5) + Math.random() * 5; }
    }
    stepFX(frozen ? null : S, frozen ? 0 : rdt);
    if (ui.screen === 'play' && !frozen) preroll(S);
    c.setTransform(dpr * scale, 0, 0, dpr * scale, 0, 0);
    c.fillStyle = '#05060a'; c.fillRect(0, 0, viewW, viewH);
    if (ui.screen === 'card') { overlay(function () { sceneCard(ui, t); }); return; }
    if (REELON) {
      // the highlight reel: the recorded take, filmed from the shot's own camera, with titles
      RCAM = ui.reel && ui.reel.cam || null;
      try { world(S, t, rdt); } finally { RCAM = null; }
      c.setTransform(dpr * scale, 0, 0, dpr * scale, 0, 0);
      postFX(S, ui, t);
      overlay(function () { reelOverlay(ui, t); });
      return;
    }
    world(S, t, rdt);
    c.setTransform(dpr * scale, 0, 0, dpr * scale, 0, 0);
    postFX(S, ui, t);
    var onTitle = ui.screen === 'title' || (ui.screen === 'controls' && ui.ctl.from !== 'paused');
    filmLook(onTitle ? null : S);
    if (ui.screen === 'controls') { overlay(function () { if (!onTitle) hud(S, ui, t); controlsScreen(ui, t); }); return; }
    if (ui.screen === 'title') { overlay(function () { title(ui, t); }); return; }
    if (ui.screen !== 'garage') overlay(function () { hud(S, ui, t); if (ui.screen === 'play') action(ui); });
    if (ui.screen === 'play') touchUI(ui);
    overlay(function () {
      if (ui.screen === 'paused') paused(ui);
      if (ui.screen === 'rating') ratingScreen(S, ui, t);
      if (ui.screen === 'garage') garageScreen(S, ui, t);
      if (ui.mutedHint > 0) {
        c.globalAlpha = clamp(ui.mutedHint, 0, 1); c.fillStyle = '#fff'; c.font = font(14, 800, true); c.textAlign = 'center';
        c.fillText(ui.muted ? 'SOUND OFF' : 'SOUND ON', viewW / 2, viewH - 20); c.globalAlpha = 1;
      }
    });
    // if the machine can't keep up, the post pass drops its bloom and grain for the rest of the session
    FRAME_MS = FRAME_MS * 0.96 + (performance.now() - t0) * 0.04;
    if (!LOWFX && !window.FT_FULLFX && FRAME_MS > 15 && t > 6) LOWFX = true;
  }


  // ======================= 1.7.2: warm-up, so nothing is made or uploaded in the middle of a race =======================
  // Everything the renderer used to make the first time it was needed is made ahead, a few milliseconds a
  // frame on the title and the scene card (and whatever is left, all at once as the race starts): each
  // sprite sheet is drawn once so the GPU has it before its first appearance (the boss sheets are 6,000+
  // px wide and stalled a frame by 30-40 ms when the boss rolled in), the white hit-flash and damage
  // overlays of every vehicle, the haze silhouettes of this level's props, the soft glow textures, and the
  // roadside prop lists for the whole track. The 1x1 draws land in the corner of the frame and are painted
  // over by the frame itself.
  var WARM_COLS = ['10,0,30', '110,225,255', '120,220,255', '120,255,160', '125,255,155', '130,230,255', '140,160,255', '140,220,255', '150,230,255', '150,235,255', '160,210,255', '190,160,255', '190,210,255', '200,160,255', '200,170,255', '200,90,255', '210,255,255', '215,227,240', '232,74,255', '255,110,40', '255,110,60', '255,120,200', '255,120,220', '255,120,30', '255,120,40', '255,120,90', '255,140,50', '255,150,60', '255,158,61', '255,160,70', '255,165,61', '255,170,80', '255,190,110', '255,190,140', '255,190,90', '255,200,120', '255,200,140', '255,201,60', '255,210,63', '255,210,90', '255,214,70', '255,214,90', '255,220,120', '255,220,140', '255,220,150', '255,220,160', '255,230,140', '255,230,80', '255,236,110', '255,236,170', '255,240,160', '255,240,200', '255,245,210', '255,250,230', '255,255,255', '255,30,40', '255,30,60', '255,40,40', '255,40,60', '255,40,70', '255,50,70', '255,60,60', '255,60,70', '255,70,60', '255,79,184', '255,80,170', '255,80,20', '255,80,62', '46,230,255', '54,241,255', '90,230,255', '95,208,255', '95,240,255'];
  var VEH = ['player', 'player_d1', 'player_d2', 'rammer', 'rammer_rev', 'suv', 'truck', 'bus', 'moto', 'heli', 'hog', 'hog2', 'ramp',
    'civ0', 'civ1', 'civ2', 'civ3', 'civ4', 'civ5', 'civ6', 'civ7', 'wheel_kr', 'wheel_kf', 'wheel_rm', 'wheel_suv'];
  var DMGK = ['player', 'player_d1', 'player_d2', 'rammer', 'rammer_rev', 'suv', 'truck', 'heli', 'hog', 'hog2'];
  var WQ = null, WI = 0, WTF = null;
  // every sheet decoded (or failed); after 10 s it gives up waiting, so a slow disk can't lock the title
  function artReady() { if (performance.now() - ART_T0 > 10000) return true; for (var k in SP) if (!IMG[k] && !ART_BAD[k]) return false; return true; }
  // drawn at 1:1 (one source pixel to one screen pixel): Chrome decodes an image at the scale it is drawn,
  // so a whole sheet squeezed into 1x1 got only a tiny decode and the real one still came mid-race
  function touch(im) { if (im) c.drawImage(im, 0, 0, 1, 1, 0, 0, 1, 1); }
  function warmQueue() {
    var q = [], k, i, boss = T.level === 2 ? 'boss2' : T.level === 3 ? 'boss3' : 'boss';
    for (i = 0; i < WARM_COLS.length; i++) q.push(['glow', WARM_COLS[i]]);
    q.push(['misc']);
    // the props of the whole track (and through them the haze sheets they need)
    var n = Math.ceil((TF.n * TF.step) / PSLOT) + 2;
    for (k = -2; k < n; k += 24) q.push(['slots', k]);
    q.push(['fog']);
    for (k in SP) q.push(['img', k]);
    q.push(['pins']);
    var veh = VEH.concat([boss]);
    for (i = 0; i < veh.length; i++) if (SP[veh[i]]) q.push(['wht', veh[i]]);
    var dk = DMGK.concat([boss]);
    for (i = 0; i < dk.length; i++) if (SP[dk[i]]) q.push(['dmg', dk[i]]);
    return q;
  }
  function warmOne(w) {
    switch (w[0]) {
      case 'glow': touch(glowImg(w[1])); break;
      case 'misc': touch(shadowImg()); postSetup(); touch(VIG); touch(VIGR); touch(VIGC); touch(GRN); break;
      case 'slots': for (var k = w[1]; k < w[1] + 24; k++) slotList(k); break;
      case 'fog':
        var seen = {};
        for (var key in PCACHE) { var L = PCACHE[key]; for (var i = 0; i < L.length; i++) seen[L[i].k] = 1; }
        // the reel cameras and the title see the same props; make and upload each haze sheet
        for (var pk in seen) WQ.splice(WI, 0, ['fog1', pk]);
        break;
      case 'fog1': touch(fogOf(w[1])); break;
      case 'pins': pinLevel(); break;
      case 'img': touch(IMG[w[1]]); break;
      case 'wht': touch(whiteOf(w[1])); break;
      case 'dmg': touch(dmgOf(w[1])); break;
    }
  }
  // Pinning: Chrome keeps decoded <img> pixels in a cache it trims as a race goes on, so a sheet not drawn
  // for a few minutes (the boss's, the SUV's, even the KESTREL's after a long reel) was decoded all over
  // again on its next appearance, 8-25 ms in one frame. The sheets the level uses are held as decoded
  // ImageBitmaps instead, which are never thrown away; the other levels' props and bosses are let go.
  function pinLevel() {
    if (!window.createImageBitmap) return;
    var boss = T.level === 2 ? 'boss2' : T.level === 3 ? 'boss3' : 'boss', sky = T.level === 2 ? 'chem_skyline' : T.level === 3 ? 'vegas' : 'skyline';
    var want = {}, k, i;
    for (i = 0; i < VEH.length; i++) want[VEH[i]] = 1;
    want[boss] = want[sky] = want.wreck = want.fx_fireball = want.fx_burst = want.fx_flame = want.fx_debris = 1;
    for (k in PCACHE) { var L = PCACHE[k]; for (i = 0; i < L.length; i++) want[L[i].k] = 1; }
    WANT = want;
    for (k in PIN) if (!want[k]) { if (ORIG[k]) IMG[k] = ORIG[k]; try { PIN[k].close(); } catch (e) { /* fine */ } delete PIN[k]; }
    for (k in want) if (SP[k] && ORIG[k] && !PIN[k]) (function (k) {
      createImageBitmap(ORIG[k]).then(function (bm) {
        if (WANT[k] && !PIN[k]) { PIN[k] = bm; IMG[k] = bm; PINQ.push(k); } else bm.close();
      }, function () { /* keep the <img> */ });
    })(k);
  }
  // The boss rolls in minutes into a race, by which time the GPU may have let its big sheets go: in the
  // last few seconds before its cue they are drawn once more (one a frame, 1x1, painted over by the frame).
  var PRE = { S: null, d: -1, i: 0 };
  function preroll(S) {
    if (!S || !S.script || !c) return;
    if (PRE.S !== S) {
      PRE.S = S; PRE.i = 0; PRE.d = -1;
      for (var i = 0; i < S.script.length; i++) { var it = S.script[i]; if (it.spawn && it.spawn.indexOf('boss') >= 0) { PRE.d = it.d; break; } }
    }
    if (PRE.d < 0 || S.boss || PRE.i >= 3 || S.dist < PRE.d - 2600) return;
    var bk = T.level === 2 ? 'boss2' : T.level === 3 ? 'boss3' : 'boss';
    c.save(); c.setTransform(1, 0, 0, 1, 0, 0); c.globalAlpha = 1; c.globalCompositeOperation = 'source-over';
    touch(PRE.i === 0 ? IMG[bk] : PRE.i === 1 ? whiteOf(bk) : dmgOf(bk));
    c.restore();
    PRE.i++;
  }
  // budget: milliseconds to spend now. Returns true once everything for this level is ready.
  function warm(budget) {
    if (!c || !artReady()) return false;
    useTrack();
    if (WTF !== TF) { WTF = TF; WQ = warmQueue(); WI = 0; }
    if (WI >= WQ.length && !PINQ.length) return true;
    var t0 = performance.now();
    c.save(); c.setTransform(1, 0, 0, 1, 0, 0); c.globalAlpha = 1; c.globalCompositeOperation = 'source-over';
    try {
      // a freshly pinned sheet goes up to the GPU now, not on its first appearance
      while (PINQ.length) { touch(IMG[PINQ.pop()]); if (performance.now() - t0 > budget) break; }
      while (WI < WQ.length && performance.now() - t0 <= budget) warmOne(WQ[WI++]);
    }
    finally { c.restore(); }
    return WI >= WQ.length && !PINQ.length;
  }

  // for tests: how many roadside props each stretch places, and how many it held back off a road
  function audit(d0, d1) {
    useTrack();
    var kept = 0, asked = 0;
    for (var k = Math.floor(d0 * ZK / PSLOT); k <= Math.floor(d1 * ZK / PSLOT); k++) {
      kept += slotList(k).length;
      SLOT.length = 0; propsOf(k, SLOT); asked += SLOT.length;
    }
    return { asked: asked, kept: kept, held: asked - kept };
  }

  window.FT_RENDER = { init: init, resize: resize, frame: frame, consume: consume, clearFX: clearFX, layout: layout, audit: audit, warm: warm, artReady: artReady, carAt: function () { return { x: V.px, y: V.py }; }, sus: function () { return PSUS; }, parts: function () { return parts; } };
})();
