//
// Created by Sayama on 27/09/2023.
//

#pragma once

#include "Material.hpp"
#include "Voxymore/Core/FileSystem.hpp"
#include "Voxymore/Core/Logger.hpp"
#include "Voxymore/Core/Macros.hpp"
#include "Voxymore/Core/SmartPointers.hpp"
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
		inline Vertex() = default;
		Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texCoord, glm::vec4 color = glm::vec4(1.0f));

		inline static BufferLayout Layout()
		{
			return {
					BufferElement(ShaderDataType::Float3, "Position"),
					BufferElement(ShaderDataType::Float3, "Normal"),
					BufferElement(ShaderDataType::Float2, "TexCoord"),
					BufferElement(ShaderDataType::Float4, "Color"),
			};
		}
	};


	//TODO: create an API to be able to create Mesh from the client side.
	class MeshGroup
	{
	private:
		friend class Model;
	public:
	private:
		std::vector<Mesh> m_Meshes;
	public:
		MeshGroup() = default;
		~MeshGroup() = default;
		inline const std::vector<Mesh>& GetSubMeshes() const { return m_Meshes; }
		//void AddSubMesh(const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& normals, const std::vector<glm::vec2>& texcoords, const std::vector<glm::vec4> &colors, const std::vector<uint32_t >& indexes);
		//void AddSubMesh(const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& normals, const std::vector<glm::vec2>& texcoords, const std::vector<glm::vec4> &colors, const std::vector<uint32_t >& indexes, const Ref<Material>& material);
		void AddSubMesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indexes);
		void AddSubMesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indexes, const Ref<Material> &material);
	};

	class Mesh
	{
		friend class MeshGroup;
	public:
		//Mesh(const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& normals, const std::vector<glm::vec2>& texcoords, const std::vector<glm::vec4>& colors, const std::vector<uint32_t >& indexes);
		Mesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indexes);
		~Mesh() = default;
		inline const Ref<VertexArray>& GetVertexArray() const { return m_VertexArray; }
		void Bind() const;
		void Unbind() const;
		void SetMaterial(Ref<Material> material);
		const Ref<Material>& GetMaterial() const;
	private:
		Ref<VertexArray> m_VertexArray;
		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;
		Ref<Material> m_Material;
		BufferLayout m_BufferLayout;
	};

} // namespace Voxymore::Core

