//
// Created by Giulio Carota on 17/10/21.
//

#include "transcoding/SRDecoder.h"


int SRDecoder::getDecodedFrame(AVFrame* frame) {
    int ret;
    if(frame == nullptr) throw SRNullFrameException("Output frame has to be initialized by the caller");
    if(decoder_context == nullptr) throw SRNullContextException("Decoder has not been initialized correctly or has a null context");
        ret = avcodec_receive_frame(decoder_context, frame);
        if (ret < 0 && ret != AVERROR(EAGAIN) && ret != AVERROR_EOF) {
            fprintf(stderr, "Error during decoding\n");
            throw DecoderException ("Error during decoding");
        } else
            return ret;
}

int SRDecoder::decodePacket(AVPacket* packet) {
    int ret;
    if(packet == nullptr) throw SRNullPacketException("Decoder received a null packet.");
    if(decoder_context == nullptr) throw SRNullContextException("Decoder has been initialized with a null Codec Context");
        ret = avcodec_send_packet(decoder_context, packet);
        av_packet_unref(packet);
        return ret;
}

void SRDecoder::setDecoderContext(AVCodecContext *decoder_ctx) {
    this->decoder_context = decoder_ctx;
}

AVCodecContext *SRDecoder::getDecoderContext() {
    return decoder_context;
}
