//
// Created by ianpo on 07/04/2024.
//

#include <Voxymore/Voxymore.hpp>
#include <ImGuizmo.h>
#include "Voxymore/Editor/Panels/Viewport.hpp"
#include "Voxymore/Editor/Panels/PropertyPanel.hpp"
#include "Voxymore/Editor/EditorLayer.hpp"
#include "Voxymore/Assets/Asset.hpp"

using namespace Voxymore::Core;

namespace Voxymore::Editor
{
	Viewport::Viewport(const Core::FramebufferSpecification& renderFbSpecs) : m_EditorCamera(60.0f, 1280.0f / 720.0f, 0.1f, 1000.0f)
	{
		VXM_PROFILE_FUNCTION();
		m_RenderFramebuffer = Framebuffer::Create(renderFbSpecs);
		m_ViewportSize = {renderFbSpecs.Width, renderFbSpecs.Height};
		m_DeferredFramebuffer = nullptr;
	}
	Viewport::Viewport(const Core::FramebufferSpecification& renderFbSpecs, const Core::FramebufferSpecification& deferredFbSpecs) {
		VXM_PROFILE_FUNCTION();
		m_RenderFramebuffer = Framebuffer::Create(renderFbSpecs);
		m_ViewportSize = {renderFbSpecs.Width, renderFbSpecs.Height};
		m_DeferredFramebuffer = Framebuffer::Create(deferredFbSpecs);
		m_DeferredFramebuffer->Resize(m_ViewportSize.x, m_ViewportSize.y);
	}

	Viewport::~Viewport()
	{

	}

