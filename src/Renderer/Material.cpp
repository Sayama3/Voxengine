//
// Created by ianpo on 01/08/2023.
//

#include "Voxymore/Renderer/Material.hpp"


namespace Voxymore::Core {

    Material::Material(Ref<Shader> &shader) : m_Shader(shader), m_Uniforms()
    {
        VXM_PROFILE_FUNCTION();
        m_Uniforms.reserve(m_Shader->GetUniforms().size());
        for (const std::string& uniform : m_Shader->GetUniforms()) {
            m_Uniforms[uniform] = {ShaderDataType::None, nullptr};
        }
    }

    void Material::SetUniformInt(const std::string& name, int value)
    {
        VXM_PROFILE_FUNCTION();
        VXM_CORE_ASSERT(m_Uniforms.contains(name), "The uniform '{0}' doesn't exist.");
        if(m_Uniforms[name].ValuePtr != nullptr)
        {
            Delete(m_Uniforms[name]);
        }
        m_Uniforms[name].DataType = ShaderDataType::Int;
        m_Uniforms[name].ValuePtr = new int(value);
    }

    void Material::SetUniformFloat(const std::string& name, float value)
    {
        VXM_PROFILE_FUNCTION();
        VXM_CORE_ASSERT(m_Uniforms.contains(name), "The uniform '{0}' doesn't exist.");
        if(m_Uniforms[name].ValuePtr != nullptr)
        {
            Delete(m_Uniforms[name]);
        }
        m_Uniforms[name].DataType = ShaderDataType::Float;
        m_Uniforms[name].ValuePtr = new float(value);
    }
    void Material::SetUniformFloat2(const std::string& name, const glm::vec2& value)
    {
        VXM_PROFILE_FUNCTION();
        VXM_CORE_ASSERT(m_Uniforms.contains(name), "The uniform '{0}' doesn't exist.");
        if(m_Uniforms[name].ValuePtr != nullptr)
        {
            Delete(m_Uniforms[name]);
        }
        m_Uniforms[name].DataType = ShaderDataType::Float2;
        m_Uniforms[name].ValuePtr = new glm::vec2(value);
    }
    void Material::SetUniformFloat3(const std::string& name, const glm::vec3& value)
    {
        VXM_PROFILE_FUNCTION();
        VXM_CORE_ASSERT(m_Uniforms.contains(name), "The uniform '{0}' doesn't exist.");
        if(m_Uniforms[name].ValuePtr != nullptr)
        {
            Delete(m_Uniforms[name]);
        }
        m_Uniforms[name].DataType = ShaderDataType::Float3;
        m_Uniforms[name].ValuePtr = new glm::vec3(value);
    }
    void Material::SetUniformFloat4(const std::string& name, const glm::vec4& value)
    {
        VXM_PROFILE_FUNCTION();
        VXM_CORE_ASSERT(m_Uniforms.contains(name), "The uniform '{0}' doesn't exist.");
        if(m_Uniforms[name].ValuePtr != nullptr)
        {
            Delete(m_Uniforms[name]);
        }
        m_Uniforms[name].DataType = ShaderDataType::Float4;
        m_Uniforms[name].ValuePtr = new glm::vec4(value);
    }

    void Material::SetUniformMat3(const std::string& name, const glm::mat3& value)
    {
        VXM_PROFILE_FUNCTION();
        VXM_CORE_ASSERT(m_Uniforms.contains(name), "The uniform '{0}' doesn't exist.");
        if(m_Uniforms[name].ValuePtr != nullptr)
        {
            Delete(m_Uniforms[name]);
        }
        m_Uniforms[name].DataType = ShaderDataType::Mat3;
        m_Uniforms[name].ValuePtr = new glm::mat3(value);
    }
    void Material::SetUniformMat4(const std::string& name, const glm::mat4& value)
    {
        VXM_PROFILE_FUNCTION();
        VXM_CORE_ASSERT(m_Uniforms.contains(name), "The uniform '{0}' doesn't exist.");
        if(m_Uniforms[name].ValuePtr != nullptr)
        {
            Delete(m_Uniforms[name]);
        }
        m_Uniforms[name].DataType = ShaderDataType::Mat4;
        m_Uniforms[name].ValuePtr = new glm::mat4(value);
    }

