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
#include "Voxymore/Renderer/Texture.hpp"
#include <vector>

namespace Voxymore::Core
{
	struct Node
	{
	public:
		Node() = default;
		inline Node(int meshIndex) : mesh(meshIndex) {}
		inline Node(int meshIndex, const std::vector<int>& children) : mesh(meshIndex), children(children) {}
		inline Node(int meshIndex, const std::vector<int>& children, const glm::mat4& transform) : mesh(meshIndex), children(children), transform(transform) {}
		inline Node(const std::vector<int>& children) : mesh(-1), children(children) {}
		inline Node(const std::vector<int>& children, const glm::mat4& transform) : mesh(-1), children(children), transform(transform) {}
	public:
		int mesh = -1;
		std::vector<int> children;
		glm::mat4 transform = glm::mat4(1.0f);
		//		inline Ref<::Voxymore::Core::MeshGroup>& GetMesh() {VXM_CORE_ASSERT(HasMesh(), "Node don't have a mesh.");return mesh.value();}
		//		inline const Ref<::Voxymore::Core::MeshGroup>& GetMesh() const {VXM_CORE_ASSERT(HasMesh(), "Node don't have a mesh.");return mesh.value();}
		inline int GetMeshIndex() const {return mesh;}
		inline bool HasMesh() const { return mesh > -1;}
		inline bool HasChildren() const { return !children.empty();}
	};

	class Model
	{
	private:
		std::vector<MeshGroup> m_Meshes;
		std::vector<Node> m_Nodes;
		std::vector<std::vector<int>> m_Scenes;
		std::vector<Ref<Texture2D>> m_Textures;
		Ref<Shader> m_Shader;

		int m_DefaultScene = 0;
		Path m_Path;
	public:
		Model(const Path& path, const Ref<Shader>& shader);
		~Model();
		static Ref<Model> CreateModel(const Path& path, const Ref<Shader>& shader);
		const Node& GetNode(int index) const;
		const MeshGroup& GetMeshGroup(int index) const;
		const std::vector<int>& GetDefaultScene() const;

		void Bind();
		void Unbind();

		const Path& GetPath() const;
		const Ref<Shader>& GetShader() const;
		void SetShader(Ref<Shader>&);
		void SetShader(const std::string&);
	private:
		//		Node& GetNode(int index);
	};

} // namespace Voxymore::Core

