#pragma once
#include "AppLayer.h"
#include "Camera.h"
class EditorLayer : public Solo::AppLayer
{
public:
	EditorLayer();
	~EditorLayer();
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Solo::TimeStep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Solo::Event& event) override;

	Solo::Camera* editorCamera = nullptr;
	

protected:

private:
};
