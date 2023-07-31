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

    class Shader {
    public:
        virtual ~Shader() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual const std::string& GetName() const = 0;

        static Ref<Shader> Create(const std::string& path);
        static Ref<Shader> Create(const std::string& name, const std::string& path);
        static Ref<Shader> Create(const std::string& name, const std::string& srcVertex, const std::string& srcFragment);
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
