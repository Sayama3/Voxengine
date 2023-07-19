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
    enum KeyCode : int {
        KEY_UNKNOWN = VXM_KEY_UNKNOWN,
        KEY_SPACE = VXM_KEY_SPACE,
        KEY_APOSTROPHE = VXM_KEY_APOSTROPHE,
        KEY_COMMA = VXM_KEY_COMMA,
        KEY_MINUS = VXM_KEY_MINUS,
        KEY_PERIOD = VXM_KEY_PERIOD,
        KEY_SLASH = VXM_KEY_SLASH,
        KEY_0 = VXM_KEY_0,
        KEY_1 = VXM_KEY_1,
        KEY_2 = VXM_KEY_2,
        KEY_3 = VXM_KEY_3,
        KEY_4 = VXM_KEY_4,
        KEY_5 = VXM_KEY_5,
        KEY_6 = VXM_KEY_6,
        KEY_7 = VXM_KEY_7,
        KEY_8 = VXM_KEY_8,
        KEY_9 = VXM_KEY_9,
        KEY_SEMICOLON = VXM_KEY_SEMICOLON,
        KEY_EQUAL = VXM_KEY_EQUAL,
        KEY_A = VXM_KEY_A,
        KEY_B = VXM_KEY_B,
        KEY_C = VXM_KEY_C,
        KEY_D = VXM_KEY_D,
        KEY_E = VXM_KEY_E,
        KEY_F = VXM_KEY_F,
        KEY_G = VXM_KEY_G,
        KEY_H = VXM_KEY_H,
        KEY_I = VXM_KEY_I,
        KEY_J = VXM_KEY_J,
        KEY_K = VXM_KEY_K,
        KEY_L = VXM_KEY_L,
        KEY_M = VXM_KEY_M,
        KEY_N = VXM_KEY_N,
        KEY_O = VXM_KEY_O,
        KEY_P = VXM_KEY_P,
        KEY_Q = VXM_KEY_Q,
        KEY_R = VXM_KEY_R,
        KEY_S = VXM_KEY_S,
        KEY_T = VXM_KEY_T,
        KEY_U = VXM_KEY_U,
        KEY_V = VXM_KEY_V,
        KEY_W = VXM_KEY_W,
        KEY_X = VXM_KEY_X,
        KEY_Y = VXM_KEY_Y,
        KEY_Z = VXM_KEY_Z,
        KEY_LEFT_BRACKET = VXM_KEY_LEFT_BRACKET,
        KEY_BACKSLASH = VXM_KEY_BACKSLASH,
        KEY_RIGHT_BRACKET = VXM_KEY_RIGHT_BRACKET,
        KEY_GRAVE_ACCENT = VXM_KEY_GRAVE_ACCENT,
        KEY_WORLD_1 = VXM_KEY_WORLD_1,
        KEY_WORLD_2 = VXM_KEY_WORLD_2,

        KEY_ESCAPE = VXM_KEY_ESCAPE,
        KEY_ENTER = VXM_KEY_ENTER,
        KEY_TAB = VXM_KEY_TAB,
        KEY_BACKSPACE = VXM_KEY_BACKSPACE,
        KEY_INSERT = VXM_KEY_INSERT,
        KEY_DELETE = VXM_KEY_DELETE,
        KEY_RIGHT = VXM_KEY_RIGHT,
        KEY_LEFT = VXM_KEY_LEFT,
        KEY_DOWN = VXM_KEY_DOWN,
        KEY_UP = VXM_KEY_UP,
        KEY_PAGE_UP = VXM_KEY_PAGE_UP,
        KEY_PAGE_DOWN = VXM_KEY_PAGE_DOWN,
        KEY_HOME = VXM_KEY_HOME,
        KEY_END = VXM_KEY_END,
        KEY_CAPS_LOCK = VXM_KEY_CAPS_LOCK,
        KEY_SCROLL_LOCK = VXM_KEY_SCROLL_LOCK,
        KEY_NUM_LOCK = VXM_KEY_NUM_LOCK,
        KEY_PRINT_SCREEN = VXM_KEY_PRINT_SCREEN,
        KEY_PAUSE = VXM_KEY_PAUSE,
        KEY_F1 = VXM_KEY_F1,
        KEY_F2 = VXM_KEY_F2,
        KEY_F3 = VXM_KEY_F3,
        KEY_F4 = VXM_KEY_F4,
        KEY_F5 = VXM_KEY_F5,
        KEY_F6 = VXM_KEY_F6,
        KEY_F7 = VXM_KEY_F7,
        KEY_F8 = VXM_KEY_F8,
        KEY_F9 = VXM_KEY_F9,
        KEY_F10 = VXM_KEY_F10,
        KEY_F11 = VXM_KEY_F11,
        KEY_F12 = VXM_KEY_F12,
        KEY_F13 = VXM_KEY_F13,
        KEY_F14 = VXM_KEY_F14,
        KEY_F15 = VXM_KEY_F15,
        KEY_F16 = VXM_KEY_F16,
        KEY_F17 = VXM_KEY_F17,
        KEY_F18 = VXM_KEY_F18,
        KEY_F19 = VXM_KEY_F19,
        KEY_F20 = VXM_KEY_F20,
        KEY_F21 = VXM_KEY_F21,
        KEY_F22 = VXM_KEY_F22,
        KEY_F23 = VXM_KEY_F23,
        KEY_F24 = VXM_KEY_F24,
        KEY_F25 = VXM_KEY_F25,
        KEY_KP_0 = VXM_KEY_KP_0,
        KEY_KP_1 = VXM_KEY_KP_1,
        KEY_KP_2 = VXM_KEY_KP_2,
        KEY_KP_3 = VXM_KEY_KP_3,
        KEY_KP_4 = VXM_KEY_KP_4,
        KEY_KP_5 = VXM_KEY_KP_5,
        KEY_KP_6 = VXM_KEY_KP_6,
        KEY_KP_7 = VXM_KEY_KP_7,
        KEY_KP_8 = VXM_KEY_KP_8,
        KEY_KP_9 = VXM_KEY_KP_9,
        KEY_KP_DECIMAL = VXM_KEY_KP_DECIMAL,
        KEY_KP_DIVIDE = VXM_KEY_KP_DIVIDE,
        KEY_KP_MULTIPLY = VXM_KEY_KP_MULTIPLY,
        KEY_KP_SUBTRACT = VXM_KEY_KP_SUBTRACT,
        KEY_KP_ADD = VXM_KEY_KP_ADD,
        KEY_KP_ENTER = VXM_KEY_KP_ENTER,
        KEY_KP_EQUAL = VXM_KEY_KP_EQUAL,
        KEY_LEFT_SHIFT = VXM_KEY_LEFT_SHIFT,
        KEY_LEFT_CONTROL = VXM_KEY_LEFT_CONTROL,
        KEY_LEFT_ALT = VXM_KEY_LEFT_ALT,
        KEY_LEFT_SUPER = VXM_KEY_LEFT_SUPER,
        KEY_RIGHT_SHIFT = VXM_KEY_RIGHT_SHIFT,
        KEY_RIGHT_CONTROL = VXM_KEY_RIGHT_CONTROL,
        KEY_RIGHT_ALT = VXM_KEY_RIGHT_ALT,
        KEY_RIGHT_SUPER = VXM_KEY_RIGHT_SUPER,
        KEY_MENU = VXM_KEY_MENU,
        KEY_LAST = VXM_KEY_LAST,
    };
}