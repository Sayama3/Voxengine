//
// Created by ianpo on 20/07/2023.
//

#pragma once

#include <memory>
#include "Voxymore/Renderer/Buffer.hpp"

namespace Voxymore::Core {

    class VertexArray {
    public:
        static VertexArray* Create();

        virtual ~VertexArray() {}

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;
        virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) = 0;

        virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const = 0;
    };

} // Core
