#pragma once


#include <iostream>
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>


class Window
{
private:
	const char *m_Title;
	int m_Width, m_Height;
	GLFWwindow *m_Window;
	bool m_Closed;

public:
	Window(const char *name, int width, int height);
	~Window();
	GLFWwindow* getWindow();


private:
	bool init();

	void framebuffer_size_callback(GLFWwindow* window, int width, int height);

};