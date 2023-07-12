//
// Created by ianpo on 26/05/2023.
//

#ifndef LEARNOPENGL_SHADER_HPP
#define LEARNOPENGL_SHADER_HPP

#include <string>
#include "ShaderType.hpp"

#define ViewMatrixName "view"
#define ProjectionMatrixName "projection"
#define ModelMatrixName "model"

namespace Voxymore::Core::Renderer {
    class Shader {
    private:
        const ShaderType m_ShaderType;
        unsigned int m_ShaderId;
    public:
        Shader(const std::string &shaderPath, ShaderType shaderType);

        ~Shader();

        Shader(const Shader &) = delete;

        Shader &operator=(const Shader &) = delete;

        bool CheckCompilation() const;

        unsigned int GetId() const;
    };

    struct ShaderConstructor {
        ShaderConstructor(std::string path, ShaderType type);

        const std::string shaderPath;
        const ShaderType shaderType;

        Shader CreateShader() const;
    };
}

#endif //LEARNOPENGL_SHADER_HPP
