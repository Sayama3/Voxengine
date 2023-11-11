//
// Created by ianpo on 24/08/2023.
//

#pragma once

#include "CustomComponent.hpp"
#include "Voxymore/Core/Math.hpp"
#include "Voxymore/Core/SmartPointers.hpp"
#include "Voxymore/Core/UUID.hpp"
#include "Voxymore/Renderer/Material.hpp"
#include "Voxymore/Renderer/VertexArray.hpp"
#include "Voxymore/Scene/ScriptableEntity.hpp"

namespace Voxymore::Core
{
	struct IDComponent
	{
		public:
		UUID ID;

		inline IDComponent() = default;
		inline IDComponent(const IDComponent&) = default;
		inline IDComponent(UUID id) : ID(id) {}

		inline operator UUID() const { return ID; }
	};

	struct TagComponent
	{
		public:
		std::string Tag;

		inline TagComponent() = default;
		inline TagComponent(const TagComponent&) = default;
		inline TagComponent(const std::string& tag) : Tag(tag) {}
	};

	struct TransformComponent
	{
		public:
	private:
		glm::vec3 Position = glm::vec3(0.0f);
		glm::quat Rotation = glm::identity<glm::quat>();
		glm::vec3 EulerRotation = glm::vec3(0.0f);
		glm::vec3 Scale = glm::vec3(1.0f);
	public:

		inline TransformComponent() = default;
		inline TransformComponent(const TransformComponent&) = default;
		inline TransformComponent(const glm::vec3& position, const glm::quat& rotation = glm::identity<glm::quat>(), const glm::vec3& scale = glm::vec3(1.0f)) : Position(position), Rotation(rotation), EulerRotation(glm::eulerAngles(rotation)), Scale(scale) {}
	public:
		inline glm::vec3 GetPosition() const { return Position; }
		inline void SetPosition(const glm::vec3& position) { Position = position; }

		inline glm::vec3 GetScale() const { return Scale; }
		inline void SetScale(const glm::vec3& scale) { Scale = scale; }

		inline glm::quat GetRotation() const { return Rotation; }
		inline void SetRotation(const glm::quat& rotation) {Rotation = rotation; EulerRotation = glm::eulerAngles(rotation);}

		inline glm::vec3 GetEulerRotation() const { return EulerRotation; }
		inline void SetEulerRotation(const glm::vec3& rotation) {EulerRotation = rotation; Rotation = glm::quat(rotation); }
	public:
		inline glm::mat4 GetTransform() const
		{
			// Not using Math::TRS because I don't have to pass reference this way, so it should therefore be faster... I think... Need to be verified.
			VXM_PROFILE_FUNCTION();
			glm::mat4 trs(1.0f);
			trs = glm::translate(trs, Position); // Translation Matrix
			trs = trs * glm::toMat4(Rotation); // Rotation Matrix
			trs = glm::scale(trs, Scale); // Scale Matrix
			return trs; // Translation * Rotation * Scale => TRS Matrix.
		}
	};

//	struct MeshComponent
//	{
//		private:
//		VXM_IMPLEMENT_COMPONENT(::Voxymore::Core::MeshComponent);
//		public:
//		Ref<Voxymore::Core::Material> Material;
//		Ref<VertexArray> Mesh;
//
//		inline MeshComponent() = default;
//		inline MeshComponent(const MeshComponent&) = default;
//		inline MeshComponent(const Ref<Voxymore::Core::Material>& material,const Ref<VertexArray>& vertexArray) : Material(material), Mesh(vertexArray) {}
//	};

	struct NativeScriptComponent
	{
	public:
		friend class Scene;
	private:
		ScriptableEntity* Instance = nullptr;

		ScriptableEntity*(*InstantiateScript)();
		void(*DestroyScript)(NativeScriptComponent*);

		inline bool IsValid() { return Instance != nullptr; }

		inline void CreateInstance() { Instance = InstantiateScript(); }
		inline void DestroyInstance() { DestroyScript(this); }
	public:
		template<typename T>
		inline void Bind()
		{
			VXM_CORE_ASSERT(Instance != nullptr, "Instance already exist. Binding will cause memory leaks.");

			InstantiateScript = []() { return static_cast<ScriptableEntity>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete (T*)nsc->Instance; nsc->Instance = nullptr; };
		}
	};

}