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
		Texture2DSpecification spec;
		stbi_set_flip_vertically_on_load(true);

		int width, height, channels;
		std::string strPath = path.string();
		VXM_CORE_ASSERT(FileSystem::Exist(metadata.), "The file {0} do not exist...", strPath);

		spec.image = stbi_load(strPath.c_str(), &width, &height, &channels, static_cast<int>(spec.channels));
		VXM_CORE_ASSERT(spec.image, "Load of image '{0}' failed.\n{1}.", strPath, stbi_failure_reason());

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