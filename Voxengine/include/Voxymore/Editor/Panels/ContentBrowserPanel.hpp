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
		using ImGuiMenuFunc = std::function<bool(Voxymore::Core::AssetMetadata)>;
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
		ContentBrowserPanel();
		ContentBrowserPanel(Core::Path p);

		virtual void OnImGuiRender() override;
	private:
		void DrawRoot();
		void DrawContent();
		static ButtonClickState ImageButton(uint64_t id, float size);
	private:
		Core::Path m_Path = {};
		float m_ThumbnailSize;
		float m_Padding;


		using StringBuffer = std::array<char, 255>;
		StringBuffer m_FileNameBuffer;
		inline void ClearFileNameBuffer() {std::fill(m_FileNameBuffer.begin(), m_FileNameBuffer.end(), (char)0);}

	private:
		static inline constexpr float c_ThumbnailSizeMin = 16.0f;
		static inline constexpr float c_ThumbnailSizeMax = 512.0f;
		static inline float s_ThumbnailSize = 64.0f;
		static inline float s_Padding = 16.0f;
		static inline constexpr float c_Increment = 16.0f;

		static std::unordered_map<Core::AssetType, ImGuiMenuFunc> s_MenuFunc;

		static inline Core::Ref<Core::Texture2D> s_BackTexture = nullptr;
		static inline Core::Ref<Core::Texture2D> s_FolderTexture = nullptr;
		static inline Core::Ref<Core::Texture2D> s_EmptyFolderTexture = nullptr;
		static inline Core::Ref<Core::Texture2D> s_FullFolderTexture = nullptr;
		static inline Core::Ref<Core::Texture2D> s_FileTexture = nullptr;
	};
}