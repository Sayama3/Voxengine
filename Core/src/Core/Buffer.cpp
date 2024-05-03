//
// Created by ianpo on 09/04/2024.
//

#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Core/Buffer.hpp"

namespace Voxymore::Core
{

	Buffer::Buffer(void * data, uint64_t size) : Data((uint8_t*)data), Size(size)
	{
	}

	Buffer::Buffer(uint64_t size)
	{
		Allocate(size);
	}

	void Buffer::Allocate(uint64_t size)
	{
		VXM_PROFILE_FUNCTION();
		Release();

		Data = (uint8_t*)malloc(size);
		Size = size;
	}

	void Buffer::Release()
	{
		VXM_PROFILE_FUNCTION();
		free(Data);
		Data = nullptr;
		Size = 0;
	}

	Buffer Buffer::Copy(const Buffer & other)
	{
		VXM_PROFILE_FUNCTION();
		Buffer buff(other.Size);
		memcpy(buff.Data, other.Data, other.Size);
		return buff;
	}
}// namespace Voxymore::Core