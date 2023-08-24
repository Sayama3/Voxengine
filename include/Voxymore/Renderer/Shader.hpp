//
// Created by ianpo on 20/07/2023.
//

#pragma once
#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Core/SmartPointers.hpp"
#include "Voxymore/Core/SystemHelper.hpp"
#include "Voxymore/Core/Math.hpp"
#include <unordered_map>

namespace Voxymore::Core {
    enum class ShaderDataType {
        None = 0,
        Float,
        Float2,
        Float3,
        Float4,
        Mat2,
        Mat3,
        Mat4,
        Int,
        Int2,
        Int3,
        Int4,
        UInt,
        UInt2,
        UInt3,
        UInt4,
        Bool,
        Bool2,
        Bool3,
        Bool4,
        Sampler1D,
        Sampler2D,
        Sampler3D,
    };

    inline std::string ShaderDataTypeToString(ShaderDataType shaderDataType)
    {
        VXM_PROFILE_FUNCTION();
        switch (shaderDataType) {
            case ShaderDataType::None: return "None";
            case ShaderDataType::Float: return "Float";
            case ShaderDataType::Float2: return "Float2";
            case ShaderDataType::Float3: return "Float3";
            case ShaderDataType::Float4: return "Float4";
            case ShaderDataType::Mat2: return "Mat2";
            case ShaderDataType::Mat3: return "Mat3";
            case ShaderDataType::Mat4: return "Mat4";
            case ShaderDataType::Int: return "Int";
            case ShaderDataType::Int2: return "Int2";
            case ShaderDataType::Int3: return "Int3";
            case ShaderDataType::Int4: return "Int4";
            case ShaderDataType::UInt: return "UInt";
            case ShaderDataType::UInt2: return "UInt2";
            case ShaderDataType::UInt3: return "UInt3";
            case ShaderDataType::UInt4: return "UInt4";
            case ShaderDataType::Bool: return "Bool";
            case ShaderDataType::Bool2: return "Bool2";
            case ShaderDataType::Bool3: return "Bool3";
            case ShaderDataType::Bool4: return "Bool4";
            case ShaderDataType::Sampler1D: return "Sampler1D";
            case ShaderDataType::Sampler2D: return "Sampler2D";
            case ShaderDataType::Sampler3D: return "Sampler3D";
        }
        return "Unknown";
    }

    /**
     * Get the size in byte of a certain shader data type.
     * @param type
     * @return
     */
    inline static uint32_t GetShaderDataTypeSize(ShaderDataType type)
    {
        VXM_PROFILE_FUNCTION();
        switch (type) {
            case ShaderDataType::None:      return 0;
            case ShaderDataType::Float:     return 4 * 1;
            case ShaderDataType::Float2:    return 4 * 2;
            case ShaderDataType::Float3:    return 4 * 3;
            case ShaderDataType::Float4:    return 4 * 4;
            case ShaderDataType::Mat2:      return 4 * 2 * 2;
            case ShaderDataType::Mat3:      return 4 * 3 * 3;
            case ShaderDataType::Mat4:      return 4 * 4 * 4;
            case ShaderDataType::Int:       return 4 * 1;
            case ShaderDataType::Int2:      return 4 * 2;
            case ShaderDataType::Int3:      return 4 * 3;
            case ShaderDataType::Int4:      return 4 * 4;
            case ShaderDataType::UInt:      return 4 * 1;
            case ShaderDataType::UInt2:     return 4 * 2;
            case ShaderDataType::UInt3:     return 4 * 3;
            case ShaderDataType::UInt4:     return 4 * 4;
            case ShaderDataType::Bool:      return 1 * 1;
            case ShaderDataType::Bool2:     return 1 * 2;
            case ShaderDataType::Bool3:     return 1 * 3;
            case ShaderDataType::Bool4:     return 1 * 4;
            case ShaderDataType::Sampler1D: return 4 * 1;
            case ShaderDataType::Sampler2D: return 4 * 1;
            case ShaderDataType::Sampler3D: return 4 * 1;
        }

        VXM_CORE_ERROR("Unknown ShaderDataType {0}.", (int)type);
        return 0;
    }

	inline static uint32_t GetShaderDataTypeCount(ShaderDataType type)
	{
		switch (type) {
			case ShaderDataType::Float: return 1;
			case ShaderDataType::Float2: return 2;
			case ShaderDataType::Float3: return 3;
			case ShaderDataType::Float4: return 4;

			case ShaderDataType::Mat2: return 2 * 2;
			case ShaderDataType::Mat3: return 3 * 3;
			case ShaderDataType::Mat4: return 4 * 4;

			case ShaderDataType::Int: return 1;
			case ShaderDataType::Int2: return 2;
			case ShaderDataType::Int3: return 3;
			case ShaderDataType::Int4: return 4;

			case ShaderDataType::UInt: return 1;
			case ShaderDataType::UInt2: return 2;
			case ShaderDataType::UInt3: return 3;
			case ShaderDataType::UInt4: return 4;

			case ShaderDataType::Bool: return 1;
			case ShaderDataType::Bool2: return 2;
			case ShaderDataType::Bool3: return 3;
			case ShaderDataType::Bool4: return 4;

			case ShaderDataType::Sampler1D: return 1;
			case ShaderDataType::Sampler2D: return 1;
			case ShaderDataType::Sampler3D: return 1;
		}
	}

    enum class ShaderType : int {
        None = 0,
        COMPUTE_SHADER,
        VERTEX_SHADER,
        TESS_CONTROL_SHADER,
        TESS_EVALUATION_SHADER,
        GEOMETRY_SHADER,
        FRAGMENT_SHADER,
    };
    static const int ShaderTypeCount = 6;

    struct UniformDescription
    {
    public:
        std::string Name;
        ShaderDataType Type;
        int Size;
    public:
        inline UniformDescription() = default;
        inline UniformDescription(const std::string& name, ShaderDataType type, int size) : Name(name), Type(type), Size(size) {}
    };

    class Shader {
    public:
        virtual ~Shader() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual const std::string& GetName() const = 0;

        static Ref<Shader> Create(const std::string& path);
        static Ref<Shader> Create(const std::string& name, const std::string& path);
        static Ref<Shader> Create(const std::string& name, const std::string& srcVertex, const std::string& srcFragment);

        virtual std::unordered_map<std::string, UniformDescription> GetUniforms() const = 0;

        virtual void SetUniform(const std::string& name, const void* valuePtr, uint32_t size) = 0;

        virtual void SetUniformInt(const std::string& name, int value) = 0;

        virtual void SetUniformFloat(const std::string& name, float value) = 0;
        virtual void SetUniformFloat2(const std::string& name, const glm::vec2& value) = 0;
        virtual void SetUniformFloat3(const std::string& name, const glm::vec3& value) = 0;
        virtual void SetUniformFloat4(const std::string& name, const glm::vec4& value) = 0;

        virtual void SetUniformMat3(const std::string& name, const glm::mat3& value) = 0;
        virtual void SetUniformMat4(const std::string& name, const glm::mat4& value) = 0;
    };

    class ShaderLibrary
    {
    public:
        void Add(const Ref<Shader>& shader);
        void Add(const std::string& name, const Ref<Shader>& shader);
        Ref<Shader> Load(const std::string& path);
        Ref<Shader> Load(const std::string& name, const std::string& path);
        Ref<Shader> Get(const std::string& name);
        bool Exists(const std::string& name) const;
    private:
        std::unordered_map<std::string, Ref<Shader>> m_Shaders;
    };




} // Core
