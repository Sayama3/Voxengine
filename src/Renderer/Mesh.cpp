//
// Created by Sayama on 27/09/2023.
//

#include "Voxymore/Renderer/Mesh.hpp"


namespace Voxymore::Core
{
	void Mesh::Bind() const
	{
		for (auto& sm : m_SubMeshes)
		{
			sm.GetVertexArray()->Bind();
		}
		if(m_Shader.has_value()) m_Shader.value()->Bind();
	}

	void Mesh::Unbind() const
	{
		for (auto& sm : m_SubMeshes)
		{
			sm.GetVertexArray()->Unbind();
		}
		if(m_Shader.has_value()) m_Shader.value()->Unbind();
	}

	void Mesh::AddSubMesh(const std::vector<glm::vec3> &positions, const std::vector<glm::vec3> &normals, const std::vector<glm::vec2> &texcoords, const std::vector<uint32_t> &indexes)
	{
		m_SubMeshes.emplace_back(positions, normals, texcoords, indexes);
	}

	Mesh::SubMesh::SubMesh(const std::vector<glm::vec3> &positions, const std::vector<glm::vec3> &normals, const std::vector<glm::vec2> &texcoords, const std::vector<uint32_t> &indexes) : Positions(positions), Normals(normals), Texcoords(texcoords), Indexes(indexes), VerticeCount(positions.size()), VerticeFloatCount(3 + 3 + 2), BufferSize(VerticeCount * VerticeFloatCount), VertexData(new float[BufferSize])
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(positions.size() == normals.size() && normals.size() == texcoords.size(), "All the vectors should do the same size !");
		m_VertexArray = VertexArray::Create();

		for (int i = 0; i < VerticeCount; ++i) {
			(glm::vec3) VertexData[(i * VerticeFloatCount)] = Positions[i];
			(glm::vec3) VertexData[(i * VerticeFloatCount) + 3] = Normals[i];
			(glm::vec2) VertexData[(i * VerticeFloatCount) + 3 + 2] = Texcoords[i];
		}

		m_BufferLayout = {
				BufferElement(ShaderDataType::Float3, "Position"),
				BufferElement(ShaderDataType::Float3, "Normal"),
				BufferElement(ShaderDataType::Float2, "TexCoord_0"),
		};

		m_VertexBuffer = VertexBuffer::Create(BufferSize * sizeof(float), VertexData);
		m_VertexBuffer->SetLayout(m_BufferLayout);

		m_IndexBuffer = IndexBuffer::Create(Indexes.size(), Indexes.data());

		m_VertexArray->AddVertexBuffer(m_VertexBuffer);
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);
	}

	Mesh::SubMesh::~SubMesh()
	{
		delete[] VertexData;
	}
} // namespace Voxymore::Core
