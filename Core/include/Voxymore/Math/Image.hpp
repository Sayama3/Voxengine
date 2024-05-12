//
// Created by ianpo on 12/05/2024.
//

#pragma once

#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Core/Buffer.hpp"

namespace Voxymore::Core
{
	class Image {
	public:
		Image() = default;
		Image(void* source, uint32_t w, uint32_t h, uint32_t c);
		Image(uint32_t w, uint32_t h, uint32_t c);
		Image(Image&& image);
		~Image();

		Image& operator=(Image&& other) noexcept;
		void Reset(void* source, uint32_t w, uint32_t h, uint32_t c);

		void Allocate(uint32_t w, uint32_t h, uint32_t c);
		void Release();

		uint8_t& operator()(uint32_t x, uint32_t y, uint32_t c);
		const uint8_t& operator()(uint32_t x, uint32_t y, uint32_t c) const;

		[[nodiscard]] Image Splice(uint32_t offsetX, uint32_t offsetY, uint32_t newWidth, uint32_t newHeight) const;
	public:
		Buffer source = Buffer();
		uint32_t width = 0, height = 0, channels = 0;
	};

} // namespace Voxymore::Core

