//
// Created by ianpo on 21/08/2023.
//

#pragma once

#include "Voxymore/Renderer/Framebuffer.hpp"

namespace Voxymore::Core {

    class OpenGLFramebuffer : public Framebuffer {
    private:
        FramebufferSpecification m_Specification;
        uint32_t m_RendererID;
        uint32_t m_ColorAttachment;
        uint32_t m_DepthAttachment;
    private:
        void Invalidate();
    public:
        OpenGLFramebuffer(const FramebufferSpecification& specification);
        virtual ~OpenGLFramebuffer() override;
        virtual const FramebufferSpecification& GetSpecification() const override;

        virtual void Bind() override;
        virtual void Unbind() override;
        virtual uint32_t GetColorAttachmentRendererID() const override;
        virtual uint32_t GetDepthAttachmentRendererID() const override;
    };

} // Voxymore
// Core
