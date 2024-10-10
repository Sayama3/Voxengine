//
// Created by Sayama on 10/10/2024.
//

#pragma once

#include "Voxymore/Renderer/Buffer.hpp"


namespace Voxymore::Core
{
	class OpenGLSSBO : public SSBO
	{
	public:
		OpenGLSSBO(uint64_t size, SSBO::Usage usage);
		OpenGLSSBO(Buffer data, SSBO::Usage usage);
		virtual ~OpenGLSSBO() override;
	public:
		virtual void Bind(uint32_t slot) override;
//		virtual void Bind() override {Bind(0u);}
//		virtual void Unbind() override;
	public:
		void SetData(Buffer data, int64_t offset);
	private:
		void Allocate(uint64_t size, SSBO::Usage usage);
	private:
		uint32_t m_RendererID{~0u};
	};
} // namespace Voxymore::Core

