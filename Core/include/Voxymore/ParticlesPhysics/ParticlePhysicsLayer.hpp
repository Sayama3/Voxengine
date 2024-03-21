//
// Created by ianpo on 05/01/2024.
//

#pragma once

#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Core/SmartPointers.hpp"
#include "Voxymore/Math/Math.hpp"

#include "Voxymore/Layers/Layer.hpp"
#include "Voxymore/Events/MouseEvent.hpp"
#include "Voxymore/Events/KeyEvent.hpp"
#include "Voxymore/Events/ApplicationEvent.hpp"

#include "Voxymore/Core/FileSystem.hpp"
#include "Voxymore/Scene/Scene.hpp"

#include "Voxymore/ParticlesPhysics/Collisions/ParticleContact.hpp"
#include "Voxymore/ParticlesPhysics/Collisions/ParticleContactResolver.hpp"
namespace Voxymore::Core
{

	class ParticlePhysicsLayer : public Layer
	{
	public:
		ParticlePhysicsLayer();
		~ParticlePhysicsLayer() override;

		virtual void OnUpdate(TimeStep ts) override;
		virtual void OnAttach() override;
		virtual void OnDetach() override;
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

