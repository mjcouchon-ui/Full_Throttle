"""FULL THROTTLE sound effects, synthesized like the 1.x Web Audio ones (original sounds, no samples).

Writes 16-bit mono WAVs to Saved/SFXWav; the editor imports them to /Game/Audio/SFX.
Usage: python sfx_synth.py <out_dir>
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
print('wrote', len(os.listdir(OUT)), 'files to', OUT)
