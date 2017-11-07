#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "StdImage/std_image.h"

#include <math.h>
#include <iostream>
#include <assert.h>

#include "src/graphics/Shader.h"
#include "src/graphics/camera.h"
#include "src/graphics/window.h"
#include "src/utils/csvReader.h"


struct buffers {
	unsigned int* VAO;
	unsigned int* VBO;
};


buffers initBuffer(const std::vector<std::vector<float>>& allData, unsigned int datIdx);
void updateBufferOnly(unsigned int bufferUpdate, const std::vector<std::vector<float>>& allData, unsigned int& datIdx, int n_timesteps);

int main()
{
	// ---------------------------
	// files to load
	int startIdx = 50;
	int endIdx   = 200;
	int stepSize = 50;

	
	std::vector<std::string> outSteps = arange(startIdx, endIdx, stepSize);
	std::vector<std::vector<float>> allData = loadAllFiles( outSteps);
	
	unsigned int datIdx = 0;
	std::string currentOutputStep = outSteps[datIdx];
	
	int n_timesteps = allData.size();
	int n_vertices = allData[datIdx].size() / 8;
	float pointSize = 1.0f;

	// ---------------------------

	Window window("sphGL", 1200, 700);
	Camera camera(glm::vec3(0.0f, 0.0f, 20.0f));
	window.attachCamera(camera);

	// ---------------------------
	// TW section
	float fps = 0.0f;

	// Creating a shader program
	Shader shaderProgram("src/shaders/shader.vs", "src/shaders/shader.fs");
	shaderProgram.enable();

	buffers buff = initBuffer(allData, datIdx);
	unsigned int* VAOp = buff.VAO;
	unsigned int* VBOp = buff.VBO;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_PROGRAM_POINT_SIZE);

	int numberOfRenders = 0;
	float timeValue = 0.0f;
	float lastTimeValue = 0.0f;
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	//TwAddVarRW(bar, "n renders", TW_TYPE_STDSTRING, &numberOfRenders, "");
	//TwDraw();


	while (!window.closed())
	{
		timeValue = glfwGetTime();
		deltaTime = timeValue - lastFrame;
		lastFrame = timeValue;
		glfwPollEvents();
		window.processInput(deltaTime, datIdx, n_timesteps - 1, pointSize);
		window.clear();
		
		// transform into world coordiates
		glm::mat4 model, trans;
		shaderProgram.setMat4("model", model);
		shaderProgram.setMat4("transform", trans);

		glm::mat4 view = camera.GetViewMatrix();
		shaderProgram.setMat4("view", view);

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)window.getWidth() / (float)window.getHeight(), 0.01f, 100.0f);
		shaderProgram.setMat4("projection", projection);

		window.updatePointSize(pointSize);
		shaderProgram.setFloat("pointSize", pointSize);

		shaderProgram.enable();
		glBindVertexArray(*VAOp);

		unsigned int bufferUpdate = window.queryBufferUpdate();
		//std::cout << bufferUpdate << std::endl;
		updateBufferOnly(bufferUpdate, allData, datIdx, n_timesteps);
		
		//-------------
		// draw call
		glDrawArrays(GL_POINTS, 0, n_vertices);

		if (window.isMouseButtonClicked(GLFW_MOUSE_BUTTON_LEFT)) {
			double x, y;
			window.getMousePosition(x, y);
			std::cout << "Mouse clicked at pixel location: " << x << ", " << y << std::endl;
		}

		if (numberOfRenders % 1000 == 0) {
			fps = 1000 / (timeValue - lastTimeValue);
			lastTimeValue = timeValue;
		}

		numberOfRenders += 1;

		window.update();
	}
	glDeleteVertexArrays(1, VAOp);
	glDeleteBuffers(1, VBOp);
	
	glfwTerminate();
	return 0;
}

buffers initBuffer(const std::vector<std::vector<float>>& allData, unsigned int datIdx) {
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, allData[datIdx].size() * sizeof(float), allData[datIdx].data(), GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//density attribute
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(7 * sizeof(float)));
	glEnableVertexAttribArray(1);
	buffers buff;
	buff.VAO = &VAO;
	buff.VBO = &VBO;

	return buff;
}

void updateBufferOnly(unsigned int bufferUpdate, const std::vector<std::vector<float>>& allData, unsigned int& datIdx, int n_timesteps) {
	// bufferUpdate: 1 = BACKSTEP, 2 = FRONTSTEP

	if (bufferUpdate == 1 && datIdx > 0) {
		datIdx--;
		std::cout << "timestep " << datIdx << std::endl;
		glBufferSubData(GL_ARRAY_BUFFER, 0, allData[datIdx].size() * sizeof(float), allData[datIdx].data());
	}
	else if (bufferUpdate == 2 && datIdx < n_timesteps - 1) {
		datIdx++;
		std::cout << "timestep " << datIdx << std::endl;
		glBufferSubData(GL_ARRAY_BUFFER, 0, allData[datIdx].size() * sizeof(float), allData[datIdx].data());
	}
}


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
