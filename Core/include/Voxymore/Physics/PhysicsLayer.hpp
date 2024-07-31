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

#include "Voxymore/Physics/ObjectLayerPairFilter.hpp"
#include "Voxymore/Physics/BroadPhaseLayerInterface.hpp"
#include "Voxymore/Physics/PhysicsListener.hpp"
#include "Voxymore/Physics/ObjectVsBroadPhaseLayerFilter.hpp"

#include <Jolt/Jolt.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>

#include <thread>


namespace Voxymore::Core
{

	class PhysicsLayer : public Layer
	{
	public:
		PhysicsLayer();
		~PhysicsLayer() override;

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
	public:
		/// This is the max amount of rigid bodies that you can add to the physics system. If you try to add more you'll get an error.
		static constexpr uint32_t cMaxBodies = 65536;

		/// This determines how many mutexes to allocate to protect rigid bodies from concurrent access. Set it to 0 for the default settings.
		static constexpr uint32_t cNumBodyMutexes = 0;

		/// This is the max amount of body pairs that can be queued at any time (the broad phase will detect overlapping
		/// body pairs based on their bounding boxes and will insert them into a queue for the narrowphase). If you make this buffer
		/// too small the queue will fill up and the broad phase jobs will start to do narrow phase work. This is slightly less efficient.
		static constexpr uint32_t cMaxBodyPairs = 65536;

		/// This is the maximum size of the contact constraint buffer. If more contacts (collisions between bodies) are detected than this
		/// number then these contacts will be ignored and bodies will start interpenetrating / fall through the world.
		static constexpr uint32_t cMaxContactConstraints = 10240;
	public:

		/// We need a temp allocator for temporary allocations during the physics update. We're
		/// pre-allocating 10 MB to avoid having to do allocations during the physics update.
		JPH::TempAllocatorImpl m_TempAllocator{10 * 1024 * 1024};

		/// We need a job system that will execute physics jobs on multiple threads. Typically
		/// you would implement the JobSystem interface yourself and let Jolt Physics run on top
		/// of your own job scheduler. JobSystemThreadPool is an example implementation.
		JPH::JobSystemThreadPool m_JobSystem{JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, static_cast<int>(std::thread::hardware_concurrency() - 1)};
	};

} // namespace Voxymore::Core

