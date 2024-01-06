//
// Created by ianpo on 06/01/2024.
//

#include "Voxymore/Physics/Systems/SpringForceSystem.hpp"
#include "Voxymore/Physics/Components/ParticleComponent.hpp"


namespace Voxymore::Core
{
	VXM_DECLARE_SYSTEM(SpringForceSystem);
	void SpringForceSystem::Update(Scene& scene, TimeStep ts)
	{
		VXM_PROFILE_FUNCTION();
		scene.each<AnchoredSpringComponent, TransformComponent>(exclude<DisableComponent>, [&](auto entity, AnchoredSpringComponent& asc, TransformComponent& tc)
		{
			VXM_PROFILE_FUNCTION();
			Vec3 anchorPos = tc.GetPosition();
			auto current = Entity(entity, &scene);
			for(auto& field : asc.EntitiesConnected)
			{
				auto e = field.GetEntity(scene);
				if(!e.HasComponent<ParticleComponent>()) continue;

				Vec3 posE = e.GetComponent<TransformComponent>().GetPosition();
				Vec3 currToE = posE - anchorPos;
				float distance = Math::Magnitude(currToE);
				float forceLength = (asc.RestLength - distance) * asc.SpringConstant;

				Vec3 force = Math::Normalize(currToE) * (-forceLength);
				e.GetComponent<ParticleComponent>().AddForce(force);
			}
		});
	}
} // namespace Voxymore::Core
