#include "MGame.h"
#include "../OGraphicsEngine/MGraphicsEngine.h"
#include "../Entity/MEntitySystem.h"
#include "../OGraphicsEngine/RenderSystem/MVertexArrayObject.h"
#include "../OGraphicsEngine/RenderSystem/MShaderProgram.h"
#include "../OGraphicsEngine/RenderSystem/MUniformBuffer.h"
#include "../OGraphicsEngine/RenderSystem/MDeviceContext.h"
#include "../Resource/MResourceManager.h"

// to remove
#include "../Resource/MMesh.h"
#include "../Resource/MMaterial.h"
#include "../Resource/MTexture.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

using namespace MG;

namespace MG
{
	class DeltaTime
	{
	public:

		DeltaTime(float& lastTime)
			: mLastTime(lastTime)
		{
		}

		float calculate()
		{
			float current = glfwGetTime();
			return current - mLastTime;
		}

		~DeltaTime()
		{
			mLastTime = glfwGetTime();
		}
	private:

		float& mLastTime;
	};
}

MGame::MGame()
{
	m_graphicEngine = std::make_unique<MGraphicsEngine>();
	m_resourceManager = std::make_unique<MResourceManager>(this);
	m_entitySystem = std::make_unique<MEntitySystem>(this);
}

void MGame::create()
{
}

void MGame::updateInternal()
{
	while (m_isRunning && !m_graphicEngine->getRenderSystem()->getDeviceContext()->shouldCloseWindow())
	{
		DeltaTime deltaTime(m_lastTime);
		float dt = deltaTime.calculate();
		dt = 0.016;
		//
		update(dt);
		m_entitySystem->update(dt);
		m_isRunning = m_graphicEngine->update();
	}
}

void MGame::quit()
{
	m_isRunning = false;
}

MGame::~MGame()
{
}

void MGame::run()
{
	updateInternal();
}

MEntitySystem* MGame::getEntitySystem()
{
	return m_entitySystem.get();
}

MGraphicsEngine* MGame::getGraphicEngine()
{
	return m_graphicEngine.get();
}

MResourceManager* MGame::getResourceManager()
{
	return m_resourceManager.get();
}
