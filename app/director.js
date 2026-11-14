/* FULL THROTTLE — the director. Watches the take (the engine's state, its events and what the
   player is pressing) and calls out direction the whole way through, like a director on a megaphone.
   Pure: no DOM, no canvas, no audio. FT_DIRECTOR.make() -> D; D.reset(); D.update(S, events, input, dt).
   The page draws D.line (the current direction), D.shout (one big word: ACTION!, FLY!) and D.slate
   (the scene board that opens each scene). */
(function (root) {
  'use strict';

  // ---------- what the director says ----------
  var INTRO = [
    ['Scene one. You have the codebook. Drive like they want it back!', 'Weave that traffic. Make it look dangerous!'],
    ['Bikers on your tail, and the road goes up and over! Take them out!', 'Guns up over those humps. Don’t let them close in!'],
    ['Rammers on the banked turn! Ride the wall and make them miss!', 'Up on the bank! Dodge the rammers!'],
    ['Car carrier, then the road just ENDS! Turbo and fly the gap!', 'Ramp, then the gap, then the loop. Give me air!'],
    ['Armored truck! Missiles, then hang on for the corkscrew!', 'That truck is the shot. Crack it before the corkscrew!'],
    ['Gunships! Shoot back, then fly the big gap!', 'Crossfire, then the big jump. Keep moving!'],
    ['Three rammers and another loop. Make me gasp!', 'The wolf pack. Thread them, then loop it!'],
    ['There he is. The Juggernaut. Take him DOWN!', 'Final shot. Everything you’ve got on the Juggernaut!']
  ];
  var RETAKE = [
    'Take {k}. Same scene, more conviction!',
    'Take {k}. Stay on the road this time!',
    'Take {k}. Fresh paint on the car. Don’t waste it!',
    'Take {k}. From the top. Eyes up!'
  ];
  var CUT_AGAIN = ['CUT! CUT! Again, from the top!', 'CUT! We lost the car. Again!', 'CUT! Reset the scene. Again!'];
  var P = {
    kill: ['Yes! That’s the shot!', 'Boom! Beautiful!', 'Gorgeous wreck! Keep rolling!', 'That’s going in the trailer!', 'Print it! Next!'],
    bigKill: ['THAT’S the money shot!', 'The truck! The studio’s going to love this!', 'Look at that fireball! Gorgeous!'],
    near: ['Closer! I want paint on paint!', 'Ooh, inches! Again!', 'That’s the danger I want!', 'Close shave! Love it!'],
    rail: ['Sparks! YES! More sparks!', 'Grind that rail! Gorgeous!', 'Look at those sparks fly!'],
    collateral: ['Not the civilians! Insurance is killing me!', 'Who hired that extra?! Careful!', 'Civilians cost money, Kane!'],
    hit: ['You’re taking fire! Move!', 'Get out of his line!', 'Don’t let them scratch my star car!', 'Shake him off!'],
    lowHp: ['The car’s on fire! Find a wrench!', 'That’s our only car! Get repairs!'],
    pickupMissiles: ['Props! More missiles!', 'Missiles loaded. Use them!'],
    pickupRepair: ['Crew patched you up. Go!', 'Fresh armor. Back in it!'],
    rev: ['He’s lining you up! Get out of his lane!', 'Headlights! He’s about to charge. Move!'],
    turbo: ['Punch it!', 'TURBO! Yes!', 'Speed lines! Beautiful!'],
    empty: ['No missiles! Grab an orange crate!', 'You’re dry! Find the missile crates!'],
    land: ['Stick the landing! Yes!', 'And down! Gorgeous!'],
    takeOut: ['Take him out, now!', 'Shoot! What are you waiting for?', 'Light him up!', 'He’s right there! Fire!'],
    keepFiring: ['Good! Keep pouring it on!', 'Hold that fire! Don’t let up!', 'More bullets! More!'],
    missiles: ['Missiles! Crack that armor!', 'Use the missiles! Now!'],
    ramp: ['Ramp ahead! Line it up and turbo!', 'There’s your ramp! Hit it fast!'],
    careful: ['The car’s smoking. Careful, that’s our only one!', 'Keep it together. We need that car!'],
    faster: ['Faster! This is a chase, not a commute!', 'Speed! I need speed!', 'Don’t brake! Faster!'],
    turboAsk: ['Faster! Hit the turbo!', 'Turbo’s full! Use it, give me speed!'],
    sparks: ['Faster, give me sparks! Kiss that rail!', 'Scrape the barrier! I want sparks!'],
    nearAsk: ['Get closer to that traffic! Paint on paint!', 'Weave! Make it look dangerous!', 'Thread the cars! Closer!'],
    aheadLoop: ['LOOP ahead! Keep your foot down all the way round!', 'Here comes the loop! Don’t lift!'],
    aheadJump: ['The road ENDS up there! Hit the lip fast and FLY!', 'Gap ahead! Turbo off the lip. Big air!'],
    aheadTwist: ['Corkscrew coming! Hold on to your lunch!', 'The road rolls over up there. Keep shooting!'],
    aheadWall: ['Banked wall ahead! Ride it up high!', 'Big banked turn! Get up on the wall!'],
    loopIn: ['Up the wall! Don’t lift!', 'Round you go! Keep it floored!'],
    loopTop: ['UPSIDE DOWN! The camera loves it!', 'Over the top! Keep rolling!'],
    twistIn: ['Here we go! Roll it!', 'Corkscrew! Keep it steady!'],
    twistTop: ['Upside down and still driving! Gorgeous!', 'Look at that sky under you!'],
    wallIn: ['Up on the bank! Higher! Higher!', 'Ride the wall! Get up there!'],
    aheadPipe: ['Pipe ahead! The road goes all the way round inside it!', 'Into the tube! Keep it floored all the way round!'],
    pipeIn: ['Into the pipe! Up the wall!', 'Round the tube you go! Don’t lift!'],
    pipeTop: ['On the CEILING! Keep driving!', 'Upside down in the pipe! Gorgeous!'],
    gapLand: ['And DOWN! Stick it!', 'Landed it! Print that!', 'The road came back! Beautiful!'],
    inverted: ['Shoot upside down! Make it look easy!', 'Hang on up there!'],
    boost: ['Boost! Go, go, go!', 'Slide and BOOST! That’s driving!', 'Out of the drift and GONE!'],
    driftAsk: ['Throw it sideways into the bend! Drift it!', 'Hold the drift through the turn! Sparks!'],
    zapWarn: ['Lightning in your lane! MOVE!', 'He’s charging the coil! Change lanes!', 'That lane’s going live! Get out!'],
    zoneWarn: ['He’s locked on! Keep moving!', 'Targets on the road! Don’t sit still!', 'Lightning’s coming down! Move it!'],
    ringWarn: ['Shock wave! Find the green gap!', 'Here comes the wave! Get to the gap!', 'He’s discharging! Gap, gap, GAP!'],
    shock: ['You’re getting fried! Out of the light!', 'Zapped! Read the warnings, Kane!', 'Sparks on the star car! Dodge it!'],
    dodge: ['Through the gap! Beautiful!', 'Clean dodge! The crew is screaming!', 'Missed you by a mile! Gorgeous!'],
    heliIn: ['Chopper! They’re dropping men on you!', 'Here comes the chopper! Swerve, don’t let them land!', 'Bird overhead! Shoot it or shake them!'],
    latch: ['He’s on the car! Slam him into the wall!', 'Passenger! Scrape him off on the barrier!', 'Hitchhiker! Into the wall with him!'],
    latch2: ['Two of them! Wall, wall, WALL!', 'Both doors! Loop it or grind them off!'],
    henchHang: ['He’s still on! Ram that side into the wall!', 'Get him off my star car! The wall, Kane!', 'Go upside down and he’s gone!'],
    wallSmash: ['Into the wall! Ha! Print it!', 'Scraped him right off! Gorgeous!', 'Ooh, that’s gonna need a stunt double!'],
    swipe: ['Wiped him off on that car! Genius!', 'Sideswipe! Beautiful!'],
    fallOff: ['Upside down and he’s GONE! Gravity wins!', 'He fell off the ceiling! Print it!'],
    heliDown: ['You shot down a CHOPPER?! Somebody call the studio!', 'The chopper! Look at it burn!'],
    dropDodged: ['Missed you! He kissed the road!', 'Ha! He landed on nothing!'],
    partOff: ['There goes a piece of the car!', 'You’re shedding parts! Careful!', 'The car’s falling apart! Find a wrench!'],
    // 1.5: the SNAKE EYES and the Big Dealer's carrier
    chainWarn: ['Chain! He\u2019s swinging a chain! Get away from him!', 'Biker on your door! Slam him or move!', 'He\u2019s winding up! Ram him off the bike!'],
    chainDodged: ['Missed you! Now hit him back!', 'Ducked it! Gorgeous!'],
    chainHit: ['He chained the star car! Hit him back!', 'Ow! Ram that chopper!'],
    hogJump: ['They\u2019re JUMPING off the truck! Watch the marks!', 'Flying bikers! Get out from under them!'],
    brakeWarn: ['Brake lights! He\u2019s brake-checking you! Back off!', 'He\u2019s stopping! Don\u2019t kiss that ramp!'],
    calm: ['Good. Keep it rolling.', 'Hold that line. Camera loves you.', 'Stay on it. Don’t lose the light.', 'Nice. Nice. Keep going.']
  };
  var STUNT = {
    'AIRBORNE KILL': 'From the AIR?! Genius!',
    'TURBO RAM': 'Ram him! YES!',
    'ROOF LANDING': 'On the ROOF! Legendary!',
    'DOUBLE KILL': 'Two at once! Print it!',
    'TRIPLE KILL': 'Three! I’m crying!',
    'MASSACRE': 'MASSACRE! Somebody call the Academy!',
    'THREAD THE NEEDLE': 'Through the gap! Perfect!',
    'RAMP JUMP': 'FLY! Fly, Kane!',
    'TURBO JUMP': 'Look at that AIR! Fly!',
    'BIG AIR': 'FLY! Clear that gap!',
    'TURBO AIR': 'LOOK at that air! Over the whole gap!',
    'LOOP THE LOOP': 'A full LOOP! The crew is screaming!',
    'TURBO LOOP': 'Turbo through the loop! Print it twice!',
    'CORKSCREW': 'Through the corkscrew! Legendary!',
    'WALL RIDE': 'Riding the wall! Gorgeous!',
    'PIPE RUN': 'All the way round the pipe! Print it!',
    'UPSIDE-DOWN KILL': 'Upside down?! That’s cinema!',
    'POWER DRIFT': 'Look at that slide! Gorgeous!',
    'ULTRA DRIFT': 'Sideways at that speed?! Print it!',
    'WALL SMASH': 'Into the wall! Ha! Print it!',
    'SIDESWIPE': 'Wiped him off on that car! Genius!',
    'GRAVITY CHECK': 'Upside down and he’s GONE! Gravity wins!',
    'GRAVITY CHECK x2': 'BOTH of them! Off the ceiling! Legendary!',
    'BIKER SLAM': 'Off the bike! Ha! The stuntman earned his pay!'
  };

  function make() {
    var D = {
      line: null, shout: null, slate: null, time: 0, log: [],
      cd: {}, idx: {}, queue: [], tr: {}, boss: {}
    };

    function pick(key, pool) {
      var i = D.idx[key] === undefined ? 0 : D.idx[key] + 1;
      D.idx[key] = i;
      return pool[i % pool.length];
    }
    // put a line up. pri: 0 ambient direction, 1 small reaction, 2 big reaction, 3 scene call, 4 cut
    function say(key, text, pri, cd) {
      if (D.cd[key] > 0) return false;
      var L = D.line;
      if (L && pri < L.pri && L.age < L.hold) return false;
      if (L && L.age < 0.7 && pri <= L.pri && pri < 3) return false; // let a line land before the next
      D.line = { text: text, pri: pri, age: 0, hold: Math.min(3.6, Math.max(1.8, 0.9 + text.length * 0.045)), key: key };
      if (cd) D.cd[key] = cd;
      D.log.push({ t: +D.time.toFixed(2), key: key, text: text });
      if (D.log.length > 400) D.log.shift();
      return true;
    }
    function sayFrom(key, pri, cd) { return say(key, pick(key, P[key]), pri, cd); }
    function shout(text, col, life) { D.shout = { text: text, col: col || '#ffd23f', age: 0, life: life || 1.1 }; }
    function later(at, fn) { D.queue.push({ at: at, fn: fn }); }
    // a set-piece call that can't get a word in yet keeps trying for a moment, then lets it go
    function insist(key, pri, ttl) {
      var text = pick(key, P[key]);
      if (!say(key, text, pri, 0)) D.pend.push({ key: key, text: text, pri: pri, ttl: ttl });
    }

    D.reset = function () {
      D.line = null; D.shout = null; D.slate = null; D.queue = []; D.cd = {}; D.boss = {}; D.gap = false; D.pend = [];
      D.tr = { fire: 0, near: 0, rail: 0, turbo: 0, missile: 0, drift: 0 };
    };
    D.reset();

    function scene(e) {
      D.slate = { n: e.n, name: e.name, take: e.take, age: 0 };
      D.queue = []; D.pend = [];
      D.boss = {};
      D.tr = { fire: 0, near: 0, rail: 0, turbo: 0, missile: 0, drift: 0 };
      D.cd = {};
      var intro = e.intro || INTRO[e.n] || INTRO[0];
      if (e.take > 1) {
        say('scene', pick('retake', RETAKE).replace('{k}', e.take), 3);
        later(2.4, function () { say('sceneNote', intro[1], 2); });
      } else {
        say('scene', intro[0], 3);
      }
      later(0.45, function () { shout('ACTION!', '#ffd23f', 1.0); });
    }

    function ambient(S) {
      var K = root.FT_ENGINE.KIND, p = S.p, foes = 0, big = false, ramp = false, i, e;
      for (i = 0; i < S.ents.length; i++) {
        e = S.ents[i];
        var k = K[e.kind];
        if (k.foe && e.y > -40 && e.y < p.y - 30) { foes++; if (k.big) big = true; }
        if (k.ramp && !e.used && e.y > -160 && e.y < p.y - 60) ramp = true;
      }
      var t = D.tr;
      if (S.inverted && say('inverted', pick('inverted', P.inverted), 0, 4)) return;
      if (ramp && say('ramp', pick('ramp', P.ramp), 0, 5)) return;
      if (big && p.missiles > 0 && t.missile > 4 && say('missiles', pick('missiles', P.missiles), 0, 6)) return;
      if (foes && t.fire > 1.2 && say('takeOut', pick('takeOut', P.takeOut), 0, 2.5)) return;
      if (p.hp < 40 && say('careful', pick('careful', P.careful), 0, 8)) return;
      if (S.speed < 360 && t.turbo > 2 && say('faster', pick('faster', P.faster), 0, 5)) return;
      if (foes && t.fire < 0.5 && say('keepFiring', pick('keepFiring', P.keepFiring), 0, 5)) return;
      if (p.turbo > 95 && t.turbo > 7 && say('turboAsk', pick('turboAsk', P.turboAsk), 0, 8)) return;
      if (!foes && t.rail > 10 && say('sparks', pick('sparks', P.sparks), 0, 10)) return;
      if (!foes && t.near > 6 && say('nearAsk', pick('nearAsk', P.nearAsk), 0, 7)) return;
      var E = root.FT_ENGINE, bend = Math.abs(E.curveAt(S.dist + 600 - p.y + 700));
      if (!foes && bend > 0.0009 && t.drift > 14 && say('driftAsk', pick('driftAsk', P.driftAsk), 0, 14)) return;
      if (foes && say('takeOut', pick('takeOut', P.takeOut), 0, 2.5)) return;
      say('calm', pick('calm', P.calm), 0, 3);
    }

    D.update = function (S, evs, inp, dt) {
      D.time += dt;
      inp = inp || {};
      var k;
      for (k in D.cd) if (D.cd[k] > 0) D.cd[k] -= dt;
      if (D.line) D.line.age += dt;
      if (D.shout) { D.shout.age += dt; if (D.shout.age > D.shout.life) D.shout = null; }
      if (D.slate) { D.slate.age += dt; if (D.slate.age > 2.8) D.slate = null; }
      var t = D.tr;
      for (k in t) t[k] += dt;
      if (inp.fire) t.fire = 0;
      if (inp.turbo) t.turbo = 0;
      if (S.p.turboOn) t.turbo = 0;

      for (var i = 0; i < evs.length; i++) {
        var e = evs[i];
        switch (e.t) {
          case 'scene': scene(e); break;
          case 'cutAgain': D.queue = []; D.slate = null; say('cut', pick('cutAgain', CUT_AGAIN), 4); break;
          case 'outOfFilm': D.queue = []; say('cut', 'CUT! That’s it. We’re out of film!', 4); break;
          case 'bossDown': D.queue = []; say('wrap', 'CUT! PRINT IT! That’s a WRAP, people!', 5); break;
          case 'escaped': D.queue = []; say('wrap', 'CUT... He got away. Unbelievable.', 5); break;
          case 'stunt':
            var sk = e.name.indexOf('PIPE RUN') === 0 ? 'PIPE RUN' : e.name;
            if (STUNT[sk] && say('stunt', STUNT[sk], 2.5, 0.8)) {
              if (e.name === 'RAMP JUMP' || e.name === 'TURBO JUMP' || e.name === 'BIG AIR' || e.name === 'TURBO AIR') shout('FLY!', '#7ff7ff', 1.2);
              else if (e.name === 'LOOP THE LOOP' || e.name === 'TURBO LOOP') shout('LOOP!', '#7ff7ff', 1.0);
              else if (e.name === 'CORKSCREW') shout('CORKSCREW!', '#7ff7ff', 1.0);
              else if (sk === 'PIPE RUN') shout(e.name.replace('PIPE RUN', 'PIPE!'), '#7ff7ff', 1.0);
              else if (e.name === 'ULTRA DRIFT') shout('DRIFT!', '#ff4fb8', 1.0);
            }
            break;
          case 'ahead':
            var ak = { loop: 'aheadLoop', jump: 'aheadJump', twist: 'aheadTwist', wall: 'aheadWall', pipe: 'aheadPipe' }[e.kind];
            if (ak) insist(ak, 2.6, 2.2);
            break;
          case 'loopIn': case 'loopTop': case 'twistIn': case 'twistTop': case 'wallIn': case 'pipeIn': case 'pipeTop':
            insist(e.t, 2.6, e.t === 'loopTop' || e.t === 'twistTop' || e.t === 'pipeTop' ? 0.8 : 1.2);
            if (e.t === 'loopTop' || e.t === 'twistTop' || e.t === 'pipeTop') shout('UPSIDE DOWN!', '#ffd23f', 0.9);
            break;
          case 'gapJump': D.gap = true; break;
          case 'heliIn': insist('heliIn', 2.6, 1.6); shout('CHOPPER!', '#ff9e3d', 0.9); break;
          case 'latch':
            insist(S.p.latch.length > 1 ? 'latch2' : 'latch', 2.7, 1.2); shout('HE\u2019S ON THE CAR!', '#ff4fb8', 1.0); t.latch = 0; break;
          case 'henchOff':
            if (e.how === 'miss') sayFrom('dropDodged', 2, 3);
            break;
          case 'partOff': if (S.p.hp > 0) sayFrom('partOff', 1, 8); break;
          case 'kill':
            if (e.kind === 'boss' || e.kind === 'hench') break;
            if (e.kind === 'heli') { say('bigKill', pick('heliDown', P.heliDown), 2.5, 0); shout('CHOPPER DOWN!', '#ffd23f', 1.2); break; }
            if (e.kind === 'truck') say('bigKill', pick('bigKill', P.bigKill), 2, 0);
            else sayFrom('kill', 2, 2.2);
            break;
          case 'near': t.near = 0; sayFrom('near', 1, 3.5); break;
          case 'rail': t.rail = 0; sayFrom('rail', 1, 5); break;
          case 'collateral': sayFrom('collateral', 2, 3); break;
          case 'playerHit':
            if (S.p.hp > 0 && S.p.hp < 35) sayFrom('lowHp', 2, 6); else if (S.p.hp > 0) sayFrom('hit', 1, 3.5);
            break;
          case 'pickup': sayFrom(e.kind === 'repair' ? 'pickupRepair' : 'pickupMissiles', 1, 2); break;
          case 'rev': sayFrom('rev', 2, 4); break;
          case 'turbo': t.turbo = 0; sayFrom('turbo', 1, 12); break;
          case 'empty': sayFrom('empty', 2, 5); break;
          case 'boost': if (e.lvl >= 2) sayFrom('boost', 1, 6); break;
          case 'zapWarn': insist('zapWarn', 2.6, 0.6); break;
          case 'zoneWarn': insist('zoneWarn', 2.6, 0.6); break;
          case 'ringWarn': insist('ringWarn', 2.6, 0.6); shout('SHOCK WAVE!', '#7ff7ff', 0.9); break;
          case 'shock': sayFrom('shock', 2, 3); break;
          case 'dodge': sayFrom('dodge', 2, 2.5); break;
          case 'bossPhase':
            if (S.level === 2) say('boss', e.n === 1 ? 'They\u2019re jumping off the top deck! Watch the road!' : 'He\u2019s emptying the whole truck! Shoot the carrier!', 3);
            else say('boss', e.n === 1 ? 'He’s overcharging! Double arcs! Watch both lanes!' : 'MELTDOWN! Everything’s live! Dodge and SHOOT!', 3);
            break;
          case 'chainWarn': insist('chainWarn', 2.6, 0.5); shout('CHAIN!', '#ff4fb8', 0.7); break;
          case 'chain': sayFrom(e.hit ? 'chainHit' : 'chainDodged', 2, 3); break;
          case 'hogJump': if (sayFrom('hogJump', 2, 6)) shout('INCOMING!', '#ff9e3d', 0.8); break;
          case 'brakeWarn': insist('brakeWarn', 2.6, 0.6); shout('BRAKES!', '#ff3355', 0.8); break;
          case 'missile': t.missile = 0; break;
          case 'driftStart': t.drift = 0; break;
          case 'land':
            if (D.gap) { D.gap = false; insist('gapLand', 2.7, 1.2); shout('LANDED!', '#7dff9b', 0.8); }
            else sayFrom('land', 1, 2);
            break;
        }
      }

      // a henchman still riding: keep telling him how to get rid of him
      if (S.p.latch && S.p.latch.length && S.phase === 'play') {
        if ((t.latch || 0) > 3.2) { t.latch = 0; sayFrom('henchHang', 2, 3); }
      }

      // the boss fight calls itself as the Juggernaut breaks up
      var b = S.boss;
      if (b && !b.dead && S.phase === 'play' && b.st !== 'enter') {
        var f = b.hp / b.max;
        if (S.level === 1 || S.level === 2) { D.boss.a = 1; } // the Alchemist's and the carrier's phases call themselves (bossPhase)
        if (f < 0.66 && !D.boss.a) { D.boss.a = 1; say('boss', 'He’s calling in the bikers! Hold your line!', 2); }
        else if (f < 0.45 && !D.boss.b) { D.boss.b = 1; say('boss', 'He’s cracking! Pour it on!', 2); }
        else if (f < 0.2 && !D.boss.c) { D.boss.c = 1; say('boss', 'Finish him! FINISH HIM!', 2); }
        if (S.bossT > (S.level === 2 ? 72 : 55) && !D.boss.d) { D.boss.d = 1; say('boss', 'He’s getting away! Faster! Take him out, NOW!', 3); }
      }

      for (i = D.queue.length - 1; i >= 0; i--) {
        D.queue[i].at -= dt;
        if (D.queue[i].at <= 0) { var q = D.queue[i]; D.queue.splice(i, 1); q.fn(); }
      }

      for (i = D.pend.length - 1; i >= 0; i--) {
        var pd = D.pend[i]; pd.ttl -= dt;
        if (pd.ttl <= 0 || say(pd.key, pd.text, pd.pri, 0)) D.pend.splice(i, 1);
      }

      // never quiet: once a line has had its moment, the next direction follows
      if (S.phase === 'play' && (!D.line || D.line.age > D.line.hold + 0.4)) ambient(S);
    };
    return D;
  }

  root.FT_DIRECTOR = { make: make };
})(typeof window !== 'undefined' ? window : this);
