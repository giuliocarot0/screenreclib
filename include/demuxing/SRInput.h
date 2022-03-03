//
// Created by giuli on 28/12/2021.
//

#ifndef SCREENRECLIB_SRINPUT_H
#define SCREENRECLIB_SRINPUT_H

#include "SRTools.h"
//todo enable shared pointer on inCodecContext
class SRInput {
    protected:
        long long int first_pts; /* for PTS normalization*/
        char* device_src;
        char* device_url;
        AVDictionary *options;
        int streamIndex;
        std::shared_ptr<AVFormatContext> inFormatContext;
        std::shared_ptr<AVCodecContext> inCodecContext;

    public:
        SRInput();

        virtual std::shared_ptr<AVFormatContext> open() = 0;
        virtual ~SRInput();
        std::shared_ptr<AVCodecContext> getCodecContext() const;

    int readPacket(AVPacket* read_packet, long long int pts_offset);
};



#endif //SCREENRECLIB_SRINPUT_H
