//
// Created by ianpo on 16/02/2024.
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

#include "Voxymore/RigidbodiesPhysics/Collisions/RigidbodyContact.hpp"
#include "Voxymore/RigidbodiesPhysics/Collisions/RigidbodyContactResolver.hpp"
#include "Voxymore/RigidbodiesPhysics/Components/RigidbodyComponent.hpp"
#include "Voxymore/RigidbodiesPhysics/Systems/RigidbodyGravitySystem.hpp"

namespace Voxymore
{
	namespace Core
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
		private:
			Vec3 m_Gravity = Vec3(0.0, -9.8, 0.0);
			Ref<Scene> m_SceneHandle = nullptr;
			std::vector<RigidbodyContact> m_Contacts;
			RigidbodyContactResolver m_Resolver;

		};

	}// namespace Core
}// namespace Voxymore
