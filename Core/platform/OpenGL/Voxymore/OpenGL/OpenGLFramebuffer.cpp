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
			VXM_PROFILE_FUNCTION();
			return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}

		static void CreateTextures(bool multisampled, uint32_t* outIds, uint32_t count)
		{
			VXM_PROFILE_FUNCTION();
			glCreateTextures(TextureTarget(multisampled), count, outIds);
		}

		static void BindTexture(bool multisampled, uint32_t textureId)
		{
			VXM_PROFILE_FUNCTION();
			glBindTexture(TextureTarget(multisampled), textureId);
		}

		static void AttachmentColorTexture(uint32_t id, uint32_t samples, GLenum textureFormat, GLenum internalFormat, uint32_t width, uint32_t height, int index)
		{
			VXM_PROFILE_FUNCTION();
			bool multisample = samples > 1;
			if(multisample)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
			}
			else
			{
				glTexImage2D(GL_TEXTURE_2D, 0, textureFormat, width, height, 0, internalFormat, GL_UNSIGNED_BYTE, nullptr);

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
			VXM_PROFILE_FUNCTION();
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
			VXM_PROFILE_FUNCTION();
			switch (format) {
				// Other Depth format.
				case FramebufferTextureFormat::DEPTH24STENCIL8:
					return true;
				default:
					return false;
			}
			return false;
		}

		static GLenum GetTextureFormat(FramebufferTextureFormat format)
		{
			VXM_PROFILE_FUNCTION();
			switch (format)
			{
				case FramebufferTextureFormat::RGBA8: return GL_RGBA8;
				case FramebufferTextureFormat::RGBA16: return GL_RGBA16;
				case FramebufferTextureFormat::RED_INTEGER: return GL_R16I;
				case FramebufferTextureFormat::DEPTH24STENCIL8: return GL_DEPTH24_STENCIL8;
			}
			VXM_CORE_ASSERT(false, "The format {0} is not valid.", static_cast<int>(format));
			return 0;
		}

		static GLenum GetInternalTextureFormat(FramebufferTextureFormat format)
		{
			VXM_PROFILE_FUNCTION();
			switch (format)
			{
				case FramebufferTextureFormat::RGBA8: return GL_RGBA;
				case FramebufferTextureFormat::RGBA16: return GL_RGBA;
				case FramebufferTextureFormat::RED_INTEGER: return GL_RED_INTEGER;
				case FramebufferTextureFormat::DEPTH24STENCIL8: return GL_DEPTH_STENCIL_ATTACHMENT;
			}
			VXM_CORE_ASSERT(false, "The format {0} is not valid.", static_cast<int>(format));
			return 0;
		}
	}

    OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& specification) : m_Specification(specification)
    {
		VXM_PROFILE_FUNCTION();
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
		VXM_PROFILE_FUNCTION();
        return m_Specification;
    }

    OpenGLFramebuffer::~OpenGLFramebuffer()
    {
		VXM_PROFILE_FUNCTION();
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
		glDeleteTextures(1, &m_DepthAttachment);
    }

    void OpenGLFramebuffer::Invalidate() {
		VXM_PROFILE_FUNCTION();
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

		if(!m_ColorAttachmentSpecifications.empty())
		{
			m_ColorAttachments.resize(m_ColorAttachmentSpecifications.size());

			Utils::CreateTextures(multisample, m_ColorAttachments.data(), m_ColorAttachments.size());
			for (int i = 0; i < m_ColorAttachments.size(); ++i)
			{
				auto spec = m_ColorAttachmentSpecifications[i];
				Utils::BindTexture(multisample, m_ColorAttachments[i]);
				Utils::AttachmentColorTexture(m_ColorAttachments[i], m_Specification.Samples, Utils::GetTextureFormat(spec.TextureFormat), Utils::GetInternalTextureFormat(spec.TextureFormat), m_Specification.Width, m_Specification.Height, i);
			}
		}

		if(m_DepthAttachmentSpecification.TextureFormat != FramebufferTextureFormat::None)
		{
			Utils::CreateTextures(multisample, &m_DepthAttachment, 1);
			Utils::BindTexture(multisample, m_DepthAttachment);
			Utils::AttachmentDepthTexture(m_DepthAttachment, m_Specification.Samples, Utils::GetTextureFormat(m_DepthAttachmentSpecification.TextureFormat), Utils::GetInternalTextureFormat(m_DepthAttachmentSpecification.TextureFormat), m_Specification.Width, m_Specification.Height);
		}

		if(m_ColorAttachments.size() > 1)
		{
#define MAX_COLOR_ATTACHEMENT 5
			VXM_ASSERT(m_ColorAttachments.size() <= MAX_COLOR_ATTACHEMENT, "We only support {0} color attachment on a framebuffer at the moment.", MAX_COLOR_ATTACHEMENT);

			GLenum buffers[MAX_COLOR_ATTACHEMENT] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4};

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
		VXM_PROFILE_FUNCTION();
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
        glViewport(0, 0, m_Specification.Width, m_Specification.Height);
    }

    void OpenGLFramebuffer::Unbind() {
		VXM_PROFILE_FUNCTION();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    uint32_t OpenGLFramebuffer::GetColorAttachmentRendererID(uint32_t index) const {
		VXM_PROFILE_FUNCTION();
        return m_ColorAttachments[index];
    }

    uint32_t OpenGLFramebuffer::GetDepthAttachmentRendererID() const {
		VXM_PROFILE_FUNCTION();
        return m_DepthAttachment;
    }

    void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
    {
		VXM_PROFILE_FUNCTION();
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

	int OpenGLFramebuffer::ReadPixel(uint32_t index, int x, int y)
	{
		VXM_PROFILE_FUNCTION();
		int pixelData;

		VXM_CORE_ASSERT(index < m_ColorAttachments.size(), "The index {0} doesn't exist on this framebuffer.");
		VXM_CORE_ASSERT(m_ColorAttachmentSpecifications[index].TextureFormat == FramebufferTextureFormat::RED_INTEGER, "This function read from a RED_INTEGER texture format.");

		glReadBuffer(GL_COLOR_ATTACHMENT0 + index);
		glReadPixels(x, y, 1 , 1, GL_RED_INTEGER, GL_INT, &pixelData);

		return pixelData;
	}

	void OpenGLFramebuffer::ClearColorAttachment(uint32_t index, uint8_t value)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(index < m_ColorAttachments.size(), "The index {0} doesn't exist on this framebuffer.");
		auto& spec = m_ColorAttachmentSpecifications[index];

		glClearTexImage(m_ColorAttachments[index], 0, Utils::GetInternalTextureFormat(spec.TextureFormat), GL_UNSIGNED_BYTE, &value);
	}

	void OpenGLFramebuffer::ClearColorAttachment(uint32_t index, uint32_t value)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(index < m_ColorAttachments.size(), "The index {0} doesn't exist on this framebuffer.");
		auto& spec = m_ColorAttachmentSpecifications[index];

		glClearTexImage(m_ColorAttachments[index], 0, Utils::GetInternalTextureFormat(spec.TextureFormat), GL_UNSIGNED_INT, &value);
	}

	void OpenGLFramebuffer::ClearColorAttachment(uint32_t index, int value)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(index < m_ColorAttachments.size(), "The index {0} doesn't exist on this framebuffer.");
		auto& spec = m_ColorAttachmentSpecifications[index];

		glClearTexImage(m_ColorAttachments[index], 0, Utils::GetInternalTextureFormat(spec.TextureFormat), GL_INT, &value);
	}

	void OpenGLFramebuffer::ClearColorAttachment(uint32_t index, float value)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(index < m_ColorAttachments.size(), "The index {0} doesn't exist on this framebuffer.");
		auto& spec = m_ColorAttachmentSpecifications[index];

		glClearTexImage(m_ColorAttachments[index], 0, Utils::GetInternalTextureFormat(spec.TextureFormat), GL_FLOAT, &value);
	}
} // Voxymore
// Core