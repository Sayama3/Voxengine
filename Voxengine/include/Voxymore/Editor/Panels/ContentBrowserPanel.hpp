//
// Created by ianpo on 07/04/2024.
//

#pragma once

#include <Voxymore/Voxymore.hpp>
#include "Panel.hpp"

namespace Voxymore::Editor
{
	class ContentBrowserPanel : public Panel<ContentBrowserPanel>
	{
		VXM_IMPLEMENT_PANEL("Content Browser")
	public:
		inline ContentBrowserPanel() = default;
		inline ContentBrowserPanel(Core::Path p) : m_Path(p) {}

		virtual void OnImGuiRender() override;
	private:

		Core::Path m_Path = {};
	};
}