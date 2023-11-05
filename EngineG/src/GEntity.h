#pragma once

#include "Inputs.h"
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

class Camera;
class Cube;
struct RigidBody;

class GEntity
{
	
public:

	GEntity();

	void init(GLuint* vbo, glm::mat4& modelMatrix);
	void bindAxis();
	void moveForward(float val);
	void moveSide(float val);
	void consumeInput();
	void setIndexRigid(int index);
	void update(float currentTime, Camera* camera, GLuint renderingProgram);

	RigidBody* rigidBody;

private:
	
	Cube* meshCube;
	
};