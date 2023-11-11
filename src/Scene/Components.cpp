//
// Created by ianpo on 11/11/2023.
//

#include "Voxymore/Scene/Components.hpp"
#include "Voxymore/Scene/Scene.hpp"

namespace Voxymore::Core
{
//	VXM_CREATE_COMPONENT(IDComponent);
//	VXM_CREATE_COMPONENT(TagComponent);
//	VXM_CREATE_COMPONENT(TransformComponent);
//	VXM_CREATE_COMPONENT(NativeScriptComponent);

	VXM_CREATE_LIGHT_COMPONENT(CameraComponent);
	template<>
	void ::Voxymore::Core::Scene::OnComponentAdded<::Voxymore::Core::CameraComponent>(entt::entity entity, ::Voxymore::Core::CameraComponent &component)
	{
		if (!component.FixedAspectRatio) {
			component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
		}
	}

	void CameraComponent::DeserializeComponent(YAML::Node& cameraComponent, ::Voxymore::Core::Entity entity)
	{
		auto &cc = entity.GetComponent<CameraComponent>();
		cc.Primary = cameraComponent["Primary"].as<bool>();
		cc.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();

		auto camera = cameraComponent["Camera"];
		auto aspectRatio = camera["AspectRatio"].as<float>();
		cc.Camera.SetAspectRatio(aspectRatio);

		auto orthographicSize = camera["OrthographicSize"].as<float>();
		auto orthographicNear = camera["OrthographicNear"].as<float>();
		auto orthographicFar = camera["OrthographicFar"].as<float>();
		cc.Camera.SetOrthographic(orthographicSize, orthographicNear, orthographicFar);

		auto perspectiveVerticalFOV = camera["PerspectiveVerticalFOV"].as<float>();
		auto perspectiveNear = camera["PerspectiveNear"].as<float>();
		auto perspectiveFar = camera["PerspectiveFar"].as<float>();
		cc.Camera.SetPerspective(perspectiveVerticalFOV, perspectiveNear, perspectiveFar);

		cc.Camera.SwitchToOrthographic(camera["IsOrthographic"].as<bool>());
	}

	void CameraComponent::SerializeComponent(YAML::Emitter& out, ::Voxymore::Core::Entity entity)
	{
		auto &component = entity.GetComponent<CameraComponent>();
		out << KEYVAL("Primary", component.Primary);
		out << KEYVAL("FixedAspectRatio", component.FixedAspectRatio);

		out << KEY("Camera");
		out << YAML::BeginMap;// Camera
		{
			out << KEYVAL("IsOrthographic", component.Camera.IsOrthographic());
			out << KEYVAL("AspectRatio", component.Camera.GetAspectRatio());

			out << KEYVAL("OrthographicSize", component.Camera.GetOrthographicSize());
			out << KEYVAL("OrthographicNear", component.Camera.GetOrthographicNear());
			out << KEYVAL("OrthographicFar", component.Camera.GetOrthographicFar());

			out << KEYVAL("PerspectiveVerticalFOV", component.Camera.GetPerspectiveVerticalFOV());
			out << KEYVAL("PerspectiveNear", component.Camera.GetPerspectiveNear());
			out << KEYVAL("PerspectiveFar", component.Camera.GetPerspectiveFar());
			out << YAML::EndMap;// Camera
		}
		out << YAML::EndMap;// CameraComponent
	}

	bool CameraComponent::OnImGuiRender(::Voxymore::Core::Entity sourceEntity)
	{
		bool componentChanged = false;
		CameraComponent& cameraComponent = sourceEntity.GetComponent<CameraComponent>();

		componentChanged |= ImGui::Checkbox("Primary", &cameraComponent.Primary);
		componentChanged |= ImGui::Checkbox("Fixed Aspect Ratio", &cameraComponent.FixedAspectRatio);
		if(cameraComponent.FixedAspectRatio)
		{
			ImGui::SameLine();
			float ar = cameraComponent.Camera.GetAspectRatio();
			if(ImGui::DragFloat("##AspectRatio", &ar, 0.001f))
			{
				componentChanged |= true;

				cameraComponent.Camera.SetAspectRatio(ar);
			}
		}

		const char* projectionTypeStrings[] = {"Perspective", "Orthographic"};
		uint8_t isOrthographic = cameraComponent.Camera.IsOrthographic();
		const char* currentProjectionTypeString = projectionTypeStrings[isOrthographic];
		if(ImGui::BeginCombo("Projection",currentProjectionTypeString))
		{
			for (int i = 0; i < 2; ++i) {
				bool isSelected = isOrthographic == i;
				if(ImGui::Selectable(projectionTypeStrings[i], isSelected))
				{
					componentChanged |= true;
					cameraComponent.Camera.SwitchToOrthographic(i);
				}

				if(isSelected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}

		if (isOrthographic)
		{
			float size = cameraComponent.Camera.GetOrthographicSize();
			float orthoNearClip = cameraComponent.Camera.GetOrthographicNear();
			float orthoFarClip = cameraComponent.Camera.GetOrthographicFar();

			bool changed = false;
			changed |= ImGui::DragFloat("Size", &size, 0.1f, 0.1f);
			changed |= ImGui::DragFloat("Near", &orthoNearClip, 0.01f);
			changed |= ImGui::DragFloat("Far", &orthoFarClip, 0.01f);
			if(changed) cameraComponent.Camera.SetOrthographic(size, orthoNearClip, orthoFarClip);
			componentChanged |= changed;
		}
		else
		{
			float fov = glm::degrees(cameraComponent.Camera.GetPerspectiveVerticalFOV());
			float perspectiveNearClip = cameraComponent.Camera.GetPerspectiveNear();
			float perspectiveFarClip = cameraComponent.Camera.GetPerspectiveFar();

			bool changed = false;
			changed |= ImGui::DragFloat("Vertical Fov", &fov, 1.0f, 1.0f, 179.0f);
			changed |= ImGui::DragFloat("Near", &perspectiveNearClip, 0.01f);
			changed |= ImGui::DragFloat("Far", &perspectiveFarClip);
			if(changed) cameraComponent.Camera.SetPerspective(glm::radians(fov), perspectiveNearClip, perspectiveFarClip);
			componentChanged |= changed;
		}

		return componentChanged;
	}
} // Voxymore::Core