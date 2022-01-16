#include <iostream>
#include <transcoding/SREncoder.h>
#include <muxing/SRMediaOutput.h>
#include <transcoding/SRVideoFilter.h>
#include "demuxing/SRVideoInput.h"
#include "demuxing/SRAudioInput.h"
#include "transcoding/SRDecoder.h"


int main() {
    int fps = 30;
    char filename[512] = "testfile.mp4";

    AVPacket *inPacket, *outPacket;
    AVFrame *rawFrame, *scaled_frame;
    rawFrame = av_frame_alloc();
    inPacket = av_packet_alloc();
    outPacket = av_packet_alloc();
    avdevice_register_all();

    //Open two input devices


   //set output file for video only
    SRSettings outputSettings;



    //SRResolution{0,0} means auto selection from input device
    SRVideoInput videoInput("avfoundation", "1:none", AUTO_RESOLUTION, fps );

    outputSettings.video_codec = AV_CODEC_ID_MPEG4;
    outputSettings.audio_codec = AV_CODEC_ID_NONE;
    outputSettings.enable_crop = true;
    outputSettings.crop = {SROffset{200,800}, SRResolution {1280,800}};
    outputSettings.fps = fps;
    outputSettings.filename = filename;
    //use the same resolution as input unless differently specified
    videoInput.open();

    outputSettings.outscreenres = videoInput.getInputResolution();

    SRMediaOutput outputFile(outputSettings);

    SRDecoder videoDecoder;
    SREncoder videoEncoder;

    videoDecoder.setDecoderContext(videoInput.getCodecContext());
    outputFile.initFile();
    videoEncoder.setEncoderContext(outputFile.getVideoCodecContext());

    SRVideoFilter videoFilter(outputFile.getVideoCodecContext(), videoInput.getCodecContext(), outputSettings);
    videoFilter.enableBasic();
    videoFilter.enableCropper();

    long long int last = 0;
    printf("[SRlib] recording screen\n");
    while(last/outputSettings.fps < 1 /*record for five sec*/) {


        if(videoInput.readPacket(inPacket) >= 0){
            last = inPacket->pts;
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
    printf("[SRlib] cleaning up\n");
    av_frame_free(&rawFrame);
    av_packet_free(&inPacket);
    av_packet_free(&outPacket);
    return 0;
}
