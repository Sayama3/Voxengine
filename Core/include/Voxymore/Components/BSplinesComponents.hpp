//
// Created by ianpo on 13/04/2024.
//

#pragma once

#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Math/Math.hpp"
#include "Voxymore/Math/BoundingBox.hpp"
#include "Voxymore/Math/BoundingSphere.hpp"
#include "Voxymore/Components/CustomComponent.hpp"
#include "Voxymore/Renderer/Material.hpp"


namespace Voxymore::Core
{

	class BSplinesComponents : public SelfAwareComponent<BSplinesComponents>
	{
	public:
		VXM_IMPLEMENT_NAME(BSplinesComponents);
	public:
		BSplinesComponents() = default;
		~BSplinesComponents() = default;

		void DeserializeComponent(YAML::Node& node, Entity e);
		void SerializeComponent(YAML::Emitter& out, Entity e);
		bool OnImGuiRender(Entity e);
		bool OnImGuizmo(Entity e, const float* viewMatrix, const float* projectionMatrix);

		[[nodiscard]] bool IsValid() const;
		[[nodiscard]] std::vector<glm::vec3> GetWorldPoints(const Mat4& transform) const;
	public:
		MaterialField m_Material;
		int m_Degree = 1; // Must be superior or equal to one.
		int m_Definition = 1000;
		std::vector<glm::vec3> m_Points;
		std::vector<float> m_Nodes; // count must be m_Points.size() - m_Degree
		std::vector<float> m_Weight; // The number of points is equal to
		std::vector<glm::mat4> m_Matrices = {};
	};

	VXM_CREATE_COMPONENT(BSplinesComponents);

} // namespace Voxymore::Core

