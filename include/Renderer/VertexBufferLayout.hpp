//
// Created by ianpo on 26/05/2023.
//

#ifndef LEARNOPENGL_VERTEXBUFFERLAYOUT_HPP
#define LEARNOPENGL_VERTEXBUFFERLAYOUT_HPP

#include <vector>
#include <stdexcept>
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "OpenGLType.hpp"


namespace Voxymore::Core::Renderer {
    struct VertexBufferElement {
        GLType type;
        int count;
        bool normalized;

        inline unsigned int GetSizeOfType() const { return GetSizeOfGLType(type); }
    };

    class VertexBufferLayout {
    private:
        unsigned int m_Stride;
        std::vector<VertexBufferElement> m_Elements;
    public:
        VertexBufferLayout();

        template<typename T>
        inline void Push(int count = 1) {
            throw std::runtime_error("Type not implemented.");
        }

        inline const std::vector<VertexBufferElement> &GetElements() const { return m_Elements; }

        inline unsigned int GetStride() const { return m_Stride; }
    };

    template<>
    void VertexBufferLayout::Push<float>(int count);

    template<>
    void VertexBufferLayout::Push<glm::vec1>(int count);

    template<>
    void VertexBufferLayout::Push<glm::vec2>(int count);

    template<>
    void VertexBufferLayout::Push<glm::vec3>(int count);

    template<>
    void VertexBufferLayout::Push<glm::vec4>(int count);

    template<>
    void VertexBufferLayout::Push<unsigned int>(int count);

    template<>
    void VertexBufferLayout::Push<int>(int count);

    template<>
    void VertexBufferLayout::Push<glm::ivec1>(int count);

    template<>
    void VertexBufferLayout::Push<glm::ivec2>(int count);

    template<>
    void VertexBufferLayout::Push<glm::ivec3>(int count);

    template<>
    void VertexBufferLayout::Push<glm::ivec4>(int count);

    template<>
    void VertexBufferLayout::Push<unsigned char>(int count);

    template<>
    void VertexBufferLayout::Push<char>(int count);

}

#endif //LEARNOPENGL_VERTEXBUFFERLAYOUT_HPP
