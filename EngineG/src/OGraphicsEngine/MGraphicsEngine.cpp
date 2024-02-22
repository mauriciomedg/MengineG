#include "MGraphicsEngine.h"

#include "RenderSystem/MDeviceContext.h"

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Resource/MMesh.h"
#include "../Resource/MMaterial.h"

#include "RenderSystem/MVertexArrayObject.h"

using namespace MG;

MGraphicsEngine::MGraphicsEngine()
{
	m_renderSystem = std::make_unique<MRenderSystem>();
}

bool MGraphicsEngine::update(const MeshData& meshData)
{
	if (getRenderSystem()->getDeviceContext()->shouldCloseWindow()) return false;

	getRenderSystem()->getDeviceContext()->clear();
	getRenderSystem()->getDeviceContext()->setFaceCulling(MCullType::BackFace);
	getRenderSystem()->getDeviceContext()->setWindingOrder(MWindingOrder::CounterClockWise);
	
	////

	glm::vec3 Pos = glm::vec3(0.0f, 0.0f, 30.0f);
	auto mMat = glm::translate(glm::mat4(1.0f), Pos);

	glm::vec3 cameraPos = mMat[3];
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	auto vMat = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	int width = 0;
	int height = 0;
	glfwGetFramebufferSize(getRenderSystem()->getDeviceContext()->getWindow(), &width, &height);
	auto aspect = (float)width / (float)height; // new width&height provided by the callback
	auto pMat = glm::perspective(1.3f, aspect, 0.1f, 1000.0f);
	
	////


	auto R = glm::rotate(glm::mat4(1.0f), meshData.m_theta, glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)));
	glm::mat4 world = glm::mat4(R);
	UniformData data = { world, vMat, pMat };

	meshData.material.get()->setUniformData(&data);

	getRenderSystem()->getDeviceContext()->setVextexArrayObject(meshData.mesh.get()->getVertexArrayObject());
	getRenderSystem()->getDeviceContext()->setTexture2D(meshData.material.get()->getTexture());
	getRenderSystem()->getDeviceContext()->setUniformBuffer(meshData.material.get()->getUniform(), 0);
	getRenderSystem()->getDeviceContext()->setShaderProgram(meshData.material.get()->getShaderProgram());
	
	//getRenderSystem()->getDeviceContext()->drawIndexedTriangles(MTriangleType::TriangleList, meshData.mesh.get()->getVertexArrayObject()->getElementBufferSize() / sizeof(int));
	getRenderSystem()->getDeviceContext()->drawTriangles(MTriangleType::TriangleList, meshData.mesh.get()->getVertexArrayObject()->getVertexBufferSize(), 0);

	getRenderSystem()->getDeviceContext()->swapBuffer();

	return true;
}

MGraphicsEngine::~MGraphicsEngine()
{
}

MRenderSystem* MGraphicsEngine::getRenderSystem()
{
	return m_renderSystem.get();
}




