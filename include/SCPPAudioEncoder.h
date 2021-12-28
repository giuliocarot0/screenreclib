//
// Created by Giulio Carota on 17/10/21.
//

#ifndef SCREENRECLIB_SCPPAUDIODECODER_H
#define SCREENRECLIB_SCPPAUDIODECODER_H

typedef struct a{

}SRAudioTransceiverOptions;

class SCPPAudioDecoder {

private:
    SCPPAudioDecoder* _currentOptions;
    int verifyOptions(SRAudioTransceiverOptions *options);
public:
    int initOptions(SRAudioTransceiverOptions **);
    int setOptions(SRAudioTransceiverOptions *);
    int receivePacket();
    int getPacket();
};


#endif //SCREENRECLIB_SCPPAUDIODECODER_H
