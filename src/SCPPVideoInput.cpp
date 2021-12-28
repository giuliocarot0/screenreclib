//
// Created by Giulio Carota on 17/10/21.
//

#include "SCPPVideoInput.h"


/**
 * the constructor initialize the
 * input device with requested options
 */
SCPPVideoInput::SCPPVideoInput(char *video_src, char *video_url, SRResolution res, SROffset off, int fps) : SCPPInput(video_src, video_url) {

    char s[30];
    int value = 0;
    sprintf(s,"%", fps);

    value = av_dict_set(&options, "framerate", s, 0);
    if (value < 0) {
        cout << "\nerror in setting dictionary value";
        exit(1);
    }
    s[0] = '\0';
    sprintf(s,"%dx%d", res.width, res.height);

    value = av_dict_set(&options, "video_size", s, 0);
    if (value < 0) {
        cout << "\nerror in setting dictionary value";
        exit(1);
    }
    value = av_dict_set(&options, "preset", "medium", 0);
    if (value < 0) {
        cout << "\nerror in setting preset values";
        exit(1);
    }

    value = av_dict_set(&options, "probesize", "60M", 0);
    if (value < 0) {
        cout << "\nerror in setting preset values";
        exit(1);
    }

}

AVFormatContext* SCPPVideoInput::open(){
    //if one of them != nullptr then input already initialized
    if(inFormatContext != nullptr || inCodecContext!= nullptr || streamIndex != -1)
        return inFormatContext;

    AVInputFormat* inVInputFormat =nullptr;
    int value = 0;

    inFormatContext = avformat_alloc_context();

    //get input format
    inVInputFormat = av_find_input_format(device_src);
    value = avformat_open_input(&inFormatContext, device_url, inVInputFormat, &options);
    if (value != 0) {
        cout << "\nCannot open selected device";
        exit(1);
    }


    //get video stream infos from context
    value = avformat_find_stream_info(inFormatContext, nullptr);
    if (value < 0) {
        cout << "\nCannot find the stream information";
        exit(1);
    }

    //find the first video stream with a given code
    for (int i = 0; i < inFormatContext->nb_streams; i++){
        if (inFormatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            streamIndex = i;
            break;
        }
    }

    if (streamIndex == -1) {
        cout << "\nCannot find the video stream index. (-1)";
        exit(1);
    }

    AVCodecParameters *params = inFormatContext->streams[streamIndex]->codecpar;
    AVCodec *inVCodec = avcodec_find_decoder(params->codec_id);
    if (inVCodec == nullptr) {
        cout << "\nCannot find the decoder";
        exit(1);
    }

    inCodecContext = avcodec_alloc_context3(inVCodec);
    avcodec_parameters_to_context(inCodecContext, params);

    value = avcodec_open2(inCodecContext, inVCodec, nullptr);
    if (value < 0) {
        cout << "\nCannot open the av codec";
        exit(1);
    }

    return inFormatContext;
}


