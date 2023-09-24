#pragma once

#include "Voxymore/Core/Core.hpp"
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
        FileSource Source = FileSource::None;
        std::filesystem::path Path;
        std::filesystem::path GetFullPath() const;
    };

    class FileSystem
    {
private:
friend class Application;
        static std::filesystem::path s_RootPath;
        static std::filesystem::path GetRootPath(FileSource source);
public:
        static std::ifstream ReadFileAsByte(const Path& path);
        static std::stringstream ReadFileAsString(const Path& path);
        static YAML::Node ReadFileAsYAML(const Path& path);
        static void WriteYamlFile(const Path& path, YAML::Emitter& emitter);
        // static bool WriteFile(const Path& path, const std::string& content);
        // static bool WriteFile(const Path& path, std::vector<uint8_t> content);
        static std::filesystem::path GetPath(const Path& path);
        static bool Exist(const Path& path);
    };
} // namespace Voxymore::Core
