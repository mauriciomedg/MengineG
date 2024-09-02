#pragma once

#ifdef BLENDERPLUGIN_EXPORTS
#define BLENDERPLUGIN_API _declspec(dllexport)
#else
#define BLENDERPLUGIN_API _declspec(dllimport)
#endif

#include <iostream>
#include <thread>

extern "C" BLENDERPLUGIN_API void getInstance();
extern "C" BLENDERPLUGIN_API void start();
extern "C" BLENDERPLUGIN_API void quit();

extern "C" BLENDERPLUGIN_API void createEntity(float pX, float pY, float pZ);
extern "C" BLENDERPLUGIN_API void createEntityMesh(const char* modelName,
	float* vertices,
	int vertex_count,
	int* indices,
	int index_count,
	float* texture_coord,
	int text_coord_count,
	float* transform_matrix);