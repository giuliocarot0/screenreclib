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

int SREncoder::encodeFrame(AVFrame *frame) {
    return avcodec_send_frame(encoder_context, frame);
}

int SREncoder::getEncodedPacket(AVPacket* packet) {
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

AVCodecContext *SREncoder::getEncoderContext() {
    return encoder_context;
}

