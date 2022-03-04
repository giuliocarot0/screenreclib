//
// Created by Giulio Carota on 17/10/21.
//

#include "muxing/SRMediaOutput.h"
SRMediaOutput::SRMediaOutput() {
    outputFormat = nullptr;
    outputCtx = nullptr;
    videoCtx = nullptr;
    audioCtx = nullptr;
    videoStreamID= -1;
    audioStreamID= -1;
}

/**
 * The set method allows to configure the default SRMediaOutput device by
 * setting the following parameters
 *
 * @param o_filename indicates the output filename
 * @param o_settings indicates the settings set by the user
 */
void SRMediaOutput::set(string o_filename, SROutputSettings o_settings) {
    this->settings = {o_settings};
    this->filename = o_filename;

    //checks
    /*if(settings.enable_crop &&
            (settings.crop.dimension.width + settings.crop.offset.x > settings.outscreenres.width ||
                    settings.crop.dimension.height + settings.crop.offset.y > settings.outscreenres.height))*/

    //put true if selected codec is not null
    audio_recorded = settings.audio_codec != AV_CODEC_ID_NONE;
    video_recorded = settings.video_codec != AV_CODEC_ID_NONE;

}

/**
 * The method initializes the output file
 *
 * @throws OutputFormatException if the output format cannot be guessed from the filename
 * @throws OutputContextAllocationException if the allocation of the output context fails
 * @throws FileOpeningException if the creation and opening of the output file fails
 * @throws NoValidStreamsException if the output file doesn't contain any stream
 * @throws OutputHeaderWritingException if an error occurs while writing the headers of the file
 */
int SRMediaOutput::initFile() {
    const char* filename = settings.filename.c_str();

    int value = 0;

    /*get the filetype from filename extension*/
    outputFormat = av_guess_format(nullptr,filename, nullptr);
    if(!outputFormat) {
        throw OutputFormatException("Cannot get the video format. try with correct format");
    }

    /*allocate the format context*/
    avformat_alloc_output_context2(&outputCtx, outputFormat, outputFormat->name, filename);
    if (!outputCtx) {
        throw OutputContextAllocationException("Cannot allocate the output context");
    }

    if(video_recorded) createVideoStream();
    if(audio_recorded) createAudioStream();

    /* create empty video file */
    if (!(outputCtx->flags & AVFMT_NOFILE)) {
        value = avio_open2(&outputCtx->pb, filename, AVIO_FLAG_WRITE, nullptr, nullptr);
        if (value < 0) {
            throw FileOpeningException("Error in creating the video file");
        }
    }

    if (!outputCtx->nb_streams) {
        throw NoValidStreamsException("Output file dose not contain any stream");
    }


    /* imp: mp4 container or some advanced container file required header information*/
    value = avformat_write_header(outputCtx, nullptr);
    if (value < 0) {
        throw OutputHeaderWritingException("Error in writing the header context");
    }


    return 0;
}

/**
 * The method creates the Audio output context setting all the proper parameters based on the settings configured with the set method
 *
 * @throws StreamException if the audio stream cannot be created
 * @throws FindEncoderException if the encoder selected cannot be found
 * @throws AVCodecAllocationException if the allocation of the Codec Context fails
 * @throws CodecOpeningException if the opening of the codec context fails
 * @throws NoFreeStreamException if a free stream for audio on the output cannot be found
 *
 */
int SRMediaOutput::createAudioStream() {
    int i;
    AVCodec* outACodec = nullptr;

    AVStream *audio_st = avformat_new_stream(outputCtx, nullptr);
    if (!audio_st) {
        throw StreamException("Cannot create audio stream");
    }
    outACodec = avcodec_find_encoder(settings.audio_codec);
    if (!outACodec) {
        throw FindEncoderException("Cannot find requested audio encoder");
    }
    audioCtx = avcodec_alloc_context3(outACodec);
    if (!audioCtx) {
        throw AVCodecAllocationException("Cannot create related AudioCodecContext");
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
        throw CodecOpeningException("Error in opening the avcodec");
    }

    //find a free stream index
    for(i=0; i < outputCtx->nb_streams; i++)
        if(outputCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_UNKNOWN)
            audioStreamID = i;

    if(audioStreamID < 0) {
        throw NoFreeStreamException("Cannot find a free stream for audio on the output");
    }

    avcodec_parameters_from_context(outputCtx->streams[audioStreamID]->codecpar, audioCtx);
    return 1;
}

/**
 * The method creates the Video output context setting all the proper parameters based on the settings configured with the set method
 *
 * @throws StreamException if the video stream cannot be created
 * @throws FindEncoderException if the encoder selected cannot be found
 * @throws AVCodecAllocationException if the allocation of the Codec Context fails
 * @throws CodecOpeningException if the opening of the codec context fails
 * @throws NoFreeStreamException if a free stream for audio on the output cannot be found
 */
int SRMediaOutput::createVideoStream() {
    int i;
    AVStream *video_st = avformat_new_stream(outputCtx, nullptr);

    if (!video_st) {
        throw StreamException("Cannot create video stream");
    }
    AVCodec* outVCodec = avcodec_find_encoder(settings.video_codec);
    if (!outVCodec) {
        throw FindEncoderException("Cannot find requested video encoder");
    }
    videoCtx = avcodec_alloc_context3(outVCodec);
    if (!videoCtx) {
        throw AVCodecAllocationException("Cannot create related VideoCodecContext");
    }

    /* set properties for the video stream encoding */
    videoCtx->codec_id = settings.video_codec;// AV_CODEC_ID_MPEG4; // AV_CODEC_ID_H264 // AV_CODEC_ID_MPEG1VIDEO
    videoCtx->codec_type = AVMEDIA_TYPE_VIDEO;
    videoCtx->pix_fmt = AV_PIX_FMT_YUV420P;
    videoCtx->bit_rate = 400000; //

   if(settings.enable_crop) {
       SRResolution out = rescale_resolution(settings.outscreenres, settings.crop.dimension);
       videoCtx->width = out.width;
       videoCtx->height = out.height;
   }else{
       videoCtx->width = settings.outscreenres.width;
       videoCtx->height = settings.outscreenres.height;
   }
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
        throw CodecOpeningException("Error in opening the avcodec");
    }

    //find a free stream index
    videoStreamID = -1;
    for(i=0; i < outputCtx->nb_streams; i++)
        if(outputCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_UNKNOWN)
            videoStreamID = i;

    if(videoStreamID < 0) {
        throw NoFreeStreamException("Cannot find a free stream for video on the output");
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

/**
 * The method writes a packet to an output media file after doing a rescale on the packet depending on the output context
 *
 * @param packet indicates the packet that is going to be written
 * @param type indicates the type of frame to be written (audio or video)
 */
int SRMediaOutput::writePacket(AVPacket *packet, media_type type) {
    int ret;
    std::unique_lock w_lock(w_mutex);
    if(type == 0) { //video
        packet->stream_index = videoStreamID;
        av_packet_rescale_ts(packet, videoCtx->time_base, outputCtx->streams[videoStreamID]->time_base);
    }
    if(type == 1) { //audio
        packet->stream_index = audioStreamID;
        av_packet_rescale_ts(packet, audioCtx->time_base, outputCtx->streams[audioStreamID]->time_base);
    }
    ret = av_write_frame(outputCtx, packet);
    av_packet_unref(packet);
    return ret;

}

string SRMediaOutput::getFilename() {
    return settings.filename;
}

/**
 * SRMediaOutput destructor.
 * The destructor frees all the pointers contained in the object
 */
SRMediaOutput::~SRMediaOutput() {
    if(outputCtx){
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
}

