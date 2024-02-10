#pragma once
#include "Texture.h"
#include <vector>
#include <String>

namespace Solo {

	class CubeMap
	{
	public:

		CubeMap(std::string relPath);

		void bind();

		inline unsigned int getHandle() { return handle_; };

	private:
		

		std::vector<Image*> sides = {};

		unsigned int handle_ = 0;

	};
}
