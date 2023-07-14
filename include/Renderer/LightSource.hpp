//
// Created by ianpo on 31/05/2023.
//

#ifndef LEARNOPENGL_LIGHTSOURCE_HPP
#define LEARNOPENGL_LIGHTSOURCE_HPP

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <array>
#include "ElementBufferObject.hpp"
#include "VertexBufferObject.hpp"
#include "VertexArrayObject.hpp"
#include "VertexBufferLayout.hpp"
namespace Voxymore::Core::Renderer {
    struct LightSourceVertex {
        glm::vec3 position;
        glm::vec2 uv;

        static VertexBufferLayout GetLayout();
    };

    class LightSource {
    private:

        VertexArrayObject m_VertexArrayObject;
        const LightSourceVertex m_VerticesArray[16];
        const unsigned int m_IndicesArray[36];
        const VertexBufferLayout m_Layout;

        VertexBufferObject m_Vertices;
        ElementBufferObject m_Indices;


    public:
        explicit LightSource(float size = 1.0f);

        LightSource(const LightSource &) = delete;

        LightSource &operator=(const LightSource &) = delete;

        void Bind() const;

        void Unbind() const;

        void Draw() const;
    };

}
#endif //LEARNOPENGL_LIGHTSOURCE_HPP
