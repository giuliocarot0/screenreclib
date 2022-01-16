//
// Created by Giulio Carota on 17/10/21.
//

#include "muxing/SRMediaOutput.h"

SRMediaOutput::SRMediaOutput(SRSettings outputSettings) {

    settings = {outputSettings};

    //checks
    if(settings.enable_crop &&
            (settings.crop.dimension.width + settings.crop.offset.x > settings.outscreenres.width ||
                    settings.crop.dimension.height + settings.crop.offset.y > settings.outscreenres.height))
        exit(1);
    //todo crop region overflow exception
    //put true if selected codec is not null
    audio_recorded = settings.audio_codec != AV_CODEC_ID_NONE;
    video_recorded = settings.video_codec != AV_CODEC_ID_NONE;

    outputFormat = nullptr;
    outputCtx = nullptr;
    videoCtx = nullptr;
    audioCtx = nullptr;
    videoStreamID= -1;
    audioStreamID= -1;
}

//initialize the output File
int SRMediaOutput::initFile() {
    char* filename = settings.filename;

    int value = 0;

    /*get the filetype from filename extension*/
    outputFormat = av_guess_format(nullptr,filename, nullptr);
    if(!outputFormat) {
        cout << "\nCannot get the video format. try with correct format";
        exit(1); // todo: invalid output format exception
    }

    /*allocate the format context*/
    avformat_alloc_output_context2(&outputCtx, outputFormat, outputFormat->name, filename);
    if (!outputCtx) {
        cout << "\nCannot allocate the output context";
        exit(1); // todo: OutputCtx Allocation Exception
    }

    if(video_recorded) createVideoStream();
    if(audio_recorded) createAudioStream();

    /* create empty video file */
    if (!(outputCtx->flags & AVFMT_NOFILE)) {
        value = avio_open2(&outputCtx->pb, filename, AVIO_FLAG_WRITE, nullptr, nullptr);
        if (value < 0) {
            cout << "\nerror in creating the video file";
            exit(1); // todo: File Opening Exception
        }
    }

    if (!outputCtx->nb_streams) {
        cout << "\noutput file dose not contain any stream";
        exit(1); // todo: NoValidStreamsException
    }


    /* imp: mp4 container or some advanced container file required header information*/
    value = avformat_write_header(outputCtx, nullptr);
    if (value < 0) {
        cout << "\nerror in writing the header context";
        exit(1); //todo: OutputHeaderWriting Exception
    }


    return 0;
}
int SRMediaOutput::createAudioStream() {
    int i;
    AVCodec* outACodec = nullptr;

    AVStream *audio_st = avformat_new_stream(outputCtx, nullptr);
    if (!audio_st) {
        cout << "\nCannot create audio stream";
        exit(1);
    }
    outACodec = avcodec_find_encoder(AV_CODEC_ID_AAC);
    if (!outACodec) {
        cout << "\nCannot find requested encoder";
        exit(1); //todo: Invalid Video Output Codec
    }
    audioCtx = avcodec_alloc_context3(outACodec);
    if (!audioCtx) {
        cout << "\nCannot create related VideoCodecContext";
        exit(1); //todo: AudioContext Allocation Exception
    }

    /* set properties for the video stream encoding*/

    if ((outACodec)->supported_samplerates) {
        audioCtx->sample_rate = (outACodec)->supported_samplerates[0];
        for (i = 0; (outACodec)->supported_samplerates[i]; i++) {
            if ((outACodec)->supported_samplerates[i] == settings.audio_samplerate)
                audioCtx->sample_rate = settings.audio_samplerate;
        }
    }
    audioCtx->codec_id = settings.audio_codec;
    audioCtx->sample_fmt  = (outACodec)->sample_fmts ? (outACodec)->sample_fmts[0] : AV_SAMPLE_FMT_FLTP;
    audioCtx->channels  = settings.audio_channels;
    audioCtx->channel_layout = av_get_default_channel_layout(audioCtx->channels);
    audioCtx->bit_rate = 96000;
    audioCtx->time_base = { 1, audioCtx->sample_rate };

    audioCtx->strict_std_compliance = FF_COMPLIANCE_EXPERIMENTAL;

    if ((outputCtx)->oformat->flags & AVFMT_GLOBALHEADER) {
        audioCtx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    }

    if (avcodec_open2(audioCtx, outACodec, nullptr)< 0) {
        cout << "\nerror in opening the avcodec with error: ";
        exit(1); //todo:CodecOpening Exception
    }


    //find a free stream index
    for(i=0; i < outputCtx->nb_streams; i++)
        if(outputCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_UNKNOWN)
            audioStreamID = i;

    if(audioStreamID < 0) {
        cout << "\nCannot find a free stream for audio on the output";
        exit(1); // todo: no free streams on output exception
    }

    avcodec_parameters_from_context(outputCtx->streams[audioStreamID]->codecpar, audioCtx);
    return 1;
}
int SRMediaOutput::createVideoStream() {
    int i;
    AVStream *video_st = avformat_new_stream(outputCtx, nullptr);

    if (!video_st) {
        cout << "\nCannot create video stream";
        exit(1);
    }
    AVCodec* outVCodec = avcodec_find_encoder(settings.video_codec);
    if (!outVCodec) {
        cout << "\nCannot find requested encoder";
        exit(1); //todo: Invalid Codec  exception
    }
    videoCtx = avcodec_alloc_context3(outVCodec);
    if (!videoCtx) {
        cout << "\nCannot create related VideoCodecContext";
        exit(1); // todo: codec allocation exception
    }

    /* set properties for the video stream encoding */
    videoCtx->codec_id = settings.video_codec;// AV_CODEC_ID_MPEG4; // AV_CODEC_ID_H264 // AV_CODEC_ID_MPEG1VIDEO
    videoCtx->codec_type = AVMEDIA_TYPE_VIDEO;
    videoCtx->pix_fmt = AV_PIX_FMT_YUV420P;
    videoCtx->bit_rate = 400000; //

    videoCtx->width =  (settings.enable_crop) ? settings.crop.dimension.width : settings.outscreenres.width;
    videoCtx->height = (settings.enable_crop) ? settings.crop.dimension.height : settings.outscreenres.height;

    videoCtx->time_base = AVRational{1, settings.fps};
    videoCtx->framerate = AVRational{settings.fps, 1}; // 15fps
    //videoCtx->compression_level = 1;
    /* reduce preset to slow if H264 to avoid resources leak */
    if(videoCtx->codec_id == AV_CODEC_ID_H264)
        av_opt_set(videoCtx->priv_data, "preset", "slow", 0);

    /*setting global headers because some formats require them*/
    if (outputCtx->oformat->flags & AVFMT_GLOBALHEADER) {
        videoCtx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    }

    if (avcodec_open2(videoCtx, outVCodec, nullptr)< 0) {
        cout << "\nerror in opening the avcodec";
        exit(1);//todo: codec opening exception
    }

    //find a free stream index
    videoStreamID = -1;
    for(i=0; i < outputCtx->nb_streams; i++)
        if(outputCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_UNKNOWN)
            videoStreamID = i;

    if(videoStreamID < 0) {
        cout << "\nCannot find a free stream for video on the output";
        exit(1);//No Free Stream available excepetion
    }

    avcodec_parameters_from_context(outputCtx->streams[videoStreamID]->codecpar, videoCtx);
return 1;
}

