#include "MGraphicsEngine.h"

#include "MVertexArrayObject.h"
#include "MUniformBuffer.h"
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

void MGraphicsEngine::drawTriangles(const MTriangleType& triangleType, ui32 vertexCount, ui32 offset)
{
	auto glTriType = GL_TRIANGLES;
	
	switch(triangleType)
	{
	case TriangleList:
		glTriType = GL_TRIANGLES;
		break;
	case TriangleStrip:
		glTriType = GL_TRIANGLE_STRIP;
		break;
	}

	glDrawArrays(glTriType, offset, vertexCount);
}

void MGraphicsEngine::drawIndexedTriangles(const MTriangleType& triangleType, ui32 indecesCount)
{
	auto glTriType = GL_TRIANGLES;

	switch (triangleType)
	{
	case TriangleList:
		glTriType = GL_TRIANGLES;
		break;
	case TriangleStrip:
		glTriType = GL_TRIANGLE_STRIP;
		break;
	}

	glDrawElements(glTriType, indecesCount, GL_UNSIGNED_INT, nullptr); // Is nullptr because it was binded before with the vertexArray
}

void MGraphicsEngine::display(const std::vector<ui32>& modelsToRender, const std::vector<ui32>& shaders, const std::vector<ui32>& uniforms)
{
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT); // clear the background to black, each time
	//glEnable(GL_CULL_FACE);

	for (ui32 id : modelsToRender)
	{
		glBindVertexArray(m_VAOlist[id]->getId());
		//drawTriangles(TriangleStrip, m_VAOlist[id]->getVertexBufferSize(), 0);
		drawIndexedTriangles(TriangleStrip, m_VAOlist[id]->getElementBufferSize() / sizeof(int));
	}

	for (ui32 id : uniforms)
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, m_uniformBufferSlots[id], m_uniformBuffers[id]->getId());
	}

	for (ui32 id : shaders)
	{
		glUseProgram(m_shadersProgram[id]->getId());
	}
}

bool MGraphicsEngine::update(const std::vector<ui32>& modelsToRender, const std::vector<ui32>& shaders, const std::vector<ui32>& uniforms)
{
	if (glfwWindowShouldClose(m_window)) return false;
	
	display(modelsToRender, shaders, uniforms);

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

ui32 MGraphicsEngine::createVextexArrayObject(const MVertexBufferDesc& vbDes)
{
	m_VAOlist.push_back(std::make_shared<MVertexArrayObject>(vbDes));
	return m_VAOlist.size() - 1;
}

ui32 MGraphicsEngine::createVextexArrayObject(const MVertexBufferDesc& vbDes, const MIndexBufferDesc& ibDes)
{
	m_VAOlist.push_back(std::make_shared<MVertexArrayObject>(vbDes, ibDes));
	return m_VAOlist.size() - 1;
}

ui32 MGraphicsEngine::createUniformBuffer(const MUniformBufferDesc& data)
{
	auto uniformBuffer = std::make_shared<MUniformBuffer>(data);
	m_uniformBuffers[uniformBuffer->getId()] = uniformBuffer;
	return uniformBuffer->getId();
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

void MGraphicsEngine::setShaderUniformBufferSlot(const ui32 shaderId, const ui32 uniformId, const char* name, ui32 slot)
{
	m_shadersProgram[shaderId]->setUniformBufferSlot(name, slot);
	m_uniformBufferSlots[uniformId] = slot;
}

void MGraphicsEngine::setUniformData(const ui32 uniformId, void* data)
{
	m_uniformBuffers[uniformId]->setData(data);
}


