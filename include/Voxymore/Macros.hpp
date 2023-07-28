//
// Created by ianpo on 16/07/2023.
//

#ifndef VOXYMORE_MACROS_HPP
#define VOXYMORE_MACROS_HPP

#include "Voxymore/Logger.hpp"
#include <csignal>

// Defining the "_USE_MATH_DEFINES" for visual studio in the main hpp
#ifndef _USE_MATH_DEFINES
    #define _USE_MATH_DEFINES
#endif

#ifndef STB_IMAGE_IMPLEMENTATION
    #define STB_IMAGE_IMPLEMENTATION
#endif

//#define BIND_EVENT_FN(x) [this](auto && PH1) { x(std::forward<decltype(PH1)>(PH1)); }
#define BIND_EVENT_FN(func, ...)	std::bind(&func, this, __VA_ARGS__)
//#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

#ifdef VXM_Debug
#if (_MSC_VER && !__INTEL_COMPILER) || (__MINGW32__ || __MINGW64__)
#define VXM_CORE_ASSERT(condition, ...) if(condition) { VXM_CORE_ERROR(__VA_ARGS__); __debugbreak(); }
#define VXM_ASSERT(condition, ...) if(condition) { VXM_ERROR(__VA_ARGS__); __debugbreak(); }
#elif _POSIX
#define VXM_CORE_ASSERT(condition, ...) if(condition) { VXM_CORE_ERROR(__VA_ARGS__); std::raise(SIGTRAP) }
#define VXM_ASSERT(condition, ...) if(condition) { VXM_ERROR(__VA_ARGS__); std::raise(SIGTRAP) }
#else
#define VXM_CORE_ASSERT(condition, ...) if(condition) { VXM_ERROR(__VA_ARGS__); }
#define VXM_ASSERT(condition, ...) if(condition) { VXM_ERROR(__VA_ARGS__); }
#endif
#elif VXM_USE_ASSERT
#define VXM_CORE_ASSERT(condition, ...) if(condition) { VXM_ERROR(__VA_ARGS__); }
#define VXM_ASSERT(condition, ...) if(condition) { VXM_ERROR(__VA_ARGS__); }
#else
#define VXM_CORE_ASSERT(condition, ...)
#define VXM_ASSERT(condition, ...)
#endif

#endif //VOXYMORE_MACROS_HPP
