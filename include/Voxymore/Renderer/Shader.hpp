//
// Created by ianpo on 20/07/2023.
//

#pragma once
#include "Voxymore/Core.hpp"
#include "Voxymore/SystemHelper.hpp"
#include "Voxymore/Math.hpp"


namespace Voxymore {
    namespace Core {

        class Shader {
        public:
            virtual ~Shader() = default;

            virtual void Bind() const = 0;
            virtual void Unbind() const = 0;

            static Shader* CreateFromSource(const std::string& srcVertex, const std::string& srcFragment);
            static Shader* CreateFromFile(const std::string& vertexFile, const std::string& fragmentFile);
        };

    } // Voxymore
} // Core
