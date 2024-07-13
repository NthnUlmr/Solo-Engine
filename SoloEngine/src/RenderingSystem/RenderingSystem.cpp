#include "RenderingSystem.h"

#include "InputManager.h"


#include <fstream>
#include <string>
#include <sstream>


#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

/// <summary>
/// \todo temp
/// </summary>
/// <param name="source"></param>
/// <param name="type"></param>
/// <returns></returns>
static unsigned int compileShader(const std::string& source, unsigned int type)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length*sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		Solo::Logger::Log("compileShader() " 
			+ static_cast<std::string>(type==GL_VERTEX_SHADER? "vertex": "fragment")
			+ " failed", Solo::LogLevel::ERROR);

		Solo::Logger::Log("compileShader() " + static_cast<std::string>(message), Solo::LogLevel::ERROR);
		
		glDeleteShader(id);
		id = 0;
	}
	//assert(id);
	return id;
}

/// <summary>
///  \todo temp
/// </summary>
/// <param name="vertexShader"></param>
/// <param name="fragShader"></param>
/// <returns></returns>
static unsigned int createShader(const std::string& vertexShader, const std::string& fragShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = compileShader(vertexShader, GL_VERTEX_SHADER);
	unsigned int fs = compileShader(fragShader, GL_FRAGMENT_SHADER);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

static void readShader(const std::string& path, std::string& vertexShader, std::string& fragmentShader)
{
	std::ifstream stream(path);

	enum class shaderType
	{
		None = -1,
		Vertex = 0,
		Fragment = 1
	};

	std::string line;
	std::stringstream ss[2];
	shaderType type = shaderType::None;
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = shaderType::Vertex;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = shaderType::Fragment;
			}
		}
		else if (type != shaderType::None)
		{
			ss[(int)type] << line << '\n';
		}
	}
	vertexShader = ss[0].str();
	fragmentShader = ss[1].str();

}


void errorCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	bool doLog = true;
	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:
		std::cout << "severity High" << std::endl;
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		std::cout << "severity Medium" << std::endl;
		break;
	case GL_DEBUG_SEVERITY_LOW:
		std::cout << "severity Low" << std::endl;
		break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		std::cout << "severity Notify" << std::endl;
		doLog = false;
		break;
	default:
		break;
	}
	
	if (doLog)
	{
		Solo::Logger::Log("[GLError] " + static_cast<std::string>(static_cast<const char*>(message)), Solo::LogLevel::WARNING);
	}
	//throw std::runtime_error("[GLError] " + static_cast<std::string>(static_cast<const char*>(message))); // if we want to throw instead of soft failing
}


namespace Solo {

	void RenderingSystem::init(std::shared_ptr<Scene> scene)
	{	
		soloWinCopy = Window::get();
		//soloWinCopy = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
		if (!soloWinCopy)
		{
			// Window or OpenGL context creation failed
			glfwTerminate();
			Logger::Log("RenderingSystem::init() Window or OpenGL context creation failed!", LogLevel::FATAL);
		}

		glfwMakeContextCurrent(soloWinCopy);
		glfwSwapInterval(0.0);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);

		if (glewInit() != GLEW_OK)
		{
			// Initialization failed
			Logger::Log("RenderingSystem::init() GLEW initialization Failed!", LogLevel::FATAL);
			while (true);
		}
		Logger::Log("RenderingSystem::init() GLEW initialization Succeeded!", LogLevel::INFO);

		Logger::Log("RenderingSystem::init() Active GL Version: " + static_cast<std::string>((char*)glGetString(GL_VERSION)), LogLevel::INFO);

		//glDebugMessageCallback(&errorCallback,nullptr);
		//glfwPollEvents();
		///\todo Temporary
		/// 
		//  Generate vertex buffer
		std::string basePath = "res/";
		//pipelines.push_back(new ShaderPipeline());
		//ShaderPipeline* texturedModel
		//pipelines[0]->addShader(basePath + "basic.shader");
		//pipelines[0]->addTexture(basePath + "grenuk.PNG");


