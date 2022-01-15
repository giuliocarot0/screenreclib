//
// Created by Alex on 12/01/2022.
//

#include <cstdint>
#include <transcoding/SRAudioFilter.h>


int SRAudioFilter::init_fifo(AVCodecContext * outACodecContext)
{
    /* Create the FIFO buffer based on the specified output sample format. */
    if (!(fifo = av_audio_fifo_alloc(outACodecContext->sample_fmt,
                                     outACodecContext->channels, 1))) {
        fprintf(stderr, "Could not allocate FIFO\n");
        return AVERROR(ENOMEM);
    }
    return 0;
}


int SRAudioFilter::add_samples_to_fifo(uint8_t **converted_input_samples, const int frame_size){
    int error;
    /* Make the FIFO as large as it needs to be to hold both,
     * the old and the new samples. */
    if ((error = av_audio_fifo_realloc(fifo, av_audio_fifo_size(fifo) + frame_size)) < 0) {
        fprintf(stderr, "Could not reallocate FIFO\n");
        return error;
    }
    /* Store the new samples in the FIFO buffer. */
    if (av_audio_fifo_write(fifo, (void **)converted_input_samples, frame_size) < frame_size) {
        fprintf(stderr, "Could not write data to FIFO\n");
        return AVERROR_EXIT;
    }
    return 0;
}


int SRAudioFilter::initConvertedSamples(uint8_t ***converted_input_samples,
                                         AVCodecContext *output_codec_context,
                                         int frame_size){
    int error;
    /* Allocate as many pointers as there are audio channels.
     * Each pointer will later point to the audio samples of the corresponding
     * channels (although it may be NULL for interleaved formats).
     */
    if (!(*converted_input_samples = (uint8_t **)calloc(output_codec_context->channels,
                                                        sizeof(**converted_input_samples)))) {
        fprintf(stderr, "Could not allocate converted input sample pointers\n");
        return AVERROR(ENOMEM);
    }
    /* Allocate memory for the samples of all channels in one consecutive
     * block for convenience. */
    if (av_samples_alloc(*converted_input_samples, nullptr,
                         output_codec_context->channels,
                         frame_size,
                         output_codec_context->sample_fmt, 0) < 0) {

        exit(1);
    }
    return 0;
}

AVAudioFifo *SRAudioFilter::getFifo() const {
    return fifo;
}

void SRAudioFilter::init() {
    if(encoder == nullptr || decoder == nullptr) {
        //todo invalid filter parameters
        return;
    }
    scaled_frame = av_frame_alloc();

    resampling_context = swr_alloc_set_opts(resampling_context,
                                           av_get_default_channel_layout(encoder->channels),
                                           encoder->sample_fmt,
                                           encoder->sample_rate,
                                           av_get_default_channel_layout(decoder->channels),
                                           decoder->sample_fmt,
                                           decoder->sample_rate,
                                           0, NULL);

    if (!resampling_context) {
        cout << "\nCannot allocate the resample context";
        exit(1);
    }
    if ((swr_init(resampling_context)) < 0) {
        fprintf(stderr, "Could not open resample context\n");
        swr_free(&resampling_context);
        exit(1);
    }

    init_fifo(encoder);
}

AVFrame *SRAudioFilter::filterFrame(AVFrame *inputFrame) {
    if(scaled_frame == nullptr || encoder == nullptr || decoder == nullptr || resampling_context == nullptr) {
        //todo excepetion uninitialized filter
        return nullptr;
    }

    initConvertedSamples(&resampledData, encoder,
                                     inputFrame->nb_samples);

    swr_convert(resampling_context,
                resampledData, inputFrame->nb_samples,
                (const uint8_t **) inputFrame->extended_data, inputFrame->nb_samples);

    add_samples_to_fifo(resampledData, inputFrame->nb_samples);

    scaled_frame->nb_samples = encoder->frame_size;
    scaled_frame->channel_layout = encoder->channel_layout;
    scaled_frame->format = encoder->sample_fmt;
    scaled_frame->sample_rate = encoder->sample_rate;
    // scaledFrame->best_effort_timestamp = rawFrame->best_effort_timestamp;
    // scaledFrame->pts = rawFrame->pts;
    av_frame_get_buffer(scaled_frame, 0);
}
