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

		template<typename T>
		T* As()
		{
			VXM_CORE_ASSERT(Data, "No data inside the buffer");
			VXM_CORE_CHECK(Size >= sizeof(T), "The buffer is not big enough to be cast as '{0}'", typeid(T).name());
			return (T*)Data;
		}

		template<typename T>
		const T* As() const
		{
			VXM_CORE_ASSERT(Data, "No data inside the buffer");
			VXM_CORE_CHECK(Size >= sizeof(T), "The buffer is not big enough to be cast as '{0}'", typeid(T).name());
			return (T*)Data;
		}

		uint8_t* Data = nullptr;
		uint64_t Size = 0;
	};

} // namespace Voxymore::Core

