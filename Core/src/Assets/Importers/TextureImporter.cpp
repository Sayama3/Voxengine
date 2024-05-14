//
// Created by ianpo on 09/04/2024.
//

#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Math/Image.hpp"
#include "Voxymore/Assets/Importers/TextureImporter.hpp"
#include <stb_image.h>

namespace Voxymore::Core
{
	// cf. std_image to see the extensions supported.
	static std::vector<std::string> Extensions = {".jpeg", ".jpg", ".png", ".bmp", ".hdr", ".psd", ".tga", ".gif", ".pic", ".pgm", ".ppm", ".gif"};

	Ref<Asset> TextureImporter::ImportTexture2D(const AssetMetadata &metadata)
	{
		VXM_PROFILE_FUNCTION();
		return LoadTexture2D(metadata.FilePath);
	}

	Ref<Texture2D> TextureImporter::LoadTexture2D(const std::filesystem::path& path)
	{
		Buffer buffer;
		Texture2DSpecification spec;
		stbi_set_flip_vertically_on_load(true);

		int width, height, channels;
		std::string strPath = path.string();

		if(!FileSystem::Exist(path)) {
			VXM_CORE_ERROR("The file {0} do not exist.", strPath);
			return nullptr;
		}

		{
			VXM_PROFILE_SCOPE("ImportTexture2D - stbi_load");
			buffer.Data = stbi_load(strPath.c_str(), &width, &height, &channels, 0);
		}

		if(!buffer.Data) {
			VXM_CORE_ERROR("Load of image '{0}' failed.\n{1}.", strPath, stbi_failure_reason());
			return nullptr;
		}

		spec.width = width;
		spec.height = height;
		spec.channels = channels;

		//TODO: Handle HDR ?
		buffer.Size = spec.width * spec.height * spec.channels;

		spec.pixelType = PixelType::PX_8;
		spec.pixelFormat = static_cast<PixelFormat>(channels);

		Ref<Texture2D> asset = Texture2D::Create(spec, buffer);
		buffer.Release();
		return asset;
	}

	bool TextureImporter::IsTexture(const std::filesystem::path &path)
	{
		VXM_PROFILE_FUNCTION();
		std::string extension = path.extension().string();

		std::transform(extension.begin(), extension.end(), extension.begin(), [](char c){ return std::tolower(c);});

		return path.has_extension() && std::find(Extensions.begin(), Extensions.end(), extension) != Extensions.end();
	}

	void TextureImporter::ExportEditorTexture2D(const AssetMetadata &metadata, Ref<Texture2D> texture)
	{
		VXM_CORE_INFO("Nothing to export on a texture 2D yet.");
	}

	bool TextureImporter::IsCubemap(const std::filesystem::path& path)
	{
		bool isTex = IsTexture(path);
		if(isTex) {
			int width, height, channel;
			bool stbi_result = stbi_info(path.string().c_str(), &width, &height, &channel);
			if (stbi_result) {
				if (width / 4 == height / 3) {
					return true;
				} else if (width / 3 == height / 4) {
					return true;
				}
			}
		}
		return false;
	}

	void TextureImporter::ExportEditorCubemap(const AssetMetadata& metadata, Ref<Cubemap> cubemap)
	{
		VXM_CORE_INFO("Nothing to export on a cubemap yet.");
	}

	Ref<Asset> TextureImporter::ImportCubemap(const AssetMetadata& metadata)
	{
		VXM_PROFILE_FUNCTION();
		return LoadCubemap(metadata.FilePath);
	}

	Ref<Cubemap> TextureImporter::LoadCubemap(const std::filesystem::path &path)
	{
		VXM_PROFILE_FUNCTION();
		Image source;
		std::array<Image, 6> images;
		CubemapSpecification spec;
		stbi_set_flip_vertically_on_load(true);

		int width, height, channels;
		std::string strPath = path.string();

		if(!FileSystem::Exist(path)) {
			VXM_CORE_ERROR("The file {0} do not exist.", strPath);
			return nullptr;
		}

		uint8_t* raw = nullptr;
		{
			VXM_PROFILE_SCOPE("ImportTexture2D - stbi_load");
			raw = stbi_load(strPath.c_str(), &width, &height, &channels, 0);
		}

		if(!raw) {
			VXM_CORE_ERROR("Load of cubemap '{0}' failed.\n{1}.", strPath, stbi_failure_reason());
			return nullptr;
		}

		//TODO: Handle HDR ?
		source.Reset(raw, width, height, channels);
		raw = nullptr;

		spec.channels = channels;
		spec.pixelType = PixelType::PX_8;
		spec.pixelFormat = static_cast<PixelFormat>(channels);

		uint32_t textureSize = 0;
		if(width > height) {
			if (width / 4 == height / 3) {
				textureSize = height / 3;

				images[Cubemap::Top   ] = source.Splice(textureSize * 1, textureSize * 0, textureSize, textureSize);
				images[Cubemap::Left  ] = source.Splice(textureSize * 0, textureSize * 1, textureSize, textureSize);
				images[Cubemap::Front ] = source.Splice(textureSize * 1, textureSize * 1, textureSize, textureSize);
				images[Cubemap::Right ] = source.Splice(textureSize * 2, textureSize * 1, textureSize, textureSize);
				images[Cubemap::Back  ] = source.Splice(textureSize * 3, textureSize * 1, textureSize, textureSize);
				images[Cubemap::Bottom] = source.Splice(textureSize * 1, textureSize * 2, textureSize, textureSize);

			} else {
				VXM_CORE_ERROR("Invalid cubemap dimensions: {0}x{1}.", width, height);
				return nullptr;
			}
		} else /*width < height*/{
			if (width / 3 == height / 4) {
				textureSize = height / 4;

				images[Cubemap::Top   ] = source.Splice(textureSize * 1, textureSize * 0, textureSize, textureSize);
				images[Cubemap::Left  ] = source.Splice(textureSize * 0, textureSize * 1, textureSize, textureSize);
				images[Cubemap::Front ] = source.Splice(textureSize * 1, textureSize * 1, textureSize, textureSize);
				images[Cubemap::Right ] = source.Splice(textureSize * 2, textureSize * 1, textureSize, textureSize);
				images[Cubemap::Back  ] = source.Splice(textureSize * 1, textureSize * 3, textureSize, textureSize);
				images[Cubemap::Bottom] = source.Splice(textureSize * 1, textureSize * 2, textureSize, textureSize);
			} else {
				VXM_CORE_ERROR("Invalid cubemap dimensions: {0}x{1}.", width, height);
				return nullptr;
			}
		}
		spec.width = textureSize;
		spec.height = textureSize;

		Ref<Cubemap> cm = Cubemap::Create(spec);
		cm->SetData(images);

		return cm;
	}
}