#include "GEntity.h"
#include "physicsEngine/ParticlesSimulation.h"
#include "Cube.h"
#include "physicsEngine/RigidBody.h"

GEntity::GEntity()
{
	meshCube = new Cube;
	rigidBody = new RigidBody;
}

void GEntity::init(GLuint* vbo)
{
	//phyCube->x = meshCube->mPos;
	//phyCube->v = glm::vec3(0.0f, 50.0f, 0.0f);
	rigidBody->init(meshCube->mPos, glm::vec3(0.0f, 100.0f, 0.0f), glm::mat3(1.0f));
	//
	//float a = pSystem->gravity[1] / 2.0f;
	//float b = phyCube->v[1];
	//float c = phyCube->x[1];
	//
	//float t = (-b - glm::sqrt((b * b) - 4 * a * c)) / (2 * a);
	//std::cout << " Time in the air " << t << std::endl;
	//
	meshCube->init(vbo);
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
	//phyCube->addMovement(impulse, val);
	
	//std::cout << "moveeeeeeeee " << val << std::endl;
}

void GEntity::moveSide(float val)
{
	glm::vec3 impulse(2000.0f, 0.0f, 0.0f);
	//phyCube->addMovement(impulse, val);
}

void GEntity::update(float currentTime, Camera* camera, GLuint renderingProgram)
{
	meshCube->mPos = rigidBody->mX;
	meshCube->update(currentTime, camera, renderingProgram);
}