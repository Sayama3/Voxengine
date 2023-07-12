//
// Created by ianpo on 27/05/2023.
//

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif

#include "Renderer/Texture2D.hpp"
#include "stb_image.h"
#include "Logger.hpp"

namespace Voxymore::Core::Renderer {
    Texture2D::Texture2D(const std::string &path, TextureUsage textureUsage, enum TextureFormat textureFormat,
                         enum GLType pixelDataType) : m_Path(path), m_CurrentSlot(-1), m_TextureId(),
                                                      format(textureFormat), m_Width(-1), m_Height(-1),
                                                      m_NbrChannels(-1), m_PixelType(pixelDataType), m_Params(),
                                                      m_TextureUsage(textureUsage) {
        LOG("Creating \"" + m_Path + "\"");
        glGenTextures(1, &m_TextureId);
        glBindTexture(TextureType::TEXTURE_2D,
                      m_TextureId);    // set the texture wrapping/filtering options (on currently bound texture)

        // Set Some default params
        this->SetParam(TextureParameterName::TEXTURE_WRAP_S, TextureWrapping::REPEAT);
        this->SetParam(TextureParameterName::TEXTURE_WRAP_T, TextureWrapping::REPEAT);
        this->SetParam(TextureParameterName::TEXTURE_MIN_FILTER, GL_LINEAR);
        this->SetParam(TextureParameterName::TEXTURE_MAG_FILTER, GL_LINEAR);

        // We flip the texture as we are on opengl.
        stbi_set_flip_vertically_on_load(true);
        const char *cstr = path.c_str();
        unsigned char *data = stbi_load(cstr, &m_Width, &m_Height, &m_NbrChannels, 0);

        if (data) {
            if (format == TextureFormat::NONE) {
                switch (m_NbrChannels) {
                    case 1: {
                        format = TextureFormat::RED;
                        break;
                    }
                    case 2: {
                        format = TextureFormat::RG;
                        break;
                    }
                    case 3: {
                        format = TextureFormat::RGB;
                        break;
                    }
                    case 4: {
                        format = TextureFormat::RGBA;
                        break;
                    }
                }
            }

            glTexImage2D(TextureType::TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, m_PixelType, data);
            glGenerateMipmap(TextureType::TEXTURE_2D);
        } else {
            LOG("Failed to load texture \"" + path + "\".\n" + stbi_failure_reason());
        }

        stbi_image_free(data);
        Unbind();
    }

    Texture2D::Texture2D(glm::vec4 color, TextureUsage textureUsage)
            : m_Path(), m_CurrentSlot(-1), m_TextureId(), format(TextureFormat::RGBA), m_Width(1), m_Height(1),
              m_NbrChannels(4), m_PixelType(GLType::UNSIGNED_BYTE), m_Params(), m_TextureUsage(textureUsage) {
        glGenTextures(1, &m_TextureId);
        glBindTexture(TextureType::TEXTURE_2D,
                      m_TextureId);    // set the texture wrapping/filtering options (on currently bound texture)

        // Set Some default params
        SetParam(TextureParameterName::TEXTURE_WRAP_S, TextureWrapping::REPEAT);
        SetParam(TextureParameterName::TEXTURE_WRAP_T, TextureWrapping::REPEAT);
        SetParam(TextureParameterName::TEXTURE_MIN_FILTER, GL_LINEAR);
        SetParam(TextureParameterName::TEXTURE_MAG_FILTER, GL_LINEAR);
        unsigned char data[4] = {
                static_cast<unsigned char>(glm::clamp(color.r, 0.0f, 1.0f) * 255),
                static_cast<unsigned char>(glm::clamp(color.g, 0.0f, 1.0f) * 255),
                static_cast<unsigned char>(glm::clamp(color.b, 0.0f, 1.0f) * 255),
                static_cast<unsigned char>(glm::clamp(color.a, 0.0f, 1.0f) * 255),
        };
        glTexImage2D(TextureType::TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, m_PixelType, data);
        glGenerateMipmap(TextureType::TEXTURE_2D);

        Unbind();
    }

    Texture2D::~Texture2D() {
        Destroy();
    }

    void Texture2D::Destroy() {
        LOG("Destroying \"" + m_Path + "\"");
        glDeleteTextures(1, &m_TextureId);
    }

    void Texture2D::Bind(unsigned int slot) {
        if (slot >= TextureSlot::COUNT) {
            LOG("The maximum texture slot is " + std::to_string(TextureSlot::COUNT - 1) + ". You want the " +
                std::to_string(slot) + " slot, the result will be unexpected.");
        }
        m_CurrentSlot = static_cast<int>(slot);
        glActiveTexture(TextureSlot::TEXTURE0 + m_CurrentSlot);
        glBindTexture(TextureType::TEXTURE_2D, m_TextureId);
    }

    void Texture2D::Unbind() {
        glActiveTexture(TextureSlot::TEXTURE0 + m_CurrentSlot);
        glBindTexture(TextureType::TEXTURE_2D, 0);
        m_CurrentSlot = -1;
    }

    TextureSlot Texture2D::GetTextureSlot() {
        return static_cast<TextureSlot>(TextureSlot::TEXTURE0 + m_CurrentSlot);
    }

    int Texture2D::GetTextureIndex() {
        return static_cast<int>(m_CurrentSlot);
    }

/**
 * Must bind the texture before changing it's param.
 * @param name The name of the parameter.
 * @param value The value of the parameter.
 */
    void Texture2D::SetParam(TextureParameterName name, int value) {
        SetParam(name, value, true);
    }

    void Texture2D::SetParam(TextureParameterName name, int value, bool updateMap) {
        if (updateMap) m_Params[name] = value;
        glTexParameteri(TextureType::TEXTURE_2D, name, value);
    }
}
