//
// Created by giuli on 27/12/2021.
//

#ifndef SCREENRECLIB_SRTOOLS_H
#define SCREENRECLIB_SRTOOLS_H

#define AUTO_RESOLUTION SRResolution{0,0}
#define NULL_RESOLUTION SRResolution{0,0}

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <cstring>
#include <cmath>
#include <ctime>

#include <cstring>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <assert.h>
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
typedef struct R{
    double num;
    double den;
}SRRational;
typedef struct S{
    int width;
    int height;
}SRResolution;

typedef struct M {
    int x;
    int y;
}SROffset;

typedef struct V{
    SRRational width;
    SRRational height;
}SRResolutionRational;

typedef struct Z {
    SRRational x;
    SRRational y;
}SROffsetRational;

typedef struct T{
    SROffsetRational offset;
    SRResolutionRational dimension;
}SRCropRegion;

typedef struct A{
    SRCropRegion crop;
    SRResolution  outscreenres;
    uint16_t  fps;
    int audio_samplerate;
    int audio_channels;
    string filename;
    AVCodecID audio_codec;
    AVCodecID video_codec;
    bool enable_crop;
}SROutputSettings;

typedef struct B{
   bool enable_audio;
   bool enable_video;
   bool enable_crop;
   SRCropRegion crop_info;
   string filename;
   string video_url;
   string audio_url;
}SRConfiguration;

static SRResolution rescale_resolution(SRResolution input, SRResolutionRational rescaler){
    SRResolution  out;
    out.width = floor(input.width* rescaler.width.num / rescaler.width.den);
    if(out.width%2 != 0) out.width += 1;
    out.height = floor(input.height* rescaler.height.num / rescaler.height.den);
    if(out.height%2 != 0) out.height += 1;

    return out;
}

static SROffset rescale_offset(SRResolution input, SROffsetRational rescaler){
    SROffset out;
    out.x = ceil(input.width*rescaler.x.num / rescaler.x.den);
    out.y = ceil(input.height*rescaler.y.num / rescaler.y.den);
    return out;
}

static void lock_thread_for(int sec){
    std::this_thread::sleep_for(std::chrono::milliseconds(sec*1000));
}

static bool assertMP4(string filename){
    filename = filename.substr(filename.find_last_of('.'));
    if(filename!=".mp4")
        return false;
    else
        return true;
}

#endif //SCREENRECLIB_SRTOOLS_H

