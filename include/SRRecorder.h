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
    #define VIDEO_FPS -1 //automagical detection
    #define VIDEO_CODEC AV_CODEC_ID_MPEG4
    #define CODEC_NULL AV_CODEC_ID_NONE

    #define AUDIO_SRC "avfoundation"
    #define AUDIO_URL "none:0"
    #define AUDIO_CODEC AV_CODEC_ID_AAC
#endif

#ifdef _WIN32
    #define VIDEO_SRC ("gdigrab")
    #define VIDEO_URL ("desktop")
    #define VIDEO_FPS 30
    #define VIDEO_CODEC AV_CODEC_ID_MPEG4
    #define CODEC_NULL AV_CODEC_ID_NONE

    #define AUDIO_SRC ("dshow")
    #define AUDIO_URL ("audio=Microfono (Realtek High Definition Audio)")
    #define AUDIO_CODEC AV_CODEC_ID_AAC
#endif


class SRRecorder {
private:
    /*pointers for holding initialized units*/
    /*demuxer*/
    SRVideoInput videoInput;
    SRAudioInput audioInput;
    /*transcoding*/
    SREncoder videoEncoder;
    SRDecoder videoDecoder;
    SRVideoFilter videoFilter;
    SRAudioFilter audioFilter;
    SREncoder audioEncoder;
    SRDecoder audioDecoder;
    //SRVideoFilter videoFilter;
    /*muxing*/
    SRMediaOutput outputFile;
    SROutputSettings outputSettings{};

    /*the configuration is created by the main app and passed through the constructor*/
    SRConfiguration configuration{};

    thread videoThread;
    thread audioThread;

    /*condition variables for threads*/
    bool capture_switch;
    bool kill_switch{};
    shared_mutex r_mutex;


    /*the parser analyzes configurations and throws exception if it is wrong*/

    void parseConfiguration();



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

     ~SRRecorder();
};



#endif //SCREENRECLIB_SRRECORDER_H
