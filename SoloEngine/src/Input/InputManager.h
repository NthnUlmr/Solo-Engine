#pragma once

#include "glm/glm.hpp"

#include "GamepadCodes.h"
#include "KeyCodes.h"
#include "MouseCodes.h"


#define BUTTON_PRESSED 1



namespace Solo {

	enum class kState {
		NOTPRESS = 0,
		PRESS = 1,
		HELD = 2,
		RELEASE = 3
	};

	class InputManager
	{
	public:

		static void Init();
		
		// Keyboard

		static bool IsKeyPressed(const KeyCode key);
		static bool IsKeyReleased(const KeyCode key);

		static bool IsKeyHeld(const KeyCode key);

		static bool IsKeyPressedOrHeld(const KeyCode key);
		static bool IsKeyUnPressed(const KeyCode key);

		static bool IsKeyReleasedOrUnPressed(const KeyCode key);



		// Mouse

		static bool IsMouseButtonPressed(const MouseCode button);



		static glm::vec2 GetMousePosition();

		static  void  SetMousePosition(glm::vec2);

		static float GetMouseX();

		static float GetMouseY();


		// Controller

		static void GetGamepadState(GamepadState& state);




	private:

		static void propagateState(kState state, int key);


	};

}

