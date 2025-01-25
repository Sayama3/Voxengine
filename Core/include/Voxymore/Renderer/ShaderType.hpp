//
// Created by ianpo on 25/01/2025.
//

#pragma once
#include "ShaderDefines.hpp"
#include "Voxymore/Debug/Profiling.hpp"

namespace Voxymore::Core {

    enum class ShaderType : uint8_t {
        None = 0,
        VERTEX_SHADER,
        TESS_CONTROL_SHADER,
        TESS_EVALUATION_SHADER,
        GEOMETRY_SHADER,
        FRAGMENT_SHADER,
		COMPUTE_SHADER,
    };
    inline static constexpr int ShaderTypeCount = 6;


	namespace Utils {
		inline static std::string ShaderTypeToString(const ShaderType shaderType)
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

		inline static std::string ShaderTypeToStringBeautify(ShaderType shaderType)
		{
			VXM_PROFILE_FUNCTION();

			switch (shaderType) {
				case ShaderType::COMPUTE_SHADER:
					return "Compute Shader";
					break;
				case ShaderType::VERTEX_SHADER:
					return "Vertex Shader";
					break;
				case ShaderType::TESS_CONTROL_SHADER:
					return "Tessellation Control Shader";
					break;
				case ShaderType::TESS_EVALUATION_SHADER:
					return "Tessellation Evaluation Shader";
					break;
				case ShaderType::GEOMETRY_SHADER:
					return "Geometry Shader";
					break;
				case ShaderType::FRAGMENT_SHADER:
					return "Fragment Shader";
					break;
				case ShaderType::None: break;
					return "None";
					break;
			}
			return "Unknown";
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
}
