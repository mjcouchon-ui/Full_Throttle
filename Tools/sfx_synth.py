"""FULL THROTTLE sound effects, synthesized like the 1.x Web Audio ones (original sounds, no samples).

Writes 16-bit mono WAVs to Saved/SFXWav; the editor imports them to /Game/Audio/SFX.
Usage: python sfx_synth.py <out_dir>          (needs numpy: Blender's Python has it, the PC's own does not)

The overhaul of 15 Nov 2026 (the block at the end) adds the menus' and results' cues and the net-new takes. It writes
<out_dir>/overhaul.txt, the NEW names it made; `import ft_audio; ft_audio.build(<out_dir>)` in the editor
(Content/Python/ft_audio.py) imports exactly those. A sound the game already has never changes (owner's rule, the
revert of 15 Nov, commit ff773438): the block's heavier versions of ten 2.x sounds are OFFERS, written to
<out_dir>/offered/ and to no list, so <out_dir>/S_Explode.wav and the other nine stay the 2.x files.
"""
import os, sys, wave
import numpy as np

SR = 44100
OUT = sys.argv[1]
os.makedirs(OUT, exist_ok=True)
rng = np.random.default_rng(88)


def t(d):
    return np.arange(int(SR * d)) / SR


def env(n, a=0.005, r=None, curve=3.0):
    x = np.linspace(0, 1, n)
    e = np.minimum(1.0, x * (n / SR) / max(a, 1e-4))
    return e * (1 - x) ** curve if r is None else e


def noise(d):
    return rng.uniform(-1, 1, int(SR * d))


def lp(x, cut):
    a = np.exp(-2 * np.pi * cut / SR)
    y = np.zeros_like(x)
    acc = 0.0
    for i, v in enumerate(x):
        acc = (1 - a) * v + a * acc
        y[i] = acc
    return y


def lp_fast(x, cut):
    # cascaded moving averages: cheap low-pass for long buffers
    k = max(1, int(SR / (cut * 2.0)))
    ker = np.ones(k) / k
    return np.convolve(np.convolve(x, ker, 'same'), ker, 'same')


def hp(x, cut):
    return x - lp_fast(x, cut)


def sweep(f0, f1, d, wave_='sin', curve=1.0):
    tt = t(d)
    f = f0 + (f1 - f0) * (tt / d) ** curve
    ph = 2 * np.pi * np.cumsum(f) / SR
    if wave_ == 'sin':
        return np.sin(ph)
    if wave_ == 'saw':
        return 2 * ((ph / (2 * np.pi)) % 1.0) - 1
    if wave_ == 'square':
        return np.sign(np.sin(ph))
    return np.abs(2 * ((ph / (2 * np.pi)) % 1.0) - 1) * 2 - 1


def tone(freqs, d, wave_='square', dec=3.0):
    out = np.zeros(int(SR * d))
    for f in freqs:
        out += sweep(f, f, d, wave_)
    return out / len(freqs) * env(len(out), 0.003, None, dec)


def seq(parts):
    return np.concatenate(parts)


def write(name, x, gain=0.8, loop=False):
    x = np.asarray(x, dtype=np.float64)
    if not loop:
        fade = min(len(x), int(0.01 * SR))
        x[-fade:] *= np.linspace(1, 0, fade)
    peak = np.max(np.abs(x)) or 1.0
    x = np.clip(x / peak * gain, -1, 1)
    with wave.open(os.path.join(OUT, 'S_' + name + '.wav'), 'wb') as w:
        w.setnchannels(1); w.setsampwidth(2); w.setframerate(SR)
        w.writeframes((x * 32767).astype('<i2').tobytes())


def boom(d=1.6, cut=900, sub=48):
    n = noise(d)
    body = lp_fast(n, cut) * env(len(n), 0.002, None, 2.2)
    thump = sweep(sub * 2.2, sub * 0.6, d, 'sin', 0.4) * env(len(n), 0.002, None, 4.0)
    crack = hp(noise(d), 2000) * env(len(n), 0.001, None, 30.0) * 0.6
    return body * 1.2 + thump * 1.4 + crack


