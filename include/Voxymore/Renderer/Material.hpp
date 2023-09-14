//
// Created by ianpo on 01/08/2023.
//

#pragma once

#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Core/SmartPointers.hpp"
#include "Voxymore/Renderer/Shader.hpp"
#include "Voxymore/Renderer/Buffer.hpp"


namespace Voxymore::Core {

    struct MaterialValue {
    private :
        ShaderDataType DataType;
        void* ValuePtr;
    public:
        MaterialValue(ShaderDataType dataType);
        ~MaterialValue();

        MaterialValue ();
        MaterialValue (const MaterialValue&);
        MaterialValue& operator= (const MaterialValue&);
    public:
        template<typename T>
        void SetValue(const T& value);
        void SetValue(const void* value, uint32_t size);

        const void* GetValue() const;
        uint32_t GetSize() const;
        ShaderDataType GetType() const;
    private:
        void DeleteValue();
        void CreateValue();
    };

    class Material {
	private:
		Ref<Shader> m_Shader;
		std::unordered_map<std::string, MaterialValue> m_Uniforms;
    public:
        Material(Ref<Shader>& shader);
        ~Material();

        void SetUniformInt(const std::string& name, int value);
		void SetUniformInt2(const std::string& name, const glm::ivec2& value);
		void SetUniformInt3(const std::string& name, const glm::ivec3& value);
		void SetUniformInt4(const std::string& name, const glm::ivec4& value);

        void SetUniformFloat(const std::string& name, float value);
        void SetUniformFloat2(const std::string& name, const glm::vec2& value);
        void SetUniformFloat3(const std::string& name, const glm::vec3& value);
        void SetUniformFloat4(const std::string& name, const glm::vec4& value);

		void SetUniformMat2(const std::string& name, const glm::mat2& value);
        void SetUniformMat3(const std::string& name, const glm::mat3& value);
        void SetUniformMat4(const std::string& name, const glm::mat4& value);

        void Bind() const;
        void Unbind() const;

		inline Ref<Shader> GetShader() { return m_Shader; }
    };

} // Voxymore
// Core
