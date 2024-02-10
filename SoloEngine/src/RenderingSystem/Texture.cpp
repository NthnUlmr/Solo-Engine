#include "Texture.h"
#define GLEW_STATIC 1
#include <GL/glew.h>
#include <glfw3.h>

namespace Solo
{
	Texture::Texture(std::string path)
	{
		path_ = path;
		
		
	}

	Texture::~Texture()
	{
		delete image_;
	}

	void Texture::bind() {
		image_ = new Image(path_);
		glGenTextures(1, &handle_);
		glBindTexture(GL_TEXTURE_2D, handle_);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 
					0, 
					GL_RGB,
					image_->getWidth(),
					image_->getHeight(),
					0,
					GL_RGBA,
					GL_UNSIGNED_BYTE,
					image_->getData());
		glGenerateMipmap(GL_TEXTURE_2D);
		image_->free();
	}
} // namespace Solo