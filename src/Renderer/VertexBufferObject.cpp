//
// Created by ianpo on 26/05/2023.
//

#include "Renderer/VertexBufferObject.hpp"

namespace Voxymore::Core::Renderer {
    VertexBufferObject::VertexBufferObject(const void *vertices, unsigned int size, BufferUsage bufferUsage)
            : m_BufferId(), m_BufferType(BufferType::ARRAY_BUFFER) {
        glGenBuffers(1, &this->m_BufferId);
        glBindBuffer(m_BufferType, m_BufferId);
        glBufferData(m_BufferType, size, vertices, bufferUsage);
    }

    VertexBufferObject::~VertexBufferObject() {
        glDeleteBuffers(1, &this->m_BufferId);
    }

    void VertexBufferObject::Bind() const {
        glBindBuffer(m_BufferType, m_BufferId);
    }

    void VertexBufferObject::Unbind() const {
        glBindBuffer(m_BufferType, 0);
    }
}