#pragma once


#include "System.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Image.h"
#include "Texture.h"
#include "ShaderPipeline.h"
#include "Window.h"
#include "CubeMap.h"

namespace Solo {
	



	class RenderingSystem : System {
	public:
		RenderingSystem() = default;
		virtual void init(std::shared_ptr<Scene> scene) override;
		virtual bool update(std::shared_ptr<Scene> scene) override;
		virtual void destroy(std::shared_ptr<Scene> scene) override;
		~RenderingSystem() = default;
	protected:
	private:
		

		void generateCube(double wposx, double wposy, double wposz);

		float t = 0.0;
		int location = 0;
		int location2 = 0;
		int location3 = 0;
		int location4 = 0;
		int location5 = 0;

		Texture* texture;
		Texture* texture2;
		CubeMap* skybox;

		unsigned int shader = 0;
		unsigned int skyboxShader = 0;

		unsigned int modelVao = 0;
		unsigned int skyboxVao = 0;

		glm::mat4x4 model = {};
		glm::mat4x4 view = {};
		glm::mat4x4 proj = {};
		int width = 0;
		int height = 0;
		int oldwidth = 0;
		int oldheight = 0;

		std::vector<ShaderPipeline*> pipelines = {};

		std::vector<glm::vec3> voxels = {};
		std::vector<float> vertexData = {};
		std::vector<unsigned int> indexData = {};
		

		int worldLength = 50;
		int worldWidth = 0;
		int worldHeight = 50;

		unsigned int vbuf;
		unsigned int ibo;

		bool initialized = true;
		GLFWwindow* soloWinCopy = nullptr;



		float positions[40] = {
	-0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,   1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,   0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,   1.0f, 1.0f,

	-0.5f, -0.5f, -0.5f,   1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,   0.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,   1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,   0.0f, 0.0f

		};

		unsigned int indices[36] = {
			//Top
			2, 6, 7,
			2, 3, 7,

			//Bottom
			0, 4, 5,
			0, 1, 5,

			//Left
			0, 2, 6,
			0, 4, 6,

			//Right
			1, 3, 7,
			1, 5, 7,

			//Front
			0, 2, 3,
			0, 1, 3,

			//Back
			4, 6, 7,
			4, 5, 7,
		};



	}; // class RenderingSystem
} // namespace Solo