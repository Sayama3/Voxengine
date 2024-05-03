//
// Created by ianpo on 29/10/2023.
//

#include "Voxymore/Assets/EditorAssetManager.hpp"
#include "Voxymore/Project/ProjectSerializer.hpp"
#include "Voxymore/Core/YamlHelper.hpp"
#include "Voxymore/Core/Core.hpp"


namespace Voxymore::Core
{
	ProjectSerializer::ProjectSerializer(Ref<Project> project) : m_ProjectHandle(project)
	{
	}

	bool ProjectSerializer::Serialize(const std::filesystem::path& filepath)
	{
		VXM_PROFILE_FUNCTION();
		const auto& config = m_ProjectHandle->m_Config;
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << KEYVAL("Project", YAML::BeginMap);
		out << KEYVAL("Name", config.name);
		out << KEYVAL("AssetDirectory", config.assetDirectory);
		out << KEYVAL("CacheDirectory", config.cacheDirectory);
		out << KEYVAL("SystemDirectory", config.systemDirectory);
		out << KEYVAL("AssetRegistryPath", config.assetRegistryPath);
		if(config.startSceneId.has_value())
		{
			out << KEYVAL("StartSceneId", config.startSceneId.value());
		}

		out << YAML::EndMap;
		out << YAML::EndMap;
		std::ofstream fout(filepath);
		fout << out.c_str();

		m_ProjectHandle->m_ProjectPath = filepath;
		if(m_ProjectHandle->m_ProjectPath.is_relative()) {
			m_ProjectHandle->m_ProjectPath = std::filesystem::current_path() / m_ProjectHandle->m_ProjectPath;
		}

		return true;
	}

	bool ProjectSerializer::Deserialize(const std::filesystem::path& filepath)
	{
		VXM_PROFILE_FUNCTION();
		auto& config = m_ProjectHandle->m_Config;

		YAML::Node data;
		std::ifstream ifstream(filepath);
		std::stringstream  stringstream;
		stringstream << ifstream.rdbuf();
		try
		{
			data = YAML::Load(stringstream.str());
		}
		catch(YAML::ParserException& e)
		{
			VXM_CORE_CRITICAL("Failed to load project file '{0}'.\n\t{1}", filepath.string(), e.what());
			return false;
		}

		auto projectNode = data["Project"];
		if(!projectNode) return false;
		VXM_CORE_ASSERT(projectNode["Name"], "The node Name doesn't exist.");
		config.name = projectNode["Name"].as<std::string>("Untitled");

		VXM_CORE_ASSERT(projectNode["AssetDirectory"], "The node AssetDirectory doesn't exist.");
		config.assetDirectory = projectNode["AssetDirectory"].as<std::string>("Assets");

		VXM_CORE_ASSERT(projectNode["CacheDirectory"], "The node CacheDirectory doesn't exist.");
		config.cacheDirectory = projectNode["CacheDirectory"].as<std::string>("Cache");

		VXM_CORE_ASSERT(projectNode["SystemDirectory"], "The node SystemDirectory doesn't exist.");
		config.systemDirectory = projectNode["SystemDirectory"].as<std::string>("Systems");

		VXM_CORE_CHECK(projectNode["AssetRegistryPath"], "The node AssetRegistryPath doesn't exist.");
		config.systemDirectory = projectNode["AssetRegistryPath"].as<std::string>("AssetRegistry.vxm");

//		VXM_CORE_ASSERT(projectNode["StartScene"], "The node StartScene doesn't exist.");
		if(projectNode["StartSceneId"]) config.startSceneId = projectNode["StartSceneId"].as<UUID>();

		m_ProjectHandle->m_ProjectPath = filepath;
		if(m_ProjectHandle->m_ProjectPath.is_relative()) {
			m_ProjectHandle->m_ProjectPath = std::filesystem::current_path() / m_ProjectHandle->m_ProjectPath;
		}

		//TODO: Find a smart way to load the project
		auto assetManager = CreateRef<EditorAssetManager>();
		assetManager->DeserializeAssetRegistry();
		m_ProjectHandle->m_AssetManager = assetManager;

		return true;
	}

} // namespace Voxymore::Core
