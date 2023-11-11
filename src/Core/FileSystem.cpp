#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Project/Project.hpp"
#include "Voxymore/Core/FileSystem.hpp"

namespace Voxymore::Core
{

    std::string Helper::GetFileSourceName(FileSource fileSource)
    {
        return GetFileSourceNames()[static_cast<int32_t>(fileSource)];
    }

	std::filesystem::path FileSystem::s_EditorPath = "./";

	std::filesystem::path FileSystem::GetRootPath(FileSource source)
	{
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
		std::ifstream ifstream(GetPath(path));
		return ifstream;
	}

	std::stringstream FileSystem::ReadFileAsStringStream(const Path& path)
	{
		std::stringstream  stringstream;
		std::ifstream ifstream(GetPath(path));
		stringstream << ifstream.rdbuf();
		return stringstream;
	}

	void FileSystem::WriteYamlFile(const Path& path, YAML::Emitter& emitter)
	{
		std::ofstream fileOut(GetPath(path));
		fileOut << emitter.c_str();
	}

	YAML::Node FileSystem::ReadFileAsYAML(const Path& path)
	{
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
		return GetRootPath(path.source) / path.path;
	}

	bool FileSystem::Exist(const Path& path)
	{
		return std::filesystem::exists(GetPath(path));
	}
	std::string FileSystem::ReadFileAsString(const Path &path)
	{
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
		return FileSystem::GetPath(*this);
	}

	bool Path::operator==(const Path &rhs) const
	{
		return this->path == rhs.path
			   && this->source == rhs.source;
	}

	bool Path::operator!=(const Path &rhs) const
	{
		return !(*this == rhs);
	}

	std::string Path::GetPathId() const
	{
		std::filesystem::path p = GetFileSourceName(source) / path;
		return p.string();
	}

	YAML::Emitter& operator <<(YAML::Emitter& out, const ::Voxymore::Core::Path& p)
	{
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
