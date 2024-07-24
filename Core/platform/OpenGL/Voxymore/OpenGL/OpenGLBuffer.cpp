//
// Created by ianpo on 20/07/2023.
//

#include "OpenGLBuffer.hpp"
#include "glad/glad.h"
namespace Voxymore::Core {

    OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size, const void* vertices) {
        VXM_PROFILE_FUNCTION();
		glCreateBuffers(1, &m_RendererID);
		glNamedBufferData(m_RendererID, static_cast<GLsizeiptr>(size), vertices, GL_STATIC_DRAW);
    }

    void OpenGLVertexBuffer::Bind() const {
        VXM_PROFILE_FUNCTION();
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    }

    void OpenGLVertexBuffer::Unbind() const {
        VXM_PROFILE_FUNCTION();
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer() {
        VXM_PROFILE_FUNCTION();
		glDeleteBuffers(1, &m_RendererID);
		m_RendererID = 0;
    }

    OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t count, const uint32_t *indices) : m_Count(count) {
        VXM_PROFILE_FUNCTION();
		glCreateBuffers(1, &m_RendererID);
		glNamedBufferData(m_RendererID, static_cast<GLsizeiptr>(count * sizeof(uint32_t)), indices, GL_STATIC_DRAW);
    }

    void OpenGLIndexBuffer::Bind() const {
        VXM_PROFILE_FUNCTION();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    }

    void OpenGLIndexBuffer::Unbind() const {
        VXM_PROFILE_FUNCTION();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer() {
        VXM_PROFILE_FUNCTION();
        glDeleteBuffers(1, &m_RendererID);
    }
} // VoxymoreCore