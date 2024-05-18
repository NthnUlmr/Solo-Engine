#include "App.h"
#include "AudioSystem.h"
#include "PhysicsSystem.h"
#include "RenderingSystem.h"
#include "Logger.h"
#include "Scene.h"
#include "InputManager.h"

namespace Solo {



	App* App::s_Instance = nullptr; // Singleton

	App::App(const AppSettings& settings)
		: settings_(settings)
		, audioSystem_(std::make_shared<AudioSystem>())
		, physicsSystem_(std::make_shared<PhysicsSystem>())
		, renderingSystem_(std::make_shared<RenderingSystem>())
	{
		Logger::SetLevel(LogLevel::TRACE);
		Logger::Log("App::App() Entered", LogLevel::TRACE);

		assert(!s_Instance);// "App already exists! Only one instawnce of App is allowed.");
		s_Instance = this;

		// Create window - must be done before anyone needs it
		window_ = Window::init(WinProperties(settings_.name));
		window_->SetEventCallback(SOLO_BIND_EVENT_CALLBACK(App::OnEvent));
		InputManager::Init();
		renderingSystem_->init(activeScene_);
		audioSystem_->init(activeScene_);
	}

	void App::run()
	{
		Logger::Log("App::run() Entered", LogLevel::TRACE);
		double timeSinceLastLog = 0.0;
		double logRate = 2.0; // Hz
		double logInterval = 1.0 / logRate; // sec
		double currentFrameCount = 0;
		while (isRunning) {
			if (!activeScene_) continue;

			long long time = std::chrono::high_resolution_clock::now().time_since_epoch().count();
			TimeStep timestep = static_cast<long long>(time - timeLastFrameMS_);

			CameraController(timestep);

			
			activeScene_->update();
			audioSystem_->update(activeScene_, timestep);
			Logger::Log("RenderingSystem::update() Entered", LogLevel::INSANE);

			for (auto layer : appLayerStack)
			{
				layer->OnUpdate(timestep);
			}

			isRunning = isRunning 
					&& renderingSystem_->update(activeScene_, timestep);
			timeLastFrameMS_ = time;

			timeSinceLastLog += timestep.GetSeconds();
			currentFrameCount++;
			if (timeSinceLastLog >= logInterval)
			{
				
				std::cout << "FPS : " << currentFrameCount / timeSinceLastLog << std::endl;
				timeSinceLastLog = 0.0;
				currentFrameCount = 0.0;
			}

		}
	}

