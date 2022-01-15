#include <iostream>
#include <transcoding/SREncoder.h>
#include <muxing/SRMediaOutput.h>
#include <transcoding/SRVideoFilter.h>
#include "demuxing/SRVideoInput.h"
#include "demuxing/SRAudioInput.h"
#include "transcoding/SRDecoder.h"


int main() {
    AVPacket *inPacket, *outPacket;
    AVFrame *rawFrame, *scaled_frame;
    scaled_frame = av_frame_alloc();
    rawFrame = av_frame_alloc();
    inPacket = av_packet_alloc();
    outPacket = av_packet_alloc();
    avdevice_register_all();

    //Open two input devices


   //set output file for video only
    SRSettings outputSettings;

    outputSettings.video_codec = AV_CODEC_ID_MPEG4;
    outputSettings.audio_codec = AV_CODEC_ID_NONE;
    outputSettings.offset = SROffset{1000,1000};
    outputSettings.fps = 30;
    outputSettings.filename = "testfile.mp4";
    outputSettings.outscreenres =SRResolution{2560,1600};

    SRMediaOutput outputFile(outputSettings);
    SRVideoInput videoInput("avfoundation", "1:none", outputSettings.outscreenres, outputSettings.offset, outputSettings.fps );
    SRDecoder videoDecoder;
    SREncoder videoEncoder;

    videoInput.open();
    videoDecoder.setDecoderContext(videoInput.getCodecContext());
    outputFile.initFile();
    videoEncoder.setEncoderContext(outputFile.getVideoCodecContext());

    SRVideoFilter videoFilter(outputFile.getVideoCodecContext(), videoInput.getCodecContext(), outputSettings);
    videoFilter.enableBasic();
    videoFilter.enableCropper();

    long long int last = 0;
    printf("[SRlib - recording screen]\n");
    while(last/30 < 10 /*record for five sec*/) {


        if(videoInput.readPacket(inPacket) >= 0){
            last = inPacket->pts;
            videoDecoder.decodePacket(inPacket);
            while(videoDecoder.getDecodedFrame(rawFrame)>=0){
                scaled_frame= videoFilter.filterFrame(rawFrame);
                    if(videoEncoder.encodeFrame(scaled_frame) < 0)
                        printf("DROPPED");
                    while (videoEncoder.getEncodedPacket(outPacket) >= 0) {
                        if (outputFile.writePacket(outPacket, video /*passing a video packet*/) < 0) {
                            printf("DROPPED");
                        }
                    }
                }
            av_packet_unref(inPacket);
        }
    }
    av_frame_free(&rawFrame);
    av_packet_free(&inPacket);
    av_packet_free(&outPacket);
    return 0;
}
