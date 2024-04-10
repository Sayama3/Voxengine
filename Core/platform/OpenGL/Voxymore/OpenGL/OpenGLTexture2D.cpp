//
// Created by ianpo on 29/07/2023.
//

#include "OpenGLTexture2D.hpp"

#include <utility>
#include "glad/glad.h"


namespace Voxymore::Core {
	class TexSpecHelper
	{
	private:
		Texture2DSpecification& spec;

	public:
		TexSpecHelper(Texture2DSpecification& specification) : spec(specification) {}
		~TexSpecHelper() = default;

		[[nodiscard]] GLenum GetInternalFormat() const
		{
			VXM_PROFILE_FUNCTION();
			switch (spec.pixelFormat) {
				case RED:
				{
					switch (spec.pixelType) {
						case PX_8: return GL_R8;
						case PX_16: return GL_R16;

						case PX_8UI: return GL_R8UI;
						case PX_16UI: return GL_R16UI;
						case PX_32UI: return GL_R32UI;

						case PX_8I: return GL_R8I;
						case PX_16I: return GL_R16I;
						case PX_32I: return GL_R32I;

						case PX_16F: return GL_R16F;
						case PX_32F: return GL_R32F;
					}
				}
				case RG:
				{
					switch (spec.pixelType) {
						case PX_8: return GL_RG8;
						case PX_16: return GL_RG16;

						case PX_8UI: return GL_RG8UI;
						case PX_16UI: return GL_RG16UI;
						case PX_32UI: return GL_RG32UI;

						case PX_8I: return GL_RG8I;
						case PX_16I: return GL_RG16I;
						case PX_32I: return GL_RG32I;

						case PX_16F: return GL_RG16F;
						case PX_32F: return GL_RG32F;
					}
				}
				case RGB:
				{
					switch (spec.pixelType) {
						case PX_8: return GL_RGB8;
						case PX_16: return GL_RGB16;

						case PX_8UI: return GL_RGB8UI;
						case PX_16UI: return GL_RGB16UI;
						case PX_32UI: return GL_RGB32UI;

						case PX_8I: return GL_RGB8I;
						case PX_16I: return GL_RGB16I;
						case PX_32I: return GL_RGB32I;

						case PX_16F: return GL_RGB16F;
						case PX_32F: return GL_RGB32F;
					}
				}
				case RGBA:
				{
					switch (spec.pixelType) {
						case PX_8: return GL_RGBA8;
						case PX_16: return GL_RGBA16;

						case PX_8UI: return GL_RGBA8UI;
						case PX_16UI: return GL_RGBA16UI;
						case PX_32UI: return GL_RGBA32UI;

						case PX_8I: return GL_RGBA8I;
						case PX_16I: return GL_RGBA16I;
						case PX_32I: return GL_RGBA32I;

						case PX_16F: return GL_RGBA16F;
						case PX_32F: return GL_RGBA32F;
					}
				}
			}

			VXM_CORE_WARNING("No valid internal format found. Defaulting to GL_R8.");
			return GL_R8;
		}

		[[nodiscard]] GLenum GetFormat() const
		{
			VXM_PROFILE_FUNCTION();
			switch (spec.pixelFormat) {
				case RED: return GL_RED;
				case RG: return GL_RG;
				case RGB: return GL_RGB;
				case RGBA: return GL_RGBA;
			}

			VXM_CORE_ASSERT(false, "The pixel format {0} do not exist...", (int)spec.pixelType);
			return GL_RGBA;
		}

		[[nodiscard]] GLenum GetType() const
		{
			VXM_PROFILE_FUNCTION();
			switch (spec.pixelType) {
				case PX_8: return GL_UNSIGNED_BYTE;
				case PX_16: return GL_UNSIGNED_SHORT;
				case PX_8UI: return GL_UNSIGNED_BYTE;
				case PX_16UI: return GL_UNSIGNED_SHORT;
				case PX_32UI: return GL_UNSIGNED_INT;
				case PX_8I: return GL_BYTE;
				case PX_16I: return GL_SHORT;
				case PX_32I: return GL_INT;
				case PX_16F: return GL_HALF_FLOAT;
				case PX_32F: return GL_FLOAT;
			}

			VXM_CORE_ASSERT(false, "The pixel type {0} do not exist...", (int)spec.pixelType);
			return GL_UNSIGNED_BYTE;
		}

		[[nodiscard]] std::string_view GetTypeToString() const
		{
			VXM_PROFILE_FUNCTION();
			switch (spec.pixelType) {
				case PX_8: return "PX_8";
				case PX_16: return "PX_16";
				case PX_8UI: return "PX_8UI";
				case PX_16UI: return "PX_16UI";
				case PX_32UI: return "PX_32UI";
				case PX_8I: return "PX_8I";
				case PX_16I: return "PX_16I";
				case PX_32I: return "PX_32I";
				case PX_16F: return "PX_16F";
				case PX_32F: return "PX_32F";
			}

			VXM_CORE_ASSERT(false, "The pixel type {0} do not exist...", (int)spec.pixelType);
			return "Unknown";
		}

		[[nodiscard]] uint32_t GetPixelSize() const {
			VXM_PROFILE_FUNCTION();
			switch (spec.pixelType) {
				case PX_8:
				case PX_8UI:
				case PX_8I:
					return 1U;
				case PX_16:
				case PX_16UI:
				case PX_16I:
				case PX_16F:
					return 2U;
				case PX_32UI:
				case PX_32I:
				case PX_32F:
					return 4U;
			}

			VXM_CORE_ASSERT(false, "The pixel type {0} do not exist...", (int)spec.pixelType);
			return 1U;
		}

