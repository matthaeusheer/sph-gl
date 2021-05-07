#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "STDIMAGE/std_image.h"

#include <math.h>
#include <iostream>
#include <assert.h>
#include <fstream>

#include "Shader.h"
#include "camera.h"
#include "window.h"
#include "datamanager.h"
#include "filesize.h"

#include "buffers/buffer.h"
#include "buffers/indexbuffer.h"
#include "buffers/vertexarray.h"


int main()
{
	// ---------------------------
	// Data management
	int startIdx = 50;
	int endIdx   = 200;
	int stepSize = 50;

	//std::vector<std::string> outSteps = arange(startIdx, endIdx, stepSize);
	//DataManager fileReader(outSteps);
	//fileReader.loadAllFiles();

	//size_t n_timesteps = fileReader.getNTimesteps();
	size_t n_timesteps = 3;
	size_t n_vertices = 1;
	//size_t n_vertices = fileReader.getNumberOfParticles();

    std::vector<float> m_Position = {-0.531881,-0.549267,-1.75193e-06};
    std::vector<float> m_Velocity = {3.43858,0.161529,-0.000335416,0.000654889};
    std::vector<float> m_Density = {16.4774};

	// ---------------------------
	// Window initialization
	Window window("sphGL", 1200, 700);
	Camera camera(glm::vec3(0.0f, 0.0f, 20.0f));
	window.attachCamera(camera);

	unsigned int timestep = 0;
	//std::string currentOutputStep = outSteps[timestep];

	// ---------------------------
	// Initialize buffers & shader program
	VertexArray VAO;
	//Buffer* VBO_pos = new Buffer(fileReader.m_Position.data(), fileReader.getPosSize(), 3);
	Buffer* VBO_pos = new Buffer(m_Position.data(), 3, 3);
	//Buffer* VBO_dens = new Buffer(fileReader.m_Density.data(), fileReader.getDensSize(), 1);
	Buffer* VBO_dens = new Buffer(m_Density.data(), 1, 1);

	VAO.addBuffer(VBO_pos, 0);
	VAO.addBuffer(VBO_dens, 2);

	// Creating a shader program
	Shader shaderProgram("../src/shaders/shader.vs", "../src/shaders/shader.fs");
	shaderProgram.enable();

	float pointSize = 1.0f;

	// ---------------------------
	// TW section
	float fps = 0.0f;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_PROGRAM_POINT_SIZE);

	int numberOfRenders = 0;
	float timeValue = 0.0f;
	float lastTimeValue = 0.0f;
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	// ---------------------------
	// main rendering loop
	while (!window.closed())
	{
		timeValue = glfwGetTime();
		deltaTime = timeValue - lastFrame;
		lastFrame = timeValue;

		glfwPollEvents();
		window.processInput(deltaTime, timestep, n_timesteps - 1, pointSize);
		window.clear();
		
		glm::mat4 model, trans;
		shaderProgram.setMat4("model", model);
		shaderProgram.setMat4("transform", trans);

		glm::mat4 view = camera.GetViewMatrix();
		shaderProgram.setMat4("view", view);

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)window.getWidth() / (float)window.getHeight(), 0.01f, 100.0f);
		shaderProgram.setMat4("projection", projection);

		window.updatePointSize(pointSize);
		shaderProgram.setFloat("pointSize", pointSize);

		unsigned int timestepDirection = window.queryBufferUpdate();

		if (timestepDirection != Window::timeStepDirection::NOUPDATE)
		{
			if (timestepDirection == Window::timeStepDirection::BACKSTEP && timestep > 0) {
				timestep--;
			}
			//else if (timestepDirection == Window::timeStepDirection::FRONTSTEP && timestep < fileReader.getNTimesteps() - 1) {
			//	timestep++;
			//}

			//VBO_pos->updateBuffer(fileReader.getPosSize() / fileReader.getNTimesteps(), fileReader.getPositionP(timestep));
			//VBO_dens->updateBuffer(fileReader.getDensSize() / fileReader.getNTimesteps(), fileReader.getDensityP(timestep));
		}

		
		//-------------
		// draw call
		VAO.bind();
		glDrawArrays(GL_POINTS, 0, n_vertices);
		VAO.unbind();

		if (numberOfRenders % 1000 == 0) {
			fps = 1000 / (timeValue - lastTimeValue);
			lastTimeValue = timeValue;
		}

		numberOfRenders += 1;

		window.update();
	}

	VAO.~VertexArray();
	glfwTerminate();

	return 0;
}

// TODO: outsource this function to where it belongs
unsigned int createTexture() {
	//
	unsigned int texture; // also multiple textures possible, then create list and pass to glGenTextures
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// set tex wrapping options (s, t, r) -> (x, y, z)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set tex filterung options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // when making image smaller
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // when largening image

																	  // load image and match on texture
	int width, height, nrChannels;
	unsigned char *img_data = stbi_load("Textures/container.jpg", &width, &height, &nrChannels, 0);
	if (img_data) { // valid c++ pointers are implicitely converted to boolean true
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img_data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load image." << std::endl;
	}
	stbi_image_free(img_data); // free image data

	return texture;
}
