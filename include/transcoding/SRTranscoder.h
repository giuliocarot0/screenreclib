//
// Created by Giulio Carota on 10/01/22.
//
//                                     |--------------|
// inputPacket -- transcodePacket()--> |--TRANSCODER--| -- getTranscodedPacket()--> outputPacket
//                                     |--------------|
//

#ifndef SCREENRECLIB_SRTRANSCODER_H
#define SCREENRECLIB_SRTRANSCODER_H


#include "SREncoder.h"
#include "SRDecoder.h"

class SRTranscoder {
private:
    SREncoder* encoder;
    SRDecoder* decoder;

public:
    SRTranscoder(SREncoder* encoder,SRDecoder* decoder):encoder(encoder), decoder(decoder){};
    int transcodePacket(AVPacket* inputPacket);
    virtual int getTranscodedPacket(AVPacket* outputPacket) = 0 ;
};


#endif //SCREENRECLIB_SRTRANSCODER_H
