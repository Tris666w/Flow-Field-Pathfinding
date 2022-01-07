//=== General Includes ===
#include "stdafx.h"
#include "EInputManager.h"
using namespace Elite;

//=== Public Functions ===
MouseData EInputManager::GetMouseData(InputType type, InputMouseButton button)
{
	auto result = std::find_if(m_InputContainer.begin(), m_InputContainer.end(),
		[type, button](const InputAction& ia)
	{
		return
			(ia.InputActionType == type) &&
			(ia.InputActionData.MouseInputData.Button == button);
	});

	if (result != m_InputContainer.end())
		return (*result).InputActionData.MouseInputData;
	return
		MouseData();
}

//=== Private Functions ===
bool EInputManager::IsKeyPresent(InputType type, InputState state, InputScancode code)
{
	auto result = std::find_if(m_InputContainer.begin(), m_InputContainer.end(),
		[type, state, code](const InputAction& ia)
	{
		return
			(ia.InputActionType == type) &&
			(ia.InputActionState == state) &&
			(ia.InputActionData.KeyboardInputData.ScanCode == code);
	});
	return (result != m_InputContainer.end());
}

bool EInputManager::IsMousePresent(InputType type, InputState state, InputMouseButton button)
{
	auto result = std::find_if(m_InputContainer.begin(), m_InputContainer.end(),
		[type, state, button](const InputAction& ia)
	{
		return
			(ia.InputActionType == type) &&
			(ia.InputActionState == state) &&
			(ia.InputActionData.MouseInputData.Button == button);
	});
	return (result != m_InputContainer.end());
}