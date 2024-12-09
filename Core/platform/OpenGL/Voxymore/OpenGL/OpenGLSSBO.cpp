//
// Created by Sayama on 10/10/2024.
//

#include "OpenGLSSBO.hpp"
#include <glad/glad.h>


namespace Voxymore::Core
{
	GLenum getUsage(SSBO::Usage usage)
	{
		switch (usage) {
			case SSBO::Usage::StreamDraw: 	return GL_STREAM_DRAW;
			case SSBO::Usage::StreamRead: 	return GL_STREAM_READ;
			case SSBO::Usage::StreamCopy: 	return GL_STREAM_COPY;
			case SSBO::Usage::StaticDraw: 	return GL_STATIC_DRAW;
			case SSBO::Usage::StaticRead: 	return GL_STATIC_READ;
			case SSBO::Usage::StaticCopy: 	return GL_STATIC_COPY;
			case SSBO::Usage::DynamicDraw: 	return GL_DYNAMIC_DRAW;
			case SSBO::Usage::DynamicRead: 	return GL_DYNAMIC_READ;
			case SSBO::Usage::DynamicCopy: 	return GL_DYNAMIC_COPY;
		}

		return 0;
	}

	OpenGLSSBO::OpenGLSSBO(uint64_t size, SSBO::Usage usage)
	{
		glCreateBuffers(1, &m_RendererID);
		Allocate(size, usage);
	}

	OpenGLSSBO::OpenGLSSBO(Buffer data, SSBO::Usage usage)
	{
		glCreateBuffers(1, &m_RendererID);
		Allocate(data.Size, usage);
		OpenGLSSBO::SetData(data, 0);
	}

	OpenGLSSBO::~OpenGLSSBO()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLSSBO::Bind(uint32_t slot)
	{
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, slot, m_RendererID);
	}

	void OpenGLSSBO::Allocate(uint64_t size, SSBO::Usage usage)
	{
		glNamedBufferData(m_RendererID, size, nullptr, getUsage(usage));
	}

	void OpenGLSSBO::SetData(Buffer data, int64_t offset)
	{
		glNamedBufferSubData(m_RendererID, offset, data.Size, data.Data);
	}

	ScopeBuffer OpenGLSSBO::GetData(uint64_t size, int64_t offset) {
		ScopeBuffer buffer(size);
		glGetNamedBufferSubData(m_RendererID, offset, size, buffer.As<void>());
		return std::move(buffer);
	}
} // namespace Voxymore::Core
