//
// Created by ianpo on 21/08/2023.
//

#pragma once

#include "Voxymore/Core/PerspectiveCameraController.hpp"
#include "Voxymore/Renderer/EditorCamera.hpp"
#include "Voxymore/Renderer/Framebuffer.hpp"
#include "Voxymore/Renderer/Model.hpp"
#include "Voxymore/Voxymore.hpp"

#include "Voxymore/Editor/Panels/PropertyPanel.hpp"
#include "Voxymore/Editor/Panels/SceneHierarchyPanel.hpp"
#include "Voxymore/Editor/Panels/ContentBrowserPanel.hpp"
#include "Voxymore/Editor/Panels/SystemPanel.hpp"
#include "Voxymore/Editor/Panels/Viewport.hpp"
#include "Voxymore/Editor/Panels/Panel.hpp"

#include "Voxymore/Editor/EditorHelper.hpp"

#include <map>

using namespace Voxymore::Core;

namespace Voxymore::Editor {

	class EditorLayer : public Layer {
	private:
		// TODO: Create a SubPanel system. (openable, closable, etc.)
		// Panels
		struct PanelMetadata {
			inline PanelMetadata() =default;
			inline ~PanelMetadata() =default;
			inline PanelMetadata(std::string _name, uint64_t _typeId, Core::Ref<BasePanel>(* _createPanelFunc)(void)) : name(_name), typeId(_typeId), createPanelFunc(_createPanelFunc) {}
			std::string name;
			uint64_t typeId;
			Core::Ref<BasePanel>(* createPanelFunc)(void);
		};
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnUpdate(TimeStep timeStep) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event);
		virtual void OnAttach() override;
		virtual void OnDetach() override;
	private:
		void RenderDockspace();
		void DrawGizmosWindow();
		void DrawImGuiToolbar();

		void RenderMenuBar();
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMousePressed(MouseButtonPressedEvent& e);

		void NewProject();
		void OpenProject();
		void OpenProject(const std::filesystem::path&);
		void SaveProject();
		void SaveProjectAs();
		void SaveProjectAs(const std::filesystem::path& path);

		void CreateNewScene();
		void SaveSceneAs();
		void SaveScene();
		void OpenScene();
		void OpenScene(const Path& path);
		void OpenScene(const std::filesystem::path& path);
	public:
		void OpenScene(AssetHandle id);
	private:
		void OnScenePlay();
		void OnSceneStop();
		void OnScenePause();
		void OnSceneResume();
		void ReloadAssets();
	private:
		std::map<uint64_t, PanelMetadata> m_PanelCreator;
		std::vector<Core::Ref<BasePanel>> m_Panels;
		std::vector<Core::Ref<Viewport>> m_Viewports;
		std::multimap<uint64_t, Core::Ref<BasePanel>> m_UnloadedPanels;
		std::vector<Core::Ref<Viewport>> m_UnloadedViewport;

		Ref<Scene> m_ActiveScene = nullptr;
		Ref<Scene> m_CacheScene = nullptr;
		std::filesystem::path m_FilePath;
		std::filesystem::path m_CacheFilePath;

		// Editor.
		SceneState m_SceneState = SceneState::Edit;
		Ref<Texture2D> m_PlayTexture;
		Ref<Texture2D> m_StopTexture;
		Ref<Texture2D> m_PauseTexture;
	private: // Gizmo
		GizmoOperation m_GizmoOperation = GizmoOperation::TRANSLATE;
		GizmoMode m_GizmoMode = GizmoMode::LOCAL;
		float m_GizmoTranslationSnapValue = 0.5f;
		float m_GizmoRotationSnapValue = 45.0f;
		float m_GizmoScaleSnapValue = 0.5f;
	private:
		Ref<Texture2D> m_Texture;
	private:
		UUID m_OnProjectReloadId;
	};

} // Voxymore
// Editor
