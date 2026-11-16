"""FULL THROTTLE: the world's sound. Every level's ambience, synthesised (numpy only: nothing sampled, nothing downloaded).

    <a Python with numpy> -I Tools/sfx_ambience.py <out_dir> [L0 L7 ...]
    ("C:\\Program Files\\Blender Foundation\\Blender 5.2\\5.2\\python\\bin\\python.exe" has numpy; the PC's own Python has not)

Writes 16-bit WAVs at 32 kHz and <out_dir>/ambience.txt (a line a sound: name, kind, channels, seconds, level, where its
climax is). Content/Python/ft_ambience.py imports them into /Game/Audio/Ambience; Source/FullThrottle/FTAmbience.cpp
plays them by the tables FTAmbienceL<n>.inl. A second run writes every file byte for byte again.

BEDS are stereo loops: a place, in layers. A low body, a mid texture, and sparse high details that come and go on
their own slow clocks. Every part is made to be periodic in the loop's own length (noise is shaped in the frequency
domain, so it has no beginning and no end; clocks are sums of whole cycles; an event that runs past the end comes in
again at the start; steady tones have a whole number of cycles), so a loop has no join to hide. The loops of one level
have lengths with no common measure, so two of them together do not repeat for minutes.
Every bed is written at the same level (BED_RMS): the tables say how far under the music each one plays.

PASSES are mono (the game stands them beside the road and pans them): one-shots with their climax at a known moment
(the game starts them so that the climax falls where the car passes), and loops for things that go on as long as the
car is near ("spots"). Every pass has its loudest half second at PASS_LOUD.

The method is the one of Tools/sfx_synth.py (noise, sines, envelopes; original sounds) with three more tools: noise
shaped by its spectrum, a voice built from the harmonics of a pitch weighted by vowel formants (crowds, screams,
laughter: many of them, each on its own clock), and a room (a dying burst of noise convolved in).
"""
import math
import os
import sys
import wave
import zlib

import numpy as np

SR = 32000
BED_RMS = -23.0     # dBFS: every bed
PASS_LOUD = -17.0   # dBFS: the loudest half second of every pass
KEEP_BITS = 12      # of the 16: a megabyte of .uasset then holds 11 s of stereo (the dropped bits lie 54 dB under a bed)
CTL = 160           # a voice's pitch, loudness and formants are set every 5 ms


def db(x):
    return 10.0 ** (x / 20.0)


def rms(x):
    return float(np.sqrt(np.mean(np.square(x)))) + 1e-12


# ------------------------------------------------------------------------------------------------ spectra
def lowpass(f, fc, order=2):
    return 1.0 / np.sqrt(1.0 + (f / fc) ** (2 * order))


def highpass(f, fc, order=2):
    return 1.0 / np.sqrt(1.0 + (fc / np.maximum(f, 1e-9)) ** (2 * order))


def bandpass(f, lo, hi, order=2):
    return highpass(f, lo, order) * lowpass(f, hi, order)


def resonance(f, f0, q):
    return 1.0 / np.sqrt(1.0 + (q * (f / f0 - f0 / np.maximum(f, 1e-9))) ** 2)


def tilt(f, db_per_octave, ref=1000.0):
    return (np.maximum(f, 20.0) / ref) ** (db_per_octave / 6.0206)


def pan(p):
    """Equal power: -1 left, 0 middle, +1 right. p may be an array."""
    a = (np.clip(p, -1.0, 1.0) + 1.0) * (np.pi / 4)
    return np.cos(a), np.sin(a)


# ------------------------------------------------------------------------------------------------ a loop
class Loop:
    """One sound: its length, its clock and its own random numbers. Everything made through it closes on itself."""

    def __init__(self, seconds, seed):
        self.n = int(round(seconds * SR))
        self.T = self.n / SR
        self.t = np.arange(self.n) / SR
        self.f = np.fft.rfftfreq(self.n, 1.0 / SR)
        self.rng = np.random.default_rng(seed)
        self.nc = self.n // CTL + 1
        self.tc = np.linspace(0.0, self.T, self.nc + 1)   # control times; the last one is the first again

    def snap(self, hz):
        """The nearest frequency with a whole number of cycles in the loop."""
        return max(1, round(hz * self.T)) / self.T

    def since(self, t0, ctl=False):
        """Seconds since t0, the short way round the loop (-T/2 .. T/2)."""
        d = (self.tc if ctl else self.t) - t0
        return (d + self.T / 2) % self.T - self.T / 2

    def up(self, track):
        """A control-rate track at the sample rate."""
        return np.interp(self.t, self.tc, track)

    def noise(self, shape):
        """Noise with the given spectrum (a gain for every frequency of self.f), unit RMS. It has no join."""
        X = np.fft.rfft(self.rng.standard_normal(self.n)) * shape
        X[0] = 0.0
        y = np.fft.irfft(X, self.n)
        return y / rms(y)

    def noise2(self, shape, width=1.0):
        """The same for two ears: width 0 is one noise in the middle, 1 is two unrelated ones."""
        s, a, b = self.noise(shape), self.noise(shape), self.noise(shape)
        k, w = math.sqrt(max(0.0, 1.0 - width)), math.sqrt(min(1.0, width))
        return np.stack([k * s + w * a, k * s + w * b], axis=1)

    def filt(self, x, shape):
        X = np.fft.rfft(x, axis=0)
        return np.fft.irfft(X * (shape if x.ndim == 1 else shape[:, None]), self.n, axis=0)

    def clock(self, cycles, power=1.0, lo=0.0, hi=1.0):
        """A slow curve that wanders between lo and hi and closes on itself: the loop's first `cycles` harmonics,
        each weaker than the last, in random phase."""
        y = np.zeros(self.n)
        for k in range(1, cycles + 1):
            y += k ** -power * np.sin(2 * np.pi * k * self.t / self.T + self.rng.uniform(0, 2 * np.pi))
        y = (y - y.min()) / (y.max() - y.min() + 1e-12)
        return lo + (hi - lo) * y

    def flutter(self, lo, hi, depth):
        """1 +- depth, moving at lo..hi Hz at random (the grain of foam, of leaves, of cloth)."""
        return 1.0 + depth * np.clip(self.noise(bandpass(self.f, lo, hi, 1)) / 2.2, -1.0, 1.0)

    def swell(self, t0, rise, fall, power=2.0):
        """0..1: comes up over `rise` seconds to its top at t0, then dies away with the time constant `fall`."""
        d = self.since(t0)
        e = np.where(d < 0, np.clip(1.0 + d / rise, 0.0, 1.0) ** power, np.exp(-np.maximum(d, 0.0) / fall))
        return e * np.clip((self.T / 2 - np.abs(d)) / (0.08 * self.T), 0.0, 1.0)

    def bump(self, t0, width, ctl=False):
        d = self.since(t0, ctl)
        return np.where(np.abs(d) < width / 2, 0.5 + 0.5 * np.cos(2 * np.pi * d / width), 0.0)

    def put(self, out, x, at, gl=1.0, gr=None):
        """Adds the short sound x into `out` at `at` seconds; what runs past the end comes in again at the start."""
        i = int(round(at * SR)) % self.n
        x = x[:self.n]
        k = min(len(x), self.n - i)
        if out.ndim == 1:
            out[i:i + k] += x[:k] * gl
            out[:len(x) - k] += x[k:] * gl
        else:
            gr = gl if gr is None else gr
            out[i:i + k, 0] += x[:k] * gl
            out[i:i + k, 1] += x[:k] * gr
            out[:len(x) - k, 0] += x[k:] * gl
            out[:len(x) - k, 1] += x[k:] * gr

    def room(self, x, rt=1.2, lo=150.0, hi=5000.0, pre=0.012, echoes=5, wrap=True):
        """What a space gives back of x (mono or stereo in, stereo out, the dry sound not included): a few early
        echoes and a dying burst of noise for each ear (not the same noise, so the tail is wide). Round the loop:
        the tail of what sounds at a loop's end is at its start. (wrap=False, for a one-shot: the tail is cut off at
        the end instead; see tail().)"""
        m = min(self.n, int(SR * rt * 2.0))
        tt = np.arange(m) / SR
        L = self.n if wrap else self.n + m
        out = np.zeros((self.n, 2))
        shape = bandpass(self.f if wrap else np.fft.rfftfreq(L, 1.0 / SR), lo, hi, 1)
        for ch in (0, 1):
            ir = self.rng.standard_normal(m) * np.exp(-6.91 * tt / rt)
            ir[:int(pre * SR)] = 0.0
            for _ in range(echoes):
                ir[int((pre + self.rng.uniform(0.003, 0.07)) * SR) % m] += self.rng.choice((-1.0, 1.0)) * self.rng.uniform(4.0, 9.0)
            pad = np.zeros(L)
            pad[:m] = ir
            H = np.fft.rfft(pad) * shape
            h = np.fft.irfft(H, L)
            H /= math.sqrt(float(np.sum(h * h))) + 1e-12
            src = x if x.ndim == 1 else x[:, ch]
            out[:, ch] = np.fft.irfft(np.fft.rfft(src, L) * H, L)[:self.n]
        return out

    def tail(self, x, rt=1.2, lo=150.0, hi=5000.0, pre=0.012, echoes=5):
        """The room of a one-shot, for one ear: nothing comes round from its end to its beginning."""
        return self.room(x, rt, lo, hi, pre, echoes, wrap=False)[:, 0]

    def edges(self, attack=0.02, release=0.3):
        """For a one-shot: silent at both ends."""
        return np.clip(self.t / attack, 0.0, 1.0) * np.clip((self.T - self.t) / release, 0.0, 1.0)


def at_level(x, level_db):
    """A layer that sounds all the time: by its RMS."""
    return x * (db(level_db) / rms(x))


def at_peak(x, level_db):
    """A layer of separate events: by its highest peak."""
    return x * (db(level_db) / (float(np.max(np.abs(x))) + 1e-12))


# ------------------------------------------------------------------------------------------------ voices
VOWELS = np.array([   # F1, F2, F3 (Hz) of ten vowels as a man says them; a woman's lie about a sixth higher
    (730, 1090, 2440), (530, 1840, 2480), (270, 2290, 3010), (570, 840, 2410), (300, 870, 2240),
    (490, 1350, 1690), (660, 1720, 2410), (640, 1190, 2390), (390, 1990, 2550), (440, 1020, 2240)], dtype=float)


def syllables(c, base, female=False, rate=5.0, phrase=(0.9, 2.4), pause=(0.5, 2.2), energy=1.0, lilt=0.12):
    """Somebody talking all through the loop: phrases of syllables, pauses between. A list of
    (time, seconds, pitch, formants, stress) and the consonants before some of them (time, seconds, kind, gain)."""
    rng = c.rng
    k = 1.17 if female else 1.0
    out, cons = [], []
    t = rng.uniform(0.0, c.T)
    end = t + c.T
    while t < end - 0.35:
        dur = min(rng.uniform(*phrase), end - t - 0.2)
        n = max(1, int(dur * rate * rng.uniform(0.85, 1.15)))
        for i in range(n):
            u = i / max(1, n - 1)
            ts = t + dur * (i + rng.uniform(-0.15, 0.15)) / n
            d = rng.uniform(0.11, 0.21)
            stress = rng.uniform(0.45, 1.0) * (1.3 if rng.random() < 0.25 else 1.0)
            f0 = base * (1.0 + lilt * (1.0 - 2.0 * u)) * rng.uniform(0.94, 1.08) * (1.1 if stress > 1.0 else 1.0)
            V = VOWELS[rng.integers(len(VOWELS))] * k * rng.uniform(0.95, 1.05)
            out.append((ts % c.T, d, f0, V, stress * energy))
            if rng.random() < 0.45:
                cons.append(((ts - 0.035) % c.T, rng.uniform(0.025, 0.07), int(rng.integers(3)), stress * energy * rng.uniform(0.3, 0.8)))
        t += dur + rng.uniform(*pause)
    return out, cons


def tracks(c, syl, base, female=False):
    """Pitch, loudness and formant tracks (control rate, closed) for a list of syllables: each syllable is a bump of
    loudness, and pitch and formants glide from one syllable's to the next's."""
    k = 1.17 if female else 1.0
    amp = np.zeros(c.nc + 1)
    den = np.full(c.nc + 1, 1e-3)
    nf0 = den * base
    nF = np.outer(np.array([500.0, 1500.0, 2500.0]) * k, den)
    for ts, d, f0, V, stress in syl:
        w = d * 1.5
        dd = c.since(ts + d / 2, True)
        b = np.where(np.abs(dd) < w / 2, 0.5 + 0.5 * np.cos(2 * np.pi * dd / w), 0.0)
        amp += b * stress
        nf0 = nf0 + b * f0
        den = den + b
        nF = nF + V[:, None] * b[None, :]
    return nf0 / den, amp, nF / den[None, :]


def sing(c, f0c, ampc, F, top=4200.0, roll=1.0, bw=(90.0, 120.0, 180.0), fg=(1.0, 0.55, 0.3), vib=None):
    """A voice: the harmonics of the pitch track, each as loud as the vowel's formants make its frequency."""
    f0 = c.up(f0c)
    if vib is not None:
        f0 = f0 * vib
    cyc = f0.sum() / SR
    f0 *= max(1.0, round(cyc)) / cyc           # a whole number of cycles in the loop: every harmonic closes
    ph = 2 * np.pi * np.cumsum(f0) / SR
    out = np.zeros(c.n)
    for h in range(1, int(top / max(40.0, float(f0c.min()))) + 1):
        fh = h * f0c
        w = np.zeros_like(fh)
        for k in range(3):
            w += fg[k] / np.sqrt(1.0 + ((fh - F[k]) / (0.5 * bw[k])) ** 2)
        w = (w + 0.015) * ampc / h ** roll * np.clip((top - fh) / 300.0, 0.0, 1.0)
        if float(w.max()) < 1e-4:
            continue
        out += c.up(w) * np.sin(h * ph)
    return out


def crowd(c, people, rt=0.9, top=4200.0, wet=0.6):
    """Many people talking. people: (pitch, woman?, pan, distance 0 near .. 1 far, syllables a second, energy).
    Stereo; the far ones are duller, quieter and mostly room."""
    dry = np.zeros((c.n, 2))
    send = np.zeros(c.n)
    env = np.zeros((3, c.n, 2))
    for base, female, p, dist, rate, energy in people:
        syl, cons = syllables(c, base, female, rate, energy=energy)
        f0c, amp, F = tracks(c, syl, base, female)
        v = sing(c, f0c, amp, F, top=top - 2000.0 * dist)
        g = 1.0 - 0.6 * dist
        gl, gr = pan(p)
        dry[:, 0] += v * g * gl
        dry[:, 1] += v * g * gr
        send += v * g * (0.25 + 0.75 * dist)
        for t0, d, kind, a in cons:
            m = int(d * SR)
            c.put(env[kind], np.hanning(m) * a * g * (1.0 - 0.7 * dist), t0, gl, gr)
    f = c.f
    for kind, (lo, hi) in enumerate(((4500.0, 9000.0), (2200.0, 5000.0), (2500.0, 7000.0))):
        dry += env[kind] * c.noise2(bandpass(f, lo, hi, 2), 1.0) * 0.5
    return dry + wet * c.room(send, rt, 200.0, 3500.0)


def one_voice(seconds, seed, f0_of, amp_of, vowel, female=True, top=8000.0, roll=0.8, vib_hz=6.5, vib=0.03):
    """A single cry (a scream, a whoop, a laugh's "ha"): pitch and loudness as functions of 0..1 through it."""
    e = Loop(seconds, seed)
    u = e.tc / e.T
    f0c = f0_of(u)
    amp = amp_of(u)
    amp[0] = amp[-1] = 0.0
    F = np.outer(VOWELS[vowel] * (1.25 if female else 1.0), np.ones(e.nc + 1))
    v = 1.0 + vib * np.sin(2 * np.pi * vib_hz * e.t + e.rng.uniform(0, 6.28)) * np.clip(e.t / 0.25, 0.0, 1.0)
    x = sing(e, f0c, amp, F, top=top, roll=roll, vib=v)
    breath = e.noise(bandpass(e.f, 1500.0, 6000.0, 1)) * e.up(amp) * 0.12 * (np.max(np.abs(x)) + 1e-9)
    return x + breath


def laugh(seed, base=260.0, n=6, female=True):
    """ha-ha-ha: short open vowels, each a little lower and softer than the last."""
    rng = np.random.default_rng(seed)
    step = rng.uniform(0.17, 0.21)
    e = Loop(n * step + 0.3, seed)
    syl = [(0.05 + i * step, 0.09, base * (1.25 - 0.07 * i) * rng.uniform(0.97, 1.03), VOWELS[0] * (1.2 if female else 1.0), 1.0 - 0.1 * i) for i in range(n)]
    f0c, amp, F = tracks(e, syl, base, female)
    x = sing(e, f0c, amp, F, top=5500.0, roll=0.9)
    return x + e.noise(bandpass(e.f, 1200.0, 5000.0, 1)) * e.up(amp) * 0.25 * np.max(np.abs(x))


# ------------------------------------------------------------------------------------------------ small things
def ping(rng, f1, ratios, amps, decays, seconds, click=0.3):
    """A struck thing: partials with their own decays, and the click of the strike."""
    m = int(seconds * SR)
    tt = np.arange(m) / SR
    x = np.zeros(m)
    for r, a, d in zip(ratios, amps, decays):
        x += a * np.sin(2 * np.pi * f1 * r * tt + rng.uniform(0, 6.28)) * np.exp(-tt / d)
    k = int(0.0015 * SR)
    x[:k] += click * rng.uniform(-1, 1, k) * np.linspace(1, 0, k)
    # (the last fifth is faded out: a long ring is not cut off with a tick where its time ends)
    return x * np.clip(tt / 0.0004, 0.0, 1.0) * np.clip((seconds - tt) / (0.2 * seconds), 0.0, 1.0)


def glass(rng):
    return ping(rng, rng.uniform(2600.0, 4300.0), (1.0, 1.52, 2.31, 3.4), (1.0, 0.6, 0.35, 0.2), (0.12, 0.09, 0.06, 0.04), 0.45)


def knock(rng, f1, noise_hz=(400.0, 1800.0)):
    """A foot on boards, a hull on a fender: a short low thud with a tick in it."""
    m = int(0.16 * SR)
    tt = np.arange(m) / SR
    x = np.sin(2 * np.pi * f1 * tt) * np.exp(-tt / 0.035) + 0.5 * np.sin(2 * np.pi * f1 * 2.3 * tt) * np.exp(-tt / 0.02)
    e = Loop(0.16, int(rng.integers(1 << 30)))
    return x + 0.5 * e.noise(bandpass(e.f, noise_hz[0], noise_hz[1], 1))[:m] * np.exp(-tt / 0.012)


def bloop(rng, f1, seconds=0.05, rise=1.6):
    """A drop of water: a sine that climbs as it dies."""
    m = int(seconds * SR)
    u = np.arange(m) / m
    ph = 2 * np.pi * np.cumsum(f1 * (1.0 + (rise - 1.0) * u)) / SR
    return np.sin(ph) * np.exp(-4.0 * u) * np.clip(u / 0.05, 0.0, 1.0)


def bell(rng, f1, seconds, bright=1.0):
    """A cast bell: hum, prime, tierce, quint, nominal and the high partials, the low ones ringing longest."""
    ratios = (0.5, 1.0, 1.19, 1.5, 2.0, 2.51, 2.66, 3.01, 4.17, 5.43)
    amps = (0.55, 1.0, 0.7, 0.35, 0.8, 0.3 * bright, 0.25 * bright, 0.3 * bright, 0.2 * bright, 0.12 * bright)
    decays = [seconds * k for k in (0.38, 0.3, 0.2, 0.16, 0.14, 0.08, 0.07, 0.06, 0.04, 0.03)]
    return ping(rng, f1, ratios, amps, decays, seconds, click=0.15)


# ================================================================================================ LEVEL 0: BAYSHORE
# The bay is on the LEFT of travel all the way (the beach, then the sea 350 m off), the town on the RIGHT: the sea's
# beds lean left, the town's lean right.

