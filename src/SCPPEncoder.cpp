//
// Created by Giulio Carota on 17/10/21.
//
extern "C"{
    #include <libavcodec/avcodec.h>
};
#include <SCPPEncoder.h>


void SCPPEncoder::setEncoderContext(AVCodecContext *encoderContext) {
    encoder_context = encoderContext;
}

int SCPPEncoder::encodeFrame(AVFrame *frame) {
    int ret = avcodec_send_frame(encoder_context, frame);
    return ret;
}

int SCPPEncoder::getEncodedPacket(AVPacket* packet) {
    int ret;
    ret = avcodec_receive_packet(encoder_context, packet);
    if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
        return ret;
    else if (ret < 0) {
        fprintf(stderr, "Error during encoding\n");
        exit(1);
    }
    return 0;
}

