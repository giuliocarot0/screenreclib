//
// Created by Giulio Carota on 17/10/21.
//

#include "../include/SCPPRecorder.h"


SCPPRecorder::SCPPRecorder(bool sessionIsMoving, SROptions *currOptions) : sessionIsMoving(false),
                                                                           _currOptions(nullptr) {}

int SCPPRecorder::setOptions(SROptions *) {
    return 0;
}

int SCPPRecorder::initOptions(SROptions **) {
    return 0;
}

int SCPPRecorder::verifyOptions(SROptions *) {
    if(_currOptions== nullptr)
        return -1;
    else
        return 0;
}

int SCPPRecorder::stopRecording() {
    return 0;
}

int SCPPRecorder::startRecording() {
    if(verifyOptions(_currOptions) < 0)
        return -1;
    else
        return 0;
}

int SCPPRecorder::pauseRecording() {
    return 0;
}


