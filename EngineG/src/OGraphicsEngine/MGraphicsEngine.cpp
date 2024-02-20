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
	getRenderSystem()->getDeviceContext()->setWindingOrder(MWindingOrder::ClockWise);

	auto R = glm::rotate(glm::mat4(1.0f), meshData.m_theta, glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)));
	glm::mat4 mat = glm::mat4(R);
	UniformData data = { mat };

	meshData.material.get()->setUniformData(&data);

	getRenderSystem()->getDeviceContext()->setVextexArrayObject(meshData.mesh.get()->getVertexArrayObject());
	getRenderSystem()->getDeviceContext()->setUniformBuffer(meshData.material.get()->getUniform(), 0);
	getRenderSystem()->getDeviceContext()->setShaderProgram(meshData.material.get()->getShaderProgram());

	

	getRenderSystem()->getDeviceContext()->drawIndexedTriangles(MTriangleType::TriangleList, meshData.mesh.get()->getVertexArrayObject()->getElementBufferSize() / sizeof(int));

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




