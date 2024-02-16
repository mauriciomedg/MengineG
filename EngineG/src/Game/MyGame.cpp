#include "MyGame.h"

using namespace MG;

MyGame::MyGame()
{

}

MyGame::~MyGame()
{

}

void MyGame::onCreate()
{
	MGame::onCreate();
	m_entity = getEntitySystem()->createEntity<MEntity>();
}

void MyGame::update(f32 dt)
{
	m_timeSec += dt;

	if (m_entity && m_timeSec > 3.0f)
	{
		m_entity->release();
		m_entity = nullptr;
	}

}

void MyGame::quit()
{
	MGame::quit();
}
