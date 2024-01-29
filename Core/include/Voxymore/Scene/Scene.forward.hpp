//
// Created by ianpo on 27/01/2024.
//

#pragma once

#include <entt/entt.hpp>

namespace Voxymore::Core
{
	template<typename... Type>
	inline constexpr entt::exclude_t<Type...> exclude{};

	class Scene;
}