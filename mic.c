#include "mic.h"

#include <CoreAudio/CoreAudioTypes.h>
#include <CoreAudio/AudioHardware.h>
#include <AudioToolbox/AUComponent.h>
#include <AudioToolbox/AudioToolbox.h>
#include <AudioToolbox/AudioFormat.h>

struct MyRecorder {
    AudioFileID recordFile;
    SInt64      recordPacket;
    Boolean     running;
};

typedef struct MyRecorder MyRecorder;

static void MyAQInputCallback(void *inUserData, AudioQueueRef inAQ, AudioQueueBufferRef inBuffer, const AudioTimeStamp *inStartTime, UInt32 inNumberPacketDescriptions, const AudioStreamPacketDescription *inPacketDescs) {
  MyRecorder *recorder = (MyRecorder *)inUserData;
    if (inNumberPacketDescriptions > 0) {
    AudioFileWritePackets(recorder->recordFile, FALSE, inBuffer->mAudioDataByteSize, inPacketDescs, recorder->recordPacket, &inNumberPacketDescriptions, inBuffer->mAudioData);
    recorder->recordPacket += inNumberPacketDescriptions;
    }
    if (recorder->running) {
        AudioQueueEnqueueBuffer(inAQ, inBuffer, 0, NULL);
    }
}

// AudioObjectID* defaultMicrophone(AudioObjectID* propData) {
//     int target = kAudioObjectSystemObject;
//     int selector = kAudioHardwarePropertyDefaultInputDevice;
//     int scope = kAudioObjectPropertyScopeGlobal;
//     int element = kAudioObjectPropertyElementMain;
//     AudioObjectPropertyAddress aopa = { .mSelector = selector,
//                                         .mScope = scope, 
//                                         .mElement = element};
//     AudioObjectPropertyAddress* prop = &aopa;
//     Boolean hasProp = AudioObjectHasProperty(target, prop);
//     if (hasProp != 1) {
//         printf("Core Audio does not have the requested property.\n");
//     }
//     UInt32* size = malloc(sizeof(UInt32));
//     SInt32 err = AudioObjectGetPropertyDataSize(target, prop, 0, NULL, size);
//     if (err != 0) {
//         printf("Can't get Core Audio property size.\n");
//     }

//     int numValues = size[0]/sizeof(AudioObjectID);

//     if (numValues > 128) {
//         printf("not enough allocated\n");
//     }
//     err = AudioObjectGetPropertyData(target, prop, 0, NULL, size, propData);
//     if (err != 0) {
//         printf("Can't get Core Audio property data.\n");
//     }
//     return propData;
// }

void e(SInt32 err) {
    if (err != noErr) {
        printf("Error code: %d\n", err);
        exit(1);
    }
}

void GetDefaultInputDeviceSampleRate(Float64 *oSampleRate) {
    AudioObjectPropertyAddress propertyAddress;
    
    propertyAddress.mSelector = kAudioHardwarePropertyDefaultInputDevice;
    propertyAddress.mScope = kAudioObjectPropertyScopeGlobal;
    propertyAddress.mElement = 0; // master element
    
    AudioDeviceID deviceID = 0;
    UInt32 propertySize = sizeof(AudioDeviceID);
    
    SInt32 err = AudioObjectGetPropertyData(kAudioObjectSystemObject,
        &propertyAddress,
        0,
        NULL,
        &propertySize,
        &deviceID);
    e(err);

    propertyAddress.mSelector = kAudioDevicePropertyNominalSampleRate;
    propertyAddress.mScope = kAudioObjectPropertyScopeGlobal;
    propertyAddress.mElement = 0;
    propertySize = sizeof(Float64);
    
    err = AudioObjectGetPropertyData(deviceID,
        &propertyAddress,
        0,
        NULL,
        &propertySize,
        oSampleRate);
    e(err);
}

