//
// Created by ianpo on 12/05/2024.
//

#include "Voxymore/Math/Image.hpp"


namespace Voxymore::Core
{
	void Image::Release()
	{
		source.Release();
		width = 0;
		height = 0;
		channels = 0;
	}
	void Image::Allocate(uint32_t w, uint32_t h, uint32_t c)
	{
		source.Allocate(w * h * c);
		width = w;
		height = h;
		channels = c;
	}
	const uint8_t &Image::operator()(uint32_t x, uint32_t y, uint32_t c) const
	{
		return source.At<uint8_t>(y * width * channels + x * channels + c);
	}
	uint8_t &Image::operator()(uint32_t x, uint32_t y, uint32_t c)
	{
		return source.At<uint8_t>(y * width * channels + x * channels + c);
	}

	void Image::Reset(void* s, uint32_t w, uint32_t h, uint32_t c)
	{
		source.Release();
		source.Data = static_cast<uint8_t *>(s);
		source.Size = w * h * c;
		width = w;
		height = h;
		channels = c;
	}

	Image& Image::operator=(Image&& other)
 noexcept 	{
		// Release previous image
		source.Release();

		// Take the image from the other
		source = other.source;
		width = other.width;
		height = other.height;
		channels = other.channels;

		// Clear the other
		other.source.Clear();
		other.width = 0;
		other.height = 0;
		other.channels = 0;

		return *this;
	}
	Image::~Image()
	{
		Release();
	}

	Image::Image(uint32_t w, uint32_t h, uint32_t c)
			: source(w*h*c), width(w), height(h), channels(c)
	{
	}

	Image::Image(void* s, uint32_t w, uint32_t h, uint32_t c)
			: source(s, w * h * c), width(w), height(h), channels(c)
	{
	}

	Image::Image(const Image& other) : source(Buffer::Copy(other.source)), width(other.width), height(other.height), channels(other.channels)
	{
	}

	Image::Image(Image&& other) noexcept : source(other.source), width(other.width), height(other.height), channels(other.channels)
	{
		other.source.Clear();
		other.width = 0;
		other.height = 0;
		other.channels = 0;
	}

	Image Image::Splice(uint32_t offsetX, uint32_t offsetY, uint32_t newWidth, uint32_t newHeight) const
	{
		VXM_PROFILE_FUNCTION();
		Image result(newWidth, newHeight, channels);

		for (uint32_t x = 0; x < newWidth; ++x) {
			auto offX = offsetX + x;
			for (uint32_t y = 0; y < newHeight; ++y) {
				auto offY = offsetY + y;
				for (uint32_t c = 0; c < channels; ++c) {
					result(x,y,c) = operator()(offX, offY, c);
				}
			}
		}

		return result;
	}
} // namespace Voxymore::Core
