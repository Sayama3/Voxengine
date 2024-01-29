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

        inline BufferElement(ShaderDataType type, const std::string& name, bool normalized = false) : Name(name), Type(type), Size(GetShaderDataTypeSize(type)), Offset(0), Normalized(normalized) {}

        inline uint32_t GetComponentCount() const
        {
			VXM_PROFILE_FUNCTION();
            return GetShaderDataTypeCount(Type);
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
			VXM_PROFILE_FUNCTION();
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

	class Buffer
	{
	public:
		virtual ~Buffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
	};

    class VertexBuffer : public Buffer {
    public:
        virtual ~VertexBuffer() {}

//        virtual void Bind() const = 0;
//        virtual void Unbind() const = 0;

        virtual void SetLayout(const BufferLayout& layout) = 0;
        virtual const BufferLayout& GetLayout() const = 0;

        static Ref<VertexBuffer> Create(uint32_t size, const void* vertices);
    };

    class IndexBuffer : public Buffer {
    public:
        virtual ~IndexBuffer() {}

//        virtual void Bind() const = 0;
//        virtual void Unbind() const = 0;

        virtual uint32_t GetCount() const = 0;

        static Ref<IndexBuffer> Create(uint32_t count, const uint32_t* indices);
    };
}
