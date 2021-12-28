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

    public:
        SCPPInput(char *device_src, char *device_url);


        virtual AVFormatContext* open() = 0;
        int getStreamIndex() const;
        AVFormatContext *getFormatContext() const;
        AVCodecContext *geCodecContext() const;

    };

SCPPInput::SCPPInput(char *device_src, char *device_url) {
    options = nullptr;
    streamIndex = -1;
    inFormatContext = nullptr;
    inCodecContext = nullptr;
    this->device_src = device_src;
    this->device_url = device_url;
}

int SCPPInput::getStreamIndex() const {
    return streamIndex;
}

AVFormatContext *SCPPInput::getFormatContext() const {
    return inFormatContext;
}

AVCodecContext *SCPPInput::geCodecContext() const {
    return inCodecContext;
}

#endif //SCREENRECLIB_SCPPINPUT_H
