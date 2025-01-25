//
// Created by ianpo on 25/01/2025.
//

#pragma once
#include "Voxymore/Debug/Profiling.hpp"

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

    inline std::string ShaderDataTypeToString(const ShaderDataType shaderDataType)
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
}
