#pragma once
#include "Image.h"

namespace Solo
{
	class Texture3D
	{
	public:
		Texture3D(std::string path);
		Texture3D() = default;
		~Texture3D();

		void bind();

		inline unsigned int getHandle()
		{
			return handle_;
		}

	private:
		std::string path_ = "";
		Image* image_;

		unsigned int handle_;


	};
} // namespace Solo
