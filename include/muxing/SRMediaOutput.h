//
// Created by Giulio Carota on 17/10/21.
//

#ifndef SCREENRECLIB_SRMEDIAOUTPUT_H
#define SCREENRECLIB_SRMEDIAOUTPUT_H

#include "SRTools.h"
typedef enum {video, audio} media_type;
class SRMediaOutput {
private:
    bool video_recorded;
    bool audio_recorded;
    AVOutputFormat* outputFormat;
    AVFormatContext* outputCtx;
    AVCodecContext* videoCtx;
    AVCodecContext* audioCtx;
    SRSettings settings{};
    int audioStreamID;
    int videoStreamID;

    //these are the only two supported streams by the recorder
    int createAudioStream();
    int createVideoStream();
public:
    explicit SRMediaOutput(SRSettings outputSettings);
    int initFile();
    AVCodecContext* getVideoCodecContext();
    AVCodecContext* getAudioCodecContext();
    //output format context cannot be disclosed due to critical region in writing packets
    int writePacket(AVPacket* packet, media_type type);

    char *getFilename();

    ~SRMediaOutput();
};


#endif //SCREENRECLIB_SRMEDIAOUTPUT_H
