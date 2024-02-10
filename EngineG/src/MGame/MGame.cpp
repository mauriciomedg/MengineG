#include "MGame.h"
#include "../OGraphicsEngine/MGraphicsEngine.h"

using namespace MG;

MGame::MGame()
{
	m_GraphicsEngine = std::make_unique<MGraphicsEngine>();
}

void MGame::init()
{
	m_GraphicsEngine->init();

	const f32 triangle[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};

	ui32 id = m_GraphicsEngine->createVextexArrayObject({(void*)triangle, sizeof(f32) * 3, 3});
	m_modelsToRender.push_back(id);
}

void MGame::update()
{
	while (m_isRunning)
	{
		m_isRunning = m_GraphicsEngine->update(m_modelsToRender);
	}
}

void MGame::quit()
{
	m_isRunning = false;
}

MGame::~MGame()
{
}