	void App::CameraController(TimeStep dt)
	{
		diff = dt.GetSeconds(); // time
		glm::vec2 mouseP = InputManager::GetMousePosition();

		if (InputManager::IsKeyPressed(Key::Escape))
		{
			window_->ToggleMouseCapture();
			processInput = !processInput; // For turning off input when you press escape
		}

		if (processInput)
		{

			if (InputManager::IsKeyPressedOrHeld(Key::W))
			{
				m_CameraPosition[0] -= -sin(glm::radians(-m_CameraRotationX)) * diff * keyBoardSensitivity * playerBaseMoveSpeed;
				m_CameraPosition[2] -= cos(glm::radians(-m_CameraRotationX)) * diff * keyBoardSensitivity * playerBaseMoveSpeed;

			}
			if (InputManager::IsKeyPressedOrHeld(Key::A))
			{
				m_CameraPosition[0] -= cos(glm::radians(-m_CameraRotationX)) * diff * keyBoardSensitivity * playerBaseMoveSpeed;
				m_CameraPosition[2] -= sin(glm::radians(-m_CameraRotationX)) * diff * keyBoardSensitivity * playerBaseMoveSpeed;

			}

			if (InputManager::IsKeyPressedOrHeld(Key::S))
			{
				m_CameraPosition[0] += -sin(glm::radians(-m_CameraRotationX)) * diff * keyBoardSensitivity * playerBaseMoveSpeed;
				m_CameraPosition[2] += cos(glm::radians(-m_CameraRotationX)) * diff * keyBoardSensitivity * playerBaseMoveSpeed;

			}

			if (InputManager::IsKeyPressedOrHeld(Key::D))
			{
				m_CameraPosition[0] += cos(glm::radians(-m_CameraRotationX)) * diff * keyBoardSensitivity * playerBaseMoveSpeed;
				m_CameraPosition[2] += sin(glm::radians(-m_CameraRotationX)) * diff * keyBoardSensitivity * playerBaseMoveSpeed;

			}

			if (InputManager::IsKeyPressed(Key::Space))
			{
				std::cout << "JUMP" << std::endl;
				m_CameraVelocityZ = 10;
			}

			m_CameraPosition[1] += m_CameraVelocityZ * diff;
			m_CameraPosition[1] = std::clamp(m_CameraPosition[1], 16.0, 999.0);

			if (m_CameraPosition[1] > 0.0)
			{

				m_CameraVelocityZ -= 32.2 * diff;
				m_CameraVelocityZ = std::clamp(m_CameraVelocityZ, -999.0, 999.0);
			}
			else {
				m_CameraVelocityZ = 0.0;
			}

			if (prevMousePee[0] != 0.0)
			{
				m_CameraRotationX -= ((mouseP[0] - prevMousePee[0]) * mouseXScale * mouseHorzSensitivity);
				m_CameraRotationY -= ((mouseP[1] - prevMousePee[1]) * mouseYScale * mouseVertSensitivity);
				m_CameraRotationY = std::clamp(m_CameraRotationY, -89., 89.);
			}




			if (fabs(gpState.axes[gpState.GAMEPAD_AXIS_LEFT_X]) >= 0.2)
			{
				m_CameraPosition[0] += cos(glm::radians(-m_CameraRotationX)) * gpState.axes[gpState.GAMEPAD_AXIS_LEFT_X] * diff * leftStickSensitivity * playerBaseMoveSpeed;
				m_CameraPosition[2] += sin(glm::radians(-m_CameraRotationX)) * gpState.axes[gpState.GAMEPAD_AXIS_LEFT_X] * diff * leftStickSensitivity * playerBaseMoveSpeed;
			}


			if (fabs(gpState.axes[gpState.GAMEPAD_AXIS_LEFT_Y]) >= 0.2)
			{
				m_CameraPosition[0] += -sin(glm::radians(-m_CameraRotationX)) * gpState.axes[gpState.GAMEPAD_AXIS_LEFT_Y] * diff * leftStickSensitivity * playerBaseMoveSpeed;
				m_CameraPosition[2] += cos(glm::radians(-m_CameraRotationX)) * gpState.axes[gpState.GAMEPAD_AXIS_LEFT_Y] * diff * leftStickSensitivity * playerBaseMoveSpeed;
			}


			if (fabs(gpState.axes[gpState.GAMEPAD_AXIS_RIGHT_X]) >= 0.2)
			{
				m_CameraRotationX -= gpState.axes[gpState.GAMEPAD_AXIS_RIGHT_X] * diff * rightStickSensitivity * playerBaseTurnSpeed;
			}

			if (fabs(gpState.axes[gpState.GAMEPAD_AXIS_RIGHT_Y]) >= 0.2)
			{
				m_CameraRotationY -= gpState.axes[gpState.GAMEPAD_AXIS_RIGHT_Y] * diff * rightStickSensitivity * playerBaseTurnSpeed;
				m_CameraRotationY = std::clamp(m_CameraRotationY, -89., 89.);
			}
		}
	prevMousePee = mouseP;
	
	m_CameraPosition[0] = 0.0;
	m_CameraPosition[1] = 0.0;
	m_CameraPosition[2] = 0.0;

	activeScene_->camera_->setRotation((float)m_CameraRotationY, (float)m_CameraRotationX);
	activeScene_->camera_->setPosition(m_CameraPosition);
	}

	void App::stop()
	{
		Logger::Log("App::stop() Entered", LogLevel::TRACE);

		Logger::Log("RenderingSystem::destroy() Entered", LogLevel::TRACE);
		renderingSystem_->destroy(activeScene_);
	}


	void App::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(SOLO_BIND_EVENT_CALLBACK(App::OnWindowClose));
	}

	bool App::OnWindowClose(WindowCloseEvent& e)
	{
		isRunning = false;
		return true;
	}

	void App::PushLayer(AppLayer* layer)
	{
		appLayerStack.PushLayer(layer);
		layer->OnAttach();


	}

	void App::PushOverlay(AppLayer* layer)
	{
		appLayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

}
