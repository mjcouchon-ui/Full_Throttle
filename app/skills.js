/* FULL THROTTLE 1.8: skill points and the GARAGE's upgrades. The data below is the same file as the Unreal build's
   Content/Data/skills.json (copied verbatim): keep the two in sync. FT_SKILL is the arithmetic both builds share. */
window.FT_SKILLS = {
  "format": "full-throttle.skills/1",
  "about": "Skill points and upgrades for the KESTREL-88. The same file drives the Unreal build (Content/Data/skills.json) and the web app (app/skills.js). Points are earned when a level ends and spent on the GARAGE screen between levels. Every upgrade adds its effects once per rank. A stat of kind 'mul' is a multiplier (1 + the sum of its effects, never below 0.2); kind 'add' is a plain count. The coming skill tree adds entries to 'upgrades' with a 'branch', a 'tier' and 'requires' (other upgrades and the rank they need); the points, the save and the screen stay the same.",
  "points": {
    "wrap": 2,
    "escape": 1,
    "perRating": 1,
    "noRetakes": 1
  },
  "pointLabels": {
    "wrap": "THAT'S A WRAP",
    "escape": "THE TARGET GOT AWAY",
    "perRating": "DIRECTOR'S RATING",
    "noRetakes": "NO RETAKES"
  },
  "stats": {
    "steer": { "label": "steering response", "kind": "mul" },
    "driftCharge": { "label": "drift charge", "kind": "mul" },
    "turboBurn": { "label": "turbo burn", "kind": "mul" },
    "turboRegen": { "label": "turbo recharge", "kind": "mul" },
    "damageTaken": { "label": "damage taken", "kind": "mul" },
    "gunDamage": { "label": "gun damage", "kind": "mul" },
    "missiles": { "label": "missiles", "kind": "add" }
  },
  "upgrades": [
    {
      "id": "handling",
      "name": "HANDLING",
      "branch": "drive",
      "tier": 0,
      "color": "#36F2FF",
      "desc": "Quicker hands on the wheel and a drift that charges faster.",
      "maxRank": 5,
      "cost": [1, 1, 2, 2, 3],
      "requires": [],
      "effects": [
        { "stat": "steer", "perRank": 0.10 },
        { "stat": "driftCharge", "perRank": 0.12 }
      ]
    },
    {
      "id": "turbo",
      "name": "TURBO",
      "branch": "drive",
      "tier": 0,
      "color": "#FF4FA6",
      "desc": "A bigger burn from the same tank, and it refills faster.",
      "maxRank": 5,
      "cost": [1, 1, 2, 2, 3],
      "requires": [],
      "effects": [
        { "stat": "turboBurn", "perRank": -0.08 },
        { "stat": "turboRegen", "perRank": 0.12 }
      ]
    },
    {
      "id": "armor",
      "name": "ARMOR",
      "branch": "body",
      "tier": 0,
      "color": "#5BFF73",
      "desc": "Thicker plate: every hit takes less off the armor bar.",
      "maxRank": 5,
      "cost": [1, 1, 2, 2, 3],
      "requires": [],
      "effects": [
        { "stat": "damageTaken", "perRank": -0.07 }
      ]
    },
    {
      "id": "weapons",
      "name": "WEAPONS",
      "branch": "arms",
      "tier": 0,
      "color": "#FFD140",
      "desc": "Hotter rounds for the guns and one more missile in the pods.",
      "maxRank": 5,
      "cost": [1, 1, 2, 2, 3],
      "requires": [],
      "effects": [
        { "stat": "gunDamage", "perRank": 0.10 },
        { "stat": "missiles", "perRank": 1 }
      ]
    }
  ]
};

