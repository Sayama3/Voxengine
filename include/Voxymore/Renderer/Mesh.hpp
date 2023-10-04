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

namespace Voxymore::Core
{
	class Model;
	class Mesh
	{
		friend class Model;
	private:
		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;
		Ref<VertexArray> m_VertexArray;
		std::optional<Ref<Material>> m_Material;
		Ref<Shader> m_Shader;
	public:
	};

} // namespace Voxymore::Core

