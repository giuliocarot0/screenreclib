//
// Created by Giulio Carota on 17/10/21.
//

#include "demuxing/SRAudioInput.h"

/**
 * The method opens the device, defines a codec context for decoding the audio stream and starts
 * putting data in the device buffer.
 *
 * @override
 * @return the device context
 */
AVFormatContext* SRAudioInput::open(){
    //if one of them != nullptr then input already initialized
    if(inFormatContext != nullptr || inCodecContext!= nullptr || streamIndex != -1)
        return inFormatContext;

    AVInputFormat* inAInputFormat =nullptr;
    int value = 0;

    inFormatContext = avformat_alloc_context();

    //get input format
    inAInputFormat = av_find_input_format(device_src);
    value = avformat_open_input(&inFormatContext, device_url, inAInputFormat, &options);
    if (value != 0) {
        throw SRDeviceException("Cannot open selected audio device");

    }


    //get audio stream infos from context
    value = avformat_find_stream_info(inFormatContext, nullptr);
    if (value < 0) {
        throw SRStreamInformationException("Cannot find the stream information");
    }

    //find the first audio stream with a given code
    streamIndex = -1;
    for (int i = 0; i < inFormatContext->nb_streams; i++){
        if (inFormatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            streamIndex = i;
            break;
        }
    }

    if (streamIndex == -1) {
        throw SRStreamIndexException("Cannot find the video stream index.");

    }

    AVCodecParameters *params = inFormatContext->streams[streamIndex]->codecpar;
    AVCodec *inACodec = avcodec_find_decoder(params->codec_id);
    if (inACodec == nullptr) {
        throw findDecoderException("Cannot find the decoder.");

    }

    inCodecContext = avcodec_alloc_context3(inACodec);
    avcodec_parameters_to_context(inCodecContext, params);

    value = avcodec_open2(inCodecContext, inACodec, nullptr);
    if (value < 0) {
        throw openAVCodecException("Cannot open the av codec.");

    }

    return inFormatContext;
}

/**
 * The set method allows to configure the default SRAudioInput device by
 * setting the following parameters
 *
 * @param audio_src indicates the source device
 * @param audio_url indicates the device internal path
 */
void SRAudioInput::set(char *audio_src, char *audio_url) {
    device_url = audio_url;
    device_src = audio_src;
}

