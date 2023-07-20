//
// Created by ianpo on 20/07/2023.
//

#pragma once

#include "Voxymore/Renderer/Buffer.hpp"

namespace Voxymore::Core {

    class OpenGLVertexBuffer : public VertexBuffer {
        public:
        OpenGLVertexBuffer(uint32_t size, void* vertices);
        virtual ~OpenGLVertexBuffer() override;
        virtual void Bind() const override;
        virtual void Unbind() const override;
        OpenGLVertexBuffer(const OpenGLVertexBuffer &) = delete;
        OpenGLVertexBuffer &operator=(const OpenGLVertexBuffer &) = delete;

    private:
        uint32_t m_RendererID;
    };
    class OpenGLIndexBuffer : public IndexBuffer {
    public:
        OpenGLIndexBuffer(uint32_t count, uint32_t* indices);
        virtual ~OpenGLIndexBuffer() override;
        virtual void Bind() const override;
        virtual void Unbind() const override;
        inline virtual uint32_t GetCount() const override { return m_Count; }
        OpenGLIndexBuffer(const OpenGLIndexBuffer &) = delete;
        OpenGLIndexBuffer &operator=(const OpenGLIndexBuffer &) = delete;
    private:
        uint32_t m_RendererID;
        uint32_t m_Count;
    };

} // VoxymoreCore
