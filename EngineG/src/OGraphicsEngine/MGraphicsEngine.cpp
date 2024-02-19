#include "MGraphicsEngine.h"

#include "RenderSystem/MDeviceContext.h"

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../Resource/MMesh.h"
#include "../Resource/MMaterial.h"

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

	getRenderSystem()->getDeviceContext()->setVextexArrayObject(meshData.mesh.get()->getVertexArrayObject());
	getRenderSystem()->getDeviceContext()->setShaderProgram(meshData.material.get()->getShaderProgram());

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




