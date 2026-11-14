/* FULL THROTTLE — the track. Pure maths: no DOM, no canvas, no audio.
   The road is a list of sections laid end to end (lengths in engine road units, the same "d" the
   engine counts). Each section bends (curve), leans (bank), climbs or dips (pitch), and a few are
   set pieces: a full LOOP, a gap JUMP where the road stops and starts again, and a corkscrew TWIST
   where the road rolls over once. The engine reads curve, bank and the set pieces for the rules; the
   renderer asks for the 3D frames (position plus forward/right/up) to film it.
   There are three levels, each its own layout: 1 Bayshore (the beach town), 2 the Volta Chemical
   Works, whose PIPE sections run inside a glass tube while the road rolls all the way round it, and
   (1.5) 3 the Nevada desert, the open highway to Las Vegas.
   FT_TRACK.use(n) picks the level; everything else reads the level in use.
   The layouts are our own. */
(function (root) {
  'use strict';
  var ZK = 1.5;          // world units per road unit along the track (the renderer's depth scale)
  var CM = 1e-3;         // curvature at curve 1.0 (per road unit, the scale the engine's pull uses)
  var TAU = Math.PI * 2;

  // [length, curve (+ right), bank (rad, + leans into a right turn), shape, a, b]
  // shapes: flat | hill a=amp n=humps | rise a | fall a | loop | jump a=kick b=landing | twist a=dir
  var OPENING = [
    [1200, 0, 0, 'flat'],                    // THE GETAWAY: a straight start
    [1600, -0.35, -0.12, 'hill', 0.05, 2],   // (1.0) Beach Drive: a long easy left along the sand
    [900, 0.7, 0.25, 'flat'],                // a banked sweep right
    [900, -0.8, -0.3, 'hill', 0.10, 1],      // THE TAIL: left over a crest
    [700, 0, 0, 'rise', 0.16],               // climb onto the high road
    [1300, 0.5, 0.2, 'hill', 0.12, 3],       // three humps, up high
    [800, -0.6, -0.25, 'fall', 0.14],        // and back down
    [1600, 1.6, 0.95, 'flat'],               // THE AMBUSH: a banked wall, hard right
    [2000, 0, 0, 'hill', 0.07, 1],           // THE JUMP: a long straight for the car carrier
    [900, 0, 0, 'rise', 0.22],               // up to the lip
    [1700, 0, 0, 'jump', 0.32, 0.40],        // the road stops: fly the gap
    [400, 0, 0, 'flat'],
    [2100, 0, 0, 'loop'],                    // the first loop
    [300, 0, 0, 'flat'],
    [1400, 0.9, 0.4, 'flat'],                // THE CONVOY: long banked esses for the truck fight
    [1000, -0.9, -0.4, 'rise', 0.2],
    [1500, 0.45, 0.18, 'hill', 0.06, 2],     // (1.0) the boardwalk run, up high over the beach
    [500, 0, 0, 'flat'],
    [1300, 0, 0, 'twist', 1],                // a corkscrew, up high
    [1200, 0.6, 0.25, 'fall', 0.167],
    [1000, -0.5, -0.2, 'rise', 0.27],        // CROSSFIRE: climb...
    [400, 0, 0, 'flat'],
    [2000, 0, 0, 'jump', 0.36, 0.45],        // ...for the big gap
    [600, 0, 0, 'flat'],
    [1700, -1.7, -1.0, 'flat'],              // a banked wall, hard left
    [1400, 0.7, 0.3, 'hill', 0.05, 1],       // (1.0) THE WOLF PACK: the esplanade sweep
    [500, 0, 0, 'flat'],
    [2100, 0, 0, 'loop'],                    // the second loop
    [500, 1.1, 0.5, 'flat'],
    [1000, -1.0, -0.45, 'flat'],             // to THE PIER at 34500
    // (1.1) the long cut: seven more scenes along the coast, THE FINALE moves to 122500
    [1500, 0.4, 0.15, 'flat'],               // THE PIER
    [1800, -0.6, -0.25, 'hill', 0.08, 2],
    [900, 0, 0, 'rise', 0.18],
    [1800, 0, 0, 'jump', 0.30, 0.38],        // off the end of the pier
    [600, 0, 0, 'flat'],
    [1600, 1.5, 0.9, 'flat'],                // a banked wall, hard right
    [1400, -0.5, -0.2, 'flat'],
    [1400, 0, 0, 'flat'],
    [2000, 0, 0, 'hill', 0.06, 3],           // NEON MILE (45500): three humps under the signs
    [1200, 0.8, 0.35, 'flat'],
    [1200, -0.8, -0.35, 'flat'],
    [500, 0, 0, 'flat'],
    [2100, 0, 0, 'loop'],
    [400, 0, 0, 'flat'],
    [1500, 0.5, 0.2, 'rise', 0.15],
    [1300, 0, 0, 'twist', -1],
    [800, -0.4, -0.15, 'fall', 0.15],
    [1800, -0.4, -0.15, 'hill', 0.05, 2],    // THE MARINA (56500)
    [1700, -1.6, -0.95, 'flat'],             // a banked wall, hard left
    [800, 0, 0, 'flat'],
    [900, 0, 0, 'rise', 0.22],
    [1700, 0, 0, 'jump', 0.32, 0.40],        // over the boat slips
    [500, 0, 0, 'flat'],
    [1600, 0.7, 0.3, 'hill', 0.08, 2],
    [2000, 0, 0, 'flat'],
    [1400, 0.9, 0.4, 'rise', 0.2],           // THE CLIFF ROAD (67500): esses up the headland
    [1400, -0.9, -0.4, 'hill', 0.1, 2],
    [1400, 1.0, 0.45, 'flat'],
    [1400, -1.0, -0.45, 'fall', 0.17],
    [600, 0, 0, 'flat'],
    [1300, 0, 0, 'twist', 1],
    [500, 0, 0, 'flat'],
    [2100, 0, 0, 'loop'],
    [900, 0.3, 0.1, 'flat'],
    [1600, 0, 0, 'hill', 0.07, 2],           // THE LIGHTHOUSE (78500)
    [1700, 1.7, 1.0, 'flat'],                // a banked wall round the point
    [700, 0, 0, 'flat'],
    [1000, 0, 0, 'rise', 0.27],
    [400, 0, 0, 'flat'],
    [2000, 0, 0, 'jump', 0.36, 0.45],        // the biggest gap on the coast
    [600, 0, 0, 'flat'],
    [1500, -0.6, -0.25, 'hill', 0.06, 1],
    [1500, 0.4, 0.15, 'flat'],
    [1500, -0.8, -0.35, 'flat'],             // THE LAST MILE (89500)
    [1500, 0.8, 0.35, 'hill', 0.06, 2],
    [500, 0, 0, 'flat'],
    [2100, 0, 0, 'loop'],
    [300, 0, 0, 'flat'],
    [1300, 0, 0, 'twist', -1],
    [800, 0, 0, 'fall', 0.15],
    [1000, 1.1, 0.5, 'flat'],
    [1000, -1.0, -0.45, 'flat'],
    [1000, 0, 0, 'flat'],
    [1600, 0, 0, 'hill', 0.05, 3],           // THE CAUSEWAY (100500): out over the bay
    [1500, 0.9, 0.4, 'flat'],
    [1500, -0.9, -0.4, 'flat'],
    [800, 0, 0, 'rise', 0.2],
    [1700, 0, 0, 'jump', 0.32, 0.40],        // the drawbridge is up
    [500, 0, 0, 'flat'],
    [2100, 0, 0, 'loop'],
    [1300, 0.4, 0.15, 'flat'],
    [1700, -1.6, -0.95, 'flat'],             // SUNSET STRIP (111500): a banked wall, hard left
    [800, 0, 0, 'flat'],
    [1300, 0, 0, 'twist', 1],
    [1400, 0.8, 0.35, 'hill', 0.08, 2],
    [1400, -0.8, -0.35, 'flat'],
    [500, 0, 0, 'flat'],
    [2100, 0, 0, 'loop'],
    [800, 0, 0, 'flat'],
    [1000, 0, 0, 'flat']                     // to THE FINALE at 122500
  ];
  // the Juggernaut fight runs laps of this, as long as it lasts
  var CIRCUIT = [
    [1500, 0.8, 0.35, 'hill', 0.1, 2],
    [1200, -1.2, -0.7, 'flat'],
    [600, 0, 0, 'flat'],
    [2100, 0, 0, 'loop'],
    [800, 0, 0, 'rise', 0.2],
    [1400, 1.4, 0.8, 'flat'],
    [600, 0, 0, 'flat'],
    [1300, 0, 0, 'twist', -1],
    [1000, -0.6, -0.25, 'fall', 0.16]
  ];

  // ---------- level 2: the Volta Chemical Works ----------
  // Twisty S-bends, helix ramps, and PIPES: [len, 0, 0, 'pipe', a = roll direction, b = turns]. In a
  // pipe the road runs inside a glass tube and rolls round it, so you drive the walls and the ceiling.
  var CHEM = [
    [1000, 0, 0, 'flat'],                    // THE FACTORY GATE
    [1100, 0.9, 0.4, 'flat'],
    [1100, -0.9, -0.4, 'flat'],
    [1100, 1.1, 0.5, 'hill', 0.06, 1],
    [1100, -1.1, -0.5, 'flat'],
    [600, 0, 0, 'flat'],
    [1600, 0, 0, 'pipe', 1, 1],              // the first tube: once round
    [400, 0, 0, 'flat'],
    [1000, 0.6, 0.25, 'flat'],
    [1200, -0.8, -0.35, 'rise', 0.18],       // THE TUBES (9000)
    [1200, 0.9, 0.4, 'flat'],
    [500, 0, 0, 'flat'],
    [2400, 0, 0, 'pipe', -1, 2],             // twice round, the other way
    [500, 0, 0, 'flat'],
    [1200, -1.0, -0.45, 'fall', 0.15],
    [1200, 1.0, 0.45, 'flat'],
    [1800, 0, 0, 'hill', 0.07, 2],
    [2600, 1.7, 1.0, 'rise', 0.22],          // SPIN CYCLE (19000): a banked helix up
    [600, 0, 0, 'flat'],
    [2100, 0, 0, 'loop'],
    [400, 0, 0, 'flat'],
    [1300, 0, 0, 'twist', 1],
    [2600, -1.7, -1.0, 'fall', 0.2],         // and the helix back down
    [400, 0, 0, 'flat'],
    [1000, 0.8, 0.35, 'flat'],               // THE REACTOR (29000)
    [1000, -0.8, -0.35, 'flat'],
    [900, 0, 0, 'rise', 0.22],
    [1700, 0, 0, 'jump', 0.32, 0.40],        // over the coolant pond
    [500, 0, 0, 'flat'],
    [1800, 0, 0, 'pipe', 1, 1],
    [500, 0, 0, 'flat'],
    [1300, 1.2, 0.6, 'hill', 0.06, 1],
    [1300, -1.2, -0.6, 'flat'],
    [800, 0, 0, 'flat'],                     // PIPE DREAMS (39000)
    [2400, 0, 0, 'pipe', -1, 2],
    [400, 0, 0, 'flat'],
    [2100, 0, 0, 'loop'],
    [300, 0, 0, 'flat'],
    [1800, 0, 0, 'pipe', 1, 1],
    [1200, 0.9, 0.4, 'fall', 0.15],
    [1000, -0.6, -0.25, 'flat'],
    [1100, 1.0, 0.45, 'flat'],               // MEGA VAT (49000): esses round the vats
    [1100, -1.0, -0.45, 'flat'],
    [1100, 1.0, 0.45, 'hill', 0.08, 1],
    [1100, -1.0, -0.45, 'flat'],
    [1000, 0, 0, 'rise', 0.27],
    [400, 0, 0, 'flat'],
    [2000, 0, 0, 'jump', 0.36, 0.45],        // over the big vat
    [600, 0, 0, 'flat'],
    [1600, 1.6, 0.95, 'flat'],               // a banked wall
    [1000, -0.5, -0.2, 'flat'],              // THE TOXIC RUN (59000)
    [2800, 0, 0, 'pipe', 1, 3],              // three times round
    [400, 0, 0, 'flat'],
    [1300, 0, 0, 'twist', -1],
    [500, 0, 0, 'flat'],
    [2100, 0, 0, 'loop'],
    [900, 0.8, 0.35, 'flat'],
    [1000, -0.8, -0.35, 'flat'],             // to THE FINALE at 69000
    // (1.2) the Alchemist's arena: his fight runs on long, easy bends with no tube or loop to hide
    // his lightning in, so every warning on the road can be read
    [4000, 0.35, 0.15, 'flat'],
    [4000, -0.35, -0.15, 'flat'],
    [4000, 0.3, 0.12, 'hill', 0.035, 1],
    [4000, -0.3, -0.12, 'flat'],
    [4000, 0.35, 0.15, 'flat'],
    [4000, -0.35, -0.15, 'flat'],
    [4000, 0.25, 0.1, 'hill', 0.03, 1],
    [4000, -0.25, -0.1, 'flat'],
    [4000, 0, 0, 'flat']
  ];
  var CHEM_CIRCUIT = [
    [1200, 1.0, 0.45, 'flat'],
    [1200, -1.0, -0.45, 'hill', 0.06, 1],
    [600, 0, 0, 'flat'],
    [1800, 0, 0, 'pipe', 1, 1],
    [500, 0, 0, 'flat'],
    [2100, 0, 0, 'loop'],
    [400, 0, 0, 'flat'],
    [1400, 1.5, 0.9, 'flat'],
    [1000, -0.6, -0.25, 'flat']
  ];
  // ---------- level 3 (1.5): the Nevada desert, the open highway to Las Vegas ----------
  // Long straights and rolling desert dips under a dusk sky, with the city lit up dead ahead. Every
  // bend is paid back by one the other way, so the road keeps heading for Vegas the whole level. The
  // movie crew still built a few set pieces out here: gaps over dry washes, two loops, a corkscrew
  // through the canyon and banked walls round the red rock.
  var DESERT = [
    [1500, 0, 0, 'flat'],                    // THE STATE LINE
    [2400, 0, 0, 'hill', 0.05, 3],           // rolling desert dips
    [1600, 0.35, 0.12, 'flat'],
    [1600, -0.35, -0.12, 'flat'],
    [1400, 0, 0, 'hill', 0.06, 1],
    [1200, 0, 0, 'flat'],                    // THE DRY WASH (8500)
    [900, 0, 0, 'rise', 0.2],
    [1700, 0, 0, 'jump', 0.32, 0.40],        // the bridge is out: fly the wash
    [600, 0, 0, 'flat'],
    [2000, 0, 0, 'hill', 0.05, 2],
    [1400, 0.6, 0.25, 'flat'],
    [1400, -0.6, -0.25, 'flat'],
    [1800, 0, 0, 'flat'],                    // SNAKE EYES (17700): the gang rides out
    [2400, 0, 0, 'hill', 0.04, 2],
    [500, 0, 0, 'flat'],
    [2100, 0, 0, 'loop'],                    // the stunt crew's loop in the middle of nowhere
    [400, 0, 0, 'flat'],
    [1500, 0.45, 0.18, 'hill', 0.05, 1],
    [1500, -0.45, -0.18, 'flat'],
    [2600, 0, 0, 'hill', 0.06, 3],           // JOSHUA FLATS (27900)
    [1000, 0.8, 0.35, 'flat'],
    [1000, -0.8, -0.35, 'flat'],
    [900, 0, 0, 'rise', 0.22],
    [1700, 0, 0, 'jump', 0.32, 0.40],
    [600, 0, 0, 'flat'],
    [1800, 0, 0, 'hill', 0.05, 2],
    [1700, 1.6, 0.95, 'flat'],               // RED ROCK PASS (37500): a banked wall round the rock
    [600, 0, 0, 'flat'],
    [1300, 0, 0, 'twist', 1],                // a corkscrew through the canyon
    [600, 0, 0, 'flat'],
    [1700, -1.6, -0.95, 'flat'],             // and the wall back the other way
    [1000, 0, 0, 'fall', 0.12],
    [1600, 0, 0, 'hill', 0.05, 2],
    [2000, 0, 0, 'flat'],                    // LAST CHANCE GAS (46000)
    [1500, 0.5, 0.2, 'flat'],
    [1500, -0.5, -0.2, 'flat'],
    [1000, 0, 0, 'rise', 0.27],
    [400, 0, 0, 'flat'],
    [2000, 0, 0, 'jump', 0.36, 0.45],        // the biggest wash in the county
    [600, 0, 0, 'flat'],
    [1500, 0, 0, 'hill', 0.06, 2],
    [1200, 0.9, 0.4, 'flat'],                // JACKPOT JUNCTION (56500)
    [1200, -0.9, -0.4, 'flat'],
    [500, 0, 0, 'flat'],
    [2100, 0, 0, 'loop'],
    [300, 0, 0, 'flat'],
    [1300, 0, 0, 'twist', -1],
    [800, 0, 0, 'fall', 0.15],
    [2100, 0, 0, 'hill', 0.05, 3],
    [1500, 0.4, 0.15, 'flat'],               // THE CITY LIGHTS (66000)
    [1500, -0.4, -0.15, 'flat'],
    [800, 0, 0, 'rise', 0.2],
    [1700, 0, 0, 'jump', 0.32, 0.40],
    [500, 0, 0, 'flat'],
    [2100, 0, 0, 'loop'],
    [900, 0, 0, 'flat'],
    [1500, 0, 0, 'hill', 0.04, 2],
    [1500, 0, 0, 'flat'],                    // to THE FINALE at 78000
    // the carrier fight: the open highway, dead straight or nearly, so nothing hides the bikers
    [5000, 0, 0, 'flat'],
    [4000, 0.2, 0.08, 'hill', 0.03, 1],
    [4000, -0.2, -0.08, 'flat'],
    [5000, 0, 0, 'hill', 0.03, 2],
    [4000, 0.25, 0.1, 'flat'],
    [4000, -0.25, -0.1, 'flat'],
    [6000, 0, 0, 'flat'],
    [4000, 0.2, 0.08, 'flat'],
    [4000, -0.2, -0.08, 'flat']
  ];
  var DESERT_CIRCUIT = [
    [3000, 0, 0, 'hill', 0.04, 2],
    [2000, 0.3, 0.12, 'flat'],
    [2000, -0.3, -0.12, 'flat'],
    [3000, 0, 0, 'flat']
  ];
  var LAYOUTS = { 1: [OPENING, CIRCUIT], 2: [CHEM, CHEM_CIRCUIT], 3: [DESERT, DESERT_CIRCUIT] };

  var END = 180000;       // road units built; far beyond any take
  var BLEND = 450;        // curve and bank ease from one section into the next over this
  var PIPE_C = 80;        // a pipe's axis rides this far above the road (its radius is hypot(EDGE, PIPE_C))

  var SEC, FEATS, LOOPS_BEFORE, FR = null, LEVEL = 0, i;
  var STORE = {};
  function layout(n) {
    var sec = [], d = 0, i, op = LAYOUTS[n][0], ci = LAYOUTS[n][1];
    function add(r) { sec.push({ d0: d, len: r[0], d1: d + r[0], curve: r[1], bank: r[2], shape: r[3], a: r[4] || 0, b: r[5] || 0 }); d += r[0]; }
    for (i = 0; i < op.length; i++) add(op[i]);
    while (d < END) for (i = 0; i < ci.length; i++) add(ci[i]);
    return sec;
  }

  function secAt(x) {
    if (x < 0) return 0;
    var lo = 0, hi = SEC.length - 1;
    while (lo < hi) { var m = (lo + hi + 1) >> 1; if (SEC[m].d0 <= x) lo = m; else hi = m - 1; }
    return lo;
  }
  function smooth(t) { t = t < 0 ? 0 : t > 1 ? 1 : t; return t * t * (3 - 2 * t); }
  function smoother(t) { t = t < 0 ? 0 : t > 1 ? 1 : t; return t * t * t * (t * (t * 6 - 15) + 10); }
  function eased(x, key) {
    var k = secAt(x), s = SEC[k], prev = k > 0 ? SEC[k - 1][key] : 0;
    var T = Math.min(BLEND, s.len * 0.45), f = smooth((x - s.d0) / T);
    return prev + (s[key] - prev) * f;
  }
  // the engine's view: how hard the road bends here (same scale as before) and how much it leans
  function curveAt(x) { return x < 0 ? 0 : eased(x, 'curve') * CM; }
  function bankAt(x) { return x < 0 ? 0 : eased(x, 'bank'); }

  var JA = 0.22, JB = 0.6; // a jump section: kicker to JA, the gap JA..JB, landing ramp after
  function pitchAt(x) {
    if (x < 0) return 0;
    var s = SEC[secAt(x)], u = (x - s.d0) / s.len;
    switch (s.shape) {
      case 'hill': return s.a * Math.sin(TAU * s.b * u);
      case 'rise': return s.a * (1 - Math.cos(TAU * u)) / 2;
      case 'fall': return -s.a * (1 - Math.cos(TAU * u)) / 2;
      case 'loop': return TAU * smoother(u);
      case 'jump':
        if (u < JA) return s.a * (1 - Math.cos(Math.PI * u / JA)) / 2;
        if (u < JB) return s.a + (-s.b - s.a) * (u - JA) / (JB - JA);
        return -s.b * (1 + Math.cos(Math.PI * (u - JB) / (1 - JB))) / 2;
    }
    return 0;
  }
  function twistAt(x) {
    if (x < 0) return 0;
    var s = SEC[secAt(x)];
    if (s.shape === 'twist') return s.a * TAU * smoother((x - s.d0) / s.len);
    if (s.shape === 'pipe') return s.a * TAU * (s.b || 1) * smoother((x - s.d0) / s.len);
    return 0;
  }
  function inPipe(x) { return x >= 0 && SEC[secAt(x)].shape === 'pipe'; }
  // A loop comes down beside where it went up, not on top of itself. The sideways slide happens
  // only up in the loop (LS0..LS1 of the way round), so both bottoms run straight and a full road
  // apart, and it is kept afterwards (the offset counts every loop already passed), so the road
  // out of the loop carries on from exactly where the loop lands: no gap, step or overlap.
  var LOOP_SIDE = 360, LS0 = 0.2, LS1 = 0.8;
  function loopSide(x) {
    if (x < 0) return 0;
    var k = secAt(x), s = SEC[k], side = LOOPS_BEFORE[k] * LOOP_SIDE;
    return s.shape === 'loop' ? side + LOOP_SIDE * smoother(((x - s.d0) / s.len - LS0) / (LS1 - LS0)) : side;
  }

  // the set pieces, in order, for the engine and the director
  function feats(SEC) {
    var F = [];
    for (var i = 0; i < SEC.length; i++) {
      var s = SEC[i];
      if (s.shape === 'loop' || s.shape === 'twist') F.push({ kind: s.shape, d0: s.d0, d1: s.d1 });
      else if (s.shape === 'pipe') F.push({ kind: 'pipe', d0: s.d0, d1: s.d1, turns: s.b || 1 });
      else if (s.shape === 'jump') F.push({ kind: 'jump', d0: s.d0, d1: s.d1, g0: s.d0 + s.len * JA, g1: s.d0 + s.len * JB });
      else if (Math.abs(s.bank) >= 0.8) F.push({ kind: 'wall', d0: s.d0 + BLEND * 0.6, d1: s.d1, dir: s.curve > 0 ? 1 : -1 });
    }
    return F;
  }
  // is road distance x over a gap (no road under you)?
  function gapAt(x) {
    var s = SEC[secAt(x)];
    if (s.shape !== 'jump') return null;
    var u = (x - s.d0) / s.len;
    return u >= JA && u < JB ? { g0: s.d0 + s.len * JA, g1: s.d0 + s.len * JB } : null;
  }
  // what to paint on the road here: 'kick' (the ramp up to a gap), 'warn' (stripes before a set piece)
  function zoneAt(x) {
    var k = secAt(x), s = SEC[k], u = (x - s.d0) / s.len;
    if (s.shape === 'jump') return u < JA ? 'kick' : u < JB ? 'gap' : u < JB + 0.08 ? 'warn' : null;
    var n = SEC[k + 1];
    if (s.shape === 'pipe') return 'pipe';
    if (n && (n.shape === 'loop' || n.shape === 'jump' || n.shape === 'twist' || n.shape === 'pipe') && n.d0 - x < 260) return 'warn';
    if (s.shape === 'loop' || s.shape === 'twist') return 'set';
    return null;
  }

  // ---------- the 3D frames, built once on first use (the renderer's job, not the engine's) ----------
  // World: x right, y up, z forward at the start. Heading h turns right, pitch p climbs.
  var STEP = 12, PRE = 3000; // world units per sample; a straight run-up before d = 0
  function build() {
    if (FR) return FR;
    var total = END * ZK + PRE, n = Math.ceil(total / STEP) + 2;
    var A = function () { return new Float64Array(n); };
    FR = { level: LEVEL, n: n, step: STEP, pre: PRE, px: A(), py: A(), pz: A(), fx: A(), fy: A(), fz: A(), rx: A(), ry: A(), rz: A(), ux: A(), uy: A(), uz: A(), corner: A(), twist: A(), minY: 0 };
    var h = 0, x = 0, y = 0, z = -PRE, side = 0, minY = 1e9;
    for (var j = 0; j < n; j++) {
      var sw = j * STEP - PRE, dd = sw / ZK;
      var p = pitchAt(dd), cp = Math.cos(p), sp = Math.sin(p), ch = Math.cos(h), sh = Math.sin(h);
      var fx = cp * sh, fy = sp, fz = cp * ch, rx = ch, ry = 0, rz = -sh, ux = -sp * sh, uy = cp, uz = -sp * ch;
      var corner = bankAt(dd), tw = twistAt(dd), phi = corner + tw, cb = Math.cos(phi), sb = Math.sin(phi);
      FR.px[j] = x; FR.py[j] = y; FR.pz[j] = z;
      if (tw && inPipe(dd)) {
        // in a pipe the road turns round the tube's axis, not its own centre line: the road's centre
        // sits PIPE_C below the axis along the rolled up, so it climbs the wall and runs the ceiling
        var c0 = Math.cos(corner), s0 = Math.sin(corner);
        var u0x = ux * c0 + rx * s0, u0y = uy * c0 + ry * s0, u0z = uz * c0 + rz * s0;
        var u1x = ux * cb + rx * sb, u1y = uy * cb + ry * sb, u1z = uz * cb + rz * sb;
        FR.px[j] += PIPE_C * (u0x - u1x); FR.py[j] += PIPE_C * (u0y - u1y); FR.pz[j] += PIPE_C * (u0z - u1z);
      }
      FR.fx[j] = fx; FR.fy[j] = fy; FR.fz[j] = fz;
      // banked: right dips and up leans toward it (+phi lowers the right side)
      FR.rx[j] = rx * cb - ux * sb; FR.ry[j] = ry * cb - uy * sb; FR.rz[j] = rz * cb - uz * sb;
      FR.ux[j] = ux * cb + rx * sb; FR.uy[j] = uy * cb + ry * sb; FR.uz[j] = uz * cb + rz * sb;
      FR.corner[j] = corner; FR.twist[j] = tw;
      if (FR.py[j] < minY) minY = FR.py[j];
      // step along
      var ns = loopSide((sw + STEP) / ZK), dside = ns - side; side = ns;
      x += fx * STEP + rx * dside; y += fy * STEP; z += fz * STEP + rz * dside;
      h += curveAt(dd) / (ZK * ZK) * STEP; // heading change per world unit = curve / ZK^2
    }
    FR.minY = minY;
    return FR;
  }

  // pick a level: its sections, set pieces and (built on first use) its 3D frames
  function use(n) {
    n = LAYOUTS[n] ? n : 1;
    if (n === LEVEL) return API;
    if (LEVEL) STORE[LEVEL] = { SEC: SEC, FEATS: FEATS, LOOPS_BEFORE: LOOPS_BEFORE, FR: FR };
    LEVEL = n;
    var st = STORE[n];
    if (st) { SEC = st.SEC; FEATS = st.FEATS; LOOPS_BEFORE = st.LOOPS_BEFORE; FR = st.FR; }
    else {
      SEC = layout(n); FEATS = feats(SEC); FR = null;
      LOOPS_BEFORE = []; var nl = 0;
      for (var i = 0; i < SEC.length; i++) { LOOPS_BEFORE.push(nl); if (SEC[i].shape === 'loop') nl++; }
    }
    API.level = n; API.SECTIONS = SEC; API.FEATS = FEATS;
    return API;
  }

  var API = root.FT_TRACK = {
    ZK: ZK, END: END, PIPE_C: PIPE_C, level: 0, SECTIONS: null, FEATS: null, use: use,
    curveAt: curveAt, bankAt: bankAt, pitchAt: pitchAt, twistAt: twistAt, gapAt: gapAt, zoneAt: zoneAt, inPipe: inPipe,
    secAt: function (x) { return SEC[secAt(x)]; }, build: build
  };
  use(1);
})(typeof window !== 'undefined' ? window : this);
