#include "MGame.h"
#include "../OGraphicsEngine/MGraphicsEngine.h"
#include "../Entity/MEntitySystem.h"
#include "../OGraphicsEngine/RenderSystem/MVertexArrayObject.h"
#include "../OGraphicsEngine/RenderSystem/MShaderProgram.h"
#include "../OGraphicsEngine/RenderSystem/MUniformBuffer.h"
#include "../OGraphicsEngine/RenderSystem/MDeviceContext.h"

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
	MGraphicsEngine::getInstance();
	m_entitySystem = std::make_unique<MEntitySystem>();
}

void MGame::create()
{
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

	m_vertexArrayObject = MGraphicsEngine::getInstance().getRenderSystem()->createVextexArrayObject(
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

	m_uniform = MGraphicsEngine::getInstance().getRenderSystem()->createUniformBuffer(
		{
			sizeof(UniformData)
		}
	);

	m_shader = MGraphicsEngine::getInstance().getRenderSystem()->createShaderProgram({ L"shaders/basicVertShader.glsl", L"shaders/basicFragShader.glsl" });
	m_shader->setUniformBufferSlot("UniformData", 0);
}

void MGame::updateInternal()
{
	MGraphicsEngine& graphicEngine = MGraphicsEngine::getInstance();
	while (m_isRunning && !graphicEngine.getRenderSystem()->getDeviceContext()->shouldCloseWindow())
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
		
		graphicEngine.getRenderSystem()->getDeviceContext()->clear();
		graphicEngine.getRenderSystem()->getDeviceContext()->setFaceCulling(MCullType::BackFace);
		graphicEngine.getRenderSystem()->getDeviceContext()->setWindingOrder(MWindingOrder::ClockWise);

		graphicEngine.getRenderSystem()->getDeviceContext()->setVextexArrayObject(m_vertexArrayObject);
		graphicEngine.getRenderSystem()->getDeviceContext()->setUniformBuffer(m_uniform, 0);
		graphicEngine.getRenderSystem()->getDeviceContext()->setShaderProgram(m_shader);
		graphicEngine.getRenderSystem()->getDeviceContext()->drawIndexedTriangles(MTriangleType::TriangleList, m_vertexArrayObject.get()->getElementBufferSize() / sizeof(int));
		graphicEngine.getRenderSystem()->getDeviceContext()->swapBuffer();
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
