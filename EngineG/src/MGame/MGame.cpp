#include "MGame.h"
#include "../OGraphicsEngine/MGraphicsEngine.h"

using namespace MG;

namespace MG
{
	struct UniformData
	{
		f32 scale;
	};
}

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

	auto uniformId = m_GraphicsEngine->createUniformBuffer(
		{
			sizeof(UniformData)
		}
	);
	m_uniforms.push_back(uniformId);
	m_uniformData[uniformId] = { 1.0 };

	id = m_GraphicsEngine->createShaderProgram({ L"shaders/basicVertShader.glsl", L"shaders/basicFragShader.glsl" });
	m_shaders.push_back(id);

	m_GraphicsEngine->setShaderUniformBufferSlot(id, uniformId, "UniformData", 0);
}

void MGame::update()
{
	while (m_isRunning)
	{
		auto uniformId = m_uniforms.at(0);
		m_uniformData[uniformId] = { 0.1 };
		m_GraphicsEngine->setUniformData(uniformId, &m_uniformData[uniformId]);

		m_isRunning = m_GraphicsEngine->update(m_modelsToRender, m_shaders, m_uniforms);
	}
}

void MGame::quit()
{
	m_isRunning = false;
}

MGame::~MGame()
{
}