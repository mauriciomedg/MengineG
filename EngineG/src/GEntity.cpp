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

int GPrimitiveEntity::getPhysicsID() const
{
	return mCollisionPrimitiveId;
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

void GPrimitiveEntity::setAffectedByGravity(bool isAffectedByGravity)
{
	if (mWorld && (mCollisionPrimitiveId != -1))
	{
		mWorld->setAffectedByGravity(mCollisionPrimitiveId, isAffectedByGravity);
	}
}

void GPrimitiveEntity::init(GLuint* vbo, const glm::mat4& modelMatrix, float halfSize, bool simulatePhysics)
{
	if (mShape)
	{
		mShape->init(vbo, modelMatrix, halfSize);
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

void GEntityBox::init(GLuint* vbo, const glm::mat4& modelMatrix, float halfSize, float mass, bool simulatePhysics)
{
	glm::vec3 halfSizeVector(halfSize, halfSize, halfSize);
	//static_cast<Cube*>(mShape)->setHalfSize(mhalfSize);
	mCollisionPrimitiveId = mWorld->instanciatePrimitiveBox(modelMatrix, halfSizeVector, mass, simulatePhysics);

	GPrimitiveEntity::init(vbo, modelMatrix, halfSize, simulatePhysics);
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

void GEntityBoxControlled::init(GLuint* vbo, const glm::mat4& modelMatrix, float halfSize, float mass, bool simulatePhysics)
{
	GEntityBox::init(vbo, modelMatrix, halfSize, mass, simulatePhysics);

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
	glm::vec3 impulse(1000.0f, 0.0f, 0.0f);
	mWorld->addMovement(mCollisionPrimitiveId, impulse, val);
}

void GEntityBoxControlled::update(Camera* camera, GLuint renderingProgram)
{
	GEntityBox::update(camera, renderingProgram);
}
////////////////////////////////////////////////////////////