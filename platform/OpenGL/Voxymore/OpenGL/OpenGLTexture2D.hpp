//
// Created by ianpo on 29/07/2023.
//

#pragma once
#include "Voxymore/Renderer/Texture.hpp"
namespace Voxymore::Core {

    class OpenGLTexture2D : public Texture2D {
    public:
        OpenGLTexture2D(const std::string& path);
        virtual ~OpenGLTexture2D() override;
        virtual uint32_t GetWidth() const override;
        virtual uint32_t GetHeight() const override;
        virtual void Bind(uint32_t slot = 0) const override;
    private:
        std::string m_Path;
        uint32_t m_Width, m_Height, m_Channels;
        uint32_t m_RendererID;
    };

} // Core
