#pragma once

#ifdef BLENDERPLUGIN_EXPORTS
#define BLENDERPLUGIN_API _declspec(dllexport)
#else
#define BLENDERPLUGIN_API _declspec(dllimport)
#endif

#include <iostream>
#include <thread>

extern "C" BLENDERPLUGIN_API void getInstance();
extern "C" BLENDERPLUGIN_API void execute();

extern "C" BLENDERPLUGIN_API void createEntity(float pX, float pY, float pZ);