AVCodecContext *SRMediaOutput::getVideoCodecContext() {
    return videoCtx;
}

AVCodecContext *SRMediaOutput::getAudioCodecContext() {
    return audioCtx;
}

int SRMediaOutput::writePacket(AVPacket *packet, media_type type) {
    int ret;
    if(type == 0) { //video
        packet->stream_index = videoStreamID;
        av_packet_rescale_ts(packet, videoCtx->time_base, outputCtx->streams[videoStreamID]->time_base);
    }
    ret = av_write_frame(outputCtx, packet);
    av_packet_unref(packet);
    return ret;
}

char *SRMediaOutput::getFilename() {
    return settings.filename;
}

SRMediaOutput::~SRMediaOutput() {
    if( av_write_trailer(outputCtx) < 0)
    {
        cout<<"\nerror in writing av trailer";
        exit(1);
    }

    if(video_recorded){
        avcodec_free_context(&videoCtx);
        if (videoCtx){
            cout << "\nunable to free video avformat context";
            exit(1);
        }
    }
    if(audio_recorded){
        avcodec_free_context(&audioCtx);
        if (audioCtx){
            cout << "\nunable to free audio avformat context";
            exit(1);
        }
    }
    avformat_close_input(&outputCtx);
    if (outputCtx){
        cout << "\nunable to free output avformat context";
        exit(1);
    }
}
