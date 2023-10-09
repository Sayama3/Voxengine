//
// Created by Sayama on 27/09/2023.
//

#pragma once

#include "Voxymore/Core/Macros.hpp"
#include "Voxymore/Core/Logger.hpp"
#include "Voxymore/Core/SmartPointers.hpp"
#include "Voxymore/Core/FileSystem.hpp"
#include "Voxymore/Core/Math.hpp"
#include "Voxymore/Renderer/Buffer.hpp"
#include "Voxymore/Renderer/VertexArray.hpp"
#include "Voxymore/Renderer/UniformBuffer.hpp"
#include "Voxymore/Renderer/Shader.hpp"
#include "Voxymore/Renderer/Material.hpp"

#define EXTENSION_GLTF ".gltf"
#define EXTENSION_GLB ".glb"

#include "Voxymore/Renderer/Mesh.hpp"
#include <vector>

namespace Voxymore::Core
{
	struct Node
	{
	public:
		Node() = default;
		inline Node(const Ref<::Voxymore::Core::Mesh>& mesh) : Mesh(mesh) {}
		inline Node(const Ref<::Voxymore::Core::Mesh>& mesh, std::vector<int> children) : Mesh(mesh), Children(children) {}
		inline Node(const Ref<::Voxymore::Core::Mesh>& mesh, std::vector<int> children, const glm::mat4& transform) : Mesh(mesh), Children(children), Transform(transform) {}
	public:
		Ref<Mesh> Mesh;
		std::vector<int> Children;
		glm::mat4 Transform = glm::mat4(1.0f);
		inline bool HasMesh() const { return Mesh != nullptr;}
		inline bool HasChildren() const { return !Children.empty();}
	};

	class Model
	{
	private:
		std::vector<Ref<Mesh>> m_Meshes;
		std::vector<Node> m_Nodes;
		std::vector<std::vector<int>> m_Scenes;
		int m_DefaultScene = 0;
		Path m_Path;
	public:
		Model(const Path& path);
		static Ref<Model> CreateModel(const Path& path);
		const Node& GetNode(int index) const;
		const std::vector<int>& GetDefaultScene() const;
	private:
		//		Node& GetNode(int index);
	};

} // namespace Voxymore::Core

