#pragma once

#include <vector>
#include "Texture.h"
#include "Uniform.h"
#include "Shader.h"
#include "VertexArrayObject.h"
namespace Solo
{
	class ShaderPipeline
	{
	public:
		ShaderPipeline();
		~ShaderPipeline();

		void init();
		void update();
		void destroy();

		void addShader(std::string path); // Parse shader and automatically determine vao
		void addTexture(std::string path);
		void addUniformV4();
		void addUniformMat4();

	private:

		std::vector<Shader> shaders = {};
		std::vector<Texture> textures = {};
		std::vector<Uniform> uniforms = {};
		std::vector<VertexArrayObject> vaos = {};

	};

}