def L0_Surf(c):
    """The sea down the beach: the shore's steady roar, breakers one after another along it (each a wash that comes
    up, a crash, and foam hissing away up the sand, drifting along the beach as it dies), a second line out on the
    bar, and gulls far out over the water."""
    f = c.f
    out = at_level(c.noise2(bandpass(f, 45.0, 320.0, 2) * tilt(f, -3.0, 100.0), 0.85) * c.clock(2, 1.0, 0.75, 1.0)[:, None], -7.0) * (1.0, 0.8)
    out += at_level(c.noise2(bandpass(f, 900.0, 4200.0, 1) * tilt(f, -2.0), 1.0) * c.clock(3, 1.0, 0.4, 1.0)[:, None], -22.0) * (1.0, 0.7)
    K = 6
    brk = np.zeros((c.n, 2))
    for i in range(K):
        t0 = (i + c.rng.uniform(-0.3, 0.3)) * c.T / K
        big = c.rng.uniform(0.5, 1.0)
        wash = c.noise(bandpass(f, 220.0, 1500.0, 2) * tilt(f, -2.0)) * c.swell(t0, c.rng.uniform(1.2, 2.0), c.rng.uniform(0.9, 1.5))
        crash = c.noise(bandpass(f, 500.0, 3800.0, 2)) * c.swell(t0 + 0.05, 0.45, 0.55, 3.0)
        thump = c.noise(bandpass(f, 40.0, 130.0, 2)) * c.swell(t0, 0.5, 0.5)
        foam = c.noise(bandpass(f, 2800.0, 10500.0, 2) * tilt(f, -1.5, 5000.0)) * c.swell(t0 + 0.35, 0.3, c.rng.uniform(1.2, 2.0), 1.0) * c.flutter(12.0, 70.0, 0.45)
        w = big * (0.9 * wash + 0.75 * crash + 0.5 * thump + 0.55 * foam)
        gl, gr = pan(c.rng.uniform(-0.95, 0.25) + c.rng.uniform(-0.35, 0.35) * np.clip(c.since(t0) / 2.5, -0.5, 1.0))
        brk[:, 0] += w * gl
        brk[:, 1] += w * gr
    out += at_level(brk, -3.0)
    # gulls: two birds, each a run of falling cries, far out: dull, small and mostly echo
    g = np.zeros(c.n)
    for t0, n, hi, lo, step, dur in ((0.19 * c.T, 4, 1550.0, 950.0, 0.34, 0.26), (0.67 * c.T, 3, 1300.0, 820.0, 0.43, 0.33), (0.88 * c.T, 2, 1700.0, 1100.0, 0.3, 0.22)):
        for i in range(n):
            m = int(dur * SR)
            u = np.arange(m) / m
            f0 = hi * (1.0 - 0.04 * i) * (lo / hi) ** (u ** 0.7) * (0.9 + 0.1 * np.clip(u / 0.08, 0.0, 1.0))
            ph = 2 * np.pi * np.cumsum(f0) / SR
            cry = (np.sin(ph) + 0.55 * np.sin(2 * ph) + 0.35 * np.sin(3 * ph) + 0.15 * np.sin(4 * ph)) * np.sin(np.pi * u) ** 0.6 * np.exp(-1.2 * u) * (1.0 + 0.25 * np.sin(2 * np.pi * 38.0 * u * dur))
            c.put(g, cry * (1.0 - 0.12 * i), t0 + i * step * c.rng.uniform(0.92, 1.08))
    g = c.filt(g, lowpass(f, 3800.0, 2))
    gulls = np.stack([g, g * 0.6], axis=1) * 0.35 + c.room(g, 1.8, 400.0, 3500.0, 0.05)
    out += at_peak(gulls, -20.0)
    return c.filt(out, lowpass(f, 9500.0, 1))


def L0_Boardwalk(c):
    """The boardwalk at night: people strolling and talking on both sides (a dozen voices near and far, a murmur of
    hundreds behind them), someone laughing, glasses, feet on the boards."""
    f = c.f
    r = c.rng
    people = [(r.uniform(95, 135) if i % 2 else r.uniform(175, 235), i % 2 == 0, r.uniform(-0.95, 0.95), (r.uniform(0.05, 0.35) if i < 4 else r.uniform(0.5, 1.0)),
               r.uniform(4.2, 5.8), r.uniform(0.7, 1.0)) for i in range(12)]
    out = at_level(crowd(c, people, 0.8), 0.0)
    wash = c.noise2(bandpass(f, 250.0, 2400.0, 1) * (0.5 + resonance(f, 520.0, 1.6) + 0.7 * resonance(f, 1400.0, 2.2)) * tilt(f, -3.0), 1.0)
    out += at_level(wash * c.clock(4, 1.0, 0.6, 1.0)[:, None], -8.0)
    out += at_level(c.noise2(bandpass(f, 70.0, 220.0, 2), 0.9), -15.0)          # the air of an open place
    ev = np.zeros((c.n, 2))
    for seed, t0, base, p in ((11, 0.31 * c.T, 290.0, 0.55), (12, 0.78 * c.T, 215.0, -0.6)):
        c.put(ev, at_peak(laugh(seed, base, int(r.integers(5, 8))), -9.0), t0, *pan(p))
    for t0 in sorted(r.uniform(0.0, c.T, 4)):
        gl, gr = pan(r.uniform(-0.8, 0.8))
        c.put(ev, at_peak(glass(r), -15.0), t0, gl, gr)
        c.put(ev, at_peak(glass(r), -17.0), t0 + r.uniform(0.06, 0.1), gl, gr)
    for t0, p0, dp in ((0.08 * c.T, -0.7, 0.25), (0.52 * c.T, 0.3, -0.4)):   # two people walking by on the boards
        steps = int(r.integers(5, 8))
        for i in range(steps):
            c.put(ev, at_peak(knock(r, r.uniform(140.0, 250.0)), -14.0 - 2.0 * (i % 2)), t0 + i * 0.52 * r.uniform(0.94, 1.06), *pan(p0 + dp * i / steps))
    out += ev + 0.35 * c.room(ev, 0.6, 250.0, 5000.0)
    return out


def L0_Marina(c):
    """Among the moored boats: the harbour's hush, water slapping and gurgling against hulls, halyards tapping their
    masts whenever the breeze gets up (every mast its own note), a hull creaking on its ropes, a buoy's bell far out."""
    f = c.f
    r = c.rng
    out = at_level(c.noise2(bandpass(f, 70.0, 600.0, 2) * tilt(f, -3.0, 200.0), 0.9) * c.clock(3, 1.0, 0.6, 1.0)[:, None], -6.0)
    out += at_level(c.noise2(bandpass(f, 1500.0, 6000.0, 1) * tilt(f, -3.0), 1.0) * c.clock(4, 1.0, 0.3, 1.0)[:, None], -19.0)   # ripples
    # lapping: slaps and bloops on a slow swell of their own
    lap = np.zeros((c.n, 2))
    sw = c.clock(3, 0.8, 0.25, 1.0)
    t0 = 0.0
    while t0 < c.T:
        a = sw[int(t0 * SR) % c.n] * r.uniform(0.4, 1.0)
        gl, gr = pan(r.uniform(-0.9, 0.9))
        if r.random() < 0.55:
            e = Loop(0.12, int(r.integers(1 << 30)))
            c.put(lap, e.noise(bandpass(e.f, r.uniform(350.0, 700.0), r.uniform(1200.0, 2600.0), 1)) * np.hanning(e.n) ** 2 * a, t0, gl, gr)
        else:
            c.put(lap, bloop(r, r.uniform(280.0, 800.0), r.uniform(0.04, 0.09), r.uniform(1.3, 1.9)) * a * 1.4, t0, gl, gr)
        t0 += r.uniform(0.09, 0.42)
    out += at_level(lap, -9.0)
    # rigging: five masts, each tapping while the breeze (its own clock) is up
    rig = np.zeros((c.n, 2))
    for i in range(5):
        f1 = r.uniform(1700.0, 3600.0)
        gust = c.clock(3, 0.7, -0.6, 1.0)
        p = r.uniform(-0.9, 0.9)
        far = r.uniform(0.25, 1.0)
        gap = r.uniform(0.13, 0.3)
        t0 = r.uniform(0.0, 0.5)
        while t0 < c.T:
            g = gust[int(t0 * SR) % c.n]
            if g > 0.0:
                c.put(rig, ping(r, f1 * r.uniform(0.995, 1.005), (1.0, 1.47, 2.09, 2.56, 3.9), (1.0, 0.5, 0.4, 0.25, 0.15), (0.11, 0.08, 0.05, 0.04, 0.02), 0.4, 0.5) * g * far * r.uniform(0.5, 1.0), t0, *pan(p))
            t0 += gap * r.uniform(0.75, 1.4) * (1.0 if g > 0.0 else 2.0)
    out += at_peak(rig * 0.7 + 0.5 * c.room(rig, 0.9, 800.0, 6000.0), -13.0)
    # creaks: a hull on its ropes (stick and slip: a rough low buzz through two resonances, sliding)
    cr = np.zeros((c.n, 2))
    for t0, p in ((0.22 * c.T, -0.5), (0.61 * c.T, 0.6), (0.9 * c.T, -0.1)):
        e = Loop(r.uniform(0.5, 0.9), int(r.integers(1 << 30)))
        rate = r.uniform(38.0, 70.0) * (1.0 + 0.5 * e.t / e.T)
        saw = 2.0 * ((np.cumsum(rate) / SR) % 1.0) - 1.0
        x = e.filt(saw + 0.2 * e.noise(np.ones_like(e.f)), resonance(e.f, r.uniform(380.0, 620.0), 9.0) + 0.6 * resonance(e.f, r.uniform(900.0, 1400.0), 12.0))
        c.put(cr, x * np.hanning(e.n) ** 0.7, t0, *pan(p))
    out += at_peak(cr + 0.5 * c.room(cr, 0.7, 300.0, 3000.0), -19.0)
    # the buoy's bell: twice, far out to the left
    b = np.zeros(c.n)
    c.put(b, bell(r, 640.0, 2.4, 0.6), 0.4 * c.T)
    c.put(b, bell(r, 640.0, 2.0, 0.6) * 0.6, 0.4 * c.T + 1.3)
    b = c.filt(b, lowpass(f, 2600.0, 1))
    out += at_peak(np.stack([b, 0.45 * b], axis=1) * 0.4 + c.room(b, 2.2, 300.0, 2500.0, 0.06), -21.0)
    return out


def L0_Fair(c):
    """The funfair on Pleasure Beach: an excited crowd, the coaster's chain lift clacking up, then its train coming
    down with a rumble and a carful of screams, a game's bell, a barker on a megaphone, the rides' motors under it all."""
    f = c.f
    r = c.rng
    people = [(r.uniform(110, 150) if i % 3 == 0 else r.uniform(190, 270), i % 3 != 0, r.uniform(-0.95, 0.95), r.uniform(0.2, 1.0), r.uniform(5.0, 6.5), r.uniform(0.8, 1.1)) for i in range(9)]
    out = at_level(crowd(c, people, 0.9, 4600.0), -2.0)
    wash = c.noise2(bandpass(f, 300.0, 3000.0, 1) * (0.5 + resonance(f, 650.0, 1.5) + 0.8 * resonance(f, 1700.0, 2.0)) * tilt(f, -2.5), 1.0)
    out += at_level(wash * c.clock(4, 1.0, 0.6, 1.0)[:, None], -7.0)
    # the rides' motors: two hums a few cycles apart, beating
    hum = np.zeros(c.n)
    for fm, a in ((c.snap(110.0), 1.0), (c.snap(110.0) + 6.0 / c.T, 0.8), (c.snap(165.0), 0.5), (c.snap(220.0) + 4.0 / c.T, 0.3)):
        hum += a * np.sin(2 * np.pi * fm * c.t + r.uniform(0, 6.28))
    out += at_level(np.stack([hum, hum], axis=1) + 0.3 * c.noise2(bandpass(f, 60.0, 260.0, 2), 0.8), -17.0)
    ride = np.zeros((c.n, 2))
    # the chain lift: clack, clack, clack, a little faster as the train nears the top
    t0, gap = 0.09 * c.T, 0.15
    while t0 < 0.46 * c.T:
        e = Loop(0.06, int(r.integers(1 << 30)))
        x = e.noise(bandpass(e.f, 700.0, 2800.0, 1)) * np.exp(-e.t / 0.01) + 0.6 * np.sin(2 * np.pi * 180.0 * e.t) * np.exp(-e.t / 0.02)
        c.put(ride, x * r.uniform(0.7, 1.0), t0, *pan(-0.45))
        t0 += gap * r.uniform(0.96, 1.04)
        gap *= 0.992
    # the drop: the train's rumble sweeps across, its wheels' roar rising and falling
    td = 0.56 * c.T
    sweep = np.clip(c.since(td) / 1.6, -1.0, 1.0)
    gl, gr = pan(-0.5 + 0.9 * (sweep * 0.5 + 0.5))
    rumble = c.noise(bandpass(f, 60.0, 420.0, 2)) * c.swell(td, 1.1, 1.2) * (1.0 + 0.5 * np.sin(2 * np.pi * c.snap(41.0) * c.t))
    whoosh = c.noise(bandpass(f, 500.0, 2600.0, 1)) * c.swell(td + 0.15, 0.7, 0.8, 3.0)
    tr = 2.2 * rumble + 1.0 * whoosh
    ride[:, 0] += tr * gl * 0.9
    ride[:, 1] += tr * gr * 0.9
    out += at_peak(ride + 0.4 * c.room(ride, 1.0, 200.0, 4000.0), -7.0)
    # the carful of screams, riding the drop
    sc = np.zeros(c.n)
    for i in range(6):
        top = r.uniform(950.0, 1400.0)
        dur = r.uniform(1.1, 1.8)
        x = one_voice(dur, 100 + i, lambda u: top * (0.72 + 0.28 * np.clip(u / 0.12, 0, 1)) * (1.0 - 0.22 * u ** 1.5), lambda u: np.clip(u / 0.08, 0, 1) * (1.0 - u) ** 0.8, int(r.choice((0, 6, 1))))
        c.put(sc, x * r.uniform(0.6, 1.0), td - 0.35 + r.uniform(0.0, 0.5))
    sc = c.filt(sc, bandpass(f, 500.0, 5000.0, 1))
    gl, gr = pan(-0.4 + 0.8 * (sweep * 0.5 + 0.5))
    out += at_peak(np.stack([sc * gl, sc * gr], axis=1) * 0.6 + c.room(sc, 1.2, 500.0, 4500.0, 0.03), -11.0)
    # a game's bell: ding ding ding
    gb = np.zeros((c.n, 2))
    for i in range(3):
        c.put(gb, ping(r, 2093.0, (1.0, 2.76, 5.4), (1.0, 0.45, 0.2), (0.22, 0.1, 0.05), 0.6, 0.4), 0.86 * c.T + i * 0.15, *pan(0.6))
    out += at_peak(gb * 0.6 + 0.5 * c.room(gb, 0.9, 800.0, 6000.0), -17.0)
    # the barker: one line through a megaphone (a nasal band, pushed)
    e = Loop(2.0, 77)
    syl = [(0.1 + i * 0.19, 0.14, 150.0 * (1.25 - 0.03 * i) * e.rng.uniform(0.95, 1.1), VOWELS[e.rng.integers(len(VOWELS))], e.rng.uniform(0.7, 1.2)) for i in range(8)]
    f0c, amp, F = tracks(e, syl, 150.0)
    bark = np.tanh(3.0 * e.filt(sing(e, f0c, amp, F, top=3200.0, roll=0.7), bandpass(e.f, 600.0, 2600.0, 2)) / 0.3)
    bk = np.zeros(c.n)
    c.put(bk, bark, 0.02 * c.T)
    out += at_peak(np.stack([0.5 * bk, bk], axis=1) * 0.5 + c.room(bk, 1.1, 500.0, 3000.0, 0.04), -15.0)
    return out


def L0_Night(c):
    """The quiet end of the coast at night: crickets in the hedges and the dune grass (each on its own beat, falling
    silent and starting again), a trill of them further off, wind moving through the grass, and from the town on the
    right the hum of its air conditioning and a car now and then on the back road."""
    f = c.f
    r = c.rng
    out = at_level(c.noise2(bandpass(f, 500.0, 3600.0, 1) * tilt(f, -3.0), 1.0) * (c.clock(3, 0.8, 0.12, 1.0) ** 1.5 * c.flutter(6.0, 18.0, 0.35))[:, None], -7.0)   # grass
    out += at_level(c.noise2(bandpass(f, 60.0, 300.0, 2) * tilt(f, -3.0, 100.0), 0.8), -9.0)                                                        # the night's own air
    ck = np.zeros((c.n, 2))
    for i in range(8):
        fc = c.snap(r.uniform(3800.0, 5500.0))
        chirps = max(1, round(r.uniform(1.7, 3.2) * c.T))          # a whole number of chirps in the loop
        pulses = int(r.integers(3, 5))
        pr = r.uniform(27.0, 35.0)
        tau = ((c.t * chirps / c.T + r.uniform(0, 1)) % 1.0) * c.T / chirps
        env = np.zeros(c.n)
        for k in range(pulses):
            d = tau - (0.008 + k / pr)
            env += np.where(np.abs(d) < 0.007, 0.5 + 0.5 * np.cos(np.pi * d / 0.007), 0.0) * (0.7 + 0.3 * k / pulses)
        awake = np.clip(c.clock(3, 0.6, -0.9, 1.0) * 4.0, 0.0, 1.0)     # sings for a while, stops, starts again
        x = (np.sin(2 * np.pi * fc * c.t) + 0.12 * np.sin(2 * np.pi * 2 * fc * c.t)) * env * awake * r.uniform(0.35, 1.0)
        gl, gr = pan(r.uniform(-1.0, 1.0))
        ck[:, 0] += x * gl
        ck[:, 1] += x * gr
    out += at_peak(ck + 0.25 * c.room(ck, 0.5, 2500.0, 7000.0), -13.0)
    trill = c.noise2(bandpass(f, 4500.0, 6600.0, 3), 1.0) * ((0.5 + 0.5 * np.sin(2 * np.pi * c.snap(43.0) * c.t)) ** 2 * c.clock(2, 1.0, 0.5, 1.0))[:, None]
    out += at_level(trill, -24.0)
    # the town, on the right: two air conditioners a few cycles apart, and the back road
    hum = np.zeros(c.n)
    for fm, a in ((c.snap(100.0), 1.0), (c.snap(100.0) + 7.0 / c.T, 0.8), (c.snap(200.0), 0.35), (c.snap(200.0) + 9.0 / c.T, 0.25), (c.snap(300.0), 0.12)):
        hum += a * np.sin(2 * np.pi * fm * c.t + r.uniform(0, 6.28))
    out += at_level(np.stack([0.45 * hum, hum], axis=1), -17.0)
    road = np.zeros((c.n, 2))
    for t0 in (0.3 * c.T, 0.8 * c.T):
        x = c.noise(bandpass(f, 80.0, 700.0, 1) * tilt(f, -3.0, 200.0)) * c.swell(t0, r.uniform(1.4, 2.2), r.uniform(1.0, 1.6))
        gl, gr = pan(0.2 + 0.6 * np.clip(c.since(t0) / 3.0, -1.0, 1.0))
        road[:, 0] += x * gl
        road[:, 1] += x * gr
    out += at_level(road, -12.0)
    return out


