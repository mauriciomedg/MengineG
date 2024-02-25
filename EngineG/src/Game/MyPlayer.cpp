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
	MEntity::create();
	createComponent<MCameraComponent>();
	getTransform()->setPosition(glm::vec3(0.0f, 0.0f, 30.0f));
}

void MyPlayer::update(f32 dt)
{
	MEntity::update(dt);
}
