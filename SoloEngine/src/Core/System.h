#pragma once

#include <memory>
#include <chrono>
#include "Logger.h"
#include "Scene.h"
namespace Solo {


	class System {
	public:

		virtual void init(std::shared_ptr<Scene> scene) = 0;
		virtual bool update(std::shared_ptr<Scene> scene) = 0;
		virtual void destroy(std::shared_ptr<Scene> scene) = 0;

	protected:
	private:
	};

} // namespace Solo
