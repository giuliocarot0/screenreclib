//
// Created by Giulio Carota on 17/10/21.
//

#include "transcoding/SRDecoder.h"


int SRDecoder::getDecodedFrame(AVFrame* frame) {
    int ret;
    if(frame!= nullptr && decoder_context!=nullptr) {
        ret = avcodec_receive_frame(decoder_context, frame);
        if (ret < 0 && ret != AVERROR(EAGAIN) && ret != AVERROR_EOF) {
            fprintf(stderr, "Error during decoding\n");
            exit(1); //todo: implement decoder_exception
        } else
            return ret;
    }
    else
        return -15; //todo: implement null from or context exception
}

int SRDecoder::decodePacket(AVPacket* packet) {
    if(packet!= nullptr && decoder_context!=nullptr)
        return avcodec_send_packet(decoder_context, packet);
    else

        return -15; // todo: implement null packet or context exception
}

void SRDecoder::setDecoderContext(AVCodecContext *decoder_ctx) {
    this->decoder_context = decoder_ctx;
}

AVCodecContext *SRDecoder::getDecoderContext() {
    return decoder_context;
}
