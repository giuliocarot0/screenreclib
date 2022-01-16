//
// Created by Giulio Carota on 12/01/22.
//

#ifndef SCREENRECLIB_SRVIDEOFILTER_H
#define SCREENRECLIB_SRVIDEOFILTER_H


#include "SRTools.h"
typedef struct a{

    AVFilterInOut *outputs;
    AVFilterInOut *inputs;
    AVFilterGraph *graph;
    AVFilterContext *src_ctx;
    AVFilterContext *sink_ctx;
}cropperdata_t;



class SRVideoFilter {
private:
    bool cropper_enabled;
    cropperdata_t cropfilter{};
    SRSettings settings;
    SwsContext* rescaling_context;
    SwsContext* rescaling_context2;

    AVFrame* scaled_frame;
    AVFrame* cropped_frame;
    AVFrame* returned_frame;

    AVCodecContext *encoder;
    AVCodecContext *decoder;



public:
    SRVideoFilter( AVCodecContext *encoder, AVCodecContext *decoder, SRSettings settings): settings(settings),encoder(encoder), decoder(decoder),  rescaling_context(nullptr),scaled_frame(
            nullptr), cropped_frame(nullptr){};
    void enableBasic();
    void enableCropper();
    AVFrame * filterFrame(AVFrame* input_frame);
};


#endif //SCREENRECLIB_SRVIDEOFILTER_H
