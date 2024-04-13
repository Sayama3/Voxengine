//
// Created by ianpo on 25/08/2023.
//

#pragma once

#include <Voxymore/Voxymore.hpp>
#include <imgui.h>
#include <imgui_internal.h>
#include "Panel.hpp"

using namespace Voxymore::Core;

namespace Voxymore::Editor {

	using ImGuiFunction = std::function<bool(Ref<Asset>)>;

	class PropertyPanel : public Panel<PropertyPanel>
	{
	private:
		static std::unordered_map<AssetType, ImGuiFunction> s_ImGuiAssetRenderer;
	private:
		static Entity s_SelectedEntity;
		static Ref<Asset> s_SelectedAsset;
		friend class SceneHierarchyPanel;

	public:
		VXM_IMPLEMENT_PANEL("Properties");
		inline static Entity GetSelectedEntity() { return s_SelectedEntity; }
		inline static Ref<Asset> GetSelectedAsset() { return s_SelectedAsset; }

		inline static bool HasSelectedEntity() { return s_SelectedEntity; }
		inline static bool HasSelectedAsset() { return s_SelectedAsset != nullptr; }

		inline static void SetSelectedEntity(Entity entity) { s_SelectedEntity = entity; s_SelectedAsset = nullptr;}
		inline static void SetSelectedAsset(Ref<Asset> asset) { s_SelectedEntity = {}; s_SelectedAsset = std::move(asset);}

		inline static void Reset() { s_SelectedEntity = {}; s_SelectedAsset = nullptr;}
	public:
		virtual void OnImGuiRender() override;
		virtual void OnImGuizmo(const float* viewMatrix, const float* projectionMatrix) override;
	private:
		bool RenderEntity();
		bool RenderAsset();
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
	private:
	};

} // Voxymore
// Editor
