#include "SRRecorder.h"

int main(){
    //todo: parse filename and append .mp4 to it, if null then uses default name
    char filename[512] = "filmato_di_prova.mp4";


    SRConfiguration configuration;
    configuration.enable_audio = true;
    configuration.enable_video = true;
    configuration.filename = filename;
    configuration.enable_crop = false;
    configuration.crop_info = SRCropRegion {SROffsetRational{{1,4},{1,4}},SRResolutionRational {{1,4},{1,4}} };
    /* a recorder is instantiated*/
    try {
        SRRecorder recorder(configuration);
        /*before setting crop the recorder must be initialized to retrieve capabilities from the devices*/
        recorder.initCapture();
        lock_thread_for(8);
        recorder.startCapture();
        lock_thread_for(4);
        recorder.pauseCapture();
        lock_thread_for(3);
        recorder.startCapture();
        lock_thread_for(5);
        recorder.stopCaputure();
    }
    catch(SRException& exc){
        fprintf(stderr,"[SRLib] Exception occured: %s", exc.what());
    }
}