cmake_minimum_required(VERSION 3.24)


target_sources(${LIBRARY_TARGET_NAME} PRIVATE
        Voxymore/Linux/LinuxPlatform.cpp
        Voxymore/Linux/portable-file-dialogs.h
)

target_include_directories(${LIBRARY_TARGET_NAME} PRIVATE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>")