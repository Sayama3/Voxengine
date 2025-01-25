//
// Created by ianpo on 25/01/2025.
//

#pragma once
#include "Voxymore/Renderer/ShaderType.hpp"
#include "Voxymore/Assets/Asset.hpp"
#include "Voxymore/Assets/AssetField.hpp"

namespace Voxymore::Core {

    class ShaderSource : public Asset {
    public:
        VXM_IMPLEMENT_ASSET(AssetType::ShaderSource);
    public:
        [[nodiscard]] virtual ShaderType GetShaderType() const = 0;
        [[nodiscard]] virtual Ref<const std::string> GetString() = 0;
    };

    class FileShaderSource final : public ShaderSource {
    public:
        FileShaderSource();
        virtual ~FileShaderSource() override = default;
        FileShaderSource(ShaderType shaderType, Path path);
        FileShaderSource(ShaderType shaderType, Path path, std::string source);
    public:
        [[nodiscard]] virtual ShaderType GetShaderType() const override;
        [[nodiscard]] virtual Ref<const std::string> GetString() override;
    public:
        void TryReloadSource();
        void ForceReloadSource();
        [[nodiscard]] bool HasChanged() const;
    public:
        Path SourcePath;
        std::filesystem::file_time_type SourcePathTime = std::filesystem::file_time_type::min();
        Ref<std::string> SourceRef;
        ShaderType Type;
    };

    class MemoryShaderSource final : public ShaderSource {
    public:
        MemoryShaderSource();
        virtual ~MemoryShaderSource() override = default;
        MemoryShaderSource(ShaderType shaderType, std::string source);
        MemoryShaderSource(ShaderType shaderType, Ref<std::string> source);
    public:
        [[nodiscard]] virtual ShaderType GetShaderType() const override;
        [[nodiscard]] virtual Ref<const std::string> GetString() override;
    public:
    public:
        Ref<std::string> SourceRef;
        ShaderType Type;
    };

    using ShaderSourceField = AssetField<ShaderSource>;

}
