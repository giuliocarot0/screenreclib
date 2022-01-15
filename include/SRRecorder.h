//
// Created by Giulio Carota on 15/01/22.
//

#ifndef SCREENRECLIB_SRRECORDER_H
#define SCREENRECLIB_SRRECORDER_H

#include "SRTools.h"

class SRRecorder {
private:
    SRSettings recorder_configuration;
    void initRecordingSession();
    /*thread management*/
    void videoLoop();
    void audioLoop();
public:
    /*simplest user API*/
    void startCapture();
    void stopCaputure();
    void pauseCapture();
};


#endif //SCREENRECLIB_SRRECORDER_H
