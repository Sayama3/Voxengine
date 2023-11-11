//
// Created by ianpo on 29/10/2023.
//

#include "Voxymore/Project/ProjectSerializer.hpp"
#include "Voxymore/Core/YamlHelper.hpp"
#include "Voxymore/Core/Core.hpp"


namespace Voxymore::Core
{
	ProjectSerializer::ProjectSerializer(Ref<Project> project) : m_Handle(project)
	{
	}

	bool ProjectSerializer::Serialize(const std::filesystem::path& filepath)
	{
		const auto& config = m_Handle->m_Config;
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << KEYVAL("Project", YAML::BeginMap);
		out << KEYVAL("Name", config.name);
		out << KEYVAL("AssetDirectory", config.assetDirectory);
		out << KEYVAL("CacheDirectory", config.cacheDirectory);
		out << KEYVAL("SystemDirectory", config.systemDirectory);
		out << KEYVAL("StartSceneId", config.startSceneId);

		out << YAML::EndMap;
		out << YAML::EndMap;
		std::ofstream fout(filepath);
		fout << out.c_str();
		m_Handle->m_ProjectPath = filepath;
		return true;
	}

	bool ProjectSerializer::Deserialize(const std::filesystem::path& filepath)
	{
		auto& config = m_Handle->m_Config;

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
		config.name = projectNode["Name"].as<std::string>();

		VXM_CORE_ASSERT(projectNode["AssetDirectory"], "The node AssetDirectory doesn't exist.");
		config.assetDirectory = projectNode["AssetDirectory"].as<std::string>();

		VXM_CORE_ASSERT(projectNode["CacheDirectory"], "The node CacheDirectory doesn't exist.");
		config.cacheDirectory = projectNode["CacheDirectory"].as<std::string>();

		VXM_CORE_ASSERT(projectNode["SystemDirectory"], "The node SystemDirectory doesn't exist.");
		config.systemDirectory = projectNode["SystemDirectory"].as<std::string>();

		VXM_CORE_ASSERT(projectNode["StartScene"], "The node StartScene doesn't exist.");
		config.startSceneId = projectNode["StartSceneId"].as<uint64_t>();

		m_Handle->m_ProjectPath = filepath;
		return true;
	}

} // namespace Voxymore::Core
