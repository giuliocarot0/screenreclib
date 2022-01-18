//
// Created by Giulio Carota on 17/10/21.
//

#ifndef SCREENRECLIB_SRMEDIAOUTPUT_H
#define SCREENRECLIB_SRMEDIAOUTPUT_H

#include "SRTools.h"
#include <shared_mutex>

typedef enum {video, audio} media_type;

class SRMediaOutput {
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
    char* filename;
    shared_mutex w_mutex;
    //these are the only two supported streams by the recorder
    int createAudioStream();
    int createVideoStream();
public:
    SRMediaOutput();
    void set(char* o_filename, SROutputSettings o_settings);

    int initFile();
    AVCodecContext* getVideoCodecContext();
    AVCodecContext* getAudioCodecContext();
    //output format context cannot be disclosed due to critical region in writing packets
    int writePacket(AVPacket* packet, media_type type);

    char *getFilename();

    ~SRMediaOutput();
};


#endif //SCREENRECLIB_SRMEDIAOUTPUT_H
