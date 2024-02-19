//
// Created by ianpo on 21/09/2023.
//

#pragma once

#include "Voxymore/Components/Components.hpp"
#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Core/FileSystem.hpp"
#include "Voxymore/Core/Input.hpp"
#include "Voxymore/Core/KeyCodes.hpp"
#include "Voxymore/Core/MouseButtonCodes.hpp"
#include "Voxymore/Core/SmartPointers.hpp"
#include "Voxymore/Core/TimeStep.hpp"
#include "Voxymore/Core/YamlHelper.hpp"
#include "Voxymore/Scene/Scene.hpp"
#include <imgui.h>
#include <string>
#include <unordered_map>
#include <static_block.hpp>

#define VXM_SYSTEM_EXTENSION ".vxm"

namespace Voxymore::Core
{
	class SystemManager;

	class System
	{
	private:
		friend class SystemManager;
	protected:
		/**
		 * Deserialize from yaml to this system.
		 * @param node The node of the current system.
		 */
		inline virtual void DeserializeSystem(YAML::Node& node) {
			VXM_PROFILE_FUNCTION();
			if(node["UUID"].IsDefined()) { m_ID = node["UUID"].as<uint64_t >(); }
		}

		/**
		 * Serializing to yaml the current system.
		 * @param out The emitter of the current system.
		 */
		inline virtual void SerializeSystem(YAML::Emitter& out) {
			VXM_PROFILE_FUNCTION();
			out << KEYVAL("UUID", m_ID);
		}

		/**
		 * Reset the current system.
		 */
		inline virtual void ResetSystem() {}
	public:
		inline virtual bool RunOnAllScenes() {return true;}
		inline virtual bool OnImGuiRender() {return false;}
		inline virtual const std::string GetName() const {return "System";}

		inline virtual void OnAttachToScene(Scene& scene) {}
		inline virtual void OnDetachFromScene(Scene& scene) {}
		inline virtual void Update(Scene& scene, TimeStep ts) {}

		inline virtual void OnStart(Scene& scene) {}
		inline virtual void OnStop(Scene& scene) {}

		inline virtual MultiThreading::ExecutionPolicy GetExecutionPolicy() { return MultiThreading::ExecutionPolicy::None; }

		inline UUID id() const {return m_ID;}
	protected:
		UUID m_ID;
	};

	class SystemManager
	{
	private:
		//TODO: Replace the systems name by UUID.
		std::unordered_map<std::string, Ref<System>> s_Systems;
		std::unordered_map<std::string, std::vector<UUID>> s_SystemToScene;
		std::unordered_map<std::string, bool> s_SystemEnabled;
		UUID m_OnProjectLoadId;
		static SystemManager* s_SystemManager;
		static SystemManager& GetInstance();
	public:
		SystemManager();
		~SystemManager();
	private:
		static void WriteSystem(YAML::Emitter& out, const std::string& name);
		static bool HasSaveFile(const std::string& name);
		static void FillSystem(const std::string& name);
		static void ResetSystem(const std::string& name);
		static Path GetPath(const std::string& name);
	public:
		static void AddSystem(std::string name, Ref<System> system);
		static void SaveSystem(const std::string& name);
		static void LoadSystem(const std::string& name);
		static bool IsActive(const std::string& name);
		static void SetActive(const std::string& systemName, bool enable);
		static void DeleteSystem(const std::string& systemName);

		static void AddSceneToSystem(const std::string& systemName, UUID sceneId);
		static void AddSceneToSystemIfNotExist(const std::string& systemName, UUID sceneId);
		static void RemoveSceneFromSystem(const std::string& systemName, UUID sceneId);

		static std::vector<UUID>& GetSystemScenes(const std::string& name);
		static Ref<System> GetSystem(const std::string& name);
		static std::vector<Ref<System>> GetSystems(UUID sceneId);
		static std::vector<std::string> GetSystemsName();
	private:
		static void ReloadSystems();
	};
}

// TODO: Replace the macro with a self implementing templated inheritance
#define VXM_IMPLEMENT_SYSTEM(SYS) private: \
    static ::Voxymore::Core::Ref<SYS> s_Instance; \
public: \
	inline virtual const std::string GetName() const override { return #SYS; }; \
	inline static const std::string StaticGetName() { return #SYS; }; \
    inline static ::Voxymore::Core::Ref<SYS> GetInstance() { return s_Instance; }\
	inline static ::Voxymore::Core::Ref<SYS> CreateSystem() \
	{ \
        VXM_PROFILE_FUNCTION();            \
        if(SYS::s_Instance != nullptr) return SYS::s_Instance;                                   \
		SYS::s_Instance = ::Voxymore::Core::CreateRef<SYS>();\
		::Voxymore::Core::SystemManager::AddSystem(SYS::s_Instance->GetName(), std::static_pointer_cast<::Voxymore::Core::System>(SYS::s_Instance));\
		return SYS::s_Instance; \
	} \
	inline static void DeleteSystem() \
	{ \
        VXM_PROFILE_FUNCTION(); \
        if(SYS::s_Instance != nullptr) return; \
		::Voxymore::Core::SystemManager::DeleteSystem(SYS::s_Instance->GetName()); \
		SYS::s_Instance = nullptr; \
	}


#define VXM_DECLARE_SYSTEM(SYS) ::Voxymore::Core::Ref<SYS> SYS::s_Instance = nullptr;
#define VXM_CREATE_SYSTEM(SYS) static_block{ SYS::CreateSystem(); }

/*
// ======== CameraControllerSystem ========
class CameraControllerSystem : public ::Voxymore::Core::System
{
	VXM_IMPLEMENT_SYSTEM(CameraControllerSystem);
protected:
	virtual void DeserializeSystem(YAML::Node& componentNode) override;
	virtual void SerializeSystem(YAML::Emitter& emitter) override;
	virtual void ResetSystem() override;
public:
	virtual bool OnImGuiRender() override;
	virtual void Update(::Voxymore::Core::Scene& scene, ::Voxymore::Core::TimeStep ts) override;
private:
	float m_Speed = 5.0f;
	::Voxymore::Core::Key m_ForwardKey = ::Voxymore::Core::Key::W;
	::Voxymore::Core::Key m_BackwardKey = ::Voxymore::Core::Key::S;
	::Voxymore::Core::Key m_RightKey = ::Voxymore::Core::Key::D;
	::Voxymore::Core::Key m_LeftKey = ::Voxymore::Core::Key::A;
	::Voxymore::Core::Key m_UpKey = ::Voxymore::Core::Key::E;
	::Voxymore::Core::Key m_DownKey = ::Voxymore::Core::Key::Q;
};
*/
