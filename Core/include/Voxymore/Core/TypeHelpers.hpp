//
// Created by ianpo on 17/03/2024.
//

#pragma once

namespace Voxymore::Core
{
	// helper type for the visitor
	template<class... Ts>
	struct overloads : Ts... {
		using Ts::operator()...;
	};

	template <class... Ts>
	overloads(Ts...) -> overloads<Ts...>;

	template<typename T2, typename T1>
	inline static T2& reinterpret_raw(T1& value)
	{
		static_assert(sizeof(T2) == sizeof(T1));
		return *reinterpret_cast<T2*>(&value);
	}

	template<typename T2, typename T1>
	inline static const T2& reinterpret_raw(const T1& value)
	{
		static_assert(sizeof(T2) == sizeof(T1));
		return *reinterpret_cast<const T2*>(&value);
	}
}
