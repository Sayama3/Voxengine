//
// Created by ianpo on 29/07/2023.
//

#include "OpenGLTexture2D.hpp"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include "vendor/stb_image.h"
#include "glad/glad.h"

namespace Voxymore {
    namespace Core {

        OpenGLTexture2D::OpenGLTexture2D(const std::string& path) : m_Path(path)
        {
            stbi_set_flip_vertically_on_load(true);
            int width, height, channels;
            stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
            VXM_CORE_ASSERT(data, "Load of image '{0}' failed.\n{1}.",path, stbi_failure_reason());
            m_Width = width;
            m_Height = height;
            m_Channels = channels;

            glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
            glTextureStorage2D(m_RendererID, 1, GL_RGBA8, m_Width, m_Height);

            glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            GLenum format = 0;

            VXM_CORE_ASSERT(channels > 0 && channels < 5, "The number of channel {0} is not handle at the moment.", channels);

            if(channels == 4) format = GL_RGBA;
            else if(channels == 3) format = GL_RGB;
            else if(channels == 2) format = GL_RG;
            else format = GL_RED;

            glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, format, GL_UNSIGNED_BYTE, data);

            stbi_image_free(data);
        }

        OpenGLTexture2D::~OpenGLTexture2D() {
            glDeleteTextures(1, &m_RendererID);
        }

        uint32_t OpenGLTexture2D::GetWidth() const {
            return m_Width;
        }

        uint32_t OpenGLTexture2D::GetHeight() const {
            return m_Height;
        }

        void OpenGLTexture2D::Bind(uint32_t slot) const {
            glBindTextureUnit(slot, m_RendererID);
        }
    } // Voxymore
} // Core