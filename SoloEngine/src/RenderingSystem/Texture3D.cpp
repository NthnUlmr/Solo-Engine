#include "Texture3D.h"
#define GLEW_STATIC 1
#include <GL/glew.h>
#include <glfw3.h>

namespace Solo
{

	Texture3D::Texture3D(std::string path)
	{
		path_ = path;

		image_ = new Image3D(path_);
		glGenTextures(1, &handle_);
		

	}

	Texture3D::~Texture3D()
	{
		delete image_;
	}

	void Texture3D::bind() {
		glBindTexture(GL_TEXTURE_3D, handle_);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage3D(GL_TEXTURE_3D,
			0,
			GL_RG,
			image_->getWidth(),
			image_->getHeight(),
			image_->getDepth(), // Depth
			0,
			GL_RG,
			GL_FLOAT,
			image_->getData());
		glGenerateMipmap(GL_TEXTURE_3D);
		image_->free();
	}


	void Texture3D::change()
	{
		image_->change();
	}
} // namespace Solo