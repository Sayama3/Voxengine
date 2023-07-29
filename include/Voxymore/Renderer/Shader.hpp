//
// Created by ianpo on 20/07/2023.
//

#pragma once
#include "Voxymore/Core.hpp"
#include "Voxymore/SystemHelper.hpp"
#include "Voxymore/Math.hpp"


namespace Voxymore {
    namespace Core {
        enum class ShaderType {
            None = 0,
            COMPUTE_SHADER,
            VERTEX_SHADER,
            TESS_CONTROL_SHADER,
            TESS_EVALUATION_SHADER,
            GEOMETRY_SHADER,
            FRAGMENT_SHADER
        };

        class Shader {
        public:
            virtual ~Shader() = default;

            virtual void Bind() const = 0;
            virtual void Unbind() const = 0;

            static Shader* Create(const std::vector<std::string>& paths);
            static Shader* Create(const std::initializer_list<std::string>& paths);
            static Shader* Create(const std::unordered_map<ShaderType, std::string>& paths);
            static Shader* Create(const std::string& srcVertex, const std::string& srcFragment);
        };

    } // Voxymore
} // Core