(function (root) {
  'use strict';
  var D = root.FT_SKILLS;
  function byId(id) { for (var i = 0; i < D.upgrades.length; i++) if (D.upgrades[i].id === id) return D.upgrades[i]; return null; }
  function rankOf(P, id) { return Math.max(0, Math.floor((P && P.ranks && P.ranks[id]) || 0)); }
  // a stat: kind 'mul' is 1 + the sum of its effects (never below 0.2), kind 'add' the plain sum
  function stats(P) {
    var o = {}, k;
    for (k in D.stats) o[k] = 0;
    D.upgrades.forEach(function (u) {
      var r = Math.min(rankOf(P, u.id), u.maxRank);
      (u.effects || []).forEach(function (e) { if (o[e.stat] !== undefined) o[e.stat] += e.perRank * r; });
    });
    for (k in D.stats) if (D.stats[k].kind === 'mul') o[k] = Math.max(0.2, 1 + o[k]);
    return o;
  }
  function locked(P, u) { // the first requirement not met, or null
    var q = u.requires || [];
    for (var i = 0; i < q.length; i++) if (rankOf(P, q[i].id) < q[i].rank) return q[i];
    return null;
  }
  function nextCost(P, u) { var r = rankOf(P, u.id); return r >= u.maxRank ? 0 : (u.cost[Math.min(r, u.cost.length - 1)] || 1); }
  function canBuy(P, u) { return !locked(P, u) && rankOf(P, u.id) < u.maxRank && P.points >= nextCost(P, u); }
  function buy(P, u) {
    if (!canBuy(P, u)) return false;
    P.points -= nextCost(P, u); P.ranks[u.id] = rankOf(P, u.id) + 1; return true;
  }
  function refund(P, u) {
    var r = rankOf(P, u.id); if (r <= 0) return false;
    // anything that needs this rank comes off first
    D.upgrades.forEach(function (o) { (o.requires || []).forEach(function (q) { if (q.id === u.id && q.rank >= r) while (rankOf(P, o.id) > 0) refund(P, o); }); });
    P.ranks[u.id] = r - 1; P.points += u.cost[Math.min(r - 1, u.cost.length - 1)] || 1; return true;
  }
  // one effect at a rank, as the screen says it: "+12% drift charge", "+1 missiles"
  function effectText(e, rank) {
    var st = D.stats[e.stat] || { label: e.stat, kind: 'mul' }, v = e.perRank * rank;
    if (st.kind === 'add') return (v >= 0 ? '+' : '') + Math.round(v) + ' ' + st.label;
    var pc = Math.round(v * 100);
    return (pc >= 0 ? '+' : '') + pc + '% ' + st.label;
  }
  // the points a finished level pays: [{key, label, n}], and the total
  function earn(outcome, ratingIdx, retakes) {
    var pt = D.points, L = D.pointLabels || {}, lines = [];
    if (outcome !== 'wrapped' && outcome !== 'escaped') return { total: 0, lines: lines };
    if (outcome === 'wrapped') lines.push({ key: 'wrap', label: L.wrap || 'WRAP', n: pt.wrap });
    else lines.push({ key: 'escape', label: L.escape || 'ESCAPED', n: pt.escape });
    if (ratingIdx > 0 && pt.perRating) lines.push({ key: 'perRating', label: L.perRating || 'RATING', n: pt.perRating * ratingIdx });
    if (!retakes && pt.noRetakes) lines.push({ key: 'noRetakes', label: L.noRetakes || 'NO RETAKES', n: pt.noRetakes });
    var t = 0; lines.forEach(function (l) { t += l.n; });
    return { total: t, lines: lines };
  }
  function fresh() { return { v: 1, points: 0, earned: 0, ranks: {} }; }
  function clean(P) { // a save read back: only known upgrades, ranks within their caps
    var o = fresh();
    if (!P || typeof P !== 'object') return o;
    o.points = Math.max(0, Math.floor(+P.points || 0)); o.earned = Math.max(0, Math.floor(+P.earned || 0));
    D.upgrades.forEach(function (u) { var r = rankOf(P, u.id); if (r) o.ranks[u.id] = Math.min(r, u.maxRank); });
    return o;
  }
  root.FT_SKILL = { data: D, byId: byId, rank: rankOf, stats: stats, locked: locked, nextCost: nextCost, canBuy: canBuy, buy: buy, refund: refund,
    effectText: effectText, earn: earn, fresh: fresh, clean: clean };
})(typeof window !== 'undefined' ? window : this);