def L0_Organ(c):
    """The big wheel's fairground organ: eight bars of an original waltz (bass on the one, chords on the two and
    three, an arpeggio tune on top) on stopped pipes with their breathy speech and a tremulant. Loops; the game makes
    it swell and fall with the car's distance."""
    r = c.rng
    beat = c.T / 24.0
    N = {'C3': 130.81, 'D3': 146.83, 'F2': 87.31, 'G2': 98.0, 'F3': 174.61, 'G3': 196.0, 'A3': 220.0, 'Bb3': 233.08, 'C4': 261.63, 'D4': 293.66, 'E4': 329.63, 'F4': 349.23,
         'G4': 392.0, 'A4': 440.0, 'Bb4': 466.16, 'C5': 523.25, 'D5': 587.33, 'E5': 659.26, 'F5': 698.46, 'G5': 783.99, 'A5': 880.0, 'Bb5': 932.33, 'C6': 1046.5}
    bars = [('F2', ('A3', 'C4', 'F4'), ('F5', 'A5', 'C6')), ('G2', ('Bb3', 'D4', 'G4'), ('Bb5', None, 'G5')), ('C3', ('Bb3', 'C4', 'E4'), ('E5', 'G5', 'Bb5')), ('F2', ('A3', 'C4', 'F4'), ('A5', None, None)),
            ('D3', ('A3', 'D4', 'F4'), ('D5', 'F5', 'A5')), ('C3', ('G3', 'C4', 'E4'), ('G5', None, 'E5')), ('C3', ('Bb3', 'C4', 'E4'), ('C5', 'E5', 'G5')), ('F2', ('A3', 'C4', 'F4'), ('F5', None, None))]
    out = np.zeros(c.n)

    def pipe(hz, at, dur, amp, partials):
        m = int((dur + 0.06) * SR)
        tt = np.arange(m) / SR
        x = np.zeros(m)
        for h, a in enumerate(partials, 1):
            if hz * h < 7000.0:
                x += a * np.sin(2 * np.pi * hz * h * (1.0 + r.uniform(-0.0015, 0.0015)) * tt + r.uniform(0, 6.28))
        env = np.clip(tt / 0.018, 0.0, 1.0) * np.clip((dur + 0.05 - tt) / 0.05, 0.0, 1.0)
        e = Loop(0.05, int(r.integers(1 << 30)))
        x[:e.n] += 0.5 * e.noise(resonance(e.f, min(hz * 3.0, 6000.0), 3.0)) * np.hanning(e.n)     # the pipe's chiff
        c.put(out, x * env * amp, at)

    flute = (1.0, 0.25, 0.42, 0.08, 0.18, 0.03, 0.07)
    reed = (1.0, 0.6, 0.5, 0.35, 0.25, 0.18, 0.12, 0.08)
    for b, (bass, chord, tune) in enumerate(bars):
        t0 = b * 3 * beat
        pipe(N[bass], t0, beat * 0.8, 0.9, reed)
        for k in (1, 2):
            for note in chord:
                pipe(N[note], t0 + k * beat, beat * 0.42, 0.28, flute)
        k = 0
        while k < 3:
            if tune[k] is not None:
                hold = 1
                while k + hold < 3 and tune[k + hold] is None:
                    hold += 1
                pipe(N[tune[k]], t0 + k * beat, beat * (hold - 0.1), 0.55, flute)
                pipe(N[tune[k]] * 2.0, t0 + k * beat, beat * (hold - 0.1), 0.16, flute)
                k += hold
            else:
                k += 1
    out *= 1.0 + 0.11 * np.sin(2 * np.pi * c.snap(5.8) * c.t)                  # the tremulant
    out = c.filt(out, bandpass(c.f, 70.0, 5200.0, 1))
    return out + 0.3 * c.room(out, 0.8, 200.0, 4000.0)[:, 0]


def L0_Swing(c):
    """The chair swing by a jump, as the car flies past its crown: the chairs' rush of air coming round three times,
    riders whooping, the motor under them. A one-shot; its climax is at 1.7 s."""
    f = c.f
    r = c.rng
    turn = 0.5 + 0.5 * np.sin(2 * np.pi * (c.t - 1.7) / 1.15 + np.pi / 2)            # a chair comes by every 1.15 s; one is nearest at 1.7 s
    air = c.noise(bandpass(f, 350.0, 2400.0, 1) * tilt(f, -2.0)) * (0.25 + 0.75 * turn ** 2)
    motor = np.sin(2 * np.pi * 61.0 * c.t) + 0.5 * np.sin(2 * np.pi * 122.5 * c.t) + 0.3 * np.sin(2 * np.pi * 183.0 * c.t)
    who = np.zeros(c.n)
    for i in range(6):
        top = r.uniform(850.0, 1300.0)
        dur = r.uniform(0.7, 1.3)
        x = one_voice(dur, 300 + i, lambda u: top * (0.6 + 0.4 * np.sin(np.pi * np.clip(u * 1.2, 0, 1)) ** 0.7), lambda u: np.sin(np.pi * u) ** 0.8, int(r.choice((2, 0, 8))))
        c.put(who, x * r.uniform(0.6, 1.0), max(0.0, min(c.T - dur - 0.1, 1.7 - dur / 2 + r.uniform(-0.9, 0.9))))
    who = c.filt(who, bandpass(f, 500.0, 5500.0, 1))
    x = at_level(air, -4.0) + at_level(motor, -16.0) + at_peak(who + 0.6 * c.tail(who, 0.9, 500.0, 4500.0), -3.0)
    return x * np.exp(-0.5 * ((c.t - 1.7) / 1.05) ** 2) * c.edges(0.05, 0.4)


def L0_ShipHorn(c):
    """A liner out in the bay sounds her horn: one long low chord, slow to speak, and the bay giving it back.
    A one-shot; its climax is at 1.5 s."""
    r = c.rng
    x = np.zeros(c.n)
    for f0, a in ((98.0, 1.0), (123.5, 0.8), (147.0, 0.5)):
        for h in range(1, 14):
            x += a / h ** 1.1 * np.sin(2 * np.pi * f0 * h * (1.0 + 0.002 * np.sin(2 * np.pi * 0.7 * c.t + h)) * c.t + r.uniform(0, 6.28)) * (1.0 + 0.3 * (h in (3, 4, 5)))
    env = np.clip(c.t / 0.35, 0.0, 1.0) ** 1.5 * np.clip((3.1 - c.t) / 0.45, 0.0, 1.0)
    x = c.filt(x * env * (1.0 + 0.06 * np.sin(2 * np.pi * 5.1 * c.t)), bandpass(c.f, 70.0, 1500.0, 1))
    wet = c.tail(x, 3.2, 90.0, 1300.0, 0.18, 8)
    return (x + 0.9 * wet) * c.edges(0.02, 0.6)


def L0_Cheer(c):
    """Sunset Strip turns out to watch: a crowd's roar coming up and falling away, whoops and two whistles over it,
    clapping under it. A one-shot; its climax is at 1.6 s."""
    f = c.f
    r = c.rng
    up = np.exp(-0.5 * ((c.t - 1.6) / 1.0) ** 2)
    roar = c.noise(bandpass(f, 300.0, 3200.0, 1) * (0.4 + resonance(f, 750.0, 1.6) + 0.8 * resonance(f, 1250.0, 2.0) + 0.5 * resonance(f, 2600.0, 2.5)) * tilt(f, -2.0)) * c.flutter(3.0, 9.0, 0.3)
    who = np.zeros(c.n)
    for i in range(10):
        lo, hi = r.uniform(420.0, 620.0), r.uniform(800.0, 1250.0)
        dur = r.uniform(0.5, 1.1)
        x = one_voice(dur, 500 + i, lambda u: lo + (hi - lo) * np.clip(u * 1.6, 0, 1) ** 0.6, lambda u: np.sin(np.pi * u) ** 0.7, int(r.choice((4, 3, 0))), female=bool(i % 2))
        c.put(who, x * r.uniform(0.5, 1.0), max(0.0, min(c.T - dur - 0.1, 1.6 - dur / 2 + r.normal(0.0, 0.7))))
    for t0, a, b in ((1.25, 2700.0, 3500.0), (2.05, 3100.0, 2500.0)):                       # whistles through the fingers
        m = int(0.32 * SR)
        u = np.arange(m) / m
        c.put(who, 0.5 * np.sin(2 * np.pi * np.cumsum(a + (b - a) * u ** 0.5) / SR) * np.sin(np.pi * u) ** 0.5, t0)
    claps = np.zeros(c.n)
    k = 0
    while k < 420:
        t0 = float(np.clip(r.normal(1.9, 0.95), 0.05, c.T - 0.1))
        e = Loop(0.02, int(r.integers(1 << 30)))
        c.put(claps, e.noise(bandpass(e.f, r.uniform(900.0, 1500.0), r.uniform(2600.0, 4200.0), 1)) * np.exp(-e.t / 0.004) * r.uniform(0.3, 1.0), t0)
        k += 1
    x = at_level(roar * up, 0.0) + at_peak(who, 0.0) * up ** 0.5 + at_level(claps, -7.0)
    x = x + 0.5 * c.tail(x, 1.0, 300.0, 4500.0)
    return x * c.edges(0.08, 0.6)


# ================================================================================================ more tools
# What the other seven levels' sounds are put together from.

def stereo(x, p=0.0):
    gl, gr = pan(p)
    return np.stack([x * gl, x * gr], axis=1)


def far(c, x, rt=1.5, lo=200.0, hi=4000.0, dry=0.35, p=0.0, pre=0.03):
    """A mono layer heard from a distance: a little of itself, mostly what the place gives back."""
    return stereo(x, p) * dry + c.room(x, rt, lo, hi, pre)


def thud(f1, decay=0.06, seconds=0.3, drop=0.5):
    """A low thump: a sine that sinks as it dies."""
    m = int(seconds * SR)
    tt = np.arange(m) / SR
    fr = f1 * (drop + (1.0 - drop) * np.exp(-tt / (decay * 0.8)))
    return np.sin(2 * np.pi * np.cumsum(fr) / SR) * np.exp(-tt / decay) * np.clip(tt / 0.002, 0.0, 1.0)


def click(rng, lo, hi, decay=0.006, seconds=0.05):
    e = Loop(seconds, int(rng.integers(1 << 30)))
    return e.noise(bandpass(e.f, lo, hi, 1)) * np.exp(-e.t / decay)


def burst(rng, lo, hi, seconds, attack=0.2, power=1.0):
    """A short breath of noise with soft ends (a hiss, a splash, a puff, a flap of cloth)."""
    e = Loop(seconds, int(rng.integers(1 << 30)))
    u = e.t / e.T
    return e.noise(bandpass(e.f, lo, hi, 1)) * np.clip(u / attack, 0.0, 1.0) * np.clip((1.0 - u) / (1.0 - attack), 0.0, 1.0) ** power


def creak(rng, seconds, f_lo, f_hi, rate=(38.0, 70.0), bend=0.5):
    """Something giving under load (a rope, a board, a hull): stick and slip, a rough buzz through two resonances."""
    e = Loop(seconds, int(rng.integers(1 << 30)))
    rr = rng.uniform(*rate) * (1.0 + bend * e.t / e.T)
    saw = 2.0 * ((np.cumsum(rr) / SR) % 1.0) - 1.0
    x = e.filt(saw + 0.2 * e.noise(np.ones_like(e.f)), resonance(e.f, rng.uniform(f_lo, f_hi), 9.0) + 0.6 * resonance(e.f, rng.uniform(f_lo, f_hi) * 2.3, 12.0))
    return x * np.hanning(e.n) ** 0.7


def crackle(c, rate, lo, hi, clock=None):
    """Ticks at random, `rate` a second (a fire, grit, shrimp, current). Mono, unit RMS."""
    x = np.zeros(c.n)
    k = int(rate * c.T)
    x[c.rng.integers(0, c.n, k)] = c.rng.uniform(-1.0, 1.0, k) * c.rng.uniform(0.2, 1.0, k) ** 2
    if clock is not None:
        x = x * clock
    y = c.filt(x, bandpass(c.f, lo, hi, 1))
    y = 4.0 * np.tanh(y / (4.0 * rms(y)))       # no tick more than four times the crackle's own level
    return y / rms(y)


def air(c, lo, hi, tilt_db=-3.0, width=0.9, cycles=3, depth=0.3, order=2):
    """A body of air for two ears: shaped noise on a slow clock."""
    f = c.f
    return c.noise2(bandpass(f, lo, hi, order) * tilt(f, tilt_db, math.sqrt(lo * hi)), width) * c.clock(cycles, 1.0, 1.0 - depth, 1.0)[:, None]


def gusts(c, lo, hi, cycles=3, floor=0.15, power=1.5, flut=(6.0, 18.0, 0.3), whistle=None, width=1.0):
    """Wind: noise that comes and goes in gusts with a flutter in it. whistle = (hz, sharpness, amount) adds a note
    that the gusts pull upwards. Returns the wind and the gusts' own curve (0..1)."""
    f = c.f
    g = c.clock(cycles, 0.8, floor, 1.0) ** power
    x = c.noise2(bandpass(f, lo, hi, 1) * tilt(f, -3.0, math.sqrt(lo * hi)), width) * (g * c.flutter(*flut))[:, None]
    if whistle is not None:
        hz, q, amount = whistle
        x = x + amount * (c.noise2(resonance(f, hz, q), 0.6) * (2.0 * g * (1.0 - g))[:, None] + c.noise2(resonance(f, hz * 1.22, q), 0.6) * (g * g)[:, None])
    return x, g


def hums(c, parts, lr=(1.0, 1.0)):
    """Machines that run steadily: (hz, loudness, beat). Each is a tone and, if beat is not 0, a second one that
    many cycles of the loop away, so the pair beats."""
    x = np.zeros(c.n)
    for hz, amp, beat in parts:
        f0 = c.snap(hz)
        x += amp * np.sin(2 * np.pi * f0 * c.t + c.rng.uniform(0, 6.28))
        if beat:
            x += 0.8 * amp * np.sin(2 * np.pi * (f0 + beat / c.T) * c.t + c.rng.uniform(0, 6.28))
    return np.stack([x * lr[0], x * lr[1]], axis=1)


def buzz(c, hz, top=6000.0, roll=1.0, lo=300.0, detune=0):
    """A buzz (a transformer, a neon tube): every harmonic of hz up to `top`. Mono."""
    f0 = c.snap(hz) + detune / c.T
    x = np.zeros(c.n)
    for h in range(1, int(top / f0) + 1):
        x += np.sin(2 * np.pi * f0 * h * c.t + c.rng.uniform(0, 6.28)) / h ** roll
    return c.filt(x, highpass(c.f, lo, 1))


def swells(c, count, lo, hi, rise, fall, pans=(-0.8, 0.8), move=0.0, power=2.0, jitter=0.3, tilt_db=-2.0):
    """Noise that comes up and dies away, `count` times round the loop, each at its own place and moving across by
    `move` as it passes (cars far off, steam, surges of water, wind in trees)."""
    f = c.f
    r = c.rng
    out = np.zeros((c.n, 2))
    for i in range(count):
        t0 = (i + r.uniform(-jitter, jitter)) * c.T / count
        x = c.noise(bandpass(f, lo * r.uniform(0.8, 1.25), hi * r.uniform(0.8, 1.25), 1) * tilt(f, tilt_db)) * c.swell(t0, r.uniform(*rise), r.uniform(*fall), power) * r.uniform(0.5, 1.0)
        gl, gr = pan(r.uniform(*pans) + move * np.clip(c.since(t0) / (rise[1] + fall[1]), -1.0, 1.0))
        out[:, 0] += x * gl
        out[:, 1] += x * gr
    return out


def scatter(c, make, count=0, every=None, pans=(-0.9, 0.9), level=(0.4, 1.0), clock=None, mono=False):
    """Small sounds strewn round the loop: make(rng) gives one. `count` of them at random, or one every (min, max)
    seconds; `clock` (a 0..1 curve) thins them out where it is low."""
    r = c.rng
    out = np.zeros(c.n) if mono else np.zeros((c.n, 2))
    if every is not None:
        times, t0 = [], r.uniform(0.0, every[1])
        while t0 < c.T:
            times.append(t0)
            t0 += r.uniform(*every)
    else:
        times = sorted(r.uniform(0.0, c.T, count))
    for t0 in times:
        a = r.uniform(*level)
        if clock is not None:
            a *= float(clock[int(t0 * SR) % c.n])
            if a < 0.04:
                continue
        x = make(r) * a
        if mono:
            c.put(out, x, t0)
        else:
            c.put(out, x, t0, *pan(r.uniform(*pans)))
    return out


def lapping(c, gap=(0.09, 0.42), lo=280.0, hi=800.0):
    """Water against something: slaps and bloops on a slow swell of their own."""
    r = c.rng
    lap = np.zeros((c.n, 2))
    sw = c.clock(3, 0.8, 0.25, 1.0)
    t0 = 0.0
    while t0 < c.T:
        a = float(sw[int(t0 * SR) % c.n]) * r.uniform(0.4, 1.0)
        gl, gr = pan(r.uniform(-0.9, 0.9))
        if r.random() < 0.55:
            c.put(lap, burst(r, r.uniform(350.0, 700.0), r.uniform(1200.0, 2600.0), 0.12, 0.5, 2.0) * a, t0, gl, gr)
        else:
            c.put(lap, bloop(r, r.uniform(lo, hi), r.uniform(0.04, 0.09), r.uniform(1.3, 1.9)) * a * 1.4, t0, gl, gr)
        t0 += r.uniform(*gap)
    return lap


def crickets(c, count, lo=3800.0, hi=5500.0):
    """Crickets, each on its own beat (a whole number of chirps in the loop), each falling silent and starting again."""
    r = c.rng
    ck = np.zeros((c.n, 2))
    for _ in range(count):
        fc = c.snap(r.uniform(lo, hi))
        chirps = max(1, round(r.uniform(1.7, 3.2) * c.T))
        pulses = int(r.integers(3, 5))
        pr = r.uniform(27.0, 35.0)
        tau = ((c.t * chirps / c.T + r.uniform(0, 1)) % 1.0) * c.T / chirps
        env = np.zeros(c.n)
        for k in range(pulses):
            d = tau - (0.008 + k / pr)
            env += np.where(np.abs(d) < 0.007, 0.5 + 0.5 * np.cos(np.pi * d / 0.007), 0.0) * (0.7 + 0.3 * k / pulses)
        awake = np.clip(c.clock(3, 0.6, -0.9, 1.0) * 4.0, 0.0, 1.0)
        x = (np.sin(2 * np.pi * fc * c.t) + 0.12 * np.sin(2 * np.pi * 2 * fc * c.t)) * env * awake * r.uniform(0.35, 1.0)
        gl, gr = pan(r.uniform(-1.0, 1.0))
        ck[:, 0] += x * gl
        ck[:, 1] += x * gr
    return ck


def train(c, peak, wagons, gap, rumble=(50.0, 420.0), knock_hz=120.0, squeal=0.5, whine=0.0, width=1.3):
    """A train going by (mono, a one-shot): its rumble coming up and going away, each wagon's two pairs of wheels
    knocking over the same joint, a flange squealing, the motors' whine. Its middle passes at `peak` seconds."""
    f = c.f
    r = c.rng
    env = np.exp(-0.5 * ((c.t - peak) / width) ** 2)
    x = at_level(c.noise(bandpass(f, rumble[0], rumble[1], 2) * tilt(f, -3.0, 150.0)) * c.flutter(18.0, 32.0, 0.35) * env, 0.0)
    kn = np.zeros(c.n)
    for k in range(wagons):
        t0 = peak + (k - wagons / 2.0) * gap
        for dt in (0.0, 0.085, 0.55 * gap, 0.55 * gap + 0.085):
            if 0.05 < t0 + dt < c.T - 0.4:
                a = float(np.exp(-0.5 * ((t0 + dt - peak) / width) ** 2)) * r.uniform(0.7, 1.0)
                c.put(kn, thud(knock_hz * r.uniform(0.9, 1.1), 0.035, 0.2) * a, t0 + dt)
                c.put(kn, click(r, 500.0, 2600.0, 0.008, 0.06) * a * 0.7, t0 + dt)
    x += at_peak(kn, 3.0)
    if squeal:
        x += squeal * at_peak(c.noise(resonance(f, 3100.0, 40.0) + 0.6 * resonance(f, 4300.0, 50.0)) * c.clock(5, 0.7, 0.0, 1.0) ** 2 * env, -3.0)
    if whine:
        w = np.zeros(c.n)
        for h in range(1, 7):
            w += np.sin(2 * np.pi * 190.0 * h * c.t + r.uniform(0, 6.28)) / h
        x += whine * at_level(w * env, -8.0)
    return x * c.edges(0.05, 0.5)


def steam(c, t0, lo=1500.0, hi=11000.0, attack=0.03, hold=0.5, fall=0.9, note=None):
    """Steam let go at t0 (mono, a one-shot): at once, held, then dying away; `note` adds a whistle to it."""
    f = c.f
    d = c.t - t0
    env = np.where(d < 0, 0.0, np.clip(d / attack, 0.0, 1.0)) * np.where(d < hold, 1.0, np.exp(-np.maximum(d - hold, 0.0) / fall))
    x = c.noise(bandpass(f, lo, hi, 1) * tilt(f, -1.5, 4000.0)) * env * c.flutter(20.0, 80.0, 0.2)
    if note is not None:
        x = x + 1.5 * c.noise(resonance(f, note, 14.0)) * env
    return x


