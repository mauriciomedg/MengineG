#pragma once

#ifdef MGAMEENGINE_EXPORTS
#define MGAMEENGINE_API _declspec(dllexport)
#else
#define MGAMEENGINE_API _declspec(dllimport)
#endif

extern "C" MGAMEENGINE_API int runExample(float pos);

namespace MG
{
	class MyGame;
}

class MGAMEENGINE_API MGameEngine {

public:
	MGameEngine();
	~MGameEngine();

	void run();

private:
	MG::MyGame* mGame;

};