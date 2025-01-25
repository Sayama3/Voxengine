//
// Created by ianpo on 25/01/2025.
//

#pragma once
#include "Voxymore/Renderer/ShaderType.hpp"
#include "Voxymore/Assets/Asset.hpp"
#include "Voxymore/Assets/AssetField.hpp"

namespace Voxymore::Core {

    struct ShaderSource : public Asset
    {
        VXM_IMPLEMENT_ASSET(AssetType::ShaderSource);

        inline ShaderSource() = default;
        inline ~ShaderSource() = default;
        ShaderSource(ShaderType type);
        ShaderSource(std::string source);
        ShaderSource(ShaderType type, std::string source);

        std::string Source;
        ShaderType Type;

        inline const char* GetRawString() const {return Source.c_str();}
        inline std::string GetString() const {return GetRawString();}
    };

    using ShaderSourceField = AssetField<ShaderSource>;

}
