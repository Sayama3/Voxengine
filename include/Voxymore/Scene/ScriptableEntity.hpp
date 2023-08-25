//
// Created by ianpo on 25/08/2023.
//

#pragma once

#include "Voxymore/Scene/Entity.hpp"

namespace Voxymore::Core
{
	class ScriptableEntity
	{
	protected:
		inline virtual void OnCreate() {}
		inline virtual void OnDestroy() {}
		inline virtual void OnUpdate(TimeStep ts) {}
	public:
		inline virtual ~ScriptableEntity() = default;
	public:
		template<typename T>
		inline bool HasComponent() const
		{
			return m_Entity.HasComponent<T>();
		}

		template<typename T>
		inline T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}

		template<typename T,  typename... Args>
		inline T& AddComponent(Args &&...args)
		{
			return m_Entity.AddComponent<T>(std::forward<Args>(args)...);
		}

		template<typename T>
		inline void RemoveComponent()
		{
			m_Entity.RemoveComponent<T>();
		}
	private:
		Entity m_Entity;
		friend class Scene;
	};
}