//
// Created by ianpo on 16/02/2024.
//

#pragma once

#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Core/SmartPointers.hpp"
#include "Voxymore/Math/Math.hpp"
#include "Voxymore/Math/BoundingObject.hpp"
#include "Voxymore/Math/BoundingSphere.hpp"

#include "Voxymore/Layers/Layer.hpp"
#include "Voxymore/Events/MouseEvent.hpp"
#include "Voxymore/Events/KeyEvent.hpp"
#include "Voxymore/Events/ApplicationEvent.hpp"

#include "Voxymore/Core/FileSystem.hpp"
#include "Voxymore/Scene/Scene.hpp"

#include "Voxymore/RigidbodiesPhysics/Collisions/BroadCollisions.hpp"
#include "Voxymore/RigidbodiesPhysics/Collisions/RBContactResolver.hpp"
#include "Voxymore/RigidbodiesPhysics/Collisions/RigidbodyContact.hpp"
#include "Voxymore/RigidbodiesPhysics/Components/RigidbodyComponent.hpp"
#include "Voxymore/RigidbodiesPhysics/Primitive.hpp"


namespace Voxymore::Core
{

	class RigidbodyPhysicsLayer : public Layer
	{
	public:
		RigidbodyPhysicsLayer();
		~RigidbodyPhysicsLayer() override;

		virtual void OnUpdate(TimeStep ts) override;
		virtual void OnAttach() override;
		virtual void OnDetach() override;
	public:
		void SetScene(Ref<Scene> scene);
		void ResetScene();
	public:
		const Vec3& GetGravity() const;
		void SetGravity(const Vec3& g);

		void AddContact(const RigidbodyContact & contact);
		void AddContacts(const std::vector<RigidbodyContact>& contacts);
	private:
		[[nodiscard]] bool HasScene() const;

		void Integrate(TimeStep ts);
		bool BroadCollisionCheck(TimeStep ts);
		bool FineCollisionCheck(TimeStep ts);
		void CollisionResolution(TimeStep ts);
	private:
		Vec3 m_Gravity = Vec3(0.0, -9.8, 0.0);
		Ref<Scene> m_SceneHandle = nullptr;
		std::vector<PotentialContact> m_PotentialContacts {};
		CollisionData m_Contacts;
		RBContactResolver m_Resolver;
		BVHNode<BoundingSphere>* m_Root = nullptr;
	};

} // namespace Voxymore::Core

