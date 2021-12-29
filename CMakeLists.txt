cmake_minimum_required(VERSION 3.20)
project(screenreclib)

set(CMAKE_CXX_STANDARD 14)

add_library(screenreclib src/SCPPDecoder.cpp include/SCPPDecoder.h src/SCPPEncoder.cpp include/SCPPEncoder.h src/SCPPMediaMuxer.cpp include/SCPPMediaMuxer.h src/SCPPVideoInput.cpp include/SCPPVideoInput.h src/SCPPAudioInput.cpp include/SCPPAudioInput.h src/SCPPRecorder.cpp include/SCPPRecorder.h include/SCPPtools.h src/SCPPinput.cpp)
include_directories(include)
link_directories(lib)

target_link_libraries(
        screenreclib
        avformat
        avutil
        avdevice
        avfilter
        avcodec
        swscale
        pthread
        swresample
)


