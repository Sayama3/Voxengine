//
// Created by ianpo on 20/07/2023.
//

#pragma once

#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Core/Logger.hpp"
#include "Voxymore/Renderer/Shader.hpp"

namespace Voxymore::Core {

    struct BufferElement{
        std::string Name;
        ShaderDataType Type;
        uint32_t Size;
        uint32_t Offset;
        bool Normalized;

        inline BufferElement(ShaderDataType type, const std::string& name, bool normalized = false) : Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized) {}

        inline uint32_t GetComponentCount() const
        {
            switch (Type) {
                case ShaderDataType::Float: return 1;
                case ShaderDataType::Float2: return 2;
                case ShaderDataType::Float3: return 3;
                case ShaderDataType::Float4: return 4;

                case ShaderDataType::Mat2: return 2 * 2;
                case ShaderDataType::Mat3: return 3 * 3;
                case ShaderDataType::Mat4: return 4 * 4;

                case ShaderDataType::Int: return 1;
                case ShaderDataType::Int2: return 2;
                case ShaderDataType::Int3: return 3;
                case ShaderDataType::Int4: return 4;

                case ShaderDataType::UInt: return 1;
                case ShaderDataType::UInt2: return 2;
                case ShaderDataType::UInt3: return 3;
                case ShaderDataType::UInt4: return 4;

                case ShaderDataType::Bool: return 1;
                case ShaderDataType::Bool2: return 2;
                case ShaderDataType::Bool3: return 3;
                case ShaderDataType::Bool4: return 4;

                case ShaderDataType::Sampler1D: return 1;
                case ShaderDataType::Sampler2D: return 1;
                case ShaderDataType::Sampler3D: return 1;
            }

            VXM_CORE_ERROR("Unknown ShaderDataType {0}.", (int)Type);
            return 0;
        }
    };

    class BufferLayout {
    public:
        inline BufferLayout() : m_Elements(), m_Stride(0) {}
        inline BufferLayout(const std::initializer_list<BufferElement>& elements) : m_Elements(elements), m_Stride(0) {
            CalculateOffsetAndStride();
        }

        inline std::vector<BufferElement>::const_iterator begin() const { return GetElements().begin();}
        inline std::vector<BufferElement>::const_iterator end() const { return GetElements().end();}

        inline const std::vector<BufferElement>& GetElements() const {return m_Elements;}
        inline uint32_t GetStride() const { return m_Stride; }

    private:
        inline void CalculateOffsetAndStride() {
            uint32_t offset = 0;
            m_Stride = 0;
            for (auto& element:m_Elements) {
                element.Offset = offset;
                offset += element.Size;
                m_Stride += element.Size;
            }
        }
    private:
        std::vector<BufferElement> m_Elements;
        uint32_t m_Stride;
    };

    class VertexBuffer {
    public:
        virtual ~VertexBuffer() {}

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void SetLayout(const BufferLayout& layout) = 0;
        virtual const BufferLayout& GetLayout() const = 0;

        static VertexBuffer* Create(uint32_t size, void* vertices);
    };

    class IndexBuffer {
    public:
        virtual ~IndexBuffer() {}

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual uint32_t GetCount() const = 0;

        static IndexBuffer* Create(uint32_t count, uint32_t* indices);
    };
}
