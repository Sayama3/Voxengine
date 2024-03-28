//
// Created by ianpo on 25/08/2023.
//

#pragma once

#include <Voxymore/Voxymore.hpp>
#include <imgui.h>
#include <imgui_internal.h>

using namespace Voxymore::Core;

namespace Voxymore::Editor {

    class PropertyPanel
    {
private:
        Entity m_SelectedEntity;
        friend class SceneHierarchyPanel;
    public:
        void OnImGuiRender();
		void DrawGizmos(const float* viewMatrix, const float* projectionMatrix);
    private:
        template<typename T, typename UIFunction>
        inline static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction, bool canBeDeleted = true)
        {
            const ImGuiTreeNodeFlags treeNodeFlags =
                    ImGuiTreeNodeFlags_DefaultOpen |
                    ImGuiTreeNodeFlags_AllowItemOverlap |
                    ImGuiTreeNodeFlags_SpanAvailWidth |
                    ImGuiTreeNodeFlags_FramePadding |
                    ImGuiTreeNodeFlags_Framed;
            if (entity.HasComponent<T>())
            {
                auto& component = entity.GetComponent<T>();
                ImVec2 contentAvailable = ImGui::GetContentRegionAvail();
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
                float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
                ImGui::Separator();
                bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, "%s", name.c_str());
                ImGui::PopStyleVar();

                bool removeComponent = false;
                ImGui::SameLine(contentAvailable.x - lineHeight * 0.5f);
                if(ImGui::Button("+", ImVec2(lineHeight, lineHeight)))
                {
                    ImGui::OpenPopup("ComponentSettings");
                }
                if(ImGui::BeginPopup("ComponentSettings"))
                {
                    if(canBeDeleted && ImGui::MenuItem("Remove Component"))
                    {
                        removeComponent = true;
                    }
                    ImGui::EndPopup();
                }

                if(open)
                {
                    uiFunction(component);
                    ImGui::TreePop();
                }

                if(removeComponent)
                {
                    entity.RemoveComponent<T>();
                }
            }

        }

        static void DrawComponent(Entity entity, const ComponentChecker& component, bool canBeDeleted = true);

        void DrawComponents();
        void DrawAddComponent();

        inline Entity GetSelectedEntity() { return m_SelectedEntity; }
        inline void SetSelectedEntity(Entity entity) { m_SelectedEntity = entity; }

	private:
    };

} // Voxymore
// Editor
