//
// Created by ianpo on 16/02/2024.
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
	private:
		[[nodiscard]] bool HasScene() const;
	private:
		Ref<Scene> m_SceneHandle = nullptr;
	};

} // namespace Voxymore::Core

