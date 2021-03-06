//
// Created by Giulio Carota on 17/10/21.
//

#include "transcoding/SRDecoder.h"

/**
 * The method wraps the avcodec_receive_frame which returns decoded output data from a decoder and includes some error checks
 *
 * @param frame is the frame that needs to be decoded
 * @throws SRNullFrameException if the output frame has not been initialized
 * @throws SRNullContextException if the decoder has not been initialized correctly
 * @throws DecoderException if an error occurs while getting the frame from the decoder
 */
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

/**
 * The method wraps the avcodec_send_packet which supplies raw packet data as input to a decoder.
 * Internally, this call will copy relevant AVCodecContext fields, which can influence decoding per-packet, and apply them when the packet is actually decoded.
 *
 * @param packet is the packet that needs to be decoded
 * @throws SRNullPacketException if the received packet is null
 * @throws SRNullContextException if the decoder has not been initialized correctly
 */
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
