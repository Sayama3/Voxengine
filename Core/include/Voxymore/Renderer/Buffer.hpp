//
// Created by ianpo on 20/07/2023.
//

#pragma once

#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Core/Logger.hpp"
#include "Voxymore/Core/Buffer.hpp"
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

	class RendererBuffer
	{
	public:
		RendererBuffer(const RendererBuffer&) = delete;
		RendererBuffer& operator=(const RendererBuffer&) = delete;

		RendererBuffer() = default;
		virtual ~RendererBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetRendererID() const = 0;
	};

    class VertexBuffer : public RendererBuffer
	{
    public:
        virtual ~VertexBuffer() {}

//        virtual void Bind() const = 0;
//        virtual void Unbind() const = 0;

        virtual void SetLayout(const BufferLayout& layout) = 0;
        virtual const BufferLayout& GetLayout() const = 0;

        static Ref<VertexBuffer> Create(uint32_t size, const void* vertices);
    };

    class IndexBuffer : public RendererBuffer
	{
    public:
        virtual ~IndexBuffer() {}

//        virtual void Bind() const = 0;
//        virtual void Unbind() const = 0;

        virtual uint32_t GetCount() const = 0;

        static Ref<IndexBuffer> Create(uint32_t count, const uint32_t* indices);
    };

	class SSBO : public RendererBuffer
	{
	public:
	/**
	 * The frequency of access may be one of these:
	 * STREAM 	- The data store contents will be modified once and used at most a few times.
	 * STATIC 	- The data store contents will be modified once and used many times.
	 * DYNAMIC	- The data store contents will be modified repeatedly and used many times.
	 *
	 * The nature of access may be one of these:
	 * DRAW 	- The data store contents are modified by the application, and used as the source for GL drawing and image specification commands.
	 * READ 	- The data store contents are modified by reading data from the GL, and used to return that data when queried by the application.
	 * COPY 	- The data store contents are modified by reading data from the GL, and used as the source for GL drawing and image specification commands.
	 * */
		enum Usage {
			StreamDraw,
			StreamRead,
			StreamCopy,

			StaticDraw,
			StaticRead,
			StaticCopy,

			DynamicDraw,
			DynamicRead,
			DynamicCopy,
		};
	public:
		virtual ~SSBO() {}

		virtual void Bind(uint32_t index) = 0;
		virtual void SetData(Buffer data, int64_t offset = 0) = 0;

		template<typename T>
		void SetData(const T* data, int64_t offset = 0);

		template<typename T>
		void SetArray(const T* data, uint64_t count, int64_t offset = 0);

		template<typename T>
		void SetVector(const std::vector<T>& data, int64_t offset = 0);

		template<typename Iter>
		void SetData(Iter begin, Iter end, int64_t offset = 0);

		template<typename T>
		void SetElementInArray(const T* element, int64_t index, int64_t rawOffset = 0);

	public:
		static Ref<SSBO> Create(uint64_t size, Usage usage = Usage::DynamicDraw);
		static Ref<SSBO> Create(Buffer data, Usage usage = Usage::DynamicDraw);
	};

	template<typename T>
	void SSBO::SetElementInArray(const T *element, int64_t index, int64_t rawOffset) {
		SetData(Buffer{const_cast<T*>(element), sizeof(T)}, rawOffset + index * sizeof(T));
	}

	template<typename T>
	void SSBO::SetVector(const std::vector<T> &data, int64_t offset) {
		SetData(Buffer{const_cast<T*>(data.data()), data.size() * sizeof(T)}, offset);
	}

	template<typename T>
	void SSBO::SetArray(const T *data, uint64_t count, int64_t offset) {
		SetData(Buffer{const_cast<T*>(data), sizeof(T) * count}, offset);
	}

	template<typename T>
	void SSBO::SetData(const T *data, int64_t offset) {
		SetData(Buffer{const_cast<T*>(data), sizeof(T)}, offset);
	}

	template<typename Iter>
	void SSBO::SetData(Iter begin, Iter end, int64_t offset)
	{
		uint64_t index = 0;
		for (auto it = begin; it != end; ++it)
		{
			SetElementInArray(&(*it), index++, offset);
		}
	}
}
