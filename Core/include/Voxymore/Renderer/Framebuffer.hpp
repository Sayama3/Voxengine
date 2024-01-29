//
// Created by ianpo on 21/08/2023.
//

#pragma once

#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Core/Math.hpp"
#include "Voxymore/Core/SmartPointers.hpp"

namespace Voxymore::Core {

	enum class FramebufferTextureFormat
	{
		None = 0,
		// Color
		RGBA8,
		RGBA16,
		RED_INTEGER,

		// Depth/Stencil
		DEPTH24STENCIL8,

		//Default
		Color = RGBA8,
		Depth = DEPTH24STENCIL8,
	};

	struct FramebufferTextureSpecification
	{
		FramebufferTextureSpecification() = default;
		inline FramebufferTextureSpecification(FramebufferTextureFormat format) : TextureFormat(format) {}

		FramebufferTextureFormat TextureFormat;
		//TODO: filtering/wrap
	};

	struct FramebufferAttachmentSpecification
	{
		FramebufferAttachmentSpecification() = default;
		inline FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments) : Attachments(attachments) {}
		std::vector<FramebufferTextureSpecification> Attachments;
	};

    struct FramebufferSpecification
    {
        uint32_t Width, Height;
        uint32_t Samples = 1;
		FramebufferAttachmentSpecification Attachements;

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

		virtual int ReadPixel(uint32_t index, int x, int y) = 0;

        virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const = 0;
        virtual uint32_t GetDepthAttachmentRendererID() const = 0;

		virtual void ClearColorAttachment(uint32_t index, uint8_t value) = 0;
		virtual void ClearColorAttachment(uint32_t index, uint32_t value) = 0;
		virtual void ClearColorAttachment(uint32_t index, int value) = 0;
		virtual void ClearColorAttachment(uint32_t index, float value) = 0;
    };

} // Voxymore
// Core
