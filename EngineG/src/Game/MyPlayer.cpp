#include "MyPlayer.h"
#include "../AllMHeaders.h"
#include "../Inputs/InputSystem.h"
#include <iostream>

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

	bindAxis();
}

void MyPlayer::update(f32 dt)
{
	MEntity::update(dt);


}

void MyPlayer::bindAxis()
{
	InputSystem::get().addBinding("MoveForwardCamera", delegate::from_method<MyPlayer, &MyPlayer::moveForward>(this));
	InputSystem::get().addBinding("MoveBackwardCamera", delegate::from_method<MyPlayer, &MyPlayer::moveForward>(this));
	InputSystem::get().addBinding("MoveLeftCamera", delegate::from_method<MyPlayer, &MyPlayer::moveSide>(this));
	InputSystem::get().addBinding("MoveRightCamera", delegate::from_method<MyPlayer, &MyPlayer::moveSide>(this));
	InputSystem::get().addBinding("MouseX", delegate::from_method<MyPlayer, &MyPlayer::mouseX>(this));
	InputSystem::get().addBinding("MouseY", delegate::from_method<MyPlayer, &MyPlayer::mouseY>(this));
}

void MyPlayer::moveForward(float val)
{
	auto worldMat = getTransform()->getWorldMat();
	auto pos = glm::vec3(worldMat[3]);
	pos += -glm::vec3(worldMat[2]) * 1.0f * val;
	getTransform()->setPosition(pos);
}

void MyPlayer::moveSide(float val)
{
	auto worldMat = getTransform()->getWorldMat();
	auto pos = glm::vec3(worldMat[3]);
	pos += glm::vec3(worldMat[0]) * 1.0f * val;
	getTransform()->setPosition(pos);
}

void MyPlayer::mouseX(float val)
{
	std::cout << "x " << val << std::endl;
}

void MyPlayer::mouseY(float val)
{
	std::cout << "y " << val << std::endl;
}
