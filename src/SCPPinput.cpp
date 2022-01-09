//
// Created by giuli on 29/12/2021.
//
#include "SCPPInput.h"


SCPPInput::SCPPInput(char *device_src, char *device_url) {
    options = nullptr;
    streamIndex = -1;
    inFormatContext = nullptr;
    inCodecContext = nullptr;
    this->device_src = device_src;
    this->device_url = device_url;
    // a void packet with default settings is allocated
    // payload is injected by read frame each time and does not need to be allocated
    // it is destroyed when the input is closed and the payload changes each time read_frame() is called
    deliverable_packet = av_packet_alloc();
}

int SCPPInput::readPacket(AVPacket* read_packet) {
    int ret;
    ret = av_read_frame(inFormatContext, read_packet);
    if (ret >= 0 && deliverable_packet->stream_index == streamIndex) {
        //todo: check if a rescale is needed here
        return ret;
    } else
        return -1;
}

int SCPPInput::getStreamIndex() const {
    return streamIndex;
}

AVFormatContext *SCPPInput::getFormatContext() const {
    return inFormatContext;
}

AVCodecContext *SCPPInput::getCodecContext() const {
    return inCodecContext;
}

SCPPInput::~SCPPInput() {

    av_packet_free(&deliverable_packet);
    avformat_close_input(&inFormatContext);
    if (!inFormatContext) {
        cout << "\nInput closed sucessfully";
    }
    else {
        cout << "\nunable to close the input";
        exit(1);
    }
    avformat_free_context(inFormatContext);
    if (!inFormatContext) {
        cout << "\navformat free successfully";
    }
    else {
        cout << "\nunable to free avformat context";
        exit(1);
    }
}