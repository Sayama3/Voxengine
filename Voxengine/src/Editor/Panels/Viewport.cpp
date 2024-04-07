//
// Created by ianpo on 07/04/2024.
//

#include "Voxymore/Editor/Panels/Viewport.hpp"

using namespace Voxymore::Core;

namespace Voxymore::Editor
{
	Viewport::Viewport(const Core::FramebufferSpecification& specs)
	{
		m_Framebuffer = Framebuffer::Create(specs);
		m_ViewportSize = {specs.Width, specs.Height};
	}
	Viewport::~Viewport()
	{

	}
	void Viewport::OnImGuiRender()
	{

	}

	void Viewport::PrepareFramebuffer()
	{
		if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
		}
	}
}// namespace Voxymore::Editor