cmake_minimum_required(VERSION 3.20)
project(screenreclib)

set(CMAKE_CXX_STANDARD 14)

if(WIN32)
    link_directories(C:\\Users\\aless\\Desktop\\ffmpeg-4.4-full_build-shared\\ffmpeg-4.4-full_build-shared\\lib)
    include_directories(C:\\Users\\aless\\Desktop\\ffmpeg-4.4-full_build-shared\\ffmpeg-4.4-full_build-shared\\include)
endif()


if(APPLE)
    link_directories(/usr/local/Cellar/ffmpeg/4.4_2/lib)
    include_directories(/usr/local/Cellar/ffmpeg/4.4_2/include)
endif()

include_directories(include)
add_executable(screenreclib main.cpp src/SCPPDecoder.cpp include/SCPPDecoder.h src/SCPPEncoder.cpp include/SCPPEncoder.h src/SCPPMediaOutput.cpp include/SCPPMediaOutput.h src/SCPPVideoInput.cpp include/SCPPVideoInput.h src/SCPPAudioInput.cpp include/SCPPAudioInput.h include/SCPPtools.h src/SCPPinput.cpp include/exceptions/exceptions.h)

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


