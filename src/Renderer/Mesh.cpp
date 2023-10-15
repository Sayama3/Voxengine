//
// Created by Sayama on 27/09/2023.
//

#include <utility>

#include "Voxymore/Renderer/Mesh.hpp"


namespace Voxymore::Core
{
	void Mesh::AddSubMesh(const std::vector<glm::vec3> &positions, const std::vector<glm::vec3> &normals, const std::vector<glm::vec2> &texcoords, const std::vector<glm::vec4> &colors, const std::vector<uint32_t> &indexes)
	{
		auto& sm = m_SubMeshes.emplace_back(positions, normals, texcoords, colors, indexes);
	}

	void Mesh::AddSubMesh(const std::vector<glm::vec3> &positions, const std::vector<glm::vec3> &normals, const std::vector<glm::vec2> &texcoords, const std::vector<glm::vec4> &colors, const std::vector<uint32_t> &indexes, const Ref<Material>& material)
	{
		auto& sm = m_SubMeshes.emplace_back(positions, normals, texcoords, colors, indexes);
		sm.SetMaterial(material);
	}

	SubMesh::SubMesh(const std::vector<glm::vec3> &positions, const std::vector<glm::vec3> &normals, const std::vector<glm::vec2> &texcoords, const std::vector<glm::vec4> &colors, const std::vector<uint32_t> &indexes) : VerticeCount(positions.size()), VerticeFloatCount(3 + 3 + 2 + 4), BufferSize(VerticeCount * VerticeFloatCount)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(positions.size() == normals.size() && normals.size() == texcoords.size() && texcoords.size() == colors.size(), "All vertex elements should have the same size.");

		float* VertexData = new float[BufferSize];

		m_VertexArray = VertexArray::Create();

		for (int i = 0; i < VerticeCount; ++i) {
			std::memcpy(&VertexData[(i * VerticeFloatCount)], &positions[i], 3 * (sizeof(float)));
			std::memcpy(&VertexData[(i * VerticeFloatCount) + 3], &normals[i], 3 * (sizeof(float)));
			std::memcpy(&VertexData[(i * VerticeFloatCount) + 3 + 3], &texcoords[i], 2 * (sizeof(float)));
			std::memcpy(&VertexData[(i * VerticeFloatCount) + 3 + 3 + 2], &colors[i], 4 * (sizeof(float)));
		}

		m_BufferLayout = {
				BufferElement(ShaderDataType::Float3, "Position"),
				BufferElement(ShaderDataType::Float3, "Normal"),
				BufferElement(ShaderDataType::Float2, "TexCoord"),
				BufferElement(ShaderDataType::Float4, "Color"),
		};

		m_VertexBuffer = VertexBuffer::Create(BufferSize * sizeof(float), VertexData);
		m_VertexBuffer->SetLayout(m_BufferLayout);

		m_IndexBuffer = IndexBuffer::Create(indexes.size(), indexes.data());

		m_VertexArray->AddVertexBuffer(m_VertexBuffer);
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		delete[] VertexData;
	}

	void SubMesh::Bind() const
	{
		VXM_CORE_ASSERT(m_VertexArray != nullptr, "The VertexArray doesn't exist.");
		VXM_CORE_ASSERT(m_Material != nullptr, "The Material must be set.");
		m_Material->Bind();
		m_VertexArray->Bind();
	}

	void SubMesh::Unbind() const
	{
		VXM_CORE_ASSERT(m_VertexArray != nullptr, "The VertexArray doesn't exist.");
		VXM_CORE_ASSERT(m_Material != nullptr, "The Material must be set.");
		m_VertexArray->Unbind();
		m_Material->Unbind();
	}

	void SubMesh::SetMaterial(Ref<Material> material)
	{
		m_Material = std::move(material);
	}
} // namespace Voxymore::Core
