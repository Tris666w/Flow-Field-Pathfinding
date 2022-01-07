/*=============================================================================*/
// Copyright 2021-2022 Elite Engine
// Authors: Matthieu Delaere, Thomas Goussaert
/*=============================================================================*/
// EInputCodes.h: all codes for all possible input types.
/*=============================================================================*/
#ifndef ELITE_INPUT_CODES
#define	ELITE_INPUT_CODES
namespace Elite
{
	/*! InputType: holds what type of input this action comes from */
	enum InputType
	{
		eDefault,
		eKeyboard,
		eMouseButton,
		eMouseWheel,
		eMouseMotion
	};

	/*! InputState: holds current state of input */
	enum InputState
	{
		eDown,
		eReleased,
	};

	/*! InputMouseButton: holds current button of mouse used */
	enum InputMouseButton
	{
		eLeft = 1,
		eMiddle = 2,
		eRight = 3,
	};

	/*! InputScancode: scancode based on SDL 
	(which is based on the USB usage page: http://www.usb.org/developers/hidpage/Hut1_12v2.pdf) */
	enum InputScancode
	{
		eScancode_Unknown = 0,
		eScancode_A = 4,
		eScancode_B = 5,
		eScancode_C = 6,
		eScancode_D = 7,
		eScancode_E = 8,
		eScancode_F = 9,
		eScancode_G = 10,
		eScancode_H = 11,
		eScancode_I = 12,
		eScancode_J = 13,
		eScancode_K = 14,
		eScancode_L = 15,
		eScancode_M = 16,
		eScancode_N = 17,
		eScancode_O = 18,
		eScancode_P = 19,
		eScancode_Q = 20,
		eScancode_R = 21,
		eScancode_S = 22,
		eScancode_T = 23,
		eScancode_U = 24,
		eScancode_V = 25,
		eScancode_W = 26,
		eScancode_X = 27,
		eScancode_Y = 28,
		eScancode_Z = 29,

		eScancode_1 = 30,
		eScancode_2 = 31,
		eScancode_3 = 32,
		eScancode_4 = 33,
		eScancode_5 = 34,
		eScancode_6 = 35,
		eScancode_7 = 36,
		eScancode_8 = 37,
		eScancode_9 = 38,
		eScancode_0 = 39,

		eScancode_Return = 40,
		eScancode_Escape = 41,
		eScancode_Backspace = 42,
		eScancode_Tab = 43,
		eScancode_Space = 44,

		eScancode_Minus = 45,
		eScancode_Equals = 46,
		eScancode_LeftBracket = 47,
		eScancode_RightBracket = 48,
		eScancode_Backslash = 49, 
		eScancode_Nonuslash = 50, 
		eScancode_Semicolon = 51,
		eScancode_Apostrophe = 52,
		eScancode_Grave = 53, 
		eScancode_Comma = 54,
		eScancode_Period = 55,
		eScancode_Slash = 56,

		eScancode_Capslock = 57,

		eScancode_F1 = 58,
		eScancode_F2 = 59,
		eScancode_F3 = 60,
		eScancode_F4 = 61,
		eScancode_F5 = 62,
		eScancode_F6 = 63,
		eScancode_F7 = 64,
		eScancode_F8 = 65,
		eScancode_F9 = 66,
		eScancode_F10 = 67,
		eScancode_F11 = 68,
		eScancode_F12 = 69,

		eScancode_PrintScreen = 70,
		eScancode_ScrollLock = 71,
		eScancode_Pause = 72,
		eScancode_Insert = 73, 
		eScancode_Home = 74,
		eScancode_PageUp = 75,
		eScancode_Delete = 76,
		eScancode_End = 77,
		eScancode_PageDown = 78,
		eScancode_Right = 79,
		eScancode_Left = 80,
		eScancode_Down = 81,
		eScancode_Up = 82,

		eScancode_NumLockClear = 83,
		eScancode_KP_Divide = 84,
		eScancode_KP_Mulitply = 85,
		eScancode_KP_Minus = 86,
		eScancode_KP_Plus = 87,
		eScancode_KP_Enter = 88,
		eScancode_KP_1 = 89,
		eScancode_KP_2 = 90,
		eScancode_KP_3 = 91,
		eScancode_KP_4 = 92,
		eScancode_KP_5 = 93,
		eScancode_KP_6 = 94,
		eScancode_KP_7 = 95,
		eScancode_KP_8 = 96,
		eScancode_KP_9 = 97,
		eScancode_KP_0 = 98,
		eScancode_KP_Period = 99,

		eScancode_F13 = 104,
		eScancode_F14 = 105,
		eScancode_F15 = 106,
		eScancode_F16 = 107,
		eScancode_F17 = 108,
		eScancode_F18 = 109,
		eScancode_F19 = 110,
		eScancode_F20 = 111,
		eScancode_F21 = 112,
		eScancode_F22 = 113,
		eScancode_F23 = 114,
		eScancode_F24 = 115,

		eScancode_VolumeUp = 128,
		eScancode_VolumeDown = 129,
		eScancode_LockingCapsLock = 130,  
		eScancode_LockingNumLock = 131, 
		eScancode_LockingScrollLock = 132, 
		eScancode_KP_Comma = 133,
		eScancode_KP_EqualsSAS400 = 134, 

		eScancode_LCtrl = 224,
		eScancode_LShift = 225,
		eScancode_LAlt = 226,
		eScancode_LGUI = 227,
		eScancode_RCtrl = 228,
		eScancode_RShift = 229,
		eScancode_RAlt = 230,
		eScancode_RGUI = 231,
	};
}
#endif