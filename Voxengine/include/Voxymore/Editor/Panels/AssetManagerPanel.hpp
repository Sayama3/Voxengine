//
// Created by ianpo on 12/04/2024.
//

#pragma once

#include <Voxymore/Voxymore.hpp>
#include "Panel.hpp"

namespace Voxymore::Editor
{
	class AssetManagerPanel : public Panel<AssetManagerPanel>
	{
	private:
		enum ButtonClickState {
			STATE_NONE = 0,
			STATE_HOVERED,
			STATE_CLICKED,
			STATE_DOUBLE_CLICKED,
		};
	public:
		VXM_IMPLEMENT_PANEL("Content Browser")
	public:
		AssetManagerPanel();
		virtual void OnImGuiRender() override;
	private:
		static ButtonClickState ImageButton(uint64_t id, float size);
	private:
		float m_ThumbnailSize;
		float m_Padding;

		static inline float s_ThumbnailSize = 64.0f;
		static inline float s_Padding = 16.0f;
		static inline constexpr float c_Increment = 16.0f;

		static inline Core::Ref<Core::Texture2D> s_BackTexture = nullptr;
		static inline Core::Ref<Core::Texture2D> s_FolderTexture = nullptr;
		static inline Core::Ref<Core::Texture2D> s_EmptyFolderTexture = nullptr;
		static inline Core::Ref<Core::Texture2D> s_FullFolderTexture = nullptr;
		static inline Core::Ref<Core::Texture2D> s_FileTexture = nullptr;
	};
}// namespace Voxymore
