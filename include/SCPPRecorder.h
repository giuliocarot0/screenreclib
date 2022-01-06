/*
 * SCPPRecorder.h is the main interface for the library.
 * Once a SCPPRecorder element is constructed, it can be configured for
 * the current recording session.
 * The recording session starts with a specific method, can be paused, but it's stopped
 * end finalized only when the recorder element is destroyed.
 */

#ifndef SCREENRECLIB_SCPPRECORDER_H
#define SCREENRECLIB_SCPPRECORDER_H

#include "SCPPtools.h"

typedef struct a{

}SROptions;

class SCPPRecorder {
private:
    SCPPRecorder(bool sessionIsMoving, SROptions *currOptions);

    bool sessionIsMoving;
    SROptions* _currOptions;
    int verifyOptions(SROptions*);
    int stopRecording();

public:



    SCPPRecorder();

    int initOptions(SROptions**);
    int setOptions(SROptions*);
    int startRecording();
    int pauseRecording();
};


#endif //SCREENRECLIB_SCPPRECORDER_H
