//
// Created by Giulio Carota on 17/10/21.
//

#ifndef SCREENRECLIB_SRDECODER_H
#define SCREENRECLIB_SRDECODER_H

#include "SRTools.h"

class SRDecoder {

private:
    AVCodecContext* decoder_context;

public:
    SRDecoder():decoder_context(nullptr){};
    void setDecoderContext(AVCodecContext *decoder_ctx);
    int decodePacket(AVPacket* packet);
    int getDecodedFrame(AVFrame* frame);
};


#endif //SCREENRECLIB_SRDECODER_H
