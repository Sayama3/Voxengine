//
// Created by ianpo on 20/07/2023.
//

#pragma once

#include "Voxymore/Renderer/Buffer.hpp"

namespace Voxymore::Core {

    class OpenGLVertexBuffer : public VertexBuffer {
        public:
        OpenGLVertexBuffer(uint32_t size, const void* vertices);
        virtual ~OpenGLVertexBuffer() override;
        virtual void Bind() const override;
        virtual void Unbind() const override;

        inline virtual void SetLayout(const BufferLayout& layout) override {m_Layout = layout;}
        inline virtual const BufferLayout& GetLayout() const override {return m_Layout;}

        OpenGLVertexBuffer(const OpenGLVertexBuffer &) = delete;
        OpenGLVertexBuffer &operator=(const OpenGLVertexBuffer &) = delete;

    private:
        uint32_t m_RendererID;
        BufferLayout m_Layout;
    };
    class OpenGLIndexBuffer : public IndexBuffer {
    public:
        OpenGLIndexBuffer(uint32_t count, const uint32_t* indices);
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
