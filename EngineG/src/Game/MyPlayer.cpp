#include "MyPlayer.h"
#include "../AllMHeaders.h"

using namespace MG;

MyPlayer::MyPlayer()
{
}

MyPlayer::~MyPlayer()
{
}

void MyPlayer::create()
{
	m_entity = getEntitySystem()->createEntity<MEntity>();
	m_entity->createComponent<MComponent>();
}

void MyPlayer::update(f32 dt)
{
	m_timeSec += dt;

	//m_entity->getTransform()->setRotation(glm::vec3(m_timeSec, 0, 0));
	//if (m_entity && m_timeSec > 3.0f)
	//{
	//	m_entity->getComponent<MComponent>()->release();
	//	m_entity->release();
	//	m_entity = nullptr;
	//}
}
