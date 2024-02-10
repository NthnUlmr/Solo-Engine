#pragma once
#include <string>
#include "Event.h"
#include "TimeStep.h"

namespace Solo {
	class AppLayer // Virtual
	{
	public:
		AppLayer(const std::string& name = "layer") : debugName_(name) {}
		virtual ~AppLayer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(TimeStep ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		const std::string& GetName() const { return debugName_; }

	private:
		std::string debugName_;

	};

}

