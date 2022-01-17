//
// Created by Giulio Carota on 16/01/22.
//

#include "SRRecorder.h"
/*the constructor sets the inputs and open them to get their capabilities*/

SRRecorder::SRRecorder(SRConfiguration configuration){
    /*audio and video have to be instantiated*/
    /* set the configuration*/
    this->configuration = configuration;
    try{
        parseConfiguration();
    }
    catch(SRException& e){
        throw e;
    }
    capture_switch = false;
    avdevice_register_all();
}



void SRRecorder::videoLoop() {
    AVPacket *inPacket, *outPacket;
    AVFrame *rawFrame, *scaled_frame;
    rawFrame = av_frame_alloc();
    inPacket = av_packet_alloc();
    outPacket = av_packet_alloc();



    long long int last = 0;
    printf("[SRlib] recording screen\n");
    while(last/outputSettings.fps < 10 /*record for five sec*/) {


        if(videoInput.readPacket(inPacket) >= 0){
           // r_lock.lock();
            if(capture_switch) {
                last = inPacket->pts;
                printf("[SRlib][VideoThread] recording\n");
            }
            else {
                printf("[SRlib][VideoThread] paused\n");
                continue;

            }
           // r_lock.unlock();
            videoDecoder.decodePacket(inPacket);
            while(videoDecoder.getDecodedFrame(rawFrame)>=0){
                scaled_frame = videoFilter.filterFrame(rawFrame);
                if(videoEncoder.encodeFrame(scaled_frame) < 0)
                    printf("DROPPED");
                while (videoEncoder.getEncodedPacket(outPacket) >= 0) {
                    if (outputFile.writePacket(outPacket, video /*passing a video packet*/) < 0) {
                        printf("DROPPED");
                    }
                }
            }
        }
    }
}


void SRRecorder::audioLoop() {

}

void SRRecorder::initCapture() {
 /*instantiate SR modules */
 try{
     // set basics in outputSettings

     //set ouput values for audio here
     /*build output configuration*/
     if(configuration.enable_video){
        videoInput.set(VIDEO_SRC, VIDEO_URL, AUTO_RESOLUTION, VIDEO_FPS);
        videoInput.open();
        videoDecoder.setDecoderContext(videoInput.getCodecContext());
        outputSettings.enable_crop = configuration.enable_crop;
        outputSettings.crop = configuration.crop_info;
        outputSettings.fps = VIDEO_FPS;
        outputSettings.video_codec = VIDEO_CODEC;
        outputSettings.outscreenres =videoInput.getInputResolution();
     }
     else
         outputSettings.video_codec =AV_CODEC_ID_NONE;

     if(configuration.enable_audio){
         audioInput.set(AUDIO_SRC, AUDIO_URL);
         audioInput.open();
         audioDecoder = SRDecoder();
         audioDecoder.setDecoderContext(audioInput.getCodecContext());
         outputSettings.video_codec = AUDIO_CODEC;
         outputSettings.audio_samplerate = 0;
         outputSettings.audio_channels = 0;
     }
     else
         outputSettings.audio_codec =AV_CODEC_ID_NONE;
     outputSettings.filename = configuration.filename;

     outputFile.set(configuration.filename,outputSettings);
     outputFile.initFile();

     if(configuration.enable_video){
         videoEncoder.setEncoderContext(outputFile.getVideoCodecContext());
         videoFilter.set(videoEncoder.getEncoderContext(), videoDecoder.getDecoderContext(),outputSettings);
         videoFilter.enableBasic();
         if(configuration.enable_crop) videoFilter.enableCropper();
     }
     if(configuration.enable_audio){
         audioEncoder.setEncoderContext(outputFile.getAudioCodecContext());
     }
    }catch(SRException& e){
     throw e;
 }

    if(configuration.enable_video) videoThread = thread([&](){videoLoop();});
   // if(configuration.enable_audio) audioThread = thread([&](){audioLoop();});
}


void SRRecorder::stopCaputure() {
}

void SRRecorder::pauseCapture() {

}
void SRRecorder::resumeCapture() {

}

/*throws parse configuration exception*/
void SRRecorder::parseConfiguration() {
    /* at least one codec should be provided*/
    if(!configuration.enable_audio && !configuration.enable_video )
        throw ConfigurationParserException("At least one codec must be set");
    if(configuration.filename == nullptr || strcmp(configuration.filename, "") == 0)
        throw ConfigurationParserException("Invalid file name");
    if(configuration.enable_crop &&
    (configuration.crop_info.dimension.width.num > configuration.crop_info.dimension.width.den || configuration.crop_info.offset.x.num >= configuration.crop_info.offset.x.den||
    (configuration.crop_info.dimension.height.num > configuration.crop_info.dimension.height.den + configuration.crop_info.offset.y.num >= configuration.crop_info.offset.y.den )))
        throw ConfigurationParserException("Invalid crop data");
}

void SRRecorder::startCapture() {
   // std::lock_guard<std::mutex> r_lock(r_mutex);
   // capture_switch = true;
}
SRRecorder::~SRRecorder() {
    videoThread.join();
}
>>>>>>> 38da28b6d17bf15b8a1bd91328c32947f07650be
