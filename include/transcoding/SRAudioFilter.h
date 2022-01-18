//
// Created by Alex on 12/01/2022.
//

#ifndef SCREENRECLIB_SRAUDIOFILTER_H
#define SCREENRECLIB_SRAUDIOFILTER_H

#include "SRTools.h"

class SRAudioFilter {
private:
    SwrContext* resampling_context;
    AVCodecContext *encoder;
    AVCodecContext *decoder;
    AVAudioFifo *fifo;
    AVFrame* scaled_frame;
    uint8_t **resampledData;


public:
    SRAudioFilter(): encoder(nullptr), decoder(nullptr), resampling_context(nullptr){};
    void init();
    AVFrame* filterFrame(AVFrame* inputFrame);
    void set(AVCodecContext *v_encoder, AVCodecContext *v_decoder);

    AVAudioFifo *getFifo() const;

    virtual ~SRAudioFilter();

    int add_samples_to_fifo(const int frame_size);

    int init_fifo();

    int initConvertedSamples(int frame_size);
};


#endif //SCREENRECLIB_SRAUDIOFILTER_H
