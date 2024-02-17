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
#include "Voxymore/Scene/SceneCamera.hpp"

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
		Vec3 Position = Vec3(0.0f);
		Quat Rotation = Math::Identity<Quat>();
		Vec3 EulerRotation = Vec3(0.0f);
		Vec3 Scale = Vec3(1.0f);
	public:

		inline TransformComponent() = default;
		inline TransformComponent(const TransformComponent&) = default;
		inline TransformComponent(const Vec3& position, const Quat& rotation = Math::Identity<Quat>(), const Vec3& scale = Vec3(1.0f)) : Position(position), Rotation(rotation), EulerRotation(glm::eulerAngles(rotation)), Scale(scale) {}
	public:
		inline Vec3 GetPosition() const { return Position; }
		inline void SetPosition(const Vec3& position) { Position = position; }
		inline void AddMovement(const Vec3& movement) { Position += movement; }

		inline Vec3 GetScale() const { return Scale; }
		inline void SetScale(const Vec3& scale) { Scale = scale; }

		inline Quat GetRotation() const { return Rotation; }
		inline void SetRotation(const Quat& rotation) {Rotation = rotation; EulerRotation = glm::eulerAngles(rotation);}

		inline Vec3 GetEulerRotation() const { return EulerRotation; }
		inline void SetEulerRotation(const Vec3& rotation) {EulerRotation = rotation; Rotation = Quat(rotation); }

		inline Vec3 GetForward() const { return Rotation * Vec3{0,0,1}; }
		inline Vec3 GetRight() const { return Rotation * Vec3{1,0,0}; }
		inline Vec3 GetUp() const { return Rotation * Vec3{0,1,0}; }

	public:
		inline Mat4 GetTransform() const
		{
			VXM_PROFILE_FUNCTION();
			return Math::TRS(Position, Rotation, Scale); // Translation * Rotation * Scale => TRS Matrix.
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
			VXM_PROFILE_FUNCTION();
			VXM_CORE_ASSERT(Instance != nullptr, "Instance already exist. Binding will cause memory leaks.");

			InstantiateScript = []() { return static_cast<ScriptableEntity>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete (T*)nsc->Instance; nsc->Instance = nullptr; };
		}
	};

	struct CameraComponent
	{
		Voxymore::Core::SceneCamera Camera;
		// TODO: Moving primary camera logic on Scene.
		bool Primary = true;
		bool FixedAspectRatio = false;

		inline CameraComponent() = default;
		inline CameraComponent(const CameraComponent&) = default;

		/**
		 * Orthographic Camera Constructor.
		 * @param width
		 * @param height
		 * @param size
		 * @param nearClip
		 * @param farClip
		 */
		inline CameraComponent(uint32_t width, uint32_t height, float size, float nearClip, float farClip) : Camera(width, height, size, nearClip, farClip) {}
		/**
		 * Perspective Camera Constructor.
		 * @param fov
		 * @param nearClip
		 * @param farClip
		 * @param width
		 * @param height
		 */
		inline CameraComponent(float fov, float nearClip, float farClip, uint32_t width, uint32_t height) : Camera(fov, nearClip, farClip, width, height) {}
		inline CameraComponent(const Voxymore::Core::SceneCamera& camera) : Camera(camera) {}

	};

	struct DisableComponent {};
}