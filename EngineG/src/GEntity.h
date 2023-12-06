#pragma once

#include "Inputs.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

class Camera;
class Cube;

class GEntity
{
	
public:

	GEntity();

	void init(GLuint* vbo, glm::mat4& modelMatrix, int physicsObjectId);
	void bindAxis();
	void moveForward(float val);
	void moveSide(float val);
	void consumeInput();
	void update(Camera* camera, GLuint renderingProgram, const glm::mat4* modelMatrix);

	//RigidBody* rigidBody;
	int mPhysicsObjectId = -1;
private:
	
	Cube* meshCube;
	
};