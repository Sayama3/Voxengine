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
		explicit OpenGLSSBO(Buffer data, SSBO::Usage usage);
		virtual ~OpenGLSSBO() override;
		OpenGLSSBO(const OpenGLSSBO&) = delete;
		OpenGLSSBO& operator=(const OpenGLSSBO&) = delete;
	public:
		virtual void Bind(uint32_t slot) override;
		virtual uint32_t GetRendererID() const override {return m_RendererID;};
	public:
		virtual void SetData(Buffer data, int64_t offset) override;
	private:
		void Allocate(uint64_t size, SSBO::Usage usage);
	private:
		uint32_t m_RendererID{~0u};
	};
} // namespace Voxymore::Core

