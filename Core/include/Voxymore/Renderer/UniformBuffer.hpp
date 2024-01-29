//
// Created by ianpo on 29/08/2023.
//

#pragma once

#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Core/SmartPointers.hpp"

namespace Voxymore::Core
{

	class UniformBuffer
	{
	public:
		virtual ~UniformBuffer() {}
		virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) = 0;

		static Ref<UniformBuffer> Create(uint32_t size, uint32_t binding);
	};

} // namespace Voxymore::Core

