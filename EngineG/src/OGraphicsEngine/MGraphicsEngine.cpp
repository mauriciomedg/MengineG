#include "MGraphicsEngine.h"
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace MG;

namespace
{
	void window_reshape_callback(GLFWwindow* window, int newWidth, int newHeight)
	{
		//camera->reshapeWindow(window, newWidth, newHeight);
	}
}

MGraphicsEngine::MGraphicsEngine()
	: m_window(nullptr)
{
}

void MGraphicsEngine::init()
{
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	m_window = glfwCreateWindow(600, 600, "MengineG", NULL, NULL);
	glfwMakeContextCurrent(m_window);

	if (glewInit() != GLEW_OK) 
	{
		m_window = nullptr;
		exit(EXIT_FAILURE);
	}
	glfwSwapInterval(1);
	glfwSetWindowSizeCallback(m_window, window_reshape_callback);
}

void MGraphicsEngine::display()
{
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT); // clear the background to black, each time
	glEnable(GL_CULL_FACE);
}

bool MGraphicsEngine::update()
{
	if (glfwWindowShouldClose(m_window)) return false;
	
	display();

	glfwSwapBuffers(m_window);
	glfwPollEvents();
	return true;
}

MGraphicsEngine::~MGraphicsEngine()
{
	if (m_window)
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
		exit(EXIT_SUCCESS);
	}
}

