#include "MGame.h"
#include "../OGraphicsEngine/MGraphicsEngine.h"

using namespace MG;

MGame::MGame()
{
	m_GraphicsEngine = std::unique_ptr<MGraphicsEngine>(new MGraphicsEngine);// std::make_unique<MGraphicsEngine>(new MGraphicsEngine());
}

void MGame::init()
{
	m_GraphicsEngine->init();
}

void MGame::update()
{
	while (m_isRunning)
	{
		m_isRunning = m_GraphicsEngine->update();

	}
}

void MGame::quit()
{
	m_isRunning = false;
}

MGame::~MGame()
{
}