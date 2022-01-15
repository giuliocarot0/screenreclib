#include <iostream>
#include <transcoding/SREncoder.h>
#include <muxing/SRMediaOutput.h>
#include <transcoding/SRVideoFilter.h>
#include <transcoding/SRAudioFilter.h>
#include "demuxing/SRVideoInput.h"
#include "demuxing/SRAudioInput.h"
#include "transcoding/SRDecoder.h"
#include "SRRecorder.h"

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
    /*SROutputSettings outputSettings;
    outputSettings.video_codec = AV_CODEC_ID_MPEG4;
    outputSettings.audio_codec = AV_CODEC_ID_NONE;

    outputSettings.fps = 30;
    outputSettings.filename = "testfile.mp4";
    outputSettings.outscreenres =SRResolution{1360,768};

    SRMediaOutput outputFile(outputSettings);
    SRVideoInput videoInput("gdigrab", "desktop", outputSettings.outscreenres, SROffset{0,0}, outputSettings.fps );
    SRDecoder videoDecoder;
    SREncoder videoEncoder;

    videoInput.open();
    videoDecoder.setDecoderContext(videoInput.getCodecContext());
    outputFile.initFile();
    videoEncoder.setEncoderContext(outputFile.getVideoCodecContext());

    SRVideoFilter videoFilter(outputFile.getVideoCodecContext(), videoInput.getCodecContext());
    videoFilter.init();

    long long int last = 0;
    printf("[SRlib - recording screen]\n");
    while(last/30 < 10 /*record for five sec*//*) {


        if(videoInput.readPacket(inPacket) >= 0){
            last = inPacket->pts;
            videoDecoder.decodePacket(inPacket);
            while(videoDecoder.getDecodedFrame(rawFrame)>=0){
              //  printf("\t decodedFrame %lld\n", rawFrame->pts);

                scaled_frame = videoFilter.filterFrame(rawFrame);
                if(videoEncoder.encodeFrame(scaled_frame)<0){
                    printf("DROPPED");
                };
                while(videoEncoder.getEncodedPacket(outPacket)>=0) {
                //    printf("\t\t encodedPacket %lld\n", outPacket->pts);

                    if(outputFile.writePacket(outPacket, video /*passing a video packet*//*)>=0){

                        // printf("PTS: %lld - Duration %lld\n", outPacket->pts, outPacket->duration);
                    }
                        //      printf("transcoded packet written on %s", outputFile.getFilename());
                    else{
                        printf("DROPPED");
                    }
                }

            }
            av_packet_unref(inPacket);

        }
    }*/

    try {
        //set output file for audio only
        SRSettings outputSettings;
        outputSettings.video_codec = AV_CODEC_ID_NONE;
        outputSettings.audio_codec = AV_CODEC_ID_AAC;

        outputSettings.filename = "testfile.mp4";


        SRAudioInput audioInput("dshow", "audio=Microfono (Realtek High Definition Audio)");
        SRDecoder audioDecoder;
        SREncoder audioEncoder;
        static int64_t pts = 0;


        audioInput.open();
        audioDecoder.setDecoderContext(audioInput.getCodecContext());

        outputSettings.audio_channels=audioInput.getCodecContext()->channels;
        outputSettings.audio_samplerate=audioInput.getCodecContext()->sample_rate;
        SRMediaOutput outputFile(outputSettings);
        outputFile.initFile();
        audioEncoder.setEncoderContext(outputFile.getAudioCodecContext());



        SRAudioFilter audioFilter(outputFile.getAudioCodecContext(), audioInput.getCodecContext());
        audioFilter.init();


        long long int last = 0;
        int secondi = 5;
        int i=0;
        printf("[SRlib - recording screen]\n");
        while (last/22000 < secondi*2-1/*record for five sec*/) {

            printf("%lld %d", last/22000, i++);
            if (audioInput.readPacket(inPacket) >= 0) {
                printf("\t PacketPTS %lld\n", inPacket->pts);
                last = inPacket->pts;
                audioDecoder.decodePacket(inPacket);
                while (audioDecoder.getDecodedFrame(rawFrame) >= 0) {
                      printf("\t decodedFrame %lld\n", rawFrame->pts);

                    scaled_frame = audioFilter.filterFrame(rawFrame);

                    while (av_audio_fifo_size(audioFilter.getFifo()) >= audioEncoder.getEncoderContext()->frame_size) {
                        av_audio_fifo_read(audioFilter.getFifo(), (void **) (scaled_frame->data),
                                                 audioEncoder.getEncoderContext()->frame_size);
                        scaled_frame->pts = pts;
                        pts += scaled_frame->nb_samples;
                        if (audioEncoder.encodeFrame(scaled_frame) < 0) {
                            printf("DROPPED");
                        };
                        while (audioEncoder.getEncodedPacket(outPacket) >= 0) {
                                printf("\t\t encodedPacket %lld\n", outPacket->pts);

                            if (outputFile.writePacket(outPacket, audio /*passing a audio packet*/) >= 0) {

                                // printf("PTS: %lld - Duration %lld\n", outPacket->pts, outPacket->duration);
                            }
                                //      printf("transcoded packet written on %s", outputFile.getFilename());
                            else {
                                printf("DROPPED");
                            }
                        }
                    }

                }
                av_packet_unref(inPacket);

            }
        }

        return 0;
    }
    catch (exception &e){
        cout << e.what() << endl;
    }
}
