//
// Created by Giulio Carota on 10/01/22.
//
//                                     |--------------|
// inputPacket -- transcodePacket()--> |--TRANSCODER--| -- getTranscodedPacket()--> outputPacket
//                                     |--------------|
//

#ifndef SCREENRECLIB_SCPPTRANSCODER_H
#define SCREENRECLIB_SCPPTRANSCODER_H


#include "SCPPEncoder.h"
#include "SCPPDecoder.h"

class SCPPTranscoder {
private:
    SCPPEncoder* encoder;
    SCPPDecoder* decoder;

public:
    SCPPTranscoder(SCPPEncoder* encoder,SCPPDecoder* decoder):encoder(encoder), decoder(decoder){};
    int transcodePacket(AVPacket* inputPacket);
    virtual int getTranscodedPacket(AVPacket* outputPacket) = 0 ;
};


#endif //SCREENRECLIB_SCPPTRANSCODER_H
