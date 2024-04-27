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

signal_1hz = Signal(amplitude=3, frequency=1, sampling_rate=200, duration=2)
sine_1hz = signal_1hz.sine()

signal_10hz = Signal(amplitude=0.5, frequency=10, sampling_rate=200, duration=2)
sine_10hz = signal_10hz.sine()

signal_20hz = Signal(amplitude=1, frequency=20, sampling_rate=200, duration=2)
sine_20hz = signal_20hz.sine()

signal_39hz = Signal(amplitude=0.25, frequency=39, sampling_rate=200, duration=2)
sine_39hz = signal_39hz.sine()

# signal = sine_1hz + sine_10hz + sine_20hz + sine_39hz

signal = np.array(np.random.random(2000))
sampling_rate = 200.0

fig, axs = plt.subplots(2)
fig.suptitle("Signal and FFT")

timearray = [x/sampling_rate for x in range(0, len(signal))]

axs[0].plot(timearray, signal, "b")
axs[0].set(xlabel="Time [sec]", ylabel="Amplitude")
axs[0].set_title("Sum of three signals")

N = len(signal)
fourier = np.fft.rfft(signal)

# print(fourier)
frequency_axis = np.fft.rfftfreq(N, d=1.0/sampling_rate)
norm_amplitude = 2*np.abs(fourier)/N

axs[1].plot(frequency_axis, norm_amplitude)
axs[1].set(xlabel="Frequency[Hz]", ylabel="Amplitude")
axs[1].set_title("Spectrum")
plt.show()