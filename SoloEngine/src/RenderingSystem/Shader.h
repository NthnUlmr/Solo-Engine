#pragma once
#include <string>
#include "glm/glm.hpp"
namespace Solo {

	// A shader is a class which basically connects up your 
	// glsl shader stages to the thing that need to go into them
	// (Uniforms, VAOs, etc)
	// 
	// It is assumed that all of your shader stages are in a single 
	// '*.shader' file broken apart by a line that consists of 
	// #shader <shader_stage_name>
	// Assumes glsl shaders.
	// 


	class Shader {
	public:
		Shader(std::string& path);
		~Shader();

		void setUniformi(const std::string& varName, const int value);
		void setUniformf(const std::string& varName, const float value);
		void setUniform(const std::string& varName, const glm::vec2& value);
		void setUniform(const std::string& varName, const glm::ivec2& value);
		void setUniform(const std::string& varName, const glm::vec3& value);
		void setUniform(const std::string& varName, const glm::vec4& value);
		void setUniform(const std::string& varName, const glm::mat3& value);
		void setUniform(const std::string& varName, const glm::mat4& value);

	private:
		void read();
		void compile();
		void create();

	};

}
