//
// Created by ianpo on 01/08/2023.
//

#include "Voxymore/Renderer/Material.hpp"


namespace Voxymore::Core {

    Material::Material(Ref<Shader> &shader) : m_Shader(shader), m_Uniforms()
    {
        VXM_PROFILE_FUNCTION();
        m_Uniforms.reserve(m_Shader->GetUniforms().size());
        for (std::pair<std::string, UniformDescription>&& uniform : m_Shader->GetUniforms()) {
            m_Uniforms[uniform.first] = MaterialValue(uniform.second.Type);
        }
    }

    void Material::SetUniformInt(const std::string& name, int value)
    {
        VXM_PROFILE_FUNCTION();
        VXM_CORE_ASSERT(m_Uniforms.contains(name), "The uniform '{0}' doesn't exist.");
        m_Uniforms[name].SetValue(value);
    }

    void Material::SetUniformFloat(const std::string& name, float value)
    {
        VXM_PROFILE_FUNCTION();
        VXM_CORE_ASSERT(m_Uniforms.contains(name), "The uniform '{0}' doesn't exist.");
        m_Uniforms[name].SetValue(value);
    }
    void Material::SetUniformFloat2(const std::string& name, const glm::vec2& value)
    {
        VXM_PROFILE_FUNCTION();
        VXM_CORE_ASSERT(m_Uniforms.contains(name), "The uniform '{0}' doesn't exist.");
        m_Uniforms[name].SetValue(value);
    }
    void Material::SetUniformFloat3(const std::string& name, const glm::vec3& value)
    {
        VXM_PROFILE_FUNCTION();
        VXM_CORE_ASSERT(m_Uniforms.contains(name), "The uniform '{0}' doesn't exist.");
        m_Uniforms[name].SetValue(value);
    }
    void Material::SetUniformFloat4(const std::string& name, const glm::vec4& value)
    {
        VXM_PROFILE_FUNCTION();
        VXM_CORE_ASSERT(m_Uniforms.contains(name), "The uniform '{0}' doesn't exist.");
        m_Uniforms[name].SetValue(value);
    }

    void Material::SetUniformMat3(const std::string& name, const glm::mat3& value)
    {
        VXM_PROFILE_FUNCTION();
        VXM_CORE_ASSERT(m_Uniforms.contains(name), "The uniform '{0}' doesn't exist.");
        m_Uniforms[name].SetValue(value);
    }
    void Material::SetUniformMat4(const std::string& name, const glm::mat4& value)
    {
        VXM_PROFILE_FUNCTION();
        VXM_CORE_ASSERT(m_Uniforms.contains(name), "The uniform '{0}' doesn't exist.");
        m_Uniforms[name].SetValue(value);
    }

    Material::~Material()
    {
        VXM_PROFILE_FUNCTION();
    }

    void Material::Bind() const {
        VXM_PROFILE_FUNCTION();
        m_Shader->Bind();
        for (auto&& kp : m_Uniforms)
        {
            m_Shader->SetUniform(kp.first, kp.second.GetValue(), kp.second.GetSize());
        }
    }
    void Material::Unbind() const {
        VXM_PROFILE_FUNCTION();
        m_Shader->Unbind();
    }


    MaterialValue::MaterialValue(ShaderDataType dataType) : DataType(dataType), ValuePtr(nullptr) {
        CreateValue();
    }

    MaterialValue::MaterialValue(const MaterialValue & other) : DataType(other.DataType), ValuePtr(nullptr) {
        CreateValue();
        SetValue(other.GetValue(), other.GetSize());
    }

    MaterialValue::MaterialValue() : DataType(ShaderDataType::None), ValuePtr(nullptr) {
    }

    MaterialValue::~MaterialValue() {
        DeleteValue();
    }

    void MaterialValue::DeleteValue() {
        VXM_PROFILE_FUNCTION();
        if(ValuePtr == nullptr) return;

        switch (DataType) {
            case ShaderDataType::Float: delete (float*)ValuePtr; break;
            case ShaderDataType::Float2: delete (glm::vec2*)ValuePtr; break;
            case ShaderDataType::Float3: delete (glm::vec3*)ValuePtr; break;
            case ShaderDataType::Float4: delete (glm::vec4*)ValuePtr; break;
            case ShaderDataType::Mat2: delete (glm::mat2*)ValuePtr; break;
            case ShaderDataType::Mat3: delete (glm::mat3*)ValuePtr; break;
            case ShaderDataType::Mat4: delete (glm::mat4*)ValuePtr; break;
            case ShaderDataType::Int: delete (int*)ValuePtr; break;
            case ShaderDataType::Int2: delete (glm::ivec2*)ValuePtr; break;
            case ShaderDataType::Int3: delete (glm::ivec3*)ValuePtr; break;
            case ShaderDataType::Int4: delete (glm::ivec4*)ValuePtr; break;
            case ShaderDataType::Bool: delete (bool*)ValuePtr; break;
            case ShaderDataType::Bool2: delete (glm::bvec2*)ValuePtr; break;
            case ShaderDataType::Bool3: delete (glm::bvec3*)ValuePtr; break;
            case ShaderDataType::Bool4: delete (glm::bvec4*)ValuePtr; break;
            case ShaderDataType::Sampler1D: delete (uint32_t*)ValuePtr; break;
            case ShaderDataType::Sampler2D: delete (uint32_t*)ValuePtr; break;
            case ShaderDataType::Sampler3D: delete (uint32_t*)ValuePtr; break;
            default: {VXM_CORE_ASSERT(false, "Couldn't delete the type '{0}'.", ShaderDataTypeToString(DataType));} break;
        }

    }

