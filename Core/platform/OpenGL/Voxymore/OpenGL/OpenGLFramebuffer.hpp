//
// Created by ianpo on 21/08/2023.
//

#pragma once

#include "Voxymore/Renderer/Framebuffer.hpp"

namespace Voxymore::Core {

    class OpenGLFramebuffer : public Framebuffer {
    private:
        FramebufferSpecification m_Specification;
        uint32_t m_RendererID = 0;
		std::vector<uint32_t> m_ColorAttachments;
		uint32_t m_DepthAttachment = 0;
		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
		FramebufferTextureSpecification m_DepthAttachmentSpecification = FramebufferTextureFormat::None;
    private:
        void Invalidate();
    public:
        OpenGLFramebuffer(const FramebufferSpecification& specification);
        virtual ~OpenGLFramebuffer() override;
        virtual const FramebufferSpecification& GetSpecification() const override;

        virtual void Bind() override;
        virtual void Unbind() override;
        virtual void Resize(uint32_t width, uint32_t height) override;
		virtual int ReadPixel(uint32_t index, int x, int y) override;
        virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override;
        virtual uint32_t GetDepthAttachmentRendererID() const override;
        virtual uint32_t GetRendererID() const override {return m_RendererID;}

		virtual void ClearColorAttachment(uint32_t index, uint8_t value) override;
		virtual void ClearColorAttachment(uint32_t index, uint32_t value) override;
		virtual void ClearColorAttachment(uint32_t index, int value) override;
		virtual void ClearColorAttachment(uint32_t index, float value) override;
    };

} // Voxymore
// Core
