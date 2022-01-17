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
    bool cropper_enabled{};
    cropperdata_t cropfilter{};
    SROutputSettings settings{};
    SwsContext* rescaling_context;
    SwsContext* rescaling_context2{};

    AVFrame* scaled_frame;
    AVFrame* cropped_frame;
    AVFrame* returned_frame{};

    AVCodecContext *encoder;
    AVCodecContext *decoder;



public:
    SRVideoFilter(): encoder(nullptr), decoder(nullptr),  rescaling_context(nullptr),scaled_frame(
            nullptr), cropped_frame(nullptr){};
    void set(AVCodecContext *v_encoder, AVCodecContext *v_decoder, SROutputSettings v_settings);
    void enableBasic();
    void enableCropper();
    AVFrame * filterFrame(AVFrame* input_frame);

    virtual ~SRVideoFilter();
};


#endif //SCREENRECLIB_SRVIDEOFILTER_H
