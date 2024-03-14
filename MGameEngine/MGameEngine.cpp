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
	mGame->create();
}

MGameEngine::~MGameEngine()
{
	delete mGame;
}

void MGameEngine::run()
{
	mGame->run();
}