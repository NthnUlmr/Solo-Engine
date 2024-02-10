#pragma once
#include "AppLayer.h"
#include <vector>
namespace Solo {

	class LayerStack
	{
	public:
		LayerStack() = default;
		~LayerStack();

		void PushLayer(AppLayer* layer);
		void PushOverlay(AppLayer* overlay);
		void PopLayer(AppLayer* layer);
		void PopOverlay(AppLayer* overlay);


		std::vector<AppLayer*>::iterator begin() { return layers_.begin(); }
		std::vector<AppLayer*>::iterator end() { return layers_.end(); }
		std::vector<AppLayer*>::reverse_iterator rbegin() { return layers_.rbegin(); }
		std::vector<AppLayer*>::reverse_iterator rend() { return layers_.rend(); }

		std::vector<AppLayer*>::const_iterator begin() const { return layers_.begin(); }
		std::vector<AppLayer*>::const_iterator end()	const { return layers_.end(); }
		std::vector<AppLayer*>::const_reverse_iterator rbegin() const { return layers_.rbegin(); }
		std::vector<AppLayer*>::const_reverse_iterator rend() const { return layers_.rend(); }



	private:
		std::vector<AppLayer*> layers_;
		unsigned int layerIndex_ = 0;

	};

}
