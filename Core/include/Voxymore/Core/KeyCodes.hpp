//
// Created by ianpo on 19/07/2023.
//

#pragma once

// Imported from VXM3.h

/* The unknown key */
#define VXM_KEY_UNKNOWN            -1

/* Printable keys */
#define VXM_KEY_SPACE              32
#define VXM_KEY_APOSTROPHE         39  /* ' */
#define VXM_KEY_COMMA              44  /* , */
#define VXM_KEY_MINUS              45  /* - */
#define VXM_KEY_PERIOD             46  /* . */
#define VXM_KEY_SLASH              47  /* / */
#define VXM_KEY_0                  48
#define VXM_KEY_1                  49
#define VXM_KEY_2                  50
#define VXM_KEY_3                  51
#define VXM_KEY_4                  52
#define VXM_KEY_5                  53
#define VXM_KEY_6                  54
#define VXM_KEY_7                  55
#define VXM_KEY_8                  56
#define VXM_KEY_9                  57
#define VXM_KEY_SEMICOLON          59  /* ; */
#define VXM_KEY_EQUAL              61  /* = */
#define VXM_KEY_A                  65
#define VXM_KEY_B                  66
#define VXM_KEY_C                  67
#define VXM_KEY_D                  68
#define VXM_KEY_E                  69
#define VXM_KEY_F                  70
#define VXM_KEY_G                  71
#define VXM_KEY_H                  72
#define VXM_KEY_I                  73
#define VXM_KEY_J                  74
#define VXM_KEY_K                  75
#define VXM_KEY_L                  76
#define VXM_KEY_M                  77
#define VXM_KEY_N                  78
#define VXM_KEY_O                  79
#define VXM_KEY_P                  80
#define VXM_KEY_Q                  81
#define VXM_KEY_R                  82
#define VXM_KEY_S                  83
#define VXM_KEY_T                  84
#define VXM_KEY_U                  85
#define VXM_KEY_V                  86
#define VXM_KEY_W                  87
#define VXM_KEY_X                  88
#define VXM_KEY_Y                  89
#define VXM_KEY_Z                  90
#define VXM_KEY_LEFT_BRACKET       91  /* [ */
#define VXM_KEY_BACKSLASH          92  /* \ */
#define VXM_KEY_RIGHT_BRACKET      93  /* ] */
#define VXM_KEY_GRAVE_ACCENT       96  /* ` */
#define VXM_KEY_WORLD_1            161 /* non-US #1 */
#define VXM_KEY_WORLD_2            162 /* non-US #2 */


