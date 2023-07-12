//
// Created by ianpo on 26/05/2023.
//

#include "Renderer/ElementBufferObject.hpp"

namespace Voxymore::Core::Renderer {
    ElementBufferObject::ElementBufferObject(const unsigned int *indices, unsigned int count, BufferUsage bufferUsage)
            : m_BufferId(), m_BufferType(BufferType::ELEMENT_ARRAY_BUFFER), m_IndicesCount(static_cast<int>(count)) {
        glGenBuffers(1, &this->m_BufferId);
        glBindBuffer(m_BufferType, m_BufferId);
        glBufferData(m_BufferType, count * sizeof(unsigned int), indices, bufferUsage);
    }

    ElementBufferObject::~ElementBufferObject() {
        glDeleteBuffers(1, &this->m_BufferId);
    }

    void ElementBufferObject::Bind() const {
        glBindBuffer(m_BufferType, m_BufferId);
    }

    void ElementBufferObject::Unbind() const {
        glBindBuffer(m_BufferType, 0);
    }

    void ElementBufferObject::Draw(GLDrawMode drawMode) const {
        glDrawElements(drawMode, static_cast<unsigned int>(m_IndicesCount), GLType::UNSIGNED_INT, (void *) 0);
    }
}