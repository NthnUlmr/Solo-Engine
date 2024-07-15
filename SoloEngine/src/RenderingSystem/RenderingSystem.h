#pragma once


#include "System.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Image.h"
#include "Texture.h"
#include "Texture3D.h"
#include "ShaderPipeline.h"
#include "Window.h"
#include "CubeMap.h"
#include <random>

namespace Solo {
	



	class RenderingSystem : System {
	public:
		RenderingSystem() = default;
		virtual void init(std::shared_ptr<Scene> scene) override;
		virtual bool update(std::shared_ptr<Scene> scene, const TimeStep& dt) override;
		virtual void destroy(std::shared_ptr<Scene> scene) override;
		~RenderingSystem() = default;
	protected:
	private:

		float t = 0.0;
		int location1 = 0;
		int location2 = 0;
		int location3 = 0;
		int location4 = 0;
		int location5 = 0;
		int location6 = 0;
		int location7 = 0;

		Texture* texture;
		Texture* texture2;
		Texture3D* voxelSdf;
		CubeMap* skybox;

		unsigned int shader = 0;
		unsigned int skyboxShader = 0;

		std::mt19937 generator;
		std::normal_distribution<double> distribution = std::normal_distribution<double>(5.0, 5.0);

		double maxX = 100;
		double maxY = 100;
		double divisor = 8;

		unsigned int modelVao = 0;
		unsigned int skyboxVao = 0;

		glm::mat4x4 model = {};
		glm::mat4x4 view = {};
		glm::mat4x4 proj = {};
		glm::vec3 eye_dir = {};
		glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 5.0f);//
		int width = 0;
		int height = 0;
		int oldwidth = 0;
		int oldheight = 0;

		std::vector<ShaderPipeline*> pipelines = {};

		std::vector<glm::vec3> voxels = {};
		std::vector<float> vertexData = {};
		std::vector<unsigned int> indexData = {};
		std::vector<unsigned int> objectIndex = {};
		std::vector<unsigned int> objectVertexIndex = {};
		

		int worldLength = 25;
		int worldWidth = 0;
		int worldHeight = 18;

		unsigned int vbuf;
		unsigned int ibo;

		bool initialized = true;
		GLFWwindow* soloWinCopy = nullptr;


		double paddleSpeed = 20.0;
		double paddleDirection = 0.0;


		glm::vec3 ballVelocity = { 80.0, -8.0, 0.0 };
		glm::vec3 ballPos = { 0.0, 0.0, 0.0 };

		float positions[48] = {
	-0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,

	-0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f

		};

		// TODO why do these need to be 6 elements long still??
		float quadPositions[24] = {
			-1.0f,	-1.0f,	0.0f,	0.0f,	0.0f,	0.0f,
			-1.0f,	1.0f,	0.0f,	0.0f,	0.0f,	0.0f,
			1.0f,	-1.0f,	0.0f,	0.0f,	0.0f,	0.0f,
			1.0f,	1.0f,	0.0f,	0.0f,	0.0f,	0.0f,
		};

		unsigned int quadIndices[6] = {
			2, 0, 1,
			1, 3, 2
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