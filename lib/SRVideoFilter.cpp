//
// Created by Giulio Carota on 12/01/22.
//

#include "transcoding/SRVideoFilter.h"



AVFrame* SRVideoFilter::filterFrame(AVFrame* input_frame) {
    if(scaled_frame == nullptr || encoder == nullptr || decoder == nullptr || rescaling_context == nullptr) {
        throw UninitializedFilterException("Audio filter not initialized");
    }
    int ret;
    //initializing scaleFrame
    scaled_frame->width = encoder->width;
    scaled_frame->height = encoder->height;
    scaled_frame->format = encoder->pix_fmt;
    scaled_frame->pts = input_frame->best_effort_timestamp;
    scaled_frame->pkt_dts=input_frame->pkt_dts;
    scaled_frame->pkt_duration = input_frame->pkt_duration;


    if(cropper_enabled) {
        av_frame_ref(cropped_frame, input_frame);
        if (av_buffersrc_add_frame(cropfilter.src_ctx, cropped_frame) < 0) {
            throw CropperException("Error while enabling the video cropper");
        } else {
            ret = av_buffersink_get_frame(cropfilter.sink_ctx, cropped_frame);
            if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
                return nullptr;
            if (ret < 0)
                return nullptr;
        }
        sws_scale(rescaling_context2, cropped_frame->data, cropped_frame->linesize,0, encoder->height, scaled_frame->data, scaled_frame->linesize);
    }
    else{

        sws_scale(rescaling_context, input_frame->data, input_frame->linesize,0, decoder->height, scaled_frame->data, scaled_frame->linesize);
    }
    av_frame_unref(input_frame);
    av_frame_ref(returned_frame,scaled_frame);
    return returned_frame;

}

void SRVideoFilter::enableBasic() {
    if(encoder == nullptr || decoder == nullptr) {
        throw InvalidFilterParametersException("Found wrong parameters while enabling the basic video filter");
    }
    //input_frame = av_frame_alloc();

    scaled_frame = av_frame_alloc();
    //scaled frame used only internally and cannot be unref from external methods
    returned_frame = av_frame_alloc();

    int nbytes = av_image_get_buffer_size(encoder->pix_fmt, encoder->width, encoder->height,32);
    auto *video_outbuf = (uint8_t*)av_malloc(nbytes*sizeof (uint8_t));
    if( video_outbuf == nullptr )
    {
        throw BufferAllocationException("Unable to allocate memory");
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
void SRVideoFilter::enableCropper() {
    char args[512];
    int ret = 0;
    cropped_frame = av_frame_alloc();
    cropfilter.outputs = nullptr;
    cropfilter.inputs = nullptr;
    cropfilter.graph = avfilter_graph_alloc();

    SRResolution dim = rescale_resolution({decoder->width, decoder->height}, settings.crop.dimension);
    SROffset off = rescale_offset({decoder->width, decoder->height}, settings.crop.offset);

    snprintf(args, sizeof(args),
             "buffer=video_size=%dx%d:pix_fmt=%d:time_base=1/1:pixel_aspect=1/1[in];"
             "[in]crop=%d:%d:%d:%d[out];"
             "[out]buffersink",
             decoder->width, decoder->height, decoder->pix_fmt, dim.width, dim.height,
             off.x, off.y);

    ret = avfilter_graph_parse2(cropfilter.graph, args, &cropfilter.inputs, &cropfilter.outputs);
    if (ret < 0) exit(1);
    assert(cropfilter.inputs == nullptr && cropfilter.outputs == nullptr);
    ret = avfilter_graph_config(cropfilter.graph, nullptr);
    if (ret < 0) exit(1);

    cropfilter.src_ctx = avfilter_graph_get_filter(cropfilter.graph, "Parsed_buffer_0");
    cropfilter.sink_ctx = avfilter_graph_get_filter(cropfilter.graph, "Parsed_buffersink_2");


        assert(cropfilter.src_ctx != nullptr);
        assert(cropfilter.sink_ctx != nullptr);

        cropper_enabled = true;
        /*end:
            avfilter_inout_free(&cropfilter.inputs);
            avfilter_inout_free(&cropfilter.outputs);
            //todo cropperException
    */

    // Allocate and return swsContext.
    // a pointer to an allocated context, or NULL in case of error
    // Deprecated : Use sws_getCachedContext() instead.
    rescaling_context2 = sws_getContext(encoder->width,
                                        encoder->height,
                                        static_cast<AVPixelFormat>(cropfilter.sink_ctx->inputs[0]->format),
                                        encoder->width,
                                        encoder->height,
                                        encoder->pix_fmt,
                                        SWS_BICUBIC, NULL, NULL, NULL);
}

SRVideoFilter::~SRVideoFilter() {
    sws_freeContext(rescaling_context);
    av_frame_free(&scaled_frame);
    av_frame_free(&returned_frame);

    if(cropper_enabled){
        av_frame_free(&cropped_frame);
        sws_freeContext(rescaling_context2);
        avfilter_free(cropfilter.sink_ctx);
        avfilter_free(cropfilter.src_ctx);
        avfilter_graph_free(&cropfilter.graph);
    }
}

void SRVideoFilter::set(AVCodecContext *v_encoder, AVCodecContext *v_decoder, SROutputSettings v_settings) {
    this->encoder = v_encoder;
    this->decoder = v_decoder;
    this->settings = SROutputSettings{v_settings};
}
