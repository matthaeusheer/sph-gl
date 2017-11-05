#include <glad/glad.h>

#include <AntTweakBar.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>

#include "Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "std_image.h"

#include "camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <math.h>
#include <iostream>

#include "csvReader.h"

// files to load
int startIdx = 50;
int endIdx   = 100;
int stepSize = 50;

// function declarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int createTexture();

// screen settings
const unsigned int SCR_WIDTH = 1700;
const unsigned int SCR_HEIGHT = 800;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 20.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
float pointSize = 1.0f;

// timing defaults
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// -----------------------
// loading data

std::vector<std::string> outSteps = arange(startIdx, endIdx, stepSize);
std::vector<std::vector<float>> allData = loadAllFiles(outSteps);
unsigned int datIdx = 0;
std::string currentOutputStep = outSteps[datIdx];
bool animationRunning = false;

int n_vertices = allData[datIdx].size() / 8;

struct buffers {
	unsigned int* VAO;
	unsigned int* VBO;
};
buffers initBuffer();

int main()
{
	// -----------------------

	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	// glfw window creation
	//--------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "GasoViz", nullptr, nullptr);
	// set monitor to glfwGetPrimaryMonitor() for full screendw
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Initialize AntTweakBar
	TwInit(TW_OPENGL_CORE, NULL);

	// Create a tweak bar
	TwBar *bar;
	TwWindowSize(SCR_WIDTH, SCR_HEIGHT - 100);
	bar = TwNewBar("GasoViz Settings");
	//TwAddVarRW(bar, "Out step", TW_TYPE_STDSTRING, &currentOutputStep, "");

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	std::cout << "GL_VERSION >>> " << glGetString(GL_VERSION) << std::endl;
	
	glEnable(GL_DEPTH_TEST);
	
	// Creating a shader program
	Shader ourShader("Shaders/shader.vs", "Shaders/shader.fs");

	buffers buff = initBuffer();
	unsigned int* VAOp = buff.VAO;
	unsigned int* VBOp = buff.VBO;

	glEnable(GL_PROGRAM_POINT_SIZE);
	// render loop
	// -----------
	int numberOfRenders = 0;
	float lastTimeValue = 0.0;
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		numberOfRenders += 1;
		// input
		processInput(window);
		float timeValue = glfwGetTime();
		deltaTime = timeValue - lastFrame;
		lastFrame = timeValue;

		// background color buffer reset
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ourShader.use();

		int somevar = 1;

		// transform into world coordiates
		glm::mat4 model;
		ourShader.setMat4("model", model);

		// camera/view transformation
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("view", view);

		glm::mat4 trans;
		ourShader.setMat4("transform", trans);

		// projection matrix
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.01f, 100.0f);
		ourShader.setMat4("projection", projection);

		ourShader.setFloat("pointSize", pointSize);

		// activate our shader
		glBindVertexArray(*VAOp);
		glDrawArrays(GL_POINTS, 0, n_vertices);

		if (numberOfRenders % 1000 == 0) {
			std::cout << "fps :" << 1000 / (timeValue - lastTimeValue) << std::endl;
			lastTimeValue = timeValue;
		}
		std::cout << datIdx << std::endl;
		
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		// Draw tweak bars
		//TwDraw(); Doesnt work right now, cannot update buffer if tweakbar is being drawn
		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, VAOp);
	glDeleteBuffers(1, VBOp);
	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	TwTerminate();
	glfwTerminate();
	return 0;
}

buffers initBuffer() {
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

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

void updateBufferOnly() {
	std::cout << "updating buffer" << std::endl;
	glBufferSubData(GL_ARRAY_BUFFER, 0, allData[datIdx].size() * sizeof(float), allData[datIdx].data());
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		camera.resetCamera();

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		if (datIdx > 0) {
			datIdx--;
			updateBufferOnly();
		}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		if ((datIdx < (allData.size() - 1))) {
			datIdx++;
			updateBufferOnly();
		}
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		if ((pointSize > 1.0f))
			pointSize -= 0.1f;

	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		if (pointSize < 7.0f)
			pointSize += 0.1f;

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
		animationRunning = !animationRunning;
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{	

		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

		lastX = xpos;
		lastY = ypos;

		camera.ProcessMouseMovement(xoffset, yoffset);
	

}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
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
