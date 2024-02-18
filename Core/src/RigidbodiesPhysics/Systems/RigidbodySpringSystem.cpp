//
// Created by ianpo on 18/02/2024.
//

#include "Voxymore/RigidbodiesPhysics/Systems/RigidbodySpringSystem.hpp"
#include "Voxymore/RigidbodiesPhysics/Components/RigidbodySpringComponent.hpp"


namespace Voxymore::Core
{
	VXM_DECLARE_SYSTEM(RigidbodySpringSystem);

	void RigidbodySpringSystem::Update(Scene &scene, TimeStep ts)
	{
		VXM_PROFILE_FUNCTION();
		auto func = [&](auto entity, RigidbodySpringComponent& asc, TransformComponent& tc)
		{
			Vec3 anchorPos = tc.GetWorldPoint(asc.LocalPosition);
			auto current = Entity(entity, &scene);
			for(auto& field : asc.EntitiesConnected)
			{
				auto e = field.Entity.GetEntity(scene);
				if(!e.HasComponent<RigidbodyComponent>()) continue;

				Vec3 posE = e.GetComponent<TransformComponent>().GetWorldPoint(field.BodyPosition);
				Vec3 springForce = posE - anchorPos;
				float magnitude = Math::Magnitude(springForce);
				if(magnitude <= asc.RestLength) {
					return;
				}

				float forceMagnitude = (asc.RestLength - magnitude) * asc.SpringConstant;

				Vec3 force = Math::Normalize(springForce) * (forceMagnitude);
				e.GetComponent<RigidbodyComponent>().AddForceAtPoint(force, posE);
			}
		};
		scene.each<RigidbodySpringComponent, TransformComponent>(exclude<DisableComponent>, func);
	}
} // namespace Voxymore::Core
