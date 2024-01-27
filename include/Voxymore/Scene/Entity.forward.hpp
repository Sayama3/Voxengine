//
// Created by ianpo on 27/01/2024.
//

#pragma once

#include <functional>

namespace Voxymore::Core
{
	class Entity;
	struct EntityField;
}

namespace std
{
	template<>
	struct hash<Voxymore::Core::Entity>;
} // namespace std