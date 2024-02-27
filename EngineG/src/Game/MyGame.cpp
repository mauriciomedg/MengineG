#include "MyGame.h"
#include "MyPlayer.h"
#include "../Inputs/InputSystem.h"
#include <iostream>

#include <GLFW/glfw3.h>
#include "../OGraphicsEngine/MGraphicsEngine.h"
#include "../OGraphicsEngine/RenderSystem/MDeviceContext.h"

using namespace MG;

MyGame::MyGame()
{

}

MyGame::~MyGame()
{

}

void MyGame::create()
{
	MGame::create();

	auto mesh = getResourceManager()->createResourceFromFile<MMesh>("models/BlockModel3.obj");
	auto terrain = getResourceManager()->createResourceFromFile<MMesh>("models/floor.obj");

	auto texture = getResourceManager()->createResourceFromFile<MTexture>("textures/wood.png");
	auto terrainTexture = getResourceManager()->createResourceFromFile<MTexture>("textures/terrain.png");
	
	auto material = getResourceManager()->createResourceFromFile<MMaterial>("shaders/basicVertShader.glsl", "shaders/basicFragShader.glsl");
	material->addTexture(texture);
	
	auto terrainMaterial = getResourceManager()->createResourceFromFile<MMaterial>("shaders/basicVertShader.glsl", "shaders/basicFragShader.glsl");
	terrainMaterial->addTexture(terrainTexture);

	{
		m_entity = getEntitySystem()->createEntity<MEntity>();
		m_entity->getTransform()->setPosition(glm::vec3(0.0f, 6.0f, 0.0f));
		auto meshComponent = m_entity->createComponent<MMeshComponent>();
		meshComponent->setMesh(mesh);
		meshComponent->addMaterial(material);
	}
	
	{
		auto terrainEntity = getEntitySystem()->createEntity<MEntity>();
		auto meshComponentTerrain = terrainEntity->createComponent<MMeshComponent>();
		meshComponentTerrain->setMesh(terrain);
		meshComponentTerrain->addMaterial(terrainMaterial);
	}

	m_player = getEntitySystem()->createEntity<MyPlayer>();
	m_player->getTransform()->setPosition(glm::vec3(0.0f, 20.0f, 30.0f));

	InputSystem::get().mapInput("MoveForwardCamera", "w", 1.0f);
	InputSystem::get().mapInput("MoveBackwardCamera", "s", -1.0f);
	InputSystem::get().mapInput("MoveLeftCamera", "a", -1.0f);
	InputSystem::get().mapInput("MoveRightCamera", "d", 1.0f);
	InputSystem::get().mapMouseInput("MouseX", "mX", 1.0f);
	InputSystem::get().mapMouseInput("MouseY", "mY", 1.0f);

	auto window = m_graphicEngine->getRenderSystem()->getDeviceContext()->getWindow();
	InputSystem::get().disableCursor(window, true);

}

void MyGame::update(f32 dt)
{
	m_theta += 1.0 * dt;
	//m_entity->getTransform()->setRotation(m_theta, glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)));
}

void MyGame::quit()
{
	MGame::quit();
}
