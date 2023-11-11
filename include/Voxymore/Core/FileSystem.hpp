#pragma once

#include "Voxymore/Core/Macros.hpp"
#include "Voxymore/Core/YamlHelper.hpp"
#include <filesystem>

namespace Voxymore::Core
{
	class Application;
	enum class FileSource
	{
		None = 0,
		EditorAsset,
		Asset,
		System,
		Cache,
		COUNT,
	};

	inline std::vector<std::string> GetFileSourceNames()
	{
		return {
			"None",
			"EditorAsset",
			"Asset",
			"System",
			"Cache",
		};
	}

	inline std::string GetFileSourceName(FileSource source)
	{
		switch (source) {

			case FileSource::None: return "None";
			case FileSource::EditorAsset: return "EditorAsset";
			case FileSource::Asset: return "Asset";
			case FileSource::System: return "System";
			case FileSource::Cache: return "Cache";
		}
		return "Unknown";
	}

    namespace Helper
    {
        std::string GetFileSourceName(FileSource fileSource);
    }


	enum class FilePermission
	{
		None = 0,
		Read = BIT(0),
		Write = BIT(1),
		ReadWrite = Read | Write,
	};

	struct Path
	{
		Path() = default;
		inline Path(FileSource source, const std::filesystem::path& path) : source(source), path(path) {}
		Path(const Path&) = default;
		Path& operator=(const Path& t) = default;

		FileSource source = FileSource::None;
		std::filesystem::path path;
		std::filesystem::path GetFullPath() const;
		std::string GetPathId() const;
		inline bool empty() const {return path.empty() || source == FileSource::None;}

		operator std::filesystem::path() const;
		bool operator==(const Path& rhs) const;
		bool operator!=(const Path& rhs) const;
		static Path GetPath(std::filesystem::path path);
	};

	class FileSystem
	{
	private:
		friend class Application;
		friend struct Path;
		static std::filesystem::path s_EditorPath;
		static std::filesystem::path GetRootPath(FileSource source);
	public:
		static std::ifstream ReadFileAsByte(const Path& path);
		static std::stringstream ReadFileAsStringStream(const Path& path);
		static std::string ReadFileAsString(const Path& path);
		static YAML::Node ReadFileAsYAML(const Path& path);
		static void WriteYamlFile(const Path& path, YAML::Emitter& emitter);
		// static bool WriteFile(const path& path, const std::string& content);
		// static bool WriteFile(const path& path, std::vector<uint8_t> content);
		static std::filesystem::path GetPath(const Path& path);
		static bool Exist(const Path& path);
	};

	YAML::Emitter& operator <<(YAML::Emitter& out, const ::Voxymore::Core::Path& p);
} // namespace Voxymore::Core

namespace YAML
{
	template<>
	struct convert<::Voxymore::Core::Path> {
		inline static Node encode(::Voxymore::Core::Path &path)
		{
			Node node;
			node["Source"] = static_cast<int>(path.source);
			node["Path"] = path.path.string();
			return node;
		}

		inline static bool decode(const Node &node, ::Voxymore::Core::Path &rhs)
		{
			if (!node.IsMap() || node.size() != 2) return false;
			rhs.source = static_cast<::Voxymore::Core::FileSource>(node["Source"].as<int>());
			rhs.path = node["Path"].as<std::string>();
			return true;
		}
	};
}

namespace std
{
	template<>
	struct hash<Voxymore::Core::Path>
	{
		inline std::size_t operator()(const Voxymore::Core::Path& path) const
		{
			//TODO: Check if I want to use the PathId or the FullPath to make the hash.
			return hash<std::string>()(path.GetPathId());
		}
	};
} // namespace std
