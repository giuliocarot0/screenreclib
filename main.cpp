#include <iostream>
#include "SCPPVideoInput.h"
#include "SCPPAudioInput.h"
#include "SCPPDecoder.h"


int main() {
    AVPacket *inPacket;
    AVFrame *rawFrame;

    rawFrame = av_frame_alloc();
    inPacket = av_packet_alloc();
    avdevice_register_all();

    //Open two input devices
    SCPPVideoInput videoInput("avfoundation", "1:none", SRResolution{2560,1600}, SROffset{0,0}, 30 );
    SCPPDecoder videoDecoder;


    videoInput.open();
    videoDecoder.setDecoderContext(videoInput.getCodecContext());

    int flag = 0;
    while(!flag) {


        if(videoInput.readPacket(inPacket) >= 0){
            printf("PacketPTS: %lld\n",inPacket->pts);
            videoDecoder.decodePacket(inPacket);
            while(videoDecoder.getDecodedFrame(rawFrame)>=0){
                printf("\t decodedFrame %lld\n", rawFrame->pts);
            }
            av_packet_unref(inPacket);

        }
        else flag = 1;
    }

    return 0;
}
