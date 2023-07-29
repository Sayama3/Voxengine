//
// Created by ianpo on 29/07/2023.
//

#pragma once

#include "Voxymore/Core.hpp"
#include "Voxymore/Core/SmartPointers.hpp"

namespace Voxymore::Core {

    class Texture {
    public:
        virtual ~Texture() = default;
        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const =0;

        virtual void Bind(uint32_t slot = 0) const = 0;
    };

    class Texture2D : public Texture {
    public:
        static Ref<Texture2D> Create(const std::string& path);
    };

} // Core
