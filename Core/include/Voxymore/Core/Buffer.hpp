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
		Buffer(void*, uint64_t size);
		Buffer(uint64_t size);

		static Buffer Copy(const Buffer& other);

		void Allocate(uint64_t size);
		void Release();
		void Clear();

		template<typename T>
		T* As()
		{
			VXM_CORE_ASSERT(Data, "No data inside the buffer");
			VXM_CORE_ASSERT(Size >= sizeof(T), "The buffer is not big enough to be cast as '{0}'", typeid(T).name());
			return (T*)Data;
		}

		template<typename T>
		const T* As() const
		{
			VXM_CORE_ASSERT(Data, "No data inside the buffer");
			VXM_CORE_ASSERT(Size >= sizeof(T), "The buffer is not big enough to be cast as '{0}'", typeid(T).name());
			return (T*)Data;
		}

		template<typename T>
		T& At(uint64_t index)
		{
			VXM_CORE_ASSERT(Data, "No data inside the buffer");
			VXM_CORE_ASSERT(Size >= sizeof(T), "The buffer is not big enough to be cast as '{0}'", typeid(T).name());
			VXM_CORE_ASSERT((index+1) * sizeof(T) >= Size, "The array is not big enough for the index {}.", index);
			return ((T*)Data)[index];
		}

		template<typename T>
		const T& At(uint64_t index) const
		{
			VXM_CORE_ASSERT(Data, "No data inside the buffer");
			VXM_CORE_ASSERT(Size >= sizeof(T), "The buffer is not big enough to be cast as '{0}'", typeid(T).name());
			VXM_CORE_ASSERT((index+1) * sizeof(T) >= Size, "The array is not big enough for the index {}.", index);
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

		// Will take and clear the buffer given in parameters.
		void Replace(Buffer&& buff);
		// Will take and clear the buffer given in parameters.
		void Replace(Buffer& buff);

		void Replace(void* ptr, uint64_t size);

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

		~ScopeBuffer();
	private:
		Buffer buffer;
	};

} // namespace Voxymore::Core

