//
// Created by ianpo on 26/05/2023.
//

#ifndef LEARNOPENGL_ELEMENTBUFFEROBJECT_HPP
#define LEARNOPENGL_ELEMENTBUFFEROBJECT_HPP

#include "BufferUsage.hpp"
#include "BufferType.hpp"
#include "OpenGLType.hpp"

namespace Voxymore::Core::Renderer {
    class ElementBufferObject {
    private:
        unsigned int m_BufferId;
        const BufferType m_BufferType;
        int m_IndicesCount;
    public:
        ElementBufferObject(const unsigned int *indices, unsigned int count,
                            BufferUsage bufferUsage = BufferUsage::STATIC_DRAW);

        ~ElementBufferObject();

        ElementBufferObject(const ElementBufferObject &) = delete;

        ElementBufferObject &operator=(const ElementBufferObject &) = delete;

        void Bind() const;

        void Unbind() const;

        void Draw(GLDrawMode drawMode = GLDrawMode::TRIANGLES) const;
    };
}
#endif //LEARNOPENGL_ELEMENTBUFFEROBJECT_HPP
