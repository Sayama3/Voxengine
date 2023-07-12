//
// Created by ianpo on 26/05/2023.
//

#ifndef LEARNOPENGL_VERTEXBUFFEROBJECT_HPP
#define LEARNOPENGL_VERTEXBUFFEROBJECT_HPP

#include "BufferUsage.hpp"
#include "BufferType.hpp"

namespace Voxymore::Core::Renderer {
    class VertexBufferObject {
    private:
        unsigned int m_BufferId;
        const BufferType m_BufferType;
    public:
        VertexBufferObject(const void *vertices, unsigned int size, BufferUsage bufferUsage = BufferUsage::STATIC_DRAW);

        ~VertexBufferObject();

        VertexBufferObject(const VertexBufferObject &) = delete;

        VertexBufferObject &operator=(const VertexBufferObject &) = delete;

        void Bind() const;

        void Unbind() const;
    };
}

#endif //LEARNOPENGL_VERTEXBUFFEROBJECT_HPP
