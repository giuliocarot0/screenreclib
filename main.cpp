#include <iostream>
#include "SCPPVideoInput.h"
#include "SCPPAudioInput.h"


int main() {
    //Open two input devices
    SCPPVideoInput scppVideoInput("x11grab", "1:none", SRResolution{1920,1080}, SROffset{0,0}, 30 );
    scppVideoInput.open();
    AVFormatContext* test_video = scppVideoInput.getFormatContext();

    return 0;
}
