//
// Created by ianpo on 20/07/2023.
//

#pragma once

#include "Voxymore/Renderer/Camera.hpp"
#include "Voxymore/Core.hpp"
#include "Voxymore/Math.hpp"

namespace Voxymore {
    namespace Core {

        class PerspectiveCamera : public Camera {
        public:
			PerspectiveCamera();
			virtual ~PerspectiveCamera() override;

            virtual glm::mat4 GetViewProjectionMatrix() const override;
            virtual glm::mat4 GetProjectionMatrix() const override;
            virtual glm::mat4 GetViewMatrix() const override;

			// View Matrix Getter/Setter
			inline void SetPosition(const glm::vec3& position) { m_Position = position; UpdateViewMatrix(); }
			inline void SetRotation(const glm::quat& rotation) { m_Rotation = rotation; UpdateViewMatrix(); }
			inline void SetScale(const glm::vec3& scale) { m_Scale = scale; UpdateViewMatrix(); }

			inline glm::vec3 GetPosition() const { return m_Position; }
			inline glm::quat GetRotation() const { return m_Rotation; }
			inline glm::vec3 GetScale() const { return m_Scale; }

			// Projection Matrix Getter / Setter
			inline void SetFOV(float fov) { m_FOV = fov; UpdateProjectionMatrix(); }
			inline void SetNearClip(float nearClip) { m_NearClip = nearClip; UpdateProjectionMatrix(); }
			inline void SetFarClip(float farClip) { m_FarClip = farClip; UpdateProjectionMatrix(); }
			inline void SetWidth(float width) { m_Width = width; UpdateProjectionMatrix(); }
			inline void SetHeight(float height) { m_Height = height; UpdateProjectionMatrix(); }

			inline float GetFOV() const { return m_FOV; }
			inline float GetNearClip() const { return m_NearClip; }
			inline float GetFarClip() const { return m_FarClip; }
			inline float GetWidth() const { return m_Width; }
			inline float GetHeight() const { return m_Height; }

		private:
			void UpdateViewMatrix();
			void UpdateProjectionMatrix();
			void UpdateViewProjectionMatrix();

        private:
			glm::mat4 m_ViewProjectionMatrix;
			glm::mat4 m_ProjectionMatrix;
			glm::mat4 m_ViewMatrix;
            // Transform components
            glm::vec3 m_Position = glm::zero<glm::vec3>();
            glm::quat m_Rotation = glm::identity<glm::quat>();
            glm::vec3 m_Scale = glm::one<glm::vec3>();
            // Perspective PerspectiveCamera components
            // In degrees.
            float m_FOV = 65.0f;
            float m_NearClip = 0.01f;
            float m_FarClip = 1000.0f;
            float m_Width = 800;
            float m_Height = 600;
        };

    } // Voxymore
} // Core
