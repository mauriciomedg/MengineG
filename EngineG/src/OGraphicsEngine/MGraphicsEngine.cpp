#include "MGraphicsEngine.h"

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace MG;

MGraphicsEngine::MGraphicsEngine()
{
	m_renderSystem = new MRenderSystem();
}

MGraphicsEngine& MGraphicsEngine::getInstance()
{
	static MGraphicsEngine graphicEngine;
	return graphicEngine;
}

MGraphicsEngine::~MGraphicsEngine()
{
	delete m_renderSystem;
}

MRenderSystem* MGraphicsEngine::getRenderSystem()
{
	return m_renderSystem;
}




