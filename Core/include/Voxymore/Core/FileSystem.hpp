#pragma once

#include "Voxymore/Core/Macros.hpp"
#include "Voxymore/Core/YamlHelper.hpp"
#include "Voxymore/Core/UUID.hpp"
#include "Voxymore/Core/Logger.hpp"
#include "Voxymore/Debug/Profiling.hpp"
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
		VXM_PROFILE_FUNCTION();
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
		VXM_PROFILE_FUNCTION();
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

		[[deprecated("Replaced by 'id()' which has a better consistency.")]]
		inline std::string GetPathId() const {return id();}

		Path GetCachePath() const;
		inline bool empty() const {return path.empty() || source == FileSource::None;}
		std::string string() const;
		std::string id() const;

		operator std::filesystem::path() const;
		bool operator==(const Path& rhs) const;
		bool operator!=(const Path& rhs) const;

		bool equivalent(const Path& rhs) const;

		static Path GetPath(std::filesystem::path path);
		inline static bool equivalent(const Path& lhs, const Path& rhs) { return lhs.equivalent(rhs); }
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

		template<typename T>
		static std::vector<T> ReadFile(const Path& path);

		static std::string ReadFileHash(const Path& path);
		static void WriteYamlFile(const Path& path, YAML::Emitter& emitter);\

		template <class _Elem, class _Traits = std::char_traits<_Elem>, class _Alloc = std::allocator<_Elem>>
		static bool Write(const Path& path, const std::basic_string<_Elem, _Traits, _Alloc>& content);

		template<typename T>
		static bool Write(const Path& path, const std::vector<T>& content);

		template<typename T>
		static bool Write(const Path& path, const T* content, size_t count);

		static std::filesystem::path GetPath(const Path& path);
		static bool Exist(const Path& path);
	};

	YAML::Emitter& operator <<(YAML::Emitter& out, const ::Voxymore::Core::Path& p);


	template<typename T>
	inline std::vector<T> FileSystem::ReadFile(const Path &path)
	{
		VXM_PROFILE_FUNCTION();
		std::vector<T> result;
		std::ifstream file(path.GetFullPath(), std::ios::binary);
		if(file) {
			std::copy(std::istream_iterator<T>(file), std::istream_iterator<T>(), std::back_inserter(result));
		} else {
			VXM_CORE_ERROR("Could not open file '{0}'.", path.GetFullPath().string());
		}
		return result;
	}

	template <class _Elem, class _Traits, class _Alloc>
	inline bool FileSystem::Write(const Path& path, const std::basic_string<_Elem, _Traits, _Alloc>& content)
	{
		VXM_PROFILE_FUNCTION();
		auto fullPath = path.GetFullPath();
		if(fullPath.has_parent_path()) {
			std::filesystem::create_directories(fullPath.parent_path());
		}

		std::ofstream file(fullPath, std::ios::binary);
		if(!file) return false;

		file.write(reinterpret_cast<const char *>(content.data()), content.size() * sizeof(_Elem));
		file.close();
		return file.good();
	}

	template<typename T>
	inline bool FileSystem::Write(const Path &path, const T *content, size_t count)
	{
		VXM_PROFILE_FUNCTION();
		auto fullPath = path.GetFullPath();
		if(fullPath.has_parent_path()) {
			std::filesystem::create_directories(fullPath.parent_path());
		}

		std::ofstream file(fullPath, std::ios::binary);
		if(!file) return false;

		file.write(reinterpret_cast<const char *>(content), count * sizeof(T));
		file.close();
		return file.good();
	}

	template<typename T>
	inline bool FileSystem::Write(const Path &path, const std::vector<T> &content)
	{
		VXM_PROFILE_FUNCTION();
		auto fullPath = path.GetFullPath();
		if(fullPath.has_parent_path()) {
			std::filesystem::create_directories(fullPath.parent_path());
		}

		std::ofstream file(fullPath, std::ios::binary);
		if(!file) return false;

		file.write(reinterpret_cast<const char *>(content.data()), content.size() * sizeof(T));
		file.close();
		return file.good();
	}
} // namespace Voxymore::Core

namespace YAML
{
	template<>
	struct convert<::Voxymore::Core::Path> {
		inline static Node encode(::Voxymore::Core::Path &path)
		{
			VXM_PROFILE_FUNCTION();
			Node node;
			node["Source"] = static_cast<int>(path.source);
			node["Path"] = path.path.string();
			return node;
		}

		inline static bool decode(const Node &node, ::Voxymore::Core::Path &rhs)
		{
			VXM_PROFILE_FUNCTION();
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
			VXM_PROFILE_FUNCTION();
			//TODO: Check if I want to use the PathId or the FullPath to make the hash.
			return hash<std::string>()(path.id());
		}
	};
} // namespace std
