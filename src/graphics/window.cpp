#include "window.h"

Window::Window(const char *title, int width, int height)
{
	m_Title = title;
	m_Width = width;
	m_Height = height;
	if (!init())
		glfwTerminate();

	for (int i = 0; i < MAX_KEYS; i++)
	{
		m_Keys[i] = false;
		m_KeyState[i] = false;
		m_KeyTyped[i] = false;
	}

	for (int i = 0; i < MAX_BUTTONS; i++)
	{
		m_MouseButtons[i] = false;
		m_MouseState[i] = false;
		m_MouseClicked[i] = false;
	}
}

Window::~Window()
{
	glfwTerminate();
}

bool Window::init()
{
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW!" << std::endl;
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_Window = glfwCreateWindow(m_Width, m_Height, m_Title, NULL, NULL);
	
	if (!m_Window)
	{
		std::cout << "Failed to create GLFW window!" << std::endl;
		return false;
	}

	glfwMakeContextCurrent(m_Window);
	glfwSetWindowUserPointer(m_Window, this);
	glfwSetFramebufferSizeCallback(m_Window, window_resize);
	glfwSetKeyCallback(m_Window, key_callback);
	glfwSetMouseButtonCallback(m_Window, mouse_button_callback);
	glfwSetCursorPosCallback(m_Window, cursor_position_callback);
	glfwSetScrollCallback(m_Window, scroll_callback);
	glfwSetCursorPos(m_Window, m_Width / 2, m_Height / 2);
	glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//glfwSwapInterval(0.0);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl;

	float last_mx = m_Width / 2.0f;
	float last_my = m_Height / 2.0f;

	return true;
}

void Window::clear() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::update()
{
	for (int i = 0; i < MAX_KEYS; i++)
		m_KeyTyped[i] = m_Keys[i] && !m_KeyState[i];

	for (int i = 0; i < MAX_BUTTONS; i++)
		m_MouseClicked[i] = m_MouseButtons[i] && !m_MouseState[i];

	memcpy(m_KeyState, m_Keys, MAX_KEYS);
	memcpy(m_MouseState, m_MouseButtons, MAX_BUTTONS);

	/*
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
		std::cout << "OpenGL Error: " << error << std::endl;
	*/


	glfwSwapBuffers(m_Window);
}

void Window::attachCamera(Camera& camera)
{
	m_Camera = &camera;
}

// outside class functions

bool Window::closed() const
{
	return glfwWindowShouldClose(m_Window) == 1;
}

bool Window::isKeyPressed(unsigned int keycode) const
{
	// TODO: Log this!
	if (keycode >= MAX_KEYS)
		return false;

	return m_Keys[keycode];
}

bool Window::isKeyTyped(unsigned int keycode) const
{
	// TODO: Log this!
	if (keycode >= MAX_KEYS)
		return false;

	return m_KeyTyped[keycode];
}

bool Window::isMouseButtonPressed(unsigned int button) const
{
	// TODO: Log this!
	if (button >= MAX_BUTTONS)
		return false;

	return m_MouseButtons[button];
}

bool Window::isMouseButtonClicked(unsigned int button) const
{
	// TODO: Log this!
	if (button >= MAX_BUTTONS)
		return false;

	return m_MouseClicked[button];
}

void Window::getMousePosition(double& x, double& y) const
{
	x = mx;
	y = my;
}

void Window::processInput(float deltaTime, unsigned int& datIdx, int maxTimeStep, float& pointSize)
{
	
		if (isKeyPressed(GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(m_Window, true);

		if (isKeyPressed(GLFW_KEY_W))
			m_Camera->ProcessKeyboard(FORWARD, deltaTime);
		
		if (isKeyPressed(GLFW_KEY_S))
			m_Camera->ProcessKeyboard(BACKWARD, deltaTime);
		
		if (isKeyPressed(GLFW_KEY_A))
			m_Camera->ProcessKeyboard(LEFT, deltaTime);

		if (isKeyPressed(GLFW_KEY_D))
			m_Camera->ProcessKeyboard(RIGHT, deltaTime);
		
		if (isKeyPressed(GLFW_KEY_R))
			m_Camera->resetCamera();

		if (isKeyPressed(GLFW_KEY_1)) 
		{
			if ((pointSize > 1.0f))
				pointSize -= 0.1f;
		}

		if (isKeyPressed(GLFW_KEY_1)) 
		{
			if (pointSize < 7.0f)
				pointSize += 0.1f;
		}

		handleMouseMovement();
}



void Window::handleMouseMovement()
{
	if (firstMouse)
	{
		last_mx = mx;
		last_my = my;
		firstMouse = false;
	}

	float xoffset = mx - last_mx;
	float yoffset = last_my - my; // reversed since y-coordinates go from bottom to top

	last_mx = mx;
	last_my = my;

	m_Camera->ProcessMouseMovement(xoffset, yoffset, true);
	if (m_WheelOffset != 0)
	{
		m_Camera->ProcessMouseScroll(m_WheelOffset);
		m_WheelOffset = 0.0f;
	}
	
}


GLFWwindow * Window::getWindow() const
{
	return m_Window;
}

unsigned int Window::queryBufferUpdate() const {
	if (isKeyTyped(GLFW_KEY_4))
		return FRONTSTEP;
	if (isKeyTyped(GLFW_KEY_3))
		return BACKSTEP;
	else
		return NOUPDATE;
}

void Window::updatePointSize(float& currentPointSize) const {
	
	float newPointSize = 0.0f;
	if (isKeyPressed(GLFW_KEY_1))
	{
		if ((currentPointSize > 1.0f))
			currentPointSize = currentPointSize - 0.1f;
	}

	else if (isKeyPressed(GLFW_KEY_2))
	{
		if (currentPointSize < 200.0f)
			currentPointSize = currentPointSize + 0.1f;
	}
}


// ---------------------------------------------------------------------------------------------------
// static call-back functions which receive information about pressed buttons and mouse movements
// and send those to the GLWFwindow instance m_Window of this Window class.
void window_resize(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
	Window* win = (Window*)glfwGetWindowUserPointer(window);
	win->m_Width = width;
	win->m_Height = height;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Window* win = (Window*)glfwGetWindowUserPointer(window);
	win->m_Keys[key] = action != GLFW_RELEASE;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	Window* win = (Window*)glfwGetWindowUserPointer(window);
	win->m_MouseButtons[button] = action != GLFW_RELEASE;
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	Window* win = (Window*)glfwGetWindowUserPointer(window);
	win->mx = xpos;
	win->my = ypos;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Window* win = (Window*)glfwGetWindowUserPointer(window);
	win->m_WheelOffset = yoffset;
}


