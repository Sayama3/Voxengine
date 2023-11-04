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
	class SubMesh;

	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoord; //TODO: Add other TexCoords (optionnal)
		glm::vec4 Color = glm::vec4(1.0f);

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
	class Mesh
	{
	private:
		friend class Model;
	public:
	private:
		std::vector<SubMesh> m_SubMeshes;
	public:
		Mesh() = default;
		~Mesh() = default;
		inline const std::vector<SubMesh>& GetSubMeshes() const { return m_SubMeshes; }
		//void AddSubMesh(const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& normals, const std::vector<glm::vec2>& texcoords, const std::vector<glm::vec4> &colors, const std::vector<uint32_t >& indexes);
		//void AddSubMesh(const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& normals, const std::vector<glm::vec2>& texcoords, const std::vector<glm::vec4> &colors, const std::vector<uint32_t >& indexes, const Ref<Material>& material);
		void AddSubMesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indexes);
		void AddSubMesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indexes, const Ref<Material> &material);
	};

	class SubMesh
	{
		friend class Mesh;
	public:
		//SubMesh(const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& normals, const std::vector<glm::vec2>& texcoords, const std::vector<glm::vec4>& colors, const std::vector<uint32_t >& indexes);
		SubMesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indexes);
		~SubMesh() = default;
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

