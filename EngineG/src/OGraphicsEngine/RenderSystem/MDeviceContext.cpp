#include "MDeviceContext.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "MVertexArrayObject.h"
#include "MShaderProgram.h"
#include "MUniformBuffer.h"

using namespace MG;

namespace MG
{
	void window_reshape_callback(GLFWwindow* window, int newWidth, int newHeight)
	{
		glViewport(0, 0, newWidth, newHeight);
	}
}

MDeviceContext::MDeviceContext(MRenderSystem* renderSystem)
	: m_renderSystem(renderSystem)
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

void MDeviceContext::drawTriangles(const MTriangleType& triangleType, ui32 vertexCount, ui32 offset)
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

	glDrawArrays(glTriType, offset, vertexCount);
}

void MDeviceContext::drawIndexedTriangles(const MTriangleType& triangleType, ui32 indexCount)
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

bool MDeviceContext::shouldCloseWindow()
{
	return glfwWindowShouldClose(m_window);
}

void MDeviceContext::clear()
{
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT); // clear the background to black, each time
}

void MDeviceContext::swapBuffer()
{
	glfwSwapBuffers(m_window);
	glfwPollEvents();
}

void MDeviceContext::setVextexArrayObject(const MVAOSharedPtr& vao)
{
	glBindVertexArray(vao->getId());
}

void MDeviceContext::setShaderProgram(const MShaderProgSharedPtr& shader)
{
	glUseProgram(shader->getId());
}

void MDeviceContext::setUniformBuffer(const MUniformBufferSharedPtr& uniform, ui32 slot)
{
	glBindBufferBase(GL_UNIFORM_BUFFER, slot, uniform->getId());
}

void MDeviceContext::setFaceCulling(const MCullType& type)
{
	auto cullType = GL_BACK;

	if (type == MCullType::FrontFace) cullType = GL_FRONT;
	else if (type == MCullType::BackFace) cullType = GL_BACK;
	else if (type == MCullType::FrontFace) cullType = GL_FRONT_AND_BACK;

	glEnable(GL_CULL_FACE);
	glCullFace(cullType);
}

void MDeviceContext::setWindingOrder(const MWindingOrder& order)
{
	auto orderType = GL_CW;

	if (order == MWindingOrder::ClockWise) orderType = GL_CW;
	else if (order == MWindingOrder::CounterClockWise) orderType = GL_CCW;

	glFrontFace(orderType);
}

MDeviceContext::~MDeviceContext()
{
	if (m_window)
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
		exit(EXIT_SUCCESS);
	}
}

