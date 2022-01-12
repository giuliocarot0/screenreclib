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
add_executable(screenreclib main.cpp src/SRDecoder.cpp include/SRDecoder.h src/SREncoder.cpp include/SREncoder.h src/SRMediaOutput.cpp include/SRMediaOutput.h src/SRVideoInput.cpp include/SRVideoInput.h src/SRAudioInput.cpp include/SRAudioInput.h include/SRTools.h src/SRInput.cpp include/exceptions/SRExceptions.h src/SRTranscoder.cpp include/SRTranscoder.h)

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


