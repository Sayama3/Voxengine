//
// Created by ianpo on 24/08/2023.
//

#pragma once
#include "Voxymore/Renderer/Camera.hpp"

namespace Voxymore::Core
{

	class RenderingCamera
	{
	private:
		glm::mat4 m_Projection;
	public:
		inline RenderingCamera(const glm::mat4& projection) : m_Projection(projection) {}
		inline const glm::mat4& GetProjection() const { return m_Projection; }
	};

} // namespace Voxymore::Core

