//
// Created by ianpo on 25/08/2023.
//

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS 1
#endif

#include "Voxymore/Editor/Panels/PropertyPanel.hpp"
#include "imgui_internal.h"
#include <cstring>


namespace Voxymore::Editor {

	void PropertyPanel::OnImGuiRender() {
		VXM_PROFILE_FUNCTION();
		ImGui::Begin("Properties");
		if (m_SelectedEntity.IsValid()) {
			bool enable = m_SelectedEntity.IsActive();
			if (ImGui::Checkbox("##Enable", &enable)) {
				m_SelectedEntity.SetActive(enable);
			}

			ImGui::SameLine();

			if (m_SelectedEntity.HasComponent<TagComponent>()) {
				auto &tag = m_SelectedEntity.GetComponent<TagComponent>().Tag;
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
		ImGui::End();
	}

	void PropertyPanel::DrawComponents() {
		VXM_PROFILE_FUNCTION();
		DrawComponent<TransformComponent>("Transform Component", m_SelectedEntity,
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

//        DrawComponent<MeshComponent>("Mesh Component", m_SelectedEntity, [](MeshComponent& meshComponent){
//            ImGui::Text("Cannot modify the mesh component yet.");
//        });

		DrawComponent<CameraComponent>("Camera Component", m_SelectedEntity, [](CameraComponent &cameraComponent) {
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
			DrawComponent(m_SelectedEntity, cc);
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
			if (!m_SelectedEntity.HasComponent<CameraComponent>() && ImGui::MenuItem("Camera")) {
				auto &newCam = m_SelectedEntity.AddComponent<CameraComponent>();
				ImGui::CloseCurrentPopup();
			}

			//                if(!m_SelectedEntity.HasComponent<MeshComponent>() && ImGui::MenuItem("Mesh Component"))
			//                {
			//                    m_SelectedEntity.AddComponent<MeshComponent>();
			//                    ImGui::CloseCurrentPopup();
			//                }

			for (const ComponentChecker &cc: ComponentManager::GetComponents()) {
				if (!cc.HasComponent(m_SelectedEntity) && ImGui::MenuItem(cc.ComponentName.c_str())) {
					cc.AddComponent(m_SelectedEntity);
					ImGui::CloseCurrentPopup();
				}
			}

			ImGui::EndPopup();
		}
	}
} // Voxymore
// Editor