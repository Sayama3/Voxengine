//
// Created by ianpo on 26/01/2024.
//

#pragma once

#include "ParticleContact.hpp"
#include "Voxymore/Core/TimeStep.hpp"
#include "Voxymore/Math/Math.hpp"

namespace Voxymore::Core
{

	class ParticleContactResolver
	{
	protected:
		uint32_t iterations;
		uint32_t iterationsUsed;
	public:
		ParticleContactResolver();
		ParticleContactResolver(uint32_t iterations);
		inline ~ParticleContactResolver() = default;

		void SetIterations(uint32_t iterations);

		void ResolveContacts(TimeStep ts, std::vector<ParticleContact>& contacts);
	};

} // namespace Voxymore::Core

