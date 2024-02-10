#pragma once

#include "System.h"

namespace Solo {
	class AudioSystem : System {
	public:
		AudioSystem() = default;
		virtual void init(std::shared_ptr<Scene> scene) override;
		virtual bool update(std::shared_ptr<Scene> scene) override;
		virtual void destroy(std::shared_ptr<Scene> scene) override;
	protected:
	private:
	}; // class AudioSystem
} // namespace Solo