/* Function keys */
#define VXM_KEY_ESCAPE             256
#define VXM_KEY_ENTER              257
#define VXM_KEY_TAB                258
#define VXM_KEY_BACKSPACE          259
#define VXM_KEY_INSERT             260
#define VXM_KEY_DELETE             261
#define VXM_KEY_RIGHT              262
#define VXM_KEY_LEFT               263
#define VXM_KEY_DOWN               264
#define VXM_KEY_UP                 265
#define VXM_KEY_PAGE_UP            266
#define VXM_KEY_PAGE_DOWN          267
#define VXM_KEY_HOME               268
#define VXM_KEY_END                269
#define VXM_KEY_CAPS_LOCK          280
#define VXM_KEY_SCROLL_LOCK        281
#define VXM_KEY_NUM_LOCK           282
#define VXM_KEY_PRINT_SCREEN       283
#define VXM_KEY_PAUSE              284
#define VXM_KEY_F1                 290
#define VXM_KEY_F2                 291
#define VXM_KEY_F3                 292
#define VXM_KEY_F4                 293
#define VXM_KEY_F5                 294
#define VXM_KEY_F6                 295
#define VXM_KEY_F7                 296
#define VXM_KEY_F8                 297
#define VXM_KEY_F9                 298
#define VXM_KEY_F10                299
#define VXM_KEY_F11                300
#define VXM_KEY_F12                301
#define VXM_KEY_F13                302
#define VXM_KEY_F14                303
#define VXM_KEY_F15                304
#define VXM_KEY_F16                305
#define VXM_KEY_F17                306
#define VXM_KEY_F18                307
#define VXM_KEY_F19                308
#define VXM_KEY_F20                309
#define VXM_KEY_F21                310
#define VXM_KEY_F22                311
#define VXM_KEY_F23                312
#define VXM_KEY_F24                313
#define VXM_KEY_F25                314
#define VXM_KEY_KP_0               320
#define VXM_KEY_KP_1               321
#define VXM_KEY_KP_2               322
#define VXM_KEY_KP_3               323
#define VXM_KEY_KP_4               324
#define VXM_KEY_KP_5               325
#define VXM_KEY_KP_6               326
#define VXM_KEY_KP_7               327
#define VXM_KEY_KP_8               328
#define VXM_KEY_KP_9               329
#define VXM_KEY_KP_DECIMAL         330
#define VXM_KEY_KP_DIVIDE          331
#define VXM_KEY_KP_MULTIPLY        332
#define VXM_KEY_KP_SUBTRACT        333
#define VXM_KEY_KP_ADD             334
#define VXM_KEY_KP_ENTER           335
#define VXM_KEY_KP_EQUAL           336
#define VXM_KEY_LEFT_SHIFT         340
#define VXM_KEY_LEFT_CONTROL       341
#define VXM_KEY_LEFT_ALT           342
#define VXM_KEY_LEFT_SUPER         343
#define VXM_KEY_RIGHT_SHIFT        344
#define VXM_KEY_RIGHT_CONTROL      345
#define VXM_KEY_RIGHT_ALT          346
#define VXM_KEY_RIGHT_SUPER        347
#define VXM_KEY_MENU               348
#define VXM_KEY_LAST               VXM_KEY_MENU

namespace Voxymore::Core {

	enum class Key : int {
		Unknown = VXM_KEY_UNKNOWN,
		Space = VXM_KEY_SPACE,
		Apostrophe = VXM_KEY_APOSTROPHE,
		Comma = VXM_KEY_COMMA,
		Minus = VXM_KEY_MINUS,
		Period = VXM_KEY_PERIOD,
		Slash = VXM_KEY_SLASH,
		Key0 = VXM_KEY_0,
		Key1 = VXM_KEY_1,
		Key2 = VXM_KEY_2,
		Key3 = VXM_KEY_3,
		Key4 = VXM_KEY_4,
		Key5 = VXM_KEY_5,
		Key6 = VXM_KEY_6,
		Key7 = VXM_KEY_7,
		Key8 = VXM_KEY_8,
		Key9 = VXM_KEY_9,
		Semicolon = VXM_KEY_SEMICOLON,
		Equal = VXM_KEY_EQUAL,
		A = VXM_KEY_A,
		B = VXM_KEY_B,
		C = VXM_KEY_C,
		D = VXM_KEY_D,
		E = VXM_KEY_E,
		F = VXM_KEY_F,
		G = VXM_KEY_G,
		H = VXM_KEY_H,
		I = VXM_KEY_I,
		J = VXM_KEY_J,
		K = VXM_KEY_K,
		L = VXM_KEY_L,
		M = VXM_KEY_M,
		N = VXM_KEY_N,
		O = VXM_KEY_O,
		P = VXM_KEY_P,
		Q = VXM_KEY_Q,
		R = VXM_KEY_R,
		S = VXM_KEY_S,
		T = VXM_KEY_T,
		U = VXM_KEY_U,
		V = VXM_KEY_V,
		W = VXM_KEY_W,
		X = VXM_KEY_X,
		Y = VXM_KEY_Y,
		Z = VXM_KEY_Z,
		LeftBracket = VXM_KEY_LEFT_BRACKET,
		Backslash = VXM_KEY_BACKSLASH,
		RightBracket = VXM_KEY_RIGHT_BRACKET,
		GraveAccent = VXM_KEY_GRAVE_ACCENT,
		World1 = VXM_KEY_WORLD_1,
		World2 = VXM_KEY_WORLD_2,

