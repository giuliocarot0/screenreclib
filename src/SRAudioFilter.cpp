//
// Created by Alex on 12/01/2022.
//

#include <cstdint>
#include <transcoding/SRAudioFilter.h>


/**
 * The method initializes the fifo buffer used for audio transcoding
 */
int SRAudioFilter::init_fifo()
{
    /* Create the FIFO buffer based on the specified output sample format. */
    if (!(fifo = av_audio_fifo_alloc(encoder->sample_fmt,
                                     encoder->channels, 1))) {
        fprintf(stderr, "Could not allocate FIFO\n");
        return AVERROR(ENOMEM);
    }
    return 0;
}

/**
 * The method adds data to the fifo buffer and reallocate the buffer in case of necessity
 *
 * @param frame_size indicates the number of samples added to the buffer
 */
int SRAudioFilter::add_samples_to_fifo(const int frame_size){
    int error;
    /* Make the FIFO as large as it needs to be to hold both,
     * the old and the new samples. */
    if ((error = av_audio_fifo_realloc(fifo, av_audio_fifo_size(fifo) + frame_size)) < 0) {
        fprintf(stderr, "Could not reallocate FIFO\n");
        return error;
    }
    /* Store the new samples in the FIFO buffer. */
    if (av_audio_fifo_write(fifo, (void **)resampledData, frame_size) < frame_size) {
        fprintf(stderr, "Could not write data to FIFO\n");
        return AVERROR_EXIT;
    }
    return 0;
}

/**
 * The method allocates as many pointers as there are audio channels.
 * Each pointer will later point to the audio samples of the corresponding
 * channels (although it may be NULL for interleaved formats).
 *
 * It also allocates memory for the samples of all channels in one consecutive
 * block for convenience.
 *
 * @param frame_size indicates the number of samples added to the buffer
 */
int SRAudioFilter::initConvertedSamples(int frame_size){
    if (!(resampledData = (uint8_t **)calloc(encoder->channels,
                                                        sizeof(resampledData)))) {
        fprintf(stderr, "Could not allocate converted input sample pointers\n");
        return AVERROR(ENOMEM);
    }
    /* Allocate memory for the samples of all channels in one consecutive
     * block for convenience. */
    if (av_samples_alloc(resampledData, nullptr,
                         encoder->channels,
                         frame_size,
                         encoder->sample_fmt, 0) < 0) {

        exit(1);
    }
    return 0;
}

AVAudioFifo *SRAudioFilter::getFifo() const {
    return fifo;
}

/**
 * The method initializes the AudioFilter allocating the resampling context and the frame used for filtering
 *
 */
void SRAudioFilter::init() {
    if(encoder == nullptr || decoder == nullptr) {
        throw InvalidFilterParametersException("Found wrong parameters while enabling the audio filter");
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

    init_fifo();
}

/**
 * The method applies the resampling to the frame and set all the parameters needed in the frame before the encoding of the frame
 *
 * @param inputFrame is the frame that needs to be filtered
 */
AVFrame *SRAudioFilter::filterFrame(AVFrame *inputFrame) {
    if(scaled_frame == nullptr || encoder == nullptr || decoder == nullptr || resampling_context == nullptr) {
        throw UninitializedFilterException("Audio filter not initialized");
    }

    initConvertedSamples(inputFrame->nb_samples);

    swr_convert(resampling_context,
                resampledData, inputFrame->nb_samples,
                (const uint8_t **) inputFrame->extended_data, inputFrame->nb_samples);

    add_samples_to_fifo(inputFrame->nb_samples);

    scaled_frame->nb_samples = encoder->frame_size;
    scaled_frame->channel_layout = encoder->channel_layout;
    scaled_frame->format = encoder->sample_fmt;
    scaled_frame->sample_rate = encoder->sample_rate;
    av_frame_get_buffer(scaled_frame, 0);
    return scaled_frame;
}

/**
 * The set method allows to configure the default SRAudioFilter device by
 * setting the following parameters
 *
 * @param v_encoder indicates the encoder
 * @param v_decoder indicates the decoder
 */
void SRAudioFilter::set(AVCodecContext *v_encoder, AVCodecContext *v_decoder) {
    encoder=v_encoder;
    decoder=v_decoder;
}


/**
 * SRAudioFilter destructor.
 * The destructor frees all the pointers contained in the object
 */
SRAudioFilter::~SRAudioFilter() {
    swr_free(&resampling_context);
    av_frame_free(&scaled_frame);
    av_audio_fifo_free(fifo);
    free(resampledData);
}
