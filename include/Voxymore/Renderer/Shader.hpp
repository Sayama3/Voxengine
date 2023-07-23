//
// Created by ianpo on 20/07/2023.
//

#pragma once
#include "Voxymore/Core.hpp"
#include "Voxymore/SystemHelper.hpp"
#include "Voxymore/Math.hpp"


namespace Voxymore {
    namespace Core {
        enum ShaderType {
            COMPUTE_SHADER = BIT(0),
            VERTEX_SHADER = BIT(1),
            TESS_CONTROL_SHADER = BIT(2),
            TESS_EVALUATION_SHADER = BIT(3),
            GEOMETRY_SHADER = BIT(4),
            FRAGMENT_SHADER = BIT(5)
        };

        inline std::string ShaderTypeToString(ShaderType shaderType) {
            std::string result = "NOT_FOUND";
            switch (shaderType) {
                case COMPUTE_SHADER: {
                    result = "ShaderType::COMPUTE_SHADER";
                    break;
                }
                case VERTEX_SHADER: {
                    result = "ShaderType::VERTEX_SHADER";
                    break;
                }
                case TESS_CONTROL_SHADER: {
                    result = "ShaderType::TESS_CONTROL_SHADER";
                    break;
                }
                case TESS_EVALUATION_SHADER: {
                    result = "ShaderType::TESS_EVALUATION_SHADER";
                    break;
                }
                case GEOMETRY_SHADER: {
                    result = "ShaderType::GEOMETRY_SHADER";
                    break;
                }
                case FRAGMENT_SHADER: {
                    result = "ShaderType::FRAGMENT_SHADER";
                    break;
                }
            }
            return result;
        }
        int GetShaderTypeValue(ShaderType shaderType);

        struct ShaderParams{
            const std::string source;
            const ShaderType type;
        };

        ShaderParams CreateShaderParams(ShaderType type, std::string path);

        class Shader {
        public:
            static Shader* CreateShader(const ShaderParams& shader1);
            static Shader* CreateShader(const ShaderParams& shader1, const ShaderParams& shader2);
            static Shader* CreateShader(const ShaderParams& shader1, const ShaderParams& shader2, const ShaderParams& shader3);

            virtual ~Shader() {}
            virtual void Bind() const = 0;
            virtual void Unbind() const = 0;
            virtual bool HasType(ShaderType shaderType) const = 0;

			// Uniforms
			virtual void SetUniformMat4(const std::string& name, const glm::mat4& mat4) = 0;
        };

    } // Voxymore
} // Core
