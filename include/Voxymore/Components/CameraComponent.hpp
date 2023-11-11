//
// Created by ianpo on 11/11/2023.
//

#pragma once

#include "Voxymore/Components/CustomComponent.hpp"
#include "Voxymore/Scene/SceneCamera.hpp"

namespace Voxymore::Core
{


	class CameraComponent
	{
	private:
		VXM_IMPLEMENT_COMPONENT(CameraComponent);
	public:
		SceneCamera Camera;
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

} // namespace Voxymore::Core

