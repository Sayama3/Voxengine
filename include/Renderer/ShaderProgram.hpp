//
// Created by ianpo on 26/05/2023.
//

#ifndef LEARNOPENGL_SHADERPROGRAM_HPP
#define LEARNOPENGL_SHADERPROGRAM_HPP

#include "Shader.hpp"
#include "Logger.hpp"

namespace Voxymore::Core::Renderer {
// TODO: Change the class to use Template instead of copy pasting the Shader Path & Type
    class ShaderProgram {
    private:
        std::unordered_map<std::string, int> m_Uniforms;
        unsigned int m_ShaderProgramId;

        void AttachShader(unsigned int shaderId);

        void AttachShader(const Shader &shader);

        void Link();

        int GetUniformLocation(const std::string &name);

    public:
        ShaderProgram(ShaderConstructor shader1);

        ShaderProgram(ShaderConstructor shader1, ShaderConstructor shader2);

        ShaderProgram(ShaderConstructor shader1, ShaderConstructor shader2, ShaderConstructor shader3);

        ShaderProgram(const std::vector<ShaderConstructor> &shaders);

        ShaderProgram(const Shader &shader1);

        ShaderProgram(const Shader &shader1, const Shader &shader2);

        ShaderProgram(const Shader &shader1, const Shader &shader2, const Shader &shader3);

        ShaderProgram(const std::vector<Shader> &shaders);

        ~ShaderProgram();

        ShaderProgram(const ShaderProgram &) = delete;

        ShaderProgram &operator=(const ShaderProgram &) = delete;

        void Bind() const;

        void Unbind() const;

        unsigned int GetId() const;

        template<typename T>
        inline void SetUniform(const std::string &name, const T &value) {
            LOG_ERROR("Type not implemented.");
        }
    };

    template<>
    void ShaderProgram::SetUniform(const std::string &name, const bool &value);

    template<>
    void ShaderProgram::SetUniform(const std::string &name, const int &value);

    template<>
    void ShaderProgram::SetUniform(const std::string &name, const unsigned int &value);

    template<>
    void ShaderProgram::SetUniform(const std::string &name, const glm::vec2 &value);

    template<>
    void ShaderProgram::SetUniform(const std::string &name, const glm::vec3 &value);

    template<>
    void ShaderProgram::SetUniform(const std::string &name, const glm::vec4 &value);

    template<>
    void ShaderProgram::SetUniform(const std::string &name, const float &value);

    template<>
    void ShaderProgram::SetUniform(const std::string &name, const glm::ivec2 &value);

    template<>
    void ShaderProgram::SetUniform(const std::string &name, const glm::ivec3 &value);

    template<>
    void ShaderProgram::SetUniform(const std::string &name, const glm::ivec4 &value);

    template<>
    void ShaderProgram::SetUniform(const std::string &name, const glm::mat4 &value);

    template<>
    void ShaderProgram::SetUniform(const std::string &name, const glm::mat3 &value);

    template<>
    void ShaderProgram::SetUniform(const std::string &name, const glm::mat2 &value);
}
#endif //LEARNOPENGL_SHADERPROGRAM_HPP
