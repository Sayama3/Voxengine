//
// Created by Sayama on 27/09/2023.
//

#pragma once

#include "Material.hpp"
#include "Voxymore/Core/FileSystem.hpp"
#include "Voxymore/Core/Logger.hpp"
#include "Voxymore/Core/Macros.hpp"
#include "Voxymore/Core/SmartPointers.hpp"
#include "Voxymore/Math/Math.hpp"
#include "Voxymore/Math/BoundingBox.hpp"
#include "Voxymore/Renderer/Buffer.hpp"
#include "Voxymore/Renderer/Shader.hpp"
#include "Voxymore/Renderer/Texture.hpp"
#include "Voxymore/Renderer/UniformBuffer.hpp"
#include "Voxymore/Renderer/VertexArray.hpp"
#include <optional>
#include <vector>

namespace Voxymore::Core
{
	class Model;
	class Mesh;

	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoord; //TODO: Add other TexCoords (optionnal)
		glm::vec4 Color = glm::vec4(1.0f);
		//TODO: Add Material ID.
		inline Vertex() = default;
		Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texCoord, glm::vec4 color = glm::vec4(1.0f));

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
			v.Position = transform * glm::vec4(v.Position, 1.0);
			v.Normal = glm::transpose(glm::inverse(transform)) * glm::vec4(v.Normal, 0.0f);
			return v;
		}
	};


	//TODO: create an API to be able to create Mesh from the client side.
	class  MeshGroup
	{
	private:
		friend class Model;
	public:
	private:
		std::vector<Ref<Mesh>> m_Meshes;
	public:
		MeshGroup() = default;
		~MeshGroup() = default;
		inline const std::vector<Ref<Mesh>>& GetSubMeshes() const { return m_Meshes; }
		//void AddSubMesh(const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& normals, const std::vector<glm::vec2>& texcoords, const std::vector<glm::vec4> &colors, const std::vector<uint32_t >& indexes);
		//void AddSubMesh(const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& normals, const std::vector<glm::vec2>& texcoords, const std::vector<glm::vec4> &colors, const std::vector<uint32_t >& indexes, const Ref<Material>& material);
		void AddSubMesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indexes);
		void AddSubMesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indexes, const Ref<Material> &material);
		void AddSubMesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indexes, const BoundingBox& aabb);
		void AddSubMesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indexes, const BoundingBox& aabb, const Ref<Material> &material);
	};

	class Mesh
	{
		friend class MeshGroup;
	public:
		//Mesh(const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& normals, const std::vector<glm::vec2>& texcoords, const std::vector<glm::vec4>& colors, const std::vector<uint32_t >& indexes);
		Mesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indexes);
		Mesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indexes, BoundingBox aabb);
		~Mesh() = default;
		inline const Ref<VertexArray>& GetVertexArray() const { return m_VertexArray; }
		void Bind() const;
		void Unbind() const;
		void SetShader(Ref<Shader>& shader);
		void SetShader(const std::string& shader);
		const Ref<Material>& GetMaterial() const;
		void SetMaterial(Ref<Material> material);
		[[nodiscard]] inline UUID id() const { return m_Id; }

		inline const BoundingBox& GetBoundingBox() const { return m_BoundingBox; }
	private:
		UUID m_Id;
		Ref<VertexArray> m_VertexArray;
		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;
		Ref<Material> m_Material;
		BufferLayout m_BufferLayout;
		BoundingBox m_BoundingBox;
	};

	class Primitive
	{
	public:
		enum Type
		{
			Square,
			Cube,
		};
		static std::string GetTypeString(Primitive::Type type);
		inline static std::unordered_map<Type, std::string> GetTypesString() { return {{Type::Square, "Square"}, {Type::Cube, "Cube"}};}
		inline static std::vector<Type> GetAllTypes() { return {Type::Square, Type::Cube};}
	private:
		static Primitive* GetInstance();
	public:
		static void InitPrimitives();
		static void DestroyPrimitives();
		static bool IsInit();

		static Ref<Mesh> GetMesh(Type type);
		static Ref<Mesh> CreateOrphan(Type type);

	public:
		Primitive() = default;
		~Primitive() = default;
	private:
		Ref<Mesh> GetOrCreateSquare();
		Ref<Mesh> GetOrCreateCube();
		static Ref<Mesh> CreateSquare();
		static Ref<Mesh> CreateCube();
	private:
		std::unordered_map<Type, Ref<Mesh>> m_Meshes;
	};

} // namespace Voxymore::Core

