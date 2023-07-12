cmake_minimum_required(VERSION 3.25)

## set enable the debugger (when the debug mode is enable?)
#set(CMAKE_CXX_FLAGS_DEBUG_INIT "-g")
#
#if (CMAKE_BUILD_TYPE MATCHES Debug)
#    add_definitions(-DDEBUG)
#endif()

add_subdirectory(lib)

project(VoxymoreCore VERSION 0.0.1 DESCRIPTION "The core library of the Voxymore Game Engine.")

option(VoxymoreCore_SHARED_BUILD "The the ${VoxymoreCore} lib in shared mode" ON)
if(VoxymoreCore_SHARED_BUILD)
    add_library(${PROJECT_NAME} SHARED
            src/library.cpp
            src/Application.cpp
            )
else()
    add_library(${PROJECT_NAME}
            src/library.cpp
            src/Application.cpp
            )
endif ()

set_target_properties(${PROJECT_NAME} PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION 0
        PREFIX ""
        )

set(CMAKE_CXX_STANDARD 20)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

configure_file(${PROJECT_NAME}.pc.in ${PROJECT_NAME}.pc @ONLY)

# add the .hpp to the projects
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_include_directories(${PROJECT_NAME} PRIVATE src)

target_link_libraries(VoxymoreCore PRIVATE glm spdlog::spdlog_header_only glfw glad)