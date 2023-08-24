//
// Created by Sayama on 22/07/2023.
//

#pragma once
#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Core/Math.hpp"

namespace Voxymore::Core {
	class Camera {
	public:
		inline Camera() : m_ProjectionMatrix(1.0f) {}
		inline Camera(const glm::mat4& projection) : m_ProjectionMatrix(projection) {}
		inline virtual ~Camera() = default;
		inline virtual glm::mat4 GetProjectionMatrix() const { return m_ProjectionMatrix; };
        // TODO: add clear color / cubemap
	protected:
		glm::mat4 m_ProjectionMatrix;
	};
}