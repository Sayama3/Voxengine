//
// Created by ianpo on 18/07/2023.
//

#pragma once

#ifndef IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM 1
#endif

#include "Voxymore/Layers/Layer.hpp"
#include "Voxymore/Events/MouseEvent.hpp"
#include "Voxymore/Events/KeyEvent.hpp"
#include "Voxymore/Events/ApplicationEvent.hpp"

#ifndef IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM 1
#endif

#include "Voxymore/Core/FileSystem.hpp"
#include <imgui.h>

namespace Voxymore::Core {
	enum class FontType : uint8_t
	{
		Bold,
		BoldItalic,
		ExtraBold,
		ExtraBoldItalic,
		Italic,
		Light,
		LightItalic,
		Medium,
		MediumItalic,
		Regular,
		SemiBold,
		SemiBoldItalic,
		Count,
	};

    class ImGUILayer : public Layer {
    public:
        ImGUILayer();
        virtual ~ImGUILayer() override;

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnEvent(Event& e) override;
        void OnImGuiRender() override;

        void Begin();
        void End();

        inline void SetBlockEvents(bool blockEvents) {m_BlockEvents = blockEvents;}
        inline bool GetBlockEvents() const {return m_BlockEvents;}

		void AddFont(const std::string& path, float size, FontType fontType, bool isDefault = false);
		inline void AddFont(const Path& path, float size, FontType fontType, bool isDefault = false) { AddFont(path.GetFullPath().string(), size, fontType, isDefault); }
		ImFont* GetFont(FontType fontType);
		void PushFont(FontType fontType);
		void SetDarkThemeColors();
		void SetLightThemeColors();
		inline bool IsDarkTheme() const { return m_IsDarkTheme; }
    private:
		bool m_IsDarkTheme;
        bool m_BlockEvents = true;
        float m_Time = 0.0f;
		std::unordered_map<FontType, ImFont*> m_Fonts;
    };

} // Core
