//
// Created by ianpo on 21/08/2023.
//

#pragma once

#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Core/Math.hpp"
#include "Voxymore/Core/SmartPointers.hpp"

namespace Voxymore::Core {

    struct FramebufferSpecification
    {
        uint32_t Width, Height;
        uint32_t Samples = 1;

        //TODO: add format.
        bool SwapChainTarget = false;

        FramebufferSpecification() = default;
        inline FramebufferSpecification(uint32_t width, uint32_t height) : Width(width), Height(height) {}
    };

    class Framebuffer {
    public:
        static Ref<Framebuffer> Create(const FramebufferSpecification& spec);

        virtual ~Framebuffer() = default;
        virtual const FramebufferSpecification& GetSpecification() const = 0;

        virtual void Bind() = 0;
        virtual void Unbind() = 0;

        virtual void Resize(uint32_t width, uint32_t height) = 0;

        virtual uint32_t GetColorAttachmentRendererID() const = 0;
        virtual uint32_t GetDepthAttachmentRendererID() const = 0;
    };

} // Voxymore
// Core
