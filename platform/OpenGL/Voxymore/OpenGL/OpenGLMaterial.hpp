//
// Created by ianpo on 01/08/2023.
//

#pragma once

#include "Voxymore/Renderer/Material.hpp"
#include "Voxymore/Renderer/Buffer.hpp"

namespace Voxymore {
    namespace Core {

        template<typename T>
        struct MaterialValue {
            ShaderDataType DataType;
            T* ValuePtr;
        };

        class OpenGLMaterial : public Material {
        public:
            OpenGLMaterial(Ref<Shader> &shader);
            virtual ~OpenGLMaterial() override;

            virtual void SetUniformInt(const std::string& name, int value) override;

            virtual void SetUniformFloat(const std::string& name, float value) override;
            virtual void SetUniformFloat2(const std::string& name, const glm::vec2& value) override;
            virtual void SetUniformFloat3(const std::string& name, const glm::vec3& value) override;
            virtual void SetUniformFloat4(const std::string& name, const glm::vec4& value) override;

            virtual void SetUniformMat3(const std::string& name, const glm::mat3& value) override;
            virtual void SetUniformMat4(const std::string& name, const glm::mat4& value) override;
        private:
            void Delete(MaterialValue<void>& materialValue);
            Ref<Shader> m_Shader;
//            std::vector<std::string> m_Uniforms;
            std::unordered_map<std::string, MaterialValue<void>> m_Uniforms;
        };

    } // Voxymore
} // Core
