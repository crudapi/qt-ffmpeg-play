cmake_minimum_required(VERSION 3.5)

project(CFfmpegPlay LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

set(ffmpeg_home "D:/work/ffmpeg/release builds/ffmpeg-7.0-full_build-shared")
include_directories(${ffmpeg_home}/include)
link_directories(${ffmpeg_home}/lib)
file(GLOB ffmpeg_libs ${ffmpeg_home}/lib/*.lib)

if(ANDROID)
  add_library(CFfmpegPlay SHARED
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
  )
else()
  add_executable(CFfmpegPlay
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    device.h
    device.cpp
    ffmpegheader.h
  )
endif()

target_link_libraries(CFfmpegPlay PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ${ffmpeg_libs})
