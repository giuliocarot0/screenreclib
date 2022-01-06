//
// Created by Giulio Carota on 17/10/21.
//

#ifndef SCREENRECLIB_SCPPDECODER_H
#define SCREENRECLIB_SCPPDECODER_H

typedef struct a{

}SRAudioTransceiverOptions;

class SCPPDecoder {

private:
    SCPPDecoder* _currentOptions;
    int verifyOptions(SRAudioTransceiverOptions *options);
public:
    int initOptions(SRAudioTransceiverOptions **);
    int setOptions(SRAudioTransceiverOptions *);
    int receivePacket();
    int getPacket();
};


#endif //SCREENRECLIB_SCPPDECODER_H
