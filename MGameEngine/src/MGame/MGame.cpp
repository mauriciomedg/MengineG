#include "MGame.h"
#include "../OGraphicsEngine/MGraphicsEngine.h"
#include "../Entity/MEntitySystem.h"
#include "../Twister/TwPhysicsEngine.h"
#include "../OGraphicsEngine/RenderSystem/MDeviceContext.h"

#include "../Inputs/InputSystem.h"
#include "../Resource/MResourceManager.h"

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
	m_physicsEngine = std::make_unique<TwPhysicsEngine>();
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
		InputSystem::get().update(m_graphicEngine->getRenderSystem()->getDeviceContext()->getWindow());
		
		update(dt);
		m_physicsEngine->update(dt);
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

TwPhysicsEngine* MGame::getPhysicsEngine()
{
	return m_physicsEngine.get();
}