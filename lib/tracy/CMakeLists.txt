cmake_minimum_required(VERSION 3.24)

project(tracy VERSION 0.10.0 DESCRIPTION "A real time, nanosecond resolution, remote telemetry, hybrid frame and sampling profiler for games and other applications.")

include(GNUInstallDirs)

file(GLOB TRACY_SRC CONFIGURE_DEPENDS 
     "*.hpp"
     "*.h"
     "*.cpp"
)

add_library(tracy
        ${TRACY_SRC}
        )


set_target_properties(tracy PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(tracy PROPERTIES SOVERSION 0)

target_include_directories(tracy PUBLIC ./)
target_compile_definitions(tracy PUBLIC TRACY_ENABLE)

set_property(TARGET tracy PROPERTY POSITION_INDEPENDENT_CODE ON)