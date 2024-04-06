//
// Created by ianpo on 25/08/2023.
//

#pragma once

#include <Voxymore/Voxymore.hpp>
#include <Voxymore/Editor/Panels/PropertyPanel.hpp>
#include <Voxymore/Editor/Panels/Panel.hpp>
#include <imgui.h>

using namespace Voxymore::Core;

namespace Voxymore::Editor {

    class SceneHierarchyPanel : public Panel<SceneHierarchyPanel>
    {
    public:
		VXM_IMPLEMENT_PANEL("Hierarchy");
        inline SceneHierarchyPanel() = default;
        static void SetContext(const Ref<Scene>& scene);

        virtual void OnImGuiRender(Core::UUID) override;
        virtual void OnImGuizmo(Core::UUID, const float* view, const float* projection) override;
    private:
        void DrawEntityNode(Entity entity);
		void DrawHierarchyOptions();
    private:
        static Ref<Scene> s_Context;
    };
} // Voxymore
// Editor
