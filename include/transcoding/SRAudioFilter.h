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
    AVFrame* filterFrame(AVFrame* inputFrame);

    AVAudioFifo *getFifo() const;

};


#endif //SCREENRECLIB_SRAUDIOFILTER_H
