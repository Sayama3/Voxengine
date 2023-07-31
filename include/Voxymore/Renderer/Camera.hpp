//
// Created by Sayama on 22/07/2023.
//

#pragma once
#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Core/Math.hpp"

namespace Voxymore::Core {
	class Camera {
	public:
		inline virtual ~Camera() {}
		virtual glm::mat4 GetViewProjectionMatrix() const = 0;
		virtual glm::mat4 GetProjectionMatrix() const = 0;
		virtual glm::mat4 GetViewMatrix() const = 0;
	};
}