void CopyEncoderCookieToFile(AudioQueueRef queue, AudioFileID fileHandle) {
    UInt32 propertyValueSize = 0;
    SInt32 err = AudioQueueGetPropertySize(queue, kAudioQueueProperty_MagicCookie, &propertyValueSize);
    e(err);
    if (propertyValueSize > 0) {
        UInt8 *magicCookie = (UInt8 *)malloc(propertyValueSize);
        err = AudioQueueGetProperty(queue, kAudioQueueProperty_MagicCookie, (void *)magicCookie, &propertyValueSize);
        e(err);
        err = AudioFileSetProperty(fileHandle, kAudioFilePropertyMagicCookieData, propertyValueSize, magicCookie);
        e(err);
        free(magicCookie);
    }
}

int ComputeRecordBufferSize(const AudioStreamBasicDescription *format, AudioQueueRef queue, float seconds) {
    assert(seconds > 0);
    assert(format->mSampleRate > 0);

    int totalNumberOfSamples = seconds * format->mSampleRate;
    int totalNumberOfFrames = (int)ceil(totalNumberOfSamples);

    if (format->mBytesPerFrame > 0) {
        return totalNumberOfFrames * format->mBytesPerFrame;
    }
    UInt32 maxPacketSize = 0;
    
    if (format->mBytesPerPacket > 0) {
        maxPacketSize = format->mBytesPerPacket;
    } else {
        UInt32 propertySize = sizeof(maxPacketSize);
        SInt32 err = AudioQueueGetProperty(queue, kAudioQueueProperty_MaximumOutputPacketSize, &maxPacketSize, &propertySize);
        e(err);
    }
    
    int totalNumberOfPackets = 0;
    int numberOfFramesPerPacket = 1;
    
    if (format->mFramesPerPacket > 0) {
        numberOfFramesPerPacket = format->mFramesPerPacket;
    }
    
    totalNumberOfPackets = totalNumberOfFrames / numberOfFramesPerPacket;
        
    // We have number of packets and packet size. Hence we can now get the number of bytes needed.
    return totalNumberOfPackets * maxPacketSize;
}

void record() {
    AudioStreamBasicDescription format;
    memset(&format, 0, sizeof(format));

    format.mFormatID = kAudioFormatMPEG4AAC; // MPEG4 AAC, Advanced Audio Coding
    format.mChannelsPerFrame = 2;

    GetDefaultInputDeviceSampleRate(&format.mSampleRate);

    UInt32 recordFormatSize = sizeof(format);
    SInt32 err = AudioFormatGetProperty(kAudioFormatProperty_FormatInfo,
        0,
        NULL,
        &recordFormatSize,
        &format);
    e(err);

    MyRecorder recorder = {0};
    AudioQueueRef queue = {0};
    err = AudioQueueNewInput(&format,
        MyAQInputCallback,
        &recorder,
        NULL,
        NULL,
        0,
        &queue);
    e(err);

    recordFormatSize = sizeof(format);
    err = AudioQueueGetProperty(queue, kAudioQueueProperty_StreamDescription, &format, &recordFormatSize);
    e(err);
    CFStringRef fpath = CFStringCreateWithCString(kCFAllocatorDefault, "/Users/jamie/Documents/code/c/fft/hello.caf", kCFStringEncodingASCII);
    CFURLRef url = CFURLCreateWithString(NULL, fpath, NULL);
    err = AudioFileCreateWithURL(url, kAudioFileCAFType, &format, kAudioFileFlags_EraseFile, &recorder.recordFile);
    e(err);
    CopyEncoderCookieToFile(queue, recorder.recordFile);
    int bufferByteSize = ComputeRecordBufferSize(&format, queue, kBufferDurationInSeconds);

    int bufferIndex = 0;
    for(bufferIndex = 0; bufferIndex < kNumberOfRecordingBuffers; bufferIndex++) {
        AudioQueueBufferRef buffer;
        err = AudioQueueAllocateBuffer(queue, bufferByteSize, &buffer);
        e(err);
        err = AudioQueueEnqueueBuffer(queue, buffer, 0, NULL);
        e(err);
    }

    recorder.running = TRUE;
    err = AudioQueueStart(queue, NULL);
    e(err);   
    printf("Press any key to stop...\n");
    getchar();
    recorder.running = FALSE;

    err = AudioQueueStop(queue, TRUE);
    e(err);

    CopyEncoderCookieToFile(queue, recorder.recordFile);
    err = AudioQueueDispose(queue, TRUE);
    e(err);
    err = AudioFileClose(recorder.recordFile);
    e(err);
}

