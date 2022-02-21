//
// Created by Giulio Carota on 16/01/22.
//

#include "SRRecorder.h"

#include <utility>
/*the constructor sets the inputs and open them to get their capabilities*/

SRRecorder::SRRecorder(SRConfiguration configuration){
    /*audio and video have to be instantiated*/
    /* set the configuration*/
    this->configuration = std::move(configuration);
    try{
        parseConfiguration();
    }
    catch(SRException& e){
        throw e;
    }
    capture_switch = false;
    kill_switch = false;

    avdevice_register_all();

}



void SRRecorder::videoLoop() {
    AVPacket *inPacket, *outPacket;
    AVFrame *rawFrame, *scaled_frame;
    rawFrame = av_frame_alloc();
    inPacket = av_packet_alloc();
    outPacket = av_packet_alloc();

    std::shared_lock<std::shared_mutex> r_lock(r_mutex, std::defer_lock);
    bool last_state;
    long long int pause_pts = 0;
    long long int last_pts = 0;
    long long int pause_dur = 0;

    printf("[SRlib][VideoThread] started\n");

    while(true) {

        if(r_lock.try_lock()){
            last_state = capture_switch;
            if(kill_switch)
                break;
            r_lock.unlock();
        }
        /* set an additive offset to readPacket*/
        if(!last_state)
            pause_dur = pause_pts - last_pts;
        else pause_dur = 0;

        if(videoInput.readPacket(inPacket, pause_dur) >= 0){
               if(last_state) {
                    last_pts = inPacket->pts;
                }
               else {
                   pause_pts = inPacket->pts;
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
    AVPacket *inPacket, *outPacket;
    AVFrame *rawFrame, *scaled_frame;
    rawFrame = av_frame_alloc();
    inPacket = av_packet_alloc();
    outPacket = av_packet_alloc();

    static int64_t pts = 0;
    long long int last = 0;

    std::shared_lock<std::shared_mutex> r_lock(r_mutex, std::defer_lock);
    bool last_state;
    long long int pause_pts = 0;
    long long int last_pts = 0;
    long long int pause_dur = 0;

    printf("[SRlib][AudioThread] started\n");

    while(true) {

        if(r_lock.try_lock()){
            last_state = capture_switch;
            if(kill_switch)
                break;
            r_lock.unlock();
        }
        /* set an additive offset to readPacket*/
        if(!last_state)
            pause_dur = pause_pts - last_pts;
        else pause_dur = 0;

        if (audioInput.readPacket(inPacket,pause_dur) >= 0) {
            if(last_state) {
                last_pts = inPacket->pts;
            }
            else {
                pause_pts = inPacket->pts;
                continue;
            }

            audioDecoder.decodePacket(inPacket);
            while (audioDecoder.getDecodedFrame(rawFrame) >= 0) {
                scaled_frame = audioFilter.filterFrame(rawFrame);
                while (av_audio_fifo_size(audioFilter.getFifo()) >= audioEncoder.getEncoderContext()->frame_size) {
                    av_audio_fifo_read(audioFilter.getFifo(), (void **) (scaled_frame->data),audioEncoder.getEncoderContext()->frame_size);
                    scaled_frame->pts = pts;
                    pts += scaled_frame->nb_samples;
                    if (audioEncoder.encodeFrame(scaled_frame) < 0) {
                        printf("DROPPED");
                    };
                    while (audioEncoder.getEncodedPacket(outPacket) >= 0) {
                        if (outputFile.writePacket(outPacket, audio /*passing a audio packet*/) < 0) {
                            printf("DROPPED");
                        }
                    }
                }
            }
        }
    }
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
         outputSettings.audio_codec = AUDIO_CODEC;
         outputSettings.audio_samplerate = audioInput.getCodecContext()->sample_rate;
         outputSettings.audio_channels = audioInput.getCodecContext()->channels;
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
         audioFilter.set(audioEncoder.getEncoderContext(), audioDecoder.getDecoderContext());
         audioFilter.init();
     }
    }catch(SRException& e){
     throw e;
 }

    if(configuration.enable_video) videoThread = thread([&](){videoLoop();});
    if(configuration.enable_audio) audioThread = thread([&](){audioLoop();});
}


/*throws parse configuration exception*/
void SRRecorder::parseConfiguration() const {
    /* at least one codec should be provided*/
    if(!configuration.enable_audio && !configuration.enable_video )
        throw ConfigurationParserException("At least one codec must be set");
    if(configuration.filename == nullptr || strcmp(configuration.filename, "") == 0 )
        throw ConfigurationParserException("Invalid file name");
    if(!assertMP4(configuration.filename)) strcat(configuration.filename, ".mp4");
    if(configuration.enable_crop &&
    (configuration.crop_info.dimension.width.num > configuration.crop_info.dimension.width.den || configuration.crop_info.offset.x.num >= configuration.crop_info.offset.x.den||
    (configuration.crop_info.dimension.height.num > configuration.crop_info.dimension.height.den || configuration.crop_info.offset.y.num >= configuration.crop_info.offset.y.den )))
        throw ConfigurationParserException("Invalid crop setup");
}

void SRRecorder::startCapture() {
    if(kill_switch) return;
    std::unique_lock<std::shared_mutex> r_lock(r_mutex);
    capture_switch = true;
    if(configuration.enable_video) printf("[SRlib][VideoThread] capturing\n");
    if(configuration.enable_audio) printf("[SRlib][AudioThread] capturing\n");

}

void SRRecorder::pauseCapture() {
    if(kill_switch) return;
    std::unique_lock<std::shared_mutex> r_lock(r_mutex);
    capture_switch = false;
    if(configuration.enable_video) printf("[SRlib][VideoThread] paused\n");
    if(configuration.enable_audio) printf("[SRlib][AudioThread] paused\n");
}


void SRRecorder::stopCaputure() {
    if(kill_switch) return;
    std::unique_lock<std::shared_mutex> r_lock(r_mutex);
    capture_switch = false;
    kill_switch = true;
    if(configuration.enable_video) printf("[SRlib][VideoThread] stopped\n");
    if(configuration.enable_audio) printf("[SRlib][AudioThread] stopped\n");
}


SRRecorder::~SRRecorder() {
    if(configuration.enable_video && videoThread.joinable()) videoThread.join();
    if(configuration.enable_audio && audioThread.joinable()) audioThread.join();

}