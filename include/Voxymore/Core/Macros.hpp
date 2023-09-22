//
// Created by ianpo on 16/07/2023.
//

#ifndef VOXYMORE_MACROS_HPP
#define VOXYMORE_MACROS_HPP

#include "Voxymore/Core/PlatformDetection.hpp"
#include <csignal>

#define VXM_CONCAT(x, y) x ## y
#define VXM_COMBINE(x, y) VXM_CONCAT(x, y)

// Defining the "_USE_MATH_DEFINES" for visual studio in the main hpp
#ifndef _USE_MATH_DEFINES
    #define _USE_MATH_DEFINES
#endif

#ifndef __FUNCSIG__
	#ifdef __PRETTY_FUNCTION__
		#define __FUNCSIG__ __PRETTY_FUNCTION__
	#else
		#define __FUNCSIG__ __FUNCTION__
	#endif
#endif

#ifdef VXM_USE_SIMPLE_FUNCTION
	#define VXM_FUNC __FUNCTION__
#else
	#define VXM_FUNC __FUNCSIG__
#endif

#ifndef STB_IMAGE_IMPLEMENTATION
    #define STB_IMAGE_IMPLEMENTATION
#endif

#define VXM_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }
//deprecated. Use VXM_BIND_EVENT_FN
#define BIND_EVENT_FN(fn) VXM_BIND_EVENT_FN(fn)
//#define BIND_EVENT_FN(func) [this](auto && PH1) { func(std::forward<decltype(PH1)>(PH1)); }
//#define BIND_EVENT_FN(func, ...)	std::bind(&func, this, __VA_ARGS__)
//#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

#ifdef VXM_DEBUG
    #ifndef VXM_USE_ASSERT
        #define VXM_USE_ASSERT 1
    #endif

    #ifndef VXM_PROFILING
        #define VXM_PROFILING 1
    #endif
#endif

#if VXM_USE_ASSERT
    #if (_MSC_VER && !__INTEL_COMPILER) || (__MINGW32__ || __MINGW64__)
        #define VXM_CORE_ASSERT(condition, ...) if(!(condition)) { VXM_CORE_ERROR(__VA_ARGS__); __debugbreak(); }
        #define VXM_ASSERT(condition, ...) if(!(condition)) { VXM_ERROR(__VA_ARGS__); __debugbreak(); }
    #elif _POSIX
        #define VXM_CORE_ASSERT(condition, ...) if(!(condition)) { VXM_CORE_ERROR(__VA_ARGS__); std::raise(SIGTRAP) }
        #define VXM_ASSERT(condition, ...) if(!(condition)) { VXM_ERROR(__VA_ARGS__); std::raise(SIGTRAP) }
    #else
        #define VXM_CORE_ASSERT(condition, ...) if(!(condition)) { VXM_ERROR(__VA_ARGS__); }
        #define VXM_ASSERT(condition, ...) if(!(condition)) { VXM_ERROR(__VA_ARGS__); }
    #endif
#else
    #define VXM_CORE_ASSERT(condition, ...)
    #define VXM_ASSERT(condition, ...)
#endif

#ifndef BIT
#define BIT(X) (1 << X)
#endif

#endif //VOXYMORE_MACROS_HPP
