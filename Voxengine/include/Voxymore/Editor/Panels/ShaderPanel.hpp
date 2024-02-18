//
// Created by ianpo on 17/02/2024.
//

#pragma once

#include "Voxymore/Voxymore.hpp"
#include "Voxymore/Editor/Panels/Panel.hpp"

namespace Voxymore::Editor
{

	class ShaderPanel : public Panel
	{
	public:
		virtual void OnImGuiRender() override;
	private:
		bool m_AutoReload = false;
	};

} // namespace Voxymore::Editor

