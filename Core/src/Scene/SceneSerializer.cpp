//
// Created by ianpo on 29/08/2023.
//

#include "Voxymore/Scene/SceneSerializer.hpp"
#include "Voxymore/Components/Components.hpp"
#include "Voxymore/Components/CustomComponent.hpp"
#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Core/UUID.hpp"
#include "Voxymore/Core/YamlHelper.hpp"
#include "Voxymore/Scene/Entity.hpp"
#include "Voxymore/Core/TypeHelpers.hpp"
#include<ranges>

namespace Voxymore::Core
{
	SceneSerializer::SceneSerializer(const Ref<Scene> &scene) : m_Scene(scene)
	{
	}

	SceneSerializer::SceneSerializer(Scene* scene) : m_Scene(scene)
	{
	}

	void SceneSerializer::ChangeSceneTarget(const Ref<Scene>& scene)
	{
		VXM_PROFILE_FUNCTION();
		m_Scene = scene;
	}
	void SceneSerializer::ChangeSceneTarget(Scene* scene)
	{
		VXM_PROFILE_FUNCTION();
		m_Scene = scene;
	}

	SceneSerializer::~SceneSerializer()
	{
	}

	bool SceneSerializer::Serialize(const Path &filePath) const
	{
		return Serialize(filePath.GetFullPath());
	}

	bool SceneSerializer::Serialize(const std::filesystem::path &filePath) const
	{
		VXM_PROFILE_FUNCTION();
		YAML::Emitter out;
		out << YAML::BeginMap;
		{
			out << KEYVAL("Scene", YAML::BeginMap);// TODO: add a name to the scenes.
			{
				out << KEYVAL("Name", GetScene().m_Name);
				out << KEYVAL("Entities", YAML::BeginSeq);
				{
					for (entt::entity id : *GetScene().m_Registry.storage<entt::entity>()) {
						Scene* ptr = std::visit<Scene*>(overloads{[](Scene* s){return s;}, [](Ref<Scene> s){return s.get();}}, m_Scene);
						Entity entity(id, ptr);
						if (!entity || !GetScene().m_Registry.valid(entity)) continue;
						out << YAML::BeginMap;// Entity
						{
							SerializeEntity(out, entity);
							out << YAML::EndMap;// Entity
						}
					}
					out << YAML::EndSeq;
				}
				out << YAML::EndMap;
			}
			out << YAML::EndMap;
		}

		std::filesystem::create_directories(filePath.parent_path());

		std::ofstream fileOut(filePath);
		fileOut << out.c_str();

		return true;
	}

	bool SceneSerializer::Deserialize(const Path &filePath)
	{
		return Deserialize(filePath.GetFullPath());
	}

	bool SceneSerializer::Deserialize(const std::filesystem::path &filePath)
	{
		VXM_PROFILE_FUNCTION();
		std::ifstream ifstream(filePath);
		std::stringstream stringstream;
		stringstream << ifstream.rdbuf();

		YAML::Node data = YAML::Load(stringstream.str());
		YAML::Node sceneNode = data["Scene"];
		if (!sceneNode) {
			return false;
		}

		GetScene().m_Name = sceneNode["Name"].as<std::string>();

		auto entities = sceneNode["Entities"];
		if (entities) {
			for (YAML::Node yamlEntity : entities) {
				UUID uuid = yamlEntity["Entity"].as<uint64_t>();

				std::string name;
				auto tagComponent = yamlEntity["TagComponent"];
				if (tagComponent) {
					name = tagComponent["Tag"].as<std::string>();
				}
				else {
					name = "Entity_" + uuid.string();
				}

				bool enable = (!yamlEntity["IsActive"]) || yamlEntity["IsActive"].as<bool>();

				VXM_CORE_TRACE("Deserialize Entity with name: {0} and ID: {1}", name, uuid.string());

				Entity entity = GetScene().CreateEntity(uuid, name);
				entity.SetActive(enable);

				auto transformComponent = yamlEntity["TransformComponent"];
				if (transformComponent) {
					auto &tc = entity.GetComponent<TransformComponent>();
					tc.SetPosition(transformComponent["Position"].as<glm::vec3>());
					tc.SetScale(transformComponent["Scale"].as<glm::vec3>());
					auto rotation = transformComponent["Rotation"].as<glm::quat>();
					auto eulerRotation = transformComponent["EulerRotation"].as<glm::vec3>();
					if (rotation != glm::quat(glm::radians(eulerRotation))) {
						tc.SetRotation(rotation);
					}
					else {
						tc.SetEulerRotation(eulerRotation);
					}
				}

				auto cameraComponent = yamlEntity["CameraComponent"];
				if (cameraComponent) {
					auto &cc = entity.AddComponent<CameraComponent>();

					cc.Primary = cameraComponent["Primary"].as<bool>();
					cc.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();

					auto camera = cameraComponent["Camera"];
					auto aspectRatio = camera["AspectRatio"].as<float>();
					cc.Camera.SetAspectRatio(aspectRatio);

					auto orthographicSize = camera["OrthographicSize"].as<float>();
					auto orthographicNear = camera["OrthographicNear"].as<float>();
					auto orthographicFar = camera["OrthographicFar"].as<float>();
					cc.Camera.SetOrthographic(orthographicSize, orthographicNear, orthographicFar);

					auto perspectiveVerticalFOV = camera["PerspectiveVerticalFOV"].as<float>();
					auto perspectiveNear = camera["PerspectiveNear"].as<float>();
					auto perspectiveFar = camera["PerspectiveFar"].as<float>();
					cc.Camera.SetPerspective(perspectiveVerticalFOV, perspectiveNear, perspectiveFar);

					cc.Camera.SwitchToOrthographic(camera["IsOrthographic"].as<bool>());
				}

				for (const ComponentChecker &cc: ComponentManager::GetComponents()) {
					YAML::Node yamlcc = yamlEntity[cc.ComponentName];
					if (yamlcc) {
						cc.AddComponent(entity);
						cc.DeserializeComponent(yamlcc, entity);
					}
				}
			}
		}
		return true;
	}

