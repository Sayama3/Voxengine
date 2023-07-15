//
// Created by ianpo on 27/05/2023.
//

#ifndef LEARNOPENGL_TEXTURE2D_HPP
#define LEARNOPENGL_TEXTURE2D_HPP

#include "TextureEnums.hpp"
#include "OpenGLType.hpp"

namespace Voxymore::Core::Renderer {
    class Texture2D {
    private:
        int m_CurrentSlot;
        unsigned int m_TextureId;
        enum TextureFormat format;
        int m_Width;
        int m_Height;
        int m_NbrChannels;
        std::string m_Path;
        GLType m_PixelType;
        std::unordered_map<TextureParameterName, int> m_Params;

        void SetParam(TextureParameterName name, int value, bool updateMap);

        void Destroy();

    public:
        TextureUsage m_TextureUsage;

        // TODO? Add a second version where the texture usage is replaced by a name.
        /**
         * Constructor for the type Texture2D
         * @param path The path of the texture.
         * @param textureUsage The usage of the texture in the Shader
         * @param textureFormat The format of the texture (i.e. RGB, RGBA..). NONE = auto
         * @param pixelDataType The type of pixel (i.e. Byte, Short...). default = UNSIGNED_BYTE
         */
        Texture2D(const std::string &path, TextureUsage textureUsage,
                  enum TextureFormat textureFormat = TextureFormat::NONE,
                  enum GLType pixelDataType = GLType::UNSIGNED_BYTE);

        /**
         * Constructor a 1x1 2D texture with just a color.
         * @param color The color of the sole pixel.
         */
        Texture2D(glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f}, TextureUsage textureUsage = TextureUsage::Other);

//    Texture2D(const Texture2D& other);

        ~Texture2D();

        Texture2D(const Texture2D &) = delete;

        Texture2D &operator=(const Texture2D &) = delete;


        void Bind(unsigned int slot = 0);

        void Unbind();

        void SetParam(TextureParameterName name, int value);

        TextureSlot GetTextureSlot();

        int GetTextureIndex();

        const std::string &GetPath() const {
            return m_Path;
        }
    };
}

#endif //LEARNOPENGL_TEXTURE2D_HPP
