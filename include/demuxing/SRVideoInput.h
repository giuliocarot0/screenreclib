//
// Created by Giulio Carota on 17/10/21.
//

#ifndef SCREENRECLIB_SRVIDEOINPUT_H
#define SCREENRECLIB_SRVIDEOINPUT_H

#include "SRInput.h"

class SRVideoInput: public SRInput{

private:
    int fps;

public:

    SRVideoInput() :fps(-1), SRInput() {};
    void set(char *deviceSrc, char *deviceUrl, SRResolution _res, int _fps);
    AVFormatContext* open() override;
    SRResolution getInputResolution();
};


#endif //SCREENRECLIB_SRVIDEOINPUT_H
