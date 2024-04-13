//
// Created by ianpo on 09/04/2024.
//

#include "Voxymore/Core/Core.hpp"
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
}