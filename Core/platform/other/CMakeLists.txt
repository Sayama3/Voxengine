cmake_minimum_required(VERSION 3.16)

target_sources(${LIBRARY_TARGET_NAME} PRIVATE
        Voxymore/Default/DefaultInput.cpp
        Voxymore/Default/DefaultWindow.cpp
        Voxymore/Default/DefaultWindow.hpp
        Voxymore/Default/DefaultPlatform.cpp
        Voxymore/Default/portable-file-dialogs.h
)

target_include_directories(${LIBRARY_TARGET_NAME} PRIVATE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>")