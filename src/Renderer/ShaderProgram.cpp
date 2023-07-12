//
// Created by ianpo on 26/05/2023.
//

#include "Renderer/ShaderProgram.hpp"

namespace Voxymore::Core::Renderer {
    ShaderProgram::ShaderProgram(const Shader &shader1) : m_Uniforms(), m_ShaderProgramId(glCreateProgram()) {
        AttachShader(shader1);

        Link();
    }

    ShaderProgram::ShaderProgram(const Shader &shader1, const Shader &shader2)
            : m_Uniforms(), m_ShaderProgramId(glCreateProgram()) {
        AttachShader(shader1);
        AttachShader(shader2);

        Link();
    }

    ShaderProgram::ShaderProgram(const Shader &shader1, const Shader &shader2, const Shader &shader3)
            : m_Uniforms(), m_ShaderProgramId(glCreateProgram()) {
        AttachShader(shader1);
        AttachShader(shader2);
        AttachShader(shader3);

        Link();
    }

    ShaderProgram::ShaderProgram(const std::vector<ShaderConstructor> &shaders)
            : m_Uniforms(), m_ShaderProgramId(glCreateProgram()) {
        auto shadersCount = shaders.size();
        for (int i = 0; i < shadersCount; ++i) {
            auto shader = shaders[i].CreateShader();
            shader.CheckCompilation();
            AttachShader(shader);
        }
        Link();
    }

    ShaderProgram::ShaderProgram(const std::vector<Shader> &shaders)
            : m_Uniforms(), m_ShaderProgramId(glCreateProgram()) {
        for (const auto &shader: shaders) {
            shader.CheckCompilation();
            AttachShader(shader);
        }
        Link();
    }

    ShaderProgram::ShaderProgram(ShaderConstructor shader1) : m_Uniforms(), m_ShaderProgramId(glCreateProgram()) {
        auto instance1 = shader1.CreateShader();
        instance1.CheckCompilation();
        AttachShader(instance1);

        Link();
    }

    ShaderProgram::ShaderProgram(ShaderConstructor shader1, ShaderConstructor shader2)
            : m_Uniforms(), m_ShaderProgramId(glCreateProgram()) {
        auto instance1 = shader1.CreateShader();
        instance1.CheckCompilation();
        AttachShader(instance1);

        auto instance2 = shader2.CreateShader();
        instance2.CheckCompilation();
        AttachShader(instance2);

        Link();
    }

    ShaderProgram::ShaderProgram(ShaderConstructor shader1, ShaderConstructor shader2, ShaderConstructor shader3)
            : m_Uniforms(), m_ShaderProgramId(glCreateProgram()) {
        auto instance1 = shader1.CreateShader();
        instance1.CheckCompilation();
        AttachShader(instance1);

        auto instance2 = shader2.CreateShader();
        instance2.CheckCompilation();
        AttachShader(instance2);

        auto instance3 = shader3.CreateShader();
        instance3.CheckCompilation();
        AttachShader(instance3);

        Link();
    }

    ShaderProgram::~ShaderProgram() {
        glDeleteProgram(m_ShaderProgramId);
    }

    void ShaderProgram::Bind() const {
        glUseProgram(m_ShaderProgramId);
    }

    void ShaderProgram::Unbind() const {
        glUseProgram(0);
    }

    unsigned int ShaderProgram::GetId() const {
        return m_ShaderProgramId;
    }

    void ShaderProgram::AttachShader(unsigned int shaderId) {
        glAttachShader(m_ShaderProgramId, shaderId);
    }

    void ShaderProgram::AttachShader(const Shader &shader) {
        glAttachShader(m_ShaderProgramId, shader.GetId());
    }


    void ShaderProgram::Link() {
        glLinkProgram(m_ShaderProgramId);
    }

    int ShaderProgram::GetUniformLocation(const std::string &name) {
        if (m_Uniforms.contains(name)) {
            return m_Uniforms[name];
        }

        int location = glGetUniformLocation(m_ShaderProgramId, name.c_str());
        m_Uniforms.insert({name, location});
        return location;
    }


    template<>
    void ShaderProgram::SetUniform(const std::string &name, const bool &value) {
        Bind();
        glUniform1i(GetUniformLocation(name), value ? 1 : 0);
    }

    template<>
    void ShaderProgram::SetUniform(const std::string &name, const float &value) {
        // TODO: See if I should bind the ShaderProgram before setting the uniform. Seems fair to me.
        Bind();
        glUniform1f(GetUniformLocation(name), value);
    }

    template<>
    void ShaderProgram::SetUniform(const std::string &name, const glm::vec2 &value) {
        // TODO: See if I should bind the ShaderProgram before setting the uniform. Seems fair to me.
        Bind();
        glUniform2f(GetUniformLocation(name), value.x, value.y);
    }

    template<>
    void ShaderProgram::SetUniform(const std::string &name, const glm::vec3 &value) {
        // TODO: See if I should bind the ShaderProgram before setting the uniform. Seems fair to me.
        Bind();
        glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
    }

    template<>
    void ShaderProgram::SetUniform(const std::string &name, const glm::vec4 &value) {
        // TODO: See if I should bind the ShaderProgram before setting the uniform. Seems fair to me.
        Bind();
        glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w);
    }

    template<>
    void ShaderProgram::SetUniform(const std::string &name, const unsigned int &value) {
        // TODO: See if I should bind the ShaderProgram before setting the uniform. Seems fair to me.
        Bind();
        glUniform1ui(GetUniformLocation(name), value);
    }

    template<>
    void ShaderProgram::SetUniform(const std::string &name, const int &value) {
        // TODO: See if I should bind the ShaderProgram before setting the uniform. Seems fair to me.
        Bind();
        glUniform1i(GetUniformLocation(name), value);
    }

    template<>
    void ShaderProgram::SetUniform(const std::string &name, const glm::ivec2 &value) {
        // TODO: See if I should bind the ShaderProgram before setting the uniform. Seems fair to me.
        Bind();
        glUniform2i(GetUniformLocation(name), value.x, value.y);
    }

    template<>
    void ShaderProgram::SetUniform(const std::string &name, const glm::ivec3 &value) {
        // TODO: See if I should bind the ShaderProgram before setting the uniform. Seems fair to me.
        Bind();
        glUniform3i(GetUniformLocation(name), value.x, value.y, value.z);
    }

    template<>
    void ShaderProgram::SetUniform(const std::string &name, const glm::ivec4 &value) {
        // TODO: See if I should bind the ShaderProgram before setting the uniform. Seems fair to me.
        Bind();
        glUniform4i(GetUniformLocation(name), value.x, value.y, value.z, value.w);
    }

    template<>
    void ShaderProgram::SetUniform(const std::string &name, const glm::mat4 &value) {
        // TODO: See if I should bind the ShaderProgram before setting the uniform. Seems fair to me.
        Bind();
        glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
    }
}