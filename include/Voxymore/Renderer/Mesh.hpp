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
#include <optional>
#include <vector>

namespace Voxymore::Core
{
	class Model;

	//TODO: create an API to be able to create Mesh from the client side.
	class Mesh
	{
		friend class Model;
	private:
		std::vector<Ref<VertexArray>> m_VertexArrays;
		std::optional<Ref<Shader>> m_Shader;
	public:
		Mesh() = default;
		void Bind() const;
		void Unbind() const;
		inline const std::vector<Ref<VertexArray>>& GetVertexArrays() const { return m_VertexArrays; }
		// inline const Ref<Material>& GetMaterial() const {return m_Material;}
	};

} // namespace Voxymore::Core

