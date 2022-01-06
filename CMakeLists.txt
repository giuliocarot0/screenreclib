cmake_minimum_required(VERSION 3.20)
project(screenreclib)

set(CMAKE_CXX_STANDARD 14)

link_directories(lib)
include_directories(include)
include_directories(exceptions)
add_executable(screenreclib main.cpp src/SCPPDecoder.cpp include/SCPPDecoder.h src/SCPPEncoder.cpp include/SCPPEncoder.h src/SCPPMediaMuxer.cpp include/SCPPMediaMuxer.h src/SCPPVideoInput.cpp include/SCPPVideoInput.h src/SCPPAudioInput.cpp include/SCPPAudioInput.h src/SCPPRecorder.cpp include/SCPPRecorder.h include/SCPPtools.h src/SCPPinput.cpp exceptions/exceptions.h)

target_link_libraries(
        screenreclib
        avformat
        avutil
        avdevice
        avfilter
        avcodec
        swscale
        #pthread
        swresample
)