	void Viewport::OnImGuiRender()
	{
		VXM_PROFILE_FUNCTION();
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0,0));
		{
			auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
			auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
			auto viewportOffset = ImGui::GetWindowPos();
			m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
			m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

			m_ViewportFocused = ImGui::IsWindowFocused();
			m_ViewportHovered = ImGui::IsWindowHovered();
			auto* imgui = Application::Get().FindLayer<ImGuiLayer>();
			if(imgui) {
				imgui->SetBlockEvents(!m_ViewportFocused && !m_ViewportHovered);
			}
			m_EditorCamera.SetViewportFocused(m_ViewportFocused);
			m_EditorCamera.SetViewportHovered(m_ViewportHovered);

			ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
			m_ViewportSize = glm::uvec2(static_cast<uint32_t>(viewportPanelSize.x), static_cast<uint32_t>(viewportPanelSize.y));

			uint64_t textureID = m_RenderFramebuffer->GetColorAttachmentRendererID();
			ImGui::Image(reinterpret_cast<void*>(textureID), viewportPanelSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

			if(ImGui::BeginDragDropTarget())
			{
				std::string payloadId = AssetTypeToPayloadID(AssetType::Scene);
				const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(payloadId.c_str());
				if (payload != nullptr) {
					VXM_CORE_ASSERT(payload->DataSize == sizeof(AssetHandle), "The ImGui payload size ({0}) is not that of AssetHandle ({1})", payload->DataSize, sizeof(AssetHandle));
					const AssetHandle* assetHandlePtr = static_cast<const AssetHandle *>(payload->Data);
					Application::Get().FindLayer<EditorLayer>()->OpenScene(*assetHandlePtr);
				}
				ImGui::EndDragDropTarget();
			}
		}
		ImGui::PopStyleVar();
	}

	void Viewport::PrepareFramebuffer()
	{
		VXM_PROFILE_FUNCTION();
		if (FramebufferSpecification spec = m_RenderFramebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_RenderFramebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			if (m_DeferredFramebuffer) m_DeferredFramebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
		}
	}

	void Viewport::DrawGizmos(const std::vector<Core::Ref<BasePanel>>& panels, const GizmoOperation gizmoOperation, const GizmoMode gizmoMode, float gizmoTranslationSnapValue, float gizmoRotationSnapValue, float gizmoScaleSnapValue)
	{
		VXM_PROFILE_FUNCTION();
		// Editor Camera
		const glm::mat4 projectionMatrix = m_EditorCamera.GetProjectionMatrix();
		const glm::mat4& viewMatrix = m_EditorCamera.GetViewMatrix();

		Entity selectedEntity = PropertyPanel::GetSelectedEntity();
		if(selectedEntity)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

			// Selected Entity
			auto& tc = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 modelMatrix = tc.GetTransform();

			bool isSnapping = (Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl)) && gizmoOperation != GizmoOperation::UNIVERSAL;

			float snapValue= gizmoOperation == GizmoOperation::TRANSLATE
									  ? gizmoTranslationSnapValue
									  : gizmoOperation == GizmoOperation::ROTATE
												? gizmoRotationSnapValue
												: gizmoScaleSnapValue;

			float snapValues[3] = {snapValue, snapValue, snapValue};
			bool manipulate = ImGuizmo::Manipulate(Math::ValuePtr(viewMatrix), Math::ValuePtr(projectionMatrix), static_cast<ImGuizmo::OPERATION>(gizmoOperation), static_cast<ImGuizmo::MODE>(gizmoMode), Math::ValuePtr(modelMatrix), nullptr, isSnapping ? snapValues : nullptr);
			m_EditorCamera.EnableMovement(!manipulate);
			if(ImGuizmo::IsUsing())
			{
				glm::vec3 scale;
				glm::vec3 position;
				glm::quat rotation;

				Math::DecomposeTransform(modelMatrix, position, rotation, scale);

				tc.SetPosition(position);
				tc.SetRotation(rotation);
				tc.SetScale(scale);
			}
		}

		for (const auto& panel : panels) {
			ImGuizmo::PushID((const void*)static_cast<uint64_t>(panel->GetHandle()));
			panel->OnImGuizmo(Math::ValuePtr(viewMatrix), Math::ValuePtr(projectionMatrix));
			ImGuizmo::PopID();
		}
	}

	void Viewport::BindRenderFramebuffer()
	{
		VXM_PROFILE_FUNCTION();
		m_RenderFramebuffer->Bind();
		RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
		RenderCommand::Clear();
		m_RenderFramebuffer->ClearColorAttachment(1, -1);
	}

	void Viewport::UnbindRenderFramebuffer()
	{
		VXM_PROFILE_FUNCTION();
		m_RenderFramebuffer->Unbind();
	}

	void Viewport::BindDeferredFramebuffer()
	{
		VXM_PROFILE_FUNCTION();
		m_DeferredFramebuffer->Bind();
		RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
		RenderCommand::Clear();
		m_DeferredFramebuffer->ClearColorAttachment(1, -1);
	}

	void Viewport::UnbindDeferredFramebuffer()
	{
		VXM_PROFILE_FUNCTION();
		m_DeferredFramebuffer->Unbind();
	}

	void BindDeferredFramebuffer();
	void UnbindDeferredFramebuffer();
	void Viewport::PostRender(Scene* scenePtr)
	{
		VXM_PROFILE_FUNCTION();
		auto [fMouseX, fMouseY] = ImGui::GetMousePos();
		fMouseX -= m_ViewportBounds[0].x;
		fMouseY -= m_ViewportBounds[0].y;

		auto viewportWidth = m_ViewportBounds[1].x - m_ViewportBounds[0].x;
		auto viewportHeight = m_ViewportBounds[1].y - m_ViewportBounds[0].y;

		//Reverse Y because the texture is reversed.
		fMouseY = viewportHeight - fMouseY;

		int mouseX = fMouseX;
		int mouseY = fMouseY;

		if(mouseX > 0 && mouseX < viewportWidth
			&& mouseY > 0 && mouseY < viewportHeight)
		{
			int value = m_RenderFramebuffer->ReadPixel(1, mouseX, mouseY);
			if(value < 0)
			{
				m_HoveredEntity = Entity();
			}
			else
			{
				auto entityId = static_cast<uint32_t>(value);
				m_HoveredEntity = Entity(static_cast<entt::entity>(entityId), scenePtr);
			}
			//TODO: pick hovered entity.
		}
	}

	void Viewport::SetFocusTarget(glm::vec3 focusTarget)
	{
		VXM_PROFILE_FUNCTION();
		m_EditorCamera.SetFocusTarget(focusTarget);
	}

	void Viewport::SetDistance(float distance)
	{
		VXM_PROFILE_FUNCTION();
		m_EditorCamera.SetDistance(distance);
	}

	void Viewport::OnEvent(Event &e)
	{
		VXM_PROFILE_FUNCTION();
		m_EditorCamera.OnEvent(e);
	}

	void Viewport::UpdateCamera(Core::TimeStep ts)
	{
		VXM_PROFILE_FUNCTION();
		if(m_ViewportFocused && m_ViewportHovered) m_EditorCamera.OnUpdate(ts);
	}
}// namespace Voxymore::Editor