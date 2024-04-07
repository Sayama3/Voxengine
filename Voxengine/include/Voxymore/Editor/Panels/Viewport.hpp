//
// Created by ianpo on 07/04/2024.
//

#pragma once

#include <Voxymore/Voxymore.hpp>
#include "Panel.hpp"

namespace Voxymore::Editor
{

	class Viewport : public Panel<Viewport>
	{
	public:
		VXM_IMPLEMENT_PANEL("Content Browser")
		enum class Type {
			Auto,
			Play,
			Edit,
		};
	public:
		Viewport(const Core::FramebufferSpecification& specs);
		~Viewport();
		virtual void OnImGuiRender() override;
	public:
		void PrepareFramebuffer();
		void UpdateCamera(Core::TimeStep ts);
	private:
		std::array<glm::vec2, 2> m_ViewportBounds;
		glm::uvec2 m_ViewportSize = glm::uvec2(0);
		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;
		Core::Ref<Core::Framebuffer> m_Framebuffer;
		Core::EditorCamera m_EditorCamera;

	};

} // namespace Voxymore::Editor

