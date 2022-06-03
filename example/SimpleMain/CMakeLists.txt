cmake_minimum_required(VERSION 3.20)
project(SimpleMain)

set(CMAKE_CXX_STANDARD 17)

if(WIN32)
    set(PATH_TO_FFMPEG C:\\Users\\aless\\Desktop\\ffmpeg-4.4.1-full_build-shared)

    link_directories(${PATH_TO_FFMPEG}\\lib)
    include_directories(${PATH_TO_FFMPEG}\\include)
endif()


if(APPLE)
    link_directories(/usr/local/Cellar/ffmpeg/4.4.1_5/lib)
    include_directories(/usr/local/Cellar/ffmpeg/4.4.1_5/include)
endif()

include_directories(../../include)
add_executable(SimpleMain
        main.cpp
        ../../src/SRDecoder.cpp
        ../../include/transcoding/SRDecoder.h
        ../../src/SREncoder.cpp
        ../../include/transcoding/SREncoder.h
        ../../src/SRMediaOutput.cpp
        ../../include/muxing/SRMediaOutput.h
        ../../src/SRVideoInput.cpp
        ../../include/demuxing/SRVideoInput.h
        ../../src/SRAudioInput.cpp
        ../../include/demuxing/SRAudioInput.h
        ../../include/SRTools.h
        ../../src/SRInput.cpp
        ../../include/exceptions/SRExceptions.h
        ../../src/SRVideoFilter.cpp
        ../../include/transcoding/SRVideoFilter.h
        ../../include/transcoding/SRAudioFilter.h
        ../../src/SRAudioFilter.cpp
        ../../src/SRRecorder.cpp
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



