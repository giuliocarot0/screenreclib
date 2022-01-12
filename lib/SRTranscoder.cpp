//
// Created by Giulio Carota on 10/01/22.
//

#include "transcoding/SRTranscoder.h"

int SRTranscoder::transcodePacket(AVPacket *inputPacket) {
    if(encoder != nullptr && decoder != nullptr &&inputPacket!=nullptr)
        return decoder->decodePacket(inputPacket);
    else
        return -15;
}

