#pragma once

#include "Inputs.h"
#include <GL/glew.h>
#include <iostream>

class Camera;
class Cube;
struct RigidBody;

class GEntity
{
	
public:

	GEntity();

	void init(GLuint* vbo);
	void bindAxis();
	void moveForward(float val);
	void moveSide(float val);
	void update(float currentTime, Camera* camera, GLuint renderingProgram);

	RigidBody* rigidBody;

private:
	
	Cube* meshCube;
	
};