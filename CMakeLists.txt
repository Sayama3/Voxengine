cmake_minimum_required(VERSION 3.25)

# set enable the debugger (when the debug mode is enable?)
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-g")

if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DDEBUG)
endif()


project(VoxymoreCore VERSION 0.0.1 DESCRIPTION "The core library of the Voxymore Game Engine.")

add_library(VoxymoreCore SHARED)

set_target_properties(VoxymoreCore PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(VoxymoreCore PROPERTIES SOVERSION 0)

set(CMAKE_CXX_STANDARD 20)
target_compile_features(VoxymoreCore PUBLIC cxx_std_20)

# add the .hpp to the projects
target_include_directories(VoxymoreCore PUBLIC include)

add_subdirectory(src)
add_subdirectory(lib)
