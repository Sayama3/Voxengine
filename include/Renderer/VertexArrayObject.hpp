//
// Created by ianpo on 26/05/2023.
//

#ifndef LEARNOPENGL_VERTEXARRAYOBJECT_HPP
#define LEARNOPENGL_VERTEXARRAYOBJECT_HPP

#include "VertexBufferObject.hpp"
#include "VertexBufferLayout.hpp"

namespace Voxymore::Core::Renderer {
    class VertexArrayObject {
    private:
        unsigned int m_VertexArrayId;
    public:
        VertexArrayObject(bool shouldBind = false);

        ~VertexArrayObject();

        VertexArrayObject(const VertexArrayObject &) = delete;

        VertexArrayObject &operator=(const VertexArrayObject &) = delete;

        void Bind() const;

        void Unbind() const;

        void AddVertex(const VertexBufferObject &vertices, const VertexBufferLayout &layout);

        inline const unsigned int GetId() const { return m_VertexArrayId; }
    };
}

#endif //LEARNOPENGL_VERTEXARRAYOBJECT_HPP