def cheering(c, peak, voices=10, claps=420, width=1.0, whistles=True, seed=500, vowels=(4, 3, 0)):
    """A crowd's cheer (mono, a one-shot): a roar coming up and falling away, whoops over it, clapping under it."""
    f = c.f
    r = c.rng
    up = np.exp(-0.5 * ((c.t - peak) / width) ** 2)
    roar = c.noise(bandpass(f, 300.0, 3200.0, 1) * (0.4 + resonance(f, 750.0, 1.6) + 0.8 * resonance(f, 1250.0, 2.0) + 0.5 * resonance(f, 2600.0, 2.5)) * tilt(f, -2.0)) * c.flutter(3.0, 9.0, 0.3)
    who = np.zeros(c.n)
    for i in range(voices):
        lo, hi = r.uniform(420.0, 620.0), r.uniform(800.0, 1250.0)
        dur = r.uniform(0.5, 1.1)
        x = one_voice(dur, seed + i, lambda u: lo + (hi - lo) * np.clip(u * 1.6, 0, 1) ** 0.6, lambda u: np.sin(np.pi * u) ** 0.7, int(r.choice(vowels)), female=bool(i % 2))
        c.put(who, x * r.uniform(0.5, 1.0), max(0.0, min(c.T - dur - 0.1, peak - dur / 2 + r.normal(0.0, 0.7 * width))))
    if whistles:
        for t0, a, b in ((peak - 0.35, 2700.0, 3500.0), (peak + 0.45, 3100.0, 2500.0)):
            m = int(0.32 * SR)
            u = np.arange(m) / m
            c.put(who, 0.5 * np.sin(2 * np.pi * np.cumsum(a + (b - a) * u ** 0.5) / SR) * np.sin(np.pi * u) ** 0.5, max(0.0, t0))
    cl = np.zeros(c.n)
    for _ in range(claps):
        t0 = float(np.clip(r.normal(peak + 0.3, 0.95 * width), 0.05, c.T - 0.1))
        e = Loop(0.02, int(r.integers(1 << 30)))
        c.put(cl, e.noise(bandpass(e.f, r.uniform(900.0, 1500.0), r.uniform(2600.0, 4200.0), 1)) * np.exp(-e.t / 0.004) * r.uniform(0.3, 1.0), t0)
    x = at_level(roar * up, 0.0) + at_peak(who, 0.0) * up ** 0.5 + (at_level(cl, -7.0) if claps else 0.0)
    return (x + 0.5 * c.tail(x, 1.0, 300.0, 4500.0)) * c.edges(0.08, 0.6)


# ================================================================================================ LEVEL 7: CITY CENTER
def L7_Traffic(c):
    """The avenues: the city's low roar, cars going by one after another (tyres and air, each engine's note falling
    as it passes), a bus letting its brakes go, a horn a long way off between the buildings, the subway under the street."""
    f = c.f
    r = c.rng
    out = at_level(air(c, 35.0, 300.0, -4.0, 0.7, 2, 0.25), -5.0)
    out += at_level(air(c, 300.0, 1400.0, -3.0, 1.0, 3, 0.35, 1), -11.0)
    cars = np.zeros((c.n, 2))
    for i in range(5):
        t0 = (i + r.uniform(-0.3, 0.3)) * c.T / 5
        d = c.since(t0)
        ph = 2 * np.pi * np.cumsum(r.uniform(70.0, 130.0) * (1.0 - 0.05 * np.tanh(d / 0.5))) / SR
        eng = sum(np.sin(h * ph) / h for h in range(1, 7))
        x = (c.noise(bandpass(f, 500.0, 3200.0, 1) * tilt(f, -2.0)) + 0.6 * eng) * c.swell(t0, r.uniform(0.8, 1.4), r.uniform(0.6, 1.1)) * r.uniform(0.5, 1.0)
        gl, gr = pan(r.choice((-1.0, 1.0)) * np.clip(-d / 1.5, -1.0, 1.0) * 0.9)
        cars[:, 0] += x * gl
        cars[:, 1] += x * gr
    out += at_level(cars, -4.0)
    out += at_peak(scatter(c, lambda q: burst(q, 3000.0, 9000.0, q.uniform(0.3, 0.5), 0.08, 2.0), 2, level=(0.6, 1.0)), -16.0)
    hn = np.zeros(c.n)
    for t0, base in ((0.23 * c.T, 311.0), (0.71 * c.T, 349.0)):       # one long dull note each: nothing like the game's own beeps
        m = int(r.uniform(0.5, 0.9) * SR)
        tt = np.arange(m) / SR
        note = sum(np.sin(2 * np.pi * fq * h * tt) / h for fq in (base, base * 1.26) for h in (1, 2, 3, 4))
        c.put(hn, note * np.clip(tt / 0.03, 0.0, 1.0) * np.clip((m / SR - tt) / 0.08, 0.0, 1.0), t0)
    out += at_peak(far(c, c.filt(hn, lowpass(f, 1300.0, 2)), 1.7, 200.0, 1500.0, 0.2, 0.3, 0.08), -24.0)
    sub = c.noise(bandpass(f, 28.0, 80.0, 2)) * c.swell(0.55 * c.T, 2.2, 2.0) * (1.0 + 0.4 * np.sin(2 * np.pi * c.snap(16.0) * c.t))
    out += at_level(stereo(sub), -12.0)
    return out


def L7_Crowd(c):
    """The sidewalks at midnight: people talking as they walk (the street gives their voices back), heels on the
    pavement, a laugh, a shout for a cab and an answer from up the block, plates in the diner."""
    f = c.f
    r = c.rng
    people = [(r.uniform(95, 140) if i % 2 else r.uniform(170, 240), i % 2 == 0, r.uniform(-0.95, 0.95), (r.uniform(0.05, 0.35) if i < 3 else r.uniform(0.45, 1.0)),
               r.uniform(4.5, 6.0), r.uniform(0.7, 1.0)) for i in range(10)]
    out = at_level(crowd(c, people, 1.1), 0.0)
    wash = c.noise2(bandpass(f, 250.0, 2400.0, 1) * (0.5 + resonance(f, 560.0, 1.6) + 0.7 * resonance(f, 1500.0, 2.2)) * tilt(f, -3.0), 1.0)
    out += at_level(wash * c.clock(4, 1.0, 0.6, 1.0)[:, None], -9.0)
    out += at_level(air(c, 60.0, 250.0, -3.0, 0.9, 2, 0.2), -13.0)
    ev = np.zeros((c.n, 2))
    for t0, p0, dp, pace in ((0.05 * c.T, -0.8, 0.5, 0.47), (0.58 * c.T, 0.7, -0.5, 0.53)):
        steps = int(r.integers(6, 9))
        for i in range(steps):
            gl, gr = pan(p0 + dp * i / steps)
            c.put(ev, at_peak(click(r, 1500.0, 4500.0, 0.004, 0.04), -15.0 - 2.0 * (i % 2)), t0 + i * pace * r.uniform(0.96, 1.04), gl, gr)
            c.put(ev, at_peak(thud(r.uniform(280.0, 380.0), 0.012, 0.06), -20.0), t0 + i * pace, gl, gr)
    c.put(ev, at_peak(laugh(21, 250.0, 6), -10.0), 0.36 * c.T, *pan(-0.5))
    for seed, t0, base, p in ((31, 0.2 * c.T, 190.0, 0.7), (32, 0.8 * c.T, 150.0, -0.65)):
        x = one_voice(0.55, seed, lambda u: base * (1.5 - 0.5 * u), lambda u: np.sin(np.pi * u) ** 0.6, 1, female=False, top=4500.0, roll=1.0, vib=0.0)
        c.put(ev, at_peak(x, -12.0), t0, *pan(p))
    for t0 in sorted(r.uniform(0.0, c.T, 3)):
        c.put(ev, at_peak(glass(r), -19.0), t0, *pan(r.uniform(-0.6, 0.6)))
    return out + ev + 0.5 * c.room(ev, 1.1, 250.0, 4500.0)


def L7_Works(c):
    """The back streets' machinery: a compressor's beat, a garage's fans, mains hum, steam coming up from the
    street, drips in an alley, a shutter coming down somewhere."""
    f = c.f
    r = c.rng
    out = at_level(air(c, 50.0, 400.0, -3.0, 0.8, 2, 0.2), -8.0)
    out += at_level(hums(c, [(60.0, 1.0, 5), (120.0, 0.8, 7), (180.0, 0.3, 4), (240.0, 0.2, 9)], (1.0, 0.7)), -11.0)
    beats = max(1, round(6.5 * c.T))
    cp = np.zeros(c.n)
    for k in range(beats):
        c.put(cp, thud(62.0, 0.05, 0.2) * (1.0 if k % 2 == 0 else 0.7), k * c.T / beats)
        c.put(cp, burst(r, 300.0, 1600.0, 0.07, 0.15, 2.0) * 0.5, k * c.T / beats + 0.02)
    out += at_level(stereo(cp, -0.45), -7.0)
    fan = c.noise2(bandpass(f, 150.0, 2600.0, 1) * tilt(f, -3.0, 600.0), 0.9) * (1.0 + 0.3 * np.sin(2 * np.pi * c.snap(21.0) * c.t))[:, None]
    out += at_level(fan * c.clock(2, 1.0, 0.8, 1.0)[:, None] * (0.7, 1.0), -9.0)
    out += at_level(swells(c, 3, 2500.0, 9000.0, (0.4, 1.0), (0.8, 1.8), (-0.8, 0.8), 0.0, 1.5), -12.0)
    dr = scatter(c, lambda q: bloop(q, q.uniform(700.0, 1400.0), 0.04, 1.7), 7, mono=True)
    for t0 in (0.33 * c.T, 0.74 * c.T):
        c.put(dr, 0.9 * ping(r, r.uniform(180.0, 260.0), (1.0, 1.7, 2.9, 4.3), (1.0, 0.7, 0.5, 0.3), (0.25, 0.18, 0.1, 0.06), 0.6, 0.6), t0)
    out += at_peak(far(c, dr, 1.8, 200.0, 4000.0, 0.4, 0.2), -15.0)
    return out


def L7_Plaza(c):
    """The Plaza's open air: the fountains' jets and the water falling back into the basins, the hush of a wide
    square, a few people crossing it, the glass towers' air conditioning."""
    f = c.f
    r = c.rng
    out = at_level(c.noise2(bandpass(f, 1200.0, 9000.0, 1) * tilt(f, -1.0, 3000.0), 1.0) * (c.flutter(20.0, 80.0, 0.3) * c.clock(3, 1.0, 0.75, 1.0))[:, None], -6.0)
    out += at_level(scatter(c, lambda q: bloop(q, q.uniform(300.0, 950.0), q.uniform(0.03, 0.06), q.uniform(1.3, 1.8)), every=(0.02, 0.09), level=(0.3, 1.0)), -10.0)
    out += at_level(air(c, 90.0, 320.0, -3.0, 0.9, 2, 0.2), -11.0)
    out += at_level(air(c, 100.0, 900.0, -4.0, 1.0, 3, 0.3, 1), -14.0)
    out += at_level(hums(c, [(120.0, 1.0, 6), (180.0, 0.4, 5)], (0.6, 1.0)), -22.0)
    people = [(r.uniform(100, 135) if i % 2 else r.uniform(180, 230), i % 2 == 0, r.uniform(-0.9, 0.9), r.uniform(0.6, 1.0), r.uniform(4.2, 5.2), r.uniform(0.6, 0.9)) for i in range(4)]
    out += at_level(crowd(c, people, 1.4), -9.0)
    ev = np.zeros((c.n, 2))
    for t0, p0, dp in ((0.15 * c.T, -0.6, 0.9), (0.6 * c.T, 0.5, -0.7)):
        for i in range(7):
            c.put(ev, at_peak(click(r, 1200.0, 4000.0, 0.005, 0.04), -16.0), t0 + i * 0.5 * r.uniform(0.96, 1.04), *pan(p0 + dp * i / 7))
    return out + ev + 0.6 * c.room(ev, 1.5, 300.0, 4000.0)


def L7_Syndicate(c):
    """Syndicate territory: transformers humming either side (a few cycles apart, so the hum crawls), something very
    low under them, wind moaning between the black walls, banners cracking in the gusts, a radio, a gate."""
    f = c.f
    r = c.rng
    out = at_level(np.stack([buzz(c, 60.0, 2400.0, 1.2, 80.0), buzz(c, 60.0, 2400.0, 1.2, 80.0, 2)], axis=1) * c.clock(2, 1.0, 0.8, 1.0)[:, None], -6.0)
    out += at_level(stereo(c.noise(bandpass(f, 30.0, 70.0, 2)) * c.clock(2, 1.0, 0.3, 1.0)), -8.0)
    moan = c.noise2(resonance(f, 310.0, 14.0), 0.7) * (c.clock(3, 0.8, 0.0, 1.0) ** 1.5)[:, None] + c.noise2(resonance(f, 465.0, 16.0), 0.7) * (c.clock(4, 0.8, 0.0, 1.0) ** 2)[:, None]
    out += at_level(moan + 0.5 * c.room(moan, 1.8, 200.0, 1500.0), -10.0)
    gust = c.clock(3, 0.7, 0.0, 1.0)
    out += at_peak(scatter(c, lambda q: burst(q, 80.0, 600.0, q.uniform(0.06, 0.13), 0.1, 2.0), every=(0.12, 0.6), clock=gust ** 2), -14.0)
    out += at_level(air(c, 300.0, 3000.0, -3.0, 1.0, 3, 0.6, 1), -17.0)
    rd = np.zeros(c.n)
    for seed, t0 in ((41, 0.27 * c.T), (42, 0.69 * c.T)):       # a squelch, a few words nobody can make out, a squelch
        e = Loop(1.1, seed)
        syl = [(0.12 + i * 0.16, 0.12, 130.0 * e.rng.uniform(0.9, 1.15), VOWELS[e.rng.integers(len(VOWELS))], e.rng.uniform(0.7, 1.1)) for i in range(5)]
        f0c, amp, F = tracks(e, syl, 130.0)
        words = np.tanh(4.0 * e.filt(sing(e, f0c, amp, F, top=3000.0, roll=0.8), bandpass(e.f, 500.0, 2600.0, 3)) / 0.3)
        c.put(rd, burst(e.rng, 1500.0, 5000.0, 0.07, 0.3, 1.0) * 0.6, t0)
        c.put(rd, words * 0.5, t0 + 0.06)
        c.put(rd, burst(e.rng, 1500.0, 5000.0, 0.09, 0.3, 1.0) * 0.6, t0 + 1.15)
    out += at_peak(far(c, rd, 1.3, 400.0, 3000.0, 0.5, -0.4), -17.0)
    gate = scatter(c, lambda q: ping(q, q.uniform(140.0, 210.0), (1.0, 1.6, 2.7, 4.1), (1.0, 0.7, 0.4, 0.3), (0.3, 0.2, 0.12, 0.07), 0.8, 0.5), 2, mono=True)
    out += at_peak(far(c, gate, 2.0, 100.0, 2500.0, 0.3, 0.4), -18.0)
    return out


def L7_Siren(c):
    """A siren a long way off, crossing an avenue: a slow wail, dull, mostly echo. A one-shot; its climax is at 3.5 s."""
    env = np.exp(-0.5 * ((c.t - 3.5) / 1.7) ** 2)
    ph = 2 * np.pi * np.cumsum(690.0 + 170.0 * np.sin(2 * np.pi * c.t / 3.4 - 0.6)) / SR
    x = c.filt((np.sin(ph) + 0.4 * np.sin(2 * ph) + 0.2 * np.sin(3 * ph)) * env, bandpass(c.f, 300.0, 1700.0, 1))
    return (0.25 * x + c.tail(x, 2.6, 250.0, 1500.0, 0.12)) * c.edges(0.3, 1.0)


def L7_ElTrain(c):
    """The elevated train: six cars over the joints of the viaduct. A one-shot; its climax is at 3.2 s."""
    return train(c, 3.2, 6, 0.62, (45.0, 450.0), 110.0, 0.6, 0.5)


def L7_Premiere(c):
    """A premiere at a picture house: the crowd's cheer as somebody arrives, clapping, flash bulbs going off.
    A one-shot; its climax is at 1.8 s."""
    x = cheering(c, 1.8, 8, 700, 1.0, False, 700)
    pops = scatter(c, lambda q: click(q, 800.0, 6000.0, 0.02, 0.1), 0, every=(0.15, 0.5), mono=True) * np.exp(-0.5 * ((c.t - 1.8) / 1.2) ** 2)
    return x + at_peak(pops, -22.0) * c.edges(0.08, 0.6)


def L7_Bell(c):
    """The clock tower strikes: two strokes of a big bell, the square giving them back. A one-shot; the first stroke
    is at 0.4 s."""
    r = c.rng
    x = np.zeros(c.n)
    c.put(x, bell(r, 196.0, 5.5), 0.4)
    c.put(x, bell(r, 196.0, 4.0) * 0.85, 2.8)
    x = c.filt(x + 0.5 * c.tail(x, 2.4, 100.0, 3000.0, 0.05), lowpass(c.f, 5000.0, 1))
    return x * c.edges(0.01, 0.6)


def L7_Boulevard(c):
    """A boulevard of traffic under the car's flight: the rush of a dozen cars at once, their engines' notes
    sliding down. A one-shot; its climax is at 2.2 s."""
    f = c.f
    r = c.rng
    x = at_level(c.noise(bandpass(f, 50.0, 500.0, 2) * tilt(f, -3.0, 150.0)), 0.0) + at_level(c.noise(bandpass(f, 500.0, 3500.0, 1) * tilt(f, -2.0)) * c.flutter(1.5, 5.0, 0.5), -3.0)
    eng = np.zeros(c.n)
    for _ in range(7):
        ph = 2 * np.pi * np.cumsum(r.uniform(70.0, 150.0) * (1.0 - 0.04 * np.tanh((c.t - r.uniform(1.2, 3.2)) / 0.4))) / SR
        eng += sum(np.sin(h * ph) / h for h in range(1, 6)) * r.uniform(0.4, 1.0)
    return (x + at_level(eng, -8.0)) * np.exp(-0.5 * ((c.t - 2.2) / 0.95) ** 2) * c.edges(0.05, 0.5)


def L7_Airship(c):
    """An airship going over: two engines a little apart, beating, and their propellers' flutter; slow and low
    (a drone, not a helicopter's chop). A one-shot; its climax is at 4.5 s."""
    f = c.f
    r = c.rng
    x = np.zeros(c.n)
    for f0 in (82.0, 84.3):
        for h in range(1, 9):
            x += np.sin(2 * np.pi * f0 * h * c.t + r.uniform(0, 6.28)) / h ** 1.3
    x = at_level(x * (1.0 + 0.25 * np.sin(2 * np.pi * 27.0 * c.t)), 0.0) + at_level(c.noise(bandpass(f, 80.0, 900.0, 1) * tilt(f, -3.0, 300.0)), -9.0)
    return c.filt(x, lowpass(f, 1800.0, 1)) * np.exp(-0.5 * ((c.t - 4.5) / 2.0) ** 2) * c.edges(0.3, 1.0)


# ================================================================================================ LEVEL 1: THE CHEMICAL PLANT
def L1_Plant(c):
    """The works' own hum: transformers either side a few cycles apart (the hum crawls), the big fans with their
    blades' beat, the air of a great shed, and now and then something heavy set down far off."""
    f = c.f
    r = c.rng
    out = at_level(air(c, 40.0, 350.0, -3.0, 0.8, 2, 0.25), -8.0)
    out += at_level(hums(c, [(60.0, 1.0, 5), (120.0, 0.9, 7), (180.0, 0.35, 4), (240.0, 0.3, 9), (360.0, 0.12, 6)], (1.0, 0.8)), -7.0)
    out += at_level(np.stack([buzz(c, 120.0, 3000.0, 1.3, 400.0), buzz(c, 120.0, 3000.0, 1.3, 400.0, 3)], axis=1), -16.0)
    fan = c.noise2(bandpass(f, 150.0, 2500.0, 1) * tilt(f, -3.0, 600.0), 0.9) * (1.0 + 0.25 * np.sin(2 * np.pi * c.snap(23.0) * c.t))[:, None]
    out += at_level(fan * c.clock(2, 1.0, 0.8, 1.0)[:, None], -9.0)
    clank = scatter(c, lambda q: ping(q, q.uniform(260.0, 520.0), (1.0, 1.58, 2.4, 3.3), (1.0, 0.7, 0.5, 0.3), (0.3, 0.2, 0.12, 0.08), 0.8, 0.6), 3, mono=True)
    out += at_peak(far(c, clank, 2.2, 150.0, 3000.0, 0.25, -0.3, 0.06), -19.0)
    return out


