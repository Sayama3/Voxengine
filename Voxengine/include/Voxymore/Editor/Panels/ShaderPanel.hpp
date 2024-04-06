//
// Created by ianpo on 17/02/2024.
//

#pragma once

#include "Voxymore/Editor/Panels/Panel.hpp"
#include "Voxymore/Voxymore.hpp"

namespace Voxymore::Editor
{

	class ShaderPanel : public Panel<ShaderPanel>
	{
	public:
		VXM_IMPLEMENT_PANEL("Shader Panel");
		virtual void OnImGuiRender(Core::UUID) override;
	private:
		bool m_AutoReload = false;
	};

} // namespace Voxymore::Editor

