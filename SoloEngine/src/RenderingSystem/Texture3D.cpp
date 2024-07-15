#include "Texture3D.h"
#define GLEW_STATIC 1
#include <GL/glew.h>
#include <glfw3.h>

namespace Solo
{
	Texture3D::Texture3D(std::string path)
	{
		path_ = path;


	}

	Texture3D::~Texture3D()
	{
		delete image_;
	}

	void Texture3D::bind() {
		image_ = new Image(path_);
		glGenTextures(1, &handle_);
		glBindTexture(GL_TEXTURE_3D, handle_);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage3D(GL_TEXTURE_3D,
			0,
			GL_RGB,
			image_->getWidth(),
			image_->getHeight(),
			1, // Depth
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			image_->getData());
		//glGenerateMipmap(GL_TEXTURE_3D);
		image_->free();
	}
} // namespace Solo