# ---- weapons and hits
write('Gun', hp(noise(0.07), 900) * env(int(0.07 * SR), 0.001, None, 5) + sweep(420, 120, 0.07, 'square') * env(int(0.07 * SR), 0.001, None, 6) * 0.5, 0.6)
write('EShot', sweep(900, 300, 0.12, 'square') * env(int(0.12 * SR), 0.001, None, 4), 0.5)
write('EShotBig', (sweep(600, 120, 0.25, 'saw') + hp(noise(0.25), 600) * 0.6) * env(int(0.25 * SR), 0.002, None, 3), 0.7)
write('Missile', (lp_fast(noise(0.7), 2500) + sweep(200, 900, 0.7, 'saw') * 0.3) * env(int(0.7 * SR), 0.01, None, 1.8), 0.7)
write('Explode', boom(), 0.95)
write('Wreck', seq([boom(2.2, 700, 40)]), 0.95)
write('HeliDown', seq([sweep(500, 80, 0.8, 'saw') * env(int(0.8 * SR), 0.01, None, 1.0) * 0.5, boom(1.6, 800, 44)]), 0.95)
write('Hit', (hp(noise(0.06), 1500) + sweep(1400, 600, 0.06, 'square') * 0.4) * env(int(0.06 * SR), 0.001, None, 4), 0.45)
write('PlayerHit', (lp_fast(noise(0.25), 1200) + sweep(160, 60, 0.25, 'square') * 0.8) * env(int(0.25 * SR), 0.001, None, 3), 0.8)
write('Bump', (lp_fast(noise(0.2), 600) + sweep(110, 50, 0.2, 'sin')) * env(int(0.2 * SR), 0.001, None, 3), 0.8)
write('Rail', hp(noise(0.4), 2500) * (0.6 + 0.4 * np.sin(2 * np.pi * 38 * t(0.4))) * env(int(0.4 * SR), 0.01, None, 1.5), 0.5)
write('Near', (lp_fast(noise(0.5), 1800) * np.sin(np.pi * np.linspace(0, 1, int(0.5 * SR))) ** 2 + sweep(700, 300, 0.5, 'sin') * 0.2), 0.6)
write('Turbo', (sweep(120, 700, 0.6, 'saw') * 0.6 + lp_fast(noise(0.6), 3000) * 0.5) * env(int(0.6 * SR), 0.02, None, 1.2), 0.7)
write('Boost', (sweep(300, 1200, 0.35, 'saw') * 0.5 + hp(noise(0.35), 1500) * 0.4) * env(int(0.35 * SR), 0.01, None, 1.5), 0.6)
write('Jump', sweep(180, 620, 0.35, 'square') * env(int(0.35 * SR), 0.005, None, 2), 0.5)
write('Land', (lp_fast(noise(0.3), 500) + sweep(90, 40, 0.3, 'sin')) * env(int(0.3 * SR), 0.001, None, 3), 0.85)
write('Pickup', seq([tone([880], 0.07), tone([1175], 0.07), tone([1568], 0.12)]), 0.55)
write('Empty', seq([tone([220], 0.08, 'square'), tone([180], 0.12, 'square')]), 0.5)
write('Rev', (sweep(90, 340, 0.6, 'saw') + sweep(91, 342, 0.6, 'square') * 0.5) * env(int(0.6 * SR), 0.05, None, 1.0), 0.6)
write('PartOff', (hp(noise(0.35), 1200) * 0.7 + sweep(900, 200, 0.35, 'square') * 0.3) * env(int(0.35 * SR), 0.001, None, 2.5), 0.6)
write('DriftStart', hp(noise(0.25), 1600) * env(int(0.25 * SR), 0.02, None, 1.5), 0.45)
write('DriftLevel', tone([880, 1320], 0.18, 'sin'), 0.5)
write('Collateral', seq([tone([400], 0.12, 'saw'), tone([300], 0.2, 'saw')]), 0.55)
# ---- screens and the movie layer
clap = hp(noise(0.12), 1200) * env(int(0.12 * SR), 0.0005, None, 18) + sweep(2200, 900, 0.12, 'sin') * env(int(0.12 * SR), 0.0005, None, 20) * 0.5
write('Clap', clap, 0.9)
write('Sting', seq([tone([392, 494, 587], 0.18, 'saw', 1.5), tone([523, 659, 784], 0.6, 'saw', 2.0)]), 0.6)
write('Stunt', seq([tone([f], 0.06, 'square', 1.0) for f in (523, 659, 784, 1046)]), 0.5)
write('Slowmo', sweep(600, 80, 0.8, 'saw') * env(int(0.8 * SR), 0.01, None, 1.0) * 0.6 + lp_fast(noise(0.8), 400) * 0.3, 0.55)
write('Wrap', seq([tone([523, 659, 784], 0.25, 'saw', 1.0), tone([587, 740, 880], 0.25, 'saw', 1.0), tone([659, 831, 988], 0.8, 'saw', 1.5)]), 0.6)
write('BossIntro', seq([tone([110, 165], 0.5, 'saw', 0.5), tone([104, 156], 0.5, 'saw', 0.5), tone([98, 147], 1.0, 'saw', 1.2)]), 0.7)
write('BossPhase', (sweep(80, 400, 0.8, 'saw') + sweep(82, 404, 0.8, 'square') * 0.5) * env(int(0.8 * SR), 0.01, None, 1.2), 0.6)
# ---- the Alchemist's lightning
crackle = hp(noise(0.4), 3000) * (rng.uniform(0, 1, int(0.4 * SR)) > 0.7)
write('ZapWarn', (sweep(300, 1600, 0.5, 'square') * 0.4 + hp(noise(0.5), 4000) * 0.3) * env(int(0.5 * SR), 0.01, None, 0.6), 0.5)
write('Zap', (crackle * 1.2 + boom(0.4, 2500, 90) * 0.5), 0.8)
write('ZoneWarn', seq([tone([1200], 0.08, 'square'), np.zeros(int(0.05 * SR)), tone([1200], 0.08, 'square'), np.zeros(int(0.05 * SR)), tone([1600], 0.12, 'square')]), 0.5)
write('Strike', (crackle[:int(0.3 * SR)] + boom(0.3, 2000, 80)), 0.85)
write('RingWarn', sweep(200, 900, 0.7, 'sin') * (0.5 + 0.5 * np.sin(2 * np.pi * 14 * t(0.7))) * env(int(0.7 * SR), 0.01, None, 0.8), 0.55)
write('Shock', (crackle + sweep(150, 60, 0.4, 'square') * 0.6), 0.8)
write('Dodge', seq([tone([784], 0.06, 'sin'), tone([1175], 0.12, 'sin')]), 0.5)
# ---- henchmen, chopper, Snake Eyes, carrier
write('HenchDrop', sweep(800, 300, 0.5, 'sin') * 0.4 + lp_fast(noise(0.5), 1200) * env(int(0.5 * SR), 0.01, None, 1.0) * 0.4, 0.5)
write('Latch', (lp_fast(noise(0.15), 900) + sweep(200, 90, 0.15, 'square') * 0.5) * env(int(0.15 * SR), 0.001, None, 3), 0.7)
write('HenchHit', (lp_fast(noise(0.1), 700) + sweep(160, 90, 0.1, 'sin')) * env(int(0.1 * SR), 0.001, None, 4), 0.6)
write('HenchOff', seq([(hp(noise(0.2), 1200) + sweep(300, 100, 0.2, 'square') * 0.5) * env(int(0.2 * SR), 0.001, None, 2), tone([659, 988], 0.15, 'sin')]), 0.7)
write('ChainWarn', hp(noise(0.6), 2500) * (0.5 + 0.5 * np.sign(np.sin(2 * np.pi * 7 * t(0.6)))) * env(int(0.6 * SR), 0.02, None, 0.5), 0.5)
write('Chain', (hp(noise(0.2), 2000) + sweep(3000, 1200, 0.2, 'square') * 0.3) * env(int(0.2 * SR), 0.001, None, 3), 0.7)
write('HogOff', (lp_fast(noise(0.35), 900) + sweep(400, 90, 0.35, 'saw') * 0.5) * env(int(0.35 * SR), 0.001, None, 2), 0.7)
write('HogJump', (sweep(100, 300, 0.5, 'saw') + lp_fast(noise(0.5), 1500) * 0.4) * env(int(0.5 * SR), 0.02, None, 1.2), 0.6)
write('HogLand', (lp_fast(noise(0.25), 600) + sweep(120, 50, 0.25, 'sin')) * env(int(0.25 * SR), 0.001, None, 3), 0.8)
write('BrakeWarn', seq([tone([440, 466], 0.15, 'square'), np.zeros(int(0.05 * SR)), tone([440, 466], 0.15, 'square')]), 0.55)
# ---- loops: engine, rotor, skid (seamless)
d = 2.0
tt = t(d)
eng = (2 * ((tt * 55) % 1) - 1) * 0.6 + np.sign(np.sin(2 * np.pi * 27.5 * tt)) * 0.3 + (2 * ((tt * 110.5) % 1) - 1) * 0.15
eng = lp_fast(eng, 900) * (0.85 + 0.15 * np.sin(2 * np.pi * 8 * tt))
write('EngineLoop', eng, 0.55, loop=True)
rot = lp_fast(noise(d), 420) * (0.35 + 0.65 * (0.5 + 0.5 * np.sin(2 * np.pi * 11 * tt)))
write('Rotor', rot, 0.7, loop=True)
skid = hp(lp_fast(noise(d), 2600), 900) * (0.8 + 0.2 * np.sin(2 * np.pi * 23 * tt))
write('Skid', skid, 0.5, loop=True)


