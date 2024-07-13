//
// Created by ianpo on 13/07/2024.
//

#pragma once

#include "Voxymore/Renderer/Texture.hpp"

namespace Voxymore::Core
{

	class OpenGLTexture2DArray : public Texture2DArray
	{
	public:
		OpenGLTexture2DArray(Texture2DArraySpecification textureSpecs);
		OpenGLTexture2DArray(Texture2DArraySpecification textureSpecs, Buffer buffer);
		~OpenGLTexture2DArray();
		OpenGLTexture2DArray(const OpenGLTexture2DArray&) = delete;
		OpenGLTexture2DArray& operator=(const OpenGLTexture2DArray&) = delete;
	public:
		[[nodiscard]] virtual uint32_t GetWidth() const override;
		[[nodiscard]] virtual uint32_t GetHeight() const override;
		[[nodiscard]] virtual uint32_t GetDepth() const override;
		[[nodiscard]] virtual uint32_t GetChannelCount() const override;
		[[nodiscard]] virtual uint32_t GetRendererID() const override;
		virtual void SetData(Buffer data) override;
		virtual void SetData(uint32_t index, Buffer data) override;
		virtual void SetData(std::vector<Buffer> data) override;
		virtual void Bind(uint32_t slot = 0) const override;
	private:
		void CreateTexture();
	private:
		Texture2DArraySpecification m_TextureSpecification;
		uint32_t m_RendererID;
	};

} // namespace Voxymore::Core

