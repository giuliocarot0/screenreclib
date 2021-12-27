cmake_minimum_required(VERSION 3.20)
project(screenreclib)

set(CMAKE_CXX_STANDARD 14)

add_library(screenreclib src/SCPPVideoTranscoder.cpp include/SCPPVideoTranscoder.h src/SCPPAudioTranscoder.cpp include/SCPPAudioTranscoder.h src/SCPPMediaMuxer.cpp include/SCPPMediaMuxer.h src/SCPPVideoInput.cpp include/SCPPVideoInput.h src/SCPPAudioInput.cpp include/SCPPAudioInput.h src/SCPPRecorder.cpp include/SCPPRecorder.h)
include_directories(/usr/local/Cellar/ffmpeg/4.4_2/include)
link_directories(/usr/local/Cellar/ffmpeg/4.4_2/lib)
target_link_libraries(
        screenreclib
        avformat
        avcodec
        avdevice
        swscale
        avutil
        avfilter
)


