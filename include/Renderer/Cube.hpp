//
// Created by ianpo on 31/05/2023.
//

#ifndef LEARNOPENGL_CUBE_HPP
#define LEARNOPENGL_CUBE_HPP

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <array>
#include "ElementBufferObject.hpp"
#include "VertexBufferObject.hpp"
#include "VertexArrayObject.hpp"
#include "VertexBufferLayout.hpp"

namespace Voxymore::Core::Renderer {
    struct CubeVertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 uv;

        static VertexBufferLayout GetLayout();
    };

    class Cube {
    private:
        VertexArrayObject m_VertexArrayObject;
        const VertexBufferLayout m_Layout;
        const float m_VerticesBuffer[36 * (3 + 3 + 2)];
        VertexBufferObject m_Vertices;


    public:
        explicit Cube();

        Cube(const Cube &) = delete;

        Cube &operator=(const Cube &) = delete;

        void Bind() const;

        void Unbind() const;

        void Draw() const;
    };
}

#endif //LEARNOPENGL_CUBE_HPP
