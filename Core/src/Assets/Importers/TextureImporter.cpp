//
// Created by ianpo on 09/04/2024.
//

#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Assets/Importers/TextureImporter.hpp"
#include "Voxymore/Renderer/Texture.hpp"
#include <stb_image.h>

namespace Voxymore::Core
{
	Ref<Asset> TextureImporter::ImportTexture2D(const AssetMetadata &metadata)
	{
		VXM_PROFILE_FUNCTION();
		Buffer buffer;
		Texture2DSpecification spec;
		stbi_set_flip_vertically_on_load(true);

		int width, height, channels;
		std::string strPath = metadata.FilePath.string();

		if(!FileSystem::Exist(metadata.FilePath)) {
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

		Ref<Asset> asset = Texture2D::Create(spec, buffer);
		buffer.Release();
		return asset;
	}
}