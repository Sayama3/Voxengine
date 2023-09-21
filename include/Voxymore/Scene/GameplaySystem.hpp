//
// Created by ianpo on 21/09/2023.
//

#include <string>
#include <unordered_map>
#include "Voxymore/Core/YamlHelper.hpp"
#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Core/TimeStep.hpp"
#include "Voxymore/Core/SmartPointers.hpp"
#include "Voxymore/Scene/Scene.hpp"

namespace Voxymore::Core
{
	class SystemManager;

	class GameplaySystem
	{
	private:
		friend class SystemManager;
	protected:
		virtual void DeserializeSystem(YAML::Node& componentNode) = 0;
		virtual void SerializeSystem(YAML::Emitter& Emitter) = 0;
	public:
		virtual const std::string GetName() const = 0;
		inline virtual void OnAttachToScene(Scene& scene) {}
		virtual void Update(Scene& scene, TimeStep ts) = 0;
		inline virtual void OnDetachFromScene(Scene& scene) {}
	};

	class SystemManager
	{
	private:
		static std::unordered_map<std::string, Ref<GameplaySystem>> s_Systems;
	public:
		static void AddSystem(std::string name, Ref<GameplaySystem> system);
		static Ref<GameplaySystem> GetSystem(const std::string& name);
		static std::vector<std::string> GetSystemsName();
	};
}

#define VXM_IMPLEMENT_SYSTEM(SYS) private: \
    static ::Voxymore::Core::Ref<SYS> s_Instance; \
public:                                    \
	inline virtual const std::string GetName() const override { return #SYS; }; \
    inline static ::Voxymore::Core::Ref<SYS> GetInstance() { return s_Instance; }\
	inline static ::Voxymore::Core::Ref<SYS> CreateSystem() \
	{ \
		auto instance = ::Voxymore::Core::CreateRef<SYS>();\
		::Voxymore::Core::SystemManager::AddSystem(instance->GetName(), std::static_pointer_cast<::Voxymore::Core::GameplaySystem>(instance));\
		return instance; \
	}


#define VXM_CREATE_SYSTEM(SYS) ::Voxymore::Core::Ref<SYS> SYS::s_Instance = SYS::CreateSystem();


// ======== CameraControllerSystem ========
class CameraControllerSystem : public ::Voxymore::Core::GameplaySystem
{
	VXM_IMPLEMENT_SYSTEM(CameraControllerSystem);
protected:
	virtual void DeserializeSystem(YAML::Node& componentNode) override;
	virtual void SerializeSystem(YAML::Emitter& emitter) override;
public:
	virtual void Update(::Voxymore::Core::Scene& scene, ::Voxymore::Core::TimeStep ts) override;
private:
	float m_Speed = 5.0f;
};