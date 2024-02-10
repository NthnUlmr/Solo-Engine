
// SoloEngine2.h : Include file for standard system include files,
// or project specific include files.

#define _SILENCE_CXX23_ALIGNED_STORAGE_DEPRECATION_WARNING
#include <iostream>
#include <App.h>
#include <Scene.h>
#include <EntryPoint.h>
#include <RenderComponents.h>
#include "EditorLayer.h"
#include "AppLayer.h"
// TODO: Reference additional headers your program requires here.

using namespace std;


class Editor : public Solo::App
{
public:
	Editor(const Solo::AppSettings& settings)
		: Solo::App(settings)
	{
		PushLayer(new EditorLayer());
		activeScene_ = make_shared<Solo::Scene>();
		Solo::TransformComponent tc = Solo::TransformComponent();

		// Generate Terrain

		tc.setTranslation({ 6.0f,5.0f,2.0f });
		glm::mat4 tf = tc.getTransform();
		auto e = activeScene_->registry_.create();
		activeScene_->registry_.emplace<Solo::TransformComponent>(e, tc);
		auto group = activeScene_->registry_.view<Solo::TransformComponent>();
		for (auto ent : group)
		{
			Solo::TransformComponent tcopy = group.get<Solo::TransformComponent>(ent);
			std::cout << "TC: " << tcopy.getTransform()[3][3] << std::endl;


		}


	}

	~Editor()
	{


	}



};


Solo::App* Solo::CreateApp(AppCLArgs args)
{
	AppSettings settings;
	settings.name = "Solo Editor";
	settings.clArgs = args;

	return new Editor(settings);
}
