//
// Created by Giulio Carota on 17/10/21.
//

#ifndef SCREENRECLIB_SCPPMEDIAOUTPUT_H
#define SCREENRECLIB_SCPPMEDIAOUTPUT_H

#include "SCPPtools.h"

class SCPPMediaOutput {
private:
    bool video_recorded;
    bool audio_recorded;
    AVOutputFormat* outputFormat;
    AVFormatContext* outputCtx;
    AVCodecContext* videoCtx;
    AVCodecContext* audioCtx;
    SROutputSettings settings{};
    int audioStreamID;
    int videoStreamID;

    //these are the only two supported streams by the recorder
    int createAudioStream();
    int createVideoStream();
public:
    explicit SCPPMediaOutput(SROutputSettings outputSettings);
    int initFile();
    AVCodecContext* getVideoCodecContext();
    AVCodecContext* getAudioCodecContext();
    //output format context cannot be disclosed due to critical region in writing packets
    int writePacket(AVPacket* packet);
};


#endif //SCREENRECLIB_SCPPMEDIAOUTPUT_H
