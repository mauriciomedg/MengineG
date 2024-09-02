#include "MGameEngine.h"
#include <iostream>
#include "src/Game/MyGame.h"

int runExample(float pos)
{
	std::cout << "runing form c++ " << pos << std::endl;

	return 1255;
}

MGameEngine::MGameEngine()
{
	mGame = new MG::MyGame();
	mGame->init();
}

MGameEngine::~MGameEngine()
{
	delete mGame;
	mGame = nullptr;
}

void MGameEngine::run()
{
	mGame->run();
}

void MGameEngine::quit()
{
	if (mGame)
		mGame->quit();
}

void MGameEngine::createEntity(float x, float y, float z)
{
	mGame->requestCreateEntity(x, y, z);
}

void MGameEngine::createEntityMesh(const char* modelName,
	float* vertices,
	int vertex_count, 
	int* indices, 
	int index_count, 
	float* texture_coord,
	int text_coord_count,
	float* transform_matrix)
{
	mGame->requestCreateEntity(modelName, vertices, vertex_count, indices, index_count, texture_coord, text_coord_count, transform_matrix);
}