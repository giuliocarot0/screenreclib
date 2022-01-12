//
// Created by Giulio Carota on 17/10/21.
//

#ifndef SCREENRECLIB_SRENCODER_H
#define SCREENRECLIB_SRENCODER_H

#include "SRTools.h"

/**
 * REMEMBER!!! both video and audio transcoders need to
 * implement their own rescaling procedure.
 */
class SREncoder {

private:
    AVCodecContext* encoder_context;

public:
    SREncoder():encoder_context(nullptr){};
    void setEncoderContext(AVCodecContext *encoder_context);
    AVCodecContext* getEncoderContext();
    //receives a rescaled frame from transcoder class eventually
    int encodeFrame(AVFrame* frame);
    int getEncodedPacket(AVPacket* packet);

};


#endif
//SCREENRECLIB_SRENCODER_H
