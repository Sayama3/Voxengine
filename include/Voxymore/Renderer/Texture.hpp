//
// Created by ianpo on 29/07/2023.
//

#pragma once

#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Core/SmartPointers.hpp"
#include "Voxymore/Core/FileSystem.hpp"

namespace Voxymore::Core {

    class Texture {
    public:
        virtual ~Texture() = default;
        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const =0;

        virtual void Bind(uint32_t slot = 0) const = 0;
		virtual uint32_t GetRendererID() const = 0;
    };

    class Texture2D : public Texture {
    public:
        static Ref<Texture2D> Create(const Path& path);
        static Ref<Texture2D> Create(const std::filesystem::path& path);
        static Ref<Texture2D> Create(const uint8_t* data, int width, int height, int channels);
        static Ref<Texture2D> Create(const uint16_t* data, int width, int height, int channels);
    };

} // Core