def L1_Steam(c):
    """Steam: leaks that hiss and die away all round, a relief valve snapping open, pipes ticking as they warm
    (the ticks slowing down), a knock in the line."""
    f = c.f
    r = c.rng
    out = at_level(swells(c, 5, 2500.0, 10000.0, (0.3, 1.2), (0.8, 2.0), (-0.9, 0.9), 0.0, 1.5), -3.0)
    out += at_level(air(c, 3000.0, 9000.0, -2.0, 1.0, 4, 0.5, 1), -14.0)
    out += at_level(air(c, 60.0, 300.0, -3.0, 0.9, 2, 0.2), -12.0)
    out += at_peak(scatter(c, lambda q: burst(q, 2000.0, 11000.0, q.uniform(0.25, 0.45), 0.04, 1.5), 2, level=(0.8, 1.0)), -10.0)
    tk = np.zeros((c.n, 2))
    for t0, p in ((0.12 * c.T, -0.6), (0.57 * c.T, 0.5)):
        gap, t = 0.11, t0
        for _ in range(14):
            c.put(tk, ping(r, r.uniform(1700.0, 3600.0), (1.0, 1.9, 2.7), (1.0, 0.4, 0.2), (0.03, 0.02, 0.012), 0.12, 0.6) * r.uniform(0.5, 1.0), t, *pan(p))
            t += gap
            gap *= 1.13
    out += at_peak(tk + 0.4 * c.room(tk, 0.8, 800.0, 6000.0), -15.0)
    kn = scatter(c, lambda q: thud(q.uniform(80.0, 110.0), 0.07, 0.3) + 0.4 * ping(q, 410.0, (1.0, 2.2), (1.0, 0.4), (0.15, 0.08), 0.3, 0.2)[:int(0.3 * SR)], 2, mono=True)
    out += at_peak(far(c, kn, 1.4, 60.0, 1500.0, 0.5, 0.2), -15.0)
    return out


def L1_Pumps(c):
    """Machinery with a beat: a pump's stroke and chuff, a compressor running faster beside it, a conveyor's
    rollers rattling, their motors' hum."""
    f = c.f
    r = c.rng
    out = at_level(air(c, 50.0, 500.0, -3.0, 0.8, 2, 0.2), -10.0)
    out += at_level(hums(c, [(120.0, 1.0, 5), (240.0, 0.4, 7), (180.0, 0.3, 0)], (0.8, 1.0)), -13.0)
    a = np.zeros(c.n)
    n1 = max(1, round(3.4 * c.T))
    for k in range(n1):
        c.put(a, thud(70.0, 0.08, 0.35), k * c.T / n1)
        c.put(a, burst(r, 300.0, 1500.0, 0.09, 0.15, 2.0) * 0.6, k * c.T / n1 + 0.11)
    out += at_level(stereo(a, -0.4), -4.0)
    b = np.zeros(c.n)
    n2 = max(1, round(11.0 * c.T))
    for k in range(n2):
        c.put(b, thud(140.0, 0.02, 0.08) * (1.0 if k % 4 == 0 else 0.6), k * c.T / n2)
        c.put(b, click(r, 600.0, 2600.0, 0.006, 0.03) * 0.5, k * c.T / n2)
    out += at_level(stereo(b, 0.5), -9.0)
    n3 = max(1, round(17.0 * c.T))
    cv = np.zeros((c.n, 2))
    for k in range(n3):
        c.put(cv, click(r, 800.0, 3200.0, 0.005, 0.03) * r.uniform(0.3, 1.0), k * c.T / n3, *pan(0.8 * math.sin(2 * math.pi * k / n3)))
    out += at_level(cv, -13.0)
    return out


def L1_Reactor(c):
    """The reactor block: a slow deep throb you feel more than hear, coolant rushing through the walls, a crackle
    of current that comes and goes, a thin whine over it all."""
    f = c.f
    r = c.rng
    beat = c.snap(0.8)
    throb = np.sin(2 * np.pi * c.snap(46.0) * c.t) * np.sin(np.pi * beat * c.t) ** 4 + 0.5 * np.sin(2 * np.pi * c.snap(92.0) * c.t) * np.sin(np.pi * beat * c.t) ** 6
    out = at_level(stereo(throb), -4.0)
    out += at_level(air(c, 30.0, 90.0, -3.0, 0.5, 2, 0.4), -8.0)
    out += at_level(air(c, 300.0, 3000.0, -3.0, 1.0, 3, 0.35, 1), -9.0)
    out += at_level(stereo(crackle(c, 40.0, 3000.0, 9000.0, np.clip(c.clock(4, 0.6, -1.0, 1.0), 0.0, 1.0) ** 2), 0.3), -17.0)
    out += at_level(hums(c, [(1900.0, 1.0, 3), (2850.0, 0.4, 5)], (1.0, 0.7)) * c.clock(2, 1.0, 0.4, 1.0)[:, None], -27.0)
    out += at_level(hums(c, [(60.0, 1.0, 4), (120.0, 0.6, 6)]), -14.0)
    return out


def L1_Yard(c):
    """The open yards at night: wind coming round the tanks with a note in it, chains swinging on a gantry, a
    tannoy across the works that nobody can make out, the plant's hum from behind."""
    f = c.f
    r = c.rng
    wind, g = gusts(c, 150.0, 2500.0, 3, 0.15, 1.5, (5.0, 14.0, 0.3), (420.0, 10.0, 0.35))
    out = at_level(wind, -3.0)
    out += at_level(air(c, 50.0, 250.0, -3.0, 0.9, 2, 0.3), -9.0)
    out += at_level(hums(c, [(60.0, 1.0, 5), (120.0, 0.7, 7)], (0.5, 1.0)), -18.0)
    ch = scatter(c, lambda q: ping(q, q.uniform(2200.0, 4800.0), (1.0, 1.6, 2.4), (1.0, 0.5, 0.3), (0.05, 0.03, 0.02), 0.2, 0.5), 0, every=(0.07, 0.35), clock=g ** 2)
    out += at_peak(ch + 0.4 * c.room(ch, 0.9, 1000.0, 7000.0), -15.0)
    tn = np.zeros(c.n)
    for seed, t0 in ((51, 0.18 * c.T), (52, 0.64 * c.T)):
        e = Loop(1.9, seed)
        syl = [(0.1 + i * 0.2, 0.15, 125.0 * (1.15 - 0.03 * i) * e.rng.uniform(0.95, 1.08), VOWELS[e.rng.integers(len(VOWELS))], e.rng.uniform(0.7, 1.1)) for i in range(8)]
        f0c, amp, F = tracks(e, syl, 125.0)
        c.put(tn, np.tanh(3.0 * e.filt(sing(e, f0c, amp, F, top=3000.0, roll=0.8), bandpass(e.f, 500.0, 2400.0, 3)) / 0.3), t0)
    out += at_peak(far(c, tn, 2.4, 400.0, 2600.0, 0.15, 0.5, 0.22), -17.0)
    return out


def L1_Hooter(c):
    """The works' hooter for the change of shift, far across the plant: one long nasal note, slow to speak, the
    tanks giving it back. A one-shot; its climax is at 1.2 s."""
    r = c.rng
    x = np.zeros(c.n)
    for h in range(1, 16):
        x += np.sin(2 * np.pi * 185.0 * h * c.t + r.uniform(0, 6.28)) / h ** 0.8 * (1.6 if h in (4, 5, 6) else 1.0)
    x = c.filt(x * np.clip(c.t / 0.3, 0.0, 1.0) ** 1.5 * np.clip((2.4 - c.t) / 0.35, 0.0, 1.0), bandpass(c.f, 150.0, 2200.0, 1))
    return (0.6 * x + c.tail(x, 2.6, 150.0, 1800.0, 0.2, 8)) * c.edges(0.02, 0.6)


def L1_Vent(c):
    """A relief valve beside the road lets go: a thump, a roar of steam with a whistle in it, dying away.
    A one-shot; its climax is at 0.9 s."""
    x = at_level(steam(c, 0.55, 1200.0, 11000.0, 0.03, 0.6, 0.5, 2400.0), 0.0)
    th = np.zeros(c.n)
    c.put(th, thud(90.0, 0.08, 0.4), 0.53)
    return (x + at_peak(th, -4.0)) * c.edges(0.01, 0.3)


def L1_Core(c):
    """The reactor core close to: its throb, the hum of its field rising and sinking, current crackling round it.
    A loop; the game makes it swell and fall with the car's distance."""
    f = c.f
    beat = c.snap(1.25)
    x = at_level(np.sin(2 * np.pi * c.snap(46.0) * c.t) * np.sin(np.pi * beat * c.t) ** 4, 0.0)
    hum = np.zeros(c.n)
    for h in range(1, 13):
        hum += np.sin(2 * np.pi * c.snap(120.0) * h * c.t + c.rng.uniform(0, 6.28)) / h
    x += at_level(hum * (0.7 + 0.3 * np.sin(2 * np.pi * c.t / c.T)), -6.0)
    x += at_level(crackle(c, 70.0, 2500.0, 9000.0, 0.4 + 0.6 * np.sin(np.pi * beat * c.t) ** 2), -12.0)
    x += at_level(c.noise(bandpass(f, 200.0, 2500.0, 1) * tilt(f, -3.0)), -12.0)
    return x


def L1_Tanker(c):
    """A train of tank wagons rolling through the loading yards: slow, heavy, the couplings clanking.
    A one-shot; its climax is at 3.0 s."""
    x = train(c, 3.0, 5, 0.85, (35.0, 350.0), 85.0, 0.35, 0.0, 1.4)
    cl = scatter(c, lambda q: ping(q, q.uniform(300.0, 480.0), (1.0, 1.5, 2.3, 3.1), (1.0, 0.8, 0.5, 0.3), (0.12, 0.09, 0.06, 0.04), 0.4, 0.8), 6, mono=True)
    return x + at_peak(cl * np.exp(-0.5 * ((c.t - 3.0) / 1.4) ** 2), -9.0) * c.edges(0.05, 0.5)


# ================================================================================================ LEVEL 2: THE NEVADA DESERT
def L2_Wind(c):
    """Desert wind: gusts across open ground with a whistle in the wire, sand hissing along the surface when it
    blows hardest, the low push of it."""
    f = c.f
    wind, g = gusts(c, 120.0, 3000.0, 3, 0.15, 1.5, (5.0, 16.0, 0.3), (600.0, 8.0, 0.3))
    out = at_level(wind, 0.0)
    out += at_level(c.noise2(bandpass(f, 3000.0, 9500.0, 1), 1.0) * (g ** 2.5 * c.flutter(20.0, 60.0, 0.4))[:, None], -9.0)
    out += at_level(air(c, 40.0, 160.0, -3.0, 0.7, 2, 0.4), -6.0)
    out += at_level(c.noise2(resonance(f, 1800.0, 25.0), 0.5) * (g ** 3)[:, None], -20.0)
    return out


def L2_Cicadas(c):
    """The heat: cicadas' rasp swelling up and dying away (two choirs, never together), crickets under them, dry
    brush ticking."""
    f = c.f
    r = c.rng
    a = c.noise2(bandpass(f, 5000.0, 9500.0, 2), 1.0) * ((0.5 + 0.5 * np.sin(2 * np.pi * c.snap(110.0) * c.t)) ** 3 * c.clock(2, 1.0, 0.1, 1.0) ** 1.5)[:, None]
    b = c.noise2(bandpass(f, 3800.0, 6800.0, 2), 1.0) * ((0.5 + 0.5 * np.sin(2 * np.pi * c.snap(72.0) * c.t)) ** 3 * c.clock(3, 1.0, 0.05, 1.0) ** 2)[:, None]
    out = at_level(a * (1.0, 0.7), -2.0) + at_level(b * (0.6, 1.0), -4.0)
    out += at_peak(crickets(c, 4, 3600.0, 4800.0), -16.0)
    out += at_level(air(c, 80.0, 500.0, -3.0, 0.9, 2, 0.3), -11.0)
    out += at_peak(scatter(c, lambda q: click(q, 2000.0, 7000.0, 0.003, 0.02), 0, every=(0.1, 0.6), level=(0.2, 1.0)), -20.0)
    return out


def L2_Highway(c):
    """A far highway: the steady drone of tyres, a truck now and then with its engine's note falling as it goes,
    one of them on its engine brake."""
    f = c.f
    r = c.rng
    out = at_level(air(c, 300.0, 1800.0, -3.0, 0.9, 3, 0.25, 1), -5.0)
    out += at_level(air(c, 50.0, 260.0, -3.0, 0.7, 2, 0.3), -5.0)
    tr = np.zeros((c.n, 2))
    for i in range(3):
        t0 = (i + r.uniform(-0.25, 0.25)) * c.T / 3
        d = c.since(t0)
        ph = 2 * np.pi * np.cumsum(r.uniform(82.0, 112.0) * (1.0 - 0.035 * np.tanh(d / 0.9))) / SR
        x = (c.noise(bandpass(f, 80.0, 900.0, 1) * tilt(f, -3.0, 250.0)) + 0.7 * sum(np.sin(h * ph) / h for h in range(1, 8))) * c.swell(t0, r.uniform(2.0, 2.8), r.uniform(1.5, 2.2))
        gl, gr = pan(r.choice((-1.0, 1.0)) * np.clip(-d / 3.0, -1.0, 1.0) * 0.8)
        tr[:, 0] += x * gl
        tr[:, 1] += x * gr
    out += at_level(tr, -3.0)
    jk = np.zeros(c.n)
    for k in range(26):
        c.put(jk, thud(95.0, 0.02, 0.06) * (1.0 - 0.02 * k), 0.4 * c.T + k / 28.0)
    out += at_peak(far(c, jk, 1.2, 60.0, 900.0, 0.3, -0.4), -14.0)
    return out


def L2_Neon(c):
    """The strip: neon tubes buzzing (one of them flickering), their transformers, slot machines chattering
    through open doors (reels, a bell, coins), a murmur of people."""
    f = c.f
    r = c.rng
    flick = np.where(c.noise(bandpass(f, 3.0, 14.0, 1)) > 0.9, 0.25, 1.0)
    flick = c.filt(flick, np.exp(-0.5 * (f / 60.0) ** 2))
    out = at_level(np.stack([buzz(c, 120.0, 7000.0, 0.8, 1200.0) * flick, buzz(c, 120.0, 7000.0, 0.8, 1500.0, 2)], axis=1), -5.0)
    out += at_level(hums(c, [(60.0, 1.0, 4), (120.0, 0.8, 6), (180.0, 0.3, 5)]), -9.0)
    slot = np.zeros((c.n, 2))
    for i in range(5):
        t0 = (i + r.uniform(-0.3, 0.3)) * c.T / 5
        gl, gr = pan(r.uniform(-0.8, 0.8))
        for k in range(int(r.integers(10, 16))):
            c.put(slot, click(r, 1200.0, 4500.0, 0.004, 0.03) * 0.6, t0 + k / 30.0, gl, gr)
        if i % 2 == 0:
            c.put(slot, ping(r, r.uniform(1300.0, 2600.0), (1.0, 2.76, 5.4), (1.0, 0.45, 0.2), (0.2, 0.1, 0.05), 0.5, 0.4), t0 + 0.6, gl, gr)
        for k in range(int(r.integers(3, 9))):
            c.put(slot, ping(r, r.uniform(3200.0, 6000.0), (1.0, 1.4, 2.2), (1.0, 0.6, 0.3), (0.03, 0.02, 0.012), 0.1, 0.6) * r.uniform(0.4, 0.9), t0 + 0.9 + k * r.uniform(0.05, 0.11), gl, gr)
    out += at_peak(slot + 0.3 * c.room(slot, 0.7, 500.0, 6000.0), -9.0)
    wash = c.noise2(bandpass(f, 250.0, 2400.0, 1) * (0.5 + resonance(f, 560.0, 1.6) + 0.7 * resonance(f, 1500.0, 2.2)) * tilt(f, -3.0), 1.0)
    out += at_level(wash * c.clock(4, 1.0, 0.6, 1.0)[:, None], -11.0)
    return out


def L2_Canyon(c):
    """Red rock: wind moaning in the pass on three notes that never sound together, stones trickling down a face,
    everything given back by the walls."""
    f = c.f
    r = c.rng
    moan = np.zeros((c.n, 2))
    for hz, q, cyc in ((180.0, 12.0, 3), (270.0, 14.0, 4), (410.0, 16.0, 5)):
        moan += c.noise2(resonance(f, hz, q), 0.7) * (c.clock(cyc, 0.8, 0.0, 1.0) ** 2)[:, None]
    out = at_level(moan + 0.7 * c.room(moan, 2.6, 100.0, 1500.0, 0.1), -2.0)
    wind, g = gusts(c, 200.0, 2500.0, 3, 0.2, 1.5)
    out += at_level(wind, -7.0)
    out += at_level(air(c, 40.0, 200.0, -3.0, 0.8, 2, 0.3), -8.0)
    st = np.zeros(c.n)
    for t0 in (0.25 * c.T, 0.7 * c.T):
        gap, t = 0.05, t0
        for _ in range(int(r.integers(9, 15))):
            c.put(st, click(r, 900.0, 4000.0, 0.006, 0.04) * r.uniform(0.3, 1.0), t)
            c.put(st, thud(r.uniform(250.0, 420.0), 0.015, 0.06) * r.uniform(0.2, 0.6), t)
            t += gap * r.uniform(0.5, 1.6)
            gap *= 1.12
    out += at_peak(far(c, st, 2.4, 300.0, 4000.0, 0.4, -0.3, 0.12), -14.0)
    return out


def L2_Truck(c):
    """A big rig going the other way: its roar coming up, tyres singing, the slap of its air as it passes, gone.
    A one-shot; its climax is at 2.2 s."""
    f = c.f
    r = c.rng
    env = np.exp(-0.5 * ((c.t - 2.2) / 0.75) ** 2)
    ph = 2 * np.pi * np.cumsum(96.0 * (1.0 - 0.04 * np.tanh((c.t - 2.2) / 0.3))) / SR
    x = at_level(c.noise(bandpass(f, 60.0, 1200.0, 1) * tilt(f, -3.0, 250.0)) * c.flutter(8.0, 25.0, 0.3), 0.0)
    x += at_level(sum(np.sin(h * ph) / h ** 0.9 for h in range(1, 10)), -3.0)
    x += at_level(c.noise(bandpass(f, 900.0, 2600.0, 2)), -9.0)
    buf = np.zeros(c.n)
    c.put(buf, burst(r, 40.0, 300.0, 0.35, 0.2, 2.0), 2.1)
    return (x * env + at_peak(buf, -6.0)) * c.edges(0.05, 0.5)


def L2_Hawk(c):
    """A hawk over the pass: one long falling cry, and the rock sending it back twice. A one-shot at 0.8 s."""
    m = int(0.95 * SR)
    u = np.arange(m) / m
    ph = 2 * np.pi * np.cumsum(2900.0 * (0.72 + 0.28 * (1.0 - u) ** 1.5) * (0.92 + 0.08 * np.clip(u / 0.06, 0.0, 1.0))) / SR
    cry = (np.sin(ph) + 0.4 * np.sin(2 * ph) + 0.15 * np.sin(3 * ph)) * np.sin(np.pi * u) ** 0.5 * np.exp(-0.8 * u) * (1.0 + 0.3 * np.sin(2 * np.pi * 55.0 * u * 0.95))
    x = np.zeros(c.n)
    c.put(x, cry, 0.35)
    c.put(x, cry * 0.3, 1.25)
    c.put(x, cry * 0.12, 2.05)
    x = c.filt(x, bandpass(c.f, 1200.0, 6000.0, 1))
    return (x + 0.8 * c.tail(x, 2.2, 800.0, 4500.0, 0.15)) * c.edges(0.01, 0.5)


def L2_GasBell(c):
    """Last Chance Gas: a car rolls over the forecourt's hose (ding, ding), and the pump starts to run.
    A one-shot; the bell is at 0.7 s."""
    r = c.rng
    x = np.zeros(c.n)
    for t0 in (0.55, 0.9):
        c.put(x, ping(r, 1240.0, (1.0, 2.4, 4.1, 5.9), (1.0, 0.5, 0.3, 0.15), (0.45, 0.25, 0.12, 0.07), 1.2, 0.3), t0)
    pump = np.zeros(c.n)
    for k in range(40):
        c.put(pump, click(r, 500.0, 2500.0, 0.008, 0.04) * 0.6 + 0.0, 1.4 + k * 0.04)
    hum = np.sin(2 * np.pi * 118.0 * c.t) * np.clip((c.t - 1.3) / 0.2, 0.0, 1.0)
    return (at_peak(x, 0.0) + at_peak(pump, -14.0) + at_peak(hum, -16.0)) * c.edges(0.01, 0.5)


