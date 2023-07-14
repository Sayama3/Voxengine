//
// Created by ianpo on 15/06/2023.
//

#ifndef LEARNOPENGL_MESH_HPP
#define LEARNOPENGL_MESH_HPP

#include <vector>
#include "Vertex.hpp"
#include "Texture2D.hpp"
#include "VertexArrayObject.hpp"
#include "VertexBufferObject.hpp"
#include "ElementBufferObject.hpp"
#include "VertexBufferLayout.hpp"
#include "ShaderProgram.hpp"
#include <memory>

namespace Voxymore::Core::Renderer {
    class Mesh {
    public:
        std::vector<Vertex> m_Vertices;
        std::vector<unsigned int> m_Indices;
        std::vector<std::shared_ptr<Texture2D>> m_Textures;
        glm::mat4 m_Model;

        Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices,
             const std::vector<std::shared_ptr<Texture2D>> &textures, BufferUsage usage = BufferUsage::STATIC_DRAW);

        ~Mesh();

        Mesh(const Mesh &) = delete;

        Mesh &operator=(const Mesh &) = delete;

        void Draw(ShaderProgram &shader);

    private:
        VertexBufferLayout m_Layout;
        VertexArrayObject m_VAO;
        VertexBufferObject m_VBO;
        ElementBufferObject m_EBO;

        void SetupMesh();
    };
}

#endif //LEARNOPENGL_MESH_HPP
