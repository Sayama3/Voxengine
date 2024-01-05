//
// Created by ianpo on 05/01/2024.
//

#pragma once

#include "Voxymore/Core/Math.hpp"

#include "Voxymore/Layers/Layer.hpp"
#include "Voxymore/Events/MouseEvent.hpp"
#include "Voxymore/Events/KeyEvent.hpp"
#include "Voxymore/Events/ApplicationEvent.hpp"

#include "Voxymore/Core/FileSystem.hpp"

namespace Voxymore::Core
{

	class PhysicsLayer : public Layer
	{
		PhysicsLayer();
		~PhysicsLayer();
	};

} // namespace Voxymore::Core

