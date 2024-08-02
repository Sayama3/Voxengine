//
// Created by ianpo on 16/07/2023.
//

#pragma once

#include "Voxymore/Core/PlatformDetection.hpp"
#include "Voxymore/Core/Logger.hpp"

#ifndef IMGUI_IMPL_OPENGL_LOADER_CUSTOM
	#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM 1
#endif

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

#define VXM_BIND_FN(fn) [](auto&&... args) -> decltype(auto) { return fn(std::forward<decltype(args)>(args)...); }
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

#ifndef VXM_DONT_PROFILE
	#ifndef VXM_PROFILING
		#define VXM_PROFILING 1
	#endif
#endif

#if (_MSC_VER && !__INTEL_COMPILER) || (__MINGW32__ || __MINGW64__)
	#define VXM_BREAK() __debugbreak();
#elif _POSIX
	#include <csignal>
	#define VXM_BREAK() std::raise(SIGTRAP)
#else
	#define VXM_BREAK()
#endif


#if VXM_USE_ASSERT
	#define VXM_CORE_ASSERT(condition, ...) if(!(condition)) { VXM_CORE_ERROR(__VA_ARGS__); VXM_BREAK(); }
	#define VXM_ASSERT(condition, ...) if(!(condition)) { VXM_ERROR(__VA_ARGS__); VXM_BREAK(); }
#else
    #define VXM_CORE_ASSERT(condition, ...)
    #define VXM_ASSERT(condition, ...)
#endif

#if VXM_DEBUG
#define VXM_CORE_CHECK(condition, ...)  if(!(condition)) { VXM_CORE_WARNING(__VA_ARGS__); }
#define VXM_CHECK(condition, ...)  if(!(condition)) { VXM_WARNING(__VA_ARGS__); }
#define VXM_CORE_CHECK_ERROR(condition, ...)  if(!(condition)) { VXM_CORE_ERROR(__VA_ARGS__); }
#define VXM_CHECK_ERROR(condition, ...)  if(!(condition)) { VXM_ERROR(__VA_ARGS__); }
#else
#define VXM_CORE_CHECK(condition, ...)
#define VXM_CHECK(condition, ...)
#endif

#ifndef BIT
#define BIT(X) (1 << X)
#endif

#ifndef CHECK_BIT
#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))
#endif