# ================================================================================================ the overhaul (15 Nov 2026)
# Menus, results and the HUD's own motion get their ticks, sweeps and stings; the sounds of the things the effects pass
# made heavier (the missile, blasts, hits, rams, landings, the bosses' cues) get weight. Rules of this block:
#   - each sound has its own noise, seeded by its name (gen): adding or changing one never changes another, and none of
#     the 2.x sounds above changes (they still draw from the one generator, in their old order);
#   - a heavier version of a 2.x sound keeps the 2.x layers, to the same recipe, and adds to them: the game keeps its
#     voice and gains body. The weight is put between 80 and 250 Hz and in the tail, where small speakers still speak
#     (the 2.x blast was a 105-to-29 Hz sine under quiet noise: all but silent on a laptop);
#   - peaks are rounded off (soft) rather than left sharp: louder for the same peak, and nothing is ever over the line;
#   - a few of the busiest cues have takes (S_Explode_B, S_Explode_C, S_Hit_B, S_Hit_C). They are in the project, but
#     the game plays exactly S_<Name> (AFTGameMode::Sfx): the code that picked a take went with the revert of 15 Nov;
#   - A SOUND THE GAME ALREADY HAS NEVER CHANGES (owner's rule; the ten that this block once replaced were put back
#     by commit ff773438). So a net-new sound goes through over(): <out_dir>/S_<name>.wav and overhaul.txt. A heavier
#     version of an existing one (EXISTING) goes through offer(): <out_dir>/offered/S_<name>.wav and no list, so the
#     2.x file of that name survives the run and the default import cannot reach the offer. over() refuses those names.
# The names over() wrote are listed in <out_dir>/overhaul.txt; Content/Python/ft_audio.py imports exactly that list.
# (Same vocabulary as the nights' cues, beat-em-up-unreal/ArtSource/tools/brawl_combat_sources.py: noise bursts,
# sines that fall in pitch, short exponential tails.)
import zlib

OVERHAUL = []
# the 2.x sounds this block has a heavier version of: offered, never written over the 2.x file, never listed
EXISTING = ('Explode', 'Wreck', 'HeliDown', 'Missile', 'Hit', 'PlayerHit', 'Bump', 'Land', 'BossIntro', 'BossPhase')
OFFERED = os.path.join(OUT, 'offered')


def gen(name):
    return np.random.default_rng(zlib.crc32(name.encode('ascii')))


def nz(g, d):
    return g.uniform(-1.0, 1.0, int(SR * d))


def unit(x):
    return x / (np.max(np.abs(x)) or 1.0)