def L2_Jackpot(c):
    """Jackpot Junction: somebody's machine pays out. Its bell rings and rings, the coins come down in a rush,
    two or three people whoop. A one-shot; its climax is at 1.4 s."""
    r = c.rng
    env = np.clip(c.t / 0.25, 0.0, 1.0) * np.clip((3.4 - c.t) / 1.2, 0.0, 1.0)
    b = np.zeros(c.n)
    for k in range(40):
        c.put(b, ping(r, 1850.0, (1.0, 2.7, 5.2), (1.0, 0.4, 0.2), (0.08, 0.05, 0.03), 0.2, 0.5), 0.3 + k / 14.0)
    co = np.zeros(c.n)
    for _ in range(70):
        c.put(co, ping(r, r.uniform(3000.0, 6200.0), (1.0, 1.45, 2.3), (1.0, 0.6, 0.3), (0.035, 0.02, 0.012), 0.1, 0.7) * r.uniform(0.3, 1.0), float(np.clip(r.normal(1.9, 0.6), 0.6, 3.6)))
    who = np.zeros(c.n)
    for i in range(3):
        lo, hi = r.uniform(450.0, 600.0), r.uniform(850.0, 1150.0)
        c.put(who, one_voice(0.8, 800 + i, lambda u: lo + (hi - lo) * np.clip(u * 1.6, 0, 1) ** 0.6, lambda u: np.sin(np.pi * u) ** 0.7, 4, female=bool(i % 2)), 1.2 + 0.4 * i)
    x = at_peak(b * env, -3.0) + at_peak(co, -5.0) + at_peak(who, -7.0)
    return (x + 0.3 * c.tail(x, 0.8, 500.0, 6000.0)) * c.edges(0.02, 0.5)


# ================================================================================================ LEVEL 3: THE VOLCANO
def L3_Rumble(c):
    """The mountain: a rumble under everything that surges and sinks, a boom far inside it now and then with the
    rattle of what it shook loose, stones coming down a slope."""
    f = c.f
    r = c.rng
    out = at_level(air(c, 25.0, 120.0, -4.0, 0.6, 2, 0.6), 0.0)
    out += at_level(air(c, 100.0, 420.0, -4.0, 0.9, 3, 0.4), -9.0)
    bm = np.zeros(c.n)
    rt = np.zeros(c.n)
    for i in range(3):
        t0 = (i + r.uniform(-0.25, 0.25)) * c.T / 3
        c.put(bm, thud(r.uniform(38.0, 52.0), 0.5, 1.6, 0.6) * r.uniform(0.6, 1.0), t0)
        t, gap = t0 + 0.3, 0.05
        for _ in range(12):
            c.put(rt, thud(r.uniform(180.0, 420.0), 0.02, 0.08) * r.uniform(0.2, 1.0), t)
            c.put(rt, click(r, 600.0, 3000.0, 0.008, 0.04) * r.uniform(0.2, 0.8), t)
            t += gap * r.uniform(0.5, 1.5)
            gap *= 1.15
    out += at_peak(far(c, bm, 2.5, 25.0, 300.0, 0.7, 0.0, 0.1), -4.0)
    out += at_peak(far(c, rt, 1.8, 150.0, 3000.0, 0.4, 0.3, 0.08), -16.0)
    return out


def L3_Lava(c):
    """The lava channels: their low roar, the crust crackling and spitting as it breaks, thick bubbles swelling
    and bursting with a gasp of gas."""
    f = c.f
    r = c.rng
    out = at_level(air(c, 60.0, 520.0, -3.0, 0.9, 3, 0.35) * c.flutter(3.0, 9.0, 0.25)[:, None], -3.0)
    sw = c.clock(4, 0.8, 0.3, 1.0)
    out += at_level(np.stack([crackle(c, 70.0, 1500.0, 8000.0, sw), crackle(c, 70.0, 1500.0, 8000.0, sw[::-1].copy())], axis=1), -9.0)
    out += at_peak(scatter(c, lambda q: click(q, 800.0, 3500.0, 0.01, 0.06) + 0.0, 9, level=(0.5, 1.0)), -11.0)

    def glop(q):
        x = bloop(q, q.uniform(110.0, 280.0), q.uniform(0.12, 0.22), q.uniform(1.3, 1.7))
        return np.concatenate((x, burst(q, 1500.0, 6000.0, 0.09, 0.1, 2.0) * 0.4))
    out += at_peak(scatter(c, glop, 11, level=(0.5, 1.0)), -8.0)
    return out


def L3_Ash(c):
    """Falling ash: a dry hush over everything, wind pushing the drifts about, grit pattering down."""
    f = c.f
    out = at_level(c.noise2(bandpass(f, 1500.0, 7000.0, 1) * tilt(f, -4.0, 3000.0), 1.0) * c.clock(3, 1.0, 0.6, 1.0)[:, None], -4.0)
    wind, g = gusts(c, 200.0, 1800.0, 3, 0.2, 1.3, (4.0, 12.0, 0.25))
    out += at_level(wind, -5.0)
    out += at_level(np.stack([crackle(c, 130.0, 3000.0, 10000.0, c.clock(3, 0.8, 0.3, 1.0)), crackle(c, 130.0, 3000.0, 10000.0, c.clock(3, 0.8, 0.3, 1.0))], axis=1), -15.0)
    out += at_level(air(c, 50.0, 220.0, -3.0, 0.8, 2, 0.3), -8.0)
    return out


def L3_Vents(c):
    """Fumaroles: three jets of steam, each with its own note, each coming and going; gas hissing from the cracks;
    mud gulping in its pots."""
    f = c.f
    r = c.rng
    out = at_level(air(c, 3000.0, 10000.0, -2.0, 1.0, 4, 0.5, 1), -10.0)
    jets = np.zeros((c.n, 2))
    for hz, p, cyc in ((700.0, -0.7, 2), (1150.0, 0.6, 3), (1900.0, 0.0, 4)):
        j = (1.6 * c.noise(resonance(f, hz, 6.0)) + c.noise(bandpass(f, 2000.0, 9000.0, 1))) * c.clock(cyc, 0.8, 0.05, 1.0) ** 1.5 * c.flutter(15.0, 50.0, 0.2)
        jets += stereo(j, p)
    out += at_level(jets, -2.0)

    def gulp(q):
        x = bloop(q, q.uniform(70.0, 160.0), q.uniform(0.2, 0.35), q.uniform(1.2, 1.5))
        return np.concatenate((x, click(q, 500.0, 2500.0, 0.02, 0.08) * 0.5))
    out += at_peak(scatter(c, gulp, 8, level=(0.5, 1.0)), -7.0)
    out += at_level(air(c, 40.0, 200.0, -4.0, 0.7, 2, 0.4), -9.0)
    return out


def L3_Works(c):
    """The mine: the crusher grinding and the rock cracking in it, its conveyor's rollers, a pump's slow stroke
    on the channel's bank, the motors."""
    f = c.f
    r = c.rng
    grind = c.noise2(bandpass(f, 150.0, 1500.0, 1) * tilt(f, -2.0, 500.0), 0.8) * (c.flutter(15.0, 40.0, 0.6) * (0.75 + 0.25 * np.sin(2 * np.pi * c.snap(0.6) * c.t)))[:, None]
    out = at_level(grind * (1.0, 0.6), -3.0)
    out += at_peak(scatter(c, lambda q: click(q, 500.0, 3500.0, 0.012, 0.07), 14, pans=(-0.9, 0.0), level=(0.4, 1.0)), -9.0)
    n3 = max(1, round(9.0 * c.T))
    cv = np.zeros((c.n, 2))
    for k in range(n3):
        c.put(cv, click(r, 700.0, 2800.0, 0.006, 0.03) * r.uniform(0.4, 1.0), k * c.T / n3, *pan(0.2 + 0.6 * math.sin(2 * math.pi * k / n3)))
    out += at_level(cv, -12.0)
    a = np.zeros(c.n)
    n1 = max(1, round(2.2 * c.T))
    for k in range(n1):
        c.put(a, thud(64.0, 0.09, 0.4), k * c.T / n1)
        c.put(a, burst(r, 250.0, 1300.0, 0.12, 0.15, 2.0) * 0.6, k * c.T / n1 + 0.14)
    out += at_level(stereo(a, 0.5), -7.0)
    out += at_level(hums(c, [(60.0, 1.0, 5), (120.0, 0.8, 7), (180.0, 0.35, 4)]), -12.0)
    out += at_level(air(c, 40.0, 300.0, -3.0, 0.8, 2, 0.25), -9.0)
    return out


def L3_FireCurtain(c):
    """The curtain of fire along the fissure: flames roaring and tearing, the crackle in them, a whoomp when a
    new sheet goes up. A loop; the game makes it swell and fall with the car's distance."""
    f = c.f
    r = c.rng
    x = at_level(c.noise(bandpass(f, 100.0, 2500.0, 1) * tilt(f, -3.0, 400.0)) * c.flutter(4.0, 14.0, 0.6) * c.clock(4, 0.8, 0.6, 1.0), 0.0)
    x += at_level(crackle(c, 60.0, 1500.0, 9000.0), -10.0)
    wh = np.zeros(c.n)
    for i in range(4):
        c.put(wh, burst(r, 40.0, 260.0, r.uniform(0.5, 0.8), 0.12, 2.0), (i + r.uniform(-0.3, 0.3)) * c.T / 4)
    return x + at_peak(wh, -5.0)


def L3_Eruption(c):
    """The cone goes up, half a kilometre off: a boom you feel first, the roar of the column behind it, then
    everything it threw coming down. A one-shot; its climax is at 2.0 s."""
    f = c.f
    r = c.rng
    x = np.zeros(c.n)
    c.put(x, thud(44.0, 0.9, 3.0, 0.55), 1.75)
    x = at_peak(x, 0.0)
    d = c.t - 1.8
    blast = c.noise(bandpass(f, 30.0, 260.0, 2)) * np.where(d < 0, 0.0, np.clip(d / 0.25, 0.0, 1.0) * np.exp(-np.maximum(d, 0.0) / 2.0))
    roar = c.noise(bandpass(f, 200.0, 1600.0, 1) * tilt(f, -3.0, 500.0)) * np.where(d < 0, 0.0, np.clip(d / 0.6, 0.0, 1.0) * np.exp(-np.maximum(d, 0.0) / 2.2)) * c.flutter(3.0, 10.0, 0.4)
    deb = scatter(c, lambda q: thud(q.uniform(120.0, 380.0), 0.03, 0.12) + 0.0, 40, mono=True) * np.clip((c.t - 3.0) / 0.8, 0.0, 1.0) * np.clip((6.8 - c.t) / 2.0, 0.0, 1.0)
    x = x + at_peak(blast, -2.0) + at_peak(roar, -7.0) + at_peak(deb, -16.0)
    return c.filt(x + 0.5 * c.tail(x, 2.8, 30.0, 1500.0, 0.2), lowpass(f, 2500.0, 1)) * c.edges(0.05, 0.8)


def L3_Geyser(c):
    """A vent beside the road lets go: a gurgle, then a jet of steam and water with a falling note, and its rain.
    A one-shot; its climax is at 0.8 s."""
    r = c.rng
    g = np.zeros(c.n)
    for k in range(6):
        c.put(g, bloop(r, r.uniform(120.0, 300.0), 0.08, 1.5), 0.05 + k * 0.07)
    x = at_level(steam(c, 0.5, 800.0, 10000.0, 0.05, 0.7, 0.6, 1500.0), 0.0) + at_peak(g, -8.0)
    rain = scatter(c, lambda q: bloop(q, q.uniform(900.0, 2400.0), 0.03, 1.6), 0, every=(0.01, 0.05), mono=True) * np.clip((c.t - 1.2) / 0.4, 0.0, 1.0)
    return (x + at_peak(rain, -14.0)) * c.edges(0.01, 0.4)


def L3_SlagTrain(c):
    """A train of slag wagons on the mine's railway: slow, heavy, its load ticking as it cools.
    A one-shot; its climax is at 3.0 s."""
    x = train(c, 3.0, 5, 0.9, (35.0, 320.0), 80.0, 0.3, 0.0, 1.4)
    tk = scatter(c, lambda q: ping(q, q.uniform(1500.0, 3500.0), (1.0, 1.9), (1.0, 0.4), (0.03, 0.02), 0.1, 0.6), 0, every=(0.04, 0.2), mono=True)
    return x + at_peak(tk * np.exp(-0.5 * ((c.t - 3.0) / 1.2) ** 2), -15.0) * c.edges(0.05, 0.5)


# ================================================================================================ LEVEL 4: THE THEME PARK
# (The level's music is a carnival tune already: nothing here plays a tune.)

def L4_Crowd(c):
    """A park full of people: families talking over one another, children shrieking with delight, a stall's bell."""
    f = c.f
    r = c.rng
    people = [((r.uniform(250, 330), True) if i % 3 == 0 else ((r.uniform(180, 250), True) if i % 3 == 1 else (r.uniform(105, 145), False))) + (r.uniform(-0.95, 0.95), r.uniform(0.1, 1.0), r.uniform(5.0, 6.5), r.uniform(0.8, 1.1)) for i in range(11)]
    out = at_level(crowd(c, people, 0.9, 4800.0), 0.0)
    wash = c.noise2(bandpass(f, 300.0, 3200.0, 1) * (0.5 + resonance(f, 700.0, 1.5) + 0.8 * resonance(f, 1800.0, 2.0)) * tilt(f, -2.5), 1.0)
    out += at_level(wash * c.clock(4, 1.0, 0.6, 1.0)[:, None], -6.0)
    sh = np.zeros((c.n, 2))
    for i in range(4):
        top = r.uniform(1100.0, 1600.0)
        dur = r.uniform(0.4, 0.8)
        x = one_voice(dur, 900 + i, lambda u: top * (0.7 + 0.3 * np.sin(np.pi * np.clip(u * 1.3, 0, 1))), lambda u: np.sin(np.pi * u) ** 0.8, int(r.choice((2, 0, 8))))
        c.put(sh, x, (i + r.uniform(-0.3, 0.3)) * c.T / 4, *pan(r.uniform(-0.8, 0.8)))
    out += at_peak(sh + 0.5 * c.room(sh, 0.9, 500.0, 5000.0), -12.0)
    bl = scatter(c, lambda q: ping(q, 2400.0, (1.0, 2.76, 5.4), (1.0, 0.45, 0.2), (0.2, 0.1, 0.05), 0.5, 0.4), 2, pans=(0.3, 0.8))
    out += at_peak(bl + 0.5 * c.room(bl, 0.9, 800.0, 6000.0), -19.0)
    out += at_level(air(c, 70.0, 240.0, -3.0, 0.9, 2, 0.2), -15.0)
    return out


def L4_Rides(c):
    """The rides: a lift chain's ratchet, a train coming down with its rumble and a carful of screams, air brakes
    letting go, a spinning ride's arms rushing round, the motors."""
    f = c.f
    r = c.rng
    out = at_level(hums(c, [(110.0, 1.0, 6), (165.0, 0.5, 0), (220.0, 0.3, 4)]) + 0.3 * c.noise2(bandpass(f, 60.0, 260.0, 2), 0.8), -14.0)
    turn = max(1, round(c.T / 1.6))
    out += at_level(c.noise2(bandpass(f, 300.0, 1600.0, 1) * tilt(f, -2.0), 0.6) * ((0.5 + 0.5 * np.sin(2 * np.pi * turn * c.t / c.T)) ** 3)[:, None] * (1.0, 0.6), -9.0)
    ride = np.zeros((c.n, 2))
    t0, gap = 0.06 * c.T, 0.125
    while t0 < 0.4 * c.T:
        c.put(ride, (click(r, 700.0, 2800.0, 0.01, 0.06)) * r.uniform(0.7, 1.0), t0, *pan(0.5))
        t0 += gap * r.uniform(0.96, 1.04)
    td = 0.55 * c.T
    sweep = np.clip(c.since(td) / 1.6, -1.0, 1.0)
    gl, gr = pan(0.6 - 1.1 * (sweep * 0.5 + 0.5))
    tr = 2.2 * c.noise(bandpass(f, 60.0, 420.0, 2)) * c.swell(td, 1.1, 1.2) * (1.0 + 0.5 * np.sin(2 * np.pi * c.snap(41.0) * c.t)) + c.noise(bandpass(f, 500.0, 2600.0, 1)) * c.swell(td + 0.15, 0.7, 0.8, 3.0)
    ride[:, 0] += tr * gl * 0.9
    ride[:, 1] += tr * gr * 0.9
    out += at_peak(ride + 0.4 * c.room(ride, 1.0, 200.0, 4000.0), -5.0)
    sc = np.zeros(c.n)
    for i in range(6):
        top = r.uniform(950.0, 1400.0)
        dur = r.uniform(1.1, 1.8)
        x = one_voice(dur, 950 + i, lambda u: top * (0.72 + 0.28 * np.clip(u / 0.12, 0, 1)) * (1.0 - 0.22 * u ** 1.5), lambda u: np.clip(u / 0.08, 0, 1) * (1.0 - u) ** 0.8, int(r.choice((0, 6, 1))))
        c.put(sc, x * r.uniform(0.6, 1.0), td - 0.35 + r.uniform(0.0, 0.5))
    sc = c.filt(sc, bandpass(f, 500.0, 5000.0, 1))
    out += at_peak(np.stack([sc * gl, sc * gr], axis=1) * 0.6 + c.room(sc, 1.2, 500.0, 4500.0, 0.03), -9.0)
    out += at_peak(scatter(c, lambda q: burst(q, 3000.0, 9500.0, q.uniform(0.3, 0.5), 0.06, 2.0), 2, level=(0.7, 1.0)), -13.0)
    return out


def L4_Lagoon(c):
    """Water: the lagoon lapping under the rails, fountains playing, a boat's bow coming down with a splash,
    mooring ropes creaking."""
    f = c.f
    r = c.rng
    out = at_level(air(c, 80.0, 520.0, -3.0, 0.9, 3, 0.35), -7.0)
    out += at_level(lapping(c), -6.0)
    out += at_level(c.noise2(bandpass(f, 1500.0, 9000.0, 1) * tilt(f, -1.0, 3500.0), 1.0) * (c.flutter(20.0, 80.0, 0.3) * c.clock(3, 1.0, 0.5, 1.0))[:, None] * (0.6, 1.0), -11.0)
    sp = np.zeros((c.n, 2))
    for t0, p in ((0.3 * c.T, -0.5), (0.78 * c.T, 0.6)):
        c.put(sp, burst(r, 400.0, 5000.0, 0.6, 0.08, 2.5), t0, *pan(p))
        for k in range(8):
            c.put(sp, bloop(r, r.uniform(400.0, 1200.0), 0.05, 1.6) * 0.4, t0 + 0.2 + k * r.uniform(0.04, 0.09), *pan(p))
    out += at_peak(sp, -9.0)
    cr = scatter(c, lambda q: creak(q, q.uniform(0.5, 0.9), 380.0, 620.0), 3)
    out += at_peak(cr + 0.5 * c.room(cr, 0.7, 300.0, 3000.0), -18.0)
    return out


def L4_Rocket(c):
    """Rocket Land: the machines' steady hum on three notes, pneumatics letting go with a thunk, a countdown over
    the speakers that nobody can make out, a crackle in the air."""
    f = c.f
    r = c.rng
    out = at_level(hums(c, [(140.0, 1.0, 5), (210.0, 0.6, 7), (280.0, 0.35, 4)]), -6.0)
    out += at_level(hums(c, [(1900.0, 1.0, 3), (2600.0, 0.5, 5)], (0.7, 1.0)) * c.clock(2, 1.0, 0.4, 1.0)[:, None], -24.0)
    out += at_level(air(c, 60.0, 320.0, -3.0, 0.8, 2, 0.25), -9.0)
    out += at_level(air(c, 400.0, 3000.0, -3.0, 1.0, 3, 0.3, 1), -13.0)

    def pneu(q):
        x = burst(q, 2000.0, 8000.0, q.uniform(0.12, 0.25), 0.05, 1.5)
        return np.concatenate((x, thud(q.uniform(90.0, 120.0), 0.04, 0.15) * 0.9))
    out += at_peak(scatter(c, pneu, 5, level=(0.6, 1.0)), -10.0)
    tn = np.zeros(c.n)
    for seed, t0 in ((61, 0.1 * c.T), (62, 0.55 * c.T)):
        e = Loop(2.2, seed)
        syl = [(0.1 + i * 0.4, 0.2, 120.0 * e.rng.uniform(0.97, 1.03), VOWELS[e.rng.integers(len(VOWELS))], 1.0) for i in range(5)]
        f0c, amp, F = tracks(e, syl, 120.0)
        c.put(tn, np.tanh(3.0 * e.filt(sing(e, f0c, amp, F, top=3000.0, roll=0.8), bandpass(e.f, 500.0, 2400.0, 3)) / 0.3), t0)
    out += at_peak(far(c, tn, 1.8, 400.0, 2600.0, 0.2, -0.4, 0.15), -15.0)
    out += at_level(stereo(crackle(c, 25.0, 3000.0, 9000.0, np.clip(c.clock(4, 0.6, -1.0, 1.0), 0.0, 1.0) ** 2), 0.4), -20.0)
    return out


