#include "GEntity.h"
#include "physicsEngine/ParticlesSimulation.h"
#include "Cube.h"
#include "physicsEngine/RigidBody.h"

GEntity::GEntity()
{
	meshCube = new Cube;
}

void GEntity::init(GLuint* vbo, glm::mat4& modelMatrix, int physicsObjectId)
{
	mPhysicsObjectId = physicsObjectId;
	meshCube->init(vbo, modelMatrix);

	bindAxis();
}

void GEntity::bindAxis()
{
	Inputs::get().addBinding("MoveForward", &GEntity::moveForward, this);
	Inputs::get().addBinding("MoveLeft", &GEntity::moveSide, this);
	Inputs::get().addBinding("MoveRight", &GEntity::moveSide, this);
}

void GEntity::moveForward(float val)
{
	glm::vec3 impulse(0.0f, 50000.0f, 0.0f);
	//rigidBody->addMovement(impulse, val);
}

void GEntity::moveSide(float val)
{
	glm::vec3 impulse(5000.0f, 0.0f, 0.0f);
	//rigidBody->addMovement(impulse, val);
}

void GEntity::update(Camera* camera, GLuint renderingProgram, const glm::mat4* modelMatrix)
{
	if (modelMatrix)
	{
		meshCube->setModelMatrix(*modelMatrix);
	}
	
	meshCube->update(camera, renderingProgram);
}