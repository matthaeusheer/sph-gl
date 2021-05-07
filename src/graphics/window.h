#pragma once

#include <iostream>
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include "camera.h"

#define MAX_KEYS	1024
#define MAX_BUTTONS	32

class Window
{

private:
	const char *m_Title;
	int m_Width, m_Height;
	GLFWwindow *m_Window;
	bool m_Closed;
	Camera* m_Camera;

	bool m_Keys[MAX_KEYS];
	bool m_KeyState[MAX_KEYS];
	bool m_KeyTyped[MAX_KEYS];
	bool m_MouseButtons[MAX_BUTTONS];
	bool m_MouseState[MAX_BUTTONS];
	bool m_MouseClicked[MAX_BUTTONS];
	double m_WheelOffset;
	bool firstMouse = true;

	double mx, my;
	double last_mx, last_my;


public:
	Window(const char *name, int width, int height);
	~Window();
	void clear() const;
	void update();
	bool closed() const;

	void attachCamera(Camera& camera);

	inline int getWidth() const { return m_Width; }
	inline int getHeight() const { return m_Height; }

	bool isKeyPressed(unsigned int keycode) const;
	bool isKeyTyped(unsigned int keycode) const;
	bool isMouseButtonPressed(unsigned int button) const;
	bool isMouseButtonClicked(unsigned int button) const;
	void getMousePosition(double& x, double& y) const;

	void processInput(float deltaTime, unsigned int& datIdx, int maxTimeStep, float& pointSize);
	unsigned int queryBufferUpdate() const;
	void updatePointSize(float& currentPointSize) const;

	GLFWwindow* getWindow() const;
	
	static enum timeStepDirection {
		NOUPDATE = 0, BACKSTEP = 1, FRONTSTEP = 2
	} timeStepDirection;


private:
	bool init();
	static void window_resize(GLFWwindow* window, int width, int height);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	void handleMouseMovement();

};
