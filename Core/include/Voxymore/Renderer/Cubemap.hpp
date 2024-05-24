//
// Created by Iannis on 10/05/2024.
//

#pragma once

#include "Voxymore/Assets/Asset.hpp"
#include "Voxymore/Renderer/Texture.hpp"
#include "Voxymore/Math/Image.hpp"

namespace Voxymore::Core
{
	struct CubemapSpecification
	{
		inline CubemapSpecification() = default;
		inline ~CubemapSpecification() = default;

		uint32_t width = 0, height = 0, channels = 0;

		TextureFilter filterMag = TextureFilter::Linear;
		TextureFilter filterMin = TextureFilter::Linear;

		TextureWrapper wrapperS = TextureWrapper::ClampToEdge;
		TextureWrapper wrapperT = TextureWrapper::ClampToEdge;
		TextureWrapper wrapperR = TextureWrapper::ClampToEdge;

		PixelFormat pixelFormat = PixelFormat::RGB;
		PixelType pixelType = PixelType::PX_8;
	};


	class Cubemap : public Asset
	{
	public:
		friend class TextureImporter;
		enum Side : uint8_t
		{
			Right,
			Left,
			Top,
			Bottom,
			Front,
			Back,
		};
	public:
		VXM_IMPLEMENT_ASSET(AssetType::CubeMap);

		static Ref<Cubemap> Create(const CubemapSpecification& textureSpecs);
		virtual ~Cubemap() {}

		virtual void Bind(uint32_t slot = 0) = 0;
		[[nodiscard]] virtual uint32_t GetRendererID() const = 0;

		virtual void SetData(Side side, const Image& image) = 0;
		virtual void SetData(const std::array<Image, 6>& images) = 0;
	};

	using CubemapField = AssetField<Cubemap>;
} // namespace Voxymore::Core