def fir(x, lo=None, hi=None, taps=None):
    """A linear-phase band filter (windowed sinc). lo and hi in Hz; either may be None."""
    edge = min(v for v in (lo, hi) if v is not None)
    if taps is None:
        taps = int(min(2047, max(255, 5.0 * SR / edge))) | 1
    n = np.arange(taps) - (taps - 1) / 2.0
    w = np.blackman(taps)

    def low(fc):
        h = np.sinc(2.0 * fc / SR * n) * w
        return h / h.sum()

    if lo is not None and hi is not None:
        h = low(hi) - low(lo)
    elif hi is not None:
        h = low(hi)
    else:
        h = -low(lo)
        h[(taps - 1) // 2] += 1.0
    return np.convolve(x, h, 'same')


def dec(d, tau):
    return np.exp(-t(d) / tau)


def glide(f0, f1, d, tau):
    """A sine whose pitch goes from f0 to f1 (time constant tau): a thump falls, a ricochet climbs."""
    tt = t(d)
    f = f1 + (f0 - f1) * np.exp(-tt / tau)
    return np.sin(2 * np.pi * np.cumsum(f) / SR)


def tri(f, d):
    """A soft triangle: the sine and two odd partials."""
    tt = t(d)
    return np.sin(2 * np.pi * f * tt) - np.sin(2 * np.pi * 3 * f * tt) / 9.0 + np.sin(2 * np.pi * 5 * f * tt) / 25.0


def at(total, *parts):
    """Mix parts into one buffer of total seconds: (start seconds, signal, gain)."""
    out = np.zeros(int(SR * total))
    for start, sig, gain in parts:
        i = int(SR * start)
        k = min(len(sig), len(out) - i)
        if k > 0:
            out[i:i + k] += sig[:k] * gain
    return out


def soft(x, drive=1.5):
    """Round the peaks off: louder for the same peak, and never over it."""
    return np.tanh(unit(x) * drive) / np.tanh(drive)


def over(name, x, gain, loop=False):
    """A net-new sound: <out_dir>/S_<name>.wav, and its name into overhaul.txt (what ft_audio.build imports).
    loop: a seamless loop (write() then leaves its end unfaded)."""
    if name in EXISTING:
        raise ValueError('S_%s is a sound the game already has: offer() it, never over() it' % name)
    x = np.asarray(x, dtype=np.float64)
    write(name, x - np.mean(x), gain, loop)
    OVERHAUL.append('S_' + name)


def offer(name, x, gain):
    """A heavier version of a sound the game already has: <out_dir>/offered/S_<name>.wav only. Not over the 2.x file
    of that name and not in overhaul.txt; it goes into the game only if the owner takes the offer, by hand."""
    global OUT
    x = np.asarray(x, dtype=np.float64)
    os.makedirs(OFFERED, exist_ok=True)
    keep, OUT = OUT, OFFERED      # write() takes its folder from OUT; the 2.x block and write() are left as they are
    try:
        write(name, x - np.mean(x), gain)
    finally:
        OUT = keep


# ---- menus: a tick for a move, two notes up for a choice, two down for going back, a buzz for a refusal
def ui_tick():
    g = gen('UiTick')
    d = 0.05
    tt = t(d)
    return (np.sin(2 * np.pi * 1320 * tt) * np.exp(-tt / 0.008) + 0.22 * np.sin(2 * np.pi * 2640 * tt) * np.exp(-tt / 0.004)
            + 0.35 * fir(nz(g, d), 2500, 9000) * np.exp(-tt / 0.0012))


def two_notes(f1, f2, gap=0.05, tau=0.03, d=0.2):
    rise = np.minimum(1.0, t(d) / 0.002)
    return at(d + gap, (0, tri(f1, d) * dec(d, tau) * rise, 1.0), (gap, tri(f2, d) * dec(d, tau * 1.3) * rise, 1.0))


def ui_deny():
    def buzz(f, d):
        e = np.minimum(1.0, t(d) / 0.003) * np.minimum(1.0, (d - t(d)) / 0.012)
        return fir(sweep(f, f, d, 'square'), None, 1500) * e
    return at(0.22, (0, buzz(165, 0.07), 1.0), (0.095, buzz(147, 0.11), 1.0))


def ui_tab():
    g = gen('UiTab')
    d = 0.035
    swish = fir(nz(g, d), 1500, 7000) * np.sin(np.pi * np.linspace(0, 1, int(SR * d))) ** 2
    tick = np.sin(2 * np.pi * 1760 * t(0.06)) * dec(0.06, 0.012)
    return at(0.1, (0, unit(swish), 0.6), (0.028, tick, 1.0))


def ui_open():
    """A screen arriving: a short rising whoosh (three bands of noise, each loudest a little later than the last)."""
    g = gen('UiOpen')
    d = 0.28
    u = np.linspace(0, 1, int(SR * d))

    def band(lo, hi, c):
        return unit(fir(nz(g, d), lo, hi)) * np.exp(-((u - c) / 0.2) ** 2)

    whoosh = band(350, 900, 0.3) * 0.9 + band(900, 2200, 0.5) + band(2200, 5000, 0.68) * 0.7
    tone = np.sin(2 * np.pi * np.cumsum(280 + 300 * u ** 1.5) / SR) * np.sin(np.pi * u) ** 2 * 0.3
    return (whoosh + tone) * np.sin(np.pi * np.clip(u * 1.02, 0, 1)) ** 0.5


over('UiTick', ui_tick(), 0.7)
over('UiSelect', two_notes(880.0, 1318.5), 0.7)
over('UiBack', two_notes(987.8, 659.3), 0.65)
over('UiDeny', ui_deny(), 0.38)
over('UiTab', ui_tab(), 0.65)
over('UiOpen', ui_open(), 0.5)


# ---- results: the rating's stamp, the money counting, a level gained, CUT!, a splice in the reel
def stamp(name='Stamp'):
    g = gen(name)
    d = 0.4
    thud = glide(150, 50, d, 0.035) * dec(d, 0.075)
    knock = unit(fir(nz(g, 0.08), 250, 1500) * dec(0.08, 0.010))
    slap = unit(fir(nz(g, 0.08), 3000, 9000) * dec(0.08, 0.011))
    board = unit(fir(nz(g, 0.3), 120, 700) * dec(0.3, 0.06))
    return soft(at(d, (0, thud, 1.0), (0, knock, 1.0), (0.003, slap, 0.6), (0.008, board, 0.35)), 1.4)


def stamp_gold():
    """BLOCKBUSTER and LEGENDARY DIRECTOR: the stamp, and a bright chord rings off it."""
    d = 1.0
    tt = t(d)
    ring = sum(a * (np.sin(2 * np.pi * f * tt) + 0.5 * np.sin(2 * np.pi * f * 1.004 * tt)) * np.exp(-tt / tau)
               for f, a, tau in ((1046.5, 1.0, 0.34), (1318.5, 0.8, 0.30), (1568.0, 0.7, 0.26), (2093.0, 0.45, 0.2)))
    ring = unit(ring * np.minimum(1.0, tt / 0.004))
    return at(d, (0, stamp('StampGold'), 1.0), (0.02, ring, 0.3))


def cash_tick(f=2400.0, g=None):
    g = g or gen('CashTick')
    d = 0.035
    tt = t(d)
    return (np.sin(2 * np.pi * f * tt) * np.exp(-tt / 0.005) + 0.4 * np.sin(2 * np.pi * f * 1.5 * tt) * np.exp(-tt / 0.003)
            + 0.4 * unit(fir(nz(g, d), 4000, 12000)) * np.exp(-tt / 0.0009))


def cash_roll():
    """A total counting up for 0.55 s: a counter's ticks, quick at first and slowing, as the figure does on screen
    (FTHUD.cpp counts with an ease-out cubic; the ticks fall on equal steps of that count)."""
    g = gen('CashRoll')
    total, n = 0.55, 22
    parts = []
    for k in range(n):
        when = total * (1.0 - (1.0 - (k + 0.5) / n) ** (1.0 / 3.0))
        parts.append((when, cash_tick(2100.0 + 32.0 * k, g), 0.55 + 0.45 * k / n))
    return at(total + 0.04, *parts)


def cash_total():
    """The total lands: a till's drawer and its bell."""
    g = gen('CashTotal')
    d = 0.8
    tt = t(d)
    thunk = glide(170, 85, 0.2, 0.03) * dec(0.2, 0.045)
    knock = unit(fir(nz(g, 0.06), 200, 1400) * dec(0.06, 0.008))
    bell = sum(a * np.sin(2 * np.pi * f * tt) * np.exp(-tt / tau)
               for f, a, tau in ((2093.0, 1.0, 0.30), (2637.0, 0.55, 0.24), (3136.0, 0.5, 0.2), (4186.0, 0.3, 0.13), (5274.0, 0.15, 0.08)))
    bell = unit(bell * np.minimum(1.0, tt / 0.002))
    coins = at(d, *[(0.07 + 0.03 * k + 0.01 * g.random(), unit(fir(nz(g, 0.03), 5000, 11000) * dec(0.03, 0.004)), 0.5 - 0.06 * k) for k in range(6)])
    return at(d, (0, thunk, 0.9), (0, knock, 0.6), (0.012, bell, 0.5), (0, coins, 0.25))


def level_up():
    d = 0.9

    def note(f, tau, dd):
        tt = t(dd)
        return (tri(f, dd) + 0.3 * np.sin(2 * np.pi * 2 * f * tt)) * np.exp(-tt / tau) * np.minimum(1.0, tt / 0.005)

    last = note(1046.5, 0.28, 0.65) + 0.25 * np.sin(2 * np.pi * 2093.0 * 1.004 * t(0.65)) * dec(0.65, 0.3)
    return at(d, (0, note(523.25, 0.08, 0.3), 1.0), (0.07, note(659.25, 0.08, 0.3), 1.0), (0.14, note(783.99, 0.08, 0.3), 1.0), (0.21, last, 1.0))


def cut_slam():
    """CUT!: the slate shut hard (the 2.x clap, to the same recipe), with the thump of the card landing and a short room."""
    g = gen('CutSlam')
    d = 0.55
    crack = unit(fir(nz(g, 0.12), 1200, 10000) * dec(0.12, 0.007) * 2.0 + 0.5 * sweep(2200, 900, 0.12, 'sin') * dec(0.12, 0.006))
    thump = glide(140, 55, 0.3, 0.035) * dec(0.3, 0.07)
    room = unit(fir(nz(g, 0.5), 300, 2600) * dec(0.5, 0.075))
    return soft(at(d, (0, crack, 1.0), (0, thump, 0.7), (0.012, room, 0.3)), 1.3)


def reel_click():
    g = gen('ReelClick')

    def click():
        return unit(fir(nz(g, 0.02), 2000, 9000) * dec(0.02, 0.0012))

    low = np.sin(2 * np.pi * 140 * t(0.05)) * dec(0.05, 0.01)
    return at(0.09, (0, click(), 1.0), (0, low, 0.4), (0.038, click(), 0.7))


over('Stamp', stamp(), 0.9)
over('StampGold', stamp_gold(), 0.9)
over('CashTick', cash_tick(), 0.6)
over('CashRoll', cash_roll(), 0.5)
over('CashTotal', cash_total(), 0.8)
over('LevelUp', level_up(), 0.7)
over('CutSlam', cut_slam(), 0.9)
over('ReelClick', reel_click(), 0.5)


# ---- weight: the blasts
def explode(name, d=1.9, cut=900.0, sub=48.0, debris=1.0):
    g = gen(name)
    n = int(SR * d)
    tt = t(d)
    # the 2.x blast, layer for layer (boom): the fireball's body, the thump that falls through the floor, the crack
    body = unit(lp_fast(nz(g, d), cut) * env(n, 0.002, None, 2.2))
    thump = sweep(sub * 2.2, sub * 0.6, d, 'sin', 0.4) * env(n, 0.002, None, 4.0)
    crack = unit(hp(nz(g, d), 2000) * env(n, 0.001, None, 30.0))
    # added: the punch a small speaker can speak (190 to 70 Hz), a second push of air a moment behind the bang, more
    # of the fireball's roar, a low tail that rolls on, and debris coming down through it
    punch = glide(190, 70, 0.35, 0.05) * dec(0.35, 0.10)
    push = glide(90, 40, 0.6, 0.10) * dec(0.6, 0.18)
    roar = unit(fir(nz(g, d), 180, 1400) * np.exp(-tt / 0.28) * np.minimum(1.0, tt / 0.004))
    rumble = unit(lp_fast(nz(g, d), 170) * np.exp(-tt / 0.55))
    fall = (g.random(n) < 0.004 * debris * np.exp(-tt / 0.45)) * g.uniform(0.3, 1.0, n) * (tt > 0.1)
    crackle = unit(fir(fall, 900, 6000))
    return soft(thump * 1.1 + body * 1.0 + crack * 0.6 + at(d, (0, punch, 0.9), (0.03, push, 0.5)) + roar * 1.1 + rumble * 0.3 + crackle * 0.25, 1.5)


def wreck():
    """The KESTREL's own wreck: the big blast (2.x: boom(2.2, 700, 40)), with the car in it: panels, then glass."""
    g = gen('WreckMetal')
    d = 2.6

    def clang(f, tau, dd=0.5):
        tt = t(dd)
        return unit(sum(a * np.sin(2 * np.pi * f * r * tt) * np.exp(-tt * r / tau) for r, a in ((1.0, 1.0), (1.59, 0.7), (2.71, 0.5), (4.13, 0.3))))

    glass = at(1.2, *[(g.uniform(0, 0.9), unit(fir(nz(g, 0.05), 5000, 12000) * dec(0.05, 0.006)), g.uniform(0.3, 1.0)) for _ in range(14)])
    return soft(at(d, (0, explode('Wreck', 2.4, 700.0, 40.0, 1.6), 1.0), (0.03, clang(310.0, 0.09), 0.45), (0.16, clang(470.0, 0.07), 0.33),
                   (0.33, clang(260.0, 0.1), 0.27), (0.08, glass, 0.2)), 1.2)


def heli_down():
    """2.x: a falling whine, then the blast. The whine now beats like a rotor running down."""
    d1 = 0.8
    tt = t(d1)
    fall = sweep(500, 80, d1, 'saw') * env(int(d1 * SR), 0.01, None, 1.0) * (0.7 + 0.3 * np.sin(2 * np.pi * np.cumsum(14.0 - 9.0 * tt / d1) / SR))
    return seq([fall * 0.3, explode('HeliDown', 1.8, 800.0, 44.0, 1.4)])


offer('Explode', explode('Explode'), 0.9)
over('Explode_B', explode('Explode_B', 1.8, 800.0, 44.0, 1.3), 0.9)
over('Explode_C', explode('Explode_C', 2.0, 1000.0, 52.0, 0.8), 0.9)
offer('Wreck', wreck(), 0.92)
offer('HeliDown', heli_down(), 0.92)


# ---- weight: the missile, hits, rams, landings
def missile():
    g = gen('Missile')
    d = 1.0
    tt = t(d)
    e = np.minimum(1.0, tt / 0.02) * (1.0 - tt / d) ** 1.8
    roar = unit(lp_fast(nz(g, d), 2500) * e)                         # 2.x: the rocket's roar ...
    whine = unit(sweep(200, 900, d, 'saw') * e)                      # ... and its rising whine
    crack = unit(fir(nz(g, 0.06), 2500, 12000) * dec(0.06, 0.006))   # added: ignition,
    kick = glide(170, 55, 0.3, 0.05) * dec(0.3, 0.08)                # the kick off the rail,
    hiss = unit(fir(nz(g, d), 3500, 11000) * np.exp(-tt / 0.3) * np.minimum(1.0, tt / 0.03))   # and the exhaust it leaves
    return soft(roar + whine * 0.6 + at(d, (0, crack, 0.9), (0, kick, 1.0)) + hiss * 0.25, 1.4)


def hit(name, ping):
    """A round landing on a car that lives: the 2.x tick, and the metal it struck rings (three takes, three pitches)."""
    g = gen(name)
    d = 0.07
    tt = t(d)
    old = unit((hp(nz(g, 0.06), 1500) + sweep(1400, 600, 0.06, 'square') * 0.4) * env(int(0.06 * SR), 0.001, None, 4))
    tink = unit(np.sin(2 * np.pi * ping * tt) * np.exp(-tt / 0.012) + 0.5 * np.sin(2 * np.pi * ping * 1.48 * tt) * np.exp(-tt / 0.008))
    return at(d, (0, old, 1.0), (0, tink, 0.35))


def armor_hit():
    """A round skipping off armour (for FTFlow.cpp BulletHit's armour branch: not wired yet, see the overhaul notes)."""
    g = gen('ArmorHit')
    d = 0.16
    tt = t(d)
    ping = np.sin(2 * np.pi * np.cumsum(1900.0 + 900.0 * (tt / d) ** 0.5) / SR) * np.exp(-tt / 0.03)
    ring = 0.4 * np.sin(2 * np.pi * 3350.0 * tt) * np.exp(-tt / 0.02)
    spark = unit(fir(nz(g, 0.03), 3000, 11000) * dec(0.03, 0.004))
    return at(d, (0, unit(ping + ring), 1.0), (0, spark, 0.6))


def player_hit():
    g = gen('PlayerHit')
    d = 0.32
    old = unit((lp_fast(nz(g, 0.25), 1200) + sweep(160, 60, 0.25, 'square') * 0.8) * env(int(0.25 * SR), 0.001, None, 3))
    sub = glide(110, 42, d, 0.05) * dec(d, 0.09)
    crunch = unit(fir(nz(g, 0.12), 700, 3200) * dec(0.12, 0.02) * (0.6 + 0.4 * np.sign(np.sin(2 * np.pi * 38 * t(0.12)))))
    return soft(at(d, (0, old, 1.0), (0, sub, 0.8), (0.004, crunch, 0.5)), 1.4)


def bump():
    g = gen('Bump')
    d = 0.28
    tt = t(0.2)
    old = unit((lp_fast(nz(g, 0.2), 600) + sweep(110, 50, 0.2, 'sin')) * env(int(0.2 * SR), 0.001, None, 3))
    clank = unit(sum(a * np.sin(2 * np.pi * f * tt) * np.exp(-tt / tau) for f, a, tau in ((523.0, 1.0, 0.035), (831.0, 0.7, 0.028), (1340.0, 0.4, 0.018))))
    scrape = unit(fir(nz(g, 0.1), 1800, 6000) * dec(0.1, 0.02))
    return soft(at(d, (0, old, 1.0), (0, clank, 0.6), (0.002, scrape, 0.3)), 1.3)


def land():
    g = gen('Land')
    d = 0.42
    old = unit((lp_fast(nz(g, 0.3), 500) + sweep(90, 40, 0.3, 'sin')) * env(int(0.3 * SR), 0.001, None, 3))
    second = glide(75, 36, 0.25, 0.04) * dec(0.25, 0.07)             # the springs come back
    rattle = unit(fir(nz(g, 0.15), 1500, 5000) * dec(0.15, 0.03) * (0.5 + 0.5 * np.sign(np.sin(2 * np.pi * 55 * t(0.15)))))
    return soft(at(d, (0, old, 1.0), (0.085, second, 0.6), (0.01, rattle, 0.35)), 1.3)


offer('Missile', missile(), 0.75)
offer('Hit', hit('Hit', 2100.0), 0.5)
over('Hit_B', hit('Hit_B', 2480.0), 0.5)
over('Hit_C', hit('Hit_C', 1850.0), 0.5)
over('ArmorHit', armor_hit(), 0.5)
offer('PlayerHit', player_hit(), 0.85)
offer('Bump', bump(), 0.85)
offer('Land', land(), 0.88)


# ---- weight: the bosses' cues (the 2.x chords and sweep, with a drum under each and a sub an octave down)
def drum(g):
    return unit(glide(120, 45, 0.35, 0.04) * dec(0.35, 0.09) + 0.5 * unit(fir(nz(g, 0.35), 200, 1500) * dec(0.35, 0.012)))


def boss_intro():
    g = gen('BossIntro')
    old = unit(seq([tone([110, 165], 0.5, 'saw', 0.5), tone([104, 156], 0.5, 'saw', 0.5), tone([98, 147], 1.0, 'saw', 1.2)]))

    def sub(f, dd, curve):
        return np.sin(2 * np.pi * f * t(dd)) * env(int(dd * SR), 0.01, None, curve)

    return soft(at(len(old) / SR, (0, old, 1.0), (0, sub(55.0, 0.5, 0.5), 0.4), (0.5, sub(52.0, 0.5, 0.5), 0.4), (1.0, sub(49.0, 1.0, 1.2), 0.45),
                   (0, drum(g), 0.8), (0.5, drum(g), 0.8), (1.0, drum(g), 1.0)), 1.3)


def boss_phase():
    g = gen('BossPhase')
    d = 0.9
    tt = t(0.4)
    old = unit((sweep(80, 400, 0.8, 'saw') + sweep(82, 404, 0.8, 'square') * 0.5) * env(int(0.8 * SR), 0.01, None, 1.2))
    clang = unit(sum(a * np.sin(2 * np.pi * 233.0 * r * tt) * np.exp(-tt * r / 0.12) for r, a in ((1.0, 1.0), (1.59, 0.7), (2.71, 0.5), (4.13, 0.3))))
    return soft(at(d, (0, old, 1.0), (0, drum(g), 0.9), (0, clang, 0.25)), 1.3)


offer('BossIntro', boss_intro(), 0.8)
offer('BossPhase', boss_phase(), 0.7)


# ================================================================================================ the title's logo
# The animated FULL THROTTLE logo on the title menu (Source/FullThrottle/FTTitleLogo.cpp plays these, by name):
#   S_TitleSlam         "Full" thrown in from behind the camera: a whoosh that rises for 0.18 s, then the thud. The game
#                       starts it with the flight, so THE THUD'S ONSET IS 0.180 s INTO THE FILE: keep it there.
#   S_TitleWipe         the chrome wipe: a shimmer that climbs and ends as the wipe ends (0.36 s)
#   S_TitleTwinkle      one star's glint: a small bright bell (the second star plays it a tone up, by pitch)
#   S_TitleTwinklePair  both stars on a confirmed selection: the bell and its fifth, 45 ms apart
#   S_TitleNeonTick     the neon tube dipping: a tiny electrical tick
#   S_TitleNeonHum      the tube's hum under the menu: A SEAMLESS LOOP of exactly 4 s (Content/Python/ft_audio.py LOOPS)
# Same rules as the overhaul: each has its own noise, seeded by its name (gen), and nothing here draws from the 2.x
# generator, so no other sound changes. Import only these six:
#   ft_audio.build(<out_dir>, names=['S_TitleSlam', 'S_TitleWipe', 'S_TitleTwinkle', 'S_TitleTwinklePair',
#                                    'S_TitleNeonTick', 'S_TitleNeonHum'])
TITLE_HIT = 0.18      # seconds into S_TitleSlam at which the thud starts (the length of the flight of "Full")


def title_slam():
    g = gen('TitleSlam')
    d = 0.45 - TITLE_HIT
    u = np.linspace(0, 1, int(SR * TITLE_HIT))

    def band(lo, hi, enters):   # a band of the whoosh; the higher ones come in later, as the thing nears
        return unit(fir(nz(g, TITLE_HIT), lo, hi)) * np.clip((u - enters) / 0.3, 0, 1)

    whoosh = (band(300, 900, 0.0) + band(900, 2600, 0.2) * 0.8 + band(2600, 7000, 0.45) * 0.6) * u ** 2.2
    # the thud, after stamp(): a sine that falls, a knock, a slap
    thud = (glide(160, 48, d, 0.03) * dec(d, 0.07) + 0.5 * unit(fir(nz(g, d), 150, 900) * dec(d, 0.02))
            + 0.35 * unit(fir(nz(g, d), 2000, 8000) * dec(d, 0.006)))
    return soft(at(0.45, (0, unit(whoosh), 0.45), (TITLE_HIT, unit(thud), 1.0)), 1.4)


def title_wipe():
    """After ui_open(): three bands of noise, each loudest later than the last, and a sine that climbs with them.
    Nearly silent for its first 40 ms (it starts on the thud of S_TitleSlam), loudest near its end, gone in 15 ms."""
    g = gen('TitleWipe')
    d = 0.36
    tt = t(d)
    u = np.linspace(0, 1, len(tt))

    def band(lo, hi, c):
        return unit(fir(nz(g, d), lo, hi)) * np.exp(-((u - c) / 0.2) ** 2)

    shimmer = band(1500, 3500, 0.35) * 0.8 + band(3500, 6500, 0.6) + band(6500, 11000, 0.85) * 0.9
    tone = np.sin(2 * np.pi * np.cumsum(1200 + 2400 * u) / SR) * (0.5 + 0.5 * np.sin(2 * np.pi * 28 * tt)) * 0.3
    return (shimmer + tone) * u ** 1.5 * np.minimum(1.0, (d - tt) / 0.015)


def title_bell(g, f0, d):
    """A small bright bell: four partials that are not harmonics, the higher ones dying sooner, and the tap that rang it.
    (A partial that would fall above 20 kHz is left out: it would fold back as a wrong note.)"""
    tt = t(d)
    ring = sum(a * np.sin(2 * np.pi * f0 * r * tt) * np.exp(-tt / tau)
               for r, a, tau in ((1.0, 1.0, 0.11), (2.76, 0.5, 0.07), (5.40, 0.25, 0.04), (8.93, 0.12, 0.025)) if f0 * r < 20000.0)
    return unit(ring * np.minimum(1.0, tt / 0.002)) + 0.2 * unit(fir(nz(g, d), 6000, 12000) * dec(d, 0.003))


def title_twinkle():
    return title_bell(gen('TitleTwinkle'), 2093.0, 0.5)


def title_twinkle_pair():
    g = gen('TitleTwinklePair')
    return at(0.65, (0, title_bell(g, 2093.0, 0.5), 1.0), (0.045, title_bell(g, 3136.0, 0.6), 1.0))


def title_neon_tick():
    g = gen('TitleNeonTick')
    d = 0.03
    return unit(fir(nz(g, d), 3000, 12000) * dec(d, 0.0015)) + 0.5 * unit(fir(sweep(240, 240, d, 'square'), None, 4000)) * dec(d, 0.008)


def title_neon_hum():
    """A transformer's hum and a tube's faint hiss, built to repeat exactly every 4 s: every part is a whole number of
    cycles long, and the hiss is the inverse transform of a spectrum (periodic by construction). Nothing here goes
    through fir() or any other filter that has a start and an end, so the loop point is like any other sample."""
    g = gen('TitleNeonHum')
    n = int(SR * 4.0)
    tt = np.arange(n) / SR
    x = 0.3 * np.sin(2 * np.pi * 60.0 * tt + g.uniform(0, 2 * np.pi))
    for k in range(1, 41):
        f = 120.0 * k
        a = k ** -1.1 * (1.0 + 0.8 * np.exp(-(np.log2(f / 750.0) / 0.9) ** 2))   # a gentle bump round 400 to 1400 Hz
        x += a * np.sin(2 * np.pi * f * tt + g.uniform(0, 2 * np.pi))
    fr = np.fft.rfftfreq(n, 1.0 / SR)
    hiss = np.fft.irfft(((fr >= 2000.0) & (fr <= 7000.0)) * np.exp(1j * g.uniform(0, 2 * np.pi, len(fr))), n)
    x += hiss * (np.sqrt(np.mean(x * x)) / np.sqrt(np.mean(hiss * hiss)) * 0.1)        # the hiss 20 dB under the hum
    return x * (1.0 + 0.06 * np.sin(2 * np.pi * 0.25 * tt) + 0.04 * np.sin(2 * np.pi * 0.75 * tt))


over('TitleSlam', title_slam(), 0.9)
over('TitleWipe', title_wipe(), 0.5)
over('TitleTwinkle', title_twinkle(), 0.6)
over('TitleTwinklePair', title_twinkle_pair(), 0.6)
over('TitleNeonTick', title_neon_tick(), 0.5)
over('TitleNeonHum', title_neon_hum(), 0.5, loop=True)

with open(os.path.join(OUT, 'overhaul.txt'), 'wb') as f:
    f.write(('\n'.join(OVERHAUL) + '\n').encode('ascii'))
print('wrote', len(os.listdir(OUT)), 'files to', OUT)
print('offered (not listed, not imported):', len(os.listdir(OFFERED)), 'files in', OFFERED)
