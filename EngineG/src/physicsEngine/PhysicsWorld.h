#pragma once

#include <vector>

typedef void (*dydt_func)(float t, float y[], float ydot[]);

class RigidBody;

class PhysicsWorld
{
public:
	static const int STATE_SIZE;

public:
	void stateToArray(RigidBody* rb, float* y);
	void arrayToState(RigidBody* rb, float* y);
	void arrayToBody(float* y);
	void bodyToArray();
	void dydt(float t, std::vector<float>& ydot);
	void computeForceAndTorque(float t, RigidBody* body);
	void ddtStateToArray(RigidBody* rb, float* ydot);

private:

	int m_numberOfNodes;
	std::vector<float> mY;
	std::vector<float> mYdot;
	std::vector<RigidBody*> mBodies;

	void ode(float y0[], float yEnd[], int len, float t0, float t1, dydt_func dydt);

};