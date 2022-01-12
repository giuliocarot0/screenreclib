//
// Created by giuli on 27/12/2021.
//

#ifndef SCREENRECLIB_SRTOOLS_H
#define SCREENRECLIB_SRTOOLS_H


#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <cstring>
#include <cmath>


#include <cstring>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include "exceptions/SRExceptions.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavcodec/avfft.h>
#include <libavdevice/avdevice.h>
#include <libavfilter/avfilter.h>
#include <libavfilter/buffersink.h>
#include <libavfilter/buffersrc.h>
#include <libavformat/avformat.h>
#include <libavformat/avio.h>

#include "libavutil/opt.h"
#include "libavutil/common.h"
#include "libavutil/channel_layout.h"
#include "libavutil/imgutils.h"
#include "libavutil/mathematics.h"
#include "libavutil/samplefmt.h"
#include "libavutil/time.h"
#include "libavutil/opt.h"
#include "libavutil/pixdesc.h"
#include "libavutil/file.h"
#include "libavutil/audio_fifo.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
}


using namespace  std;

typedef struct S{
    int width;
    int height;
}SRResolution;

typedef struct T{
    int x;
    int y;
}SROffset;

typedef struct A{
    SRResolution  _outscreenres;
    uint16_t  _fps;
    int audio_samplerate;
    int audio_channels;
    char* filename;
    AVCodecID audio_codec;
    AVCodecID video_codec;
}SROutputSettings;

#endif //SCREENRECLIB_SRTOOLS_H
