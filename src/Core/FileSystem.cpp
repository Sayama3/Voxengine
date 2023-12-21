#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Project/Project.hpp"
#include "Voxymore/Core/FileSystem.hpp"

namespace Voxymore::Core
{

	std::string Helper::GetFileSourceName(FileSource fileSource)
	{
		VXM_PROFILE_FUNCTION();
		return GetFileSourceNames()[static_cast<int32_t>(fileSource)];
	}

	std::filesystem::path FileSystem::s_EditorPath = "./";

	std::filesystem::path FileSystem::GetRootPath(FileSource source)
	{
		VXM_PROFILE_FUNCTION();
		switch (source)
		{
			case FileSource::None:
			{
				return Project::GetProjectFilePath().parent_path();
				break;
			}

			case FileSource::EditorAsset:
			{
				return s_EditorPath / "EditorAssets/";
				break;
			}

			case FileSource::Asset:
			{
				return Project::GetAssetDirectory();
				break;
			}

			case FileSource::System:
			{
				return Project::GetSystemsDirectory();
				break;
			}

			case FileSource::Cache:
			{
				return Project::GetCacheDirectory();
				break;
			}
			default:
				break;
		}
		return Project::GetProjectFilePath().parent_path();
	}

	std::ifstream FileSystem::ReadFileAsByte(const Path &path)
	{
		VXM_PROFILE_FUNCTION();
		std::ifstream ifstream(GetPath(path));
		return ifstream;
	}

	std::stringstream FileSystem::ReadFileAsStringStream(const Path& path)
	{
		VXM_PROFILE_FUNCTION();
		std::stringstream  stringstream;
		std::ifstream ifstream(GetPath(path));
		stringstream << ifstream.rdbuf();
		return stringstream;
	}

	void FileSystem::WriteYamlFile(const Path& path, YAML::Emitter& emitter)
	{
		VXM_PROFILE_FUNCTION();
		std::ofstream fileOut(GetPath(path));
		fileOut << emitter.c_str();
	}

	YAML::Node FileSystem::ReadFileAsYAML(const Path& path)
	{
		VXM_PROFILE_FUNCTION();
		YAML::Node data;
		std::ifstream ifstream(GetPath(path));
		std::stringstream  stringstream;
		stringstream << ifstream.rdbuf();

		data = YAML::Load(stringstream.str());
		return data;
	}
	// bool FileSystem::WriteFile(const path& path, const std::string& content)
	// {
	// }
	// bool FileSystem::WriteFile(const path& path, std::vector<uint8_t> content)
	// {
	// }
	std::filesystem::path FileSystem::GetPath(const Path& path)
	{
		VXM_PROFILE_FUNCTION();
		std::filesystem::path p = GetRootPath(path.source) / path.path;
		p.make_preferred();
		return p;
	}

	bool FileSystem::Exist(const Path& path)
	{
		VXM_PROFILE_FUNCTION();
		return std::filesystem::exists(GetPath(path));
	}
	std::string FileSystem::ReadFileAsString(const Path &path)
	{
		VXM_PROFILE_FUNCTION();
		std::string result;
		std::ifstream fileStream(GetPath(path), std::ios::in | std::ios::binary);
		if(fileStream)
		{
			fileStream.seekg(0, std::ios::end);
			result.resize(fileStream.tellg());
			fileStream.seekg(0, std::ios::beg);
			fileStream.read(result.data(), result.size());
			fileStream.close();
		}
		else
		{
			VXM_CORE_ERROR("Could not open file '{0}'.", path.GetFullPath().string());
		}
		return result;
	}

	std::filesystem::path Path::GetFullPath() const
	{
		VXM_PROFILE_FUNCTION();
		return FileSystem::GetPath(*this);
	}

	bool Path::operator==(const Path &rhs) const
	{
		VXM_PROFILE_FUNCTION();
		return this->source == rhs.source && this->path == rhs.path;
	}

	bool Path::equivalent(const Path &rhs) const
	{
		VXM_PROFILE_FUNCTION();
		return std::filesystem::equivalent(this->GetFullPath(), rhs.GetFullPath());
	}

	bool Path::operator!=(const Path &rhs) const
	{
		VXM_PROFILE_FUNCTION();
		return !(*this == rhs);
	}

	Path::operator std::filesystem::path() const
	{
		VXM_PROFILE_FUNCTION();
		return GetFullPath();
	}

	std::string Path::GetPathId() const
	{
		VXM_PROFILE_FUNCTION();
		std::filesystem::path p = GetFileSourceName(source) / path;
		return p.string();
	}

	Path Path::GetCachePath() const
	{
		VXM_PROFILE_FUNCTION();
		return {FileSource::Cache, GetPathId()};
	}

	Path Path::GetPath(std::filesystem::path path)
	{
		VXM_PROFILE_FUNCTION();
		auto pathStr = path.make_preferred().string();

		for (int i = 1; i < (int)FileSource::COUNT; ++i) {
			auto source = static_cast<FileSource>(i);
			std::string rootSourceStr = FileSystem::GetRootPath(source).make_preferred().string();
			if(pathStr.starts_with(rootSourceStr))
			{
				std::string localPath = pathStr.substr(rootSourceStr.size(), pathStr.size()-rootSourceStr.size());
				if(localPath.starts_with("/") || localPath.starts_with("\\"))
				{
					localPath = localPath.substr(1, localPath.size() - 1);
				}
				return {source, localPath};
			}
		}
		return {};
	}

	YAML::Emitter& operator <<(YAML::Emitter& out, const ::Voxymore::Core::Path& p)
	{
		VXM_PROFILE_FUNCTION();
		out << YAML::Flow;
		out << YAML::BeginMap;
		out << KEYVAL("Source", static_cast<int>(p.source));
		out << KEYVAL("Path", p.path.string());
		out << YAML::EndMap;
		return out;
	}
} // namespace Voxymore::Core


//namespace YAML
//{
//	template<>
//	struct convert<::Voxymore::Core::Path> {
//		static Node encode(::Voxymore::Core::Path &path)
//		{
//			Node node;
//			node["Source"] = static_cast<int>(path.source);
//			node["Path"] = path.path.string();
//			return node;
//		}
//
//		static bool decode(const Node &node, ::Voxymore::Core::Path &rhs)
//		{
//			if (!node.IsMap() || node.size() != 2) return false;
//			rhs.source = static_cast<::Voxymore::Core::FileSource>(node["Source"].as<int>());
//			rhs.path = node["Path"].as<std::string>();
//			return true;
//		}
//	};
//}
