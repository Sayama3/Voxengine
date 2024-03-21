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
}
