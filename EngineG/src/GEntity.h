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

	virtual void init(GLuint* vbo, const glm::mat4& modelMatrix, bool simulatePhysics);
	virtual void update(Camera* camera, GLuint renderingProgram);
	virtual void setAffectedByGravity(bool isAffectedByGravity);
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

	void setHalfSize(const float halfSize);
	virtual void init(GLuint* vbo, const glm::mat4& modelMatrix, bool simulatePhysics);
	virtual void update(Camera* camera, GLuint renderingProgram);
private:

	Cube* meshCube;
	float mhalfSize = 1.0f;
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

	virtual void init(GLuint* vbo, const glm::mat4& modelMatrix, bool simulatePhysics);
	virtual void update(Camera* camera, GLuint renderingProgram);
	void bindAxis();
	void moveForward(float val);
	void moveSide(float val);

private:

};