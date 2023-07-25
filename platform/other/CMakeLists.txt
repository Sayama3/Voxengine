cmake_minimum_required(VERSION 3.24)

target_sources(${LIBRARY_TARGET_NAME} PRIVATE
        Voxymore/Platform/DefaultInput.cpp
        Voxymore/Platform/DefaultInput.hpp
        Voxymore/Platform/DefaultWindow.cpp
        Voxymore/Platform/DefaultWindow.hpp
        )

target_include_directories(${LIBRARY_TARGET_NAME} PRIVATE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>")