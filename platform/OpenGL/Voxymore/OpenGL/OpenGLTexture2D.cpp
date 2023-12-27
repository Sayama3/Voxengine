//
// Created by ianpo on 29/07/2023.
//

#include "OpenGLTexture2D.hpp"

#include <stb_image.h>

#include <utility>
#include "glad/glad.h"


namespace Voxymore::Core {
	class TexSpecHelper
	{
	private:
		Texture2DSpecification& spec;
		stbi_uc* data;

	public:
		TexSpecHelper(Texture2DSpecification& specification) : spec(specification), data(nullptr) {}
		~TexSpecHelper()
		{
			VXM_PROFILE_FUNCTION();
			if(data != nullptr)
			{
				stbi_image_free(data);
			}
		}

		void ReadDataFromImage()
		{
			VXM_PROFILE_FUNCTION();
			if(std::holds_alternative<Path>(spec.image) && !data)
			{
				const Path& path = std::get<Path>(spec.image);
				stbi_set_flip_vertically_on_load(true);

				int width, height, channels;
				std::string strPath = path.string();
				VXM_CORE_ASSERT(FileSystem::Exist(path), "The file {0} do not exist...", strPath);

				data = stbi_load(strPath.c_str(), &width, &height, &channels, static_cast<int>(spec.channels));
				VXM_CORE_ASSERT(data, "Load of image '{0}' failed.\n{1}.", strPath, stbi_failure_reason());

				if(spec.width && spec.width != width) VXM_CORE_WARNING("The width set in the spec ({0}) doesn't not correspond to the image ({1}).", spec.width, width);
				if(spec.height && spec.height != height) VXM_CORE_WARNING("The height set in the spec ({0}) doesn't not correspond to the image ({1}).", spec.height, height);
				if(spec.channels && spec.channels != channels) VXM_CORE_WARNING("The channels set in the spec ({0}) doesn't not correspond to the image ({1}).", spec.channels, channels);

				spec.width = width;
				spec.height = height;
				spec.channels = channels;

				spec.pixelType = PixelType::PX_8;
				spec.pixelFormat = static_cast<PixelFormat>(channels);
			}
		}

		GLenum GetInternalFormat() const
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

		GLenum GetFormat() const
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

		GLenum GetType() const
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

		GLint GetMagFilter() const
		{
			VXM_PROFILE_FUNCTION();
			switch (spec.filterMag) {

				case Nearest: return GL_NEAREST;
				case Linear: return GL_LINEAR;
			}
			VXM_CORE_WARNING("No value found for MagFilter, defaulting to 'Linear'.");
			return GL_LINEAR;
		}

		GLint GetMinFilter() const
		{
			VXM_PROFILE_FUNCTION();
			switch (spec.filterMin) {

				case Nearest: return GL_NEAREST;
				case Linear: return GL_LINEAR;
			}
			VXM_CORE_WARNING("No value found for MinFilter, defaulting to 'Linear'.");
			return GL_LINEAR;
		}

		GLint GetWrapS() const
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

		GLint GetWrapT() const
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

		const void* GetData()
		{
			VXM_PROFILE_FUNCTION();
			VXM_ASSERT(spec.image.index() != std::variant_npos, "The spec don't contain any image.");
			if(std::holds_alternative<const void*>(spec.image))
			{
				return std::get<const void*>(spec.image);
			}
			else if(data == nullptr)
			{
				const Path& path = std::get<Path>(spec.image);
				stbi_set_flip_vertically_on_load(true);

				int width, height, channels;
				std::string strPath = path.string();
				VXM_CORE_ASSERT(FileSystem::Exist(path), "The file {0} do not exist...", strPath);

				data = stbi_load(strPath.c_str(), &width, &height, &channels, 0);
				VXM_CORE_ASSERT(data, "Load of image '{0}' failed.\n{1}.", strPath, stbi_failure_reason());
				VXM_CORE_ASSERT(spec.width == width,"The width set in the spec ({0}) doesn't not correspond to the image ({1}).", spec.width, width);
				VXM_CORE_ASSERT(spec.height == height,"The height set in the spec ({0}) doesn't not correspond to the image ({1}).", spec.height, height);
				VXM_CORE_ASSERT(spec.channels == channels,"The channels set in the spec ({0}) doesn't not correspond to the image ({1}).", spec.channels, channels);
				VXM_CORE_ASSERT(spec.pixelType == PixelType::PX_8, "The Pixel Type in the spec ({0}) doesn't not correspond to the image ({1})", spec.pixelType, PixelType::PX_8);
				VXM_CORE_ASSERT(spec.pixelFormat == static_cast<PixelFormat>(channels), "The Pixel Format in the spec ({0}) doesn't not correspond to the image ({1})", spec.pixelFormat, static_cast<PixelFormat>(channels));
			}

			if(data == nullptr) { VXM_CORE_WARNING("We return a null data pointer for the texture '{0}'", spec.name); }
			return data;
		}
	};

