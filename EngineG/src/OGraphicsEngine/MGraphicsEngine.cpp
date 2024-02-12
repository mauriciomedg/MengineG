#include "MGraphicsEngine.h"

#include "MVertexArrayObject.h"
#include "MShaderProgram.h"

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace MG;

namespace
{
	void window_reshape_callback(GLFWwindow* window, int newWidth, int newHeight)
	{
		glViewport(0, 0, newWidth, newHeight); 
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

	//init viewport
	int width, height;
	glfwGetFramebufferSize(m_window, &width, &height);
	window_reshape_callback(m_window, width, height);
}

void MGraphicsEngine::drawTriangles(ui32 vertexCount, ui32 offset)
{
	glDrawArrays(GL_TRIANGLES, offset, vertexCount);
}

void MGraphicsEngine::display(const std::vector<ui32>& modelsToRender, const std::vector<ui32>& shaders)
{
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT); // clear the background to black, each time
	glEnable(GL_CULL_FACE);

	for (ui32 id : modelsToRender)
	{
		glBindVertexArray(m_VAOlist[id]->getId());
		glDrawArrays(GL_TRIANGLES, 0, m_VAOlist[id]->getVertexBufferSize()); // 36 number of vertices
	}

	for (ui32 id : shaders)
	{
		glUseProgram(m_shadersProgram[id]->getId());
	}
}

bool MGraphicsEngine::update(const std::vector<ui32>& modelsToRender, const std::vector<ui32>& shaders)
{
	if (glfwWindowShouldClose(m_window)) return false;
	
	display(modelsToRender, shaders);

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

ui32 MGraphicsEngine::createVextexArrayObject(const MVertexBufferDesc& data)
{
	m_VAOlist.push_back(std::make_shared<MVertexArrayObject>(data));
	
	return m_VAOlist.size() - 1;
}

ui32 MGraphicsEngine::createShaderProgram(const MShaderProgramDesc& data)
{
	auto shaderProgram = std::make_shared<MShaderProgram>(data);
	m_shadersProgram[shaderProgram->getId()] = shaderProgram;
	return shaderProgram->getId();
}

void MGraphicsEngine::setVextexArrayObject(const ui32 id)
{
	glBindVertexArray(m_VAOlist[id]->getId()); // make the 0th buffer "active"
}

void MGraphicsEngine::setShaderProgram(const ui32 id)
{
	glUseProgram(m_shadersProgram[id]->getId());
}