		[[nodiscard]] GLint GetMagFilter() const
		{
			VXM_PROFILE_FUNCTION();
			switch (spec.filterMag) {

				case Nearest: return GL_NEAREST;
				case Linear: return GL_LINEAR;
			}
			VXM_CORE_WARNING("No value found for MagFilter, defaulting to 'Linear'.");
			return GL_LINEAR;
		}

		[[nodiscard]] GLint GetMinFilter() const
		{
			VXM_PROFILE_FUNCTION();
			switch (spec.filterMin) {

				case Nearest: return GL_NEAREST;
				case Linear: return GL_LINEAR;
			}
			VXM_CORE_WARNING("No value found for MinFilter, defaulting to 'Linear'.");
			return GL_LINEAR;
		}

		[[nodiscard]] GLint GetWrapS() const
		{
			VXM_PROFILE_FUNCTION();
			switch (spec.wrapperS) {
				case Repeat: return GL_REPEAT;
				case ClampToEdge: return GL_CLAMP_TO_EDGE;
				case MirroredRepeat: return GL_MIRRORED_REPEAT;
			}
			VXM_CORE_WARNING("No value found for WrapS, defaulting to 'Repeat'.");
			return GL_REPEAT;
		}

		[[nodiscard]] GLint GetWrapT() const
		{
			VXM_PROFILE_FUNCTION();
			switch (spec.wrapperT) {
				case Repeat: return GL_REPEAT;
				case ClampToEdge: return GL_CLAMP_TO_EDGE;
				case MirroredRepeat: return GL_MIRRORED_REPEAT;
			}
			VXM_CORE_WARNING("No value found for WrapT, defaulting to 'Repeat'.");
			return GL_REPEAT;
		}
	};

	OpenGLTexture2D::OpenGLTexture2D(Texture2DSpecification textureSpecs) : m_TextureSpecification(textureSpecs)
	{
		VXM_PROFILE_FUNCTION();
		CreateTexture();
	}

	OpenGLTexture2D::OpenGLTexture2D(Texture2DSpecification textureSpecs, Buffer buffer) : m_TextureSpecification(textureSpecs)
	{
		VXM_PROFILE_FUNCTION();
		CreateTexture();
		SetData(buffer);
	}

	OpenGLTexture2D::OpenGLTexture2D(const uint8_t* data, int width, int height, int channels) : m_Width(width), m_Height(height), m_Channels(channels)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(channels > 0 && channels < 5, "The number of channel {0} is not handle at the moment.", channels);
		m_TextureSpecification.pixelFormat = static_cast<PixelFormat>(channels);
		m_TextureSpecification.pixelType = PX_8;
		m_TextureSpecification.width = width;
		m_TextureSpecification.height = height;
		m_TextureSpecification.channels = channels;

		CreateTexture();
		SetData({(void*)data, width * height * channels * sizeof(uint8_t)});
	}

	OpenGLTexture2D::OpenGLTexture2D(const uint16_t* data, int width, int height, int channels) : m_Width(width), m_Height(height), m_Channels(channels)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(channels > 0 && channels < 5, "The number of channel {0} is not handle at the moment.", channels);
		m_TextureSpecification.pixelFormat = static_cast<PixelFormat>(channels);
		m_TextureSpecification.pixelType = PX_16;
		m_TextureSpecification.width = width;
		m_TextureSpecification.height = height;
		m_TextureSpecification.channels = channels;

		CreateTexture();
		SetData({(void*)data, width * height * channels * sizeof(uint16_t)});
	}

	void OpenGLTexture2D::CreateTexture()
	{
		VXM_PROFILE_FUNCTION();
		TexSpecHelper helper(m_TextureSpecification);

		m_Width = m_TextureSpecification.width;
		m_Height = m_TextureSpecification.height;
		m_Channels = m_TextureSpecification.channels;

		VXM_CORE_ASSERT(m_TextureSpecification.channels > 0 && m_TextureSpecification.channels < 5, "The number of channel {0} is not handle at the moment.", m_TextureSpecification.channels);

		GLenum internalFormat = helper.GetInternalFormat();

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

		//TODO: Add parameter on the Texture API to be able to change this type of parameters.
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, helper.GetMinFilter());
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, helper.GetMagFilter());
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, helper.GetWrapS());
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, helper.GetWrapT());
	}

	void OpenGLTexture2D::SetData(Buffer data)
	{
		VXM_PROFILE_FUNCTION();
		TexSpecHelper helper(m_TextureSpecification);
		GLenum dataFormat = helper.GetFormat();
		GLenum pixelType = helper.GetType();
		VXM_CORE_ASSERT(data.Data != nullptr, "No data where found on the image ({0}).", Handle);
		VXM_CORE_ASSERT(m_Width * m_Height * m_Channels * helper.GetPixelSize() == data.Size, "The size of the image ({0}) is different from the information of the texture (width: {1}, height: {2}, channel: {3}, pixelType: '{4}')", data.Size, m_Width, m_Height, m_Channels, helper.GetTypeToString());
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, pixelType, data.Data);
		if(m_TextureSpecification.generateMipMaps) {
			glGenerateTextureMipmap(m_RendererID);
		}
	}

	OpenGLTexture2D::~OpenGLTexture2D() {
		VXM_PROFILE_FUNCTION();
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const {
		VXM_PROFILE_FUNCTION();
		glBindTextureUnit(slot, m_RendererID);
	}
} // Voxymore
// Core