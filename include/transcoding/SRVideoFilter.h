//
// Created by Giulio Carota on 12/01/22.
//

#ifndef SCREENRECLIB_SRVIDEOFILTER_H
#define SCREENRECLIB_SRVIDEOFILTER_H


#include "SRTools.h"

class SRVideoFilter {
private:
    SwsContext* rescaling_context;
    AVFrame* scaled_frame;
    AVCodecContext *encoder;
    AVCodecContext *decoder;

public:
    SRVideoFilter( AVCodecContext *encoder, AVCodecContext *decoder): encoder(encoder), decoder(decoder), rescaling_context(nullptr), scaled_frame(nullptr){};
    void init();
    AVFrame* filterFrame(AVFrame* inputFrame);
};


#endif //SCREENRECLIB_SRVIDEOFILTER_H
