#include "BlenderPlugin.h"
#include "MGameEngine.h"

class RunEngine
{
private:
	RunEngine();
	RunEngine(const RunEngine&);

	void runEngine();

public:

	~RunEngine();

	void run();

	static RunEngine& getInstace();

private:
	std::thread first;
	MGameEngine* game;
};

RunEngine::RunEngine()
{
}

void RunEngine::runEngine()
{
	game = new MGameEngine();
	game->run();
}

RunEngine::~RunEngine()
{
	delete game;
}

void RunEngine::run()
{
	std::thread firstTemp([&]() { this->runEngine(); });
	first = std::move(firstTemp);
}

RunEngine& RunEngine::getInstace()
{
	static RunEngine instance;
	return instance;
}

void getInstance()
{
	RunEngine::getInstace();
}

void execute()
{
	RunEngine::getInstace().run();
}