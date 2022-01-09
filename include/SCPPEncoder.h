//
// Created by Giulio Carota on 17/10/21.
//

#ifndef SCREENRECLIB_SCPPENCODER_H
#define SCREENRECLIB_SCPPENCODER_H

#include "SCPPtools.h"

/**
 * REMEMBER!!! both video and audio transcoders need to
 * implement their own rescaling procedure.
 */
class SCPPEncoder {

private:
    AVCodecContext* encoder_context;

public:
    SCPPEncoder():encoder_context(nullptr){};
    void setEncoderContext(AVCodecContext *encoder_context);
    //receives a rescaled frame from transcoder class eventually
    int encodeFrame(AVFrame* frame);
    int getEncodedPacket(AVPacket* packet);

};


#endif
//SCREENRECLIB_SCPPENCODER_H
