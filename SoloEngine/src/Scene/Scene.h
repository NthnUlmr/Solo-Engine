#pragma once
#include "entt.hpp"
#include "Camera.h"
#include <memory>

namespace Solo
{
class Scene {
public:
	Scene();
	~Scene();

	void update();

	std::shared_ptr<Camera> camera_ = nullptr;
	entt::registry registry_ = {};
	
protected:
private:

	
}; 
} //namespace Solo
