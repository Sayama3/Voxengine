//
// Created by ianpo on 30/06/2024.
//

#pragma once

#include "Voxymore/Core/TimeStep.hpp"
#include "Voxymore/Math/Math.hpp"
#include "Voxymore/Math/Geometry.hpp"

#include <variant>

namespace Voxymore::Core
{
	class Renderer;

	struct Gizmos
	{
		friend class Renderer;
	public:
		enum Type {
			None,
			AABB,
			OBB,
			Plane,
			Sphere,
			Segment,
		};

		enum DrawMode {
			Fill,
			Hollow
		};

		static Vec4 constexpr DefaultColor = {0.15,0.25,0.90,1.0};

	public:
		Gizmos(Math::AABB aabb, uint64_t frameCount = 0u, Vec4 color = DefaultColor, DrawMode drawMode = Fill);
		Gizmos(Math::OBB obb, uint64_t frameCount = 0u, Vec4 color = DefaultColor, DrawMode drawMode = Fill);
		Gizmos(Math::Plane plane ,uint64_t frameCount = 0u, Vec4 color = DefaultColor, DrawMode drawMode = Fill);
		Gizmos(Math::Sphere sphere, uint64_t frameCount = 0u, Vec4 color = DefaultColor, DrawMode drawMode = Fill);
		Gizmos(Math::Segment segment, uint64_t frameCount = 0u, Vec4 color = DefaultColor, DrawMode drawMode = Fill);

		~Gizmos();
	public:

		void AddFrame(uint64_t count = 1);
		[[nodiscard]] bool ShouldBeActive() const;

		[[nodiscard]] Type GetType() const;
		[[nodiscard]] DrawMode GetDrawMode() const;
		[[nodiscard]] Vec4 GetColor() const;
		[[nodiscard]] uint64_t GetFrameCount() const;
		void SetFrameCount(uint64_t count);
		void SetDuration(Real time, Real framerate = Real(1.0/60.0));
		void SetDrawMode(DrawMode drawMode);
		void SetColor(Vec4 color);

		template<typename T>
		[[nodiscard]] const T* GetGizmo() const { return std::get_if<T>(&m_Gizmo); }
		template<typename T>
		[[nodiscard]] T* GetGizmo() { return std::get_if<T>(&m_Gizmo); }

		template<typename T>
		[[nodiscard]] bool HasGizmo() const { return std::holds_alternative<T>(m_Gizmo); }
		[[nodiscard]] bool HasGizmo(Type t) const;
		[[nodiscard]] bool IsCoherent() const;
	private:
		std::variant<Math::AABB, Math::OBB, Math::Plane, Math::Sphere, Math::Segment> m_Gizmo;
		Vec4 m_Color{DefaultColor};
		uint64_t m_FrameCount{0};
		uint64_t m_TimeActive{0};
		Type m_Type{None};
		DrawMode m_DrawMode{Fill};
	};

} // namespace Voxymore::Core

