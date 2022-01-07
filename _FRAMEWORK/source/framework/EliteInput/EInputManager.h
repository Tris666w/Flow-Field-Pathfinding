/*=============================================================================*/
// Copyright 2021-2022 Elite Engine
// Authors: Matthieu Delaere
/*=============================================================================*/
// EInputManager.h: manager class that controls the input in the engine.
/*=============================================================================*/
#ifndef ELITE_INPUT_MANAGER
#define	ELITE_INPUT_MANAGER

namespace Elite
{
	//=== Forward Declaration ===
#if (PLATFORM_ID == PLATFORM_WINDOWS)
	class SDLWindow;
#endif

	/*! EInputManager: manager class that controls all the input, captured from active platform & window*/
	class EInputManager final : public ESingleton<EInputManager>
	{
	public:
		bool IsKeyboardKeyDown(InputScancode key) { return IsKeyPresent(eKeyboard, eDown, key); };
		bool IsKeyboardKeyUp(InputScancode key) { return IsKeyPresent(eKeyboard, eReleased, key); }

		bool IsMouseButtonDown(InputMouseButton button) { return IsMousePresent(eMouseButton, eDown, button); }
		bool IsMouseButtonUp(InputMouseButton button) { return IsMousePresent(eMouseButton, eReleased, button); }
		bool IsMouseScrolling() { return IsMousePresent(eMouseWheel); }
		bool IsMouseMoving() { return IsMousePresent(eMouseMotion); }
		MouseData GetMouseData(InputType type, InputMouseButton button = InputMouseButton(0));

	private:
		//=== Friends ===
		//Our window has access to add input events to our queue, our application can later use these events
#if (PLATFORM_ID == PLATFORM_WINDOWS)
		friend SDLWindow;
#endif

		//=== Internal Functions
		void Flush(){ m_InputContainer.clear();};
		void AddInputAction(const InputAction& inputAction) 
		{ m_InputContainer.push_back(inputAction); };

		bool IsKeyPresent(InputType type, InputState state, InputScancode code);
		bool IsMousePresent(InputType type, InputState state = InputState(0), InputMouseButton button = InputMouseButton(0));

		//=== Datamembers ===
		std::vector<InputAction> m_InputContainer;
	};
}
#endif