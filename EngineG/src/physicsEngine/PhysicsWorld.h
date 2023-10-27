#pragma once

#include <vector>
#include <glm/gtc/matrix_transform.hpp>

typedef void (*dydt_func)(float t, float* ydot);

class RigidBody;

class PhysicsWorld
{
public:
	static const int STATE_SIZE;

public:
	void init();
	void stateToArray(RigidBody* rb, float* y);
	void arrayToState(RigidBody* rb, float* y);
	void arrayToBody(float* y);
	void bodiesToArray();
	void dydt(float deltaT, float* ydot);
	void computeForceAndTorque(float t, RigidBody* body);
	void ddtStateToArray(RigidBody* rb, float* ydot);
	void runSimulation(float deltaT);

	std::vector<RigidBody*> mBodies;
private:

	glm::vec3 mGravity;
	int m_numberOfNodes;
	std::vector<float> mY;
	std::vector<float> mYfinal;
	std::vector<float> mYdot;
	
	void ode(float y0[], float yEnd[], float deltaT);//, dydt_func dydtfunc);

};