//
// Created by Giulio Carota on 12/01/22.
//

#include "transcoding/SRVideoFilter.h"

AVFrame* SRVideoFilter::filterFrame(AVFrame* inputFrame) {
    if(scaled_frame == nullptr || encoder == nullptr || decoder == nullptr || rescaling_context == nullptr) {
        //todo excepetion uninitialized filter 
        return nullptr;
    }
    /*initializing scaleFrame */
    scaled_frame->width = encoder->width;
    scaled_frame->height = encoder->height;
    scaled_frame->format = encoder->pix_fmt;
    scaled_frame->pts = inputFrame->best_effort_timestamp;
    scaled_frame->pkt_dts=inputFrame->pkt_dts;
    scaled_frame->pkt_duration = inputFrame->pkt_duration;
    sws_scale(rescaling_context, inputFrame->data, inputFrame->linesize,0, decoder->height, scaled_frame->data, scaled_frame->linesize);

    return scaled_frame;
}

void SRVideoFilter::init() {
    if(encoder == nullptr || decoder == nullptr) {
        //todo invalid filter parameters
        return;
    }
    scaled_frame = av_frame_alloc();


    int nbytes = av_image_get_buffer_size(encoder->pix_fmt, encoder->width, encoder->height,32);
    auto *video_outbuf = (uint8_t*)av_malloc(nbytes*sizeof (uint8_t));
    if( video_outbuf == nullptr )
    {
        cout<<"\nunable to allocate memory";
        exit(1); //todo buffer allocation exception
    }

    int ret;
    // Setup the data pointers and linesizes based on the specified image parameters and the provided array.
    ret = av_image_fill_arrays( scaled_frame->data, scaled_frame->linesize, video_outbuf , AV_PIX_FMT_YUV420P,encoder->width,encoder->height,1 ); // returns : the size in bytes required for lib
    if(ret < 0)
    {
        cout<<"\nerror in filling image array";
    }
    // Allocate and return swsContext.
    // a pointer to an allocated context, or NULL in case of error
    // Deprecated : Use sws_getCachedContext() instead.
    rescaling_context = sws_getContext(decoder->width,
                             decoder->height,
                             decoder->pix_fmt,
                             encoder->width,
                             encoder->height,
                             encoder->pix_fmt,
                             SWS_BICUBIC, NULL, NULL, NULL);
}
