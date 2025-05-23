cmake_minimum_required(VERSION 3.16)


project(Voxymore VERSION 0.0.1 DESCRIPTION "The Voxymore Game Engine.")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_COMPUTED_DEFAULT "20")

option(VXM_USE_SIMPLE_FUNCTION "The ${VoxymoreCore} lib use simple version of the function in the logs" ON)
option(VXM_SHARED_BUILD "The ${VoxymoreCore} lib in shared mode" OFF)
option(VXM_DONT_PROFILE "Stop the profiling of the application" OFF)
option(VXM_USE_DOUBLE "Set the application to use double as default" OFF)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION OFF)
set(CPP_RTTI_ENABLED ON)

add_subdirectory(lib)
add_subdirectory(Core)
add_subdirectory(Voxengine)

message(STATUS "Created target ${LIBRARY_TARGET_NAME}.")