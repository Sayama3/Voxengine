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
		if(m_Shader) m_Shader.value()->Bind();
	}

	void Mesh::Unbind() const
	{
		for (auto& va : m_VertexArrays)
		{
			va->Unbind();
		}
		if(m_Shader) m_Shader.value()->Unbind();
	}
} // namespace Voxymore::Core
