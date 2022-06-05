cmake_minimum_required(VERSION 3.20)
project(screenreclib)

set(CMAKE_CXX_STANDARD 17)

if(WIN32)
    link_directories(C:\\Users\\aless\\Desktop\\ffmpeg-4.4-full_build-shared\\ffmpeg-4.4-full_build-shared\\lib)
    include_directories(C:\\Users\\aless\\Desktop\\ffmpeg-4.4-full_build-shared\\ffmpeg-4.4-full_build-shared\\include)
endif()


if(APPLE)
    link_directories(/usr/local/Cellar/ffmpeg/4.4.1_5/lib)
    include_directories(/usr/local/Cellar/ffmpeg/4.4.1_5/include)
endif()
    link_directories(/usr/local/Cellar/ffmpeg/4.4.1_5/lib)
    include_directories(/usr/local/Cellar/ffmpeg/4.4.1_5/include)

if (UNIX AND NOT APPLE)
    link_directories(/usr/local/ffmpeg/lib)
    include_directories(/usr/local/ffmpeg/include)
endif()

include_directories(include)

#compile the library
add_library(screenreclib src/SRDecoder.cpp include/transcoding/SRDecoder.h src/SREncoder.cpp include/transcoding/SREncoder.h src/SRMediaOutput.cpp include/muxing/SRMediaOutput.h src/SRVideoInput.cpp include/demuxing/SRVideoInput.h src/SRAudioInput.cpp include/demuxing/SRAudioInput.h include/SRTools.h src/SRInput.cpp include/exceptions/SRExceptions.h src/SRVideoFilter.cpp include/transcoding/SRVideoFilter.h include/transcoding/SRAudioFilter.h src/SRAudioFilter.cpp src/SRRecorder.cpp include/SRRecorder.h)
if (UNIX AND NOT APPLE)
    target_link_libraries(screenreclib
            avformat
            avdevice
            avfilter
            avcodec
            avutil
            swscale
            swresample
            z
            xcb
            pulse
            )
else()
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
endif()



