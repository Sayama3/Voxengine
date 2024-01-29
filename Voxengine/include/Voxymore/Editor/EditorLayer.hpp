//
// Created by ianpo on 21/08/2023.
//

#pragma once
#include "Voxymore/Voxymore.hpp"
#include "Voxymore/Renderer/Framebuffer.hpp"
#include "Voxymore/Renderer/Model.hpp"
#include "Voxymore/Renderer/EditorCamera.hpp"
#include "Voxymore/Core/PerspectiveCameraController.hpp"
#include "Voxymore/Editor/Panels/SceneHierarchyPanel.hpp"
#include "Voxymore/Editor/Panels/PropertyPanel.hpp"
#include "Voxymore/Editor/Panels/SystemPanel.hpp"

using namespace Voxymore::Core;

namespace Voxymore::Editor {

    // Fetched from ImGuizmo
    enum class GizmoOperation
    {
        NONE             = 0,
        TRANSLATE_X      = (1u << 0),
        TRANSLATE_Y      = (1u << 1),
        TRANSLATE_Z      = (1u << 2),
        ROTATE_X         = (1u << 3),
        ROTATE_Y         = (1u << 4),
        ROTATE_Z         = (1u << 5),
        ROTATE_SCREEN    = (1u << 6),
        SCALE_X          = (1u << 7),
        SCALE_Y          = (1u << 8),
        SCALE_Z          = (1u << 9),
        BOUNDS           = (1u << 10),
        SCALE_XU         = (1u << 11),
        SCALE_YU         = (1u << 12),
        SCALE_ZU         = (1u << 13),

        TRANSLATE = TRANSLATE_X | TRANSLATE_Y | TRANSLATE_Z,
        ROTATE = ROTATE_X | ROTATE_Y | ROTATE_Z | ROTATE_SCREEN,
        SCALE = SCALE_X | SCALE_Y | SCALE_Z,
        SCALEU = SCALE_XU | SCALE_YU | SCALE_ZU, // universal
        UNIVERSAL = TRANSLATE | ROTATE | SCALEU

    };
    inline GizmoOperation operator|(GizmoOperation lhs, GizmoOperation rhs)
    {
        return static_cast<GizmoOperation>(static_cast<int>(lhs) | static_cast<int>(rhs));
    }
    inline GizmoOperation operator&(GizmoOperation lhs, GizmoOperation rhs)
    {
        return static_cast<GizmoOperation>(static_cast<int>(lhs) & static_cast<int>(rhs));
    }
    enum class GizmoMode
    {
        LOCAL,
        WORLD
    };

    enum class SceneState
    {
        Edit = 0,
        Play = 1,
//		Pause = 2,
    };

    class EditorLayer : public Layer {
    private:
        Ref<Framebuffer> m_Framebuffer;
        Ref<Texture2D> m_Texture;
    private:
        std::array<glm::vec2, 2> m_ViewportBounds;
    private:
        SceneHierarchyPanel m_SceneHierarchyPanel;
        SystemPanel m_SystemPanel;
        Ref<Scene> m_ActiveScene;
        Ref<Scene> m_CacheScene = nullptr;
        Entity m_HoveredEntity;
        std::string m_FilePath;
        std::string m_CacheFilePath;

        // Editor.
        EditorCamera m_EditorCamera;
        SceneState m_SceneState = SceneState::Edit;
        Ref<Texture2D> m_PlayTexture;
        Ref<Texture2D> m_StopTexture;
        Ref<Texture2D> m_PauseTexture;
    private:
        glm::uvec2 m_ViewportSize = glm::uvec2(0);
        bool m_ViewportFocused;
        bool m_ViewportHovered;
        bool m_CameraEnable = false;
    private: // Gizmo
        GizmoOperation m_GizmoOperation = GizmoOperation::TRANSLATE;
        GizmoMode m_GizmoMode = GizmoMode::LOCAL;
        float m_GizmoTranslationSnapValue = 0.5f;
        float m_GizmoRotationSnapValue = 45.0f;
        float m_GizmoScaleSnapValue = 0.5f;
    private:
        UUID m_OnProjectReloadId;
    public:
        EditorLayer();

        virtual void OnUpdate(TimeStep timeStep) override;

        virtual void OnImGuiRender() override;

        virtual void OnEvent(Event& event);

        virtual void OnAttach() override;
        virtual void OnDetach() override;
    private:
        void DrawImGuiViewport();
        void RenderDockspace();
        void DrawGizmos();
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
        void OpenScene(UUID id);
        void OpenScene(const Path& path);
        void OpenScene(const std::filesystem::path& path);

        void OnScenePlay();
        void OnSceneStop();

        void ReloadAssets();
    };

} // Voxymore
// Editor
