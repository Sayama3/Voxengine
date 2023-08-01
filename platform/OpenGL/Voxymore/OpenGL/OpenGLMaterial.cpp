//
// Created by ianpo on 01/08/2023.
//

#include "OpenGLMaterial.hpp"
#include <glad/glad.h>

namespace Voxymore {
    namespace Core {
        OpenGLMaterial::OpenGLMaterial(Ref<Shader> &shader) : m_Shader(shader), m_Uniforms()
        {
            m_Uniforms.reserve(m_Shader->GetUniforms().size());
            for (const std::string& uniform : m_Shader->GetUniforms()) {
                m_Uniforms[uniform] = {ShaderDataType::None, nullptr};
            }
        }

        void OpenGLMaterial::SetUniformInt(const std::string& name, int value)
        {
            VXM_CORE_ASSERT(m_Uniforms.contains(name), "The uniform '{0}' doesn't exist.");
            if(m_Uniforms[name].ValuePtr != nullptr)
            {
                Delete(m_Uniforms[name]);
            }
            m_Uniforms[name].DataType = ShaderDataType::Int;
            m_Uniforms[name].ValuePtr = new int(value);
        }

        void OpenGLMaterial::SetUniformFloat(const std::string& name, float value)
        {
            VXM_CORE_ASSERT(m_Uniforms.contains(name), "The uniform '{0}' doesn't exist.");
            if(m_Uniforms[name].ValuePtr != nullptr)
            {
                Delete(m_Uniforms[name]);
            }
            m_Uniforms[name].DataType = ShaderDataType::Float;
            m_Uniforms[name].ValuePtr = new float(value);
        }
        void OpenGLMaterial::SetUniformFloat2(const std::string& name, const glm::vec2& value)
        {
            VXM_CORE_ASSERT(m_Uniforms.contains(name), "The uniform '{0}' doesn't exist.");
            if(m_Uniforms[name].ValuePtr != nullptr)
            {
                Delete(m_Uniforms[name]);
            }
            m_Uniforms[name].DataType = ShaderDataType::Float2;
            m_Uniforms[name].ValuePtr = new glm::vec2(value);
        }
        void OpenGLMaterial::SetUniformFloat3(const std::string& name, const glm::vec3& value)
        {
            VXM_CORE_ASSERT(m_Uniforms.contains(name), "The uniform '{0}' doesn't exist.");
            if(m_Uniforms[name].ValuePtr != nullptr)
            {
                Delete(m_Uniforms[name]);
            }
            m_Uniforms[name].DataType = ShaderDataType::Float3;
            m_Uniforms[name].ValuePtr = new glm::vec3(value);
        }
        void OpenGLMaterial::SetUniformFloat4(const std::string& name, const glm::vec4& value)
        {
            VXM_CORE_ASSERT(m_Uniforms.contains(name), "The uniform '{0}' doesn't exist.");
            if(m_Uniforms[name].ValuePtr != nullptr)
            {
                Delete(m_Uniforms[name]);
            }
            m_Uniforms[name].DataType = ShaderDataType::Float4;
            m_Uniforms[name].ValuePtr = new glm::vec4(value);
        }

        void OpenGLMaterial::SetUniformMat3(const std::string& name, const glm::mat3& value)
        {
            VXM_CORE_ASSERT(m_Uniforms.contains(name), "The uniform '{0}' doesn't exist.");
            if(m_Uniforms[name].ValuePtr != nullptr)
            {
                Delete(m_Uniforms[name]);
            }
            m_Uniforms[name].DataType = ShaderDataType::Mat3;
            m_Uniforms[name].ValuePtr = new glm::mat3(value);
        }
        void OpenGLMaterial::SetUniformMat4(const std::string& name, const glm::mat4& value)
        {
            VXM_CORE_ASSERT(m_Uniforms.contains(name), "The uniform '{0}' doesn't exist.");
            if(m_Uniforms[name].ValuePtr != nullptr)
            {
                Delete(m_Uniforms[name]);
            }
            m_Uniforms[name].DataType = ShaderDataType::Mat4;
            m_Uniforms[name].ValuePtr = new glm::mat4(value);
        }

        void OpenGLMaterial::Delete(MaterialValue<void>& materialValue) {
            VXM_CORE_ASSERT(materialValue.ValuePtr != nullptr, "The ValuePtr must be different than nullptr to be deleted.");
            switch (materialValue.DataType) {
                case ShaderDataType::Float: delete (float*)materialValue.ValuePtr; break;
                case ShaderDataType::Float2: delete (glm::vec2*)materialValue.ValuePtr; break;
                case ShaderDataType::Float3: delete (glm::vec3*)materialValue.ValuePtr; break;
                case ShaderDataType::Float4: delete (glm::vec4*)materialValue.ValuePtr; break;
                case ShaderDataType::Mat2: delete (glm::mat2*)materialValue.ValuePtr; break;
                case ShaderDataType::Mat3: delete (glm::mat3*)materialValue.ValuePtr; break;
                case ShaderDataType::Mat4: delete (glm::mat4*)materialValue.ValuePtr; break;
                case ShaderDataType::Int: delete (int*)materialValue.ValuePtr; break;
                case ShaderDataType::Int2: delete (glm::ivec2*)materialValue.ValuePtr; break;
                case ShaderDataType::Int3: delete (glm::ivec3*)materialValue.ValuePtr; break;
                case ShaderDataType::Int4: delete (glm::ivec4*)materialValue.ValuePtr; break;
                case ShaderDataType::Bool: delete (bool*)materialValue.ValuePtr; break;
                case ShaderDataType::Bool2: delete (glm::bvec2*)materialValue.ValuePtr; break;
                case ShaderDataType::Bool3: delete (glm::bvec3*)materialValue.ValuePtr; break;
                case ShaderDataType::Bool4: delete (glm::bvec4*)materialValue.ValuePtr; break;
                default: {VXM_CORE_ASSERT(false, "Couldn't delete the type '{0}'.", ShaderDataTypeToString(materialValue.DataType));} break;
            }
        }

        OpenGLMaterial::~OpenGLMaterial()
        {
            for (auto&& uniform : m_Uniforms)
            {
                if(uniform.second.ValuePtr != nullptr) {
                    Delete(uniform.second);
                }
            }
        }

    } // Voxymore
} // Core