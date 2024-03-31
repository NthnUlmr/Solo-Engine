#pragma once


#include "System.h"

namespace Solo {
	class PhysicsSystem : System {
	public:
		PhysicsSystem() = default;
		virtual void init(std::shared_ptr<Scene> scene) override;
		virtual bool update(std::shared_ptr<Scene> scene, const TimeStep& dt) override;
		virtual void destroy(std::shared_ptr<Scene> scene) override;
	protected:
	private:
	}; // class PhysicsSystem
} // namespace Solo