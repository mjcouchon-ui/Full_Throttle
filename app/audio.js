/* FULL THROTTLE — every effect is synthesized with Web Audio. The two files loaded are the level's song and the title theme. */
(function () {
  'use strict';
  var ac = null, master = null, sfx = null, mus = null, noise = null, muted = false, MASTER = 0.8;
  var eng = null, last = {}, musicOn = false, musicNext = 0, musicStep = 0, musicTimer = 0, musicLevel = 0.5;

  // every key and click calls this: the first one also starts the title theme if the browser held it back
  function init() {
    themeStart();
    if (ac) { if (ac.state === 'suspended') ac.resume(); return; }
    var AC = window.AudioContext || window.webkitAudioContext;
    if (!AC) return;
    ac = new AC();
    master = ac.createGain(); master.gain.value = muted ? 0 : MASTER;
    var comp = ac.createDynamicsCompressor();
    comp.threshold.value = -14; comp.ratio.value = 4;
    master.connect(comp); comp.connect(ac.destination);
    sfx = ac.createGain(); sfx.gain.value = 0.9; sfx.connect(master);
    mus = ac.createGain(); mus.gain.value = 0.32; mus.connect(master);
    var len = Math.floor(ac.sampleRate * 0.6);
    noise = ac.createBuffer(1, len, ac.sampleRate);
    var d = noise.getChannelData(0);
    for (var i = 0; i < len; i++) d[i] = Math.random() * 2 - 1;
    songMake();
    themeWire();
  }

  function ok(name, gap) {
    if (!ac || muted) return false;
    var t = ac.currentTime;
    if (gap && last[name] && t - last[name] < gap) return false;
    last[name] = t; return true;
  }
  function env(g, t, a, peak, dec) {
    g.gain.setValueAtTime(0.0001, t);
    g.gain.exponentialRampToValueAtTime(peak, t + a);
    g.gain.exponentialRampToValueAtTime(0.0001, t + a + dec);
  }
  function noiseSrc(t, dur, rate) {
    var s = ac.createBufferSource(); s.buffer = noise; s.loop = true;
    if (rate) s.playbackRate.value = rate;
    s.start(t, Math.random() * 0.4); s.stop(t + dur + 0.05);
    return s;
  }
  function tone(type, f0, f1, t, dur, peak, dest, a) {
    var o = ac.createOscillator(), g = ac.createGain();
    o.type = type; o.frequency.setValueAtTime(f0, t);
    if (f1 !== f0) o.frequency.exponentialRampToValueAtTime(Math.max(1, f1), t + dur);
    env(g, t, a || 0.004, peak, dur);
    o.connect(g); g.connect(dest || sfx);
    o.start(t); o.stop(t + dur + 0.05);
    return o;
  }
  function noiseHit(t, dur, peak, type, f0, f1, q, dest) {
    var s = noiseSrc(t, dur), f = ac.createBiquadFilter(), g = ac.createGain();
    f.type = type; f.frequency.setValueAtTime(f0, t);
    if (f1 && f1 !== f0) f.frequency.exponentialRampToValueAtTime(f1, t + dur);
    f.Q.value = q || 1;
    env(g, t, 0.003, peak, dur);
    s.connect(f); f.connect(g); g.connect(dest || sfx);
  }

  var P = {
    gun: function (t) {
      noiseHit(t, 0.06, 0.32, 'bandpass', 2400, 900, 1.2);
      tone('square', 180, 70, t, 0.05, 0.12);
    },
    eshot: function (t, o) {
      noiseHit(t, 0.09, o && o.big ? 0.35 : 0.18, 'bandpass', 1200, 500, 1.5);
      tone('sawtooth', o && o.big ? 140 : 260, 90, t, 0.08, 0.08);
    },
    missile: function (t) {
      noiseHit(t, 0.7, 0.4, 'bandpass', 500, 3800, 2.5);
      tone('sawtooth', 120, 520, t, 0.55, 0.12);
      noiseHit(t, 0.12, 0.4, 'lowpass', 1600, 300, 0.7);
    },
    explode: function (t, o) {
      var s = (o && o.size) || 1;
      // the crack, the boom, the roar of the fireball and the crackle of the fire after it (1.3)
      noiseHit(t, 0.07, Math.min(0.9, 0.5 + s * 0.15), 'highpass', 1800, 900, 0.7);
      noiseHit(t, 0.5 + s * 0.45, Math.min(0.95, 0.45 + s * 0.2), 'lowpass', 3200, 120, 0.8);
      tone('sine', 110, 28, t, 0.5 + s * 0.3, Math.min(0.9, 0.5 + s * 0.15));
      noiseHit(t + 0.05, 0.6 + s * 0.4, 0.3 + s * 0.08, 'bandpass', 600, 180, 0.9);
      for (var i = 0; i < 3 + s * 3; i++) noiseHit(t + 0.15 + Math.random() * (0.5 + s * 0.4), 0.03, 0.12 + Math.random() * 0.1, 'highpass', 2500 + Math.random() * 3000, 0, 1);
      if (s > 1.5) noiseHit(t + 0.08, 1.2, 0.4, 'lowpass', 900, 60, 0.6);
    },
    // 1.3: henchmen and the chopper
    henchDrop: function (t) { noiseHit(t, 0.6, 0.18, 'bandpass', 1400, 500, 4); tone('triangle', 520, 300, t, 0.3, 0.05); },
    latch: function (t) { noiseHit(t, 0.16, 0.7, 'lowpass', 1200, 200, 1); tone('sine', 120, 60, t, 0.2, 0.5); tone('square', 300, 200, t + 0.04, 0.1, 0.08); },
    henchHit: function (t) { noiseHit(t, 0.09, 0.45, 'bandpass', 1100, 600, 1.5); tone('sine', 160, 90, t, 0.1, 0.3); },
    henchOff: function (t, o) {
      var how = o && o.how;
      if (how === 'wall' || how === 'swipe') {
        noiseHit(t, 0.35, 0.8, 'lowpass', 2400, 150, 0.8); tone('sine', 90, 40, t, 0.3, 0.7);
        noiseHit(t, 0.5, 0.35, 'highpass', 3500, 7000, 1);
      } else if (how === 'fall') { tone('sine', 900, 180, t, 0.8, 0.12); }
      else { noiseHit(t, 0.2, 0.4, 'lowpass', 900, 200, 1); }
    },
    heliDown: function (t) { tone('sawtooth', 220, 50, t, 1.1, 0.18); noiseHit(t, 1.0, 0.3, 'bandpass', 800, 200, 2); },
    partOff: function (t) {
      for (var i = 0; i < 4; i++) tone('square', 700 + Math.random() * 900, 400, t + i * 0.07 + Math.random() * 0.03, 0.05, 0.06);
      noiseHit(t, 0.12, 0.3, 'highpass', 2000, 4000, 1);
    },
    hit: function (t, o) {
      tone('square', o && o.big ? 420 : 900 + Math.random() * 300, 300, t, 0.04, 0.07);
    },
    playerHit: function (t) {
      noiseHit(t, 0.22, 0.6, 'bandpass', 900, 300, 2);
      tone('triangle', 240, 120, t, 0.2, 0.3);
      tone('square', 1300, 900, t, 0.08, 0.08);
    },
    bump: function (t) { noiseHit(t, 0.18, 0.55, 'lowpass', 900, 200, 1); tone('sine', 90, 50, t, 0.18, 0.4); },
    rail: function (t) { noiseHit(t, 0.25, 0.3, 'highpass', 3000, 6000, 1); },
    near: function (t, o) {
      // a doppler whoosh past the window
      noiseHit(t, 0.45, 0.5, 'bandpass', 2600, 380, 3);
      tone('sawtooth', 300, 120, t, 0.35, 0.05);
    },
    turbo: function (t) {
      noiseHit(t, 0.9, 0.45, 'bandpass', 300, 2800, 1.8);
      tone('sawtooth', 70, 260, t, 0.8, 0.2, sfx, 0.05);
    },
    jump: function (t) { tone('sawtooth', 160, 640, t, 0.4, 0.2); noiseHit(t, 0.3, 0.3, 'highpass', 1500, 4000, 1); },
    land: function (t) { noiseHit(t, 0.25, 0.7, 'lowpass', 700, 120, 1); tone('sine', 80, 40, t, 0.25, 0.6); },
    pickup: function (t) { tone('square', 660, 660, t, 0.07, 0.15); tone('square', 990, 990, t + 0.07, 0.12, 0.15); },
    empty: function (t) { tone('square', 140, 120, t, 0.06, 0.1); },
    rev: function (t) { tone('sawtooth', 90, 300, t, 0.45, 0.16); },
    drop: function (t) { tone('triangle', 300, 120, t, 0.15, 0.15); },
    stunt: function (t) {
      [523, 659, 784, 1047].forEach(function (f, i) { tone('square', f, f, t + i * 0.05, 0.12, 0.08); });
    },
    slowmo: function (t) {
      // the whole world drops a tape-speed octave
      tone('sine', 220, 55, t, 0.6, 0.35);
      noiseHit(t, 0.6, 0.25, 'lowpass', 2000, 150, 1);
    },
    freeze: function (t) { tone('square', 1800, 1800, t, 0.05, 0.12); noiseHit(t, 0.08, 0.4, 'highpass', 4000, 4000, 1); },
    clap: function (t) {
      // clapperboard: wood clack
      noiseHit(t, 0.06, 0.9, 'bandpass', 1800, 1400, 4);
      tone('triangle', 900, 400, t, 0.05, 0.4);
    },
    sting: function (t) {
      // title brass stab
      [110, 164.8, 220, 277.2, 329.6].forEach(function (f) {
        var o = ac.createOscillator(), g = ac.createGain(), fl = ac.createBiquadFilter();
        o.type = 'sawtooth'; o.frequency.value = f; fl.type = 'lowpass';
        fl.frequency.setValueAtTime(400, t); fl.frequency.exponentialRampToValueAtTime(3200, t + 0.12); fl.frequency.exponentialRampToValueAtTime(600, t + 1.6);
        env(g, t, 0.02, 0.12, 1.8);
        o.connect(fl); fl.connect(g); g.connect(sfx); o.start(t); o.stop(t + 2);
      });
      noiseHit(t, 1.6, 0.3, 'lowpass', 1200, 80, 0.7);
    },
    riser: function (t) { noiseHit(t, 1.4, 0.3, 'bandpass', 200, 5000, 2); tone('sawtooth', 55, 220, t, 1.4, 0.1, sfx, 0.6); },
    bossIntro: function (t) {
      [55, 58.3].forEach(function (f) { tone('sawtooth', f, f, t, 2.2, 0.25, sfx, 0.3); });
      tone('square', 440, 440, t, 0.2, 0.08); tone('square', 440, 440, t + 0.4, 0.2, 0.08); tone('square', 440, 440, t + 0.8, 0.2, 0.08);
    },
    wrap: function (t) {
      [261.6, 329.6, 392, 523.3].forEach(function (f, i) { tone('square', f, f, t + i * 0.09, 0.5, 0.07); });
      tone('sawtooth', 130.8, 130.8, t + 0.36, 1.2, 0.12);
    },
    stamp: function (t) { tone('sine', 120, 40, t, 0.4, 0.8); noiseHit(t, 0.2, 0.7, 'lowpass', 1500, 200, 1); },
    tick: function (t) { tone('square', 1500, 1500, t, 0.03, 0.06); },
    wreck: function (t) { tone('sawtooth', 300, 40, t, 1.4, 0.25); },
    collateral: function (t) { tone('square', 300, 300, t, 0.12, 0.1); tone('square', 220, 220, t + 0.13, 0.2, 0.1); },
    // 1.2: drifting and boosting
    driftStart: function (t) { noiseHit(t, 0.28, 0.32, 'bandpass', 2600, 1700, 6); tone('triangle', 1500, 1100, t, 0.2, 0.05); },
    driftLvl: function (t, o) {
      var f = [0, 880, 1175, 1568][(o && o.lvl) || 1];
      tone('square', f, f, t, 0.08, 0.09); tone('square', f * 1.5, f * 1.5, t + 0.06, 0.12, 0.07);
      noiseHit(t, 0.12, 0.15, 'highpass', 5000, 8000, 1);
    },
    boost: function (t, o) {
      var l = (o && o.lvl) || 1;
      noiseHit(t, 0.5 + l * 0.2, 0.35 + l * 0.1, 'bandpass', 250, 3600, 1.6);
      tone('sawtooth', 60, 200 + l * 60, t, 0.45 + l * 0.15, 0.18, sfx, 0.02);
      tone('sine', 90, 40, t, 0.3, 0.45);
    },
    // 1.2: the Alchemist's electricity
    zapWarn: function (t) {
      tone('sawtooth', 160, 1300, t, 0.9, 0.07, sfx, 0.3);
      noiseHit(t, 0.9, 0.12, 'highpass', 3000, 7000, 2);
    },
    zap: function (t) {
      noiseHit(t, 0.35, 0.8, 'highpass', 6000, 1200, 0.8);
      tone('square', 2400, 70, t, 0.18, 0.25);
      tone('sawtooth', 58, 58, t, 0.4, 0.22);
    },
    zoneWarn: function (t) { for (var i = 0; i < 3; i++) tone('square', 1500, 1500, t + i * 0.13, 0.06, 0.08); },
    strike: function (t) {
      noiseHit(t, 0.15, 0.9, 'highpass', 5000, 2500, 1);
      noiseHit(t + 0.03, 1.0, 0.7, 'lowpass', 2500, 70, 0.7);
      tone('sine', 70, 30, t, 0.7, 0.6);
    },
    ringWarn: function (t) { tone('sawtooth', 70, 320, t, 0.75, 0.16, sfx, 0.25); tone('sine', 140, 640, t, 0.75, 0.08, sfx, 0.25); },
    ringOut: function (t) {
      tone('sine', 120, 35, t, 0.6, 0.8); noiseHit(t, 0.7, 0.6, 'bandpass', 3000, 500, 1.2);
      tone('sawtooth', 62, 62, t, 0.8, 0.15);
    },
    shock: function (t) {
      tone('sawtooth', 55, 55, t, 0.45, 0.35); tone('square', 110, 108, t, 0.45, 0.12);
      noiseHit(t, 0.45, 0.5, 'highpass', 4000, 4000, 1);
    },
    dodge: function (t) { tone('triangle', 988, 988, t, 0.08, 0.12); tone('triangle', 1319, 1319, t + 0.07, 0.15, 0.12); },
    bossPhase: function (t) { for (var i = 0; i < 4; i++) tone('square', i % 2 ? 660 : 880, i % 2 ? 660 : 880, t + i * 0.16, 0.14, 0.09); },
    // 1.5: the SNAKE EYES. A chain whirling (a rising whoosh), its swing (a whip crack and a clank), a
    // chopper's V-twin blat as one rolls off the carrier, a jump off the top deck and its landing, and
    // the carrier's air brakes
    chainWarn: function (t) { for (var i = 0; i < 3; i++) noiseHit(t + i * 0.18, 0.16, 0.14, 'bandpass', 900 + i * 500, 1800 + i * 600, 4); },
    chain: function (t, o) {
      noiseHit(t, 0.08, 0.4, 'highpass', 3000, 2000, 1);
      if (o && o.hit) { tone('square', 220, 90, t + 0.02, 0.12, 0.18); noiseHit(t + 0.02, 0.2, 0.3, 'bandpass', 1400, 500, 3); }
      else tone('triangle', 1320, 1320, t + 0.05, 0.1, 0.08);
    },
    hogOff: function (t) { for (var i = 0; i < 5; i++) tone('sawtooth', 70 + i * 6, 60 + i * 6, t + i * 0.07, 0.08, 0.12); },
    hogJump: function (t) { tone('sawtooth', 90, 260, t, 0.5, 0.14); noiseHit(t, 0.3, 0.12, 'bandpass', 600, 1600, 2); },
    hogLand: function (t, o) { noiseHit(t, 0.25, o && o.crush ? 0.5 : 0.3, 'lowpass', 600, 120, 1); if (o && o.crush) tone('square', 160, 60, t, 0.2, 0.2); },
    brakeWarn: function (t) { noiseHit(t, 0.7, 0.3, 'highpass', 2500, 5000, 1); tone('square', 520, 520, t, 0.12, 0.08); tone('square', 520, 520, t + 0.2, 0.12, 0.08); }
  };
  var GAP = { henchHit: 0.1, partOff: 0.15, gun: 0.06, hit: 0.035, eshot: 0.05, explode: 0.04, rail: 0.2, near: 0.1, bump: 0.1, tick: 0.03, zap: 0.05, strike: 0.06, driftStart: 0.25, shock: 0.2 };

  function play(name, o) {
    if (!P[name] || !ok(name, GAP[name])) return;
    if (DUCK[name]) duck(DUCK[name][0], DUCK[name][1]);
    try { P[name](ac.currentTime + 0.005, o); } catch (e) { /* a sound never stops the game */ }
  }

  // engine: two detuned saws through a lowpass, pitched by road speed
  function engine(on, speed, turbo, slowScale) {
    if (!ac) return;
    if (!eng) {
      var g = ac.createGain(); g.gain.value = 0;
      var f = ac.createBiquadFilter(); f.type = 'lowpass'; f.frequency.value = 500; f.Q.value = 3;
      var o1 = ac.createOscillator(), o2 = ac.createOscillator();
      o1.type = 'sawtooth'; o2.type = 'square';
      o1.connect(f); o2.connect(f); f.connect(g); g.connect(sfx);
      o1.start(); o2.start();
      eng = { g: g, f: f, o1: o1, o2: o2 };
    }
    var t = ac.currentTime, k = (slowScale === undefined ? 1 : 0.55 + 0.45 * slowScale);
    var base = (34 + speed * 0.075) * k;
    eng.o1.frequency.setTargetAtTime(base, t, 0.08);
    eng.o2.frequency.setTargetAtTime(base * 0.5 + 1.3, t, 0.08);
    eng.f.frequency.setTargetAtTime((turbo ? 1500 : 700) * k, t, 0.1);
    eng.g.gain.setTargetAtTime(on && !muted ? (turbo ? 0.13 : 0.08) : 0, t, 0.12);
    if (!on) skid(false);
  }
  // 1.3: the chopper's rotor, a low thump chopped at about 11 beats a second; lvl 0..1 is how close it is
  var rot = null;
  function rotor(lvl) {
    if (!ac) return;
    lvl = +lvl || 0;
    if (!rot) {
      var s = ac.createBufferSource(); s.buffer = noise; s.loop = true;
      var f = ac.createBiquadFilter(); f.type = 'lowpass'; f.frequency.value = 420; f.Q.value = 1.5;
      var g = ac.createGain(); g.gain.value = 0;
      var am = ac.createGain(); am.gain.value = 0.5;
      var lfo = ac.createOscillator(); lfo.type = 'square'; lfo.frequency.value = 11;
      var lg = ac.createGain(); lg.gain.value = 0.5;
      lfo.connect(lg); lg.connect(am.gain);
      s.connect(f); f.connect(am); am.connect(g); g.connect(sfx); s.start(); lfo.start();
      rot = { g: g, f: f };
    }
    rot.g.gain.setTargetAtTime(lvl > 0 && !muted ? 0.05 + 0.3 * lvl : 0, ac.currentTime, 0.15);
  }

  // tyre squeal while drifting: filtered noise that climbs a little with each charge level
  var sk = null;
  function skid(on, lvl, slowScale) {
    if (!ac) return;
    if (!sk) {
      var s = ac.createBufferSource(); s.buffer = noise; s.loop = true;
      var f = ac.createBiquadFilter(); f.type = 'bandpass'; f.frequency.value = 1900; f.Q.value = 7;
      var g = ac.createGain(); g.gain.value = 0;
      s.connect(f); f.connect(g); g.connect(sfx); s.start();
      sk = { f: f, g: g };
    }
    var t = ac.currentTime, k = slowScale === undefined ? 1 : 0.6 + 0.4 * slowScale;
    sk.f.frequency.setTargetAtTime((1700 + (lvl || 0) * 260) * k, t, 0.05);
    sk.g.gain.setTargetAtTime(on && !muted ? 0.11 : 0, t, on ? 0.04 : 0.09);
  }

  // a driving synth bassline, scheduled ahead
  var BASS = [45, 45, 57, 45, 45, 55, 45, 57, 43, 43, 55, 43, 48, 48, 50, 52];
  function mtof(m) { return 440 * Math.pow(2, (m - 69) / 12); }
  function music(on, level) {
    musicOn = !!on; if (level !== undefined) musicLevel = level;
    if (!ac) return;
    mus.gain.setTargetAtTime(musicOn && !muted ? 0.32 * musicLevel : 0, ac.currentTime, 0.3);
    if (musicOn && !musicTimer) {
      musicNext = ac.currentTime + 0.05;
      musicTimer = setInterval(schedule, 60);
    }
  }
  function schedule() {
    if (!ac || !musicOn) { clearInterval(musicTimer); musicTimer = 0; return; }
    var spb = 60 / 138 / 2;
    while (musicNext < ac.currentTime + 0.2) {
      var t = musicNext, s = musicStep % 16, n = BASS[s];
      var o = ac.createOscillator(), g = ac.createGain(), f = ac.createBiquadFilter();
      o.type = 'sawtooth'; o.frequency.value = mtof(n);
      f.type = 'lowpass'; f.frequency.setValueAtTime(1400, t); f.frequency.exponentialRampToValueAtTime(220, t + spb * 0.9); f.Q.value = 6;
      env(g, t, 0.005, 0.35, spb * 0.9);
      o.connect(f); f.connect(g); g.connect(mus); o.start(t); o.stop(t + spb);
      if (s % 4 === 0) { var k = ac.createOscillator(), kg = ac.createGain(); k.frequency.setValueAtTime(140, t); k.frequency.exponentialRampToValueAtTime(40, t + 0.12); env(kg, t, 0.002, 0.7, 0.14); k.connect(kg); kg.connect(mus); k.start(t); k.stop(t + 0.2); }
      if (s % 2 === 1) { var h = noiseSrc(t, 0.04), hf = ac.createBiquadFilter(), hg = ac.createGain(); hf.type = 'highpass'; hf.frequency.value = 7000; env(hg, t, 0.001, 0.12, 0.04); h.connect(hf); hf.connect(hg); hg.connect(mus); }
      if (s === 4 || s === 12) { var sn = noiseSrc(t, 0.15), sf = ac.createBiquadFilter(), sg = ac.createGain(); sf.type = 'bandpass'; sf.frequency.value = 1800; env(sg, t, 0.002, 0.3, 0.15); sn.connect(sf); sf.connect(sg); sg.connect(mus); }
      musicNext += spb; musicStep++;
    }
  }

  // ---------- the level's song: "Pixel Adventure", the owner's own track, streamed from the package ----------
  // It plays through the same master as everything else (so M and WispOS's suspend silence it), sits well
  // under the effects, and dips further whenever the director puts up a line or something big goes off.
  var SONG_SRC = 'assets/audio/pixel-adventure.mp3';
  var SONG_LEVEL = 0.34;            // the file is mastered loud (about -17 dBFS); this sits it about 10 dB under the busy mix
  var song = null, songOn = false, songDuck = 1, songHold = 0, songVol = 0;
  // 1.7.2: the element is made (and starts buffering) as the page loads, so the file is read and ready
  // long before the first race instead of from the first key press
  var songEl = null;
  (function songPreload() {
    if (typeof Audio === 'undefined') return;
    songEl = new Audio(); songEl.preload = 'auto'; songEl.loop = true; songEl.src = SONG_SRC;
    try { songEl.load(); } catch (e) { /* fine */ }
  })();
  function songMake() {
    if (song || !ac) return;
    var el = songEl || new Audio(); if (!songEl) { el.src = SONG_SRC; el.loop = true; el.preload = 'auto'; }
    song = { el: el, g: null };
    // in WispOS (and any served page) the song goes through Web Audio, the only volume a phone honours;
    // a page opened straight from disk can't feed a file into Web Audio, so there it uses the element's own volume
    if (location.protocol !== 'file:') {
      try {
        var src = ac.createMediaElementSource(el), g = ac.createGain();
        g.gain.value = 0; src.connect(g); g.connect(master); song.g = g;
      } catch (e) { song.g = null; }
    }
    el.volume = song.g ? 1 : 0;
    el.muted = muted;
    // first touch or key: start and stop it once so a phone lets it play later without a tap
    var p = el.play(); if (p && p.then) p.then(function () { if (!songOn) el.pause(); }).catch(function () {});
  }
  // on: play or pause it. restart: from the top (a fresh run); otherwise it carries on where it stopped
  function songPlay(on, restart) {
    songOn = !!on;
    if (!song) return;
    var el = song.el;
    if (restart) { try { el.currentTime = 0; } catch (e) { /* not loaded yet: it starts at 0 anyway */ } }
    if (songOn) { var p = el.play(); if (p && p.catch) p.catch(function () {}); }
    else el.pause();
  }
  function duck(depth, hold) {
    songDuck = Math.min(songDuck, depth);
    songHold = Math.max(songHold, hold);
  }
  // ---------- the title theme: "Bit-Beast Dash", the track the owner picked for the title screen (1.6) ----------
  // The element is made as the page loads so it can start straight away where the browser allows that;
  // where it doesn't, the first key or click starts it (init). Before the first key or click it plays at the
  // element's own volume; after that it goes through the master like the level's song, so M and WispOS's
  // suspend silence it. It fades in when the title comes up, and fades out over FADE_OUT when a race starts.
  var THEME_SRC = 'assets/audio/title-theme.mp3';
  var THEME_LEVEL = 0.55;           // mastered about 1 dB under the level's song; nothing else plays on the title
  var FADE_IN = 0.4, FADE_OUT = 1.2; // seconds; the card before a race is never shorter than 1.2 s
  var theme = { el: null, g: null, on: false, vol: 0, held: false, blocked: false };
  (function themeMake() {
    if (typeof Audio === 'undefined') return;
    var el = new Audio(); el.src = THEME_SRC; el.loop = true; el.preload = 'auto'; el.volume = 0;
    theme.el = el;
  })();
  // route it into Web Audio once the context exists (a page opened straight from disk keeps the element's volume)
  function themeWire() {
    if (!ac || theme.g || !theme.el || location.protocol === 'file:') return;
    try {
      var src = ac.createMediaElementSource(theme.el), g = ac.createGain();
      g.gain.value = theme.vol; src.connect(g); g.connect(master); theme.g = g; theme.el.volume = 1;
    } catch (e) { theme.g = null; }
  }
  // play it if it should be playing and isn't; a fresh visit to the title starts it from the top
  function themeStart() {
    var el = theme.el;
    if (!el || !theme.on || !el.paused) return;
    if (theme.vol <= 0) { try { el.currentTime = 0; } catch (e) { /* not loaded yet: starts at 0 anyway */ } }
    var p = el.play();
    if (p && p.then) p.then(function () { theme.blocked = false; if (!theme.on && theme.vol <= 0) el.pause(); })
      .catch(function () { theme.blocked = true; });
  }
  // on: fade it in (starting it if needed); off: fade it out, and tick pauses it once it is silent
  function themePlay(on) {
    theme.on = !!on;
    if (theme.on) themeStart();
  }
  function themeTick(dt) {
    var el = theme.el;
    if (!el) return;
    var target = theme.on && !muted ? THEME_LEVEL * songDuck : 0;
    var rate = THEME_LEVEL / (target > theme.vol ? FADE_IN : FADE_OUT);
    theme.vol = target > theme.vol ? Math.min(target, theme.vol + rate * dt) : Math.max(target, theme.vol - rate * dt);
    if (!theme.on && theme.vol <= 0 && !el.paused) el.pause();
    if (theme.g) theme.g.gain.setTargetAtTime(theme.vol, ac.currentTime, 0.015);
    else el.volume = Math.max(0, Math.min(1, theme.vol * MASTER));
  }

  // every frame: let a dip ride out, then come back up
  function tick(dt) {
    themeTick(dt);
    if (!song) return;
    if (songHold > 0) songHold -= dt;
    else songDuck = Math.min(1, songDuck + dt * 1.4);
    var target = songOn && !muted ? SONG_LEVEL * songDuck : 0;
    if (song.g) song.g.gain.setTargetAtTime(target, ac.currentTime, 0.05);
    else { songVol += (target - songVol) * Math.min(1, dt * 18); song.el.volume = Math.max(0, Math.min(1, songVol)); }
  }
  // the big moments push the song down for a beat so they cut through
  var DUCK = { explode: [0.55, 0.25], stunt: [0.5, 0.5], clap: [0.4, 0.4], sting: [0.35, 1.2], wrap: [0.35, 1.2],
    bossIntro: [0.35, 1.6], stamp: [0.4, 0.6], wreck: [0.4, 1], slowmo: [0.45, 0.8], playerHit: [0.7, 0.2], missile: [0.7, 0.3],
    zap: [0.55, 0.3], strike: [0.5, 0.35], heliDown: [0.4, 0.8], latch: [0.7, 0.3], ringOut: [0.5, 0.4], boost: [0.7, 0.3], bossPhase: [0.45, 0.8] };

  function setMuted(m) {
    muted = !!m;
    if (master) master.gain.setTargetAtTime(muted ? 0 : MASTER, ac.currentTime, 0.05);
    if (song) song.el.muted = muted;
    if (theme.el) theme.el.muted = muted;
  }

  window.FT_AUDIO = {
    init: init, play: play, engine: engine, skid: skid, music: music, rotor: rotor,
    song: songPlay, duck: duck, tick: tick,
    songState: function () { return song ? { on: songOn, paused: song.el.paused, t: song.el.currentTime, dur: song.el.duration, loop: song.el.loop, graph: !!song.g,
      gain: song.g ? song.g.gain.value : song.el.volume, duck: songDuck, muted: song.el.muted, err: song.el.error && song.el.error.code, ready: song.el.readyState } : null; },
    title: themePlay,
    themeState: function () { var el = theme.el; return el ? { on: theme.on, paused: el.paused, t: el.currentTime, dur: el.duration, loop: el.loop, graph: !!theme.g,
      vol: theme.vol, gain: theme.g ? theme.g.gain.value : el.volume, muted: el.muted, blocked: theme.blocked, err: el.error && el.error.code, ready: el.readyState, src: el.src } : null; },
    setMuted: setMuted, isMuted: function () { return muted; },
    // leaving the app: the theme may still be playing outside Web Audio, so it is paused, not just suspended
    suspend: function () {
      if (theme.el && !theme.el.paused) { theme.el.pause(); theme.held = true; }
      if (ac && ac.state === 'running') ac.suspend();
    },
    resume: function () {
      if (ac && ac.state === 'suspended') ac.resume();
      if (theme.held) { theme.held = false; if (theme.on) { var p = theme.el.play(); if (p && p.catch) p.catch(function () {}); } }
    }
  };
})();
