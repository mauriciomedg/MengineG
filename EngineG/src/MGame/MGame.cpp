#include "MGame.h"
#include "../OGraphicsEngine/MGraphicsEngine.h"

using namespace MG;

MGame::MGame()
{
	m_GraphicsEngine = std::make_unique<MGraphicsEngine>();
}

void MGame::init()
{
	m_GraphicsEngine->init();

	const f32 polygonVertices[] = {
		-0.5f, -0.5f, 0.0f,
		1, 0, 0,
		-0.5f, 0.5f, 0.0f,
		0, 1, 0,
		0.5f, -0.5f, 0.0f,
		0, 0, 1,
		0.5f, 0.5f, 0.0f,
		1, 1, 0
	};

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
		3, // pos
		3 // color
	};

	ui32 id = m_GraphicsEngine->createVextexArrayObject(
		{(void*)polygonVertices,
		sizeof(f32) * (3 + 3),
		4,

		attributeList,
		2
		});

	m_modelsToRender.push_back(id);

	id = m_GraphicsEngine->createShaderProgram({ L"shaders/basicVertShader.glsl", L"shaders/basicFragShader.glsl" });
	m_shaders.push_back(id);
}

void MGame::update()
{
	while (m_isRunning)
	{
		m_isRunning = m_GraphicsEngine->update(m_modelsToRender, m_shaders);
	}
}

void MGame::quit()
{
	m_isRunning = false;
}

MGame::~MGame()
{
}