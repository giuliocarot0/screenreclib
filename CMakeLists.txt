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

include_directories(include)
link_directories(lib)
add_executable(screenrec main.cpp)

target_link_libraries(
        screenrec
        avformat
        avutil
        avdevice
        avfilter
        avcodec
        swscale
        #pthread
        swresample
        screenreclib

)


