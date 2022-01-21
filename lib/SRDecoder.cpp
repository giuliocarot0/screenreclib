//
// Created by Giulio Carota on 17/10/21.
//

#include "transcoding/SRDecoder.h"

/**
 * The method wraps the avcodec_receive_frame which returns decoded output data from a decoder and includes some error checks
 *
 * @param frame is the frame that needs to be decoded
 */
int SRDecoder::getDecodedFrame(AVFrame* frame) {
    int ret;
    if(frame!= nullptr && decoder_context!=nullptr) {
        ret = avcodec_receive_frame(decoder_context, frame);
        if (ret < 0 && ret != AVERROR(EAGAIN) && ret != AVERROR_EOF) {
            fprintf(stderr, "Error during decoding\n");
            throw DecoderException ("Error during decoding");
        } else
            return ret;
    }
    else
        return -15; //todo: implement null from or context exception
}

/**
 * The method wraps the avcodec_send_packet which supplies raw packet data as input to a decoder.
 * Internally, this call will copy relevant AVCodecContext fields, which can influence decoding per-packet, and apply them when the packet is actually decoded.
 *
 * @param packet is the packet that needs to be decoded
 */
int SRDecoder::decodePacket(AVPacket* packet) {
    int ret;
    if(packet!= nullptr && decoder_context!=nullptr) {
        ret = avcodec_send_packet(decoder_context, packet);
        av_packet_unref(packet);
        return ret;
    }
    else

        return -15; // todo: implement null packet or context exception
}

void SRDecoder::setDecoderContext(AVCodecContext *decoder_ctx) {
    this->decoder_context = decoder_ctx;
}

AVCodecContext *SRDecoder::getDecoderContext() {
    return decoder_context;
}
