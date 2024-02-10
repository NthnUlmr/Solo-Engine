#pragma once

#include <string>
#include <cassert>
#include <memory>
#include <vector>
#include "LayerStack.h"

#include "Event.h"
#include "AppEvent.h"
#include "RenderingSystem.h"
#include "Window.h"
#include "AppEvent.h"
#include "GamepadEvent.h"
#include "GamepadCodes.h"

int main(int argc, char** argv);

namespace Solo {
	class AudioSystem;
	class PhysicsSystem;
	class RenderingSystem;
	class Scene;
	class System;
}




namespace Solo {




	struct AppCLArgs
	{
		int Count = 0;
		char** Args = nullptr;

		const char* operator[](int index) const { assert(index < Count);  return Args[index]; }
	};

	struct AppSettings {
		std::string name = "Empty Solo App";
		std::string workingDir;
		AppCLArgs	clArgs;
	};

	class App {
	public:
		App(const AppSettings& settings);


		void OnEvent(Event& e);

		static App& Get() { return *s_Instance; };

		inline void push_system(std::shared_ptr<System> sys)
		{
			systems_.push_back(sys);
		};

		void PushLayer(AppLayer* layer);
		void PushOverlay(AppLayer* layer);

	protected:
		std::shared_ptr<Scene> activeScene_ = nullptr;
	private:
		AppSettings settings_;
		void run();
		void stop();
		static App* s_Instance;						// Singleton instance
		friend int ::main(int argc, char** argv);	// This is the entry point of the static library
		bool isRunning = true;

		std::shared_ptr<AudioSystem> audioSystem_ = nullptr;
		std::shared_ptr<PhysicsSystem> physicsSystem_ = nullptr;
		std::shared_ptr<RenderingSystem> renderingSystem_ = nullptr;

		std::vector<std::shared_ptr<System>> systems_ = {};
		
		LayerStack appLayerStack = {};

		bool OnWindowClose(WindowCloseEvent& e);

		Window* window_ = nullptr;

		void CameraController(TimeStep dt);





		GamepadState gpState = {};

		glm::dvec3 m_CameraPosition = glm::vec3(0., 80., 0.);
		glm::dvec3 dir = glm::vec3(0., 0., 0.);
		glm::vec2 prevMousePee = glm::vec2(0., 0.);

		double m_CameraRotationY = 0;
		double m_CameraRotationX = 0;

		double m_CameraVelocityZ = 0;

		bool jumping = false;
		int count = 0;

		double diff = 0.0000001;
		double leftStickSensitivity = 5.0;
		double rightStickSensitivity = 5.0;
		double keyBoardSensitivity = 5.0;
		double playerBaseMoveSpeed = 5.0;
		double playerBaseTurnSpeed = 40.0;
		double mouseXScale = 1.0 / 50.0;
		double mouseYScale = 1.0 / 50.0;
		double mouseHorzSensitivity = 5.0;
		double mouseVertSensitivity = 5.0;

		bool processInput = true;
		long long timeLastFrameMS_ = 0;
	};
	App* CreateApp(AppCLArgs args);


}
