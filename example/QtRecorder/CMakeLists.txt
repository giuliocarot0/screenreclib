cmake_minimum_required(VERSION 3.16)

project(QTExampleApp VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_PREFIX_PATH  /Users/giuliocarota/Qt/QtDesignStudio/qt5_design_studio_reduced_version/lib/cmake/)

find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)

add_executable(QTExampleApp
        qtrecorder.h
        qtrecorder.ui
        qtrecorder.cpp
        main.cpp
        )

target_link_libraries(QTExampleApp PRIVATE
        Qt::Core
        Qt::Gui
        Qt::Widgets
        )