/* FULL THROTTLE — the page: screens, input, the loop, sound cues and saving. */
(function () {
  'use strict';
  var E = window.FT_ENGINE, R = window.FT_RENDER, A = window.FT_AUDIO, AP = window.FT_AUTOPILOT, DIR = window.FT_DIRECTOR.make();
  var REEL = window.FT_REEL.make(); // 1.3: the take is filmed for the highlight reel played after a wrap
  var STEP = 1 / 120, REDUCED = !!(window.matchMedia && matchMedia('(prefers-reduced-motion: reduce)').matches);
  var cv = document.getElementById('game');
  var probe = document.getElementById('safe');
  var S = null, attract = null, pilot = AP.make(), take = 0, seed = 0;
  var ui = { screen: 'title', t: 0, playT: 0, take: 1, touch: false, held: {}, buttons: {}, stick: null, stickHome: { x: 0, y: 0 },
    safe: { t: 0, r: 0, b: 0, l: 0 }, best: null, bests: [null, null, null], level: 0, levelBox: null, newBest: false, muted: false, mutedHint: 0, dir: DIR,
    card: { n: 0, name: E.SCENES[0].name, take: 1, retake: false } };
  var keys = {}, missileEdge = false, mouseL = false, mouseR = false, last = 0, acc = 0, clock = 0, hidden = false, lastInp = {}, lastLine = null;

  // ---------- 1.7: the controls, rebindable and saved ----------
  // 1.7.1 (the owner): split the hands. There are two keyboard LAYOUTS, one key per action in each:
  //   slot 0, WASD:   the left hand drives (WASD steer, Shift drift with the pinky, Space turbo with the
  //                   thumb) and the right hand shoots (J guns, K missiles).
  //   slot 1, ARROWS: the right hand drives on the arrows, so the left hand is free for Z guns, X missiles
  //                   (Shift drift and Space turbo as in WASD).
  // Only the layout in use reads its keys (Z and X do nothing in the WASD layout). The layout follows the
  // keys you steer with in a race. The left and right Shift / Ctrl / Alt keys count as one key.
  // The mouse (left = guns, right = missile, in both layouts), the gamepad and the touch buttons are fixed.
  var ACTIONS = [
    { id: 'left', name: 'STEER LEFT', pad: 'L-STICK / D-PAD' }, { id: 'right', name: 'STEER RIGHT', pad: 'L-STICK / D-PAD' },
    { id: 'up', name: 'SPEED UP', pad: 'STICK / D-PAD UP' }, { id: 'down', name: 'BRAKE', pad: 'STICK / D-PAD DOWN' },
    { id: 'drift', name: 'DRIFT (HOLD)', pad: 'A / RB' }, { id: 'turbo', name: 'TURBO', pad: 'LB / RT' },
    { id: 'fire', name: 'MACHINE GUNS', pad: 'X', mouse: 'L-CLICK' }, { id: 'missile', name: 'MISSILE', pad: 'B', mouse: 'R-CLICK' }
  ];
  var LAYOUTS = ['WASD', 'ARROWS'];
  var DEFAULT_BINDS = { left: ['KeyA', 'ArrowLeft'], right: ['KeyD', 'ArrowRight'], up: ['KeyW', 'ArrowUp'], down: ['KeyS', 'ArrowDown'],
    drift: ['ShiftLeft', 'ShiftLeft'], turbo: ['Space', 'Space'], fire: ['KeyJ', 'KeyZ'], missile: ['KeyK', 'KeyX'] };
  var STEERS = ['left', 'right', 'up', 'down'];
  // these keep their own jobs everywhere (pause, mute, menus), so they can't be bound
  var RESERVED = { Escape: 1, KeyP: 1, KeyM: 1, Enter: 1, NumpadEnter: 1, Tab: 1, Backspace: 1, Delete: 1, MetaLeft: 1, ContextMenu: 1 };
  var binds = copyBinds(DEFAULT_BINDS);
  ui.layout = 0;
  function copyBinds(b) { var o = {}; for (var k in DEFAULT_BINDS) o[k] = [(b[k] && b[k][0]) || null, (b[k] && b[k][1]) || null]; return o; }
  function norm(code) { return code === 'ShiftRight' ? 'ShiftLeft' : code === 'ControlRight' ? 'ControlLeft' : code === 'AltRight' ? 'AltLeft' : code === 'MetaRight' ? 'MetaLeft' : code; }
  function down(a) { var code = binds[a][ui.layout]; return !!(code && keys[code]); }
  // the action a key does in one layout, or in either (for keeping the page from scrolling on it)
  function boundIn(code, s) { for (var k in binds) if (binds[k][s] === code) return k; return null; }
  function isBound(code) { return boundIn(code, 0) || boundIn(code, 1); }
  // a race follows the keys you steer with: a steering key that only the other layout uses switches to it
  function followLayout(code) {
    var o = 1 - ui.layout, a = boundIn(code, o);
    if (a && STEERS.indexOf(a) >= 0 && !boundIn(code, ui.layout)) { ui.layout = o; saveBinds(); }
  }
  var KEYNAME = { ArrowLeft: '←', ArrowRight: '→', ArrowUp: '↑', ArrowDown: '↓', Space: 'SPACE', ShiftLeft: 'SHIFT', ControlLeft: 'CTRL', AltLeft: 'ALT',
    Backquote: '`', Minus: '-', Equal: '=', BracketLeft: '[', BracketRight: ']', Backslash: '\\', Semicolon: ';', Quote: "'", Comma: ',', Period: '.', Slash: '/',
    CapsLock: 'CAPS', Home: 'HOME', End: 'END', PageUp: 'PG UP', PageDown: 'PG DN', Insert: 'INS' };
  function keyName(code) {
    if (!code) return '—';
    if (KEYNAME[code]) return KEYNAME[code];
    if (/^Key[A-Z]$/.test(code)) return code.slice(3);
    if (/^Digit\d$/.test(code)) return code.slice(5);
    if (/^Numpad/.test(code)) return 'NUM ' + code.slice(6).toUpperCase();
    return code.toUpperCase();
  }
  function steerDefault(s) { for (var i = 0; i < STEERS.length; i++) if (binds[STEERS[i]][s] !== DEFAULT_BINDS[STEERS[i]][s]) return false; return true; }
  // the words the screens use for a control, in whatever the player is using right now
  // (s picks a layout; left out, it's the one in use)
  ui.keyText = function (a, s) {
    if (ui.pad && s === undefined) { if (a === 'steer') return 'L-STICK / D-PAD'; for (var i = 0; i < ACTIONS.length; i++) if (ACTIONS[i].id === a) return ACTIONS[i].pad; return ''; }
    if (s === undefined) s = ui.layout;
    if (a === 'steer') return steerDefault(s) ? (s ? 'ARROW KEYS' : 'WASD') : keyName(binds.left[s]) + ' ' + keyName(binds.right[s]);
    var b = binds[a]; if (!b) return '';
    return keyName(b[s]);
  };
  ui.keyOne = ui.keyText;
  ui.layoutName = function (s) { return LAYOUTS[s === undefined ? ui.layout : s]; };
  ui.pad = false; ui.padName = null;
  ui.ctl = { from: 'title', sel: 0, slot: 0, wait: false, msg: '', msgT: 0, rows: ACTIONS, binds: binds, keyName: keyName, layouts: LAYOUTS };

  // ---------- storage: wispos.storage when inside WispOS, localStorage in a plain browser ----------
  var store = {
    get: function (k) {
      if (window.wispos && wispos.storage) return wispos.storage.get(k).catch(function () { return null; });
      try { return Promise.resolve(JSON.parse(localStorage.getItem('ft.' + k))); } catch (e) { return Promise.resolve(null); }
    },
    set: function (k, v) {
      if (window.wispos && wispos.storage) return wispos.storage.set(k, v).catch(function () {});
      try { localStorage.setItem('ft.' + k, JSON.stringify(v)); } catch (e) { /* private mode */ }
      return Promise.resolve();
    }
  };
  // the best take is kept per level ('best' is level 1, as it always was; 'best2' and 'best3' the others)
  function bestKey(i) { return i ? 'best' + (i + 1) : 'best'; }
  [0, 1, 2].forEach(function (i) {
    store.get(bestKey(i)).then(function (b) { if (b && typeof b.score === 'number') { ui.bests[i] = b; ui.best = ui.bests[ui.level]; } });
  });
  // the title theme starts once the saved mute setting is known, so a muted game never blurts it out
  store.get('muted').then(function (m) { if (m) { ui.muted = true; A.setMuted(true); } if (ui.screen === 'title') A.title(true); });
  // 1.7: the player's own key bindings, and (1.7.1) the layout last raced with.
  // A 1.7.0 save (v 1) had its two slots the other way round and Space as drift, so it's dropped for the
  // new defaults rather than read back as the wrong keys.
  store.get('controls').then(function (c) {
    if (!c || c.v !== 2 || !c.binds) return;
    var b = copyBinds(c.binds);
    for (var s = 0; s < 2; s++) {
      var seen = {};
      for (var k in b) { var code = b[k][s]; if (code && (RESERVED[code] || seen[code])) b[k][s] = null; else if (code) seen[code] = 1; }
    }
    for (k in b) binds[k] = b[k];
    if (c.layout === 0 || c.layout === 1) ui.layout = c.layout;
  });
  function saveBinds() { store.set('controls', { v: 2, binds: copyBinds(binds), layout: ui.layout }); }
  // 1.8: skill points and the GARAGE's upgrades (skills.js). A career: banked points and ranks are kept
  // across runs, saved as { points, earned, ranks: { id: rank } }.
  var SK = window.FT_SKILL;
  ui.sk = SK.fresh(); ui.earn = null; ui.gar = null;
  store.get('skills').then(function (v) { if (v) ui.sk = SK.clean(v); });
  function saveSkills() { store.set('skills', { v: 1, points: ui.sk.points, earned: ui.sk.earned, ranks: ui.sk.ranks }); }
  // tests: ?autoreel (or FT_DEBUG.ui.reelAuto) lets the reel continue itself after one full play
  ui.reelAuto = /[?&]autoreel/.test(location.search);

  // ---------- layout ----------
  function readSafe() {
    var cs = getComputedStyle(probe);
    ui.safe = { t: parseFloat(cs.paddingTop) || 0, r: parseFloat(cs.paddingRight) || 0, b: parseFloat(cs.paddingBottom) || 0, l: parseFloat(cs.paddingLeft) || 0 };
  }
  function placeButtons() {
    var L = R.layout(), sc = L.scale, vw = L.viewW, vh = L.viewH;
    var sr = ui.safe.r / sc, sb = ui.safe.b / sc, sl = ui.safe.l / sc;
    var big = Math.max(46, 34 / sc), small = Math.max(36, 27 / sc), st = ui.safe.t / sc;
    // 1.7: DRIFT is a big button now (it's held through every corner), right under the thumb next to FIRE;
    // MSL and TURBO sit above them, and a small pause button sits by the film strip
    var fx = vw - sr - big - 22, fy = vh - sb - big - 26, dx = fx - big * 2 - 22;
    ui.buttons = {
      fire: { x: fx, y: fy, r: big, label: 'FIRE', col: '#ffd23f' },
      drift: { x: dx, y: fy + 8, r: big, label: 'DRIFT', col: '#ff4fb8' },
      turbo: { x: fx, y: fy - big - small - 18, r: small, label: 'TURBO', col: '#36f1ff' },
      missile: { x: dx + 10, y: fy - big - small - 10, r: small, label: 'MSL', col: '#ff9e3d' },
      pause: { x: vw / 2 + 186, y: st + 22, r: 17, label: 'II', col: '#e8f3f7', tap: true }
    };
    ui.stickHome = { x: sl + 130, y: vh - sb - 60 };
  }
  function resize() { readSafe(); R.resize(); placeButtons(); }
  R.init(cv);
  resize();
  window.addEventListener('resize', resize);

  // ---------- the level: 1 Bayshore, 2 the chemical plant, 3 the Nevada desert ----------
  function setLevel(i) {
    i = (i + E.LEVELS.length) % E.LEVELS.length;
    ui.level = i; ui.best = ui.bests[i];
    E.useLevel(i);
    if (ui.screen === 'title') { R.clearFX(); newAttract(); }
  }

  // ---------- screens ----------
  function newAttract() {
    attract = E.create(4242);
    attract.reduced = REDUCED;
    for (var i = 0; i < 240; i++) stepAttract(STEP);
  }
  function stepAttract(dt) {
    attract.p.hp = 100; attract.p.inv = 0;
    ipSnap(attract);
    E.step(attract, pilot(attract, dt), dt);
    if (attract.phase !== 'play' || attract.dist > 26000) { newAttract(); R.clearFX(); }
  }
  function go(screen) {
    ui.screen = screen; ui.t = 0;
    // 1.6: the title plays "Bit-Beast Dash", the track the owner picked (it used to be the synth bassline); a race fades it out
    if (screen === 'title') { S = null; A.song(false); A.music(false); A.title(true); A.engine(false, 0, false); R.clearFX(); newAttract(); }
    else A.title(false);
    if (screen === 'card') {
      // a fresh run opens on scene 1; a wreck's retake opens on the scene being shot again
      var again = !!(S && S.phase === 'retake');
      if (!again) { E.useLevel(ui.level); take++; ui.take = take; ui.card = { n: 0, name: E.SCENES[0].name, take: 1, retake: false }; }
      else ui.card = { n: S.cp.scene, name: E.SCENES[S.cp.scene].name, take: S.sceneTake + 1, retake: true };
      A.music(false); A.song(false); A.engine(false, 0, false);
      A.play('riser'); setTimeout(function () { if (ui.screen === 'card') A.play('clap'); }, 1000);
    }
    if (screen === 'play') {
      // the level plays his song: a retake picks it up where it stopped, a fresh run starts it from the top
      var fresh = !(S && S.phase === 'retake');
      if (!fresh) E.retake(S);
      else {
        seed = (Date.now() ^ (take * 2654435761)) >>> 0;
        S = E.create(seed); S.take = take; S.reduced = REDUCED;
        E.setMods(S, SK.stats(ui.sk)); // 1.8: the GARAGE's upgrades
        DIR.reset(); REEL.reset(ui.level);
      }
      S.events.length = 0;
      // a key still down from the menu (Space is turbo now, A on a pad is drift) doesn't count until it's let go
      ui.dMask = true; ui.tMask = true;
      S._id = undefined; R.warm(1e9); // anything the warm-up hasn't made yet is made now, before the first frame of the race
      R.clearFX(); ui.playT = 0; ui.newBest = false; A.music(false); A.song(true, fresh); lastLine = null; acc = 0;
    }
    if (screen === 'reel') {
      // the highlight reel: the song plays on under it, the engine noise stops
      A.engine(false, 0, false); A.skid(false); A.rotor(false); R.clearFX(); ui.reel = null;
      A.play('sting');
    }
    if (screen === 'rating') {
      ui.reel = null; A.rotor(false);
      // 1.8: the level pays its skill points now, banked at once (a reshoot or the title keeps them)
      ui.earn = SK.earn(S.outcome, S.ratingIdx || 0, S.retakes || 0);
      if (ui.earn.total > 0 && !S.paid) { S.paid = true; ui.sk.points += ui.earn.total; ui.sk.earned += ui.earn.total; saveSkills(); }
      A.engine(false, 0, false); A.skid(false); A.song(false); A.music(true, 0.4);
      setTimeout(function () { if (ui.screen === 'rating') A.play('stamp'); }, 2700);
      var b = ui.best;
      if (S.outcome === 'wrapped' && (!b || S.score > b.score)) {
        ui.best = ui.bests[ui.level] = { score: S.score, title: S.rating.title }; ui.newBest = true;
        store.set(bestKey(ui.level), ui.best);
      }
    }
  }
  // 1.8: the highlight reel ends only here: ENTER, the pad's START, or a click on its CONTINUE button
  function continueReel() {
    if (ui.screen !== 'reel' || ui.t <= 0.8) return;
    A.play('pickup'); ui.reelHover = false; cv.style.cursor = '';
    go('rating');
  }
  function advance() {
    A.init();
    if (ui.screen === 'reel') return; // only continueReel() leaves the reel
    if (ui.screen === 'title') { if (!R.artReady()) return; A.play('sting'); go('card'); } // 1.7.2: not before every sheet is decoded
    else if (ui.screen === 'card' && ui.t > 1.2) go('play');
    else if (ui.screen === 'rating' && ui.t > 1.5) {
      // 1.8: a level that paid skill points goes to the GARAGE first; the next level starts from there
      if (ui.earn && ui.earn.total > 0) { openGarage(); return; }
      // a wrapped level moves on to the next one; anything else shoots the same level again
      if (S && S.outcome === 'wrapped' && ui.level < E.LEVELS.length - 1) setLevel(ui.level + 1);
      go('card');
    }
    else if (ui.screen === 'paused') resume();
  }
  function pause() { mouseL = false; if (ui.screen === 'play') { ui.screen = 'paused'; A.engine(false, 0, false); A.skid(false); A.rotor(false); A.music(false); A.song(false); } }
  function resume() { if (ui.screen === 'paused') { ui.screen = 'play'; ui.dMask = true; ui.tMask = true; A.song(true); last = performance.now(); } }

  // ---------- 1.8: THE GARAGE: spend skill points between levels ----------
  // Cards 0..n-1 are the upgrades (from skills.js), n is the button that goes on. Nothing times out:
  // the next level starts only from the button (or the pad's START).
  function openGarage() {
    var ups = SK.data.upgrades, n = ups.length, last = ui.level >= E.LEVELS.length - 1, wrapped = S && S.outcome === 'wrapped';
    var nextI = wrapped ? (last ? -1 : ui.level + 1) : ui.level, lv = nextI >= 0 ? E.LEVELS[nextI] : null;
    var sel = n;
    for (var i = 0; i < n; i++) if (SK.canBuy(ui.sk, ups[i])) { sel = i; break; }
    ui.gar = { sel: sel, cols: 4, boxes: [], msg: '', msgT: 0, msgCol: '#ffd23f', flash: null,
      earned: ui.earn ? ui.earn.total : 0, earnLine: ui.earn ? ui.earn.lines.map(function (l) { return l.label + ' +' + l.n; }).join(' \u00b7 ') : '',
      next: !wrapped ? 'again' : last ? 'title' : 'next',
      confirm: !wrapped ? 'RESHOOT LEVEL ' + (ui.level + 1) : last ? 'BACK TO TITLE' : 'START LEVEL ' + (ui.level + 2),
      subtitle: 'SPEND SKILL POINTS ON THE KESTREL-88  \u00b7  ' + (lv ? 'NEXT: LEVEL ' + lv.n + '  ' + lv.zone : 'ALL THREE LEVELS IN THE CAN') };
    keys = {}; A.play('sting');
    go('garage');
  }
  function garMsg(m, col) { ui.gar.msg = m; ui.gar.msgT = 2.6; ui.gar.msgCol = col || '#ffd23f'; }
  function garBuy(i) {
    var u = SK.data.upgrades[i], G = ui.gar; if (!u) return;
    G.sel = i;
    if (SK.buy(ui.sk, u)) {
      saveSkills(); A.play('pickup'); G.flash = { i: i, t: 0.35, ok: true };
      garMsg(u.name + ' RANK ' + SK.rank(ui.sk, u.id) + '  \u00b7  ' + (u.effects || []).map(function (e) { return SK.effectText(e, SK.rank(ui.sk, u.id)); }).join(', ').toUpperCase(), u.color);
    } else {
      A.play('empty'); G.flash = { i: i, t: 0.35, ok: false };
      var lk = SK.locked(ui.sk, u), rq = lk && SK.byId(lk.id);
      garMsg(lk ? 'LOCKED: ' + u.name + ' NEEDS ' + (rq ? rq.name : lk.id) + ' RANK ' + lk.rank : SK.rank(ui.sk, u.id) >= u.maxRank ? u.name + ' IS MAXED' :
        'NOT ENOUGH POINTS: ' + u.name + ' COSTS ' + SK.nextCost(ui.sk, u), '#ff6a5e');
    }
  }
  function garRefund(i) {
    var u = SK.data.upgrades[i], G = ui.gar; if (!u) return;
    G.sel = i;
    if (SK.refund(ui.sk, u)) { saveSkills(); A.play('tick'); garMsg('REFUNDED: ' + u.name + ' BACK TO RANK ' + SK.rank(ui.sk, u.id), '#36f1ff'); }
    else { A.play('empty'); garMsg(u.name + ': NOTHING TO REFUND', '#ff6a5e'); }
  }
  function garConfirm() {
    var G = ui.gar; if (!G || ui.screen !== 'garage') return;
    A.play('sting'); cv.style.cursor = ''; keys = {};
    if (G.next === 'title') { go('title'); return; }
    if (G.next === 'next') setLevel(ui.level + 1);
    go('card');
  }
  function garMove(dx, dy) {
    var G = ui.gar, n = SK.data.upgrades.length, cols = Math.max(1, G.cols || 4), was = G.sel;
    if (dx) G.sel = (G.sel + dx + n + 1) % (n + 1);
    if (dy > 0) G.sel = G.sel >= n ? n : (G.sel + cols < n ? G.sel + cols : n);
    if (dy < 0) G.sel = G.sel >= n ? Math.floor((n - 1) / cols) * cols : Math.max(0, G.sel - cols);
    if (G.sel !== was) A.play('tick');
  }
  function garActivate() { if (ui.gar.sel >= SK.data.upgrades.length) garConfirm(); else garBuy(ui.gar.sel); }
  function garKey(code) {
    if (code === 'ArrowLeft' || code === 'KeyA' || binds.left[0] === code || binds.left[1] === code) garMove(-1, 0);
    else if (code === 'ArrowRight' || code === 'KeyD' || binds.right[0] === code || binds.right[1] === code) garMove(1, 0);
    else if (code === 'ArrowUp' || code === 'KeyW') garMove(0, -1);
    else if (code === 'ArrowDown' || code === 'KeyS' || code === 'Tab') garMove(0, 1);
    else if (code === 'Enter' || code === 'NumpadEnter' || code === 'Space') garActivate();
    else if ((code === 'Backspace' || code === 'Delete') && ui.gar.sel < SK.data.upgrades.length) garRefund(ui.gar.sel);
    // Esc and every other key: nothing (a stray key never leaves the garage)
  }

  // ---------- 1.7: the controls screen (from the title or the pause screen) ----------
  // rows 0-7 are the actions (two key slots each), then RESET TO DEFAULTS and BACK
  var CTL_RESET = ACTIONS.length, CTL_BACK = ACTIONS.length + 1;
  function openControls(from) {
    A.play('pickup');
    ui.ctl.from = from; ui.ctl.sel = 0; ui.ctl.slot = 0; ui.ctl.wait = false; ui.ctl.msg = ''; ui.ctl.msgT = 0; ui.t = 0;
    ui.screen = 'controls';
  }
  function closeControls() {
    ui.ctl.wait = false; keys = {}; // a key still down from the screen doesn't leak into play
    ui.screen = ui.ctl.from === 'paused' ? 'paused' : 'title'; ui.t = 3;
    A.play('pickup');
  }
  function ctlMsg(m) { ui.ctl.msg = m; ui.ctl.msgT = 2.4; }
  function resetBinds() { var d = copyBinds(DEFAULT_BINDS); for (var k in d) binds[k] = d[k]; saveBinds(); ctlMsg('BACK TO THE DEFAULT KEYS'); }
  function ctlActivate() {
    var C = ui.ctl;
    if (C.sel === CTL_RESET) { resetBinds(); A.play('pickup'); return; }
    if (C.sel === CTL_BACK) { closeControls(); return; }
    if (ui.pad && !ui.ctlKeyboard) { ctlMsg('USE THE KEYBOARD TO SET A KEY  ·  THE PAD BUTTONS ARE FIXED'); return; }
    C.wait = true; C.msg = '';
  }
  function ctlAssign(code) {
    var C = ui.ctl, a = ACTIONS[C.sel].id;
    C.wait = false;
    if (code === 'Escape') return;
    if (code === 'Backspace' || code === 'Delete') { binds[a][C.slot] = null; saveBinds(); ctlMsg(ACTIONS[C.sel].name + ': KEY CLEARED'); return; }
    if (RESERVED[code]) { ctlMsg(keyName(code) + ' IS TAKEN (P PAUSE, M MUTE, ESC, ENTER)'); A.play('empty'); return; }
    // a key can only do one thing in a layout: take it off wherever it was in this one
    var was = boundIn(code, C.slot), moved = '';
    if (was && was !== a) { binds[was][C.slot] = null; moved = was; }
    binds[a][C.slot] = code;
    saveBinds();
    var nm = ''; for (var i = 0; i < ACTIONS.length; i++) if (ACTIONS[i].id === moved) nm = ACTIONS[i].name;
    ctlMsg(LAYOUTS[C.slot] + ': ' + ACTIONS[C.sel].name + ' = ' + keyName(code) + (nm ? '   (TAKEN OFF ' + nm + ')' : ''));
    A.play('pickup');
  }
  function ctlMove(dy, dx) {
    var C = ui.ctl, n = CTL_BACK + 1;
    if (dy) { C.sel = (C.sel + dy + n) % n; A.play('tick'); }
    if (dx && C.sel < CTL_RESET) { C.slot = C.slot ? 0 : 1; A.play('tick'); }
    if (dx && C.sel >= CTL_RESET) { C.sel = C.sel === CTL_RESET ? CTL_BACK : CTL_RESET; A.play('tick'); }
  }
  function ctlKey(code) {
    var C = ui.ctl;
    ui.ctlKeyboard = true;
    if (C.wait) { ctlAssign(code); return; }
    if (code === 'ArrowUp') ctlMove(-1, 0);
    else if (code === 'ArrowDown' || code === 'Tab') ctlMove(1, 0);
    else if (code === 'ArrowLeft' || code === 'ArrowRight') ctlMove(0, 1);
    else if (code === 'Enter' || code === 'NumpadEnter' || code === 'Space') ctlActivate();
    else if (code === 'Escape' || code === 'KeyC') closeControls();
    else if ((code === 'Backspace' || code === 'Delete') && C.sel < CTL_RESET) { binds[ACTIONS[C.sel].id][C.slot] = null; saveBinds(); ctlMsg(ACTIONS[C.sel].name + ': KEY CLEARED'); }
  }

  // ---------- keyboard ----------
  var MENU_KEYS = { ArrowLeft: 1, ArrowRight: 1, ArrowUp: 1, ArrowDown: 1, Space: 1, Enter: 1, Tab: 1, Backspace: 1, Delete: 1 };
  window.addEventListener('keydown', function (e) {
    var code = norm(e.code);
    // keep the page from scrolling, the browser from searching or tabbing away, on any key the game uses
    if (MENU_KEYS[code] || isBound(code) || ui.screen === 'controls') e.preventDefault();
    if (e.repeat) { keys[code] = true; return; }
    keys[code] = true;
    ui.touch = false; ui.pad = false;
    A.init();
    if (ui.screen === 'controls') { ctlKey(code); return; }
    if (code === 'KeyM') { ui.muted = !ui.muted; A.setMuted(ui.muted); ui.mutedHint = 1.6; store.set('muted', ui.muted); return; }
    if (ui.screen === 'play') {
      followLayout(code);
      if (binds.missile[ui.layout] === code) missileEdge = true;
      if (code === 'KeyP' || code === 'Escape') pause();
      return;
    }
    if (ui.screen === 'paused') {
      if (code === 'KeyP' || code === 'Enter' || code === 'Space') resume();
      else if (code === 'KeyC') openControls('paused');
      else if (code === 'Escape') go('title');
      return;
    }
    if (ui.screen === 'title' && code === 'KeyC') { openControls('title'); return; }
    // 1.8: only ENTER leaves the reel (Space is turbo, Esc is pause: a stray press must not skip it)
    if (ui.screen === 'reel') { if (code === 'Enter' || code === 'NumpadEnter') continueReel(); return; }
    if (ui.screen === 'garage') { garKey(code); return; }
    if (ui.screen === 'rating' && code === 'Escape') { go('title'); return; }
    if (ui.screen === 'rating' && code === 'KeyR' && ui.t > 1.5) { go('card'); return; }
    if (ui.screen === 'title') {
      // the level picker follows the steering keys, whatever they're bound to, as well as the arrows
      // (picking a level doesn't switch the layout: only steering in a race does)
      if (code === 'ArrowLeft' || code === 'ArrowUp' || binds.left[0] === code || binds.left[1] === code) { setLevel(ui.level - 1); A.play('pickup'); return; }
      if (code === 'ArrowRight' || code === 'ArrowDown' || binds.right[0] === code || binds.right[1] === code) { setLevel(ui.level + 1); A.play('pickup'); return; }
      if (code === 'Digit1' || code === 'Numpad1') { setLevel(0); return; }
      if (code === 'Digit2' || code === 'Numpad2') { setLevel(1); return; }
      if (code === 'Digit3' || code === 'Numpad3') { setLevel(2); return; }
    }
    if (code === 'Enter' || code === 'NumpadEnter' || code === 'Space') advance();
  });
  window.addEventListener('keyup', function (e) { keys[norm(e.code)] = false; });
  window.addEventListener('blur', function () { keys = {}; ui.held = {}; ui.stick = null; mouseL = mouseR = false; });

  // ---------- touch: drag on the left to drive, buttons on the right ----------
  var ptrs = {};
  function toView(e) { var sc = R.layout().scale; return { x: e.clientX / sc, y: e.clientY / sc }; }
  function hitButton(p) {
    var best = null, bd = 1e9;
    for (var k in ui.buttons) { var b = ui.buttons[k], d = Math.hypot(p.x - b.x, p.y - b.y); if (d < b.r + (b.tap ? 8 : 14) && d < bd) { best = k; bd = d; } }
    return best;
  }
  // boxes the screens drew (in their own overlay scale, k): the first one under the pointer
  function inBox(p, list) {
    if (!list) return null;
    for (var i = 0; i < list.length; i++) {
      var bx = list[i]; if (!bx) continue;
      var f = R.layout().scale / (bx.k || 1), qx = p.x * f, qy = p.y * f;
      if (qx > bx.x - 4 && qx < bx.x + bx.w + 4 && qy > bx.y - 4 && qy < bx.y + bx.h + 4) return bx;
    }
    return null;
  }
  // the mouse: left button held fires the guns, each right click fires a missile
  function mouseButtons(e) {
    if (e.pointerType !== 'mouse') return;
    var l = !!(e.buttons & 1), r = !!(e.buttons & 2), playing = ui.screen === 'play';
    mouseL = l && playing;
    if (r && !mouseR && playing) missileEdge = true;
    mouseR = r;
  }
  cv.addEventListener('pointerdown', function (e) {
    e.preventDefault();
    if (e.pointerType === 'touch' || e.pointerType === 'pen') { ui.touch = true; placeButtons(); }
    if (e.pointerType === 'mouse') {
      ui.touch = false; mouseButtons(e);
      try { cv.setPointerCapture(e.pointerId); } catch (x) { /* fine */ }
      // 1.8: in the garage a right click on a card refunds a rank
      if (ui.screen === 'garage' && e.button === 2) { var rb = inBox(toView(e), ui.gar && ui.gar.boxes); if (rb && rb.i < SK.data.upgrades.length) garRefund(rb.i); return; }
      if (ui.screen === 'play' || e.button !== 0) return; // only a left click moves the menus on
    }
    A.init(); ui.pad = false;
    var p = toView(e);
    if (ui.screen === 'controls') {
      // click a key slot to change it (then press the new key), or RESET / BACK
      var hb = inBox(p, ui.ctlBoxes);
      if (hb) {
        ui.ctl.sel = hb.row; ui.ctl.slot = hb.slot || 0;
        if (hb.row >= CTL_RESET || !ui.touch) { ui.ctlKeyboard = true; ctlActivate(); }
      }
      return;
    }
    if ((ui.screen === 'title' || ui.screen === 'paused') && inBox(p, [ui.ctlBtn])) { openControls(ui.screen); return; }
    if (ui.screen === 'title' && ui.levelBox) {
      var lb = ui.levelBox, f = R.layout().scale / (lb.k || 1), qx = p.x * f, qy = p.y * f;
      if (qx > lb.x - 10 && qx < lb.x + lb.w + 10 && qy > lb.y - 10 && qy < lb.y + lb.h + 10) { setLevel(ui.level + (qx < lb.x + lb.w / 2 ? -1 : 1)); return; }
    }
    // 1.8: the reel moves on only from its CONTINUE button: a click anywhere else does nothing
    if (ui.screen === 'reel') { if (inBox(p, [ui.reelBtn])) continueReel(); return; }
    if (ui.screen === 'garage') {
      var gb = inBox(p, ui.gar && ui.gar.boxes);
      if (gb) { if (gb.i >= SK.data.upgrades.length) garConfirm(); else garBuy(gb.i); }
      return;
    }
    if (ui.screen !== 'play') { advance(); return; }
    if (!ui.touch) return;
    try { cv.setPointerCapture(e.pointerId); } catch (x) { /* fine */ }
    var b = hitButton(p);
    if (b === 'pause') { pause(); return; }
    if (b) { ptrs[e.pointerId] = { btn: b }; ui.held[b] = true; if (b === 'missile') missileEdge = true; return; }
    if (p.x < R.layout().viewW * 0.55) { ptrs[e.pointerId] = { stick: true }; ui.stick = { ox: p.x, oy: p.y, dx: 0, dy: 0 }; }
  });
  // 1.8: the mouse points at what it would press (the reel's CONTINUE, a garage card or button)
  function hover(e) {
    var p = toView(e), on = false;
    if (ui.screen === 'reel') { on = ui.t > 0.8 && !!inBox(p, [ui.reelBtn]); ui.reelHover = on; }
    else if (ui.screen === 'garage' && ui.gar) {
      var gb = inBox(p, ui.gar.boxes);
      if (gb) { on = true; if (gb.i !== ui.gar.sel) { ui.gar.sel = gb.i; A.play('tick'); } }
    }
    cv.style.cursor = on ? 'pointer' : '';
  }
  cv.addEventListener('pointermove', function (e) {
    mouseButtons(e);
    if (e.pointerType === 'mouse') hover(e);
    var q = ptrs[e.pointerId]; if (!q) return;
    var p = toView(e);
    if (q.stick && ui.stick) {
      var dx = (p.x - ui.stick.ox) / 46, dy = (p.y - ui.stick.oy) / 46, m = Math.hypot(dx, dy);
      if (m > 1) { dx /= m; dy /= m; }
      ui.stick.dx = dx; ui.stick.dy = dy;
    } else if (q.btn) {
      var nb = hitButton(p);
      if (nb !== q.btn) { ui.held[q.btn] = false; if (nb) { ui.held[nb] = true; if (nb === 'missile') missileEdge = true; } q.btn = nb; }
    }
  });
  function up(e) {
    mouseButtons(e);
    var q = ptrs[e.pointerId]; if (!q) return;
    if (q.stick) ui.stick = null;
    if (q.btn) ui.held[q.btn] = false;
    delete ptrs[e.pointerId];
  }
  cv.addEventListener('pointerup', up);
  cv.addEventListener('pointercancel', up);
  cv.addEventListener('contextmenu', function (e) { e.preventDefault(); });

  function readInput() {
    var steer = (down('right') ? 1 : 0) - (down('left') ? 1 : 0);
    var thr = (down('up') ? 1 : 0) - (down('down') ? 1 : 0);
    if (ui.stick) { steer = Math.abs(ui.stick.dx) > 0.12 ? ui.stick.dx : 0; thr = Math.abs(ui.stick.dy) > 0.3 ? -ui.stick.dy : 0; }
    if (pad.on) { if (pad.steer) steer = pad.steer; if (pad.thr) thr = pad.thr; }
    var drift = !!(down('drift') || ui.held.drift || pad.drift);
    if (ui.dMask) { if (!drift) ui.dMask = false; drift = false; }
    var turbo = !!(down('turbo') || ui.held.turbo || pad.turbo);
    if (ui.tMask) { if (!turbo) ui.tMask = false; turbo = false; }
    var inp = {
      steer: steer, throttle: thr,
      fire: !!(down('fire') || ui.held.fire || mouseL || pad.fire),
      missile: missileEdge,
      turbo: turbo,
      drift: drift
    };
    return inp;
  }

  // ---------- 1.7: gamepad (standard mapping: Xbox names; PlayStation is the same places) ----------
  // left stick / d-pad steer (stick up/down or d-pad up/down for speed), A or RB drift, X guns, B missile,
  // LB or RT turbo, Start pause. Menus: A or Start goes on, the stick or d-pad picks, Y opens the controls,
  // B (or Back) backs out.
  var pad = { on: false, steer: 0, thr: 0, drift: false, fire: false, turbo: false }, padPrev = [];
  function pollPad() {
    var gps = null;
    try { gps = navigator.getGamepads ? navigator.getGamepads() : null; } catch (x) { gps = null; }
    var g = null;
    if (gps) for (var i = 0; i < gps.length; i++) if (gps[i] && gps[i].connected) { g = gps[i]; break; }
    if (!g) { if (pad.on) { pad.on = false; pad.steer = pad.thr = 0; pad.drift = pad.fire = pad.turbo = false; ui.padName = null; } return; }
    pad.on = true; ui.padName = g.id;
    function bt(n) { var x = g.buttons[n]; return !!x && (x.pressed || x.value > 0.4); }
    var lx = g.axes[0] || 0, ly = g.axes[1] || 0, DZ = 0.2;
    var sx = Math.abs(lx) > DZ ? (lx > 0 ? 1 : -1) * Math.min(1, (Math.abs(lx) - DZ) / (1 - DZ)) : 0;
    if (bt(14)) sx = -1; if (bt(15)) sx = 1;
    var ty = Math.abs(ly) > 0.55 ? (ly < 0 ? 1 : -1) : 0;
    if (bt(12)) ty = 1; if (bt(13)) ty = -1;
    pad.steer = sx; pad.thr = ty;
    pad.drift = bt(0) || bt(5); pad.fire = bt(2); pad.turbo = bt(4) || bt(7);
    // buttons plus the stick as four menu "buttons" (20 left, 21 right, 22 up, 23 down)
    var now = [];
    for (i = 0; i < 18; i++) now[i] = bt(i);
    now[20] = lx < -0.6; now[21] = lx > 0.6; now[22] = ly < -0.6; now[23] = ly > 0.6;
    var prev = padPrev;
    function ed(n) { return now[n] && !prev[n]; }
    var any = false;
    for (i = 0; i < now.length; i++) if (ed(i)) any = true;
    padPrev = now;
    if (!any) return;
    ui.pad = true; ui.touch = false; A.init();
    var L = ed(14) || ed(20), Rt = ed(15) || ed(21), U = ed(12) || ed(22), D = ed(13) || ed(23);
    switch (ui.screen) {
      case 'play':
        if (ed(1)) missileEdge = true;
        if (ed(9)) pause();
        break;
      case 'paused':
        if (ed(9) || ed(0)) resume();
        else if (ed(3)) openControls('paused');
        else if (ed(8) || ed(1)) go('title');
        break;
      case 'title':
        if (L || U) { setLevel(ui.level - 1); A.play('pickup'); }
        else if (Rt || D) { setLevel(ui.level + 1); A.play('pickup'); }
        else if (ed(3)) openControls('title');
        else if (ed(0) || ed(9)) advance();
        break;
      case 'controls':
        ui.ctlKeyboard = false;
        if (U) ctlMove(-1, 0); else if (D) ctlMove(1, 0); else if (L || Rt) ctlMove(0, 1);
        else if (ed(0)) ctlActivate();
        else if (ed(1) || ed(9) || ed(3)) closeControls();
        break;
      case 'reel': // 1.8: only START continues (A is drift: a stray press must not skip it)
        if (ed(9)) continueReel();
        break;
      case 'garage':
        if (L) garMove(-1, 0); else if (Rt) garMove(1, 0); else if (U) garMove(0, -1); else if (D) garMove(0, 1);
        else if (ed(0)) garActivate();
        else if (ed(2) && ui.gar.sel < SK.data.upgrades.length) garRefund(ui.gar.sel);
        else if (ed(9)) garConfirm();
        break;
      case 'rating':
        if (ed(1) || ed(8)) go('title');
        else if (ed(3) && ui.t > 1.5) go('card');
        else if (ed(0) || ed(9)) advance();
        break;
      default: // the scene card
        if (ed(0) || ed(9)) advance();
    }
  }
  window.addEventListener('gamepadconnected', function () { pollPad(); });
  window.addEventListener('gamepaddisconnected', function () { pollPad(); if (ui.screen === 'play' && !pad.on) pause(); });

  // ---------- sound cues from the engine's events ----------
  function cues(evs) {
    for (var i = 0; i < evs.length; i++) {
      var e = evs[i];
      switch (e.t) {
        case 'gun': case 'eshot': case 'missile': case 'explode': case 'hit': case 'playerHit': case 'bump':
        case 'rail': case 'near': case 'turbo': case 'jump': case 'land': case 'pickup': case 'empty': case 'rev':
        case 'drop': case 'stunt': case 'slowmo': case 'freeze': case 'bossIntro': case 'wreck': case 'collateral':
        case 'driftStart': case 'driftLvl': case 'boost': case 'zapWarn': case 'zap': case 'zoneWarn': case 'strike':
        case 'ringWarn': case 'ringOut': case 'shock': case 'dodge': case 'bossPhase':
          if (e.t === 'explode' && e.delay) { (function (o) { setTimeout(function () { A.play('explode', o); }, o.delay * 1000 / Math.max(0.25, S.timeScale)); })(e); }
          else A.play(e.t, e);
          break;
        case 'bossDown': A.play('wrap'); break;
        case 'latch': case 'henchOff': case 'henchHit': case 'henchDrop': case 'heliDown': case 'partOff': A.play(e.t, e); break;
        case 'chainWarn': case 'chain': case 'hogOff': case 'hogJump': case 'hogLand': case 'brakeWarn': A.play(e.t, e); break;
        case 'scene': A.play('clap'); break;
        case 'cutAgain': case 'outOfFilm': A.play('stamp'); break;
        case 'escaped': A.play('wreck'); break;
      }
    }
  }

  // a chopper over the road: its rotor thumps louder the closer it is
  function heliNear(V) {
    var best = 0;
    for (var i = 0; i < V.ents.length; i++) { var e = V.ents[i]; if (e.kind === 'heli' && !e.dead) best = Math.max(best, Math.min(1, Math.max(0, (e.y + 200) / 500))); }
    return best;
  }

  // ---------- 1.7.2: smooth motion at any refresh rate ----------
  // The game steps at a fixed 120 Hz whatever the screen does, so it plays the same at 60, 120 or 144 Hz.
  // A 144 Hz frame used to show 0 or 1 or 2 new steps (a step is 8.3 ms, a frame 6.9 ms), so every sixth
  // frame stood still: a judder. Now each step remembers where things were before it, and a frame is
  // drawn between the last two steps, by how far the clock has got into the next one (alpha). The state
  // is put back exactly as it was after the draw, so the game itself never sees the in-between values.
  var IPL = [];
  function ipSnap(V) {
    var i, e, L;
    V._id = V.dist;
    var p = V.p; p._ix = p.x; p._iy = p.y; p._ih = p.liftH || 0; p._iw = p.yaw || 0; p._ir = p.roll || 0;
    for (i = 0; i < V.ents.length; i++) { e = V.ents[i]; e._ix = e.x; e._iy = e.y; e._ih = e.alt || 0; }
    L = V.bullets; for (i = 0; i < L.length; i++) { e = L[i]; e._ix = e.x; e._iy = e.y; }
    L = V.ebullets; for (i = 0; i < L.length; i++) { e = L[i]; e._ix = e.x; e._iy = e.y; }
    L = V.missiles; for (i = 0; i < L.length; i++) { e = L[i]; e._ix = e.x; e._iy = e.y; }
    L = V.hz || []; for (i = 0; i < L.length; i++) { e = L[i]; e._ix = e.x; e._iy = e.y; }
  }
  function ipOne(o, a, alt) {
    if (typeof o._ix !== 'number') return;
    var dx = o.x - o._ix, dy = o.y - o._iy;
    if (dx > 40 || dx < -40 || dy > 60 || dy < -60) return; // a jump (a respawn, a reset): drawn where it is
    o._cx = o.x; o._cy = o.y; o.x = o._ix + dx * a; o.y = o._iy + dy * a;
    if (alt && o.alt !== undefined) { o._ch = o.alt; o.alt = o._ih + (o.alt - o._ih) * a; }
    IPL.push(o);
  }
  function ipOn(V, a) {
    IPL.length = 0;
    if (!V || typeof V._id !== 'number' || a <= 0) return;
    var i, L, p = V.p;
    var dd = V.dist - V._id;
    if (dd >= 0 && dd < 40) { V._cd = V.dist; V.dist = V._id + dd * a; IPL.push(V); }
    if (typeof p._ix === 'number' && Math.abs(p.x - p._ix) < 40 && Math.abs(p.y - p._iy) < 60) {
      p._cx = p.x; p._cy = p.y; p._ch = p.liftH; p._cw = p.yaw; p._cr = p.roll;
      p.x = p._ix + (p.x - p._ix) * a; p.y = p._iy + (p.y - p._iy) * a;
      if (typeof p.liftH === 'number') p.liftH = p._ih + (p.liftH - p._ih) * a;
      if (typeof p.yaw === 'number') p.yaw = p._iw + (p.yaw - p._iw) * a;
      if (typeof p.roll === 'number') p.roll = p._ir + (p.roll - p._ir) * a;
      IPL.push(p);
    }
    for (i = 0; i < V.ents.length; i++) ipOne(V.ents[i], a, true);
    L = V.bullets; for (i = 0; i < L.length; i++) ipOne(L[i], a, false);
    L = V.ebullets; for (i = 0; i < L.length; i++) ipOne(L[i], a, false);
    L = V.missiles; for (i = 0; i < L.length; i++) ipOne(L[i], a, false);
    L = V.hz || []; for (i = 0; i < L.length; i++) ipOne(L[i], a, false);
  }
  function ipOff(V) {
    for (var i = 0; i < IPL.length; i++) {
      var o = IPL[i];
      if (o === V) { V.dist = V._cd; continue; }
      if (o === V.p) { o.x = o._cx; o.y = o._cy; o.liftH = o._ch; o.yaw = o._cw; o.roll = o._cr; continue; }
      o.x = o._cx; o.y = o._cy; if (o._ch !== undefined) { o.alt = o._ch; o._ch = undefined; }
    }
    IPL.length = 0;
  }

  // ---------- the loop ----------
  function frame(now) {
    requestAnimationFrame(frame);
    var rdt = last ? Math.min(0.05, (now - last) / 1000) : 0;
    last = now;
    if (hidden) return;
    pollPad();
    clock += rdt; ui.t += rdt;
    if (ui.mutedHint > 0) ui.mutedHint -= rdt;
    if (ui.ctl.msgT > 0) ui.ctl.msgT -= rdt;
    var view = S;
    if (ui.screen === 'controls') {
      // from the title the attract run keeps driving behind it; from the pause screen the take stays frozen
      if (ui.ctl.from === 'title') {
        acc += rdt;
        var nc = 0;
        while (acc >= STEP && nc < 6) { stepAttract(STEP); acc -= STEP; nc++; }
        if (nc === 6) acc = 0;
        R.consume(attract, attract.events); attract.events.length = 0;
        view = attract;
      }
    } else if (ui.screen === 'title') {
      acc += rdt;
      var n = 0;
      while (acc >= STEP && n < 6) { stepAttract(STEP); acc -= STEP; n++; }
      if (n === 6) acc = 0;
      R.consume(attract, attract.events); attract.events.length = 0;
      view = attract;
    } else if (ui.screen === 'card') {
      if (ui.t > 3.2) go('play');
      view = attract;
    } else if (ui.screen === 'play') {
      ui.playT += rdt;
      acc += rdt;
      var steps = 0;
      while (acc >= STEP && steps < 6) {
        var inp = readInput();
        lastInp = inp;
        missileEdge = false;
        ipSnap(S);
        E.step(S, inp, STEP);
        acc -= STEP; steps++;
      }
      if (steps === 6) acc = 0;
      DIR.update(S, S.events, lastInp, rdt);
      REEL.record(S, S.events, rdt);
      R.consume(S, S.events); cues(S.events); S.events.length = 0;
      A.rotor(heliNear(S));
      A.engine(S.phase === 'play' || S.phase === 'wrap', S.speed, S.p.turboOn || S.p.boostT > 0, S.timeScale);
      A.skid(S.phase === 'play' && !!S.p.drift, S.p.drift ? S.p.drift.lvl : 0, S.timeScale);
      // each new director line pulls the song down while it is read; scene calls and CUT! the most
      if (DIR.line && DIR.line !== lastLine) A.duck(DIR.line.pri >= 3 ? 0.4 : 0.6, DIR.line.pri >= 3 ? 1.4 : 0.7);
      lastLine = DIR.line;
      if (S.phase === 'done') go(S.outcome === 'wrapped' && REEL.cut() ? 'reel' : 'rating');
      else if (S.phase === 'retake') go('card');
    } else if (ui.screen === 'reel') {
      var fr = REEL.step(rdt);
      if (fr.done) go('rating');
      else if (ui.reelAuto && fr.shot.loops > 0) { ui.t = Math.max(ui.t, 1); continueReel(); }
      else {
        ui.reel = fr.shot;
        if (fr.shot.cut) R.clearFX();
        R.consume(fr.view, fr.evs); cues(fr.evs);
        A.rotor(heliNear(fr.view));
        view = fr.view;
      }
    } else if (ui.screen === 'rating' || ui.screen === 'garage') {
      if (ui.gar) { if (ui.gar.msgT > 0) ui.gar.msgT -= rdt; if (ui.gar.flash) ui.gar.flash.t -= rdt; }
      // let the last of the smoke drift
      E.step(S, {}, rdt * 0.3);
      S.events.length = 0;
      DIR.update(S, [], {}, rdt);
    }
    A.tick(rdt);
    ui.muted = A.isMuted();
    // 1.7.2: between races, the warm-up gets a few milliseconds a frame
    if (ui.screen !== 'play' && ui.screen !== 'reel') R.warm(4);
    if (view) {
      var stepped = (ui.screen === 'play' && view === S) || ((ui.screen === 'title' || ui.screen === 'controls') && view === attract);
      if (stepped) ipOn(view, Math.min(1, acc / STEP));
      try { R.frame(view, ui, clock, rdt); } finally { if (stepped) ipOff(view); }
    }
  }

  // ---------- leaving and coming back ----------
  document.addEventListener('visibilitychange', function () {
    hidden = document.hidden;
    if (hidden) { pause(); A.suspend(); keys = {}; ui.held = {}; ui.stick = null; mouseL = mouseR = false; }
    else { A.resume(); last = performance.now(); }
  });
  window.addEventListener('pagehide', function () { pause(); });

  newAttract();
  requestAnimationFrame(frame);
  // a hook for testing in a plain browser; the game never reads it
  window.FT_DEBUG = { ui: ui, go: go, dir: DIR, reel: REEL, get S() { return S; }, set S(v) { S = v; }, get keys() { return keys; }, attract: function () { return attract; },
    binds: binds, pad: pad, lastInp: function () { return lastInp; },
    skills: SK, garage: { open: openGarage, buy: garBuy, refund: garRefund, confirm: garConfirm }, continueReel: continueReel,
    // one fixed step of play exactly as the loop takes it (reads keys, mouse and touch)
    stepInput: function () { var inp = readInput(); missileEdge = false; E.step(S, inp, STEP); S.events.length = 0; return inp; } };
})();
