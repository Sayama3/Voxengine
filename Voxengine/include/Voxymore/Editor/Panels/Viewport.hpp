//
// Created by ianpo on 07/04/2024.
//

#pragma once

#include <Voxymore/Voxymore.hpp>
#include "Voxymore/Editor/EditorHelper.hpp"
#include "Voxymore/Editor/Panels/Panel.hpp"

namespace Voxymore::Editor
{

	class Viewport : public Panel<Viewport>
	{
	public:
		VXM_IMPLEMENT_PANEL("Viewport")
		enum class Type {
			Auto,
			Play,
			Edit,
		};
	public:
		Viewport(const Core::FramebufferSpecification& specs);
		~Viewport();
		virtual void OnImGuiRender() override;
	public:
		void PrepareFramebuffer();
		void UpdateCamera(Core::TimeStep ts);
		void DrawGizmos(const std::vector<Core::Ref<BasePanel>>& panels, const GizmoOperation gizmoOperation, const GizmoMode gizmoMode, float gizmoTranslationSnapValue, float gizmoRotationSnapValue, float gizmoScaleSnapValue);
		//void OnMousePressed(Core::MouseButtonPressedEvent& e);

		void BindFramebuffer();
		void UnbindFramebuffer();
		void PostRender(Core::Scene* currentScene);
		void OnEvent(Core::Event& e);

		inline bool HasHoveredEntity() const { return m_HoveredEntity.IsValid(); }
		inline Core::Entity GetHoveredEntity() { return m_HoveredEntity; }
		void SetFocusTarget(glm::vec3 focusTarget);
		void SetDistance(float distance);

		inline bool IsVisible() const { return m_Visible; }
		inline bool IsFocused() const { return m_ViewportFocused; }
		inline bool IsHovered() const { return m_ViewportHovered; }

		inline Core::EditorCamera& GetCamera() {return m_EditorCamera;}
	public:
		inline glm::uvec2 GetSize() const {return m_ViewportSize;}
	private:
		std::array<glm::vec2, 2> m_ViewportBounds;
		glm::uvec2 m_ViewportSize = glm::uvec2(0);
		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;
		bool m_Visible = true;
		Core::Ref<Core::Framebuffer> m_Framebuffer;
		Core::EditorCamera m_EditorCamera;
		Core::Entity m_HoveredEntity;

	};

} // namespace Voxymore::Editor

