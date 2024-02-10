#include "InputManager.h"

#include <iostream>

#include "App.h"

#include "Window.h"

static int JID = 0;



static Solo::kState _keyStates[Solo::Key::SIZE];

void joystick_callback(int jid, int evnt)
{

	if (evnt == GLFW_CONNECTED)
	{
		std::cout << "CONNECTED : " << jid << std::endl;
		JID = jid;
	}
	else if (evnt == GLFW_DISCONNECTED) {
		std::cout << "DISCONNECTED : " << jid << std::endl;
	}
}

void keypressed_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		if (_keyStates[key] == Solo::kState::NOTPRESS or _keyStates[key] == Solo::kState::RELEASE)
		{
			_keyStates[key] = Solo::kState::PRESS;
		}
		else if (_keyStates[key] == Solo::kState::PRESS)
		{
			_keyStates[key] = Solo::kState::HELD;
		}
		else if (_keyStates[key] == Solo::kState::HELD)
		{
			_keyStates[key] = Solo::kState::HELD;
		}
		else
		{
			_keyStates[key] = Solo::kState::RELEASE; // ERROR
		}
	}

	if (action == GLFW_RELEASE)
	{
		if (_keyStates[key] == Solo::kState::NOTPRESS or _keyStates[key] == Solo::kState::RELEASE)
		{
			_keyStates[key] = Solo::kState::NOTPRESS;
		}
		else if (_keyStates[key] == Solo::kState::PRESS)
		{
			_keyStates[key] = Solo::kState::RELEASE;
		}
		else if (_keyStates[key] == Solo::kState::HELD)
		{
			_keyStates[key] = Solo::kState::RELEASE;
		}
		else
		{
			_keyStates[key] = Solo::kState::RELEASE; // ERROR
		}
	}

	
}


namespace Solo {
	static GamepadState _gpState;



	static int _jid = 0;


	void InputManager::Init()
	{
		//auto* window = static_cast<GLFWwindow*>(App::Get().GetWindow()->GetNativeWindow());
		// Initialize Callbacks
		glfwSetKeyCallback(Window::get(), keypressed_callback);

		glfwSetJoystickCallback(joystick_callback); // For some reason this is necessary for joystick reconnect detection;

	}

	bool InputManager::IsKeyPressed(const KeyCode key)
	{
		kState state = _keyStates[key];
		propagateState(state, key);

		if (state == kState::PRESS)
			std::cout << key << " : " << (int)_keyStates[key] << std::endl;

		return state == kState::PRESS;
	}


	bool InputManager::IsKeyHeld(const KeyCode key) {
		kState state = _keyStates[key];
		propagateState(state, key);

		return state == kState::HELD;
	}

	bool InputManager::IsKeyPressedOrHeld(const KeyCode key) {
		kState state = _keyStates[key];
		propagateState(state, key);

		return state == kState::PRESS || state == kState::HELD;
	}

	bool InputManager::IsKeyUnPressed(const KeyCode key) {
		kState state = _keyStates[key];
		propagateState(state, key);

		return state == kState::NOTPRESS;
	}

	bool InputManager::IsKeyReleasedOrUnPressed(const KeyCode key) {
		kState state = _keyStates[key];
		propagateState(state, key);

		return state == kState::PRESS || state == kState::NOTPRESS;
	}

	bool InputManager::IsKeyReleased(const KeyCode key)
	{
		kState state = _keyStates[key];
		propagateState(state, key);

		if(state == kState::RELEASE)
		std::cout << key << " : " << (int)_keyStates[key] << std::endl;

		return state == kState::RELEASE;
	}


	bool InputManager::IsMouseButtonPressed(const MouseCode button)
	{
		//auto* window = static_cast<GLFWwindow*>(App::Get().GetWindow()->GetNativeWindow());
		auto state = glfwGetMouseButton(Window::get(), static_cast<int32_t>(button));
		return state == BUTTON_PRESSED;
	}

	glm::vec2 InputManager::GetMousePosition()
	{
		//auto* window = static_cast<GLFWwindow*>(App::Get().GetWindow()->GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(Window::get(), &xpos, &ypos);
		

		return { (float)xpos, (float)ypos };
	}

	void InputManager::SetMousePosition(glm::vec2 pos)
	{
		//auto* window = static_cast<GLFWwindow*>(App::Get().GetWindow()->GetNativeWindow());
		glfwSetCursorPos(Window::get(), pos[0], pos[1]);
	}

	float InputManager::GetMouseX()
	{
		return GetMousePosition().x;
	}

	float InputManager::GetMouseY()
	{
		return GetMousePosition().y;
	}



	void InputManager::propagateState(kState state, int key)
	{
		if (state == kState::PRESS) _keyStates[key] = kState::HELD;
		if (state == kState::RELEASE) _keyStates[key] = kState::NOTPRESS;
	}

	void InputManager::GetGamepadState(GamepadState& state)
	{
		_jid = JID;
		glfwSetJoystickCallback(joystick_callback); // For some reason this is necessary for joystick reconnect detection;

		//auto* window = static_cast<GLFWwindow*>(App::Get().GetWindow()->GetNativeWindow());

		GLFWgamepadstate* glfwGpState = new GLFWgamepadstate();

		glfwPollEvents();
		if (glfwJoystickIsGamepad(_jid))
		{
			glfwGetGamepadState(_jid, glfwGpState);

			if (glfwGpState != nullptr)
			{
				memcpy_s(state.buttons, state.SIZE_BUTTON_CODE * sizeof(unsigned char), glfwGpState->buttons, state.SIZE_BUTTON_CODE * sizeof(unsigned char));
				memcpy_s(state.axes, state.SIZE_AXIS_CODE * sizeof(float), glfwGpState->axes, state.SIZE_AXIS_CODE * sizeof(float));

				_gpState = state;
			}
		}
	}







}