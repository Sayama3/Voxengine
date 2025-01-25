//
// Created by ianpo on 25/01/2025.
//

#include "Voxymore/Renderer/ShaderSource.hpp"

namespace Voxymore::Core {
    FileShaderSource::FileShaderSource()
        : SourcePath({FileSource::None, ""}), Type(ShaderType::None) {
    }

    FileShaderSource::FileShaderSource(const ShaderType shaderType, Path path)
        : SourcePath(std::move(path)), Type(shaderType)
    {
        TryReloadSource();
    }

    FileShaderSource::FileShaderSource(ShaderType shaderType, Path path, std::string source)
        : SourcePath(std::move(path)), SourceRef(CreateRef<std::string>(std::move(source))), Type(shaderType)
    {
        FileSystem::Write(SourcePath, *SourceRef);
        SourcePathTime = std::filesystem::last_write_time(SourcePath);
    }

    ShaderType FileShaderSource::GetShaderType() const {
        return Type;
    }

    Ref<const std::string> FileShaderSource::GetString() {
        if (!FileSystem::Exist(SourcePath)) return nullptr;

        TryReloadSource();

        return SourceRef;
    }

    void FileShaderSource::TryReloadSource() {
        VXM_PROFILE_FUNCTION();
        VXM_CORE_ASSERT(FileSystem::Exist(SourcePath), "THe file '{}' doesn't exist.", SourcePath.string())

        if (const auto sourceTime = std::filesystem::last_write_time(SourcePath); sourceTime > SourcePathTime) {
            SourcePathTime = sourceTime;
            SourceRef = CreateRef<std::string>(std::move(FileSystem::ReadFileAsString(SourcePath)));
        }
    }

    void FileShaderSource::ForceReloadSource() {
        VXM_PROFILE_FUNCTION();
        VXM_CORE_ASSERT(FileSystem::Exist(SourcePath), "THe file '{}' doesn't exist.", SourcePath.string())
        SourcePathTime = std::filesystem::last_write_time(SourcePath);;
        SourceRef = CreateRef<std::string>(std::move(FileSystem::ReadFileAsString(SourcePath)));
    }

    bool FileShaderSource::HasChanged() const {
        VXM_PROFILE_FUNCTION();
        if (!FileSystem::Exist(SourcePath)) return false;
        const auto sourceTime = std::filesystem::last_write_time(SourcePath);
        return sourceTime > SourcePathTime;
    }

    MemoryShaderSource::MemoryShaderSource()
        : SourceRef(CreateRef<std::string>()), Type(ShaderType::None)
    {
    }

    MemoryShaderSource::MemoryShaderSource(const ShaderType shaderType, std::string source)
        : SourceRef(CreateRef<std::string>(std::move(source))), Type(shaderType)
    {
    }

    MemoryShaderSource::MemoryShaderSource(const ShaderType shaderType, Ref<std::string> source)
        : SourceRef(std::move(source)), Type(shaderType)
    {
    }

    ShaderType MemoryShaderSource::GetShaderType() const {
        return Type;
    }

    Ref<const std::string> MemoryShaderSource::GetString() {
        return SourceRef;
    }
}
