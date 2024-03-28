//
// Created by ianpo on 17/02/2024.
//

#pragma once


namespace Voxymore::Editor
{

	class Panel
	{
	public:
		inline virtual ~Panel() {}

		virtual void OnImGuiRender() = 0;
		virtual void OnImGuizmo();
		virtual void Open();
		virtual void Close();
	protected:
		bool m_Open;
	};

} // namespace Voxymore::Editor

