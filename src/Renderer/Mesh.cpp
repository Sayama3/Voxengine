//
// Created by Sayama on 27/09/2023.
//

#include <utility>

#include "Voxymore/Renderer/Mesh.hpp"


namespace Voxymore::Core
{
	void Mesh::AddSubMesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indexes)
	{
		VXM_PROFILE_FUNCTION();
		auto &sm = m_SubMeshes.emplace_back(vertices, indexes);
	}

	void Mesh::AddSubMesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indexes, const Ref<Material> &material)
	{
		VXM_PROFILE_FUNCTION();
		auto &sm = m_SubMeshes.emplace_back(vertices, indexes);
		sm.SetMaterial(material);
	}

	SubMesh::SubMesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indexes)
	{
		VXM_PROFILE_FUNCTION();
		m_VertexArray = VertexArray::Create();

		m_BufferLayout = Vertex::Layout();

		m_VertexBuffer = VertexBuffer::Create(vertices.size() * sizeof(Vertex), vertices.data());
		m_VertexBuffer->SetLayout(m_BufferLayout);

		m_IndexBuffer = IndexBuffer::Create(indexes.size(), indexes.data());

		m_VertexArray->AddVertexBuffer(m_VertexBuffer);
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);
	}
	
	void SubMesh::Bind() const
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(m_VertexArray != nullptr, "The VertexArray doesn't exist.");
		VXM_CORE_ASSERT(m_Material != nullptr, "The Material must be set.");
		m_Material->Bind();
		m_VertexArray->Bind();
	}

	void SubMesh::Unbind() const
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(m_VertexArray != nullptr, "The VertexArray doesn't exist.");
		VXM_CORE_ASSERT(m_Material != nullptr, "The Material must be set.");
		m_VertexArray->Unbind();
		m_Material->Unbind();
	}

	void SubMesh::SetMaterial(Ref<Material> material)
	{
		VXM_PROFILE_FUNCTION();
		m_Material = std::move(material);
	}

	const Ref<Material>& SubMesh::GetMaterial() const
	{
		return m_Material;
	}

	Vertex::Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texCoord, glm::vec4 color) : Position(position), Normal(normal), TexCoord(texCoord), Color(color)
	{
	}
} // namespace Voxymore::Core
