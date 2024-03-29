//
// Created by ianpo on 06/01/2024.
//

#include "Voxymore/ParticlesPhysics/Systems/SpringForceSystem.hpp"
#include "Voxymore/ParticlesPhysics/Components/ParticleComponent.hpp"


namespace Voxymore::Core
{
	VXM_DECLARE_SYSTEM(SpringForceSystem);
	void SpringForceSystem::Update(Scene& scene, TimeStep ts)
	{
		VXM_PROFILE_FUNCTION();
		auto func = [&](auto entity, AnchoredSpringComponent& asc, TransformComponent& tc)
		{
			VXM_PROFILE_FUNCTION();
			Vec3 anchorPos = tc.GetPosition();
			auto current = Entity(entity, &scene);
			for(auto& field : asc.EntitiesConnected)
			{
				auto e = field.GetEntity(scene);
				if(!e.HasComponent<ParticleComponent>()) continue;

				Vec3 posE = e.GetComponent<TransformComponent>().GetPosition();
				Vec3 springForce = posE - anchorPos;
				float magnitude = Math::Magnitude(springForce);
				if(magnitude <= asc.RestLength) {
					return;
				}

				float forceMagnitude = (asc.RestLength - magnitude) * asc.SpringConstant;

				Vec3 force = Math::Normalize(springForce) * (forceMagnitude);
				e.GetComponent<ParticleComponent>().AccumulateForce(force);
			}
		};
		scene.each<AnchoredSpringComponent, TransformComponent>(exclude<DisableComponent>, func);
	}
} // namespace Voxymore::Core
