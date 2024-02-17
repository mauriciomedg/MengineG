#include "MyGame.h"
#include "MyPlayer.h"

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
	m_player = getEntitySystem()->createEntity<MyPlayer>();
}

void MyGame::update(f32 dt)
{
	

}

void MyGame::quit()
{
	MGame::quit();
}