def L4_Funhouse(c):
    """The Funhouse: boards creaking underfoot, a bellows wheezing in and out, barrels rolling behind the walls,
    somebody cackling inside, a spring going."""
    f = c.f
    r = c.rng
    out = at_level(air(c, 60.0, 300.0, -3.0, 0.8, 2, 0.3), -9.0)
    breath = max(1, round(0.35 * c.T))
    whz = (c.noise(bandpass(f, 300.0, 2000.0, 1)) + 1.5 * c.noise(resonance(f, 480.0, 8.0))) * np.sin(np.pi * breath * c.t / c.T) ** 2
    out += at_level(stereo(whz, -0.3), -6.0)
    roll = c.noise2(bandpass(f, 60.0, 250.0, 2), 0.6) * (0.6 + 0.4 * np.sin(2 * np.pi * c.snap(2.3) * c.t))[:, None]
    out += at_level(roll, -6.0)
    out += at_peak(scatter(c, lambda q: thud(q.uniform(90.0, 150.0), 0.04, 0.2) + 0.0, 9, level=(0.4, 1.0)), -11.0)
    cr = scatter(c, lambda q: creak(q, q.uniform(0.3, 0.7), 250.0, 900.0, (30.0, 90.0)), 7)
    out += at_peak(cr + 0.5 * c.room(cr, 0.8, 200.0, 4000.0), -9.0)
    ck = np.zeros(c.n)
    c.put(ck, laugh(71, 150.0, 7, False), 0.2 * c.T)
    c.put(ck, laugh(72, 340.0, 6, True) * 0.8, 0.68 * c.T)
    out += at_peak(far(c, c.filt(ck, bandpass(f, 300.0, 3500.0, 1)), 1.4, 300.0, 3500.0, 0.3, 0.3, 0.08), -11.0)

    def boing(q):
        m = int(0.6 * SR)
        tt = np.arange(m) / SR
        return np.sin(2 * np.pi * np.cumsum(q.uniform(180.0, 260.0) * (1.0 + 0.25 * np.sin(2 * np.pi * 9.0 * tt) * np.exp(-tt / 0.2))) / SR) * np.exp(-tt / 0.18)
    out += at_peak(scatter(c, boing, 2), -17.0)
    return out


def L4_Coaster(c):
    """A coaster's train coming down overhead: the wheels' roar on the rail, the rush of it, a carful of screams.
    A one-shot; its climax is at 2.4 s."""
    f = c.f
    r = c.rng
    env = np.exp(-0.5 * ((c.t - 2.4) / 0.85) ** 2)
    x = at_level(c.noise(bandpass(f, 60.0, 500.0, 2)) * (1.0 + 0.5 * np.sin(2 * np.pi * 43.0 * c.t)), 0.0) + at_level(c.noise(bandpass(f, 500.0, 2800.0, 1)), -5.0)
    x += at_level(crackle(c, 22.0, 600.0, 3000.0), -8.0)
    sc = np.zeros(c.n)
    for i in range(7):
        top = r.uniform(950.0, 1450.0)
        dur = r.uniform(1.0, 1.7)
        v = one_voice(dur, 1000 + i, lambda u: top * (0.72 + 0.28 * np.clip(u / 0.12, 0, 1)) * (1.0 - 0.22 * u ** 1.5), lambda u: np.clip(u / 0.08, 0, 1) * (1.0 - u) ** 0.8, int(r.choice((0, 6, 1))))
        c.put(sc, v * r.uniform(0.6, 1.0), max(0.0, min(c.T - dur - 0.1, 2.4 - dur / 2 + r.uniform(-0.8, 0.6))))
    sc = c.filt(sc, bandpass(f, 500.0, 5200.0, 1))
    return (x * env + at_peak(sc + 0.5 * c.tail(sc, 1.0, 500.0, 4500.0), -2.0) * env ** 0.4) * c.edges(0.05, 0.5)


def L4_ShipBell(c):
    """Pirate Cove: the galleon's bell, twice, and her timbers working. A one-shot; the bell is at 0.6 s."""
    r = c.rng
    x = np.zeros(c.n)
    c.put(x, bell(r, 880.0, 2.2, 0.8), 0.5)
    c.put(x, bell(r, 880.0, 2.0, 0.8) * 0.9, 0.95)
    cr = np.zeros(c.n)
    for t0 in (0.1, 1.8, 2.7):
        c.put(cr, creak(r, 0.8, 220.0, 420.0, (20.0, 45.0)), t0)
    x = at_peak(x, 0.0) + at_peak(cr, -9.0)
    return (x + 0.3 * c.tail(x, 1.2, 200.0, 4000.0)) * c.edges(0.02, 0.5)


def L4_Splash(c):
    """Splashdown: the boat hits the water (a thump, a wall of spray coming down like rain) and everybody who
    was watching gets it. A one-shot; its climax is at 1.3 s."""
    f = c.f
    r = c.rng
    d = c.t - 1.2
    hit = c.noise(bandpass(f, 200.0, 7000.0, 1) * tilt(f, -2.0)) * np.where(d < 0, 0.0, np.clip(d / 0.05, 0.0, 1.0) * np.exp(-np.maximum(d, 0.0) / 0.5))
    spray = c.noise(bandpass(f, 3500.0, 11000.0, 1)) * np.where(d < 0.15, 0.0, np.exp(-np.maximum(d - 0.15, 0.0) / 0.9)) * c.flutter(20.0, 70.0, 0.4)
    th = np.zeros(c.n)
    c.put(th, thud(62.0, 0.12, 0.6), 1.18)
    run = c.noise(bandpass(f, 300.0, 2500.0, 1)) * np.clip(c.t / 1.2, 0.0, 1.0) ** 2 * (c.t < 1.2)
    x = at_peak(hit, 0.0) + at_peak(spray, -8.0) + at_peak(th, -3.0) + at_peak(run, -12.0)
    ch = cheering(Loop(c.T, 1470), 2.3, 7, 120, 0.7, False, 1100, (2, 0, 4))
    return (x + 0.5 * ch) * c.edges(0.05, 0.5)


def L4_Launch(c):
    """Rocket Land: a ride goes up. A rumble on the pad, then the roar climbing as it goes and thinning out above.
    A one-shot; its climax is at 2.6 s."""
    f = c.f
    env = np.clip(c.t / 2.4, 0.0, 1.0) ** 2 * np.where(c.t < 2.6, 1.0, np.exp(-(c.t - 2.6) / 1.1))
    u = np.clip(c.t / 4.5, 0.0, 1.0)
    lo = c.noise(bandpass(f, 40.0, 300.0, 2)) * (1.0 - u) ** 1.5
    mid = c.noise(bandpass(f, 300.0, 1800.0, 1)) * np.sin(np.pi * np.clip(u * 1.2, 0.0, 1.0))
    hi = c.noise(bandpass(f, 1800.0, 8000.0, 1)) * u ** 1.5
    return (1.6 * lo + mid + 0.6 * hi) * env * c.flutter(6.0, 20.0, 0.35) * c.edges(0.1, 0.8)


def L4_Laugh(c):
    """The laughing clown over the Funhouse door: a machine's laugh through a tired speaker, the same every time,
    with the ratchet that works its jaw. A one-shot; its climax is at 1.5 s."""
    r = c.rng
    x = np.zeros(c.n)
    for t0, base, n in ((0.25, 175.0, 9), (2.0, 160.0, 7)):
        e = Loop(n * 0.17 + 0.3, 1480 + n)
        syl = [(0.05 + i * 0.17, 0.1, base * (1.5 - 0.11 * i if i < 5 else 0.95 + 0.1 * (i - 5)), VOWELS[0] if i % 3 else VOWELS[1], 1.0 - 0.05 * i) for i in range(n)]
        f0c, amp, F = tracks(e, syl, base)
        c.put(x, sing(e, f0c, amp, F, top=4000.0, roll=0.8), t0)
    x = c.filt(x, bandpass(c.f, 400.0, 3000.0, 2))
    x = np.tanh(2.2 * x / (np.max(np.abs(x)) + 1e-9))        # the speaker gives up at the top of every "ha"
    rt = np.zeros(c.n)
    for k in range(int(3.2 / 0.085)):
        c.put(rt, click(r, 900.0, 3500.0, 0.004, 0.02), 0.2 + k * 0.085)
    return (at_peak(x, 0.0) + at_peak(rt, -16.0)) * c.edges(0.02, 0.3)


# ================================================================================================ LEVEL 5: ICE MOUNTAIN
def L5_Wind(c):
    """Mountain wind: gusts over the ridge with a whistle in them, the pines' needles hissing when it blows,
    a flag's rope tapping its pole."""
    f = c.f
    wind, g = gusts(c, 200.0, 3500.0, 3, 0.15, 1.5, (5.0, 15.0, 0.3), (900.0, 10.0, 0.35))
    out = at_level(wind, 0.0)
    out += at_level(air(c, 60.0, 300.0, -3.0, 0.8, 2, 0.4), -6.0)
    out += at_level(c.noise2(bandpass(f, 2000.0, 6500.0, 1), 1.0) * (g ** 2 * c.flutter(8.0, 30.0, 0.3))[:, None], -9.0)
    tp = scatter(c, lambda q: ping(q, 1100.0, (1.0, 2.3, 3.7), (1.0, 0.4, 0.2), (0.04, 0.025, 0.015), 0.15, 0.8), 0, every=(0.2, 0.55), pans=(0.4, 0.6), clock=g)
    out += at_peak(tp, -18.0)
    return out


def L5_Snow(c):
    """Snow's hush: the muffled quiet of a slope, the pines sighing, a branch letting its load go, a skier's edges
    somewhere below."""
    f = c.f
    r = c.rng
    out = at_level(air(c, 80.0, 900.0, -5.0, 0.9, 2, 0.3), -3.0)
    out += at_level(air(c, 3000.0, 7000.0, -3.0, 1.0, 3, 0.4, 1), -19.0)
    out += at_level(swells(c, 3, 800.0, 3000.0, (1.6, 2.4), (2.0, 2.8), (-0.8, 0.8), 0.3, 1.5), -6.0)

    fw = scatter(c, lambda q: burst(q, 150.0, 1200.0, 0.35, 0.3, 2.0), 2, level=(0.7, 1.0))
    pw = scatter(c, lambda q: burst(q, 2000.0, 6000.0, 0.9, 0.1, 2.5), 2, level=(0.5, 0.8))
    out += at_peak(fw, -10.0) + at_peak(pw, -16.0)
    out += at_peak(swells(c, 2, 1500.0, 5000.0, (0.3, 0.5), (0.4, 0.6), (-0.9, 0.9), 0.8, 2.0), -14.0)
    return out


def L5_Lift(c):
    """Lift machinery: the haul cable's hum, chairs clicking over a tower's sheaves one after another (now this
    tower, now the next), the motor house, a chair rattling."""
    f = c.f
    r = c.rng
    out = at_level(hums(c, [(88.0, 1.0, 5), (176.0, 0.45, 7), (264.0, 0.2, 4)]), -6.0)
    out += at_level(air(c, 100.0, 600.0, -3.0, 0.8, 2, 0.2), -9.0)
    out += at_level(hums(c, [(520.0, 1.0, 3)], (1.0, 0.5)) * c.clock(2, 1.0, 0.5, 1.0)[:, None], -22.0)
    groups = max(1, round(c.T / 2.4))
    sh = np.zeros((c.n, 2))
    for g in range(groups):
        t0 = g * c.T / groups
        gl, gr = pan(-0.6 if g % 2 == 0 else 0.6)
        for k in range(8):
            c.put(sh, click(r, 900.0, 3800.0, 0.006, 0.04) * r.uniform(0.7, 1.0), t0 + k * 0.07, gl, gr)
            c.put(sh, thud(r.uniform(150.0, 200.0), 0.015, 0.06) * 0.6, t0 + k * 0.07, gl, gr)
        for k in range(4):
            c.put(sh, ping(r, r.uniform(1800.0, 2600.0), (1.0, 1.6), (1.0, 0.4), (0.04, 0.02), 0.12, 0.5) * 0.4, t0 + 0.7 + k * r.uniform(0.08, 0.2), gl, gr)
    out += at_peak(sh + 0.3 * c.room(sh, 0.7, 400.0, 5000.0), -6.0)
    return out


def L5_Blizzard(c):
    """The whiteout: wind roaring with nothing to stop it, ice grains hissing past, the air thumping at your ears."""
    f = c.f
    wind, g = gusts(c, 80.0, 2500.0, 4, 0.45, 1.2, (4.0, 14.0, 0.3), (700.0, 6.0, 0.5))
    out = at_level(wind, 0.0)
    out += at_level(c.noise2(bandpass(f, 3500.0, 11000.0, 1), 1.0) * (g * c.flutter(30.0, 90.0, 0.4))[:, None], -8.0)
    out += at_level(c.noise2(bandpass(f, 30.0, 120.0, 2), 0.5) * c.flutter(2.0, 5.0, 0.5)[:, None], -7.0)
    return out


def L5_Glacier(c):
    """The glacier: a crack running through the ice and its echo, the ice groaning under its own weight,
    meltwater trickling somewhere under it, wind hollow over the crevasses."""
    f = c.f
    r = c.rng
    out = at_level(c.noise2(resonance(f, 240.0, 14.0), 0.7) * (c.clock(3, 0.8, 0.1, 1.0) ** 1.5)[:, None], -5.0)
    out += at_level(air(c, 60.0, 400.0, -4.0, 0.9, 2, 0.4), -6.0)
    ck = np.zeros(c.n)
    for i in range(3):
        t0 = (i + r.uniform(-0.25, 0.25)) * c.T / 3
        c.put(ck, click(r, 500.0, 5000.0, 0.012, 0.08), t0)
        c.put(ck, thud(r.uniform(48.0, 62.0), 0.3, 1.0, 0.6) * 0.9, t0 + 0.01)
    out += at_peak(far(c, ck, 2.6, 40.0, 3500.0, 0.6, 0.0, 0.12), -4.0)
    gr = scatter(c, lambda q: creak(q, q.uniform(1.0, 1.8), 120.0, 300.0, (16.0, 38.0), 0.3), 3)
    out += at_peak(gr + 0.6 * c.room(gr, 2.0, 80.0, 1500.0), -9.0)
    out += at_level(scatter(c, lambda q: bloop(q, q.uniform(600.0, 2000.0), q.uniform(0.02, 0.05), 1.5), 0, every=(0.03, 0.14), pans=(-0.3, 0.9), level=(0.3, 1.0)), -15.0)
    return out


def L5_Gondola(c):
    """A gondola crossing over the road: its hanger rattling over a tower's sheaves (two banks of rollers), the
    cable singing, the cabin creaking. A one-shot; its climax is at 2.2 s."""
    f = c.f
    r = c.rng
    env = np.exp(-0.5 * ((c.t - 2.2) / 1.1) ** 2)
    hum = sum(np.sin(2 * np.pi * 88.0 * h * c.t + r.uniform(0, 6.28)) / h for h in range(1, 7))
    x = at_level(hum * env, 0.0) + at_level(c.noise(bandpass(f, 150.0, 1500.0, 1)) * env, -6.0)
    rl = np.zeros(c.n)
    for bank in (1.7, 2.45):
        for k in range(8):
            c.put(rl, click(r, 900.0, 4200.0, 0.007, 0.05) * r.uniform(0.7, 1.0), bank + k * 0.065)
            c.put(rl, thud(r.uniform(140.0, 210.0), 0.02, 0.08), bank + k * 0.065)
            c.put(rl, ping(r, r.uniform(1500.0, 2400.0), (1.0, 1.7), (1.0, 0.4), (0.05, 0.03), 0.15, 0.4) * 0.5, bank + k * 0.065 + 0.02)
    cr = np.zeros(c.n)
    c.put(cr, creak(r, 0.7, 300.0, 600.0), 2.9)
    return (x + at_peak(rl, 4.0) + at_peak(cr, -8.0)) * c.edges(0.05, 0.5)


def L5_Cowbells(c):
    """The half pipe's stands: cowbells swung for all they are worth, and a cheer. A one-shot; its climax is at 1.6 s."""
    r = c.rng
    env = np.exp(-0.5 * ((c.t - 1.7) / 1.1) ** 2)
    cb = np.zeros(c.n)
    for b in range(4):
        f1 = r.uniform(520.0, 900.0)
        t0 = r.uniform(0.2, 0.5)
        while t0 < c.T - 0.5:
            c.put(cb, ping(r, f1, (1.0, 1.5, 2.1, 2.8, 4.2), (1.0, 0.8, 0.5, 0.35, 0.2), (0.12, 0.09, 0.07, 0.05, 0.03), 0.4, 0.7) * r.uniform(0.5, 1.0), t0)
            t0 += r.uniform(0.11, 0.2)
    ch = cheering(Loop(c.T, 1590), 1.6, 7, 0, 1.0, True, 1200)
    return (at_peak(cb * env, 0.0) + 0.7 * at_peak(ch, 0.0)) * c.edges(0.05, 0.5)


def L5_Avalanche(c):
    """Something lets go high in the chute: a crack, then a slide's long low roar coming down and spreading out.
    A one-shot; its climax is at 2.5 s."""
    f = c.f
    r = c.rng
    x = np.zeros(c.n)
    c.put(x, click(r, 400.0, 3500.0, 0.02, 0.12), 0.3)
    d = c.t - 0.5
    roar = c.noise(bandpass(f, 30.0, 320.0, 2)) * np.where(d < 0, 0.0, np.clip(d / 2.0, 0.0, 1.0) ** 1.5 * np.exp(-np.maximum(d - 2.0, 0.0) / 1.6)) * c.flutter(3.0, 10.0, 0.4)
    hiss = c.noise(bandpass(f, 400.0, 3200.0, 1) * tilt(f, -3.0)) * np.where(d < 0.6, 0.0, np.clip((d - 0.6) / 2.0, 0.0, 1.0) * np.exp(-np.maximum(d - 2.6, 0.0) / 1.4))
    x = at_peak(x, -8.0) + at_peak(roar, 0.0) + at_peak(hiss, -9.0)
    return c.filt(x + 0.5 * c.tail(x, 2.4, 30.0, 2000.0, 0.2), lowpass(f, 3500.0, 1)) * c.edges(0.02, 0.8)


# ================================================================================================ LEVEL 6: UNDER THE SEA
# Everything is heard through water: dull, slow, a long way off. Each bed ends in a low-pass.

def L6_Deep(c):
    """The deep itself: pressure's hum on notes that crawl against each other, slow surges of the whole sea,
    water moving past, the far thud of something large."""
    f = c.f
    r = c.rng
    out = at_level(hums(c, [(38.0, 1.0, 3), (57.0, 0.8, 4), (76.0, 0.5, 5), (114.0, 0.2, 3)]), -6.0)
    out += at_level(air(c, 25.0, 200.0, -5.0, 0.7, 2, 0.6), -3.0)
    out += at_level(swells(c, 3, 150.0, 900.0, (1.8, 2.6), (2.0, 3.0), (-0.8, 0.8), 0.5, 1.5, 0.3, -4.0), -8.0)
    th = scatter(c, lambda q: thud(q.uniform(36.0, 46.0), 0.4, 1.4, 0.6), 2, mono=True)
    out += at_peak(far(c, th, 3.0, 25.0, 400.0, 0.5, 0.0, 0.15), -8.0)
    return c.filt(out, lowpass(f, 2500.0, 2))


def L6_Bubbles(c):
    """The reef: streams of bubbles climbing (each stream's pitch rising as it goes), the crackle of shrimp
    everywhere, fish flicking past."""
    f = c.f
    r = c.rng
    out = at_level(air(c, 60.0, 500.0, -4.0, 0.8, 2, 0.4), -8.0)
    bb = np.zeros((c.n, 2))
    for i in range(5):
        t0 = (i + r.uniform(-0.3, 0.3)) * c.T / 5
        dur = r.uniform(1.0, 2.5)
        gl, gr = pan(r.uniform(-0.9, 0.9))
        base = r.uniform(400.0, 900.0)
        t = 0.0
        while t < dur:
            c.put(bb, bloop(r, base * (1.0 + 1.2 * t / dur) * r.uniform(0.85, 1.15), r.uniform(0.015, 0.05), 1.5) * r.uniform(0.4, 1.0), t0 + t, gl, gr)
            t += r.uniform(0.04, 0.085)
    out += at_level(bb, -3.0)
    sw = c.clock(3, 0.8, 0.5, 1.0)
    out += at_level(np.stack([crackle(c, 90.0, 2500.0, 7000.0, sw), crackle(c, 90.0, 2500.0, 7000.0, sw)], axis=1), -10.0)
    out += at_peak(scatter(c, lambda q: burst(q, 300.0, 1500.0, 0.09, 0.3, 1.5), 5, level=(0.5, 1.0)), -14.0)
    return c.filt(out, lowpass(f, 6500.0, 2))


