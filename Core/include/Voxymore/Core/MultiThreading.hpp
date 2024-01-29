//
// Created by ianpo on 26/01/2024.
//

#pragma once

#include "Voxymore/Core/PlatformDetection.hpp"
#include "Voxymore/Core/Logger.hpp"
#include <algorithm>
#include <cstdint>
#include <thread>

#if !(defined(__APPLE__) || defined(__MACH__))
#include <execution>
#endif

namespace Voxymore::Core
{

	class MultiThreading
	{
	public:
		enum ExecutionPolicy
		{
			None,
			Sequenced,
			Parallel,
			Parallel_Unsequenced,
			Unsequenced,
		};

		template <class _FwdIt, class _Fn>
		inline static void for_each(ExecutionPolicy exec, _FwdIt _First, _FwdIt _Last, _Fn _Func) noexcept
		{
#if defined(__APPLE__) || defined(__MACH__)
			// Macos doesn't support std::for_each in multithreading
			VXM_CORE_WARNING("No multithreading on IOS.");
			std::for_each(_First, _Last, _Func);
#else
			switch (exec) {
				case ExecutionPolicy::Sequenced:
				{
					std::for_each(std::execution::seq, _First, _Last, _Func);
					break;
				}
				case ExecutionPolicy::Unsequenced:
				{
					std::for_each(std::execution::unseq, _First, _Last, _Func);
					break;
				}
				case ExecutionPolicy::Parallel:
				{
					std::for_each(std::execution::par, _First, _Last, _Func);
					break;
				}
				case ExecutionPolicy::Parallel_Unsequenced:
				{
					std::for_each(std::execution::par_unseq, _First, _Last, _Func);
					break;
				}
				default:
				{
					VXM_CORE_WARNING("Execution policy is none or undefined. Using standard std::for_each.");
					std::for_each(_First, _Last, _Func);
				}
			}
#endif
		}
	};

	inline static uint32_t thread_count() noexcept
	{
		return std::thread::hardware_concurrency();
	}
}
