cmake_minimum_required(VERSION 3.20)
project(screenreclib)

set(CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

if(WIN32)
    link_directories(C:\\Users\\aless\\Desktop\\ffmpeg-4.4-full_build-shared\\ffmpeg-4.4-full_build-shared\\lib)
    include_directories(C:\\Users\\aless\\Desktop\\ffmpeg-4.4-full_build-shared\\ffmpeg-4.4-full_build-shared\\include)
endif()


if(APPLE)
    link_directories(/usr/local/Cellar/ffmpeg/4.4_2/lib)
    include_directories(/usr/local/Cellar/ffmpeg/4.4_2/include)
endif()

include_directories(include)
add_executable(screenreclib main.cpp lib/SRDecoder.cpp include/transcoding/SRDecoder.h lib/SREncoder.cpp include/transcoding/SREncoder.h lib/SRMediaOutput.cpp include/muxing/SRMediaOutput.h lib/SRVideoInput.cpp include/demuxing/SRVideoInput.h lib/SRAudioInput.cpp include/demuxing/SRAudioInput.h include/SRTools.h lib/SRInput.cpp include/exceptions/SRExceptions.h lib/SRVideoFilter.cpp include/transcoding/SRVideoFilter.h include/transcoding/SRAudioFilter.h lib/SRAudioFilter.cpp)

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


