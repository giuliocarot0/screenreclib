cmake_minimum_required(VERSION 3.20)
project(SimpleMain)

set(CMAKE_CXX_STANDARD 17)

if(WIN32)
    set(PATH_TO_FFMPEG C:\\Users\\aless\\Desktop\\ffmpeg-4.4.1-full_build-shared)

    link_directories(${PATH_TO_FFMPEG}\\lib)
    include_directories(${PATH_TO_FFMPEG}\\include)
endif()


if(APPLE)
    set(PATH_TO_FFMPEG /usr/local/Cellar/ffmpeg@4/4.4.2_2/)
    link_directories(${PATH_TO_FFMPEG}/lib)
    include_directories(${PATH_TO_FFMPEG}include)
endif()

if (UNIX AND NOT APPLE)
    link_directories(/usr/local/ffmpeg/lib)
    include_directories(/usr/local/ffmpeg/include)
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
if (UNIX AND NOT APPLE)
    target_link_libraries(QTExampleApp PRIVATE
            Qt::Core
            Qt::Gui
            Qt::Widgets
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



