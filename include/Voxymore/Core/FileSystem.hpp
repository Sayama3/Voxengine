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
        EditorShader,
        Shader,
        System,
        Scene,
    };

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
		inline bool empty() const {return path.empty() || source == FileSource::None;}
    };

    class FileSystem
    {
private:
friend class Application;
        static std::filesystem::path s_RootPath;
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
} // namespace Voxymore::Core
