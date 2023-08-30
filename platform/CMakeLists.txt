cmake_minimum_required(VERSION 3.24)

# This is a platform dependent that include the right folder depending on the platform.
# The fallback is currently "other".

if (WIN32 OR MSYS OR MINGW)
    target_compile_definitions(${LIBRARY_TARGET_NAME} PUBLIC VXM_PLATFORM_WINDOWS NOMINMAX)
    set(VOXYMORE_PLATFORM_WINDOWS 1)
elseif(APPLE)
    target_compile_definitions(${LIBRARY_TARGET_NAME} PUBLIC VXM_PLATFORM_MACOS)
    set(VOXYMORE_PLATFORM_MACOS 1)
    #TODO: Check for IOS / IPAD / etc

# /* We also have to check __ANDROID__ before __linux__
# * since android is based on the linux kernel
# * it has __linux__ defined */
elseif (CMAKE_SYSTEM_NAME STREQUAL "Android")
    target_compile_definitions(${LIBRARY_TARGET_NAME} PUBLIC VXM_PLATFORM_ANDROID)
    set(VOXYMORE_PLATFORM_ANDROID 1)
    message(FATAL_ERROR "Android is not supported!")
elseif (UNIX)
    target_compile_definitions(${LIBRARY_TARGET_NAME} PUBLIC VXM_PLATFORM_LINUX)
    set(VOXYMORE_PLATFORM_LINUX 1)
else()
    # /* Unknown compiler/platform */
    message(FATAL_ERROR "Unknown platform!")
endif()

# TODO: add renderer dependent choice.
target_compile_definitions(${LIBRARY_TARGET_NAME} PUBLIC VXM_OPENGL)
add_subdirectory(OpenGL)

if(VOXYMORE_PLATFORM_WINDOWS OR VOXYMORE_PLATFORM_MACOS OR VOXYMORE_PLATFORM_LINUX)
    target_compile_definitions(${LIBRARY_TARGET_NAME} PUBLIC VXM_DEFAULT)
    add_subdirectory(other)
endif()

if(VOXYMORE_PLATFORM_WINDOWS)
    add_subdirectory(Windows)
elseif(VOXYMORE_PLATFORM_MACOS)
    add_subdirectory(MacOS)
elseif(VOXYMORE_PLATFORM_LINUX)
    add_subdirectory(Linux)
endif()