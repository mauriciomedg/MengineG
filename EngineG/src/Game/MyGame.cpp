#include "MyGame.h"
#include "MyPlayer.h"
#include <iostream>

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
	auto texture = getResourceManager()->createResourceFromFile<MTexture>("textures/block.png");
	auto material = getResourceManager()->createResourceFromFile<MMaterial>("shaders/basicVertShader.glsl", "shaders/basicFragShader.glsl");
	material->addTexture(texture);
	//const glm::vec3 positionsList[] = {
	//	glm::vec3(-0.5f, -0.5f, -0.5f),
	//	glm::vec3(-0.5f, 0.5f, -0.5f),
	//	glm::vec3(0.5f, 0.5f, -0.5f),
	//	glm::vec3(0.5f, -0.5f, -0.5f),
	//
	//	glm::vec3(0.5f, -0.5f, 0.5f),
	//	glm::vec3(0.5f, 0.5f, 0.5f),
	//	glm::vec3(-0.5f, 0.5f, 0.5f),
	//	glm::vec3(-0.5f, -0.5f, 0.5f)
	//};
	//
	//glm::vec2 texCoordsList[]
	//{
	//	glm::vec2(0, 0),
	//	glm::vec2(0, 1),
	//	glm::vec2(1, 0),
	//	glm::vec2(1, 1)
	//};
	//
	//std::vector<VertexMesh> verticesList =
	//{
	//	//front
	//	{ positionsList[0], texCoordsList[1] },
	//	{ positionsList[1], texCoordsList[0] },
	//	{ positionsList[2], texCoordsList[2] },
	//	{ positionsList[3], texCoordsList[3] },
	//
	//	//back
	//	{ positionsList[4], texCoordsList[1] },
	//	{ positionsList[5], texCoordsList[0] },
	//	{ positionsList[6], texCoordsList[2] },
	//	{ positionsList[7], texCoordsList[3] },
	//
	//	//top
	//	{ positionsList[1], texCoordsList[1] },
	//	{ positionsList[6], texCoordsList[0] },
	//	{ positionsList[5], texCoordsList[2] },
	//	{ positionsList[2], texCoordsList[3] },
	//	
	//	// bottom
	//	{ positionsList[7], texCoordsList[1] },
	//	{ positionsList[0], texCoordsList[0] },
	//	{ positionsList[3], texCoordsList[2] },
	//	{ positionsList[4], texCoordsList[3] },
	//
	//	// right
	//	{ positionsList[3], texCoordsList[1] },
	//	{ positionsList[2], texCoordsList[0] },
	//	{ positionsList[5], texCoordsList[2] },
	//	{ positionsList[4], texCoordsList[3] },
	//
	//	// left
	//	{ positionsList[7], texCoordsList[1] },
	//	{ positionsList[6], texCoordsList[0] },
	//	{ positionsList[1], texCoordsList[2] },
	//	{ positionsList[0], texCoordsList[3] }
	//};
	//
	//ui32 indicesList[] = {
	//	0, 1, 2,
	//	2, 3, 0,
	//
	//	4, 5, 6,
	//	6, 7, 4,
	//
	//	8, 9, 10,
	//	10, 11, 8,
	//
	//	12, 13, 14,
	//	14, 15, 12,
	//
	//	16, 17, 18,
	//	18, 19, 16,
	//
	//	20, 21, 22,
	//	22, 23, 20
	//};
	//
	//MVertexAtrribute attributeList[] =
	//{
	//	sizeof(glm::vec3) / sizeof(f32), // pos
	//	sizeof(glm::vec2) / sizeof(f32) // text coord
	//};
	//
	//m_vertexArrayObject = m_graphicEngine->getRenderSystem()->createVextexArrayObject(
	//	{(void*)(&verticesList[0]),
	//	sizeof(VertexMesh),
	//	(ui32)verticesList.size(),
	//
	//	attributeList,
	//	sizeof(attributeList) / (sizeof(MVertexAtrribute)),
	//	},
	//
	//	{
	//		(void*)indicesList,
	//		sizeof(indicesList)
	//	}
	//);
	//
	

	//material->addUniform("UniformData", sizeof(UniformData), 0);

	m_player = getEntitySystem()->createEntity<MyPlayer>();

	auto meshComponent = m_player->createComponent<MMeshComponent>();
	meshComponent->setMesh(mesh);
	meshComponent->addMaterial(material);
}

void MyGame::update(f32 dt)
{
	MGame::update(dt);

	m_theta += 1.0 * dt;
	m_player->getTransform()->setRotation(m_theta, glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)));
}

void MyGame::quit()
{
	MGame::quit();
}
