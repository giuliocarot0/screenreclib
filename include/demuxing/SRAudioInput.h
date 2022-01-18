//
// Created by Giulio Carota on 17/10/21.
//

#ifndef SCREENRECLIB_SRAUDIOINPUT_H
#define SCREENRECLIB_SRAUDIOINPUT_H

#include "SRInput.h"


class SRAudioInput: public SRInput {

public:
    SRAudioInput() : SRInput() {};
    void set(char *audio_src, char *audio_url);
    AVFormatContext* open() override;
};


#endif //SCREENRECLIB_SRAUDIOINPUT_H
