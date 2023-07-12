//
// Created by ianpo on 31/05/2023.
//

#include "Renderer/LightSource.hpp"


namespace Voxymore::Core::Renderer {
    VertexBufferLayout LightSourceVertex::GetLayout() {
        VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(2);
        return layout;
    }

    LightSource::LightSource(float size) :
            m_VertexArrayObject(true),
            m_VerticesArray{{{-(size * 0.5f), -(size * 0.5f), -(size * 0.5f)}, {0.0f, 0.0f}},
                            {{+(size * 0.5f), -(size * 0.5f), -(size * 0.5f)}, {1.0f, 0.0f}},
                            {{+(size * 0.5f), +(size * 0.5f), -(size * 0.5f)}, {1.0f, 1.0f}},
                            {{-(size * 0.5f), +(size * 0.5f), -(size * 0.5f)}, {0.0f, 1.0f}},
                            {{-(size * 0.5f), -(size * 0.5f), +(size * 0.5f)}, {0.0f, 0.0f}},
                            {{+(size * 0.5f), -(size * 0.5f), +(size * 0.5f)}, {1.0f, 0.0f}},
                            {{+(size * 0.5f), +(size * 0.5f), +(size * 0.5f)}, {1.0f, 1.0f}},
                            {{-(size * 0.5f), +(size * 0.5f), +(size * 0.5f)}, {0.0f, 1.0f}},
                            {{-(size * 0.5f), +(size * 0.5f), +(size * 0.5f)}, {1.0f, 0.0f}},
                            {{-(size * 0.5f), +(size * 0.5f), -(size * 0.5f)}, {1.0f, 1.0f}},
                            {{-(size * 0.5f), -(size * 0.5f), -(size * 0.5f)}, {0.0f, 1.0f}},
                            {{+(size * 0.5f), +(size * 0.5f), +(size * 0.5f)}, {1.0f, 0.0f}},
                            {{+(size * 0.5f), -(size * 0.5f), -(size * 0.5f)}, {0.0f, 1.0f}},
                            {{+(size * 0.5f), -(size * 0.5f), +(size * 0.5f)}, {0.0f, 0.0f}},
                            {{+(size * 0.5f), -(size * 0.5f), -(size * 0.5f)}, {1.0f, 1.0f}},
                            {{-(size * 0.5f), +(size * 0.5f), +(size * 0.5f)}, {0.0f, 0.0f}}},
            m_IndicesArray{0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4, 8, 9, 10, 10, 4, 8, 11, 2, 12, 12, 13, 11, 10, 14, 5, 5,
                           4, 10, 3, 2, 11, 11, 15, 3},
            m_Layout(LightSourceVertex::GetLayout()),
            m_Vertices((void *) m_VerticesArray, 16 * m_Layout.GetStride(), BufferUsage::STATIC_DRAW),
            m_Indices(m_IndicesArray, 36, BufferUsage::STATIC_DRAW) {
        m_VertexArrayObject.Bind();
        m_Vertices.Bind();
        m_Indices.Bind();
        m_VertexArrayObject.AddVertex(m_Vertices, m_Layout);
        m_VertexArrayObject.Unbind();
        m_Vertices.Unbind();
        m_Indices.Unbind();
    }

    void LightSource::Bind() const {
        m_VertexArrayObject.Bind();
    }

    void LightSource::Unbind() const {
        m_VertexArrayObject.Unbind();
    }

    void LightSource::Draw() const {
        Bind();
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }
}