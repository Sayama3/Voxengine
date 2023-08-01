//
// Created by ianpo on 01/08/2023.
//

#pragma once

#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Core/SmartPointers.hpp"
#include "Voxymore/Renderer/Shader.hpp"


namespace Voxymore::Core {
    class Material {
    public:
        inline virtual ~Material() = default;
        static Ref<Material> Create(Ref<Shader>& shader);

        virtual void SetUniformInt(const std::string& name, int value) = 0;

        virtual void SetUniformFloat(const std::string& name, float value) = 0;
        virtual void SetUniformFloat2(const std::string& name, const glm::vec2& value) = 0;
        virtual void SetUniformFloat3(const std::string& name, const glm::vec3& value) = 0;
        virtual void SetUniformFloat4(const std::string& name, const glm::vec4& value) = 0;

        virtual void SetUniformMat3(const std::string& name, const glm::mat3& value) = 0;
        virtual void SetUniformMat4(const std::string& name, const glm::mat4& value) = 0;

    };

} // Voxymore
// Core
