//
// Created by ianpo on 09/04/2024.
//

#pragma once

#include <Voxymore/Core/Macros.hpp>
#include <cinttypes>

namespace Voxymore::Core
{

	struct Buffer {
		inline Buffer() = default;
		inline ~Buffer() = default;
		inline Buffer(const Buffer&) = default;
		inline Buffer& operator=(const Buffer&) = default;
		Buffer(void*, uint64_t size);
		explicit Buffer(uint64_t size);

		static Buffer Copy(const Buffer& other);

		void Allocate(uint64_t size);
		void Release();
		void Clear();

		void swap(Buffer& other);

		template<typename T>
		T* As()
		{
			VXM_CORE_ASSERT(Data, "Buffer is empty - has no initialized data.");
			VXM_CORE_ASSERT(Size >= sizeof(T), "Buffer cannot be cast to {0} - insufficient size. Required size: {1}. Current size: {2}.", typeid(T).name(), sizeof(T), Size);
			return (T*)Data;
		}

		template<typename T>
		const T* As() const
		{
			VXM_CORE_ASSERT(Data, "Buffer is empty - has no initialized data.");
			VXM_CORE_ASSERT(Size >= sizeof(T), "Buffer cannot be cast to {0} - insufficient size. Required size: {1}. Current size: {2}.", typeid(T).name(), sizeof(T), Size);
			return (T*)Data;
		}

		template<typename T>
		T& At(uint64_t index)
		{
			VXM_CORE_ASSERT(Data, "Buffer is empty - has no initialized data.");
			VXM_CORE_ASSERT(Size >= (index+1) * sizeof(T), "Cannot access index {0} for type {1}. Buffer size: {2}. Required size: {3}.",
							index,
							typeid(T).name(),
							Size,
							(index+1) * sizeof(T));
			return ((T*)Data)[index];
		}

		template<typename T>
		const T& At(uint64_t index) const
		{
			VXM_CORE_ASSERT(Data, "Buffer is empty - has no initialized data.");
			VXM_CORE_ASSERT(Size >= (index+1) * sizeof(T), "Cannot access index {0} for type {1}. Buffer size: {2}. Required size: {3}.",
							index,
							typeid(T).name(),
							Size,
							(index+1) * sizeof(T));
			return ((T*)Data)[index];
		}

		uint8_t* Data = nullptr;
		uint64_t Size = 0;
	};

	struct ScopeBuffer {
		ScopeBuffer();
		ScopeBuffer(void*, uint64_t size);
		ScopeBuffer(uint64_t size);
		// Will take and clear the buffer given in parameters.
		ScopeBuffer(Buffer&& buffer);
		// Will take and clear the buffer given in parameters.
		ScopeBuffer(Buffer& buffer);

		ScopeBuffer(const ScopeBuffer& other) = delete;
		ScopeBuffer& operator=(const ScopeBuffer& other) = delete;

		ScopeBuffer(ScopeBuffer&& other) noexcept;
		ScopeBuffer& operator=(ScopeBuffer&& other) noexcept;

		void swap(ScopeBuffer& other);

		// Will take and clear the buffer given in parameters.
		void Replace(Buffer&& buff);
		// Will take and clear the buffer given in parameters.
		void Replace(Buffer& buff);

		void Replace(void* ptr, uint64_t size);

		[[nodiscard]] ScopeBuffer Duplicate() const;

		template<typename T>
		T* As()
		{
			return buffer.As<T>();
		}

		template<typename T>
		const T* As() const
		{
			return buffer.As<T>();
		}

		void* GetPtr() {return buffer.Data;}
		const void* GetPtr() const {return buffer.Data;}

		uint64_t GetSize() const {return buffer.Size;}

		~ScopeBuffer();
	private:
		Buffer buffer;
	};

} // namespace Voxymore::Core

