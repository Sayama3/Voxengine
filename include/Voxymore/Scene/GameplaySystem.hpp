//
// Created by ianpo on 21/09/2023.
//

#pragma once

#include <string>
#include <imgui.h>
#include <unordered_map>
#include "Voxymore/Core/YamlHelper.hpp"
#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Core/Input.hpp"
#include "Voxymore/Core/KeyCodes.hpp"
#include "Voxymore/Core/MouseButtonCodes.hpp"
#include "Voxymore/Core/TimeStep.hpp"
#include "Voxymore/Core/SmartPointers.hpp"
#include "Voxymore/Core/FileSystem.hpp"
#include "Voxymore/Scene/Scene.hpp"
#include "Voxymore/Scene/Components.hpp"

#define VXM_SYSTEM_EXTENSION ".vxm"

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
		virtual void ResetSystem() = 0;
	public:
		virtual bool OnImGuiRender() = 0;
		virtual const std::string GetName() const = 0;
		inline virtual void OnAttachToScene(Scene& scene) {}
		virtual void Update(Scene& scene, TimeStep ts) = 0;
		inline virtual void OnDetachFromScene(Scene& scene) {}
	};

	class SystemManager
	{
	private:
		std::unordered_map<std::string, Ref<GameplaySystem>> s_Systems;
		std::unordered_map<std::string, std::vector<std::string>> s_SystemToScene;
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
		static void AddSystem(std::string name, Ref<GameplaySystem> system);
		static void SaveSystem(const std::string& name);
		static void LoadSystem(const std::string& name);
		static bool IsActive(const std::string& name);
		static void SetActive(const std::string& systemName, bool enable);

		static void AddSceneToSystem(const std::string& systemName, const std::string& sceneName);
		static void AddSceneToSystemIfNotExist(const std::string& systemName, const std::string& sceneName);
		static void RemoveSceneFromSystem(const std::string& systemName, const std::string& sceneName);

		static std::vector<std::string>& GetSystemScenes(const std::string& name);
		static Ref<GameplaySystem> GetSystem(const std::string& name);
		static std::vector<Ref<GameplaySystem>> GetSystems(const std::string& sceneName);
		static std::vector<std::string> GetSystemsName();
	private:
		static void ReloadSystems();
	};
}

#define VXM_IMPLEMENT_SYSTEM(SYS) private: \
    static ::Voxymore::Core::Ref<SYS> s_Instance; \
public: \
	inline virtual const std::string GetName() const override { return #SYS; }; \
    inline static ::Voxymore::Core::Ref<SYS> GetInstance() { return s_Instance; }\
	inline static ::Voxymore::Core::Ref<SYS> CreateSystem() \
	{ \
		auto instance = ::Voxymore::Core::CreateRef<SYS>();\
		::Voxymore::Core::SystemManager::AddSystem(instance->GetName(), std::static_pointer_cast<::Voxymore::Core::GameplaySystem>(instance));\
		return instance; \
	}


#define VXM_CREATE_SYSTEM(SYS) ::Voxymore::Core::Ref<SYS> SYS::s_Instance = SYS::CreateSystem();

/*
// ======== CameraControllerSystem ========
class CameraControllerSystem : public ::Voxymore::Core::GameplaySystem
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
