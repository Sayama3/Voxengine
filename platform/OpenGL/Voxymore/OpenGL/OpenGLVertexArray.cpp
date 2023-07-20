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

            case ShaderDataType::Bool:      return GL_BOOL;
            case ShaderDataType::Bool2:     return GL_BOOL;
            case ShaderDataType::Bool3:     return GL_BOOL;
            case ShaderDataType::Bool4:     return GL_BOOL;
        }
        VXM_CORE_ERROR("Unknown ShaderDataType {0}.", (int)shaderDataType);
        return 0;
    }

    OpenGLVertexArray::OpenGLVertexArray() {
        glGenVertexArrays(1, &m_RendererID);
    }

    OpenGLVertexArray::~OpenGLVertexArray() {
        glDeleteVertexArrays(1, &m_RendererID);
    }

    void OpenGLVertexArray::Bind() const {
        glBindVertexArray(m_RendererID);
    }

    void OpenGLVertexArray::Unbind() const {
        glBindVertexArray(0);
    }

    void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer> &vertexBuffer) {
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
            glVertexAttribPointer(i, element.GetComponentCount(), ShaderDataTypeToOpenGLBaseType(element.Type), element.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)element.Size);
        }

        m_VertexBuffers.emplace_back(vertexBuffer);

        glBindVertexArray(0);
        vertexBuffer->Unbind();
    }

    void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer> &indexBuffer) {
        glBindVertexArray(m_RendererID);
        indexBuffer->Bind();

        m_IndexBuffer = indexBuffer;

        glBindVertexArray(0);
        m_IndexBuffer->Unbind();
    }
} // Core