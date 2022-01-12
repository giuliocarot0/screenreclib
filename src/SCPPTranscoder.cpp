//
// Created by Giulio Carota on 10/01/22.
//

#include "SCPPTranscoder.h"

int SCPPTranscoder::transcodePacket(AVPacket *inputPacket) {
    if(encoder != nullptr && decoder != nullptr &&inputPacket!=nullptr)
        return decoder->decodePacket(inputPacket);
    else
        return -15;
}

