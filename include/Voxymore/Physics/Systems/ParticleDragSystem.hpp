//
// Created by ianpo on 05/01/2024.
//

#pragma once

#include "Voxymore/Scene/Systems.hpp"
#include "static_block.hpp"

namespace Voxymore::Core
{

	class ParticleDragSystem : public System
	{
		VXM_IMPLEMENT_SYSTEM(ParticleDragSystem);
	public:
		ParticleDragSystem() = default;
		~ParticleDragSystem() = default;
	protected:
		virtual void DeserializeSystem(YAML::Node& node) override;
		virtual void SerializeSystem(YAML::Emitter& out) override;
		virtual void ResetSystem() override;
	public:
		inline ParticleDragSystem(Real dragCoef, Real dragCoefSqr) : m_DragCoef(dragCoef), m_DragCoefSqr(dragCoefSqr) {}
		virtual bool OnImGuiRender() override;
		virtual void Update(Scene& scene, TimeStep ts) override;

		inline virtual bool RunOnAllScenes() override { return true; }
	private:
		Real m_DragCoef = 0.9;
		Real m_DragCoefSqr = 0.8;
	};

	static_block{ ParticleDragSystem::CreateSystem(); };

} // namespace Voxymore::Core

