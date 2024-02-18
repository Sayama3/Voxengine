//
// Created by ianpo on 17/02/2024.
//

#include "Voxymore/Editor/Panels/Panel.hpp"


namespace Voxymore::Editor
{
	void Panel::Open()
	{
		m_Open = true;
	}

	void Panel::Close()
	{
		m_Open = false;
	}
}