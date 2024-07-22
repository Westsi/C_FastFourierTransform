#ifndef FFT_MIC_H_
#define FFT_MIC_H_

#include <CoreAudio/CoreAudioTypes.h>
#include <CoreAudio/AudioHardware.h>
#include <AudioToolbox/AUComponent.h>

// AudioObjectID* defaultMicrophone(AudioObjectID* propData);
void record();

#define kNumberOfRecordingBuffers 3
#define kBufferDurationInSeconds 0.5

#endif