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


	void Buffer::Clear()
	{
		VXM_PROFILE_FUNCTION();
		Data = nullptr;
		Size = 0;
	}


	void Buffer::swap(Buffer& other)
	{
		std::swap(Data, other.Data);
		std::swap(Size, other.Size);
	}

	Buffer Buffer::Copy(const Buffer & other)
	{
		VXM_PROFILE_FUNCTION();
		Buffer buff(other.Size);
		memcpy(buff.Data, other.Data, other.Size);
		return buff;
	}

	ScopeBuffer::ScopeBuffer()
	{
	}

	ScopeBuffer::ScopeBuffer(void* ptr, uint64_t size) : buffer(ptr, size)
	{

	}
	ScopeBuffer::ScopeBuffer(uint64_t size) : buffer(size)
	{

	}

	ScopeBuffer::ScopeBuffer(Buffer&& buffer) : buffer(buffer)
	{
		buffer.Clear();
	}

	ScopeBuffer::ScopeBuffer(Buffer& buffer) : buffer(buffer)
	{
		buffer.Clear();
	}

	ScopeBuffer::ScopeBuffer(ScopeBuffer&& other) noexcept
	{
		swap(other);
	}

	ScopeBuffer& ScopeBuffer::operator=(ScopeBuffer&& other) noexcept
	{
		swap(other);
		return *this;
	}

	void ScopeBuffer::swap(ScopeBuffer& other)
	{
		std::swap(buffer, other.buffer);
	}

	void ScopeBuffer::Replace(Buffer&& buff)
	{
		buffer.Release();
		buffer = buff;
		buff.Clear();
	}

	void ScopeBuffer::Replace(Buffer& buff)
	{
		buffer.Release();
		buffer = buff;
		buff.Clear();
	}


	void ScopeBuffer::Replace(void* ptr, uint64_t size)
	{
		buffer.Release();
		buffer.Data = static_cast<uint8_t *>(ptr);
		buffer.Size = size;
	}

	ScopeBuffer::~ScopeBuffer()
	{
		buffer.Release();
	}

}// namespace Voxymore::Core