		{
			glGenVertexArrays(1, &modelVao);
			glBindVertexArray(modelVao);


			
			glGenBuffers(1, &vbuf);
			glBindBuffer(GL_ARRAY_BUFFER, vbuf);
			//glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_DYNAMIC_DRAW);
			glBufferData(GL_ARRAY_BUFFER, sizeof(quadPositions), quadPositions, GL_DYNAMIC_DRAW);


			
			glGenBuffers(1, &ibo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
			//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_DYNAMIC_DRAW);

			// Specify layout of buffer
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (const void*)0);
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (const void*)(sizeof(float) * 3));
			glEnableVertexAttribArray(1);
		
		}

		GLfloat skyboxVertices[] = {
			// Positions
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f, 1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f, 1.0f,
			1.0f, -1.0f, 1.0f
		};

		glGenVertexArrays(1, &skyboxVao);
		glBindVertexArray(skyboxVao);
		unsigned int skyboxVbo = 0;
		glGenBuffers(1, &skyboxVbo);
		glBindBuffer(GL_ARRAY_BUFFER, skyboxVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glBindVertexArray(modelVao);
		{
			std::string vertexShader;
			std::string fragShader;
			readShader(basePath + "skybox.shader", vertexShader, fragShader);
			Logger::Log("vertexShader: \n" + vertexShader, LogLevel::TRACE);
			Logger::Log("fragShader: \n" + fragShader, LogLevel::TRACE);

			glActiveTexture(GL_TEXTURE0);
			skybox = new CubeMap(basePath + "skybox/");
			skybox->bind();

			skyboxShader = createShader(vertexShader, fragShader);
			glUseProgram(skyboxShader);
		}




		{
			std::string vertexShader;
			std::string fragShader;
			readShader(basePath + "basic.shader", vertexShader, fragShader);
			Logger::Log("vertexShader: \n" + vertexShader, LogLevel::TRACE);
			Logger::Log("fragShader: \n" + fragShader, LogLevel::TRACE);

			glActiveTexture(GL_TEXTURE0 + 1);
			texture = new Texture(basePath + "grenuk.PNG");
			texture->bind();

			glActiveTexture(GL_TEXTURE0 + 2);
			texture2 = new Texture(basePath + "grenuk2.PNG");
			texture2->bind();

			shader = createShader(vertexShader, fragShader);
			glUseProgram(shader);

			location1 = glGetUniformLocation(shader, "u_Color");
			glUniform4f(location1, 1.0f, t, t, 1.0f);


			view = glm::mat4(1.0);
			view = glm::translate(view, glm::vec3(0.0, 0.0, 0.5));
			view = glm::inverse(view);
			location2 = glGetUniformLocation(shader, "view");
			glUniformMatrix4fv(location2, 1, GL_FALSE, &view[0].x);

			proj = glm::mat4(1.0);
			location3 = glGetUniformLocation(shader, "proj");
			glUniformMatrix4fv(location3, 1, GL_FALSE, &proj[0].x);

			model = glm::mat4(1.0);
			location4 = glGetUniformLocation(shader, "model");
			glUniformMatrix4fv(location4, 1, GL_FALSE, &model[0].x);

			location5 = glGetUniformLocation(shader, "u_Tex");
			glUniform1i(location5, texture->getHandle());

			location6 = glGetUniformLocation(shader, "u_iResolution");
			glUniform3f(location6, 1280.0f, 720.0f,1.0f); /////


			//cameraPosition = scene->camera_->getPosition();
			glUniform3fv(glGetUniformLocation(shader, "cameraPos"), 1, glm::value_ptr(cameraPosition));

			glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
			eye_dir = glm::normalize(cameraTarget - cameraPosition);
			glUniform3fv(glGetUniformLocation(shader, "eye_dir"), 1, glm::value_ptr(eye_dir));


			
		}




		//IMGUI_CHECKVERSION();
		//ImGui::CreateContext();
		//ImGuiIO& io = ImGui::GetIO();
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		// Setup Platform/Renderer backends
		//ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
		//ImGui_ImplOpenGL3_Init();
		//glfwPollEvents();
		//glCullFace(GL_BACK);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		//generateCube(0,-worldLength / 2, worldHeight / 2 - 2, -50, 1.0, 1.0, 1.0);
		//generateCube(1,-worldLength / 2, worldHeight / 2 - 1, -50, 1.0, 1.0, 1.0);
		//generateCube(2,-worldLength / 2, worldHeight / 2, -50, 1.0, 1.0, 1.0);
		//generateCube(3,-worldLength / 2, worldHeight / 2 + 1, -50, 1.0, 1.0, 1.0);
		//generateCube(4,-worldLength / 2, worldHeight / 2 + 2, -50, 1.0, 1.0, 1.0);

		//generateCube(5,worldLength / 2, worldHeight / 2 - 2, -50, 1.0, 1.0, 1.0);
		//generateCube(6,worldLength / 2, worldHeight / 2 - 1, -50, 1.0, 1.0, 1.0);
		//generateCube(7,worldLength / 2, worldHeight / 2, -50, 1.0, 1.0, 1.0);
		//generateCube(8,worldLength / 2, worldHeight / 2 + 1, -50, 1.0, 1.0, 1.0);
		//generateCube(9,worldLength / 2, worldHeight / 2 + 2, -50, 1.0, 1.0, 1.0);

		//generateCube(10, 0.0, worldHeight / 2 + 2, -50, 1.0, 1.0, 1.0);

		//generateRectByCenter(11, 0.0, worldHeight/2.0, -51.0, worldLength, worldHeight);

		
		//distribution = std::normal_distribution<double>(0.0, 1.0);

		/*for (int ii = 0; ii < maxX; ii++)
		{
			for (int jj = 0; jj < maxY; jj++)
			{
				generateCube(ii + 11, ii, distribution(generator), jj, std::remainder((double)ii , divisor), std::remainder((double)jj, divisor), divisor - std::max(std::remainder(ii, divisor), std::remainder(jj, divisor)));
			}
		}*/


		for (int ii = 0; ii < 24; ii ++)
		{
			vertexData.push_back(quadPositions[ii]);
		}

		for (int ii = 0; ii < 6; ii++)
		{
			indexData.push_back(quadIndices[ii]);
		}
		

		glBindBuffer(GL_ARRAY_BUFFER, vbuf);
		glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), &vertexData[0], GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexData.size() * sizeof(unsigned int), &indexData[0], GL_DYNAMIC_DRAW);

	}

	bool RenderingSystem::update(std::shared_ptr<Scene> scene, const TimeStep& dt)
	{
		//scene->camera_->addRotation(0.0f, 0.01f);
		view = scene->camera_->getView();
		proj = scene->camera_->getProjection();

		cameraPosition = scene->camera_->getPosition();
		glUniform3fv(glGetUniformLocation(shader, "cameraPos"), 1, glm::value_ptr(cameraPosition)); //

		// Get all entities with a transform, a model, and a texture component
		// These are non-voxel things

		// Get all entities with a transform, and a texture component
		// These are voxels

		t = t + 0.001f;
		t = fmod(t, 1.0);
		if (!glfwWindowShouldClose(soloWinCopy) && initialized)
		{
			glfwGetFramebufferSize(soloWinCopy, &width, &height);
			glViewport(0, 0, width, height);

			/*ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			ImGui::ShowDemoWindow();*/

			glClearColor(1.0f, 0.0f, .5f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			//model = glm::rotate(model, dt.GetSeconds(), glm::vec3(0, 1, 0));
			//model = glm::rotate(model, dt.GetSeconds(), glm::vec3(0, 0, 1));

			
			paddleDirection = 0;
			if(InputManager::IsKeyPressedOrHeld(Key::E))
			{
				paddleDirection += 1; // up
			}

			if (InputManager::IsKeyPressedOrHeld(Key::Q))
			{
				paddleDirection += -1; // down
			}
	

			generator.seed(distribution(generator));

			/*for (int ii = 0; ii < maxX; ii++)
			{
				for (int jj = 0; jj < maxY; jj++)
				{
					moveCube(ii + 11, 0.0, -1.0, 0.0, std::remainder((double)ii, divisor), std::remainder((double)jj, divisor), divisor - std::max(std::remainder(ii, divisor), std::remainder(jj, divisor)));
				}
			}*/



			//if (paddleDirection != 0.0)
			//{
			//	moveCube(0, 0.000f, dt.GetSeconds() * paddleSpeed * paddleDirection, 0.0, 0.0, 0.0, 0.0);
			//	moveCube(1, 0.000f, dt.GetSeconds() * paddleSpeed * paddleDirection, 0.0, 0.0, 0.0, 0.0);
			//	moveCube(2, 0.000f, dt.GetSeconds() * paddleSpeed * paddleDirection, 0.0, 0.0, 0.0, 0.0);
			//	moveCube(3, 0.000f, dt.GetSeconds() * paddleSpeed * paddleDirection, 0.0, 0.0, 0.0, 0.0);
			//	moveCube(4, 0.000f, dt.GetSeconds() * paddleSpeed * paddleDirection, 0.0, 0.0, 0.0, 0.0);
			//}
			//if (glm::length(ballVelocity) > 1e3)
			//{
			//	moveCube(10, ballVelocity.x / 1e12, ballVelocity.y / 1e12, ballVelocity.z / 1e12, 0.0f, 0.0f, 1.0);
			//}
			//// Check collision with middle of paddle
			//if (checkCubeCollide(10, 7) || checkCubeCollide(10, 2) )
			//{
			//	ballVelocity.x = -1.5*ballVelocity.x;
			//	ballVelocity.y = -1.5*ballVelocity.y;
			//	ballVelocity.z = -ballVelocity.z;
			//} // Top of paddle
			//else if (checkCubeCollide(10, 5) || checkCubeCollide(10, 6)  || checkCubeCollide(10, 0) || checkCubeCollide(10, 1))
			//{
			//	ballVelocity.x = -0.9 * ballVelocity.x;
			//	ballVelocity.y =  1.25*ballVelocity.y;
			//	ballVelocity.z = -ballVelocity.z;
			//} // Bottom of paddle
			//else if (checkCubeCollide(10, 8) || checkCubeCollide(10, 9) || checkCubeCollide(10, 3) || checkCubeCollide(10, 4))
			//{
			//	ballVelocity.x = -0.9*ballVelocity.x;
			//	ballVelocity.y = 1.25*ballVelocity.y;
			//	ballVelocity.z = -ballVelocity.z;
			//}
			//else if (checkCubeWallCollide(10))
			//{
			//	ballVelocity.x = 1.05 * ballVelocity.x;
			//	ballVelocity.y = -1.05 * ballVelocity.y;
			//	ballVelocity.z = -ballVelocity.z;
			//}
			//ballVelocity.x = 0.99997 * ballVelocity.x;
			//ballVelocity.y = 0.99997 * ballVelocity.y;
			//if (glm::length(ballVelocity) > 1e3)
			//{
			//	// Move right paddle to align with ball y coord
			//	moveCube(5, 0.000f, ballVelocity.y / 1e12, 0.0, 0.0, 0.0, 0.0);
			//	moveCube(6, 0.000f, ballVelocity.y / 1e12, 0.0, 0.0, 0.0, 0.0);
			//	moveCube(7, 0.000f, ballVelocity.y / 1e12, 0.0, 0.0, 0.0, 0.0);
			//	moveCube(8, 0.000f, ballVelocity.y / 1e12, 0.0, 0.0, 0.0, 0.0);
			//	moveCube(9, 0.000f, ballVelocity.y / 1e12, 0.0, 0.0, 0.0, 0.0);
			//}
			//
			glBindBuffer(GL_ARRAY_BUFFER, vbuf);
			glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), &vertexData[0], GL_DYNAMIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexData.size() * sizeof(unsigned int), &indexData[0], GL_DYNAMIC_DRAW);

			glUseProgram(shader);
			glBindVertexArray(modelVao);
			glUniformMatrix4fv(location4, 1, GL_FALSE, &model[0].x);
			glUniformMatrix4fv(location2, 1, GL_FALSE, &view[0].x);

			if (( oldwidth != width || oldheight != height ) && width > 1e-5 && height > 1e-5)
			{
				scene->camera_->setProjectionPerspective(60.0f, (float)width / (float)height, 0.1f, 10000.0f);
				glUniformMatrix4fv(location3, 1, GL_FALSE, &scene->camera_->getProjection()[0].x);
				oldwidth = width;
				oldheight = height;
				glfwPollEvents();
			}
			


			glUniform4f(location1, 1.0f, t, 1.0f-t, 1.0f);

			glUniform1i(location5, texture->getHandle());
			glActiveTexture(GL_TEXTURE0 + 1); 
			glBindTexture(GL_TEXTURE_2D, texture->getHandle());
			//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
			//a;lkasd;lfjk
			//model = glm::translate(model, glm::vec3(0, 1.1, 0));

			//glUniformMatrix4fv(location4, 1, GL_FALSE, &model[0].x);
			//glUniform1i(location5, texture2->getHandle());
			//glActiveTexture(GL_TEXTURE0 + 2); 
			//glBindTexture(GL_TEXTURE_2D, texture2->getHandle());
			//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
			//model = glm::translate(model, glm::vec3(0, -1.1, 0));
			//
			{ // Draw all voxels in the voxel array

				//

				glUniformMatrix4fv(location4, 1, GL_FALSE, &model[0].x);
				glDrawElements(GL_TRIANGLES, indexData.size(), GL_UNSIGNED_INT, nullptr);
			}

			{ // Skybox here
				//glDepthMask(GL_FALSE);
				//glUseProgram(skyboxShader);

				//glBindVertexArray(skyboxVao);
				////view = glm::mat4(glm::mat3(view));
				////proj = glm::perspective(glm::radians(10.0f), (float)width / (float)height, 0.1f, 10.1f);
				//glUniformMatrix4fv(glGetUniformLocation(skyboxShader, "view"), 1, GL_FALSE, glm::value_ptr(glm::mat4(glm::mat3(view))));
				//glUniformMatrix4fv(glGetUniformLocation(skyboxShader, "projection"), 1, GL_FALSE, glm::value_ptr(proj));
				//glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->getHandle());
				//glDrawArrays(GL_TRIANGLES, 0, 36);

				//glDepthMask(GL_TRUE);
				
			}
			////

			/*ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());*/
			glfwSwapBuffers(soloWinCopy);
			glfwPollEvents();
			return true;
		}
		else if(initialized) {
			/*ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();*/
			//glDeleteProgram(shaderProgram);
			glfwTerminate();
			initialized = false;
			return false;
		}

	}



	void RenderingSystem::generateCube(double objIdx, double wposx, double wposy, double wposz, double red, double green, double blue)
	{
		double startIndex = (vertexData.size() / 6);
		if(startIndex > 1e15)
		{
			startIndex = 0;
		}
		// Generate Vertices for Cube
		for (int ip = 0; ip < 48; ip++)
		{
			int res = ip % 6;
			if (res == 0)
			{
				vertexData.push_back(positions[ip] + wposx);
			}
			else if (res == 1)
			{
				vertexData.push_back(positions[ip] + wposy);
			}
			else if (res == 2)
			{
				vertexData.push_back(positions[ip] + wposz);
			}
			else if (res == 3)
			{
				vertexData.push_back(red);
			}
			else if (res == 4)
			{
				vertexData.push_back(green);
			}
			else if (res == 5)
			{
				vertexData.push_back(blue);
			}
			objectVertexIndex.push_back(objIdx);
		}

		for (int ii = 0; ii < 36; ii++)
		{
			indexData.push_back(indices[ii] + startIndex);
			objectIndex.push_back(objIdx);
		}
	}

	void RenderingSystem::moveCube(double idx, double dx, double dy, double dz, double r, double g, double b)
	{
		// This is garbage for large vertexData lists lmao don't do this
		int numVinV = 6;
		int numVInCube = 36* numVinV;
		int mindex = idx * numVInCube;
		int maxdex = (idx * numVInCube + numVInCube);
		for (int vertId = mindex; vertId + numVinV-1 < maxdex; vertId += numVinV) // 5 is width of rows in vertex, 
		{
			if (objectVertexIndex.at(vertId) == idx)
			{
				vertexData.at(vertId) += dx;
				vertexData.at(vertId + 1) += dy;
				vertexData.at(vertId + 2) += dz;
				vertexData.at(vertId + 3) = r;
				vertexData.at(vertId + 4) = g;
				vertexData.at(vertId + 5) = b;
			}
		}

	}

	bool RenderingSystem::checkCubeCollide(double idxa, double idxb)
	{
		return false;
		glm::vec3 vertexASum = { 0.0,0.0,0.0 };
		glm::vec3 vertexBSum = { 0.0,0.0,0.0 };
		double vertACount = 0.0;
		double vertBCount = 0.0;
		// This is garbage for large vertexData lists lmao don't do this
		// OOPSie
		int numVinV = 6;
		int numVInCube = 36* numVinV;
		int stidx = std::min(idxa, idxb)* numVInCube;
		int endx = std::max(idxa, idxb) * numVInCube + numVInCube;
		for (int vertId = stidx; vertId + 2 < endx; vertId += numVinV) // 5 is width of rows in vertex, 
		{
			if (objectVertexIndex.at(vertId) == idxa)
			{
				vertexASum = vertexASum + glm::vec3(vertexData.at(vertId), vertexData.at(vertId + 1), vertexData.at(vertId + 2) );
				vertACount++;
			}

			if (objectVertexIndex.at(vertId) == idxb)
			{
				vertexBSum = vertexBSum + glm::vec3(vertexData.at(vertId), vertexData.at(vertId + 1), vertexData.at(vertId + 2));
				vertBCount++;
			}
		}

		glm::vec3 vertexAAvg = { vertexASum.x/ vertACount, vertexASum.y / vertACount, vertexASum.z / vertACount, };
		glm::vec3 vertexBAvg = { vertexBSum.x / vertBCount, vertexBSum.y / vertBCount, vertexBSum.z / vertBCount, };

		glm::vec3 vertDiff = vertexAAvg - vertexBAvg;
		double distance = glm::length(vertDiff);

		if (distance < 1.0) // Assume cubes each have 'radius' of 0.5 not a valid assumption but whatever lol
		{
			return true;
		}
		else
		{
			return false;
		}

	}

	bool RenderingSystem::checkCubeWallCollide(double idx)
	{
		return false;
		bool res = false;
		// This is garbage for large vertexData lists lmao don't do this
		for (int vertId = 0; vertId + 2 < vertexData.size(); vertId += 6) // 5 is width of rows in vertex, 
		{
			if (objectVertexIndex.at(vertId) == idx)
			{
				res = res || (vertexData.at(vertId + 1) < 0.0) || (vertexData.at(vertId+1) > worldHeight);
			}
		}

		return res;
	}

	void RenderingSystem::generateRectByCenter(double objIdx, double wposx, double wposy, double wposz, double width, double height)
	{
		double startIndex = (vertexData.size() / 6);
		if (startIndex > 1e15)
		{
			startIndex = 0;
		}
		// Generate Vertices for Cube
		for (int ip = 0; ip < 48; ip++)
		{
			int res = ip % 6;
			if (res == 0)
			{
				vertexData.push_back(positions[ip]*width + wposx);
			}
			else if (res == 1)
			{
				vertexData.push_back(positions[ip]*height + wposy);
			}
			else if (res == 2)
			{
				vertexData.push_back(positions[ip] + wposz);
			}
			else
			{
				vertexData.push_back(0.5);
			}
			objectVertexIndex.push_back(objIdx);
		}

		for (int ii = 0; ii < 36; ii++)
		{
			indexData.push_back(indices[ii] + startIndex);
			objectIndex.push_back(objIdx);
		}

	}

	void RenderingSystem::destroy(std::shared_ptr<Scene> scene)
	{

	}

} // namespace Solo