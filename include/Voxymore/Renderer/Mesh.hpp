//
// Created by Sayama on 27/09/2023.
//

#pragma once

#include "Voxymore/Core/Macros.hpp"
#include "Voxymore/Core/Logger.hpp"
#include "Voxymore/Core/SmartPointers.hpp"
#include "Voxymore/Core/FileSystem.hpp"
#include "Voxymore/Renderer/Buffer.hpp"
#include "Voxymore/Renderer/VertexArray.hpp"
#include "Voxymore/Renderer/UniformBuffer.hpp"
#include "Voxymore/Renderer/Shader.hpp"
#include "Voxymore/Renderer/Material.hpp"
#include <optional>
#include <vector>

namespace Voxymore::Core
{
	class Model;


	class Mesh
	{
		friend class Model;
	private:
		std::vector<Ref<VertexArray>> m_VertexArrays;
	public:
		Mesh() = default;
		void Bind() const;
		void Unbind() const;
		void Draw(const glm::mat4& transform = glm::mat4(1.0f)) const;
	};

} // namespace Voxymore::Core

