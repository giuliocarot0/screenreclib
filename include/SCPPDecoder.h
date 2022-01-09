//
// Created by Giulio Carota on 17/10/21.
//

#ifndef SCREENRECLIB_SCPPDECODER_H
#define SCREENRECLIB_SCPPDECODER_H

#include "SCPPtools.h"

class SCPPDecoder {

private:
    AVCodecContext* decoder_context;

public:
    SCPPDecoder():decoder_context(nullptr){};
    void setDecoderContext(AVCodecContext *decoder_ctx);
    int decodePacket(AVPacket* packet);
    int getDecodedFrame(AVFrame* frame);
};


#endif //SCREENRECLIB_SCPPDECODER_H
