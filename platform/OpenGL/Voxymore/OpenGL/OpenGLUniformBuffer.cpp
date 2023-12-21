//
// Created by ianpo on 29/08/2023.
//

#include "OpenGLUniformBuffer.hpp"
#include <glad/glad.h>

namespace Voxymore
{
	namespace Core
	{
		OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t size, uint32_t binding)
		{
			VXM_PROFILE_FUNCTION();
			glCreateBuffers(1, &m_RendererID);
			glNamedBufferData(m_RendererID, size, nullptr, GL_DYNAMIC_DRAW); // TODO: investigate usage hint
			glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_RendererID);
		}

		OpenGLUniformBuffer::~OpenGLUniformBuffer()
		{
			VXM_PROFILE_FUNCTION();
			glDeleteBuffers(1, &m_RendererID);
		}


		void OpenGLUniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
		{
			VXM_PROFILE_FUNCTION();
			glNamedBufferSubData(m_RendererID, offset, size, data);
		}
	}// namespace Core
}// namespace Voxymore