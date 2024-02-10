#include "RenderingSystem.h"




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

		{


			glGenVertexArrays(1, &modelVao);
			glBindVertexArray(modelVao);


			
			glGenBuffers(1, &vbuf);
			glBindBuffer(GL_ARRAY_BUFFER, vbuf);
			glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_DYNAMIC_DRAW);


			
			glGenBuffers(1, &ibo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

			// Specify layout of buffer
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)0);
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)(sizeof(float) * 3));
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

			location = glGetUniformLocation(shader, "u_Color");
			glUniform4f(location, 1.0, t, t, 1.0f);


			view = glm::mat4(1.0);
			view = glm::translate(view, glm::vec3(0.0, 0.0, 0.5));
			view = glm::inverse(view);
			location2 = glGetUniformLocation(shader, "u_View");
			glUniformMatrix4fv(location2, 1, GL_FALSE, &view[0].x);

			proj = glm::mat4(1.0);
			location3 = glGetUniformLocation(shader, "u_Proj");
			glUniformMatrix4fv(location3, 1, GL_FALSE, &proj[0].x);

			model = glm::mat4(1.0);
			location4 = glGetUniformLocation(shader, "u_Model");
			glUniformMatrix4fv(location4, 1, GL_FALSE, &model[0].x);

			location5 = glGetUniformLocation(shader, "u_Tex");
			glUniform1i(location5, texture->getHandle());
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





		int count = 0;
		for (int ix = 0; ix < worldLength; ix++)
		{
			for (int iy = 0; iy < worldHeight; iy++)
			{
				for(int iz = 0; iz < worldWidth; iz++)
				{
					voxels.push_back({ ix,iy,iz });
					for (int ip = 0; ip < 40; ip++)
					{
						int res = ip % 5;
						if (res == 0)
						{
							vertexData.push_back(positions[ip] + ix);
						}
						else if (res == 1)
						{
							vertexData.push_back(positions[ip] + iy);
						}
						else if (res == 2)
						{
							vertexData.push_back(positions[ip] + iz);
						}
						else
						{
							vertexData.push_back(positions[ip]);
						}
						
					}
					
					for (int ii = 0; ii < 36; ii++)
					{
						indexData.push_back(indices[ii] + count*8);
					}
					count++;
					
				}
			}
		}


		glBindBuffer(GL_ARRAY_BUFFER, vbuf);
		glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), &vertexData[0], GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexData.size() * sizeof(unsigned int), &indexData[0], GL_DYNAMIC_DRAW);

	}

	bool RenderingSystem::update(std::shared_ptr<Scene> scene)
	{
		//scene->camera_->addRotation(0.0f, 0.01f);
		view = scene->camera_->getView();
		proj = scene->camera_->getProjection();
		
		
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

			glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			//model = glm::rotate(model, 0.0001f, glm::vec3(0, 1, 0));
			//model = glm::rotate(model, 0.0001f, glm::vec3(0, 0, 1));

			glUseProgram(shader);
			glBindVertexArray(modelVao);
			glUniformMatrix4fv(location4, 1, GL_FALSE, &model[0].x);
			glUniformMatrix4fv(location2, 1, GL_FALSE, &view[0].x);

			if (( oldwidth != width || oldheight != height ) && width > 1e-5 && height > 1e-5)
			{
				scene->camera_->setProjectionPerspective(60.0f, (float)width / (float)height, 0.1f, 1000.0f);
				glUniformMatrix4fv(location3, 1, GL_FALSE, &scene->camera_->getProjection()[0].x);
				oldwidth = width;
				oldheight = height;
				glfwPollEvents();
			}
			


			glUniform4f(location, 1.0f, t, 1.0f-t, 1.0f);

			glUniform1i(location5, texture->getHandle());
			glActiveTexture(GL_TEXTURE0 + 1); 
			glBindTexture(GL_TEXTURE_2D, texture->getHandle());
			//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
			
			//model = glm::translate(model, glm::vec3(0, 1.1, 0));

			//glUniformMatrix4fv(location4, 1, GL_FALSE, &model[0].x);
			//glUniform1i(location5, texture2->getHandle());
			//glActiveTexture(GL_TEXTURE0 + 2); 
			//glBindTexture(GL_TEXTURE_2D, texture2->getHandle());
			//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
			//model = glm::translate(model, glm::vec3(0, -1.1, 0));
			
			{ // Draw all voxels in the voxel array



				glUniformMatrix4fv(location4, 1, GL_FALSE, &model[0].x);
				glDrawElements(GL_TRIANGLES, indexData.size(), GL_UNSIGNED_INT, nullptr);
			}

			{ // Skybox here
				glDepthMask(GL_FALSE);
				glUseProgram(skyboxShader);

				glBindVertexArray(skyboxVao);
				//view = glm::mat4(glm::mat3(view));
				//proj = glm::perspective(glm::radians(10.0f), (float)width / (float)height, 0.1f, 10.1f);
				glUniformMatrix4fv(glGetUniformLocation(skyboxShader, "view"), 1, GL_FALSE, glm::value_ptr(glm::mat4(glm::mat3(view))));
				glUniformMatrix4fv(glGetUniformLocation(skyboxShader, "projection"), 1, GL_FALSE, glm::value_ptr(proj));
				glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->getHandle());
				glDrawArrays(GL_TRIANGLES, 0, 36);

				glDepthMask(GL_TRUE);
			}

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

	void RenderingSystem::destroy(std::shared_ptr<Scene> scene)
	{

	}

} // namespace Solo