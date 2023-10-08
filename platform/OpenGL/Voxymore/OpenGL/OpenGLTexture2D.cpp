//
// Created by ianpo on 29/07/2023.
//

#include "OpenGLTexture2D.hpp"

#include "vendor/stb_image.h"
#include "glad/glad.h"

namespace Voxymore {
    namespace Core {

        OpenGLTexture2D::OpenGLTexture2D(const Path& path) : m_Path(path)
        {
            VXM_PROFILE_FUNCTION();

            stbi_set_flip_vertically_on_load(true);
            int width, height, channels;
			std::string strPath = path.GetFullPath().string();
			VXM_CORE_ASSERT(std::filesystem::exists(path.GetFullPath()), "The file {0} do not exist...", strPath);
            stbi_uc* data = stbi_load(strPath.c_str(), &width, &height, &channels, 0);
            VXM_CORE_ASSERT(data, "Load of image '{0}' failed.\n{1}.", strPath, stbi_failure_reason());
            m_Width = width;
            m_Height = height;
            m_Channels = channels;

            VXM_CORE_ASSERT(channels > 0 && channels < 5, "The number of channel {0} is not handle at the moment.", channels);

            GLenum internalFormat = 0;
            GLenum dataFormat = 0;
            if(channels == 4) { internalFormat = GL_RGBA8; dataFormat = GL_RGBA; }
            else if(channels == 3) { internalFormat = GL_RGB8; dataFormat = GL_RGB; }
            else if(channels == 2) { internalFormat = GL_RG8; dataFormat = GL_RG; }
            else if(channels == 1) { internalFormat = GL_R8; dataFormat = GL_RED; }

            glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
            glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

            //TODO: Add parameter on the Texture API to be able to change this type of parameters.
            glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            GLenum format = 0;

            glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

            stbi_image_free(data);
        }

        OpenGLTexture2D::~OpenGLTexture2D() {
            VXM_PROFILE_FUNCTION();
            glDeleteTextures(1, &m_RendererID);
        }

        uint32_t OpenGLTexture2D::GetWidth() const {
            return m_Width;
        }

        uint32_t OpenGLTexture2D::GetHeight() const {
            return m_Height;
        }

        void OpenGLTexture2D::Bind(uint32_t slot) const {
            VXM_PROFILE_FUNCTION();
            glBindTextureUnit(slot, m_RendererID);
        }
    } // Voxymore
} // Core