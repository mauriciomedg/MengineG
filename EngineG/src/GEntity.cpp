#include "GEntity.h"
#include "physicsEngine/ParticlesSimulation.h"
#include "Cube.h"
#include "physicsEngine/RigidBody.h"
#include "physicsEngine/PhysicsWorld.h"
#include "Delegate.h"

GPrimitiveEntity::GPrimitiveEntity(PhysicsWorld* pWorld)
	: mWorld(pWorld), mShape(nullptr), mCollisionPrimitiveId(-1), mSimulatePhysics(false)
{
}

void GPrimitiveEntity::update(Camera* camera, GLuint renderingProgram)
{
	if (mWorld)
	{
		mShape->setModelMatrix(mWorld->getPrimitiveLocation(mCollisionPrimitiveId));
	}

	if (mShape)
	{
		mShape->update(camera, renderingProgram);
	}
}

void GPrimitiveEntity::init(GLuint* vbo, glm::mat4& modelMatrix, bool simulatePhysics)
{
	if (mShape)
	{
		mShape->init(vbo, modelMatrix);
	}

	if (mWorld && (mCollisionPrimitiveId == -1))
	{
		// instantiate generique primitive
		//mCollisionPrimitiveId = mWorld->instanciatePrimitiveBox(modelMatrix, halfSize, simulatePhysics);
	}
}

//////////////////////////////////////////////////////////

GEntityBox::GEntityBox(PhysicsWorld* pWorld)
	: GPrimitiveEntity(pWorld)
{
	mShape = new Cube;
}

void GEntityBox::init(GLuint* vbo, glm::mat4& modelMatrix, bool simulatePhysics)
{
	glm::vec3 halfSize(1.0f, 1.0f, 1.0f);
	mCollisionPrimitiveId = mWorld->instanciatePrimitiveBox(modelMatrix, halfSize, simulatePhysics);

	GPrimitiveEntity::init(vbo, modelMatrix, simulatePhysics);
}

void GEntityBox::update(Camera* camera, GLuint renderingProgram)
{
	GPrimitiveEntity::update(camera, renderingProgram);
}

////////////////////////////////////////////////////////////

GEntityBoxControlled::GEntityBoxControlled(PhysicsWorld* pWorld)
	: GEntityBox(pWorld)
{
}

void GEntityBoxControlled::init(GLuint* vbo, glm::mat4& modelMatrix, bool simulatePhysics)
{
	GEntityBox::init(vbo, modelMatrix, simulatePhysics);

	bindAxis();
}

void GEntityBoxControlled::bindAxis()
{
	Inputs::get().addBinding("MoveForward", delegate::from_method<GEntityBoxControlled, &GEntityBoxControlled::moveForward>(this));
	Inputs::get().addBinding("MoveLeft", delegate::from_method<GEntityBoxControlled, &GEntityBoxControlled::moveSide>(this));
	Inputs::get().addBinding("MoveRight", delegate::from_method<GEntityBoxControlled, &GEntityBoxControlled::moveSide>(this));
}

void GEntityBoxControlled::moveForward(float val)
{
	glm::vec3 impulse(0.0f, 5000.0f, 0.0f);
	mWorld->addMovement(mCollisionPrimitiveId, impulse, val);
}

void GEntityBoxControlled::moveSide(float val)
{
	glm::vec3 impulse(100.0f, 0.0f, 0.0f);
	mWorld->addMovement(mCollisionPrimitiveId, impulse, val);
}

void GEntityBoxControlled::update(Camera* camera, GLuint renderingProgram)
{
	GEntityBox::update(camera, renderingProgram);
}
////////////////////////////////////////////////////////////