def L6_Kelp(c):
    """The kelp forest: the swell pulling everything one way and then the other, the stalks rubbing, the
    current's wash through the fronds."""
    f = c.f
    r = c.rng
    cyc = max(1, round(c.T / 5.5))
    sway = np.sin(2 * np.pi * cyc * c.t / c.T)
    w = c.noise(bandpass(f, 100.0, 1200.0, 1) * tilt(f, -4.0, 350.0))
    out = at_level(np.stack([w * np.clip(sway, 0.0, 1.0) ** 1.5, c.noise(bandpass(f, 100.0, 1200.0, 1) * tilt(f, -4.0, 350.0)) * np.clip(-sway, 0.0, 1.0) ** 1.5], axis=1), 0.0)
    out += at_level(air(c, 200.0, 2000.0, -4.0, 1.0, 3, 0.4, 1), -7.0)
    out += at_level(air(c, 40.0, 200.0, -4.0, 0.7, 2, 0.4), -6.0)
    cr = scatter(c, lambda q: creak(q, q.uniform(0.5, 1.1), 300.0, 700.0, (25.0, 55.0), 0.3), 5)
    out += at_peak(cr + 0.6 * c.room(cr, 1.6, 150.0, 2000.0), -10.0)
    out += at_peak(scatter(c, lambda q: bloop(q, q.uniform(400.0, 1100.0), q.uniform(0.02, 0.05), 1.5), 9, level=(0.4, 1.0)), -16.0)
    return c.filt(out, lowpass(f, 3000.0, 2))


def L6_Station(c):
    """The research station: its pumps heard through the hull, the hum of its power, an airlock venting, a sonar
    far off with the sea's answer, a voice on the radio."""
    f = c.f
    r = c.rng
    out = at_level(hums(c, [(50.0, 1.0, 4), (100.0, 0.8, 6), (150.0, 0.35, 5), (200.0, 0.2, 3)]), -6.0)
    out += at_level(air(c, 60.0, 500.0, -4.0, 0.8, 2, 0.3), -8.0)
    a = np.zeros(c.n)
    n1 = max(1, round(2.6 * c.T))
    for k in range(n1):
        c.put(a, thud(60.0, 0.09, 0.35), k * c.T / n1)
    out += at_level(stereo(a, 0.4), -6.0)
    out += at_peak(scatter(c, lambda q: burst(q, 800.0, 3500.0, q.uniform(0.9, 1.4), 0.08, 2.0), 2, level=(0.7, 1.0)), -10.0)
    pg = np.zeros(c.n)
    for t0 in (0.2 * c.T, 0.66 * c.T):
        m = int(0.14 * SR)
        u = np.arange(m) / m
        c.put(pg, np.sin(2 * np.pi * 780.0 * u * 0.14) * np.sin(np.pi * u) ** 0.5, t0)
    out += at_peak(far(c, pg, 3.5, 400.0, 1500.0, 0.3, -0.5, 0.3), -17.0)
    rd = np.zeros(c.n)
    e = Loop(1.6, 81)
    syl = [(0.1 + i * 0.19, 0.14, 125.0 * e.rng.uniform(0.92, 1.1), VOWELS[e.rng.integers(len(VOWELS))], e.rng.uniform(0.7, 1.1)) for i in range(7)]
    f0c, amp, F = tracks(e, syl, 125.0)
    c.put(rd, np.tanh(4.0 * e.filt(sing(e, f0c, amp, F, top=2600.0, roll=0.8), bandpass(e.f, 400.0, 1800.0, 3)) / 0.3), 0.43 * c.T)
    out += at_peak(far(c, rd, 1.5, 300.0, 2000.0, 0.4, 0.5), -16.0)
    return c.filt(out, lowpass(f, 3500.0, 2))


def L6_Hull(c):
    """The salvage field: wrecked hulls groaning as the current leans on them, plates knocking, a chain dragging
    link by link, something big ringing dully when it is struck."""
    f = c.f
    r = c.rng
    out = at_level(air(c, 80.0, 600.0, -4.0, 0.9, 3, 0.4), -5.0)
    gn = scatter(c, lambda q: creak(q, q.uniform(1.5, 2.5), 90.0, 260.0, (12.0, 30.0), 0.4), 4)
    out += at_peak(gn + 0.7 * c.room(gn, 2.8, 60.0, 1500.0), 0.0)
    ch = np.zeros(c.n)
    for t0 in (0.15 * c.T, 0.62 * c.T):
        for k in range(int(r.integers(6, 10))):
            c.put(ch, ping(r, r.uniform(600.0, 1400.0), (1.0, 1.6, 2.4), (1.0, 0.5, 0.3), (0.06, 0.04, 0.025), 0.2, 0.3) * r.uniform(0.4, 1.0), t0 + k * r.uniform(0.12, 0.3))
    out += at_peak(far(c, ch, 1.8, 300.0, 2500.0, 0.4, -0.4), -11.0)
    kn = np.zeros(c.n)
    for i in range(4):
        t0 = (i + r.uniform(-0.3, 0.3)) * c.T / 4
        c.put(kn, thud(r.uniform(90.0, 160.0), 0.08, 0.4), t0)
        c.put(kn, bell(r, 110.0, 1.6, 0.3) * 0.5, t0 + 0.01)
    out += at_peak(far(c, kn, 2.4, 50.0, 1800.0, 0.5, 0.3, 0.1), -6.0)
    return c.filt(out, lowpass(f, 3000.0, 2))


def L6_Swirl(c):
    """The fish tornado: thousands of fish turning as one. The rush of them coming round and round, and the tick
    of their bodies. A one-shot; its climax is at 1.8 s."""
    f = c.f
    env = np.exp(-0.5 * ((c.t - 1.9) / 1.0) ** 2)
    x = at_level(c.noise(bandpass(f, 200.0, 2500.0, 1) * tilt(f, -3.0)) * (0.6 + 0.4 * np.sin(2 * np.pi * 3.0 * c.t)), 0.0)
    x += at_level(crackle(c, 160.0, 1500.0, 5000.0), -8.0)
    x += at_level(c.noise(bandpass(f, 60.0, 250.0, 2)), -6.0)
    return c.filt(x * env, lowpass(f, 4500.0, 2)) * c.edges(0.1, 0.6)


def L6_Whale(c):
    """A whale: two long calls that climb and sink, a higher cry after them, and the whole sea giving them back.
    A one-shot; its climax is at 2.0 s."""
    r = c.rng
    x = np.zeros(c.n)
    for t0, dur, a, b, e_ in ((0.5, 1.9, 170.0, 420.0, 250.0), (2.3, 1.5, 220.0, 380.0, 190.0)):
        m = int(dur * SR)
        u = np.arange(m) / m
        fr = a + (b - a) * np.sin(np.pi * np.clip(u * 1.4, 0.0, 1.0)) ** 2 * (u < 0.5 / 1.4 + 0.5) + (e_ - a) * np.clip((u - 0.6) / 0.4, 0.0, 1.0) ** 2
        ph = 2 * np.pi * np.cumsum(fr * (1.0 + 0.012 * np.sin(2 * np.pi * 5.0 * u * dur))) / SR
        c.put(x, (np.sin(ph) + 0.5 * np.sin(2 * ph) + 0.3 * np.sin(3 * ph) + 0.12 * np.sin(5 * ph)) * np.sin(np.pi * u) ** 0.6, t0)
    m = int(0.7 * SR)
    u = np.arange(m) / m
    ph = 2 * np.pi * np.cumsum(600.0 + 800.0 * u ** 1.5) / SR
    c.put(x, 0.35 * (np.sin(ph) + 0.3 * np.sin(2 * ph)) * np.sin(np.pi * u) ** 0.7, 3.9)
    x = c.filt(x, bandpass(c.f, 100.0, 3000.0, 1))
    return (0.5 * x + c.tail(x, 3.6, 100.0, 2200.0, 0.2, 8)) * c.edges(0.05, 0.8)


def L6_Sonar(c):
    """The station's sonar: one soft ping (lower and rounder than any of the game's own beeps) and what the sea
    sends back of it, twice, fainter. A one-shot; the ping is at 0.3 s."""
    m = int(0.16 * SR)
    u = np.arange(m) / m
    p = (np.sin(2 * np.pi * 780.0 * u * 0.16) + 0.15 * np.sin(2 * np.pi * 1560.0 * u * 0.16)) * np.sin(np.pi * u) ** 0.5
    x = np.zeros(c.n)
    c.put(x, p, 0.25)
    c.put(x, p * 0.25, 1.2)
    c.put(x, p * 0.1, 2.0)
    return (0.6 * x + c.tail(x, 3.2, 400.0, 1600.0, 0.25)) * c.edges(0.01, 0.6)


def L6_Salvage(c):
    """The salvage ship hanging over the field: her hull working. One long groan sliding down, a plate ringing,
    her chain taking the strain. A one-shot; its climax is at 2.0 s."""
    r = c.rng
    x = np.zeros(c.n)
    c.put(x, creak(r, 2.8, 80.0, 180.0, (14.0, 22.0), -0.4), 0.6)
    c.put(x, creak(r, 1.2, 160.0, 320.0, (20.0, 34.0), 0.3) * 0.5, 2.9)
    bg = np.zeros(c.n)
    c.put(bg, bell(r, 98.0, 2.4, 0.3), 2.0)
    ch = np.zeros(c.n)
    for k in range(9):
        c.put(ch, ping(r, r.uniform(600.0, 1300.0), (1.0, 1.6, 2.4), (1.0, 0.5, 0.3), (0.06, 0.04, 0.025), 0.2, 0.3) * r.uniform(0.4, 1.0), 3.3 + k * r.uniform(0.1, 0.22))
    x = at_peak(x, 0.0) + at_peak(bg, -5.0) + at_peak(ch, -10.0)
    return c.filt(x + 0.7 * c.tail(x, 2.8, 60.0, 1800.0, 0.1), lowpass(c.f, 3000.0, 2)) * c.edges(0.05, 0.6)


# ================================================================================================ the table of sounds
# name: (kind, seconds, seed, function, where its climax is (s)). A level's beds have lengths with no common measure.
# The climax of a pass is the "climax" its Pass line in FTAmbienceL<n>.inl names: change one, change the other.
SOUNDS = {
    'A_L0_Surf': ('bed', 12.5, 1001, L0_Surf, 0.0),
    'A_L0_Boardwalk': ('bed', 11.3, 1002, L0_Boardwalk, 0.0),
    'A_L0_Marina': ('bed', 10.1, 1003, L0_Marina, 0.0),
    'A_L0_Fair': ('bed', 11.9, 1004, L0_Fair, 0.0),
    'A_L0_Night': ('bed', 9.7, 1005, L0_Night, 0.0),
    'A_L0_Organ': ('spot', 8.5714, 1011, L0_Organ, 0.0),
    'A_L0_Swing': ('pass', 3.8, 1012, L0_Swing, 1.7),
    'A_L0_ShipHorn': ('pass', 6.2, 1013, L0_ShipHorn, 1.5),
    'A_L0_Cheer': ('pass', 4.8, 1014, L0_Cheer, 1.6),
    'A_L1_Plant': ('bed', 11.7, 1101, L1_Plant, 0.0),
    'A_L1_Steam': ('bed', 10.3, 1102, L1_Steam, 0.0),
    'A_L1_Pumps': ('bed', 9.6, 1103, L1_Pumps, 0.0),
    'A_L1_Reactor': ('bed', 12.1, 1104, L1_Reactor, 0.0),
    'A_L1_Yard': ('bed', 10.9, 1105, L1_Yard, 0.0),
    'A_L1_Hooter': ('pass', 5.0, 1111, L1_Hooter, 1.2),
    'A_L1_Vent': ('pass', 2.6, 1112, L1_Vent, 0.9),
    'A_L1_Core': ('spot', 6.4, 1113, L1_Core, 0.0),
    'A_L1_Tanker': ('pass', 7.0, 1114, L1_Tanker, 3.0),
    'A_L2_Wind': ('bed', 12.3, 1201, L2_Wind, 0.0),
    'A_L2_Cicadas': ('bed', 9.9, 1202, L2_Cicadas, 0.0),
    'A_L2_Highway': ('bed', 11.1, 1203, L2_Highway, 0.0),
    'A_L2_Neon': ('bed', 10.7, 1204, L2_Neon, 0.0),
    'A_L2_Canyon': ('bed', 12.5, 1205, L2_Canyon, 0.0),
    'A_L2_Truck': ('pass', 5.0, 1211, L2_Truck, 2.2),
    'A_L2_Hawk': ('pass', 4.0, 1212, L2_Hawk, 0.8),
    'A_L2_GasBell': ('pass', 3.2, 1213, L2_GasBell, 0.7),
    'A_L2_Jackpot': ('pass', 4.6, 1214, L2_Jackpot, 1.4),
    'A_L3_Rumble': ('bed', 12.5, 1301, L3_Rumble, 0.0),
    'A_L3_Lava': ('bed', 10.9, 1302, L3_Lava, 0.0),
    'A_L3_Ash': ('bed', 9.5, 1303, L3_Ash, 0.0),
    'A_L3_Vents': ('bed', 11.3, 1304, L3_Vents, 0.0),
    'A_L3_Works': ('bed', 10.1, 1305, L3_Works, 0.0),
    'A_L3_FireCurtain': ('spot', 7.3, 1311, L3_FireCurtain, 0.0),
    'A_L3_Eruption': ('pass', 7.5, 1312, L3_Eruption, 2.0),
    'A_L3_Geyser': ('pass', 3.0, 1313, L3_Geyser, 0.8),
    'A_L3_SlagTrain': ('pass', 7.0, 1314, L3_SlagTrain, 3.0),
    'A_L4_Crowd': ('bed', 11.5, 1401, L4_Crowd, 0.0),
    'A_L4_Rides': ('bed', 12.3, 1402, L4_Rides, 0.0),
    'A_L4_Lagoon': ('bed', 10.7, 1403, L4_Lagoon, 0.0),
    'A_L4_Rocket': ('bed', 9.9, 1404, L4_Rocket, 0.0),
    'A_L4_Funhouse': ('bed', 11.1, 1405, L4_Funhouse, 0.0),
    'A_L4_Coaster': ('pass', 5.2, 1411, L4_Coaster, 2.4),
    'A_L4_ShipBell': ('pass', 4.0, 1412, L4_ShipBell, 0.6),
    'A_L4_Splash': ('pass', 4.2, 1413, L4_Splash, 1.3),
    'A_L4_Launch': ('pass', 5.6, 1414, L4_Launch, 2.6),
    'A_L4_Laugh': ('pass', 3.6, 1415, L4_Laugh, 1.5),
    'A_L5_Wind': ('bed', 12.1, 1501, L5_Wind, 0.0),
    'A_L5_Snow': ('bed', 11.7, 1502, L5_Snow, 0.0),
    'A_L5_Lift': ('bed', 9.6, 1503, L5_Lift, 0.0),
    'A_L5_Blizzard': ('bed', 10.5, 1504, L5_Blizzard, 0.0),
    'A_L5_Glacier': ('bed', 12.5, 1505, L5_Glacier, 0.0),
    'A_L5_Gondola': ('pass', 5.0, 1511, L5_Gondola, 2.2),
    'A_L5_Cowbells': ('pass', 4.6, 1512, L5_Cowbells, 1.6),
    'A_L5_Avalanche': ('pass', 7.0, 1513, L5_Avalanche, 2.5),
    'A_L6_Deep': ('bed', 12.7, 1601, L6_Deep, 0.0),
    'A_L6_Bubbles': ('bed', 10.3, 1602, L6_Bubbles, 0.0),
    'A_L6_Kelp': ('bed', 11.9, 1603, L6_Kelp, 0.0),
    'A_L6_Station': ('bed', 11.1, 1604, L6_Station, 0.0),
    'A_L6_Hull': ('bed', 12.5, 1605, L6_Hull, 0.0),
    'A_L6_Swirl': ('pass', 4.6, 1611, L6_Swirl, 1.8),
    'A_L6_Whale': ('pass', 6.6, 1612, L6_Whale, 2.0),
    'A_L6_Sonar': ('pass', 4.6, 1613, L6_Sonar, 0.3),
    'A_L6_Salvage': ('pass', 5.6, 1614, L6_Salvage, 2.0),
    'A_L7_Traffic': ('bed', 12.3, 1701, L7_Traffic, 0.0),
    'A_L7_Crowd': ('bed', 10.9, 1702, L7_Crowd, 0.0),
    'A_L7_Works': ('bed', 11.5, 1703, L7_Works, 0.0),
    'A_L7_Plaza': ('bed', 10.3, 1704, L7_Plaza, 0.0),
    'A_L7_Syndicate': ('bed', 12.5, 1705, L7_Syndicate, 0.0),
    'A_L7_Siren': ('pass', 8.0, 1711, L7_Siren, 3.5),
    'A_L7_ElTrain': ('pass', 7.6, 1712, L7_ElTrain, 3.2),
    'A_L7_Premiere': ('pass', 5.0, 1713, L7_Premiere, 1.8),
    'A_L7_Bell': ('pass', 7.0, 1714, L7_Bell, 0.4),
    'A_L7_Boulevard': ('pass', 5.0, 1715, L7_Boulevard, 2.2),
    'A_L7_Airship': ('pass', 9.0, 1716, L7_Airship, 4.5),
}


def finish(x, kind):
    """The level every sound of its kind is written at, peaks rounded off, the lowest bits cleared."""
    if kind == 'bed':
        # a bed is a level, not a string of events: whatever stands more than five times over its RMS is rounded
        # off (nothing over eight times: -5 dBFS), so no tick or knock of a bed can jump out of the mix
        x = x * (db(BED_RMS) / rms(x))
        knee, top = 5.0 * db(BED_RMS), 8.0 * db(BED_RMS)
        a = np.abs(x)
        x = np.where(a > knee, np.sign(x) * (knee + (top - knee) * np.tanh((a - knee) / (top - knee))), x)
        x = x * (db(BED_RMS) / rms(x))
    else:
        k = int(0.5 * SR)
        e = np.cumsum(np.concatenate(([0.0], x * x)))
        loud = math.sqrt(float(np.max(e[k:] - e[:-k])) / k) if len(x) > k else rms(x)
        x = x * (db(PASS_LOUD) / loud)
    a = np.abs(x)
    x = np.where(a > 0.6, np.sign(x) * (0.6 + 0.3 * np.tanh((a - 0.6) / 0.3)), x)     # nothing over 0.9
    step = 1 << (16 - KEEP_BITS)
    q = np.round(x * 32767.0 / step) * step
    return np.clip(q, -32768, 32767).astype('<i2')


def main():
    out_dir = sys.argv[1]
    levels = sys.argv[2:]
    os.makedirs(out_dir, exist_ok=True)
    lines = []
    failed = []
    for name, (kind, seconds, seed, fn, climax) in SOUNDS.items():
        if levels and name.split('_')[1] not in levels:
            continue
        c = Loop(seconds, seed)
        try:
            q = finish(fn(c), kind)
        except Exception:
            import traceback
            print('FAILED %s\n%s' % (name, traceback.format_exc()))
            failed.append(name)
            continue
        with wave.open(os.path.join(out_dir, name + '.wav'), 'wb') as w:
            w.setnchannels(1 if q.ndim == 1 else 2)
            w.setsampwidth(2)
            w.setframerate(SR)
            w.writeframes(q.tobytes())
        x = q.astype(np.float64) / 32768.0
        line = '%-18s %-4s ch=%d sec=%.4f rms=%.1f peak=%.1f climax=%.2f pcm=%d packed=%d' % (
            name, kind, x.ndim, c.T, 20 * math.log10(rms(x)), 20 * math.log10(float(np.max(np.abs(x))) + 1e-12), climax, q.nbytes, len(zlib.compress(q.tobytes(), 6)))
        print(line)
        lines.append(line)
    # the list keeps the lines of levels that were not made this time
    path = os.path.join(out_dir, 'ambience.txt')
    old = {}
    if os.path.exists(path):
        with open(path) as fh:
            old = dict((ln.split()[0], ln.rstrip('\n')) for ln in fh if ln.strip())
    for ln in lines:
        old[ln.split()[0]] = ln
    with open(path, 'w') as fh:
        fh.write('\n'.join(old[k] for k in sorted(old)) + '\n')
    if failed:
        print('NOT MADE: ' + ', '.join(failed))
        sys.exit(1)


if __name__ == '__main__':
    main()
