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
    SRAudioFilter ( AVCodecContext *encoder, AVCodecContext *decoder): encoder(encoder), decoder(decoder), resampling_context(nullptr){};
    void init();
    int init_fifo(AVCodecContext * outACodecContext);
    int add_samples_to_fifo(uint8_t **converted_input_samples, const int frame_size);
    int initConvertedSamples(uint8_t ***converted_input_samples, AVCodecContext *output_codec_context, int frame_size);
    AVFrame* filterFrame(AVFrame* inputFrame);

    AVAudioFifo *getFifo() const;

};


#endif //SCREENRECLIB_SRAUDIOFILTER_H
