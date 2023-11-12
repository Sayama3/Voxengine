//
// Created by ianpo on 20/07/2023.
//

#include "OpenGLVertexArray.hpp"
#include <glad/glad.h>

namespace Voxymore::Core {
    //TODO: remove and abstract
    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType shaderDataType){
        switch (shaderDataType) {
            case ShaderDataType::Float:     return GL_FLOAT;
            case ShaderDataType::Float2:    return GL_FLOAT;
            case ShaderDataType::Float3:    return GL_FLOAT;
            case ShaderDataType::Float4:    return GL_FLOAT;

            case ShaderDataType::Mat2:      return GL_FLOAT;
            case ShaderDataType::Mat3:      return GL_FLOAT;
            case ShaderDataType::Mat4:      return GL_FLOAT;

            case ShaderDataType::Int:       return GL_INT;
            case ShaderDataType::Int2:      return GL_INT;
            case ShaderDataType::Int3:      return GL_INT;
            case ShaderDataType::Int4:      return GL_INT;

            case ShaderDataType::UInt:       return GL_UNSIGNED_INT;
            case ShaderDataType::UInt2:      return GL_UNSIGNED_INT;
            case ShaderDataType::UInt3:      return GL_UNSIGNED_INT;
            case ShaderDataType::UInt4:      return GL_UNSIGNED_INT;

            case ShaderDataType::Bool:      return GL_BOOL;
            case ShaderDataType::Bool2:     return GL_BOOL;
            case ShaderDataType::Bool3:     return GL_BOOL;
            case ShaderDataType::Bool4:     return GL_BOOL;

            case ShaderDataType::Sampler1D: return GL_SAMPLER_1D;
            case ShaderDataType::Sampler2D: return GL_SAMPLER_2D;
            case ShaderDataType::Sampler3D: return GL_SAMPLER_3D;
        }
        VXM_CORE_ERROR("Unknown ShaderDataType {0}.", (int)shaderDataType);
        return 0;
    }

    OpenGLVertexArray::OpenGLVertexArray() {
        VXM_PROFILE_FUNCTION();
        glGenVertexArrays(1, &m_RendererID);
        VXM_CORE_TRACE("Create Vertex Array '{0}'", m_RendererID);
    }

    OpenGLVertexArray::~OpenGLVertexArray() {
        VXM_PROFILE_FUNCTION();
        VXM_CORE_TRACE("Delete Vertex Array '{0}'", m_RendererID);
        glDeleteVertexArrays(1, &m_RendererID);
    }

    void OpenGLVertexArray::Bind() const {
        VXM_PROFILE_FUNCTION();
        glBindVertexArray(m_RendererID);
    }

    void OpenGLVertexArray::Unbind() const {
        VXM_PROFILE_FUNCTION();
        glBindVertexArray(0);
    }

    void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer> &vertexBuffer) {
        VXM_PROFILE_FUNCTION();

        glBindVertexArray(m_RendererID);
        vertexBuffer->Bind();

        if(!vertexBuffer->GetLayout().GetElements().size()){
            VXM_CORE_ERROR("The vertex buffer doesn't have a layout.");
        }

        const auto& layout = vertexBuffer->GetLayout();
        const auto elementCount = layout.GetElements().size();
        for (int i = 0; i < elementCount; ++i) {
            const auto& element = layout.GetElements()[i];
            glEnableVertexAttribArray(i);
            glVertexAttribPointer(i, static_cast<GLint>(element.GetComponentCount()), ShaderDataTypeToOpenGLBaseType(element.Type), element.Normalized ? GL_TRUE : GL_FALSE, static_cast<GLsizei>(layout.GetStride()), (const void*)element.Offset);
        }

        m_VertexBuffers.emplace_back(vertexBuffer);

        glBindVertexArray(0);
        vertexBuffer->Unbind();
    }

    void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer> &indexBuffer) {
        VXM_PROFILE_FUNCTION();

        glBindVertexArray(m_RendererID);
        indexBuffer->Bind();

        m_IndexBuffer = indexBuffer;

        glBindVertexArray(0);
        m_IndexBuffer->Unbind();
    }
} // Core