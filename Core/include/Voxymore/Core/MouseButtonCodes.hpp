//
// Created by ianpo on 19/07/2023.
//

#pragma once

// Imported from glfw3.h
namespace Voxymore::Core {
	enum class Mouse : int
	{
		Button1 = 0,
		Button2 = 1,
		Button3 = 2,
		Button4 = 3,
		Button5 = 4,
		Button6 = 5,
		Button7 = 6,
		Button8 = 7,
		Last = Button8,
		Left = Button1,
		Right = Button2,
		Middle = Button3,
	};
}

#define VXM_MOUSE_BUTTON_1         ::Voxymore::Core::Mouse::Button1
#define VXM_MOUSE_BUTTON_2         ::Voxymore::Core::Mouse::Button2
#define VXM_MOUSE_BUTTON_3         ::Voxymore::Core::Mouse::Button3
#define VXM_MOUSE_BUTTON_4         ::Voxymore::Core::Mouse::Button4
#define VXM_MOUSE_BUTTON_5         ::Voxymore::Core::Mouse::Button5
#define VXM_MOUSE_BUTTON_6         ::Voxymore::Core::Mouse::Button6
#define VXM_MOUSE_BUTTON_7         ::Voxymore::Core::Mouse::Button7
#define VXM_MOUSE_BUTTON_8         ::Voxymore::Core::Mouse::Button8
#define VXM_MOUSE_BUTTON_LAST      ::Voxymore::Core::Mouse::Last
#define VXM_MOUSE_BUTTON_LEFT      ::Voxymore::Core::Mouse::Left
#define VXM_MOUSE_BUTTON_RIGHT     ::Voxymore::Core::Mouse::Right
#define VXM_MOUSE_BUTTON_MIDDLE    ::Voxymore::Core::Mouse::Middle