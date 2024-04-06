//
// Created by ianpo on 25/08/2023.
//

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS 1
#endif

#include "Voxymore/Editor/Panels/PropertyPanel.hpp"
#include "imgui_internal.h"
#include "ImGuizmo.h"
#include <cstring>


namespace Voxymore::Editor {
	Entity PropertyPanel::s_SelectedEntity = Entity();

	void PropertyPanel::OnImGuiRender(UUID id) {
		VXM_PROFILE_FUNCTION();
		if (s_SelectedEntity.IsValid()) {
			bool enable = s_SelectedEntity.IsActive();
			if (ImGui::Checkbox("##Enable", &enable)) {
				s_SelectedEntity.SetActive(enable);
			}

			ImGui::SameLine();

			if (s_SelectedEntity.HasComponent<TagComponent>()) {
				auto &tag = s_SelectedEntity.GetComponent<TagComponent>().Tag;
				const size_t bufferSize = 256;
				char buffer[bufferSize];
				buffer[bufferSize - 1] = 0;
				std::strncpy(buffer, tag.c_str(), sizeof(buffer) - 1);
				if (ImGui::InputText("##Tag", buffer, bufferSize)) {
					tag = std::string(buffer);
				}
			}

			ImGui::SameLine();

			DrawAddComponent();

			DrawComponents();
		} else {
			ImGui::Text("No entity selected.");
		}
	}

	void PropertyPanel::DrawComponents() {
		VXM_PROFILE_FUNCTION();
		DrawComponent<TransformComponent>("Transform Component", s_SelectedEntity,
										  [](TransformComponent &transformComponent) {
											  glm::vec3 pos = transformComponent.GetPosition();
											  if (ImGuiLib::DrawVec3Control("Position", pos)) {
												  transformComponent.SetPosition(pos);
											  }

											  glm::vec3 rot = transformComponent.GetEulerRotation();
											  if (ImGuiLib::DrawVec3Control("Rotation", rot)) {
												  transformComponent.SetEulerRotation(rot);
											  }

											  glm::vec3 scale = transformComponent.GetScale();
											  if (ImGuiLib::DrawVec3Control("Scale", scale, 1.0f)) {
												  transformComponent.SetScale(scale);
											  }
										  }, false);

//        DrawComponent<MeshComponent>("Mesh Component", s_SelectedEntity, [](MeshComponent& meshComponent){
//            ImGui::Text("Cannot modify the mesh component yet.");
//        });

		DrawComponent<CameraComponent>("Camera Component", s_SelectedEntity, [](CameraComponent &cameraComponent) {
			ImGui::Checkbox("Primary", &cameraComponent.Primary);
			ImGui::Checkbox("Fixed Aspect Ratio", &cameraComponent.FixedAspectRatio);
			if (cameraComponent.FixedAspectRatio) {
				ImGui::SameLine();
				float ar = cameraComponent.Camera.GetAspectRatio();
				if (ImGui::DragFloat("##AspectRatio", &ar, 0.001f)) {
					cameraComponent.Camera.SetAspectRatio(ar);
				}
			}

			const char *projectionTypeStrings[] = {"Perspective", "Orthographic"};
			uint8_t isOrthographic = cameraComponent.Camera.IsOrthographic();
			const char *currentProjectionTypeString = projectionTypeStrings[isOrthographic];
			if (ImGui::BeginCombo("Projection", currentProjectionTypeString)) {
				for (int i = 0; i < 2; ++i) {
					bool isSelected = isOrthographic == i;
					if (ImGui::Selectable(projectionTypeStrings[i], isSelected)) {
						cameraComponent.Camera.SwitchToOrthographic(i);
					}

					if (isSelected) {
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}

			if (isOrthographic) {
				float size = cameraComponent.Camera.GetOrthographicSize();
				float orthoNearClip = cameraComponent.Camera.GetOrthographicNear();
				float orthoFarClip = cameraComponent.Camera.GetOrthographicFar();

				bool changed = false;
				changed |= ImGui::DragFloat("Size", &size, 0.1f, 0.1f);
				changed |= ImGui::DragFloat("Near", &orthoNearClip, 0.01f);
				changed |= ImGui::DragFloat("Far", &orthoFarClip, 0.01f);
				if (changed) cameraComponent.Camera.SetOrthographic(size, orthoNearClip, orthoFarClip);
			} else {
				float fov = glm::degrees(cameraComponent.Camera.GetPerspectiveVerticalFOV());
				float perspectiveNearClip = cameraComponent.Camera.GetPerspectiveNear();
				float perspectiveFarClip = cameraComponent.Camera.GetPerspectiveFar();

				bool changed = false;
				changed |= ImGui::DragFloat("Vertical Fov", &fov, 1.0f, 1.0f, 179.0f);
				changed |= ImGui::DragFloat("Near", &perspectiveNearClip, 0.01f);
				changed |= ImGui::DragFloat("Far", &perspectiveFarClip);
				if (changed)
					cameraComponent.Camera.SetPerspective(glm::radians(fov), perspectiveNearClip, perspectiveFarClip);
			}
		});

		for (const ComponentChecker &cc: ComponentManager::GetComponents()) {
			DrawComponent(s_SelectedEntity, cc);
		}
	}

	void PropertyPanel::DrawComponent(Entity entity, const ComponentChecker &component, bool canBeDeleted) {
		VXM_PROFILE_FUNCTION();
		const ImGuiTreeNodeFlags treeNodeFlags =
				ImGuiTreeNodeFlags_DefaultOpen |
				ImGuiTreeNodeFlags_AllowItemOverlap |
				ImGuiTreeNodeFlags_SpanAvailWidth |
				ImGuiTreeNodeFlags_FramePadding |
				ImGuiTreeNodeFlags_Framed;
		if (component.HasComponent(entity)) {
			ImVec2 contentAvailable = ImGui::GetContentRegionAvail();
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void *) component.ComponentHash, treeNodeFlags, "%s", component.ComponentName.c_str());
			ImGui::PopStyleVar();

			bool removeComponent = false;
			ImGui::SameLine(contentAvailable.x - lineHeight * 0.5f);
			if (ImGui::Button("-", ImVec2(lineHeight, lineHeight))) {
				ImGui::OpenPopup("ComponentSettings");
			}
			if (ImGui::BeginPopup("ComponentSettings")) {
				if (canBeDeleted && ImGui::MenuItem("Remove Component")) {
					removeComponent = true;
				}
				ImGui::EndPopup();
			}

			if (open) {
				std::string id = component.ComponentName + std::to_string(entity.id());
				ImGui::PushID(id.c_str());
				component.OnImGuiRender(entity);
				ImGui::PopID();
				ImGui::TreePop();
			}

			if (removeComponent) {
				component.RemoveComponent(entity);
			}
		}

	}

