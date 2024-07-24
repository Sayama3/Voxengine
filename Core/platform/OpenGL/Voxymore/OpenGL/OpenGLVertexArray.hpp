//
// Created by ianpo on 20/07/2023.
//

#pragma once

#include "Voxymore/Renderer/VertexArray.hpp"

namespace Voxymore::Core {

    class OpenGLVertexArray : public VertexArray {
    public:
        OpenGLVertexArray();
        virtual ~OpenGLVertexArray() override;

        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
        virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

        inline virtual const Ref<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; }
		virtual uint32_t GetRendererID() const override {return m_RendererID;};
    private:
        std::vector<Ref<VertexBuffer>> m_VertexBuffers;
        Ref<IndexBuffer> m_IndexBuffer;
        uint32_t m_RendererID;
    };

} // Core
