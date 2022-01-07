/*=============================================================================*/
// Copyright 2018-2019 Elite Engine
// Authors: Matthieu Delaere
/*=============================================================================*/
// EInputData.h: all structs/data used by the input manager
/*=============================================================================*/
#ifndef ELITE_INPUT_DATA
#define	ELITE_INPUT_DATA
namespace Elite
{
	struct KeyboardData //8 bytes
	{
		int TimeStamp = 0;
		InputScancode ScanCode = eScancode_Unknown;

		KeyboardData() = default;
		KeyboardData(int timeStamp, InputScancode keyCode) :
			TimeStamp(timeStamp), ScanCode(keyCode)
		{}
	};

	struct MouseData //== 24 bytes
	{
		int TimeStamp = 0;
		InputMouseButton Button = InputMouseButton(0);
		int X = 0; //Position X relative to window OR amound of scroll, based on Type!
		int Y = 0; //Position Y relative to window OR amound of scroll, based on Type!
		int XRel = 0;
		int YRel = 0; //Y == Direction when scrolling (1 == UP, -1 == DOWN)

		MouseData() = default;
		MouseData(int timeStamp, InputMouseButton button, int x, int y,
			int xRel = 0, int yRel = 0) :
			TimeStamp(timeStamp), Button(button), X(x), Y(y),
			XRel(xRel), YRel(yRel)
		{}
	};

	union InputData //"Wasting" 16 bytes for a more user-friendly setup, SDL wastes even more memory (= 48 bytes)
	{
		MouseData MouseInputData;
		KeyboardData KeyboardInputData;

		InputData(MouseData data) : MouseInputData(data) {}
		InputData(KeyboardData data) : KeyboardInputData(data) {}
	};

	//=== Actual InputAction used by the InputManager ===
	struct InputAction
	{
		InputType InputActionType = InputType::eDefault;
		InputState InputActionState = eDown;
		InputData InputActionData;

		InputAction(InputType type, InputState state, InputData data) :
			InputActionType(type), InputActionState(state), InputActionData(data) {}
		InputAction() = default;
	};
}
#endif