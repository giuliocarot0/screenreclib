//
// Created by Giulio Carota on 17/10/21.
//

#ifndef SCREENRECLIB_SCPPAUDIOTRANSCODER_H
#define SCREENRECLIB_SCPPAUDIOTRANSCODER_H

typedef struct a{

}SRAudioTransceiverOptions;

class SCPPAudioTranscoder {

private:
    SCPPAudioTranscoder* _currentOptions;
    int verifyOptions(SRAudioTransceiverOptions *options);
public:
    int initOptions(SRAudioTransceiverOptions **);
    int setOptions(SRAudioTransceiverOptions *);
    int receivePacket();
    int getPacket();
};


#endif //SCREENRECLIB_SCPPAUDIOTRANSCODER_H
