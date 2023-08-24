//
// Created by ianpo on 24/08/2023.
//

#pragma once

#include "Voxymore/Core/SmartPointers.hpp"
#include "Voxymore/Core/Math.hpp"
#include "Voxymore/Renderer/Material.hpp"
#include "Voxymore/Renderer/VertexArray.hpp"

namespace Voxymore::Core
{
	struct TransformComponent
	{
		glm::vec3 Position = glm::vec3(0.0f);
		glm::quat Rotation = glm::identity<glm::quat>();
		glm::vec3 Scale = glm::vec3(1.0f);

		inline TransformComponent() = default;
		inline TransformComponent(const TransformComponent&) = default;
		inline TransformComponent(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale) : Position(position), Rotation(rotation), Scale(scale) {}

		inline glm::mat4 GetTransform() const
		{
			// Not using Math::TRS because I don't have to pass reference this way so it should therefore be faster... I think... Need to be verified.
			VXM_PROFILE_FUNCTION();
			glm::mat4 trs(1.0f);
			trs = glm::translate(trs, Position); // Translation Matrix
			trs = trs * glm::toMat4(Rotation); // Rotation Matrix
			trs = glm::scale(trs, Scale); // Scale Matrix
			return trs; // Translation * Rotation * Scale => TRS Matrix.
		}
	};

	struct MeshComponent
	{
		Ref<Material> Mat;
		Ref<VertexArray> Mesh;

		inline MeshComponent() = default;
		inline MeshComponent(const MeshComponent&) = default;
		inline MeshComponent(const Ref<Material>& material,const Ref<VertexArray>& vertexArray) : Mat(material), Mesh(vertexArray) {}
	};
}