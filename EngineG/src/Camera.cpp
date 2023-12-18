#include "Camera.h"
#include "physicsEngine/PhysicsWorld.h"
#include "Delegate.h"
#include "Inputs.h"

Camera::Camera()
	: cameraX(0.0f), cameraY(30.0f), cameraZ(60.0f)
{	
}

void Camera::init(const glm::mat4& modelMatrix, PhysicsWorld* pWorld, bool simulatePhysics)
{
	mWorld = pWorld;
	mMat = modelMatrix;
	glm::vec3 halfSize(1.0f, 1.0f, 1.0f);
	mCollisionPrimitiveId = mWorld->instanciatePrimitiveBox(modelMatrix, halfSize, simulatePhysics);
	mWorld->setAffectedByGravity(mCollisionPrimitiveId, false);
	mWorld->setIgnoreCollision(mCollisionPrimitiveId, false);

	bindAxis();
}

void Camera::bindAxis()
{
	Inputs::get().addBinding("MoveForwardCamera", delegate::from_method<Camera, &Camera::moveForward>(this));
	Inputs::get().addBinding("MoveBackwardCamera", delegate::from_method<Camera, &Camera::moveForward>(this));
	Inputs::get().addBinding("MoveLeftCamera", delegate::from_method<Camera, &Camera::moveSide>(this));
	Inputs::get().addBinding("MoveRightCamera", delegate::from_method<Camera, &Camera::moveSide>(this));
}

void Camera::moveForward(float val)
{
	glm::vec3 impulse(0.0f, 0.0f, -100.0f);
	impulse *= val;
	mWorld->move(mCollisionPrimitiveId, impulse);
}

void Camera::moveSide(float val)
{
	glm::vec3 impulse(100.0f, 0.0f, 0.0f);
	impulse *= val;
	mWorld->move(mCollisionPrimitiveId, impulse);
}

void Camera::update(GLFWwindow* window)
{
//https://learnopengl.com/Getting-started/Camera

	mMat = mWorld->getPrimitiveLocation(mCollisionPrimitiveId);
	// build perspective matrix
	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f); // 1.0472 radians = 60 degrees

	// build view matrix
	glm::vec3 cameraPos = mMat[3];
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	vMat = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

}

