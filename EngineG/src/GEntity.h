#pragma once

#include "Inputs.h"
#include <GL/glew.h>
#include <iostream>

class Particle;
class ParticleSystem;
class Camera;
class Cube;

class GEntity
{
	
public:

	GEntity(ParticleSystem* pSystem);

	void init(GLuint* vbo);
	void bindAxis();
	void moveForward(float val);
	void moveSide(float val);
	void update(float currentTime, Camera* camera, GLuint renderingProgram);

private:
	
	Particle* phyCube;
	Cube* meshCube;
};