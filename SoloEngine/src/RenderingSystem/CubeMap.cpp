#include "CubeMap.h"
#define GLEW_STATIC 1
#include <GL/glew.h>
#include <glfw3.h>


namespace Solo {

	CubeMap::CubeMap(std::string relPath)
	{
		glGenTextures(1, &handle_);
		glBindTexture(GL_TEXTURE_CUBE_MAP, handle_);

		// Rel path is the path to the folder containing the cube map
		// expects the parts to be labeled as follows: back, bottom, front, left, right, top
		// with .jpg at the end.

		int width = 0;
		int height = 0;
		int nrChannels = 0;


		Image* timg = new Image(relPath + "right.jpg");
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, 
			GL_RGB, timg->getWidth(), timg->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, timg->getData());
		timg->free();

		timg = new Image(relPath + "left.jpg");
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0,
			GL_RGB, timg->getWidth(), timg->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, timg->getData());
		timg->free();

		timg = new Image(relPath + "top.jpg");
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0,
			GL_RGB, timg->getWidth(), timg->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, timg->getData());
		timg->free();

		timg = new Image(relPath + "bottom.jpg");
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0,
			GL_RGB, timg->getWidth(), timg->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, timg->getData());
		timg->free();

		timg = new Image(relPath + "back.jpg");
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0,
			GL_RGB, timg->getWidth(), timg->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, timg->getData());
		timg->free();

		timg = new Image(relPath + "front.jpg");
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0,
			GL_RGB, timg->getWidth(), timg->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, timg->getData());
		timg->free();


		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}

	void CubeMap::bind()
	{

	}
}