    void MaterialValue::CreateValue() {
        VXM_PROFILE_FUNCTION();
        VXM_CORE_ASSERT(DataType != ShaderDataType::None, "The DataType must be a valid type.");
        switch (DataType) {
            case ShaderDataType::Float: ValuePtr = new float(); break;
            case ShaderDataType::Float2: ValuePtr = new glm::vec2(); break;
            case ShaderDataType::Float3: ValuePtr = new glm::vec3(); break;
            case ShaderDataType::Float4: ValuePtr = new glm::vec4(); break;
            case ShaderDataType::Mat2: ValuePtr = new glm::mat2(); break;
            case ShaderDataType::Mat3: ValuePtr = new glm::mat3(); break;
            case ShaderDataType::Mat4: ValuePtr = new glm::mat4(); break;
            case ShaderDataType::Int: ValuePtr = new int(); break;
            case ShaderDataType::Int2: ValuePtr = new glm::ivec2(); break;
            case ShaderDataType::Int3: ValuePtr = new glm::ivec3(); break;
            case ShaderDataType::Int4: ValuePtr = new glm::ivec4(); break;
            case ShaderDataType::Bool: ValuePtr = new bool(); break;
            case ShaderDataType::Bool2: ValuePtr = new glm::bvec2(); break;
            case ShaderDataType::Bool3: ValuePtr = new glm::bvec3(); break;
            case ShaderDataType::Bool4: ValuePtr = new glm::bvec4(); break;
            case ShaderDataType::Sampler1D: ValuePtr = new uint32_t(); break;
            case ShaderDataType::Sampler2D: ValuePtr = new uint32_t(); break;
            case ShaderDataType::Sampler3D: ValuePtr = new uint32_t(); break;
            default: {VXM_CORE_ASSERT(false, "Couldn't delete the type '{0}'.", ShaderDataTypeToString(DataType));} break;
        }
    }

    MaterialValue& MaterialValue::operator= (const MaterialValue& other)
    {
        VXM_PROFILE_FUNCTION();
        this->DeleteValue();
        this->DataType = other.DataType;
        this->CreateValue();
        this->SetValue(other.GetValue(), other.GetSize());
        return *this;
    };

    template<typename T>
    void MaterialValue::SetValue(const T &value) {
        VXM_PROFILE_FUNCTION();
        SetValue(&value, sizeof(T));
    }

    void MaterialValue::SetValue(const void* value, uint32_t size)
    {
        VXM_PROFILE_FUNCTION();
        VXM_CORE_ASSERT(size == ShaderDataTypeSize(DataType), "The size is different than the one in the struct.");
        switch (DataType) {
            case ShaderDataType::Float: *(float*)ValuePtr = *(float*)value; break;
            case ShaderDataType::Float2: *(glm::vec2*)ValuePtr = *(glm::vec2*)value; break;
            case ShaderDataType::Float3: *(glm::vec3*)ValuePtr = *(glm::vec3*)value; break;
            case ShaderDataType::Float4: *(glm::vec4*)ValuePtr = *(glm::vec4*)value; break;
            case ShaderDataType::Mat2: *(glm::mat2*)ValuePtr = *(glm::mat2*)value; break;
            case ShaderDataType::Mat3: *(glm::mat3*)ValuePtr = *(glm::mat3*)value; break;
            case ShaderDataType::Mat4: *(glm::mat4*)ValuePtr = *(glm::mat4*)value; break;
            case ShaderDataType::Int: *(int*)ValuePtr = *(int*)value; break;
            case ShaderDataType::Int2: *(glm::ivec2*)ValuePtr = *(glm::ivec2*)value; break;
            case ShaderDataType::Int3: *(glm::ivec3*)ValuePtr = *(glm::ivec3*)value; break;
            case ShaderDataType::Int4: *(glm::ivec4*)ValuePtr = *(glm::ivec4*)value; break;
            case ShaderDataType::Bool: *(bool*)ValuePtr = *(bool*)value; break;
            case ShaderDataType::Bool2: *(glm::bvec2*)ValuePtr = *(glm::bvec2*)value; break;
            case ShaderDataType::Bool3: *(glm::bvec3*)ValuePtr = *(glm::bvec3*)value; break;
            case ShaderDataType::Bool4: *(glm::bvec4*)ValuePtr = *(glm::bvec4*)value; break;
            case ShaderDataType::Sampler1D: *(uint32_t*)ValuePtr = *(uint32_t*)value; break;
            case ShaderDataType::Sampler2D: *(uint32_t*)ValuePtr = *(uint32_t*)value; break;
            case ShaderDataType::Sampler3D: *(uint32_t*)ValuePtr = *(uint32_t*)value; break;
            default: {VXM_CORE_ASSERT(false, "Couldn't delete the type '{0}'.", ShaderDataTypeToString(DataType));} break;
        }

    }

    const void* MaterialValue::GetValue() const
    {
        return ValuePtr;
    }

    uint32_t MaterialValue::GetSize() const
    {
        return ShaderDataTypeSize(DataType);
    }

    ShaderDataType MaterialValue::GetType() const
    {
        return DataType;
    }

} // Voxymore
// Core