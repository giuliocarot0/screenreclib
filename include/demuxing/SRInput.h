//
// Created by giuli on 28/12/2021.
//

#ifndef SCREENRECLIB_SRINPUT_H
#define SCREENRECLIB_SRINPUT_H

#include "SRTools.h"

class SRInput {
    protected:
        long long int first_pts; /* for PTS normalization*/
        char* device_src;
        char* device_url;
        AVDictionary *options;
        int streamIndex;
        AVFormatContext* inFormatContext;
        AVCodecContext *inCodecContext;

    public:
        SRInput();

        virtual AVFormatContext* open() = 0;
        int getStreamIndex() const;

    virtual ~SRInput();

    AVFormatContext *getFormatContext() const;
    AVCodecContext *getCodecContext() const;

    int readPacket(AVPacket* read_packet);
};



#endif //SCREENRECLIB_SRINPUT_H
