#include "MGame.h"
#include "../OGraphicsEngine/MGraphicsEngine.h"
#include "../Entity/MEntitySystem.h"
#include "../OGraphicsEngine/MVertexArrayObject.h"
#include "../OGraphicsEngine/MShaderProgram.h"
#include "../OGraphicsEngine/MUniformBuffer.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

using namespace MG;

namespace MG
{
	struct UniformData
	{
		glm::mat4 mat;
		//f32 scale;
	};

	struct Vertex
	{
		glm::vec3 position;
		glm::vec2 textcoord;
	};

	class DeltaTime
	{
	public:

		DeltaTime(float& lastTime)
			: mLastTime(lastTime)
		{
		}

		float calculate()
		{
			float current = glfwGetTime();
			return current - mLastTime;
		}

		~DeltaTime()
		{
			mLastTime = glfwGetTime();
		}
	private:

		float& mLastTime;
	};
}

MGame::MGame()
{
	m_GraphicsEngine = std::make_unique<MGraphicsEngine>();
	m_entitySystem = std::make_unique<MEntitySystem>();
}

void MGame::create()
{
	m_GraphicsEngine->init();

	const glm::vec3 positionsList[] = {
		glm::vec3(-0.5f, -0.5f, -0.5f),
		glm::vec3(-0.5f, 0.5f, -0.5f),
		glm::vec3(0.5f, 0.5f, -0.5f),
		glm::vec3(0.5f, -0.5f, -0.5f),

		glm::vec3(0.5f, -0.5f, 0.5f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(-0.5f, 0.5f, 0.5f),
		glm::vec3(-0.5f, -0.5f, 0.5f)
	};

	glm::vec2 texCoordsList[]
	{
		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 0),
		glm::vec2(1, 1)
	};

	Vertex verticesList[] =
	{
		//front
		{ positionsList[0], texCoordsList[1] },
		{ positionsList[1], texCoordsList[0] },
		{ positionsList[2], texCoordsList[2] },
		{ positionsList[3], texCoordsList[3] },

		//back
		{ positionsList[4], texCoordsList[1] },
		{ positionsList[5], texCoordsList[0] },
		{ positionsList[6], texCoordsList[2] },
		{ positionsList[7], texCoordsList[3] },

		//top
		{ positionsList[1], texCoordsList[1] },
		{ positionsList[6], texCoordsList[0] },
		{ positionsList[5], texCoordsList[2] },
		{ positionsList[2], texCoordsList[3] },
		
		// bottom
		{ positionsList[7], texCoordsList[1] },
		{ positionsList[0], texCoordsList[0] },
		{ positionsList[3], texCoordsList[2] },
		{ positionsList[4], texCoordsList[3] },

		// right
		{ positionsList[3], texCoordsList[1] },
		{ positionsList[2], texCoordsList[0] },
		{ positionsList[5], texCoordsList[2] },
		{ positionsList[4], texCoordsList[3] },

		// left
		{ positionsList[7], texCoordsList[1] },
		{ positionsList[6], texCoordsList[0] },
		{ positionsList[1], texCoordsList[2] },
		{ positionsList[0], texCoordsList[3] }
	};

	ui32 indicesList[] = {
		0, 1, 2,
		2, 3, 0,

		4, 5, 6,
		6, 7, 4,

		8, 9, 10,
		10, 11, 8,

		12, 13, 14,
		14, 15, 12,

		16, 17, 18,
		18, 19, 16,

		20, 21, 22,
		22, 23, 20
	};

	//const f32 polygonVertices[] = {
	//	-0.5f, -0.5f, 0.0f,
	//	1, 0, 0,
	//	-0.5f, 0.5f, 0.0f,
	//	0, 1, 0,
	//	0.5f, -0.5f, 0.0f,
	//	0, 0, 1,
	//	0.5f, 0.5f, 0.0f,
	//	1, 1, 0
	//};

	//const f32 polygonVertices[] = {
	//	-0.5f, -0.5f, 0.0f,
	//	1, 0, 0,
	//	0.5f, -0.5f, 0.0f,
	//	0, 1, 0,
	//	0.0f, 0.5f, 0.0f,
	//	0, 0, 1
	//};

	MVertexAtrribute attributeList[] =
	{
		sizeof(glm::vec3) / sizeof(f32), // pos
		sizeof(glm::vec2) / sizeof(f32) // text coord
	};

	m_vertexArrayObject = m_GraphicsEngine->createVextexArrayObject(
		{(void*)verticesList,
		sizeof(Vertex),
		sizeof(verticesList) / sizeof(Vertex),

		attributeList,
		sizeof(attributeList) / (sizeof(MVertexAtrribute)),
		},

		{
			(void*)indicesList,
			sizeof(indicesList)
		}
	);

	m_uniform = m_GraphicsEngine->createUniformBuffer(
		{
			sizeof(UniformData)
		}
	);

	m_shader = m_GraphicsEngine->createShaderProgram({ L"shaders/basicVertShader.glsl", L"shaders/basicFragShader.glsl" });
	m_shader->setUniformBufferSlot("UniformData", 0);
}

void MGame::updateInternal()
{
	while (m_isRunning && !m_GraphicsEngine->shouldCloseWindow())
	{
		DeltaTime deltaTime(m_lastTime);
		float dt = deltaTime.calculate();
		dt = 0.016;

		update(dt);
		m_entitySystem->update(dt);

		m_theta += 1.0 * dt;
		
		auto R = glm::rotate(glm::mat4(1.0f), m_theta, glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)));
		glm::mat4 mat = glm::mat4(R);
		UniformData data = { mat };
		m_uniform->setData(&data);
		
		m_GraphicsEngine->clear();
		m_GraphicsEngine->setFaceCulling(MCullType::BackFace);
		m_GraphicsEngine->setWindingOrder(MWindingOrder::ClockWise);

		m_GraphicsEngine->setVextexArrayObject(m_vertexArrayObject);
		m_GraphicsEngine->setUniformBuffer(m_uniform, 0);
		m_GraphicsEngine->setShaderProgram(m_shader);
		m_GraphicsEngine->drawIndexedTriangles(MTriangleType::TriangleList, m_vertexArrayObject.get()->getElementBufferSize() / sizeof(int));
		m_GraphicsEngine->swapBuffer();
	}
}

void MGame::quit()
{
	m_isRunning = false;
}

MGame::~MGame()
{
}

void MGame::run()
{
	updateInternal();
}

MEntitySystem* MGame::getEntitySystem()
{
	return m_entitySystem.get();
}
