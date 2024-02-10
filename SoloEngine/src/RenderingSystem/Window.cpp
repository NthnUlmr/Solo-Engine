#include "Window.h"
#include <assert.h>
#include "AppEvent.h"
#include "KeyEvent.h"
#include "MouseEvent.h"
#include "Logger.h"
namespace Solo {

	Window* Window::soloWindow = nullptr;

	Window::Window(const WinProperties& properties)
	{

		assert(!soloWindow); // singleton



		if (!glfwInit())
		{
			// Initialization failed
			Logger::Log("RenderingSystem::init() GLFW initialization Failed!", LogLevel::FATAL);
			while (true);
		}
		Logger::Log("RenderingSystem::init() GLFW initialization Succeeded!", LogLevel::INFO);


		// Create Window
		{
			// TODO: Check Rendering API 

			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

			glfwWindow_ = glfwCreateWindow((int)properties.width, (int)properties.height, properties.name.c_str(), nullptr, nullptr);
			//++s_GLFWWindowCount;
		}


		// Create and init Graphics Context (Vulkan For Now)


		//// Use UsrPtr to set window event callbacks
		usrPtrData_.title = "b\0";
		glfwSetWindowUserPointer(glfwWindow_, &usrPtrData_);
		//glfwSetJoystickUserPointer();
		//glfwSetMonitorUserPointer();


		//// Each callback should correspond to an event type  in Events.h
		glfwSetCursorEnterCallback(glfwWindow_, [](GLFWwindow* window, int entered)
			{
				UsrPtrData& data = *(UsrPtrData*)glfwGetWindowUserPointer(window);
				assert(data.eventCallback);
				Logger::Log("Cursor Enter / Exit", LogLevel::TRACE);
				MouseEnteredEvent e;
				e.entered = entered;
				data.eventCallback(e);

			});


		glfwSetCursorPosCallback(glfwWindow_, [](GLFWwindow* window, double xpos, double ypos)
			{
				UsrPtrData& data = *(UsrPtrData*)glfwGetWindowUserPointer(window);
				assert(data.eventCallback);
				Logger::Log("Cursor Pos: " + std::to_string(xpos) + " " + std::to_string(ypos), LogLevel::INSANE);
				MouseRepositionedEvent e;
				e.xPos = xpos;
				e.yPos = ypos;
				data.eventCallback(e);

			});

		//glfwSetDropCallback(glfwWindow_, []());

		//glfwSetFramebufferSizeCallback(glfwWindow_, []());

		//glfwSetJoystickCallback([]());

		//glfwSetKeyCallback(glfwWindow_, []());

		//glfwSetMonitorCallback([]());

		//glfwSetMouseButtonCallback(glfwWindow_, []());

		//glfwSetScrollCallback(glfwWindow_, []());

		glfwSetWindowCloseCallback(glfwWindow_, [](GLFWwindow* window)
			{
				UsrPtrData& data = *(UsrPtrData*)glfwGetWindowUserPointer(window);
				assert(data.eventCallback);
				Logger::Log("Window Close Callback Reached", LogLevel::TRACE);
				WindowCloseEvent e;
				data.eventCallback(e);

			});

		//glfwSetWindowContentScaleCallback(glfwWindow_, []());

		//glfwSetWindowFocusCallback(glfwWindow_, []());

		//glfwSetWindowIconifyCallback(glfwWindow_, []());

		//glfwSetWindowMaximizeCallback(glfwWindow_, []());

		//glfwSetWindowPosCallback(glfwWindow_, []());

		//glfwSetWindowRefreshCallback(glfwWindow_, []());

		glfwSetWindowSizeCallback(glfwWindow_, [](GLFWwindow* window, int width, int height)
			{
				UsrPtrData& data = *(UsrPtrData*)glfwGetWindowUserPointer(window);
				assert(data.eventCallback);
				Logger::Log("Window Resized: " + std::to_string(width) + " " + std::to_string(height), LogLevel::TRACE);
				WindowResizedEvent e;
				e.width = width;
				e.height = height;

				data.width;
				data.height;
				data.eventCallback(e);

			});

		if (glfwRawMouseMotionSupported())
			glfwSetInputMode(glfwWindow_, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

		glfwSetInputMode(glfwWindow_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetInputMode(glfwWindow_, GLFW_STICKY_KEYS, GLFW_TRUE);
	}


	void Window::ToggleMouseCapture()
	{
		if (GLFW_CURSOR_DISABLED == glfwGetInputMode(glfwWindow_, GLFW_CURSOR))
		{
			glfwSetInputMode(glfwWindow_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		else
		{
			glfwSetInputMode(glfwWindow_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
	}
}
