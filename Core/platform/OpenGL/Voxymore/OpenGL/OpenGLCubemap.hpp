//
// Created by ianpo on 12/05/2024.
//

#pragma once

#include "Voxymore/Renderer/Cubemap.hpp"

namespace Voxymore::Core
{

	class OpenGLCubemap : public Cubemap
	{
	public:
		OpenGLCubemap(const CubemapSpecification& spec);
		virtual ~OpenGLCubemap() override;

		virtual void Bind() override;
		static void Unbind();
		[[nodiscard]] virtual uint32_t GetRendererID() const override;

		virtual void SetData(Side side, const Image& image) override;
		virtual void SetData(const std::array<Image, 6>& images) override;
	private:
		CubemapSpecification m_Specification;
		uint32_t m_RendererID = 0;
	};

} // namespace Voxymore::Core

