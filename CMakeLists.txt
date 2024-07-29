cmake_minimum_required(VERSION 3.16)


project(Voxymore VERSION 0.0.1 DESCRIPTION "The Voxymore Game Engine.")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_COMPUTED_DEFAULT "20")

option(VXM_SHARED_BUILD "The ${VoxymoreCore} lib in shared mode" OFF)
option(VXM_DONT_PROFILE "Stop the profiling of the application" OFF)
option(VXM_USE_DOUBLE "Set the application to use double as default" OFF)
option(VXM_BUILD_THREAD "Number of thread to compile Voxengine" 12)

set(CMAKE_BUILD_PARALLEL_LEVEL ${VXM_BUILD_THREAD})

add_subdirectory(lib)
add_subdirectory(Core)
add_subdirectory(Voxengine)

message(STATUS "Created target ${LIBRARY_TARGET_NAME}.")