	OpenGLTexture2D::OpenGLTexture2D(const Path& path) : m_Path(path)
	{
		VXM_PROFILE_FUNCTION();
		m_TextureSpecification.name = m_Path.id();
		m_TextureSpecification.image = m_Path;

		CreateTexture();
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::filesystem::path& path) : m_Path(Path::GetPath(path))
	{
		VXM_PROFILE_FUNCTION();
		m_TextureSpecification.name = m_Path.id();
		m_TextureSpecification.image = m_Path;

		CreateTexture();
	}

	OpenGLTexture2D::OpenGLTexture2D(const uint8_t* data, int width, int height, int channels) : m_Width(width), m_Height(height), m_Channels(channels)
	{
		VXM_PROFILE_FUNCTION();
		m_TextureSpecification.name = std::to_string(m_TextureSpecification.id);
		VXM_CORE_ASSERT(channels > 0 && channels < 5, "The number of channel {0} is not handle at the moment.", channels);
		m_TextureSpecification.pixelFormat = static_cast<PixelFormat>(channels);
		m_TextureSpecification.pixelType = PX_8;
		m_TextureSpecification.width = width;
		m_TextureSpecification.height = height;
		m_TextureSpecification.channels = channels;
		m_TextureSpecification.image = data;

		CreateTexture();
	}

	OpenGLTexture2D::OpenGLTexture2D(const uint16_t* data, int width, int height, int channels) : m_Width(width), m_Height(height), m_Channels(channels)
	{
		VXM_PROFILE_FUNCTION();
		m_TextureSpecification.name = std::to_string(m_TextureSpecification.id);
		VXM_CORE_ASSERT(channels > 0 && channels < 5, "The number of channel {0} is not handle at the moment.", channels);
		m_TextureSpecification.pixelFormat = static_cast<PixelFormat>(channels);
		m_TextureSpecification.pixelType = PX_16;
		m_TextureSpecification.width = width;
		m_TextureSpecification.height = height;
		m_TextureSpecification.channels = channels;
		m_TextureSpecification.image = data;

		CreateTexture();
	}

	OpenGLTexture2D::OpenGLTexture2D(Texture2DSpecification textureSpecs) : m_TextureSpecification(std::move(textureSpecs))
	{
		VXM_PROFILE_FUNCTION();
		CreateTexture();
	}

	void OpenGLTexture2D::CreateTexture()
	{
		VXM_PROFILE_FUNCTION();
		TexSpecHelper helper(m_TextureSpecification);
		helper.ReadDataFromImage();

		m_Width = m_TextureSpecification.width;
		m_Height = m_TextureSpecification.height;
		m_Channels = m_TextureSpecification.channels;

		VXM_CORE_ASSERT(m_TextureSpecification.channels > 0 && m_TextureSpecification.channels < 5, "The number of channel {0} is not handle at the moment.", m_TextureSpecification.channels);

		GLenum internalFormat = helper.GetInternalFormat();
		GLenum dataFormat = helper.GetFormat();
		GLenum pixelType = helper.GetType();
		const void* data = helper.GetData();
		VXM_CORE_ASSERT(data != nullptr, "No data where found on the image {0}.", m_TextureSpecification.name);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

		//TODO: Add parameter on the Texture API to be able to change this type of parameters.
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, helper.GetMinFilter());
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, helper.GetMagFilter());
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, helper.GetWrapS());
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, helper.GetWrapT());

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, pixelType, helper.GetData());
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