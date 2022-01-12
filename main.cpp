#include <iostream>
#include <transcoding/SREncoder.h>
#include <muxing/SRMediaOutput.h>
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
    SROutputSettings outputSettings;
    outputSettings.video_codec = AV_CODEC_ID_MPEG4;
    outputSettings.audio_codec = AV_CODEC_ID_NONE;

    outputSettings._fps = 30;
    outputSettings.filename = "testfile.mp4";
    outputSettings._outscreenres =SRResolution{2560,1600};

    SRMediaOutput outputFile(outputSettings);
    SRVideoInput videoInput("avfoundation", "1:none", outputSettings._outscreenres, SROffset{0,0}, outputSettings._fps );
    SRDecoder videoDecoder;
    SREncoder videoEncoder;

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
    ret = av_image_fill_arrays( scaled_frame->data, scaled_frame->linesize, video_outbuf , AV_PIX_FMT_YUV420P, outputFile.getVideoCodecContext()->width,outputFile.getVideoCodecContext()->height,1 ); // returns : the size in bytes required for lib
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

    long long int last = 0;
    while(last/30 < 10 /*record for five sec*/) {


        if(videoInput.readPacket(inPacket) >= 0){
            last = inPacket->pts;
            videoDecoder.decodePacket(inPacket);
            while(videoDecoder.getDecodedFrame(rawFrame)>=0){
              //  printf("\t decodedFrame %lld\n", rawFrame->pts);


                /*initializing scaleFrame */
                scaled_frame->width = outputFile.getVideoCodecContext()->width;
                scaled_frame->height = outputFile.getVideoCodecContext()->height;
                scaled_frame->format = outputFile.getVideoCodecContext()->pix_fmt;
                scaled_frame->pts = rawFrame->best_effort_timestamp;
                scaled_frame->pkt_dts=rawFrame->pkt_dts;
                scaled_frame->pkt_duration = rawFrame->pkt_duration;
                sws_scale(swsCtx_, rawFrame->data, rawFrame->linesize,0, videoInput.getCodecContext()->height, scaled_frame->data, scaled_frame->linesize);


                if(videoEncoder.encodeFrame(scaled_frame)<0){
                    printf("DROPPED");
                };
                while(videoEncoder.getEncodedPacket(outPacket)>=0) {
                //    printf("\t\t encodedPacket %lld\n", outPacket->pts);

                    if(outputFile.writePacket(outPacket, video /*passing a video packet*/)>=0){

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
    }

    return 0;
}
