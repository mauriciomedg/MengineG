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
	void quit();
	void createEntity(float x, float y, float z);
	void createEntityMesh(const char* modelName,
		float* vertices,
		int vertex_count,
		int* indices,
		int index_count,
		float* texture_coord,
		int text_coord_count,
		float* transform_matrix);

private:
	MG::MyGame* mGame;

};