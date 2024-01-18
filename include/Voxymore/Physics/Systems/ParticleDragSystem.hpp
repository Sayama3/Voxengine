//
// Created by ianpo on 05/01/2024.
//

#pragma once

#include "Voxymore/Scene/Systems.hpp"
#include "static_block.hpp"

#define VXM_DEFAULT_DRAG_COEF 0.01f
#define VXM_DEFAULT_DRAG_COEF_SQR 0.02f

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
		Real m_DragCoef = VXM_DEFAULT_DRAG_COEF;
		Real m_DragCoefSqr = VXM_DEFAULT_DRAG_COEF_SQR;
	};

	static_block{ ParticleDragSystem::CreateSystem(); };

} // namespace Voxymore::Core