		Escape = VXM_KEY_ESCAPE,
		Enter = VXM_KEY_ENTER,
		Tab = VXM_KEY_TAB,
		Backspace = VXM_KEY_BACKSPACE,
		Insert = VXM_KEY_INSERT,
		Delete = VXM_KEY_DELETE,
		Right = VXM_KEY_RIGHT,
		Left = VXM_KEY_LEFT,
		Down = VXM_KEY_DOWN,
		Up = VXM_KEY_UP,
		PageUp = VXM_KEY_PAGE_UP,
		PageDown = VXM_KEY_PAGE_DOWN,
		Home = VXM_KEY_HOME,
		End = VXM_KEY_END,
		CapsLock = VXM_KEY_CAPS_LOCK,
		ScrollLock = VXM_KEY_SCROLL_LOCK,
		NumLock = VXM_KEY_NUM_LOCK,
		PrintScreen = VXM_KEY_PRINT_SCREEN,
		Pause = VXM_KEY_PAUSE,
		F1 = VXM_KEY_F1,
		F2 = VXM_KEY_F2,
		F3 = VXM_KEY_F3,
		F4 = VXM_KEY_F4,
		F5 = VXM_KEY_F5,
		F6 = VXM_KEY_F6,
		F7 = VXM_KEY_F7,
		F8 = VXM_KEY_F8,
		F9 = VXM_KEY_F9,
		F10 = VXM_KEY_F10,
		F11 = VXM_KEY_F11,
		F12 = VXM_KEY_F12,
		F13 = VXM_KEY_F13,
		F14 = VXM_KEY_F14,
		F15 = VXM_KEY_F15,
		F16 = VXM_KEY_F16,
		F17 = VXM_KEY_F17,
		F18 = VXM_KEY_F18,
		F19 = VXM_KEY_F19,
		F20 = VXM_KEY_F20,
		F21 = VXM_KEY_F21,
		F22 = VXM_KEY_F22,
		F23 = VXM_KEY_F23,
		F24 = VXM_KEY_F24,
		F25 = VXM_KEY_F25,
		Kp0 = VXM_KEY_KP_0,
		Kp1 = VXM_KEY_KP_1,
		Kp2 = VXM_KEY_KP_2,
		Kp3 = VXM_KEY_KP_3,
		Kp4 = VXM_KEY_KP_4,
		Kp5 = VXM_KEY_KP_5,
		Kp6 = VXM_KEY_KP_6,
		Kp7 = VXM_KEY_KP_7,
		Kp8 = VXM_KEY_KP_8,
		Kp9 = VXM_KEY_KP_9,
		KpDecimal = VXM_KEY_KP_DECIMAL,
		KpDivide = VXM_KEY_KP_DIVIDE,
		KpMultiply = VXM_KEY_KP_MULTIPLY,
		KpSubtract = VXM_KEY_KP_SUBTRACT,
		KpAdd = VXM_KEY_KP_ADD,
		KpEnter = VXM_KEY_KP_ENTER,
		KpEqual = VXM_KEY_KP_EQUAL,
		LeftShift = VXM_KEY_LEFT_SHIFT,
		LeftControl = VXM_KEY_LEFT_CONTROL,
		LeftAlt = VXM_KEY_LEFT_ALT,
		LeftSuper = VXM_KEY_LEFT_SUPER,
		RightShift = VXM_KEY_RIGHT_SHIFT,
		RightControl = VXM_KEY_RIGHT_CONTROL,
		RightAlt = VXM_KEY_RIGHT_ALT,
		RightSuper = VXM_KEY_RIGHT_SUPER,
		Menu = VXM_KEY_MENU,
		Last = VXM_KEY_LAST,
	};
}