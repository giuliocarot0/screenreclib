cmake_minimum_required(VERSION 3.20)
project(SimpleMain)

set(CMAKE_CXX_STANDARD 17)

if(WIN32)
    link_directories(C:\\Users\\aless\\Desktop\\ffmpeg-4.4-full_build-shared\\ffmpeg-4.4-full_build-shared\\lib)
    include_directories(C:\\Users\\aless\\Desktop\\ffmpeg-4.4-full_build-shared\\ffmpeg-4.4-full_build-shared\\include)
endif()


if(APPLE)
    link_directories(/usr/local/Cellar/ffmpeg/4.4.1_5/lib)
    include_directories(/usr/local/Cellar/ffmpeg/4.4.1_5/include)
endif()

include_directories(include)
add_executable(SimpleMain
        main.cpp
        ../../lib/SRDecoder.cpp
        ../../include/transcoding/SRDecoder.h
        ../../lib/SREncoder.cpp
        ../../include/transcoding/SREncoder.h
        ../../lib/SRMediaOutput.cpp
        ../../include/muxing/SRMediaOutput.h
        ../../lib/SRVideoInput.cpp
        ../../include/demuxing/SRVideoInput.h
        ../../lib/SRAudioInput.cpp
        ../../include/demuxing/SRAudioInput.h
        ../../include/SRTools.h
        ../../lib/SRInput.cpp
        ../../include/exceptions/SRExceptions.h
        ../../lib/SRVideoFilter.cpp
        ../../include/transcoding/SRVideoFilter.h
        ../../include/transcoding/SRAudioFilter.h
        ../../lib/SRAudioFilter.cpp
        ../../lib/SRRecorder.cpp
        ../../include/SRRecorder.h)

target_link_libraries(
        SimpleMain
        avformat
        avutil
        avdevice
        avfilter
        avcodec
        swscale
        #pthread
        swresample
)




