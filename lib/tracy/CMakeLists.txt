cmake_minimum_required(VERSION 3.24)


# Run version helper script
include(version.cmake)

set(TRACY_VERSION_MAJOR ${major})
set(TRACY_VERSION_MINOR ${minor})
set(TRACY_VERSION_PATCH ${patch})
set(TRACY_VERSION_STRING "${TRACY_VERSION_MAJOR}.${TRACY_VERSION_MINOR}.${TRACY_VERSION_PATCH}")

project(tracy VERSION ${TRACY_VERSION_STRING} DESCRIPTION "A real time, nanosecond resolution, remote telemetry, hybrid frame and sampling profiler for games and other applications.")

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
macro(set_option option help value)
        option(${option} ${help} ${value})
        if(${option})
                message(STATUS "${option}: ON")
                target_compile_definitions(tracy PUBLIC ${option})
        else()
                message(STATUS "${option}: OFF")
        endif()
endmacro()

set_option(TRACY_ENABLE "Enable profiling" ON)
set_option(TRACY_ON_DEMAND "On-demand profiling" OFF)
set_option(TRACY_CALLSTACK "Enforce callstack collection for tracy regions" OFF)
set_option(TRACY_NO_CALLSTACK "Disable all callstack related functionality" OFF)
set_option(TRACY_NO_CALLSTACK_INLINES "Disables the inline functions in callstacks" OFF)
set_option(TRACY_ONLY_LOCALHOST "Only listen on the localhost interface" OFF)
set_option(TRACY_NO_BROADCAST "Disable client discovery by broadcast to local network" OFF)
set_option(TRACY_ONLY_IPV4 "Tracy will only accept connections on IPv4 addresses (disable IPv6)" OFF)
set_option(TRACY_NO_CODE_TRANSFER "Disable collection of source code" OFF)
set_option(TRACY_NO_CONTEXT_SWITCH "Disable capture of context switches" OFF)
set_option(TRACY_NO_EXIT "Client executable does not exit until all profile data is sent to server" OFF)
set_option(TRACY_NO_SAMPLING "Disable call stack sampling" OFF)
set_option(TRACY_NO_VERIFY "Disable zone validation for C API" OFF)
set_option(TRACY_NO_VSYNC_CAPTURE "Disable capture of hardware Vsync events" OFF)
set_option(TRACY_NO_FRAME_IMAGE  "Disable the frame image support and its thread" OFF)
set_option(TRACY_NO_SYSTEM_TRACING  "Disable systrace sampling" OFF)
set_option(TRACY_PATCHABLE_NOPSLEDS  "Enable nopsleds for efficient patching by system-level tools (e.g. rr)" OFF)
set_option(TRACY_DELAYED_INIT "Enable delayed initialization of the library (init on first call)" OFF)
set_option(TRACY_MANUAL_LIFETIME "Enable the manual lifetime management of the profile" OFF)
set_option(TRACY_FIBERS "Enable fibers support" OFF)
set_option(TRACY_NO_CRASH_HANDLER "Disable crash handling" OFF)
set_option(TRACY_TIMER_FALLBACK "Use lower resolution timers" OFF)
set_option(TRACE_CLIENT_LIBUNWIND_BACKTRACE "Use libunwind backtracing where supported" OFF)
set_option(TRACY_SYMBOL_OFFLINE_RESOLVE "Instead of full runtime symbol resolution, only resolve the image path and offset to enable offline symbol resolution" OFF)
set_option(TRACY_LIBBACKTRACE_ELF_DYNLOAD_SUPPORT "Enable libbacktrace to support dynamically loaded elfs in symbol resolution resolution after the first symbol resolve operation" OFF)


set_property(TARGET tracy PROPERTY POSITION_INDEPENDENT_CODE ON)