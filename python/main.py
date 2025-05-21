import numpy as np
import matplotlib.pyplot as plt

class Signal:

    def __init__(self, amplitude=1, frequency=10, duration=1, sampling_rate=100.0, phase=0):
        self.amplitude = amplitude
        self.frequency = frequency
        self.duration = duration
        self.sampling_rate = sampling_rate
        self.phase = phase
        self.time_step = 1.0/self.sampling_rate
        self.time_axis = np.arange(0, self.duration, self.time_step)

    # Generate sine wave
    def sine(self):
        return self.amplitude*np.sin(2*np.pi*self.frequency*self.time_axis+self.phase)

    # Generate cosine wave
    def cosine(self):
        return self.amplitude*np.cos(2*np.pi*self.frequency*self.time_axis+self.phase)

# signal_1hz = Signal(amplitude=3, frequency=1, sampling_rate=200, duration=2)
# sine_1hz = signal_1hz.sine()

# signal_10hz = Signal(amplitude=0.5, frequency=10, sampling_rate=200, duration=2)
# sine_10hz = signal_10hz.sine()

# signal_20hz = Signal(amplitude=1, frequency=20, sampling_rate=200, duration=2)
# sine_20hz = signal_20hz.sine()

# signal_39hz = Signal(amplitude=0.25, frequency=39, sampling_rate=200, duration=2)
# sine_39hz = signal_39hz.sine()

# signal = sine_1hz + sine_10hz + sine_20hz + sine_39hz

sampling_rate = 256.0
duration = 2

sig1 = Signal(amplitude=4, frequency=3, phase=20*np.pi/180, sampling_rate=int(sampling_rate), duration=duration)
sig2 = Signal(amplitude=1, frequency=10, phase=37*np.pi/180, sampling_rate=int(sampling_rate), duration=duration)
# sig3 = Signal(amplitude=3, frequency=23, phase=13*np.pi/180, sampling_rate=int(sampling_rate), duration=duration)
# sig4 = Signal(amplitude=6, frequency=18, phase=110*np.pi/180, sampling_rate=int(sampling_rate), duration=duration)
sig5 = Signal(amplitude=2, frequency=120, phase=82*np.pi/180, sampling_rate=int(sampling_rate), duration=duration)

# signal = sig1.sine() + sig2.sine() + sig3.sine() + sig4.sine() + sig5.sine()
signal = sig1.sine() + sig2.sine() + sig5.sine()

with open("signal.txt", "w") as f:
    for val in signal:
        imag_str = "+" if (np.imag(val) >= 0) else ""
        imag_str += str(np.imag(val))
        f.write(str(np.real(val)) + imag_str + "j\n")

fig, axs = plt.subplots(nrows=2, ncols=2)
fig.suptitle("Signal and FFT")

timearray = [x/sampling_rate for x in range(0, len(signal))]

axs[0,0].plot(timearray, signal, "b")
axs[0,0].set(xlabel="Time [sec]", ylabel="Amplitude")
axs[0,0].set_title("Input signal")

N = len(signal)
fourier = np.fft.rfft(signal)

threshold = max(np.abs(fourier))/10000
thresholded_fourier = [(x if abs(x) > threshold else 0) for x in fourier]

# print(fourier)
frequency_axis = np.fft.rfftfreq(N, d=1.0/sampling_rate)
norm_amplitude = 2*np.abs(thresholded_fourier)/N

phase = [(x+90 if x != 0 else 0) for x in np.angle(thresholded_fourier, deg=True)]

axs[1,0].plot(frequency_axis, norm_amplitude)
axs[1,0].set(xlabel="Frequency[Hz]", ylabel="Amplitude")
axs[1,0].set_title("Spectrum")

axs[0,1].plot(frequency_axis, phase)
axs[0,1].set(xlabel="Frequency[Hz]", ylabel="Phase shift of waves (degrees)")
axs[0,1].set_title("Phase")

resignal = np.array([0.0 for _ in range(int(sampling_rate*duration))])
for i in range(len(thresholded_fourier)):
    if thresholded_fourier[i] == 0:
        continue

    amp = 2*abs(thresholded_fourier[i])/N
    phase = np.angle(thresholded_fourier[i], deg=True) + 90
    freq = frequency_axis[i]
    print(f"Found signal with amplitude {amp}, phase {phase} and frequency {freq}")
    if freq > 100:
        continue
    else:
        sig = Signal(amplitude=amp, frequency=freq, phase=(((phase-90)*np.pi/180)+np.pi/2), sampling_rate=sampling_rate, duration=duration)
        resignal += sig.sine()


axs[1,1].plot(timearray, resignal, "b")
axs[1,1].set(xlabel="Time [sec]", ylabel="Amplitude")
axs[1,1].set_title("Reverse engineered signal")

plt.show()