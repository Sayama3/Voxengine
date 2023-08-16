//
// Created by ianpo on 01/08/2023.
//

#pragma once

#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Core/SmartPointers.hpp"
#include "Voxymore/Renderer/Shader.hpp"
#include "Voxymore/Renderer/Buffer.hpp"


namespace Voxymore::Core {

    template<typename T>
    struct MaterialValue {
        ShaderDataType DataType;
        T* ValuePtr;
    };

    class Material {
    public:
        Material(Ref<Shader>& shader);
        ~Material();

        void SetUniformInt(const std::string& name, int value);

        void SetUniformFloat(const std::string& name, float value);
        void SetUniformFloat2(const std::string& name, const glm::vec2& value);
        void SetUniformFloat3(const std::string& name, const glm::vec3& value);
        void SetUniformFloat4(const std::string& name, const glm::vec4& value);

        void SetUniformMat3(const std::string& name, const glm::mat3& value);
        void SetUniformMat4(const std::string& name, const glm::mat4& value);

        void Bind() const;
        void Unbind() const;
    private:
        void Delete(MaterialValue<void>& materialValue);
        Ref<Shader> m_Shader;
//            std::vector<std::string> m_Uniforms;
        std::unordered_map<std::string, MaterialValue<void>> m_Uniforms;
    };

} // Voxymore
// Core