	bool SceneSerializer::SerializeRuntime(const std::filesystem::path &filePath) const
	{
		VXM_PROFILE_FUNCTION();
		return false;
	}

	bool SceneSerializer::DeserializeRuntime(const std::filesystem::path &filePath, bool deserializeId)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(false, "Function DeserializeRuntime not implemented yet.");
		return false;
	}

	void SceneSerializer::SerializeEntity(YAML::Emitter &out, Entity entity)
	{
		VXM_PROFILE_FUNCTION();
		out << KEYVAL("Entity", entity.id());//TODO: set entity ID;

		out << KEYVAL("IsActive", entity.IsActive());

		if (entity.HasComponent<TagComponent>()) {
			out << KEYVAL("TagComponent", YAML::BeginMap);// TagComponent
			{
				auto &component = entity.GetComponent<TagComponent>();
				out << KEYVAL("Tag", component.Tag);

				out << YAML::EndMap;// TagComponent
			}
		}

		if (entity.HasComponent<TransformComponent>()) {
			out << KEY("TransformComponent");
			out << YAML::BeginMap;// TransformComponent
			{
				auto &component = entity.GetComponent<TransformComponent>();
				out << KEYVAL("Position", component.GetPosition());
				out << KEYVAL("Rotation", component.GetRotation());
				out << KEYVAL("EulerRotation", component.GetEulerRotation());
				out << KEYVAL("Scale", component.GetScale());

				out << YAML::EndMap;// TransformComponent
			}
		}

		if (entity.HasComponent<CameraComponent>()) {
			out << KEYVAL("CameraComponent", YAML::BeginMap);// CameraComponent
			{
				auto &component = entity.GetComponent<CameraComponent>();
				out << KEYVAL("Primary", component.Primary);
				out << KEYVAL("FixedAspectRatio", component.FixedAspectRatio);

				out << KEY("Camera");
				out << YAML::BeginMap;// Camera
				{
					out << KEYVAL("IsOrthographic", component.Camera.IsOrthographic());
					out << KEYVAL("AspectRatio", component.Camera.GetAspectRatio());

					out << KEYVAL("OrthographicSize", component.Camera.GetOrthographicSize());
					out << KEYVAL("OrthographicNear", component.Camera.GetOrthographicNear());
					out << KEYVAL("OrthographicFar", component.Camera.GetOrthographicFar());

					out << KEYVAL("PerspectiveVerticalFOV", component.Camera.GetPerspectiveVerticalFOV());
					out << KEYVAL("PerspectiveNear", component.Camera.GetPerspectiveNear());
					out << KEYVAL("PerspectiveFar", component.Camera.GetPerspectiveFar());
					out << YAML::EndMap;// Camera
				}
				out << YAML::EndMap;// CameraComponent
			}
		}

		//TODO: serialize the NativeScriptComponent.

		for (const ComponentChecker &cc: ComponentManager::GetComponents()) {
			if (cc.HasComponent(entity)) {
				out << KEYVAL(cc.ComponentName, YAML::BeginMap);
				{
					cc.SerializeComponent(out, entity);
					out << YAML::EndMap;
				}
			}
		}
	}

	std::optional<UUID> SceneSerializer::GetSceneID(const std::filesystem::path &filePath)
	{
		VXM_PROFILE_FUNCTION();
		std::ifstream ifstream(filePath);
		std::stringstream stringstream;
		stringstream << ifstream.rdbuf();

		YAML::Node data = YAML::Load(stringstream.str());
		YAML::Node sceneNode = data["Scene"];
		if (!sceneNode) {
			return {};
		}

		UUID id = sceneNode["UUID"].as<uint64_t>();
		return id;
	}

	Scene& SceneSerializer::GetScene()
	{
		VXM_PROFILE_FUNCTION();
		Scene* scene = std::visit<Scene*>(overloads{[](Scene* s){return s;}, [](Ref<Scene> s){return s.get();}}, m_Scene);
		return *scene;
	}

	const Scene& SceneSerializer::GetScene() const
	{
		VXM_PROFILE_FUNCTION();
		Scene* scene = std::visit<Scene*>(overloads{[](Scene* s){return s;}, [](Ref<Scene> s){return s.get();}}, m_Scene);
		return *scene;
	}
}// namespace Voxymore::Core
