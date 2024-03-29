#pragma once


#include <vector>

namespace Solo {




	struct GamepadState {


		// Compliant with glfw
		enum ButtonCode {
			GAMEPAD_BUTTON_A,
			GAMEPAD_BUTTON_B,
			GAMEPAD_BUTTON_X,
			GAMEPAD_BUTTON_Y,
			GAMEPAD_BUTTON_LEFT_BUMPER,
			GAMEPAD_BUTTON_RIGHT_BUMPER,
			GAMEPAD_BUTTON_BACK,
			GAMEPAD_BUTTON_START,
			GAMEPAD_BUTTON_GUIDE,
			GAMEPAD_BUTTON_LEFT_THUMB,
			GAMEPAD_BUTTON_RIGHT_THUMB,
			GAMEPAD_BUTTON_DPAD_UP,
			GAMEPAD_BUTTON_DPAD_RIGHT,
			GAMEPAD_BUTTON_DPAD_DOWN,
			GAMEPAD_BUTTON_DPAD_LEFT,
			SIZE_BUTTON_CODE, ///15
			GAMEPAD_BUTTON_LAST = GAMEPAD_BUTTON_DPAD_LEFT,
			GAMEPAD_BUTTON_CROSS = GAMEPAD_BUTTON_A,
			GAMEPAD_BUTTON_CIRCLE = GAMEPAD_BUTTON_B,
			GAMEPAD_BUTTON_SQUARE = GAMEPAD_BUTTON_X,
			GAMEPAD_BUTTON_TRIANGLE = GAMEPAD_BUTTON_Y
		};



		// Compliant with glfw
		enum AxisCode {
			GAMEPAD_AXIS_LEFT_X,
			GAMEPAD_AXIS_LEFT_Y,
			GAMEPAD_AXIS_RIGHT_X,
			GAMEPAD_AXIS_RIGHT_Y,
			GAMEPAD_AXIS_LEFT_TRIGGER,
			GAMEPAD_AXIS_RIGHT_TRIGGER,
			SIZE_AXIS_CODE, ///6
			GAMEPAD_AXIS_LAST = GAMEPAD_AXIS_RIGHT_TRIGGER
		};


		unsigned char buttons[SIZE_BUTTON_CODE] = { 0 };

		float axes[SIZE_AXIS_CODE] = { 0 };

	};

}