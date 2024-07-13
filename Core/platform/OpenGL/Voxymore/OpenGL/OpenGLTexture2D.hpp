//
// Created by ianpo on 29/07/2023.
//

#pragma once
#include "Voxymore/Renderer/Texture.hpp"

namespace Voxymore::Core {

    class OpenGLTexture2D : public Texture2D
	{
    public:
		OpenGLTexture2D(Texture2DSpecification textureSpecs);
		OpenGLTexture2D(Texture2DSpecification textureSpecs, Buffer buffer);
		OpenGLTexture2D(const uint8_t* data, int width, int height, int channels);
		OpenGLTexture2D(const uint16_t* data, int width, int height, int channels);
        virtual ~OpenGLTexture2D() override;
		OpenGLTexture2D(const OpenGLTexture2D&) = delete;
		OpenGLTexture2D& operator=(const OpenGLTexture2D&) = delete;

        [[nodiscard]] inline virtual uint32_t GetWidth() const override {return m_Width; }
        [[nodiscard]] inline virtual uint32_t GetHeight() const override {return m_Height; }
        [[nodiscard]] inline virtual uint32_t GetDepth() const override { return 0; }
		[[nodiscard]] inline virtual uint32_t GetChannelCount() const override {return m_Channels; }
		[[nodiscard]] inline virtual uint32_t GetRendererID() const override { return m_RendererID; };

		virtual void Bind(uint32_t slot = 0) const override;
		virtual void SetData(Buffer data) override;
    private:
		void CreateTexture();
	private:
		Texture2DSpecification m_TextureSpecification;
		Path m_Path;
		uint32_t m_Width, m_Height, m_Channels;
        uint32_t m_RendererID;
	};

} // Core
