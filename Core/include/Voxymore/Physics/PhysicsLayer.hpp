//
// Created by ianpo on 05/01/2024.
//

#pragma once

#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Core/SmartPointers.hpp"
#include "Voxymore/Core/Math.hpp"

#include "Voxymore/Layers/Layer.hpp"
#include "Voxymore/Events/MouseEvent.hpp"
#include "Voxymore/Events/KeyEvent.hpp"
#include "Voxymore/Events/ApplicationEvent.hpp"

#include "Voxymore/Core/FileSystem.hpp"
#include "Voxymore/Scene/Scene.hpp"

#include "Voxymore/Physics/Particles/Collisions/ParticleContact.hpp"
#include "Voxymore/Physics/Particles/Collisions/ParticleContactResolver.hpp"

#include "Voxymore/Physics/Rigidbodies/Components/RigidbodyComponent.hpp"
#include "Voxymore/Physics/Rigidbodies/Systems/RBGravitySystem.hpp"
namespace Voxymore::Core
{

	class PhysicsLayer : public Layer
	{
	public:
		PhysicsLayer();
		~PhysicsLayer() override;

		virtual void OnUpdate(TimeStep ts) override;
	public:
		void SetScene(Ref<Scene> scene);
		void ResetScene();
	public:
		const Vec3& GetGravity() const;
		void SetGravity(const Vec3& g);

		void AddContact(const ParticleContact& contact);
		void AddContacts(const std::vector<ParticleContact>& contacts);
	private:
		[[nodiscard]] bool HasScene() const;
	private:
		Vec3 m_Gravity = Vec3(0.0, -9.8, 0.0);
		Ref<Scene> m_SceneHandle = nullptr;
		std::vector<ParticleContact> m_Contacts;
		ParticleContactResolver m_Resolver;
	};

} // namespace Voxymore::Core

