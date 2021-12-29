//
// Created by giuli on 28/12/2021.
//

#ifndef SCREENRECLIB_SCPPINPUT_H
#define SCREENRECLIB_SCPPINPUT_H

#include "SCPPtools.h"
class SCPPInput {
    protected:
        char* device_src;
        char* device_url;
        AVDictionary *options;
        int streamIndex;
        AVFormatContext* inFormatContext;
        AVCodecContext *inCodecContext;
        AVPacket* deliverable_packet;

    public:
        SCPPInput(char *device_src, char *device_url);

        virtual AVFormatContext* open() = 0;
        int getStreamIndex() const;

    virtual ~SCPPInput();

    AVFormatContext *getFormatContext() const;
        AVCodecContext *geCodecContext() const;

    };



#endif //SCREENRECLIB_SCPPINPUT_H
