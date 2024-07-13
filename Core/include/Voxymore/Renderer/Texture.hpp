//
// Created by ianpo on 29/07/2023.
//

#pragma once

#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Core/SmartPointers.hpp"
#include "Voxymore/Core/FileSystem.hpp"
#include "Voxymore/Core/UUID.hpp"
#include "Voxymore/Core/Buffer.hpp"
#include "Voxymore/Assets/Asset.hpp"
#include "Voxymore/Assets/AssetField.hpp"

namespace Voxymore::Core {

	enum TextureFilter : uint8_t
	{
		Nearest,
		Linear,
	};

	enum TextureWrapper : uint8_t
	{
		Repeat,
		ClampToEdge,
		MirroredRepeat,
	};

	enum PixelFormat : uint8_t
	{
		RED = 1,
		RG = 2,
		RGB = 3,
		RGBA = 4,
		STENCIL,
		DEPTH,
		DEPTH_STENCIL,
	};

	enum PixelType : uint8_t
	{
		PX_8,
		PX_16,

		PX_8UI,
		PX_16UI,
		PX_32UI,

		PX_8I,
		PX_16I,
		PX_32I,

		PX_16F, // This is a Half Float.
		PX_32F,
	};

	struct Texture2DSpecification
	{
		inline Texture2DSpecification() = default;
		inline ~Texture2DSpecification() = default;

		uint32_t width = 0, height = 0, channels = 0;

		TextureFilter filterMag = TextureFilter::Linear;
		TextureFilter filterMin = TextureFilter::Linear;

		TextureWrapper wrapperS = TextureWrapper::Repeat;
		TextureWrapper wrapperT = TextureWrapper::Repeat;

		PixelFormat pixelFormat = PixelFormat::RGBA;
		PixelType pixelType = PixelType::PX_8;

		bool generateMipMaps = true;
	};

	struct Texture2DArraySpecification
	{
		inline Texture2DArraySpecification() = default;
		inline ~Texture2DArraySpecification() = default;

		uint32_t width = 0, height = 0, channels = 0, count = 0;

		TextureFilter filterMag = TextureFilter::Linear;
		TextureFilter filterMin = TextureFilter::Linear;

		TextureWrapper wrapperS = TextureWrapper::Repeat;
		TextureWrapper wrapperT = TextureWrapper::Repeat;

		PixelFormat pixelFormat = PixelFormat::RGBA;
		PixelType pixelType = PixelType::PX_8;
	};

    class Texture : public Asset
	{
    public:
        virtual ~Texture() = default;
        [[nodiscard]] virtual uint32_t GetWidth() const = 0;
        [[nodiscard]] virtual uint32_t GetHeight() const = 0;
        [[nodiscard]] virtual uint32_t GetDepth() const = 0;

		virtual void SetData(Buffer data) = 0;

        virtual void Bind(uint32_t slot = 0) const = 0;
        static void Unbind(uint32_t slot = 0);
		[[nodiscard]] virtual uint32_t GetRendererID() const = 0;

		//TODO: Add a 'T GetPixel<T>(uint32_t index) const;' function.
		//TODO: Add a 'std::vector<T> GetPixels<T>() const;' function.

		//TODO: Add a 'void SetPixel<T>(uint32_t index, T pixel);' function.
		//TODO: Add a 'void SetPixels<T>(std::vector<T> pixels);' function.

    };

    class Texture2D : public Texture {
    public:
		static Ref<Texture2D> Create(const Texture2DSpecification& textureSpecs);
		static Ref<Texture2D> Create(const Texture2DSpecification& textureSpecs, Buffer buffer);
		static Ref<Texture2D> Create(const uint8_t* data, int width, int height, int channels);
		static Ref<Texture2D> Create(const uint16_t* data, int width, int height, int channels);

		[[nodiscard]] virtual uint32_t GetChannelCount() const = 0;
		//TODO: Add a 'T GetPixel<T>(uint32_t x, uint32_t y) const;' function.
		//TODO: Add a 'void SetPixel<T>(uint32_t x, uint32_t y, T& pixel);' function.

		VXM_IMPLEMENT_ASSET(AssetType::Texture2D);
    };

    class Texture2DArray : public Texture {
    public:
		static Ref<Texture2DArray> Create(const Texture2DArraySpecification& textureSpecs);
		static Ref<Texture2DArray> Create(const Texture2DArraySpecification& textureSpecs, Buffer buffer);

		[[nodiscard]] virtual uint32_t GetChannelCount() const = 0;
		virtual void SetData(uint32_t index, Buffer data) = 0;
		virtual void SetData(const std::vector<Buffer> data) = 0;
		template<uint32_t N>
		void SetData(const std::array<Buffer, N>& data) {
			VXM_PROFILE_FUNCTION();
			VXM_CORE_CHECK(data.size() > GetDepth(), "Cannot send {} image. Only {} available.", data.size(), GetDepth());
			for (uint32_t i = 0; i < std::min(N, GetDepth()); i++) {
				SetData(i, data[i]);
			}
		}
		//TODO: Add a 'T GetPixel<T>(uint32_t x, uint32_t y, uint32_t index) const;' function.
		//TODO: Add a 'void SetPixel<T>(uint32_t x, uint32_t y, uint32_t index, T& pixel);' function.

		VXM_IMPLEMENT_ASSET(AssetType::Texture2DArray);
    };

	using Texture2DField = AssetField<Texture2D>;
	using Texture2DArrayField = AssetField<Texture2DArray>;
} // Core
