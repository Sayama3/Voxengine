//
// Created by Sayama on 27/09/2023.
//

#pragma once

#include "Voxymore/Assets/Asset.hpp"
#include "Voxymore/Core/FileSystem.hpp"
#include "Voxymore/Core/Logger.hpp"
#include "Voxymore/Core/Macros.hpp"
#include "Voxymore/Core/SmartPointers.hpp"
#include "Voxymore/Math/Math.hpp"
#include "Voxymore/Renderer/Buffer.hpp"
#include "Voxymore/Renderer/Material.hpp"
#include "Voxymore/Renderer/Shader.hpp"
#include "Voxymore/Renderer/UniformBuffer.hpp"
#include "Voxymore/Renderer/VertexArray.hpp"

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

	using ModelRootScene = std::vector<int>;

	class MeshSerializer;
	class MeshImGui;

	class Model : public Asset
	{
	private:
		friend class MeshSerializer;
		friend class MeshImGui;
	public:
		VXM_IMPLEMENT_ASSET(AssetType::Model);
	private:
		std::vector<MeshGroup> m_Meshes;
		std::vector<Node> m_Nodes;
		std::vector<ModelRootScene> m_Scenes;
		std::vector<MaterialField> m_Materials;

		int m_DefaultScene = 0;
	public:
		Model(const std::vector<MeshGroup>& meshes, const std::vector<Node>& nodes, std::vector<ModelRootScene> scenes, int defaultScene = 0);
		Model();
		~Model();

		const Node& GetNode(int index) const;
		const MeshGroup& GetMeshGroup(int index) const;
		const std::vector<int>& GetDefaultScene() const;

		void Bind();
		void Unbind();
	};

	using ModelField = AssetField<Model>;

} // namespace Voxymore::Core

