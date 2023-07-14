//
// Created by ianpo on 15/06/2023.
//

#ifndef LEARNOPENGL_VERTEX_HPP
#define LEARNOPENGL_VERTEX_HPP

#include "VertexBufferLayout.hpp"

namespace Voxymore::Core::Renderer {
    struct Vertex {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;

        inline static VertexBufferLayout GetVertexBufferLayout() {
            VertexBufferLayout vbl;
            vbl.Push<glm::vec3>();
            vbl.Push<glm::vec3>();
            vbl.Push<glm::vec2>();
            return vbl;
        }
    };
}
#endif //LEARNOPENGL_VERTEX_HPP
