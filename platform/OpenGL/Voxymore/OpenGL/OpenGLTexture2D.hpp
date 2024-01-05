//
// Created by ianpo on 29/07/2023.
//

#pragma once
#include "Voxymore/Renderer/Texture.hpp"

namespace Voxymore::Core {

    class OpenGLTexture2D : public Texture2D
	{
    public:
        OpenGLTexture2D(const Path& path);
        OpenGLTexture2D(const std::filesystem::path& path);
		OpenGLTexture2D(const uint8_t* data, int width, int height, int channels);
		OpenGLTexture2D(const uint16_t* data, int width, int height, int channels);
		OpenGLTexture2D(Texture2DSpecification textureSpecs);
        virtual ~OpenGLTexture2D() override;

        inline virtual uint32_t GetWidth() const override {return m_Width; }
        inline virtual uint32_t GetHeight() const override {return m_Height; }
        inline virtual uint32_t GetDepth() const override { return 0; }
		inline virtual uint32_t GetRendererID() const override { return m_RendererID; };
		inline virtual uint32_t GetChannelCount() const override {return m_Channels; }

		virtual void Bind(uint32_t slot = 0) const override;
    private:
		void CreateTexture();
	private:
		Texture2DSpecification m_TextureSpecification;
		Path m_Path;
		uint32_t m_Width, m_Height, m_Channels;
        uint32_t m_RendererID;
	};

} // Core
