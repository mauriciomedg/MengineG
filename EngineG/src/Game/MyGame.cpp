#include "MyGame.h"

using namespace MG;

MyGame::MyGame()
{

}

MyGame::~MyGame()
{

}

void MyGame::onCreate()
{
	MGame::onCreate();
	auto e = getEntitySystem()->createEntity<MEntity>();
}

void MyGame::update()
{
	MGame::update();
}

void MyGame::quit()
{
	MGame::quit();
}
