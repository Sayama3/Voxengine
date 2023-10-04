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

#define EXTENSION_GLTF "gltf"
#define EXTENSION_GLB "glb"

#include "Voxymore/Renderer/Mesh.hpp"

namespace Voxymore::Core
{

	class Model
	{
	private:
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		std::vector<Ref<IndexBuffer>> m_IndexBuffers;
		std::vector<Ref<Mesh>> m_Meshes;
	private:
		Model(const std::filesystem::path& path);
	public:
		static Ref<Model> CreateModel(const Path& path);
	};

} // namespace Voxymore::Core

