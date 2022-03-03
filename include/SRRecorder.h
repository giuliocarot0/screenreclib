//
// Created by Giulio Carota on 16/01/22.
//

#ifndef SCREENRECLIB_SRRECORDER_H
#define SCREENRECLIB_SRRECORDER_H
#include "SRTools.h"
#include <iostream>
#include <transcoding/SREncoder.h>
#include <muxing/SRMediaOutput.h>
#include <transcoding/SRVideoFilter.h>
#include "demuxing/SRVideoInput.h"
#include "demuxing/SRAudioInput.h"
#include "transcoding/SRDecoder.h"
#include "transcoding/SRAudioFilter.h"
#include <shared_mutex>

#ifdef __APPLE__
    #define VIDEO_SRC "avfoundation"
    #define VIDEO_URL "1:none"
    #define VIDEO_FPS 30 //automagical detection
    #define VIDEO_CODEC AV_CODEC_ID_H264
    #define CODEC_NULL AV_CODEC_ID_NONE

    #define AUDIO_SRC "avfoundation"
    #define AUDIO_URL "none:0"
    #define AUDIO_CODEC AV_CODEC_ID_AAC

#elif _WIN32
    #define VIDEO_SRC ("gdigrab")
    #define VIDEO_URL ("desktop")
    #define VIDEO_FPS 30
    #define VIDEO_CODEC AV_CODEC_ID_H264
    #define CODEC_NULL AV_CODEC_ID_NONE

    #define AUDIO_SRC ("dshow")
    #define AUDIO_URL ("audio=Microfono (Realtek High Definition Audio)")
    #define AUDIO_CODEC AV_CODEC_ID_AAC

#elif __LINUX__
    #define VIDEO_SRC ("x11grab")
    #define VIDEO_URL (":0.0+0,0")
    #define VIDEO_FPS 30
    #define VIDEO_CODEC AV_CODEC_ID_MPEG4
    #define CODEC_NULL AV_CODEC_ID_NONE

    #define AUDIO_SRC ("pulse")
    #define AUDIO_URL ("alsa_output.pci-0000_00_1b.0.analog-stereo.monitor")
    #define AUDIO_CODEC AV_CODEC_ID_AAC
#endif

class SRRecorder {
private:
public:
    virtual ~SRRecorder();

private:
    /*pointers for holding initialized units*/

    /*demuxer*/
    std::unique_ptr<SRVideoInput> videoInput;
    std::unique_ptr<SRAudioInput> audioInput;
    /*transcoding*/
    std::unique_ptr<SREncoder> videoEncoder;
    std::unique_ptr<SRDecoder> videoDecoder;
    std::unique_ptr<SRVideoFilter> videoFilter;
    std::unique_ptr<SRAudioFilter> audioFilter;
    std::unique_ptr<SREncoder> audioEncoder;
    std::unique_ptr<SRDecoder> audioDecoder;
    //SRVideoFilter videoFilter;
    /*muxing*/
    std::unique_ptr<SRMediaOutput> outputFile;
    SROutputSettings outputSettings{};

    /*the configuration is created by the main app and passed through the constructor*/
    SRConfiguration configuration{};

    /* threads */
    std::unique_ptr<thread> videoThread;
    std::unique_ptr<thread> audioThread;
    exception_ptr v_exception;
    exception_ptr a_exception;

    /*condition variables for threads*/
    bool capture_switch;
    bool kill_switch{};
    shared_mutex r_mutex;

    int status;

    /*the parser analyzes configurations and throws exception if it is wrong*/
    void parseConfiguration() const;



    /*thread functions*/
    void videoLoop();
    void audioLoop();

public:
    SRRecorder(SRConfiguration configuration);
    void initCapture();
    /*simplest user API*/
    void startCapture();
    void stopCaputure();
    void pauseCapture();
    void resumeCapture();
    bool isRecording();
    bool isPaused();
    bool isInitialized();
};


#endif //SCREENRECLIB_SRRECORDER_H
