//
// Created by ianpo on 30/06/2024.
//

#include "Voxymore/Renderer/Gizmos.hpp"


namespace Voxymore::Core
{
	Gizmos::Gizmos(Math::AABB aabb, uint64_t frameCount, Vec4 color, DrawMode drawMode) : m_Gizmo(aabb), m_Type(AABB), m_Color(color), m_FrameCount(frameCount), m_DrawMode(drawMode) {}

	Gizmos::Gizmos(Math::OBB obb, uint64_t frameCount, Vec4 color, DrawMode drawMode) : m_Gizmo(obb), m_Type(OBB), m_Color(color), m_FrameCount(frameCount), m_DrawMode(drawMode) {}

	Gizmos::Gizmos(Math::Plane plane, uint64_t frameCount, Vec4 color, DrawMode drawMode) : m_Gizmo(plane), m_Type(Plane), m_Color(color), m_FrameCount(frameCount), m_DrawMode(drawMode) {}

	Gizmos::Gizmos(Math::Sphere sphere, uint64_t frameCount, Vec4 color, DrawMode drawMode) : m_Gizmo(sphere), m_Type(Sphere), m_Color(color), m_FrameCount(frameCount), m_DrawMode(drawMode) {}

	Gizmos::Gizmos(Math::Segment segment, uint64_t frameCount, Vec4 color, DrawMode drawMode) : m_Gizmo(segment), m_Type(Segment), m_Color(color), m_FrameCount(frameCount), m_DrawMode(drawMode) {}

	void Gizmos::AddFrame(uint64_t count)
	{
		m_TimeActive += count;
	}

	bool Gizmos::ShouldBeActive() const
	{
		return m_TimeActive > m_FrameCount;
	}

	Gizmos::Type Gizmos::GetType() const
	{
		return m_Type;
	}
	Gizmos::DrawMode Gizmos::GetDrawMode() const
	{
		return m_DrawMode;
	}
	Vec4 Gizmos::GetColor() const
	{
		return m_Color;
	}
	uint64_t Gizmos::GetFrameCount() const
	{
		return m_FrameCount;
	}
	void Gizmos::SetDrawMode(Gizmos::DrawMode drawMode)
	{
		m_DrawMode = drawMode;
	}
	void Gizmos::SetColor(Vec4 color)
	{
		m_Color = color;
	}
	bool Gizmos::HasGizmo(Gizmos::Type t) const
	{
		return m_Type == t;
	}

	bool Gizmos::IsCoherent() const
	{
		switch (m_Type) {
			case None: return false;
			case AABB: return HasGizmo<Math::AABB>();
			case OBB: return HasGizmo<Math::OBB>();
			case Plane: return HasGizmo<Math::Plane>();
			case Sphere: return HasGizmo<Math::Sphere>();
			case Segment: return HasGizmo<Math::Segment>();
		}
		return false;
	}

	void Gizmos::SetFrameCount(uint64_t count)
	{
		m_FrameCount = count;
	}

	void Gizmos::SetDuration(Real time, Real framerate)
	{
		if(framerate == 0) framerate = Real(1.0/60.0);
		if(time <= 0) time = 0;

		m_FrameCount = uint64_t( Math::Ceil(time / framerate) );
	}

	Gizmos::~Gizmos() = default;
}// namespace Voxymore::Core