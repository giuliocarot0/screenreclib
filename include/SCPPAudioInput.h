//
// Created by Giulio Carota on 17/10/21.
//

#ifndef SCREENRECLIB_SCPPAUDIOINPUT_H
#define SCREENRECLIB_SCPPAUDIOINPUT_H

#include "SCPPInput.h"

class SCPPAudioInput: public SCPPInput {

public:

    SCPPAudioInput(char *audio_src, char *audio_url);

    virtual ~SCPPAudioInput();

    AVFormatContext* open() override;
    int getInputAudioStreamIndex() const;
    AVFormatContext *getInputAudioFormatContext() const;
    AVCodecContext *getInputAudioCodecContext() const;

};


#endif //SCREENRECLIB_SCPPAUDIOINPUT_H
