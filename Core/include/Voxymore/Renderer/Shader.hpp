//
// Created by ianpo on 20/07/2023.
//

#pragma once

#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Core/FileSystem.hpp"
#include "Voxymore/Core/SmartPointers.hpp"
#include "Voxymore/Core/SystemHelper.hpp"
#include "Voxymore/Assets/Asset.hpp"
#include "Voxymore/Assets/AssetField.hpp"
#include "Voxymore/Math/Math.hpp"
#include <unordered_map>

#define VXM_VERTEX_TYPE "__TYPE_VERTEX_SHADER__"
#define VXM_FRAGMENT_TYPE "__TYPE_FRAGMENT_SHADER__"
#define VXM_PIXEL_TYPE "__TYPE_PIXEL_SHADER__"
#define VXM_GEOMETRY_TYPE "__TYPE_GEOMETRY_SHADER__"
#define VXM_COMPUTE_TYPE "__TYPE_COMPUTE_SHADER__"
#define VXM_TESS_CONTROL_SHADER_TYPE "__TYPE_TESS_CONTROL_SHADER__"
#define VXM_TESS_EVALUATION_SHADER_TYPE "__TYPE_TESS_EVALUATION_SHADER__"

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
		VXM_PROFILE_FUNCTION();
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
		VXM_CORE_ASSERT(false, "The type {0} is unknown...", (int)type);
		return 0;
	}

    enum class ShaderType : uint8_t {
        None = 0,
        COMPUTE_SHADER,
        VERTEX_SHADER,
        TESS_CONTROL_SHADER,
        TESS_EVALUATION_SHADER,
        GEOMETRY_SHADER,
        FRAGMENT_SHADER,
    };
    static const int ShaderTypeCount = 6;


	namespace Utils {
		inline static std::string ShaderTypeToString(ShaderType shaderType)
		{
			VXM_PROFILE_FUNCTION();

			switch (shaderType) {
				case ShaderType::COMPUTE_SHADER:
					return VXM_COMPUTE_TYPE;
					break;
				case ShaderType::VERTEX_SHADER:
					return VXM_VERTEX_TYPE;
					break;
				case ShaderType::TESS_CONTROL_SHADER:
					return VXM_TESS_CONTROL_SHADER_TYPE;
					break;
				case ShaderType::TESS_EVALUATION_SHADER:
					return VXM_TESS_EVALUATION_SHADER_TYPE;
					break;
				case ShaderType::GEOMETRY_SHADER:
					return VXM_GEOMETRY_TYPE;
					break;
				case ShaderType::FRAGMENT_SHADER:
					return VXM_FRAGMENT_TYPE;
					break;
				case ShaderType::None: break;
					return "__TYPE_NONE__";
					break;
			}
			return "__TYPE_UNKNOWN__";
		}
		inline static ShaderType ShaderTypeFromString(std::string type)
		{
			VXM_PROFILE_FUNCTION();
			if (type == (VXM_VERTEX_TYPE)) return ShaderType::VERTEX_SHADER;
			else if (type == (VXM_FRAGMENT_TYPE) || type == VXM_PIXEL_TYPE)
				return ShaderType::FRAGMENT_SHADER;
			else if (type == (VXM_GEOMETRY_TYPE))
				return ShaderType::GEOMETRY_SHADER;
			else if (type == (VXM_COMPUTE_TYPE))
				return ShaderType::COMPUTE_SHADER;
			else if (type == (VXM_TESS_CONTROL_SHADER_TYPE))
				return ShaderType::TESS_CONTROL_SHADER;
			else if (type == (VXM_TESS_EVALUATION_SHADER_TYPE))
				return ShaderType::TESS_EVALUATION_SHADER;
			else if (type == ("__TYPE_NONE__"))
				return ShaderType::None;
			VXM_CORE_ASSERT(false, "Type {0} unknown.", type);
			return ShaderType::None;
		}
	}

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

    class Shader : public Asset
	{
    public:
		VXM_IMPLEMENT_ASSET(AssetType::Shader);
        virtual ~Shader() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
		virtual void Reload() = 0;

        virtual std::string GetName() const = 0;
        virtual void SetName(const std::string& name) = 0;

		virtual std::vector<ShaderSourceField> GetSources() const = 0;
		virtual void SetSources(const std::vector<ShaderSourceField>& sources) = 0;

		static Ref<Shader> Create(const std::string& name, const std::unordered_map<ShaderType, ShaderSourceField>& sources);
		static Ref<Shader> Create(const std::string& name, const std::vector<ShaderSourceField>& sources);
    };

	using ShaderField = AssetField<Shader>;

} // Core
