#pragma once

#include <vector>
#include <glm/gtc/matrix_transform.hpp>

typedef void (*dydt_func)(float t, float* ydot);

class RigidBody;

class PhysicsWorld
{
public:
	void init();
	void runSimulation(float deltaT);

	std::vector<RigidBody*> mBodies;
private:

	glm::vec3 mGravity;
	int m_numberOfNodes;
	std::vector<float> mY;
	std::vector<float> mYdot;

};