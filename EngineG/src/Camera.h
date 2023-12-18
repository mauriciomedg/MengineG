#pragma once

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

class PhysicsWorld;

class Camera
{

public:

	Camera();

	void init(const glm::mat4& modelMatrix, PhysicsWorld* pWorld, bool simulatePhysics);
	void update(GLFWwindow* window);
	const glm::mat4& getViewMatrix() { return vMat; };
	const glm::mat4& getProjectionMatrix() { return pMat; };

private:
	void bindAxis();
	void moveForward(float val);
	void moveSide(float val);

private:
	float cameraX;
	float cameraY;
	float cameraZ;
	int width;
	int height;
	float aspect;

	glm::mat4 pMat;
	glm::mat4 vMat;

	////////////////////////////////
	int mCollisionPrimitiveId;
	bool mSimulatePhysics;
	glm::mat4 mMat;
	PhysicsWorld* mWorld;
};