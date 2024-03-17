//
// Created by ianpo on 16/02/2024.
//

#pragma once

#include "RigidbodyContact.hpp"
#include "Voxymore/Core/TimeStep.hpp"
#include "Voxymore/Math/Math.hpp"

namespace Voxymore::Core
{

		class RigidbodyContactResolver
		{
			protected:
			uint32_t iterations;
			uint32_t iterationsUsed;
		public:
			RigidbodyContactResolver();
			RigidbodyContactResolver(uint32_t iterations);
			inline ~RigidbodyContactResolver() = default;

			void SetIterations(uint32_t iterations);

			void ResolveContacts(TimeStep ts, std::vector<RigidbodyContact>& contacts);
		};

} // namespace Voxymore::Core

