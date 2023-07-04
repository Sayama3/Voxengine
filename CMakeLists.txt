cmake_minimum_required(VERSION 3.25)

# set enable the debugger (when the debug mode is enable?)
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-g")

if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DDEBUG)
endif()


project(VoxymoreCore VERSION 0.0.1 DESCRIPTION "The core library of the Voxymore Game Engine.")
include(GNUInstallDirs)
add_library(VoxymoreCore)

set_target_properties(VoxymoreCore PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION 0
        )

set(CMAKE_CXX_STANDARD 20)
target_compile_features(VoxymoreCore PUBLIC cxx_std_20)

# add the .hpp to the projects
target_include_directories(VoxymoreCore PUBLIC include)
target_include_directories(VoxymoreCore PRIVATE src)

add_subdirectory(src)
add_subdirectory(lib)

install(TARGETS VoxymoreCore
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
