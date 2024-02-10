#include "Image.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
namespace Solo {

	Image::Image(std::string path) 
	: path_(path)
	{
		/// \todo Double check path is valid so can catch error and give user useful error
		stbi_set_flip_vertically_on_load(false);
		data_ = stbi_load(path_.c_str(), &width_, &height_, &nChannels_, 0);
		assert(data_);
		if (data_)
			isLoaded_ = true;
		else isLoaded_ = false;
		
	}

	Image::~Image() 
	{
	}

	void Image::free()
	{
		if (isLoaded_)
		{
			stbi_image_free(data_);
			isLoaded_ = false;
		}
	}
}