	void PropertyPanel::DrawAddComponent() {
		VXM_PROFILE_FUNCTION();
		ImGui::PushItemWidth(-1);

		ImVec2 available = ImGui::GetContentRegionAvail();
		if (ImGui::Button("Add Component", ImVec2(available.x, 24))) {
			ImGui::OpenPopup("AddComponentPopup");
		}

		ImGui::PopItemWidth();
		if (ImGui::BeginPopup("AddComponentPopup")) {
			if (!s_SelectedEntity.HasComponent<CameraComponent>() && ImGui::MenuItem("Camera")) {
				auto &newCam = s_SelectedEntity.AddComponent<CameraComponent>();
				ImGui::CloseCurrentPopup();
			}

			//                if(!s_SelectedEntity.HasComponent<MeshComponent>() && ImGui::MenuItem("Mesh Component"))
			//                {
			//                    s_SelectedEntity.AddComponent<MeshComponent>();
			//                    ImGui::CloseCurrentPopup();
			//                }

			for (const ComponentChecker &cc: ComponentManager::GetComponents()) {
				if (!cc.HasComponent(s_SelectedEntity) && ImGui::MenuItem(cc.ComponentName.c_str())) {
					cc.AddComponent(s_SelectedEntity);
					ImGui::CloseCurrentPopup();
				}
			}

			ImGui::EndPopup();
		}
	}

	void PropertyPanel::OnImGuizmo(UUID id, const float *viewMatrix, const float *projectionMatrix)
	{
		VXM_PROFILE_FUNCTION();
		if (s_SelectedEntity.IsValid()) {
			ImGuizmo::PushID(s_SelectedEntity);
			for (const ComponentChecker &cc: ComponentManager::GetComponents()) {
				if(cc.OnImGuizmo && cc.HasComponent(s_SelectedEntity))
				{
					ImGuizmo::PushID((const void*)cc.ComponentHash);
					cc.OnImGuizmo(s_SelectedEntity, viewMatrix, projectionMatrix);
					ImGuizmo::PopID();
				}
			}
			ImGuizmo::PopID();
		}
	}
} // Voxymore
// Editor