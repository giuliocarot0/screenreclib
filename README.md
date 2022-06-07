# screenreclib c++ library
<p> <img alt="c++" src="https://img.shields.io/badge/C++-17-blue.svg?style=flat&logo=c%2B%2B"/> 
<p/>

[![CMake](https://github.com/giuliocarot0/screenreclib/actions/workflows/build.yml/badge.svg)(https://github.com/giuliocarot0/screenreclib/actions/workflows/build.yml)

**Screenreclib** is a multiplatform library which allows to record your desktop and capture some audio at the same time. Users can exploit its simple and intuitive interfaces.
The library wraps FFMPEG functionalities and adapts them for screen capturing purposes. 

## Requirements
The screenreclib itself has only few dependencies which can be easily installed on Linux/macOS/Windows

 | Dependency | Version |
 | ------- | -------|
 | cmake | >= 3.20 |
 | ffmpeg | >= 4.4.x |

### Install ffmpeg

For **Linux** users the ffmpeg can be installed by using official repositories.
The following example refers to debian-based OSs which use atp as packet manager
```bash
sudo apt-get update && sudo apt-get -y install cmake libsdl2-dev libavcodec-dev libavfilter-dev libpostproc-dev libavformat-dev libavutil-dev  libswresample-dev libswscale-dev libavdevice-dev
```
For **Windows** users, ffmpeg can be installed by downloading the precompiled library [here](https://www.gyan.dev/ffmpeg/builds/packages/ffmpeg-4.4.1-full_build-shared.7z).
The cmake file has to be changed according to your installation path. See the CmakeList file from the official repo to understand how to link ffmpeg.

**macOS** users can install ffmpeg from their terminal using brew. Please ensure that both brew and the XCode Command Line Tools are correctly installed on your mac.
```bash
brew install cmake ffmpeg
```
## Building the library
The installation procedure is almost trivial and it is based on few steps.

Clone the repository from github by prompting the following command
```bash
mkdir screenreclib
cd screenreclib
git clone www.github.com/giuliocarot0/screenreclib
```
Ensure that the macros that define the devices are set correctly based on your configuration a
and use the CmakeList.txt in the root directory to build the library.
```bash
cd screenreclib
cmake . 
```
The cmake procedure will output a directory called cmake-build-debug in which the builded library will be stored

## Modules


## Examples
The library comes with some examples. Each example has a CMakeLists.txt file that allows to build it by using cmake.
These CMakeLists.txt files are written to build both the entire lib and the example app. So if you want to take a try there is no need for prebuilding the lib.

### QtRecorder
The QtRecorder is a Qt app which requires Qt version >= 5 to work well.
Before building it please ensure to write the exact QtCmake path in the relative CMakeLists.txt.
To use this example

```bash
cd screenreclib\example
cmake QtRecorder
```
### SimpleMain
SimpleMain is a simple console applications which executes a recording routing automatically.
```bash
cd screenreclib\example
cmake SimpleMain
```
