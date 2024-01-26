//
// Created by ianpo on 26/01/2024.
//

#pragma once

#include <execution>
#include <thread>

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
					std::for_each(_First, _Last, _Func);
				}
			}
		}
	};


	inline static unsigned int thread_count() noexcept
	{
		return std::thread::hardware_concurrency();
	}

}
