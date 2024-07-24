//
// Created by ianpo on 20/07/2023.
//

#pragma once

#include "Voxymore/Core/SmartPointers.hpp"
#include "Voxymore/Renderer/Buffer.hpp"

namespace Voxymore::Core {

    class VertexArray {
    public:
        static Ref<VertexArray> Create();

        virtual ~VertexArray() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;
        virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) = 0;

        virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;
		virtual uint32_t GetRendererID() const = 0;
    };

} // Core
