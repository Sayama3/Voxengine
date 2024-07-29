//
// Created by ianpo on 16/02/2024.
//

#pragma once

#include "RigidbodyContact.hpp"
#include "Voxymore/Core/TimeStep.hpp"
#include "Voxymore/Math/Math.hpp"

namespace Voxymore::Core
{
	class RBContactResolver
	{
	protected:
		uint32_t m_Iterations;
		uint32_t m_PositionIterationsUsed;
		uint32_t m_VelocityIterationsUsed;

		/**
		 * The minimum penetration to be considered an interpenetration.
		 */
		const Real m_ContactEpsilon = 0.01;
	public:
		RBContactResolver();
		RBContactResolver(uint32_t iterations);
		inline ~RBContactResolver() = default;

		void SetIterations(uint32_t iterations);

		void PrepareContacts(TimeStep ts, std::vector<RigidbodyContact>& contacts);
		void ResolveContacts(TimeStep ts, std::vector<RigidbodyContact>& contacts);
	private:
		void AdjustPositions(TimeStep ts, std::vector<RigidbodyContact>& contacts);
		void AdjustVelocities(TimeStep ts, std::vector<RigidbodyContact>& contacts);
	};

} // namespace Voxymore::Core

