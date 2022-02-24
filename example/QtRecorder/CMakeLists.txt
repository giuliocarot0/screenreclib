cmake_minimum_required(VERSION 3.16)

project(QTExampleApp VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(WIN32)
    set(CMAKE_PREFIX_PATH C:\\Qt\\6.1.2\\mingw81_64\\lib\\cmake\\)
    link_directories(C:\\Users\\aless\\Desktop\\ffmpeg-4.4-full_build-shared\\ffmpeg-4.4-full_build-shared\\lib)
    include_directories(C:\\Users\\aless\\Desktop\\ffmpeg-4.4-full_build-shared\\ffmpeg-4.4-full_build-shared\\include)
    find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

endif()


if(APPLE)
    set(CMAKE_PREFIX_PATH  /Users/giuliocarota/Qt/QtDesignStudio/qt5_design_studio_reduced_version/lib/cmake/)
    link_directories(/usr/local/Cellar/ffmpeg/4.4.1_5/lib)
    include_directories(/usr/local/Cellar/ffmpeg/4.4.1_5/include)
    find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)

endif()

include_directories(../../include)
add_executable(QTExampleApp
        qtrecorder.h
        qtrecorder.ui
        qtrecorder.cpp
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


target_link_libraries(QTExampleApp PRIVATE
        Qt::Core
        Qt::Gui
        Qt::Widgets
        avformat
        avutil
        avdevice
        avfilter
        avcodec
        swscale
        #pthread
        swresample
        )


