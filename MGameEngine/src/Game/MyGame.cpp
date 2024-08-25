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

void MyGame::requestCreateEntity(float x, float y, float z)
{
	m_delegate_create({ x, y, z });
}

void MyGame::requestCreateEntity(float* vertices,
	int vertex_count,
	int* indices,
	int index_count,
	float* texture_coord,
	int text_coord_count)
{

	std::vector<ui32> indicesList;
	for (int i = 0; i < index_count; i++)
	{
		indicesList.push_back(ui32(indices[i]));
		std::cout << indicesList[i] << std::endl;
	}

	std::vector<glm::vec3> verticesAll;
	for (int i = 0; i < vertex_count; i = i + 3)
	{
		verticesAll.push_back(glm::vec3(f32(vertices[i]), f32(vertices[i + 1]), f32(vertices[i + 2])));
	}

	std::vector<glm::vec2> textCoordAll;
	for (int i = 0; i < text_coord_count; i = i + 2)
	{
		textCoordAll.push_back(glm::vec2(f32(texture_coord[i]), f32(texture_coord[i + 1])));
	}

	std::vector<VertexMesh> verticesList;
	
	for (int i = 0; i < index_count; i++)
	{
		verticesList.push_back({ verticesAll[indicesList[i]],
								 textCoordAll[indicesList[i]]});
	}

	m_delegate_create_mesh({ verticesList, indicesList });
}

void MyGame::commandCreateEntity(data p)
{
	std::lock_guard<std::mutex> guard(m_mutex);
	m_list_data.push_back(p);
}

void MyGame::commandCreateEntityMesh(meshData p)
{
	std::lock_guard<std::mutex> guard(m_mutex);
	m_list_mesh_data.push_back(p);
}

void MyGame::init()
{
	MGame::init();

	m_delegate_create = delegate<data>::from_method<MyGame, &MyGame::commandCreateEntity>(this);
	m_delegate_create_mesh = delegate<meshData>::from_method<MyGame, &MyGame::commandCreateEntityMesh>(this);

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
		auto meshComponent = m_entity->createComponent<MMeshComponent>();
		m_entity->createComponent<MRigidBodyComponent>();

		m_entity->getTransform()->setPosition(glm::vec3(0.0f, 50.0f, 0.0f));
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

void MyGame::createEntity(data& p)
{
	auto mesh = getResourceManager()->createResourceFromFile<MMesh>("models/BlockModel3.obj");
	auto texture = getResourceManager()->createResourceFromFile<MTexture>("textures/wood.png");

	auto material = getResourceManager()->createResourceFromFile<MMaterial>("shaders/basicVertShader.glsl", "shaders/basicFragShader.glsl");
	material->addTexture(texture);

	auto entity = getEntitySystem()->createEntity<MEntity>();
	entity->getTransform()->setPosition(glm::vec3(p.m_px, p.m_py, p.m_pz));
	auto meshComponent = entity->createComponent<MMeshComponent>();
	meshComponent->setMesh(mesh);
	meshComponent->addMaterial(material);
}

void MyGame::createEntity(meshData& p)
{
	MVertexAtrribute attributeList[] =
	{
		sizeof(glm::vec3) / sizeof(f32), // pos
		sizeof(glm::vec2) / sizeof(f32) // text coord
	};
	
	auto mesh = std::make_shared<MMesh>(
		MVertexBufferDesc({ 
				(void*)(&p.verticesList[0]),
				sizeof(VertexMesh),
				(ui32)p.verticesList.size(),
				attributeList,
				sizeof(attributeList) / (sizeof(MVertexAtrribute))
			}),
		MIndexBufferDesc({
					(void*)(&p.indices[0]),
					ui32(p.indices.size())
				}), 
		getResourceManager());
	
	auto texture = getResourceManager()->createResourceFromFile<MTexture>("textures/wood.png");
	auto material = getResourceManager()->createResourceFromFile<MMaterial>("shaders/basicVertShader.glsl", "shaders/basicFragShader.glsl");
	
	material->addTexture(texture);

	auto entity = getEntitySystem()->createEntity<MEntity>();
	entity->getTransform()->setPosition(glm::vec3(0, 6, 0));
	
	auto meshComponent = entity->createComponent<MMeshComponent>();
	meshComponent->setMesh(mesh);
	meshComponent->addMaterial(material);

}

void MyGame::update(f32 dt)
{
	m_theta += 1.0 * dt;

	{
		std::lock_guard<std::mutex> guard(m_mutex);
		if (!m_list_data.empty())
		{
			data p = m_list_data.back();
			m_list_data.pop_back();

			createEntity(p);
		}

		if (!m_list_mesh_data.empty())
		{
			meshData p = m_list_mesh_data.back();
			m_list_mesh_data.pop_back();

			createEntity(p);
		}
	}
	//m_entity->getTransform()->setRotation(m_theta, glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)));
}

void MyGame::quit()
{
	MGame::quit();
}