    void Material::Delete(MaterialValue<void>& materialValue) {
        VXM_PROFILE_FUNCTION();
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

    Material::~Material()
    {
        VXM_PROFILE_FUNCTION();
        for (auto&& uniform : m_Uniforms)
        {
            if(uniform.second.ValuePtr != nullptr) {
                Delete(uniform.second);
            }
        }
    }

    void Material::Bind() const {
        VXM_PROFILE_FUNCTION();
        m_Shader->Bind();
        for (auto&& kp : m_Uniforms)
        {
            switch (kp.second.DataType) {
                case ShaderDataType::None: {
                    break;
                }
                case ShaderDataType::Float: {
                    m_Shader->SetUniformFloat(kp.first, *(float*)kp.second.ValuePtr);
                    break;
                }
                case ShaderDataType::Float2: {
                    m_Shader->SetUniformFloat2(kp.first, *(glm::vec2*)kp.second.ValuePtr);
                    break;
                }
                case ShaderDataType::Float3: {
                    m_Shader->SetUniformFloat3(kp.first, *(glm::vec3*)kp.second.ValuePtr);
                    break;
                }
                case ShaderDataType::Float4: {
                    m_Shader->SetUniformFloat4(kp.first, *(glm::vec4*)kp.second.ValuePtr);
                    break;
                }
//                case ShaderDataType::Mat2: {
//                    m_Shader->SetUniformMat2(kp.first, *(glm::mat2*)kp.second.ValuePtr);
//                    break;
//                }
                case ShaderDataType::Mat3: {
                    m_Shader->SetUniformMat3(kp.first, *(glm::mat3*)kp.second.ValuePtr);
                    break;
                }
                case ShaderDataType::Mat4: {
                    m_Shader->SetUniformMat4(kp.first, *(glm::mat4*)kp.second.ValuePtr);
                    break;
                }
                case ShaderDataType::Int: {
                    m_Shader->SetUniformInt(kp.first, *(int*)kp.second.ValuePtr);
                    break;
                }
//                case ShaderDataType::Int2: {
//                    m_Shader->SetUniformInt2(kp.first, *(glm::ivec2*)kp.second.ValuePtr);
//                    break;
//                }
//                case ShaderDataType::Int3: {
//                    m_Shader->SetUniformInt3(kp.first, *(glm::ivec3*)kp.second.ValuePtr);
//                    break;
//                }
//                case ShaderDataType::Int4: {
//                    m_Shader->SetUniformInt4(kp.first, *(glm::ivec4*)kp.second.ValuePtr);
//                    break;
//                }
//                case ShaderDataType::Bool: {
//                    m_Shader->SetUniformBool(kp.first, *(glm::bool*)kp.second.ValuePtr);
//                    break;
//                }
//                case ShaderDataType::Bool2: {
//                    m_Shader->SetUniformBool2(kp.first, *(glm::bvec2*)kp.second.ValuePtr);
//                    break;
//                }
//                case ShaderDataType::Bool3: {
//                    m_Shader->SetUniformBool3(kp.first, *(glm::bvec3*)kp.second.ValuePtr);
//                    break;
//                }
//                case ShaderDataType::Bool4: {
//                    m_Shader->SetUniformBool4(kp.first, *(glm::bvec4*)kp.second.ValuePtr);
//                    break;
//                }
                default:
                    VXM_CORE_ASSERT(false, "The type '{0}' is not handled at the moment.", ShaderDataTypeToString(kp.second.DataType));
            }
        }
    }
    void Material::Unbind() const {
        VXM_PROFILE_FUNCTION();
        m_Shader->Unbind();
    }
} // Voxymore
// Core