#pragma once

#include "Inputs.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <string>

class Camera;
class GShape;
class Cube;
class PhysicsWorld;

class GPrimitiveEntity
{

public:
	GPrimitiveEntity(PhysicsWorld* mWorld);

	virtual void init(GLuint* vbo, glm::mat4& modelMatrix, bool simulatePhysics);
	virtual void update(Camera* camera, GLuint renderingProgram);

protected:
	glm::mat4 mMat;
	PhysicsWorld* mWorld;
	GShape* mShape;
	int mCollisionPrimitiveId;
	bool mSimulatePhysics;
};

class GEntityBox : public GPrimitiveEntity
{

public:

	GEntityBox(PhysicsWorld* pWorld);

	virtual void init(GLuint* vbo, glm::mat4& modelMatrix, bool simulatePhysics);
	virtual void update(Camera* camera, GLuint renderingProgram);
private:

	Cube* meshCube;
};

////////////////////////////////////////////////////
class Controller
{
public:
	Controller();

};

class GEntityBoxControlled : public GEntityBox
{
	
public:

	GEntityBoxControlled(PhysicsWorld* pWorld);

	virtual void init(GLuint* vbo, glm::mat4& modelMatrix, bool simulatePhysics);
	virtual void update(Camera* camera, GLuint renderingProgram);
	void bindAxis();
	void moveForward(float val);
	void moveSide(float val);
	void consumeInput();
	
private:

};