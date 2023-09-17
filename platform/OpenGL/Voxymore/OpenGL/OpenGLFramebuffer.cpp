//
// Created by ianpo on 21/08/2023.
//

#include "OpenGLFramebuffer.hpp"
#include "glad/glad.h"


namespace Voxymore::Core {

    //TODO: retrieve the max depending on the GPU capabilities.
    static const uint32_t s_MaxFramebufferSize = 8192;
	namespace Utils
	{
		static GLenum TextureTarget(bool multisampled)
		{
			return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}

		static void CreateTextures(bool multisampled, uint32_t* outIds, uint32_t count)
		{
			glCreateTextures(TextureTarget(multisampled), count, outIds);
		}

		static void BindTexture(bool multisampled, uint32_t textureId)
		{
			glBindTexture(TextureTarget(multisampled), textureId);
		}

		static void AttachmentColorTexture(uint32_t id, uint32_t samples, GLenum textureFormat, uint32_t width, uint32_t height, int index)
		{
			bool multisample = samples > 1;
			if(multisample)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, textureFormat, width, height, GL_FALSE);
			}
			else
			{
				glTexImage2D(GL_TEXTURE_2D, 0, textureFormat, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisample), id, 0);
		}

		static void AttachmentDepthTexture(uint32_t id, uint32_t samples, GLenum textureFormat, GLenum attachmentType, uint32_t width, uint32_t height)
		{
			bool multisample = samples > 1;
			if(multisample)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, textureFormat, width, height, GL_FALSE);
			}
			else
			{
				glTexStorage2D(GL_TEXTURE_2D, 1, textureFormat, width, height);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisample), id, 0);
		}

		static bool IsDepthFormat(FramebufferTextureFormat format)
		{
			switch (format) {
				// Other Depth format.
				case FramebufferTextureFormat::DEPTH24STENCIL8:
					return true;
				default:
					return false;
			}
			return false;
		}
	}

    OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& specification) : m_Specification(specification)
    {
        VXM_CORE_ASSERT(m_Specification.Width > 0 && m_Specification.Width <= s_MaxFramebufferSize, "Framebufffer width '{0}' must be superior to 0 and inferior to {1}.", m_Specification.Width, s_MaxFramebufferSize);
        VXM_CORE_ASSERT(m_Specification.Height > 0 && m_Specification.Height <= s_MaxFramebufferSize, "Framebufffer height '{0}'  must be superior to 0 and inferior to {1}.", m_Specification.Height, s_MaxFramebufferSize);

		for (auto spec: m_Specification.Attachements.Attachments)
		{
			if(Utils::IsDepthFormat(spec.TextureFormat))
			{
				m_DepthAttachmentSpecification = spec;
			}
			else
			{
				m_ColorAttachmentSpecifications.push_back(spec);
			}
		}

        Invalidate();
    }

    const FramebufferSpecification& OpenGLFramebuffer::GetSpecification() const
    {
        return m_Specification;
    }

    OpenGLFramebuffer::~OpenGLFramebuffer()
    {
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
		glDeleteTextures(1, &m_DepthAttachment);
    }

    void OpenGLFramebuffer::Invalidate() {
        if(m_RendererID)
        {
            glDeleteFramebuffers(1, &m_RendererID);
            glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
            glDeleteTextures(1, &m_DepthAttachment);
			m_ColorAttachments.clear();
			m_DepthAttachment = 0;
        }

        glCreateFramebuffers(1, &m_RendererID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		bool multisample = m_Specification.Samples > 1;

		if(m_ColorAttachmentSpecifications.size())
		{
			m_ColorAttachments.resize(m_ColorAttachmentSpecifications.size());

			Utils::CreateTextures(multisample, m_ColorAttachments.data(), m_ColorAttachments.size());
			for (size_t i = 0; i < m_ColorAttachments.size(); ++i)
			{
				auto spec = m_ColorAttachmentSpecifications[i];
				Utils::BindTexture(multisample, m_ColorAttachments[i]);
				switch (spec.TextureFormat)
				{
					case FramebufferTextureFormat::RGBA8:
					{
						Utils::AttachmentColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_RGBA8, m_Specification.Width, m_Specification.Height, i);
						break;
					}
					case FramebufferTextureFormat::RGBA16:
					{
						Utils::AttachmentColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_RGBA16, m_Specification.Width, m_Specification.Height, i);
						break;
					}
					default:
					{
						VXM_ASSERT(false, "The Texture Format {0} is not handled for the creation of a color texture.");
					}

				}
			}
		}

		if(m_DepthAttachmentSpecification.TextureFormat != FramebufferTextureFormat::None)
		{
			Utils::CreateTextures(multisample, &m_DepthAttachment, 1);
			Utils::BindTexture(multisample, m_DepthAttachment);
			switch (m_DepthAttachmentSpecification.TextureFormat)
			{
				case FramebufferTextureFormat::DEPTH24STENCIL8:
				{
					Utils::AttachmentDepthTexture(m_DepthAttachment, m_Specification.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Specification.Width, m_Specification.Height);
					break;
				}
				default:
				{
					VXM_ASSERT(false, "The Texture Format {0} is not handled for the creation of a depth texture.");
				}
			}
		}

		if(m_ColorAttachments.size() > 1)
		{
#define MAX_COLOR_ATTACHEMENT 5
			VXM_ASSERT(m_ColorAttachments.size() <= MAX_COLOR_ATTACHEMENT, "We only support {0} color attachment on a framebuffer at the moment.", MAX_COLOR_ATTACHEMENT);

			GLenum buffers[MAX_COLOR_ATTACHEMENT] = {GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5};

			glDrawBuffers(m_ColorAttachments.size(), buffers);
		}
		else if(m_ColorAttachments.empty())
		{
			// Only Depth pass.
			glDrawBuffer(GL_NONE);
		}

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

    uint32_t OpenGLFramebuffer::GetColorAttachmentRendererID(uint32_t index) const {
        return m_ColorAttachments[index];
    }

    uint32_t OpenGLFramebuffer::GetDepthAttachmentRendererID() const {
        return m_DepthAttachment;
    }

    void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
    {
        if(width == 0 || width > s_MaxFramebufferSize ||
            height == 0 || height > s_MaxFramebufferSize)
        {
            VXM_CORE_WARNING("The Framebuffer size [{0}, {1}] is not valid.", width, height);
            return;
        }

        m_Specification.Width = width;
        m_Specification.Height = height;

        Invalidate();
    }
} // Voxymore
// Core