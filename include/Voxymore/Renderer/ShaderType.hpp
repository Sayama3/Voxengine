//
// Created by ianpo on 26/05/2023.
//

#ifndef LEARNOPENGL_SHADERTYPE_HPP
#define LEARNOPENGL_SHADERTYPE_HPP

#include <glad/glad.h>
#include <string>

namespace Voxymore::Core {

    enum ShaderType {
        COMPUTE_SHADER = GL_COMPUTE_SHADER,
        VERTEX_SHADER = GL_VERTEX_SHADER,
        TESS_CONTROL_SHADER = GL_TESS_CONTROL_SHADER,
        TESS_EVALUATION_SHADER = GL_TESS_EVALUATION_SHADER,
        GEOMETRY_SHADER = GL_GEOMETRY_SHADER,
        FRAGMENT_SHADER = GL_FRAGMENT_SHADER
    };

    inline std::string ShaderTypeToString(ShaderType shaderType) {
        std::string result = "NotFound";
        switch (shaderType) {
            case COMPUTE_SHADER: {
                result = "COMPUTE_SHADER";
                break;
            }
            case VERTEX_SHADER: {
                result = "VERTEX_SHADER";
                break;
            }
            case TESS_CONTROL_SHADER: {
                result = "TESS_CONTROL_SHADER";
                break;
            }
            case TESS_EVALUATION_SHADER: {
                result = "TESS_EVALUATION_SHADER";
                break;
            }
            case GEOMETRY_SHADER: {
                result = "GEOMETRY_SHADER";
                break;
            }
            case FRAGMENT_SHADER: {
                result = "FRAGMENT_SHADER";
                break;
            }
        }
        return result;
    }
}
#endif //LEARNOPENGL_SHADERTYPE_HPP
