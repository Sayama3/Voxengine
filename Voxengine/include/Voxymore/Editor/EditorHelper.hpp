//
// Created by ianpo on 07/04/2024.
//

#pragma once

namespace Voxymore::Editor
{
	// Fetched from ImGuizmo
	enum class GizmoOperation
	{
		NONE = 0,
		TRANSLATE_X = (1u << 0),
		TRANSLATE_Y = (1u << 1),
		TRANSLATE_Z = (1u << 2),
		ROTATE_X = (1u << 3),
		ROTATE_Y = (1u << 4),
		ROTATE_Z = (1u << 5),
		ROTATE_SCREEN = (1u << 6),
		SCALE_X = (1u << 7),
		SCALE_Y = (1u << 8),
		SCALE_Z = (1u << 9),
		BOUNDS = (1u << 10),
		SCALE_XU = (1u << 11),
		SCALE_YU = (1u << 12),
		SCALE_ZU = (1u << 13),

		TRANSLATE = TRANSLATE_X | TRANSLATE_Y | TRANSLATE_Z,
		ROTATE = ROTATE_X | ROTATE_Y | ROTATE_Z | ROTATE_SCREEN,
		SCALE = SCALE_X | SCALE_Y | SCALE_Z,
		SCALEU = SCALE_XU | SCALE_YU | SCALE_ZU,// universal
		UNIVERSAL = TRANSLATE | ROTATE | SCALEU

	};
	inline GizmoOperation operator|(GizmoOperation lhs, GizmoOperation rhs)
	{
		return static_cast<GizmoOperation>(static_cast<int>(lhs) | static_cast<int>(rhs));
	}
	inline GizmoOperation operator&(GizmoOperation lhs, GizmoOperation rhs)
	{
		return static_cast<GizmoOperation>(static_cast<int>(lhs) & static_cast<int>(rhs));
	}
	enum class GizmoMode
	{
		LOCAL,
		WORLD
	};

	enum class SceneState
	{
		Edit = 0,
		Play = 1,
		Pause = 2,
	};
}