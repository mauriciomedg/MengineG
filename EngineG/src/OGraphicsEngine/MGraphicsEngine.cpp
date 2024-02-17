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
	case MTriangleType::TriangleList:
		glTriType = GL_TRIANGLES;
		break;
	case MTriangleType::TriangleStrip:
		glTriType = GL_TRIANGLE_STRIP;
		break;
	}

	glDrawArrays(glTriType, offset, vertexCount);
}

void MGraphicsEngine::drawIndexedTriangles(const MTriangleType& triangleType, ui32 indexCount)
{
	auto glTriType = GL_TRIANGLES;

	switch (triangleType)
	{
	case MTriangleType::TriangleList:
		glTriType = GL_TRIANGLES;
		break;
	case MTriangleType::TriangleStrip:
		glTriType = GL_TRIANGLE_STRIP;
		break;
	}

	glDrawElements(glTriType, indexCount, GL_UNSIGNED_INT, nullptr); // Is nullptr because it was binded before with the vertexArray
}

bool MGraphicsEngine::shouldCloseWindow()
{
	return glfwWindowShouldClose(m_window);
}

void MGraphicsEngine::clear()
{
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT); // clear the background to black, each time
}

void MGraphicsEngine::swapBuffer()
{
	glfwSwapBuffers(m_window);
	glfwPollEvents();
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

MVAOSharedPtr MGraphicsEngine::createVextexArrayObject(const MVertexBufferDesc& vbDes)
{
	return std::make_shared<MVertexArrayObject>(vbDes);
}

MVAOSharedPtr MGraphicsEngine::createVextexArrayObject(const MVertexBufferDesc& vbDes, const MIndexBufferDesc& ibDes)
{
	return std::make_shared<MVertexArrayObject>(vbDes, ibDes);
}

MUniformBufferSharedPtr MGraphicsEngine::createUniformBuffer(const MUniformBufferDesc& data)
{
	return std::make_shared<MUniformBuffer>(data);
}

MShaderProgSharedPtr MGraphicsEngine::createShaderProgram(const MShaderProgramDesc& data)
{
	return std::make_shared<MShaderProgram>(data);
}

void MGraphicsEngine::setVextexArrayObject(const MVAOSharedPtr& vao)
{
	glBindVertexArray(vao->getId());
}

void MGraphicsEngine::setShaderProgram(const MShaderProgSharedPtr& shader)
{
	glUseProgram(shader->getId());
}

void MGraphicsEngine::setUniformBuffer(const MUniformBufferSharedPtr& uniform, ui32 slot)
{
	glBindBufferBase(GL_UNIFORM_BUFFER, slot, uniform->getId());
}

void MGraphicsEngine::setFaceCulling(const MCullType& type)
{
	auto cullType = GL_BACK;

	if (type == MCullType::FrontFace) cullType = GL_FRONT;
	else if (type == MCullType::BackFace) cullType = GL_BACK;
	else if (type == MCullType::FrontFace) cullType = GL_FRONT_AND_BACK;

	glEnable(GL_CULL_FACE);
	glCullFace(cullType);
}

void MGraphicsEngine::setWindingOrder(const MWindingOrder& order)
{
	auto orderType = GL_CW;

	if (order == MWindingOrder::ClockWise) orderType = GL_CW;
	else if (order == MWindingOrder::CounterClockWise) orderType = GL_CCW;

	glFrontFace(orderType);
}


