#include "MGraphicsEngine.h"

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace MG;

MGraphicsEngine::MGraphicsEngine()
{
	m_renderSystem = std::make_unique<MRenderSystem>();
}

MGraphicsEngine::~MGraphicsEngine()
{
}

MRenderSystem* MGraphicsEngine::getRenderSystem()
{
	return m_renderSystem.get();
}




