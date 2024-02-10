#include "LayerStack.h"



namespace Solo {



	LayerStack::~LayerStack()
	{
		for (AppLayer* layer : layers_)
		{
			layer->OnDetach();
			delete layer;
		}

	}

	void LayerStack::PushLayer(AppLayer* layer)
	{
		layers_.emplace(layers_.begin() + layerIndex_, layer);
	}

	void LayerStack::PushOverlay(AppLayer* overlay)
	{
		layers_.emplace_back(overlay);
	}

	void LayerStack::PopLayer(AppLayer* layer)
	{

	}

	void LayerStack::PopOverlay(AppLayer* overlay)
	{
	}


}