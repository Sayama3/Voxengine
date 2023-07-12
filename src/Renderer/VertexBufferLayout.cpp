//
// Created by ianpo on 26/05/2023.
//

#include "Renderer/VertexBufferLayout.hpp"
namespace Voxymore::Core::Renderer {
    VertexBufferLayout::VertexBufferLayout() : m_Stride(0) {}

    template<>
    void VertexBufferLayout::Push<float>(int count) {
        m_Elements.push_back({GLType::FLOAT, count, false});
        m_Stride += GetSizeOfGLType(GLType::FLOAT) * count;
    }

    template<>
    void VertexBufferLayout::Push<glm::vec1>(int count) {
        m_Elements.push_back({GLType::FLOAT, count * 1, false});
        m_Stride += GetSizeOfGLType(GLType::FLOAT) * count * 1;
    }

    template<>
    void VertexBufferLayout::Push<glm::vec2>(int count) {
        m_Elements.push_back({GLType::FLOAT, count * 2, false});
        m_Stride += GetSizeOfGLType(GLType::FLOAT) * count * 2;
    }

    template<>
    void VertexBufferLayout::Push<glm::vec3>(int count) {
        m_Elements.push_back({GLType::FLOAT, count * 3, false});
        m_Stride += GetSizeOfGLType(GLType::FLOAT) * count * 3;
    }

    template<>
    void VertexBufferLayout::Push<glm::vec4>(int count) {
        m_Elements.push_back({GLType::FLOAT, count * 4, false});
        m_Stride += GetSizeOfGLType(GLType::FLOAT) * count * 4;
    }

    template<>
    void VertexBufferLayout::Push<unsigned int>(int count) {
        m_Elements.push_back({GLType::UNSIGNED_INT, count, false});
        m_Stride += GetSizeOfGLType(GLType::UNSIGNED_INT) * count;
    }

    template<>
    void VertexBufferLayout::Push<int>(int count) {
        m_Elements.push_back({GLType::INT, count, false});
        m_Stride += GetSizeOfGLType(GLType::INT) * count;
    }

    template<>
    void VertexBufferLayout::Push<glm::ivec1>(int count) {
        m_Elements.push_back({GLType::INT, count * 1, false});
        m_Stride += GetSizeOfGLType(GLType::INT) * count * 1;
    }

    template<>
    void VertexBufferLayout::Push<glm::ivec2>(int count) {
        m_Elements.push_back({GLType::INT, count * 2, false});
        m_Stride += GetSizeOfGLType(GLType::INT) * count * 2;
    }

    template<>
    void VertexBufferLayout::Push<glm::ivec3>(int count) {
        m_Elements.push_back({GLType::INT, count * 3, false});
        m_Stride += GetSizeOfGLType(GLType::INT) * count * 3;
    }

    template<>
    void VertexBufferLayout::Push<glm::ivec4>(int count) {
        m_Elements.push_back({GLType::INT, count * 4, false});
        m_Stride += GetSizeOfGLType(GLType::INT) * count * 4;
    }

    template<>
    void VertexBufferLayout::Push<unsigned char>(int count) {
        m_Elements.push_back({GLType::UNSIGNED_BYTE, count, true});
        m_Stride += GetSizeOfGLType(GLType::UNSIGNED_BYTE) * count;
    }

    template<>
    void VertexBufferLayout::Push<char>(int count) {
        m_Elements.push_back({GLType::BYTE, count, true});
        m_Stride += GetSizeOfGLType(GLType::BYTE) * count;
    }
}