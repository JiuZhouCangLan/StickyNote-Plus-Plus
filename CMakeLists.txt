cmake_minimum_required(VERSION 3.14)

project(StickyNote)
if(MSVC)
    add_compile_options("/utf-8")
endif()

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
set(version 1.002)
#生成版本号
configure_file(version.hpp version.hpp)

find_package(QT NAMES Qt6 Qt5 REQUIRED)

OPTION(BUILD_QT6 "Build with Qt6." OFF)
OPTION(BUILD_QT5 "Build with Qt5." ON)
OPTION(BUILD_TO_WIN7 "Run with windows 7" OFF)

#检查qt版本使用对应的C++版本
#QT6需要使用C++17
if(BUILD_QT6)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

#查找qt组件
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Widgets Gui LinguistTools Network REQUIRED)

include(CMake/Mod.cmake)
add_subdirectory(OriginUI)
add_subdirectory(Gui)
add_subdirectory(Log)
add_subdirectory(StickyNoteFile)
add_subdirectory(WinApiPackage)
add_subdirectory(GlobalShortcut)
add_subdirectory(xml)
