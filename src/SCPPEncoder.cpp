//
// Created by Giulio Carota on 17/10/21.
//

#include <libavcodec/avcodec.h>
#include <SCPPEncoder.h>


void SCPPEncoder::setEncoderContext(AVCodecContext *encoderContext) {
    encoder_context = encoderContext;
}


SCPPEncoder::SCPPEncoder() {
    encoder_context = nullptr;
    output_packet = av_packet_alloc();

}

SCPPEncoder::~SCPPEncoder() {
    av_packet_free(&output_packet);
}

int SCPPEncoder::encodeFrame(AVFrame *frame) {
    return avcodec_send_frame(encoder_context, frame);
}

AVPacket *SCPPEncoder::getEncodedPacket() {
    int ret;
    ret = avcodec_receive_packet(encoder_context, output_packet);
    if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
        return nullptr;
    else if (ret < 0) {
        fprintf(stderr, "Error during encoding\n");
        exit(1);
    }
    return output_packet;
}

