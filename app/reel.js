/* FULL THROTTLE — the highlight reel (1.3). Pure: no DOM, no canvas, no audio.
   While a level is shot, FT_REEL watches the take: it keeps the last few seconds of the scene as
   snapshots (30 a second) and, when something worth seeing happens (a big kill, a chopper coming down,
   a henchman slammed into the wall, big air, a loop, a near miss, the boss going down), it keeps a clip
   of it: a moment and a half before, a couple of seconds after. A wrecked scene's clips are thrown out
   when it is reshot, so only printed takes make the cut.
   After the level is wrapped, play() cuts the best clips together in the order they happened, each
   from its own camera (low, alongside, a crane, a roadside telephoto, a kill cam, an aerial), slows
   down on the money shot and ends on the boss going down, twice, from two angles.
   R = FT_REEL.make(); R.reset(); R.record(S, events, rdt) each frame of play; R.cut() -> true if there
   is a reel; R.step(rdt) -> { view, evs, shot, done } while it plays. */
(function (root) {
  'use strict';
  var HZ = 30, KEEP = 4.2, PRE = 1.7, POST = 1.9, MAXLEN = 7, MAXCLIPS = 12, H = 600;
  // what is worth a clip, and what the clip is called
  var STUNT = { 'DOUBLE KILL': 5, 'TRIPLE KILL': 8, 'MASSACRE': 10, 'TURBO AIR': 7, 'BIG AIR': 5, 'TURBO JUMP': 5, 'RAMP JUMP': 3,
    'LOOP THE LOOP': 4, 'TURBO LOOP': 6, 'CORKSCREW': 5, 'WALL RIDE': 2, 'WALL SMASH': 8, 'SIDESWIPE': 6, 'GRAVITY CHECK': 8,
    'GRAVITY CHECK x2': 10, 'ROOF LANDING': 9, 'AIRBORNE KILL': 6, 'UPSIDE-DOWN KILL': 6, 'TURBO RAM': 4, 'THREAD THE NEEDLE': 5,
    'ULTRA DRIFT': 4, 'POWER DRIFT': 2, 'BIKER SLAM': 6 };
  var KILL = { heli: [9, 'CHOPPER DOWN'], truck: [7, 'ARMOR CRACKED'], suv: [5, 'GUNSHIP DOWN'], rammer: [3, 'RAMMER WRECKED'], moto: [2, 'BIKER DOWN'], hog: [3, 'SNAKE EYES DOWN'] };
  // the kind of shot each highlight wants, best first
  var CAMS = {
    air: ['side', 'flyby', 'crane'], loop: ['chase', 'low', 'side'], smash: ['side', 'low', 'dolly'], kill: ['target', 'low', 'dolly'],
    chopper: ['aerial', 'side', 'crane'], near: ['low', 'side', 'dolly'], drift: ['dolly', 'low', 'side'], boss: ['crane', 'low'], other: ['dolly', 'chase', 'low']
  };
  var CAMNAME = { chase: 'CAM A · CHASE', low: 'CAM B · BUMPER', side: 'CAM C · TRACKING', crane: 'CAM D · CRANE', aerial: 'CAM E · AERIAL',
    flyby: 'CAM F · ROADSIDE', target: 'CAM G · KILL CAM', dolly: 'CAM H · DOLLY' };

  function make() {
    var R = { buf: [], clips: [], cur: null, clock: 0, acc: 0, pend: [], level: 0, list: null, i: 0, ft: 0, view: null, objs: {}, fired: -1 };
    R.reset = function (level) {
      R.buf.length = 0; R.clips.length = 0; R.cur = null; R.clock = 0; R.acc = 0; R.pend.length = 0; R.level = level || 0; R.list = null;
    };

    // ---------- recording ----------
    function snap(S) {
      var p = S.p, i, e, o, ents = [], b = [], eb = [], m = [], hz = [];
      for (i = 0; i < S.ents.length; i++) {
        e = S.ents[i];
        if (e.dead || e.gone) continue;
        ents.push({ id: e.id, kind: e.kind, x: e.x, y: e.y, w: e.w, h: e.h, hp: e.hp, max: e.max, st: e.st, col: e.col, lean: e.lean || 0,
          flash: e.flash, age: e.age, alt: e.alt || 0, tx: e.tx, ty2: e.ty2, heli: e.heli, t: e.t, T: e.T, used: e.used, burst: e.burst || 0,
          charge: e.charge || 0, chargeK: e.chargeK, cx: e.cx || 0, vx: e.vx || 0, side: e.side || 0, brake: e.brake ? e.brake.t : -1 });
      }
      for (i = 0; i < S.bullets.length; i++) b.push(S.bullets[i].x, S.bullets[i].y);
      for (i = 0; i < S.ebullets.length; i++) eb.push(S.ebullets[i].x, S.ebullets[i].y);
      for (i = 0; i < S.missiles.length; i++) m.push(S.missiles[i].x, S.missiles[i].y);
      for (i = 0; i < (S.hz || []).length; i++) { o = {}; for (var k in S.hz[i]) o[k] = S.hz[i][k]; hz.push(o); }
      var L = [];
      for (i = 0; i < p.latch.length; i++) L.push({ side: p.latch[i].side, t: p.latch[i].t, hitT: p.latch[i].hitT });
      return {
        t: R.clock, dist: S.dist, speed: S.speed, ts: S.timeScale, phase: S.phase, outcome: S.outcome, inv: S.inverted, tr: S.trauma,
        boss: S.boss && !S.boss.dead ? S.boss.id : 0, scene: S.scene,
        p: { x: p.x, y: p.y, vx: p.vx, vy: p.vy, w: p.w, h: p.h, yaw: p.yaw, roll: p.roll, liftH: p.liftH, hp: p.hp, maxHp: p.maxHp,
          turboOn: p.turboOn, boostT: p.boostT, boostLvl: p.boostLvl, air: p.air, squash: p.squash, kick: p.kick, steerIn: p.steerIn,
          drift: p.drift ? { dir: p.drift.dir, lvl: p.drift.lvl } : null, latch: L, parts: p.parts },
        ents: ents, b: b, eb: eb, m: m, hz: hz, ev: null
      };
    }
    // the effects worth re-running in the reel (no score popups: the reel has its own titles)
    var VIS = { explode: 1, hit: 1, spark: 1, rail: 1, bump: 1, playerHit: 1, gun: 1, missile: 1, land: 1, zap: 1, strike: 1, ringOut: 1,
      shock: 1, dodge: 1, bossPhase: 1, driftStart: 1, driftLvl: 1, boost: 1, turbo: 1, pickup: 1, latch: 1, henchOff: 1, henchHit: 1,
      henchDrop: 1, heliDown: 1, partOff: 1, jump: 1, eshot: 1, wreck: 1, bossDown: 1, stunt: 1,
      chainWarn: 1, chain: 1, hogOff: 1, hogJump: 1, hogLand: 1, brakeWarn: 1 };
    function interest(e, S) {
      if (e.t === 'bossDown') return [100, 'THE BOSS GOES DOWN', 'boss', 3.6];
      if (e.t === 'stunt' && STUNT[e.name] !== undefined) {
        var n = e.name, kind = /AIR|JUMP|LANDING/.test(n) ? 'air' : /LOOP|CORKSCREW|PIPE|GRAVITY/.test(n) ? 'loop' : /SMASH|SIDESWIPE|RAM|SLAM/.test(n) ? 'smash' :
          /KILL|MASSACRE/.test(n) ? 'kill' : /NEEDLE/.test(n) ? 'near' : /DRIFT/.test(n) ? 'drift' : 'other';
        return [STUNT[n], n, kind];
      }
      if (e.t === 'stunt' && /^PIPE RUN/.test(e.name)) return [5, e.name, 'loop'];
      if (e.t === 'kill' && KILL[e.kind]) return [KILL[e.kind][0], KILL[e.kind][1], e.kind === 'heli' ? 'chopper' : 'kill'];
      if (e.t === 'near') return [2, 'NEAR MISS', 'near'];
      if (e.t === 'latch') return [3, 'HENCHMAN ON BOARD', 'smash'];
      if (e.t === 'dodge') return [2, 'DODGED', 'near'];
      return null;
    }
    function startClip(S) {
      var c = { frames: [], end: R.clock + POST, start: R.clock, score: 0, best: -1, label: '', kind: 'other', scene: S.scene, sceneName: S.beat,
        focusT: R.clock, fx: 0, fd: 0, focusId: 0 };
      var t0 = R.clock - PRE;
      for (var i = 0; i < R.buf.length; i++) if (R.buf[i].t >= t0) c.frames.push(R.buf[i]);
      return c;
    }
    function finish() {
      var c = R.cur; R.cur = null;
      if (!c || c.frames.length < 12) return;
      R.clips.push(c);
      if (R.clips.length > MAXCLIPS) {
        // drop the weakest clip that isn't the boss going down
        var wi = -1;
        for (var i = 0; i < R.clips.length; i++) if (R.clips[i].kind !== 'boss' && (wi < 0 || R.clips[i].score < R.clips[wi].score)) wi = i;
        if (wi >= 0) R.clips.splice(wi, 1);
      }
    }
    R.record = function (S, evs, rdt) {
      R.clock += rdt;
      var i, e;
      for (i = 0; i < evs.length; i++) {
        e = evs[i];
        if (e.t === 'cutAgain' || e.t === 'outOfFilm') {
          // that scene is going to be reshot: its clips never made it
          var n = S.scene;
          R.clips = R.clips.filter(function (c) { return c.scene < n; });
          R.cur = null; R.buf.length = 0; R.pend.length = 0;
          continue;
        }
        if (VIS[e.t]) { var cp = {}; for (var k in e) cp[k] = e[k]; R.pend.push(cp); }
        if (S.phase !== 'play' && e.t !== 'bossDown' && !(R.cur && R.cur.kind === 'boss')) continue;
        var it = interest(e, S);
        if (!it) continue;
        if (!R.cur) R.cur = startClip(S);
        var c = R.cur;
        c.score += it[0];
        c.end = Math.max(c.end, Math.min(c.start + MAXLEN, R.clock + (it[3] || POST)));
        if (it[2] === 'boss') c.end = R.clock + it[3];
        if (it[0] > c.best) {
          c.best = it[0]; c.label = it[2] === 'boss' ? (root.FT_ENGINE.KIND.boss.name + ' GOES DOWN') : it[1]; c.kind = it[2]; c.focusT = R.clock;
          var fx = e.x !== undefined ? e.x : S.p.x, fy = e.y !== undefined ? e.y : S.p.y;
          if (it[2] === 'boss' && S.boss) { fx = S.boss.x; fy = S.boss.y; }
          c.fx = fx; c.fd = S.dist + H - fy;
        }
      }
      R.acc += rdt;
      if (R.acc < 1 / HZ) return;
      R.acc = Math.min(R.acc - 1 / HZ, 1 / HZ);
      var f = snap(S);
      f.ev = R.pend; R.pend = [];
      R.buf.push(f);
      while (R.buf.length && R.buf[0].t < R.clock - KEEP) R.buf.shift();
      if (R.cur) {
        R.cur.frames.push(f);
        if (R.clock >= R.cur.end || (S.phase === 'done')) finish();
      }
    };
    R.flush = function () { if (R.cur) finish(); };

    // ---------- cutting it together ----------
    R.cut = function () {
      R.flush();
      var boss = null, rest = [];
      R.clips.forEach(function (c) { if (c.kind === 'boss') boss = c; else rest.push(c); });
      rest.sort(function (a, b) { return b.score - a.score; });
      rest = rest.slice(0, 5).sort(function (a, b) { return a.start - b.start; });
      var list = [], last = '', used = { crane: 1 }; // the boss gets the crane
      // the camera this kind of moment wants, but never the same one twice running, and a fresh one if it can
      function pick(kind) {
        var c = (CAMS[kind] || CAMS.other).concat(['dolly', 'side', 'low', 'crane', 'chase']), best = null, i;
        for (i = 0; i < c.length && !best; i++) if (c[i] !== last && !used[c[i]]) best = c[i];
        for (i = 0; i < c.length && !best; i++) if (c[i] !== last) best = c[i];
        last = best; used[best] = (used[best] || 0) + 1;
        return best;
      }
      rest.forEach(function (c) { list.push({ clip: c, cam: pick(c.kind), slow: c.best >= 5, trim: 2.0 }); });
      if (boss) {
        list.push({ clip: boss, cam: 'crane', slow: true, trim: 2.2 });
        list.push({ clip: boss, cam: 'target', slow: true, again: true });
      }
      R.list = list; R.i = -1; R.ft = 0; R.done = !list.length; R.t = 0; R.side = 1;
      if (list.length) next();
      return list.length > 0;
    };
    function next() {
      R.i++;
      R.objs = {}; R.fired = -1;
      if (R.i >= R.list.length) { R.done = true; return; }
      var it = R.list[R.i], c = it.clip;
      // start a little later on long clips, and the second boss angle starts close to the kill
      R.ft = it.again ? Math.max(0, c.focusT - c.frames[0].t - 0.9) : Math.max(0, c.focusT - c.frames[0].t - (it.trim > 2.1 ? 2.6 : 1.6));
      R.shotT = 0;
      R.side = (R.i % 2) ? -1 : 1;
      var f0 = c.frames[Math.min(c.frames.length - 1, Math.floor(R.ft * HZ))];
      if (it.cam === 'side' || it.cam === 'dolly') R.side = f0.p.x > 0 ? -1 : 1;
      if (c.kind === 'smash') for (var i = 0; i < c.frames.length; i++) {
        // film a wall smash from the wall's side
        var fe = c.frames[i].ev || [];
        for (var j = 0; j < fe.length; j++) if (fe[j].t === 'henchOff' && fe[j].side) { R.side = fe[j].side; i = c.frames.length; break; }
      }
      R.fixD = null;
    }
    function lerp(a, b, u) { return a + (b - a) * u; }
    function clamp(v, a, b) { return v < a ? a : v > b ? b : v; }
    // a view object the renderer can draw, from two snapshots either side of the moment
    function viewOf(fa, fb, u) {
      var V = R.view || (R.view = { p: {}, ents: [], bullets: [], ebullets: [], missiles: [], hz: [], boss: null, events: [] });
      var pa = fa.p, pb = fb.p, P = V.p, k;
      for (k in pa) P[k] = pa[k];
      P.x = lerp(pa.x, pb.x, u); P.y = lerp(pa.y, pb.y, u); P.yaw = lerp(pa.yaw, pb.yaw, u); P.roll = lerp(pa.roll, pb.roll, u);
      P.liftH = lerp(pa.liftH, pb.liftH, u); P.vx = lerp(pa.vx, pb.vx, u); P.inv = 0;
      V.dist = lerp(fa.dist, fb.dist, u); V.speed = lerp(fa.speed, fb.speed, u); V.timeScale = fa.ts; V.phase = fa.phase; V.outcome = fa.outcome;
      V.inverted = fa.inv; V.trauma = fa.tr * 0.6; V.scene = fa.scene; V.t = fa.t;
      var byB = {};
      for (var i = 0; i < fb.ents.length; i++) byB[fb.ents[i].id] = fb.ents[i];
      V.ents.length = 0; V.boss = null;
      var seen = {};
      for (i = 0; i < fa.ents.length; i++) {
        var a = fa.ents[i], b = byB[a.id] || a, o = R.objs[a.id] || (R.objs[a.id] = {});
        for (k in a) o[k] = a[k];
        o.x = lerp(a.x, b.x, u); o.y = lerp(a.y, b.y, u); o.alt = lerp(a.alt, b.alt, u); o.lean = lerp(a.lean, b.lean, u);
        if (a.tx !== undefined && b.tx !== undefined) { o.tx = lerp(a.tx, b.tx, u); o.ty2 = lerp(a.ty2, b.ty2, u); }
        V.ents.push(o); seen[a.id] = 1;
        if (a.id === fa.boss) V.boss = o;
      }
      function pts(src, dst) {
        dst.length = src.length / 2;
        for (var j = 0; j < src.length; j += 2) { var q = dst[j / 2] || (dst[j / 2] = {}); q.x = src[j]; q.y = src[j + 1]; q.vy = -1; }
        dst.length = src.length / 2;
      }
      pts(fa.b, V.bullets); pts(fa.eb, V.ebullets); pts(fa.m, V.missiles);
      V.hz = fa.hz;
      return V;
    }
    // the camera for this shot: road terms (cd/td = road distance, cx/tx across, ch/th height), a zoom
    function camera(it, V, u, f) {
      var p = V.p, dp = V.dist + H - p.y, s = R.side, kind = it.cam, c = it.clip;
      if (kind === 'chase') return null;
      if (kind === 'low') return { cd: dp - 40, cx: p.x + s * 16, ch: 6, td: dp + 70, tx: p.x, th: 7, zoom: 0.85, roll: -s * 0.03 };
      if (kind === 'side') return { cd: dp - 88 + 20 * u, cx: p.x + s * 100, ch: 16, td: dp + 26, tx: p.x - s * 6, th: 9, zoom: 1.25, roll: 0 };
      if (kind === 'crane' && c.kind === 'boss') return { cd: dp - 190 + 60 * u, cx: p.x * 0.4 - s * 40, ch: 190 - 50 * u, td: lerp(dp + 80, c.fd, 0.5), tx: lerp(p.x, c.fx, 0.5), th: 25, zoom: 0.95, roll: 0, wup: true };
      if (kind === 'crane') return { cd: dp - 170 + 70 * u, cx: p.x * 0.5 - s * 30, ch: 160 - 60 * u, td: dp + 80, tx: p.x, th: 0, zoom: 0.95, roll: 0, wup: true };
      if (kind === 'aerial') {
        var hx = c.fx, hd = c.fd;
        return { cd: dp - 240 + 40 * u, cx: lerp(p.x, hx, 0.4) - s * 60, ch: 210, td: lerp(dp + 60, hd, 0.4), tx: lerp(p.x, hx, 0.5), th: 20, zoom: 1.05, roll: s * 0.02, wup: true };
      }
      if (kind === 'dolly') return { cd: dp - 105, cx: p.x + lerp(-95, 95, u) * s, ch: 24, td: dp + 30, tx: p.x, th: 8, zoom: 1.1, roll: 0 };
      if (kind === 'target') {
        var w = clamp(1 - Math.abs(f.t - c.focusT) / 1.6, 0, 1) * 0.65;
        return { cd: dp - 80, cx: p.x - s * 26, ch: 30, td: lerp(dp + 60, c.fd, w), tx: lerp(p.x, c.fx, w), th: 12, zoom: 1.15 + 0.25 * w, roll: 0 };
      }
      if (kind === 'flyby') {
        // a camera bolted to the roadside: it holds still while the car blasts away, panning and zooming after it
        if (R.fixD === null) R.fixD = dp - 150;
        var far = Math.max(1, dp - R.fixD);
        return { cd: R.fixD, cx: s * 172, ch: 14, td: dp + 10, tx: p.x, th: 8, zoom: clamp(0.9 + far / 260, 0.9, 3.2), roll: s * 0.04 };
      }
      return null;
    }
    R.step = function (rdt) {
      if (R.done || !R.list) return { done: true };
      var it = R.list[R.i], c = it.clip, F = c.frames, n = F.length;
      // slow motion on the money shot: half a second either side of it
      var ft0 = F[0].t, near = Math.abs(ft0 + R.ft - c.focusT);
      var speed = it.slow && near < 0.3 ? 0.35 : it.slow && near < 0.6 ? 0.6 : 1;
      R.ft += rdt * speed; R.shotT += rdt; R.t += rdt;
      var fi = R.ft * HZ, i0 = Math.min(n - 1, Math.floor(fi)), i1 = Math.min(n - 1, i0 + 1), u = fi - Math.floor(fi);
      var evs = [];
      for (var j = R.fired + 1; j <= i0; j++) if (F[j].ev) for (var q = 0; q < F[j].ev.length; q++) evs.push(F[j].ev[q]);
      if (R.fired < i0) R.fired = i0;
      var V = viewOf(F[i0], F[i1], u);
      var shot = { cam: camera(it, V, clamp(R.ft * HZ / n, 0, 1), F[i0]), camName: CAMNAME[it.cam], label: c.label, kind: c.kind, scene: c.scene, sceneName: c.sceneName,
        index: R.i, count: R.list.length, again: !!it.again, shotT: R.shotT, left: (n - fi) / HZ, slow: speed < 1, cut: R.shotT < rdt * 1.5 };
      var endAt = it.trim ? Math.min(n, Math.floor((c.focusT - ft0 + it.trim) * HZ)) : n;
      if (fi >= endAt - 1) next();
      return { view: V, evs: evs, shot: shot, done: false };
    };
    R.has = function () { return R.clips.length > 0 || !!R.cur; };
    return R;
  }
  root.FT_REEL = { make: make };
})(typeof window !== 'undefined' ? window : this);
