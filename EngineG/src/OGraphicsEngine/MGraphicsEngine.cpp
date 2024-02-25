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

#include "../Components/MMeshComponent.h"
#include "../Components/MCameraComponent.h"
#include "../Entity/MEntity.h"

using namespace MG;

MGraphicsEngine::MGraphicsEngine()
{
	m_renderSystem = std::make_unique<MRenderSystem>();
}

bool MGraphicsEngine::update()
{
	if (getRenderSystem()->getDeviceContext()->shouldCloseWindow()) return false;

	getRenderSystem()->getDeviceContext()->clear();
	getRenderSystem()->getDeviceContext()->setFaceCulling(MCullType::BackFace);
	getRenderSystem()->getDeviceContext()->setWindingOrder(MWindingOrder::CounterClockWise);
	
	UniformData data = {};
	
	int width = 0;
	int height = 0;
	glfwGetFramebufferSize(getRenderSystem()->getDeviceContext()->getWindow(), &width, &height);
	
	for (auto c : m_cameras)
	{
		c->setScreenArea({(float)width, (float)height});
		c->getView(data.vMat);
		c->getProjection(data.pMat);
	}

	for (auto m : m_meshes)
	{
		auto transform = m->getEntity()->getTransform();
		data.world = transform->getWorldMat();

		auto mesh = m->getMesh().get();
		const auto materials = m->getMaterials();

		getRenderSystem()->getDeviceContext()->setVextexArrayObject(mesh->getVertexArrayObject());

		for (auto materialPtr : materials)
		{
			auto material = materialPtr.get();
			getRenderSystem()->getDeviceContext()->setTexture2D(material->getTexture());
			getRenderSystem()->getDeviceContext()->setUniformBuffer(material->getUniform("UniformData"), 0);
			getRenderSystem()->getDeviceContext()->setShaderProgram(material->getShaderProgram());
			material->setUniformData("UniformData", &data);

			getRenderSystem()->getDeviceContext()->drawTriangles(MTriangleType::TriangleList, mesh->getVertexArrayObject()->getVertexBufferSize(), 0);
		}
	}

	getRenderSystem()->getDeviceContext()->swapBuffer();

	return true;
}

void MGraphicsEngine::addComponent(MComponent* component)
{
	if (auto c = dynamic_cast<MMeshComponent*>(component))
		m_meshes.emplace(c);
	else if (auto c = dynamic_cast<MCameraComponent*>(component))
		m_cameras.emplace(c);
}

void MGraphicsEngine::removeComponent(MComponent* component)
{
	if (auto c = dynamic_cast<MMeshComponent*>(component))
		m_meshes.erase(c);
	else if (auto c = dynamic_cast<MCameraComponent*>(component))
		m_cameras.erase(c);
}

MGraphicsEngine::~MGraphicsEngine()
{
}

MRenderSystem* MGraphicsEngine::getRenderSystem()
{
	return m_renderSystem.get();
}




