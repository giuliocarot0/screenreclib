//
// Created by giuli on 29/12/2021.
//
#include "demuxing/SRInput.h"


SRInput::SRInput(char *device_src, char *device_url) {
    options = nullptr;
    first_pts = -1;
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

int SRInput::readPacket(AVPacket* read_packet) {
    int ret;
    ret = av_read_frame(inFormatContext, read_packet);
    if (ret >= 0 && read_packet->stream_index == streamIndex) {
        //todo: check if a rescale is needed here
        //printf("\nBefore: %lld,%lld\n", read_packet->pts, read_packet->duration);
        av_packet_rescale_ts(read_packet, inFormatContext->streams[streamIndex]->time_base,inCodecContext->time_base);
        if(first_pts<0)
            first_pts = read_packet->pts;
        read_packet->pts -= first_pts;
        return ret;
    } else
        return -1;
}

int SRInput::getStreamIndex() const {
    return streamIndex;
}

AVFormatContext *SRInput::getFormatContext() const {
    return inFormatContext;
}

AVCodecContext *SRInput::getCodecContext() const {
    return inCodecContext;
}

SRInput::~SRInput() {

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