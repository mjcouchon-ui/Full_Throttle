/* FULL THROTTLE — the rules of the scene. Pure: no DOM, no canvas, no audio.
   FT_ENGINE.create(seed) -> state S; FT_ENGINE.step(S, input, dt) advances one fixed step.
   input: { steer -1..1, throttle -1..1 (up = +1), fire, missile (edge), turbo }.
   Everything the page should show or play is pushed to S.events and drained by the page. */
(function (root) {
  'use strict';
  var H = 600, LANES = [-96, -32, 32, 96], ROAD_HALF = 128, EDGE = 150, SCENE_LEN = 122500, PY = 470;
  var SCENE = null; // the level in use (LEVELS below): its number, title, place, scenes and script
  var KIND = {
    sedan:  { w: 28, h: 52, hp: 4, civ: true },
    hatch:  { w: 26, h: 44, hp: 3, civ: true },
    van:    { w: 32, h: 64, hp: 6, civ: true },
    bus:    { w: 36, h: 112, hp: 14, civ: true },
    ramp:   { w: 42, h: 124, hp: 9999, civ: true, ramp: true },
    moto:   { w: 14, h: 32, hp: 2, pts: 150, foe: true, name: 'BIKER' },
    rammer: { w: 30, h: 54, hp: 5, pts: 250, foe: true, name: 'RAMMER' },
    suv:    { w: 36, h: 64, hp: 10, pts: 400, foe: true, name: 'GUNSHIP SUV' },
    truck:  { w: 46, h: 104, hp: 26, pts: 900, foe: true, big: true, name: 'ARMORED TRUCK' },
    boss:   { w: 64, h: 150, hp: 120, pts: 3000, foe: true, big: true, name: 'THE JUGGERNAUT' },
    // 1.3: the Syndicate chopper hangs over the road and drops henchmen onto the KESTREL. Both fly:
    // nothing on the road bumps into them, but guns and missiles still reach them.
    heli:   { w: 40, h: 60, hp: 40, pts: 1000, foe: true, air: true, name: 'SYNDICATE CHOPPER' },
    hench:  { w: 12, h: 14, hp: 1, pts: 200, foe: true, air: true, name: 'HENCHMAN' },
    // 1.5: the SNAKE EYES, outlaw bikers on choppers (level 3). They pull up beside a door and swing a
    // chain; slam into them sideways and they're gone. The carrier boss rolls them off its tail ramp.
    hog:    { w: 16, h: 38, hp: 3, pts: 300, foe: true, name: 'SNAKE EYES BIKER' },
    barrel: { w: 18, h: 18, hp: 1, pts: 50, hazard: true },
    crate:  { w: 22, h: 22, pickup: 'missiles' },
    wrench: { w: 22, h: 22, pickup: 'repair' }
  };
  var RATINGS = [
    { key: 'bmovie', title: 'B-MOVIE', stars: 1, quip: 'Straight to video. The stunt budget went on catering.' },
    { key: 'hero', title: 'ACTION HERO', stars: 2, quip: 'Solid action. The trailer guy is warming up his voice.' },
    { key: 'blockbuster', title: 'BLOCKBUSTER', stars: 3, quip: 'Lines around the block. Sequel greenlit.' },
    { key: 'legend', title: 'LEGENDARY DIRECTOR', stars: 4, quip: 'They’ll teach this chase in film school.' }
  ];
  var RATING_AT = [0, 90000, 160000, 230000];

  function mulberry32(a) {
    return function () {
      a |= 0; a = a + 0x6D2B79F5 | 0;
      var t = Math.imul(a ^ a >>> 15, 1 | a);
      t = t + Math.imul(t ^ t >>> 7, 61 | t) ^ t;
      return ((t ^ t >>> 14) >>> 0) / 4294967296;
    };
  }
  function clamp(v, a, b) { return v < a ? a : v > b ? b : v; }

  // The track (track.js) bends, banks, climbs, loops, twists and stops for gaps. curveAt(d) is the
  // road's curvature at road distance d (engine units, d grows up the road; a point at screen-y y
  // sits at d = S.dist + H - y). + bends right, - bends left. The same road every take.
  var TR = root.FT_TRACK, curveAt = TR.curveAt, bankAt = TR.bankAt;
  function approach(v, t, d) { return v < t ? Math.min(t, v + d) : Math.max(t, v - d); }
  function sign(v) { return v < 0 ? -1 : 1; }

  // Each level is shot as a run of scenes. Each starts where its beat sits on the road; a wreck sends
  // you back to the top of the scene you were in (a retake), with the box office as it stood then.
  // intro: the director's two lines for the scene (the call, then a note on a retake).
  var LEVELS = [
    { n: 1, track: 1, num: '01', zone: 'BAYSHORE', title: 'THE HIGHWAY ASSASSINATION', place: 'ROUTE 9 COASTAL HIGHWAY  ·  03:00 HRS',
      len: 122500, boss: 'THE JUGGERNAUT', ratingAt: [0, 90000, 160000, 230000],
      scenes: [
        { d: 0, name: 'THE GETAWAY', intro: ['Scene one. You have the codebook. Drive like they want it back!', 'Weave that traffic. Make it look dangerous!'] },
        { d: 3300, name: 'THE TAIL', intro: ['Bikers on your tail, and the road goes up and over! Take them out!', 'Guns up over those humps. Don’t let them close in!'] },
        { d: 7200, name: 'THE AMBUSH', intro: ['Rammers on the banked turn! Ride the wall and make them miss!', 'Up on the bank! Dodge the rammers!'] },
        { d: 9200, name: 'THE JUMP', intro: ['Car carrier, then the road just ENDS! Turbo and fly the gap!', 'Ramp, then the gap, then the loop. Give me air!'] },
        { d: 16400, name: 'THE CONVOY', intro: ['Armored truck! Missiles, then hang on for the corkscrew!', 'That truck is the shot. Crack it before the corkscrew!'] },
        { d: 23300, name: 'CROSSFIRE', intro: ['Gunships! Shoot back, then fly the big gap!', 'Crossfire, then the big jump. Keep moving!'] },
        { d: 28300, name: 'THE WOLF PACK', intro: ['Three rammers and another loop. Make me gasp!', 'The wolf pack. Thread them, then loop it!'] },
        { d: 34500, name: 'THE PIER', intro: ['The pier! Clear the boardwalk, then fly off the end!', 'Off the end of the pier. Turbo and FLY!'] },
        { d: 45500, name: 'NEON MILE', intro: ['Neon Mile! Every sign in town is on. Make it glow!', 'Loop it, roll it, light them up!'] },
        { d: 56500, name: 'THE MARINA', intro: ['The marina! Armor on the docks and a gap over the slips!', 'Crack that truck, then jump the boats!'] },
        { d: 67500, name: 'THE CLIFF ROAD', intro: ['The cliff road! Bikers on every bend. Hold your line!', 'Esses, a corkscrew and a loop. Don’t look down!'] },
        { d: 78500, name: 'THE LIGHTHOUSE', intro: ['Gunships on the point! Then the biggest gap on the coast!', 'Ride the wall round the lighthouse and FLY!'] },
        { d: 89500, name: 'THE LAST MILE', intro: ['The last mile! Everything they’ve got. Give me everything you’ve got!', 'Last loop before the big man. Make it count!'] },
        { d: 100500, name: 'THE CAUSEWAY', intro: ['The causeway! The drawbridge is up. Fly it!', 'Over the bridge and round the loop. Go!'] },
        { d: 111500, name: 'SUNSET STRIP', intro: ['Sunset Strip! The whole town is watching. Show off!', 'Ride the wall, roll it, loop it!'] },
        { d: 122500, name: 'THE FINALE', intro: ['There he is. The Juggernaut. Take him DOWN!', 'Final shot. Everything you’ve got on the Juggernaut!'] }
      ],
      script: [
        { d: 0, scene: 0 },
        { d: 200, cap: 'AGENT NOVA KANE HAS THE CODEBOOK. THE SYNDICATE WANTS IT BACK.' },
        { d: 3300, scene: 1, spawn: ['moto', 'moto'], cap: 'BIKERS. SYNDICATE COLOURS.' },
        { d: 5200, spawn: ['moto', 'rammer'] },
        { d: 7200, scene: 2, spawn: ['rammer', 'rammer'], cap: 'THEY MEAN TO RUN YOU OFF THE ROAD.' },
        { d: 9200, scene: 3, spawn: ['ramp'], cap: 'CAR CARRIER AHEAD. TURBO UP THE RAMP!' },
        { d: 10400, spawn: ['suv'] },
        { d: 11800, spawn: ['crate'] },
        { d: 12600, spawn: ['moto', 'moto', 'moto'] },
        { d: 14400, spawn: ['suv', 'rammer'] },
        { d: 16400, scene: 4, spawn: ['truck'], cap: 'ARMORED TRUCK. MISSILES WILL CRACK IT.' },
        { d: 17200, spawn: ['ramp'] },
        { d: 19300, spawn: ['suv', 'moto'] },
        { d: 21300, spawn: ['rammer', 'rammer', 'moto'] },
        { d: 23300, scene: 5, spawn: ['suv', 'suv'] },
        { d: 24500, spawn: ['wrench', 'crate'] },
        { d: 25700, spawn: ['truck', 'moto', 'moto'] },
        { d: 26800, spawn: ['heli'], cap: 'SYNDICATE CHOPPER. HENCHMEN DROPPING IN.' },
        { d: 28300, scene: 6, spawn: ['rammer', 'rammer', 'rammer'] },
        { d: 29700, spawn: ['moto', 'moto'] },
        { d: 31500, spawn: ['ramp', 'crate'] },
        { d: 32700, spawn: ['suv', 'moto'] },
        // (1.1) the long cut
        { d: 34500, scene: 7, spawn: ['moto', 'moto', 'rammer'], cap: 'THE PIER. THEY’VE BLOCKED THE BOARDWALK.' },
        { d: 36200, spawn: ['suv'] },
        { d: 37600, spawn: ['crate'] },
        { d: 38500, spawn: ['heli'], cap: 'CHOPPER OVER THE PIER.' },
        { d: 41100, spawn: ['rammer', 'rammer'] },
        { d: 43000, spawn: ['truck'] },
        { d: 44600, spawn: ['wrench'] },
        { d: 45500, scene: 8, spawn: ['suv', 'moto'], cap: 'NEON MILE. EVERY SIGN IN TOWN IS ON.' },
        { d: 46800, spawn: ['ramp'] },
        { d: 48500, spawn: ['rammer', 'moto', 'moto'] },
        { d: 50000, spawn: ['heli'] },
        { d: 51800, spawn: ['suv', 'suv'] },
        { d: 53600, spawn: ['crate', 'wrench'] },
        { d: 55000, spawn: ['moto', 'moto', 'moto'] },
        { d: 56500, scene: 9, spawn: ['truck', 'moto'], cap: 'THE MARINA. ARMOR ON THE DOCKS.' },
        { d: 58300, spawn: ['rammer', 'rammer'] },
        { d: 60500, spawn: ['crate'] },
        { d: 61500, spawn: ['heli'] },
        { d: 62500, spawn: ['suv', 'moto'] },
        { d: 64000, spawn: ['ramp'] },
        { d: 65500, spawn: ['rammer', 'suv'] },
        { d: 67500, scene: 10, spawn: ['moto', 'moto', 'moto', 'moto'], cap: 'THE CLIFF ROAD. BIKERS ON EVERY BEND.' },
        { d: 69500, spawn: ['suv'] },
        { d: 70500, spawn: ['heli'], cap: 'A CHOPPER ON THE CLIFFS. ROLL THEM OFF IN THE CORKSCREW.' },
        { d: 71500, spawn: ['rammer', 'rammer'] },
        { d: 73000, spawn: ['wrench', 'crate'] },
        { d: 75500, spawn: ['truck'] },
        { d: 77300, spawn: ['moto', 'moto'] },
        { d: 78500, scene: 11, spawn: ['suv', 'suv', 'moto'], cap: 'THE LIGHTHOUSE. GUNSHIPS ON THE POINT.' },
        { d: 80100, spawn: ['rammer', 'rammer', 'rammer'] },
        { d: 82300, spawn: ['crate'] },
        { d: 84800, spawn: ['truck', 'moto'] },
        { d: 86500, spawn: ['ramp', 'wrench'] },
        { d: 88000, spawn: ['suv', 'rammer'] },
        { d: 89500, scene: 12, spawn: ['rammer', 'rammer', 'moto', 'moto'], cap: 'THE LAST MILE. EVERYTHING THEY’VE GOT.' },
        { d: 91500, spawn: ['suv', 'suv'] },
        { d: 93000, spawn: ['crate', 'wrench'] },
        { d: 93800, spawn: ['heli'] },
        { d: 95500, spawn: ['truck', 'rammer'] },
        { d: 97500, spawn: ['moto', 'moto', 'moto'] },
        { d: 99000, spawn: ['suv', 'crate'] },
        { d: 100500, scene: 13, spawn: ['suv', 'moto', 'moto'], cap: 'THE CAUSEWAY. THE DRAWBRIDGE IS UP.' },
        { d: 102300, spawn: ['ramp'] },
        { d: 104200, spawn: ['rammer', 'rammer'] },
        { d: 106000, spawn: ['crate', 'wrench'] },
        { d: 108500, spawn: ['truck', 'moto'] },
        { d: 110300, spawn: ['suv'] },
        { d: 111500, scene: 14, spawn: ['rammer', 'rammer', 'rammer'], cap: 'SUNSET STRIP. THE WHOLE TOWN IS WATCHING.' },
        { d: 113800, spawn: ['moto', 'moto', 'suv'] },
        { d: 115000, spawn: ['heli'] },
        { d: 116000, spawn: ['crate'] },
        { d: 117500, spawn: ['truck', 'rammer'] },
        { d: 119500, spawn: ['wrench', 'crate'] },
        { d: 120800, spawn: ['suv', 'moto', 'moto'] },
        { d: 122500, scene: 15, spawn: ['boss'], cap: 'THE JUGGERNAUT. THE MAN WHO ORDERED THE HIT.' }
      ] },
    { n: 2, track: 2, num: '02', zone: 'CHEMICAL PLANT', title: 'THE FORMULA', place: 'VOLTA CHEMICAL WORKS  ·  23:00 HRS',
      len: 69000, boss: 'THE ALCHEMIST', ratingAt: [0, 50000, 90000, 130000],
      scenes: [
        { d: 0, name: 'THE FACTORY GATE', intro: ['Scene two. The chemical plant. Get the formula and get OUT!', 'Through the gate and into the tubes!'] },
        { d: 9000, name: 'THE TUBES', intro: ['The tubes! The road goes all the way round. Keep driving!', 'Twice round the pipe. Don’t lift!'] },
        { d: 19000, name: 'SPIN CYCLE', intro: ['Up the helix, round the loop, down again! Spin it!', 'The spin cycle. Ride the bank!'] },
        { d: 29000, name: 'THE REACTOR', intro: ['Armored tanker on the line! Crack it, then fly the pond!', 'Missiles on the truck, then the tube!'] },
        { d: 39000, name: 'PIPE DREAMS', intro: ['Two pipes and a loop. The crew built this for you!', 'Round and round. Keep shooting upside down!'] },
        { d: 49000, name: 'MEGA VAT', intro: ['Esses round the vats, then jump the big one. Don’t fall in!', 'Over the Mega Vat. Turbo off the lip!'] },
        { d: 59000, name: 'THE TOXIC RUN', intro: ['Three times round the big pipe! Make me dizzy!', 'The toxic run. Hold it together!'] },
        { d: 69000, name: 'THE FINALE', intro: ['The Alchemist’s tanker! Blow it before he gets away!', 'Final shot. Everything on the tanker!'] }
      ],
      script: [
        { d: 0, scene: 0 },
        { d: 200, cap: 'THE SYNDICATE’S LAB. THE FORMULA IS INSIDE.' },
        { d: 1500, spawn: ['moto', 'moto'] },
        { d: 3200, spawn: ['rammer'] },
        { d: 4600, spawn: ['suv'] },
        { d: 7800, spawn: ['crate'] },
        { d: 9000, scene: 1, spawn: ['moto', 'moto', 'rammer'], cap: 'THE TUBES. STAY ON THE ROAD WHEN IT GOES ROUND.' },
        { d: 11000, spawn: ['suv'] },
        { d: 11500, spawn: ['heli'], cap: 'CHOPPER IN THE PLANT. THE TUBES WILL SHAKE THEM OFF.' },
        { d: 15000, spawn: ['rammer', 'rammer'] },
        { d: 16500, spawn: ['crate', 'wrench'] },
        { d: 17500, spawn: ['suv', 'moto'] },
        { d: 19000, scene: 2, spawn: ['rammer', 'rammer'], cap: 'SPIN CYCLE. UP THE HELIX, ROUND THE LOOP.' },
        { d: 21000, spawn: ['moto', 'moto'] },
        { d: 22500, spawn: ['heli'] },
        { d: 25500, spawn: ['suv', 'moto'] },
        { d: 27500, spawn: ['crate'] },
        { d: 29000, scene: 3, spawn: ['truck'], cap: 'THE REACTOR. ARMORED TANKERS ON THE LINE.' },
        { d: 30500, spawn: ['ramp'] },
        { d: 32000, spawn: ['heli'] },
        { d: 33500, spawn: ['suv', 'suv'] },
        { d: 35300, spawn: ['wrench', 'crate'] },
        { d: 36500, spawn: ['rammer', 'moto', 'moto'] },
        { d: 39000, scene: 4, spawn: ['moto', 'moto', 'moto'], cap: 'PIPE DREAMS. TWO TUBES AND A LOOP.' },
        { d: 43000, spawn: ['suv', 'rammer'] },
        { d: 44500, spawn: ['heli'] },
        { d: 46500, spawn: ['crate'] },
        { d: 47500, spawn: ['truck'] },
        { d: 49000, scene: 5, spawn: ['rammer', 'rammer', 'rammer'], cap: 'THE MEGA VAT. DON’T FALL IN.' },
        { d: 51000, spawn: ['suv', 'moto'] },
        { d: 53000, spawn: ['ramp', 'crate'] },
        { d: 56500, spawn: ['wrench'] },
        { d: 57400, spawn: ['suv', 'suv'] },
        { d: 59000, scene: 6, spawn: ['truck', 'moto', 'moto'], cap: 'THE TOXIC RUN. THREE TIMES ROUND THE BIG PIPE.' },
        { d: 61000, spawn: ['heli'] },
        { d: 64000, spawn: ['rammer', 'suv'] },
        { d: 66000, spawn: ['crate', 'wrench'] },
        { d: 67500, spawn: ['moto', 'moto', 'rammer'] },
        { d: 69000, scene: 7, spawn: ['boss'], cap: 'THE ALCHEMIST. HE BREWED THE FORMULA. END HIM.' }
      ] },
    // 1.5: level 3, the open highway across the Nevada desert at dusk, Las Vegas lit up dead ahead
    { n: 3, track: 3, num: '03', zone: 'NEVADA DESERT', title: 'ALL IN', place: 'HIGHWAY 93 TO LAS VEGAS  ·  19:30 HRS',
      len: 78000, boss: 'THE BIG DEALER', bossHp: 200, escapeT: 90, ratingAt: [0, 80000, 150000, 210000],
      scenes: [
        { d: 0, name: 'THE STATE LINE', intro: ['Scene three. The open road to Vegas. Floor it!', 'Nevada, baby. Make it look like a postcard!'] },
        { d: 8500, name: 'THE DRY WASH', intro: ['The bridge is out over the wash! Turbo and FLY!', 'Up the ramp and over the wash!'] },
        { d: 17700, name: 'SNAKE EYES', intro: ['The Snake Eyes! Bikers on your doors. Slam them off the road!', 'Ram those choppers! Don\u2019t let them swing!'] },
        { d: 27900, name: 'JOSHUA FLATS', intro: ['Joshua Flats! Gunships in the trees. Keep it moving!', 'Through the trees and over the gap!'] },
        { d: 37500, name: 'RED ROCK PASS', intro: ['Red Rock Pass! Ride the canyon wall and roll it!', 'Wall, corkscrew, wall. Hold it together!'] },
        { d: 46000, name: 'LAST CHANCE GAS', intro: ['Last chance gas! Chopper over the pumps, then the big wash!', 'Blow the chopper, then fly the wash!'] },
        { d: 56500, name: 'JACKPOT JUNCTION', intro: ['Jackpot Junction! Loop it, roll it, let it ride!', 'Double or nothing. Loop and corkscrew!'] },
        { d: 66000, name: 'THE CITY LIGHTS', intro: ['There\u2019s Vegas! The whole Strip is lit up for us. Give me a show!', 'Neon on the paint. Loop it under the lights!'] },
        { d: 78000, name: 'THE FINALE', intro: ['The Big Dealer\u2019s carrier! Every Snake Eyes in Nevada is on that truck. Take it DOWN!', 'Final shot. Shoot the truck, slam the bikes!'] }
      ],
      script: [
        { d: 0, scene: 0 },
        { d: 200, cap: 'THE SYNDICATE\u2019S BUYER IS IN VEGAS. GET THERE FIRST.' },
        { d: 1500, spawn: ['moto', 'moto'] },
        { d: 3200, spawn: ['rammer'] },
        { d: 4800, spawn: ['suv'] },
        { d: 6500, spawn: ['crate'] },
        { d: 7300, spawn: ['rammer', 'moto'] },
        { d: 8500, scene: 1, spawn: ['moto', 'moto'], cap: 'THE BRIDGE OVER THE WASH IS OUT. TURBO UP THE LIP!' },
        { d: 10500, spawn: ['suv'] },
        { d: 13000, spawn: ['heli'], cap: 'CHOPPER OVER THE DESERT.' },
        { d: 14500, spawn: ['rammer', 'rammer'] },
        { d: 16000, spawn: ['crate', 'wrench'] },
        { d: 17700, scene: 2, spawn: ['hog', 'hog'], cap: 'THE SNAKE EYES. SLAM THEM OFF THEIR CHOPPERS.' },
        { d: 19500, spawn: ['hog', 'hog', 'hog'] },
        { d: 21500, spawn: ['suv'] },
        { d: 23000, spawn: ['hog', 'hog'] },
        { d: 25000, spawn: ['wrench'] },
        { d: 26200, spawn: ['hog', 'rammer'] },
        { d: 27900, scene: 3, spawn: ['suv', 'suv'], cap: 'JOSHUA FLATS. GUNSHIPS IN THE TREES.' },
        { d: 29500, spawn: ['hog', 'hog'] },
        { d: 31000, spawn: ['crate'] },
        { d: 32500, spawn: ['truck'] },
        { d: 35800, spawn: ['hog', 'hog', 'moto'] },
        { d: 37000, spawn: ['wrench'] },
        { d: 37500, scene: 4, spawn: ['rammer', 'rammer'], cap: 'RED ROCK PASS. RIDE THE CANYON WALL.' },
        { d: 39000, spawn: ['hog', 'hog'] },
        { d: 40500, spawn: ['heli'] },
        { d: 42500, spawn: ['suv', 'hog'] },
        { d: 44500, spawn: ['crate', 'wrench'] },
        { d: 46000, scene: 5, spawn: ['heli'], cap: 'LAST CHANCE GAS. A CHOPPER OVER THE PUMPS.' },
        { d: 47500, spawn: ['hog', 'hog', 'hog'] },
        { d: 49000, spawn: ['ramp'] },
        { d: 50200, spawn: ['truck'] },
        { d: 55000, spawn: ['suv', 'rammer'] },
        { d: 56000, spawn: ['crate'] },
        { d: 56500, scene: 6, spawn: ['rammer', 'rammer', 'rammer'], cap: 'JACKPOT JUNCTION. LET IT RIDE.' },
        { d: 58000, spawn: ['hog', 'hog'] },
        { d: 60000, spawn: ['heli'] },
        { d: 62000, spawn: ['suv', 'hog', 'hog'] },
        { d: 64500, spawn: ['wrench', 'crate'] },
        { d: 65500, spawn: ['moto', 'moto'] },
        { d: 66000, scene: 7, spawn: ['hog', 'hog', 'hog', 'hog'], cap: 'LAS VEGAS. THE WHOLE STRIP IS LIT UP.' },
        { d: 68000, spawn: ['truck', 'moto'] },
        { d: 71800, spawn: ['suv'] },
        { d: 73500, spawn: ['heli'] },
        { d: 75000, spawn: ['hog', 'hog'] },
        { d: 76500, spawn: ['crate', 'wrench'] },
        { d: 78000, scene: 8, spawn: ['boss'], cap: 'THE BIG DEALER. HIS CARRIER IS LOADED WITH SNAKE EYES.' }
      ] }
  ];
  var REELS = 3; // retakes a run can spend before the studio runs out of film
  var LV = LEVELS[0], SCENES = LV.scenes;
  function script() { return LV.script.map(function (o) { return o; }); }
  // pick level i (0, 1 or 2): the track, the scenes, the boss's name and the rating bars all follow
  function useLevel(i) {
    LV = LEVELS[i] || LEVELS[0];
    TR.use(LV.track);
    SCENES = LV.scenes; SCENE_LEN = LV.len; RATING_AT = LV.ratingAt;
    SCENE = { num: LV.num, title: LV.title, place: LV.place, zone: LV.zone, level: LV.n };
    KIND.boss.name = LV.boss; KIND.boss.hp = LV.bossHp || 120;
    var A = root.FT_ENGINE;
    if (A) { A.SCENES = SCENES; A.SCENE = SCENE; A.SCENE_LEN = SCENE_LEN; A.RATING_AT = RATING_AT; A.FEATS = TR.FEATS; A.level = LEVELS.indexOf(LV); }
    return LV;
  }
  function copy(o) { var r = {}; for (var k in o) r[k] = o[k]; return r; }

  // the top of a scene: where a retake starts from
  function checkpoint(S, si, n) {
    var p = S.p;
    S.cp = { scene: n, si: si, d: SCENES[n].d, score: S.score, tally: copy(S.tally), counts: copy(S.counts),
      hp: p.hp, missiles: p.missiles };
  }
  function enterScene(S, n, si) {
    var again = S.cp && S.cp.scene === n;
    if (!again) S.sceneTake = 1;
    S.scene = n; S.beat = SCENES[n].name;
    checkpoint(S, si, n);
    ev(S, { t: 'scene', n: n, name: SCENES[n].name, take: S.sceneTake, retake: again, intro: SCENES[n].intro });
    ev(S, { t: 'beat', name: SCENES[n].name });
  }

  // CUT, again: back to the top of the scene. The page calls this once S.phase is 'retake'.
  function retake(S) {
    var cp = S.cp, p = S.p;
    S.retakes++; S.sceneTake++;
    S.ents = []; S.bullets = []; S.ebullets = []; S.missiles = []; S.boss = null; S.bossT = 0;
    S.si = cp.si; S.dist = Math.max(0, cp.d - 1); S.speed = 380;
    S.score = cp.score; S.tally = copy(cp.tally); S.counts = copy(cp.counts);
    p.x = 32; p.y = PY; p.vx = 0; p.vy = 0; p.air = 0; p.grace = 0; p.inv = 1.2; p.gapJ = null; p.liftH = 0;
    S.pd = null; S.inLoop = null; S.inTwist = null; S.inPipe = null; S.inWall = null; S.inverted = false;
    p.hp = Math.max(cp.hp, 75); p.missiles = Math.max(cp.missiles, 3); p.turbo = 100; p.turboOn = false; p.turboLock = false;
    p.steerIn = 0; p.yaw = 0; p.yawT = 0; p.roll = 0; p.drift = null; p.boostT = 0; p.kick = 0; p.squash = 0;
    p.latch = []; p.parts = partsFor(p.hp);
    S.hz = [];
    S.phase = 'play'; S.outcome = null; S.cutAgain = false; S.wrapT = 0;
    S.combo = 1; S.comboT = 0; S.kills = []; S.timeScale = 1; S.slowT = 0; S.slowScale = 1; S.freezeT = 0; S.slowCd = 0;
    S.trauma = 0; S.caption = null; S.capT = 0; S.civT = 1.2; S.lastNear = { t: -9, side: 0 };
    ev(S, { t: 'retake', n: cp.scene, take: S.sceneTake });
  }

  function create(seed) {
    seed = (seed >>> 0) || 1;
    return {
      seed: seed, level: LEVELS.indexOf(LV), rnd: mulberry32(seed), t: 0, dist: 0, speed: 380, phase: 'play', events: [],
      ents: [], bullets: [], ebullets: [], missiles: [], idc: 1,
      p: { x: 32, y: PY, vx: 0, vy: 0, w: 28, h: 54, hp: 100, maxHp: 100, missiles: 6, turbo: 100, turboOn: false,
        turboLock: false, inv: 0, air: 0, airMax: 0, fireCd: 0, missCd: 0, side: 0, railCd: 0, grace: 0, gapJ: null, liftH: 0,
        steerIn: 0, yaw: 0, yawT: 0, roll: 0, drift: null, boostT: 0, boostLvl: 0, kick: 0, squash: 0,
        latch: [], parts: 0 },
      score: 0, tally: { destruction: 0, near: 0, stunts: 0, collateral: 0, armor: 0 },
      counts: { kills: 0, near: 0, stunts: 0, civ: 0, jumps: 0, hits: 0, loops: 0, gaps: 0, twists: 0, pipes: 0 },
      combo: 1, comboT: 0, kills: [],
      timeScale: 1, slowT: 0, slowScale: 1, freezeT: 0, slowCd: 0, trauma: 0,
      script: script(), si: 0, civT: 1.2, boss: null, bossT: 0, wrapT: 0, outcome: null, hz: [],
      lastNear: { t: -9, side: 0 }, progress: 0, beat: SCENES[0].name, caption: null, capT: 0,
      take: 1, rating: null, reduced: false,
      scene: 0, sceneTake: 1, cp: null, retakes: 0, reels: REELS, cutAgain: false,
      pd: null, inLoop: null, inTwist: null, inPipe: null, inWall: null, inverted: false
    };
  }

  function ev(S, o) { S.events.push(o); }
  function popup(S, x, y, text, color, big) { ev(S, { t: 'popup', x: x, y: y, text: text, color: color || '#ffd23f', big: !!big }); }
  function shake(S, a) { S.trauma = Math.min(1, S.trauma + a); }
  function slow(S, dur, scale) {
    if (S.reduced) { scale = Math.max(scale, 0.6); dur *= 0.6; }
    S.slowScale = S.slowT > 0 ? Math.min(S.slowScale, scale) : scale;
    S.slowT = Math.max(S.slowT, dur);
    ev(S, { t: 'slowmo' });
  }
  function freeze(S, dur) { S.freezeT = Math.max(S.freezeT, dur); ev(S, { t: 'freeze' }); }
  function caption(S, text) { S.caption = text; S.capT = 4.2; }

  function addScore(S, pts, bucket) {
    S.score += pts; S.tally[bucket] += pts;
  }
  function stunt(S, name, pts, x, y) {
    pts = pts * S.combo;
    addScore(S, pts, 'stunts'); S.counts.stunts++;
    popup(S, x, y, name + '  +' + pts, '#7ff7ff', true);
    ev(S, { t: 'stunt', name: name });
    bumpCombo(S);
  }
  function bumpCombo(S) { S.comboT = 3; }

  function spawn(S, kind, i) {
    var k = KIND[kind], r = S.rnd, lane = LANES[Math.floor(r() * 4)];
    var e = { id: S.idc++, kind: kind, x: lane, y: -k.h / 2 - 30 - (i || 0) * 90, w: k.w, h: k.h, hp: k.hp, max: k.hp,
      vy: 0, v: 0, age: 0, st: 'enter', t: 0, flash: 0, rel: 0, touched: false, nearDone: false, bumpCd: 0,
      ty: 0, cx: lane, ph: r() * 6.28, col: Math.floor(r() * 8), fireT: 1 + r(), dropT: 2, used: false, lean: 0 };
    if (k.civ) {
      e.v = kind === 'ramp' ? 235 : kind === 'bus' ? 190 : 200 + r() * 70;
      if (kind === 'ramp') e.x = LANES[1 + Math.floor(r() * 2)];
    } else if (k.pickup || k.hazard) {
      e.v = 0;
    } else if (kind === 'heli') {
      e.x = clamp(S.p.x + (r() < 0.5 ? -1 : 1) * (60 + r() * 40), -110, 110); e.y = -150; e.ty = 235 + r() * 40;
      e.alt = 200; e.riders = 2; e.dropT = 1.3; e.cx = r() < 0.5 ? -30 : 30; e.vx = 0;
    } else if (kind === 'hog') {
      // a chopper from up the road: it rides back to you and pulls up beside a door
      e.ty = 150 + r() * 120; e.x = clamp(S.p.x + (r() - 0.5) * 160, -ROAD_HALF + 16, ROAD_HALF - 16);
      e.side = r() < 0.5 ? -1 : 1; e.cd = 0.8 + r(); e.col = Math.floor(r() * 2); e.alt = 0; e.vx = 0;
    } else if (kind === 'hench') {
      e.alt = 90; e.alt0 = 90; e.T = 1.25; e.tx = S.p.x; e.ty2 = S.p.y;
    } else if (kind === 'boss') {
      e.x = 0; e.y = -k.h / 2 - 20; e.ty = LV.n === 3 ? 170 : 128; // the carrier rides closer: its bikers come off the back at you
      S.boss = e; S.bossT = 0;
      ev(S, { t: 'bossIntro' }); shake(S, 0.4);
    } else {
      e.ty = kind === 'suv' ? 90 + r() * 70 : kind === 'truck' ? 110 : kind === 'moto' ? 170 + r() * 130 : 120 + r() * 70;
      e.x = clamp(S.p.x + (r() - 0.5) * 160, -ROAD_HALF + 20, ROAD_HALF - 20);
      if (kind === 'truck') e.x = LANES[1 + Math.floor(r() * 2)];
    }
    if (!k.civ && !k.pickup && !k.hazard && kind !== 'boss') ev(S, { t: 'enemyIn', kind: kind });
    // keep spawns from landing on top of one another
    for (var n = 0; n < S.ents.length && !k.air; n++) {
      var o = S.ents[n];
      if (KIND[o.kind].air) continue;
      if (Math.abs(o.x - e.x) < (o.w + e.w) / 2 + 4 && Math.abs(o.y - e.y) < (o.h + e.h) / 2 + 10) e.y = o.y - (o.h + e.h) / 2 - 14;
    }
    S.ents.push(e);
    return e;
  }

  function hurtPlayer(S, d, x, y) {
    var p = S.p;
    if (p.inv > 0 || p.air > 0 || S.phase !== 'play') return;
    p.hp = Math.max(0, p.hp - d); p.inv = 0.6; S.counts.hits++;
    loosen(S);
    // 1.7: a hit knocks the drift's charge back about one colour instead of killing the slide outright
    // (in a gunfight a single bullet used to throw the whole slide away)
    if (p.drift) {
      var dr = p.drift;
      dr.charge = Math.max(0, dr.charge - 0.6 - d * 0.02);
      dr.lvl = dr.charge >= DRIFT_AT[2] ? 3 : dr.charge >= DRIFT_AT[1] ? 2 : dr.charge >= DRIFT_AT[0] ? 1 : 0;
    }
    shake(S, 0.25 + d / 60);
    ev(S, { t: 'playerHit', x: x, y: y, d: d });
    if (p.hp <= 0) {
      S.phase = 'wreck'; S.wrapT = 2.8;
      if (S.retakes < S.reels) { S.cutAgain = true; ev(S, { t: 'cutAgain', n: S.scene }); }
      else { S.outcome = 'wrecked'; ev(S, { t: 'outOfFilm' }); }
      ev(S, { t: 'explode', x: p.x, y: p.y, size: 2.2 });
      ev(S, { t: 'wreck' });
      for (var li = 0; li < p.latch.length; li++) ev(S, { t: 'henchOff', side: p.latch[li].side, how: 'wreck', x: p.x, y: p.y });
      p.latch.length = 0;
      freeze(S, 0.14); slow(S, 1.8, 0.3); shake(S, 1);
    }
  }

  // 1.3 damage: the KESTREL sheds a part each time the armor drops past one of these marks (a mirror,
  // a wing endplate, a hubcap...). A repair puts the parts back on.
  var PART_AT = [88, 76, 64, 52, 40, 28, 16];
  function partsFor(hp) { var n = 0; for (var i = 0; i < PART_AT.length; i++) if (hp < PART_AT[i]) n++; return n; }
  function loosen(S) {
    var p = S.p, n = partsFor(p.hp);
    while (p.parts < n) { ev(S, { t: 'partOff', n: p.parts, x: p.x, y: p.y }); p.parts++; }
    if (n < p.parts) p.parts = n;
  }

  function damage(S, e, d, src, x, y) {
    if (e.dead) return;
    var k = KIND[e.kind];
    if (k.pickup) return;
    if (k.ramp) { ev(S, { t: 'spark', x: x, y: y }); return; }
    e.hp -= d; e.flash = 0.07;
    ev(S, { t: 'hit', x: x === undefined ? e.x : x, y: y === undefined ? e.y : y, big: !!k.big, kind: e.kind, d: d });
    if (e.hp <= 0) kill(S, e, src);
  }

  function kill(S, e, src) {
    var k = KIND[e.kind], p = S.p;
    e.dead = true;
    var size = e.kind === 'boss' ? 3 : e.kind === 'heli' ? 2.4 : k.big ? 2 : e.kind === 'moto' || e.kind === 'hog' || e.kind === 'barrel' ? 0.8 : e.kind === 'bus' ? 1.8 : 1.2;
    if (e.kind === 'hench') { size = 0; ev(S, { t: 'henchOff', how: 'shot', x: e.x, y: e.y, alt: e.alt }); }
    else ev(S, { t: 'explode', x: e.x, y: e.y, size: size, kind: e.kind, alt: e.alt || 0 });
    if (e.kind === 'heli') {
      // the chopper comes down burning and blows again where it hits the road
      ev(S, { t: 'heliDown', x: e.x, y: e.y, alt: e.alt, lean: e.lean || 0, vx: e.vx || 0 });
      ev(S, { t: 'explode', x: e.x + (e.vx || 0) * 0.3, y: e.y + 70, size: 2, kind: 'crash', delay: 0.7 });
    }
    shake(S, 0.2 + size * 0.12);
    if (k.civ) {
      if (src === 'player' || src === 'gun' || src === 'missile' || src === 'ram' || src === 'crush' || src === 'splash') {
        addScore(S, -300, 'collateral'); S.counts.civ++;
        popup(S, e.x, e.y, 'COLLATERAL!  -300', '#ff5a4e', true);
        ev(S, { t: 'collateral' });
      }
      return;
    }
    if (e.kind === 'barrel') {
      // a shot barrel blows up whatever is beside it
      for (var i = 0; i < S.ents.length; i++) {
        var o = S.ents[i];
        if (o !== e && !o.dead && Math.abs(o.x - e.x) < 60 && Math.abs(o.y - e.y) < 60) damage(S, o, 4, 'splash');
      }
      if (Math.abs(p.x - e.x) < 34 && Math.abs(p.y - e.y) < 40) hurtPlayer(S, 10, e.x, e.y);
      if (src === 'gun' || src === 'missile') { addScore(S, 50, 'destruction'); popup(S, e.x, e.y, '+50', '#ffd23f'); }
      return;
    }
    if (!k.foe) return;
    if (src === 'enemy') return;
    S.counts.kills++;
    var pts = k.pts * S.combo;
    addScore(S, pts, 'destruction');
    popup(S, e.x, e.y - 20, k.name + '  +' + pts, '#ffd23f', e.kind === 'boss' || e.kind === 'heli' || !!k.big);
    S.combo = Math.min(5, S.combo + 1); bumpCombo(S);
    ev(S, { t: 'kill', kind: e.kind, src: src });
    var spectacular = false;
    if (p.air > 0) { stunt(S, 'AIRBORNE KILL', 500, e.x, e.y + 22); spectacular = true; }
    if (src === 'ram') { stunt(S, 'TURBO RAM', 300, e.x, e.y + 22); spectacular = true; }
    if (src === 'crush') { stunt(S, 'ROOF LANDING', 1000, e.x, e.y + 22); spectacular = true; }
    if (src === 'slam') { stunt(S, 'BIKER SLAM', 300, e.x, e.y + 22); spectacular = true; }
    if (S.inverted && src !== 'crush') { stunt(S, 'UPSIDE-DOWN KILL', 600, e.x, e.y + 22); spectacular = true; }
    // multi-kills inside 0.7 s
    S.kills.push(S.t);
    while (S.kills.length && S.t - S.kills[0] > 0.7) S.kills.shift();
    if (S.kills.length >= 2) {
      stunt(S, S.kills.length === 2 ? 'DOUBLE KILL' : S.kills.length === 3 ? 'TRIPLE KILL' : 'MASSACRE', 300 * (S.kills.length - 1), e.x, e.y + 44);
      spectacular = true;
    }
    if (k.big || e.kind === 'heli') spectacular = true;
    if (e.kind === 'boss') {
      S.phase = 'wrap'; S.wrapT = 3.6; S.outcome = 'wrapped'; S.cutAgain = false;
      freeze(S, 0.22); slow(S, 2.4, 0.22); shake(S, 1);
      ev(S, { t: 'bossDown' });
      for (var lj = 0; lj < p.latch.length; lj++) ev(S, { t: 'henchOff', side: p.latch[lj].side, how: 'fall', x: p.x, y: p.y });
      p.latch.length = 0;
      S.ebullets.length = 0; S.hz.length = 0;
      for (var j = 0; j < S.ents.length; j++) {
        var f = S.ents[j];
        if (!f.dead && (KIND[f.kind].foe || f.kind === 'barrel')) { f.dead = true; ev(S, { t: 'explode', x: f.x, y: f.y, size: 1.2, delay: 0.2 + j * 0.05 }); }
      }
      addScore(S, Math.round(p.hp) * 20, 'armor');
      popup(S, p.x, p.y - 60, 'ARMOR BONUS  +' + Math.round(p.hp) * 20, '#7dff9b', true);
    } else if (spectacular) {
      freeze(S, 0.09); if (S.slowCd <= 0) { slow(S, 0.7, 0.3); S.slowCd = 1.2; }
    }
    // drops
    var r = S.rnd();
    if (e.kind === 'suv' || (e.kind === 'rammer' && r < 0.25)) dropAt(S, 'crate', e);
    else if (e.kind === 'truck') { dropAt(S, 'wrench', e); dropAt(S, 'crate', e, 30); }
    else if (e.kind === 'heli') { dropAt(S, 'crate', e, -20); dropAt(S, 'wrench', e, 20); }
    else if (e.kind === 'moto' && r < 0.15) dropAt(S, 'wrench', e);
    else if (e.kind === 'hog') { if (r < 0.1) dropAt(S, 'wrench', e); else if (r < 0.22) dropAt(S, 'crate', e); }
  }

  function dropAt(S, kind, e, dx) {
    var d = spawn(S, kind);
    d.x = clamp(e.x + (dx || 0), -ROAD_HALF + 14, ROAD_HALF - 14); d.y = e.y; d.v = S.speed * 0.6;
    return d;
  }

  function nearMiss(S, e, gap) {
    var p = S.p, side = sign(e.x - p.x), turbo = hot(p);
    var pts = 100 * (turbo ? 2 : 1) * S.combo;
    addScore(S, pts, 'near'); S.counts.near++;
    popup(S, (e.x + p.x) / 2, p.y - 40, (turbo ? 'TURBO ' : '') + 'NEAR MISS  +' + pts, '#ffffff');
    ev(S, { t: 'near', x: e.x, y: e.y, side: side });
    bumpCombo(S);
    if (S.t - S.lastNear.t < 0.45 && S.lastNear.side !== side) stunt(S, 'THREAD THE NEEDLE', 400, p.x, p.y - 70);
    S.lastNear = { t: S.t, side: side };
    if ((gap < 7 || turbo || KIND[e.kind].foe) && S.slowCd <= 0) { slow(S, 0.45, 0.35); S.slowCd = 2.4; }
  }

  function overlap(a, b, inset) {
    inset = inset || 0;
    return Math.abs(a.x - b.x) < (a.w + b.w) / 2 - inset && Math.abs(a.y - b.y) < (a.h + b.h) / 2 - inset;
  }
  // a fast thing moving from y0 to y1 at x, against a box
  function sweptHit(x, y0, y1, e, pad) {
    if (Math.abs(x - e.x) > e.w / 2 + (pad || 0)) return false;
    var top = Math.min(y0, y1), bot = Math.max(y0, y1);
    return bot >= e.y - e.h / 2 && top <= e.y + e.h / 2;
  }

  function enemyShot(S, x, y, ang, spd) {
    S.ebullets.push({ x: x, y: y, vx: Math.sin(ang) * spd, vy: Math.cos(ang) * spd, life: 3 });
  }
  function aimAt(S, x, y) { return Math.atan2(S.p.x - x, S.p.y - y); }

  // 1.2 handling. Steering eases in over a tenth of a second (so a tap is a nudge and a hold is a full
  // lane change) and counter-steer bites harder than steer. Hold DRIFT (Shift since 1.7.1, A or RB on a pad)
  // and turn, and the KESTREL kicks its tail out: it slides on a lower grip, carves against the bend's
  // pull, and charges a mini-boost (blue, then orange, then pink sparks) that fires when you let go.
  // 1.7: was "held AND steer > 0.25 AND speed > 300 AND on the ground", which ignored a press made a beat
  // before the steer. Now a press is buffered, the thresholds are lower and a hop doesn't end the slide.
  var DRIFT_AT = [0.65, 1.45, 2.35], BOOST_T = [0, 0.55, 0.95, 1.45];
  var DRIFT_BUF = 0.25, DRIFT_STEER = 0.12, DRIFT_SPEED = 200;
  function hot(p) { return p.turboOn || p.boostT > 0; } // turbo or a drift boost: both count as flat out
  function endDrift(S, cash) {
    var p = S.p, d = p.drift;
    if (!d) return;
    p.drift = null;
    ev(S, { t: 'driftEnd', lvl: cash ? d.lvl : 0, x: p.x, y: p.y });
    if (!cash || !d.lvl) return;
    p.boostT = BOOST_T[d.lvl]; p.boostLvl = d.lvl; p.kick = 1;
    p.turbo = Math.min(100, p.turbo + 8 * d.lvl);
    S.speed = Math.max(S.speed, Math.min(S.speed + 50 + 30 * d.lvl, 640));
    ev(S, { t: 'boost', lvl: d.lvl, x: p.x, y: p.y });
    shake(S, 0.1 + 0.06 * d.lvl);
    if (d.lvl >= 2) stunt(S, d.lvl === 3 ? 'ULTRA DRIFT' : 'POWER DRIFT', d.lvl === 3 ? 600 : 300, p.x, p.y - 60);
  }

  // ---------- 1.3: henchmen riding the KESTREL ----------
  // A henchman who lands on the car grabs a door (left or right; two at most), drags the car toward
  // his side and punches through the window now and then. Ram his side into the wall (or another car)
  // to scrape him off, or go upside down (a loop, a corkscrew, a pipe's ceiling) and he falls off.
  function freeSides(S) { var f = [-1, 1], L = S.p.latch; for (var i = 0; i < L.length; i++) f.splice(f.indexOf(L[i].side), 1); return f; }
  function dropping(S) { var n = 0; for (var i = 0; i < S.ents.length; i++) if (S.ents[i].kind === 'hench' && !S.ents[i].dead && !S.ents[i].gone) n++; return n; }
  function henchGone(S, side, how) {
    var p = S.p, L = p.latch;
    for (var i = 0; i < L.length; i++) {
      if (L[i].side !== side) continue;
      L.splice(i, 1);
      ev(S, { t: 'henchOff', side: side, how: how, x: p.x + side * 16, y: p.y });
      S.counts.kills++;
      var pts = KIND.hench.pts * S.combo;
      addScore(S, pts, 'destruction');
      popup(S, p.x + side * 30, p.y - 30, 'HENCHMAN  +' + pts, '#ffd23f');
      S.combo = Math.min(5, S.combo + 1); bumpCombo(S);
      ev(S, { t: 'kill', kind: 'hench', src: how });
      return true;
    }
    return false;
  }
  function scrapeOff(S, side, how) {
    if (!henchGone(S, side, how)) return;
    var p = S.p;
    stunt(S, how === 'wall' ? 'WALL SMASH' : 'SIDESWIPE', how === 'wall' ? 500 : 400, p.x, p.y - 60);
    shake(S, 0.3); freeze(S, 0.07);
    if (S.slowCd <= 0) { slow(S, 0.55, 0.35); S.slowCd = 1.2; }
  }
  function shakeOff(S) {
    var p = S.p, n = p.latch.length;
    if (!n) return;
    while (p.latch.length) henchGone(S, p.latch[0].side, 'fall');
    stunt(S, n > 1 ? 'GRAVITY CHECK x2' : 'GRAVITY CHECK', 600 * n, p.x, p.y - 60);
  }

  function updatePlayer(S, inp, dt, rdt) {
    var p = S.p;
    var raw = clamp(inp.steer || 0, -1, 1), thr = clamp(inp.throttle || 0, -1, 1);
    var air = p.air > 0, playing = S.phase === 'play';
    // the wheel: quick to turn in, quicker to come back
    var growing = Math.abs(raw) > Math.abs(p.steerIn) && (raw * p.steerIn >= 0);
    p.steerIn = approach(p.steerIn, raw, (growing ? 9 : 16) * dt);
    var steer = p.steerIn;
    // 1.7 drift, kart-style and forgiving. Hold DRIFT and turn the wheel, in either order:
    //  - a press is remembered for DRIFT_BUF seconds (held over while airborne), so press-then-steer
    //    starts a slide as soon as the wheel turns;
    //  - holding DRIFT and then turning the wheel past DRIFT_STEER starts one too, however long it was held;
    //  - only a light touch of steer and DRIFT_SPEED are needed (a braking car can still drift).
    // A slide survives a hop or a jump (it just doesn't charge in the air) and ends when you let go.
    var dKey = !!inp.drift && playing, dPress = dKey && !p.dHeld;
    p.dHeld = dKey;
    if (dPress) p.dBuf = DRIFT_BUF;
    else if (!air) p.dBuf = Math.max(0, (p.dBuf || 0) - (rdt || dt));
    if (!dKey) p.dBuf = 0;
    var turned = Math.abs(raw) > DRIFT_STEER, turnedNow = turned && !(Math.abs(p.rawWas || 0) > DRIFT_STEER);
    p.rawWas = raw;
    if (!p.drift && !air && playing && dKey && turned && S.speed > DRIFT_SPEED && (p.dBuf > 0 || turnedNow || p.dLand)) {
      p.drift = { dir: sign(raw), t: 0, charge: 0, lvl: 0 };
      p.vx += sign(raw) * 70;
      p.dBuf = 0;
      ev(S, { t: 'driftStart', x: p.x, y: p.y, dir: p.drift.dir });
    }
    // held DRIFT through a landing counts as a fresh press (kart hop)
    p.dLand = false;
    if (air && dKey && !p.drift) p.dLand = true;
    var D = p.drift;
    if (D) {
      // a slide flipped the other way in its first moment follows the wheel (a late or mis-timed steer)
      if (D.t < 0.18 && turned && sign(raw) !== D.dir) { D.dir = sign(raw); p.vx += D.dir * 70; }
      if (!dKey || !playing) endDrift(S, playing);
    }
    D = p.drift;
    var acc = air ? 700 : 2400, target;
    if (D) {
      // sliding: steer into the drift to tighten it, against it to swing wide
      var into = clamp(steer * D.dir, -1, 1);
      target = D.dir * 70 + steer * 210;
      p.vx = approach(p.vx, target, (air ? 400 : 1300) * dt);
      D.t += dt;
      if (!air) D.charge += dt * (0.65 + 0.55 * Math.max(0, into));
      var lv = D.charge >= DRIFT_AT[2] ? 3 : D.charge >= DRIFT_AT[1] ? 2 : D.charge >= DRIFT_AT[0] ? 1 : 0;
      if (lv > D.lvl) { D.lvl = lv; ev(S, { t: 'driftLvl', lvl: lv, x: p.x, y: p.y }); }
      p.yawT = D.dir * (0.4 + 0.2 * Math.max(0, into)) - (into < 0 ? D.dir * 0.12 * -into : 0);
    } else {
      if (Math.abs(steer) > 0.01) {
        target = steer * 300;
        var rate = sign(target) !== sign(p.vx) && Math.abs(p.vx) > 10 ? acc * 1.8 : acc;
        p.vx = approach(p.vx, target, rate * dt);
      } else p.vx = approach(p.vx, 0, (air ? 300 : 2800) * dt);
      p.yawT = p.vx / 300 * 0.2;
    }
    // the car's visual heading follows with a little weight; body roll leans against the turn
    p.yaw = approach(p.yaw, p.yawT, (D ? 2.6 : 3.4) * dt * (1 + Math.abs(p.yawT - p.yaw) * 4));
    p.roll += ((-p.vx / 300) * (D ? 0.09 : 0.06) - p.roll) * Math.min(1, dt * 9);
    p.vy = approach(p.vy, -thr * 230, 1800 * dt);
    p.x += p.vx * dt; p.y += p.vy * dt;
    // a bend pulls the car to the outside of the curve; steering holds the line
    // (a banked corner leans into the bend and takes most of that pull away, and a drift into the
    // bend carves against it)
    if (S.phase === 'play') {
      var pdd = S.dist + H - p.y, cv = curveAt(pdd), bk = bankAt(pdd);
      var held = cv * bk > 0 ? 1 - 0.75 * Math.min(1, Math.abs(bk) / 0.9) : 1;
      if (D && D.dir * cv > 0) held *= 0.25;
      p.x -= cv * S.speed * (air ? 120 : 225) * held * dt;
    }
    // riders drag the car toward their door and land a punch now and then
    for (var li = 0; li < p.latch.length; li++) {
      var Lr = p.latch[li];
      Lr.t += dt; p.x += Lr.side * 18 * dt;
      Lr.hitT -= dt;
      if (Lr.hitT <= 0 && playing) {
        Lr.hitT = 1.4 + S.rnd() * 0.6;
        ev(S, { t: 'henchHit', side: Lr.side, x: p.x + Lr.side * 14, y: p.y });
        hurtPlayer(S, 4, p.x + Lr.side * 14, p.y);
      }
    }
    p.y = clamp(p.y, 290, 548);
    var lim = EDGE - p.w / 2 - 2;
    if (Math.abs(p.x) > lim) {
      p.x = sign(p.x) * lim;
      // slam a door with a henchman on it into the wall and he's gone
      if (playing && Math.abs(p.vx) > 60 && sign(p.vx) === sign(p.x)) scrapeOff(S, sign(p.x), 'wall');
      if (Math.abs(p.vx) > 80 && p.railCd <= 0) { ev(S, { t: 'rail', x: p.x + sign(p.x) * 14, y: p.y }); shake(S, 0.12); p.railCd = 0.25; }
      p.vx = 0;
    }
    p.railCd -= dt;
    // turbo
    var want = !!inp.turbo && S.phase === 'play';
    if (p.turbo <= 0) p.turboLock = true;
    if (p.turboLock && (!want && p.turbo > 25)) p.turboLock = false;
    var was = p.turboOn;
    p.turboOn = want && !p.turboLock && p.turbo > 0;
    if (p.turboOn) p.turbo = Math.max(0, p.turbo - 38 * dt);
    else p.turbo = Math.min(100, p.turbo + 13 * dt);
    p.turboEvT = (p.turboEvT || 0) - dt;
    if (p.turboOn && !was) {
      // the punch: a shove of speed the moment it lights, not just a slow climb. Only from a real
      // start (turbo off for a second), so tapping the button can't pump the speed up
      if (p.turboEvT <= 0) { ev(S, { t: 'turbo' }); shake(S, 0.18); p.turboEvT = 0.6; }
      if (p.offT > 1) { S.speed = Math.max(S.speed, Math.min(S.speed + 90, 700)); p.kick = 1; }
    }
    p.offT = p.turboOn ? 0 : (p.offT || 0) + dt;
    if (p.boostT > 0) p.boostT -= dt;
    if (p.kick > 0) p.kick = Math.max(0, p.kick - dt * 2.2);
    if (p.squash > 0) p.squash = Math.max(0, p.squash - dt * 3);
    var tgt = 380 + thr * (thr > 0 ? 90 : 120) + (p.turboOn ? 320 : 0) + (p.boostT > 0 && !p.turboOn ? 200 + 40 * p.boostLvl : 0);
    if (D) tgt -= 25;
    tgt -= 18 * p.latch.length;
    if (S.phase !== 'play') tgt = 300;
    S.speed = approach(S.speed, tgt, (hot(p) ? 900 : 420) * dt);
    // weapons
    p.fireCd -= dt; p.missCd -= dt;
    if (inp.fire && p.fireCd <= 0 && S.phase === 'play') {
      p.fireCd = 0.085; p.side = 1 - p.side;
      var off = p.side ? 8 : -8;
      S.bullets.push({ x: p.x + off, y: p.y - p.h / 2, py: p.y - p.h / 2, vy: -1150 });
      ev(S, { t: 'gun', x: p.x + off, y: p.y - p.h / 2 - 4 });
    }
    if (inp.missile && p.missCd <= 0 && S.phase === 'play') {
      if (p.missiles > 0) {
        p.missiles--; p.missCd = 0.3;
        S.missiles.push({ x: p.x, y: p.y - 30, px: p.x, py: p.y - 30, ang: 0, spd: 260, life: 2.6, target: null });
        ev(S, { t: 'missile', x: p.x, y: p.y - 30 });
      } else { ev(S, { t: 'empty' }); p.missCd = 0.3; }
    }
    // a gap jump lasts until the road comes back under you, however fast you go
    if (p.gapJ) {
      if (S.dist + H - p.y >= p.gapJ.g1) { p.gapJ = null; p.air = Math.min(p.air, 1e-4); }
      else p.air = Math.max(p.air, 0.3);
    }
    // airborne
    if (air) {
      p.air -= dt;
      if (p.air <= 0) {
        p.air = 0; p.grace = 0.25; p.squash = 1;
        ev(S, { t: 'land', x: p.x, y: p.y }); shake(S, 0.35);
        for (var i = 0; i < S.ents.length; i++) {
          var e = S.ents[i];
          if (!e.dead && KIND[e.kind].foe && e.kind !== 'boss' && overlap(p, e, 2)) { e.hp = 0; kill(S, e, 'crush'); }
        }
      }
    }
    if (p.grace > 0) p.grace -= dt;
    if (p.inv > 0) p.inv -= dt;
    // how high the car rides above the road (the renderer's lift)
    if (p.gapJ) { var gu = (S.dist + H - p.y - p.gapJ.g0) / (p.gapJ.g1 - p.gapJ.g0); p.liftH = 4 * p.gapJ.h * clamp(gu, 0, 1) * (1 - clamp(gu, 0, 1)); }
    else p.liftH = p.air > 0 && p.airMax ? Math.sin(Math.PI * (1 - p.air / p.airMax)) * 58 : 0;
  }

  // the set pieces: warn ahead, then loops, gaps, corkscrews and banked walls as you reach them
  var WARN = 1150;
  function setPieces(S, dt) {
    var p = S.p, now = S.dist + H - p.y, was = S.pd === null ? now : S.pd, FEATS = TR.FEATS;
    S.pd = now;
    if (S.phase !== 'play') return;
    for (var i = 0; i < FEATS.length; i++) {
      var f = FEATS[i];
      if (f.d0 - WARN > now) break;
      if (f.d1 < was - 10) continue;
      if (was < f.d0 - WARN && now >= f.d0 - WARN) ev(S, { t: 'ahead', kind: f.kind, dir: f.dir || 0 });
      if (f.kind === 'jump' && was < f.g0 && now >= f.g0) {
        var big = hot(p);
        p.gapJ = { g0: f.g0, g1: f.g1, h: big ? 70 : 34 }; p.air = 0.3; p.airMax = 0.3; p.grace = 0;
        S.counts.jumps++; S.counts.gaps++;
        ev(S, { t: 'jump', x: p.x, y: p.y, gap: true });
        ev(S, { t: 'gapJump', turbo: big });
        stunt(S, big ? 'TURBO AIR' : 'BIG AIR', big ? 1200 : 700, p.x, p.y - 60);
        slow(S, 0.9, 0.4); shake(S, 0.3);
      }
      if (f.kind === 'loop' || f.kind === 'twist' || f.kind === 'pipe') {
        var key = f.kind === 'loop' ? 'inLoop' : f.kind === 'pipe' ? 'inPipe' : 'inTwist';
        if (was < f.d0 && now >= f.d0) { S[key] = { f: f, turbo: hot(p), hp: p.hp }; ev(S, { t: f.kind + 'In' }); }
        if (S[key] && S[key].f === f) {
          var u = (now - f.d0) / (f.d1 - f.d0);
          if (hot(p)) S[key].turbo = true;
          if (u > 0.5 && !S[key].top) { S[key].top = true; ev(S, { t: f.kind + 'Top' }); }
          if (now >= f.d1) {
            var L = S[key]; S[key] = null;
            if (f.kind === 'loop') { S.counts.loops++; stunt(S, L.turbo ? 'TURBO LOOP' : 'LOOP THE LOOP', L.turbo ? 1200 : 700, p.x, p.y - 60); }
            else if (f.kind === 'pipe') { S.counts.pipes++; stunt(S, f.turns > 1 ? 'PIPE RUN x' + f.turns : 'PIPE RUN', 700 + 300 * f.turns, p.x, p.y - 60); }
            else { S.counts.twists++; stunt(S, 'CORKSCREW', 800, p.x, p.y - 60); }
            ev(S, { t: f.kind + 'Out' });
          }
        }
      }
      if (f.kind === 'wall') {
        if (was < f.d0 && now >= f.d0) { S.inWall = { f: f, t: 0, done: false }; ev(S, { t: 'wallIn', dir: f.dir }); }
        if (S.inWall && S.inWall.f === f) {
          // ride the high side of the bank (the outside of the bend) to earn a WALL RIDE
          if (p.x * f.dir < -50 && p.air <= 0) S.inWall.t += dt; else S.inWall.t = Math.max(0, S.inWall.t - dt * 0.5);
          if (!S.inWall.done && S.inWall.t > 1.2) { S.inWall.done = true; stunt(S, 'WALL RIDE', 600, p.x, p.y - 60); }
          if (now >= f.d1) S.inWall = null;
        }
      }
    }
    // upside down: the middle of a loop or a corkscrew
    var inv = false;
    if (S.inLoop) { var lu = (now - S.inLoop.f.d0) / (S.inLoop.f.d1 - S.inLoop.f.d0); inv = lu > 0.3 && lu < 0.7; }
    if (S.inTwist) { var tu = (now - S.inTwist.f.d0) / (S.inTwist.f.d1 - S.inTwist.f.d0); inv = inv || (tu > 0.3 && tu < 0.7); }
    if (S.inPipe) inv = inv || Math.cos(TR.twistAt(now)) < -0.3; // on the ceiling of the tube
    S.inverted = inv;
    if (inv && p.latch.length && S.phase === 'play') shakeOff(S);
  }

  function updateEnt(S, e, dt) {
    var p = S.p, k = KIND[e.kind], r = S.rnd;
    e.age += dt; e.t += dt;
    if (e.flash > 0) e.flash -= dt;
    if (e.bumpCd > 0) e.bumpCd -= dt;
    if (k.civ || k.pickup || k.hazard) {
      e.vy = S.speed - e.v;
      if (k.pickup && e.v > 0) e.v = Math.max(0, e.v - 300 * dt);
    } else if (e.kind === 'moto') {
      if (e.st === 'enter') { e.vy = 210; if (e.y >= e.ty) { e.st = 'weave'; e.t = 0; } }
      else if (e.st === 'weave') {
        e.vy = (e.ty + Math.sin(e.age * 1.3 + e.ph) * 40 - e.y) * 2;
        e.cx = approach(e.cx, clamp(p.x, -100, 100), 55 * dt);
        e.fireT -= dt;
        if (e.fireT <= 0 && e.y < p.y - 80) { e.fireT = 1.9; enemyShot(S, e.x, e.y + 16, aimAt(S, e.x, e.y), 330); ev(S, { t: 'eshot', x: e.x, y: e.y }); }
        if (e.t > 9) { e.st = 'dive'; }
      } else { e.vy = approach(e.vy, 300, 400 * dt); }
      var nx = e.cx + Math.sin(e.age * 2.7 + e.ph) * 52;
      e.lean = clamp((nx - e.x) * 0.15, -1, 1);
      e.x = nx;
    } else if (e.kind === 'rammer') {
      if (e.st === 'enter') { e.vy = 220; if (e.y >= e.ty) { e.st = 'track'; e.t = 0; } }
      else if (e.st === 'track') {
        e.vy = (e.ty - e.y) * 3; e.x = approach(e.x, p.x, 170 * dt);
        if (e.t > 1.1) { e.st = 'rev'; e.t = 0; ev(S, { t: 'rev', x: e.x, y: e.y }); }
      } else if (e.st === 'rev') {
        e.vy = -40; e.x = approach(e.x, p.x, 50 * dt);
        if (e.t > 0.5) { e.st = 'charge'; e.t = 0; }
      } else { e.vy = approach(e.vy, 560, 1600 * dt); e.x = approach(e.x, p.x, 26 * dt); }
    } else if (e.kind === 'suv') {
      if (e.st === 'enter') { e.vy = 170; if (e.y >= e.ty) { e.st = 'hunt'; e.t = 0; } }
      else if (e.st === 'hunt') {
        e.vy = (e.ty - e.y) * 2;
        if (e.t > 2) { e.t = 0; e.cx = (r() - 0.5) * 120; }
        e.x = approach(e.x, clamp(p.x + e.cx, -ROAD_HALF + 20, ROAD_HALF - 20), 90 * dt);
        e.fireT -= dt;
        if (e.fireT <= 0) { e.fireT = 2.3; e.burst = 3; e.burstT = 0; }
        if (e.burst > 0) {
          e.burstT -= dt;
          if (e.burstT <= 0) { e.burst--; e.burstT = 0.13; enemyShot(S, e.x, e.y + 20, aimAt(S, e.x, e.y) + (r() - 0.5) * 0.12, 360); ev(S, { t: 'eshot', x: e.x, y: e.y }); }
        }
        if (e.age > 22) e.st = 'leave';
      } else e.vy = approach(e.vy, -260, 300 * dt);
    } else if (e.kind === 'truck') {
      if (e.st === 'enter') { e.vy = 100; if (e.y >= e.ty) { e.st = 'block'; e.t = 0; } }
      else if (e.st === 'block') {
        e.vy = (e.ty - e.y) * 2;
        if (e.t > 3) { e.t = 0; e.cx = clamp(Math.round(p.x / 64 - 0.5) * 64 + 32, -96, 96); }
        if (e.cx) e.x = approach(e.x, e.cx, 40 * dt);
        e.dropT -= dt;
        if (e.dropT <= 0) { e.dropT = 2.1; var b = spawn(S, 'barrel'); b.x = e.x; b.y = e.y + e.h / 2 + 12; b.v = 0; ev(S, { t: 'drop', x: b.x, y: b.y }); }
        if (e.age > 26) e.st = 'leave';
      } else e.vy = approach(e.vy, -200, 200 * dt);
    } else if (e.kind === 'heli') {
      if (e.st === 'enter') {
        e.vy = 240; e.alt = approach(e.alt, 120, 70 * dt);
        if (e.y >= e.ty) { e.st = 'hover'; e.t = 0; ev(S, { t: 'heliIn', x: e.x, y: e.y }); }
      } else if (e.st === 'hover') {
        e.vy = (e.ty + Math.sin(e.age * 0.9) * 14 - e.y) * 2;
        var want = clamp(p.x + e.cx, -110, 110);
        e.vx = approach(e.vx, clamp((want - e.x) * 1.6, -120, 120), 170 * dt); e.x += e.vx * dt;
        e.alt = approach(e.alt, 78 + Math.sin(e.age * 1.3) * 6, 50 * dt);
        e.dropT -= dt;
        if (e.dropT <= 0 && e.riders > 0 && S.phase === 'play') {
          var fs = freeSides(S);
          if (fs.length > dropping(S) && p.air <= 0) {
            var h = spawn(S, 'hench');
            h.x = e.x; h.y = e.y + 8; h.alt = h.alt0 = e.alt - 14; h.tx = e.x; h.ty2 = e.y + 60; h.heli = e.id; h.st = 'drop'; h.t = 0;
            e.riders--; e.dropT = 2.3; e.cx = -e.cx;
            ev(S, { t: 'henchDrop', x: h.x, y: h.y });
          } else e.dropT = 0.5;
        }
        if ((e.riders <= 0 && e.dropT < -1.4) || e.t > 26) { e.st = 'leave'; e.t = 0; }
      } else { e.vy = approach(e.vy, -240, 220 * dt); e.alt += 40 * dt; e.vx *= 0.98; e.x += e.vx * dt; }
      e.lean = clamp(e.vx / 120, -1, 1);
    } else if (e.kind === 'hench') {
      if (e.st === 'drop') {
        // the landing mark chases the car, a little slower than the car can steer: swerve to dodge
        e.tx = approach(e.tx, p.x, 95 * dt); e.ty2 = approach(e.ty2, p.y, 170 * dt);
        var u = clamp(e.t / e.T, 0, 1);
        e.x += (e.tx - e.x) * Math.min(1, dt * 6);
        e.vy = (e.ty2 - e.y) * 6;
        e.alt = 10 + (e.alt0 - 10) * (1 - u * u);
        if (u >= 1) {
          var fr = freeSides(S), side = fr.length === 2 ? sign(e.x - p.x) : fr[0];
          e.gone = true;
          if (S.phase === 'play' && p.air <= 0 && side && Math.abs(p.x - e.tx) < 22 && Math.abs(p.y - e.ty2) < 30) {
            p.latch.push({ side: side, t: 0, hitT: 1.1 + r() * 0.4 });
            ev(S, { t: 'latch', side: side, x: p.x, y: p.y }); shake(S, 0.25);
          } else {
            ev(S, { t: 'henchOff', how: 'miss', x: e.x, y: e.y, alt: 6 });
            popup(S, e.x, e.y - 20, 'DODGED THE DROP  +150', '#7dff9b'); addScore(S, 150, 'stunts');
          }
        }
      }
    } else if (e.kind === 'hog') {
      hog(S, e, dt);
    } else if (e.kind === 'boss' && LV.n === 3) {
      dealer(S, e, dt);
    } else if (e.kind === 'boss' && LV.n === 2) {
      alchemist(S, e, dt);
    } else if (e.kind === 'boss') {
      S.bossT += dt;
      if (e.st === 'enter') { e.vy = 70; if (e.y >= e.ty) { e.st = 'fight'; e.t = 0; e.fireT = 1.2; e.dropT = 2.5; } }
      else if (e.st === 'fight') {
        e.vy = (e.ty + Math.sin(e.age * 0.7) * 26 - e.y) * 2;
        e.x = approach(e.x, Math.sin(e.age * 0.55) * 74, 90 * dt);
        var rage = e.hp < e.max * 0.5;
        e.fireT -= dt;
        if (e.fireT <= 0) {
          e.fireT = rage ? 1.5 : 2.1;
          var base = aimAt(S, e.x, e.y + 40), n = rage ? 7 : 5;
          for (var j = 0; j < n; j++) enemyShot(S, e.x, e.y + 40, base + (j - (n - 1) / 2) * 0.17, 300);
          ev(S, { t: 'eshot', x: e.x, y: e.y, big: true });
        }
        e.dropT -= dt;
        if (e.dropT <= 0) {
          e.dropT = rage ? 2.2 : 3;
          for (var q = -1; q <= 1; q += 2) { var bb = spawn(S, 'barrel'); bb.x = e.x + q * 22; bb.y = e.y + e.h / 2 + 12; bb.v = 0; }
          ev(S, { t: 'drop', x: e.x, y: e.y + e.h / 2 });
        }
        if (!e.s1 && e.hp < e.max * 0.66) { e.s1 = true; spawn(S, 'moto'); spawn(S, 'moto', 1); caption(S, 'HE’S CALLING IN THE BIKERS!'); }
        if (!e.s2 && e.hp < e.max * 0.33) { e.s2 = true; spawn(S, 'heli'); spawn(S, 'moto', 1); caption(S, 'HE\u2019S CALLED IN A CHOPPER! ONE MORE PUSH!'); }
        if (S.bossT > 70 && S.phase === 'play') {
          e.st = 'flee'; S.phase = 'wrap'; S.wrapT = 3; S.outcome = 'escaped';
          caption(S, KIND.boss.name + ' GOT AWAY...'); ev(S, { t: 'escaped' });
        }
      } else e.vy = approach(e.vy, -260, 200 * dt);
    }
    var py = e.y;
    e.y += e.vy * dt;
    // near-miss: a car that passes the player's line without touching
    var isCar = k.civ || (k.foe && e.kind !== 'boss' && !k.air);
    if (isCar && S.phase === 'play') {
      var rel = sign(e.y - p.y);
      if (e.rel && rel !== e.rel && !e.touched && !e.nearDone && p.air <= 0) {
        var gap = Math.abs(e.x - p.x) - (e.w + p.w) / 2;
        if (gap > -2 && gap < 14) { e.nearDone = true; nearMiss(S, e, gap); }
      }
      e.rel = rel;
    }
    if (e.y > H + 160 || e.y < -420 || (py < e.y && e.y > H + e.h)) e.gone = true;
  }

  // ---------- 1.5, level 3: the SNAKE EYES on their choppers ----------
  // A biker rides back down the road to you (or rolls off the carrier's tail ramp, or flies off its top
  // deck), pulls up level with one of your doors and whirls a chain over his head (the warning), then
  // swings it. Be a car's width away when it lands, or slam into him sideways first: a chopper
  // can't take a hit from the KESTREL. After a while he brakes and drops back out of the shot.
  function hog(S, e, dt) {
    var p = S.p, r = S.rnd, x0 = e.x, gap = p.w / 2 + e.w / 2 + 8;
    if (e.cd > 0) e.cd -= dt;
    if (e.st === 'enter') {
      e.vy = 200; if (e.y >= e.ty) { e.st = 'ride'; e.t = 0; }
    } else if (e.st === 'ramp') {
      // down the tail ramp and onto the road, falling back toward you
      e.vy = approach(e.vy, 170, 300 * dt); e.x += e.vx * dt; e.vx *= Math.max(0, 1 - dt * 2);
      if (e.t > 0.55) { e.st = 'ride'; e.t = 0; }
    } else if (e.st === 'fly') {
      // off the kicker on the top deck: the landing spot chases you, slower than you can steer
      var u = clamp(e.t / e.T, 0, 1);
      e.tx = approach(e.tx, p.x, 80 * dt); e.ty2 = approach(e.ty2, p.y - 10, 120 * dt);
      e.x += (e.tx - e.x) * Math.min(1, dt * 4);
      e.vy = (e.ty2 - e.y) / Math.max(0.05, e.T - e.t);
      e.alt = e.alt0 * (1 - u) + 4 * 46 * u * (1 - u);
      if (u >= 1) {
        e.alt = 0; e.st = 'ride'; e.t = 0; e.cd = 0.9;
        var crush = S.phase === 'play' && p.air <= 0 && Math.abs(p.x - e.x) < 20 && Math.abs(p.y - e.y) < 34;
        ev(S, { t: 'hogLand', x: e.x, y: e.y, crush: crush }); shake(S, 0.15);
        if (crush) { hurtPlayer(S, 12, e.x, e.y); e.x = clamp(e.x + (e.x > p.x ? 1 : -1) * 26, -EDGE + 10, EDGE - 10); }
        else if (S.phase === 'play') { popup(S, e.x, e.y - 20, 'DODGED THE JUMP  +150', '#7dff9b'); addScore(S, 150, 'stunts'); }
      }
    } else if (e.st === 'ride') {
      // pull up beside the door on his side (the other side if the wall is in the way)
      var sx = p.x + e.side * gap;
      if (Math.abs(sx) > EDGE - e.w / 2 - 4) { e.side = -e.side; sx = p.x + e.side * gap; }
      e.x = approach(e.x, clamp(sx, -EDGE + e.w / 2 + 2, EDGE - e.w / 2 - 2), 120 * dt);
      e.vy = clamp((p.y - 6 + Math.sin(e.age * 1.7 + e.ph) * 10 - e.y) * 2.2, -260, 260);
      if (e.cd <= 0 && S.phase === 'play' && p.air <= 0 && Math.abs(e.y - p.y) < 34 && Math.abs(e.x - p.x) < 40) {
        e.st = 'wind'; e.t = 0; e.side = e.x < p.x ? -1 : 1;
        ev(S, { t: 'chainWarn', x: e.x, y: e.y, side: e.side });
      }
      if (e.age > 16 || S.phase !== 'play') e.st = 'peel';
    } else if (e.st === 'wind') {
      // the chain whirls over his head: get a car's width away, or hit him first
      e.vy = (p.y - 6 - e.y) * 2; e.x = approach(e.x, p.x + e.side * (gap - 2), 60 * dt);
      if (e.t > 0.6) {
        e.st = 'ride'; e.t = 0; e.cd = 1.8 + r() * 0.8;
        var reach = S.phase === 'play' && p.air <= 0 && Math.abs(e.x - p.x) < 36 && Math.abs(e.y - p.y) < 36;
        ev(S, { t: 'chain', x: e.x, y: e.y, side: e.side, hit: reach });
        if (reach) hurtPlayer(S, 7, (p.x + e.x) / 2, p.y);
        else if (S.phase === 'play') { popup(S, e.x, e.y - 24, 'DUCKED THE CHAIN  +150', '#7dff9b'); addScore(S, 150, 'stunts'); }
      }
    } else {
      // brakes and drops back out of the shot
      e.vy = approach(e.vy, 330, 320 * dt);
    }
    e.lean = dt > 0 ? clamp((e.x - x0) / dt / 140, -1, 1) : e.lean;
  }

  // ---------- level 3's boss: THE BIG DEALER and his motorcycle carrier ----------
  // A two-deck car carrier loaded with SNAKE EYES. It never stops sending them: one rolls off the tail
  // ramp every couple of seconds, and from two thirds down they also launch off the top deck's kicker
  // and fly at you (their landing spot is marked on the road). Below two thirds it brake-checks you
  // (its tail lights flare, then it drops back onto you), and below a third it empties the whole truck.
  function hogsOut(S) { var n = 0; for (var i = 0; i < S.ents.length; i++) if (S.ents[i].kind === 'hog' && !S.ents[i].dead && !S.ents[i].gone) n++; return n; }
  function launch(S, b, jump) {
    var r = S.rnd, h = spawn(S, 'hog');
    h.x = clamp(b.x + (r() - 0.5) * b.w * 0.6, -ROAD_HALF + 14, ROAD_HALF - 14);
    h.y = b.y + b.h / 2 + (jump ? -14 : 8); h.age = 0; h.t = 0;
    if (jump) {
      h.st = 'fly'; h.T = 1.15; h.alt0 = 34; h.alt = 34; h.tx = S.p.x + (r() - 0.5) * 30; h.ty2 = S.p.y - 10;
      ev(S, { t: 'hogJump', x: h.x, y: h.y });
    } else {
      h.st = 'ramp'; h.vy = 140; h.vx = (r() - 0.5) * 80;
      ev(S, { t: 'hogOff', x: h.x, y: h.y });
    }
    return h;
  }
  function dealer(S, e, dt) {
    var f = e.hp / e.max;
    S.bossT += dt;
    if (e.st === 'enter') {
      e.vy = 70;
      if (e.y >= e.ty) { e.st = 'fight'; e.t = 0; e.spT = 1.0; e.phase = 0; e.brakeT = 6; e.n = 0; e.brake = null; }
    } else if (e.st === 'fight') {
      var phase = f < 0.33 ? 2 : f < 0.66 ? 1 : 0;
      if (phase > e.phase) {
        e.phase = phase;
        caption(S, phase === 1 ? 'THEY\u2019RE JUMPING OFF THE TOP DECK!' : 'HE\u2019S EMPTYING THE WHOLE TRUCK!');
        ev(S, { t: 'bossPhase', n: phase }); shake(S, 0.3);
        e.spT = Math.min(e.spT, 0.6); e.brakeT = Math.min(e.brakeT, 2.5);
      }
      // the brake check: tail lights flare, then it drops back right onto your bonnet
      var ty = e.ty + Math.sin(e.age * 0.6) * 18;
      e.brakeT -= dt;
      if (e.brake) {
        e.brake.t += dt;
        if (e.brake.t > 0.8) ty = e.ty + 150;
        if (e.brake.t > 2.3) { e.brake = null; e.brakeT = phase === 2 ? 5 : 7.5; }
      } else if (phase >= 1 && e.brakeT <= 0 && S.phase === 'play') {
        e.brake = { t: 0 }; ev(S, { t: 'brakeWarn', x: e.x, y: e.y });
      }
      e.vy = clamp((ty - e.y) * 2, -150, 190);
      e.x = approach(e.x, Math.sin(e.age * 0.4) * 70, 60 * dt);
      // the bikers, off the back
      e.spT -= dt;
      if (e.spT <= 0 && S.phase === 'play') {
        var out = hogsOut(S), cap = [4, 5, 6][phase];
        if (out < cap) {
          var jump = phase >= 1 && e.n % (phase === 2 ? 2 : 3) === 1;
          launch(S, e, jump);
          if (phase === 2 && out + 1 < cap) launch(S, e, false);
          e.n++;
        }
        e.spT = [2.6, 2.1, 1.6][phase];
      }
      if (S.bossT > (LV.escapeT || 70) && S.phase === 'play') {
        e.st = 'flee'; S.phase = 'wrap'; S.wrapT = 3; S.outcome = 'escaped';
        caption(S, KIND.boss.name + ' GOT AWAY...'); ev(S, { t: 'escaped' });
      }
    } else e.vy = approach(e.vy, -260, 200 * dt);
  }

  // ---------- level 2's boss: THE ALCHEMIST and his tesla tanker ----------
  // He never shoots bullets or drops barrels like the Juggernaut. Everything he does is electric and
  // warned first, so it is about reading the road and moving:
  //  LANE ARC: a coil charges over one lane (two when he is enraged), the lane lights up with a crackling
  //    outline, then a bolt runs the length of it.
  //  LOCK-ON: reticles chase you across the road, lock, and lightning falls on them from the storm.
  //  SHOCK WAVE: the tanker discharges a ring that rolls down the road at you; it has one dead gap,
  //    marked green. Be in the gap or in the air when it passes.
  // Below two thirds he overcharges (faster, doubled), and below a third it is a meltdown (combos).
  var LANE_W = 64;
  function laneOf(x) { return clamp(Math.floor((x + ROAD_HALF) / LANE_W), 0, 3); }
  function laneX(i) { return -ROAD_HALF + LANE_W * (i + 0.5); }
  function hzAdd(S, o) { o.age = 0; o.done = false; S.hz.push(o); return o; }
  function alchemist(S, e, dt) {
    var r = S.rnd, p = S.p, f = e.hp / e.max;
    S.bossT += dt;
    if (e.st === 'enter') {
      e.vy = 70;
      if (e.y >= e.ty) { e.st = 'fight'; e.t = 0; e.atkT = 1.6; e.atk = 0; e.charge = 0; e.phase = 0; }
    } else if (e.st === 'fight') {
      e.vy = (e.ty + Math.sin(e.age * 0.8) * 22 - e.y) * 2;
      // he drifts across the road, but stays over the middle so his arcs can reach every lane
      e.x = approach(e.x, Math.sin(e.age * 0.45) * 60, 70 * dt);
      var phase = f < 0.33 ? 2 : f < 0.66 ? 1 : 0;
      if (phase > e.phase) {
        e.phase = phase;
        caption(S, phase === 1 ? 'HE\u2019S OVERCHARGING THE COILS!' : 'MELTDOWN! THE WHOLE TANKER IS LIVE!');
        ev(S, { t: 'bossPhase', n: phase }); shake(S, 0.3);
        e.atkT = Math.min(e.atkT, 1.2);
      }
      if (e.charge > 0) e.charge = Math.max(0, e.charge - dt);
      e.atkT -= dt;
      if (e.atkT <= 0) {
        var seq = phase === 0 ? ['lane', 'zone', 'lane', 'ring'] : phase === 1 ? ['lane2', 'ring', 'zone', 'lane2', 'zone'] : ['ring2', 'lane2', 'zone3', 'ring', 'lane2'];
        var a = seq[e.atk % seq.length]; e.atk++;
        var gapT = phase === 0 ? 2.6 : phase === 1 ? 2.1 : 1.75;
        if (a === 'lane' || a === 'lane2') {
          var warn = phase === 0 ? 1.0 : 0.85, l0 = laneOf(p.x);
          var lanes = [l0];
          if (a === 'lane2') {
            // the second arc goes beside the first, so a whole half of the road stays clear
            var l1 = l0 <= 1 ? l0 + 1 : l0 - 1;
            if (r() < 0.35) l1 = l0 <= 1 ? (l0 === 0 ? 1 : 0) : (l0 === 3 ? 2 : 3);
            lanes.push(l1);
          }
          for (var i = 0; i < lanes.length; i++) hzAdd(S, { k: 'lane', lane: lanes[i], x: laneX(lanes[i]), warn: warn, live: 0.4, side: i ? 1 : -1 });
          e.charge = warn; e.chargeK = 'lane';
          ev(S, { t: 'zapWarn', x: laneX(l0) });
          e.atkT = gapT;
        } else if (a === 'zone' || a === 'zone3') {
          var n = a === 'zone3' ? 3 : 2, zw = phase === 0 ? 1.15 : 0.95;
          hzAdd(S, { k: 'zone', x: p.x, y: p.y, r: 36, warn: zw, live: 0.32, track: 0.55 });
          for (var j = 1; j < n; j++) {
            var away = p.x > 0 ? -1 : 1, zx = clamp(p.x + away * (j === 1 ? 1 : 1.8) * (75 + r() * 30), -ROAD_HALF + 20, ROAD_HALF - 20);
            hzAdd(S, { k: 'zone', x: zx, y: clamp(p.y - 60 + r() * 120, 310, 530), r: 36, warn: zw + 0.18 * j, live: 0.32, track: 0 });
          }
          e.charge = zw; e.chargeK = 'zone';
          ev(S, { t: 'zoneWarn' });
          e.atkT = gapT + 0.2;
        } else {
          // the shock wave: its gap is never where you are, so you have to move to it
          var gx, tries = 0;
          do { gx = laneX(Math.floor(r() * 4)); tries++; } while (Math.abs(gx - p.x) < 50 && tries < 8);
          var cw = phase === 0 ? 0.8 : 0.65;
          hzAdd(S, { k: 'ring', gx: gx, gw: phase === 2 ? 66 : 78, warn: cw, y: e.y + e.h / 2, spd: phase === 0 ? 300 : 340 });
          if (a === 'ring2') {
            var g2 = laneX(laneOf(gx) < 2 ? laneOf(gx) + 2 : laneOf(gx) - 2);
            hzAdd(S, { k: 'ring', gx: g2, gw: 66, warn: cw + 0.9, y: e.y + e.h / 2, spd: 340 });
          }
          e.charge = cw; e.chargeK = 'ring';
          ev(S, { t: 'ringWarn' });
          e.atkT = gapT + (a === 'ring2' ? 0.9 : 0.4);
        }
      }
      if (S.bossT > 70 && S.phase === 'play') {
        e.st = 'flee'; S.phase = 'wrap'; S.wrapT = 3; S.outcome = 'escaped'; S.hz.length = 0;
        caption(S, KIND.boss.name + ' GOT AWAY...'); ev(S, { t: 'escaped' });
      }
    } else e.vy = approach(e.vy, -260, 200 * dt);
  }
  // the electric hazards: warned, then live; they hurt only while live, and only once each
  function hazards(S, dt) {
    var p = S.p, b = S.boss;
    for (var i = S.hz.length - 1; i >= 0; i--) {
      var h = S.hz[i];
      h.age += dt;
      var live = h.age >= h.warn;
      if (h.k === 'lane') {
        if (live && !h.fired) { h.fired = true; ev(S, { t: 'zap', x: h.x, lane: h.lane }); shake(S, 0.22); }
        if (live && !h.done && S.phase === 'play' && p.air <= 0 && Math.abs(p.x - h.x) < LANE_W / 2 + p.w / 2 - 7) {
          h.done = true; hurtPlayer(S, 16, p.x, p.y - 10); ev(S, { t: 'shock', x: p.x, y: p.y });
        }
        if (h.age >= h.warn + h.live) S.hz.splice(i, 1);
      } else if (h.k === 'zone') {
        if (!live && h.age < h.track) h.x = approach(h.x, p.x, 70 * dt);
        if (live && !h.fired) { h.fired = true; ev(S, { t: 'strike', x: h.x, y: h.y }); shake(S, 0.18); }
        if (live && !h.done && S.phase === 'play' && p.air <= 0 && Math.abs(p.x - h.x) < h.r + 6 && Math.abs(p.y - h.y) < h.r + 18) {
          h.done = true; hurtPlayer(S, 18, p.x, p.y - 10); ev(S, { t: 'shock', x: p.x, y: p.y });
        }
        if (h.age >= h.warn + h.live) S.hz.splice(i, 1);
      } else if (h.k === 'ring') {
        if (!live) { if (b) h.y = b.y + b.h / 2; continue; }
        if (!h.fired) { h.fired = true; ev(S, { t: 'ringOut' }); shake(S, 0.25); }
        var py = h.y;
        h.y += h.spd * dt;
        if (!h.done && py < p.y && h.y >= p.y && S.phase === 'play') {
          h.done = true;
          if (p.air <= 0 && Math.abs(p.x - h.gx) > h.gw / 2 - p.w / 2 + 4) { hurtPlayer(S, 14, p.x, p.y - 10); ev(S, { t: 'shock', x: p.x, y: p.y }); }
          else { popup(S, p.x, p.y - 50, 'DODGED!  +200', '#7dff9b'); addScore(S, 200, 'stunts'); ev(S, { t: 'dodge' }); }
        }
        if (h.y > H + 60) S.hz.splice(i, 1);
      }
    }
  }

  function collide(S) {
    var p = S.p, ents = S.ents, i, e, k;
    for (i = 0; i < ents.length; i++) {
      e = ents[i]; k = KIND[e.kind];
      if (e.dead || e.gone) continue;
      if (k.pickup) {
        if (overlap(p, e, -6) && S.phase === 'play') {
          e.gone = true;
          if (k.pickup === 'missiles') { p.missiles = Math.min(9, p.missiles + 3); popup(S, e.x, e.y, '+3 MISSILES', '#ff9e3d'); }
          else { p.hp = Math.min(p.maxHp, p.hp + 30); p.parts = partsFor(p.hp); popup(S, e.x, e.y, '+30 ARMOR', '#7dff9b'); }
          ev(S, { t: 'pickup', x: e.x, y: e.y, kind: k.pickup });
        }
        continue;
      }
      if (k.air || e.st === 'fly') continue;
      if (p.air > 0 || p.grace > 0 || S.phase !== 'play') continue;
      if (k.ramp && !e.used) {
        var rampTop = e.y + e.h / 2 - 30;
        if (Math.abs(p.x - e.x) < e.w / 2 + 2 && p.y - p.h / 2 < e.y + e.h / 2 && p.y - p.h / 2 > rampTop - 6) {
          e.used = true; e.touched = true;
          var T = hot(p) ? 1.5 : 1.05;
          p.air = T; p.airMax = T; S.counts.jumps++;
          ev(S, { t: 'jump', x: p.x, y: p.y });
          stunt(S, hot(p) ? 'TURBO JUMP' : 'RAMP JUMP', hot(p) ? 700 : 400, p.x, p.y - 60);
          slow(S, 0.6, 0.4); shake(S, 0.3);
          continue;
        }
      }
      if (!overlap(p, e, 3)) continue;
      e.touched = true;
      if (e.kind === 'barrel') { e.hp = 0; kill(S, e, 'enemy'); hurtPlayer(S, 14, e.x, e.y); continue; }
      if (e.kind === 'boss') {
        // the Juggernaut is a wall: shove the spy car back down
        p.y = Math.max(p.y, e.y + e.h / 2 + p.h / 2 + 1); p.vy = 120;
        if (e.bumpCd <= 0) { e.bumpCd = 0.4; hurtPlayer(S, 10, p.x, p.y - p.h / 2); }
        continue;
      }
      // 1.5: a chopper hit side-on by the KESTREL is finished (and so is one you ram at turbo)
      if (e.kind === 'hog' && Math.abs(p.y - e.y) < (p.h + e.h) / 2 - 10) {
        var into = (e.x - p.x) * p.vx > 0 && Math.abs(p.vx) > 40;
        if (into || hot(p)) {
          if (e.bumpCd <= 0) { e.bumpCd = 0.3; ev(S, { t: 'bump', x: (p.x + e.x) / 2, y: (p.y + e.y) / 2 }); shake(S, 0.2); damage(S, e, 3, 'slam', (p.x + e.x) / 2, e.y); }
          p.vx *= 0.6;
          continue;
        }
      }
      // a bump: push sideways, then damage both
      var dx = p.x - e.x || 0.01, push = (p.w + e.w) / 2 - Math.abs(dx) + 1;
      var heavy = k.big || e.kind === 'bus';
      var vertical = Math.abs(p.y - e.y) > (p.h + e.h) / 2 - 10;
      if (vertical) {
        if (p.y > e.y) { p.y += 6; p.vy = 160; S.speed *= 0.86; } else { p.y -= 4; }
      } else {
        p.x += sign(dx) * push * (heavy ? 1 : 0.6); p.vx = sign(dx) * 200;
        if (p.latch.length) scrapeOff(S, -sign(dx), 'swipe'); // the car on his side wipes him off the door
        if (!heavy) e.x -= sign(dx) * push * 0.4;
      }
      if (e.bumpCd > 0) continue;
      e.bumpCd = 0.45;
      ev(S, { t: 'bump', x: (p.x + e.x) / 2, y: (p.y + e.y) / 2 });
      if (hot(p) && k.foe) {
        damage(S, e, k.big ? 4 : 6, 'ram', (p.x + e.x) / 2, (p.y + e.y) / 2);
        hurtPlayer(S, 3, e.x, e.y);
      } else {
        var dmg = e.kind === 'rammer' && e.st === 'charge' ? 18 : heavy ? 14 : 9;
        hurtPlayer(S, dmg, (p.x + e.x) / 2, (p.y + e.y) / 2);
        damage(S, e, k.civ ? 2 : 1, k.civ ? 'player' : 'bump');
      }
    }
    // enemy cars plough through traffic: a charging rammer or a heavy shoves and wrecks civilians
    for (i = 0; i < ents.length; i++) {
      e = ents[i]; k = KIND[e.kind];
      if (e.dead || e.gone || !k.foe || k.air) continue;
      for (var j = 0; j < ents.length; j++) {
        var o = ents[j], ko = KIND[o.kind];
        if (o === e || o.dead || o.gone || ko.pickup || ko.hazard || ko.air || o.kind === 'boss' || o.st === 'fly' || e.st === 'fly') continue;
        if (!overlap(e, o, 2)) continue;
        var ddx = o.x - e.x || 0.01;
        if ((e.kind === 'rammer' && e.st === 'charge') || k.big) {
          if (ko.civ && !ko.ramp) { o.hp = 0; kill(S, o, 'enemy'); }
          else if (!ko.ramp) { o.x += sign(ddx) * 3; }
          if (e.kind === 'rammer' && ko.big) { e.hp = 0; kill(S, e, 'enemy'); }
        } else {
          var pp = ((e.w + o.w) / 2 - Math.abs(ddx)) / 2 + 0.5;
          if (!ko.ramp && o.kind !== 'bus') o.x += sign(ddx) * pp;
          e.x -= sign(ddx) * pp;
        }
      }
      e.x = clamp(e.x, -EDGE + e.w / 2, EDGE - e.w / 2);
    }
  }

  function updateShots(S, dt) {
    var i, j, e, b, ents = S.ents, p = S.p;
    for (i = S.bullets.length - 1; i >= 0; i--) {
      b = S.bullets[i]; b.py = b.y; b.y += b.vy * dt;
      var hit = null, best = -1e9;
      for (j = 0; j < ents.length; j++) {
        e = ents[j];
        if (e.dead || e.gone || KIND[e.kind].pickup || e.kind === 'hench') continue; // a man on a rope is above the guns
        if (sweptHit(b.x, b.py, b.y, e, 1) && e.y + e.h / 2 > best) { best = e.y + e.h / 2; hit = e; }
      }
      if (hit) { damage(S, hit, 1, 'gun', b.x, hit.y + hit.h / 2 - 2); S.bullets.splice(i, 1); continue; }
      if (b.y < -40) S.bullets.splice(i, 1);
    }
    for (i = S.missiles.length - 1; i >= 0; i--) {
      var m = S.missiles[i];
      m.life -= dt;
      if (!m.target || m.target.dead || m.target.gone || m.target.y > m.y) {
        m.target = null; var bd = 1e9;
        for (j = 0; j < ents.length; j++) {
          e = ents[j];
          if (e.dead || e.gone || !KIND[e.kind].foe || e.y > m.y - 10) continue;
          var d = Math.abs(e.x - m.x) * 2 + (m.y - e.y);
          if (d < bd) { bd = d; m.target = e; }
        }
      }
      if (m.target) {
        var want = Math.atan2(m.target.x - m.x, m.y - m.target.y);
        m.ang = approach(m.ang, clamp(want, -1.1, 1.1), 4.2 * dt);
      }
      m.spd = Math.min(980, m.spd + 1500 * dt);
      m.px = m.x; m.py = m.y;
      m.x += Math.sin(m.ang) * m.spd * dt; m.y -= Math.cos(m.ang) * m.spd * dt;
      var mh = null;
      for (j = 0; j < ents.length; j++) {
        e = ents[j];
        if (e.dead || e.gone || KIND[e.kind].pickup) continue;
        if (Math.abs(m.x - e.x) < e.w / 2 + 4 && sweptHit(e.x, m.py, m.y, e, 0)) { mh = e; break; }
      }
      if (mh || m.life <= 0 || m.y < -60) {
        S.missiles.splice(i, 1);
        if (m.y < -60 && !mh) continue;
        var ex = mh ? m.x : m.x, ey = mh ? Math.max(m.y, mh.y + mh.h / 2 - 6) : m.y;
        ev(S, { t: 'explode', x: ex, y: ey, size: 1.1, missile: true });
        shake(S, 0.3);
        if (mh) damage(S, mh, 9, 'missile', ex, ey);
        for (j = 0; j < ents.length; j++) {
          e = ents[j];
          if (e === mh || e.dead || e.gone || KIND[e.kind].pickup) continue;
          if (Math.abs(e.x - ex) < 46 + e.w / 2 && Math.abs(e.y - ey) < 46 + e.h / 2) damage(S, e, 3, 'splash');
        }
      }
    }
    for (i = S.ebullets.length - 1; i >= 0; i--) {
      b = S.ebullets[i];
      b.x += b.vx * dt; b.y += b.vy * dt; b.life -= dt;
      if (Math.abs(b.x - p.x) < p.w / 2 - 2 && Math.abs(b.y - p.y) < p.h / 2 - 2 && p.air <= 0 && S.phase === 'play') {
        S.ebullets.splice(i, 1); hurtPlayer(S, 6, b.x, b.y); continue;
      }
      if (b.life <= 0 || b.y > H + 20 || b.y < -40 || Math.abs(b.x) > 600) S.ebullets.splice(i, 1);
    }
  }

  function civilians(S, dt) {
    S.civT -= dt;
    if (S.civT > 0) return;
    var r = S.rnd, busy = S.boss ? 1.6 : 1;
    S.civT = (0.9 + r() * 1.3) * busy;
    var x = r(), kind = x < 0.5 ? 'sedan' : x < 0.75 ? 'hatch' : x < 0.93 ? 'van' : 'bus';
    var e = spawn(S, kind);
    // never wall off all four lanes: a lane with a car near the top already is skipped
    for (var tries = 0; tries < 4; tries++) {
      var clear = true;
      for (var i = 0; i < S.ents.length; i++) { var o = S.ents[i]; if (o !== e && Math.abs(o.x - e.x) < 40 && o.y < 140) clear = false; }
      if (clear) break;
      e.x = LANES[(LANES.indexOf(e.x) + 1) % 4];
    }
  }

  function step(S, inp, dt) {
    inp = inp || {};
    // real-time things: shake, slow-mo timers, the freeze
    S.trauma = Math.max(0, S.trauma - 1.3 * dt);
    if (S.freezeT > 0) { S.freezeT -= dt; S.timeScale = 0; return; }
    if (S.slowT > 0) { S.slowT -= dt; if (S.slowT <= 0) S.slowScale = 1; }
    if (S.slowCd > 0) S.slowCd -= dt;
    var targetScale = S.slowT > 0 ? S.slowScale : 1;
    S.timeScale = approach(S.timeScale || 1, targetScale, (targetScale < S.timeScale ? 12 : 3) * dt);
    var g = dt * S.timeScale;
    S.t += g;
    if (S.capT > 0) { S.capT -= dt; if (S.capT <= 0) S.caption = null; }
    if (S.comboT > 0) { S.comboT -= g; if (S.comboT <= 0) S.combo = 1; }
    if (S.phase === 'done' || S.phase === 'retake') return;

    updatePlayer(S, S.phase === 'play' ? inp : {}, g, dt);
    S.dist += S.speed * g;
    setPieces(S, g);
    while (S.phase === 'play' && S.si < S.script.length && S.dist >= S.script[S.si].d) {
      var it = S.script[S.si++];
      if (it.cap) caption(S, it.cap);
      if (it.scene !== undefined) enterScene(S, it.scene, S.si - 1);
      if (it.spawn) for (var n = 0; n < it.spawn.length; n++) spawn(S, it.spawn[n], n);
    }
    if (S.phase === 'play') civilians(S, g);
    for (var i = 0; i < S.ents.length; i++) if (!S.ents[i].dead) updateEnt(S, S.ents[i], g);
    if (S.hz.length) hazards(S, g);
    collide(S);
    updateShots(S, g);
    // 1.7.2: compacted in place (a new array every step was 240 arrays a second of garbage)
    for (var w = 0, j = 0; j < S.ents.length; j++) { var ej = S.ents[j]; if (!ej.dead && !ej.gone) S.ents[w++] = ej; }
    S.ents.length = w;

    if (S.boss) S.progress = 0.85 + 0.15 * (1 - Math.max(0, S.boss.hp) / S.boss.max);
    else S.progress = Math.min(S.dist / SCENE_LEN, 1) * 0.85;
    if (S.phase !== 'play') {
      if (S.outcome === 'wrapped') S.progress = 1;
      S.wrapT -= dt;
      if (S.wrapT <= 0) {
        if (S.cutAgain) { S.phase = 'retake'; ev(S, { t: 'retakeReady', n: S.scene }); }
        else finish(S);
      }
    }
  }

  function finish(S) {
    S.phase = 'done';
    var idx = 0;
    for (var i = 0; i < RATING_AT.length; i++) if (S.score >= RATING_AT[i]) idx = i;
    if (S.outcome === 'wrecked') idx = 0;
    if (S.outcome === 'escaped') idx = Math.min(idx, 1);
    S.rating = RATINGS[idx];
    ev(S, { t: 'done', outcome: S.outcome, rating: S.rating.key });
  }

  root.FT_ENGINE = {
    create: create, step: step, retake: retake, SCENES: SCENES, REELS: REELS, KIND: KIND, SCENE: SCENE, RATINGS: RATINGS, RATING_AT: RATING_AT,
    H: H, LANES: LANES, ROAD_HALF: ROAD_HALF, EDGE: EDGE, SCENE_LEN: SCENE_LEN, PY: PY, mulberry32: mulberry32,
    curveAt: curveAt, bankAt: bankAt, FEATS: TR.FEATS, LEVELS: LEVELS, useLevel: useLevel, level: 0
  };
  useLevel(0);
})(typeof window !== 'undefined' ? window : this);
