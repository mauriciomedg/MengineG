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
	m_camera = createComponent<MCameraComponent>();
	getTransform()->setPosition(glm::vec3(0.0f, 0.0f, 30.0f));

	bindAxis();
}

void MyPlayer::update(f32 dt)
{
	MEntity::update(dt);
	m_camera->setReadInput(true);

}

void MyPlayer::bindAxis()
{
	InputSystem::get().addBinding("MoveForwardCamera", delegate<float>::from_method<MyPlayer, &MyPlayer::moveForward>(this));
	InputSystem::get().addBinding("MoveBackwardCamera", delegate<float>::from_method<MyPlayer, &MyPlayer::moveForward>(this));
	InputSystem::get().addBinding("MoveLeftCamera", delegate<float>::from_method<MyPlayer, &MyPlayer::moveSide>(this));
	InputSystem::get().addBinding("MoveRightCamera", delegate<float>::from_method<MyPlayer, &MyPlayer::moveSide>(this));
	InputSystem::get().addBinding("MouseX", delegate<float>::from_method<MyPlayer, &MyPlayer::mouseX>(this));
	InputSystem::get().addBinding("MouseY", delegate<float>::from_method<MyPlayer, &MyPlayer::mouseY>(this));
}

void MyPlayer::moveForward(float val)
{
	const auto& cameraFront = m_camera->getLookAt();

	auto worldMat = getTransform()->getWorldMat();
	worldMat = glm::translate(worldMat, glm::normalize(cameraFront) * 1.0f * val);
	getTransform()->setWorldMat(worldMat);
}

void MyPlayer::moveSide(float val)
{
	const auto& side = m_camera->getSide();

	auto worldMat = getTransform()->getWorldMat();
	worldMat = glm::translate(worldMat, glm::normalize(side) * 1.0f * val);
	getTransform()->setWorldMat(worldMat);
}

void MyPlayer::mouseX(float delta)
{
	//std::cout << "X " << delta << std::endl;
	m_camera->rotateCameraYaw(delta * 0.005f);
}

void MyPlayer::mouseY(float delta)
{
	//std::cout << "Y " << delta << std::endl;
	m_camera->rotateCameraPitch(delta * 0.005f);
}
