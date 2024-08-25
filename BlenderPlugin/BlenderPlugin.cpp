#include "BlenderPlugin.h"
#include "MGameEngine.h"
#include <mutex>

class RunEngine
{
private:
	RunEngine() {};
	RunEngine(const RunEngine&);
	RunEngine& operator=(const RunEngine&);

	void runEngine();

public:

	~RunEngine();
	void start();
	void quit()
	{
		if (m_game)
			m_game->quit();

		if (m_first.joinable())
			m_first.join();

		m_game = nullptr;
	}

	void createEntity(float pX, float pY, float pZ)
	{
		if (m_game)
			m_game->createEntity(pX, pY, pZ);
	}

	void createEntityMesh(float* vertices, 
		int vertex_count, 
		int* indices, 
		int index_count, 
		float* texture_coord, 
		int text_coord_count)
	{
		if (m_game)
			m_game->createEntityMesh(vertices,
				vertex_count, indices, index_count, texture_coord, text_coord_count);
	}

	static RunEngine& getInstace();

private:
	std::thread m_first;
	MGameEngine* m_game = nullptr;
};

void RunEngine::runEngine()
{
	m_game = new MGameEngine();
	m_game->run();
	delete m_game;
}

RunEngine::~RunEngine()
{
	quit();
	std::cout << "Dll killed.." << std::endl;
}

void RunEngine::start()
{
	std::cout << "Star Engine from plugin" << std::endl;

	std::thread firstTemp([&]() { this->runEngine(); });
	m_first.swap(firstTemp);
}

RunEngine& RunEngine::getInstace()
{
	static RunEngine instance;
	return instance;
}

/// //////////////////////////////////////////////
void getInstance()
{
	RunEngine::getInstace();
}

void start()
{
	RunEngine::getInstace().start();
}

void createEntity(float pX, float pY, float pZ)
{
	RunEngine::getInstace().createEntity(pX, pY, pZ);
}

void createEntityMesh(float* vertices,
	int vertex_count,
	int* indices,
	int index_count,
	float* texture_coord,
	int text_coord_count)
{
	//for (int i = 0; i < vertex_count; ++i)
	//	std::cout << "vertex " << vertices[i] << std::endl;
	//
	//for (int i = 0; i < index_count; ++i)
	//	std::cout << "indices " << indices[i] << std::endl;
	//
	//for (int i = 0; i < text_coord_count; ++i)
	//	std::cout << "texture coord " << texture_coord[i] << std::endl;

	RunEngine::getInstace().createEntityMesh(vertices,
		vertex_count,
		indices,
		index_count,
		texture_coord,
		text_coord_count);
}

void quit()
{
	RunEngine::getInstace().quit();
}