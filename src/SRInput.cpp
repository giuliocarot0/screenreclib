//
// Created by giuli on 29/12/2021.
//
#include "demuxing/SRInput.h"

/**
 * Default SRInput Constructor
 * Instantiate a default object.
 */
SRInput::SRInput() {
    options = nullptr;
    first_pts = -1;
    streamIndex = -1;
    inFormatContext = nullptr;
    inCodecContext = nullptr;
    device_src = nullptr;
    device_url = nullptr;
    // a void packet with default settings is allocated
    // payload is injected by read frame each time and does not need to be allocated
    // it is destroyed when the input is closed and the payload changes each time read_frame() is called
}

/**
 * This method reads data from the device buffer and reference a packet with them.
 *
 * @param read_packet An initialized packet that will be referenced with read data
 * @param pts_offset  A call with pts_offset > 0 will add a pts_offset to the internal offset
 * @return  A negative value if the packet cannot be read, any positive value if the packet successfully read.
 */
int SRInput::readPacket(AVPacket* read_packet, long long int pts_offset) {
    int ret;
    if(inFormatContext == nullptr) throw SRNullContextException("Invalid Input Format Context. Check if SRInput has been correctly initialized");
    ret = av_read_frame(inFormatContext, read_packet);
    if (ret >= 0 && read_packet->stream_index == streamIndex) {
        av_packet_rescale_ts(read_packet, inFormatContext->streams[streamIndex]->time_base,inCodecContext->time_base);
        if(first_pts<0)
            first_pts = read_packet->pts;
        if(pts_offset>0){
            first_pts = first_pts + pts_offset;
        }
        read_packet->dts -= first_pts;
        read_packet->pts -= first_pts;
        return ret;
    } else
        return -1;
}

AVCodecContext *SRInput::getCodecContext() const {
    return inCodecContext;
}

/**
 * SRInput destructor.
 * The destructor frees all the pointers contained in the object
 */
SRInput::~SRInput() {

    if (inFormatContext) {
        avformat_close_input(&inFormatContext);
        if (inFormatContext) {
            cerr << "\n[SRLib][SRInput] Unable to close device";
            exit(1);
        }
    }
    if (inCodecContext) {
       avcodec_free_context(&inCodecContext);
        if (inCodecContext) {
            cerr << "\n[SRLib][SRInput] Unable to close device";
            exit(1);
        }
    }
}

