//
// Created by ianpo on 25/08/2023.
//

#pragma once

#include <Voxymore/Voxymore.hpp>
#include <Voxymore/Editor/Panels/PropertyPanel.hpp>
#include <imgui.h>

using namespace Voxymore::Core;

namespace Voxymore::Editor {

    class SceneHierarchyPanel
    {
    public:
        inline SceneHierarchyPanel() = default;
        SceneHierarchyPanel(const Ref<Scene>& scene);
        void SetContext(const Ref<Scene>& scene);

        void OnImGuiRender();

        inline Entity GetSelectedEntity() { return m_PropertyPanel.GetSelectedEntity(); }
        inline void SetSelectedEntity(Entity entity) {m_PropertyPanel.SetSelectedEntity(entity); }
    private:
        void DrawEntityNode(Entity entity);
		void DrawHierarchyOptions();
    private:
        Ref<Scene> m_Context;
//        Entity m_SelectionContext;
        PropertyPanel m_PropertyPanel;
    };
} // Voxymore
// Editor
