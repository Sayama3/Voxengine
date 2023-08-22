//
// Created by ianpo on 21/08/2023.
//

#include "OpenGLFramebuffer.hpp"
#include "glad/glad.h"


namespace Voxymore::Core {

    OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& specification) : m_Specification(specification)
    {
        VXM_CORE_ASSERT(m_Specification.Width > 0, "Framebufffer Width must be superior to 0.");
        VXM_CORE_ASSERT(m_Specification.Height > 0, "Framebufffer Height must be superior to 0.");

        Invalidate();
    }

    const FramebufferSpecification& OpenGLFramebuffer::GetSpecification() const
    {
        return m_Specification;
    }

    OpenGLFramebuffer::~OpenGLFramebuffer()
    {
        glDeleteFramebuffers(1, &m_RendererID);
        glDeleteTextures(1, &m_ColorAttachment);
        glDeleteTextures(1, &m_DepthAttachment);
    }

    void OpenGLFramebuffer::Invalidate() {
        if(m_RendererID)
        {
            glDeleteFramebuffers(1, &m_RendererID);
            glDeleteTextures(1, &m_ColorAttachment);
            glDeleteTextures(1, &m_DepthAttachment);
        }

        glCreateFramebuffers(1, &m_RendererID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

        glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachment);
        glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);

        //TODO: put the GL_RGBA8 into the specification so that it's can be HDR compatible.
        //TODO: Put the GL_RGBA into the specification because we might not need all the info.
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Specification.Width, m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);

        glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
        glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height);
//        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);

        VXM_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete.");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLFramebuffer::Bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
        glViewport(0, 0, m_Specification.Width, m_Specification.Height);
    }

    void OpenGLFramebuffer::Unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    uint32_t OpenGLFramebuffer::GetColorAttachmentRendererID() const {
        return m_ColorAttachment;
    }

    uint32_t OpenGLFramebuffer::GetDepthAttachmentRendererID() const {
        return m_DepthAttachment;
    }

    void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
    {
        VXM_CORE_ASSERT(width > 0, "Framebufffer Width must be superior to 0.");
        VXM_CORE_ASSERT(height > 0, "Framebufffer Height must be superior to 0.");

        m_Specification.Width = width;
        m_Specification.Height = height;

        Invalidate();
    }
} // Voxymore
// Core