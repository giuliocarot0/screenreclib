cmake_minimum_required(VERSION 3.20)
project(screenreclib)

set(CMAKE_CXX_STANDARD 14)

if(WIN32)
    link_directories(/usr/local/Cellar/ffmpeg/4.4_2/lib)
    include_directories(/usr/local/Cellar/ffmpeg/4.4_2/include)
endif()


if(APPLE)
    link_directories(/usr/local/Cellar/ffmpeg/4.4_2/lib)
    include_directories(/usr/local/Cellar/ffmpeg/4.4_2/include)
endif()

include_directories(include)
add_executable(screenreclib main.cpp lib/SRDecoder.cpp include/transcoding/SRDecoder.h lib/SREncoder.cpp include/transcoding/SREncoder.h lib/SRMediaOutput.cpp include/muxing/SRMediaOutput.h lib/SRVideoInput.cpp include/demuxing/SRVideoInput.h lib/SRAudioInput.cpp include/demuxing/SRAudioInput.h include/SRTools.h lib/SRInput.cpp include/exceptions/SRExceptions.h lib/SRVideoFilter.cpp include/transcoding/SRVideoFilter.h)

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


