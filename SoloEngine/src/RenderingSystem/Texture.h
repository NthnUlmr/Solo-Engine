#pragma once
#include "Image.h"

namespace Solo
{
	class Texture
	{
	public:
		Texture(std::string path);
		Texture() = default;
		~Texture();

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
