#pragma once
#define GLEW_STATIC 1
#include <GL/glew.h>
#include <glfw3.h>
#include <functional>
#include <string>
#include "Event.h"
#define SOLO_BIND_EVENT_CALLBACK(fun) [this] (auto&&... args) -> decltype(auto) {return this->fun(std::forward<decltype(args)>(args)...);}
#include <assert.h>

namespace Solo
{
	

using EventCallbackFn = std::function<void(Event&)>;
struct UsrPtrData
{
	std::string title = "a\0";
	unsigned int width = 0;
	unsigned int height = 0;
	bool fullScreen = false;
	bool _bFrameBufferResized = false;

	EventCallbackFn eventCallback = nullptr;
};

struct WinProperties
{
	std::string name;
	uint32_t width;
	uint32_t height;

	WinProperties(const std::string& name = "Solo", uint32_t width = 1280, uint32_t height = 720)
		: name(name),
		width(width),
		height(height)
	{
	}
};



class Window
{
public:

	typedef GLFWwindow PlatformWindow ;
	Window(const WinProperties& properties);

	static Window* init(const WinProperties& properties) {
		assert(!soloWindow);
		soloWindow = new Window(properties);
		return soloWindow;
	}
	static PlatformWindow* get() { return soloWindow->glfwWindow_;  };

	void SetEventCallback(const EventCallbackFn& callback) { usrPtrData_.eventCallback = callback; }

	void ToggleMouseCapture();

protected:

private:

	PlatformWindow* glfwWindow_ = nullptr;
	UsrPtrData usrPtrData_{};
	static Window* soloWindow;


};

}
