//
// Created by Sayama on 27/09/2023.
//

#pragma once

#include "Material.hpp"
#include "Voxymore/Core/FileSystem.hpp"
#include "Voxymore/Core/Logger.hpp"
#include "Voxymore/Core/Macros.hpp"
#include "Voxymore/Core/SmartPointers.hpp"
#include "Voxymore/Assets/Asset.hpp"
#include "Voxymore/Math/Math.hpp"
#include "Voxymore/Math/BoundingBox.hpp"
#include "Voxymore/Renderer/Buffer.hpp"
#include "Voxymore/Renderer/Shader.hpp"
#include "Voxymore/Renderer/Texture.hpp"
#include "Voxymore/Renderer/UniformBuffer.hpp"
#include "Voxymore/Renderer/VertexArray.hpp"
#include "Voxymore/Renderer/RendererAPI.hpp"
#include <optional>
#include <vector>

namespace Voxymore::Core
{
	class Model;
	class Mesh;
	class MeshGroup;

	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoord; //TODO: Add other TexCoords (optionnal)
		glm::vec4 Color = glm::vec4(1.0f);
		//TODO: Add Material ID.
		inline Vertex() = default;
		inline ~Vertex() = default;
		Vertex(glm::vec3 position, glm::vec3 normal = glm::vec3(0), glm::vec2 texCoord = glm::vec2(0), glm::vec4 color = glm::vec4(1.0f));

		inline static BufferLayout Layout()
		{
			VXM_PROFILE_FUNCTION();
			return {
					BufferElement(ShaderDataType::Float3, "Position"),
					BufferElement(ShaderDataType::Float3, "Normal"),
					BufferElement(ShaderDataType::Float2, "TexCoord"),
					BufferElement(ShaderDataType::Float4, "Color"),
			};
		}

		inline static Vertex UpdateVertex(Vertex v, const glm::mat4& transform)
		{
			VXM_PROFILE_FUNCTION();
			v.Position = Math::TransformPoint(transform, v.Position);
			v.Normal = Math::TransformDirection(glm::transpose(glm::inverse(transform)), v.Normal);
			return v;
		}
	};

	class Mesh : public Asset
	{
	public:
		VXM_IMPLEMENT_ASSET(AssetType::Mesh);
		friend class MeshGroup;
	public:
		Mesh(const std::vector<Vertex> &vertices);
		Mesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indexes);
		Mesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indexes, BoundingBox aabb);
		~Mesh() = default;
		[[nodiscard]] inline const Ref<VertexArray>& GetVertexArray() const { return m_VertexArray; }
		void Bind() const;
		void Unbind() const;
		[[nodiscard]] MaterialField GetMaterial() const;
		void SetMaterial(MaterialField material);
		[[nodiscard]] inline UUID id() const { return Handle; }

		inline const BoundingBox& GetBoundingBox() const { return m_BoundingBox; }

		inline void SetDrawMode(DrawMode drawMode) { m_DrawMode = drawMode; }
		inline DrawMode GetDrawMode() const { return m_DrawMode; }
	private:
		Ref<VertexArray> m_VertexArray;
		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;
		MaterialField m_Material;
		BufferLayout m_BufferLayout;
		BoundingBox m_BoundingBox;
		DrawMode m_DrawMode = DrawMode::Triangles;
	};

	using MeshField = AssetField<Mesh>;

	//TODO: create an API to be able to create Mesh from the client side.
	class MeshGroup
	{
	public:
		using MeshIterator = std::vector<MeshField>::iterator;
		using ConstMeshIterator = std::vector<MeshField>::const_iterator;
	private:
		std::vector<MeshField> m_Meshes;
	public:
		MeshGroup() = default;
		~MeshGroup() = default;
		inline const std::vector<MeshField>& GetSubMeshes() const { return m_Meshes; }

		inline MeshIterator begin() {return m_Meshes.begin(); }
		inline MeshIterator end() {return m_Meshes.end(); }

		inline ConstMeshIterator begin() const {return m_Meshes.cbegin(); }
		inline ConstMeshIterator end() const {return m_Meshes.cend(); }

		inline ConstMeshIterator cbegin() const {return m_Meshes.cbegin(); }
		inline ConstMeshIterator cend() const {return m_Meshes.cend(); }

		inline void reserve(uint64_t size) {m_Meshes.reserve(size);}
		inline uint64_t size() const {return m_Meshes.size();}

		void AddSubMesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indexes);
		void AddSubMesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indexes, const MaterialField& material);
		void AddSubMesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indexes, const BoundingBox& aabb);
		void AddSubMesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indexes, const BoundingBox& aabb, const MaterialField& material);
		void AddSubMesh(MeshField mesh);
	};

	class PrimitiveMesh
	{
	public:
		enum Type
		{
			Square,
			Cube,
		};
		static std::string GetTypeString(PrimitiveMesh::Type type);
		inline static std::unordered_map<Type, std::string> GetTypesString() { return {{Type::Square, "Square"}, {Type::Cube, "Cube"}};}
		inline static std::vector<Type> GetAllTypes() { return {Type::Square, Type::Cube};}
	private:
		static PrimitiveMesh * GetInstance();
	public:
		static void InitPrimitives();
		static void DestroyPrimitives();
		static bool IsInit();

		static Ref<Mesh> GetMesh(Type type);
		static Ref<Mesh> CreateOrphan(Type type, const MaterialField& material);

	public:
		PrimitiveMesh() = default;
		~PrimitiveMesh() = default;
	private:
		Ref<Mesh> GetOrCreateSquare();
		Ref<Mesh> GetOrCreateCube();
		static Ref<Mesh> CreateSquare();
		static Ref<Mesh> CreateCube();
	private:
		std::unordered_map<Type, Ref<Mesh>> m_Meshes;
	};

} // namespace Voxymore::Core

