//
// Created by Giulio Carota on 17/10/21.
//
extern "C"{
    #include <libavcodec/avcodec.h>
};
#include <transcoding/SREncoder.h>


void SREncoder::setEncoderContext(AVCodecContext *encoderContext) {
    encoder_context = encoderContext;
}

/**
 * The method wraps the avcodec_send_frame which supplies a raw video or audio frame to the encoder and includes some error checks
 *
 * @param frame is the frame that needs to be decoded
 */
int SREncoder::encodeFrame(AVFrame *frame) {
    int ret = avcodec_send_frame(encoder_context, frame);
    //av_frame_unref(frame);
    return ret;
}

/**
 * The method wraps the avcodec_receive_packet which reads encoded data from the encoder and includes some error checks
 *
 * @param packet is the packet that needs to be decoded
 * @throws EncoderException if an error occurs while getting the packet from the encoder
 */
int SREncoder::getEncodedPacket(AVPacket* packet) {
    int ret;
    ret = avcodec_receive_packet(encoder_context, packet);
    if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
        return ret;
    else if (ret < 0) {
        fprintf(stderr, "Error during encoding\n");
        throw EncoderException ("Error during encoding");
    }
    return 0;
}

AVCodecContext *SREncoder::getEncoderContext() {
    return encoder_context;
}

