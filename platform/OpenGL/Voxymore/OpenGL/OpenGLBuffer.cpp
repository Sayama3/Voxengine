//
// Created by ianpo on 20/07/2023.
//

#include "OpenGLBuffer.hpp"
#include "glad/glad.h"
namespace Voxymore::Core {

    OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size, void* vertices) {
        VXM_PROFILE_FUNCTION();
        glGenBuffers(1, &m_RendererID);
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        // TODO: abstract the GL_STATIC_DRAW to have it as a param.
        glBufferData(GL_ARRAY_BUFFER, (long)size, vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
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
    }

    OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t count, const uint32_t *indices) : m_Count(count) {
        VXM_PROFILE_FUNCTION();
        glGenBuffers(1, &m_RendererID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
        // TODO: abstract the GL_STATIC_DRAW to have it as a param.
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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