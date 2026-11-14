/* FULL THROTTLE — an autopilot that drives the title screen's attract loop.
   Pure: reads the engine's state and returns an input, like a player would. */
(function (root) {
  'use strict';
  function make() {
    var missT = 0, wantX = 32, back = 0;
    return function (S, dt) {
      var E = root.FT_ENGINE, K = E.KIND, p = S.p, best = p.x, bestCost = 1e9, i, e;
      var ramp = null;
      for (i = 0; i < S.ents.length; i++) {
        e = S.ents[i];
        if (e.kind === 'ramp' && !e.used && e.y < p.y && e.y > p.y - 420) ramp = e;
      }
      for (var x = -118; x <= 118; x += 8) {
        var cost = Math.abs(x - p.x) * 0.06;
        for (i = 0; i < S.ents.length; i++) {
          e = S.ents[i];
          var k = K[e.kind];
          if (k.pickup) { if (e.y < p.y && e.y > p.y - 300 && Math.abs(e.x - x) < 20) cost -= 30; continue; }
          var ahead = p.y - e.y, closing = e.vy > -20;
          if (ahead < -60 || ahead > 330) continue;
          if (e.kind === 'ramp' && ramp === e && Math.abs(e.x - x) < 8) { cost -= 60; continue; }
          var reach = (e.w + p.w) / 2 + (k.civ ? 12 : 16);
          if (Math.abs(e.x - x) < reach && (closing || ahead < 70)) cost += 900 / Math.max(30, ahead + 40);
          if (k.foe && !k.big && e.kind !== 'moto' && Math.abs(e.x - x) < 14) cost -= 8; // line up shots
          if (e.kind === 'rammer' && (e.st === 'rev' || e.st === 'charge') && Math.abs(e.x - x) < 46) cost += 120;
        }
        for (i = 0; i < S.ebullets.length; i++) {
          var b = S.ebullets[i];
          if (b.vy <= 0 || b.y > p.y + 20) continue;
          var tt = (p.y - b.y) / b.vy, bx = b.x + b.vx * tt;
          if (tt < 1.2 && Math.abs(bx - x) < 22) cost += 60 / Math.max(0.15, tt);
        }
        // the Alchemist's electricity: stay out of a charging lane, off a lock-on, and in a shock wave's gap
        var hz = S.hz || [];
        for (i = 0; i < hz.length; i++) {
          var h = hz[i];
          if (h.k === 'lane' && Math.abs(x - h.x) < 32 + p.w / 2) cost += 500;
          else if (h.k === 'zone' && Math.abs(x - h.x) < h.r + p.w / 2 + 4 && Math.abs(p.y - h.y) < h.r + 24) cost += 400;
          else if (h.k === 'ring' && h.y < p.y && p.y - h.y < 420 && Math.abs(x - h.gx) > h.gw / 2 - p.w / 2 - 2) cost += 300;
        }
        // 1.3: a henchman dropping on a rope: keep out from under his landing mark
        for (i = 0; i < S.ents.length; i++) {
          e = S.ents[i];
          if (e.kind === 'hench' && e.st === 'drop' && Math.abs(x - e.tx) < 30) cost += 60;
          // 1.5: a biker flying off the carrier: out from under him. One beside you: ram him, unless
          // his chain is already whirling, then get a car's width away
          if (e.kind === 'hog') {
            if (e.st === 'fly' && Math.abs(x - e.tx) < 30) cost += 90;
            else if (Math.abs(e.y - p.y) < 50 && e.st !== 'fly') {
              if (e.st === 'wind' && e.t > 0.25) { if (Math.abs(x - e.x) < 40) cost += 300; }
              else if (Math.abs(x - e.x) < 10) cost -= 40;
            }
          }
        }
        if (Math.abs(x) > 128) cost += 6;
        if (cost < bestCost) { bestCost = cost; best = x; }
      }
      wantX = best;
      // a henchman on a door: swerve away, then slam that side into the wall
      var L = p.latch || [];
      if (L.length && bestCost < 200) {
        var sd = L[0].side;
        if (sd * p.x > 95 && Math.abs(p.vx) < 60 && !back) back = 0.35;
        if (back > 0) { back -= dt; wantX = -sd * 20; } else { back = 0; wantX = sd * 160; }
      }
      var dx = wantX - p.x;
      // the wheel now eases in, so lead the turn a little harder than before
      var steer = Math.abs(dx) < 4 ? 0 : Math.max(-1, Math.min(1, dx / 18));
      // shoot when the first thing in our column is a foe or a barrel, not a civilian
      var first = null, fy = -1e9;
      for (i = 0; i < S.ents.length; i++) {
        e = S.ents[i];
        if (K[e.kind].pickup || e.y > p.y - 20 || e.kind === 'ramp') continue;
        if (Math.abs(e.x - p.x) < e.w / 2 + 6 && e.y > fy) { fy = e.y; first = e; }
      }
      var fire = !!first && (K[first.kind].foe || first.kind === 'barrel');
      missT -= dt;
      var missile = false;
      if (missT <= 0 && p.missiles > 0) {
        for (i = 0; i < S.ents.length; i++) {
          e = S.ents[i];
          if ((e.kind === 'suv' || K[e.kind].big) && e.y > 0 && e.y < p.y - 60) { missile = true; missT = 0.9; break; }
        }
      }
      var turbo = (!!ramp && Math.abs(ramp.x - p.x) < 10 && p.turbo > 30) || (p.turbo > 95 && bestCost < 1);
      var throttle = bestCost > 40 ? -1 : p.y > 500 ? 0.4 : 0;
      return { steer: steer, throttle: throttle, fire: fire, missile: missile, turbo: turbo };
    };
  }
  root.FT_AUTOPILOT = { make: make };
})(typeof window !== 'undefined' ? window : this);
