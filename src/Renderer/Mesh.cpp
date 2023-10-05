//
// Created by Sayama on 27/09/2023.
//

#include "Voxymore/Renderer/Mesh.hpp"


namespace Voxymore::Core
{
	void Mesh::Bind() const
	{
		for (auto& va : m_VertexArrays)
		{
			va->Bind();
		}
	}

	void Mesh::Unbind() const
	{
		for (auto& va : m_VertexArrays)
		{
			va->Unbind();
		}
	}

	void Mesh::Draw(const glm::mat4& transform) const
	{
	}
} // namespace Voxymore::Core
