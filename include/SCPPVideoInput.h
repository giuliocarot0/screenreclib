//
// Created by Giulio Carota on 17/10/21.
//

#ifndef SCREENRECLIB_SCPPVIDEOINPUT_H
#define SCREENRECLIB_SCPPVIDEOINPUT_H

#include "SCPPInput.h"

class SCPPVideoInput: public SCPPInput{

private:
    int fps;
public:
    SCPPVideoInput(char *deviceSrc, char *deviceUrl, SRResolution res, SROffset off, int fps);
    AVFormatContext* open() override;
};


#endif //SCREENRECLIB_SCPPVIDEOINPUT_H
