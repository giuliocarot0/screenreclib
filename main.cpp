#include <iostream>
#include <SCPPEncoder.h>
#include <SCPPMediaOutput.h>
#include "SCPPVideoInput.h"
#include "SCPPAudioInput.h"
#include "SCPPDecoder.h"
#include <ctime>


int main() {
    AVPacket *inPacket, *outPacket;
    AVFrame *rawFrame, *scaled_frame;
    scaled_frame = av_frame_alloc();
    rawFrame = av_frame_alloc();
    inPacket = av_packet_alloc();
    outPacket = av_packet_alloc();
    avdevice_register_all();

    //Open two input devices
    SCPPVideoInput videoInput("gdigrab", "desktop", SRResolution{1366,768}, SROffset{0,0}, 15 );
    SCPPDecoder videoDecoder;
    SCPPEncoder videoEncoder;

   //set output file for video only
    SROutputSettings outputSettings;
    outputSettings.video_codec = AV_CODEC_ID_MPEG4;
    outputSettings.audio_codec = AV_CODEC_ID_NONE;

    outputSettings._fps = 30;
    outputSettings.filename = "testfile.mp4";
    outputSettings._outscreenres = SRResolution{1366,768};
    SCPPMediaOutput outputFile(outputSettings);


    videoInput.open();
    videoDecoder.setDecoderContext(videoInput.getCodecContext());
    outputFile.initFile();
    videoEncoder.setEncoderContext(outputFile.getVideoCodecContext());


    //this block provides data dtructures for video frames rescaling...
    int video_outbuf_size;
    int nbytes = av_image_get_buffer_size(outputFile.getVideoCodecContext()->pix_fmt,outputFile.getVideoCodecContext()->width,outputFile.getVideoCodecContext()->height,32);
    uint8_t *video_outbuf = (uint8_t*)av_malloc(nbytes*sizeof (uint8_t));
    if( video_outbuf == nullptr )
    {
        cout<<"\nunable to allocate memory";
        exit(1);
    }
    int ret;
    // Setup the data pointers and linesizes based on the specified image parameters and the provided array.
    ret = av_image_fill_arrays( scaled_frame->data, scaled_frame->linesize, video_outbuf , AV_PIX_FMT_YUV420P, outputFile.getVideoCodecContext()->width,outputFile.getVideoCodecContext()->height,1 ); // returns : the size in bytes required for src
    if(ret < 0)
    {
        cout<<"\nerror in filling image array";
    }
    SwsContext* swsCtx_ ;
    // Allocate and return swsContext.
    // a pointer to an allocated context, or NULL in case of error
    // Deprecated : Use sws_getCachedContext() instead.
    swsCtx_ = sws_getContext(videoInput.getCodecContext()->width,
                             videoInput.getCodecContext()->height,
                             videoInput.getCodecContext()->pix_fmt,
                             outputFile.getVideoCodecContext()->width,
                             outputFile.getVideoCodecContext()->height,
                             outputFile.getVideoCodecContext()->pix_fmt,
                             SWS_BICUBIC, NULL, NULL, NULL);

    std::clock_t start;
    start = std::clock();

    int flag = 0;
    while((( std::clock() - start ) / (double) CLOCKS_PER_SEC) < 5) {


        if(videoInput.readPacket(inPacket) >= 0){
            printf("PacketPTS: %lld\n",inPacket->pts);
            videoDecoder.decodePacket(inPacket);
            printf("Packet duration: %lld\n",inPacket->duration);
            while(videoDecoder.getDecodedFrame(rawFrame)>=0){
                printf("\t decodedFrame %lld,%lld\n", rawFrame->pts,rawFrame->pkt_duration);


                /*initializing scaleFrame */
                scaled_frame->width = outputFile.getVideoCodecContext()->width;
                scaled_frame->height = outputFile.getVideoCodecContext()->height;
                scaled_frame->format = outputFile.getVideoCodecContext()->pix_fmt;
                scaled_frame->pts = rawFrame->pts;
                scaled_frame->pkt_dts=rawFrame->pkt_dts;
                scaled_frame->pkt_duration=rawFrame->pkt_duration;
                scaled_frame->best_effort_timestamp = rawFrame->best_effort_timestamp;
                //av_frame_get_buffer(scaled_frame, 0);

                sws_scale(swsCtx_, rawFrame->data, rawFrame->linesize,0, videoInput.getCodecContext()->height, scaled_frame->data, scaled_frame->linesize);

                printf("duration: %lld\n", scaled_frame->pkt_duration);

                videoEncoder.encodeFrame(scaled_frame);
                while(videoEncoder.getEncodedPacket(outPacket)>=0) {
                    printf("\t\t encodedPacket %lld,%lld\n", outPacket->pts, outPacket->duration);

                    if(outputFile.writePacket(outPacket)>=0)
                        printf("transcoded packet written on %s", outputFile.getFilename());
                    }
            }
            av_packet_unref(inPacket);

        }
        else flag = 1;
    }

    return 0;
}
